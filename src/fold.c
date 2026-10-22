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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "mbbuf.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ସ୍ଥିର bool break_spaces;

/* Mode to operate in.  */
ସ୍ଥିର ଗଣନା
  {
    ଗଣନା_COLUMNS,
    ଗଣନା_BYTES,
    ଗଣନା_CHARACTERS
  } counting_mode = ଗଣନା_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ସ୍ଥିର bool have_read_stdin;

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"bytes", no_argument, ଖାଲି, 'b'},
  {"characters", no_argument, ଖାଲି, 'c'},
  {"spaces", no_argument, ଖାଲି, 's'},
  {"width", required_argument, ଖାଲି, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap ନିବେଶ ଧାଡ଼ିଗୁଡ଼ିକ in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         ଗଣନା_2 bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ଗଣନା_2 characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ଭାଙ୍ଗ after blanks, or in ଶବ୍ଦଗୁଡ଼ିକ greater than WIDTH\n\
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

ସ୍ଥିର size_t
adjust_column (size_t column, mcel_t g)
{
  ଯଦି (counting_mode != ଗଣନା_BYTES)
    {
      ଯଦି (g.ch == '\b')
        column -= !!column;
      ଅନ୍ୟଥା ଯଦି (g.ch == '\r')
        column = 0;
      ଅନ୍ୟଥା ଯଦି (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = (counting_mode == ଗଣନା_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  ଅନ୍ୟଥା
    column += g.len;
  ଫେରନ୍ତୁ column;
}

ସ୍ଥିର ଶୂନ୍ୟ
write_out (ଅକ୍ଷର ଅଚଳ *ଧାଡ଼ି, size_t ଧାଡ଼ି_len, bool newline)
{
  ଯଦି (fwrite (ଧାଡ଼ି, ଆକାର (ଅକ୍ଷର), ଧାଡ଼ି_len, stdout) != ଧାଡ଼ି_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ସ୍ଥିର bool
fold_file (ଅକ୍ଷର ଅଚଳ *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_out[IO_BUFSIZE];
  ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno;

  ଯଦି (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ଅନ୍ୟଥା
    istream = fopen (filename, "r");

  ଯଦି (istream == ଖାଲି)
    {
      error (0, errno, "%s", quotef (filename));
      ଫେରନ୍ତୁ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, ଧାଡ଼ି_in, ଆକାର ଧାଡ଼ି_in, istream);

  mcel_t g;
  ଯେତେବେଳେ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ଯଦି (g.ch == '\n')
        {
          write_out (ଧାଡ଼ି_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ଜାରି;
        }
    rescan:
      column = adjust_column (column, g);

      ଯଦି (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ଯଦି (break_spaces)
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା space_length = 0;
              idx_t logical_end = offset_out;
              ଅକ୍ଷର *logical_p = ଧାଡ଼ି_out;
              ଅକ୍ଷର *logical_lim = logical_p + logical_end;

              ପାଇଁ (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  ଯଦି (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - ଧାଡ଼ି_out;
                    }
                }

              ଯଦି (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (ଧାଡ଼ି_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (ଧାଡ଼ି_out, ଧାଡ଼ି_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  ଅକ୍ଷର *printed_p = ଧାଡ଼ି_out;
                  ଅକ୍ଷର *printed_lim = printed_p + offset_out;
                  ପାଇଁ (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  ଯାଅ rescan;
                }
            }

          ଯଦି (offset_out == 0)
            {
              memcpy (ଧାଡ଼ି_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              ଜାରି;
            }

          write_out (ଧାଡ଼ି_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          ଯାଅ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ଯଦି (ଆକାର ଧାଡ଼ି_out <= offset_out + g.len)
        {
          write_out (ଧାଡ଼ି_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (ଧାଡ଼ି_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  ଯଦି (!ferror (istream))
    saved_errno = 0;

  ଯଦି (offset_out)
    write_out (ଧାଡ଼ି_out, offset_out, /*newline=*/ false);

  ଯଦି (streq (filename, "-"))
    clearerr (istream);
  ଅନ୍ୟଥା ଯଦି (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ଯଦି (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  size_t width = 80;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, shortopts, longopts, ଖାଲି)) != -1)
    {
      ଅକ୍ଷର optargbuf[2];

      ବାଛ (optc)
        {
        ସ୍ଥିତି 'b':		/* Count bytes rather than columns. */
          counting_mode = ଗଣନା_BYTES;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':               /* Count characters rather than columns. */
          counting_mode = ଗଣନା_CHARACTERS;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':		/* Break at word boundaries. */
          break_spaces = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3': ସ୍ଥିତି '4':
        ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7': ସ୍ଥିତି '8': ସ୍ଥିତି '9':
          ଯଦି (optarg)
            optarg--;
          ଅନ୍ୟଥା
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        ସ୍ଥିତି 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (argc == optind)
    ok = fold_file ("-", width);
  ଅନ୍ୟଥା
    {
      ok = true;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
