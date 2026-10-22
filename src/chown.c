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
स्थिर अक्षर *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = अक्षर_उच्च + 1,
  FROM_OPTION,
  NO_PRESERVE_मूळ,
  PRESERVE_मूळ,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"recursive", no_argument, रिक्त, 'R'},
  {"changes", no_argument, रिक्त, 'c'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"from", required_argument, रिक्त, FROM_OPTION},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूळ},
  {"preserve-root", no_argument, रिक्त, PRESERVE_मूळ},
  {"quiet", no_argument, रिक्त, 'f'},
  {"silent", no_argument, रिक्त, 'f'},
  {"reference", required_argument, रिक्त, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, रिक्त, 'v'},
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
      म_लिखो (_("\
Usage: %s [OPTION]... %s खाता...\n\
  or:  %s [OPTION]... --reference=RFILE खाता...\n\
"),
              program_name,
              chown_mode == CHOWN_CHOWN ? _("[OWNER][:[GROUP]]") : _("GROUP"),
              program_name);
      जर (chown_mode == CHOWN_CHOWN)
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
         output a diagnostic साठी every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the पूर्वनिर्धारित), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only जर its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which स्थिती a match\n\
         is not required साठी the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-मूळ\n\
         करा not treat '/' specially (the पूर्वनिर्धारित)\n\
"));
      oputs (_("\
      --preserve-मूळ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying मूल्ये.\n\
         RFILE is always dereferenced जर a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      जर (chown_mode == CHOWN_CHOWN)
        ख_माला_दो (_("\
\n\
Owner is unchanged जर missing.  Group is unchanged जर missing, but changed\n\
to login group जर implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), मानक_निकास);

      जर (chown_mode == CHOWN_CHOWN)
        म_लिखो (_("\
\n\
Examples:\n\
  %s मूळ /u        Change the owner of /u to \"मूळ\".\n\
  %s मूळ:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR मूळ /u    Change the owner of /u and subfiles to \"मूळ\".\n\
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

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool preserve_मूळ = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  पूर्णांक bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णांक dereference = -1;

  संरचना Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  chopt_init (&chopt);

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, रिक्त))
         != -1)
    {
      निवड (optc)
        {
        स्थिती 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          थांबा;

        स्थिती 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          थांबा;

        स्थिती 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          थांबा;

        स्थिती 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          थांबा;

        स्थिती DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          थांबा;

        स्थिती NO_PRESERVE_मूळ:
          preserve_मूळ = false;
          थांबा;

        स्थिती PRESERVE_मूळ:
          preserve_मूळ = true;
          थांबा;

        स्थिती REFERENCE_FILE_OPTION:
          reference_file = optarg;
          थांबा;

        स्थिती FROM_OPTION:
          {
            bool warn;
            अक्षर स्थिरांक *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  रिक्त, रिक्त, &warn);
            जर (e)
              error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (optarg));
            थांबा;
          }

        स्थिती 'R':
          chopt.recurse = true;
          थांबा;

        स्थिती 'c':
          chopt.verbosity = V_changes_only;
          थांबा;

        स्थिती 'f':
          chopt.force_silent = true;
          थांबा;

        स्थिती 'v':
          chopt.verbosity = V_वरचा;
          थांबा;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (chopt.recurse)
    {
      जर (bit_flags == FTS_PHYSICAL)
        {
          जर (dereference == 1)
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

  जर (argc - optind < (reference_file ? 1 : 2))
    {
      जर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  जर (reference_file)
    {
      संरचना stat ref_stats;
      जर (stat (reference_file, &ref_stats))
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      जर (chown_mode == CHOWN_CHOWN)
        {
          uid = ref_stats.st_uid;
          chopt.user_name = uid_to_name (ref_stats.st_uid);
        }
      gid = ref_stats.st_gid;
      chopt.group_name = gid_to_name (ref_stats.st_gid);
    }
  अन्यथा
    {
      अक्षर *ug = argv[optind];
      जर (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + म_माप (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      अक्षर स्थिरांक *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      जर (ug != argv[optind])
        मुक्त (ug);

      जर (e)
        error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      जर (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  जर (chopt.recurse && preserve_मूळ)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      chopt.मूळ_dev_ino = get_मूळ_dev_ino (&dev_ino_buf);
      जर (chopt.मूळ_dev_ino == रिक्त)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
