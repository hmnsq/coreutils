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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "dev-ino.h"
#అన్తర్భావ "filemode.h"
#అన్తర్భావ "fts-missing.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "modechange.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "chmod"

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

నిర్మాణం change_స్థితి
{
  గణన
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    స్థితి_2;
  mode_t old_బహుళకం;
  mode_t new_బహుళకం;
};

గణన Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ఎగువ,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
స్థిర నిర్మాణం బహుళకం_change *change;

/* The initial umask value, if it might be needed.  */
స్థిర mode_t umask_విలువ;

/* If true, change the modes of directories recursively. */
స్థిర bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
స్థిర పూర్ణాంకం dereference = -1;

/* If true, force silence (suppress most of error messages). */
స్థిర bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
స్థిర bool diagnose_surprises;

/* Level of verbosity.  */
స్థిర గణన Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
స్థిర నిర్మాణం dev_ino *మూలం_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_మూలం,
  PRESERVE_మూలం,
  REFERENCE_FILE_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"changes", no_వాదం, ఖాళీ, 'c'},
  {"dereference", no_వాదం, ఖాళీ, DEREFERENCE_OPTION},
  {"recursive", no_వాదం, ఖాళీ, 'R'},
  {"no-dereference", no_వాదం, ఖాళీ, 'h'},
  {"no-preserve-root", no_వాదం, ఖాళీ, NO_PRESERVE_మూలం},
  {"preserve-root", no_వాదం, ఖాళీ, PRESERVE_మూలం},
  {"quiet", no_వాదం, ఖాళీ, 'f'},
  {"reference", required_వాదం, ఖాళీ, REFERENCE_FILE_OPTION},
  {"silent", no_వాదం, ఖాళీ, 'f'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

స్థిర bool
బహుళకం_మారింది (పూర్ణాంకం dir_fd, అక్షరం స్థిరరాశి *file, అక్షరం స్థిరరాశి *file_నిండిన_name,
              mode_t old_బహుళకం, mode_t new_బహుళకం, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ఒకవేళ (new_బహుళకం & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      నిర్మాణం stat new_stats;

      ఒకవేళ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ఒకవేళ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_నిండిన_name));
          తిరిగి false;
        }

      new_బహుళకం = new_stats.st_బహుళకం;
    }

  తిరిగి ((old_బహుళకం ^ new_బహుళకం) & CHMOD_బహుళకం_బిట్లు) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

స్థిర శూన్య
describe_change (అక్షరం స్థిరరాశి *file, నిర్మాణం change_స్థితి స్థిరరాశి *ch)
{
  అక్షరం perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  అక్షరం old_perms[12];
  అక్షరం స్థిరరాశి *fmt;
  అక్షరం స్థిరరాశి *quoted_file = quoteaf (file);

  ఎంపిక (ch->స్థితి_2)
    {
    స్థితి CH_NOT_APPLIED:
      రాయి (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      తిరిగి;

    స్థితి CH_NO_STAT:
      రాయి (_("%s could not be accessed\n"), quoted_file);
      తిరిగి;

    స్థితి CH_FAILED: స్థితి CH_NO_CHANGE_REQUESTED: స్థితి CH_SUCCEEDED:
      ఆపు;
  }

  గుర్తులేని దీర్ఘ పూర్ణాంకం
    old_m = ch->old_బహుళకం & CHMOD_బహుళకం_బిట్లు,
    m = ch->new_బహుళకం & CHMOD_బహుళకం_బిట్లు;

  strmode (ch->new_బహుళకం, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_బహుళకం, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ఎంపిక (ch->స్థితి_2)
    {
    స్థితి CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ఆపు;
    స్థితి CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ఆపు;
    స్థితి CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      రాయి (fmt, quoted_file, m, &perms[1]);
      తిరిగి;
    స్థితి CH_NO_STAT: స్థితి CH_NOT_APPLIED: అప్రమేయ:
      affirm (false);
    }
  రాయి (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

స్థిర bool
process_file (FTS *fts, FTSENT *ent)
{
  అక్షరం స్థిరరాశి *file_నిండిన_name = ent->fts_మార్గం;
  అక్షరం స్థిరరాశి *file = ent->fts_accpath;
  స్థిరరాశి నిర్మాణం stat *file_stats = ent->fts_statp;
  నిర్మాణం change_స్థితి ch = {0};
  ch.స్థితి_2 = CH_NO_STAT;
  నిర్మాణం stat stat_buf;
  bool ignore_missing = ignore_missing_fts_నమోదు (ent);

  ఒకవేళ (ignore_missing && ignorable_fts_error (ent))
    తిరిగి true;

  ఎంపిక (ent->fts_info)
    {
    స్థితి FTS_DP:
      తిరిగి true;

    స్థితి FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ఒకవేళ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_సంఖ్య == 0)
        {
          ent->fts_సంఖ్య = 1;
          fts_సమితి (fts, ent, FTS_AGAIN);
          తిరిగి true;
        }
      ఒకవేళ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_నిండిన_name));
      ఆపు;

    స్థితి FTS_ERR:
      ఒకవేళ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_నిండిన_name));
      ఆపు;

    స్థితి FTS_DNR:
      ఒకవేళ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_నిండిన_name));
      ఆపు;

    స్థితి FTS_SLNONE:
      ఒకవేళ (dereference)
        {
          ఒకవేళ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_నిండిన_name));
          ఆపు;
        }
      ch.స్థితి_2 = CH_NOT_APPLIED;
      ఆపు;

    స్థితి FTS_SL:
      ఒకవేళ (dereference == 1)
        {
          ఒకవేళ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ఒకవేళ (ignore_missing
                  && ignorable_traversal_errno (errno))
                తిరిగి true;
              ఒకవేళ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_నిండిన_name));
              ఆపు;
            }

          file_stats = &stat_buf;
        }
      ch.స్థితి_2 = CH_NOT_APPLIED;
      ఆపు;

    స్థితి FTS_DC:		/* directory that causes cycles */
      ఒకవేళ (చక్రం_warning_required (fts, ent))
        {
          emit_చక్రం_warning (file_నిండిన_name);
          తిరిగి false;
        }
      FALLTHROUGH;
    అప్రమేయ:
      ch.స్థితి_2 = CH_NOT_APPLIED;
      ఆపు;
    }

  ఒకవేళ (ch.స్థితి_2 == CH_NOT_APPLIED
      && మూలం_DEV_INO_CHECK (మూలం_dev_ino, file_stats))
    {
      మూలం_DEV_INO_WARN (file_నిండిన_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_సమితి (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_విలువ (fts_read (fts));
      తిరిగి false;
    }

  ఒకవేళ (ch.స్థితి_2 == CH_NOT_APPLIED)
    {
      ch.old_బహుళకం = file_stats->st_బహుళకం;
      ch.new_బహుళకం = బహుళకం_adjust (ch.old_బహుళకం, S_ISDIR (ch.old_బహుళకం) != 0,
                                 umask_విలువ, change, ఖాళీ);
      bool follow_symlink = !!dereference;
      ఒకవేళ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ఒకవేళ (fchmodat (fts->fts_cwd_fd, file, ch.new_బహుళకం,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.స్థితి_2 = CH_SUCCEEDED;
      లేకపోతే
        {
          ఒకవేళ (ignore_missing
              && ignorable_traversal_errno (errno))
            తిరిగి true;
          ఒకవేళ (! is_ENOTSUP (errno))
            {
              ఒకవేళ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_నిండిన_name));

              ch.స్థితి_2 = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ఒకవేళ (verbosity != V_off)
    {
      bool file_missing;
      ఒకవేళ (ch.స్థితి_2 == CH_SUCCEEDED
          && !బహుళకం_మారింది (fts->fts_cwd_fd, file, file_నిండిన_name,
                            ch.old_బహుళకం, ch.new_బహుళకం, ignore_missing,
                            &file_missing))
        {
          ఒకవేళ (file_missing)
            తిరిగి true;
          ch.స్థితి_2 = CH_NO_CHANGE_REQUESTED;
        }

      ఒకవేళ (ch.స్థితి_2 == CH_SUCCEEDED || verbosity == V_ఎగువ)
        describe_change (file_నిండిన_name, &ch);
    }

  ఒకవేళ (CH_NO_CHANGE_REQUESTED <= ch.స్థితి_2 && diagnose_surprises)
    {
      mode_t naively_expected_బహుళకం =
        బహుళకం_adjust (ch.old_బహుళకం, S_ISDIR (ch.old_బహుళకం) != 0,
                     0, change, ఖాళీ);
      ఒకవేళ (ch.new_బహుళకం & ~naively_expected_బహుళకం)
        {
          అక్షరం new_perms[12];
          అక్షరం naively_expected_perms[12];
          strmode (ch.new_బహుళకం, new_perms);
          strmode (naively_expected_బహుళకం, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_నిండిన_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.స్థితి_2 = CH_FAILED;
        }
    }

  ఒకవేళ ( ! recurse)
    fts_సమితి (fts, ent, FTS_SKIP);

  తిరిగి CH_NOT_APPLIED <= ch.స్థితి_2;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

స్థిర bool
process_files (అక్షరం **files, పూర్ణాంకం బిట్టు_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, బిట్టు_flags, ఖాళీ);

  అయితే (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ఒకవేళ (ent == ఖాళీ)
        {
          ఒకవేళ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ఒకవేళ (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ఆపు;
        }

      ok &= process_file (fts, ent);
    }

  ఒకవేళ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  తిరిగి ok;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... బహుళకం[,బహుళకం]... FILE...\n\
  or:  %s [OPTION]... OCTAL-బహుళకం FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Change the బహుళకం_2 యొక్క each FILE గమ్యం బహుళకం.\n\
With --reference, change the బహుళకం_2 యొక్క each FILE గమ్యం that యొక్క RFILE.\n\
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
         output a diagnostic కొరకు every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent యొక్క each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
"));
      oputs (_("\
      --no-preserve-మూలం\n\
         చేయి not treat '/' specially (the అప్రమేయ)\n\
"));
      oputs (_("\
      --preserve-మూలం\n\
         fail గమ్యం నిర్వహించు recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's బహుళకం_2 instead యొక్క specifying బహుళకం విలువలు.\n\
         RFILE is always dereferenced ఒకవేళ a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each బహుళకం is యొక్క the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *బహుళకం_2 = ఖాళీ;
  idx_t బహుళకం_len = 0;
  idx_t బహుళకం_alloc = 0;
  bool ok;
  bool preserve_మూలం = false;
  అక్షరం స్థిరరాశి *reference_file = ఖాళీ;
  పూర్ణాంకం c;
  పూర్ణాంకం బిట్టు_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {

        స్థితి 'H': /* Traverse command-line symlinks-to-directories.  */
          బిట్టు_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ఆపు;

        స్థితి 'L': /* Traverse all symlinks-to-directories.  */
          బిట్టు_flags = FTS_LOGICAL;
          ఆపు;

        స్థితి 'P': /* Traverse no symlinks-to-directories.  */
          బిట్టు_flags = FTS_PHYSICAL;
          ఆపు;

        స్థితి 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ఆపు;

        స్థితి DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ఆపు;

        స్థితి 'r':
        స్థితి 'w':
        స్థితి 'x':
        స్థితి 'X':
        స్థితి 's':
        స్థితి 't':
        స్థితి 'u':
        స్థితి 'g':
        స్థితి 'o':
        స్థితి 'a':
        స్థితి ',':
        స్థితి '+':
        స్థితి '=':
        స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3':
        స్థితి '4': స్థితి '5': స్థితి '6': స్థితి '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            అక్షరం స్థిరరాశి *arg = argv[optind - 1];
            idx_t arg_len = అక్షరశ్రేణి_పొడవు (arg);
            idx_t బహుళకం_comma_len = బహుళకం_len + !!బహుళకం_len;
            idx_t new_బహుళకం_len = బహుళకం_comma_len + arg_len;
            assume (0 <= new_బహుళకం_len);  /* Pacify GCC bug #109613.  */
            ఒకవేళ (బహుళకం_alloc <= new_బహుళకం_len)
              బహుళకం_2 = xpalloc (బహుళకం_2, &బహుళకం_alloc,
                              new_బహుళకం_len + 1 - బహుళకం_alloc, -1, 1);
            బహుళకం_2[బహుళకం_len] = ',';
            జ్ఞాపకం_నకలు (బహుళకం_2 + బహుళకం_comma_len, arg, arg_len + 1);
            బహుళకం_len = new_బహుళకం_len;

            diagnose_surprises = true;
          }
          ఆపు;
        స్థితి NO_PRESERVE_మూలం:
          preserve_మూలం = false;
          ఆపు;
        స్థితి PRESERVE_మూలం:
          preserve_మూలం = true;
          ఆపు;
        స్థితి REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ఆపు;
        స్థితి 'R':
          recurse = true;
          ఆపు;
        స్థితి 'c':
          verbosity = V_changes_only;
          ఆపు;
        స్థితి 'f':
          force_silent = true;
          ఆపు;
        స్థితి 'v':
          verbosity = V_ఎగువ;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (recurse)
    {
      ఒకవేళ (బిట్టు_flags == FTS_PHYSICAL)
        {
          ఒకవేళ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  ఒకవేళ (dereference == -1 && బిట్టు_flags == FTS_LOGICAL)
    dereference = 1;

  ఒకవేళ (reference_file)
    {
      ఒకవేళ (బహుళకం_2)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  లేకపోతే
    {
      ఒకవేళ (!బహుళకం_2)
        బహుళకం_2 = argv[optind++];
    }

  ఒకవేళ (optind >= argc)
    {
      ఒకవేళ (!బహుళకం_2 || బహుళకం_2 != argv[optind - 1])
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (reference_file)
    {
      change = బహుళకం_create_from_ref (reference_file);
      ఒకవేళ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  లేకపోతే
    {
      change = బహుళకం_compile (బహుళకం_2);
      ఒకవేళ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (బహుళకం_2));
          usage (EXIT_FAILURE);
        }
      umask_విలువ = umask (0);
    }

  ఒకవేళ (recurse && preserve_మూలం)
    {
      స్థిర నిర్మాణం dev_ino dev_ino_buf;
      మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);
      ఒకవేళ (మూలం_dev_ino == ఖాళీ)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  లేకపోతే
    {
      మూలం_dev_ino = ఖాళీ;
    }

  బిట్టు_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, బిట్టు_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
