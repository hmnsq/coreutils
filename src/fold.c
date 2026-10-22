/* fold -- wrap each input line to fit in specified width.
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie, djm@gnu.ai.mit.edu. */

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "fadvise.h"
#समावेश "ioblksize.h"
#समावेश "mcel.h"
#समावेश "mbbuf.h"
#समावेश "xdectoint.h"

#घोषणा TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "fold"

#घोषणा AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
अटल bool break_spaces;

/* Mode to operate in.  */
अटल क्रमागत
  {
    गिनती_COLUMNS,
    गिनती_BYTES,
    गिनती_CHARACTERS
  } counting_mode = गिनती_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
अटल bool have_read_stdin;

अटल अक्षर स्थिर shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

अटल काष्ठा option स्थिर longopts[] =
{
  {"bytes", no_argument, शून्य, 'b'},
  {"characters", no_argument, शून्य, 'c'},
  {"spaces", no_argument, शून्य, 's'},
  {"width", required_argument, शून्य, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

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
Wrap निवेश पंक्तियाँ in each खाता, writing to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         गिनती bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         गिनती characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         अवरोध after blanks, or in शब्दगण greater than WIDTH\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         use WIDTH columns instead of 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

अटल माप_प्रकार
adjust_column (माप_प्रकार column, mcel_t g)
{
  अगर (counting_mode != गिनती_BYTES)
    {
      अगर (g.ch == '\b')
        column -= !!column;
      अन्यथा अगर (g.ch == '\r')
        column = 0;
      अन्यथा अगर (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      अन्यथा
        {
          पूर्णांक width = (counting_mode == गिनती_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  अन्यथा
    column += g.len;
  वापस column;
}

अटल व्योम
write_out (अक्षर स्थिर *पंक्ति, माप_प्रकार पंक्ति_len, bool newline)
{
  अगर (ख_डालो (पंक्ति, माप (अक्षर), पंक्ति_len, मानक_निकास) != पंक्ति_len
      || (newline && अक्षर_दो ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

अटल bool
fold_file (अक्षर स्थिर *filename, माप_प्रकार width)
{
  खाता *istream;
  माप_प्रकार column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  अटल अक्षर पंक्ति_out[IO_BUFSIZE];
  अटल अक्षर पंक्ति_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  पूर्णांक saved_errno;

  अगर (streq (filename, "-"))
    {
      istream = मानक_निवेश;
      have_read_stdin = true;
    }
  अन्यथा
    istream = ख_खोलो (filename, "r");

  अगर (istream == शून्य)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      वापस false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, पंक्ति_in, माप पंक्ति_in, istream);

  mcel_t g;
  जबतक ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      अगर (g.ch == '\n')
        {
          write_out (पंक्ति_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          जारी;
        }
    rescan:
      column = adjust_column (column, g);

      अगर (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          अगर (break_spaces)
            {
              पूर्णांक space_लंबाई = 0;
              idx_t logical_end = offset_out;
              अक्षर *logical_p = पंक्ति_out;
              अक्षर *logical_lim = logical_p + logical_end;

              क्रम (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  अगर (c32issep (g2.ch))
                    {
                      space_लंबाई = g2.len;
                      logical_end = logical_p - पंक्ति_out;
                    }
                }

              अगर (space_लंबाई)
                {
                  logical_end += space_लंबाई;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (पंक्ति_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  स_हटाओ (पंक्ति_out, पंक्ति_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  अक्षर *printed_p = पंक्ति_out;
                  अक्षर *printed_lim = printed_p + offset_out;
                  क्रम (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  जाओ rescan;
                }
            }

          अगर (offset_out == 0)
            {
              स_नकल (पंक्ति_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              जारी;
            }

          write_out (पंक्ति_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          जाओ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      अगर (माप पंक्ति_out <= offset_out + g.len)
        {
          write_out (पंक्ति_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      स_नकल (पंक्ति_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = त्रुटि_सं;
  अगर (!ख_त्रुटि (istream))
    saved_errno = 0;

  अगर (offset_out)
    write_out (पंक्ति_out, offset_out, /*newline=*/ false);

  अगर (streq (filename, "-"))
    साफ_त्रुटि (istream);
  अन्यथा अगर (ख_बंद (istream) != 0 && !saved_errno)
    saved_errno = त्रुटि_सं;

  अगर (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      वापस false;
    }

  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  माप_प्रकार width = 80;
  पूर्णांक optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, shortopts, longopts, शून्य)) != -1)
    {
      अक्षर optargbuf[2];

      चयन (optc)
        {
        हाल 'b':		/* Count bytes rather than columns. */
          counting_mode = गिनती_BYTES;
          अवरोध;

        हाल 'c':               /* Count characters rather than columns. */
          counting_mode = गिनती_CHARACTERS;
          अवरोध;

        हाल 's':		/* Break at word boundaries. */
          break_spaces = true;
          अवरोध;

        हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
        हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
          अगर (optarg)
            optarg--;
          अन्यथा
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        हाल 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (argc == optind)
    ok = fold_file ("-", width);
  अन्यथा
    {
      ok = true;
      क्रम (पूर्णांक i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
