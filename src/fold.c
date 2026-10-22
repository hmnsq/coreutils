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

#include <config.h>

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "system.h"
#include "fadvise.h"
#include "ioblksize.h"
#include "mcel.h"
#include "mbbuf.h"
#include "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
static bool break_spaces;

/* Mode to operate in.  */
static enum
  {
    COUNT_COLUMNS,
    COUNT_BYTES,
    COUNT_CHARACTERS
  } counting_mode = COUNT_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
static bool have_read_stdin;

static char const shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

static struct option const longopts[] =
{
  {"bytes", no_argument, NULL, 'b'},
  {"characters", no_argument, NULL, 'c'},
  {"spaces", no_argument, NULL, 's'},
  {"width", required_argument, NULL, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (정수 status)
{
  만약 (status != EXIT_SUCCESS)
    emit_try_help ();
  아니면
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
         break after blanks, or in words greater than WIDTH\n\
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

static size_t
adjust_column (size_t column, mcel_t g)
{
  만약 (counting_mode != COUNT_BYTES)
    {
      만약 (g.ch == '\b')
        column -= !!column;
      아니면 만약 (g.ch == '\r')
        column = 0;
      아니면 만약 (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      아니면
        {
          정수 width = (counting_mode == COUNT_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  아니면
    column += g.len;
  반환 column;
}

static void
write_out (char const *line, size_t line_len, bool newline)
{
  만약 (fwrite (line, sizeof (char), line_len, stdout) != line_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

static bool
fold_file (char const *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  static char line_out[IO_BUFSIZE];
  static char line_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  정수 saved_errno;

  만약 (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  아니면
    istream = fopen (filename, "r");

  만약 (istream == NULL)
    {
      error (0, errno, "%s", quotef (filename));
      반환 false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, line_in, sizeof line_in, istream);

  mcel_t g;
  동안 ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      만약 (g.ch == '\n')
        {
          write_out (line_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          continue;
        }
    rescan:
      column = adjust_column (column, g);

      만약 (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          만약 (break_spaces)
            {
              정수 space_length = 0;
              idx_t logical_end = offset_out;
              char *logical_p = line_out;
              char *logical_lim = logical_p + logical_end;

              위해 (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  만약 (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - line_out;
                    }
                }

              만약 (space_length)
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
                  char *printed_p = line_out;
                  char *printed_lim = printed_p + offset_out;
                  위해 (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  goto rescan;
                }
            }

          만약 (offset_out == 0)
            {
              memcpy (line_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              continue;
            }

          write_out (line_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          goto rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      만약 (sizeof line_out <= offset_out + g.len)
        {
          write_out (line_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (line_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  만약 (!ferror (istream))
    saved_errno = 0;

  만약 (offset_out)
    write_out (line_out, offset_out, /*newline=*/ false);

  만약 (streq (filename, "-"))
    clearerr (istream);
  아니면 만약 (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  만약 (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      반환 false;
    }

  반환 true;
}

정수
main (정수 argc, char **argv)
{
  size_t width = 80;
  정수 optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  동안 ((optc = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      char optargbuf[2];

      switch (optc)
        {
        case 'b':		/* Count bytes rather than columns. */
          counting_mode = COUNT_BYTES;
          break;

        case 'c':               /* Count characters rather than columns. */
          counting_mode = COUNT_CHARACTERS;
          break;

        case 's':		/* Break at word boundaries. */
          break_spaces = true;
          break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          만약 (optarg)
            optarg--;
          아니면
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        case 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          break;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  만약 (argc == optind)
    ok = fold_file ("-", width);
  아니면
    {
      ok = true;
      위해 (정수 i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  만약 (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  반환 ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
