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

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ایستا bool break_spaces;

/* Mode to operate in.  */
ایستا شمارش
  {
    شمار_COLUMNS,
    شمار_BYTES,
    شمار_CHARACTERS
  } counting_mode = شمار_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ایستا bool have_read_stdin;

ایستا نویسه ثابت shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ایستا ساختار option ثابت longopts[] =
{
  {"bytes", no_argument, پوچ, 'b'},
  {"characters", no_argument, پوچ, 'c'},
  {"spaces", no_argument, پوچ, 's'},
  {"width", required_argument, پوچ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap درون_داد سطرها in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         شمار bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         شمار characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         بشکن after blanks, or in واژگان greater than WIDTH\n\
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

ایستا size_t
adjust_column (size_t column, mcel_t g)
{
  اگر (counting_mode != شمار_BYTES)
    {
      اگر (g.ch == '\b')
        column -= !!column;
      وگرنه اگر (g.ch == '\r')
        column = 0;
      وگرنه اگر (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      وگرنه
        {
          صحیح width = (counting_mode == شمار_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  وگرنه
    column += g.len;
  بازگشت column;
}

ایستا تهی
write_out (نویسه ثابت *سطر, size_t سطر_len, bool newline)
{
  اگر (fwrite (سطر, اندازه (نویسه), سطر_len, stdout) != سطر_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ایستا bool
fold_file (نویسه ثابت *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  ایستا نویسه سطر_out[IO_BUFSIZE];
  ایستا نویسه سطر_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  صحیح saved_errno;

  اگر (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  وگرنه
    istream = fopen (filename, "r");

  اگر (istream == پوچ)
    {
      error (0, errno, "%s", quotef (filename));
      بازگشت false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, سطر_in, اندازه سطر_in, istream);

  mcel_t g;
  وقتی ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      اگر (g.ch == '\n')
        {
          write_out (سطر_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ادامه;
        }
    rescan:
      column = adjust_column (column, g);

      اگر (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          اگر (break_spaces)
            {
              صحیح space_length = 0;
              idx_t logical_end = offset_out;
              نویسه *logical_p = سطر_out;
              نویسه *logical_lim = logical_p + logical_end;

              برای (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  اگر (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - سطر_out;
                    }
                }

              اگر (space_length)
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
                  نویسه *printed_p = سطر_out;
                  نویسه *printed_lim = printed_p + offset_out;
                  برای (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  برو rescan;
                }
            }

          اگر (offset_out == 0)
            {
              memcpy (سطر_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              ادامه;
            }

          write_out (سطر_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          برو rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      اگر (اندازه سطر_out <= offset_out + g.len)
        {
          write_out (سطر_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (سطر_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  اگر (!ferror (istream))
    saved_errno = 0;

  اگر (offset_out)
    write_out (سطر_out, offset_out, /*newline=*/ false);

  اگر (streq (filename, "-"))
    clearerr (istream);
  وگرنه اگر (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  اگر (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      بازگشت false;
    }

  بازگشت true;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  size_t width = 80;
  صحیح optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, shortopts, longopts, پوچ)) != -1)
    {
      نویسه optargbuf[2];

      انتخاب (optc)
        {
        حالت 'b':		/* Count bytes rather than columns. */
          counting_mode = شمار_BYTES;
          بشکن;

        حالت 'c':               /* Count characters rather than columns. */
          counting_mode = شمار_CHARACTERS;
          بشکن;

        حالت 's':		/* Break at word boundaries. */
          break_spaces = true;
          بشکن;

        حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
        حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
          اگر (optarg)
            optarg--;
          وگرنه
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        حالت 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc == optind)
    ok = fold_file ("-", width);
  وگرنه
    {
      ok = true;
      برای (صحیح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
