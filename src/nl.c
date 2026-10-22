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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"

#समावेश <regex.h>

#समावेश "fadvise.h"
#समावेश "linebuffer.h"
#समावेश "mcel.h"
#समावेश "quote.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "nl"

#घोषणा AUTHORS \
  proper_name ("Scott Bartram"), \
  proper_name ("David MacKenzie")

/* Line-number formats.  They are given an int width, an intmax_t
   value, and a string separator.  */

/* Right justified, no leading zeroes.  */
अटल अक्षर स्थिर FORMAT_दायाँ_NOLZ[] = "%*jd%s";

/* Right justified, leading zeroes.  */
अटल अक्षर स्थिर FORMAT_दायाँ_LZ[] = "%0*jd%s";

/* Left justified, no leading zeroes.  */
अटल अक्षर स्थिर FORMAT_बायाँ[] = "%-*jd%s";

/* Default section delimiter characters.  */
अटल अक्षर DEFAULT_SECTION_DELIMITERS[MCEL_LEN_MAX * 2 + 1] = "\\:";

/* Types of input lines: either one of the section delimiters,
   or text to output. */
क्रमागत section
{
  Header, Body, Footer, Text
};

/* Format of body lines (-b).  */
अटल अक्षर स्थिर *body_type = "t";

/* Format of header lines (-h).  */
अटल अक्षर स्थिर *header_type = "n";

/* Format of footer lines (-f).  */
अटल अक्षर स्थिर *footer_type = "n";

/* Format currently being used (body, header, or footer).  */
अटल अक्षर स्थिर *current_type;

/* Regex for body lines to number (-bp).  */
अटल काष्ठा re_pattern_बफर body_regex;

/* Regex for header lines to number (-hp).  */
अटल काष्ठा re_pattern_बफर header_regex;

/* Regex for footer lines to number (-fp).  */
अटल काष्ठा re_pattern_बफर footer_regex;

/* Fastmaps for the above.  */
अटल अक्षर body_fastmap[अच_अक्षर_उच्च + 1];
अटल अक्षर header_fastmap[अच_अक्षर_उच्च + 1];
अटल अक्षर footer_fastmap[अच_अक्षर_उच्च + 1];

/* Pointer to current regex, if any.  */
अटल काष्ठा re_pattern_बफर *current_regex = शून्य;

/* Separator string to print after line number (-s).  */
अटल अक्षर स्थिर *separator_str = "\t";

/* Input section delimiter string (-d).  */
अटल अक्षर *section_del = DEFAULT_SECTION_DELIMITERS;

/* Input section delimiter length.  */
अटल माप_प्रकार section_del_len;

/* Header delimiter string.  */
अटल अक्षर *header_del = शून्य;

/* Header section delimiter length.  */
अटल माप_प्रकार header_del_len;

/* Body delimiter string.  */
अटल अक्षर *body_del = शून्य;

/* Body section delimiter length.  */
अटल माप_प्रकार body_del_len;

/* Footer delimiter string.  */
अटल अक्षर *footer_del = शून्य;

/* Footer section delimiter length.  */
अटल माप_प्रकार footer_del_len;

/* Input buffer.  */
अटल काष्ठा linebuffer पंक्ति_buf;

/* printf format string for unnumbered lines.  */
अटल अक्षर *print_no_पंक्ति_fmt = शून्य;

/* Starting line number on each page (-v).  */
अटल intmax_t starting_पंक्ति_संख्या = 1;

/* Line number increment (-i).  */
अटल intmax_t page_incr = 1;

/* If true, reset line number at start of each page (-p).  */
अटल bool reset_numbers = true;

/* Number of blank lines to consider to be one line for numbering (-l).  */
अटल intmax_t blank_join = 1;

/* Width of line numbers (-w).  */
अटल पूर्णांक lineno_width = 6;

/* Line number format (-n).  */
अटल अक्षर स्थिर *lineno_format = FORMAT_दायाँ_NOLZ;

/* Current print line number.  */
अटल intmax_t पंक्ति_no;

/* Whether the current line number has incremented past limits.  */
अटल bool पंक्ति_no_overflow;

/* True if we have ever read standard input.  */
अटल bool have_read_stdin;

अटल काष्ठा option स्थिर longopts[] =
{
  {"header-numbering", required_argument, शून्य, 'h'},
  {"body-numbering", required_argument, शून्य, 'b'},
  {"footer-numbering", required_argument, शून्य, 'f'},
  {"starting-line-number", required_argument, शून्य, 'v'},
  {"line-increment", required_argument, शून्य, 'i'},
  {"no-renumber", no_argument, शून्य, 'p'},
  {"join-blank-lines", required_argument, शून्य, 'l'},
  {"number-separator", required_argument, शून्य, 's'},
  {"number-width", required_argument, शून्य, 'w'},
  {"number-format", required_argument, शून्य, 'n'},
  {"section-delimiter", required_argument, शून्य, 'd'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Print a usage message and quit. */

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Write each खाता to standard output, with पंक्ति numbers added.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --body-numbering=STYLE      use STYLE क्रम numbering body पंक्तियाँ\n\
"));
      oputs (_("\
  -d, --section-delimiter=CC      use CC क्रम logical page delimiters\n\
"));
      oputs (_("\
  -f, --footer-numbering=STYLE    use STYLE क्रम numbering footer पंक्तियाँ\n\
"));
      oputs (_("\
  -h, --header-numbering=STYLE    use STYLE क्रम numbering header पंक्तियाँ\n\
"));
      oputs (_("\
  -i, --पंक्ति-increment=संख्या_2     पंक्ति संख्या increment at each पंक्ति\n\
"));
      oputs (_("\
  -l, --join-blank-पंक्तियाँ=संख्या_2   group of संख्या_2 empty पंक्तियाँ counted as one\n\
"));
      oputs (_("\
  -n, --संख्या-format=FORMAT      जोड़ो पंक्ति numbers according to FORMAT\n\
"));
      oputs (_("\
  -p, --no-renumber               करो not reset पंक्ति numbers क्रम each section\n\
"));
      oputs (_("\
  -s, --संख्या-separator=STRING   add STRING after (possible) पंक्ति संख्या\n\
"));
      oputs (_("\
  -v, --starting-पंक्ति-संख्या=संख्या_2  first पंक्ति संख्या क्रम each section\n\
"));
      oputs (_("\
  -w, --संख्या-width=संख्या_2       use संख्या_2 columns क्रम पंक्ति numbers\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Default options are: -bt -d'\\:' -fn -hn -i1 -l1 -n'rn' -s<TAB> -v1 -w6\n\
\n\
CC are two delimiter characters used to construct logical page delimiters;\n\
a missing second character implies ':'.  As a GNU extension one can specify\n\
more than two characters, and also specifying the empty string (-d '')\n\
disables section matching.\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
STYLE is one of:\n\
\n\
  a      संख्या all पंक्तियाँ\n\
  t      संख्या only nonempty पंक्तियाँ\n\
  n      संख्या no पंक्तियाँ\n\
  pBRE   संख्या only पंक्तियाँ that contain a match क्रम the basic regular\n\
         expression, BRE\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
FORMAT is one of:\n\
\n\
  ln     बायाँ justified, no leading zeros\n\
  rn     दायाँ justified, no leading zeros\n\
  rz     दायाँ justified, leading zeros\n\
\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Set the command line flag TYPEP and possibly the regex pointer REGEXP,
   according to 'optarg'.  */

अटल bool
build_type_arg (अक्षर स्थिर **typep,
                काष्ठा re_pattern_बफर *regexp, अक्षर *fastmap)
{
  अक्षर स्थिर *errmsg;
  bool rval = true;

  चयन (*optarg)
    {
    हाल 'a':
    हाल 't':
    हाल 'n':
      *typep = optarg;
      अवरोध;
    हाल 'p':
      *typep = optarg++;
      regexp->बफर = शून्य;
      regexp->allocated = 0;
      regexp->fastmap = fastmap;
      regexp->translate = शून्य;
      re_syntax_options =
        RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
      errmsg = re_compile_pattern (optarg, म_माप (optarg), regexp);
      अगर (errmsg)
        error (निकास_त्रुटि, 0, "%s", (errmsg));
      अवरोध;
    शेष:
      rval = false;
      अवरोध;
    }
  वापस rval;
}

/* Print the line number and separator; increment the line number. */

अटल व्योम
print_lineno (व्योम)
{
  अगर (पंक्ति_no_overflow)
    error (निकास_त्रुटि, 0, _("line number overflow"));

  म_लिखो (lineno_format, lineno_width, पंक्ति_no, separator_str);

  अगर (ckd_add (&पंक्ति_no, पंक्ति_no, page_incr))
    पंक्ति_no_overflow = true;
}

अटल व्योम
reset_lineno (व्योम)
{
  अगर (reset_numbers)
    {
      पंक्ति_no = starting_पंक्ति_संख्या;
      पंक्ति_no_overflow = false;
    }
}

/* Switch to a header section. */

अटल व्योम
proc_header (व्योम)
{
  current_type = header_type;
  current_regex = &header_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Switch to a body section. */

अटल व्योम
proc_body (व्योम)
{
  current_type = body_type;
  current_regex = &body_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Switch to a footer section. */

अटल व्योम
proc_footer (व्योम)
{
  current_type = footer_type;
  current_regex = &footer_regex;
  reset_lineno ();
  अक्षर_दो ('\n');
}

/* Process a regular text line in 'line_buf'. */

अटल व्योम
proc_text (व्योम)
{
  अटल intmax_t blank_पंक्तियाँ = 0;	/* Consecutive blank lines so far. */

  चयन (*current_type)
    {
    हाल 'a':
      अगर (blank_join > 1)
        {
          अगर (1 < पंक्ति_buf.लंबाई || ++blank_पंक्तियाँ == blank_join)
            {
              print_lineno ();
              blank_पंक्तियाँ = 0;
            }
          अन्यथा
            ख_माला_दो (print_no_पंक्ति_fmt, मानक_निकास);
        }
      अन्यथा
        print_lineno ();
      अवरोध;
    हाल 't':
      अगर (1 < पंक्ति_buf.लंबाई)
        print_lineno ();
      अन्यथा
        ख_माला_दो (print_no_पंक्ति_fmt, मानक_निकास);
      अवरोध;
    हाल 'n':
      ख_माला_दो (print_no_पंक्ति_fmt, मानक_निकास);
      अवरोध;
    हाल 'p':
      चयन (re_खोज (current_regex, पंक्ति_buf.बफर, पंक्ति_buf.लंबाई - 1,
                         0, पंक्ति_buf.लंबाई - 1, शून्य))
        {
        हाल -2:
          error (निकास_त्रुटि, त्रुटि_सं, _("error in regular expression search"));

        हाल -1:
          ख_माला_दो (print_no_पंक्ति_fmt, मानक_निकास);
          अवरोध;

        शेष:
          print_lineno ();
          अवरोध;
        }
    }
  ख_डालो (पंक्ति_buf.बफर, माप (अक्षर), पंक्ति_buf.लंबाई, मानक_निकास);
}

/* Return the type of line in 'line_buf'. */

अटल क्रमागत section
check_section (व्योम)
{
  माप_प्रकार len = पंक्ति_buf.लंबाई - 1;

  अगर (len < section_del_len || footer_del_len < section_del_len
      || !memeq (पंक्ति_buf.बफर, section_del, section_del_len))
    वापस Text;
  अगर (len == header_del_len
      && memeq (पंक्ति_buf.बफर, header_del, header_del_len))
    वापस Header;
  अगर (len == body_del_len
      && memeq (पंक्ति_buf.बफर, body_del, body_del_len))
    वापस Body;
  अगर (len == footer_del_len
      && memeq (पंक्ति_buf.बफर, footer_del, footer_del_len))
    वापस Footer;
  वापस Text;
}

/* Read and process the file pointed to by FP. */

अटल व्योम
process_file (खाता *fp)
{
  जबतक (readlinebuffer (&पंक्ति_buf, fp))
    {
      चयन (check_section ())
        {
        हाल Header:
          proc_header ();
          अवरोध;
        हाल Body:
          proc_body ();
          अवरोध;
        हाल Footer:
          proc_footer ();
          अवरोध;
        हाल Text:
          proc_text ();
          अवरोध;
        }

      अगर (ख_त्रुटि (मानक_निकास))
        write_error ();
    }
}

/* Process file FILE to standard output.
   Return true if successful.  */

अटल bool
nl_file (अक्षर स्थिर *file)
{
  खाता *stream;

  अगर (streq (file, "-"))
    {
      have_read_stdin = true;
      stream = मानक_निवेश;
      assume (stream);  /* Pacify GCC bug#109613.  */
    }
  अन्यथा
    {
      stream = ख_खोलो (file, "r");
      अगर (stream == शून्य)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          वापस false;
        }
    }

  fadvise (stream, FADVISE_SEQUENTIAL);

  process_file (stream);

  पूर्णांक err = त्रुटि_सं;
  अगर (!ख_त्रुटि (stream))
    err = 0;
  अगर (streq (file, "-"))
    साफ_त्रुटि (stream);		/* Also clear EOF. */
  अन्यथा अगर (ख_बंद (stream) != 0 && !err)
    err = त्रुटि_सं;
  अगर (err)
    {
      error (0, err, "%s", quotef (file));
      वापस false;
    }
  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;
  माप_प्रकार len;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "h:b:f:v:i:pl:s:w:n:d:", longopts,
                           शून्य))
         != -1)
    {
      चयन (c)
        {
        हाल 'h':
          अगर (! build_type_arg (&header_type, &header_regex, header_fastmap))
            {
              error (0, 0, _("invalid header numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          अवरोध;
        हाल 'b':
          अगर (! build_type_arg (&body_type, &body_regex, body_fastmap))
            {
              error (0, 0, _("invalid body numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          अवरोध;
        हाल 'f':
          अगर (! build_type_arg (&footer_type, &footer_regex, footer_fastmap))
            {
              error (0, 0, _("invalid footer numbering style: %s"),
                     quote (optarg));
              ok = false;
            }
          अवरोध;
        हाल 'v':
          starting_पंक्ति_संख्या = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                             _("invalid starting line number"),
                                             0);
          अवरोध;
        हाल 'i':
          page_incr = xdectoimax (optarg, INTMAX_MIN, INTMAX_MAX, "",
                                  _("invalid line number increment"), 0);
          अवरोध;
        हाल 'p':
          reset_numbers = false;
          अवरोध;
        हाल 'l':
          blank_join = xnumtoimax (optarg, 10, 1, INTMAX_MAX, "",
                                   _("invalid line number of blank lines"),
                                   0, XTOINT_MIN_RANGE | XTOINT_MAX_QUIET);
          अवरोध;
        हाल 's':
          separator_str = optarg;
          अवरोध;
        हाल 'w':
          lineno_width = xnumtoimax (optarg, 10, 1, पूर्णांक_उच्च, "",
                                     _("invalid line number field width"),
                                     0, XTOINT_MIN_RANGE);
          अवरोध;
        हाल 'n':
          अगर (streq (optarg, "ln"))
            lineno_format = FORMAT_बायाँ;
          अन्यथा अगर (streq (optarg, "rn"))
            lineno_format = FORMAT_दायाँ_NOLZ;
          अन्यथा अगर (streq (optarg, "rz"))
            lineno_format = FORMAT_दायाँ_LZ;
          अन्यथा
            {
              error (0, 0, _("invalid line numbering format: %s"),
                     quote (optarg));
              ok = false;
            }
          अवरोध;
        हाल 'd':
          len = म_माप (optarg);
          अगर (1 < MB_CUR_MAX)
            {
              अक्षर स्थिर *p = optarg;
              अक्षर स्थिर *lim = p + len;
              पूर्णांक n_chars = 0;
              क्रम (; p < lim && n_chars < 2; ++n_chars)
                  p += mcel_scan (p, lim).len;
              अगर (n_chars == 1)
                स_नकल (mempcpy (section_del, optarg, len),  ":", माप ":");
              अन्यथा
                section_del = optarg;
            }
          अन्यथा
            {
              अगर (len == 1)
                *section_del = *optarg;
              अन्यथा
                section_del = optarg;
            }
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          ok = false;
          अवरोध;
        }
    }

  अगर (!ok)
    usage (निकास_त्रुटि);

  /* Initialize the section delimiters.  */
  section_del_len = len = म_माप (section_del);

  header_del_len = len * 3;
  header_del = xmalloc (header_del_len + 1);
  stpcpy (stpcpy (stpcpy (header_del, section_del), section_del), section_del);

  body_del_len = len * 2;
  body_del = header_del + len;

  footer_del_len = len;
  footer_del = body_del + len;

  /* Initialize the input buffer.  */
  initbuffer (&पंक्ति_buf);

  /* Initialize the printf format for unnumbered lines. */
  len = म_माप (separator_str);
  print_no_पंक्ति_fmt = xmalloc (lineno_width + len + 1);
  स_रखो (print_no_पंक्ति_fmt, ' ', lineno_width + len);
  print_no_पंक्ति_fmt[lineno_width + len] = '\0';

  पंक्ति_no = starting_पंक्ति_संख्या;
  current_type = body_type;
  current_regex = &body_regex;

  /* Main processing. */

  अगर (optind == argc)
    ok = nl_file ("-");
  अन्यथा
    क्रम (; optind < argc; optind++)
      ok &= nl_file (argv[optind]);

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    error (निकास_त्रुटि, त्रुटि_सं, "-");

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
