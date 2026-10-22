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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linebuffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "nl"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ FORMAT_ਸੱਜਾ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ FORMAT_ਸੱਜਾ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ FORMAT_ਖੱਬਾ[] = "%-*jd%s";

/* Default section delimiter characters.  */
ਸਥਿਰ ਅੱਖਰ DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
ਗਿਣਤੀ section
{
  ਸਿਰਲੇਖ_2, Body, Footer, ਪਾਠ_2
};

/* Format of body lines (-b).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *body_type = "t";

/* Format of header lines (-h).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਸਿਰਲੇਖ_type = "n";

/* Format of footer lines (-f).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *current_type;

/* Regex for body lines to number (-bp).  */
ਸਥਿਰ ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ body_regex;

/* Regex for header lines to number (-hp).  */
ਸਥਿਰ ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ ਸਿਰਲੇਖ_regex;

/* Regex for footer lines to number (-fp).  */
ਸਥਿਰ ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ footer_regex;

/* Fastmaps for the above.  */
ਸਥਿਰ ਅੱਖਰ body_fastmap[UCHAR_MAX + 1];
ਸਥਿਰ ਅੱਖਰ ਸਿਰਲੇਖ_fastmap[UCHAR_MAX + 1];
ਸਥਿਰ ਅੱਖਰ footer_fastmap[UCHAR_MAX + 1];

/* Pointer to current regex, if any.  */
ਸਥਿਰ ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ *current_regex = ਨਿਰਾ;

/* Separator string to print after line number (-s).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *separator_str = "\t";

/* Input section delimiter string (-d).  */
ਸਥਿਰ ਅੱਖਰ *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
ਸਥਿਰ size_t section_del_len;

/* Header delimiter string.  */
ਸਥਿਰ ਅੱਖਰ *ਸਿਰਲੇਖ_del = ਨਿਰਾ;

/* Header section delimiter length.  */
ਸਥਿਰ size_t ਸਿਰਲੇਖ_del_len;

/* Body delimiter string.  */
ਸਥਿਰ ਅੱਖਰ *body_del = ਨਿਰਾ;

/* Body section delimiter length.  */
ਸਥਿਰ size_t body_del_len;

/* Footer delimiter string.  */
ਸਥਿਰ ਅੱਖਰ *footer_del = ਨਿਰਾ;

/* Footer section delimiter length.  */
ਸਥਿਰ size_t footer_del_len;

/* Input buffer.  */
ਸਥਿਰ ਬਣਤਰ linebuffer ਸਤਰ_buf;

/* printf format string for unnumbered lines.  */
ਸਥਿਰ ਅੱਖਰ *print_no_ਸਤਰ_fmt = ਨਿਰਾ;

/* Starting line number on each page (-v).  */
ਸਥਿਰ intmax_t starting_ਸਤਰ_ਸੰਖਿਆ = 1;

/* Line number increment (-i).  */
ਸਥਿਰ intmax_t ਪੰਨਾ_incr = 1;

/* If true, reset line number at start of each page (-p).  */
ਸਥਿਰ bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
ਸਥਿਰ intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
ਸਥਿਰ ਪੂਰਨ lineno_ਚੌੜਾਈ = 6;

/* Line number format (-n).  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *lineno_format = FORMAT_ਸੱਜਾ_NOLZ;

/* Current print line number.  */
ਸਥਿਰ intmax_t ਸਤਰ_no;

/* Whether the current line number has incremented past limits.  */
ਸਥਿਰ bool ਸਤਰ_no_ਉਛਾਲ;

/* True if we have ever read standard input.  */
ਸਥਿਰ bool have_read_stdin;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"header-numbering", required_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"body-numbering", required_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"footer-numbering", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"starting-line-number", required_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"line-increment", required_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"no-renumber", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"join-blank-lines", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"number-separator", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"number-width", required_ਦਲੀਲ, ਨਿਰਾ, 'w'},
  {"number-format", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"section-delimiter", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Print a usage message and quit. */

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
Write each FILE ਮੰਜ਼ਿਲ standard output, with ਸਤਰ numbers added.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE ਲਈ numbering body ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC ਲਈ logical ਪੰਨਾ delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE ਲਈ numbering footer ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -h, --ਸਿਰਲੇਖ-numbering=STYLE    use STYLE ਲਈ numbering ਸਿਰਲੇਖ ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -i, --ਸਤਰ-increment=ਸੰਖਿਆ_2     ਸਤਰ ਸੰਖਿਆ increment at each ਸਤਰ\n\
"));
      oputs (_("\
  -l, --join-blank-ਸਤਰਾਂ=ਸੰਖਿਆ_2   ਸਮੂਹ ਦਾ ਸੰਖਿਆ_2 ਖਾਲੀ_ਥਾਵਾਂ ਸਤਰਾਂ counted as one\n\
"));
      oputs (_("\
  -n, --ਸੰਖਿਆ-format=FORMAT      ਪਾਓ ਸਤਰ numbers according ਮੰਜ਼ਿਲ FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               ਕਰੋ not reset ਸਤਰ numbers ਲਈ each section\n\
"));
      oputs (_("\
  -s, --ਸੰਖਿਆ-separator=STRING   ਜੋੜੋ STRING after (possible) ਸਤਰ ਸੰਖਿਆ\n\
"));
      oputs (_("\
  -v, --starting-ਸਤਰ-ਸੰਖਿਆ=ਸੰਖਿਆ_2  ਪਹਿਲਾ ਸਤਰ ਸੰਖਿਆ ਲਈ each section\n\
"));
      oputs (_("\
  -w, --ਸੰਖਿਆ-ਚੌੜਾਈ=ਸੰਖਿਆ_2       use ਸੰਖਿਆ_2 columns ਲਈ ਸਤਰ numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters ਵਰਤਿਆ ਮੰਜ਼ਿਲ construct logical ਪੰਨਾ delimiters;\n\
a missing ਦੂਜਾ character ਸਿੱਟਾ_ਦਿੰਦਾ ':'.  As a GNU extension one can specify\n\
ਹੋਰ than two characters, and also specifying the ਖਾਲੀ_ਥਾਵਾਂ string (-d '')\n\
disables section matching.\n\
"), stdout);
      fputs (_("\
\n\
STYLE is one ਦਾ:\n\
\n\
  a      ਸੰਖਿਆ all ਸਤਰਾਂ\n\
  t      ਸੰਖਿਆ only nonempty ਸਤਰਾਂ\n\
  n      ਸੰਖਿਆ no ਸਤਰਾਂ\n\
  pBRE   ਸੰਖਿਆ only ਸਤਰਾਂ that contain a ਮੇਲ ਲਈ the basic regular\n\
         ਵਿਅੰਜਕ_2, BRE\n\
"), stdout);
      fputs (_("\
\n\
FORMAT is one ਦਾ:\n\
\n\
  ln     ਖੱਬਾ justified, no leading zeros\n\
  rn     ਸੱਜਾ justified, no leading zeros\n\
  rz     ਸੱਜਾ justified, leading zeros\n\
\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

ਸਥਿਰ bool
build_type_arg (ਅੱਖਰ ਅਚਲ **typep,
                ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ *regexp, ਅੱਖਰ *fastmap)
{
  ਅੱਖਰ ਅਚਲ *errmsg;
  bool rval = true;

  ਚੋਣ (*optarg)
    {
    ਹਾਲਤ 'a':
    ਹਾਲਤ 't':
    ਹਾਲਤ 'n':
      *typep = optarg;
      ਰੋਕੋ;
    ਹਾਲਤ 'p':
      *typep = optarg++;
      regexp->ਬਫਰ = ਨਿਰਾ;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = ਨਿਰਾ;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ਖਾਲੀ_ਥਾਵਾਂ_RANGES;
      errmsg = re_compile_ਨਮੂਨਾ (optarg, ਲੜੀ_ਲੰਬਾਈ (optarg), regexp);
      ਜੇ (errmsg)
        error (EXIT_FAILURE, 0, "%s", (errmsg));
      ਰੋਕੋ;
    ਮੂਲ:
      rval = false;
      ਰੋਕੋ;
    }
  ਵਾਪਸ rval;
}

/* Print the line number and separator; increment the line number. */

ਸਥਿਰ ਖਾਲੀ
print_lineno (ਖਾਲੀ)
{
  ਜੇ (ਸਤਰ_no_ਉਛਾਲ)
    error (EXIT_FAILURE, 0, _("line number overflow"));

  ਲਿਖੋ (lineno_format, lineno_ਚੌੜਾਈ, ਸਤਰ_no, separator_str);

  ਜੇ (ckd_ਜੋੜੋ (&ਸਤਰ_no, ਸਤਰ_no, ਪੰਨਾ_incr))
    ਸਤਰ_no_ਉਛਾਲ = true;
}

ਸਥਿਰ ਖਾਲੀ
reset_lineno (ਖਾਲੀ)
{
  ਜੇ (reset_numbers)
    {
      ਸਤਰ_no = starting_ਸਤਰ_ਸੰਖਿਆ;
      ਸਤਰ_no_ਉਛਾਲ = false;
    }
}

/* Switch to a header section. */

ਸਥਿਰ ਖਾਲੀ
proc_ਸਿਰਲੇਖ (ਖਾਲੀ)
{
  current_type = ਸਿਰਲੇਖ_type;
  current_regex = &ਸਿਰਲੇਖ_regex;
  reset_lineno ();
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Switch to a body section. */

ਸਥਿਰ ਖਾਲੀ
proc_body (ਖਾਲੀ)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Switch to a footer section. */

ਸਥਿਰ ਖਾਲੀ
proc_footer (ਖਾਲੀ)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  ਅੱਖਰ_ਲਿਖੋ ('\n');
}

/* Process a regular text line in 'line_buf'. */

ਸਥਿਰ ਖਾਲੀ
proc_ਪਾਠ (ਖਾਲੀ)
{
  ਸਥਿਰ intmax_t blank_ਸਤਰਾਂ = 0;	/* Consecutive blank lines so far. */

  ਚੋਣ (*current_type)
    {
    ਹਾਲਤ 'a':
      ਜੇ (blank_join > 1)
        {
          ਜੇ (1 < ਸਤਰ_buf.ਲੰਮਾਈ || ++blank_ਸਤਰਾਂ == blank_join)
            {
              print_lineno ();
              blank_ਸਤਰਾਂ = 0;
            }
          ਨਹੀਂਤਾਂ
            fputs (print_no_ਸਤਰ_fmt, stdout);
        }
      ਨਹੀਂਤਾਂ
        print_lineno ();
      ਰੋਕੋ;
    ਹਾਲਤ 't':
      ਜੇ (1 < ਸਤਰ_buf.ਲੰਮਾਈ)
        print_lineno ();
      ਨਹੀਂਤਾਂ
        fputs (print_no_ਸਤਰ_fmt, stdout);
      ਰੋਕੋ;
    ਹਾਲਤ 'n':
      fputs (print_no_ਸਤਰ_fmt, stdout);
      ਰੋਕੋ;
    ਹਾਲਤ 'p':
      ਚੋਣ (re_ਖੋਜ (current_regex, ਸਤਰ_buf.ਬਫਰ, ਸਤਰ_buf.ਲੰਮਾਈ - 1,
                         0, ਸਤਰ_buf.ਲੰਮਾਈ - 1, ਨਿਰਾ))
        {
        ਹਾਲਤ -2:
          error (EXIT_FAILURE, errno, _("error in regular expression search"));

        ਹਾਲਤ -1:
          fputs (print_no_ਸਤਰ_fmt, stdout);
          ਰੋਕੋ;

        ਮੂਲ:
          print_lineno ();
          ਰੋਕੋ;
        }
    }
  fwrite (ਸਤਰ_buf.ਬਫਰ, ਆਕਾਰ (ਅੱਖਰ), ਸਤਰ_buf.ਲੰਮਾਈ, stdout);
}

/* Return the type of line in 'line_buf'. */

ਸਥਿਰ ਗਿਣਤੀ section
check_section (ਖਾਲੀ)
{
  size_t len = ਸਤਰ_buf.ਲੰਮਾਈ - 1;

  ਜੇ (len < section_del_len || footer_del_len < section_del_len
      || !memeq (ਸਤਰ_buf.ਬਫਰ, section_del, section_del_len))
    ਵਾਪਸ ਪਾਠ_2;
  ਜੇ (len == ਸਿਰਲੇਖ_del_len
      && memeq (ਸਤਰ_buf.ਬਫਰ, ਸਿਰਲੇਖ_del, ਸਿਰਲੇਖ_del_len))
    ਵਾਪਸ ਸਿਰਲੇਖ_2;
  ਜੇ (len == body_del_len
      && memeq (ਸਤਰ_buf.ਬਫਰ, body_del, body_del_len))
    ਵਾਪਸ Body;
  ਜੇ (len == footer_del_len
      && memeq (ਸਤਰ_buf.ਬਫਰ, footer_del, footer_del_len))
    ਵਾਪਸ Footer;
  ਵਾਪਸ ਪਾਠ_2;
}

/* Read and process the file pointed to by FP. */

ਸਥਿਰ ਖਾਲੀ
process_file (FILE *fp)
{
  ਜਦੋਂ (readlinebuffer (&ਸਤਰ_buf, fp))
    {
      ਚੋਣ (check_section ())
        {
        ਹਾਲਤ ਸਿਰਲੇਖ_2:
          proc_ਸਿਰਲੇਖ ();
          ਰੋਕੋ;
        ਹਾਲਤ Body:
          proc_body ();
          ਰੋਕੋ;
        ਹਾਲਤ Footer:
          proc_footer ();
          ਰੋਕੋ;
        ਹਾਲਤ ਪਾਠ_2:
          proc_ਪਾਠ ();
          ਰੋਕੋ;
        }

      ਜੇ (ferror (stdout))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

ਸਥਿਰ bool
nl_file (ਅੱਖਰ ਅਚਲ *file)
{
  FILE *ਧਾਰਾ;

  ਜੇ (streq (file, "-"))
    {
      have_read_stdin = true;
      ਧਾਰਾ = stdin;
      assume (ਧਾਰਾ);  /* Pacify GCC bug#109613.  */
    }
  ਨਹੀਂਤਾਂ
    {
      ਧਾਰਾ = fopen (file, "r");
      ਜੇ (ਧਾਰਾ == ਨਿਰਾ)
        {
          error (0, errno, "%s", quotef (file));
          ਵਾਪਸ false;
        }
    }

  fadvise (ਧਾਰਾ, FADVISE_SEQUENTIAL);

  process_file (ਧਾਰਾ);

  ਪੂਰਨ err = errno;
  ਜੇ (!ferror (ਧਾਰਾ))
    err = 0;
  ਜੇ (streq (file, "-"))
    clearerr (ਧਾਰਾ);		/* Also clear EOF. */
  ਨਹੀਂਤਾਂ ਜੇ (fclose (ਧਾਰਾ) != 0 && !err)
    err = errno;
  ਜੇ (err)
    {
      error (0, err, "%s", quotef (file));
      ਵਾਪਸ false;
    }
  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;
  size_t len;
  bool ok = true;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'h':
          ਜੇ (! build_type_arg (&ਸਿਰਲੇਖ_type, &ਸਿਰਲੇਖ_regex, ਸਿਰਲੇਖ_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ਰੋਕੋ;
        ਹਾਲਤ 'b':
          ਜੇ (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          ਜੇ (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          starting_ਸਤਰ_ਸੰਖਿਆ = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          ਪੰਨਾ_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          ਰੋਕੋ;
        ਹਾਲਤ 'p':
          reset_numbers = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          separator_str = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'w':
          lineno_ਚੌੜਾਈ = xnumtoimax (optarg, 10, 1, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          ਜੇ (streq (optarg, "ln"))
            lineno_format = FORMAT_ਖੱਬਾ;
          ਨਹੀਂਤਾਂ ਜੇ (streq (optarg, "rn"))
            lineno_format = FORMAT_ਸੱਜਾ_NOLZ;
          ਨਹੀਂਤਾਂ ਜੇ (streq (optarg, "rz"))
            lineno_format = FORMAT_ਸੱਜਾ_LZ;
          ਨਹੀਂਤਾਂ
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          ਰੋਕੋ;
        ਹਾਲਤ 'd':
          len = ਲੜੀ_ਲੰਬਾਈ (optarg);
          ਜੇ (1 < MB_CUR_MAX)
            {
              ਅੱਖਰ ਅਚਲ *p = optarg;
              ਅੱਖਰ ਅਚਲ *lim = p + len;
              ਪੂਰਨ n_chars = 0;
              ਲਈ (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_ਜਾਂਚ (p, lim).len;
              ਜੇ (n_chars == 1)
                ਸਮ੍ਰਿਤੀ_ਨਕਲ (mempcpy (section_del, optarg, len),  ":", ਆਕਾਰ ":");
              ਨਹੀਂਤਾਂ
                section_del = optarg;
            }
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (len == 1)
                *section_del = *optarg;
              ਨਹੀਂਤਾਂ
                section_del = optarg;
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          ok = false;
          ਰੋਕੋ;
        }
    }

  ਜੇ (!ok)
    usage (EXIT_FAILURE);

  /* Initialize the section delimiters.  */
  section_del_len = len = ਲੜੀ_ਲੰਬਾਈ (section_del);

  ਸਿਰਲੇਖ_del_len = len * 3;
  ਸਿਰਲੇਖ_del = xmalloc (ਸਿਰਲੇਖ_del_len + 1);
  stpcpy (stpcpy (stpcpy (ਸਿਰਲੇਖ_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = ਸਿਰਲੇਖ_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&ਸਤਰ_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = ਲੜੀ_ਲੰਬਾਈ (separator_str);
  print_no_ਸਤਰ_fmt = xmalloc (lineno_ਚੌੜਾਈ + len + 1);
  memset (print_no_ਸਤਰ_fmt, ' ', lineno_ਚੌੜਾਈ + len);
  print_no_ਸਤਰ_fmt[lineno_ਚੌੜਾਈ + len] = '\0';

  ਸਤਰ_no = starting_ਸਤਰ_ਸੰਖਿਆ;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  ਜੇ (optind == argc)
    ok = nl_file ("-");
  ਨਹੀਂਤਾਂ
    ਲਈ (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    error (EXIT_FAILURE, errno, "-");

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
