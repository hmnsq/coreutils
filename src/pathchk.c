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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <wchar.h>

#شامل "system.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "pathchk"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Eggert"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#اگر_غیر_تعریف_شدہ _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 256
#اگر_ختم
#اگر_غیر_تعریف_شدہ _POSIX_NAME_MAX
# define _POSIX_NAME_MAX 14
#اگر_ختم

#اگر_تعریف_شدہ _XOPEN_NAME_MAX
# define NAME_MAX_MINIMUM _XOPEN_NAME_MAX
#ورنہ
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#اگر_ختم
#اگر_تعریف_شدہ XOPEN_راستہ_MAX
# define راستہ_MAX_MINIMUM XOPEN_راستہ_MAX
#ورنہ
# define راستہ_MAX_MINIMUM _POSIX_PATH_MAX
#اگر_ختم

#اگر ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined PC_راستہ_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define PC_راستہ_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : راستہ_MAX_MINIMUM)
# endif
#اگر_ختم

ساکن bool validate_file_name (حرف *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"portability", no_دلیل, تہی, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... NAME...\n"), پروگرام_name);
      fputs (_("\
Diagnose invalid or non-portable file نام.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check برائے most POSIX systems\n\
"));
      oputs (_("\
  -P     check برائے خالی_خانے نام and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check برائے all POSIX systems (equivalent مقصد -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  صحیح optc;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "+pP", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          توڑو;

        حالت 'p':
          check_basic_portability = true;
          توڑو;

        حالت 'P':
          check_extra_portability = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  برائے (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

ساکن bool
no_leading_hyphen (حرف ثابت *file)
{
  برائے (حرف ثابت *p = file;  (p = strchr (p, '-'));  p++)
    اگر (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        واپس false;
      }

  واپس true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

ساکن bool
portable_chars_only (حرف ثابت *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  حرف ثابت *invalid = file + validlen;

  اگر (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      واپس false;
    }

  واپس true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_خالص
ساکن حرف *
component_آغاز (حرف *f)
{
  جب (*f == '/')
    f++;
  واپس f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_خالص
ساکن size_t
component_len (حرف ثابت *f)
{
  size_t len;
  برائے (len = 1; f[len] != '/' && f[len]; len++)
    جاری;
  واپس len;
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

ساکن bool
validate_file_name (حرف *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = لڑی_لمبائی (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  اگر (check_extra_portability && ! no_leading_hyphen (file))
    واپس false;

  اگر ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      واپس false;
    }

  اگر (check_basic_portability)
    {
      اگر (! portable_chars_only (file, filelen))
        واپس false;
    }
  ورنہ
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      ساخت stat st;
      اگر (lstat (file, &st) == 0)
        file_exists = true;
      ورنہ اگر (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          واپس false;
        }
    }

  اگر (check_basic_portability
      || (! file_exists && راستہ_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      اگر (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      ورنہ
        {
          طویل صحیح سائز;
          حرف ثابت *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          سائز = pathconf (dir, PC_راستہ_MAX);
          اگر (سائز < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              واپس false;
            }
          maxsize = MIN (سائز, MIN (SSIZE_MAX, IDX_MAX));
        }

      اگر (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          واپس false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  اگر (! check_component_lengths && ! file_exists)
    {
      برائے (حرف *آغاز = file; *(آغاز = component_آغاز (آغاز)); )
        {
          size_t لمبائی = component_len (آغاز);

          اگر (NAME_MAX_MINIMUM < لمبائی)
            {
              check_component_lengths = true;
              توڑو;
            }

          آغاز += لمبائی;
        }
    }

  اگر (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t معلوم_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      برائے (حرف *آغاز = file; *(آغاز = component_آغاز (آغاز)); )
        {
          idx_t لمبائی;

          اگر (معلوم_name_max)
            name_max = معلوم_name_max;
          ورنہ
            {
              طویل صحیح len;
              حرف ثابت *dir = (آغاز == file ? "." : file);
              حرف c = *آغاز;
              errno = 0;
              *آغاز = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *آغاز = c;
              اگر (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              ورنہ
                انتخاب (errno)
                  {
                  حالت 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    توڑو;

                  حالت ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    معلوم_name_max = name_max;
                    توڑو;

                  طےشدہ:
                    *آغاز = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *آغاز = c;
                    واپس false;
                  }
            }

          لمبائی = component_len (آغاز);

          اگر (name_max < لمبائی)
            {
              حرف c = آغاز[لمبائی];
              آغاز[لمبائی] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, لمبائی, quote (آغاز));
              آغاز[لمبائی] = c;
              واپس false;
            }

          آغاز += لمبائی;
        }
    }

  واپس true;
}
