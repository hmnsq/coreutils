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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <wchar.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "quote.h"

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
#অন্যথা
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#endif
#ifdef _XOPEN_PATH_MAX
# define PATH_MAX_MINIMUM _XOPEN_PATH_MAX
#অন্যথা
# define PATH_MAX_MINIMUM _POSIX_PATH_MAX
#endif

#যদি ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined _PC_PATH_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define _PC_PATH_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : PATH_MAX_MINIMUM)
# endif
#endif

স্থির bool validate_file_name (অক্ষর *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"portability", no_argument, ফাঁকা, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      fputs (_("\
Diagnose invalid or non-portable file names.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check জন্য most POSIX systems\n\
"));
      oputs (_("\
  -P     check জন্য empty names and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check জন্য all POSIX systems (equivalent to -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  পূর্ণসংখ্যা optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "+pP", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          বিরতি;

        ক্ষেত্র 'p':
          check_basic_portability = true;
          বিরতি;

        ক্ষেত্র 'P':
          check_extra_portability = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  জন্য (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

স্থির bool
no_leading_hyphen (অক্ষর ধ্রুবক *file)
{
  জন্য (অক্ষর ধ্রুবক *p = file;  (p = strchr (p, '-'));  p++)
    যদি (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        ফেরত false;
      }

  ফেরত true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

স্থির bool
portable_chars_only (অক্ষর ধ্রুবক *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  অক্ষর ধ্রুবক *invalid = file + validlen;

  যদি (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      ফেরত false;
    }

  ফেরত true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_PURE
স্থির অক্ষর *
component_start (অক্ষর *f)
{
  যখন (*f == '/')
    f++;
  ফেরত f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_PURE
স্থির size_t
component_len (অক্ষর ধ্রুবক *f)
{
  size_t len;
  জন্য (len = 1; f[len] != '/' && f[len]; len++)
    চালু;
  ফেরত len;
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

স্থির bool
validate_file_name (অক্ষর *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = strlen (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  যদি (check_extra_portability && ! no_leading_hyphen (file))
    ফেরত false;

  যদি ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      ফেরত false;
    }

  যদি (check_basic_portability)
    {
      যদি (! portable_chars_only (file, filelen))
        ফেরত false;
    }
  অন্যথা
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      কাঠামো stat st;
      যদি (lstat (file, &st) == 0)
        file_exists = true;
      অন্যথা যদি (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          ফেরত false;
        }
    }

  যদি (check_basic_portability
      || (! file_exists && PATH_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      যদি (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      অন্যথা
        {
          long পূর্ণসংখ্যা আকার_2;
          অক্ষর ধ্রুবক *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          আকার_2 = pathconf (dir, _PC_PATH_MAX);
          যদি (আকার_2 < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              ফেরত false;
            }
          maxsize = MIN (আকার_2, MIN (SSIZE_MAX, IDX_MAX));
        }

      যদি (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          ফেরত false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  যদি (! check_component_lengths && ! file_exists)
    {
      জন্য (অক্ষর *start = file; *(start = component_start (start)); )
        {
          size_t দৈর্ঘ্য = component_len (start);

          যদি (NAME_MAX_MINIMUM < দৈর্ঘ্য)
            {
              check_component_lengths = true;
              বিরতি;
            }

          start += দৈর্ঘ্য;
        }
    }

  যদি (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t known_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      জন্য (অক্ষর *start = file; *(start = component_start (start)); )
        {
          idx_t দৈর্ঘ্য;

          যদি (known_name_max)
            name_max = known_name_max;
          অন্যথা
            {
              long পূর্ণসংখ্যা len;
              অক্ষর ধ্রুবক *dir = (start == file ? "." : file);
              অক্ষর c = *start;
              errno = 0;
              *start = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *start = c;
              যদি (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              অন্যথা
                নির্বাচন (errno)
                  {
                  ক্ষেত্র 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    বিরতি;

                  ক্ষেত্র ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    known_name_max = name_max;
                    বিরতি;

                  পূর্বনির্ধারিত:
                    *start = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *start = c;
                    ফেরত false;
                  }
            }

          দৈর্ঘ্য = component_len (start);

          যদি (name_max < দৈর্ঘ্য)
            {
              অক্ষর c = start[দৈর্ঘ্য];
              start[দৈর্ঘ্য] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, দৈর্ঘ্য, quote (start));
              start[দৈর্ঘ্য] = c;
              ফেরত false;
            }

          start += দৈর্ঘ্য;
        }
    }

  ফেরত true;
}
