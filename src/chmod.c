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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "system.h"
#include "assure.h"
#include "dev-ino.h"
#include "filemode.h"
#include "fts-missing.h"
#include "ignore-value.h"
#include "modechange.h"
#include "quote.h"
#include "root-dev-ino.h"
#include "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

sa_nracanaa change_status
{
  ga_nanaa
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

ga_nanaa Verbosity
{
  /* Print a message for each file that is processed.  */
  V_high,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
static sa_nracanaa mode_change *change;

/* The initial umask value, if it might be needed.  */
static mode_t umask_value;

/* If true, change the modes of directories recursively. */
static bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
static จำนวนเต็ม dereference = -1;

/* If true, force silence (suppress most of error messages). */
static bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
static bool diagnose_surprises;

/* Level of verbosity.  */
static ga_nanaa Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
static sa_nracanaa dev_ino *root_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ROOT,
  PRESERVE_ROOT,
  REFERENCE_FILE_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"changes", no_argument, NULL, 'c'},
  {"dereference", no_argument, NULL, DEREFERENCE_OPTION},
  {"recursive", no_argument, NULL, 'R'},
  {"no-dereference", no_argument, NULL, 'h'},
  {"no-preserve-root", no_argument, NULL, NO_PRESERVE_ROOT},
  {"preserve-root", no_argument, NULL, PRESERVE_ROOT},
  {"quiet", no_argument, NULL, 'f'},
  {"reference", required_argument, NULL, REFERENCE_FILE_OPTION},
  {"silent", no_argument, NULL, 'f'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

static bool
mode_changed (จำนวนเต็ม dir_fd, var_na const *file, var_na const *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ถ้า (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      sa_nracanaa stat new_stats;

      ถ้า (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ถ้า (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          คืน false;
        }

      new_mode = new_stats.st_mode;
    }

  คืน ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

static void
describe_change (var_na const *file, sa_nracanaa change_status const *ch)
{
  var_na perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  var_na old_perms[12];
  var_na const *fmt;
  var_na const *quoted_file = quoteaf (file);

  switch (ch->status)
    {
    pak_sa CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      คืน;

    pak_sa CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      คืน;

    pak_sa CH_FAILED: pak_sa CH_NO_CHANGE_REQUESTED: pak_sa CH_SUCCEEDED:
      bha_nga;
  }

  unsigned long จำนวนเต็ม
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  switch (ch->status)
    {
    pak_sa CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      bha_nga;
    pak_sa CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      bha_nga;
    pak_sa CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      คืน;
    pak_sa CH_NO_STAT: pak_sa CH_NOT_APPLIED: default:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

static bool
process_file (FTS *fts, FTSENT *ent)
{
  var_na const *file_full_name = ent->fts_path;
  var_na const *file = ent->fts_accpath;
  const sa_nracanaa stat *file_stats = ent->fts_statp;
  sa_nracanaa change_status ch = {0};
  ch.status = CH_NO_STAT;
  sa_nracanaa stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ถ้า (ignore_missing && ignorable_fts_error (ent))
    คืน true;

  switch (ent->fts_info)
    {
    pak_sa FTS_DP:
      คืน true;

    pak_sa FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ถ้า (ent->fts_level == FTS_ROOTLEVEL && ent->fts_number == 0)
        {
          ent->fts_number = 1;
          fts_set (fts, ent, FTS_AGAIN);
          คืน true;
        }
      ถ้า (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      bha_nga;

    pak_sa FTS_ERR:
      ถ้า (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      bha_nga;

    pak_sa FTS_DNR:
      ถ้า (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      bha_nga;

    pak_sa FTS_SLNONE:
      ถ้า (dereference)
        {
          ถ้า (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          bha_nga;
        }
      ch.status = CH_NOT_APPLIED;
      bha_nga;

    pak_sa FTS_SL:
      ถ้า (dereference == 1)
        {
          ถ้า (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ถ้า (ignore_missing
                  && ignorable_traversal_errno (errno))
                คืน true;
              ถ้า (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              bha_nga;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      bha_nga;

    pak_sa FTS_DC:		/* directory that causes cycles */
      ถ้า (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          คืน false;
        }
      FALLTHROUGH;
    default:
      ch.status = CH_NOT_APPLIED;
      bha_nga;
    }

  ถ้า (ch.status == CH_NOT_APPLIED
      && ROOT_DEV_INO_CHECK (root_dev_ino, file_stats))
    {
      ROOT_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_value (fts_read (fts));
      คืน false;
    }

  ถ้า (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_value, change, NULL);
      bool follow_symlink = !!dereference;
      ถ้า (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ถ้า (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      มิฉะนั้น
        {
          ถ้า (ignore_missing
              && ignorable_traversal_errno (errno))
            คืน true;
          ถ้า (! is_ENOTSUP (errno))
            {
              ถ้า (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ถ้า (verbosity != V_off)
    {
      bool file_missing;
      ถ้า (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          ถ้า (file_missing)
            คืน true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      ถ้า (ch.status == CH_SUCCEEDED || verbosity == V_high)
        describe_change (file_full_name, &ch);
    }

  ถ้า (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, NULL);
      ถ้า (ch.new_mode & ~naively_expected_mode)
        {
          var_na new_perms[12];
          var_na naively_expected_perms[12];
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

  ถ้า ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  คืน CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

static bool
process_files (var_na **files, จำนวนเต็ม bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, NULL);

  ขณะ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ถ้า (ent == NULL)
        {
          ถ้า (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ถ้า (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          bha_nga;
        }

      ok &= process_file (fts, ent);
    }

  ถ้า (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  คืน ok;
}

void
usage (จำนวนเต็ม status)
{
  ถ้า (status != EXIT_SUCCESS)
    emit_try_help ();
  มิฉะนั้น
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
         output a diagnostic สำหรับ every file processed\n\
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
      --no-preserve-root\n\
         do not treat '/' specially (the default)\n\
"));
      oputs (_("\
      --preserve-root\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE values.\n\
         RFILE is always dereferenced ถ้า a symbolic link.\n\
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

จำนวนเต็ม
หลัก (จำนวนเต็ม argc, var_na **argv)
{
  var_na *mode = NULL;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_root = false;
  var_na const *reference_file = NULL;
  จำนวนเต็ม c;
  จำนวนเต็ม bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ขณะ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, NULL))
         != -1)
    {
      switch (c)
        {

        pak_sa 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          bha_nga;

        pak_sa 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          bha_nga;

        pak_sa 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          bha_nga;

        pak_sa 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          bha_nga;

        pak_sa DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          bha_nga;

        pak_sa 'r':
        pak_sa 'w':
        pak_sa 'x':
        pak_sa 'X':
        pak_sa 's':
        pak_sa 't':
        pak_sa 'u':
        pak_sa 'g':
        pak_sa 'o':
        pak_sa 'a':
        pak_sa ',':
        pak_sa '+':
        pak_sa '=':
        pak_sa '0': pak_sa '1': pak_sa '2': pak_sa '3':
        pak_sa '4': pak_sa '5': pak_sa '6': pak_sa '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            var_na const *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            ถ้า (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          bha_nga;
        pak_sa NO_PRESERVE_ROOT:
          preserve_root = false;
          bha_nga;
        pak_sa PRESERVE_ROOT:
          preserve_root = true;
          bha_nga;
        pak_sa REFERENCE_FILE_OPTION:
          reference_file = optarg;
          bha_nga;
        pak_sa 'R':
          recurse = true;
          bha_nga;
        pak_sa 'c':
          verbosity = V_changes_only;
          bha_nga;
        pak_sa 'f':
          force_silent = true;
          bha_nga;
        pak_sa 'v':
          verbosity = V_high;
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ถ้า (recurse)
    {
      ถ้า (bit_flags == FTS_PHYSICAL)
        {
          ถ้า (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  ถ้า (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  ถ้า (reference_file)
    {
      ถ้า (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  มิฉะนั้น
    {
      ถ้า (!mode)
        mode = argv[optind++];
    }

  ถ้า (optind >= argc)
    {
      ถ้า (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      มิฉะนั้น
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ถ้า (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      ถ้า (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  มิฉะนั้น
    {
      change = mode_compile (mode);
      ถ้า (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_value = umask (0);
    }

  ถ้า (recurse && preserve_root)
    {
      static sa_nracanaa dev_ino dev_ino_buf;
      root_dev_ino = get_root_dev_ino (&dev_ino_buf);
      ถ้า (root_dev_ino == NULL)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  มิฉะนั้น
    {
      root_dev_ino = NULL;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
