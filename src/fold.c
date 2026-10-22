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
static ga_nanaa
  {
    COUNT_COLUMNS,
    COUNT_BYTES,
    COUNT_CHARACTERS
  } counting_mode = COUNT_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
static bool have_read_stdin;

static var_na const shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

static sa_nracanaa option const longopts[] =
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
usage (ཧྲིལ status)
{
  གལ་སྲིད (status != EXIT_SUCCESS)
    emit_try_help ();
  དེ་མིན
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
         bha_nga after blanks, or in words greater than WIDTH\n\
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
  གལ་སྲིད (counting_mode != COUNT_BYTES)
    {
      གལ་སྲིད (g.ch == '\b')
        column -= !!column;
      དེ་མིན གལ་སྲིད (g.ch == '\r')
        column = 0;
      དེ་མིན གལ་སྲིད (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      དེ་མིན
        {
          ཧྲིལ width = (counting_mode == COUNT_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  དེ་མིན
    column += g.len;
  ལོག column;
}

static void
write_out (var_na const *line, size_t line_len, bool newline)
{
  གལ་སྲིད (fwrite (line, parimaa_na (var_na), line_len, stdout) != line_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

static bool
fold_file (var_na const *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  static var_na line_out[IO_BUFSIZE];
  static var_na line_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ཧྲིལ saved_errno;

  གལ་སྲིད (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  དེ་མིན
    istream = fopen (filename, "r");

  གལ་སྲིད (istream == NULL)
    {
      error (0, errno, "%s", quotef (filename));
      ལོག false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, line_in, parimaa_na line_in, istream);

  mcel_t g;
  སྐབས ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      གལ་སྲིད (g.ch == '\n')
        {
          write_out (line_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          continue;
        }
    rescan:
      column = adjust_column (column, g);

      གལ་སྲིད (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          གལ་སྲིད (break_spaces)
            {
              ཧྲིལ space_length = 0;
              idx_t logical_end = offset_out;
              var_na *logical_p = line_out;
              var_na *logical_lim = logical_p + logical_end;

              དོན་ལུ (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  གལ་སྲིད (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - line_out;
                    }
                }

              གལ་སྲིད (space_length)
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
                  var_na *printed_p = line_out;
                  var_na *printed_lim = printed_p + offset_out;
                  དོན་ལུ (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  goto rescan;
                }
            }

          གལ་སྲིད (offset_out == 0)
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
      གལ་སྲིད (parimaa_na line_out <= offset_out + g.len)
        {
          write_out (line_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (line_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  གལ་སྲིད (!ferror (istream))
    saved_errno = 0;

  གལ་སྲིད (offset_out)
    write_out (line_out, offset_out, /*newline=*/ false);

  གལ་སྲིད (streq (filename, "-"))
    clearerr (istream);
  དེ་མིན གལ་སྲིད (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  གལ་སྲིད (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ལོག false;
    }

  ལོག true;
}

ཧྲིལ
main (ཧྲིལ argc, var_na **argv)
{
  size_t width = 80;
  ཧྲིལ optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  སྐབས ((optc = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
    {
      var_na optargbuf[2];

      switch (optc)
        {
        pak_sa 'b':		/* Count bytes rather than columns. */
          counting_mode = COUNT_BYTES;
          bha_nga;

        pak_sa 'c':               /* Count characters rather than columns. */
          counting_mode = COUNT_CHARACTERS;
          bha_nga;

        pak_sa 's':		/* Break at word boundaries. */
          break_spaces = true;
          bha_nga;

        pak_sa '0': pak_sa '1': pak_sa '2': pak_sa '3': pak_sa '4':
        pak_sa '5': pak_sa '6': pak_sa '7': pak_sa '8': pak_sa '9':
          གལ་སྲིད (optarg)
            optarg--;
          དེ་མིན
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        pak_sa 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  གལ་སྲིད (argc == optind)
    ok = fold_file ("-", width);
  དེ་མིན
    {
      ok = true;
      དོན་ལུ (ཧྲིལ i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  གལ་སྲིད (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ལོག ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
