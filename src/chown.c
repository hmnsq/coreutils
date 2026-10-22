/* chown, chgrp -- change user and group ownership of files
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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu>. */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "chown.h"
#अन्तर्भाव "chown-core.h"
#अन्तर्भाव "fts_.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME (chown_बहुलकम् == CHOWN_CHOWN ? "chown" : "chgrp")

#परिभाषय AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
स्थिर वर्ण *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  FROM_OPTION,
  NO_PRESERVE_मूलम्,
  PRESERVE_मूलम्,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"recursive", no_तर्कः, रिक्त, 'R'},
  {"changes", no_तर्कः, रिक्त, 'c'},
  {"dereference", no_तर्कः, रिक्त, DEREFERENCE_OPTION},
  {"from", required_तर्कः, रिक्त, FROM_OPTION},
  {"no-dereference", no_तर्कः, रिक्त, 'h'},
  {"no-preserve-root", no_तर्कः, रिक्त, NO_PRESERVE_मूलम्},
  {"preserve-root", no_तर्कः, रिक्त, PRESERVE_मूलम्},
  {"quiet", no_तर्कः, रिक्त, 'f'},
  {"silent", no_तर्कः, रिक्त, 'f'},
  {"reference", required_तर्कः, रिक्त, REFERENCE_FILE_OPTION},
  {"verbose", no_तर्कः, रिक्त, 'v'},
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
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... %s FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              क्रमादेशः_name,
              chown_बहुलकम् == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              क्रमादेशः_name);
      यदि (chown_बहुलकम् == CHOWN_CHOWN)
        fputs (_("\
Change the owner and/or गणः सम्बन्धि each FILE गन्तव्यम् OWNER and/or गणः_2.\n\
With --reference, change the owner and गणः सम्बन्धि each FILE गन्तव्यम् those सम्बन्धि RFILE.\n\
\n\
"), stdout);
      अन्यथा
        fputs (_("\
Change the गणः सम्बन्धि each FILE गन्तव्यम् गणः_2.\n\
With --reference, change the गणः सम्बन्धि each FILE गन्तव्यम् that सम्बन्धि RFILE.\n\
\n\
"), stdout);
      oputs (_("\
  -c, --changes\n\
         like verbose but report only when a change is made\n\
"));
      oputs (_("\
  -f, --silent, --quiet\n\
         suppress most error messages\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic प्रति every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent सम्बन्धि each symbolic link (this is\n\
         the स्वभाव), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead सम्बन्धि any referenced file;\n\
         useful only on systems that can change the ownership सम्बन्धि a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_गणः\n\
         change the ownership सम्बन्धि each file only यदि its\n\
         current owner and/or गणः मेलनम् those specified अत्र.\n\
         Either may be omitted, अन्तर् which पक्ष a मेलनम्\n\
         is not required प्रति the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-मूलम्\n\
         कुरु not treat '/' specially (the स्वभाव)\n\
"));
      oputs (_("\
      --preserve-मूलम्\n\
         fail गन्तव्यम् सञ्चालय recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying मानानि.\n\
         RFILE is always dereferenced यदि a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         सञ्चालय on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      यदि (chown_बहुलकम् == CHOWN_CHOWN)
        fputs (_("\
\n\
Owner is unchanged यदि missing.  गणः_3 is unchanged यदि missing, but परिवर्तितम्\n\
गन्तव्यम् login गणः यदि implied by a ':' following a symbolic OWNER.\n\
OWNER and गणः_2 may be numeric as well as symbolic.\n\
"), stdout);

      यदि (chown_बहुलकम् == CHOWN_CHOWN)
        प्रारूप_लिख (_("\
\n\
Examples:\n\
  %s मूलम् /u        Change the owner सम्बन्धि /u गन्तव्यम् \"मूलम्\".\n\
  %s मूलम्:staff /u  Likewise, but also change its गणः गन्तव्यम् \"staff\".\n\
  %s -hR मूलम् /u    Change the owner सम्बन्धि /u and subfiles गन्तव्यम् \"मूलम्\".\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      अन्यथा
        प्रारूप_लिख (_("\
\n\
Examples:\n\
  %s staff /u      Change the गणः सम्बन्धि /u गन्तव्यम् \"staff\".\n\
  %s -hR staff /u  Change the गणः सम्बन्धि /u and subfiles गन्तव्यम् \"staff\".\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool preserve_मूलम् = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  पूर्णाङ्क bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णाङ्क dereference = -1;

  संरचना Chown_option chopt;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  chopt_init (&chopt);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          भङ्ग;

        पक्ष 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          भङ्ग;

        पक्ष DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          भङ्ग;

        पक्ष NO_PRESERVE_मूलम्:
          preserve_मूलम् = false;
          भङ्ग;

        पक्ष PRESERVE_मूलम्:
          preserve_मूलम् = true;
          भङ्ग;

        पक्ष REFERENCE_FILE_OPTION:
          reference_file = optarg;
          भङ्ग;

        पक्ष FROM_OPTION:
          {
            bool warn;
            वर्ण ध्रुव *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  रिक्त, रिक्त, &warn);
            यदि (e)
              error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (optarg));
            भङ्ग;
          }

        पक्ष 'R':
          chopt.recurse = true;
          भङ्ग;

        पक्ष 'c':
          chopt.verbosity = V_changes_only;
          भङ्ग;

        पक्ष 'f':
          chopt.force_silent = true;
          भङ्ग;

        पक्ष 'v':
          chopt.verbosity = V_उच्चः;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (chopt.recurse)
    {
      यदि (bit_flags == FTS_PHYSICAL)
        {
          यदि (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }
  अन्यथा
    {
      bit_flags = FTS_PHYSICAL;
    }
  chopt.affect_symlink_referent = (dereference != 0);

  यदि (argc - optind < (reference_file ? 1 : 2))
    {
      यदि (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  यदि (reference_file)
    {
      संरचना stat ref_stats;
      यदि (stat (reference_file, &ref_stats))
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      यदि (chown_बहुलकम् == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_गन्तव्यम्_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.गणः_name = gid_गन्तव्यम्_name (ref_stats.st_gid);
    }
  अन्यथा
    {
      वर्ण *ug = argv[optind];
      यदि (chown_बहुलकम् == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + वाक्यदैर्घ्यम् (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      वर्ण ध्रुव *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.गणः_name, &warn);

      यदि (ug != argv[optind])
        मुक्त (ug);

      यदि (e)
        error (warn ? 0 : EXIT_FAILURE, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      यदि (chown_बहुलकम् == CHOWN_CHOWN && !chopt.user_name && chopt.गणः_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  यदि (chopt.recurse && preserve_मूलम्)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      chopt.मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);
      यदि (chopt.मूलम्_dev_ino == रिक्त)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
