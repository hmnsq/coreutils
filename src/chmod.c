/* chmod -- change permission modes of files
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

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "dev-ino.h"
#अन्तर्भाव "filemode.h"
#अन्तर्भाव "fts-missing.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "modechange.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

संरचना change_status
{
  गणना
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    status;
  mode_t old_mode;
  mode_t new_mode;
};

गणना Verbosity
{
  /* Print a message for each file that is processed.  */
  V_वरचा,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
स्थिर संरचना mode_change *change;

/* The initial umask value, if it might be needed.  */
स्थिर mode_t umask_मूल्य;

/* If true, change the modes of directories recursively. */
स्थिर bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
स्थिर पूर्णांक dereference = -1;

/* If true, force silence (suppress most of error messages). */
स्थिर bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
स्थिर bool diagnose_surprises;

/* Level of verbosity.  */
स्थिर गणना Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
स्थिर संरचना dev_ino *मूळ_dev_ino;

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
  {"changes", no_argument, रिक्त, 'c'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"recursive", no_argument, रिक्त, 'R'},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूळ},
  {"preserve-root", no_argument, रिक्त, PRESERVE_मूळ},
  {"quiet", no_argument, रिक्त, 'f'},
  {"reference", required_argument, रिक्त, REFERENCE_FILE_OPTION},
  {"silent", no_argument, रिक्त, 'f'},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

स्थिर bool
mode_changed (पूर्णांक dir_fd, अक्षर स्थिरांक *file, अक्षर स्थिरांक *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  जर (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      संरचना stat new_stats;

      जर (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (त्रुटि_सं));
          जर (! *file_missing && ! force_silent)
            error (0, त्रुटि_सं, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          परत false;
        }

      new_mode = new_stats.st_mode;
    }

  परत ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

स्थिर शून्य
describe_change (अक्षर स्थिरांक *file, संरचना change_status स्थिरांक *ch)
{
  अक्षर perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  अक्षर old_perms[12];
  अक्षर स्थिरांक *fmt;
  अक्षर स्थिरांक *quoted_file = quoteaf (file);

  निवड (ch->status)
    {
    स्थिती CH_NOT_APPLIED:
      म_लिखो (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      परत;

    स्थिती CH_NO_STAT:
      म_लिखो (_("%s could not be accessed\n"), quoted_file);
      परत;

    स्थिती CH_FAILED: स्थिती CH_NO_CHANGE_REQUESTED: स्थिती CH_SUCCEEDED:
      थांबा;
  }

  अचिन्हित दीर्घ पूर्णांक
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  निवड (ch->status)
    {
    स्थिती CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      थांबा;
    स्थिती CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      थांबा;
    स्थिती CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      म_लिखो (fmt, quoted_file, m, &perms[1]);
      परत;
    स्थिती CH_NO_STAT: स्थिती CH_NOT_APPLIED: पूर्वनिर्धारित:
      affirm (false);
    }
  म_लिखो (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

स्थिर bool
process_file (FTS *fts, FTSENT *ent)
{
  अक्षर स्थिरांक *file_full_name = ent->fts_path;
  अक्षर स्थिरांक *file = ent->fts_accpath;
  स्थिरांक संरचना stat *file_stats = ent->fts_statp;
  संरचना change_status ch = {0};
  ch.status = CH_NO_STAT;
  संरचना stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  जर (ignore_missing && ignorable_fts_error (ent))
    परत true;

  निवड (ent->fts_info)
    {
    स्थिती FTS_DP:
      परत true;

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
      जर (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      थांबा;

    स्थिती FTS_ERR:
      जर (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      थांबा;

    स्थिती FTS_DNR:
      जर (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      थांबा;

    स्थिती FTS_SLNONE:
      जर (dereference)
        {
          जर (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          थांबा;
        }
      ch.status = CH_NOT_APPLIED;
      थांबा;

    स्थिती FTS_SL:
      जर (dereference == 1)
        {
          जर (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              जर (ignore_missing
                  && ignorable_traversal_errno (त्रुटि_सं))
                परत true;
              जर (! force_silent)
                error (0, त्रुटि_सं, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              थांबा;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      थांबा;

    स्थिती FTS_DC:		/* directory that causes cycles */
      जर (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          परत false;
        }
      FALLTHROUGH;
    पूर्वनिर्धारित:
      ch.status = CH_NOT_APPLIED;
      थांबा;
    }

  जर (ch.status == CH_NOT_APPLIED
      && मूळ_DEV_INO_CHECK (मूळ_dev_ino, file_stats))
    {
      मूळ_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_मूल्य (fts_read (fts));
      परत false;
    }

  जर (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_मूल्य, change, रिक्त);
      bool follow_symlink = !!dereference;
      जर (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      जर (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      अन्यथा
        {
          जर (ignore_missing
              && ignorable_traversal_errno (त्रुटि_सं))
            परत true;
          जर (! is_ENOTSUP (त्रुटि_सं))
            {
              जर (! force_silent)
                error (0, त्रुटि_सं, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  जर (verbosity != V_off)
    {
      bool file_missing;
      जर (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          जर (file_missing)
            परत true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      जर (ch.status == CH_SUCCEEDED || verbosity == V_वरचा)
        describe_change (file_full_name, &ch);
    }

  जर (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, रिक्त);
      जर (ch.new_mode & ~naively_expected_mode)
        {
          अक्षर new_perms[12];
          अक्षर naively_expected_perms[12];
          strmode (ch.new_mode, new_perms);
          strmode (naively_expected_mode, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_full_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.status = CH_FAILED;
        }
    }

  जर ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  परत CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
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
              जर (! force_silent)
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
Usage: %s [OPTION]... MODE[,MODE]... खाता...\n\
  or:  %s [OPTION]... OCTAL-MODE खाता...\n\
  or:  %s [OPTION]... --reference=RFILE खाता...\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\
Change the mode of each खाता to MODE.\n\
With --reference, change the mode of each खाता to that of RFILE.\n\
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
         affect the referent of each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
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
         use RFILE's mode instead of specifying MODE मूल्ये.\n\
         RFILE is always dereferenced जर a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Each MODE is of the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *mode = रिक्त;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_मूळ = false;
  अक्षर स्थिरांक *reference_file = रिक्त;
  पूर्णांक c;
  पूर्णांक bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, रिक्त))
         != -1)
    {
      निवड (c)
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

        स्थिती 'r':
        स्थिती 'w':
        स्थिती 'x':
        स्थिती 'X':
        स्थिती 's':
        स्थिती 't':
        स्थिती 'u':
        स्थिती 'g':
        स्थिती 'o':
        स्थिती 'a':
        स्थिती ',':
        स्थिती '+':
        स्थिती '=':
        स्थिती '0': स्थिती '1': स्थिती '2': स्थिती '3':
        स्थिती '4': स्थिती '5': स्थिती '6': स्थिती '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            अक्षर स्थिरांक *arg = argv[optind - 1];
            idx_t arg_len = म_माप (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            जर (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            स_नकल (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
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
        स्थिती 'c':
          verbosity = V_changes_only;
          थांबा;
        स्थिती 'f':
          force_silent = true;
          थांबा;
        स्थिती 'v':
          verbosity = V_वरचा;
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
          dereference = 0;
        }
    }

  जर (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  जर (reference_file)
    {
      जर (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (निकास_त्रुटि);
        }
    }
  अन्यथा
    {
      जर (!mode)
        mode = argv[optind++];
    }

  जर (optind >= argc)
    {
      जर (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  जर (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      जर (!change)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  अन्यथा
    {
      change = mode_compile (mode);
      जर (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (निकास_त्रुटि);
        }
      umask_मूल्य = umask (0);
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

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
