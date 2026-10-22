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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "fadvise.h"
#شامل "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "paste"

#تعریف_کرو AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ساکن bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ساکن bool serial_ملاؤ;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ساکن حرف *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ساکن size_t *delim_lens;

/* Number of delimiters.  */
ساکن idx_t عدد_delims;

ساکن غیرعلامتی حرف سطر_delim = '\n';

ساکن ساخت option ثابت longopts[] =
{
  {"serial", no_دلیل, تہی, 's'},
  {"delimiters", required_دلیل, تہی, 'd'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ساکن صحیح
collapse_escapes (حرف ثابت *strptr)
{
  حرف *strout = xstrdup (strptr);
  bool backslash_at_انتہا = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, لڑی_لمبائی (strptr)), حجم *delim_lens);

  حرف ثابت *s = strptr;
  idx_t idx = 0;

  جب (*s)
    {
      اگر (*s == '\\')
        {
          s++;
          اگر (*s == '\0')
            {
              backslash_at_انتہا = true;
              توڑو;
            }
          ورنہ اگر (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          ورنہ
            {
              انتخاب (*s)
                {
                حالت 'b': *strout++ = '\b'; توڑو;
                حالت 'f': *strout++ = '\f'; توڑو;
                حالت 'n': *strout++ = '\n'; توڑو;
                حالت 'r': *strout++ = '\r'; توڑو;
                حالت 't': *strout++ = '\t'; توڑو;
                حالت 'v': *strout++ = '\v'; توڑو;
                حالت '\\': *strout++ = '\\'; توڑو;
                طےشدہ: جاؤ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          جاری;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  اگر (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  عدد_delims = idx;

  واپس backslash_at_انتہا ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ساکن اندرونی خالی
xputchar (حرف c)
{
  اگر (حرف_لکھو (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ساکن اندرونی خالی
output_delim (حرف ثابت *delimptr, size_t len)
{
  اگر (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ساکن bool
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

  برائے (files_open = 0; files_open < nfiles; ++files_open)
    {
      اگر (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      ورنہ
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          اگر (fileptr[files_open] == تہی)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          ورنہ اگر (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  اگر (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  جب (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      برائے (size_t i = 0; i < nfiles && files_open; i++)
        {
          صحیح chr;			/* Input character. */
          صحیح err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          اگر (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              اگر (chr != EOF && delims_saved)
                {
                  اگر (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              جب (chr != EOF)
                {
                  sometodo = true;
                  اگر (chr == سطر_delim)
                    توڑو;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          اگر (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              اگر (fileptr[i])
                {
                  اگر (!ferror (fileptr[i]))
                    err = 0;
                  اگر (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  ورنہ اگر (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  اگر (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = تہی;
                  files_open--;
                }

              اگر (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  اگر (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      اگر (delims_saved)
                        {
                          اگر (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (سطر_delim);
                    }
                  جاری;	/* Next read of files, or exit. */
                }
              ورنہ
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  اگر (len > 0)
                    {
                      حافظہ_نقل (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  اگر (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          ورنہ
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              اگر (i + 1 != nfiles)
                {
                  اگر (chr != سطر_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  اگر (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ورنہ
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  حرف c = (chr == EOF ? سطر_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  آزاد (fileptr);
  آزاد (delbuf);
  واپس ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ساکن bool
paste_serial (size_t nfiles, حرف **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  صحیح charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  برائے (; nfiles; nfiles--, fnamptr++)
    {
      صحیح saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      اگر (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      ورنہ
        {
          fileptr = fopen (*fnamptr, "r");
          اگر (fileptr == تہی)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              جاری;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      اگر (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          جب ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              اگر (charold == سطر_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  اگر (++delimidx == عدد_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ورنہ
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      اگر (charold != سطر_delim)
        xputchar (سطر_delim);

      اگر (!ferror (fileptr))
        saved_errno = 0;
      اگر (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      ورنہ اگر (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      اگر (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  واپس ok;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Write سطریں consisting کا corresponding سطریں from each file joined by TABs\n\
مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead کا TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a وقت instead کا اندر parallel; the newline کا\n\
         every سطر except the آخری سطر اندر each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  حرف ثابت *delim_arg = "\t";

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "d:sz", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          توڑو;

        حالت 's':
          serial_ملاؤ = true;
          توڑو;

        حالت 'z':
          سطر_delim = '\0';
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  صحیح nfiles = argc - optind;
  اگر (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  اگر (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_ملاؤ ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  آزاد (delims);
  آزاد (delim_lens);

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
