/* du -- summarize device usage
   Copyright (C) 1988-2026 Free Software Foundation, Inc.

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

/* Differences from the Unix du:
   * Doesn't simply ignore the names of regular files given as arguments
     when -a is given.

   By tege@sics.se, Torbjörn Granlund,
   and djm@ai.mit.edu, David MacKenzie.
   Variable blocks added by lm@sgi.com and eggert@twinsun.com.
   Rewritten to use nftw, then to use fts by Jim Meyering.  */

#شامل <config.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"
#شامل "argv-iter.h"
#شامل "assure.h"
#شامل "di-set.h"
#شامل "exclude.h"
#شامل "fts-missing.h"
#شامل "human.h"
#شامل "mountlist.h"
#شامل "quote.h"
#شامل "show-date.h"
#شامل "stat-size.h"
#شامل "stat-time.h"
#شامل "stdio--.h"
#شامل "xfts.h"
#شامل "xmemdup0.h"
#شامل "xstrtol.h"
#شامل "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "du"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ساکن ساخت di_سیٹ *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ساکن ساخت di_سیٹ *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ساکن idx_t پچھلا_level;

/* Define a class for collecting directory information. */
ساخت duinfo
{
  /* Size of files in directory.  */
  uintmax_t سائز;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  ساخت timespec tmax;
};

/* Initialize directory data.  */
ساکن اندرونی خالی
duinfo_init (ساخت duinfo *a)
{
  a->سائز = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ساکن اندرونی خالی
duinfo_سیٹ (ساخت duinfo *a, uintmax_t سائز, ساخت timespec tmax)
{
  a->سائز = سائز;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ساکن اندرونی خالی
duinfo_جوڑو (ساخت duinfo *a, ساخت duinfo ثابت *b)
{
  uintmax_t مجموع = a->سائز + b->سائز;
  a->سائز = a->سائز <= مجموع ? مجموع : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  اگر (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
ساخت dulevel
{
  /* Entries in this directory.  */
  ساخت duinfo ent;

  /* Total for subdirectories.  */
  ساخت duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
ساکن bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
ساکن bool apparent_سائز = false;

/* If true, count each hard link of files with multiple links.  */
ساکن bool opt_گنتی_all = false;

/* If true, hash all files to look for hard links.  */
ساکن bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ساکن bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ساکن bool quote_output;

/* If true, print a grand total at the end.  */
ساکن bool print_grand_کل = false;

/* If nonzero, do not add sizes of subdirectories.  */
ساکن bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ساکن idx_t max_گہرائی = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ساکن intmax_t opt_حد = 0;

/* Human-readable options for output.  */
ساکن صحیح human_output_opts;

/* Output inodes count instead of blocks used.  */
ساکن bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ساکن bool opt_وقت = false;

/* Type of time to display. controlled by --time.  */

شمار وقت_type
  {
    وقت_mtime,			/* default */
    وقت_ctime,
    وقت_atime
  };

ساکن شمار وقت_type وقت_type = وقت_mtime;

/* User specified date / time style */
ساکن حرف ثابت *وقت_style = تہی;

/* Format used to display date / time. Controlled by --time-style */
ساکن حرف ثابت *وقت_format = تہی;

/* The local time zone rules, as per the TZ environment variable.  */
ساکن timezone_t localtz;

/* The units to use when printing sizes.  */
ساکن uintmax_t output_بلاک_سائز;

/* File name patterns to exclude.  */
ساکن ساخت exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ساکن ساخت duinfo tot_dui;

#تعریف_کرو IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#اگر GNULIB_FTS_DEBUG
  FTS_DEBUG,
#اگر_ختم
  وقت_OPTION,
  وقت_STYLE_OPTION,
  INODES_OPTION
};

ساکن ساخت option ثابت long_options[] =
{
  {"all", no_دلیل, تہی, 'a'},
  {"apparent-size", no_دلیل, تہی, 'A'},
  {"block-size", required_دلیل, تہی, 'B'},
  {"bytes", no_دلیل, تہی, 'b'},
  {"count-links", no_دلیل, تہی, 'l'},
#اگر GNULIB_FTS_DEBUG
  {"-debug", no_دلیل, تہی, FTS_DEBUG},
#اگر_ختم
  {"dereference", no_دلیل, تہی, 'L'},
  {"dereference-args", no_دلیل, تہی, 'D'},
  {"exclude", required_دلیل, تہی, EXCLUDE_OPTION},
  {"exclude-from", required_دلیل, تہی, 'X'},
  {"files0-from", required_دلیل, تہی, FILES0_FROM_OPTION},
  {"human-readable", no_دلیل, تہی, 'h'},
  {"inodes", no_دلیل, تہی, INODES_OPTION},
  {"si", no_دلیل, تہی, HUMAN_SI_OPTION},
  {"max-depth", required_دلیل, تہی, 'd'},
  {"null", no_دلیل, تہی, '0'},
  {"no-dereference", no_دلیل, تہی, 'P'},
  {"one-file-system", no_دلیل, تہی, 'x'},
  {"separate-dirs", no_دلیل, تہی, 'S'},
  {"summarize", no_دلیل, تہی, 's'},
  {"total", no_دلیل, تہی, 'c'},
  {"threshold", required_دلیل, تہی, 't'},
  {"time", optional_دلیل, تہی, وقت_OPTION},
  {"time-style", required_دلیل, تہی, وقت_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

ساکن حرف ثابت *ثابت وقت_دلائل[] =
{
  "atime", "access", "use", "ctime", "status", تہی
};
ساکن شمار وقت_type ثابت وقت_types[] =
{
  وقت_atime, وقت_atime, وقت_atime, وقت_ctime, وقت_ctime
};
ARGMATCH_VERIFY (وقت_دلائل, وقت_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
شمار وقت_style
  {
    بھرا_iso_وقت_style,       /* --time-style=full-iso */
    long_iso_وقت_style,       /* --time-style=long-iso */
    iso_وقت_style	       /* --time-style=iso */
  };

ساکن حرف ثابت *ثابت وقت_style_دلائل[] =
{
  "full-iso", "long-iso", "iso", تہی
};
ساکن شمار وقت_style ثابت وقت_style_types[] =
{
  بھرا_iso_وقت_style, long_iso_وقت_style, iso_وقت_style
};
ARGMATCH_VERIFY (وقت_style_دلائل, وقت_style_types);

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), پروگرام_name, پروگرام_name);
      fputs (_("\
Summarize device usage کا the سیٹ_2 کا FILEs, recursively برائے directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         انتہا each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         اكتب counts برائے all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-سائز\n\
         print apparent sizes rather than device usage;\n\
         although the apparent سائز is usually smaller, it may be\n\
         larger due مقصد holes اندر ('sparse') files,\n\
         اندرونی_2 fragmentation, بالواسطہ بلاکیں, etc.\n\
"));
      oputs (_("\
  -B, --بلاک-سائز=سائز_2\n\
         scale sizes by سائز_2 before printing them; See سائز_2 format نیچے;\n\
         E.g., '-BM' prints sizes اندر units کا 1,048,576 بائٹیں\n\
"));
      oputs (_("\
  -b, --بائٹیں\n\
         equivalent مقصد '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --کل\n\
         produce a grand کل\n\
"));
      oputs (_("\
  -D, --dereference-دلائل\n\
         dereference only symlinks that are listed on the command سطر\n\
"));
      oputs (_("\
  -d, --max-گہرائی=N\n\
         print the کل برائے a directory (or file, with --all)\n\
         only اگر it is N or fewer سطحیں نیچے the command\n\
         سطر دلیل;  --max-گہرائی=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage کا the NUL-terminated file نام\n\
         specified اندر file F;  اگر F is -, اقرأ نام from standard داخلہ\n\
"));
      oputs (_("\
  -H\n\
         equivalent مقصد --dereference-دلائل (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes اندر human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead کا بلاک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاک-سائز=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --گنتی-links\n\
         گنتی sizes many times اگر hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --بلاک-سائز=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the طےشدہ)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         برائے directories کرو not include سائز کا subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers کا 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a کل برائے each دلیل\n\
"));
      oputs (_("\
  -t, --حد=سائز_2\n\
         exclude entries smaller than سائز_2 اگر مثبت,\n\
         or entries greater than سائز_2 اگر negative\n\
"));
      oputs (_("\
      --وقت\n\
         show وقت کا the آخری modification کا any file اندر the directory,\n\
         or any کا its subdirectories\n\
"));
      oputs (_("\
      --وقت=لفظ\n\
         show وقت as لفظ instead کا modification وقت:\n\
         atime, access, use, ctime or حالت_2\n\
"));
      oputs (_("\
      --وقت-style=STYLE\n\
         وقت/date format with --وقت; see وقت_STYLE نیچے\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that مطابقت any نمونہ_2 اندر FILE\n\
"));
      oputs (_("\
      --exclude=نمونہ\n\
         exclude files that مطابقت نمونہ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_سائز_note ();
      fputs (_("\
\n\
The --وقت-style STYLE دلیل can be بھرا-iso, طویل-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like اندر date(1).\n\
Also the وقت_STYLE environment variable sets the طےشدہ style مقصد use.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ساکن bool
hash_ins (ساخت di_سیٹ *di_سیٹ, ino_t ino, dev_t dev)
{
  صحیح inserted = di_سیٹ_داخل_کرو (di_سیٹ, dev, ino);
  اگر (inserted < 0)
    xalloc_die ();
  واپس inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ساکن خالی
print_only_سائز (uintmax_t n_بائٹیں)
{
  حرف buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_بائٹیں == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_بائٹیں, buf, human_output_opts,
                            1, output_بلاک_سائز)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ساکن خالی
print_سائز (ثابت ساخت duinfo *pdui, حرف ثابت *string, bool quote)
{
  print_only_سائز (opt_inodes
                   ? pdui->inodes
                   : pdui->سائز);

  اگر (opt_وقت)
    {
      حرف_لکھو ('\t');
      bool ok = show_date (وقت_format, pdui->tmax, localtz);
      اگر (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  حرف_لکھو ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  حرف_لکھو (opt_nul_terminate_output ? '\0' : '\n');
  اگر (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ساکن خالی
fill_mount_جدول (خالی)
{
  ساخت mount_اندراج *mnt_ent = read_file_system_list (false);
  جب (mnt_ent)
    {
      ساخت mount_اندراج *mnt_free;
      اگر (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          ساخت stat buf;
          اگر (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          ورنہ
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_اگلا;
      free_mount_اندراج (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

ساکن bool
mount_نقطہ_اندر_fts_چکر (FTSENT ثابت *ent)
{
  FTSENT ثابت *چکر_ent = ent->fts_چکر;

  اگر (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_سیٹ_alloc ();
      اگر (!di_mnt)
        xalloc_die ();

      fill_mount_جدول ();
    }

  جب (ent && ent != چکر_ent)
    {
      اگر (di_سیٹ_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          واپس true;
        }
      ent = ent->fts_والد;
    }

  واپس false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

ساکن bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  ساخت duinfo dui;
  ساخت duinfo dui_مقصد_print;
  ساکن idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  ساکن ساخت dulevel *dulvl;

  حرف ثابت *file = ent->fts_راستہ;
  ثابت ساخت stat *sb = ent->fts_statp;
  صحیح info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_اندراج (ent);

  اگر (ignore_missing && ignorable_fts_error (ent))
    واپس true;

  اگر (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  ورنہ اگر (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      اگر (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          اگر (info == FTS_NSOK)
            {
              fts_سیٹ (fts, ent, FTS_AGAIN);
              MAYBE_غیر_مستعمل FTSENT ثابت *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              اگر (ignore_missing && ignorable_fts_error (ent))
                واپس true;
            }

          اگر (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              واپس false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          اگر (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      اگر (excluded
          || (! opt_گنتی_all
              && (hash_all || (! S_ISDIR (sb->st_منوال) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          اگر (info == FTS_D)
            {
              fts_سیٹ (fts, ent, FTS_SKIP);
              MAYBE_غیر_مستعمل FTSENT ثابت *e = fts_read (fts);
              affirm (e == ent);
            }

          واپس true;
        }

      انتخاب (info)
        {
        حالت FTS_D:
          واپس true;

        حالت FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          توڑو;

        حالت FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          اگر (چکر_warning_required (fts, ent)
              && ! mount_نقطہ_اندر_fts_چکر (ent))
            {
              emit_چکر_warning (file);
              واپس false;
            }
          واپس true;
        }
    }

  duinfo_سیٹ (&dui,
              (apparent_سائز
               ? (usable_st_سائز (sb) ? MAX (0, sb->st_سائز) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (وقت_type == وقت_mtime ? get_stat_mtime (sb)
               : وقت_type == وقت_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_مقصد_print = dui;

  اگر (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, حجم *dulvl);
    }
  ورنہ
    {
      اگر (level == پچھلا_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      ورنہ اگر (level > پچھلا_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          اگر (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             حجم *dulvl);

          برائے (idx_t i = پچھلا_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      ورنہ /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == پچھلا_level - 1);
          duinfo_جوڑو (&dui_مقصد_print, &dulvl[پچھلا_level].ent);
          اگر (!opt_separate_dirs)
            duinfo_جوڑو (&dui_مقصد_print, &dulvl[پچھلا_level].subdir);
          duinfo_جوڑو (&dulvl[level].subdir, &dulvl[پچھلا_level].ent);
          duinfo_جوڑو (&dulvl[level].subdir, &dulvl[پچھلا_level].subdir);
        }
    }

  پچھلا_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  اگر (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_جوڑو (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_جوڑو (&tot_dui, &dui);

  اگر ((IS_DIR_TYPE (info) && level <= max_گہرائی)
      || (opt_all && level <= max_گہرائی)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_مقصد_print.inodes : dui_مقصد_print.سائز;
      اگر (opt_حد < 0
          ? v <= -opt_حد
          : v >= opt_حد)
        print_سائز (&dui_مقصد_print, file, quote_output);
    }

  واپس ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساکن bool
du_files (حرف **files, صحیح ثنائی_ہندسہ_flags)
{
  bool ok = true;

  اگر (*files)
    {
      FTS *fts = xfts_open (files, ثنائی_ہندسہ_flags, تہی);

      جب (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          اگر (ent == تہی)
            {
              اگر (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_راستہ));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              پچھلا_level = 0;
              توڑو;
            }

#اگر GNULIB_FTS_DEBUG
          اگر (fts_debug)
            fts_cross_check (fts);
#اگر_ختم

          ok &= process_file (fts, ent);
        }

      اگر (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  واپس ok;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *cwd_only[2];
  bool max_گہرائی_specified = false;
  bool ok = true;
  حرف *files_from = تہی;

  /* Bit flags that control how fts works.  */
  صحیح ثنائی_ہندسہ_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  صحیح symlink_deref_بٹ = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_بلاک_سائز);

  جب (true)
    {
      صحیح oi = -1;
      صحیح c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
#اگر GNULIB_FTS_DEBUG
        حالت FTS_DEBUG:
          fts_debug = true;
          توڑو;
#اگر_ختم

        حالت '0':
          opt_nul_terminate_output = true;
          توڑو;

        حالت 'a':
          opt_all = true;
          توڑو;

        حالت 'A':
          apparent_سائز = true;
          توڑو;

        حالت 'b':
          apparent_سائز = true;
          human_output_opts = 0;
          output_بلاک_سائز = 1;
          توڑو;

        حالت 'c':
          print_grand_کل = true;
          توڑو;

        حالت 'h':
          human_output_opts = human_autoscale | human_SI | human_بنیاد_1024;
          output_بلاک_سائز = 1;
          توڑو;

        حالت HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_بلاک_سائز = 1;
          توڑو;

        حالت 'k':
          human_output_opts = 0;
          output_بلاک_سائز = 1024;
          توڑو;

        حالت 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            اگر (xstrtoimax (optarg, تہی, 0, &tmp, "") == LONGINT_OK
                && ! ckd_جوڑو (&max_گہرائی, tmp, 0) && 0 <= max_گہرائی)
              max_گہرائی_specified = true;
            ورنہ
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          توڑو;

        حالت 'm':
          human_output_opts = 0;
          output_بلاک_سائز = 1024 * 1024;
          توڑو;

        حالت 'l':
          opt_گنتی_all = true;
          توڑو;

        حالت 's':
          opt_summarize_only = true;
          توڑو;

        حالت 't':
          {
            شمار strtol_error e;
            e = xstrtoimax (optarg, تہی, 0, &opt_حد,
                            "kKmMGTPEZYRQ0");
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            اگر (opt_حد == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          توڑو;

        حالت 'x':
          ثنائی_ہندسہ_flags |= FTS_XDEV;
          توڑو;

        حالت 'B':
          {
            شمار strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاک_سائز);
            اگر (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          توڑو;

        حالت 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        حالت 'D':
          symlink_deref_بٹ = FTS_COMFOLLOW | FTS_PHYSICAL;
          توڑو;

        حالت 'L': /* --dereference */
          symlink_deref_بٹ = FTS_LOGICAL;
          توڑو;

        حالت 'P': /* --no-dereference */
          symlink_deref_بٹ = FTS_PHYSICAL;
          توڑو;

        حالت 'S':
          opt_separate_dirs = true;
          توڑو;

        حالت 'X':
          اگر (جوڑو_exclude_file (جوڑو_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          توڑو;

        حالت FILES0_FROM_OPTION:
          files_from = optarg;
          توڑو;

        حالت EXCLUDE_OPTION:
          جوڑو_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          توڑو;

        حالت INODES_OPTION:
          opt_inodes = true;
          توڑو;

        حالت وقت_OPTION:
          opt_وقت = true;
          وقت_type =
            (optarg
             ? XARGMATCH ("--time", optarg, وقت_دلائل, وقت_types)
             : وقت_mtime);
          localtz = tzalloc (getenv ("TZ"));
          توڑو;

        حالت وقت_STYLE_OPTION:
          وقت_style = optarg;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          ok = false;
        }
    }

  اگر (!ok)
    usage (EXIT_FAILURE);

  اگر (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      صحیح qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
      اگر (qs != literal_quoting_style)
        {
          سیٹ_quoting_style (تہی, qs);
          quote_output = true;
        }
    }

  اگر (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  اگر (opt_summarize_only && max_گہرائی_specified && max_گہرائی == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  اگر (opt_summarize_only && max_گہرائی_specified && max_گہرائی != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_گہرائی);
      usage (EXIT_FAILURE);
    }

  اگر (opt_summarize_only)
    max_گہرائی = 0;

  اگر (opt_inodes)
    {
      اگر (apparent_سائز)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_بلاک_سائز = 1;
    }

  /* Process time style if printing last times.  */
  اگر (opt_وقت)
    {
      اگر (! وقت_style)
        {
          وقت_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          اگر (! وقت_style || streq (وقت_style, "locale"))
            وقت_style = "long-iso";
          ورنہ اگر (*وقت_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              حرف ثابت *p = strchr (وقت_style, '\n');
              اگر (p)
                وقت_style = xmemdup0 (وقت_style, p - وقت_style);
            }
          ورنہ
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ساکن حرف ثابت posix_سابقہ[] = "posix-";
              ساکن ثابت size_t سابقہ_len = حجم posix_سابقہ - 1;
              جب (STREQ_LEN (وقت_style, posix_سابقہ, سابقہ_len))
                وقت_style += سابقہ_len;
            }
        }

      اگر (*وقت_style == '+')
        وقت_format = وقت_style + 1;
      ورنہ
        {
          انتخاب (x_timestyle_مطابقت (وقت_style, /*allow_posix=*/ false,
                                     وقت_style_دلائل,
                                     (حرف ثابت *) وقت_style_types,
                                     حجم (*وقت_style_types), EXIT_FAILURE))
            {
            حالت بھرا_iso_وقت_style:
              وقت_format = "%Y-%m-%d %H:%M:%S.%N %z";
              توڑو;

            حالت long_iso_وقت_style:
              وقت_format = "%Y-%m-%d %H:%M";
              توڑو;

            حالت iso_وقت_style:
              وقت_format = "%Y-%m-%d";
              توڑو;
            }
        }
    }

  ساخت argv_iterator *ai;
  اگر (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      اگر (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      اگر (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_دھارا (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  ورنہ
    {
      حرف **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_بٹ == FTS_LOGICAL);
    }

  اگر (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_سیٹ_alloc ();
  اگر (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  اگر (opt_گنتی_all || ! hash_all)
    ثنائی_ہندسہ_flags |= FTS_TIGHT_چکر_CHECK;

  ثنائی_ہندسہ_flags |= symlink_deref_بٹ;
  ساکن حرف *عارضی_argv[] = { تہی, تہی };

  جب (true)
    {
      bool skip_file = false;
      شمار argv_iter_err ai_err;
      حرف *file_name = argv_iter (ai, &ai_err);
      اگر (!file_name)
        {
          انتخاب (ai_err)
            {
            حالت AI_ERR_EOF:
              جاؤ argv_iter_مکمل;
            حالت AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              جاؤ argv_iter_مکمل;
            حالت AI_ERR_MEM:
              xalloc_die ();
            حالت AI_ERR_OK: طےشدہ:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      اگر (files_from && streq (files_from, "-") && streq (file_name, "-"))
        {
          /* Give a better diagnostic in an unusual case:
             printf - | du --files0-from=- */
          error (0, 0, _("when reading file names from standard input, "
                         "no file name of %s allowed"),
                 quoteaf (file_name));
          skip_file = true;
        }

      /* Report and skip any empty file names before invoking fts.
         This works around a glitch in fts, which fails immediately
         (without looking at the other file names) when given an empty
         file name.  */
      اگر (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          اگر (files_from == تہی)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ورنہ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_نمبر = argv_iter_n_دلائل (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_نمبر, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      اگر (skip_file)
        ok = false;
      ورنہ
        {
          عارضی_argv[0] = file_name;
          ok &= du_files (عارضی_argv, ثنائی_ہندسہ_flags);
        }
    }
 argv_iter_مکمل:

  argv_iter_free (ai);
  di_سیٹ_free (di_files);
  اگر (di_mnt)
    di_سیٹ_free (di_mnt);

  اگر (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  اگر (print_grand_کل)
    print_سائز (&tot_dui, _("total"), false);

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
