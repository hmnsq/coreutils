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

#تعریف_کن TAB_عرض 8

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "fold"

#تعریف_کن AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساكن bool break_spaces;

/* Mode to operate in.  */
ساكن تعداد
  {
    شمار_COLUMNS,
    شمار_بایتها,
    شمار_CHARACTERS
  } counting_نما = شمار_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ساكن bool have_read_stdin;

ساكن حرف ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_آرگومان, عدم, 'b'},
  {"characters", no_آرگومان, عدم, 'c'},
  {"spaces", no_آرگومان, عدم, 's'},
  {"width", required_آرگومان, عدم, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              برنامه_name);
      fputs (_("\
Wrap درون_داد سطرها درون each FILE, writing مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بایتها\n\
         شمار بایتها rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         شمار characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or درون واژگان greater than عرض_2\n\
"));
      oputs (_("\
  -w, --عرض=عرض_2\n\
         use عرض_2 columns instead از 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساكن size_t
adjust_ستونی (size_t ستونی, mcel_t g)
{
  إذا (counting_نما != شمار_بایتها)
    {
      إذا (g.ch == '\b')
        ستونی -= !!ستونی;
      وإلا إذا (g.ch == '\r')
        ستونی = 0;
      وإلا إذا (g.ch == '\t')
        ستونی += TAB_عرض - ستونی % TAB_عرض;
      وإلا
        {
          صحيح عرض = (counting_نما == شمار_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          ستونی += عرض < 0 ? 1 : عرض;
        }
    }
  وإلا
    ستونی += g.len;
  أرجع ستونی;
}

ساكن فراغ
write_بیرون (حرف ثابت *سطر, size_t سطر_len, bool newline)
{
  إذا (fwrite (سطر, حجم (حرف), سطر_len, stdout) != سطر_len
      || (newline && حرف_چاپ_کن ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساكن bool
fold_file (حرف ثابت *filename, size_t عرض)
{
  FILE *istream;
  size_t ستونی = 0;		/* Screen column where next char will go. */
  idx_t جابه_جایی_بیرون = 0;		/* Index in 'line_out' for next char. */
  ساكن حرف سطر_بیرون[IO_BUFSIZE];
  ساكن حرف سطر_درون[IO_BUFSIZE];
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
  mbbuf_init (&mbbuf, سطر_درون, حجم سطر_درون, istream);

  mcel_t g;
  طالما ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      إذا (g.ch == '\n')
        {
          write_بیرون (سطر_بیرون, جابه_جایی_بیرون, /*newline=*/ true);
          ستونی = جابه_جایی_بیرون = 0;
          استمر;
        }
    rescan:
      ستونی = adjust_ستونی (ستونی, g);

      إذا (ستونی > عرض)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          إذا (break_spaces)
            {
              صحيح space_درازی = 0;
              idx_t logical_انتها = جابه_جایی_بیرون;
              حرف *logical_p = سطر_بیرون;
              حرف *logical_lim = logical_p + logical_انتها;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_پیمایش (logical_p, logical_lim);
                  إذا (c32issep (g2.ch))
                    {
                      space_درازی = g2.len;
                      logical_انتها = logical_p - سطر_بیرون;
                    }
                }

              إذا (space_درازی)
                {
                  logical_انتها += space_درازی;
                  /* Found a blank.  Don't output the part after it. */
                  write_بیرون (سطر_بیرون, logical_انتها, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (سطر_بیرون, سطر_بیرون + logical_انتها,
                           جابه_جایی_بیرون - logical_انتها);
                  جابه_جایی_بیرون -= logical_انتها;
                  ستونی = 0;
                  حرف *printed_p = سطر_بیرون;
                  حرف *printed_lim = printed_p + جابه_جایی_بیرون;
                  لكل (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_پیمایش (printed_p, printed_lim);
                      ستونی = adjust_ستونی (ستونی, g2);
                    }
                  اذهب rescan;
                }
            }

          إذا (جابه_جایی_بیرون == 0)
            {
              کپی_حافظه (سطر_بیرون, mbbuf_char_جابه_جایی (&mbbuf, g), g.len);
              جابه_جایی_بیرون += g.len;
              استمر;
            }

          write_بیرون (سطر_بیرون, جابه_جایی_بیرون, /*newline=*/ true);
          ستونی = جابه_جایی_بیرون = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      إذا (حجم سطر_بیرون <= جابه_جایی_بیرون + g.len)
        {
          write_بیرون (سطر_بیرون, جابه_جایی_بیرون, /*newline=*/ false);
          جابه_جایی_بیرون = 0;
        }

      کپی_حافظه (سطر_بیرون + جابه_جایی_بیرون, mbbuf_char_جابه_جایی (&mbbuf, g), g.len);
      جابه_جایی_بیرون += g.len;
    }

  saved_errno = errno;
  إذا (!ferror (istream))
    saved_errno = 0;

  إذا (جابه_جایی_بیرون)
    write_بیرون (سطر_بیرون, جابه_جایی_بیرون, /*newline=*/ false);

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
اصلی (صحيح argc, حرف **argv)
{
  size_t عرض = 80;
  صحيح optc;
  bool ok;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          counting_نما = شمار_بایتها;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_نما = شمار_CHARACTERS;
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
          عرض = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_عرض - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    ok = fold_file ("-", عرض);
  وإلا
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], عرض);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
