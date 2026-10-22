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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "mbbuf.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ಸ್ಥಿರ bool break_spaces;

/* Mode to operate in.  */
ಸ್ಥಿರ ಗಣನಾ
  {
    ಎಣಿಕೆ_COLUMNS,
    ಎಣಿಕೆ_BYTES,
    ಎಣಿಕೆ_CHARACTERS
  } counting_mode = ಎಣಿಕೆ_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ಸ್ಥಿರ bool have_read_stdin;

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"bytes", no_argument, ರಿಕ್ತ, 'b'},
  {"characters", no_argument, ರಿಕ್ತ, 'c'},
  {"spaces", no_argument, ರಿಕ್ತ, 's'},
  {"width", required_argument, ರಿಕ್ತ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap ಒಳಹರಿವು ಸಾಲುಗಳು in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         ಎಣಿಕೆ bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ಎಣಿಕೆ characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ಭಙ್ಗ after blanks, or in ಪದಗಳು greater than WIDTH\n\
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

ಸ್ಥಿರ size_t
adjust_column (size_t column, mcel_t g)
{
  ಯದಿ (counting_mode != ಎಣಿಕೆ_BYTES)
    {
      ಯದಿ (g.ch == '\b')
        column -= !!column;
      ಅನ್ಯಥಾ ಯದಿ (g.ch == '\r')
        column = 0;
      ಅನ್ಯಥಾ ಯದಿ (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ width = (counting_mode == ಎಣಿಕೆ_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  ಅನ್ಯಥಾ
    column += g.len;
  ಪ್ರತ್ಯಾಗಮ column;
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_out (ವರ್ಣ ಧ್ರುವ *ಸಾಲು, size_t ಸಾಲು_len, bool newline)
{
  ಯದಿ (fwrite (ಸಾಲು, ಪರಿಮಾಣ (ವರ್ಣ), ಸಾಲು_len, stdout) != ಸಾಲು_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ಸ್ಥಿರ bool
fold_file (ವರ್ಣ ಧ್ರುವ *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_out[IO_BUFSIZE];
  ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ಪೂರ್ಣಾಙ್ಕ saved_errno;

  ಯದಿ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ಅನ್ಯಥಾ
    istream = fopen (filename, "r");

  ಯದಿ (istream == ರಿಕ್ತ)
    {
      error (0, errno, "%s", quotef (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, ಸಾಲು_in, ಪರಿಮಾಣ ಸಾಲು_in, istream);

  mcel_t g;
  ಯಾವತ್ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ಯದಿ (g.ch == '\n')
        {
          write_out (ಸಾಲು_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ಅನುವರ್ತ;
        }
    rescan:
      column = adjust_column (column, g);

      ಯದಿ (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ಯದಿ (break_spaces)
            {
              ಪೂರ್ಣಾಙ್ಕ space_length = 0;
              idx_t logical_end = offset_out;
              ವರ್ಣ *logical_p = ಸಾಲು_out;
              ವರ್ಣ *logical_lim = logical_p + logical_end;

              ಪ್ರತಿ (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  ಯದಿ (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - ಸಾಲು_out;
                    }
                }

              ಯದಿ (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (ಸಾಲು_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (ಸಾಲು_out, ಸಾಲು_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  ವರ್ಣ *printed_p = ಸಾಲು_out;
                  ವರ್ಣ *printed_lim = printed_p + offset_out;
                  ಪ್ರತಿ (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  ಗಚ್ಛ rescan;
                }
            }

          ಯದಿ (offset_out == 0)
            {
              memcpy (ಸಾಲು_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              ಅನುವರ್ತ;
            }

          write_out (ಸಾಲು_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ಗಚ್ಛ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ಯದಿ (ಪರಿಮಾಣ ಸಾಲು_out <= offset_out + g.len)
        {
          write_out (ಸಾಲು_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (ಸಾಲು_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  ಯದಿ (!ferror (istream))
    saved_errno = 0;

  ಯದಿ (offset_out)
    write_out (ಸಾಲು_out, offset_out, /*newline=*/ false);

  ಯದಿ (streq (filename, "-"))
    clearerr (istream);
  ಅನ್ಯಥಾ ಯದಿ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ಯದಿ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  size_t width = 80;
  ಪೂರ್ಣಾಙ್ಕ optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, shortopts, longopts, ರಿಕ್ತ)) != -1)
    {
      ವರ್ಣ optargbuf[2];

      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'b':		/* Count bytes rather than columns. */
          counting_mode = ಎಣಿಕೆ_BYTES;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'c':               /* Count characters rather than columns. */
          counting_mode = ಎಣಿಕೆ_CHARACTERS;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':		/* Break at word boundaries. */
          break_spaces = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
        ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
          ಯದಿ (optarg)
            optarg--;
          ಅನ್ಯಥಾ
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        ಪಕ್ಷ 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (argc == optind)
    ok = fold_file ("-", width);
  ಅನ್ಯಥಾ
    {
      ok = true;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
