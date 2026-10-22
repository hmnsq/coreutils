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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "dev-ino.h"
#അന്തര്ഭാവ "filemode.h"
#അന്തര്ഭാവ "fts-missing.h"
#അന്തര്ഭാവ "ignore-value.h"
#അന്തര്ഭാവ "modechange.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "root-dev-ino.h"
#അന്തര്ഭാവ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ഘടന change_status
{
  എണ്ണൽ
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

എണ്ണൽ Verbosity
{
  /* Print a message for each file that is processed.  */
  V_മുകളിലെ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
സ്ഥിതം ഘടന mode_change *change;

/* The initial umask value, if it might be needed.  */
സ്ഥിതം mode_t umask_മൂല്യം;

/* If true, change the modes of directories recursively. */
സ്ഥിതം bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ dereference = -1;

/* If true, force silence (suppress most of error messages). */
സ്ഥിതം bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
സ്ഥിതം bool diagnose_surprises;

/* Level of verbosity.  */
സ്ഥിതം എണ്ണൽ Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
സ്ഥിതം ഘടന dev_ino *വേര്_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_വേര്,
  PRESERVE_വേര്,
  REFERENCE_FILE_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"changes", no_argument, ശൂന്യ, 'c'},
  {"dereference", no_argument, ശൂന്യ, DEREFERENCE_OPTION},
  {"recursive", no_argument, ശൂന്യ, 'R'},
  {"no-dereference", no_argument, ശൂന്യ, 'h'},
  {"no-preserve-root", no_argument, ശൂന്യ, NO_PRESERVE_വേര്},
  {"preserve-root", no_argument, ശൂന്യ, PRESERVE_വേര്},
  {"quiet", no_argument, ശൂന്യ, 'f'},
  {"reference", required_argument, ശൂന്യ, REFERENCE_FILE_OPTION},
  {"silent", no_argument, ശൂന്യ, 'f'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

സ്ഥിതം bool
mode_changed (പൂർണ്ണസംഖ്യ dir_fd, അക്ഷരം സ്ഥിരം *file, അക്ഷരം സ്ഥിരം *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  എങ്കിൽ (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ഘടന stat new_stats;

      എങ്കിൽ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          എങ്കിൽ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          തിരികെ false;
        }

      new_mode = new_stats.st_mode;
    }

  തിരികെ ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

സ്ഥിതം ശൂന്യം
describe_change (അക്ഷരം സ്ഥിരം *file, ഘടന change_status സ്ഥിരം *ch)
{
  അക്ഷരം perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  അക്ഷരം old_perms[12];
  അക്ഷരം സ്ഥിരം *fmt;
  അക്ഷരം സ്ഥിരം *quoted_file = quoteaf (file);

  തിരഞ്ഞെടുപ്പ് (ch->status)
    {
    അവസ്ഥ CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      തിരികെ;

    അവസ്ഥ CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      തിരികെ;

    അവസ്ഥ CH_FAILED: അവസ്ഥ CH_NO_CHANGE_REQUESTED: അവസ്ഥ CH_SUCCEEDED:
      നിർത്തുക;
  }

  unsigned long പൂർണ്ണസംഖ്യ
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  തിരഞ്ഞെടുപ്പ് (ch->status)
    {
    അവസ്ഥ CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      നിർത്തുക;
    അവസ്ഥ CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      നിർത്തുക;
    അവസ്ഥ CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      തിരികെ;
    അവസ്ഥ CH_NO_STAT: അവസ്ഥ CH_NOT_APPLIED: സ്ഥിരസ്ഥിതി:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

സ്ഥിതം bool
process_file (FTS *fts, FTSENT *ent)
{
  അക്ഷരം സ്ഥിരം *file_full_name = ent->fts_path;
  അക്ഷരം സ്ഥിരം *file = ent->fts_accpath;
  സ്ഥിരം ഘടന stat *file_stats = ent->fts_statp;
  ഘടന change_status ch = {0};
  ch.status = CH_NO_STAT;
  ഘടന stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  എങ്കിൽ (ignore_missing && ignorable_fts_error (ent))
    തിരികെ true;

  തിരഞ്ഞെടുപ്പ് (ent->fts_info)
    {
    അവസ്ഥ FTS_DP:
      തിരികെ true;

    അവസ്ഥ FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      എങ്കിൽ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_സംഖ്യ == 0)
        {
          ent->fts_സംഖ്യ = 1;
          fts_set (fts, ent, FTS_AGAIN);
          തിരികെ true;
        }
      എങ്കിൽ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      നിർത്തുക;

    അവസ്ഥ FTS_ERR:
      എങ്കിൽ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      നിർത്തുക;

    അവസ്ഥ FTS_DNR:
      എങ്കിൽ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      നിർത്തുക;

    അവസ്ഥ FTS_SLNONE:
      എങ്കിൽ (dereference)
        {
          എങ്കിൽ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          നിർത്തുക;
        }
      ch.status = CH_NOT_APPLIED;
      നിർത്തുക;

    അവസ്ഥ FTS_SL:
      എങ്കിൽ (dereference == 1)
        {
          എങ്കിൽ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              എങ്കിൽ (ignore_missing
                  && ignorable_traversal_errno (errno))
                തിരികെ true;
              എങ്കിൽ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              നിർത്തുക;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      നിർത്തുക;

    അവസ്ഥ FTS_DC:		/* directory that causes cycles */
      എങ്കിൽ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          തിരികെ false;
        }
      FALLTHROUGH;
    സ്ഥിരസ്ഥിതി:
      ch.status = CH_NOT_APPLIED;
      നിർത്തുക;
    }

  എങ്കിൽ (ch.status == CH_NOT_APPLIED
      && വേര്_DEV_INO_CHECK (വേര്_dev_ino, file_stats))
    {
      വേര്_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_മൂല്യം (fts_read (fts));
      തിരികെ false;
    }

  എങ്കിൽ (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_മൂല്യം, change, ശൂന്യ);
      bool follow_symlink = !!dereference;
      എങ്കിൽ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      എങ്കിൽ (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (ignore_missing
              && ignorable_traversal_errno (errno))
            തിരികെ true;
          എങ്കിൽ (! is_ENOTSUP (errno))
            {
              എങ്കിൽ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  എങ്കിൽ (verbosity != V_off)
    {
      bool file_missing;
      എങ്കിൽ (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          എങ്കിൽ (file_missing)
            തിരികെ true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      എങ്കിൽ (ch.status == CH_SUCCEEDED || verbosity == V_മുകളിലെ)
        describe_change (file_full_name, &ch);
    }

  എങ്കിൽ (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, ശൂന്യ);
      എങ്കിൽ (ch.new_mode & ~naively_expected_mode)
        {
          അക്ഷരം new_perms[12];
          അക്ഷരം naively_expected_perms[12];
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

  എങ്കിൽ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  തിരികെ CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

സ്ഥിതം bool
process_files (അക്ഷരം **files, പൂർണ്ണസംഖ്യ bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ശൂന്യ);

  ആകുമ്പോൾ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      എങ്കിൽ (ent == ശൂന്യ)
        {
          എങ്കിൽ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              എങ്കിൽ (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          നിർത്തുക;
        }

      ok &= process_file (fts, ent);
    }

  എങ്കിൽ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  തിരികെ ok;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         output a diagnostic വേണ്ടി every file processed\n\
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
      --no-preserve-വേര്\n\
         ചെയ്യുക not treat '/' specially (the സ്ഥിരസ്ഥിതി)\n\
"));
      oputs (_("\
      --preserve-വേര്\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE മൂല്യങ്ങൾ.\n\
         RFILE is always dereferenced എങ്കിൽ a symbolic link.\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം *mode = ശൂന്യ;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_വേര് = false;
  അക്ഷരം സ്ഥിരം *reference_file = ശൂന്യ;
  പൂർണ്ണസംഖ്യ c;
  പൂർണ്ണസംഖ്യ bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {

        അവസ്ഥ 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          നിർത്തുക;

        അവസ്ഥ 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          നിർത്തുക;

        അവസ്ഥ 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          നിർത്തുക;

        അവസ്ഥ DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          നിർത്തുക;

        അവസ്ഥ 'r':
        അവസ്ഥ 'w':
        അവസ്ഥ 'x':
        അവസ്ഥ 'X':
        അവസ്ഥ 's':
        അവസ്ഥ 't':
        അവസ്ഥ 'u':
        അവസ്ഥ 'g':
        അവസ്ഥ 'o':
        അവസ്ഥ 'a':
        അവസ്ഥ ',':
        അവസ്ഥ '+':
        അവസ്ഥ '=':
        അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3':
        അവസ്ഥ '4': അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            അക്ഷരം സ്ഥിരം *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            എങ്കിൽ (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          നിർത്തുക;
        അവസ്ഥ NO_PRESERVE_വേര്:
          preserve_വേര് = false;
          നിർത്തുക;
        അവസ്ഥ PRESERVE_വേര്:
          preserve_വേര് = true;
          നിർത്തുക;
        അവസ്ഥ REFERENCE_FILE_OPTION:
          reference_file = optarg;
          നിർത്തുക;
        അവസ്ഥ 'R':
          recurse = true;
          നിർത്തുക;
        അവസ്ഥ 'c':
          verbosity = V_changes_only;
          നിർത്തുക;
        അവസ്ഥ 'f':
          force_silent = true;
          നിർത്തുക;
        അവസ്ഥ 'v':
          verbosity = V_മുകളിലെ;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (recurse)
    {
      എങ്കിൽ (bit_flags == FTS_PHYSICAL)
        {
          എങ്കിൽ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  എങ്കിൽ (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  എങ്കിൽ (reference_file)
    {
      എങ്കിൽ (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (!mode)
        mode = argv[optind++];
    }

  എങ്കിൽ (optind >= argc)
    {
      എങ്കിൽ (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      എങ്കിൽ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  അല്ലെങ്കിൽ
    {
      change = mode_compile (mode);
      എങ്കിൽ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_മൂല്യം = umask (0);
    }

  എങ്കിൽ (recurse && preserve_വേര്)
    {
      സ്ഥിതം ഘടന dev_ino dev_ino_buf;
      വേര്_dev_ino = get_വേര്_dev_ino (&dev_ino_buf);
      എങ്കിൽ (വേര്_dev_ino == ശൂന്യ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  അല്ലെങ്കിൽ
    {
      വേര്_dev_ino = ശൂന്യ;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
