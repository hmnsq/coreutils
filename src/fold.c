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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "mbbuf.h"
#അന്തര്ഭാവ "xdectoint.h"

#define TAB_WIDTH 8

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "fold"

#define AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
സ്ഥിതം bool break_spaces;

/* Mode to operate in.  */
സ്ഥിതം എണ്ണൽ
  {
    എണ്ണം_COLUMNS,
    എണ്ണം_BYTES,
    എണ്ണം_CHARACTERS
  } counting_mode = എണ്ണം_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
സ്ഥിതം bool have_read_stdin;

സ്ഥിതം അക്ഷരം സ്ഥിരം shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"bytes", no_argument, ശൂന്യ, 'b'},
  {"characters", no_argument, ശൂന്യ, 'c'},
  {"spaces", no_argument, ശൂന്യ, 's'},
  {"width", required_argument, ശൂന്യ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Wrap നിവേശം വരികൾ in each FILE, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --bytes\n\
         എണ്ണം bytes rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         എണ്ണം characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         നിർത്തുക after blanks, or in വാക്കുകൾ greater than WIDTH\n\
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

സ്ഥിതം size_t
adjust_column (size_t column, mcel_t g)
{
  എങ്കിൽ (counting_mode != എണ്ണം_BYTES)
    {
      എങ്കിൽ (g.ch == '\b')
        column -= !!column;
      അല്ലെങ്കിൽ എങ്കിൽ (g.ch == '\r')
        column = 0;
      അല്ലെങ്കിൽ എങ്കിൽ (g.ch == '\t')
        column += TAB_WIDTH - column % TAB_WIDTH;
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ width = (counting_mode == എണ്ണം_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          column += width < 0 ? 1 : width;
        }
    }
  അല്ലെങ്കിൽ
    column += g.len;
  തിരികെ column;
}

സ്ഥിതം ശൂന്യം
write_out (അക്ഷരം സ്ഥിരം *വരി, size_t വരി_len, bool newline)
{
  എങ്കിൽ (fwrite (വരി, വലുപ്പം (അക്ഷരം), വരി_len, stdout) != വരി_len
      || (newline && putchar ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

സ്ഥിതം bool
fold_file (അക്ഷരം സ്ഥിരം *filename, size_t width)
{
  FILE *istream;
  size_t column = 0;		/* Screen column where next char will go. */
  idx_t offset_out = 0;		/* Index in 'line_out' for next char. */
  സ്ഥിതം അക്ഷരം വരി_out[IO_BUFSIZE];
  സ്ഥിതം അക്ഷരം വരി_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  പൂർണ്ണസംഖ്യ saved_errno;

  എങ്കിൽ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  അല്ലെങ്കിൽ
    istream = fopen (filename, "r");

  എങ്കിൽ (istream == ശൂന്യ)
    {
      error (0, errno, "%s", quotef (filename));
      തിരികെ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, വരി_in, വലുപ്പം വരി_in, istream);

  mcel_t g;
  ആകുമ്പോൾ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      എങ്കിൽ (g.ch == '\n')
        {
          write_out (വരി_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          തുടരുക;
        }
    rescan:
      column = adjust_column (column, g);

      എങ്കിൽ (column > width)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          എങ്കിൽ (break_spaces)
            {
              പൂർണ്ണസംഖ്യ space_length = 0;
              idx_t logical_end = offset_out;
              അക്ഷരം *logical_p = വരി_out;
              അക്ഷരം *logical_lim = logical_p + logical_end;

              വേണ്ടി (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_scan (logical_p, logical_lim);
                  എങ്കിൽ (c32issep (g2.ch))
                    {
                      space_length = g2.len;
                      logical_end = logical_p - വരി_out;
                    }
                }

              എങ്കിൽ (space_length)
                {
                  logical_end += space_length;
                  /* Found a blank.  Don't output the part after it. */
                  write_out (വരി_out, logical_end, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (വരി_out, വരി_out + logical_end,
                           offset_out - logical_end);
                  offset_out -= logical_end;
                  column = 0;
                  അക്ഷരം *printed_p = വരി_out;
                  അക്ഷരം *printed_lim = printed_p + offset_out;
                  വേണ്ടി (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_scan (printed_p, printed_lim);
                      column = adjust_column (column, g2);
                    }
                  പോകുക rescan;
                }
            }

          എങ്കിൽ (offset_out == 0)
            {
              memcpy (വരി_out, mbbuf_char_offset (&mbbuf, g), g.len);
              offset_out += g.len;
              തുടരുക;
            }

          write_out (വരി_out, offset_out, /*newline=*/ true);
          column = offset_out = 0;
          പോകുക rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      എങ്കിൽ (വലുപ്പം വരി_out <= offset_out + g.len)
        {
          write_out (വരി_out, offset_out, /*newline=*/ false);
          offset_out = 0;
        }

      memcpy (വരി_out + offset_out, mbbuf_char_offset (&mbbuf, g), g.len);
      offset_out += g.len;
    }

  saved_errno = errno;
  എങ്കിൽ (!ferror (istream))
    saved_errno = 0;

  എങ്കിൽ (offset_out)
    write_out (വരി_out, offset_out, /*newline=*/ false);

  എങ്കിൽ (streq (filename, "-"))
    clearerr (istream);
  അല്ലെങ്കിൽ എങ്കിൽ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  എങ്കിൽ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      തിരികെ false;
    }

  തിരികെ true;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  size_t width = 80;
  പൂർണ്ണസംഖ്യ optc;
  bool ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, shortopts, longopts, ശൂന്യ)) != -1)
    {
      അക്ഷരം optargbuf[2];

      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'b':		/* Count bytes rather than columns. */
          counting_mode = എണ്ണം_BYTES;
          നിർത്തുക;

        അവസ്ഥ 'c':               /* Count characters rather than columns. */
          counting_mode = എണ്ണം_CHARACTERS;
          നിർത്തുക;

        അവസ്ഥ 's':		/* Break at word boundaries. */
          break_spaces = true;
          നിർത്തുക;

        അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
        അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
          എങ്കിൽ (optarg)
            optarg--;
          അല്ലെങ്കിൽ
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        അവസ്ഥ 'w':		/* Line width. */
          width = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_WIDTH - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (argc == optind)
    ok = fold_file ("-", width);
  അല്ലെങ്കിൽ
    {
      ok = true;
      വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; i++)
        ok &= fold_file (argv[i], width);
    }

  എങ്കിൽ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
