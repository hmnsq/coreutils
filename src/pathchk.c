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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <wchar.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "pathchk"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Eggert"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 256
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _POSIX_NAME_MAX
# define _POSIX_NAME_MAX 14
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _XOPEN_NAME_MAX
# define NAME_MAX_MINIMUM _XOPEN_NAME_MAX
#ਨਹੀਂਤਾਂ
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XOPEN_ਰਸਤਾ_MAX
# define ਰਸਤਾ_MAX_MINIMUM XOPEN_ਰਸਤਾ_MAX
#ਨਹੀਂਤਾਂ
# define ਰਸਤਾ_MAX_MINIMUM _POSIX_PATH_MAX
#ਜੇ_ਅੰਤ

#ਜੇ ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined PC_ਰਸਤਾ_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define PC_ਰਸਤਾ_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : ਰਸਤਾ_MAX_MINIMUM)
# endif
#ਜੇ_ਅੰਤ

ਸਥਿਰ bool validate_file_name (ਅੱਖਰ *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"portability", no_ਦਲੀਲ, ਨਿਰਾ, PORTABILITY_OPTION},
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
      ਲਿਖੋ (_("Usage: %s [OPTION]... NAME...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Diagnose invalid or non-portable file ਨਾਂ.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check ਲਈ most POSIX systems\n\
"));
      oputs (_("\
  -P     check ਲਈ ਖਾਲੀ_ਥਾਵਾਂ ਨਾਂ and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check ਲਈ all POSIX systems (equivalent ਮੰਜ਼ਿਲ -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  ਪੂਰਨ optc;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "+pP", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'p':
          check_basic_portability = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'P':
          check_extra_portability = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਲਈ (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

ਸਥਿਰ bool
no_leading_hyphen (ਅੱਖਰ ਅਚਲ *file)
{
  ਲਈ (ਅੱਖਰ ਅਚਲ *p = file;  (p = strchr (p, '-'));  p++)
    ਜੇ (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        ਵਾਪਸ false;
      }

  ਵਾਪਸ true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

ਸਥਿਰ bool
portable_chars_only (ਅੱਖਰ ਅਚਲ *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  ਅੱਖਰ ਅਚਲ *invalid = file + validlen;

  ਜੇ (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ *
component_ਸ਼ੁਰੂ (ਅੱਖਰ *f)
{
  ਜਦੋਂ (*f == '/')
    f++;
  ਵਾਪਸ f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ size_t
component_len (ਅੱਖਰ ਅਚਲ *f)
{
  size_t len;
  ਲਈ (len = 1; f[len] != '/' && f[len]; len++)
    ਜਾਰੀ;
  ਵਾਪਸ len;
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

ਸਥਿਰ bool
validate_file_name (ਅੱਖਰ *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = ਲੜੀ_ਲੰਬਾਈ (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  ਜੇ (check_extra_portability && ! no_leading_hyphen (file))
    ਵਾਪਸ false;

  ਜੇ ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      ਵਾਪਸ false;
    }

  ਜੇ (check_basic_portability)
    {
      ਜੇ (! portable_chars_only (file, filelen))
        ਵਾਪਸ false;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      ਬਣਤਰ stat st;
      ਜੇ (lstat (file, &st) == 0)
        file_exists = true;
      ਨਹੀਂਤਾਂ ਜੇ (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          ਵਾਪਸ false;
        }
    }

  ਜੇ (check_basic_portability
      || (! file_exists && ਰਸਤਾ_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      ਜੇ (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      ਨਹੀਂਤਾਂ
        {
          ਲੰਮਾ ਪੂਰਨ ਮਾਪ;
          ਅੱਖਰ ਅਚਲ *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          ਮਾਪ = pathconf (dir, PC_ਰਸਤਾ_MAX);
          ਜੇ (ਮਾਪ < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              ਵਾਪਸ false;
            }
          maxsize = MIN (ਮਾਪ, MIN (SSIZE_MAX, IDX_MAX));
        }

      ਜੇ (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          ਵਾਪਸ false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  ਜੇ (! check_component_lengths && ! file_exists)
    {
      ਲਈ (ਅੱਖਰ *ਸ਼ੁਰੂ = file; *(ਸ਼ੁਰੂ = component_ਸ਼ੁਰੂ (ਸ਼ੁਰੂ)); )
        {
          size_t ਲੰਮਾਈ = component_len (ਸ਼ੁਰੂ);

          ਜੇ (NAME_MAX_MINIMUM < ਲੰਮਾਈ)
            {
              check_component_lengths = true;
              ਰੋਕੋ;
            }

          ਸ਼ੁਰੂ += ਲੰਮਾਈ;
        }
    }

  ਜੇ (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t ਗਿਆਤ_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      ਲਈ (ਅੱਖਰ *ਸ਼ੁਰੂ = file; *(ਸ਼ੁਰੂ = component_ਸ਼ੁਰੂ (ਸ਼ੁਰੂ)); )
        {
          idx_t ਲੰਮਾਈ;

          ਜੇ (ਗਿਆਤ_name_max)
            name_max = ਗਿਆਤ_name_max;
          ਨਹੀਂਤਾਂ
            {
              ਲੰਮਾ ਪੂਰਨ len;
              ਅੱਖਰ ਅਚਲ *dir = (ਸ਼ੁਰੂ == file ? "." : file);
              ਅੱਖਰ c = *ਸ਼ੁਰੂ;
              errno = 0;
              *ਸ਼ੁਰੂ = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *ਸ਼ੁਰੂ = c;
              ਜੇ (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              ਨਹੀਂਤਾਂ
                ਚੋਣ (errno)
                  {
                  ਹਾਲਤ 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    ਰੋਕੋ;

                  ਹਾਲਤ ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    ਗਿਆਤ_name_max = name_max;
                    ਰੋਕੋ;

                  ਮੂਲ:
                    *ਸ਼ੁਰੂ = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *ਸ਼ੁਰੂ = c;
                    ਵਾਪਸ false;
                  }
            }

          ਲੰਮਾਈ = component_len (ਸ਼ੁਰੂ);

          ਜੇ (name_max < ਲੰਮਾਈ)
            {
              ਅੱਖਰ c = ਸ਼ੁਰੂ[ਲੰਮਾਈ];
              ਸ਼ੁਰੂ[ਲੰਮਾਈ] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, ਲੰਮਾਈ, quote (ਸ਼ੁਰੂ));
              ਸ਼ੁਰੂ[ਲੰਮਾਈ] = c;
              ਵਾਪਸ false;
            }

          ਸ਼ੁਰੂ += ਲੰਮਾਈ;
        }
    }

  ਵਾਪਸ true;
}
