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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "paste"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
ਸਥਿਰ bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
ਸਥਿਰ bool serial_ਮਿਲਾਓ;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
ਸਥਿਰ ਅੱਖਰ *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
ਸਥਿਰ size_t *delim_lens;

/* Number of delimiters.  */
ਸਥਿਰ idx_t ਅੰਕ_delims;

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਸਤਰ_delim = '\n';

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"serial", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"delimiters", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

ਸਥਿਰ ਪੂਰਨ
collapse_escapes (ਅੱਖਰ ਅਚਲ *strptr)
{
  ਅੱਖਰ *strout = xstrdup (strptr);
  bool backslash_at_ਸਿਰਾ = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, ਲੜੀ_ਲੰਬਾਈ (strptr)), ਆਕਾਰ *delim_lens);

  ਅੱਖਰ ਅਚਲ *s = strptr;
  idx_t idx = 0;

  ਜਦੋਂ (*s)
    {
      ਜੇ (*s == '\\')
        {
          s++;
          ਜੇ (*s == '\0')
            {
              backslash_at_ਸਿਰਾ = true;
              ਰੋਕੋ;
            }
          ਨਹੀਂਤਾਂ ਜੇ (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          ਨਹੀਂਤਾਂ
            {
              ਚੋਣ (*s)
                {
                ਹਾਲਤ 'b': *strout++ = '\b'; ਰੋਕੋ;
                ਹਾਲਤ 'f': *strout++ = '\f'; ਰੋਕੋ;
                ਹਾਲਤ 'n': *strout++ = '\n'; ਰੋਕੋ;
                ਹਾਲਤ 'r': *strout++ = '\r'; ਰੋਕੋ;
                ਹਾਲਤ 't': *strout++ = '\t'; ਰੋਕੋ;
                ਹਾਲਤ 'v': *strout++ = '\v'; ਰੋਕੋ;
                ਹਾਲਤ '\\': *strout++ = '\\'; ਰੋਕੋ;
                ਮੂਲ: ਜਾਓ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          ਜਾਰੀ;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  ਜੇ (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  ਅੰਕ_delims = idx;

  ਵਾਪਸ backslash_at_ਸਿਰਾ ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
xputchar (ਅੱਖਰ c)
{
  ਜੇ (ਅੱਖਰ_ਲਿਖੋ (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
output_delim (ਅੱਖਰ ਅਚਲ *delimptr, size_t len)
{
  ਜੇ (len > 0 && fwrite (delimptr, 1, len, stdout) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

ਸਥਿਰ bool
paste_parallel (size_t nfiles, ਅੱਖਰ **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  ਅੱਖਰ *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  FILE **fileptr = xnmalloc (nfiles + 1, ਆਕਾਰ *fileptr);

  /* Number of files still open to process.  */
  size_t files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  ਲਈ (files_open = 0; files_open < nfiles; ++files_open)
    {
      ਜੇ (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = stdin;
        }
      ਨਹੀਂਤਾਂ
        {
          fileptr[files_open] = fopen (fnamptr[files_open], "r");
          ਜੇ (fileptr[files_open] == ਨਿਰਾ)
            error (EXIT_FAILURE, errno, "%s", quotef (fnamptr[files_open]));
          ਨਹੀਂਤਾਂ ਜੇ (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  ਜੇ (opened_stdin && have_read_stdin)
    error (EXIT_FAILURE, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  ਜਦੋਂ (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      ਲਈ (size_t i = 0; i < nfiles && files_open; i++)
        {
          ਪੂਰਨ chr;			/* Input character. */
          ਪੂਰਨ err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          ਜੇ (fileptr[i])
            {
              chr = getc (fileptr[i]);
              err = errno;
              ਜੇ (chr != EOF && delims_saved)
                {
                  ਜੇ (fwrite (delbuf, 1, delims_saved, stdout) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              ਜਦੋਂ (chr != EOF)
                {
                  sometodo = true;
                  ਜੇ (chr == ਸਤਰ_delim)
                    ਰੋਕੋ;
                  xputchar (chr);
                  chr = getc (fileptr[i]);
                  err = errno;
                }
            }

          ਜੇ (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              ਜੇ (fileptr[i])
                {
                  ਜੇ (!ferror (fileptr[i]))
                    err = 0;
                  ਜੇ (fileptr[i] == stdin)
                    clearerr (fileptr[i]); /* Also clear EOF. */
                  ਨਹੀਂਤਾਂ ਜੇ (fclose (fileptr[i]) == EOF && !err)
                    err = errno;
                  ਜੇ (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = ਨਿਰਾ;
                  files_open--;
                }

              ਜੇ (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  ਜੇ (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      ਜੇ (delims_saved)
                        {
                          ਜੇ (fwrite (delbuf, 1, delims_saved, stdout)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (ਸਤਰ_delim);
                    }
                  ਜਾਰੀ;	/* Next read of files, or exit. */
                }
              ਨਹੀਂਤਾਂ
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  size_t len = delim_lens[delimidx];
                  ਜੇ (len > 0)
                    {
                      ਸਮ੍ਰਿਤੀ_ਨਕਲ (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  ਜੇ (++delimidx == ਅੰਕ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          ਨਹੀਂਤਾਂ
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              ਜੇ (i + 1 != nfiles)
                {
                  ਜੇ (chr != ਸਤਰ_delim && chr != EOF)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ਜੇ (++delimidx == ਅੰਕ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ਨਹੀਂਤਾਂ
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  ਅੱਖਰ c = (chr == EOF ? ਸਤਰ_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  ਮੁਕਤ (fileptr);
  ਮੁਕਤ (delbuf);
  ਵਾਪਸ ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

ਸਥਿਰ bool
paste_serial (size_t nfiles, ਅੱਖਰ **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  ਪੂਰਨ charnew, charold; /* Current and previous char read. */
  FILE *fileptr;	/* Open for reading current file. */

  ਲਈ (; nfiles; nfiles--, fnamptr++)
    {
      ਪੂਰਨ saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      ਜੇ (is_stdin)
        {
          have_read_stdin = true;
          fileptr = stdin;
        }
      ਨਹੀਂਤਾਂ
        {
          fileptr = fopen (*fnamptr, "r");
          ਜੇ (fileptr == ਨਿਰਾ)
            {
              error (0, errno, "%s", quotef (*fnamptr));
              ok = false;
              ਜਾਰੀ;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = getc (fileptr);
      saved_errno = errno;
      ਜੇ (charold != EOF)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          ਜਦੋਂ ((charnew = getc (fileptr)) != EOF)
            {
              /* Process the old character. */
              ਜੇ (charold == ਸਤਰ_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  ਜੇ (++delimidx == ਅੰਕ_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              ਨਹੀਂਤਾਂ
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = errno;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      ਜੇ (charold != ਸਤਰ_delim)
        xputchar (ਸਤਰ_delim);

      ਜੇ (!ferror (fileptr))
        saved_errno = 0;
      ਜੇ (is_stdin)
        clearerr (fileptr);	/* Also clear EOF. */
      ਨਹੀਂਤਾਂ ਜੇ (fclose (fileptr) != 0 && !saved_errno)
        saved_errno = errno;
      ਜੇ (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  ਵਾਪਸ ok;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Write ਸਤਰਾਂ consisting ਦਾ corresponding ਸਤਰਾਂ from each file joined by TABs\n\
ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead ਦਾ TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a ਸਮਾਂ instead ਦਾ ਵਿੱਚ parallel; the newline ਦਾ\n\
         every ਸਤਰ except the ਆਖਰੀ ਸਤਰ ਵਿੱਚ each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  ਅੱਖਰ ਅਚਲ *delim_arg = "\t";

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "d:sz", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          serial_ਮਿਲਾਓ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          ਸਤਰ_delim = '\0';
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਪੂਰਨ nfiles = argc - optind;
  ਜੇ (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  ਜੇ (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (EXIT_FAILURE, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_ਮਿਲਾਓ ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  ਮੁਕਤ (delims);
  ਮੁਕਤ (delim_lens);

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");
  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
