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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
സ്ഥിതം bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
സ്ഥിതം bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
സ്ഥിതം അക്ഷരം *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
സ്ഥിതം size_t *delim_lens;

/* Number of delimiters.  */
സ്ഥിതം idx_t അക്കം_delims;

സ്ഥിതം unsigned അക്ഷരം വരി_delim = '\n';

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"serial", no_argument, ശൂന്യ, 's'},
  {"delimiters", required_argument, ശൂന്യ, 'd'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
collapse_escapes (അക്ഷരം സ്ഥിരം *strptr)
{
  അക്ഷരം *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), വലുപ്പം *delim_lens);

  അക്ഷരം സ്ഥിരം *s = strptr;
  idx_t idx = 0;

  ആകുമ്പോൾ (*s)
    {
      എങ്കിൽ (*s == '\\')
        {
          s++;
          എങ്കിൽ (*s == '\0')
            {
              backslash_at_end = true;
              നിർത്തുക;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          അല്ലെങ്കിൽ
            {
              തിരഞ്ഞെടുപ്പ് (*s)
                {
                അവസ്ഥ 'b': *strout++ = '\b'; നിർത്തുക;
                അവസ്ഥ 'f': *strout++ = '\f'; നിർത്തുക;
                അവസ്ഥ 'n': *strout++ = '\n'; നിർത്തുക;
                അവസ്ഥ 'r': *strout++ = '\r'; നിർത്തുക;
                അവസ്ഥ 't': *strout++ = '\t'; നിർത്തുക;
                അവസ്ഥ 'v': *strout++ = '\v'; നിർത്തുക;
                അവസ്ഥ '\\': *strout++ = '\\'; നിർത്തുക;
                സ്ഥിരസ്ഥിതി: പോകുക copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          തുടരുക;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  എങ്കിൽ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  അക്കം_delims = idx;

  തിരികെ backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

സ്ഥിതം വരിയിൽ ശൂന്യം
xputchar (അക്ഷരം c)
{
  എങ്കിൽ (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

സ്ഥിതം വരിയിൽ ശൂന്യം
output_delim (അക്ഷരം സ്ഥിരം *delimptr, size_t len)
{
  എങ്കിൽ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

സ്ഥിതം bool
paste_parallel (size_t nfiles, അക്ഷരം **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  അക്ഷരം *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, വലുപ്പം *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  വേണ്ടി (files_open = 0; files_open < nfiles; ++files_open)
    {
      എങ്കിൽ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      അല്ലെങ്കിൽ
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          എങ്കിൽ (fileptr[files_open] == ശൂന്യ)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          അല്ലെങ്കിൽ എങ്കിൽ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  എങ്കിൽ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  ആകുമ്പോൾ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      വേണ്ടി (size_t i = 0; i < nfiles && files_open; i++)
        {
          പൂർണ്ണസംഖ്യ chr;			/* Input character. */
          പൂർണ്ണസംഖ്യ err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          എങ്കിൽ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              എങ്കിൽ (chr != EOF && delims_saved)
                {
                  എങ്കിൽ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              ആകുമ്പോൾ (chr != EOF)
                {
                  sometodo = true;
                  എങ്കിൽ (chr == വരി_delim)
                    നിർത്തുക;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          എങ്കിൽ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              എങ്കിൽ (fileptr[i])
                {
                  എങ്കിൽ (!ferror (fileptr[i]))
                    err = 0;
                  എങ്കിൽ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  അല്ലെങ്കിൽ എങ്കിൽ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  എങ്കിൽ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ശൂന്യ;
                  files_open--;
                }

              എങ്കിൽ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  എങ്കിൽ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      എങ്കിൽ (delims_saved)
                        {
                          എങ്കിൽ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (വരി_delim);
                    }
                  തുടരുക;	/* Next read of files, or exit. */
                }
              അല്ലെങ്കിൽ
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  എങ്കിൽ (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  എങ്കിൽ (++delimidx == അക്കം_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          അല്ലെങ്കിൽ
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              എങ്കിൽ (i + 1 != nfiles)
                {
                  എങ്കിൽ (chr != വരി_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  എങ്കിൽ (++delimidx == അക്കം_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              അല്ലെങ്കിൽ
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  അക്ഷരം c = (chr == EOF ? വരി_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  സ്വതന്ത്രം (fileptr);
  സ്വതന്ത്രം (delbuf);
  തിരികെ ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

സ്ഥിതം bool
paste_serial (size_t nfiles, അക്ഷരം **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  പൂർണ്ണസംഖ്യ charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  വേണ്ടി (; nfiles; nfiles--, fnamptr++)
    {
      പൂർണ്ണസംഖ്യ saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      എങ്കിൽ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      അല്ലെങ്കിൽ
        {
          fileptr = fopen (*fnamptr, "r");
          എങ്കിൽ (fileptr == ശൂന്യ)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              തുടരുക;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      എങ്കിൽ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          ആകുമ്പോൾ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              എങ്കിൽ (charold == വരി_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  എങ്കിൽ (++delimidx == അക്കം_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              അല്ലെങ്കിൽ
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      എങ്കിൽ (charold != വരി_delim)
        xputchar (വരി_delim);

      എങ്കിൽ (!ferror (fileptr))
        saved_errno = 0;
      എങ്കിൽ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      അല്ലെങ്കിൽ എങ്കിൽ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      എങ്കിൽ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  തിരികെ ok;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write വരികൾ consisting of corresponding വരികൾ from each file joined by TABs\n\
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
         every വരി except the last വരി in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;
  അക്ഷരം സ്ഥിരം *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "d:sz", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          നിർത്തുക;

        അവസ്ഥ 's':
          serial_merge = true;
          നിർത്തുക;

        അവസ്ഥ 'z':
          വരി_delim = '\0';
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  പൂർണ്ണസംഖ്യ nfiles = argc - optind;
  എങ്കിൽ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  എങ്കിൽ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  സ്വതന്ത്രം (delims);
  സ്വതന്ത്രം (delim_lens);

  എങ്കിൽ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
