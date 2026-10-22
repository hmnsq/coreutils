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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <wchar.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

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
#ଅନ୍ୟଥା
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#endif
#ifdef _XOPEN_PATH_MAX
# define PATH_MAX_MINIMUM _XOPEN_PATH_MAX
#ଅନ୍ୟଥା
# define PATH_MAX_MINIMUM _POSIX_PATH_MAX
#endif

#ଯଦି ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined _PC_PATH_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define _PC_PATH_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : PATH_MAX_MINIMUM)
# endif
#endif

ସ୍ଥିର bool validate_file_name (ଅକ୍ଷର *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"portability", no_argument, ଖାଲି, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      fputs (_("\
Diagnose invalid or non-portable file names.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check ପାଇଁ most POSIX systems\n\
"));
      oputs (_("\
  -P     check ପାଇଁ empty names and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check ପାଇଁ all POSIX systems (equivalent to -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "+pP", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'p':
          check_basic_portability = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'P':
          check_extra_portability = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ପାଇଁ (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

ସ୍ଥିର bool
no_leading_hyphen (ଅକ୍ଷର ଅଚଳ *file)
{
  ପାଇଁ (ଅକ୍ଷର ଅଚଳ *p = file;  (p = strchr (p, '-'));  p++)
    ଯଦି (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        ଫେରନ୍ତୁ false;
      }

  ଫେରନ୍ତୁ true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

ସ୍ଥିର bool
portable_chars_only (ଅକ୍ଷର ଅଚଳ *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  ଅକ୍ଷର ଅଚଳ *invalid = file + validlen;

  ଯଦି (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର *
component_start (ଅକ୍ଷର *f)
{
  ଯେତେବେଳେ (*f == '/')
    f++;
  ଫେରନ୍ତୁ f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_PURE
ସ୍ଥିର size_t
component_len (ଅକ୍ଷର ଅଚଳ *f)
{
  size_t len;
  ପାଇଁ (len = 1; f[len] != '/' && f[len]; len++)
    ଜାରି;
  ଫେରନ୍ତୁ len;
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

ସ୍ଥିର bool
validate_file_name (ଅକ୍ଷର *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = strlen (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  ଯଦି (check_extra_portability && ! no_leading_hyphen (file))
    ଫେରନ୍ତୁ false;

  ଯଦି ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (check_basic_portability)
    {
      ଯଦି (! portable_chars_only (file, filelen))
        ଫେରନ୍ତୁ false;
    }
  ଅନ୍ୟଥା
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      ସଂରଚନା stat st;
      ଯଦି (lstat (file, &st) == 0)
        file_exists = true;
      ଅନ୍ୟଥା ଯଦି (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          ଫେରନ୍ତୁ false;
        }
    }

  ଯଦି (check_basic_portability
      || (! file_exists && PATH_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      ଯଦି (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      ଅନ୍ୟଥା
        {
          long ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
          ଅକ୍ଷର ଅଚଳ *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          size = pathconf (dir, _PC_PATH_MAX);
          ଯଦି (size < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              ଫେରନ୍ତୁ false;
            }
          maxsize = MIN (size, MIN (SSIZE_MAX, IDX_MAX));
        }

      ଯଦି (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          ଫେରନ୍ତୁ false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  ଯଦି (! check_component_lengths && ! file_exists)
    {
      ପାଇଁ (ଅକ୍ଷର *start = file; *(start = component_start (start)); )
        {
          size_t length = component_len (start);

          ଯଦି (NAME_MAX_MINIMUM < length)
            {
              check_component_lengths = true;
              ଭାଙ୍ଗ;
            }

          start += length;
        }
    }

  ଯଦି (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t known_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      ପାଇଁ (ଅକ୍ଷର *start = file; *(start = component_start (start)); )
        {
          idx_t length;

          ଯଦି (known_name_max)
            name_max = known_name_max;
          ଅନ୍ୟଥା
            {
              long ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
              ଅକ୍ଷର ଅଚଳ *dir = (start == file ? "." : file);
              ଅକ୍ଷର c = *start;
              errno = 0;
              *start = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *start = c;
              ଯଦି (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              ଅନ୍ୟଥା
                ବାଛ (errno)
                  {
                  ସ୍ଥିତି 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    ଭାଙ୍ଗ;

                  ସ୍ଥିତି ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    known_name_max = name_max;
                    ଭାଙ୍ଗ;

                  ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                    *start = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *start = c;
                    ଫେରନ୍ତୁ false;
                  }
            }

          length = component_len (start);

          ଯଦି (name_max < length)
            {
              ଅକ୍ଷର c = start[length];
              start[length] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, length, quote (start));
              start[length] = c;
              ଫେରନ୍ତୁ false;
            }

          start += length;
        }
    }

  ଫେରନ୍ତୁ true;
}
