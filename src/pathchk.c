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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <wchar.h>

#include "system.h"
#include "quote.h"

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
#มิฉะนั้น
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#endif
#ifdef _XOPEN_PATH_MAX
# define PATH_MAX_MINIMUM _XOPEN_PATH_MAX
#มิฉะนั้น
# define PATH_MAX_MINIMUM _POSIX_PATH_MAX
#endif

#ถ้า ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined _PC_PATH_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define _PC_PATH_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : PATH_MAX_MINIMUM)
# endif
#endif

static bool validate_file_name (var_na *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  {"portability", no_argument, NULL, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
    {
      printf (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      fputs (_("\
Diagnose invalid or non-portable file names.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check สำหรับ most POSIX systems\n\
"));
      oputs (_("\
  -P     check สำหรับ empty names and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check สำหรับ all POSIX systems (equivalent to -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  จำนวนเต็ม optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ขณะ ((optc = getopt_long (argc, argv, "+pP", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          bha_nga;

        pak_sa 'p':
          check_basic_portability = true;
          bha_nga;

        pak_sa 'P':
          check_extra_portability = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ถ้า (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  สำหรับ (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  คืน ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

static bool
no_leading_hyphen (var_na const *file)
{
  สำหรับ (var_na const *p = file;  (p = strchr (p, '-'));  p++)
    ถ้า (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        คืน false;
      }

  คืน true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

static bool
portable_chars_only (var_na const *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  var_na const *invalid = file + validlen;

  ถ้า (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      คืน false;
    }

  คืน true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_PURE
static var_na *
component_start (var_na *f)
{
  ขณะ (*f == '/')
    f++;
  คืน f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_PURE
static size_t
component_len (var_na const *f)
{
  size_t len;
  สำหรับ (len = 1; f[len] != '/' && f[len]; len++)
    continue;
  คืน len;
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

static bool
validate_file_name (var_na *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = strlen (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  ถ้า (check_extra_portability && ! no_leading_hyphen (file))
    คืน false;

  ถ้า ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      คืน false;
    }

  ถ้า (check_basic_portability)
    {
      ถ้า (! portable_chars_only (file, filelen))
        คืน false;
    }
  มิฉะนั้น
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      sa_nracanaa stat st;
      ถ้า (lstat (file, &st) == 0)
        file_exists = true;
      มิฉะนั้น ถ้า (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          คืน false;
        }
    }

  ถ้า (check_basic_portability
      || (! file_exists && PATH_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      ถ้า (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      มิฉะนั้น
        {
          long จำนวนเต็ม size;
          var_na const *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          size = pathconf (dir, _PC_PATH_MAX);
          ถ้า (size < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              คืน false;
            }
          maxsize = MIN (size, MIN (SSIZE_MAX, IDX_MAX));
        }

      ถ้า (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          คืน false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  ถ้า (! check_component_lengths && ! file_exists)
    {
      สำหรับ (var_na *start = file; *(start = component_start (start)); )
        {
          size_t length = component_len (start);

          ถ้า (NAME_MAX_MINIMUM < length)
            {
              check_component_lengths = true;
              bha_nga;
            }

          start += length;
        }
    }

  ถ้า (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t known_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      สำหรับ (var_na *start = file; *(start = component_start (start)); )
        {
          idx_t length;

          ถ้า (known_name_max)
            name_max = known_name_max;
          มิฉะนั้น
            {
              long จำนวนเต็ม len;
              var_na const *dir = (start == file ? "." : file);
              var_na c = *start;
              errno = 0;
              *start = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *start = c;
              ถ้า (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              มิฉะนั้น
                switch (errno)
                  {
                  pak_sa 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    bha_nga;

                  pak_sa ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    known_name_max = name_max;
                    bha_nga;

                  default:
                    *start = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *start = c;
                    คืน false;
                  }
            }

          length = component_len (start);

          ถ้า (name_max < length)
            {
              var_na c = start[length];
              start[length] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, length, quote (start));
              start[length] = c;
              คืน false;
            }

          start += length;
        }
    }

  คืน true;
}
