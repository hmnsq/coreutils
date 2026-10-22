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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "system.h"
#ضمن "argv-iter.h"
#ضمن "assure.h"
#ضمن "di-set.h"
#ضمن "exclude.h"
#ضمن "fts-missing.h"
#ضمن "human.h"
#ضمن "mountlist.h"
#ضمن "quote.h"
#ضمن "show-date.h"
#ضمن "stat-size.h"
#ضمن "stat-time.h"
#ضمن "stdio--.h"
#ضمن "xfts.h"
#ضمن "xmemdup0.h"
#ضمن "xstrtol.h"
#ضمن "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "du"

#تعریف_کرو AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ساكن بنية di_سیٹ *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ساكن بنية di_سیٹ *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ساكن idx_t prev_level;

/* Define a class for collecting directory information. */
بنية duinfo
{
  /* Size of files in directory.  */
  uintmax_t ناپ;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  بنية timespec tmax;
};

/* Initialize directory data.  */
ساكن مضمن فراغ
duinfo_init (بنية duinfo *a)
{
  a->ناپ = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ساكن مضمن فراغ
duinfo_سیٹ (بنية duinfo *a, uintmax_t ناپ, بنية timespec tmax)
{
  a->ناپ = ناپ;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ساكن مضمن فراغ
duinfo_جوڑو (بنية duinfo *a, بنية duinfo ثابت *b)
{
  uintmax_t جوڑ = a->ناپ + b->ناپ;
  a->ناپ = a->ناپ <= جوڑ ? جوڑ : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  إذا (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
بنية dulevel
{
  /* Entries in this directory.  */
  بنية duinfo ent;

  /* Total for subdirectories.  */
  بنية duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
ساكن bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
ساكن bool apparent_ناپ = false;

/* If true, count each hard link of files with multiple links.  */
ساكن bool opt_گنتی_all = false;

/* If true, hash all files to look for hard links.  */
ساكن bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ساكن bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ساكن bool quote_output;

/* If true, print a grand total at the end.  */
ساكن bool print_grand_کل = false;

/* If nonzero, do not add sizes of subdirectories.  */
ساكن bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ساكن idx_t max_ڈونگھائی = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ساكن intmax_t opt_حد = 0;

/* Human-readable options for output.  */
ساكن صحيح human_output_opts;

/* Output inodes count instead of blocks used.  */
ساكن bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ساكن bool opt_ویلا = false;

/* Type of time to display. controlled by --time.  */

تعداد ویلا_type
  {
    ویلا_mtime,			/* default */
    ویلا_ctime,
    ویلا_atime
  };

ساكن تعداد ویلا_type ویلا_type = ویلا_mtime;

/* User specified date / time style */
ساكن حرف ثابت *ویلا_style = عدم;

/* Format used to display date / time. Controlled by --time-style */
ساكن حرف ثابت *ویلا_format = عدم;

/* The local time zone rules, as per the TZ environment variable.  */
ساكن timezone_t localtz;

/* The units to use when printing sizes.  */
ساكن uintmax_t output_بلاک_ناپ;

/* File name patterns to exclude.  */
ساكن بنية exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ساكن بنية duinfo tot_dui;

#تعریف_کرو IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#إذا GNULIB_FTS_DEBUG
  FTS_DEBUG,
#جے_انت
  ویلا_OPTION,
  ویلا_STYLE_OPTION,
  INODES_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_دلیل, عدم, 'a'},
  {"apparent-size", no_دلیل, عدم, 'A'},
  {"block-size", required_دلیل, عدم, 'B'},
  {"bytes", no_دلیل, عدم, 'b'},
  {"count-links", no_دلیل, عدم, 'l'},
#إذا GNULIB_FTS_DEBUG
  {"-debug", no_دلیل, عدم, FTS_DEBUG},
#جے_انت
  {"dereference", no_دلیل, عدم, 'L'},
  {"dereference-args", no_دلیل, عدم, 'D'},
  {"exclude", required_دلیل, عدم, EXCLUDE_OPTION},
  {"exclude-from", required_دلیل, عدم, 'X'},
  {"files0-from", required_دلیل, عدم, FILES0_FROM_OPTION},
  {"human-readable", no_دلیل, عدم, 'h'},
  {"inodes", no_دلیل, عدم, INODES_OPTION},
  {"si", no_دلیل, عدم, HUMAN_SI_OPTION},
  {"max-depth", required_دلیل, عدم, 'd'},
  {"null", no_دلیل, عدم, '0'},
  {"no-dereference", no_دلیل, عدم, 'P'},
  {"one-file-system", no_دلیل, عدم, 'x'},
  {"separate-dirs", no_دلیل, عدم, 'S'},
  {"summarize", no_دلیل, عدم, 's'},
  {"total", no_دلیل, عدم, 'c'},
  {"threshold", required_دلیل, عدم, 't'},
  {"time", optional_دلیل, عدم, ویلا_OPTION},
  {"time-style", required_دلیل, عدم, ویلا_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت ویلا_دلیلاں[] =
{
  "atime", "access", "use", "ctime", "status", عدم
};
ساكن تعداد ویلا_type ثابت ویلا_types[] =
{
  ویلا_atime, ویلا_atime, ویلا_atime, ویلا_ctime, ویلا_ctime
};
ARGMATCH_VERIFY (ویلا_دلیلاں, ویلا_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
تعداد ویلا_style
  {
    بھریا_iso_ویلا_style,       /* --time-style=full-iso */
    long_iso_ویلا_style,       /* --time-style=long-iso */
    iso_ویلا_style	       /* --time-style=iso */
  };

ساكن حرف ثابت *ثابت ویلا_style_دلیلاں[] =
{
  "full-iso", "long-iso", "iso", عدم
};
ساكن تعداد ویلا_style ثابت ویلا_style_types[] =
{
  بھریا_iso_ویلا_style, long_iso_ویلا_style, iso_ویلا_style
};
ARGMATCH_VERIFY (ویلا_style_دلیلاں, ویلا_style_types);

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), پروگرام_name, پروگرام_name);
      fputs (_("\
Summarize device usage دا the سیٹ_2 دا FILEs, recursively لكل directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         سرا each output سطر with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         اكتب counts لكل all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-ناپ\n\
         print apparent sizes rather than device usage;\n\
         although the apparent ناپ is usually smaller, it may be\n\
         larger due منزل holes وچ ('sparse') files,\n\
         اندرونی fragmentation, اسدھا بلاکاں, etc.\n\
"));
      oputs (_("\
  -B, --بلاک-ناپ=ناپ_2\n\
         scale sizes by ناپ_2 before printing them; See ناپ_2 format ہیٹھاں;\n\
         E.g., '-BM' prints sizes وچ units دا 1,048,576 بائٹاں\n\
"));
      oputs (_("\
  -b, --بائٹاں\n\
         equivalent منزل '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --کل\n\
         produce a grand کل\n\
"));
      oputs (_("\
  -D, --dereference-دلیلاں\n\
         dereference only symlinks that are listed on the command سطر\n\
"));
      oputs (_("\
  -d, --max-ڈونگھائی=N\n\
         print the کل لكل a directory (or file, with --all)\n\
         only إذا it is N or fewer پدھراں ہیٹھاں the command\n\
         سطر دلیل;  --max-ڈونگھائی=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage دا the NUL-terminated file ناں\n\
         specified وچ file F;  إذا F is -, اقرأ ناں from standard داخلہ\n\
"));
      oputs (_("\
  -H\n\
         equivalent منزل --dereference-دلیلاں (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes وچ human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead دا بلاک usage\n\
"));
      oputs (_("\
  -k\n\
         like --بلاک-ناپ=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --گنتی-links\n\
         گنتی sizes many times إذا hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --بلاک-ناپ=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the افتراضي)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         لكل directories افعل not include ناپ دا subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers دا 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a کل لكل each دلیل\n\
"));
      oputs (_("\
  -t, --حد=ناپ_2\n\
         exclude entries smaller than ناپ_2 إذا مثبت,\n\
         or entries greater than ناپ_2 إذا negative\n\
"));
      oputs (_("\
      --ویلا\n\
         show ویلا دا the آخری modification دا any file وچ the directory,\n\
         or any دا its subdirectories\n\
"));
      oputs (_("\
      --ویلا=لفظ\n\
         show ویلا as لفظ instead دا modification ویلا:\n\
         atime, access, use, ctime or حالت\n\
"));
      oputs (_("\
      --ویلا-style=STYLE\n\
         ویلا/date format with --ویلا; see ویلا_STYLE ہیٹھاں\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that میل any نمونہ_2 وچ FILE\n\
"));
      oputs (_("\
      --exclude=نمونہ\n\
         exclude files that میل نمونہ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_ناپ_note ();
      fputs (_("\
\n\
The --ویلا-style STYLE دلیل can be بھریا-iso, لما-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like وچ date(1).\n\
Also the ویلا_STYLE environment variable sets the افتراضي style منزل use.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ساكن bool
hash_ins (بنية di_سیٹ *di_سیٹ, ino_t ino, dev_t dev)
{
  صحيح inserted = di_سیٹ_داخل_کرو (di_سیٹ, dev, ino);
  إذا (inserted < 0)
    xalloc_die ();
  أرجع inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ساكن فراغ
print_only_ناپ (uintmax_t n_بائٹاں)
{
  حرف buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_بائٹاں == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_بائٹاں, buf, human_output_opts,
                            1, output_بلاک_ناپ)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ساكن فراغ
print_ناپ (ثابت بنية duinfo *pdui, حرف ثابت *string, bool quote)
{
  print_only_ناپ (opt_inodes
                   ? pdui->inodes
                   : pdui->ناپ);

  إذا (opt_ویلا)
    {
      حرف_لکھو ('\t');
      bool ok = show_date (ویلا_format, pdui->tmax, localtz);
      إذا (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  حرف_لکھو ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  حرف_لکھو (opt_nul_terminate_output ? '\0' : '\n');
  إذا (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ساكن فراغ
fill_mount_جدول (فراغ)
{
  بنية mount_اندراج *mnt_ent = read_file_system_list (false);
  طالما (mnt_ent)
    {
      بنية mount_اندراج *mnt_free;
      إذا (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          بنية stat buf;
          إذا (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          وإلا
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

ساكن bool
mount_نقطہ_وچ_fts_چکر (FTSENT ثابت *ent)
{
  FTSENT ثابت *چکر_ent = ent->fts_چکر;

  إذا (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_سیٹ_alloc ();
      إذا (!di_mnt)
        xalloc_die ();

      fill_mount_جدول ();
    }

  طالما (ent && ent != چکر_ent)
    {
      إذا (di_سیٹ_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          أرجع true;
        }
      ent = ent->fts_ماپا;
    }

  أرجع false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

ساكن bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  بنية duinfo dui;
  بنية duinfo dui_منزل_print;
  ساكن idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  ساكن بنية dulevel *dulvl;

  حرف ثابت *file = ent->fts_رستہ;
  ثابت بنية stat *sb = ent->fts_statp;
  صحيح info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_اندراج (ent);

  إذا (ignore_missing && ignorable_fts_error (ent))
    أرجع true;

  إذا (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  وإلا إذا (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      إذا (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          إذا (info == FTS_NSOK)
            {
              fts_سیٹ (fts, ent, FTS_AGAIN);
              MAYBE_بن_ورتیا FTSENT ثابت *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              إذا (ignore_missing && ignorable_fts_error (ent))
                أرجع true;
            }

          إذا (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              أرجع false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          إذا (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      إذا (excluded
          || (! opt_گنتی_all
              && (hash_all || (! S_ISDIR (sb->st_منوال) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          إذا (info == FTS_D)
            {
              fts_سیٹ (fts, ent, FTS_SKIP);
              MAYBE_بن_ورتیا FTSENT ثابت *e = fts_read (fts);
              affirm (e == ent);
            }

          أرجع true;
        }

      بدل (info)
        {
        حالة FTS_D:
          أرجع true;

        حالة FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          اكسر;

        حالة FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          إذا (چکر_warning_required (fts, ent)
              && ! mount_نقطہ_وچ_fts_چکر (ent))
            {
              emit_چکر_warning (file);
              أرجع false;
            }
          أرجع true;
        }
    }

  duinfo_سیٹ (&dui,
              (apparent_ناپ
               ? (usable_st_ناپ (sb) ? MAX (0, sb->st_ناپ) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (ویلا_type == ویلا_mtime ? get_stat_mtime (sb)
               : ویلا_type == ویلا_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_منزل_print = dui;

  إذا (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, حجم *dulvl);
    }
  وإلا
    {
      إذا (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      وإلا إذا (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          إذا (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             حجم *dulvl);

          لكل (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      وإلا /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_جوڑو (&dui_منزل_print, &dulvl[prev_level].ent);
          إذا (!opt_separate_dirs)
            duinfo_جوڑو (&dui_منزل_print, &dulvl[prev_level].subdir);
          duinfo_جوڑو (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_جوڑو (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  إذا (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_جوڑو (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_جوڑو (&tot_dui, &dui);

  إذا ((IS_DIR_TYPE (info) && level <= max_ڈونگھائی)
      || (opt_all && level <= max_ڈونگھائی)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_منزل_print.inodes : dui_منزل_print.ناپ;
      إذا (opt_حد < 0
          ? v <= -opt_حد
          : v >= opt_حد)
        print_ناپ (&dui_منزل_print, file, quote_output);
    }

  أرجع ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساكن bool
du_files (حرف **files, صحيح ثنائی_ہندسہ_flags)
{
  bool ok = true;

  إذا (*files)
    {
      FTS *fts = xfts_open (files, ثنائی_ہندسہ_flags, عدم);

      طالما (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          إذا (ent == عدم)
            {
              إذا (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_رستہ));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              اكسر;
            }

#إذا GNULIB_FTS_DEBUG
          إذا (fts_debug)
            fts_cross_check (fts);
#جے_انت

          ok &= process_file (fts, ent);
        }

      إذا (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  أرجع ok;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  حرف *cwd_only[2];
  bool max_ڈونگھائی_specified = false;
  bool ok = true;
  حرف *files_from = عدم;

  /* Bit flags that control how fts works.  */
  صحيح ثنائی_ہندسہ_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  صحيح symlink_deref_بٹ = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_بلاک_ناپ);

  طالما (true)
    {
      صحيح oi = -1;
      صحيح c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      إذا (c == -1)
        اكسر;

      بدل (c)
        {
#إذا GNULIB_FTS_DEBUG
        حالة FTS_DEBUG:
          fts_debug = true;
          اكسر;
#جے_انت

        حالة '0':
          opt_nul_terminate_output = true;
          اكسر;

        حالة 'a':
          opt_all = true;
          اكسر;

        حالة 'A':
          apparent_ناپ = true;
          اكسر;

        حالة 'b':
          apparent_ناپ = true;
          human_output_opts = 0;
          output_بلاک_ناپ = 1;
          اكسر;

        حالة 'c':
          print_grand_کل = true;
          اكسر;

        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_ادھار_1024;
          output_بلاک_ناپ = 1;
          اكسر;

        حالة HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_بلاک_ناپ = 1;
          اكسر;

        حالة 'k':
          human_output_opts = 0;
          output_بلاک_ناپ = 1024;
          اكسر;

        حالة 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            إذا (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                && ! ckd_جوڑو (&max_ڈونگھائی, tmp, 0) && 0 <= max_ڈونگھائی)
              max_ڈونگھائی_specified = true;
            وإلا
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          اكسر;

        حالة 'm':
          human_output_opts = 0;
          output_بلاک_ناپ = 1024 * 1024;
          اكسر;

        حالة 'l':
          opt_گنتی_all = true;
          اكسر;

        حالة 's':
          opt_summarize_only = true;
          اكسر;

        حالة 't':
          {
            تعداد strtol_error e;
            e = xstrtoimax (optarg, عدم, 0, &opt_حد,
                            "kKmMGTPEZYRQ0");
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            إذا (opt_حد == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          اكسر;

        حالة 'x':
          ثنائی_ہندسہ_flags |= FTS_XDEV;
          اكسر;

        حالة 'B':
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_بلاک_ناپ);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;

        حالة 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        حالة 'D':
          symlink_deref_بٹ = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* --dereference */
          symlink_deref_بٹ = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* --no-dereference */
          symlink_deref_بٹ = FTS_PHYSICAL;
          اكسر;

        حالة 'S':
          opt_separate_dirs = true;
          اكسر;

        حالة 'X':
          إذا (جوڑو_exclude_file (جوڑو_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          اكسر;

        حالة FILES0_FROM_OPTION:
          files_from = optarg;
          اكسر;

        حالة EXCLUDE_OPTION:
          جوڑو_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          اكسر;

        حالة INODES_OPTION:
          opt_inodes = true;
          اكسر;

        حالة ویلا_OPTION:
          opt_ویلا = true;
          ویلا_type =
            (optarg
             ? XARGMATCH ("--time", optarg, ویلا_دلیلاں, ویلا_types)
             : ویلا_mtime);
          localtz = tzalloc (getenv ("TZ"));
          اكسر;

        حالة ویلا_STYLE_OPTION:
          ویلا_style = optarg;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          ok = false;
        }
    }

  إذا (!ok)
    usage (EXIT_FAILURE);

  إذا (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      صحيح qs = getenv_quoting_style ();
      إذا (qs < 0)
        qs = shell_escape_quoting_style;
      إذا (qs != literal_quoting_style)
        {
          سیٹ_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  إذا (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  إذا (opt_summarize_only && max_ڈونگھائی_specified && max_ڈونگھائی == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  إذا (opt_summarize_only && max_ڈونگھائی_specified && max_ڈونگھائی != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_ڈونگھائی);
      usage (EXIT_FAILURE);
    }

  إذا (opt_summarize_only)
    max_ڈونگھائی = 0;

  إذا (opt_inodes)
    {
      إذا (apparent_ناپ)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_بلاک_ناپ = 1;
    }

  /* Process time style if printing last times.  */
  إذا (opt_ویلا)
    {
      إذا (! ویلا_style)
        {
          ویلا_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          إذا (! ویلا_style || streq (ویلا_style, "locale"))
            ویلا_style = "long-iso";
          وإلا إذا (*ویلا_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              حرف ثابت *p = strchr (ویلا_style, '\n');
              إذا (p)
                ویلا_style = xmemdup0 (ویلا_style, p - ویلا_style);
            }
          وإلا
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ساكن حرف ثابت posix_اگیتر[] = "posix-";
              ساكن ثابت size_t اگیتر_len = حجم posix_اگیتر - 1;
              طالما (STREQ_LEN (ویلا_style, posix_اگیتر, اگیتر_len))
                ویلا_style += اگیتر_len;
            }
        }

      إذا (*ویلا_style == '+')
        ویلا_format = ویلا_style + 1;
      وإلا
        {
          بدل (x_timestyle_میل (ویلا_style, /*allow_posix=*/ false,
                                     ویلا_style_دلیلاں,
                                     (حرف ثابت *) ویلا_style_types,
                                     حجم (*ویلا_style_types), EXIT_FAILURE))
            {
            حالة بھریا_iso_ویلا_style:
              ویلا_format = "%Y-%m-%d %H:%M:%S.%N %z";
              اكسر;

            حالة long_iso_ویلا_style:
              ویلا_format = "%Y-%m-%d %H:%M";
              اكسر;

            حالة iso_ویلا_style:
              ویلا_format = "%Y-%m-%d";
              اكسر;
            }
        }
    }

  بنية argv_iterator *ai;
  إذا (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      إذا (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      إذا (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_دھارا (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  وإلا
    {
      حرف **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_بٹ == FTS_LOGICAL);
    }

  إذا (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_سیٹ_alloc ();
  إذا (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  إذا (opt_گنتی_all || ! hash_all)
    ثنائی_ہندسہ_flags |= FTS_TIGHT_چکر_CHECK;

  ثنائی_ہندسہ_flags |= symlink_deref_بٹ;
  ساكن حرف *عارضی_argv[] = { عدم, عدم };

  طالما (true)
    {
      bool skip_file = false;
      تعداد argv_iter_err ai_err;
      حرف *file_name = argv_iter (ai, &ai_err);
      إذا (!file_name)
        {
          بدل (ai_err)
            {
            حالة AI_ERR_EOF:
              اذهب argv_iter_پورا;
            حالة AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              اذهب argv_iter_پورا;
            حالة AI_ERR_MEM:
              xalloc_die ();
            حالة AI_ERR_OK: افتراضي:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      إذا (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      إذا (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          إذا (files_from == عدم)
            error (0, 0, "%s", _("invalid zero-length file name"));
          وإلا
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_نمبر = argv_iter_n_دلیلاں (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_نمبر, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      إذا (skip_file)
        ok = false;
      وإلا
        {
          عارضی_argv[0] = file_name;
          ok &= du_files (عارضی_argv, ثنائی_ہندسہ_flags);
        }
    }
 argv_iter_پورا:

  argv_iter_free (ai);
  di_سیٹ_free (di_files);
  إذا (di_mnt)
    di_سیٹ_free (di_mnt);

  إذا (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  إذا (print_grand_کل)
    print_ناپ (&tot_dui, _("total"), false);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
