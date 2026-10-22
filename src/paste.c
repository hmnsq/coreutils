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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "paste"

#define AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
સ્થિર bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
સ્થિર bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
સ્થિર અક્ષર *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
સ્થિર size_t *delim_lens;

/* Number of delimiters.  */
સ્થિર idx_t અંક_delims;

સ્થિર unsigned અક્ષર લીટી_delim = '\n';

સ્થિર સંરચના option અચલ longopts[] =
{
  {"serial", no_argument, ખાલી, 's'},
  {"delimiters", required_argument, ખાલી, 'd'},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

સ્થિર પૂર્ણાંક
collapse_escapes (અક્ષર અચલ *strptr)
{
  અક્ષર *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, strlen (strptr)), કદ *delim_lens);

  અક્ષર અચલ *s = strptr;
  idx_t idx = 0;

  જ્યારે (*s)
    {
      જો (*s == '\\')
        {
          s++;
          જો (*s == '\0')
            {
              backslash_at_end = true;
              તોડો;
            }
          નહીંતર જો (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          નહીંતર
            {
              પસંદગી (*s)
                {
                સ્થિતિ 'b': *strout++ = '\b'; તોડો;
                સ્થિતિ 'f': *strout++ = '\f'; તોડો;
                સ્થિતિ 'n': *strout++ = '\n'; તોડો;
                સ્થિતિ 'r': *strout++ = '\r'; તોડો;
                સ્થિતિ 't': *strout++ = '\t'; તોડો;
                સ્થિતિ 'v': *strout++ = '\v'; તોડો;
                સ્થિતિ '\\': *strout++ = '\\'; તોડો;
                મૂળભૂત: જાઓ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          ચાલુ;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  જો (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  અંક_delims = idx;

  પાછા backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

સ્થિર અંતઃરેખ શૂન્ય
xputchar (અક્ષર c)
{
  જો (putchar (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

સ્થિર અંતઃરેખ શૂન્ય
output_delim (અક્ષર અચલ *delimptr, size_t len)
{
  જો (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

સ્થિર bool
paste_parallel (size_t nfiles, અક્ષર **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  અક્ષર *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, કદ *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  માટે (files_open = 0; files_open < nfiles; ++files_open)
    {
      જો (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      નહીંતર
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          જો (fileptr[files_open] == ખાલી)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          નહીંતર જો (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  જો (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  જ્યારે (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      માટે (size_t i = 0; i < nfiles && files_open; i++)
        {
          પૂર્ણાંક chr;			/* Input character. */
          પૂર્ણાંક err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          જો (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              જો (chr != EOF && delims_saved)
                {
                  જો (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              જ્યારે (chr != EOF)
                {
                  sometodo = true;
                  જો (chr == લીટી_delim)
                    તોડો;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          જો (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              જો (fileptr[i])
                {
                  જો (!ferror (fileptr[i]))
                    err = 0;
                  જો (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  નહીંતર જો (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  જો (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ખાલી;
                  files_open--;
                }

              જો (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  જો (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      જો (delims_saved)
                        {
                          જો (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (લીટી_delim);
                    }
                  ચાલુ;	/* Next read of files, or exit. */
                }
              નહીંતર
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  જો (len > 0)
                    {
                      memcpy (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  જો (++delimidx == અંક_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          નહીંતર
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              જો (i + 1 != nfiles)
                {
                  જો (chr != લીટી_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  જો (++delimidx == અંક_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              નહીંતર
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  અક્ષર c = (chr == EOF ? લીટી_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  મુક્ત (fileptr);
  મુક્ત (delbuf);
  પાછા ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

સ્થિર bool
paste_serial (size_t nfiles, અક્ષર **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  પૂર્ણાંક charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  માટે (; nfiles; nfiles--, fnamptr++)
    {
      પૂર્ણાંક saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      જો (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      નહીંતર
        {
          fileptr = fopen (*fnamptr, "r");
          જો (fileptr == ખાલી)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              ચાલુ;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      જો (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          જ્યારે ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              જો (charold == લીટી_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  જો (++delimidx == અંક_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              નહીંતર
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      જો (charold != લીટી_delim)
        xputchar (લીટી_delim);

      જો (!ferror (fileptr))
        saved_errno = 0;
      જો (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      નહીંતર જો (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      જો (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  પાછા ok;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Write લીટીઓ consisting of corresponding લીટીઓ from each file joined by TABs\n\
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
         every લીટી except the last લીટી in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;
  અક્ષર અચલ *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "d:sz", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          તોડો;

        સ્થિતિ 's':
          serial_merge = true;
          તોડો;

        સ્થિતિ 'z':
          લીટી_delim = '\0';
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  પૂર્ણાંક nfiles = argc - optind;
  જો (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  જો (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  મુક્ત (delims);
  મુક્ત (delim_lens);

  જો (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
