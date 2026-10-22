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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "mbbuf.h"
#অন্তর্ভাব "xdectoint.h"

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

স্থির অক্ষর ধ্রুবক shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"bytes", no_argument, ফাঁকা, 'b'},
  {"characters", no_argument, ফাঁকা, 'c'},
  {"spaces", no_argument, ফাঁকা, 's'},
  {"width", required_argument, ফাঁকা, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
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
Wrap প্রবেশ সারিগুলি in each FILE, writing to standard output.\n\
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
         বিরতি after blanks, or in শব্দগুলি greater than WIDTH\n\
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
          পূর্ণসংখ্যা width = (counting_mode == গুনতি_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  অন্যথা
    column += g.len;
  ফেরত column;
}

স্থির শূন্য
write_out (অক্ষর ধ্রুবক *সারি, size_t সারি_len, bool newline)
{
  যদি (fwrite (সারি, আকার (অক্ষর), সারি_len, stdout) != সারি_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

স্থির bool
fold_file (অক্ষর ধ্রুবক *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  স্থির অক্ষর সারি_out[IO_BUFSIZE];
  স্থির অক্ষর সারি_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  পূর্ণসংখ্যা saved_errno;

  যদি (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  অন্যথা
    istream = fopen (filename, "r");

  যদি (istream == ফাঁকা)
    {
      error (0, errno, "%s", quotef (filename));
      ফেরত false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, সারি_in, আকার সারি_in, istream);

  mcel_t g;
  যখন ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      যদি (g.ch == '\n')
        {
          write_out (সারি_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          চালু;
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
              পূর্ণসংখ্যা space_দৈর্ঘ্য = 0;
              idx_t logical_end = offset_out;
              অক্ষর *logical_p = সারি_out;
              অক্ষর *logical_lim = logical_p + logical_end;

              জন্য (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  যদি (c32issep (g2.ch))
                    {
                      space_দৈর্ঘ্য = g2.len;
                      logical_end = logical_p - সারি_out;
                    }
                }

              যদি (space_দৈর্ঘ্য)
                {
                  logical_end += space_দৈর্ঘ্য;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (সারি_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (সারি_out, সারি_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  অক্ষর *printed_p = সারি_out;
                  অক্ষর *printed_lim = printed_p + offset_out;
                  জন্য (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  যাও rescan;
                }
            }

          যদি (offset_out == 0)
            {
              memcpy (সারি_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              চালু;
            }

          write_out (সারি_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          যাও rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      যদি (আকার সারি_out <= offset_out + g.len)
        {
          write_out (সারি_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (সারি_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  যদি (!ferror (istream))
    saved_errno = 0;

  যদি (offset_out)
    write_out (সারি_out, offset_out, /*newline=*/ false);

  যদি (streq (filename, "-"))
    clearerr (istream);
  অন্যথা যদি (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  যদি (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ফেরত false;
    }

  ফেরত true;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  size_t width = 80;
  পূর্ণসংখ্যা optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, shortopts, longopts, ফাঁকা)) != -1)
    {
      অক্ষর optargbuf[2];

      নির্বাচন (optc)
        {
        ক্ষেত্র 'b':		/* Count bytes rather than columns. */
          counting_mode = গুনতি_BYTES;
          বিরতি;

        ক্ষেত্র 'c':               /* Count characters rather than columns. */
          counting_mode = গুনতি_CHARACTERS;
          বিরতি;

        ক্ষেত্র 's':		/* Break at word boundaries. */
          break_spaces = true;
          বিরতি;

        ক্ষেত্র '0': ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3': ক্ষেত্র '4':
        ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7': ক্ষেত্র '8': ক্ষেত্র '9':
          যদি (optarg)
            optarg--;
          অন্যথা
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        ক্ষেত্র 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc == optind)
    ok = fold_file ("-", width);
  অন্যথা
    {
      ok = true;
      জন্য (পূর্ণসংখ্যা i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  যদি (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
