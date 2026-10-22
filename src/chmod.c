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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "dev-ino.h"
#અન્તર્ભાવ "filemode.h"
#અન્તર્ભાવ "fts-missing.h"
#અન્તર્ભાવ "ignore-value.h"
#અન્તર્ભાવ "modechange.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

સંરચના change_status
{
  ગણના
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

ગણના Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ઉપલું,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
સ્થિર સંરચના mode_change *change;

/* The initial umask value, if it might be needed.  */
સ્થિર mode_t umask_કિંમત;

/* If true, change the modes of directories recursively. */
સ્થિર bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
સ્થિર પૂર્ણાંક dereference = -1;

/* If true, force silence (suppress most of error messages). */
સ્થિર bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
સ્થિર bool diagnose_surprises;

/* Level of verbosity.  */
સ્થિર ગણના Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
સ્થિર સંરચના dev_ino *મૂળ_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_મૂળ,
  PRESERVE_મૂળ,
  REFERENCE_FILE_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"changes", no_argument, ખાલી, 'c'},
  {"dereference", no_argument, ખાલી, DEREFERENCE_OPTION},
  {"recursive", no_argument, ખાલી, 'R'},
  {"no-dereference", no_argument, ખાલી, 'h'},
  {"no-preserve-root", no_argument, ખાલી, NO_PRESERVE_મૂળ},
  {"preserve-root", no_argument, ખાલી, PRESERVE_મૂળ},
  {"quiet", no_argument, ખાલી, 'f'},
  {"reference", required_argument, ખાલી, REFERENCE_FILE_OPTION},
  {"silent", no_argument, ખાલી, 'f'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

સ્થિર bool
mode_changed (પૂર્ણાંક dir_fd, અક્ષર અચલ *file, અક્ષર અચલ *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  જો (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      સંરચના stat new_stats;

      જો (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          જો (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          પાછા false;
        }

      new_mode = new_stats.st_mode;
    }

  પાછા ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

સ્થિર શૂન્ય
describe_change (અક્ષર અચલ *file, સંરચના change_status અચલ *ch)
{
  અક્ષર perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  અક્ષર old_perms[12];
  અક્ષર અચલ *fmt;
  અક્ષર અચલ *quoted_file = quoteaf (file);

  પસંદગી (ch->status)
    {
    સ્થિતિ CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      પાછા;

    સ્થિતિ CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      પાછા;

    સ્થિતિ CH_FAILED: સ્થિતિ CH_NO_CHANGE_REQUESTED: સ્થિતિ CH_SUCCEEDED:
      તોડો;
  }

  unsigned long પૂર્ણાંક
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  પસંદગી (ch->status)
    {
    સ્થિતિ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      તોડો;
    સ્થિતિ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      તોડો;
    સ્થિતિ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      પાછા;
    સ્થિતિ CH_NO_STAT: સ્થિતિ CH_NOT_APPLIED: મૂળભૂત:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

સ્થિર bool
process_file (FTS *fts, FTSENT *ent)
{
  અક્ષર અચલ *file_full_name = ent->fts_path;
  અક્ષર અચલ *file = ent->fts_accpath;
  અચલ સંરચના stat *file_stats = ent->fts_statp;
  સંરચના change_status ch = {0};
  ch.status = CH_NO_STAT;
  સંરચના stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  જો (ignore_missing && ignorable_fts_error (ent))
    પાછા true;

  પસંદગી (ent->fts_info)
    {
    સ્થિતિ FTS_DP:
      પાછા true;

    સ્થિતિ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      જો (ent->fts_level == FTS_ROOTLEVEL && ent->fts_સંખ્યા == 0)
        {
          ent->fts_સંખ્યા = 1;
          fts_set (fts, ent, FTS_AGAIN);
          પાછા true;
        }
      જો (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      તોડો;

    સ્થિતિ FTS_ERR:
      જો (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      તોડો;

    સ્થિતિ FTS_DNR:
      જો (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      તોડો;

    સ્થિતિ FTS_SLNONE:
      જો (dereference)
        {
          જો (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          તોડો;
        }
      ch.status = CH_NOT_APPLIED;
      તોડો;

    સ્થિતિ FTS_SL:
      જો (dereference == 1)
        {
          જો (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              જો (ignore_missing
                  && ignorable_traversal_errno (errno))
                પાછા true;
              જો (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              તોડો;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      તોડો;

    સ્થિતિ FTS_DC:		/* directory that causes cycles */
      જો (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          પાછા false;
        }
      FALLTHROUGH;
    મૂળભૂત:
      ch.status = CH_NOT_APPLIED;
      તોડો;
    }

  જો (ch.status == CH_NOT_APPLIED
      && મૂળ_DEV_INO_CHECK (મૂળ_dev_ino, file_stats))
    {
      મૂળ_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_કિંમત (fts_read (fts));
      પાછા false;
    }

  જો (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_કિંમત, change, ખાલી);
      bool follow_symlink = !!dereference;
      જો (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      જો (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      નહીંતર
        {
          જો (ignore_missing
              && ignorable_traversal_errno (errno))
            પાછા true;
          જો (! is_ENOTSUP (errno))
            {
              જો (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  જો (verbosity != V_off)
    {
      bool file_missing;
      જો (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          જો (file_missing)
            પાછા true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      જો (ch.status == CH_SUCCEEDED || verbosity == V_ઉપલું)
        describe_change (file_full_name, &ch);
    }

  જો (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, ખાલી);
      જો (ch.new_mode & ~naively_expected_mode)
        {
          અક્ષર new_perms[12];
          અક્ષર naively_expected_perms[12];
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

  જો ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  પાછા CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

સ્થિર bool
process_files (અક્ષર **files, પૂર્ણાંક bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ખાલી);

  જ્યારે (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      જો (ent == ખાલી)
        {
          જો (errno != 0)
            {
              /* FIXME: try to give a better message  */
              જો (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          તોડો;
        }

      ok &= process_file (fts, ent);
    }

  જો (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  પાછા ok;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         output a diagnostic માટે every file processed\n\
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
      --no-preserve-મૂળ\n\
         કરો not treat '/' specially (the મૂળભૂત)\n\
"));
      oputs (_("\
      --preserve-મૂળ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE કિંમતો.\n\
         RFILE is always dereferenced જો a symbolic link.\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર *mode = ખાલી;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_મૂળ = false;
  અક્ષર અચલ *reference_file = ખાલી;
  પૂર્ણાંક c;
  પૂર્ણાંક bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ખાલી))
         != -1)
    {
      પસંદગી (c)
        {

        સ્થિતિ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          તોડો;

        સ્થિતિ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          તોડો;

        સ્થિતિ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          તોડો;

        સ્થિતિ 'r':
        સ્થિતિ 'w':
        સ્થિતિ 'x':
        સ્થિતિ 'X':
        સ્થિતિ 's':
        સ્થિતિ 't':
        સ્થિતિ 'u':
        સ્થિતિ 'g':
        સ્થિતિ 'o':
        સ્થિતિ 'a':
        સ્થિતિ ',':
        સ્થિતિ '+':
        સ્થિતિ '=':
        સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3':
        સ્થિતિ '4': સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            અક્ષર અચલ *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            જો (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          તોડો;
        સ્થિતિ NO_PRESERVE_મૂળ:
          preserve_મૂળ = false;
          તોડો;
        સ્થિતિ PRESERVE_મૂળ:
          preserve_મૂળ = true;
          તોડો;
        સ્થિતિ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          તોડો;
        સ્થિતિ 'R':
          recurse = true;
          તોડો;
        સ્થિતિ 'c':
          verbosity = V_changes_only;
          તોડો;
        સ્થિતિ 'f':
          force_silent = true;
          તોડો;
        સ્થિતિ 'v':
          verbosity = V_ઉપલું;
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (recurse)
    {
      જો (bit_flags == FTS_PHYSICAL)
        {
          જો (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  જો (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  જો (reference_file)
    {
      જો (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  નહીંતર
    {
      જો (!mode)
        mode = argv[optind++];
    }

  જો (optind >= argc)
    {
      જો (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  જો (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      જો (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  નહીંતર
    {
      change = mode_compile (mode);
      જો (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_કિંમત = umask (0);
    }

  જો (recurse && preserve_મૂળ)
    {
      સ્થિર સંરચના dev_ino dev_ino_buf;
      મૂળ_dev_ino = get_મૂળ_dev_ino (&dev_ino_buf);
      જો (મૂળ_dev_ino == ખાલી)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  નહીંતર
    {
      મૂળ_dev_ino = ખાલી;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
