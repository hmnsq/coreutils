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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "fadvise.h"
#ضمن "ioblksize.h"
#ضمن "mcel.h"
#ضمن "mbbuf.h"
#ضمن "xdectoint.h"

#تعریف_کړه TAB_پلنوالی 8

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "fold"

#تعریف_کړه AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساكن bool break_spaces;

/* Mode to operate in.  */
ساكن تعداد
  {
    شمېر_COLUMNS,
    شمېر_بایټونه,
    شمېر_CHARACTERS
  } counting_ډېرپېښ = شمېر_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ساكن bool have_read_stdin;

ساكن حرف ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_دلیل, عدم, 'b'},
  {"characters", no_دلیل, عدم, 'c'},
  {"spaces", no_دلیل, عدم, 's'},
  {"width", required_دلیل, عدم, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروګرام_name);
      fputs (_("\
Wrap ننوت کرښې دننه each FILE, writing منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بایټونه\n\
         شمېر بایټونه rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         شمېر characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or دننه ټکي greater than پلنوالی_2\n\
"));
      oputs (_("\
  -w, --پلنوالی=پلنوالی_2\n\
         use پلنوالی_2 columns instead د 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساكن size_t
adjust_ستنه (size_t ستنه, mcel_t g)
{
  إذا (counting_ډېرپېښ != شمېر_بایټونه)
    {
      إذا (g.ch == '\b')
        ستنه -= !!ستنه;
      وإلا إذا (g.ch == '\r')
        ستنه = 0;
      وإلا إذا (g.ch == '\t')
        ستنه += TAB_پلنوالی - ستنه % TAB_پلنوالی;
      وإلا
        {
          صحيح پلنوالی = (counting_ډېرپېښ == شمېر_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          ستنه += پلنوالی < 0 ? 1 : پلنوالی;
        }
    }
  وإلا
    ستنه += g.len;
  أرجع ستنه;
}

ساكن فراغ
write_بهر (حرف ثابت *کرښه, size_t کرښه_len, bool newline)
{
  إذا (fwrite (کرښه, حجم (حرف), کرښه_len, stdout) != کرښه_len
      || (newline && توری_چاپ_کړه ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساكن bool
fold_file (حرف ثابت *filename, size_t پلنوالی)
{
  FILE *istream;
  size_t ستنه = 0;		/* Screen column where next char will go. */
  idx_t دننه_واټن_بهر = 0;		/* Index in 'line_out' for next char. */
  ساكن حرف کرښه_بهر[IO_BUFSIZE];
  ساكن حرف کرښه_دننه[IO_BUFSIZE];
  mbbuf_t mbbuf;
  صحيح saved_errno;

  إذا (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  وإلا
    istream = fopen (filename, "r");

  إذا (istream == عدم)
    {
      error (0, errno, "%s", quotef (filename));
      أرجع false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, کرښه_دننه, حجم کرښه_دننه, istream);

  mcel_t g;
  طالما ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      إذا (g.ch == '\n')
        {
          write_بهر (کرښه_بهر, دننه_واټن_بهر, /*newline=*/ true);
          ستنه = دننه_واټن_بهر = 0;
          استمر;
        }
    rescan:
      ستنه = adjust_ستنه (ستنه, g);

      إذا (ستنه > پلنوالی)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          إذا (break_spaces)
            {
              صحيح space_اوږدتیا = 0;
              idx_t logical_پای_ټکی = دننه_واټن_بهر;
              حرف *logical_p = کرښه_بهر;
              حرف *logical_lim = logical_p + logical_پای_ټکی;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_لټون (logical_p, logical_lim);
                  إذا (c32issep (g2.ch))
                    {
                      space_اوږدتیا = g2.len;
                      logical_پای_ټکی = logical_p - کرښه_بهر;
                    }
                }

              إذا (space_اوږدتیا)
                {
                  logical_پای_ټکی += space_اوږدتیا;
                  /* Found a blank.  Don't output the part after it. */
                  write_بهر (کرښه_بهر, logical_پای_ټکی, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (کرښه_بهر, کرښه_بهر + logical_پای_ټکی,
                           دننه_واټن_بهر - logical_پای_ټکی);
                  دننه_واټن_بهر -= logical_پای_ټکی;
                  ستنه = 0;
                  حرف *printed_p = کرښه_بهر;
                  حرف *printed_lim = printed_p + دننه_واټن_بهر;
                  لكل (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_لټون (printed_p, printed_lim);
                      ستنه = adjust_ستنه (ستنه, g2);
                    }
                  اذهب rescan;
                }
            }

          إذا (دننه_واټن_بهر == 0)
            {
              حافظه_نقل (کرښه_بهر, mbbuf_char_دننه_واټن (&mbbuf, g), g.len);
              دننه_واټن_بهر += g.len;
              استمر;
            }

          write_بهر (کرښه_بهر, دننه_واټن_بهر, /*newline=*/ true);
          ستنه = دننه_واټن_بهر = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      إذا (حجم کرښه_بهر <= دننه_واټن_بهر + g.len)
        {
          write_بهر (کرښه_بهر, دننه_واټن_بهر, /*newline=*/ false);
          دننه_واټن_بهر = 0;
        }

      حافظه_نقل (کرښه_بهر + دننه_واټن_بهر, mbbuf_char_دننه_واټن (&mbbuf, g), g.len);
      دننه_واټن_بهر += g.len;
    }

  saved_errno = errno;
  إذا (!ferror (istream))
    saved_errno = 0;

  إذا (دننه_واټن_بهر)
    write_بهر (کرښه_بهر, دننه_واټن_بهر, /*newline=*/ false);

  إذا (streq (filename, "-"))
    clearerr (istream);
  وإلا إذا (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  إذا (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      أرجع false;
    }

  أرجع true;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  size_t پلنوالی = 80;
  صحيح optc;
  bool ok;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, shortopts, longopts, عدم)) != -1)
    {
      حرف optargbuf[2];

      بدل (optc)
        {
        حالة 'b':		/* Count bytes rather than columns. */
          counting_ډېرپېښ = شمېر_بایټونه;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_ډېرپېښ = شمېر_CHARACTERS;
          اكسر;

        حالة 's':		/* Break at word boundaries. */
          break_spaces = true;
          اكسر;

        حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
        حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
          إذا (optarg)
            optarg--;
          وإلا
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        حالة 'w':		/* Line width. */
          پلنوالی = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_پلنوالی - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    ok = fold_file ("-", پلنوالی);
  وإلا
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], پلنوالی);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
