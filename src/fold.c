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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "mbbuf.h"
#அந்தர்பாவ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
நிலையான bool break_spaces;

/* Mode to operate in.  */
நிலையான எண்ணிக்கை
  {
    மொத்தம்_COLUMNS,
    மொத்தம்_BYTES,
    மொத்தம்_CHARACTERS
  } counting_mode = மொத்தம்_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
நிலையான bool have_read_stdin;

நிலையான எழுத்து மாறிலி shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"bytes", no_argument, வெற்று, 'b'},
  {"characters", no_argument, வெற்று, 'c'},
  {"spaces", no_argument, வெற்று, 's'},
  {"width", required_argument, வெற்று, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap உள்ளீடு வரிகள் in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         மொத்தம் bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         மொத்தம் characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         நிறுத்து after blanks, or in சொற்கள் greater than WIDTH\n\
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

நிலையான size_t
adjust_column (size_t column, mcel_t g)
{
  எனில் (counting_mode != மொத்தம்_BYTES)
    {
      எனில் (g.ch == '\b')
        column -= !!column;
      இல்லையெனில் எனில் (g.ch == '\r')
        column = 0;
      இல்லையெனில் எனில் (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      இல்லையெனில்
        {
          முழு width = (counting_mode == மொத்தம்_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  இல்லையெனில்
    column += g.len;
  திரும்பு column;
}

நிலையான வெறுமை
write_out (எழுத்து மாறிலி *வரி, size_t வரி_len, bool newline)
{
  எனில் (fwrite (வரி, அளவு (எழுத்து), வரி_len, stdout) != வரி_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

நிலையான bool
fold_file (எழுத்து மாறிலி *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  நிலையான எழுத்து வரி_out[IO_BUFSIZE];
  நிலையான எழுத்து வரி_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  முழு saved_errno;

  எனில் (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  இல்லையெனில்
    istream = fopen (filename, "r");

  எனில் (istream == வெற்று)
    {
      error (0, errno, "%s", quotef (filename));
      திரும்பு false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, வரி_in, அளவு வரி_in, istream);

  mcel_t g;
  போது ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      எனில் (g.ch == '\n')
        {
          write_out (வரி_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          தொடர்;
        }
    rescan:
      column = adjust_column (column, g);

      எனில் (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          எனில் (break_spaces)
            {
              முழு space_length = 0;
              idx_t logical_end = offset_out;
              எழுத்து *logical_p = வரி_out;
              எழுத்து *logical_lim = logical_p + logical_end;

              க்கு (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  எனில் (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - வரி_out;
                    }
                }

              எனில் (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (வரி_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (வரி_out, வரி_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  எழுத்து *printed_p = வரி_out;
                  எழுத்து *printed_lim = printed_p + offset_out;
                  க்கு (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  செல் rescan;
                }
            }

          எனில் (offset_out == 0)
            {
              memcpy (வரி_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              தொடர்;
            }

          write_out (வரி_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          செல் rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      எனில் (அளவு வரி_out <= offset_out + g.len)
        {
          write_out (வரி_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (வரி_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  எனில் (!ferror (istream))
    saved_errno = 0;

  எனில் (offset_out)
    write_out (வரி_out, offset_out, /*newline=*/ false);

  எனில் (streq (filename, "-"))
    clearerr (istream);
  இல்லையெனில் எனில் (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  எனில் (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      திரும்பு false;
    }

  திரும்பு true;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  size_t width = 80;
  முழு optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, shortopts, longopts, வெற்று)) != -1)
    {
      எழுத்து optargbuf[2];

      தேர்வு (optc)
        {
        நிலை 'b':		/* Count bytes rather than columns. */
          counting_mode = மொத்தம்_BYTES;
          நிறுத்து;

        நிலை 'c':               /* Count characters rather than columns. */
          counting_mode = மொத்தம்_CHARACTERS;
          நிறுத்து;

        நிலை 's':		/* Break at word boundaries. */
          break_spaces = true;
          நிறுத்து;

        நிலை '0': நிலை '1': நிலை '2': நிலை '3': நிலை '4':
        நிலை '5': நிலை '6': நிலை '7': நிலை '8': நிலை '9':
          எனில் (optarg)
            optarg--;
          இல்லையெனில்
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        நிலை 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (argc == optind)
    ok = fold_file ("-", width);
  இல்லையெனில்
    {
      ok = true;
      க்கு (முழு i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
