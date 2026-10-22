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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TAB_ਚੌੜਾਈ 8

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "fold"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
ਸਥਿਰ bool break_spaces;

/* Mode to operate in.  */
ਸਥਿਰ ਗਿਣਤੀ
  {
    ਗਿਣਤੀ_COLUMNS,
    ਗਿਣਤੀ_ਬਾਈਟਾਂ,
    ਗਿਣਤੀ_CHARACTERS
  } counting_ਬਹੁਲਕ = ਗਿਣਤੀ_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
ਸਥਿਰ bool have_read_stdin;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"bytes", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"characters", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"spaces", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Wrap ਨਿਵੇਸ਼ ਸਤਰਾਂ ਵਿੱਚ each FILE, writing ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --ਬਾਈਟਾਂ\n\
         ਗਿਣਤੀ_2 ਬਾਈਟਾਂ rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         ਗਿਣਤੀ_2 characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ਰੋਕੋ after blanks, or ਵਿੱਚ ਸ਼ਬਦ_4 greater than ਚੌੜਾਈ_2\n\
"));
      oputs (_("\
  -w, --ਚੌੜਾਈ=ਚੌੜਾਈ_2\n\
         use ਚੌੜਾਈ_2 columns instead ਦਾ 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

ਸਥਿਰ size_t
adjust_ਕਾਲਮ (size_t ਕਾਲਮ, mcel_t g)
{
  ਜੇ (counting_ਬਹੁਲਕ != ਗਿਣਤੀ_ਬਾਈਟਾਂ)
    {
      ਜੇ (g.ch == '\b')
        ਕਾਲਮ -= !!ਕਾਲਮ;
      ਨਹੀਂਤਾਂ ਜੇ (g.ch == '\r')
        ਕਾਲਮ = 0;
      ਨਹੀਂਤਾਂ ਜੇ (g.ch == '\t')
        ਕਾਲਮ += TAB_ਚੌੜਾਈ - ਕਾਲਮ % TAB_ਚੌੜਾਈ;
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ ਚੌੜਾਈ = (counting_ਬਹੁਲਕ == ਗਿਣਤੀ_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          ਕਾਲਮ += ਚੌੜਾਈ < 0 ? 1 : ਚੌੜਾਈ;
        }
    }
  ਨਹੀਂਤਾਂ
    ਕਾਲਮ += g.len;
  ਵਾਪਸ ਕਾਲਮ;
}

ਸਥਿਰ ਖਾਲੀ
write_ਬਾਹਰਲਾ (ਅੱਖਰ ਅਚਲ *ਸਤਰ, size_t ਸਤਰ_len, bool newline)
{
  ਜੇ (fwrite (ਸਤਰ, ਆਕਾਰ (ਅੱਖਰ), ਸਤਰ_len, stdout) != ਸਤਰ_len
      || (newline && ਅੱਖਰ_ਲਿਖੋ ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

ਸਥਿਰ bool
fold_file (ਅੱਖਰ ਅਚਲ *filename, size_t ਚੌੜਾਈ)
{
  FILE *istream;
  size_t ਕਾਲਮ = 0;		/* Screen column where next char will go. */
  idx_t ਵਿੱਥ_ਬਾਹਰਲਾ = 0;		/* Index in 'line_out' for next char. */
  ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਬਾਹਰਲਾ[IO_BUFSIZE];
  ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਵਿੱਚ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  ਪੂਰਨ saved_errno;

  ਜੇ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  ਨਹੀਂਤਾਂ
    istream = fopen (filename, "r");

  ਜੇ (istream == ਨਿਰਾ)
    {
      error (0, errno, "%s", quotef (filename));
      ਵਾਪਸ false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, ਸਤਰ_ਵਿੱਚ, ਆਕਾਰ ਸਤਰ_ਵਿੱਚ, istream);

  mcel_t g;
  ਜਦੋਂ ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ਜੇ (g.ch == '\n')
        {
          write_ਬਾਹਰਲਾ (ਸਤਰ_ਬਾਹਰਲਾ, ਵਿੱਥ_ਬਾਹਰਲਾ, /*newline=*/ true);
          ਕਾਲਮ = ਵਿੱਥ_ਬਾਹਰਲਾ = 0;
          ਜਾਰੀ;
        }
    rescan:
      ਕਾਲਮ = adjust_ਕਾਲਮ (ਕਾਲਮ, g);

      ਜੇ (ਕਾਲਮ > ਚੌੜਾਈ)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ਜੇ (break_spaces)
            {
              ਪੂਰਨ space_ਲੰਮਾਈ = 0;
              idx_t logical_ਸਿਰਾ = ਵਿੱਥ_ਬਾਹਰਲਾ;
              ਅੱਖਰ *logical_p = ਸਤਰ_ਬਾਹਰਲਾ;
              ਅੱਖਰ *logical_lim = logical_p + logical_ਸਿਰਾ;

              ਲਈ (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_ਜਾਂਚ (logical_p, logical_lim);
                  ਜੇ (c32issep (g2.ch))
                    {
                      space_ਲੰਮਾਈ = g2.len;
                      logical_ਸਿਰਾ = logical_p - ਸਤਰ_ਬਾਹਰਲਾ;
                    }
                }

              ਜੇ (space_ਲੰਮਾਈ)
                {
                  logical_ਸਿਰਾ += space_ਲੰਮਾਈ;
                  /* Found a blank.  Don't output the part after it. */
                  write_ਬਾਹਰਲਾ (ਸਤਰ_ਬਾਹਰਲਾ, logical_ਸਿਰਾ, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (ਸਤਰ_ਬਾਹਰਲਾ, ਸਤਰ_ਬਾਹਰਲਾ + logical_ਸਿਰਾ,
                           ਵਿੱਥ_ਬਾਹਰਲਾ - logical_ਸਿਰਾ);
                  ਵਿੱਥ_ਬਾਹਰਲਾ -= logical_ਸਿਰਾ;
                  ਕਾਲਮ = 0;
                  ਅੱਖਰ *printed_p = ਸਤਰ_ਬਾਹਰਲਾ;
                  ਅੱਖਰ *printed_lim = printed_p + ਵਿੱਥ_ਬਾਹਰਲਾ;
                  ਲਈ (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_ਜਾਂਚ (printed_p, printed_lim);
                      ਕਾਲਮ = adjust_ਕਾਲਮ (ਕਾਲਮ, g2);
                    }
                  ਜਾਓ rescan;
                }
            }

          ਜੇ (ਵਿੱਥ_ਬਾਹਰਲਾ == 0)
            {
              ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਸਤਰ_ਬਾਹਰਲਾ, mbbuf_char_ਵਿੱਥ (&mbbuf, g), g.len);
              ਵਿੱਥ_ਬਾਹਰਲਾ += g.len;
              ਜਾਰੀ;
            }

          write_ਬਾਹਰਲਾ (ਸਤਰ_ਬਾਹਰਲਾ, ਵਿੱਥ_ਬਾਹਰਲਾ, /*newline=*/ true);
          ਕਾਲਮ = ਵਿੱਥ_ਬਾਹਰਲਾ = 0;
          ਜਾਓ rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ਜੇ (ਆਕਾਰ ਸਤਰ_ਬਾਹਰਲਾ <= ਵਿੱਥ_ਬਾਹਰਲਾ + g.len)
        {
          write_ਬਾਹਰਲਾ (ਸਤਰ_ਬਾਹਰਲਾ, ਵਿੱਥ_ਬਾਹਰਲਾ, /*newline=*/ false);
          ਵਿੱਥ_ਬਾਹਰਲਾ = 0;
        }

      ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਸਤਰ_ਬਾਹਰਲਾ + ਵਿੱਥ_ਬਾਹਰਲਾ, mbbuf_char_ਵਿੱਥ (&mbbuf, g), g.len);
      ਵਿੱਥ_ਬਾਹਰਲਾ += g.len;
    }

  saved_errno = errno;
  ਜੇ (!ferror (istream))
    saved_errno = 0;

  ਜੇ (ਵਿੱਥ_ਬਾਹਰਲਾ)
    write_ਬਾਹਰਲਾ (ਸਤਰ_ਬਾਹਰਲਾ, ਵਿੱਥ_ਬਾਹਰਲਾ, /*newline=*/ false);

  ਜੇ (streq (filename, "-"))
    clearerr (istream);
  ਨਹੀਂਤਾਂ ਜੇ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ਜੇ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  size_t ਚੌੜਾਈ = 80;
  ਪੂਰਨ optc;
  bool ok;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, shortopts, longopts, ਨਿਰਾ)) != -1)
    {
      ਅੱਖਰ optargbuf[2];

      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'b':		/* Count bytes rather than columns. */
          counting_ਬਹੁਲਕ = ਗਿਣਤੀ_ਬਾਈਟਾਂ;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':               /* Count characters rather than columns. */
          counting_ਬਹੁਲਕ = ਗਿਣਤੀ_CHARACTERS;
          ਰੋਕੋ;

        ਹਾਲਤ 's':		/* Break at word boundaries. */
          break_spaces = true;
          ਰੋਕੋ;

        ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3': ਹਾਲਤ '4':
        ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7': ਹਾਲਤ '8': ਹਾਲਤ '9':
          ਜੇ (optarg)
            optarg--;
          ਨਹੀਂਤਾਂ
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        ਹਾਲਤ 'w':		/* Line width. */
          ਚੌੜਾਈ = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_ਚੌੜਾਈ - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (argc == optind)
    ok = fold_file ("-", ਚੌੜਾਈ);
  ਨਹੀਂਤਾਂ
    {
      ok = true;
      ਲਈ (ਪੂਰਨ i = optind; i < argc; i++)
        ok &= fold_file (argv[i], ਚੌੜਾਈ);
    }

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
