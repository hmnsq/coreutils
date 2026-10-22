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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
স্থির bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
স্থির bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
স্থির বর্ণ *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
স্থির size_t *delim_lens;

/* Number of delimiters.  */
স্থির idx_t অঙ্ক_delims;

স্থির unsigned বর্ণ সারি_delim = '\n';

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"serial", no_argument, রিক্ত, 's'},
  {"delimiters", required_argument, রিক্ত, 'd'},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

স্থির পূর্ণাঙ্ক
collapse_escapes (বর্ণ ধ্রুব *strptr)
{
  বর্ণ *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), পরিমাণ *delim_lens);

  বর্ণ ধ্রুব *s = strptr;
  idx_t idx = 0;

  যাবত্ (*s)
    {
      যদি (*s == '\\')
        {
          s++;
          যদি (*s == '\0')
            {
              backslash_at_end = true;
              ভঙ্গ;
            }
          অন্যথা যদি (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          অন্যথা
            {
              বিকল্প (*s)
                {
                পক্ষ 'b': *strout++ = '\b'; ভঙ্গ;
                পক্ষ 'f': *strout++ = '\f'; ভঙ্গ;
                পক্ষ 'n': *strout++ = '\n'; ভঙ্গ;
                পক্ষ 'r': *strout++ = '\r'; ভঙ্গ;
                পক্ষ 't': *strout++ = '\t'; ভঙ্গ;
                পক্ষ 'v': *strout++ = '\v'; ভঙ্গ;
                পক্ষ '\\': *strout++ = '\\'; ভঙ্গ;
                স্বভাব: গচ্ছ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          অনুবর্ত;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  যদি (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  অঙ্ক_delims = idx;

  প্রত্যাগম backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

স্থির অন্তঃস্থ শূন্য
xputchar (বর্ণ c)
{
  যদি (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

স্থির অন্তঃস্থ শূন্য
output_delim (বর্ণ ধ্রুব *delimptr, size_t len)
{
  যদি (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

স্থির bool
paste_parallel (size_t nfiles, বর্ণ **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  বর্ণ *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, পরিমাণ *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  প্রতি (files_open = 0; files_open < nfiles; ++files_open)
    {
      যদি (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      অন্যথা
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          যদি (fileptr[files_open] == রিক্ত)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          অন্যথা যদি (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  যদি (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  যাবত্ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      প্রতি (size_t i = 0; i < nfiles && files_open; i++)
        {
          পূর্ণাঙ্ক chr;			/* Input character. */
          পূর্ণাঙ্ক err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          যদি (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              যদি (chr != EOF && delims_saved)
                {
                  যদি (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              যাবত্ (chr != EOF)
                {
                  sometodo = true;
                  যদি (chr == সারি_delim)
                    ভঙ্গ;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          যদি (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              যদি (fileptr[i])
                {
                  যদি (!ferror (fileptr[i]))
                    err = 0;
                  যদি (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  অন্যথা যদি (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  যদি (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = রিক্ত;
                  files_open--;
                }

              যদি (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  যদি (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      যদি (delims_saved)
                        {
                          যদি (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (সারি_delim);
                    }
                  অনুবর্ত;	/* Next read of files, or exit. */
                }
              অন্যথা
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  যদি (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  যদি (++delimidx == অঙ্ক_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          অন্যথা
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              যদি (i + 1 != nfiles)
                {
                  যদি (chr != সারি_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  যদি (++delimidx == অঙ্ক_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              অন্যথা
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  বর্ণ c = (chr == EOF ? সারি_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  মুক্ত (fileptr);
  মুক্ত (delbuf);
  প্রত্যাগম ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

স্থির bool
paste_serial (size_t nfiles, বর্ণ **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  পূর্ণাঙ্ক charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  প্রতি (; nfiles; nfiles--, fnamptr++)
    {
      পূর্ণাঙ্ক saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      যদি (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      অন্যথা
        {
          fileptr = fopen (*fnamptr, "r");
          যদি (fileptr == রিক্ত)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              অনুবর্ত;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      যদি (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          যাবত্ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              যদি (charold == সারি_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  যদি (++delimidx == অঙ্ক_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              অন্যথা
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      যদি (charold != সারি_delim)
        xputchar (সারি_delim);

      যদি (!ferror (fileptr))
        saved_errno = 0;
      যদি (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      অন্যথা যদি (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      যদি (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  প্রত্যাগম ok;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write সারিগুলি consisting of corresponding সারিগুলি from each file joined by TABs\n\
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
         every সারি except the last সারি in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক optc;
  বর্ণ ধ্রুব *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যাবত্ ((optc = getopt_long (argc, argv, "d:sz", longopts, রিক্ত)) != -1)
    {
      বিকল্প (optc)
        {
        পক্ষ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ভঙ্গ;

        পক্ষ 's':
          serial_merge = true;
          ভঙ্গ;

        পক্ষ 'z':
          সারি_delim = '\0';
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  পূর্ণাঙ্ক nfiles = argc - optind;
  যদি (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  যদি (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  মুক্ত (delims);
  মুক্ত (delim_lens);

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  প্রত্যাগম ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
