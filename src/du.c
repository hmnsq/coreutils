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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argv-iter.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "di-set.h"
#অন্তর্ভাব "exclude.h"
#অন্তর্ভাব "fts-missing.h"
#অন্তর্ভাব "human.h"
#অন্তর্ভাব "mountlist.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "show-date.h"
#অন্তর্ভাব "stat-size.h"
#অন্তর্ভাব "stat-time.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xfts.h"
#অন্তর্ভাব "xmemdup0.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
স্থির কাঠামো di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
স্থির কাঠামো di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
স্থির idx_t পূর্ব_level;

/* Define a class for collecting directory information. */
কাঠামো duinfo
{
  /* Size of files in directory.  */
  uintmax_t আকার_2;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  কাঠামো timespec tmax;
};

/* Initialize directory data.  */
স্থির অন্তঃরেখ শূন্য
duinfo_init (কাঠামো duinfo *a)
{
  a->আকার_2 = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
স্থির অন্তঃরেখ শূন্য
duinfo_set (কাঠামো duinfo *a, uintmax_t আকার_2, কাঠামো timespec tmax)
{
  a->আকার_2 = আকার_2;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
স্থির অন্তঃরেখ শূন্য
duinfo_add (কাঠামো duinfo *a, কাঠামো duinfo ধ্রুবক *b)
{
  uintmax_t যোগ = a->আকার_2 + b->আকার_2;
  a->আকার_2 = a->আকার_2 <= যোগ ? যোগ : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  যদি (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
কাঠামো dulevel
{
  /* Entries in this directory.  */
  কাঠামো duinfo ent;

  /* Total for subdirectories.  */
  কাঠামো duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
স্থির bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
স্থির bool apparent_আকার = false;

/* If true, count each hard link of files with multiple links.  */
স্থির bool opt_গুনতি_all = false;

/* If true, hash all files to look for hard links.  */
স্থির bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
স্থির bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
স্থির bool quote_output;

/* If true, print a grand total at the end.  */
স্থির bool print_grand_যোগ = false;

/* If nonzero, do not add sizes of subdirectories.  */
স্থির bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
স্থির idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
স্থির intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
স্থির পূর্ণসংখ্যা human_output_opts;

/* Output inodes count instead of blocks used.  */
স্থির bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
স্থির bool opt_time = false;

/* Type of time to display. controlled by --time.  */

গণনা time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

স্থির গণনা time_type time_type = time_mtime;

/* User specified date / time style */
স্থির অক্ষর ধ্রুবক *time_style = ফাঁকা;

/* Format used to display date / time. Controlled by --time-style */
স্থির অক্ষর ধ্রুবক *time_format = ফাঁকা;

/* The local time zone rules, as per the TZ environment variable.  */
স্থির timezone_t localtz;

/* The units to use when printing sizes.  */
স্থির uintmax_t output_block_আকার;

/* File name patterns to exclude.  */
স্থির কাঠামো exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
স্থির কাঠামো duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#যদি GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"all", no_argument, ফাঁকা, 'a'},
  {"apparent-size", no_argument, ফাঁকা, 'A'},
  {"block-size", required_argument, ফাঁকা, 'B'},
  {"bytes", no_argument, ফাঁকা, 'b'},
  {"count-links", no_argument, ফাঁকা, 'l'},
#যদি GNULIB_FTS_DEBUG
  {"-debug", no_argument, ফাঁকা, FTS_DEBUG},
#endif
  {"dereference", no_argument, ফাঁকা, 'L'},
  {"dereference-args", no_argument, ফাঁকা, 'D'},
  {"exclude", required_argument, ফাঁকা, EXCLUDE_OPTION},
  {"exclude-from", required_argument, ফাঁকা, 'X'},
  {"files0-from", required_argument, ফাঁকা, FILES0_FROM_OPTION},
  {"human-readable", no_argument, ফাঁকা, 'h'},
  {"inodes", no_argument, ফাঁকা, INODES_OPTION},
  {"si", no_argument, ফাঁকা, HUMAN_SI_OPTION},
  {"max-depth", required_argument, ফাঁকা, 'd'},
  {"null", no_argument, ফাঁকা, '0'},
  {"no-dereference", no_argument, ফাঁকা, 'P'},
  {"one-file-system", no_argument, ফাঁকা, 'x'},
  {"separate-dirs", no_argument, ফাঁকা, 'S'},
  {"summarize", no_argument, ফাঁকা, 's'},
  {"total", no_argument, ফাঁকা, 'c'},
  {"threshold", required_argument, ফাঁকা, 't'},
  {"time", optional_argument, ফাঁকা, TIME_OPTION},
  {"time-style", required_argument, ফাঁকা, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক time_args[] =
{
  "atime", "access", "use", "ctime", "status", ফাঁকা
};
স্থির গণনা time_type ধ্রুবক time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
গণনা time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

স্থির অক্ষর ধ্রুবক *ধ্রুবক time_style_args[] =
{
  "full-iso", "long-iso", "iso", ফাঁকা
};
স্থির গণনা time_style ধ্রুবক time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively জন্য directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output সারি with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         লিখ counts জন্য all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-আকার_2\n\
         print apparent sizes rather than device usage;\n\
         although the apparent আকার_2 is usually smaller, it may be\n\
         larger due to holes in ('sparse') files,\n\
         internal fragmentation, indirect blocks, etc.\n\
"));
      oputs (_("\
  -B, --block-আকার_2=আকার_3\n\
         scale sizes by আকার_3 before printing them; See আকার_3 format below;\n\
         E.g., '-BM' prints sizes in units of 1,048,576 bytes\n\
"));
      oputs (_("\
  -b, --bytes\n\
         equivalent to '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --যোগ_2\n\
         produce a grand যোগ_2\n\
"));
      oputs (_("\
  -D, --dereference-args\n\
         dereference only symlinks that are listed on the command সারি\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the যোগ_2 জন্য a directory (or file, with --all)\n\
         only যদি it is N or fewer levels below the command\n\
         সারি argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  যদি F is -, পঠ names from standard প্রবেশ\n\
"));
      oputs (_("\
  -H\n\
         equivalent to --dereference-args (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes in human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead of block usage\n\
"));
      oputs (_("\
  -k\n\
         like --block-আকার_2=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --গুনতি-links\n\
         গুনতি sizes many times যদি hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-আকার_2=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the পূর্বনির্ধারিত)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         জন্য directories করো not include আকার_2 of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a যোগ_2 জন্য each argument\n\
"));
      oputs (_("\
  -t, --threshold=আকার_3\n\
         exclude entries smaller than আকার_3 যদি positive,\n\
         or entries greater than আকার_3 যদি negative\n\
"));
      oputs (_("\
      --time\n\
         show time of the last modification of any file in the directory,\n\
         or any of its subdirectories\n\
"));
      oputs (_("\
      --time=WORD\n\
         show time as WORD instead of modification time:\n\
         atime, access, use, ctime or status\n\
"));
      oputs (_("\
      --time-style=STYLE\n\
         time/date format with --time; see TIME_STYLE below\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that match any pattern in FILE\n\
"));
      oputs (_("\
      --exclude=PATTERN\n\
         exclude files that match PATTERN\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_আকার_note ();
      fputs (_("\
\n\
The --time-style STYLE argument can be full-iso, long-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like in date(1).\n\
Also the TIME_STYLE environment variable sets the পূর্বনির্ধারিত style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
স্থির bool
hash_ins (কাঠামো di_set *di_set, ino_t ino, dev_t dev)
{
  পূর্ণসংখ্যা inserted = di_set_ঢোকাও (di_set, dev, ino);
  যদি (inserted < 0)
    xalloc_die ();
  ফেরত inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

স্থির শূন্য
print_only_আকার (uintmax_t n_bytes)
{
  অক্ষর buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_আকার)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

স্থির শূন্য
print_আকার (ধ্রুবক কাঠামো duinfo *pdui, অক্ষর ধ্রুবক *string, bool quote)
{
  print_only_আকার (opt_inodes
                   ? pdui->inodes
                   : pdui->আকার_2);

  যদি (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      যদি (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          অক্ষর buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  যদি (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

স্থির শূন্য
fill_mount_table (শূন্য)
{
  কাঠামো mount_entry *mnt_ent = read_file_system_list (false);
  যখন (mnt_ent)
    {
      কাঠামো mount_entry *mnt_free;
      যদি (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          কাঠামো stat buf;
          যদি (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          অন্যথা
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_পরবর্তী;
      free_mount_entry (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

স্থির bool
mount_point_in_fts_cycle (FTSENT ধ্রুবক *ent)
{
  FTSENT ধ্রুবক *cycle_ent = ent->fts_cycle;

  যদি (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      যদি (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  যখন (ent && ent != cycle_ent)
    {
      যদি (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          ফেরত true;
        }
      ent = ent->fts_parent;
    }

  ফেরত false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

স্থির bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  কাঠামো duinfo dui;
  কাঠামো duinfo dui_to_print;
  স্থির idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  স্থির কাঠামো dulevel *dulvl;

  অক্ষর ধ্রুবক *file = ent->fts_path;
  ধ্রুবক কাঠামো stat *sb = ent->fts_statp;
  পূর্ণসংখ্যা info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  যদি (ignore_missing && ignorable_fts_error (ent))
    ফেরত true;

  যদি (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  অন্যথা যদি (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      যদি (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          যদি (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT ধ্রুবক *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              যদি (ignore_missing && ignorable_fts_error (ent))
                ফেরত true;
            }

          যদি (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              ফেরত false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          যদি (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      যদি (excluded
          || (! opt_গুনতি_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          যদি (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT ধ্রুবক *e = fts_read (fts);
              affirm (e == ent);
            }

          ফেরত true;
        }

      নির্বাচন (info)
        {
        ক্ষেত্র FTS_D:
          ফেরত true;

        ক্ষেত্র FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          বিরতি;

        ক্ষেত্র FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          যদি (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              ফেরত false;
            }
          ফেরত true;
        }
    }

  duinfo_set (&dui,
              (apparent_আকার
               ? (usable_st_আকার (sb) ? MAX (0, sb->st_আকার) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (time_type == time_mtime ? get_stat_mtime (sb)
               : time_type == time_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_to_print = dui;

  যদি (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, আকার *dulvl);
    }
  অন্যথা
    {
      যদি (level == পূর্ব_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      অন্যথা যদি (level > পূর্ব_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          যদি (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             আকার *dulvl);

          জন্য (idx_t i = পূর্ব_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      অন্যথা /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == পূর্ব_level - 1);
          duinfo_add (&dui_to_print, &dulvl[পূর্ব_level].ent);
          যদি (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[পূর্ব_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[পূর্ব_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[পূর্ব_level].subdir);
        }
    }

  পূর্ব_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  যদি (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  যদি ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.আকার_2;
      যদি (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_আকার (&dui_to_print, file, quote_output);
    }

  ফেরত ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

স্থির bool
du_files (অক্ষর **files, পূর্ণসংখ্যা bit_flags)
{
  bool ok = true;

  যদি (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, ফাঁকা);

      যখন (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          যদি (ent == ফাঁকা)
            {
              যদি (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              পূর্ব_level = 0;
              বিরতি;
            }

#যদি GNULIB_FTS_DEBUG
          যদি (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      যদি (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  ফেরত ok;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  অক্ষর *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  অক্ষর *files_from = ফাঁকা;

  /* Bit flags that control how fts works.  */
  পূর্ণসংখ্যা bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  পূর্ণসংখ্যা symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_আকার);

  যখন (true)
    {
      পূর্ণসংখ্যা oi = -1;
      পূর্ণসংখ্যা c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      যদি (c == -1)
        বিরতি;

      নির্বাচন (c)
        {
#যদি GNULIB_FTS_DEBUG
        ক্ষেত্র FTS_DEBUG:
          fts_debug = true;
          বিরতি;
#endif

        ক্ষেত্র '0':
          opt_nul_terminate_output = true;
          বিরতি;

        ক্ষেত্র 'a':
          opt_all = true;
          বিরতি;

        ক্ষেত্র 'A':
          apparent_আকার = true;
          বিরতি;

        ক্ষেত্র 'b':
          apparent_আকার = true;
          human_output_opts = 0;
          output_block_আকার = 1;
          বিরতি;

        ক্ষেত্র 'c':
          print_grand_যোগ = true;
          বিরতি;

        ক্ষেত্র 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_আকার = 1;
          বিরতি;

        ক্ষেত্র HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_আকার = 1;
          বিরতি;

        ক্ষেত্র 'k':
          human_output_opts = 0;
          output_block_আকার = 1024;
          বিরতি;

        ক্ষেত্র 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            যদি (xstrtoimax (optarg, ফাঁকা, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            অন্যথা
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          বিরতি;

        ক্ষেত্র 'm':
          human_output_opts = 0;
          output_block_আকার = 1024 * 1024;
          বিরতি;

        ক্ষেত্র 'l':
          opt_গুনতি_all = true;
          বিরতি;

        ক্ষেত্র 's':
          opt_summarize_only = true;
          বিরতি;

        ক্ষেত্র 't':
          {
            গণনা strtol_error e;
            e = xstrtoimax (optarg, ফাঁকা, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            যদি (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            যদি (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          বিরতি;

        ক্ষেত্র 'x':
          bit_flags |= FTS_XDEV;
          বিরতি;

        ক্ষেত্র 'B':
          {
            গণনা strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_আকার);
            যদি (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          বিরতি;

        ক্ষেত্র 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        ক্ষেত্র 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          বিরতি;

        ক্ষেত্র 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          বিরতি;

        ক্ষেত্র 'S':
          opt_separate_dirs = true;
          বিরতি;

        ক্ষেত্র 'X':
          যদি (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          বিরতি;

        ক্ষেত্র FILES0_FROM_OPTION:
          files_from = optarg;
          বিরতি;

        ক্ষেত্র EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          বিরতি;

        ক্ষেত্র INODES_OPTION:
          opt_inodes = true;
          বিরতি;

        ক্ষেত্র TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          বিরতি;

        ক্ষেত্র TIME_STYLE_OPTION:
          time_style = optarg;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          ok = false;
        }
    }

  যদি (!ok)
    usage (EXIT_FAILURE);

  যদি (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      পূর্ণসংখ্যা qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (ফাঁকা, qs);
          quote_output = true;
        }
    }

  যদি (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  যদি (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  যদি (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  যদি (opt_summarize_only)
    max_depth = 0;

  যদি (opt_inodes)
    {
      যদি (apparent_আকার)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_আকার = 1;
    }

  /* Process time style if printing last times.  */
  যদি (opt_time)
    {
      যদি (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          যদি (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          অন্যথা যদি (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              অক্ষর ধ্রুবক *p = strchr (time_style, '\n');
              যদি (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          অন্যথা
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              স্থির অক্ষর ধ্রুবক posix_prefix[] = "posix-";
              স্থির ধ্রুবক size_t prefix_len = আকার posix_prefix - 1;
              যখন (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      যদি (*time_style == '+')
        time_format = time_style + 1;
      অন্যথা
        {
          নির্বাচন (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (অক্ষর ধ্রুবক *) time_style_types,
                                     আকার (*time_style_types), EXIT_FAILURE))
            {
            ক্ষেত্র full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              বিরতি;

            ক্ষেত্র long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              বিরতি;

            ক্ষেত্র iso_time_style:
              time_format = "%Y-%m-%d";
              বিরতি;
            }
        }
    }

  কাঠামো argv_iterator *ai;
  যদি (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      যদি (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      যদি (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  অন্যথা
    {
      অক্ষর **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  যদি (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  যদি (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  যদি (opt_গুনতি_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  স্থির অক্ষর *অস্থায়ী_argv[] = { ফাঁকা, ফাঁকা };

  যখন (true)
    {
      bool skip_file = false;
      গণনা argv_iter_err ai_err;
      অক্ষর *file_name = argv_iter (ai, &ai_err);
      যদি (!file_name)
        {
          নির্বাচন (ai_err)
            {
            ক্ষেত্র AI_ERR_EOF:
              যাও argv_iter_done;
            ক্ষেত্র AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              যাও argv_iter_done;
            ক্ষেত্র AI_ERR_MEM:
              xalloc_die ();
            ক্ষেত্র AI_ERR_OK: পূর্বনির্ধারিত:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      যদি (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      যদি (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          যদি (files_from == ফাঁকা)
            error (0, 0, "%s", _("invalid zero-length file name"));
          অন্যথা
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_সংখ্যা = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_সংখ্যা, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      যদি (skip_file)
        ok = false;
      অন্যথা
        {
          অস্থায়ী_argv[0] = file_name;
          ok &= du_files (অস্থায়ী_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  যদি (di_mnt)
    di_set_free (di_mnt);

  যদি (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  যদি (print_grand_যোগ)
    print_আকার (&tot_dui, _("total"), false);

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
