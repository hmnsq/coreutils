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

#परिभाषय TAB_विस्तारः 8

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "fold"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
स्थिर bool break_spaces;

/* Mode to operate in.  */
स्थिर गणना
  {
    गणनम्_COLUMNS,
    गणनम्_अष्टकानि,
    गणनम्_CHARACTERS
  } counting_बहुलकम् = गणनम्_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
स्थिर bool have_read_stdin;

स्थिर वर्ण ध्रुव shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

स्थिर संरचना option ध्रुव longopts[] =
{
  {"bytes", no_तर्कः, रिक्त, 'b'},
  {"characters", no_तर्कः, रिक्त, 'c'},
  {"spaces", no_तर्कः, रिक्त, 's'},
  {"width", required_तर्कः, रिक्त, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Wrap निवेशः पङ्क्तयः अन्तर् each FILE, writing गन्तव्यम् standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --अष्टकानि\n\
         गणनम् अष्टकानि rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         गणनम् characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         भङ्ग after blanks, or अन्तर् शब्दाः greater than विस्तारः_2\n\
"));
      oputs (_("\
  -w, --विस्तारः=विस्तारः_2\n\
         use विस्तारः_2 columns instead सम्बन्धि 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

स्थिर size_t
adjust_column (size_t column, mcel_t g)
{
  यदि (counting_बहुलकम् != गणनम्_अष्टकानि)
    {
      यदि (g.ch == '\b')
        column -= !!column;
      अन्यथा यदि (g.ch == '\r')
        column = 0;
      अन्यथा यदि (g.ch == '\t')
        column += TAB_विस्तारः - column % TAB_विस्तारः;
      अन्यथा
        {
          पूर्णाङ्क विस्तारः = (counting_बहुलकम् == गणनम्_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += विस्तारः < 0 ? 1 : विस्तारः;
        }
    }
  अन्यथा
    column += g.len;
  प्रत्यागम column;
}

स्थिर शून्य
write_बहिर्गमनम् (वर्ण ध्रुव *पङ्क्तिः, size_t पङ्क्तिः_len, bool newline)
{
  यदि (fwrite (पङ्क्तिः, परिमाण (वर्ण), पङ्क्तिः_len, stdout) != पङ्क्तिः_len
      || (newline && अक्षरं_लिख ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

स्थिर bool
fold_file (वर्ण ध्रुव *filename, size_t विस्तारः)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t विस्थापनम्_बहिर्गमनम् = 0;		/* Index in 'line_out' for next char. */
  स्थिर वर्ण पङ्क्तिः_बहिर्गमनम्[IO_BUFSIZE];
  स्थिर वर्ण पङ्क्तिः_अन्तर्[IO_BUFSIZE];
  mbbuf_t mbbuf;
  पूर्णाङ्क saved_errno;

  यदि (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  अन्यथा
    istream = fopen (filename, "r");

  यदि (istream == रिक्त)
    {
      error (0, errno, "%s", quotef (filename));
      प्रत्यागम false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, पङ्क्तिः_अन्तर्, परिमाण पङ्क्तिः_अन्तर्, istream);

  mcel_t g;
  यावत् ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      यदि (g.ch == '\n')
        {
          write_बहिर्गमनम् (पङ्क्तिः_बहिर्गमनम्, विस्थापनम्_बहिर्गमनम्, /*newline=*/ true);
          column = विस्थापनम्_बहिर्गमनम् = 0;
          अनुवर्त;
        }
    rescan:
      column = adjust_column (column, g);

      यदि (column > विस्तारः)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          यदि (break_spaces)
            {
              पूर्णाङ्क space_दीर्घता = 0;
              idx_t logical_अन्तः = विस्थापनम्_बहिर्गमनम्;
              वर्ण *logical_p = पङ्क्तिः_बहिर्गमनम्;
              वर्ण *logical_lim = logical_p + logical_अन्तः;

              प्रति (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_अवलोकनम् (logical_p, logical_lim);
                  यदि (c32issep (g2.ch))
                    {
                      space_दीर्घता = g2.len;
                      logical_अन्तः = logical_p - पङ्क्तिः_बहिर्गमनम्;
                    }
                }

              यदि (space_दीर्घता)
                {
                  logical_अन्तः += space_दीर्घता;
                  /* Found a blank.  Don't output the part after it. */
                  write_बहिर्गमनम् (पङ्क्तिः_बहिर्गमनम्, logical_अन्तः, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (पङ्क्तिः_बहिर्गमनम्, पङ्क्तिः_बहिर्गमनम् + logical_अन्तः,
                           विस्थापनम्_बहिर्गमनम् - logical_अन्तः);
                  विस्थापनम्_बहिर्गमनम् -= logical_अन्तः;
                  column = 0;
                  वर्ण *printed_p = पङ्क्तिः_बहिर्गमनम्;
                  वर्ण *printed_lim = printed_p + विस्थापनम्_बहिर्गमनम्;
                  प्रति (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_अवलोकनम् (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  गच्छ rescan;
                }
            }

          यदि (विस्थापनम्_बहिर्गमनम् == 0)
            {
              स्मृतिं_प्रतिलिख (पङ्क्तिः_बहिर्गमनम्, mbbuf_char_विस्थापनम् (&mbbuf, g), g.len);
              विस्थापनम्_बहिर्गमनम् += g.len;
              अनुवर्त;
            }

          write_बहिर्गमनम् (पङ्क्तिः_बहिर्गमनम्, विस्थापनम्_बहिर्गमनम्, /*newline=*/ true);
          column = विस्थापनम्_बहिर्गमनम् = 0;
          गच्छ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      यदि (परिमाण पङ्क्तिः_बहिर्गमनम् <= विस्थापनम्_बहिर्गमनम् + g.len)
        {
          write_बहिर्गमनम् (पङ्क्तिः_बहिर्गमनम्, विस्थापनम्_बहिर्गमनम्, /*newline=*/ false);
          विस्थापनम्_बहिर्गमनम् = 0;
        }

      स्मृतिं_प्रतिलिख (पङ्क्तिः_बहिर्गमनम् + विस्थापनम्_बहिर्गमनम्, mbbuf_char_विस्थापनम् (&mbbuf, g), g.len);
      विस्थापनम्_बहिर्गमनम् += g.len;
    }

  saved_errno = errno;
  यदि (!ferror (istream))
    saved_errno = 0;

  यदि (विस्थापनम्_बहिर्गमनम्)
    write_बहिर्गमनम् (पङ्क्तिः_बहिर्गमनम्, विस्थापनम्_बहिर्गमनम्, /*newline=*/ false);

  यदि (streq (filename, "-"))
    clearerr (istream);
  अन्यथा यदि (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  यदि (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      प्रत्यागम false;
    }

  प्रत्यागम true;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  size_t विस्तारः = 80;
  पूर्णाङ्क optc;
  bool ok;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      वर्ण optargbuf[2];

      विकल्प (optc)
        {
        पक्ष 'b':		/* Count bytes rather than columns. */
          counting_बहुलकम् = गणनम्_अष्टकानि;
          भङ्ग;

        पक्ष 'c':               /* Count characters rather than columns. */
          counting_बहुलकम् = गणनम्_CHARACTERS;
          भङ्ग;

        पक्ष 's':		/* Break at word boundaries. */
          break_spaces = true;
          भङ्ग;

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
          विस्तारः = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_विस्तारः - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (argc == optind)
    ok = fold_file ("-", विस्तारः);
  अन्यथा
    {
      ok = true;
      प्रति (पूर्णाङ्क i = optind; i < argc; i++)
        ok &= fold_file (argv[i], विस्तारः);
    }

  यदि (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
