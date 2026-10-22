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

#وصف_ڪريو TAB_ويڪر 8

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "fold"

#وصف_ڪريو AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساكن bool break_spaces;

/* Mode to operate in.  */
ساكن تعداد
  {
    ڳڻپ_COLUMNS,
    ڳڻپ_بائيٽون,
    ڳڻپ_CHARACTERS
  } counting_گهڻائي = ڳڻپ_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ساكن bool have_read_stdin;

ساكن حرف ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_دليل, عدم, 'b'},
  {"characters", no_دليل, عدم, 'c'},
  {"spaces", no_دليل, عدم, 's'},
  {"width", required_دليل, عدم, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Wrap داخلا سٽون اندريون each FILE, writing منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --بائيٽون\n\
         ڳڻپ بائيٽون rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ڳڻپ characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or اندريون لفظ_4 greater than ويڪر_2\n\
"));
      oputs (_("\
  -w, --ويڪر=ويڪر_2\n\
         use ويڪر_2 columns instead جو 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساكن size_t
adjust_ڪالم (size_t ڪالم, mcel_t g)
{
  إذا (counting_گهڻائي != ڳڻپ_بائيٽون)
    {
      إذا (g.ch == '\b')
        ڪالم -= !!ڪالم;
      وإلا إذا (g.ch == '\r')
        ڪالم = 0;
      وإلا إذا (g.ch == '\t')
        ڪالم += TAB_ويڪر - ڪالم % TAB_ويڪر;
      وإلا
        {
          صحيح ويڪر = (counting_گهڻائي == ڳڻپ_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          ڪالم += ويڪر < 0 ? 1 : ويڪر;
        }
    }
  وإلا
    ڪالم += g.len;
  أرجع ڪالم;
}

ساكن فراغ
write_ٻاهريون (حرف ثابت *سٽ, size_t سٽ_len, bool newline)
{
  إذا (fwrite (سٽ, حجم (حرف), سٽ_len, stdout) != سٽ_len
      || (newline && اکر_لکو ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساكن bool
fold_file (حرف ثابت *filename, size_t ويڪر)
{
  FILE *istream;
  size_t ڪالم = 0;		/* Screen column where next char will go. */
  idx_t وٿي_ٻاهريون = 0;		/* Index in 'line_out' for next char. */
  ساكن حرف سٽ_ٻاهريون[IO_BUFSIZE];
  ساكن حرف سٽ_اندريون[IO_BUFSIZE];
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
  mbbuf_init (&mbbuf, سٽ_اندريون, حجم سٽ_اندريون, istream);

  mcel_t g;
  طالما ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      إذا (g.ch == '\n')
        {
          write_ٻاهريون (سٽ_ٻاهريون, وٿي_ٻاهريون, /*newline=*/ true);
          ڪالم = وٿي_ٻاهريون = 0;
          استمر;
        }
    rescan:
      ڪالم = adjust_ڪالم (ڪالم, g);

      إذا (ڪالم > ويڪر)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          إذا (break_spaces)
            {
              صحيح space_ڊيگهائي = 0;
              idx_t logical_پڇاڙي = وٿي_ٻاهريون;
              حرف *logical_p = سٽ_ٻاهريون;
              حرف *logical_lim = logical_p + logical_پڇاڙي;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_جاچ (logical_p, logical_lim);
                  إذا (c32issep (g2.ch))
                    {
                      space_ڊيگهائي = g2.len;
                      logical_پڇاڙي = logical_p - سٽ_ٻاهريون;
                    }
                }

              إذا (space_ڊيگهائي)
                {
                  logical_پڇاڙي += space_ڊيگهائي;
                  /* Found a blank.  Don't output the part after it. */
                  write_ٻاهريون (سٽ_ٻاهريون, logical_پڇاڙي, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (سٽ_ٻاهريون, سٽ_ٻاهريون + logical_پڇاڙي,
                           وٿي_ٻاهريون - logical_پڇاڙي);
                  وٿي_ٻاهريون -= logical_پڇاڙي;
                  ڪالم = 0;
                  حرف *printed_p = سٽ_ٻاهريون;
                  حرف *printed_lim = printed_p + وٿي_ٻاهريون;
                  لكل (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_جاچ (printed_p, printed_lim);
                      ڪالم = adjust_ڪالم (ڪالم, g2);
                    }
                  اذهب rescan;
                }
            }

          إذا (وٿي_ٻاهريون == 0)
            {
              يادگيري_نقل (سٽ_ٻاهريون, mbbuf_char_وٿي (&mbbuf, g), g.len);
              وٿي_ٻاهريون += g.len;
              استمر;
            }

          write_ٻاهريون (سٽ_ٻاهريون, وٿي_ٻاهريون, /*newline=*/ true);
          ڪالم = وٿي_ٻاهريون = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      إذا (حجم سٽ_ٻاهريون <= وٿي_ٻاهريون + g.len)
        {
          write_ٻاهريون (سٽ_ٻاهريون, وٿي_ٻاهريون, /*newline=*/ false);
          وٿي_ٻاهريون = 0;
        }

      يادگيري_نقل (سٽ_ٻاهريون + وٿي_ٻاهريون, mbbuf_char_وٿي (&mbbuf, g), g.len);
      وٿي_ٻاهريون += g.len;
    }

  saved_errno = errno;
  إذا (!ferror (istream))
    saved_errno = 0;

  إذا (وٿي_ٻاهريون)
    write_ٻاهريون (سٽ_ٻاهريون, وٿي_ٻاهريون, /*newline=*/ false);

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
مکيه (صحيح argc, حرف **argv)
{
  size_t ويڪر = 80;
  صحيح optc;
  bool ok;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          counting_گهڻائي = ڳڻپ_بائيٽون;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_گهڻائي = ڳڻپ_CHARACTERS;
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
          ويڪر = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_ويڪر - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    ok = fold_file ("-", ويڪر);
  وإلا
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], ويڪر);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
