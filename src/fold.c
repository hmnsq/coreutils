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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "mbbuf.h"
#अन्तर्भाव "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
स्थिर bool break_spaces;

/* Mode to operate in.  */
स्थिर गणना
  {
    गन्ती_COLUMNS,
    गन्ती_BYTES,
    गन्ती_CHARACTERS
  } counting_mode = गन्ती_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
स्थिर bool have_read_stdin;

स्थिर वर्ण अचर shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

स्थिर संरचना option अचर longopts[] =
{
  {"bytes", no_argument, रिक्त, 'b'},
  {"characters", no_argument, रिक्त, 'c'},
  {"spaces", no_argument, रिक्त, 's'},
  {"width", required_argument, रिक्त, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Wrap आगत पङ्क्तिहरू in each खाता, writing to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         गन्ती bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         गन्ती characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         रोक after blanks, or in शब्दहरू greater than WIDTH\n\
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

स्थिर माप_प्रकार
adjust_column (माप_प्रकार column, mcel_t g)
{
  यदि (counting_mode != गन्ती_BYTES)
    {
      यदि (g.ch == '\b')
        column -= !!column;
      अन्यथा यदि (g.ch == '\r')
        column = 0;
      अन्यथा यदि (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      अन्यथा
        {
          पूर्णाङ्क width = (counting_mode == गन्ती_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  अन्यथा
    column += g.len;
  फर्काऊ column;
}

स्थिर शून्य
write_out (वर्ण अचर *पङ्क्ति, माप_प्रकार पङ्क्ति_len, bool newline)
{
  यदि (ख_डालो (पङ्क्ति, परिमाण (वर्ण), पङ्क्ति_len, मानक_निकास) != पङ्क्ति_len
      || (newline && अक्षर_दो ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

स्थिर bool
fold_file (वर्ण अचर *filename, माप_प्रकार width)
{
  खाता *istream;
  माप_प्रकार column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  स्थिर वर्ण पङ्क्ति_out[IO_BUFSIZE];
  स्थिर वर्ण पङ्क्ति_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  पूर्णाङ्क saved_errno;

  यदि (streq (filename, "-"))
    {
      istream = मानक_निवेश;
      have_read_stdin = true;
    }
  अन्यथा
    istream = ख_खोलो (filename, "r");

  यदि (istream == रिक्त)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      फर्काऊ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, पङ्क्ति_in, परिमाण पङ्क्ति_in, istream);

  mcel_t g;
  जबसम्म ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      यदि (g.ch == '\n')
        {
          write_out (पङ्क्ति_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          जारी;
        }
    rescan:
      column = adjust_column (column, g);

      यदि (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          यदि (break_spaces)
            {
              पूर्णाङ्क space_length = 0;
              idx_t logical_end = offset_out;
              वर्ण *logical_p = पङ्क्ति_out;
              वर्ण *logical_lim = logical_p + logical_end;

              लागि (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  यदि (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - पङ्क्ति_out;
                    }
                }

              यदि (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (पङ्क्ति_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  स_हटाओ (पङ्क्ति_out, पङ्क्ति_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  वर्ण *printed_p = पङ्क्ति_out;
                  वर्ण *printed_lim = printed_p + offset_out;
                  लागि (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  गच्छ rescan;
                }
            }

          यदि (offset_out == 0)
            {
              स_नकल (पङ्क्ति_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              जारी;
            }

          write_out (पङ्क्ति_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          गच्छ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      यदि (परिमाण पङ्क्ति_out <= offset_out + g.len)
        {
          write_out (पङ्क्ति_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      स_नकल (पङ्क्ति_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = त्रुटि_सं;
  यदि (!ख_त्रुटि (istream))
    saved_errno = 0;

  यदि (offset_out)
    write_out (पङ्क्ति_out, offset_out, /*newline=*/ false);

  यदि (streq (filename, "-"))
    साफ_त्रुटि (istream);
  अन्यथा यदि (ख_बंद (istream) != 0 && !saved_errno)
    saved_errno = त्रुटि_सं;

  यदि (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      फर्काऊ false;
    }

  फर्काऊ true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  माप_प्रकार width = 80;
  पूर्णाङ्क optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म ((optc = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      वर्ण optargbuf[2];

      विकल्प (optc)
        {
        पक्ष 'b':		/* Count bytes rather than columns. */
          counting_mode = गन्ती_BYTES;
          रोक;

        पक्ष 'c':               /* Count characters rather than columns. */
          counting_mode = गन्ती_CHARACTERS;
          रोक;

        पक्ष 's':		/* Break at word boundaries. */
          break_spaces = true;
          रोक;

        पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
        पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
          यदि (optarg)
            optarg--;
          अन्यथा
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        पक्ष 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          रोक;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (argc == optind)
    ok = fold_file ("-", width);
  अन्यथा
    {
      ok = true;
      लागि (पूर्णाङ्क i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  यदि (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  फर्काऊ ok ? निकास_सफल : निकास_त्रुटि;
}
