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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <wchar.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "pathchk"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Eggert"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

#నిర్వచితం_కాకపోతే _POSIX_PATH_MAX
# define _POSIX_PATH_MAX 256
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే _POSIX_NAME_MAX
# define _POSIX_NAME_MAX 14
#అయితే_ముగింపు

#నిర్వచితమైతే _XOPEN_NAME_MAX
# define NAME_MAX_MINIMUM _XOPEN_NAME_MAX
#లేకపోతే
# define NAME_MAX_MINIMUM _POSIX_NAME_MAX
#అయితే_ముగింపు
#నిర్వచితమైతే XOPEN_మార్గం_MAX
# define మార్గం_MAX_MINIMUM XOPEN_మార్గం_MAX
#లేకపోతే
# define మార్గం_MAX_MINIMUM _POSIX_PATH_MAX
#అయితే_ముగింపు

#ఒకవేళ ! (HAVE_PATHCONF && defined _PC_NAME_MAX && defined PC_మార్గం_MAX)
# ifndef _PC_NAME_MAX
#  define _PC_NAME_MAX 0
#  define PC_మార్గం_MAX 1
# endif
# ifndef pathconf
#  define pathconf(file, flag) \
     (flag == _PC_NAME_MAX ? NAME_MAX_MINIMUM : మార్గం_MAX_MINIMUM)
# endif
#అయితే_ముగింపు

స్థిర bool validate_file_name (అక్షరం *, bool, bool);

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  PORTABILITY_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"portability", no_వాదం, ఖాళీ, PORTABILITY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... NAME...\n"), కార్యక్రమం_name);
      fputs (_("\
Diagnose invalid or non-portable file పేర్లు.\n\
\n\
"), stdout);
      oputs (_("\
  -p     check కొరకు most POSIX systems\n\
"));
      oputs (_("\
  -P     check కొరకు ఖాళీలు పేర్లు and leading \"-\"\n\
"));
      oputs (_("\
      --portability\n\
         check కొరకు all POSIX systems (equivalent గమ్యం -p -P)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool ok = true;
  bool check_basic_portability = false;
  bool check_extra_portability = false;
  పూర్ణాంకం optc;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "+pP", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి PORTABILITY_OPTION:
          check_basic_portability = true;
          check_extra_portability = true;
          ఆపు;

        స్థితి 'p':
          check_basic_portability = true;
          ఆపు;

        స్థితి 'P':
          check_extra_portability = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  కొరకు (; optind < argc; ++optind)
    ok &= validate_file_name (argv[optind],
                              check_basic_portability, check_extra_portability);

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* If FILE contains a component with a leading "-", report an error
   and return false; otherwise, return true.  */

స్థిర bool
no_leading_hyphen (అక్షరం స్థిరరాశి *file)
{
  కొరకు (అక్షరం స్థిరరాశి *p = file;  (p = strchr (p, '-'));  p++)
    ఒకవేళ (p == file || p[-1] == '/')
      {
        error (0, 0, _("leading '-' in a component of file name %s"),
               quoteaf (file));
        తిరిగి false;
      }

  తిరిగి true;
}

/* If FILE (of length FILELEN) contains only portable characters,
   return true, else report an error and return false.  */

స్థిర bool
portable_chars_only (అక్షరం స్థిరరాశి *file, size_t filelen)
{
  size_t validlen = strspn (file,
                            ("/"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789._-"));
  అక్షరం స్థిరరాశి *invalid = file + validlen;

  ఒకవేళ (*invalid)
    {
      mbstate_t mbstate; mbszero (&mbstate);
      size_t charlen = mbrlen (invalid, filelen - validlen, &mbstate);
      error (0, 0,
             _("non-portable character %s in file name %s"),
             quotearg_n_style_mem (1, locale_quoting_style, invalid,
                                   (charlen <= MB_LEN_MAX ? charlen : 1)),
             quoteaf_n (0, file));
      తిరిగి false;
    }

  తిరిగి true;
}

/* Return the address of the start of the next file name component in F.  */

ATTRIBUTE_శుద్ధ
స్థిర అక్షరం *
component_ప్రారంభం (అక్షరం *f)
{
  అయితే (*f == '/')
    f++;
  తిరిగి f;
}

/* Return the size of the file name component F.  F must be nonempty.  */

ATTRIBUTE_శుద్ధ
స్థిర size_t
component_len (అక్షరం స్థిరరాశి *f)
{
  size_t len;
  కొరకు (len = 1; f[len] != '/' && f[len]; len++)
    కొనసాగు;
  తిరిగి len;
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

స్థిర bool
validate_file_name (అక్షరం *file, bool check_basic_portability,
                    bool check_extra_portability)
{
  idx_t filelen = అక్షరశ్రేణి_పొడవు (file);

  /* True if component lengths need to be checked.  */
  bool check_component_lengths;

  /* True if the file is known to exist.  */
  bool file_exists = false;

  ఒకవేళ (check_extra_portability && ! no_leading_hyphen (file))
    తిరిగి false;

  ఒకవేళ ((check_basic_portability || check_extra_portability)
      && filelen == 0)
    {
      /* Fail, since empty names are not portable.  As of
         2005-01-06 POSIX does not address whether "pathchk -p ''"
         should (or is allowed to) fail, so this is not a
         conformance violation.  */
      error (0, 0, _("empty file name"));
      తిరిగి false;
    }

  ఒకవేళ (check_basic_portability)
    {
      ఒకవేళ (! portable_chars_only (file, filelen))
        తిరిగి false;
    }
  లేకపోతే
    {
      /* Check whether a file name component is in a directory that
         is not searchable, or has some other serious problem.
         POSIX does not allow "" as a file name, but some non-POSIX
         hosts do (as an alias for "."), so allow "" if lstat does.  */

      నిర్మాణం stat st;
      ఒకవేళ (lstat (file, &st) == 0)
        file_exists = true;
      లేకపోతే ఒకవేళ (errno != ENOENT || filelen == 0)
        {
          error (0, errno, "%s", quotef (file));
          తిరిగి false;
        }
    }

  ఒకవేళ (check_basic_portability
      || (! file_exists && మార్గం_MAX_MINIMUM <= filelen))
    {
      idx_t maxsize;

      ఒకవేళ (check_basic_portability)
        maxsize = _POSIX_PATH_MAX;
      లేకపోతే
        {
          దీర్ఘ పూర్ణాంకం కొలత;
          అక్షరం స్థిరరాశి *dir = (*file == '/' ? "/" : ".");
          errno = 0;
          కొలత = pathconf (dir, PC_మార్గం_MAX);
          ఒకవేళ (కొలత < 0 && errno != 0)
            {
              error (0, errno,
                     _("%s: unable to determine maximum file name length"),
                     dir);
              తిరిగి false;
            }
          maxsize = MIN (కొలత, MIN (SSIZE_MAX, IDX_MAX));
        }

      ఒకవేళ (maxsize <= filelen)
        {
          error (0, 0, _("limit %td exceeded by length %td of file name %s"),
                 maxsize - 1, filelen, quoteaf (file));
          తిరిగి false;
        }
    }

  /* Check whether pathconf (..., _PC_NAME_MAX) can be avoided, i.e.,
     whether all file name components are so short that they are valid
     in any file system on this platform.  If CHECK_BASIC_PORTABILITY, though,
     it's more convenient to check component lengths below.  */

  check_component_lengths = check_basic_portability;
  ఒకవేళ (! check_component_lengths && ! file_exists)
    {
      కొరకు (అక్షరం *ప్రారంభం = file; *(ప్రారంభం = component_ప్రారంభం (ప్రారంభం)); )
        {
          size_t నిడివి = component_len (ప్రారంభం);

          ఒకవేళ (NAME_MAX_MINIMUM < నిడివి)
            {
              check_component_lengths = true;
              ఆపు;
            }

          ప్రారంభం += నిడివి;
        }
    }

  ఒకవేళ (check_component_lengths)
    {
      /* The limit on file name components for the current component.
         This defaults to NAME_MAX_MINIMUM, for the sake of non-POSIX
         systems (NFS, say?) where pathconf fails on "." or "/" with
         errno == ENOENT.  */
      idx_t name_max = NAME_MAX_MINIMUM;

      /* If nonzero, the known limit on file name components.  */
      idx_t తెలిసిన_name_max = check_basic_portability ? _POSIX_NAME_MAX : 0;

      కొరకు (అక్షరం *ప్రారంభం = file; *(ప్రారంభం = component_ప్రారంభం (ప్రారంభం)); )
        {
          idx_t నిడివి;

          ఒకవేళ (తెలిసిన_name_max)
            name_max = తెలిసిన_name_max;
          లేకపోతే
            {
              దీర్ఘ పూర్ణాంకం len;
              అక్షరం స్థిరరాశి *dir = (ప్రారంభం == file ? "." : file);
              అక్షరం c = *ప్రారంభం;
              errno = 0;
              *ప్రారంభం = '\0';
              len = pathconf (dir, _PC_NAME_MAX);
              *ప్రారంభం = c;
              ఒకవేళ (0 <= len)
                name_max = MIN (len, MIN (SSIZE_MAX, IDX_MAX));
              లేకపోతే
                ఎంపిక (errno)
                  {
                  స్థితి 0:
                    /* There is no limit.  */
                    name_max = IDX_MAX;
                    ఆపు;

                  స్థితి ENOENT:
                    /* DIR does not exist; use its parent's maximum.  */
                    తెలిసిన_name_max = name_max;
                    ఆపు;

                  అప్రమేయ:
                    *ప్రారంభం = '\0';
                    error (0, errno, "%s", quotef (dir));
                    *ప్రారంభం = c;
                    తిరిగి false;
                  }
            }

          నిడివి = component_len (ప్రారంభం);

          ఒకవేళ (name_max < నిడివి)
            {
              అక్షరం c = ప్రారంభం[నిడివి];
              ప్రారంభం[నిడివి] = '\0';
              error (0, 0,
                     _("limit %td exceeded by length %td "
                       "of file name component %s"),
                     name_max, నిడివి, quote (ప్రారంభం));
              ప్రారంభం[నిడివి] = c;
              తిరిగి false;
            }

          ప్రారంభం += నిడివి;
        }
    }

  తిరిగి true;
}
