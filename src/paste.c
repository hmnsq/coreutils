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
static var_na *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
static size_t *delim_lens;

/* Number of delimiters.  */
static idx_t num_delims;

static unsigned var_na line_delim = '\n';

static sa_nracanaa option const longopts[] =
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

static ཧྲིལ
collapse_escapes (var_na const *strptr)
{
  var_na *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), parimaa_na *delim_lens);

  var_na const *s = strptr;
  idx_t idx = 0;

  སྐབས (*s)
    {
      གལ་སྲིད (*s == '\\')
        {
          s++;
          གལ་སྲིད (*s == '\0')
            {
              backslash_at_end = true;
              bha_nga;
            }
          དེ་མིན གལ་སྲིད (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          དེ་མིན
            {
              switch (*s)
                {
                pak_sa 'b': *strout++ = '\b'; bha_nga;
                pak_sa 'f': *strout++ = '\f'; bha_nga;
                pak_sa 'n': *strout++ = '\n'; bha_nga;
                pak_sa 'r': *strout++ = '\r'; bha_nga;
                pak_sa 't': *strout++ = '\t'; bha_nga;
                pak_sa 'v': *strout++ = '\v'; bha_nga;
                pak_sa '\\': *strout++ = '\\'; bha_nga;
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

  གལ་སྲིད (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  num_delims = idx;

  ལོག backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

static anta_hstha void
xputchar (var_na c)
{
  གལ་སྲིད (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

static anta_hstha void
output_delim (var_na const *delimptr, size_t len)
{
  གལ་སྲིད (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

static bool
paste_parallel (size_t nfiles, var_na **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  var_na *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, parimaa_na *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  དོན་ལུ (files_open = 0; files_open < nfiles; ++files_open)
    {
      གལ་སྲིད (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      དེ་མིན
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          གལ་སྲིད (fileptr[files_open] == NULL)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          དེ་མིན གལ་སྲིད (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  གལ་སྲིད (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  སྐབས (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      དོན་ལུ (size_t i = 0; i < nfiles && files_open; i++)
        {
          ཧྲིལ chr;			/* Input character. */
          ཧྲིལ err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          གལ་སྲིད (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              གལ་སྲིད (chr != EOF && delims_saved)
                {
                  གལ་སྲིད (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              སྐབས (chr != EOF)
                {
                  sometodo = true;
                  གལ་སྲིད (chr == line_delim)
                    bha_nga;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          གལ་སྲིད (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              གལ་སྲིད (fileptr[i])
                {
                  གལ་སྲིད (!ferror (fileptr[i]))
                    err = 0;
                  གལ་སྲིད (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  དེ་མིན གལ་སྲིད (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  གལ་སྲིད (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = NULL;
                  files_open--;
                }

              གལ་སྲིད (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  གལ་སྲིད (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      གལ་སྲིད (delims_saved)
                        {
                          གལ་སྲིད (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (line_delim);
                    }
                  continue;	/* Next read of files, or exit. */
                }
              དེ་མིན
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  གལ་སྲིད (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  གལ་སྲིད (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          དེ་མིན
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              གལ་སྲིད (i + 1 != nfiles)
                {
                  གལ་སྲིད (chr != line_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  གལ་སྲིད (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              དེ་མིན
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  var_na c = (chr == EOF ? line_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  free (fileptr);
  free (delbuf);
  ལོག ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

static bool
paste_serial (size_t nfiles, var_na **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ཧྲིལ charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  དོན་ལུ (; nfiles; nfiles--, fnamptr++)
    {
      ཧྲིལ saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      གལ་སྲིད (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      དེ་མིན
        {
          fileptr = fopen (*fnamptr, "r");
          གལ་སྲིད (fileptr == NULL)
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
      གལ་སྲིད (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          སྐབས ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              གལ་སྲིད (charold == line_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  གལ་སྲིད (++delimidx == num_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              དེ་མིན
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      གལ་སྲིད (charold != line_delim)
        xputchar (line_delim);

      གལ་སྲིད (!ferror (fileptr))
        saved_errno = 0;
      གལ་སྲིད (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      དེ་མིན གལ་སྲིད (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      གལ་སྲིད (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  ལོག ok;
}

void
usage (ཧྲིལ status)
{
  གལ་སྲིད (status != EXIT_SUCCESS)
    emit_try_help ();
  དེ་མིན
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

ཧྲིལ
main (ཧྲིལ argc, var_na **argv)
{
  ཧྲིལ optc;
  var_na const *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  སྐབས ((optc = getopt_long (argc, argv, "d:sz", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          bha_nga;

        pak_sa 's':
          serial_merge = true;
          bha_nga;

        pak_sa 'z':
          line_delim = '\0';
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ཧྲིལ nfiles = argc - optind;
  གལ་སྲིད (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  གལ་སྲིད (collapse_escapes (delim_arg))
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

  གལ་སྲིད (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  ལོག ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
