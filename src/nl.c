/* nl -- number lines of files
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by Scott Bartram (nancy!scott@uunet.uu.net)
   Revised by David MacKenzie (djm@gnu.ai.mit.edu) */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "nl"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ FORMAT_ಬಲ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ FORMAT_ಬಲ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ FORMAT_ಎಡ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ಸ್ಥಿರ ಅಕ್ಷರ DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ಎಣಿಕೆ section
{
  ಶೀರ್ಷಿಕೆ_2, Body, Footer, ಪಠ್ಯ_2
};

/* Format of body lines (-b).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *body_type = "t";

/* Format of header lines (-h).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಶೀರ್ಷಿಕೆ_type = "n";

/* Format of footer lines (-f).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *current_type;

/* Regex for body lines to number (-bp).  */
ಸ್ಥಿರ ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ body_regex;

/* Regex for header lines to number (-hp).  */
ಸ್ಥಿರ ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ ಶೀರ್ಷಿಕೆ_regex;

/* Regex for footer lines to number (-fp).  */
ಸ್ಥಿರ ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ footer_regex;

/* Fastmaps for the above.  */
ಸ್ಥಿರ ಅಕ್ಷರ body_fastmap[UCHAR_MAX + 1];
ಸ್ಥಿರ ಅಕ್ಷರ ಶೀರ್ಷಿಕೆ_fastmap[UCHAR_MAX + 1];
ಸ್ಥಿರ ಅಕ್ಷರ footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ಸ್ಥಿರ ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ *current_regex = ಖಾಲಿ;

/* Separator string to print after line number (-s).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *separator_str = "\t";

/* Input section delimiter string (-d).  */
ಸ್ಥಿರ ಅಕ್ಷರ *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ಸ್ಥಿರ size_t section_del_len;

/* Header delimiter string.  */
ಸ್ಥಿರ ಅಕ್ಷರ *ಶೀರ್ಷಿಕೆ_del = ಖಾಲಿ;

/* Header section delimiter length.  */
ಸ್ಥಿರ size_t ಶೀರ್ಷಿಕೆ_del_len;

/* Body delimiter string.  */
ಸ್ಥಿರ ಅಕ್ಷರ *body_del = ಖಾಲಿ;

/* Body section delimiter length.  */
ಸ್ಥಿರ size_t body_del_len;

/* Footer delimiter string.  */
ಸ್ಥಿರ ಅಕ್ಷರ *footer_del = ಖಾಲಿ;

/* Footer section delimiter length.  */
ಸ್ಥಿರ size_t footer_del_len;

/* Input buffer.  */
ಸ್ಥಿರ ರಚನೆ linebuffer ಸಾಲು_buf;

/* printf format string for unnumbered lines.  */
ಸ್ಥಿರ ಅಕ್ಷರ *print_no_ಸಾಲು_fmt = ಖಾಲಿ;

/* Starting line number on each page (-v).  */
ಸ್ಥಿರ intmax_t starting_ಸಾಲು_ಸಂಖ್ಯೆ = 1;

/* Line number increment (-i).  */
ಸ್ಥಿರ intmax_t ಪುಟ_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ಸ್ಥಿರ bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ಸ್ಥಿರ intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ lineno_ಅಗಲ = 6;

/* Line number format (-n).  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lineno_format = FORMAT_ಬಲ_NOLZ;

/* Current print line number.  */
ಸ್ಥಿರ intmax_t ಸಾಲು_no;

/* Whether the current line number has incremented past limits.  */
ಸ್ಥಿರ bool ಸಾಲು_no_ಅತಿಪ್ರವಾಹ;

/* True if we have ever read standard input.  */
ಸ್ಥಿರ bool have_read_stdin;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"header-numbering", required_ವಾದ, ಖಾಲಿ, 'h'},
  {"body-numbering", required_ವಾದ, ಖಾಲಿ, 'b'},
  {"footer-numbering", required_ವಾದ, ಖಾಲಿ, 'f'},
  {"starting-line-number", required_ವಾದ, ಖಾಲಿ, 'v'},
  {"line-increment", required_ವಾದ, ಖಾಲಿ, 'i'},
  {"no-renumber", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"join-blank-lines", required_ವಾದ, ಖಾಲಿ, 'l'},
  {"number-separator", required_ವಾದ, ಖಾಲಿ, 's'},
  {"number-width", required_ವಾದ, ಖಾಲಿ, 'w'},
  {"number-format", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"section-delimiter", required_ವಾದ, ಖಾಲಿ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Print a usage message and quit. */

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
Write each FILE ಗಮ್ಯ standard output, with ಸಾಲು numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE ಗಾಗಿ numbering body ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC ಗಾಗಿ logical ಪುಟ delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE ಗಾಗಿ numbering footer ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -h, --ಶೀರ್ಷಿಕೆ-numbering=STYLE    use STYLE ಗಾಗಿ numbering ಶೀರ್ಷಿಕೆ ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -i, --ಸಾಲು-increment=ಸಂಖ್ಯೆ_2     ಸಾಲು ಸಂಖ್ಯೆ increment at each ಸಾಲು\n\
"));
      oputs (_("\
  -l, --join-blank-ಸಾಲುಗಳು=ಸಂಖ್ಯೆ_2   ಗುಂಪು ದ ಸಂಖ್ಯೆ_2 ಖಾಲಿಗಳು ಸಾಲುಗಳು counted as one\n\
"));
      oputs (_("\
  -n, --ಸಂಖ್ಯೆ-format=FORMAT      ಸೇರಿಸು ಸಾಲು numbers according ಗಮ್ಯ FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               ಮಾಡು not reset ಸಾಲು numbers ಗಾಗಿ each section\n\
"));
      oputs (_("\
  -s, --ಸಂಖ್ಯೆ-separator=STRING   ಕೂಡಿಸು STRING after (possible) ಸಾಲು ಸಂಖ್ಯೆ\n\
"));
      oputs (_("\
  -v, --starting-ಸಾಲು-ಸಂಖ್ಯೆ=ಸಂಖ್ಯೆ_2  ಮೊದಲ ಸಾಲು ಸಂಖ್ಯೆ ಗಾಗಿ each section\n\
"));
      oputs (_("\
  -w, --ಸಂಖ್ಯೆ-ಅಗಲ=ಸಂಖ್ಯೆ_2       use ಸಂಖ್ಯೆ_2 columns ಗಾಗಿ ಸಾಲು numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters ಬಳಸಿದ ಗಮ್ಯ construct logical ಪುಟ delimiters;\n\
a missing ಎರಡನೇ character ಸೂಚಿಸುತ್ತದೆ ':'.  As a GNU extension one can specify\n\
ಇನ್ನಷ್ಟು than two characters, and also specifying the ಖಾಲಿಗಳು string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one ದ:\n\
\n\
  a      ಸಂಖ್ಯೆ all ಸಾಲುಗಳು\n\
  t      ಸಂಖ್ಯೆ only nonempty ಸಾಲುಗಳು\n\
  n      ಸಂಖ್ಯೆ no ಸಾಲುಗಳು\n\
  pBRE   ಸಂಖ್ಯೆ only ಸಾಲುಗಳು that contain a ಹೊಂದಿಕೆ ಗಾಗಿ the basic regular\n\
         ಅಭಿವ್ಯಕ್ತಿ_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one ದ:\n\
\n\
  ln     ಎಡ justified, no leading zeros\n\
  rn     ಬಲ justified, no leading zeros\n\
  rz     ಬಲ justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ಸ್ಥಿರ bool
build_type_arg (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **typep,
                ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ *regexp, ಅಕ್ಷರ *fastmap)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *errmsg;
  bool rval = true;

  ಆಯ್ಕೆ (*optarg)
    {
    ಸ್ಥಿತಿ 'a':
    ಸ್ಥಿತಿ 't':
    ಸ್ಥಿತಿ 'n':
      *typep = optarg;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'p':
      *typep = optarg++;
      regexp->ಬಫರ್ = ಖಾಲಿ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ಖಾಲಿ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ಖಾಲಿಗಳು_RANGES;
      errmsg = re_compile_ನಮೂನೆ (optarg, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg), regexp);
      ಆದರೆ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      rval = false;
      ನಿಲ್ಲಿಸು;
    }
  ಹಿಂತಿರುಗಿ rval;
}

/* Print the line number and separator; increment the line number. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_lineno (ಶೂನ್ಯ)
{
  ಆದರೆ (ಸಾಲು_no_ಅತಿಪ್ರವಾಹ)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  ಬರೆ (lineno_format, lineno_ಅಗಲ, ಸಾಲು_no, separator_str);

  ಆದರೆ (ckd_ಕೂಡಿಸು (&ಸಾಲು_no, ಸಾಲು_no, ಪುಟ_incr))
    ಸಾಲು_no_ಅತಿಪ್ರವಾಹ = true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
reset_lineno (ಶೂನ್ಯ)
{
  ಆದರೆ (reset_numbers)
    {
      ಸಾಲು_no = starting_ಸಾಲು_ಸಂಖ್ಯೆ;
      ಸಾಲು_no_ಅತಿಪ್ರವಾಹ = false;
    }
}

/* Switch to a header section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_ಶೀರ್ಷಿಕೆ (ಶೂನ್ಯ)
{
  current_type = ಶೀರ್ಷಿಕೆ_type;
  current_regex = &ಶೀರ್ಷಿಕೆ_regex;
  reset_lineno ();
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Switch to a body section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_body (ಶೂನ್ಯ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Switch to a footer section. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_footer (ಶೂನ್ಯ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  ಅಕ್ಷರ_ಬರೆ ('\n');
}

/* Process a regular text line in 'line_buf'. */

ಸ್ಥಿರ ಶೂನ್ಯ
proc_ಪಠ್ಯ (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ intmax_t blank_ಸಾಲುಗಳು = 0;	/* Consecutive blank lines so far. */

  ಆಯ್ಕೆ (*current_type)
    {
    ಸ್ಥಿತಿ 'a':
      ಆದರೆ (blank_join > 1)
        {
          ಆದರೆ (1 < ಸಾಲು_buf.ಉದ್ದಳತೆ || ++blank_ಸಾಲುಗಳು == blank_join)
            {
              print_lineno ();
              blank_ಸಾಲುಗಳು = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            fputs (print_no_ಸಾಲು_fmt, stdout);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        print_lineno ();
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 't':
      ಆದರೆ (1 < ಸಾಲು_buf.ಉದ್ದಳತೆ)
        print_lineno ();
      ಇಲ್ಲದಿದ್ದರೆ
        fputs (print_no_ಸಾಲು_fmt, stdout);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'n':
      fputs (print_no_ಸಾಲು_fmt, stdout);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'p':
      ಆಯ್ಕೆ (re_ಹುಡುಕು (current_regex, ಸಾಲು_buf.ಬಫರ್, ಸಾಲು_buf.ಉದ್ದಳತೆ - 1,
                         0, ಸಾಲು_buf.ಉದ್ದಳತೆ - 1, ಖಾಲಿ))
        {
        ಸ್ಥಿತಿ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        ಸ್ಥಿತಿ -1:
          fputs (print_no_ಸಾಲು_fmt, stdout);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          print_lineno ();
          ನಿಲ್ಲಿಸು;
        }
    }
  fwrite (ಸಾಲು_buf.ಬಫರ್, ಗಾತ್ರ (ಅಕ್ಷರ), ಸಾಲು_buf.ಉದ್ದಳತೆ, stdout);
}

/* Return the type of line in 'line_buf'. */

ಸ್ಥಿರ ಎಣಿಕೆ section
check_section (ಶೂನ್ಯ)
{
  size_t len = ಸಾಲು_buf.ಉದ್ದಳತೆ - 1;

  ಆದರೆ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ಸಾಲು_buf.ಬಫರ್, section_del, section_del_len))
    ಹಿಂತಿರುಗಿ ಪಠ್ಯ_2;
  ಆದರೆ (len == ಶೀರ್ಷಿಕೆ_del_len
      && memeq (ಸಾಲು_buf.ಬಫರ್, ಶೀರ್ಷಿಕೆ_del, ಶೀರ್ಷಿಕೆ_del_len))
    ಹಿಂತಿರುಗಿ ಶೀರ್ಷಿಕೆ_2;
  ಆದರೆ (len == body_del_len
      && memeq (ಸಾಲು_buf.ಬಫರ್, body_del, body_del_len))
    ಹಿಂತಿರುಗಿ Body;
  ಆದರೆ (len == footer_del_len
      && memeq (ಸಾಲು_buf.ಬಫರ್, footer_del, footer_del_len))
    ಹಿಂತಿರುಗಿ Footer;
  ಹಿಂತಿರುಗಿ ಪಠ್ಯ_2;
}

/* Read and process the file pointed to by FP. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_file (FILE *fp)
{
  ಆಗ (readlinebuffer (&ಸಾಲು_buf, fp))
    {
      ಆಯ್ಕೆ (check_section ())
        {
        ಸ್ಥಿತಿ ಶೀರ್ಷಿಕೆ_2:
          proc_ಶೀರ್ಷಿಕೆ ();
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ Body:
          proc_body ();
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ Footer:
          proc_footer ();
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ಪಠ್ಯ_2:
          proc_ಪಠ್ಯ ();
          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ಸ್ಥಿರ bool
nl_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  FILE *ಹರಿವು;

  ಆದರೆ (streq (file, "-"))
    {
      have_read_stdin = true;
      ಹರಿವು = stdin;
      assume (ಹರಿವು);  /* Pacify GCC bug#109613.  */
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಹರಿವು = fopen (file, "r");
      ಆದರೆ (ಹರಿವು == ಖಾಲಿ)
        {
          error (0, errno, "%s", quotef (file));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  fadvise (ಹರಿವು, FADVISE_SEQUENTIAL);

  process_file (ಹರಿವು);

  ಪೂರ್ಣಾಂಕ err = errno;
  ಆದರೆ (!ferror (ಹರಿವು))
    err = 0;
  ಆದರೆ (streq (file, "-"))
    clearerr (ಹರಿವು);		/* Also clear EOF. */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fclose (ಹರಿವು) != 0 && !err)
    err = errno;
  ಆದರೆ (err)
    {
      error (0, err, "%s", quotef (file));
      ಹಿಂತಿರುಗಿ false;
    }
  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'h':
          ಆದರೆ (! build_type_arg (&ಶೀರ್ಷಿಕೆ_type, &ಶೀರ್ಷಿಕೆ_regex, ಶೀರ್ಷಿಕೆ_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'b':
          ಆದರೆ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          ಆದರೆ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          starting_ಸಾಲು_ಸಂಖ್ಯೆ = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          ಪುಟ_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'p':
          reset_numbers = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          separator_str = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'w':
          lineno_ಅಗಲ = xnumtoimax (optarg, 10, 1, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          ಆದರೆ (streq (optarg, "ln"))
            lineno_format = FORMAT_ಎಡ;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (optarg, "rn"))
            lineno_format = FORMAT_ಬಲ_NOLZ;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (optarg, "rz"))
            lineno_format = FORMAT_ಬಲ_LZ;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'd':
          len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (optarg);
          ಆದರೆ (1 < MB_CUR_MAX)
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = optarg;
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lim = p + len;
              ಪೂರ್ಣಾಂಕ n_chars = 0;
              ಗಾಗಿ (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_ಪರಿಶೀಲನೆ (p, lim).len;
              ಆದರೆ (n_chars == 1)
                ಸ್ಮರಣೆ_ನಕಲು (mempcpy (section_del, optarg, len),  ":", ಗಾತ್ರ ":");
              ಇಲ್ಲದಿದ್ದರೆ
                section_del = optarg;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (len == 1)
                *section_del = *optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                section_del = optarg;
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          ok = false;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (section_del);

  ಶೀರ್ಷಿಕೆ_del_len = len * 3;
  ಶೀರ್ಷಿಕೆ_del = xmalloc (ಶೀರ್ಷಿಕೆ_del_len + 1);
  stpcpy (stpcpy (stpcpy (ಶೀರ್ಷಿಕೆ_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = ಶೀರ್ಷಿಕೆ_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&ಸಾಲು_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (separator_str);
  print_no_ಸಾಲು_fmt = xmalloc (lineno_ಅಗಲ + len + 1);
  memset (print_no_ಸಾಲು_fmt, ' ', lineno_ಅಗಲ + len);
  print_no_ಸಾಲು_fmt[lineno_ಅಗಲ + len] = '\0';

  ಸಾಲು_no = starting_ಸಾಲು_ಸಂಖ್ಯೆ;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ಆದರೆ (optind == argc)
    ok = nl_file ("-");
  ಇಲ್ಲದಿದ್ದರೆ
    ಗಾಗಿ (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
