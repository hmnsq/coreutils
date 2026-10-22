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

#ವ್ಯಾಖ್ಯಾನಿಸು TAB_ಅಗಲ 8

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "fold"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ಸ್ಥಿರ bool break_spaces;

/* Mode to operate in.  */
ಸ್ಥಿರ ಎಣಿಕೆ
  {
    ಎಣಿಕೆ_COLUMNS,
    ಎಣಿಕೆ_ಬೈಟುಗಳು,
    ಎಣಿಕೆ_CHARACTERS
  } counting_ಬಹುಲಕ = ಎಣಿಕೆ_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ಸ್ಥಿರ bool have_read_stdin;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"bytes", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"characters", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"spaces", no_ವಾದ, ಖಾಲಿ, 's'},
  {"width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Wrap ಒಳಹರಿವು ಸಾಲುಗಳು ಒಳ each FILE, writing ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --ಬೈಟುಗಳು\n\
         ಎಣಿಕೆ_2 ಬೈಟುಗಳು rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ಎಣಿಕೆ_2 characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ನಿಲ್ಲಿಸು after blanks, or ಒಳ ಪದಗಳು greater than ಅಗಲ_2\n\
"));
      oputs (_("\
  -w, --ಅಗಲ=ಅಗಲ_2\n\
         use ಅಗಲ_2 columns instead ದ 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ಸ್ಥಿರ size_t
adjust_ಸ್ತಂಭ (size_t ಸ್ತಂಭ, mcel_t g)
{
  ಆದರೆ (counting_ಬಹುಲಕ != ಎಣಿಕೆ_ಬೈಟುಗಳು)
    {
      ಆದರೆ (g.ch == '\b')
        ಸ್ತಂಭ -= !!ಸ್ತಂಭ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g.ch == '\r')
        ಸ್ತಂಭ = 0;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g.ch == '\t')
        ಸ್ತಂಭ += TAB_ಅಗಲ - ಸ್ತಂಭ % TAB_ಅಗಲ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ ಅಗಲ = (counting_ಬಹುಲಕ == ಎಣಿಕೆ_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          ಸ್ತಂಭ += ಅಗಲ < 0 ? 1 : ಅಗಲ;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ತಂಭ += g.len;
  ಹಿಂತಿರುಗಿ ಸ್ತಂಭ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಹೊರಹೋಗು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಾಲು, size_t ಸಾಲು_len, bool newline)
{
  ಆದರೆ (fwrite (ಸಾಲು, ಗಾತ್ರ (ಅಕ್ಷರ), ಸಾಲು_len, stdout) != ಸಾಲು_len
      || (newline && ಅಕ್ಷರ_ಬರೆ ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ಸ್ಥಿರ bool
fold_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, size_t ಅಗಲ)
{
  FILE *istream;
  size_t ಸ್ತಂಭ = 0;		/* Screen column where next char will go. */
  idx_t ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು = 0;		/* Index in 'line_out' for next char. */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಹೊರಹೋಗು[IO_BUFSIZE];
  ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಒಳ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ಪೂರ್ಣಾಂಕ saved_errno;

  ಆದರೆ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    istream = fopen (filename, "r");

  ಆದರೆ (istream == ಖಾಲಿ)
    {
      error (0, errno, "%s", quotef (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, ಸಾಲು_ಒಳ, ಗಾತ್ರ ಸಾಲು_ಒಳ, istream);

  mcel_t g;
  ಆಗ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ಆದರೆ (g.ch == '\n')
        {
          write_ಹೊರಹೋಗು (ಸಾಲು_ಹೊರಹೋಗು, ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು, /*newline=*/ true);
          ಸ್ತಂಭ = ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು = 0;
          ಮುಂದುವರಿ;
        }
    rescan:
      ಸ್ತಂಭ = adjust_ಸ್ತಂಭ (ಸ್ತಂಭ, g);

      ಆದರೆ (ಸ್ತಂಭ > ಅಗಲ)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ಆದರೆ (break_spaces)
            {
              ಪೂರ್ಣಾಂಕ space_ಉದ್ದಳತೆ = 0;
              idx_t logical_ಕೊನೆ = ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು;
              ಅಕ್ಷರ *logical_p = ಸಾಲು_ಹೊರಹೋಗು;
              ಅಕ್ಷರ *logical_lim = logical_p + logical_ಕೊನೆ;

              ಗಾಗಿ (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_ಪರಿಶೀಲನೆ (logical_p, logical_lim);
                  ಆದರೆ (c32issep (g2.ch))
                    {
                      space_ಉದ್ದಳತೆ = g2.len;
                      logical_ಕೊನೆ = logical_p - ಸಾಲು_ಹೊರಹೋಗು;
                    }
                }

              ಆದರೆ (space_ಉದ್ದಳತೆ)
                {
                  logical_ಕೊನೆ += space_ಉದ್ದಳತೆ;
                  /* Found a blank.  Don't output the part after it. */
                  write_ಹೊರಹೋಗು (ಸಾಲು_ಹೊರಹೋಗು, logical_ಕೊನೆ, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (ಸಾಲು_ಹೊರಹೋಗು, ಸಾಲು_ಹೊರಹೋಗು + logical_ಕೊನೆ,
                           ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು - logical_ಕೊನೆ);
                  ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು -= logical_ಕೊನೆ;
                  ಸ್ತಂಭ = 0;
                  ಅಕ್ಷರ *printed_p = ಸಾಲು_ಹೊರಹೋಗು;
                  ಅಕ್ಷರ *printed_lim = printed_p + ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು;
                  ಗಾಗಿ (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_ಪರಿಶೀಲನೆ (printed_p, printed_lim);
                      ಸ್ತಂಭ = adjust_ಸ್ತಂಭ (ಸ್ತಂಭ, g2);
                    }
                  ಹೋಗು rescan;
                }
            }

          ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು == 0)
            {
              ಸ್ಮರಣೆ_ನಕಲು (ಸಾಲು_ಹೊರಹೋಗು, mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), g.len);
              ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು += g.len;
              ಮುಂದುವರಿ;
            }

          write_ಹೊರಹೋಗು (ಸಾಲು_ಹೊರಹೋಗು, ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು, /*newline=*/ true);
          ಸ್ತಂಭ = ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು = 0;
          ಹೋಗು rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ಆದರೆ (ಗಾತ್ರ ಸಾಲು_ಹೊರಹೋಗು <= ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು + g.len)
        {
          write_ಹೊರಹೋಗು (ಸಾಲು_ಹೊರಹೋಗು, ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು, /*newline=*/ false);
          ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು = 0;
        }

      ಸ್ಮರಣೆ_ನಕಲು (ಸಾಲು_ಹೊರಹೋಗು + ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು, mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), g.len);
      ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು += g.len;
    }

  saved_errno = errno;
  ಆದರೆ (!ferror (istream))
    saved_errno = 0;

  ಆದರೆ (ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು)
    write_ಹೊರಹೋಗು (ಸಾಲು_ಹೊರಹೋಗು, ಸ್ಥಾನಾಂತರ_ಹೊರಹೋಗು, /*newline=*/ false);

  ಆದರೆ (streq (filename, "-"))
    clearerr (istream);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ಆದರೆ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  size_t ಅಗಲ = 80;
  ಪೂರ್ಣಾಂಕ optc;
  bool ok;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, shortopts, longopts, ಖಾಲಿ)) != -1)
    {
      ಅಕ್ಷರ optargbuf[2];

      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'b':		/* Count bytes rather than columns. */
          counting_ಬಹುಲಕ = ಎಣಿಕೆ_ಬೈಟುಗಳು;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':               /* Count characters rather than columns. */
          counting_ಬಹುಲಕ = ಎಣಿಕೆ_CHARACTERS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':		/* Break at word boundaries. */
          break_spaces = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
        ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
          ಆದರೆ (optarg)
            optarg--;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        ಸ್ಥಿತಿ 'w':		/* Line width. */
          ಅಗಲ = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_ಅಗಲ - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (argc == optind)
    ok = fold_file ("-", ಅಗಲ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ok = true;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; i++)
        ok &= fold_file (argv[i], ಅಗಲ);
    }

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
