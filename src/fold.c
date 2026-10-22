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

#تعریف_کرو TAB_چوڑائی 8

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "fold"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساكن bool break_spaces;

/* Mode to operate in.  */
ساكن تعداد
  {
    گنتی_COLUMNS,
    گنتی_بائٹاں,
    گنتی_CHARACTERS
  } counting_منوال = گنتی_COLUMNS;

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
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Wrap داخلہ سطریں وچ each FILE, writing منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائٹاں\n\
         گنتی بائٹاں rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         گنتی characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or وچ الفاظ greater than چوڑائی_2\n\
"));
      oputs (_("\
  -w, --چوڑائی=چوڑائی_2\n\
         use چوڑائی_2 columns instead دا 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساكن size_t
adjust_کالم (size_t کالم, mcel_t g)
{
  إذا (counting_منوال != گنتی_بائٹاں)
    {
      إذا (g.ch == '\b')
        کالم -= !!کالم;
      وإلا إذا (g.ch == '\r')
        کالم = 0;
      وإلا إذا (g.ch == '\t')
        کالم += TAB_چوڑائی - کالم % TAB_چوڑائی;
      وإلا
        {
          صحيح چوڑائی = (counting_منوال == گنتی_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          کالم += چوڑائی < 0 ? 1 : چوڑائی;
        }
    }
  وإلا
    کالم += g.len;
  أرجع کالم;
}

ساكن فراغ
write_باہرلا (حرف ثابت *سطر, size_t سطر_len, bool newline)
{
  إذا (fwrite (سطر, حجم (حرف), سطر_len, stdout) != سطر_len
      || (newline && حرف_لکھو ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساكن bool
fold_file (حرف ثابت *filename, size_t چوڑائی)
{
  FILE *istream;
  size_t کالم = 0;		/* Screen column where next char will go. */
  idx_t وتھ_باہرلا = 0;		/* Index in 'line_out' for next char. */
  ساكن حرف سطر_باہرلا[IO_BUFSIZE];
  ساكن حرف سطر_وچ[IO_BUFSIZE];
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
  mbbuf_init (&mbbuf, سطر_وچ, حجم سطر_وچ, istream);

  mcel_t g;
  طالما ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      إذا (g.ch == '\n')
        {
          write_باہرلا (سطر_باہرلا, وتھ_باہرلا, /*newline=*/ true);
          کالم = وتھ_باہرلا = 0;
          استمر;
        }
    rescan:
      کالم = adjust_کالم (کالم, g);

      إذا (کالم > چوڑائی)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          إذا (break_spaces)
            {
              صحيح space_لمائی = 0;
              idx_t logical_سرا = وتھ_باہرلا;
              حرف *logical_p = سطر_باہرلا;
              حرف *logical_lim = logical_p + logical_سرا;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_جانچ (logical_p, logical_lim);
                  إذا (c32issep (g2.ch))
                    {
                      space_لمائی = g2.len;
                      logical_سرا = logical_p - سطر_باہرلا;
                    }
                }

              إذا (space_لمائی)
                {
                  logical_سرا += space_لمائی;
                  /* Found a blank.  Don't output the part after it. */
                  write_باہرلا (سطر_باہرلا, logical_سرا, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (سطر_باہرلا, سطر_باہرلا + logical_سرا,
                           وتھ_باہرلا - logical_سرا);
                  وتھ_باہرلا -= logical_سرا;
                  کالم = 0;
                  حرف *printed_p = سطر_باہرلا;
                  حرف *printed_lim = printed_p + وتھ_باہرلا;
                  لكل (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_جانچ (printed_p, printed_lim);
                      کالم = adjust_کالم (کالم, g2);
                    }
                  اذهب rescan;
                }
            }

          إذا (وتھ_باہرلا == 0)
            {
              حافظہ_نقل (سطر_باہرلا, mbbuf_char_وتھ (&mbbuf, g), g.len);
              وتھ_باہرلا += g.len;
              استمر;
            }

          write_باہرلا (سطر_باہرلا, وتھ_باہرلا, /*newline=*/ true);
          کالم = وتھ_باہرلا = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      إذا (حجم سطر_باہرلا <= وتھ_باہرلا + g.len)
        {
          write_باہرلا (سطر_باہرلا, وتھ_باہرلا, /*newline=*/ false);
          وتھ_باہرلا = 0;
        }

      حافظہ_نقل (سطر_باہرلا + وتھ_باہرلا, mbbuf_char_وتھ (&mbbuf, g), g.len);
      وتھ_باہرلا += g.len;
    }

  saved_errno = errno;
  إذا (!ferror (istream))
    saved_errno = 0;

  إذا (وتھ_باہرلا)
    write_باہرلا (سطر_باہرلا, وتھ_باہرلا, /*newline=*/ false);

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
مکھ (صحيح argc, حرف **argv)
{
  size_t چوڑائی = 80;
  صحيح optc;
  bool ok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          counting_منوال = گنتی_بائٹاں;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_منوال = گنتی_CHARACTERS;
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
          چوڑائی = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_چوڑائی - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    ok = fold_file ("-", چوڑائی);
  وإلا
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], چوڑائی);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
