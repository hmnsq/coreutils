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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
நிலையான bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
நிலையான bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
நிலையான எழுத்து *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
நிலையான size_t *delim_lens;

/* Number of delimiters.  */
நிலையான idx_t இலக்கம்_delims;

நிலையான unsigned எழுத்து வரி_delim = '\n';

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"serial", no_argument, வெற்று, 's'},
  {"delimiters", required_argument, வெற்று, 'd'},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

நிலையான முழு
collapse_escapes (எழுத்து மாறிலி *strptr)
{
  எழுத்து *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), அளவு *delim_lens);

  எழுத்து மாறிலி *s = strptr;
  idx_t idx = 0;

  போது (*s)
    {
      எனில் (*s == '\\')
        {
          s++;
          எனில் (*s == '\0')
            {
              backslash_at_end = true;
              நிறுத்து;
            }
          இல்லையெனில் எனில் (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          இல்லையெனில்
            {
              தேர்வு (*s)
                {
                நிலை 'b': *strout++ = '\b'; நிறுத்து;
                நிலை 'f': *strout++ = '\f'; நிறுத்து;
                நிலை 'n': *strout++ = '\n'; நிறுத்து;
                நிலை 'r': *strout++ = '\r'; நிறுத்து;
                நிலை 't': *strout++ = '\t'; நிறுத்து;
                நிலை 'v': *strout++ = '\v'; நிறுத்து;
                நிலை '\\': *strout++ = '\\'; நிறுத்து;
                இயல்பு: செல் copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          தொடர்;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  எனில் (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  இலக்கம்_delims = idx;

  திரும்பு backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

நிலையான உள்வரி வெறுமை
xputchar (எழுத்து c)
{
  எனில் (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

நிலையான உள்வரி வெறுமை
output_delim (எழுத்து மாறிலி *delimptr, size_t len)
{
  எனில் (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

நிலையான bool
paste_parallel (size_t nfiles, எழுத்து **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  எழுத்து *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, அளவு *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  க்கு (files_open = 0; files_open < nfiles; ++files_open)
    {
      எனில் (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      இல்லையெனில்
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          எனில் (fileptr[files_open] == வெற்று)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          இல்லையெனில் எனில் (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  எனில் (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  போது (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      க்கு (size_t i = 0; i < nfiles && files_open; i++)
        {
          முழு chr;			/* Input character. */
          முழு err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          எனில் (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              எனில் (chr != EOF && delims_saved)
                {
                  எனில் (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              போது (chr != EOF)
                {
                  sometodo = true;
                  எனில் (chr == வரி_delim)
                    நிறுத்து;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          எனில் (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              எனில் (fileptr[i])
                {
                  எனில் (!ferror (fileptr[i]))
                    err = 0;
                  எனில் (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  இல்லையெனில் எனில் (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  எனில் (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = வெற்று;
                  files_open--;
                }

              எனில் (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  எனில் (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      எனில் (delims_saved)
                        {
                          எனில் (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (வரி_delim);
                    }
                  தொடர்;	/* Next read of files, or exit. */
                }
              இல்லையெனில்
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  எனில் (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  எனில் (++delimidx == இலக்கம்_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          இல்லையெனில்
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              எனில் (i + 1 != nfiles)
                {
                  எனில் (chr != வரி_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  எனில் (++delimidx == இலக்கம்_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              இல்லையெனில்
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  எழுத்து c = (chr == EOF ? வரி_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  விடுவி (fileptr);
  விடுவி (delbuf);
  திரும்பு ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

நிலையான bool
paste_serial (size_t nfiles, எழுத்து **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  முழு charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  க்கு (; nfiles; nfiles--, fnamptr++)
    {
      முழு saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      எனில் (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      இல்லையெனில்
        {
          fileptr = fopen (*fnamptr, "r");
          எனில் (fileptr == வெற்று)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              தொடர்;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      எனில் (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          போது ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              எனில் (charold == வரி_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  எனில் (++delimidx == இலக்கம்_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              இல்லையெனில்
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      எனில் (charold != வரி_delim)
        xputchar (வரி_delim);

      எனில் (!ferror (fileptr))
        saved_errno = 0;
      எனில் (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      இல்லையெனில் எனில் (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      எனில் (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  திரும்பு ok;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write வரிகள் consisting of corresponding வரிகள் from each file joined by TABs\n\
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
         every வரி except the last வரி in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;
  எழுத்து மாறிலி *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "d:sz", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          நிறுத்து;

        நிலை 's':
          serial_merge = true;
          நிறுத்து;

        நிலை 'z':
          வரி_delim = '\0';
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  முழு nfiles = argc - optind;
  எனில் (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  எனில் (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  விடுவி (delims);
  விடுவி (delim_lens);

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
