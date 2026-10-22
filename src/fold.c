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

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ساكن bool break_spaces;

/* Mode to operate in.  */
ساكن تعداد
  {
    تعداد_COLUMNS,
    تعداد_BYTES,
    تعداد_CHARACTERS
  } counting_mode = تعداد_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ساكن bool have_read_stdin;

ساكن حرف ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ساكن بنية option ثابت longopts[] =
{
  {"bytes", no_argument, عدم, 'b'},
  {"characters", no_argument, عدم, 'c'},
  {"spaces", no_argument, عدم, 's'},
  {"width", required_argument, عدم, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap إدخال أسطر in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         تعداد_2 bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         تعداد_2 characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or in كلمات greater than WIDTH\n\
"));
      oputs (_("\
  -w, --width=WIDTH\n\
         use WIDTH columns instead of 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ساكن size_t
adjust_column (size_t column, mcel_t g)
{
  إذا (counting_mode != تعداد_BYTES)
    {
      إذا (g.ch == '\b')
        column -= !!column;
      وإلا إذا (g.ch == '\r')
        column = 0;
      وإلا إذا (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      وإلا
        {
          صحيح width = (counting_mode == تعداد_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  وإلا
    column += g.len;
  إرجاع column;
}

ساكن فراغ
write_out (حرف ثابت *سطر, size_t سطر_len, bool newline)
{
  إذا (fwrite (سطر, حجم (حرف), سطر_len, stdout) != سطر_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ساكن bool
fold_file (حرف ثابت *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  ساكن حرف سطر_out[IO_BUFSIZE];
  ساكن حرف سطر_in[IO_BUFSIZE];
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
      إرجاع false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, سطر_in, حجم سطر_in, istream);

  mcel_t g;
  بينما ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      إذا (g.ch == '\n')
        {
          write_out (سطر_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          استمر;
        }
    rescan:
      column = adjust_column (column, g);

      إذا (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          إذا (break_spaces)
            {
              صحيح space_length = 0;
              idx_t logical_end = offset_out;
              حرف *logical_p = سطر_out;
              حرف *logical_lim = logical_p + logical_end;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  إذا (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - سطر_out;
                    }
                }

              إذا (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (سطر_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (سطر_out, سطر_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  حرف *printed_p = سطر_out;
                  حرف *printed_lim = printed_p + offset_out;
                  لكل (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  اذهب rescan;
                }
            }

          إذا (offset_out == 0)
            {
              memcpy (سطر_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              استمر;
            }

          write_out (سطر_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      إذا (حجم سطر_out <= offset_out + g.len)
        {
          write_out (سطر_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (سطر_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  إذا (!ferror (istream))
    saved_errno = 0;

  إذا (offset_out)
    write_out (سطر_out, offset_out, /*newline=*/ false);

  إذا (streq (filename, "-"))
    clearerr (istream);
  وإلا إذا (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  إذا (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      إرجاع false;
    }

  إرجاع true;
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  size_t width = 80;
  صحيح optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  بينما ((optc = getopt_long (argc, argv, shortopts, longopts, عدم)) != -1)
    {
      حرف optargbuf[2];

      بدل (optc)
        {
        حالة 'b':		/* Count bytes rather than columns. */
          counting_mode = تعداد_BYTES;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_mode = تعداد_CHARACTERS;
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
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc == optind)
    ok = fold_file ("-", width);
  وإلا
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  إرجاع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
