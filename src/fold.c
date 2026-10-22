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
    COUNT_COLUMNS,
    COUNT_BYTES,
    COUNT_CHARACTERS
  } counting_mode = COUNT_COLUMNS;

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
  ܐܢ (status != EXIT_SUCCESS)
    emit_try_help ();
  ܐܚܪܢܐ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap input lines in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         count bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         count characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         اكسر after blanks, or in words greater than WIDTH\n\
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
  ܐܢ (counting_mode != COUNT_BYTES)
    {
      ܐܢ (g.ch == '\b')
        column -= !!column;
      ܐܚܪܢܐ ܐܢ (g.ch == '\r')
        column = 0;
      ܐܚܪܢܐ ܐܢ (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      ܐܚܪܢܐ
        {
          صحيح width = (counting_mode == COUNT_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  ܐܚܪܢܐ
    column += g.len;
  ܦܢܐ column;
}

ساكن فراغ
write_out (حرف ثابت *line, size_t line_len, bool newline)
{
  ܐܢ (fwrite (line, حجم (حرف), line_len, stdout) != line_len
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
  ساكن حرف line_out[IO_BUFSIZE];
  ساكن حرف line_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  صحيح saved_errno;

  ܐܢ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ܐܚܪܢܐ
    istream = fopen (filename, "r");

  ܐܢ (istream == عدم)
    {
      error (0, errno, "%s", quotef (filename));
      ܦܢܐ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, line_in, حجم line_in, istream);

  mcel_t g;
  ܟܡܐ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ܐܢ (g.ch == '\n')
        {
          write_out (line_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          استمر;
        }
    rescan:
      column = adjust_column (column, g);

      ܐܢ (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ܐܢ (break_spaces)
            {
              صحيح space_length = 0;
              idx_t logical_end = offset_out;
              حرف *logical_p = line_out;
              حرف *logical_lim = logical_p + logical_end;

              لكل (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  ܐܢ (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - line_out;
                    }
                }

              ܐܢ (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (line_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (line_out, line_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  حرف *printed_p = line_out;
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

          ܐܢ (offset_out == 0)
            {
              memcpy (line_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              استمر;
            }

          write_out (line_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          اذهب rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ܐܢ (حجم line_out <= offset_out + g.len)
        {
          write_out (line_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (line_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  ܐܢ (!ferror (istream))
    saved_errno = 0;

  ܐܢ (offset_out)
    write_out (line_out, offset_out, /*newline=*/ false);

  ܐܢ (streq (filename, "-"))
    clearerr (istream);
  ܐܚܪܢܐ ܐܢ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ܐܢ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ܦܢܐ false;
    }

  ܦܢܐ true;
}

صحيح
main (صحيح argc, حرف **argv)
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

  ܟܡܐ ((optc = getopt_long (argc, argv, shortopts, longopts, عدم)) != -1)
    {
      حرف optargbuf[2];

      بدل (optc)
        {
        حالة 'b':		/* Count bytes rather than columns. */
          counting_mode = COUNT_BYTES;
          اكسر;

        حالة 'c':               /* Count characters rather than columns. */
          counting_mode = COUNT_CHARACTERS;
          اكسر;

        حالة 's':		/* Break at word boundaries. */
          break_spaces = true;
          اكسر;

        حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
        حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
          ܐܢ (optarg)
            optarg--;
          ܐܚܪܢܐ
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

  ܐܢ (argc == optind)
    ok = fold_file ("-", width);
  ܐܚܪܢܐ
    {
      ok = true;
      لكل (صحيح i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  ܐܢ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ܦܢܐ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
