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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <selinux/selinux.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "dev-ino.h"
#अन्तर्भाव "fts-missing.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "selinux-at.h"
#अन्तर्भाव "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chcon"

#define AUTHORS \
  proper_name ("Russell Coker"), \
  proper_name ("Jim Meyering")

/* If nonzero, and the systems has support for it, change the context
   of symbolic links rather than any files they point to.  */
स्थिर bool affect_symlink_referent;

/* If true, change the modes of directories recursively. */
स्थिर bool recurse;

/* Level of verbosity. */
स्थिर bool verbose;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
स्थिर संरचना dev_ino *मूळ_dev_ino;

/* The name of the context file is being given. */
स्थिर अक्षर स्थिरांक *specified_context;

/* Specific components of the context */
स्थिर अक्षर स्थिरांक *specified_user;
स्थिर अक्षर स्थिरांक *specified_role;
स्थिर अक्षर स्थिरांक *specified_range;
स्थिर अक्षर स्थिरांक *specified_type;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = अक्षर_उच्च + 1,
  NO_PRESERVE_मूळ,
  PRESERVE_मूळ,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"recursive", no_argument, रिक्त, 'R'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूळ},
  {"preserve-root", no_argument, रिक्त, PRESERVE_मूळ},
  {"reference", required_argument, रिक्त, REFERENCE_FILE_OPTION},
  {"user", required_argument, रिक्त, 'u'},
  {"role", required_argument, रिक्त, 'r'},
  {"type", required_argument, रिक्त, 't'},
  {"range", required_argument, रिक्त, 'l'},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Given a security context, CONTEXT, derive a context_t (*RET),
   setting any portions selected via the global variables, specified_user,
   specified_role, etc.  */
स्थिर पूर्णांक
compute_context_from_mask (अक्षर स्थिरांक *context, context_t *ret)
{
  bool ok = true;
  context_t new_context = context_new (context);
  जर (!new_context)
    {
      error (0, त्रुटि_सं, _("failed to create security context: %s"),
             quote (context));
      परत 1;
    }

#define SET_COMPONENT(C, comp)						\
   करा									\
     {									\
       जर (specified_ ## comp						\
           && context_ ## comp ## _set ((C), specified_ ## comp))	\
         {								\
            error (0, त्रुटि_सं,						\
                   _("failed to set %s security context component to %s"), \
                   #comp, quote (specified_ ## comp));			\
           ok = false;							\
         }								\
     }									\
   पर्यंत (0)

  SET_COMPONENT (new_context, user);
  SET_COMPONENT (new_context, range);
  SET_COMPONENT (new_context, role);
  SET_COMPONENT (new_context, type);

  जर (!ok)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      context_free (new_context);
      त्रुटि_सं = saved_errno;
      परत 1;
    }

  *ret = new_context;
  परत 0;
}

/* Change the context of FILE, using specified components.
   If it is a directory and -R is given, recurse.
   Return 0 if successful, 1 if errors occurred. */

स्थिर पूर्णांक
change_file_context (पूर्णांक fd, अक्षर स्थिरांक *file, bool ignore_missing)
{
  अक्षर *file_context = रिक्त;
  context_t context IF_LINT (= रिक्त);
  अक्षर स्थिरांक * context_string;
  पूर्णांक errors = 0;

  जर (specified_context == रिक्त)
    {
      पूर्णांक status = (affect_symlink_referent
                    ? getfileconat (fd, file, &file_context)
                    : lgetfileconat (fd, file, &file_context));

      जर (status < 0 && त्रुटि_सं != ENODATA)
        {
          जर (ignore_missing && ignorable_traversal_errno (त्रुटि_सं))
            परत 0;
          error (0, त्रुटि_सं, _("failed to get security context of %s"),
                 quoteaf (file));
          परत 1;
        }

      /* If the file doesn't have a context, and we're not setting all of
         the context components, there isn't really an obvious default.
         Thus, we just give up. */
      जर (file_context == रिक्त)
        {
          error (0, 0, _("can't apply partial context to unlabeled file %s"),
                 quoteaf (file));
          परत 1;
        }

      जर (compute_context_from_mask (file_context, &context))
        {
          freecon (file_context);
          परत 1;
        }

      context_string = context_str (context);
    }
  अन्यथा
    {
      context_string = specified_context;
    }

  जर (file_context == रिक्त || ! streq (context_string, file_context))
    {
      पूर्णांक fail = (affect_symlink_referent
                  ?  setfileconat (fd, file, context_string)
                  : lsetfileconat (fd, file, context_string));

      जर (fail)
        {
          जर (! (ignore_missing && ignorable_traversal_errno (त्रुटि_सं)))
            {
              errors = 1;
              error (0, त्रुटि_सं, _("failed to change context of %s to %s"),
                     quoteaf_n (0, file), quote_n (1, context_string));
            }
        }
    }

  जर (specified_context == रिक्त)
    {
      context_free (context);
      freecon (file_context);
    }

  परत errors;
}

/* Change the context of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

स्थिर bool
process_file (FTS *fts, FTSENT *ent)
{
  अक्षर स्थिरांक *file_full_name = ent->fts_path;
  अक्षर स्थिरांक *file = ent->fts_accpath;
  स्थिरांक संरचना stat *file_stats = ent->fts_statp;
  bool ok = true;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  जर (ignore_missing && ignorable_fts_error (ent))
    परत true;

  निवड (ent->fts_info)
    {
    स्थिती FTS_D:
      जर (recurse)
        {
          जर (मूळ_DEV_INO_CHECK (मूळ_dev_ino, ent->fts_statp))
            {
              /* This happens e.g., with "chcon -R --preserve-root ... /"
                 and with "chcon -RH --preserve-root ... symlink-to-root".  */
              मूळ_DEV_INO_WARN (file_full_name);
              /* Tell fts not to traverse into this hierarchy.  */
              fts_set (fts, ent, FTS_SKIP);
              /* Ensure that we do not process "/" on the second visit.  */
              ignore_मूल्य (fts_read (fts));
              परत false;
            }
          परत true;
        }
      थांबा;

    स्थिती FTS_DP:
      जर (! recurse)
        परत true;
      थांबा;

    स्थिती FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      जर (ent->fts_level == FTS_ROOTLEVEL && ent->fts_संख्या == 0)
        {
          ent->fts_संख्या = 1;
          fts_set (fts, ent, FTS_AGAIN);
          परत true;
        }
      error (0, ent->fts_errno, _("cannot access %s"),
             quoteaf (file_full_name));
      ok = false;
      थांबा;

    स्थिती FTS_ERR:
      error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ok = false;
      थांबा;

    स्थिती FTS_DNR:
      error (0, ent->fts_errno, _("cannot read directory %s"),
             quoteaf (file_full_name));
      ok = false;
      थांबा;

    स्थिती FTS_DC:		/* directory that causes cycles */
      जर (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          परत false;
        }
      थांबा;

    पूर्वनिर्धारित:
      थांबा;
    }

  जर (ent->fts_info == FTS_DP
      && ok && मूळ_DEV_INO_CHECK (मूळ_dev_ino, file_stats))
    {
      मूळ_DEV_INO_WARN (file_full_name);
      ok = false;
    }

  जर (ok)
    {
      जर (verbose)
        म_लिखो (_("changing security context of %s\n"),
                quoteaf (file_full_name));

      जर (change_file_context (fts->fts_cwd_fd, file, ignore_missing) != 0)
        ok = false;
    }

  जर ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  परत ok;
}

/* Recursively operate on the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

स्थिर bool
process_files (अक्षर **files, पूर्णांक bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, रिक्त);

  पर्यंत (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      जर (ent == रिक्त)
        {
          जर (त्रुटि_सं != 0)
            {
              /* FIXME: try to give a better message  */
              error (0, त्रुटि_सं, _("fts_read failed"));
              ok = false;
            }
          थांबा;
        }

      ok &= process_file (fts, ent);
    }

  जर (fts_close (fts) != 0)
    {
      error (0, त्रुटि_सं, _("fts_close failed"));
      ok = false;
    }

  परत ok;
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
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
         the पूर्वनिर्धारित), rather than the symbolic link itself\n\
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
      --no-preserve-मूळ\n\
         करा not treat '/' specially (the पूर्वनिर्धारित)\n\
"));
      oputs (_("\
      --preserve-मूळ\n\
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
         output a diagnostic साठी every file processed\n\
"));
      emit_symlink_recurse_options ("-P");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  /* Bit flags that control how fts works.  */
  पूर्णांक bit_flags = FTS_PHYSICAL;

  /* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
     specified.  */
  पूर्णांक dereference = -1;

  bool ok;
  bool preserve_मूळ = false;
  bool component_specified = false;
  अक्षर *reference_file = रिक्त;
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "HLPRhvu:r:t:l:",
                              long_options, रिक्त))
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

        स्थिती 'R':
          recurse = true;
          थांबा;

        स्थिती 'f':
          /* ignore */
          थांबा;

        स्थिती 'v':
          verbose = true;
          थांबा;

        स्थिती 'u':
          specified_user = optarg;
          component_specified = true;
          थांबा;

        स्थिती 'r':
          specified_role = optarg;
          component_specified = true;
          थांबा;

        स्थिती 't':
          specified_type = optarg;
          component_specified = true;
          थांबा;

        स्थिती 'l':
          specified_range = optarg;
          component_specified = true;
          थांबा;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (recurse)
    {
      जर (bit_flags == FTS_PHYSICAL)
        {
          जर (dereference == 1)
            error (निकास_त्रुटि, 0,
                   _("-R --dereference requires either -H or -L"));
          affect_symlink_referent = false;
        }
      अन्यथा
        {
          जर (dereference == 0)
            error (निकास_त्रुटि, 0, _("-R -h requires -P"));
          affect_symlink_referent = true;
        }
    }
  अन्यथा
    {
      bit_flags = FTS_PHYSICAL;
      affect_symlink_referent = (dereference != 0);
    }

  जर (argc - optind < (reference_file || component_specified ? 1 : 2))
    {
      जर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  जर (reference_file)
    {
      अक्षर *ref_context = रिक्त;

      जर (getfilecon (reference_file, &ref_context) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get security context of %s"),
               quoteaf (reference_file));

      specified_context = ref_context;
    }
  अन्यथा जर (component_specified)
    {
      /* FIXME: it's already null, so this is a no-op. */
      specified_context = रिक्त;
    }
  अन्यथा
    {
      specified_context = argv[optind++];
      जर (0 < is_selinux_enabled ()
          && security_check_context (specified_context) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("invalid context: %s"),
               quote (specified_context));
    }

  जर (reference_file && component_specified)
    {
      error (0, 0, _("conflicting security context specifiers given"));
      usage (निकास_त्रुटि);
    }

  जर (recurse && preserve_मूळ)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      मूळ_dev_ino = get_मूळ_dev_ino (&dev_ino_buf);
      जर (मूळ_dev_ino == रिक्त)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  अन्यथा
    {
      मूळ_dev_ino = रिक्त;
    }

  ok = process_files (argv + optind, bit_flags | FTS_NOSTAT);

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
