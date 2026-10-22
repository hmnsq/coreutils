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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "mbbuf.h"
#અન્તર્ભાવ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
સ્થિર bool break_spaces;

/* Mode to operate in.  */
સ્થિર ગણના
  {
    ગણતરી_COLUMNS,
    ગણતરી_BYTES,
    ગણતરી_CHARACTERS
  } counting_mode = ગણતરી_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
સ્થિર bool have_read_stdin;

સ્થિર અક્ષર અચલ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

સ્થિર સંરચના option અચલ longopts[] =
{
  {"bytes", no_argument, ખાલી, 'b'},
  {"characters", no_argument, ખાલી, 'c'},
  {"spaces", no_argument, ખાલી, 's'},
  {"width", required_argument, ખાલી, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap આગત લીટીઓ in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         ગણતરી bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ગણતરી characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         તોડો after blanks, or in શબ્દો greater than WIDTH\n\
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

સ્થિર size_t
adjust_column (size_t column, mcel_t g)
{
  જો (counting_mode != ગણતરી_BYTES)
    {
      જો (g.ch == '\b')
        column -= !!column;
      નહીંતર જો (g.ch == '\r')
        column = 0;
      નહીંતર જો (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      નહીંતર
        {
          પૂર્ણાંક width = (counting_mode == ગણતરી_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  નહીંતર
    column += g.len;
  પાછા column;
}

સ્થિર શૂન્ય
write_out (અક્ષર અચલ *લીટી, size_t લીટી_len, bool newline)
{
  જો (fwrite (લીટી, કદ (અક્ષર), લીટી_len, stdout) != લીટી_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

સ્થિર bool
fold_file (અક્ષર અચલ *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  સ્થિર અક્ષર લીટી_out[IO_BUFSIZE];
  સ્થિર અક્ષર લીટી_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  પૂર્ણાંક saved_errno;

  જો (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  નહીંતર
    istream = fopen (filename, "r");

  જો (istream == ખાલી)
    {
      error (0, errno, "%s", quotef (filename));
      પાછા false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, લીટી_in, કદ લીટી_in, istream);

  mcel_t g;
  જ્યારે ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      જો (g.ch == '\n')
        {
          write_out (લીટી_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ચાલુ;
        }
    rescan:
      column = adjust_column (column, g);

      જો (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          જો (break_spaces)
            {
              પૂર્ણાંક space_length = 0;
              idx_t logical_end = offset_out;
              અક્ષર *logical_p = લીટી_out;
              અક્ષર *logical_lim = logical_p + logical_end;

              માટે (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  જો (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - લીટી_out;
                    }
                }

              જો (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (લીટી_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (લીટી_out, લીટી_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  અક્ષર *printed_p = લીટી_out;
                  અક્ષર *printed_lim = printed_p + offset_out;
                  માટે (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  જાઓ rescan;
                }
            }

          જો (offset_out == 0)
            {
              memcpy (લીટી_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              ચાલુ;
            }

          write_out (લીટી_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          જાઓ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      જો (કદ લીટી_out <= offset_out + g.len)
        {
          write_out (લીટી_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (લીટી_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  જો (!ferror (istream))
    saved_errno = 0;

  જો (offset_out)
    write_out (લીટી_out, offset_out, /*newline=*/ false);

  જો (streq (filename, "-"))
    clearerr (istream);
  નહીંતર જો (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  જો (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      પાછા false;
    }

  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  size_t width = 80;
  પૂર્ણાંક optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, shortopts, longopts, ખાલી)) != -1)
    {
      અક્ષર optargbuf[2];

      પસંદગી (optc)
        {
        સ્થિતિ 'b':		/* Count bytes rather than columns. */
          counting_mode = ગણતરી_BYTES;
          તોડો;

        સ્થિતિ 'c':               /* Count characters rather than columns. */
          counting_mode = ગણતરી_CHARACTERS;
          તોડો;

        સ્થિતિ 's':		/* Break at word boundaries. */
          break_spaces = true;
          તોડો;

        સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3': સ્થિતિ '4':
        સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7': સ્થિતિ '8': સ્થિતિ '9':
          જો (optarg)
            optarg--;
          નહીંતર
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        સ્થિતિ 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (argc == optind)
    ok = fold_file ("-", width);
  નહીંતર
    {
      ok = true;
      માટે (પૂર્ણાંક i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  જો (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
