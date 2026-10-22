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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ସ୍ଥିର bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ସ୍ଥିର bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ସ୍ଥିର ଅକ୍ଷର *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ସ୍ଥିର size_t *delim_lens;

/* Number of delimiters.  */
ସ୍ଥିର idx_t ଅଙ୍କ_delims;

ସ୍ଥିର unsigned ଅକ୍ଷର ଧାଡ଼ି_delim = '\n';

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"serial", no_argument, ଖାଲି, 's'},
  {"delimiters", required_argument, ଖାଲି, 'd'},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
collapse_escapes (ଅକ୍ଷର ଅଚଳ *strptr)
{
  ଅକ୍ଷର *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), ଆକାର *delim_lens);

  ଅକ୍ଷର ଅଚଳ *s = strptr;
  idx_t idx = 0;

  ଯେତେବେଳେ (*s)
    {
      ଯଦି (*s == '\\')
        {
          s++;
          ଯଦି (*s == '\0')
            {
              backslash_at_end = true;
              ଭାଙ୍ଗ;
            }
          ଅନ୍ୟଥା ଯଦି (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          ଅନ୍ୟଥା
            {
              ବାଛ (*s)
                {
                ସ୍ଥିତି 'b': *strout++ = '\b'; ଭାଙ୍ଗ;
                ସ୍ଥିତି 'f': *strout++ = '\f'; ଭାଙ୍ଗ;
                ସ୍ଥିତି 'n': *strout++ = '\n'; ଭାଙ୍ଗ;
                ସ୍ଥିତି 'r': *strout++ = '\r'; ଭାଙ୍ଗ;
                ସ୍ଥିତି 't': *strout++ = '\t'; ଭାଙ୍ଗ;
                ସ୍ଥିତି 'v': *strout++ = '\v'; ଭାଙ୍ଗ;
                ସ୍ଥିତି '\\': *strout++ = '\\'; ଭାଙ୍ଗ;
                ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଯାଅ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          ଜାରି;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  ଯଦି (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  ଅଙ୍କ_delims = idx;

  ଫେରନ୍ତୁ backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
xputchar (ଅକ୍ଷର c)
{
  ଯଦି (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
output_delim (ଅକ୍ଷର ଅଚଳ *delimptr, size_t len)
{
  ଯଦି (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ସ୍ଥିର bool
paste_parallel (size_t nfiles, ଅକ୍ଷର **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  ଅକ୍ଷର *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, ଆକାର *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  ପାଇଁ (files_open = 0; files_open < nfiles; ++files_open)
    {
      ଯଦି (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      ଅନ୍ୟଥା
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          ଯଦି (fileptr[files_open] == ଖାଲି)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          ଅନ୍ୟଥା ଯଦି (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  ଯଦି (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  ଯେତେବେଳେ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      ପାଇଁ (size_t i = 0; i < nfiles && files_open; i++)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା chr;			/* Input character. */
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          ଯଦି (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              ଯଦି (chr != EOF && delims_saved)
                {
                  ଯଦି (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              ଯେତେବେଳେ (chr != EOF)
                {
                  sometodo = true;
                  ଯଦି (chr == ଧାଡ଼ି_delim)
                    ଭାଙ୍ଗ;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          ଯଦି (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              ଯଦି (fileptr[i])
                {
                  ଯଦି (!ferror (fileptr[i]))
                    err = 0;
                  ଯଦି (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  ଅନ୍ୟଥା ଯଦି (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  ଯଦି (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ଖାଲି;
                  files_open--;
                }

              ଯଦି (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  ଯଦି (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      ଯଦି (delims_saved)
                        {
                          ଯଦି (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (ଧାଡ଼ି_delim);
                    }
                  ଜାରି;	/* Next read of files, or exit. */
                }
              ଅନ୍ୟଥା
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  ଯଦି (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  ଯଦି (++delimidx == ଅଙ୍କ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          ଅନ୍ୟଥା
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              ଯଦି (i + 1 != nfiles)
                {
                  ଯଦି (chr != ଧାଡ଼ି_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ଯଦି (++delimidx == ଅଙ୍କ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ଅନ୍ୟଥା
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  ଅକ୍ଷର c = (chr == EOF ? ଧାଡ଼ି_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  ମୁକ୍ତ (fileptr);
  ମୁକ୍ତ (delbuf);
  ଫେରନ୍ତୁ ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ସ୍ଥିର bool
paste_serial (size_t nfiles, ଅକ୍ଷର **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  ପାଇଁ (; nfiles; nfiles--, fnamptr++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      ଯଦି (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      ଅନ୍ୟଥା
        {
          fileptr = fopen (*fnamptr, "r");
          ଯଦି (fileptr == ଖାଲି)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              ଜାରି;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      ଯଦି (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          ଯେତେବେଳେ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              ଯଦି (charold == ଧାଡ଼ି_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ଯଦି (++delimidx == ଅଙ୍କ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ଅନ୍ୟଥା
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      ଯଦି (charold != ଧାଡ଼ି_delim)
        xputchar (ଧାଡ଼ି_delim);

      ଯଦି (!ferror (fileptr))
        saved_errno = 0;
      ଯଦି (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      ଅନ୍ୟଥା ଯଦି (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      ଯଦି (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  ଫେରନ୍ତୁ ok;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write ଧାଡ଼ିଗୁଡ଼ିକ consisting of corresponding ଧାଡ଼ିଗୁଡ଼ିକ from each file joined by TABs\n\
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
         every ଧାଡ଼ି except the last ଧାଡ଼ି in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଅକ୍ଷର ଅଚଳ *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "d:sz", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          serial_merge = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          ଧାଡ଼ି_delim = '\0';
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles = argc - optind;
  ଯଦି (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  ଯଦି (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  ମୁକ୍ତ (delims);
  ମୁକ୍ତ (delim_lens);

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
