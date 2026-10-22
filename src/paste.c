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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "fadvise.h"
#समावेश "mcel.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "paste"

#घोषणा AUTHORS \
  proper_name ("David M. Ihnat"), \
  proper_name ("David MacKenzie")

/* If nonzero, we have read standard input at some point. */
अटल bool have_read_stdin;

/* If nonzero, merge subsequent lines of each file rather than
   corresponding lines from each file in parallel. */
अटल bool serial_merge;

/* The delimiters between lines of input files (used cyclically).
   This stores the raw bytes of all delimiters concatenated.  */
अटल अक्षर *delims;

/* Length of each delimiter in bytes (supports multi-byte characters).
   A length of 0 indicates no delimiter at this position (from \0 escape).  */
अटल माप_प्रकार *delim_lens;

/* Number of delimiters.  */
अटल idx_t अंक_delims;

अटल अचिन्हित अक्षर पंक्ति_delim = '\n';

अटल काष्ठा option स्थिर longopts[] =
{
  {"serial", no_argument, शून्य, 's'},
  {"delimiters", required_argument, शून्य, 'd'},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Set globals delims, delim_lens, and num_delims.
   Process STRPTR converting backslash representations of special characters
   to their actual values.  The set of possible backslash characters has been
   expanded beyond that recognized by the Unix version.
   Return 0 upon success.
   If the string ends in an odd number of backslashes, ignore the
   final backslash and return nonzero.  */

अटल पूर्णांक
collapse_escapes (अक्षर स्थिर *strptr)
{
  अक्षर *strout = xstrdup (strptr);
  bool backslash_at_end = false;

  delims = strout;
  delim_lens = xnmalloc (MAX (1, म_माप (strptr)), माप *delim_lens);

  अक्षर स्थिर *s = strptr;
  idx_t idx = 0;

  जबतक (*s)
    {
      अगर (*s == '\\')
        {
          s++;
          अगर (*s == '\0')
            {
              backslash_at_end = true;
              अवरोध;
            }
          अन्यथा अगर (*s == '0')
            {
              /* Empty delimiter at this position.  */
              s++;
              delim_lens[idx++] = 0;
            }
          अन्यथा
            {
              चयन (*s)
                {
                हाल 'b': *strout++ = '\b'; अवरोध;
                हाल 'f': *strout++ = '\f'; अवरोध;
                हाल 'n': *strout++ = '\n'; अवरोध;
                हाल 'r': *strout++ = '\r'; अवरोध;
                हाल 't': *strout++ = '\t'; अवरोध;
                हाल 'v': *strout++ = '\v'; अवरोध;
                हाल '\\': *strout++ = '\\'; अवरोध;
                शेष: जाओ copy_character;
                }

              s++;
              delim_lens[idx++] = 1;
            }

          जारी;
        }

      copy_character:;
      mcel_t g = mcel_scanz (s);
      strout = mempcpy (strout, s, g.len);
      s += g.len;
      delim_lens[idx++] = g.len;
    }

  *strout = '\0';

  अगर (idx == 0)
    {
      delim_lens[0] = 0;
      idx = 1;
    }

  अंक_delims = idx;

  वापस backslash_at_end ? 1 : 0;
}

/* Output a single byte, reporting any write errors.  */

अटल अंतरभूत व्योम
xputchar (अक्षर c)
{
  अगर (अक्षर_दो (c) < 0)
    write_error ();
}

/* Output the delimiter at DELIMPTR with length LEN.
   If LEN is 0, nothing is output (empty delimiter from \0 escape).  */

अटल अंतरभूत व्योम
output_delim (अक्षर स्थिर *delimptr, माप_प्रकार len)
{
  अगर (len > 0 && ख_डालो (delimptr, 1, len, मानक_निकास) != len)
    write_error ();
}

/* Perform column paste on the NFILES files named in FNAMPTR.
   Return true if successful, false if one or more files could not be
   opened or read. */

अटल bool
paste_parallel (माप_प्रकार nfiles, अक्षर **fnamptr)
{
  bool ok = true;
  /* If all files are just ready to be closed, or will be on this
     round, the string of delimiters must be preserved.
     delbuf stores the delimiter bytes for closed files.
     Size it to hold up to (nfiles - 1) delimiters.  */
  अक्षर *delbuf = xmalloc ((nfiles - 1) * MB_CUR_MAX + 1);

  /* Streams open to the files to process; null if the corresponding
     stream is closed.  */
  खाता **fileptr = xnmalloc (nfiles + 1, माप *fileptr);

  /* Number of files still open to process.  */
  माप_प्रकार files_open;

  /* True if any fopen got fd == STDIN_FILENO.  */
  bool opened_stdin = false;

  /* Attempt to open all files.  This could be expanded to an infinite
     number of files, but at the (considerable) expense of remembering
     each file and its current offset, then opening/reading/closing.  */

  क्रम (files_open = 0; files_open < nfiles; ++files_open)
    {
      अगर (streq (fnamptr[files_open], "-"))
        {
          have_read_stdin = true;
          fileptr[files_open] = मानक_निवेश;
        }
      अन्यथा
        {
          fileptr[files_open] = ख_खोलो (fnamptr[files_open], "r");
          अगर (fileptr[files_open] == शून्य)
            error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (fnamptr[files_open]));
          अन्यथा अगर (fileno (fileptr[files_open]) == STDIN_FILENO)
            opened_stdin = true;
          fadvise (fileptr[files_open], FADVISE_SEQUENTIAL);
        }
    }

  अगर (opened_stdin && have_read_stdin)
    error (निकास_त्रुटि, 0, _("standard input is closed"));

  /* Read a line from each file and output it to stdout separated by a
     delimiter, until we go through the loop without successfully
     reading from any of the files. */

  जबतक (files_open)
    {
      /* Set up for the next line. */
      bool somedone = false;
      idx_t delimidx = 0;              /* Current delimiter index.  */
      idx_t delimoff = 0;              /* Current offset into delims.  */
      idx_t delims_saved = 0;          /* Bytes saved in 'delbuf'. */

      क्रम (माप_प्रकार i = 0; i < nfiles && files_open; i++)
        {
          पूर्णांक chr;			/* Input character. */
          पूर्णांक err;			/* Input errno value.  */
          bool sometodo = false;	/* Input chars to process.  */

          अगर (fileptr[i])
            {
              chr = अ_लो (fileptr[i]);
              err = त्रुटि_सं;
              अगर (chr != खातापूर्ण && delims_saved)
                {
                  अगर (ख_डालो (delbuf, 1, delims_saved, मानक_निकास) != delims_saved)
                    write_error ();
                  delims_saved = 0;
                }

              जबतक (chr != खातापूर्ण)
                {
                  sometodo = true;
                  अगर (chr == पंक्ति_delim)
                    अवरोध;
                  xputchar (chr);
                  chr = अ_लो (fileptr[i]);
                  err = त्रुटि_सं;
                }
            }

          अगर (! sometodo)
            {
              /* EOF, read error, or closed file.
                 If an EOF or error, close the file.  */
              अगर (fileptr[i])
                {
                  अगर (!ख_त्रुटि (fileptr[i]))
                    err = 0;
                  अगर (fileptr[i] == मानक_निवेश)
                    साफ_त्रुटि (fileptr[i]); /* Also clear EOF. */
                  अन्यथा अगर (ख_बंद (fileptr[i]) == खातापूर्ण && !err)
                    err = त्रुटि_सं;
                  अगर (err)
                    {
                      error (0, err, "%s", quotef (fnamptr[i]));
                      ok = false;
                    }

                  fileptr[i] = शून्य;
                  files_open--;
                }

              अगर (i + 1 == nfiles)
                {
                  /* End of this output line.
                     Is this the end of the whole thing? */
                  अगर (somedone)
                    {
                      /* No.  Some files were not closed for this line. */
                      अगर (delims_saved)
                        {
                          अगर (ख_डालो (delbuf, 1, delims_saved, मानक_निकास)
                              != delims_saved)
                            write_error ();
                          delims_saved = 0;
                        }
                      xputchar (पंक्ति_delim);
                    }
                  जारी;	/* Next read of files, or exit. */
                }
              अन्यथा
                {
                  /* Closed file; add delimiter to 'delbuf'. */
                  माप_प्रकार len = delim_lens[delimidx];
                  अगर (len > 0)
                    {
                      स_नकल (delbuf + delims_saved, delims + delimoff, len);
                      delims_saved += len;
                    }
                  delimoff += len;
                  अगर (++delimidx == अंक_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
            }
          अन्यथा
            {
              /* Some data read. */
              somedone = true;

              /* Except for last file, replace last newline with delim. */
              अगर (i + 1 != nfiles)
                {
                  अगर (chr != पंक्ति_delim && chr != खातापूर्ण)
                    xputchar (chr);
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  अगर (++delimidx == अंक_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              अन्यथा
                {
                  /* If the last line of the last file lacks a newline,
                     print one anyhow.  POSIX requires this.  */
                  अक्षर c = (chr == खातापूर्ण ? पंक्ति_delim : chr);
                  xputchar (c);
                }
            }
        }
    }
  मुक्त (fileptr);
  मुक्त (delbuf);
  वापस ok;
}

/* Perform serial paste on the NFILES files named in FNAMPTR.
   Return true if no errors, false if one or more files could not be
   opened or read. */

अटल bool
paste_serial (माप_प्रकार nfiles, अक्षर **fnamptr)
{
  bool ok = true;	/* false if open or read errors occur. */
  पूर्णांक charnew, charold; /* Current and previous char read. */
  खाता *fileptr;	/* Open for reading current file. */

  क्रम (; nfiles; nfiles--, fnamptr++)
    {
      पूर्णांक saved_errno;
      bool is_stdin = streq (*fnamptr, "-");
      अगर (is_stdin)
        {
          have_read_stdin = true;
          fileptr = मानक_निवेश;
        }
      अन्यथा
        {
          fileptr = ख_खोलो (*fnamptr, "r");
          अगर (fileptr == शून्य)
            {
              error (0, त्रुटि_सं, "%s", quotef (*fnamptr));
              ok = false;
              जारी;
            }
          fadvise (fileptr, FADVISE_SEQUENTIAL);
        }

      idx_t delimidx = 0;      /* Current delimiter index.  */
      idx_t delimoff = 0;      /* Current offset into delims.  */

      charold = अ_लो (fileptr);
      saved_errno = त्रुटि_सं;
      अगर (charold != खातापूर्ण)
        {
          /* 'charold' is set up.  Hit it!
             Keep reading characters, stashing them in 'charnew';
             output 'charold', converting to the appropriate delimiter
             character if needed.  After the EOF, output 'charold'
             if it's a newline; otherwise, output it and then a newline. */

          जबतक ((charnew = अ_लो (fileptr)) != खातापूर्ण)
            {
              /* Process the old character. */
              अगर (charold == पंक्ति_delim)
                {
                  output_delim (delims + delimoff, delim_lens[delimidx]);
                  delimoff += delim_lens[delimidx];
                  अगर (++delimidx == अंक_delims)
                    {
                      delimidx = 0;
                      delimoff = 0;
                    }
                }
              अन्यथा
                xputchar (charold);

              charold = charnew;
            }
          saved_errno = त्रुटि_सं;

          /* Hit EOF.  Process that last character. */
          xputchar (charold);
        }

      अगर (charold != पंक्ति_delim)
        xputchar (पंक्ति_delim);

      अगर (!ख_त्रुटि (fileptr))
        saved_errno = 0;
      अगर (is_stdin)
        साफ_त्रुटि (fileptr);	/* Also clear EOF. */
      अन्यथा अगर (ख_बंद (fileptr) != 0 && !saved_errno)
        saved_errno = त्रुटि_सं;
      अगर (saved_errno)
        {
          error (0, saved_errno, "%s", quotef (*fnamptr));
          ok = false;
        }
    }
  वापस ok;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write पंक्तियाँ consisting of corresponding पंक्तियाँ from each file joined by TABs\n\
to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -d, --delimiters=LIST\n\
         reuse characters from LIST instead of TABs;\n\
         backslash escapes are supported\n\
"));
      oputs (_("\
  -s, --serial\n\
         paste one file at a समय instead of in parallel; the newline of\n\
         every पंक्ति except the last पंक्ति in each file is replaced with a TAB\
\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      /* FIXME: add a couple of examples.  */
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  अक्षर स्थिर *delim_arg = "\t";

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "d:sz", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'd':
          /* Delimiter character(s). */
          delim_arg = (optarg[0] == '\0' ? "\\0" : optarg);
          अवरोध;

        हाल 's':
          serial_merge = true;
          अवरोध;

        हाल 'z':
          पंक्ति_delim = '\0';
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  पूर्णांक nfiles = argc - optind;
  अगर (nfiles == 0)
    {
      argv[optind] = bad_cast ("-");
      nfiles++;
    }

  अगर (collapse_escapes (delim_arg))
    {
      /* Don't use the quote() quoting style, because that would double the
         number of displayed backslashes, making the diagnostic look bogus.  */
      error (निकास_त्रुटि, 0,
             _("delimiter list ends with an unescaped backslash: %s"),
             quotearg_n_style_colon (0, c_maybe_quoting_style, delim_arg));
    }

  bool ok = ((serial_merge ? paste_serial : paste_parallel)
             (nfiles, &argv[optind]));

  मुक्त (delims);
  मुक्त (delim_lens);

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");
  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
