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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <wchar.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "quote.h"

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
#இல்லையெனில்
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#endif
#ifdef _XOPEN_PATH_MAX
# define PATH_MAX_MINIMUM _XOPEN_PATH_MAX
#இல்லையெனில்
# define PATH_MAX_MINIMUM _POSIX_PATH_MAX
#endif

#எனில் ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined _PC_PATH_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define _PC_PATH_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : PATH_MAX_MINIMUM)
# endif
#endif

நிலையான bool validate_file_name (எழுத்து *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"portability", no_argument, வெற்று, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      fputs (_("\
Diagnose invalid or non-portable file names.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check க்கு most POSIX systems\n\
"));
      oputs (_("\
  -P     check க்கு empty names and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check க்கு all POSIX systems (equivalent to -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  முழு optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "+pP", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          நிறுத்து;

        நிலை 'p':
          check_basic_portability = true;
          நிறுத்து;

        நிலை 'P':
          check_extra_portability = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  க்கு (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

நிலையான bool
no_leading_hyphen (எழுத்து மாறிலி *file)
{
  க்கு (எழுத்து மாறிலி *p = file;  (p = strchr (p, '-'));  p++)
    எனில் (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        திரும்பு false;
      }

  திரும்பு true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

நிலையான bool
portable_chars_only (எழுத்து மாறிலி *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  எழுத்து மாறிலி *invalid = file + validlen;

  எனில் (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      திரும்பு false;
    }

  திரும்பு true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_PURE
நிலையான எழுத்து *
component_start (எழுத்து *f)
{
  போது (*f == '/')
    f++;
  திரும்பு f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_PURE
நிலையான size_t
component_len (எழுத்து மாறிலி *f)
{
  size_t len;
  க்கு (len = 1; f[len] != '/' && f[len]; len++)
    தொடர்;
  திரும்பு len;
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

நிலையான bool
validate_file_name (எழுத்து *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = strlen (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  எனில் (check_extra_portability && ! no_leading_hyphen (file))
    திரும்பு false;

  எனில் ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      திரும்பு false;
    }

  எனில் (check_basic_portability)
    {
      எனில் (! portable_chars_only (file, filelen))
        திரும்பு false;
    }
  இல்லையெனில்
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      கட்டமைப்பு stat st;
      எனில் (lstat (file, &st) == 0)
        file_exists = true;
      இல்லையெனில் எனில் (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          திரும்பு false;
        }
    }

  எனில் (check_basic_portability
      || (! file_exists && PATH_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      எனில் (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      இல்லையெனில்
        {
          long முழு size;
          எழுத்து மாறிலி *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          size = pathconf (dir, _PC_PATH_MAX);
          எனில் (size < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              திரும்பு false;
            }
          maxsize = MIN (size, MIN (SSIZE_MAX, IDX_MAX));
        }

      எனில் (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          திரும்பு false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  எனில் (! check_component_lengths && ! file_exists)
    {
      க்கு (எழுத்து *start = file; *(start = component_start (start)); )
        {
          size_t length = component_len (start);

          எனில் (NAME_MAX_MINIMUM < length)
            {
              check_component_lengths = true;
              நிறுத்து;
            }

          start += length;
        }
    }

  எனில் (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t known_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      க்கு (எழுத்து *start = file; *(start = component_start (start)); )
        {
          idx_t length;

          எனில் (known_name_max)
            name_max = known_name_max;
          இல்லையெனில்
            {
              long முழு len;
              எழுத்து மாறிலி *dir = (start == file ? "." : file);
              எழுத்து c = *start;
              errno = 0;
              *start = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *start = c;
              எனில் (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              இல்லையெனில்
                தேர்வு (errno)
                  {
                  நிலை 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    நிறுத்து;

                  நிலை ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    known_name_max = name_max;
                    நிறுத்து;

                  இயல்பு:
                    *start = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *start = c;
                    திரும்பு false;
                  }
            }

          length = component_len (start);

          எனில் (name_max < length)
            {
              எழுத்து c = start[length];
              start[length] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, length, quote (start));
              start[length] = c;
              திரும்பு false;
            }

          start += length;
        }
    }

  திரும்பு true;
}
