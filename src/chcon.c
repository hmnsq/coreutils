/* chcon -- change security context of files
   Copyright (C) 2005-2026 Free Software Foundation, Inc.

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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>
#समावेश <selinux/selinux.h>

#समावेश "system.h"
#समावेश "dev-ino.h"
#समावेश "fts-missing.h"
#समावेश "ignore-value.h"
#समावेश "quote.h"
#समावेश "root-dev-ino.h"
#समावेश "selinux-at.h"
#समावेश "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "chcon"

#घोषणा AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
अटल bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
अटल bool recurse;

/* Level of verbosity. */
अटल bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
अटल काष्ठा dev_ino *मूल_dev_ino;

/* The name of the context file is being given. */
अटल अक्षर स्थिर *specified_context;

/* Specific components of the context */
अटल अक्षर स्थिर *specified_user;
अटल अक्षर स्थिर *specified_role;
अटल अक्षर स्थिर *specified_range;
अटल अक्षर स्थिर *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  DEREFERENCE_OPTION = अक्षर_उच्च + 1,
  NO_PRESERVE_मूल,
  PRESERVE_मूल,
  REFERENCE_FILE_OPTION
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"recursive", no_argument, शून्य, 'R'},
  {"dereference", no_argument, शून्य, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, शून्य, 'h'},
  {"no-preserve-root", no_argument, शून्य, NO_PRESERVE_मूल},
  {"preserve-root", no_argument, शून्य, PRESERVE_मूल},
  {"reference", required_argument, शून्य, REFERENCE_FILE_OPTION},
  {"user", required_argument, शून्य, 'u'},
  {"role", required_argument, शून्य, 'r'},
  {"type", required_argument, शून्य, 't'},
  {"range", required_argument, शून्य, 'l'},
  {"verbose", no_argument, शून्य, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
अटल पूर्णांक
compute_context_from_mask (अक्षर स्थिर *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  अगर (!new_context)
    {
      error (0, त्रुटि_सं, _("failed to create security context: %s"),
             quote (context));
      वापस 1;
    }

#घोषणा SET_COMPONENT(C, comp)						\
   करो									\
     {									\
       अगर (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, त्रुटि_सं,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   जबतक (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  अगर (!ok)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      context_free (new_context);
      त्रुटि_सं = saved_errno;
      वापस 1;
    }

  *ret = new_context;
  वापस 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

अटल पूर्णांक
change_file_context (पूर्णांक fd, अक्षर स्थिर *file, bool ignore_missing)
{
  अक्षर *file_context = शून्य;
  context_t context IF_LINT (= शून्य);
  अक्षर स्थिर * context_string;
  पूर्णांक errors = 0;

  अगर (specified_context == शून्य)
    {
      पूर्णांक status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      अगर (status < 0 && त्रुटि_सं != ENODATA)
        {
          अगर (ignore_missing && ignorable_traversal_errno (त्रुटि_सं))
            वापस 0;
          error (0, त्रुटि_सं, _("failed to get security context of %s"),
                 quoteaf (file));
          वापस 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      अगर (file_context == शून्य)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          वापस 1;
        }

      अगर (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          वापस 1;
        }

      context_string = context_str (context);
    }
  अन्यथा
    {
      context_string = specified_context;
    }

  अगर (file_context == शून्य || ! streq (context_string, file_context))
    {
      पूर्णांक fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      अगर (fail)
        {
          अगर (! (ignore_missing && ignorable_traversal_errno (त्रुटि_सं)))
            {
              errors = 1;
              error (0, त्रुटि_सं, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  अगर (specified_context == शून्य)
    {
      context_free (context);
      freecon (file_context);
    }

  वापस errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

अटल bool
process_file (FTS *fts, FTSENT *ent)
{
  अक्षर स्थिर *file_full_name = ent->fts_path;
  अक्षर स्थिर *file = ent->fts_accpath;
  स्थिर काष्ठा stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  अगर (ignore_missing && ignorable_fts_error (ent))
    वापस true;

  चयन (ent->fts_info)
    {
    हाल FTS_D:
      अगर (recurse)
        {
          अगर (मूल_DEV_INO_CHECK (मूल_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              मूल_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_मूल्य (fts_read (fts));
              वापस false;
            }
          वापस true;
        }
      अवरोध;

    हाल FTS_DP:
      अगर (! recurse)
        वापस true;
      अवरोध;

    हाल FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      अगर (ent->fts_level == FTS_ROOTLEVEL && ent->fts_संख्या == 0)
        {
          ent->fts_संख्या = 1;
          fts_set (fts, ent, FTS_AGAIN);
          वापस true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      अवरोध;

    हाल FTS_DC:		/* directory that causes cycles */
      अगर (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          वापस false;
        }
      अवरोध;

    शेष:
      अवरोध;
    }

  अगर (ent->fts_info == FTS_DP
      && ok && मूल_DEV_INO_CHECK (मूल_dev_ino, file_stats))
    {
      मूल_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  अगर (ok)
    {
      अगर (verbose)
        म_लिखो (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      अगर (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  अगर ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  वापस ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

अटल bool
process_files (अक्षर **files, पूर्णांक bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, शून्य);

  जबतक (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      अगर (ent == शून्य)
        {
          अगर (त्रुटि_सं != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, त्रुटि_सं, _("fts_read failed"));
              ok = false;
            }
          अवरोध;
        }

      ok &= process_file (fts, ent);
    }

  अगर (fts_close (fts) != 0)
    {
      error (0, त्रुटि_सं, _("fts_close failed"));
      ok = false;
    }

  वापस ok;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... CONTEXT खाता...\n\
  or:  %s [OPTION]... [-u USER] [-r ROLE] [-l RANGE] [-t TYPE] खाता...\n\
  or:  %s [OPTION]... --reference=RFILE खाता...\n\
"),
        program_name, program_name, program_name);
      ख_माला_दो (_("\
Change the SELinux security context of each खाता to CONTEXT.\n\
With --reference, change the security context of each खाता to that of RFILE.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
      --dereference\n\
         affect the referent of each symbolic link (this is\n\
         the शेष), rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect symbolic links instead of any referenced file\n\
"));
      oputs (_("\
  -u, --user=USER\n\
         set user USER in the target security context\n\
"));
      oputs (_("\
  -r, --role=ROLE\n\
         set role ROLE in the target security context\n\
"));
      oputs (_("\
  -t, --type=TYPE\n\
         set type TYPE in the target security context\n\
"));
      oputs (_("\
  -l, --range=RANGE\n\
         set range RANGE in the target security context\n\
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
         use RFILE's security context rather than specifying a CONTEXT मूल्य\n\
"));
      oputs (_("\
  -R, --recursive\n\
         operate on files and directories recursively\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic क्रम every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* Bit flags that control how fts works.  */
  पूर्णांक bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णांक dereference = -1;

  bool ok;
  bool preserve_मूल = false;
  bool component_specified = false;
  अक्षर *reference_file = शून्य;
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, शून्य))
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

        हाल 'R':
          recurse = true;
          अवरोध;

        हाल 'f':
          /* ignore */
          अवरोध;

        हाल 'v':
          verbose = true;
          अवरोध;

        हाल 'u':
          specified_user = optarg;
          component_specified = true;
          अवरोध;

        हाल 'r':
          specified_role = optarg;
          component_specified = true;
          अवरोध;

        हाल 't':
          specified_type = optarg;
          component_specified = true;
          अवरोध;

        हाल 'l':
          specified_range = optarg;
          component_specified = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (recurse)
    {
      अगर (bit_flags == FTS_PHYSICAL)
        {
          अगर (dereference == 1)
            error (निकास_त्रुटि, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      अन्यथा
        {
          अगर (dereference == 0)
            error (निकास_त्रुटि, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  अन्यथा
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  अगर (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      अगर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  अगर (reference_file)
    {
      अक्षर *ref_context = शून्य;

      अगर (getfilecon (reference_file, &ref_context) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  अन्यथा अगर (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = शून्य;
    }
  अन्यथा
    {
      specified_context = argv[optind++];
      अगर (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("invalid context: %s"),
               quote (specified_context));
    }

  अगर (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (निकास_त्रुटि);
    }

  अगर (recurse && preserve_मूल)
    {
      अटल काष्ठा dev_ino dev_ino_buf;
      मूल_dev_ino = get_मूल_dev_ino (&dev_ino_buf);
      अगर (मूल_dev_ino == शून्य)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  अन्यथा
    {
      मूल_dev_ino = शून्य;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
