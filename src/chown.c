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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "chown.h"
#समावेश "chown-core.h"
#समावेश "fts_.h"
#समावेश "quote.h"
#समावेश "root-dev-ino.h"
#समावेश "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME (chown_mode == CHOWN_CHOWN ? "chown" : "chgrp")

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

/* The argument to the --reference option.  Use the owner and group IDs
   of this file.  This file must exist.  */
अटल अक्षर *reference_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  DEREFERENCE_OPTION = अक्षर_उच्च + 1,
  FROM_OPTION,
  NO_PRESERVE_मूल,
  PRESERVE_मूल,
  REFERENCE_FILE_OPTION
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"recursive", no_argument, शून्य, 'R'},
  {"changes", no_argument, शून्य, 'c'},
  {"dereference", no_argument, शून्य, DEREFERENCE_OPTION},
  {"from", required_argument, शून्य, FROM_OPTION},
  {"no-dereference", no_argument, शून्य, 'h'},
  {"no-preserve-root", no_argument, शून्य, NO_PRESERVE_मूल},
  {"preserve-root", no_argument, शून्य, PRESERVE_मूल},
  {"quiet", no_argument, शून्य, 'f'},
  {"silent", no_argument, शून्य, 'f'},
  {"reference", required_argument, शून्य, REFERENCE_FILE_OPTION},
  {"verbose", no_argument, शून्य, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
      अगर (chown_mode == CHOWN_CHOWN)
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
         output a diagnostic क्रम every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the शेष), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file;\n\
         useful only on systems that can change the ownership of a symlink\n\
"));
      oputs (_("\
      --from=CURRENT_OWNER:CURRENT_GROUP\n\
         change the ownership of each file only अगर its\n\
         current owner and/or group match those specified here.\n\
         Either may be omitted, in which हाल a match\n\
         is not required क्रम the omitted attribute\n\
"));
      oputs (_("\
      --no-preserve-मूल\n\
         करो not treat '/' specially (the शेष)\n\
"));
      oputs (_("\
      --preserve-मूल\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's ownership rather than specifying मूल्यावली.\n\
         RFILE is always dereferenced अगर a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      अगर (chown_mode == CHOWN_CHOWN)
        ख_माला_दो (_("\
\n\
Owner is unchanged अगर missing.  Group is unchanged अगर missing, but changed\n\
to login group अगर implied by a ':' following a symbolic OWNER.\n\
OWNER and GROUP may be numeric as well as symbolic.\n\
"), मानक_निकास);

      अगर (chown_mode == CHOWN_CHOWN)
        म_लिखो (_("\
\n\
Examples:\n\
  %s मूल /u        Change the owner of /u to \"मूल\".\n\
  %s मूल:staff /u  Likewise, but also change its group to \"staff\".\n\
  %s -hR मूल /u    Change the owner of /u and subfiles to \"मूल\".\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool preserve_मूल = false;

  /* Change the owner (group) of a file only if it has this uid (gid).
     -1 means there's no restriction.  */
  uid_t required_uid = -1;
  gid_t required_gid = -1;

  /* Bit flags that control how fts works.  */
  पूर्णांक bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णांक dereference = -1;

  काष्ठा Chown_option chopt;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  chopt_init (&chopt);

  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, "HLPRcfhv", long_options, शून्य))
         != -1)
    {
      चयन (optc)
        {
        हाल 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          अवरोध;

        हाल 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          अवरोध;

        हाल 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          अवरोध;

        हाल 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          अवरोध;

        हाल DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          अवरोध;

        हाल NO_PRESERVE_मूल:
          preserve_मूल = false;
          अवरोध;

        हाल PRESERVE_मूल:
          preserve_मूल = true;
          अवरोध;

        हाल REFERENCE_FILE_OPTION:
          reference_file = optarg;
          अवरोध;

        हाल FROM_OPTION:
          {
            bool warn;
            अक्षर स्थिर *e = parse_user_spec_warn (optarg,
                                                  &required_uid, &required_gid,
                                                  शून्य, शून्य, &warn);
            अगर (e)
              error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (optarg));
            अवरोध;
          }

        हाल 'R':
          chopt.recurse = true;
          अवरोध;

        हाल 'c':
          chopt.verbosity = V_changes_only;
          अवरोध;

        हाल 'f':
          chopt.force_silent = true;
          अवरोध;

        हाल 'v':
          chopt.verbosity = V_ऊपरी;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (chopt.recurse)
    {
      अगर (bit_flags == FTS_PHYSICAL)
        {
          अगर (dereference == 1)
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

  अगर (argc - optind < (reference_file ? 1 : 2))
    {
      अगर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  uid_t uid = -1;	/* Specified uid; -1 if not to be changed.  */
  gid_t gid = -1;	/* Specified gid; -1 if not to be changed.  */

  अगर (reference_file)
    {
      काष्ठा stat ref_stats;
      अगर (stat (reference_file, &ref_stats))
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf (reference_file));

      अगर (chown_mode == CHOWN_CHOWN)
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
      अगर (chown_mode == CHOWN_CHGRP)
        {
          ug = xmalloc (1 + म_माप (argv[optind]) + 1);
          stpcpy (stpcpy (ug, ":"), argv[optind]);
        }

      bool warn;
      अक्षर स्थिर *e = parse_user_spec_warn (ug, &uid, &gid,
                                            &chopt.user_name,
                                            &chopt.group_name, &warn);

      अगर (ug != argv[optind])
        मुक्त (ug);

      अगर (e)
        error (warn ? 0 : निकास_त्रुटि, 0, "%s: %s", e, quote (argv[optind]));

      /* If a group is specified but no user, set the user name to the
         empty string so that diagnostics say "ownership :GROUP"
         rather than "group GROUP".  */
      अगर (chown_mode == CHOWN_CHOWN && !chopt.user_name && chopt.group_name)
        chopt.user_name = xstrdup ("");

      optind++;
    }

  अगर (chopt.recurse && preserve_मूल)
    {
      अटल काष्ठा dev_ino dev_ino_buf;
      chopt.मूल_dev_ino = get_मूल_dev_ino (&dev_ino_buf);
      अगर (chopt.मूल_dev_ino == शून्य)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }

  bit_flags |= FTS_DEFER_STAT;
  bool ok = chown_files (argv + optind, bit_flags,
                         uid, gid,
                         required_uid, required_gid, &chopt);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
