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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "assure.h"
#समावेश "dev-ino.h"
#समावेश "filemode.h"
#समावेश "fts-missing.h"
#समावेश "ignore-value.h"
#समावेश "modechange.h"
#समावेश "quote.h"
#समावेश "root-dev-ino.h"
#समावेश "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "chmod"

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

काष्ठा change_status
{
  क्रमागत
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

क्रमागत Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ऊपरी,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
अटल काष्ठा mode_change *change;

/* The initial umask value, if it might be needed.  */
अटल mode_t umask_मूल्य;

/* If true, change the modes of directories recursively. */
अटल bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
अटल पूर्णांक dereference = -1;

/* If true, force silence (suppress most of error messages). */
अटल bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
अटल bool diagnose_surprises;

/* Level of verbosity.  */
अटल क्रमागत Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
अटल काष्ठा dev_ino *मूल_dev_ino;

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
  {"changes", no_argument, शून्य, 'c'},
  {"dereference", no_argument, शून्य, DEREFERENCE_OPTION},
  {"recursive", no_argument, शून्य, 'R'},
  {"no-dereference", no_argument, शून्य, 'h'},
  {"no-preserve-root", no_argument, शून्य, NO_PRESERVE_मूल},
  {"preserve-root", no_argument, शून्य, PRESERVE_मूल},
  {"quiet", no_argument, शून्य, 'f'},
  {"reference", required_argument, शून्य, REFERENCE_FILE_OPTION},
  {"silent", no_argument, शून्य, 'f'},
  {"verbose", no_argument, शून्य, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

अटल bool
mode_changed (पूर्णांक dir_fd, अक्षर स्थिर *file, अक्षर स्थिर *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  अगर (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      काष्ठा stat new_stats;

      अगर (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (त्रुटि_सं));
          अगर (! *file_missing && ! force_silent)
            error (0, त्रुटि_सं, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          वापस false;
        }

      new_mode = new_stats.st_mode;
    }

  वापस ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

अटल व्योम
describe_change (अक्षर स्थिर *file, काष्ठा change_status स्थिर *ch)
{
  अक्षर perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  अक्षर old_perms[12];
  अक्षर स्थिर *fmt;
  अक्षर स्थिर *quoted_file = quoteaf (file);

  चयन (ch->status)
    {
    हाल CH_NOT_APPLIED:
      म_लिखो (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      वापस;

    हाल CH_NO_STAT:
      म_लिखो (_("%s could not be accessed\n"), quoted_file);
      वापस;

    हाल CH_FAILED: हाल CH_NO_CHANGE_REQUESTED: हाल CH_SUCCEEDED:
      अवरोध;
  }

  अचिन्हित दीर्घ पूर्णांक
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  चयन (ch->status)
    {
    हाल CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      अवरोध;
    हाल CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      अवरोध;
    हाल CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      म_लिखो (fmt, quoted_file, m, &perms[1]);
      वापस;
    हाल CH_NO_STAT: हाल CH_NOT_APPLIED: शेष:
      affirm (false);
    }
  म_लिखो (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

अटल bool
process_file (FTS *fts, FTSENT *ent)
{
  अक्षर स्थिर *file_full_name = ent->fts_path;
  अक्षर स्थिर *file = ent->fts_accpath;
  स्थिर काष्ठा stat *file_stats = ent->fts_statp;
  काष्ठा change_status ch = {0};
  ch.status = CH_NO_STAT;
  काष्ठा stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  अगर (ignore_missing && ignorable_fts_error (ent))
    वापस true;

  चयन (ent->fts_info)
    {
    हाल FTS_DP:
      वापस true;

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
      अगर (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      अवरोध;

    हाल FTS_ERR:
      अगर (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      अवरोध;

    हाल FTS_DNR:
      अगर (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      अवरोध;

    हाल FTS_SLNONE:
      अगर (dereference)
        {
          अगर (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          अवरोध;
        }
      ch.status = CH_NOT_APPLIED;
      अवरोध;

    हाल FTS_SL:
      अगर (dereference == 1)
        {
          अगर (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              अगर (ignore_missing
                  && ignorable_traversal_errno (त्रुटि_सं))
                वापस true;
              अगर (! force_silent)
                error (0, त्रुटि_सं, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              अवरोध;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      अवरोध;

    हाल FTS_DC:		/* directory that causes cycles */
      अगर (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          वापस false;
        }
      FALLTHROUGH;
    शेष:
      ch.status = CH_NOT_APPLIED;
      अवरोध;
    }

  अगर (ch.status == CH_NOT_APPLIED
      && मूल_DEV_INO_CHECK (मूल_dev_ino, file_stats))
    {
      मूल_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_मूल्य (fts_read (fts));
      वापस false;
    }

  अगर (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_मूल्य, change, शून्य);
      bool follow_symlink = !!dereference;
      अगर (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      अगर (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      अन्यथा
        {
          अगर (ignore_missing
              && ignorable_traversal_errno (त्रुटि_सं))
            वापस true;
          अगर (! is_ENOTSUP (त्रुटि_सं))
            {
              अगर (! force_silent)
                error (0, त्रुटि_सं, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  अगर (verbosity != V_off)
    {
      bool file_missing;
      अगर (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          अगर (file_missing)
            वापस true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      अगर (ch.status == CH_SUCCEEDED || verbosity == V_ऊपरी)
        describe_change (file_full_name, &ch);
    }

  अगर (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, शून्य);
      अगर (ch.new_mode & ~naively_expected_mode)
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

  अगर ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  वापस CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
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
              अगर (! force_silent)
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
         output a diagnostic क्रम every file processed\n\
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
      --no-preserve-मूल\n\
         करो not treat '/' specially (the शेष)\n\
"));
      oputs (_("\
      --preserve-मूल\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE मूल्यावली.\n\
         RFILE is always dereferenced अगर a symbolic link.\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *mode = शून्य;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_मूल = false;
  अक्षर स्थिर *reference_file = शून्य;
  पूर्णांक c;
  पूर्णांक bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, शून्य))
         != -1)
    {
      चयन (c)
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

        हाल 'r':
        हाल 'w':
        हाल 'x':
        हाल 'X':
        हाल 's':
        हाल 't':
        हाल 'u':
        हाल 'g':
        हाल 'o':
        हाल 'a':
        हाल ',':
        हाल '+':
        हाल '=':
        हाल '0': हाल '1': हाल '2': हाल '3':
        हाल '4': हाल '5': हाल '6': हाल '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            अक्षर स्थिर *arg = argv[optind - 1];
            idx_t arg_len = म_माप (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            अगर (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            स_नकल (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
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
        हाल 'c':
          verbosity = V_changes_only;
          अवरोध;
        हाल 'f':
          force_silent = true;
          अवरोध;
        हाल 'v':
          verbosity = V_ऊपरी;
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
          dereference = 0;
        }
    }

  अगर (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  अगर (reference_file)
    {
      अगर (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (निकास_त्रुटि);
        }
    }
  अन्यथा
    {
      अगर (!mode)
        mode = argv[optind++];
    }

  अगर (optind >= argc)
    {
      अगर (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  अगर (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      अगर (!change)
        error (निकास_त्रुटि, त्रुटि_सं, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  अन्यथा
    {
      change = mode_compile (mode);
      अगर (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (निकास_त्रुटि);
        }
      umask_मूल्य = umask (0);
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

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
