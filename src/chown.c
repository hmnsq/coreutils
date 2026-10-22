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
#define PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
स्थिर वर्ण *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = अक्षर_उच्च + 1,
  FROM_OPTION,
  NO_PRESERVE_जरा,
  PRESERVE_जरा,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option अचर long_options[] =
{
  {"recursive", no_argument, रिक्त, 'R'},
  {"changes", no_argument, रिक्त, 'c'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"from", required_argument, रिक्त, FROM_OPTION},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_जरा},
  {"preserve-root", no_argument, रिक्त, PRESERVE_जरा},
  {"quiet", no_argument, रिक्त, 'f'},
  {"silent", no_argument, रिक्त, 'f'},
  {"reference", required_argument, रिक्त, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... %s खाता...\n\
  or:  %s [OPTION]... --reference=RFILE खाता...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      यदि (chown_mode == CHOWN_CHOWN)
        ख_माला_दो (_("\
Change the owner and/or group of each खाता to OWNER and/or GROUP.\n\
With --reference, change the owner and group of each खाता to those of RFILE.\n\
\n\
"), मानक_निकास);
      अन्यथा
        ख_माला_दो (_("\
Change the group of each खाता to GROUP.\n\
With --reference, change the group of each खाता to that of RFILE.\n\
\n\
"), मानक_निकास);
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
         output a diagnostic लागि every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the स्वभाव), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only यदि its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which पक्ष a match\n\
         is not required लागि the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-जरा\n\
         गर not treat '/' specially (the स्वभाव)\n\
"));
      oputs (_("\
      --preserve-जरा\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying मानहरू.\n\
         RFILE is always dereferenced यदि a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      यदि (chown_mode == CHOWN_CHOWN)
        ख_माला_दो (_("\
\n\
Owner is unchanged यदि missing.  Group is unchanged यदि missing, but changed\n\
to login group यदि implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), मानक_निकास);

      यदि (chown_mode == CHOWN_CHOWN)
        म_लिखो (_("\
\n\
Examples:\n\
  %s जरा /u        Change the owner of /u to \"जरा\".\n\
  %s जरा:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR जरा /u    Change the owner of /u and subfiles to \"जरा\".\n\
"),
              program_name, program_name, program_name);
      अन्यथा
        म_लिखो (_("\
\n\
Examples:\n\
  %s staff /u      Change the group of /u to \"staff\".\n\
  %s -hR staff /u  Change the group of /u and subfiles to \"staff\".\n\
"),
              program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool preserve_जरा = false;

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
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  chopt_init (&chopt);

  पूर्णाङ्क optc;
  जबसम्म ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, रिक्त))
         != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          रोक;

        पक्ष 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          रोक;

        पक्ष 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          रोक;

        पक्ष 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          रोक;

        पक्ष DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          रोक;

        पक्ष NO_PRESERVE_जरा:
          preserve_जरा = false;
          रोक;

        पक्ष PRESERVE_जरा:
          preserve_जरा = true;
          रोक;

        पक्ष REFERENCE_FILE_OPTION:
          reference_file = optarg;
          रोक;

        पक्ष FROM_OPTION:
          {
            bool warn;
            वर्ण अचर *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  रिक्त, रिक्त, &warn);
            यदि (e)
              error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (optarg));
            रोक;
          }

        पक्ष 'R':
          chopt.recurse = true;
          रोक;

        पक्ष 'c':
          chopt.verbosity = V_changes_only;
          रोक;

        पक्ष 'f':
          chopt.force_silent = true;
          रोक;

        पक्ष 'v':
          chopt.verbosity = V_माथिल्लो;
          रोक;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (chopt.recurse)
    {
      यदि (bit_flags == FTS_PHYSICAL)
        {
          यदि (dereference == 1)
            error (निकास_त्रुटि, 0,
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
      usage (निकास_त्रुटि);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  यदि (reference_file)
    {
      संरचना stat ref_stats;
      यदि (stat (reference_file, &ref_stats))
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      यदि (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  अन्यथा
    {
      वर्ण *ug = argv[optind];
      यदि (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + म_माप (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      वर्ण अचर *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      यदि (ug != argv[optind])
        मुक्त (ug);

      यदि (e)
        error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      यदि (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  यदि (chopt.recurse && preserve_जरा)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      chopt.जरा_dev_ino = get_जरा_dev_ino (&dev_ino_buf);
      यदि (chopt.जरा_dev_ino == रिक्त)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
