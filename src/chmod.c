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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "filemode.h"
#ଅନ୍ତର୍ଭାଵ "fts-missing.h"
#ଅନ୍ତର୍ଭାଵ "ignore-value.h"
#ଅନ୍ତର୍ଭାଵ "modechange.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chmod"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ସଂରଚନା change_status
{
  ଗଣନା
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

ଗଣନା Verbosity
{
  /* Print a message for each file that is processed.  */
  V_ଉପର,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ସ୍ଥିର ସଂରଚନା mode_change *change;

/* The initial umask value, if it might be needed.  */
ସ୍ଥିର mode_t umask_ମୂଲ୍ୟ;

/* If true, change the modes of directories recursively. */
ସ୍ଥିର bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dereference = -1;

/* If true, force silence (suppress most of error messages). */
ସ୍ଥିର bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ସ୍ଥିର bool diagnose_surprises;

/* Level of verbosity.  */
ସ୍ଥିର ଗଣନା Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ସ୍ଥିର ସଂରଚନା dev_ino *ମୂଳ_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_ମୂଳ,
  PRESERVE_ମୂଳ,
  REFERENCE_FILE_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"changes", no_argument, ଖାଲି, 'c'},
  {"dereference", no_argument, ଖାଲି, DEREFERENCE_OPTION},
  {"recursive", no_argument, ଖାଲି, 'R'},
  {"no-dereference", no_argument, ଖାଲି, 'h'},
  {"no-preserve-root", no_argument, ଖାଲି, NO_PRESERVE_ମୂଳ},
  {"preserve-root", no_argument, ଖାଲି, PRESERVE_ମୂଳ},
  {"quiet", no_argument, ଖାଲି, 'f'},
  {"reference", required_argument, ଖାଲି, REFERENCE_FILE_OPTION},
  {"silent", no_argument, ଖାଲି, 'f'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ସ୍ଥିର bool
mode_changed (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dir_fd, ଅକ୍ଷର ଅଚଳ *file, ଅକ୍ଷର ଅଚଳ *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  ଯଦି (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ସଂରଚନା stat new_stats;

      ଯଦି (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          ଯଦି (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          ଫେରନ୍ତୁ false;
        }

      new_mode = new_stats.st_mode;
    }

  ଫେରନ୍ତୁ ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ସ୍ଥିର ଶୂନ୍ୟ
describe_change (ଅକ୍ଷର ଅଚଳ *file, ସଂରଚନା change_status ଅଚଳ *ch)
{
  ଅକ୍ଷର perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  ଅକ୍ଷର old_perms[12];
  ଅକ୍ଷର ଅଚଳ *fmt;
  ଅକ୍ଷର ଅଚଳ *quoted_file = quoteaf (file);

  ବାଛ (ch->status)
    {
    ସ୍ଥିତି CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      ଫେରନ୍ତୁ;

    ସ୍ଥିତି CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      ଫେରନ୍ତୁ;

    ସ୍ଥିତି CH_FAILED: ସ୍ଥିତି CH_NO_CHANGE_REQUESTED: ସ୍ଥିତି CH_SUCCEEDED:
      ଭାଙ୍ଗ;
  }

  unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  ବାଛ (ch->status)
    {
    ସ୍ଥିତି CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      ଭାଙ୍ଗ;
    ସ୍ଥିତି CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି CH_NO_STAT: ସ୍ଥିତି CH_NOT_APPLIED: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ସ୍ଥିର bool
process_file (FTS *fts, FTSENT *ent)
{
  ଅକ୍ଷର ଅଚଳ *file_full_name = ent->fts_path;
  ଅକ୍ଷର ଅଚଳ *file = ent->fts_accpath;
  ଅଚଳ ସଂରଚନା stat *file_stats = ent->fts_statp;
  ସଂରଚନା change_status ch = {0};
  ch.status = CH_NO_STAT;
  ସଂରଚନା stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ଯଦି (ignore_missing && ignorable_fts_error (ent))
    ଫେରନ୍ତୁ true;

  ବାଛ (ent->fts_info)
    {
    ସ୍ଥିତି FTS_DP:
      ଫେରନ୍ତୁ true;

    ସ୍ଥିତି FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      ଯଦି (ent->fts_level == FTS_ROOTLEVEL && ent->fts_ସଂଖ୍ୟା == 0)
        {
          ent->fts_ସଂଖ୍ୟା = 1;
          fts_set (fts, ent, FTS_AGAIN);
          ଫେରନ୍ତୁ true;
        }
      ଯଦି (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_ERR:
      ଯଦି (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DNR:
      ଯଦି (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_SLNONE:
      ଯଦି (dereference)
        {
          ଯଦି (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          ଭାଙ୍ଗ;
        }
      ch.status = CH_NOT_APPLIED;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_SL:
      ଯଦି (dereference == 1)
        {
          ଯଦି (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              ଯଦି (ignore_missing
                  && ignorable_traversal_errno (errno))
                ଫେରନ୍ତୁ true;
              ଯଦି (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              ଭାଙ୍ଗ;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି FTS_DC:		/* directory that causes cycles */
      ଯଦି (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          ଫେରନ୍ତୁ false;
        }
      FALLTHROUGH;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ch.status = CH_NOT_APPLIED;
      ଭାଙ୍ଗ;
    }

  ଯଦି (ch.status == CH_NOT_APPLIED
      && ମୂଳ_DEV_INO_CHECK (ମୂଳ_dev_ino, file_stats))
    {
      ମୂଳ_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_ମୂଲ୍ୟ (fts_read (fts));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_ମୂଲ୍ୟ, change, ଖାଲି);
      bool follow_symlink = !!dereference;
      ଯଦି (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      ଯଦି (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      ଅନ୍ୟଥା
        {
          ଯଦି (ignore_missing
              && ignorable_traversal_errno (errno))
            ଫେରନ୍ତୁ true;
          ଯଦି (! is_ENOTSUP (errno))
            {
              ଯଦି (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  ଯଦି (verbosity != V_off)
    {
      bool file_missing;
      ଯଦି (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          ଯଦି (file_missing)
            ଫେରନ୍ତୁ true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      ଯଦି (ch.status == CH_SUCCEEDED || verbosity == V_ଉପର)
        describe_change (file_full_name, &ch);
    }

  ଯଦି (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, ଖାଲି);
      ଯଦି (ch.new_mode & ~naively_expected_mode)
        {
          ଅକ୍ଷର new_perms[12];
          ଅକ୍ଷର naively_expected_perms[12];
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

  ଯଦି ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  ଫେରନ୍ତୁ CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ସ୍ଥିର bool
process_files (ଅକ୍ଷର **files, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, ଖାଲି);

  ଯେତେବେଳେ (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      ଯଦି (ent == ଖାଲି)
        {
          ଯଦି (errno != 0)
            {
              /* FIXME: try to give a better message  */
              ଯଦି (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          ଭାଙ୍ଗ;
        }

      ok &= process_file (fts, ent);
    }

  ଯଦି (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  ଫେରନ୍ତୁ ok;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         output a diagnostic ପାଇଁ every file processed\n\
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
      --no-preserve-ମୂଳ\n\
         କର not treat '/' specially (the ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
      oputs (_("\
      --preserve-ମୂଳ\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE ମୂଲ୍ୟଗୁଡ଼ିକ.\n\
         RFILE is always dereferenced ଯଦି a symbolic link.\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର *mode = ଖାଲି;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_ମୂଳ = false;
  ଅକ୍ଷର ଅଚଳ *reference_file = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {

        ସ୍ଥିତି 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'r':
        ସ୍ଥିତି 'w':
        ସ୍ଥିତି 'x':
        ସ୍ଥିତି 'X':
        ସ୍ଥିତି 's':
        ସ୍ଥିତି 't':
        ସ୍ଥିତି 'u':
        ସ୍ଥିତି 'g':
        ସ୍ଥିତି 'o':
        ସ୍ଥିତି 'a':
        ସ୍ଥିତି ',':
        ସ୍ଥିତି '+':
        ସ୍ଥିତି '=':
        ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3':
        ସ୍ଥିତି '4': ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            ଅକ୍ଷର ଅଚଳ *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            ଯଦି (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          ଭାଙ୍ଗ;
        ସ୍ଥିତି NO_PRESERVE_ମୂଳ:
          preserve_ମୂଳ = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି PRESERVE_ମୂଳ:
          preserve_ମୂଳ = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି REFERENCE_FILE_OPTION:
          reference_file = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'R':
          recurse = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'c':
          verbosity = V_changes_only;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
          force_silent = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          verbosity = V_ଉପର;
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (recurse)
    {
      ଯଦି (bit_flags == FTS_PHYSICAL)
        {
          ଯଦି (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  ଯଦି (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  ଯଦି (reference_file)
    {
      ଯଦି (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (!mode)
        mode = argv[optind++];
    }

  ଯଦି (optind >= argc)
    {
      ଯଦି (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  ଯଦି (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      ଯଦି (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  ଅନ୍ୟଥା
    {
      change = mode_compile (mode);
      ଯଦି (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_ମୂଲ୍ୟ = umask (0);
    }

  ଯଦି (recurse && preserve_ମୂଳ)
    {
      ସ୍ଥିର ସଂରଚନା dev_ino dev_ino_buf;
      ମୂଳ_dev_ino = get_ମୂଳ_dev_ino (&dev_ino_buf);
      ଯଦି (ମୂଳ_dev_ino == ଖାଲି)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ଅନ୍ୟଥା
    {
      ମୂଳ_dev_ino = ଖାଲି;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
