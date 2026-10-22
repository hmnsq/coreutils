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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "paste"

#నిర్వచించు AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
స్థిర bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
స్థిర bool serial_విలీనం;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
స్థిర అక్షరం *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
స్థిర size_t *delim_lens;

/* Number of delimiters.  */
స్థిర idx_t అంకె_delims;

స్థిర గుర్తులేని అక్షరం పంక్తి_delim = '\n';

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"serial", no_వాదం, ఖాళీ, 's'},
  {"delimiters", required_వాదం, ఖాళీ, 'd'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

స్థిర పూర్ణాంకం
collapse_escapes (అక్షరం స్థిరరాశి *strptr)
{
  అక్షరం *strout = xstrdup (strptr);
  bool backslash_at_చివర = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, అక్షరశ్రేణి_పొడవు (strptr)), పరిమాణం *delim_lens);

  అక్షరం స్థిరరాశి *s = strptr;
  idx_t idx = 0;

  అయితే (*s)
    {
      ఒకవేళ (*s == '\\')
        {
          s++;
          ఒకవేళ (*s == '\0')
            {
              backslash_at_చివర = true;
              ఆపు;
            }
          లేకపోతే ఒకవేళ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          లేకపోతే
            {
              ఎంపిక (*s)
                {
                స్థితి 'b': *strout++ = '\b'; ఆపు;
                స్థితి 'f': *strout++ = '\f'; ఆపు;
                స్థితి 'n': *strout++ = '\n'; ఆపు;
                స్థితి 'r': *strout++ = '\r'; ఆపు;
                స్థితి 't': *strout++ = '\t'; ఆపు;
                స్థితి 'v': *strout++ = '\v'; ఆపు;
                స్థితి '\\': *strout++ = '\\'; ఆపు;
                అప్రమేయ: వెళ్ళు copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          కొనసాగు;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  ఒకవేళ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  అంకె_delims = idx;

  తిరిగి backslash_at_చివర ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

స్థిర అంతఃవరుస శూన్య
xputchar (అక్షరం c)
{
  ఒకవేళ (అక్షరం_రాయి (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

స్థిర అంతఃవరుస శూన్య
output_delim (అక్షరం స్థిరరాశి *delimptr, size_t len)
{
  ఒకవేళ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

స్థిర bool
paste_parallel (size_t nfiles, అక్షరం **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  అక్షరం *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, పరిమాణం *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  కొరకు (files_open = 0; files_open < nfiles; ++files_open)
    {
      ఒకవేళ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      లేకపోతే
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          ఒకవేళ (fileptr[files_open] == ఖాళీ)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          లేకపోతే ఒకవేళ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  ఒకవేళ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  అయితే (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      కొరకు (size_t i = 0; i < nfiles && files_open; i++)
        {
          పూర్ణాంకం chr;			/* Input character. */
          పూర్ణాంకం err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          ఒకవేళ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              ఒకవేళ (chr != EOF && delims_saved)
                {
                  ఒకవేళ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              అయితే (chr != EOF)
                {
                  sometodo = true;
                  ఒకవేళ (chr == పంక్తి_delim)
                    ఆపు;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          ఒకవేళ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              ఒకవేళ (fileptr[i])
                {
                  ఒకవేళ (!ferror (fileptr[i]))
                    err = 0;
                  ఒకవేళ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  లేకపోతే ఒకవేళ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  ఒకవేళ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ఖాళీ;
                  files_open--;
                }

              ఒకవేళ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  ఒకవేళ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      ఒకవేళ (delims_saved)
                        {
                          ఒకవేళ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (పంక్తి_delim);
                    }
                  కొనసాగు;	/* Next read of files, or exit. */
                }
              లేకపోతే
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  ఒకవేళ (len > 0)
                    {
                      జ్ఞాపకం_నకలు (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  ఒకవేళ (++delimidx == అంకె_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          లేకపోతే
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              ఒకవేళ (i + 1 != nfiles)
                {
                  ఒకవేళ (chr != పంక్తి_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ఒకవేళ (++delimidx == అంకె_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              లేకపోతే
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  అక్షరం c = (chr == EOF ? పంక్తి_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  విడుదల (fileptr);
  విడుదల (delbuf);
  తిరిగి ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

స్థిర bool
paste_serial (size_t nfiles, అక్షరం **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  పూర్ణాంకం charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  కొరకు (; nfiles; nfiles--, fnamptr++)
    {
      పూర్ణాంకం saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      ఒకవేళ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      లేకపోతే
        {
          fileptr = fopen (*fnamptr, "r");
          ఒకవేళ (fileptr == ఖాళీ)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              కొనసాగు;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      ఒకవేళ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          అయితే ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              ఒకవేళ (charold == పంక్తి_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ఒకవేళ (++delimidx == అంకె_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              లేకపోతే
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      ఒకవేళ (charold != పంక్తి_delim)
        xputchar (పంక్తి_delim);

      ఒకవేళ (!ferror (fileptr))
        saved_errno = 0;
      ఒకవేళ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      లేకపోతే ఒకవేళ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      ఒకవేళ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  తిరిగి ok;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Write పంక్తులు consisting యొక్క corresponding పంక్తులు from each file joined by TABs\n\
గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead యొక్క TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a సమయం instead యొక్క లోపల parallel; the newline యొక్క\n\
         every పంక్తి except the చివరి పంక్తి లోపల each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  అక్షరం స్థిరరాశి *delim_arg = "\t";

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "d:sz", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ఆపు;

        స్థితి 's':
          serial_విలీనం = true;
          ఆపు;

        స్థితి 'z':
          పంక్తి_delim = '\0';
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  పూర్ణాంకం nfiles = argc - optind;
  ఒకవేళ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  ఒకవేళ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_విలీనం ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  విడుదల (delims);
  విడుదల (delim_lens);

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
