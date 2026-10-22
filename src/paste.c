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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "paste"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ಸ್ಥಿರ bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ಸ್ಥಿರ bool serial_ವಿಲೀನ;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ಸ್ಥಿರ ಅಕ್ಷರ *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ಸ್ಥಿರ size_t *delim_lens;

/* Number of delimiters.  */
ಸ್ಥಿರ idx_t ಅಂಕೆ_delims;

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಸಾಲು_delim = '\n';

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"serial", no_ವಾದ, ಖಾಲಿ, 's'},
  {"delimiters", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
collapse_escapes (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *strptr)
{
  ಅಕ್ಷರ *strout = xstrdup (strptr);
  bool backslash_at_ಕೊನೆ = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (strptr)), ಗಾತ್ರ *delim_lens);

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = strptr;
  idx_t idx = 0;

  ಆಗ (*s)
    {
      ಆದರೆ (*s == '\\')
        {
          s++;
          ಆದರೆ (*s == '\0')
            {
              backslash_at_ಕೊನೆ = true;
              ನಿಲ್ಲಿಸು;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆಯ್ಕೆ (*s)
                {
                ಸ್ಥಿತಿ 'b': *strout++ = '\b'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'f': *strout++ = '\f'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'n': *strout++ = '\n'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'r': *strout++ = '\r'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 't': *strout++ = '\t'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ 'v': *strout++ = '\v'; ನಿಲ್ಲಿಸು;
                ಸ್ಥಿತಿ '\\': *strout++ = '\\'; ನಿಲ್ಲಿಸು;
                ಪೂರ್ವನಿಯೋಜಿತ: ಹೋಗು copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          ಮುಂದುವರಿ;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  ಆದರೆ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  ಅಂಕೆ_delims = idx;

  ಹಿಂತಿರುಗಿ backslash_at_ಕೊನೆ ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
xputchar (ಅಕ್ಷರ c)
{
  ಆದರೆ (ಅಕ್ಷರ_ಬರೆ (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
output_delim (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *delimptr, size_t len)
{
  ಆದರೆ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ಸ್ಥಿರ bool
paste_parallel (size_t nfiles, ಅಕ್ಷರ **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  ಅಕ್ಷರ *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, ಗಾತ್ರ *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  ಗಾಗಿ (files_open = 0; files_open < nfiles; ++files_open)
    {
      ಆದರೆ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          ಆದರೆ (fileptr[files_open] == ಖಾಲಿ)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  ಆದರೆ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  ಆಗ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      ಗಾಗಿ (size_t i = 0; i < nfiles && files_open; i++)
        {
          ಪೂರ್ಣಾಂಕ chr;			/* Input character. */
          ಪೂರ್ಣಾಂಕ err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          ಆದರೆ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              ಆದರೆ (chr != EOF && delims_saved)
                {
                  ಆದರೆ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              ಆಗ (chr != EOF)
                {
                  sometodo = true;
                  ಆದರೆ (chr == ಸಾಲು_delim)
                    ನಿಲ್ಲಿಸು;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          ಆದರೆ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              ಆದರೆ (fileptr[i])
                {
                  ಆದರೆ (!ferror (fileptr[i]))
                    err = 0;
                  ಆದರೆ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  ಆದರೆ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ಖಾಲಿ;
                  files_open--;
                }

              ಆದರೆ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  ಆದರೆ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      ಆದರೆ (delims_saved)
                        {
                          ಆದರೆ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (ಸಾಲು_delim);
                    }
                  ಮುಂದುವರಿ;	/* Next read of files, or exit. */
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  ಆದರೆ (len > 0)
                    {
                      ಸ್ಮರಣೆ_ನಕಲು (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  ಆದರೆ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              ಆದರೆ (i + 1 != nfiles)
                {
                  ಆದರೆ (chr != ಸಾಲು_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ಆದರೆ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  ಅಕ್ಷರ c = (chr == EOF ? ಸಾಲು_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  ಬಿಡುಗಡೆ (fileptr);
  ಬಿಡುಗಡೆ (delbuf);
  ಹಿಂತಿರುಗಿ ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ಸ್ಥಿರ bool
paste_serial (size_t nfiles, ಅಕ್ಷರ **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ಪೂರ್ಣಾಂಕ charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  ಗಾಗಿ (; nfiles; nfiles--, fnamptr++)
    {
      ಪೂರ್ಣಾಂಕ saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      ಆದರೆ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          fileptr = fopen (*fnamptr, "r");
          ಆದರೆ (fileptr == ಖಾಲಿ)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              ಮುಂದುವರಿ;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      ಆದರೆ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          ಆಗ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              ಆದರೆ (charold == ಸಾಲು_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ಆದರೆ (++delimidx == ಅಂಕೆ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      ಆದರೆ (charold != ಸಾಲು_delim)
        xputchar (ಸಾಲು_delim);

      ಆದರೆ (!ferror (fileptr))
        saved_errno = 0;
      ಆದರೆ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      ಆದರೆ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Write ಸಾಲುಗಳು consisting ದ corresponding ಸಾಲುಗಳು from each file joined by TABs\n\
ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead ದ TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a ಸಮಯ instead ದ ಒಳ parallel; the newline ದ\n\
         every ಸಾಲು except the ಕೊನೆಯ ಸಾಲು ಒಳ each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *delim_arg = "\t";

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "d:sz", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          serial_ವಿಲೀನ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          ಸಾಲು_delim = '\0';
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಪೂರ್ಣಾಂಕ nfiles = argc - optind;
  ಆದರೆ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  ಆದರೆ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_ವಿಲೀನ ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  ಬಿಡುಗಡೆ (delims);
  ಬಿಡುಗಡೆ (delim_lens);

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
