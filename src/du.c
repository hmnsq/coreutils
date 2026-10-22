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
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ساكن بنية di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ساكن بنية di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ساكن idx_t prev_level;

/* Define a class for collecting directory information. */
بنية duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

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
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ساكن مضمن فراغ
duinfo_set (بنية duinfo *a, uintmax_t size, بنية timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ساكن مضمن فراغ
duinfo_add (بنية duinfo *a, بنية duinfo ثابت *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
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
ساكن bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
ساكن bool opt_كدي_all = false;

/* If true, hash all files to look for hard links.  */
ساكن bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ساكن bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ساكن bool quote_output;

/* If true, print a grand total at the end.  */
ساكن bool بغ_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
ساكن bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ساكن idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ساكن intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
ساكن صحيح human_output_opts;

/* Output inodes count instead of blocks used.  */
ساكن bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ساكن bool opt_time = false;

/* Type of time to display. controlled by --time.  */

تعداد time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

ساكن تعداد time_type time_type = time_mtime;

/* User specified date / time style */
ساكن حرف ثابت *time_style = عدم;

/* Format used to display date / time. Controlled by --time-style */
ساكن حرف ثابت *time_format = عدم;

/* The local time zone rules, as per the TZ environment variable.  */
ساكن timezone_t localtz;

/* The units to use when printing sizes.  */
ساكن uintmax_t output_block_size;

/* File name patterns to exclude.  */
ساكن بنية exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ساكن بنية duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
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
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

ساكن بنية option ثابت long_options[] =
{
  {"all", no_argument, عدم, 'a'},
  {"apparent-size", no_argument, عدم, 'A'},
  {"block-size", required_argument, عدم, 'B'},
  {"bytes", no_argument, عدم, 'b'},
  {"count-links", no_argument, عدم, 'l'},
#إذا GNULIB_FTS_DEBUG
  {"-debug", no_argument, عدم, FTS_DEBUG},
#endif
  {"dereference", no_argument, عدم, 'L'},
  {"dereference-args", no_argument, عدم, 'D'},
  {"exclude", required_argument, عدم, EXCLUDE_OPTION},
  {"exclude-from", required_argument, عدم, 'X'},
  {"files0-from", required_argument, عدم, FILES0_FROM_OPTION},
  {"human-readable", no_argument, عدم, 'h'},
  {"inodes", no_argument, عدم, INODES_OPTION},
  {"si", no_argument, عدم, HUMAN_SI_OPTION},
  {"max-depth", required_argument, عدم, 'd'},
  {"null", no_argument, عدم, '0'},
  {"no-dereference", no_argument, عدم, 'P'},
  {"one-file-system", no_argument, عدم, 'x'},
  {"separate-dirs", no_argument, عدم, 'S'},
  {"summarize", no_argument, عدم, 's'},
  {"total", no_argument, عدم, 'c'},
  {"threshold", required_argument, عدم, 't'},
  {"time", optional_argument, عدم, TIME_OPTION},
  {"time-style", required_argument, عدم, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

ساكن حرف ثابت *ثابت time_حجوجي[] =
{
  "atime", "access", "use", "ctime", "status", عدم
};
ساكن تعداد time_type ثابت time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_حجوجي, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
تعداد time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

ساكن حرف ثابت *ثابت time_style_حجوجي[] =
{
  "full-iso", "long-iso", "iso", عدم
};
ساكن تعداد time_style ثابت time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_حجوجي, time_style_types);

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively لكل directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output لي with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         اكتب counts لكل all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-size\n\
         بغ_2 apparent sizes rather than device usage;\n\
         although the apparent size is usually smaller, it may be\n\
         larger due to holes in ('sparse') files,\n\
         internal fragmentation, indirect blocks, etc.\n\
"));
      oputs (_("\
  -B, --block-size=SIZE\n\
         scale sizes by SIZE before printing them; See SIZE format below;\n\
         E.g., '-BM' prints sizes in units of 1,048,576 bytes\n\
"));
      oputs (_("\
  -b, --bytes\n\
         equivalent to '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --total\n\
         produce a grand total\n\
"));
      oputs (_("\
  -D, --dereference-حجوجي\n\
         dereference only symlinks that are listed on the command لي\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         بغ_2 the total لكل a directory (or file, with --all)\n\
         only إذا it is N or fewer levels below the command\n\
         لي argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  إذا F is -, اقرأ names from standard شغرو\n\
"));
      oputs (_("\
  -H\n\
         equivalent to --dereference-حجوجي (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         بغ_2 sizes in human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead of block usage\n\
"));
      oputs (_("\
  -k\n\
         like --block-size=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --كدي-links\n\
         كدي sizes many times إذا hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the افتراضي)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         لكل directories افعل not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total لكل each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE إذا positive,\n\
         or entries greater than SIZE إذا negative\n\
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
      emit_size_note ();
      fputs (_("\
\n\
The --time-style STYLE argument can be full-iso, long-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like in date(1).\n\
Also the TIME_STYLE environment variable sets the افتراضي style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ساكن bool
hash_ins (بنية di_set *di_set, ino_t ino, dev_t dev)
{
  صحيح inserted = di_set_سك (di_set, dev, ino);
  إذا (inserted < 0)
    xalloc_die ();
  أرجع inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ساكن فراغ
بغ_only_size (uintmax_t n_bytes)
{
  حرف buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ساكن فراغ
بغ_size (ثابت بنية duinfo *pdui, حرف ثابت *string, bool quote)
{
  بغ_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  إذا (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      إذا (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          حرف buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  إذا (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ساكن فراغ
fill_mount_table (فراغ)
{
  بنية mount_entry *mnt_ent = read_file_system_list (false);
  طالما (mnt_ent)
    {
      بنية mount_entry *mnt_free;
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
      mnt_ent = mnt_ent->me_next;
      free_mount_entry (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

ساكن bool
mount_point_in_fts_cycle (FTSENT ثابت *ent)
{
  FTSENT ثابت *cycle_ent = ent->fts_cycle;

  إذا (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      إذا (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  طالما (ent && ent != cycle_ent)
    {
      إذا (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          أرجع true;
        }
      ent = ent->fts_parent;
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
  بنية duinfo dui_to_بغ;
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

  حرف ثابت *file = ent->fts_path;
  ثابت بنية stat *sb = ent->fts_statp;
  صحيح info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

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
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT ثابت *e = fts_read (fts);
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
          || (! opt_كدي_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          إذا (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT ثابت *e = fts_read (fts);
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
          إذا (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              أرجع false;
            }
          أرجع true;
        }
    }

  duinfo_set (&dui,
              (apparent_size
               ? (usable_st_size (sb) ? MAX (0, sb->st_size) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (time_type == time_mtime ? get_stat_mtime (sb)
               : time_type == time_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_to_بغ = dui;

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
          duinfo_add (&dui_to_بغ, &dulvl[prev_level].ent);
          إذا (!opt_separate_dirs)
            duinfo_add (&dui_to_بغ, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  إذا (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  إذا ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_بغ.inodes : dui_to_بغ.size;
      إذا (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        بغ_size (&dui_to_بغ, file, quote_output);
    }

  أرجع ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ساكن bool
du_files (حرف **files, صحيح bit_flags)
{
  bool ok = true;

  إذا (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, عدم);

      طالما (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          إذا (ent == عدم)
            {
              إذا (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
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
#endif

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
main (صحيح argc, حرف **argv)
{
  حرف *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  حرف *files_from = عدم;

  /* Bit flags that control how fts works.  */
  صحيح bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  صحيح symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

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
#endif

        حالة '0':
          opt_nul_terminate_output = true;
          اكسر;

        حالة 'a':
          opt_all = true;
          اكسر;

        حالة 'A':
          apparent_size = true;
          اكسر;

        حالة 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          اكسر;

        حالة 'c':
          بغ_grand_total = true;
          اكسر;

        حالة 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          اكسر;

        حالة HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          اكسر;

        حالة 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          اكسر;

        حالة 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            إذا (xstrtoimax (optarg, عدم, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
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
          output_block_size = 1024 * 1024;
          اكسر;

        حالة 'l':
          opt_كدي_all = true;
          اكسر;

        حالة 's':
          opt_summarize_only = true;
          اكسر;

        حالة 't':
          {
            تعداد strtol_error e;
            e = xstrtoimax (optarg, عدم, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            إذا (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          اكسر;

        حالة 'x':
          bit_flags |= FTS_XDEV;
          اكسر;

        حالة 'B':
          {
            تعداد strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            إذا (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          اكسر;

        حالة 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        حالة 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          اكسر;

        حالة 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          اكسر;

        حالة 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          اكسر;

        حالة 'S':
          opt_separate_dirs = true;
          اكسر;

        حالة 'X':
          إذا (add_exclude_file (add_exclude, exclude, optarg,
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
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          اكسر;

        حالة INODES_OPTION:
          opt_inodes = true;
          اكسر;

        حالة TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_حجوجي, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          اكسر;

        حالة TIME_STYLE_OPTION:
          time_style = optarg;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

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
          set_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  إذا (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  إذا (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  إذا (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  إذا (opt_summarize_only)
    max_depth = 0;

  إذا (opt_inodes)
    {
      إذا (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  إذا (opt_time)
    {
      إذا (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          إذا (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          وإلا إذا (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              حرف ثابت *p = strchr (time_style, '\n');
              إذا (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          وإلا
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ساكن حرف ثابت posix_prefix[] = "posix-";
              ساكن ثابت size_t prefix_len = حجم posix_prefix - 1;
              طالما (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      إذا (*time_style == '+')
        time_format = time_style + 1;
      وإلا
        {
          بدل (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_حجوجي,
                                     (حرف ثابت *) time_style_types,
                                     حجم (*time_style_types), EXIT_FAILURE))
            {
            حالة full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              اكسر;

            حالة long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              اكسر;

            حالة iso_time_style:
              time_format = "%Y-%m-%d";
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

      ai = argv_iter_init_stream (stdin);

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
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  إذا (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  إذا (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  إذا (opt_كدي_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  ساكن حرف *temp_argv[] = { عدم, عدم };

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
              اذهب argv_iter_done;
            حالة AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              اذهب argv_iter_done;
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
              idx_t file_لمب = argv_iter_n_حجوجي (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_لمب, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      إذا (skip_file)
        ok = false;
      وإلا
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  إذا (di_mnt)
    di_set_free (di_mnt);

  إذا (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  إذا (بغ_grand_total)
    بغ_size (&tot_dui, _("total"), false);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
