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
  V_उच्चः,

  /* Print a message for each file whose attributes we change.  */
  V_changes_only,

  /* Do not be verbose.  This is the default. */
  V_off
};

/* The desired change to the mode.  */
स्थिर संरचना mode_change *change;

/* The initial umask value, if it might be needed.  */
स्थिर mode_t umask_मानम्;

/* If true, change the modes of directories recursively. */
स्थिर bool recurse;

/* 1 if --dereference, 0 if --no-dereference, -1 if neither has been
   specified.  */
स्थिर पूर्णाङ्क dereference = -1;

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
स्थिर संरचना dev_ino *मूलम्_dev_ino;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  DEREFERENCE_OPTION = CHAR_MAX + 1,
  NO_PRESERVE_मूलम्,
  PRESERVE_मूलम्,
  REFERENCE_FILE_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"changes", no_argument, रिक्त, 'c'},
  {"dereference", no_argument, रिक्त, DEREFERENCE_OPTION},
  {"recursive", no_argument, रिक्त, 'R'},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {"no-preserve-root", no_argument, रिक्त, NO_PRESERVE_मूलम्},
  {"preserve-root", no_argument, रिक्त, PRESERVE_मूलम्},
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
mode_changed (पूर्णाङ्क dir_fd, वर्ण ध्रुव *file, वर्ण ध्रुव *file_full_name,
              mode_t old_mode, mode_t new_mode, bool ignore_missing,
              bool *file_missing)
{
  *file_missing = false;
  जइ (new_mode & (S_ISUID | S_ISGID | S_ISVTX))
    {
      /* The new mode contains unusual bits that the call to chmod may
         have silently cleared.  Check whether they actually changed.  */

      संरचना stat new_stats;

      जइ (fstatat (dir_fd, file, &new_stats, 0) != 0)
        {
          *file_missing = (ignore_missing
                           && ignorable_traversal_errno (errno));
          जइ (! *file_missing && ! force_silent)
            error (0, errno, _("getting new attributes of %s"),
                   quoteaf (file_full_name));
          प्रत्यागम false;
        }

      new_mode = new_stats.st_mode;
    }

  प्रत्यागम ((old_mode ^ new_mode) & CHMOD_MODE_BITS) != 0;
}

/* Tell the user how/if the MODE of FILE has been changed.
   CH describes what (if anything) has happened. */

स्थिर शून्य
describe_change (वर्ण ध्रुव *file, संरचना change_status ध्रुव *ch)
{
  वर्ण perms[12];		/* "-rwxrwxrwx" ls-style modes. */
  वर्ण old_perms[12];
  वर्ण ध्रुव *fmt;
  वर्ण ध्रुव *quoted_file = quoteaf (file);

  विकल्प (ch->status)
    {
    पक्ष CH_NOT_APPLIED:
      printf (_("neither symbolic link %s nor referent has been changed\n"),
              quoted_file);
      प्रत्यागम;

    पक्ष CH_NO_STAT:
      printf (_("%s could not be accessed\n"), quoted_file);
      प्रत्यागम;

    पक्ष CH_FAILED: पक्ष CH_NO_CHANGE_REQUESTED: पक्ष CH_SUCCEEDED:
      भङ्ग;
  }

  unsigned long पूर्णाङ्क
    old_m = ch->old_mode & CHMOD_MODE_BITS,
    m = ch->new_mode & CHMOD_MODE_BITS;

  strmode (ch->new_mode, perms);
  perms[10] = '\0';		/* Remove trailing space.  */

  strmode (ch->old_mode, old_perms);
  old_perms[10] = '\0';		/* Remove trailing space.  */

  विकल्प (ch->status)
    {
    पक्ष CH_SUCCEEDED:
      fmt = _("mode of %s changed from %04lo (%s) to %04lo (%s)\n");
      भङ्ग;
    पक्ष CH_FAILED:
      fmt = _("failed to change mode of %s from %04lo (%s) to %04lo (%s)\n");
      भङ्ग;
    पक्ष CH_NO_CHANGE_REQUESTED:
      fmt = _("mode of %s retained as %04lo (%s)\n");
      printf (fmt, quoted_file, m, &perms[1]);
      प्रत्यागम;
    पक्ष CH_NO_STAT: पक्ष CH_NOT_APPLIED: स्वभाव:
      affirm (false);
    }
  printf (fmt, quoted_file, old_m, &old_perms[1], m, &perms[1]);
}

/* Change the mode of FILE.
   Return true if successful.  This function is called
   once for every file system object that fts encounters.  */

स्थिर bool
process_file (FTS *fts, FTSENT *ent)
{
  वर्ण ध्रुव *file_full_name = ent->fts_path;
  वर्ण ध्रुव *file = ent->fts_accpath;
  ध्रुव संरचना stat *file_stats = ent->fts_statp;
  संरचना change_status ch = {0};
  ch.status = CH_NO_STAT;
  संरचना stat stat_buf;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  जइ (ignore_missing && ignorable_fts_error (ent))
    प्रत्यागम true;

  विकल्प (ent->fts_info)
    {
    पक्ष FTS_DP:
      प्रत्यागम true;

    पक्ष FTS_NS:
      /* For a top-level file or directory, this FTS_NS (stat failed)
         indicator is determined at the time of the initial fts_open call.
         With programs like chmod, chown, and chgrp, that modify
         permissions, it is possible that the file in question is
         accessible when control reaches this point.  So, if this is
         the first time we've seen the FTS_NS for this file, tell
         fts_read to stat it "again".  */
      जइ (ent->fts_level == FTS_ROOTLEVEL && ent->fts_सङ्ख्या == 0)
        {
          ent->fts_सङ्ख्या = 1;
          fts_set (fts, ent, FTS_AGAIN);
          प्रत्यागम true;
        }
      जइ (! force_silent)
        error (0, ent->fts_errno, _("cannot access %s"),
               quoteaf (file_full_name));
      भङ्ग;

    पक्ष FTS_ERR:
      जइ (! force_silent)
        error (0, ent->fts_errno, "%s", quotef (file_full_name));
      भङ्ग;

    पक्ष FTS_DNR:
      जइ (! force_silent)
        error (0, ent->fts_errno, _("cannot read directory %s"),
               quoteaf (file_full_name));
      भङ्ग;

    पक्ष FTS_SLNONE:
      जइ (dereference)
        {
          जइ (! force_silent)
            error (0, 0, _("cannot operate on dangling symlink %s"),
                   quoteaf (file_full_name));
          भङ्ग;
        }
      ch.status = CH_NOT_APPLIED;
      भङ्ग;

    पक्ष FTS_SL:
      जइ (dereference == 1)
        {
          जइ (fstatat (fts->fts_cwd_fd, file, &stat_buf, 0) != 0)
            {
              जइ (ignore_missing
                  && ignorable_traversal_errno (errno))
                प्रत्यागम true;
              जइ (! force_silent)
                error (0, errno, _("cannot dereference %s"),
                       quoteaf (file_full_name));
              भङ्ग;
            }

          file_stats = &stat_buf;
        }
      ch.status = CH_NOT_APPLIED;
      भङ्ग;

    पक्ष FTS_DC:		/* directory that causes cycles */
      जइ (cycle_warning_required (fts, ent))
        {
          emit_cycle_warning (file_full_name);
          प्रत्यागम false;
        }
      FALLTHROUGH;
    स्वभाव:
      ch.status = CH_NOT_APPLIED;
      भङ्ग;
    }

  जइ (ch.status == CH_NOT_APPLIED
      && मूलम्_DEV_INO_CHECK (मूलम्_dev_ino, file_stats))
    {
      मूलम्_DEV_INO_WARN (file_full_name);
      /* Tell fts not to traverse into this hierarchy.  */
      fts_set (fts, ent, FTS_SKIP);
      /* Ensure that we do not process "/" on the second visit.  */
      ignore_मानम् (fts_read (fts));
      प्रत्यागम false;
    }

  जइ (ch.status == CH_NOT_APPLIED)
    {
      ch.old_mode = file_stats->st_mode;
      ch.new_mode = mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                                 umask_मानम्, change, रिक्त);
      bool follow_symlink = !!dereference;
      जइ (dereference == -1) /* -H with/without -R, -P without -R.  */
        follow_symlink = ent->fts_level == FTS_ROOTLEVEL;
      जइ (fchmodat (fts->fts_cwd_fd, file, ch.new_mode,
                    follow_symlink ? 0 : AT_SYMLINK_NOFOLLOW) == 0)
        ch.status = CH_SUCCEEDED;
      अण्णहा
        {
          जइ (ignore_missing
              && ignorable_traversal_errno (errno))
            प्रत्यागम true;
          जइ (! is_ENOTSUP (errno))
            {
              जइ (! force_silent)
                error (0, errno, _("changing permissions of %s"),
                       quoteaf (file_full_name));

              ch.status = CH_FAILED;
            }
          /* else treat not supported as not applied.  */
        }
    }

  जइ (verbosity != V_off)
    {
      bool file_missing;
      जइ (ch.status == CH_SUCCEEDED
          && !mode_changed (fts->fts_cwd_fd, file, file_full_name,
                            ch.old_mode, ch.new_mode, ignore_missing,
                            &file_missing))
        {
          जइ (file_missing)
            प्रत्यागम true;
          ch.status = CH_NO_CHANGE_REQUESTED;
        }

      जइ (ch.status == CH_SUCCEEDED || verbosity == V_उच्चः)
        describe_change (file_full_name, &ch);
    }

  जइ (CH_NO_CHANGE_REQUESTED <= ch.status && diagnose_surprises)
    {
      mode_t naively_expected_mode =
        mode_adjust (ch.old_mode, S_ISDIR (ch.old_mode) != 0,
                     0, change, रिक्त);
      जइ (ch.new_mode & ~naively_expected_mode)
        {
          वर्ण new_perms[12];
          वर्ण naively_expected_perms[12];
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

  जइ ( ! recurse)
    fts_set (fts, ent, FTS_SKIP);

  प्रत्यागम CH_NOT_APPLIED <= ch.status;
}

/* Recursively change the modes of the specified FILES (the last entry
   of which is null).  BIT_FLAGS controls how fts works.
   Return true if successful.  */

स्थिर bool
process_files (वर्ण **files, पूर्णाङ्क bit_flags)
{
  bool ok = true;

  FTS *fts = xfts_open (files, bit_flags, रिक्त);

  जाव (true)
    {
      FTSENT *ent;

      ent = fts_read (fts);
      जइ (ent == रिक्त)
        {
          जइ (errno != 0)
            {
              /* FIXME: try to give a better message  */
              जइ (! force_silent)
                error (0, errno, _("fts_read failed"));
              ok = false;
            }
          भङ्ग;
        }

      ok &= process_file (fts, ent);
    }

  जइ (fts_close (fts) != 0)
    {
      error (0, errno, _("fts_close failed"));
      ok = false;
    }

  प्रत्यागम ok;
}

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
         output a diagnostic प्रति every file processed\n\
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
      --no-preserve-मूलम्\n\
         करेहि not treat '/' specially (the स्वभाव)\n\
"));
      oputs (_("\
      --preserve-मूलम्\n\
         fail to operate recursively on '/'\n\
"));
      oputs (_("\
      --reference=RFILE\n\
         use RFILE's mode instead of specifying MODE मानानि.\n\
         RFILE is always dereferenced जइ a symbolic link.\n\
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *mode = रिक्त;
  idx_t mode_len = 0;
  idx_t mode_alloc = 0;
  bool ok;
  bool preserve_मूलम् = false;
  वर्ण ध्रुव *reference_file = रिक्त;
  पूर्णाङ्क c;
  पूर्णाङ्क bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  जाव ((c = getopt_long (argc, argv,
                           ("HLPRcfhvr::w::x::X::s::t::u::g::o::a::,::+::=::"
                            "0::1::2::3::4::5::6::7::"),
                           long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {

        पक्ष 'H': /* Traverse command-line symlinks-to-directories.  */
          bit_flags = FTS_COMFOLLOW | FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'L': /* Traverse all symlinks-to-directories.  */
          bit_flags = FTS_LOGICAL;
          भङ्ग;

        पक्ष 'P': /* Traverse no symlinks-to-directories.  */
          bit_flags = FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'h': /* --no-dereference: affect symlinks */
          dereference = 0;
          भङ्ग;

        पक्ष DEREFERENCE_OPTION: /* --dereference: affect the referent
                                    of each symlink */
          dereference = 1;
          भङ्ग;

        पक्ष 'r':
        पक्ष 'w':
        पक्ष 'x':
        पक्ष 'X':
        पक्ष 's':
        पक्ष 't':
        पक्ष 'u':
        पक्ष 'g':
        पक्ष 'o':
        पक्ष 'a':
        पक्ष ',':
        पक्ष '+':
        पक्ष '=':
        पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3':
        पक्ष '4': पक्ष '5': पक्ष '6': पक्ष '7':
          /* Support non-portable uses like "chmod -w", but diagnose
             surprises due to umask confusion.  Even though "--", "--r",
             etc., are valid modes, there is no "case '-'" here since
             getopt_long reserves leading "--" for long options.  */
          {
            /* Allocate a mode string (e.g., "-rwx") by concatenating
               the argument containing this option.  If a previous mode
               string was given, concatenate the previous string, a
               comma, and the new string (e.g., "-s,-rwx").  */

            वर्ण ध्रुव *arg = argv[optind - 1];
            idx_t arg_len = strlen (arg);
            idx_t mode_comma_len = mode_len + !!mode_len;
            idx_t new_mode_len = mode_comma_len + arg_len;
            assume (0 <= new_mode_len);  /* Pacify GCC bug #109613.  */
            जइ (mode_alloc <= new_mode_len)
              mode = xpalloc (mode, &mode_alloc,
                              new_mode_len + 1 - mode_alloc, -1, 1);
            mode[mode_len] = ',';
            memcpy (mode + mode_comma_len, arg, arg_len + 1);
            mode_len = new_mode_len;

            diagnose_surprises = true;
          }
          भङ्ग;
        पक्ष NO_PRESERVE_मूलम्:
          preserve_मूलम् = false;
          भङ्ग;
        पक्ष PRESERVE_मूलम्:
          preserve_मूलम् = true;
          भङ्ग;
        पक्ष REFERENCE_FILE_OPTION:
          reference_file = optarg;
          भङ्ग;
        पक्ष 'R':
          recurse = true;
          भङ्ग;
        पक्ष 'c':
          verbosity = V_changes_only;
          भङ्ग;
        पक्ष 'f':
          force_silent = true;
          भङ्ग;
        पक्ष 'v':
          verbosity = V_उच्चः;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  जइ (recurse)
    {
      जइ (bit_flags == FTS_PHYSICAL)
        {
          जइ (dereference == 1)
            error (EXIT_FAILURE, 0,
                   _("-R --dereference requires either -H or -L"));
          dereference = 0;
        }
    }

  जइ (dereference == -1 && bit_flags == FTS_LOGICAL)
    dereference = 1;

  जइ (reference_file)
    {
      जइ (mode)
        {
          error (0, 0, _("cannot combine mode and --reference options"));
          usage (EXIT_FAILURE);
        }
    }
  अण्णहा
    {
      जइ (!mode)
        mode = argv[optind++];
    }

  जइ (optind >= argc)
    {
      जइ (!mode || mode != argv[optind - 1])
        error (0, 0, _("missing operand"));
      अण्णहा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  जइ (reference_file)
    {
      change = mode_create_from_ref (reference_file);
      जइ (!change)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf (reference_file));
    }
  अण्णहा
    {
      change = mode_compile (mode);
      जइ (!change)
        {
          error (0, 0, _("invalid mode: %s"), quote (mode));
          usage (EXIT_FAILURE);
        }
      umask_मानम् = umask (0);
    }

  जइ (recurse && preserve_मूलम्)
    {
      स्थिर संरचना dev_ino dev_ino_buf;
      मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);
      जइ (मूलम्_dev_ino == रिक्त)
        error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
               quoteaf ("/"));
    }
  अण्णहा
    {
      मूलम्_dev_ino = रिक्त;
    }

  bit_flags |= FTS_DEFER_STAT;
  ok = process_files (argv + optind, bit_flags);

  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
