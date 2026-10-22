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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "assure.h"
#ضمن "dev-ino.h"
#ضمن "filemode.h"
#ضمن "fts-missing.h"
#ضمن "ignore-value.h"
#ضمن "modechange.h"
#ضمن "quote.h"
#ضمن "root-dev-ino.h"
#ضمن "xfts.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "chmod"

#وصف_ڪريو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

بنية change_حالت
{
  تعداد
    {
      CH_NO_STAT,
      CH_FAILED,
      CH_NOT_APPLIED,
      CH_NO_CHANGE_REQUESTED,
      CH_SUCCEEDED
    }
    حالت;
  mode_t old_گهڻائي;
  mode_t new_گهڻائي;
};

تعداد Verbosity
{
  /* Print a message for each file that is processed.  */
  V_مٿاهون,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
ساكن بنية گهڻائي_change *change;

/* The initial umask value, if it might be needed.  */
ساكن mode_t umask_قدر;

/* If true, change the modes of directories recursively. */
ساكن bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
ساكن صحيح dereference = -1;

/* If true, force silence (suppress most of error messages). */
ساكن bool force_silent;

/* If true, diagnose surprises from naive misuses like "chmod -r file".
   POSIX allows diagnostics here, as portable code is supposed to use
   "chmod -- -r file".  */
ساكن bool diagnose_surprises;

/* Level of verbosity.  */
ساكن تعداد Verbosity verbosity = V_off;

/* Pointer to the device and inode numbers of '/', when --recursive.
   Otherwise NULL.  */
ساكن بنية dev_ino *پاڙ_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_پاڙ,
  PRESERVE_پاڙ,
  REFERENCE_FILE_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"changes", no_دليل, عدم, 'c'},
  {"dereference", no_دليل, عدم, DEREFERENCE_OPTION},
  {"recursive", no_دليل, عدم, 'R'},
  {"no-dereference", no_دليل, عدم, 'h'},
  {"no-preserve-root", no_دليل, عدم, NO_PRESERVE_پاڙ},
  {"preserve-root", no_دليل, عدم, PRESERVE_پاڙ},
  {"quiet", no_دليل, عدم, 'f'},
  {"reference", required_دليل, عدم, REFERENCE_FILE_OPTION},
  {"silent", no_دليل, عدم, 'f'},
  {"verbose", no_دليل, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Return true if the chmodable permission bits of FILE changed.
   The old mode was OLD_MODE, but it was changed to NEW_MODE.  */

ساكن bool
گهڻائي_بدليو (صحيح dir_fd, حرف ثابت *file, حرف ثابت *file_ڀريل_name,
              mode_t old_گهڻائي, mode_t new_گهڻائي, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  إذا (new_گهڻائي & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      بنية stat new_stats;

      إذا (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          إذا (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_ڀريل_name));
          أرجع false;
        }

      new_گهڻائي = new_stats.st_گهڻائي;
    }

  أرجع ((old_گهڻائي ^ new_گهڻائي) & CHMOD_گهڻائي_بٽ) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

ساكن فراغ
describe_change (حرف ثابت *file, بنية change_حالت ثابت *ch)
{
  حرف perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  حرف old_perms[12];
  حرف ثابت *fmt;
  حرف ثابت *quoted_file = quoteaf (file);

  بدل (ch->حالت)
    {
    حالة CH_NOT_APPLIED:
      لکو (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      أرجع;

    حالة CH_NO_STAT:
      لکو (_("%s could not be accessed\n"), quoted_file);
      أرجع;

    حالة CH_FAILED: حالة CH_NO_CHANGE_REQUESTED: حالة CH_SUCCEEDED:
      اكسر;
  }

  بنانشان ڊگهو صحيح
    old_m = ch->old_گهڻائي & CHMOD_گهڻائي_بٽ,
    m = ch->new_گهڻائي & CHMOD_گهڻائي_بٽ;

  strmode (ch->new_گهڻائي, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_گهڻائي, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  بدل (ch->حالت)
    {
    حالة CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      اكسر;
    حالة CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      اكسر;
    حالة CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      لکو (fmt, quoted_file, m, &perms[1]);
      أرجع;
    حالة CH_NO_STAT: حالة CH_NOT_APPLIED: افتراضي:
      affirm (false);
    }
  لکو (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

ساكن bool
process_file (FTS *fts, FTSENT *ent)
{
  حرف ثابت *file_ڀريل_name = ent->fts_رستو;
  حرف ثابت *file = ent->fts_accpath;
  ثابت بنية stat *file_stats = ent->fts_statp;
  بنية change_حالت ch = {0};
  ch.حالت = CH_NO_STAT;
  بنية stat stat_buf;
  bool ignore_missing = ignore_missing_fts_داخلا (ent);

  إذا (ignore_missing && ignorable_fts_error (ent))
    أرجع true;

  بدل (ent->fts_info)
    {
    حالة FTS_DP:
      أرجع true;

    حالة FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      إذا (ent->fts_level == FTS_ROOTLEVEL && ent->fts_عدد == 0)
        {
          ent->fts_عدد = 1;
          fts_سيٽ (fts, ent, FTS_AGAIN);
          أرجع true;
        }
      إذا (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_ڀريل_name));
      اكسر;

    حالة FTS_ERR:
      إذا (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_ڀريل_name));
      اكسر;

    حالة FTS_DNR:
      إذا (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_ڀريل_name));
      اكسر;

    حالة FTS_SLNONE:
      إذا (dereference)
        {
          إذا (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_ڀريل_name));
          اكسر;
        }
      ch.حالت = CH_NOT_APPLIED;
      اكسر;

    حالة FTS_SL:
      إذا (dereference == 1)
        {
          إذا (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              إذا (ignore_missing
                  && ignorable_traversal_errno (errno))
                أرجع true;
              إذا (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_ڀريل_name));
              اكسر;
            }

          file_stats = &stat_buf;
        }
      ch.حالت = CH_NOT_APPLIED;
      اكسر;

    حالة FTS_DC:		/* directory that causes cycles */
      إذا (چڪر_warning_required (fts, ent))
        {
          emit_چڪر_warning (file_ڀريل_name);
          أرجع false;
        }
      FALLTHROUGH;
    افتراضي:
      ch.حالت = CH_NOT_APPLIED;
      اكسر;
    }

  إذا (ch.حالت == CH_NOT_APPLIED
      && پاڙ_DEV_INO_CHECK (پاڙ_dev_ino, file_stats))
    {
      پاڙ_DEV_INO_WARN (file_ڀريل_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_سيٽ (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_قدر (fts_read (fts));
      أرجع false;
    }

  إذا (ch.حالت == CH_NOT_APPLIED)
    {
      ch.old_گهڻائي = file_stats->st_گهڻائي;
      ch.new_گهڻائي = گهڻائي_adjust (ch.old_گهڻائي, S_ISDIR (ch.old_گهڻائي) != 0,
                                 umask_قدر, change, عدم);
      bool follow_symlink = !!dereference;
      إذا (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      إذا (fchmodat (fts->fts_cwd_fd, file, ch.new_گهڻائي,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.حالت = CH_SUCCEEDED;
      وإلا
        {
          إذا (ignore_missing
              && ignorable_traversal_errno (errno))
            أرجع true;
          إذا (! is_ENOTSUP (errno))
            {
              إذا (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_ڀريل_name));

              ch.حالت = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  إذا (verbosity != V_off)
    {
      bool file_missing;
      إذا (ch.حالت == CH_SUCCEEDED
          && !گهڻائي_بدليو (fts->fts_cwd_fd, file, file_ڀريل_name,
                            ch.old_گهڻائي, ch.new_گهڻائي, ignore_missing,
                            &file_missing))
        {
          إذا (file_missing)
            أرجع true;
          ch.حالت = CH_NO_CHANGE_REQUESTED;
        }

      إذا (ch.حالت == CH_SUCCEEDED || verbosity == V_مٿاهون)
        describe_change (file_ڀريل_name, &ch);
    }

  إذا (CH_NO_CHANGE_REQUESTED <= ch.حالت && diagnose_surprises)
    {
      mode_t naively_expected_گهڻائي =
        گهڻائي_adjust (ch.old_گهڻائي, S_ISDIR (ch.old_گهڻائي) != 0,
                     0, change, عدم);
      إذا (ch.new_گهڻائي & ~naively_expected_گهڻائي)
        {
          حرف new_perms[12];
          حرف naively_expected_perms[12];
          strmode (ch.new_گهڻائي, new_perms);
          strmode (naively_expected_گهڻائي, naively_expected_perms);
          new_perms[10] = naively_expected_perms[10] = '\0';
          error (0, 0,
                 _("%s: new permissions are %s, not %s"),
                 quotef (file_ڀريل_name),
                 new_perms + 1, naively_expected_perms + 1);
          ch.حالت = CH_FAILED;
        }
    }

  إذا ( ! recurse)
    fts_سيٽ (fts, ent, FTS_SKIP);

  أرجع CH_NOT_APPLIED <= ch.حالت;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساكن bool
process_files (حرف **files, صحيح ٻٽو_انگ_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, ٻٽو_انگ_flags, عدم);

  طالما (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      إذا (ent == عدم)
        {
          إذا (errno != 0)
            {
              /* FIXME: try to give a better message  */
              إذا (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          اكسر;
        }

      ok &= process_file (fts, ent);
    }

  إذا (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  أرجع ok;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... گهڻائي[,گهڻائي]... FILE...\n\
  or:  %s [OPTION]... OCTAL-گهڻائي FILE...\n\
  or:  %s [OPTION]... --reference=RFILE FILE...\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      fputs (_("\
Change the گهڻائي_2 جو each FILE منزل گهڻائي.\n\
With --reference, change the گهڻائي_2 جو each FILE منزل that جو RFILE.\n\
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
         output a diagnostic لكل every file processed\n\
"));
      oputs (_("\
      --dereference\n\
         affect the referent جو each symbolic link,\n\
         rather than the symbolic link itself\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link, rather than the referent\n\
"));
      oputs (_("\
      --no-preserve-پاڙ\n\
         افعل not treat '/' specially (the افتراضي)\n\
"));
      oputs (_("\
      --preserve-پاڙ\n\
         fail منزل هلايو recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's گهڻائي_2 instead جو specifying گهڻائي قدرون.\n\
         RFILE is always dereferenced إذا a symbolic link.\n\
"));
      oputs (_("\
  -R, --recursive\n\
         change files and directories recursively\n\
"));
      emit_symlink_recurse_options ("-H");
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each گهڻائي is جو the form '[ugoa]*([-+=]([rwxXst]*|[ugo]))+|[-+=][0-7]+'.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Parse the ASCII mode given on the command line into a linked list
   of 'struct mode_change' and apply that to each file argument. */

صحيح
مکيه (صحيح argc, حرف **argv)
{
  حرف *گهڻائي_2 = عدم;
  idx_t گهڻائي_len = 0;
  idx_t گهڻائي_alloc = 0;
  bool ok;
  bool preserve_پاڙ = false;
  حرف ثابت *reference_file = عدم;
  صحيح c;
  صحيح ٻٽو_انگ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, عدم))
         != -1)
    {
      بدل (c)
        {

        حالة 'H': /* Traverse command-line symlinks-to-directories.  */
          ٻٽو_انگ_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* Traverse all symlinks-to-directories.  */
          ٻٽو_انگ_flags = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* Traverse no symlinks-to-directories.  */
          ٻٽو_انگ_flags = FTS_PHYSICAL;
          اكسر;

        حالة 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          اكسر;

        حالة DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          اكسر;

        حالة 'r':
        حالة 'w':
        حالة 'x':
        حالة 'X':
        حالة 's':
        حالة 't':
        حالة 'u':
        حالة 'g':
        حالة 'o':
        حالة 'a':
        حالة ',':
        حالة '+':
        حالة '=':
        حالة '0': حالة '1': حالة '2': حالة '3':
        حالة '4': حالة '5': حالة '6': حالة '7':
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
            idx_t arg_len = لڙي_ڊيگهه (arg);
            idx_t گهڻائي_comma_len = گهڻائي_len + !!گهڻائي_len;
            idx_t new_گهڻائي_len = گهڻائي_comma_len + arg_len;
            assume (0 <= new_گهڻائي_len);  /* Pacify GCC bug #109613.  */
            إذا (گهڻائي_alloc <= new_گهڻائي_len)
              گهڻائي_2 = xpalloc (گهڻائي_2, &گهڻائي_alloc,
                              new_گهڻائي_len + 1 - گهڻائي_alloc, -1, 1);
            گهڻائي_2[گهڻائي_len] = ',';
            يادگيري_نقل (گهڻائي_2 + گهڻائي_comma_len, arg, arg_len + 1);
            گهڻائي_len = new_گهڻائي_len;

            diagnose_surprises = true;
          }
          اكسر;
        حالة NO_PRESERVE_پاڙ:
          preserve_پاڙ = false;
          اكسر;
        حالة PRESERVE_پاڙ:
          preserve_پاڙ = true;
          اكسر;
        حالة REFERENCE_FILE_OPTION:
          reference_file = optarg;
          اكسر;
        حالة 'R':
          recurse = true;
          اكسر;
        حالة 'c':
          verbosity = V_changes_only;
          اكسر;
        حالة 'f':
          force_silent = true;
          اكسر;
        حالة 'v':
          verbosity = V_مٿاهون;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (recurse)
    {
      إذا (ٻٽو_انگ_flags == FTS_PHYSICAL)
        {
          إذا (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  إذا (dereference == -1 && ٻٽو_انگ_flags == FTS_LOGICAL)
    dereference = 1;

  إذا (reference_file)
    {
      إذا (گهڻائي_2)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  وإلا
    {
      إذا (!گهڻائي_2)
        گهڻائي_2 = argv[optind++];
    }

  إذا (optind >= argc)
    {
      إذا (!گهڻائي_2 || گهڻائي_2 != argv[optind - 1])
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  إذا (reference_file)
    {
      change = گهڻائي_create_from_ref (reference_file);
      إذا (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  وإلا
    {
      change = گهڻائي_compile (گهڻائي_2);
      إذا (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (گهڻائي_2));
          usage (EXIT_FAILURE);
        }
      umask_قدر = umask (0);
    }

  إذا (recurse && preserve_پاڙ)
    {
      ساكن بنية dev_ino dev_ino_buf;
      پاڙ_dev_ino = get_پاڙ_dev_ino (&dev_ino_buf);
      إذا (پاڙ_dev_ino == عدم)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  وإلا
    {
      پاڙ_dev_ino = عدم;
    }

  ٻٽو_انگ_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, ٻٽو_انگ_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
