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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "dev-ino.h"
#شامل "filemode.h"
#شامل "fts-missing.h"
#شامل "ignore-value.h"
#شامل "modechange.h"
#شامل "quote.h"
#شامل "root-dev-ino.h"
#شامل "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "chmod"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ساخت change_حالت
{
  شمار
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    حالت_2;
  mode_t old_منوال;
  mode_t new_منوال;
};

شمار Verbosity
{
  /* Print a message for each file that is processed.  */
  V_بلند,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ساکن ساخت منوال_change *change;

/* The initial umask value, if it might be needed.  */
ساکن mode_t umask_قدر;

/* If true, change the modes of directories recursively. */
ساکن bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ساکن صحیح dereference = -1;

/* If true, force silence (suppress most of error messages). */
ساکن bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ساکن bool diagnose_surprises;

/* Level of verbosity.  */
ساکن شمار Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ساکن ساخت dev_ino *جڑ_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_جڑ,
  PRESERVE_جڑ,
  REFERENCE_FILE_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"changes", no_دلیل, تہی, 'c'},
  {"dereference", no_دلیل, تہی, DEREFERENCE_OPTION},
  {"recursive", no_دلیل, تہی, 'R'},
  {"no-dereference", no_دلیل, تہی, 'h'},
  {"no-preserve-root", no_دلیل, تہی, NO_PRESERVE_جڑ},
  {"preserve-root", no_دلیل, تہی, PRESERVE_جڑ},
  {"quiet", no_دلیل, تہی, 'f'},
  {"reference", required_دلیل, تہی, REFERENCE_FILE_OPTION},
  {"silent", no_دلیل, تہی, 'f'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ساکن bool
منوال_بدلا (صحیح dir_fd, حرف ثابت *file, حرف ثابت *file_بھرا_name,
              mode_t old_منوال, mode_t new_منوال, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  اگر (new_منوال & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      ساخت stat new_stats;

      اگر (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          اگر (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_بھرا_name));
          واپس false;
        }

      new_منوال = new_stats.st_منوال;
    }

  واپس ((old_منوال ^ new_منوال) & CHMOD_منوال_بٹ) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ساکن خالی
describe_change (حرف ثابت *file, ساخت change_حالت ثابت *ch)
{
  حرف perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  حرف old_perms[12];
  حرف ثابت *fmt;
  حرف ثابت *quoted_file = quoteaf (file);

  انتخاب (ch->حالت_2)
    {
    حالت CH_NOT_APPLIED:
      لکھو (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      واپس;

    حالت CH_NO_STAT:
      لکھو (_("%s could not be accessed\n"), quoted_file);
      واپس;

    حالت CH_FAILED: حالت CH_NO_CHANGE_REQUESTED: حالت CH_SUCCEEDED:
      توڑو;
  }

  غیرعلامتی طویل صحیح
    old_m = ch->old_منوال & CHMOD_منوال_بٹ,
    m = ch->new_منوال & CHMOD_منوال_بٹ;

  strmode (ch->new_منوال, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_منوال, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  انتخاب (ch->حالت_2)
    {
    حالت CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      توڑو;
    حالت CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      توڑو;
    حالت CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      لکھو (fmt, quoted_file, m, &perms[1]);
      واپس;
    حالت CH_NO_STAT: حالت CH_NOT_APPLIED: طےشدہ:
      affirm (false);
    }
  لکھو (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ساکن bool
process_file (FTS *fts, FTSENT *ent)
{
  حرف ثابت *file_بھرا_name = ent->fts_راستہ;
  حرف ثابت *file = ent->fts_accpath;
  ثابت ساخت stat *file_stats = ent->fts_statp;
  ساخت change_حالت ch = {0};
  ch.حالت_2 = CH_NO_STAT;
  ساخت stat stat_buf;
  bool ignore_missing = ignore_missing_fts_اندراج (ent);

  اگر (ignore_missing && ignorable_fts_error (ent))
    واپس true;

  انتخاب (ent->fts_info)
    {
    حالت FTS_DP:
      واپس true;

    حالت FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      اگر (ent->fts_level == FTS_ROOTLEVEL && ent->fts_نمبر == 0)
        {
          ent->fts_نمبر = 1;
          fts_سیٹ (fts, ent, FTS_AGAIN);
          واپس true;
        }
      اگر (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_بھرا_name));
      توڑو;

    حالت FTS_ERR:
      اگر (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_بھرا_name));
      توڑو;

    حالت FTS_DNR:
      اگر (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_بھرا_name));
      توڑو;

    حالت FTS_SLNONE:
      اگر (dereference)
        {
          اگر (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_بھرا_name));
          توڑو;
        }
      ch.حالت_2 = CH_NOT_APPLIED;
      توڑو;

    حالت FTS_SL:
      اگر (dereference == 1)
        {
          اگر (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              اگر (ignore_missing
                  && ignorable_traversal_errno (errno))
                واپس true;
              اگر (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_بھرا_name));
              توڑو;
            }

          file_stats = &stat_buf;
        }
      ch.حالت_2 = CH_NOT_APPLIED;
      توڑو;

    حالت FTS_DC:		/* directory that causes cycles */
      اگر (چکر_warning_required (fts, ent))
        {
          emit_چکر_warning (file_بھرا_name);
          واپس false;
        }
      FALLTHROUGH;
    طےشدہ:
      ch.حالت_2 = CH_NOT_APPLIED;
      توڑو;
    }

  اگر (ch.حالت_2 == CH_NOT_APPLIED
      && جڑ_DEV_INO_CHECK (جڑ_dev_ino, file_stats))
    {
      جڑ_DEV_INO_WARN (file_بھرا_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_سیٹ (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_قدر (fts_read (fts));
      واپس false;
    }

  اگر (ch.حالت_2 == CH_NOT_APPLIED)
    {
      ch.old_منوال = file_stats->st_منوال;
      ch.new_منوال = منوال_adjust (ch.old_منوال, S_ISDIR (ch.old_منوال) != 0,
                                 umask_قدر, change, تہی);
      bool follow_symlink = !!dereference;
      اگر (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      اگر (fchmodat (fts->fts_cwd_fd, file, ch.new_منوال,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.حالت_2 = CH_SUCCEEDED;
      ورنہ
        {
          اگر (ignore_missing
              && ignorable_traversal_errno (errno))
            واپس true;
          اگر (! is_ENOTSUP (errno))
            {
              اگر (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_بھرا_name));

              ch.حالت_2 = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  اگر (verbosity != V_off)
    {
      bool file_missing;
      اگر (ch.حالت_2 == CH_SUCCEEDED
          && !منوال_بدلا (fts->fts_cwd_fd, file, file_بھرا_name,
                            ch.old_منوال, ch.new_منوال, ignore_missing,
                            &file_missing))
        {
          اگر (file_missing)
            واپس true;
          ch.حالت_2 = CH_NO_CHANGE_REQUESTED;
        }

      اگر (ch.حالت_2 == CH_SUCCEEDED || verbosity == V_بلند)
        describe_change (file_بھرا_name, &ch);
    }

  اگر (CH_NO_CHANGE_REQUESTED <= ch.حالت_2 && diagnose_surprises)
    {
      mode_t naively_expected_منوال =
        منوال_adjust (ch.old_منوال, S_ISDIR (ch.old_منوال) != 0,
                     0, change, تہی);
      اگر (ch.new_منوال & ~naively_expected_منوال)
        {
          حرف new_perms[12];
          حرف naively_expected_perms[12];
          strmode (ch.new_منوال, new_perms);
          strmode (naively_expected_منوال, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_بھرا_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.حالت_2 = CH_FAILED;
        }
    }

  اگر ( ! recurse)
    fts_سیٹ (fts, ent, FTS_SKIP);

  واپس CH_NOT_APPLIED <= ch.حالت_2;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساکن bool
process_files (حرف **files, صحیح ثنائی_ہندسہ_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ثنائی_ہندسہ_flags, تہی);

  جب (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      اگر (ent == تہی)
        {
          اگر (errno != 0)
            {
              /* FIXME: try to give a better message  */
              اگر (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          توڑو;
        }

      ok &= process_file (fts, ent);
    }

  اگر (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  واپس ok;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... منوال[,منوال]... FILE...\n\
  or:  %s [OPTION]... OCTAL-منوال FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Change the منوال_2 کا each FILE مقصد منوال.\n\
With --reference, change the منوال_2 کا each FILE مقصد that کا RFILE.\n\
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
         output a diagnostic برائے every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent کا each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
"));
      oputs (_("\
      --no-preserve-جڑ\n\
         کرو not treat '/' specially (the طےشدہ)\n\
"));
      oputs (_("\
      --preserve-جڑ\n\
         fail مقصد عمل_کرو recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's منوال_2 instead کا specifying منوال اقدار.\n\
         RFILE is always dereferenced اگر a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each منوال is کا the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *منوال_2 = تہی;
  idx_t منوال_len = 0;
  idx_t منوال_alloc = 0;
  bool ok;
  bool preserve_جڑ = false;
  حرف ثابت *reference_file = تہی;
  صحیح c;
  صحیح ثنائی_ہندسہ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {

        حالت 'H': /* Traverse command-line symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          توڑو;

        حالت 'L': /* Traverse all symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_LOGICAL;
          توڑو;

        حالت 'P': /* Traverse no symlinks-to-directories.  */
          ثنائی_ہندسہ_flags = FTS_PHYSICAL;
          توڑو;

        حالت 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          توڑو;

        حالت DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          توڑو;

        حالت 'r':
        حالت 'w':
        حالت 'x':
        حالت 'X':
        حالت 's':
        حالت 't':
        حالت 'u':
        حالت 'g':
        حالت 'o':
        حالت 'a':
        حالت ',':
        حالت '+':
        حالت '=':
        حالت '0': حالت '1': حالت '2': حالت '3':
        حالت '4': حالت '5': حالت '6': حالت '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            حرف ثابت *arg = argv[optind - 1];
            idx_t arg_len = لڑی_لمبائی (arg);
            idx_t منوال_comma_len = منوال_len + !!منوال_len;
            idx_t new_منوال_len = منوال_comma_len + arg_len;
            assume (0 <= new_منوال_len);  /* Pacify GCC bug #109613.  */
            اگر (منوال_alloc <= new_منوال_len)
              منوال_2 = xpalloc (منوال_2, &منوال_alloc,
                              new_منوال_len + 1 - منوال_alloc, -1, 1);
            منوال_2[منوال_len] = ',';
            حافظہ_نقل (منوال_2 + منوال_comma_len, arg, arg_len + 1);
            منوال_len = new_منوال_len;

            diagnose_surprises = true;
          }
          توڑو;
        حالت NO_PRESERVE_جڑ:
          preserve_جڑ = false;
          توڑو;
        حالت PRESERVE_جڑ:
          preserve_جڑ = true;
          توڑو;
        حالت REFERENCE_FILE_OPTION:
          reference_file = optarg;
          توڑو;
        حالت 'R':
          recurse = true;
          توڑو;
        حالت 'c':
          verbosity = V_changes_only;
          توڑو;
        حالت 'f':
          force_silent = true;
          توڑو;
        حالت 'v':
          verbosity = V_بلند;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (recurse)
    {
      اگر (ثنائی_ہندسہ_flags == FTS_PHYSICAL)
        {
          اگر (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  اگر (dereference == -1 && ثنائی_ہندسہ_flags == FTS_LOGICAL)
    dereference = 1;

  اگر (reference_file)
    {
      اگر (منوال_2)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  ورنہ
    {
      اگر (!منوال_2)
        منوال_2 = argv[optind++];
    }

  اگر (optind >= argc)
    {
      اگر (!منوال_2 || منوال_2 != argv[optind - 1])
        error (0, 0, _("missing operand"));
      ورنہ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  اگر (reference_file)
    {
      change = منوال_create_from_ref (reference_file);
      اگر (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  ورنہ
    {
      change = منوال_compile (منوال_2);
      اگر (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (منوال_2));
          usage (EXIT_FAILURE);
        }
      umask_قدر = umask (0);
    }

  اگر (recurse && preserve_جڑ)
    {
      ساکن ساخت dev_ino dev_ino_buf;
      جڑ_dev_ino = get_جڑ_dev_ino (&dev_ino_buf);
      اگر (جڑ_dev_ino == تہی)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  ورنہ
    {
      جڑ_dev_ino = تہی;
    }

  ثنائی_ہندسہ_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, ثنائی_ہندسہ_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
