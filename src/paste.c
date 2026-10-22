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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ಸ್ಥಿರ bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ಸ್ಥಿರ bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ಸ್ಥಿರ ವರ್ಣ *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ಸ್ಥಿರ size_t *delim_lens;

/* Number of delimiters.  */
ಸ್ಥಿರ idx_t ಅಂಕೆ_delims;

ಸ್ಥಿರ unsigned ವರ್ಣ ಸಾಲು_delim = '\n';

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"serial", no_argument, ರಿಕ್ತ, 's'},
  {"delimiters", required_argument, ರಿಕ್ತ, 'd'},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
collapse_escapes (ವರ್ಣ ಧ್ರುವ *strptr)
{
  ವರ್ಣ *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), ಪರಿಮಾಣ *delim_lens);

  ವರ್ಣ ಧ್ರುವ *s = strptr;
  idx_t idx = 0;

  ಯಾವತ್ (*s)
    {
      ಯದಿ (*s == '\\')
        {
          s++;
          ಯದಿ (*s == '\0')
            {
              backslash_at_end = true;
              ಭಙ್ಗ;
            }
          ಅನ್ಯಥಾ ಯದಿ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          ಅನ್ಯಥಾ
            {
              ವಿಕಲ್ಪ (*s)
                {
                ಪಕ್ಷ 'b': *strout++ = '\b'; ಭಙ್ಗ;
                ಪಕ್ಷ 'f': *strout++ = '\f'; ಭಙ್ಗ;
                ಪಕ್ಷ 'n': *strout++ = '\n'; ಭಙ್ಗ;
                ಪಕ್ಷ 'r': *strout++ = '\r'; ಭಙ್ಗ;
                ಪಕ್ಷ 't': *strout++ = '\t'; ಭಙ್ಗ;
                ಪಕ್ಷ 'v': *strout++ = '\v'; ಭಙ್ಗ;
                ಪಕ್ಷ '\\': *strout++ = '\\'; ಭಙ್ಗ;
                ಸ್ವಭಾವ: ಗಚ್ಛ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          ಅನುವರ್ತ;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  ಯದಿ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  ಅಂಕೆ_delims = idx;

  ಪ್ರತ್ಯಾಗಮ backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
xputchar (ವರ್ಣ c)
{
  ಯದಿ (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
output_delim (ವರ್ಣ ಧ್ರುವ *delimptr, size_t len)
{
  ಯದಿ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ಸ್ಥಿರ bool
paste_parallel (size_t nfiles, ವರ್ಣ **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  ವರ್ಣ *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, ಪರಿಮಾಣ *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  ಪ್ರತಿ (files_open = 0; files_open < nfiles; ++files_open)
    {
      ಯದಿ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      ಅನ್ಯಥಾ
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          ಯದಿ (fileptr[files_open] == ರಿಕ್ತ)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          ಅನ್ಯಥಾ ಯದಿ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  ಯದಿ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  ಯಾವತ್ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      ಪ್ರತಿ (size_t i = 0; i < nfiles && files_open; i++)
        {
          ಪೂರ್ಣಾಙ್ಕ chr;			/* Input character. */
          ಪೂರ್ಣಾಙ್ಕ err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          ಯದಿ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              ಯದಿ (chr != EOF && delims_saved)
                {
                  ಯದಿ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              ಯಾವತ್ (chr != EOF)
                {
                  sometodo = true;
                  ಯದಿ (chr == ಸಾಲು_delim)
                    ಭಙ್ಗ;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          ಯದಿ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              ಯದಿ (fileptr[i])
                {
                  ಯದಿ (!ferror (fileptr[i]))
                    err = 0;
                  ಯದಿ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  ಅನ್ಯಥಾ ಯದಿ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  ಯದಿ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ರಿಕ್ತ;
                  files_open--;
                }

              ಯದಿ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  ಯದಿ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      ಯದಿ (delims_saved)
                        {
                          ಯದಿ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (ಸಾಲು_delim);
                    }
                  ಅನುವರ್ತ;	/* Next read of files, or exit. */
                }
              ಅನ್ಯಥಾ
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  ಯದಿ (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  ಯದಿ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          ಅನ್ಯಥಾ
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              ಯದಿ (i + 1 != nfiles)
                {
                  ಯದಿ (chr != ಸಾಲು_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ಯದಿ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ಅನ್ಯಥಾ
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  ವರ್ಣ c = (chr == EOF ? ಸಾಲು_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  ಮುಕ್ತ (fileptr);
  ಮುಕ್ತ (delbuf);
  ಪ್ರತ್ಯಾಗಮ ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ಸ್ಥಿರ bool
paste_serial (size_t nfiles, ವರ್ಣ **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ಪೂರ್ಣಾಙ್ಕ charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  ಪ್ರತಿ (; nfiles; nfiles--, fnamptr++)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      ಯದಿ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      ಅನ್ಯಥಾ
        {
          fileptr = fopen (*fnamptr, "r");
          ಯದಿ (fileptr == ರಿಕ್ತ)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              ಅನುವರ್ತ;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      ಯದಿ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          ಯಾವತ್ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              ಯದಿ (charold == ಸಾಲು_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ಯದಿ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ಅನ್ಯಥಾ
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      ಯದಿ (charold != ಸಾಲು_delim)
        xputchar (ಸಾಲು_delim);

      ಯದಿ (!ferror (fileptr))
        saved_errno = 0;
      ಯದಿ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      ಅನ್ಯಥಾ ಯದಿ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      ಯದಿ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  ಪ್ರತ್ಯಾಗಮ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write ಸಾಲುಗಳು consisting of corresponding ಸಾಲುಗಳು from each file joined by TABs\n\
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
         every ಸಾಲು except the last ಸಾಲು in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  ವರ್ಣ ಧ್ರುವ *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "d:sz", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          serial_merge = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          ಸಾಲು_delim = '\0';
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಙ್ಕ nfiles = argc - optind;
  ಯದಿ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  ಯದಿ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  ಮುಕ್ತ (delims);
  ಮುಕ್ತ (delim_lens);

  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
