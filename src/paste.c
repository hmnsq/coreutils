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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ساكن bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ساكن bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ساكن حرف *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ساكن size_t *delim_lens;

/* Number of delimiters.  */
ساكن idx_t عدد_delims;

ساكن unsigned حرف سطر_delim = '\n';

ساكن بنية option ثابت longopts[] =
{
  {"serial", no_argument, عدم, 's'},
  {"delimiters", required_argument, عدم, 'd'},
  {"zero-terminated", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ساكن صحيح
collapse_escapes (حرف ثابت *strptr)
{
  حرف *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), حجم *delim_lens);

  حرف ثابت *s = strptr;
  idx_t idx = 0;

  طالما (*s)
    {
      إذا (*s == '\\')
        {
          s++;
          إذا (*s == '\0')
            {
              backslash_at_end = true;
              اكسر;
            }
          وإلا إذا (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          وإلا
            {
              بدل (*s)
                {
                حالة 'b': *strout++ = '\b'; اكسر;
                حالة 'f': *strout++ = '\f'; اكسر;
                حالة 'n': *strout++ = '\n'; اكسر;
                حالة 'r': *strout++ = '\r'; اكسر;
                حالة 't': *strout++ = '\t'; اكسر;
                حالة 'v': *strout++ = '\v'; اكسر;
                حالة '\\': *strout++ = '\\'; اكسر;
                افتراضي: اذهب copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          استمر;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  إذا (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  عدد_delims = idx;

  أرجع backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ساكن مضمن فراغ
xputchar (حرف c)
{
  إذا (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ساكن مضمن فراغ
output_delim (حرف ثابت *delimptr, size_t len)
{
  إذا (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ساكن bool
paste_parallel (size_t nfiles, حرف **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  حرف *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, حجم *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  لكل (files_open = 0; files_open < nfiles; ++files_open)
    {
      إذا (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      وإلا
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          إذا (fileptr[files_open] == عدم)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          وإلا إذا (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  إذا (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  طالما (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      لكل (size_t i = 0; i < nfiles && files_open; i++)
        {
          صحيح chr;			/* Input character. */
          صحيح err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          إذا (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              إذا (chr != EOF && delims_saved)
                {
                  إذا (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              طالما (chr != EOF)
                {
                  sometodo = true;
                  إذا (chr == سطر_delim)
                    اكسر;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          إذا (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              إذا (fileptr[i])
                {
                  إذا (!ferror (fileptr[i]))
                    err = 0;
                  إذا (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  وإلا إذا (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  إذا (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = عدم;
                  files_open--;
                }

              إذا (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  إذا (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      إذا (delims_saved)
                        {
                          إذا (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (سطر_delim);
                    }
                  استمر;	/* Next read of files, or exit. */
                }
              وإلا
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  إذا (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  إذا (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          وإلا
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              إذا (i + 1 != nfiles)
                {
                  إذا (chr != سطر_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  إذا (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              وإلا
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  حرف c = (chr == EOF ? سطر_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  حرر (fileptr);
  حرر (delbuf);
  أرجع ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ساكن bool
paste_serial (size_t nfiles, حرف **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  صحيح charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  لكل (; nfiles; nfiles--, fnamptr++)
    {
      صحيح saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      إذا (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      وإلا
        {
          fileptr = fopen (*fnamptr, "r");
          إذا (fileptr == عدم)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              استمر;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      إذا (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          طالما ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              إذا (charold == سطر_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  إذا (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              وإلا
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      إذا (charold != سطر_delim)
        xputchar (سطر_delim);

      إذا (!ferror (fileptr))
        saved_errno = 0;
      إذا (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      وإلا إذا (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      إذا (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  أرجع ok;
}

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write سطریں consisting of corresponding سطریں from each file joined by TABs\n\
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
         every سطر except the last سطر in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc;
  حرف ثابت *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "d:sz", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          اكسر;

        حالة 's':
          serial_merge = true;
          اكسر;

        حالة 'z':
          سطر_delim = '\0';
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  صحيح nfiles = argc - optind;
  إذا (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  إذا (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  حرر (delims);
  حرر (delim_lens);

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
