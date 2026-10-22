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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
स्थिर bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
स्थिर bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
स्थिर वर्ण *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
स्थिर size_t *delim_lens;

/* Number of delimiters.  */
स्थिर idx_t अङ्कः_delims;

स्थिर unsigned वर्ण पङ्क्तिः_delim = '\n';

स्थिर संरचना option ध्रुव longopts[] =
{
  {"serial", no_argument, रिक्त, 's'},
  {"delimiters", required_argument, रिक्त, 'd'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

स्थिर पूर्णाङ्क
collapse_escapes (वर्ण ध्रुव *strptr)
{
  वर्ण *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), परिमाण *delim_lens);

  वर्ण ध्रुव *s = strptr;
  idx_t idx = 0;

  जाव (*s)
    {
      जइ (*s == '\\')
        {
          s++;
          जइ (*s == '\0')
            {
              backslash_at_end = true;
              भङ्ग;
            }
          अण्णहा जइ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          अण्णहा
            {
              विकल्प (*s)
                {
                पक्ष 'b': *strout++ = '\b'; भङ्ग;
                पक्ष 'f': *strout++ = '\f'; भङ्ग;
                पक्ष 'n': *strout++ = '\n'; भङ्ग;
                पक्ष 'r': *strout++ = '\r'; भङ्ग;
                पक्ष 't': *strout++ = '\t'; भङ्ग;
                पक्ष 'v': *strout++ = '\v'; भङ्ग;
                पक्ष '\\': *strout++ = '\\'; भङ्ग;
                स्वभाव: गच्छ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          अनुवर्त;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  जइ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  अङ्कः_delims = idx;

  प्रत्यागम backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

स्थिर अन्तःस्थ शून्य
xputchar (वर्ण c)
{
  जइ (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

स्थिर अन्तःस्थ शून्य
output_delim (वर्ण ध्रुव *delimptr, size_t len)
{
  जइ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

स्थिर bool
paste_parallel (size_t nfiles, वर्ण **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  वर्ण *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, परिमाण *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  प्रति (files_open = 0; files_open < nfiles; ++files_open)
    {
      जइ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      अण्णहा
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          जइ (fileptr[files_open] == रिक्त)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          अण्णहा जइ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  जइ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  जाव (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      प्रति (size_t i = 0; i < nfiles && files_open; i++)
        {
          पूर्णाङ्क chr;			/* Input character. */
          पूर्णाङ्क err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          जइ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              जइ (chr != EOF && delims_saved)
                {
                  जइ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              जाव (chr != EOF)
                {
                  sometodo = true;
                  जइ (chr == पङ्क्तिः_delim)
                    भङ्ग;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          जइ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              जइ (fileptr[i])
                {
                  जइ (!ferror (fileptr[i]))
                    err = 0;
                  जइ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  अण्णहा जइ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  जइ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = रिक्त;
                  files_open--;
                }

              जइ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  जइ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      जइ (delims_saved)
                        {
                          जइ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (पङ्क्तिः_delim);
                    }
                  अनुवर्त;	/* Next read of files, or exit. */
                }
              अण्णहा
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  जइ (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  जइ (++delimidx == अङ्कः_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          अण्णहा
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              जइ (i + 1 != nfiles)
                {
                  जइ (chr != पङ्क्तिः_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  जइ (++delimidx == अङ्कः_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              अण्णहा
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  वर्ण c = (chr == EOF ? पङ्क्तिः_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  मुक्त (fileptr);
  मुक्त (delbuf);
  प्रत्यागम ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

स्थिर bool
paste_serial (size_t nfiles, वर्ण **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  पूर्णाङ्क charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  प्रति (; nfiles; nfiles--, fnamptr++)
    {
      पूर्णाङ्क saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      जइ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      अण्णहा
        {
          fileptr = fopen (*fnamptr, "r");
          जइ (fileptr == रिक्त)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              अनुवर्त;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      जइ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          जाव ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              जइ (charold == पङ्क्तिः_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  जइ (++delimidx == अङ्कः_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              अण्णहा
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      जइ (charold != पङ्क्तिः_delim)
        xputchar (पङ्क्तिः_delim);

      जइ (!ferror (fileptr))
        saved_errno = 0;
      जइ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      अण्णहा जइ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      जइ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  प्रत्यागम ok;
}

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write पङ्क्तयः consisting of corresponding पङ्क्तयः from each file joined by TABs\n\
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
         every पङ्क्तिः except the last पङ्क्तिः in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  वर्ण ध्रुव *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  जाव ((optc = getopt_long (argc, argv, "d:sz", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          भङ्ग;

        पक्ष 's':
          serial_merge = true;
          भङ्ग;

        पक्ष 'z':
          पङ्क्तिः_delim = '\0';
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  पूर्णाङ्क nfiles = argc - optind;
  जइ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  जइ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  मुक्त (delims);
  मुक्त (delim_lens);

  जइ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
