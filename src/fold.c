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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "mbbuf.h"
#అన్తర్భావ "xdectoint.h"

#నిర్వచించు TAB_వెడల్పు 8

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "fold"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

/* If nonzero, try to break on whitespace. */
స్థిర bool break_spaces;

/* Mode to operate in.  */
స్థిర గణన
  {
    లెక్క_COLUMNS,
    లెక్క_బైట్లు,
    లెక్క_CHARACTERS
  } counting_బహుళకం = లెక్క_COLUMNS;

/* If nonzero, at least one of the files we read was standard input. */
స్థిర bool have_read_stdin;

స్థిర అక్షరం స్థిరరాశి shortopts[] = "bcsw:0::1::2::3::4::5::6::7::8::9::";

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"bytes", no_వాదం, ఖాళీ, 'b'},
  {"characters", no_వాదం, ఖాళీ, 'c'},
  {"spaces", no_వాదం, ఖాళీ, 's'},
  {"width", required_వాదం, ఖాళీ, 'w'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Wrap ఆదానం పంక్తులు లోపల each FILE, writing గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --బైట్లు\n\
         లెక్క బైట్లు rather than columns\n\
"));
      oputs (_("\
  -c, --characters\n\
         లెక్క characters rather than columns\n\
"));
      oputs (_("\
  -s, --spaces\n\
         ఆపు after blanks, or లోపల పదాలు greater than వెడల్పు_2\n\
"));
      oputs (_("\
  -w, --వెడల్పు=వెడల్పు_2\n\
         use వెడల్పు_2 columns instead యొక్క 80\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Assuming the current column is COLUMN, return the column that
   printing C will move the cursor to.
   The first column is 0. */

స్థిర size_t
adjust_స్తంభం (size_t స్తంభం, mcel_t g)
{
  ఒకవేళ (counting_బహుళకం != లెక్క_బైట్లు)
    {
      ఒకవేళ (g.ch == '\b')
        స్తంభం -= !!స్తంభం;
      లేకపోతే ఒకవేళ (g.ch == '\r')
        స్తంభం = 0;
      లేకపోతే ఒకవేళ (g.ch == '\t')
        స్తంభం += TAB_వెడల్పు - స్తంభం % TAB_వెడల్పు;
      లేకపోతే
        {
          పూర్ణాంకం వెడల్పు = (counting_బహుళకం == లెక్క_CHARACTERS
                       ? 1 : c32width (g.ch));
          /* Default to a width of 1 if there is an invalid character.  */
          స్తంభం += వెడల్పు < 0 ? 1 : వెడల్పు;
        }
    }
  లేకపోతే
    స్తంభం += g.len;
  తిరిగి స్తంభం;
}

స్థిర శూన్య
write_బహిర్గతం (అక్షరం స్థిరరాశి *పంక్తి, size_t పంక్తి_len, bool newline)
{
  ఒకవేళ (fwrite (పంక్తి, పరిమాణం (అక్షరం), పంక్తి_len, stdout) != పంక్తి_len
      || (newline && అక్షరం_రాయి ('\n') < 0))
    write_error ();
}

/* Fold file FILENAME, or standard input if FILENAME is "-",
   to stdout, with maximum line length WIDTH.
   Return true if successful.  */

స్థిర bool
fold_file (అక్షరం స్థిరరాశి *filename, size_t వెడల్పు)
{
  FILE *istream;
  size_t స్తంభం = 0;		/* Screen column where next char will go. */
  idx_t స్థానభ్రంశం_బహిర్గతం = 0;		/* Index in 'line_out' for next char. */
  స్థిర అక్షరం పంక్తి_బహిర్గతం[IO_BUFSIZE];
  స్థిర అక్షరం పంక్తి_లోపల[IO_BUFSIZE];
  mbbuf_t mbbuf;
  పూర్ణాంకం saved_errno;

  ఒకవేళ (streq (filename, "-"))
    {
      istream = stdin;
      have_read_stdin = true;
    }
  లేకపోతే
    istream = fopen (filename, "r");

  ఒకవేళ (istream == ఖాళీ)
    {
      error (0, errno, "%s", quotef (filename));
      తిరిగి false;
    }

  fadvise (istream, FADVISE_SEQUENTIAL);
  mbbuf_init (&mbbuf, పంక్తి_లోపల, పరిమాణం పంక్తి_లోపల, istream);

  mcel_t g;
  అయితే ((g = mbbuf_get_char (&mbbuf)).ch != MBBUF_EOF)
    {
      ఒకవేళ (g.ch == '\n')
        {
          write_బహిర్గతం (పంక్తి_బహిర్గతం, స్థానభ్రంశం_బహిర్గతం, /*newline=*/ true);
          స్తంభం = స్థానభ్రంశం_బహిర్గతం = 0;
          కొనసాగు;
        }
    rescan:
      స్తంభం = adjust_స్తంభం (స్తంభం, g);

      ఒకవేళ (స్తంభం > వెడల్పు)
        {
          /* This character would make the line too long.
             Print the line plus a newline, and make this character
             start the next line. */
          ఒకవేళ (break_spaces)
            {
              పూర్ణాంకం space_నిడివి = 0;
              idx_t logical_చివర = స్థానభ్రంశం_బహిర్గతం;
              అక్షరం *logical_p = పంక్తి_బహిర్గతం;
              అక్షరం *logical_lim = logical_p + logical_చివర;

              కొరకు (mcel_t g2; logical_p < logical_lim; logical_p += g2.len)
                {
                  g2 = mcel_పరిశీలన (logical_p, logical_lim);
                  ఒకవేళ (c32issep (g2.ch))
                    {
                      space_నిడివి = g2.len;
                      logical_చివర = logical_p - పంక్తి_బహిర్గతం;
                    }
                }

              ఒకవేళ (space_నిడివి)
                {
                  logical_చివర += space_నిడివి;
                  /* Found a blank.  Don't output the part after it. */
                  write_బహిర్గతం (పంక్తి_బహిర్గతం, logical_చివర, /*newline=*/ true);
                  /* Move the remainder to the beginning of the next line.
                     The areas being copied here might overlap. */
                  memmove (పంక్తి_బహిర్గతం, పంక్తి_బహిర్గతం + logical_చివర,
                           స్థానభ్రంశం_బహిర్గతం - logical_చివర);
                  స్థానభ్రంశం_బహిర్గతం -= logical_చివర;
                  స్తంభం = 0;
                  అక్షరం *printed_p = పంక్తి_బహిర్గతం;
                  అక్షరం *printed_lim = printed_p + స్థానభ్రంశం_బహిర్గతం;
                  కొరకు (mcel_t g2; printed_p < printed_lim;
                       printed_p += g2.len)
                    {
                      g2 = mcel_పరిశీలన (printed_p, printed_lim);
                      స్తంభం = adjust_స్తంభం (స్తంభం, g2);
                    }
                  వెళ్ళు rescan;
                }
            }

          ఒకవేళ (స్థానభ్రంశం_బహిర్గతం == 0)
            {
              జ్ఞాపకం_నకలు (పంక్తి_బహిర్గతం, mbbuf_char_స్థానభ్రంశం (&mbbuf, g), g.len);
              స్థానభ్రంశం_బహిర్గతం += g.len;
              కొనసాగు;
            }

          write_బహిర్గతం (పంక్తి_బహిర్గతం, స్థానభ్రంశం_బహిర్గతం, /*newline=*/ true);
          స్తంభం = స్థానభ్రంశం_బహిర్గతం = 0;
          వెళ్ళు rescan;
        }

      /* This can occur if we have read characters with a width of
         zero.  */
      ఒకవేళ (పరిమాణం పంక్తి_బహిర్గతం <= స్థానభ్రంశం_బహిర్గతం + g.len)
        {
          write_బహిర్గతం (పంక్తి_బహిర్గతం, స్థానభ్రంశం_బహిర్గతం, /*newline=*/ false);
          స్థానభ్రంశం_బహిర్గతం = 0;
        }

      జ్ఞాపకం_నకలు (పంక్తి_బహిర్గతం + స్థానభ్రంశం_బహిర్గతం, mbbuf_char_స్థానభ్రంశం (&mbbuf, g), g.len);
      స్థానభ్రంశం_బహిర్గతం += g.len;
    }

  saved_errno = errno;
  ఒకవేళ (!ferror (istream))
    saved_errno = 0;

  ఒకవేళ (స్థానభ్రంశం_బహిర్గతం)
    write_బహిర్గతం (పంక్తి_బహిర్గతం, స్థానభ్రంశం_బహిర్గతం, /*newline=*/ false);

  ఒకవేళ (streq (filename, "-"))
    clearerr (istream);
  లేకపోతే ఒకవేళ (fclose (istream) != 0 && !saved_errno)
    saved_errno = errno;

  ఒకవేళ (saved_errno)
    {
      error (0, saved_errno, "%s", quotef (filename));
      తిరిగి false;
    }

  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  size_t వెడల్పు = 80;
  పూర్ణాంకం optc;
  bool ok;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, shortopts, longopts, ఖాళీ)) != -1)
    {
      అక్షరం optargbuf[2];

      ఎంపిక (optc)
        {
        స్థితి 'b':		/* Count bytes rather than columns. */
          counting_బహుళకం = లెక్క_బైట్లు;
          ఆపు;

        స్థితి 'c':               /* Count characters rather than columns. */
          counting_బహుళకం = లెక్క_CHARACTERS;
          ఆపు;

        స్థితి 's':		/* Break at word boundaries. */
          break_spaces = true;
          ఆపు;

        స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3': స్థితి '4':
        స్థితి '5': స్థితి '6': స్థితి '7': స్థితి '8': స్థితి '9':
          ఒకవేళ (optarg)
            optarg--;
          లేకపోతే
            {
              optargbuf[0] = optc;
              optargbuf[1] = '\0';
              optarg = optargbuf;
            }
          FALLTHROUGH;
        స్థితి 'w':		/* Line width. */
          వెడల్పు = xnumtoumax (optarg, 10, 1, SIZE_MAX - TAB_వెడల్పు - 1, "",
                              _("invalid number of columns"), 0,
                              XTOINT_MIN_RANGE | XTOINT_MAX_RANGE);
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (argc == optind)
    ok = fold_file ("-", వెడల్పు);
  లేకపోతే
    {
      ok = true;
      కొరకు (పూర్ణాంకం i = optind; i < argc; i++)
        ok &= fold_file (argv[i], వెడల్పు);
    }

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
