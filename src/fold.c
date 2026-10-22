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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "fadvise.h"
#شامل "ioblksize.h"
#شامل "mcel.h"
#شامل "mbbuf.h"
#شامل "xdectoint.h"

#تعریف_کرو TAB_چوڑائی 8

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "fold"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساکن bool break_spaces;

/* Mode to operate in.  */
ساکن شمار
  {
    گنتی_COLUMNS,
    گنتی_بائٹیں,
    گنتی_CHARACTERS
  } counting_منوال = گنتی_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ساکن bool have_read_stdin;

ساکن حرف ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ساکن ساخت option ثابت longopts[] =
{
  {"bytes", no_دلیل, تہی, 'b'},
  {"characters", no_دلیل, تہی, 'c'},
  {"spaces", no_دلیل, تہی, 's'},
  {"width", required_دلیل, تہی, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Wrap داخلہ سطریں اندر each FILE, writing مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائٹیں\n\
         گنتی بائٹیں rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         گنتی characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         توڑو after blanks, or اندر الفاظ greater than چوڑائی_2\n\
"));
      oputs (_("\
  -w, --چوڑائی=چوڑائی_2\n\
         use چوڑائی_2 columns instead کا 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساکن size_t
adjust_عمود (size_t عمود, mcel_t g)
{
  اگر (counting_منوال != گنتی_بائٹیں)
    {
      اگر (g.ch == '\b')
        عمود -= !!عمود;
      ورنہ اگر (g.ch == '\r')
        عمود = 0;
      ورنہ اگر (g.ch == '\t')
        عمود += TAB_چوڑائی - عمود % TAB_چوڑائی;
      ورنہ
        {
          صحیح چوڑائی = (counting_منوال == گنتی_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          عمود += چوڑائی < 0 ? 1 : چوڑائی;
        }
    }
  ورنہ
    عمود += g.len;
  واپس عمود;
}

ساکن خالی
write_برآمد (حرف ثابت *سطر, size_t سطر_len, bool newline)
{
  اگر (fwrite (سطر, حجم (حرف), سطر_len, stdout) != سطر_len
      || (newline && حرف_لکھو ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساکن bool
fold_file (حرف ثابت *filename, size_t چوڑائی)
{
  FILE *istream;
  size_t عمود = 0;		/* Screen column where next char will go. */
  idx_t انحراف_برآمد = 0;		/* Index in 'line_out' for next char. */
  ساکن حرف سطر_برآمد[IO_BUFSIZE];
  ساکن حرف سطر_اندر[IO_BUFSIZE];
  mbbuf_t mbbuf;
  صحیح saved_errno;

  اگر (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ورنہ
    istream = fopen (filename, "r");

  اگر (istream == تہی)
    {
      error (0, errno, "%s", quotef (filename));
      واپس false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, سطر_اندر, حجم سطر_اندر, istream);

  mcel_t g;
  جب ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      اگر (g.ch == '\n')
        {
          write_برآمد (سطر_برآمد, انحراف_برآمد, /*newline=*/ true);
          عمود = انحراف_برآمد = 0;
          جاری;
        }
    rescan:
      عمود = adjust_عمود (عمود, g);

      اگر (عمود > چوڑائی)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          اگر (break_spaces)
            {
              صحیح space_لمبائی = 0;
              idx_t logical_انتہا = انحراف_برآمد;
              حرف *logical_p = سطر_برآمد;
              حرف *logical_lim = logical_p + logical_انتہا;

              برائے (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_جائزہ (logical_p, logical_lim);
                  اگر (c32issep (g2.ch))
                    {
                      space_لمبائی = g2.len;
                      logical_انتہا = logical_p - سطر_برآمد;
                    }
                }

              اگر (space_لمبائی)
                {
                  logical_انتہا += space_لمبائی;
                  /* Found a blank.  Don't output the part after it. */
                  write_برآمد (سطر_برآمد, logical_انتہا, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (سطر_برآمد, سطر_برآمد + logical_انتہا,
                           انحراف_برآمد - logical_انتہا);
                  انحراف_برآمد -= logical_انتہا;
                  عمود = 0;
                  حرف *printed_p = سطر_برآمد;
                  حرف *printed_lim = printed_p + انحراف_برآمد;
                  برائے (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_جائزہ (printed_p, printed_lim);
                      عمود = adjust_عمود (عمود, g2);
                    }
                  جاؤ rescan;
                }
            }

          اگر (انحراف_برآمد == 0)
            {
              حافظہ_نقل (سطر_برآمد, mbbuf_char_انحراف (&mbbuf, g), g.len);
              انحراف_برآمد += g.len;
              جاری;
            }

          write_برآمد (سطر_برآمد, انحراف_برآمد, /*newline=*/ true);
          عمود = انحراف_برآمد = 0;
          جاؤ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      اگر (حجم سطر_برآمد <= انحراف_برآمد + g.len)
        {
          write_برآمد (سطر_برآمد, انحراف_برآمد, /*newline=*/ false);
          انحراف_برآمد = 0;
        }

      حافظہ_نقل (سطر_برآمد + انحراف_برآمد, mbbuf_char_انحراف (&mbbuf, g), g.len);
      انحراف_برآمد += g.len;
    }

  saved_errno = errno;
  اگر (!ferror (istream))
    saved_errno = 0;

  اگر (انحراف_برآمد)
    write_برآمد (سطر_برآمد, انحراف_برآمد, /*newline=*/ false);

  اگر (streq (filename, "-"))
    clearerr (istream);
  ورنہ اگر (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  اگر (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      واپس false;
    }

  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  size_t چوڑائی = 80;
  صحیح optc;
  bool ok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, shortopts, longopts, تہی)) != -1)
    {
      حرف optargbuf[2];

      انتخاب (optc)
        {
        حالت 'b':		/* Count bytes rather than columns. */
          counting_منوال = گنتی_بائٹیں;
          توڑو;

        حالت 'c':               /* Count characters rather than columns. */
          counting_منوال = گنتی_CHARACTERS;
          توڑو;

        حالت 's':		/* Break at word boundaries. */
          break_spaces = true;
          توڑو;

        حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
        حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
          اگر (optarg)
            optarg--;
          ورنہ
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        حالت 'w':		/* Line width. */
          چوڑائی = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_چوڑائی - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc == optind)
    ok = fold_file ("-", چوڑائی);
  ورنہ
    {
      ok = true;
      برائے (صحیح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], چوڑائی);
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
