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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "dev-ino.h"
#அந்தர்பாவ "filemode.h"
#அந்தர்பாவ "fts-missing.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "modechange.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "root-dev-ino.h"
#அந்தர்பாவ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

கட்டமைப்பு change_status
{
  எண்ணிக்கை
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

எண்ணிக்கை Verbosity
{
  /* Print a message for each file that is processed.  */
  V_உயர்வு,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
நிலையான கட்டமைப்பு mode_change *change;

/* The initial umask value, if it might be needed.  */
நிலையான mode_t umask_மதிப்பு;

/* If true, change the modes of directories recursively. */
நிலையான bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
நிலையான முழு dereference = -1;

/* If true, force silence (suppress most of error messages). */
நிலையான bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
நிலையான bool diagnose_surprises;

/* Level of verbosity.  */
நிலையான எண்ணிக்கை Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
நிலையான கட்டமைப்பு dev_ino *வேர்_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_வேர்,
  PRESERVE_வேர்,
  REFERENCE_FILE_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"changes", no_argument, வெற்று, 'c'},
  {"dereference", no_argument, வெற்று, DEREFERENCE_OPTION},
  {"recursive", no_argument, வெற்று, 'R'},
  {"no-dereference", no_argument, வெற்று, 'h'},
  {"no-preserve-root", no_argument, வெற்று, NO_PRESERVE_வேர்},
  {"preserve-root", no_argument, வெற்று, PRESERVE_வேர்},
  {"quiet", no_argument, வெற்று, 'f'},
  {"reference", required_argument, வெற்று, REFERENCE_FILE_OPTION},
  {"silent", no_argument, வெற்று, 'f'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

நிலையான bool
mode_changed (முழு dir_fd, எழுத்து மாறிலி *file, எழுத்து மாறிலி *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  எனில் (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      கட்டமைப்பு stat new_stats;

      எனில் (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          எனில் (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          திரும்பு false;
        }

      new_mode = new_stats.st_mode;
    }

  திரும்பு ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

நிலையான வெறுமை
describe_change (எழுத்து மாறிலி *file, கட்டமைப்பு change_status மாறிலி *ch)
{
  எழுத்து perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  எழுத்து old_perms[12];
  எழுத்து மாறிலி *fmt;
  எழுத்து மாறிலி *quoted_file = quoteaf (file);

  தேர்வு (ch->status)
    {
    நிலை CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      திரும்பு;

    நிலை CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      திரும்பு;

    நிலை CH_FAILED: நிலை CH_NO_CHANGE_REQUESTED: நிலை CH_SUCCEEDED:
      நிறுத்து;
  }

  unsigned long முழு
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  தேர்வு (ch->status)
    {
    நிலை CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      நிறுத்து;
    நிலை CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      நிறுத்து;
    நிலை CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      திரும்பு;
    நிலை CH_NO_STAT: நிலை CH_NOT_APPLIED: இயல்பு:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

நிலையான bool
process_file (FTS *fts, FTSENT *ent)
{
  எழுத்து மாறிலி *file_full_name = ent->fts_path;
  எழுத்து மாறிலி *file = ent->fts_accpath;
  மாறிலி கட்டமைப்பு stat *file_stats = ent->fts_statp;
  கட்டமைப்பு change_status ch = {0};
  ch.status = CH_NO_STAT;
  கட்டமைப்பு stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  எனில் (ignore_missing && ignorable_fts_error (ent))
    திரும்பு true;

  தேர்வு (ent->fts_info)
    {
    நிலை FTS_DP:
      திரும்பு true;

    நிலை FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      எனில் (ent->fts_level == FTS_ROOTLEVEL && ent->fts_எண் == 0)
        {
          ent->fts_எண் = 1;
          fts_set (fts, ent, FTS_AGAIN);
          திரும்பு true;
        }
      எனில் (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      நிறுத்து;

    நிலை FTS_ERR:
      எனில் (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      நிறுத்து;

    நிலை FTS_DNR:
      எனில் (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      நிறுத்து;

    நிலை FTS_SLNONE:
      எனில் (dereference)
        {
          எனில் (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          நிறுத்து;
        }
      ch.status = CH_NOT_APPLIED;
      நிறுத்து;

    நிலை FTS_SL:
      எனில் (dereference == 1)
        {
          எனில் (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              எனில் (ignore_missing
                  && ignorable_traversal_errno (errno))
                திரும்பு true;
              எனில் (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              நிறுத்து;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      நிறுத்து;

    நிலை FTS_DC:		/* directory that causes cycles */
      எனில் (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          திரும்பு false;
        }
      FALLTHROUGH;
    இயல்பு:
      ch.status = CH_NOT_APPLIED;
      நிறுத்து;
    }

  எனில் (ch.status == CH_NOT_APPLIED
      && வேர்_DEV_INO_CHECK (வேர்_dev_ino, file_stats))
    {
      வேர்_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_மதிப்பு (fts_read (fts));
      திரும்பு false;
    }

  எனில் (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_மதிப்பு, change, வெற்று);
      bool follow_symlink = !!dereference;
      எனில் (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      எனில் (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      இல்லையெனில்
        {
          எனில் (ignore_missing
              && ignorable_traversal_errno (errno))
            திரும்பு true;
          எனில் (! is_ENOTSUP (errno))
            {
              எனில் (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  எனில் (verbosity != V_off)
    {
      bool file_missing;
      எனில் (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          எனில் (file_missing)
            திரும்பு true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      எனில் (ch.status == CH_SUCCEEDED || verbosity == V_உயர்வு)
        describe_change (file_full_name, &ch);
    }

  எனில் (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, வெற்று);
      எனில் (ch.new_mode & ~naively_expected_mode)
        {
          எழுத்து new_perms[12];
          எழுத்து naively_expected_perms[12];
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

  எனில் ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  திரும்பு CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

நிலையான bool
process_files (எழுத்து **files, முழு bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, வெற்று);

  போது (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      எனில் (ent == வெற்று)
        {
          எனில் (errno != 0)
            {
              /* FIXME: try to give a better message  */
              எனில் (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          நிறுத்து;
        }

      ok &= process_file (fts, ent);
    }

  எனில் (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  திரும்பு ok;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... MODE[,MODE]... FILE...\n\
  or:  %s [OPTION]... OCTAL-MODE FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Change the mode of each FILE to MODE.\n\
With --reference, change the mode of each FILE to that of RFILE.\n\
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
         output a diagnostic க்கு every file processed\n\
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
      --no-preserve-வேர்\n\
         செய் not treat '/' specially (the இயல்பு)\n\
"));
      oputs (_("\
      --preserve-வேர்\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE மதிப்புகள்.\n\
         RFILE is always dereferenced எனில் a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each MODE is of the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து *mode = வெற்று;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_வேர் = false;
  எழுத்து மாறிலி *reference_file = வெற்று;
  முழு c;
  முழு bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, வெற்று))
         != -1)
    {
      தேர்வு (c)
        {

        நிலை 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          நிறுத்து;

        நிலை 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          நிறுத்து;

        நிலை DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          நிறுத்து;

        நிலை 'r':
        நிலை 'w':
        நிலை 'x':
        நிலை 'X':
        நிலை 's':
        நிலை 't':
        நிலை 'u':
        நிலை 'g':
        நிலை 'o':
        நிலை 'a':
        நிலை ',':
        நிலை '+':
        நிலை '=':
        நிலை '0': நிலை '1': நிலை '2': நிலை '3':
        நிலை '4': நிலை '5': நிலை '6': நிலை '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            எழுத்து மாறிலி *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            எனில் (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          நிறுத்து;
        நிலை NO_PRESERVE_வேர்:
          preserve_வேர் = false;
          நிறுத்து;
        நிலை PRESERVE_வேர்:
          preserve_வேர் = true;
          நிறுத்து;
        நிலை REFERENCE_FILE_OPTION:
          reference_file = optarg;
          நிறுத்து;
        நிலை 'R':
          recurse = true;
          நிறுத்து;
        நிலை 'c':
          verbosity = V_changes_only;
          நிறுத்து;
        நிலை 'f':
          force_silent = true;
          நிறுத்து;
        நிலை 'v':
          verbosity = V_உயர்வு;
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (recurse)
    {
      எனில் (bit_flags == FTS_PHYSICAL)
        {
          எனில் (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  எனில் (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  எனில் (reference_file)
    {
      எனில் (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  இல்லையெனில்
    {
      எனில் (!mode)
        mode = argv[optind++];
    }

  எனில் (optind >= argc)
    {
      எனில் (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  எனில் (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      எனில் (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  இல்லையெனில்
    {
      change = mode_compile (mode);
      எனில் (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_மதிப்பு = umask (0);
    }

  எனில் (recurse && preserve_வேர்)
    {
      நிலையான கட்டமைப்பு dev_ino dev_ino_buf;
      வேர்_dev_ino = get_வேர்_dev_ino (&dev_ino_buf);
      எனில் (வேர்_dev_ino == வெற்று)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  இல்லையெனில்
    {
      வேர்_dev_ino = வெற்று;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
