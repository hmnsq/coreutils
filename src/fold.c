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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "fadvise.h"
#অন্তর্ভাৱ "ioblksize.h"
#অন্তর্ভাৱ "mcel.h"
#অন্তর্ভাৱ "mbbuf.h"
#অন্তর্ভাৱ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
স্থির bool break_spaces;

/* Mode to operate in.  */
স্থির গণনা
  {
    গুনতি_COLUMNS,
    গুনতি_BYTES,
    গুনতি_CHARACTERS
  } counting_mode = গুনতি_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
স্থির bool have_read_stdin;

স্থির ৱর্ণ ধ্রুৱ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"bytes", no_argument, রিক্ত, 'b'},
  {"characters", no_argument, রিক্ত, 'c'},
  {"spaces", no_argument, রিক্ত, 's'},
  {"width", required_argument, রিক্ত, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap প্ৰবেশ সাৰিগুলি in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         গুনতি bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         গুনতি characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ভঙ্গ after blanks, or in শব্দগুলি greater than WIDTH\n\
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

স্থির size_t
adjust_column (size_t column, mcel_t g)
{
  যদি (counting_mode != গুনতি_BYTES)
    {
      যদি (g.ch == '\b')
        column -= !!column;
      অন্যথা যদি (g.ch == '\r')
        column = 0;
      অন্যথা যদি (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      অন্যথা
        {
          পূর্ণাঙ্ক width = (counting_mode == গুনতি_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  অন্যথা
    column += g.len;
  ঘূৰাই column;
}

স্থির শূন্য
write_out (ৱর্ণ ধ্রুৱ *সাৰি, size_t সাৰি_len, bool newline)
{
  যদি (fwrite (সাৰি, পরিমাণ (ৱর্ণ), সাৰি_len, stdout) != সাৰি_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

স্থির bool
fold_file (ৱর্ণ ধ্রুৱ *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  স্থির ৱর্ণ সাৰি_out[IO_BUFSIZE];
  স্থির ৱর্ণ সাৰি_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  পূর্ণাঙ্ক saved_errno;

  যদি (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  অন্যথা
    istream = fopen (filename, "r");

  যদি (istream == রিক্ত)
    {
      error (0, errno, "%s", quotef (filename));
      ঘূৰাই false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, সাৰি_in, পরিমাণ সাৰি_in, istream);

  mcel_t g;
  যেতিয়ালৈ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      যদি (g.ch == '\n')
        {
          write_out (সাৰি_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          অনুৱর্ত;
        }
    rescan:
      column = adjust_column (column, g);

      যদি (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          যদি (break_spaces)
            {
              পূর্ণাঙ্ক space_length = 0;
              idx_t logical_end = offset_out;
              ৱর্ণ *logical_p = সাৰি_out;
              ৱর্ণ *logical_lim = logical_p + logical_end;

              বাবে (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  যদি (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - সাৰি_out;
                    }
                }

              যদি (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (সাৰি_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (সাৰি_out, সাৰি_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  ৱর্ণ *printed_p = সাৰি_out;
                  ৱর্ণ *printed_lim = printed_p + offset_out;
                  বাবে (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  গচ্ছ rescan;
                }
            }

          যদি (offset_out == 0)
            {
              memcpy (সাৰি_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              অনুৱর্ত;
            }

          write_out (সাৰি_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          গচ্ছ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      যদি (পরিমাণ সাৰি_out <= offset_out + g.len)
        {
          write_out (সাৰি_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (সাৰি_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  যদি (!ferror (istream))
    saved_errno = 0;

  যদি (offset_out)
    write_out (সাৰি_out, offset_out, /*newline=*/ false);

  যদি (streq (filename, "-"))
    clearerr (istream);
  অন্যথা যদি (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  যদি (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ঘূৰাই false;
    }

  ঘূৰাই true;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  size_t width = 80;
  পূর্ণাঙ্ক optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, shortopts, longopts, রিক্ত)) != -1)
    {
      ৱর্ণ optargbuf[2];

      ৱিকল্প (optc)
        {
        পক্ষ 'b':		/* Count bytes rather than columns. */
          counting_mode = গুনতি_BYTES;
          ভঙ্গ;

        পক্ষ 'c':               /* Count characters rather than columns. */
          counting_mode = গুনতি_CHARACTERS;
          ভঙ্গ;

        পক্ষ 's':		/* Break at word boundaries. */
          break_spaces = true;
          ভঙ্গ;

        পক্ষ '0': পক্ষ '1': পক্ষ '2': পক্ষ '3': পক্ষ '4':
        পক্ষ '5': পক্ষ '6': পক্ষ '7': পক্ষ '8': পক্ষ '9':
          যদি (optarg)
            optarg--;
          অন্যথা
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        পক্ষ 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc == optind)
    ok = fold_file ("-", width);
  অন্যথা
    {
      ok = true;
      বাবে (পূর্ণাঙ্ক i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
