/* pathchk -- check whether file names are valid or portable
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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <wchar.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pathchk"

#define AUTHORS \
  proper_name ("Paul Eggert"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#ifndef _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 256
#endif
#ifndef _POSIX_NAME_MAX
# define _POSIX_NAME_MAX 14
#endif

#ifdef _XOPEN_NAME_MAX
# define NAME_MAX_MINIMUM _XOPEN_NAME_MAX
#अन्यथा
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#endif
#ifdef _XOPEN_PATH_MAX
# define PATH_MAX_MINIMUM _XOPEN_PATH_MAX
#अन्यथा
# define PATH_MAX_MINIMUM _POSIX_PATH_MAX
#endif

#जर ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined _PC_PATH_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define _PC_PATH_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : PATH_MAX_MINIMUM)
# endif
#endif

स्थिर bool validate_file_name (अक्षर *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  PORTABILITY_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"portability", no_argument, रिक्त, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      ख_माला_दो (_("\
Diagnose invalid or non-portable file names.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -p     check साठी most POSIX systems\n\
"));
      oputs (_("\
  -P     check साठी empty names and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check साठी all POSIX systems (equivalent to -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "+pP", longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          थांबा;

        स्थिती 'p':
          check_basic_portability = true;
          थांबा;

        स्थिती 'P':
          check_extra_portability = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  साठी (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  परत ok ? निकास_सफल : निकास_त्रुटि;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

स्थिर bool
no_leading_hyphen (अक्षर स्थिरांक *file)
{
  साठी (अक्षर स्थिरांक *p = file;  (p = म_अक्षर (p, '-'));  p++)
    जर (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        परत false;
      }

  परत true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

स्थिर bool
portable_chars_only (अक्षर स्थिरांक *file, माप_प्रकार filelen)
{
  माप_प्रकार validlen = म_अखोज (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  अक्षर स्थिरांक *invalid = file + validlen;

  जर (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      माप_प्रकार charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      परत false;
    }

  परत true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_PURE
स्थिर अक्षर *
component_start (अक्षर *f)
{
  पर्यंत (*f == '/')
    f++;
  परत f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_PURE
स्थिर माप_प्रकार
component_len (अक्षर स्थिरांक *f)
{
  माप_प्रकार len;
  साठी (len = 1; f[len] != '/' && f[len]; len++)
    सुरूठेवा;
  परत len;
}

/* Make sure that
   strlen (FILE) <= PATH_MAX
   && strlen (each-existing-directory-in-FILE) <= NAME_MAX

   If CHECK_BASIC_PORTABILITY is true, compare against _POSIX_PATH_MAX and
   _POSIX_NAME_MAX instead, and make sure that FILE contains no
   characters not in the POSIX portable filename character set, which
   consists of A-Z, a-z, 0-9, ., _, - (plus / for separators).

   If CHECK_BASIC_PORTABILITY is false, make sure that all leading directories
   along FILE that exist are searchable.

   If CHECK_EXTRA_PORTABILITY is true, check that file name components do not
   begin with "-".

   If either CHECK_BASIC_PORTABILITY or CHECK_EXTRA_PORTABILITY is true,
   check that the file name is not empty.

   Return true if all of these tests are successful, false if any fail.  */

स्थिर bool
validate_file_name (अक्षर *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = म_माप (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  जर (check_extra_portability && ! no_leading_hyphen (file))
    परत false;

  जर ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      परत false;
    }

  जर (check_basic_portability)
    {
      जर (! portable_chars_only (file, filelen))
        परत false;
    }
  अन्यथा
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      संरचना stat st;
      जर (lstat (file, &st) == 0)
        file_exists = true;
      अन्यथा जर (त्रुटि_सं != ENOENT || filelen == 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (file));
          परत false;
        }
    }

  जर (check_basic_portability
      || (! file_exists && PATH_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      जर (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      अन्यथा
        {
          दीर्घ पूर्णांक size;
          अक्षर स्थिरांक *dir = (*file == '/' ? "/" : ".");
          त्रुटि_सं = 0;
          size = pathconf (dir, _PC_PATH_MAX);
          जर (size < 0 && त्रुटि_सं != 0)
            {
              error (0, त्रुटि_सं,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              परत false;
            }
          maxsize = MIN (size, MIN (SSIZE_MAX, IDX_MAX));
        }

      जर (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          परत false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  जर (! check_component_lengths && ! file_exists)
    {
      साठी (अक्षर *start = file; *(start = component_start (start)); )
        {
          माप_प्रकार length = component_len (start);

          जर (NAME_MAX_MINIMUM < length)
            {
              check_component_lengths = true;
              थांबा;
            }

          start += length;
        }
    }

  जर (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t known_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      साठी (अक्षर *start = file; *(start = component_start (start)); )
        {
          idx_t length;

          जर (known_name_max)
            name_max = known_name_max;
          अन्यथा
            {
              दीर्घ पूर्णांक len;
              अक्षर स्थिरांक *dir = (start == file ? "." : file);
              अक्षर c = *start;
              त्रुटि_सं = 0;
              *start = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *start = c;
              जर (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              अन्यथा
                निवड (त्रुटि_सं)
                  {
                  स्थिती 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    थांबा;

                  स्थिती ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    known_name_max = name_max;
                    थांबा;

                  पूर्वनिर्धारित:
                    *start = '\0';
                    error (0, त्रुटि_सं, "%s", quotef (dir));
                    *start = c;
                    परत false;
                  }
            }

          length = component_len (start);

          जर (name_max < length)
            {
              अक्षर c = start[length];
              start[length] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, length, quote (start));
              start[length] = c;
              परत false;
            }

          start += length;
        }
    }

  परत true;
}
