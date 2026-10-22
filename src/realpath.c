/* realpath - print the resolved path
   Copyright (C) 2011-2026 Free Software Foundation, Inc.

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

/* Written by Pádraig Brady.  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "realpath"

#परिभाषय AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

गणना
{
  RELATIVE_गन्तव्यम्_OPTION = CHAR_MAX + 1,
  RELATIVE_आधारः_OPTION
};

स्थिर bool verbose = true;
स्थिर bool logical;
स्थिर bool use_nuls;
स्थिर bool quote_output;
स्थिर वर्ण ध्रुव *can_relative_गन्तव्यम्;
स्थिर वर्ण ध्रुव *can_relative_आधारः;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"canonicalize", no_तर्कः, रिक्त, 'E'},
  {"canonicalize-existing", no_तर्कः, रिक्त, 'e'},
  {"canonicalize-missing", no_तर्कः, रिक्त, 'm'},
  {"relative-to", required_तर्कः, रिक्त, RELATIVE_गन्तव्यम्_OPTION},
  {"relative-base", required_तर्कः, रिक्त, RELATIVE_आधारः_OPTION},
  {"quiet", no_तर्कः, रिक्त, 'q'},
  {"strip", no_तर्कः, रिक्त, 's'},
  {"no-symlinks", no_तर्कः, रिक्त, 's'},
  {"zero", no_तर्कः, रिक्त, 'z'},
  {"logical", no_तर्कः, रिक्त, 'L'},
  {"physical", no_तर्कः, रिक्त, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... FILE...\n"), क्रमादेशः_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the चरमः component must exist (स्वभाव)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components सम्बन्धि the मार्गः must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no मार्गः components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (स्वभाव)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-गन्तव्यम्=DIR        print the resolved मार्गः relative गन्तव्यम् DIR\n\
"));
      oputs (_("\
      --relative-आधारः_2=DIR      print absolute paths unless paths अधः DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   अन्तः each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
स्थिर वर्ण *
realpath_canon (वर्ण ध्रुव *fname, पूर्णाङ्क can_बहुलकम्)
{
  वर्ण *can_fname = canonicalize_filename_बहुलकम् (fname, can_बहुलकम्);
  यदि (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_बहुलकम् &= ~CAN_NOLINKS;
      वर्ण *can_fname2 = canonicalize_filename_बहुलकम् (can_fname, can_बहुलकम्);
      मुक्त (can_fname);
      प्रत्यागम can_fname2;
    }
  प्रत्यागम can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_विशुद्धम्
स्थिर bool
मार्गः_उपसर्गः (वर्ण ध्रुव *उपसर्गः, वर्ण ध्रुव *मार्गः)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  उपसर्गः++;
  मार्गः++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  यदि (!*उपसर्गः)
    प्रत्यागम *मार्गः != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  यदि (*उपसर्गः == '/' && !उपसर्गः[1])
    प्रत्यागम *मार्गः == '/';

  /* Any other prefix has a non-slash portion.  */
  यावत् (*उपसर्गः && *मार्गः)
    {
      यदि (*उपसर्गः != *मार्गः)
        भङ्ग;
      उपसर्गः++;
      मार्गः++;
    }
  प्रत्यागम (!*उपसर्गः && (*मार्गः == '/' || !*मार्गः));
}

स्थिर bool
isdir (वर्ण ध्रुव *मार्गः)
{
  संरचना stat sb;
  यदि (stat (मार्गः, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (मार्गः));
  प्रत्यागम S_ISDIR (sb.st_बहुलकम्);
}

स्थिर शून्य
print_मार्गः (वर्ण ध्रुव *मार्गः)
{
  fputs (quote_output ? quoteN (मार्गः) : मार्गः, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
स्थिर bool
print_relative_मार्गः (वर्ण ध्रुव *can_fname)
{
  यदि (!quote_output)
    प्रत्यागम relpath (can_fname, can_relative_गन्तव्यम्, रिक्त, 0);

  size_t आकार;
  यदि (ckd_mul (&आकार, वाक्यदैर्घ्यम् (can_relative_गन्तव्यम्), 2)
      || ckd_सङ्कलय (&आकार, आकार, वाक्यदैर्घ्यम् (can_fname))
      || ckd_सङ्कलय (&आकार, आकार, 1))
    xalloc_die ();

  वर्ण *relative_मार्गः = xmalloc (आकार);
  bool success = relpath (can_fname, can_relative_गन्तव्यम्, relative_मार्गः, आकार);
  यदि (success)
    print_मार्गः (relative_मार्गः);
  मुक्त (relative_मार्गः);
  प्रत्यागम success;
}

स्थिर bool
process_मार्गः (वर्ण ध्रुव *fname, पूर्णाङ्क can_बहुलकम्)
{
  वर्ण *can_fname = realpath_canon (fname, can_बहुलकम्);
  यदि (!can_fname)
    {
      यदि (verbose)
        error (0, errno, "%s", quotef (fname));
      प्रत्यागम false;
    }

  यदि (!can_relative_गन्तव्यम्
      || (can_relative_आधारः && !मार्गः_उपसर्गः (can_relative_आधारः, can_fname))
      || (can_relative_गन्तव्यम् && !print_relative_मार्गः (can_fname)))
    print_मार्गः (can_fname);

  अक्षरं_लिख (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  यदि (ferror (stdout))
    write_error ();

  प्रत्यागम true;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ok = true;
  पूर्णाङ्क can_बहुलकम् = CAN_ALL_BUT_चरमः;
  वर्ण ध्रुव *relative_गन्तव्यम् = रिक्त;
  वर्ण ध्रुव *relative_आधारः = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "EeLmPqsz", longopts, रिक्त);
      यदि (c == -1)
        भङ्ग;
      विकल्प (c)
        {
        पक्ष 'E':
          can_बहुलकम् &= ~CAN_बहुलकम्_आवरणम्;
          can_बहुलकम् |= CAN_ALL_BUT_चरमः;
          भङ्ग;
        पक्ष 'e':
          can_बहुलकम् &= ~CAN_बहुलकम्_आवरणम्;
          can_बहुलकम् |= CAN_EXISTING;
          भङ्ग;
        पक्ष 'm':
          can_बहुलकम् &= ~CAN_बहुलकम्_आवरणम्;
          can_बहुलकम् |= CAN_MISSING;
          भङ्ग;
        पक्ष 'L':
          can_बहुलकम् |= CAN_NOLINKS;
          logical = true;
          भङ्ग;
        पक्ष 's':
          can_बहुलकम् |= CAN_NOLINKS;
          logical = false;
          भङ्ग;
        पक्ष 'P':
          can_बहुलकम् &= ~CAN_NOLINKS;
          logical = false;
          भङ्ग;
        पक्ष 'q':
          verbose = false;
          भङ्ग;
        पक्ष 'z':
          use_nuls = true;
          भङ्ग;
        पक्ष RELATIVE_गन्तव्यम्_OPTION:
          relative_गन्तव्यम् = optarg;
          भङ्ग;
        पक्ष RELATIVE_आधारः_OPTION:
          relative_आधारः = optarg;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  यदि (! use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
      यदि (qs != literal_quoting_style)
        {
          समुच्चयः_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  यदि (relative_आधारः && !relative_गन्तव्यम्)
    relative_गन्तव्यम् = relative_आधारः;

  bool need_dir = (can_बहुलकम् & CAN_बहुलकम्_आवरणम्) == CAN_EXISTING;
  यदि (relative_गन्तव्यम्)
    {
      can_relative_गन्तव्यम् = realpath_canon (relative_गन्तव्यम्, can_बहुलकम्);
      यदि (!can_relative_गन्तव्यम्)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_गन्तव्यम्));
      यदि (need_dir && !isdir (can_relative_गन्तव्यम्))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_गन्तव्यम्));
    }
  यदि (relative_आधारः == relative_गन्तव्यम्)
    can_relative_आधारः = can_relative_गन्तव्यम्;
  अन्यथा यदि (relative_आधारः)
    {
      वर्ण *आधारः_2 = realpath_canon (relative_आधारः, can_बहुलकम्);
      यदि (!आधारः_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_आधारः));
      यदि (need_dir && !isdir (आधारः_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_आधारः));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      यदि (मार्गः_उपसर्गः (आधारः_2, can_relative_गन्तव्यम्))
        can_relative_आधारः = आधारः_2;
      अन्यथा
        {
          मुक्त (आधारः_2);
          can_relative_आधारः = can_relative_गन्तव्यम्;
          can_relative_गन्तव्यम् = रिक्त;
        }
    }

  प्रति (; optind < argc; ++optind)
    ok &= process_मार्गः (argv[optind], can_बहुलकम्);

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
