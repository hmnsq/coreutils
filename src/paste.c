/* paste - merge lines of files
   Copyright (C) 1997-2026 Free Software Foundation, Inc.
   Copyright (C) 1984 David M. Ihnat

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by David Ihnat.  */

/* The list of valid escape sequences has been expanded over the Unix
   version, to include \b, \f, \r, and \v.

   POSIX changes, bug fixes, long-named options, and cleanup
   by David MacKenzie <djm@gnu.ai.mit.edu>.

   Options:
   --serial
   -s				Paste one file at a time rather than
                                one line from each file.
   --delimiters=delim-list
   -d delim-list		Consecutively use the characters in
                                DELIM-LIST instead of tab to separate
                                merged lines.  When DELIM-LIST is exhausted,
                                start again at its beginning.
   A FILE of '-' means standard input.
   If no FILEs are given, standard input is used. */

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include "system.h"
#include "fadvise.h"
#include "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
static bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
static bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
static char *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
static size_t *delim_lens;

/* Number of delimiters.  */
static idx_t num_delims;

static unsigned char line_delim = '\n';

static struct option const longopts[] =
{
  {"serial", no_argument, NULL, 's'},
  {"delimiters", required_argument, NULL, 'd'},
  {"zero-terminated", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

static ціле
collapse_escapes (char const *strptr)
{
  char *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), sizeof *delim_lens);

  char const *s = strptr;
  idx_t idx = 0;

  поки (*s)
    {
      якщо (*s == '\\')
        {
          s++;
          якщо (*s == '\0')
            {
              backslash_at_end = true;
              break;
            }
          інакше якщо (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          інакше
            {
              switch (*s)
                {
                case 'b': *strout++ = '\b'; break;
                case 'f': *strout++ = '\f'; break;
                case 'n': *strout++ = '\n'; break;
                case 'r': *strout++ = '\r'; break;
                case 't': *strout++ = '\t'; break;
                case 'v': *strout++ = '\v'; break;
                case '\\': *strout++ = '\\'; break;
                default: goto copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          continue;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  якщо (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  num_delims = idx;

  повернути backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

static inline void
xputchar (char c)
{
  якщо (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

static inline void
output_delim (char const *delimptr, size_t len)
{
  якщо (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

static bool
paste_parallel (size_t nfiles, char **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  char *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, sizeof *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  для (files_open = 0; files_open < nfiles; ++files_open)
    {
      якщо (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      інакше
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          якщо (fileptr[files_open] == NULL)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          інакше якщо (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  якщо (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  поки (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      для (size_t i = 0; i < nfiles && files_open; i++)
        {
          ціле chr;			/* Input character. */
          ціле err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          якщо (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              якщо (chr != EOF && delims_saved)
                {
                  якщо (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              поки (chr != EOF)
                {
                  sometodo = true;
                  якщо (chr == line_delim)
                    break;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          якщо (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              якщо (fileptr[i])
                {
                  якщо (!ferror (fileptr[i]))
                    err = 0;
                  якщо (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  інакше якщо (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  якщо (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = NULL;
                  files_open--;
                }

              якщо (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  якщо (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      якщо (delims_saved)
                        {
                          якщо (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (line_delim);
                    }
                  continue;	/* Next read of files, or exit. */
                }
              інакше
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  якщо (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  якщо (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          інакше
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              якщо (i + 1 != nfiles)
                {
                  якщо (chr != line_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  якщо (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              інакше
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  char c = (chr == EOF ? line_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  free (fileptr);
  free (delbuf);
  повернути ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

static bool
paste_serial (size_t nfiles, char **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ціле charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  для (; nfiles; nfiles--, fnamptr++)
    {
      ціле saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      якщо (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      інакше
        {
          fileptr = fopen (*fnamptr, "r");
          якщо (fileptr == NULL)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              continue;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      якщо (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          поки ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              якщо (charold == line_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  якщо (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              інакше
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      якщо (charold != line_delim)
        xputchar (line_delim);

      якщо (!ferror (fileptr))
        saved_errno = 0;
      якщо (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      інакше якщо (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      якщо (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  повернути ok;
}

void
usage (ціле status)
{
  якщо (status != EXIT_SUCCESS)
    emit_try_help ();
  інакше
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write lines consisting of corresponding lines from each file joined by TABs\n\
to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead of TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a time instead of in parallel; the newline of\n\
         every line except the last line in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ціле
main (ціле argc, char **argv)
{
  ціле optc;
  char const *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  поки ((optc = getopt_long (argc, argv, "d:sz", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        case 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          break;

        case 's':
          serial_merge = true;
          break;

        case 'z':
          line_delim = '\0';
          break;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ціле nfiles = argc - optind;
  якщо (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  якщо (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  free (delims);
  free (delim_lens);

  якщо (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  повернути ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
