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

#include <config.h>
#include <getopt.h>
#include <sys/types.h>
#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"
#include "argv-iter.h"
#include "assure.h"
#include "di-set.h"
#include "exclude.h"
#include "fts-missing.h"
#include "human.h"
#include "mountlist.h"
#include "quote.h"
#include "show-date.h"
#include "stat-size.h"
#include "stat-time.h"
#include "stdio--.h"
#include "xfts.h"
#include "xmemdup0.h"
#include "xstrtol.h"
#include "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
static sa_nracanaa di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
static sa_nracanaa di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
static idx_t prev_level;

/* Define a class for collecting directory information. */
sa_nracanaa duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  sa_nracanaa timespec tmax;
};

/* Initialize directory data.  */
static anta_hstha void
duinfo_init (sa_nracanaa duinfo *a)
{
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
static anta_hstha void
duinfo_set (sa_nracanaa duinfo *a, uintmax_t size, sa_nracanaa timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
static anta_hstha void
duinfo_add (sa_nracanaa duinfo *a, sa_nracanaa duinfo const *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  නම් (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
sa_nracanaa dulevel
{
  /* Entries in this directory.  */
  sa_nracanaa duinfo ent;

  /* Total for subdirectories.  */
  sa_nracanaa duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
static bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
static bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
static bool opt_count_all = false;

/* If true, hash all files to look for hard links.  */
static bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
static bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
static bool quote_output;

/* If true, print a grand total at the end.  */
static bool print_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
static bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
static idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
static intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
static පූර්ණ_සංඛ්‍යා human_output_opts;

/* Output inodes count instead of blocks used.  */
static bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
static bool opt_time = false;

/* Type of time to display. controlled by --time.  */

ga_nanaa time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

static ga_nanaa time_type time_type = time_mtime;

/* User specified date / time style */
static var_na const *time_style = NULL;

/* Format used to display date / time. Controlled by --time-style */
static var_na const *time_format = NULL;

/* The local time zone rules, as per the TZ environment variable.  */
static timezone_t localtz;

/* The units to use when printing sizes.  */
static uintmax_t output_block_size;

/* File name patterns to exclude.  */
static sa_nracanaa exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
static sa_nracanaa duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#නම් GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

static sa_nracanaa option const long_options[] =
{
  {"all", no_argument, NULL, 'a'},
  {"apparent-size", no_argument, NULL, 'A'},
  {"block-size", required_argument, NULL, 'B'},
  {"bytes", no_argument, NULL, 'b'},
  {"count-links", no_argument, NULL, 'l'},
#නම් GNULIB_FTS_DEBUG
  {"-debug", no_argument, NULL, FTS_DEBUG},
#endif
  {"dereference", no_argument, NULL, 'L'},
  {"dereference-args", no_argument, NULL, 'D'},
  {"exclude", required_argument, NULL, EXCLUDE_OPTION},
  {"exclude-from", required_argument, NULL, 'X'},
  {"files0-from", required_argument, NULL, FILES0_FROM_OPTION},
  {"human-readable", no_argument, NULL, 'h'},
  {"inodes", no_argument, NULL, INODES_OPTION},
  {"si", no_argument, NULL, HUMAN_SI_OPTION},
  {"max-depth", required_argument, NULL, 'd'},
  {"null", no_argument, NULL, '0'},
  {"no-dereference", no_argument, NULL, 'P'},
  {"one-file-system", no_argument, NULL, 'x'},
  {"separate-dirs", no_argument, NULL, 'S'},
  {"summarize", no_argument, NULL, 's'},
  {"total", no_argument, NULL, 'c'},
  {"threshold", required_argument, NULL, 't'},
  {"time", optional_argument, NULL, TIME_OPTION},
  {"time-style", required_argument, NULL, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

static var_na const *const time_args[] =
{
  "atime", "access", "use", "ctime", "status", NULL
};
static ga_nanaa time_type const time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
ga_nanaa time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

static var_na const *const time_style_args[] =
{
  "full-iso", "long-iso", "iso", NULL
};
static ga_nanaa time_style const time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  නම් (status != EXIT_SUCCESS)
    emit_try_help ();
  නැතහොත්
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively සඳහා directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output line with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         write counts සඳහා all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-size\n\
         print apparent sizes rather than device usage;\n\
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
  -D, --dereference-args\n\
         dereference only symlinks that are listed on the command line\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the total සඳහා a directory (or file, with --all)\n\
         only නම් it is N or fewer levels below the command\n\
         line argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  නම් F is -, read names from standard input\n\
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
         like --block-size=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --count-links\n\
         count sizes many times නම් hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the default)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         සඳහා directories do not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total සඳහා each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE නම් positive,\n\
         or entries greater than SIZE නම් negative\n\
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
Also the TIME_STYLE environment variable sets the default style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
static bool
hash_ins (sa_nracanaa di_set *di_set, ino_t ino, dev_t dev)
{
  පූර්ණ_සංඛ්‍යා inserted = di_set_insert (di_set, dev, ino);
  නම් (inserted < 0)
    xalloc_die ();
  ආපසු inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

static void
print_only_size (uintmax_t n_bytes)
{
  var_na buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

static void
print_size (const sa_nracanaa duinfo *pdui, var_na const *string, bool quote)
{
  print_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  නම් (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      නම් (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          var_na buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  නම් (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

static void
fill_mount_table (void)
{
  sa_nracanaa mount_entry *mnt_ent = read_file_system_list (false);
  අතරතුර (mnt_ent)
    {
      sa_nracanaa mount_entry *mnt_free;
      නම් (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          sa_nracanaa stat buf;
          නම් (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          නැතහොත්
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

static bool
mount_point_in_fts_cycle (FTSENT const *ent)
{
  FTSENT const *cycle_ent = ent->fts_cycle;

  නම් (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      නම් (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  අතරතුර (ent && ent != cycle_ent)
    {
      නම් (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          ආපසු true;
        }
      ent = ent->fts_parent;
    }

  ආපසු false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

static bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  sa_nracanaa duinfo dui;
  sa_nracanaa duinfo dui_to_print;
  static idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  static sa_nracanaa dulevel *dulvl;

  var_na const *file = ent->fts_path;
  const sa_nracanaa stat *sb = ent->fts_statp;
  පූර්ණ_සංඛ්‍යා info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  නම් (ignore_missing && ignorable_fts_error (ent))
    ආපසු true;

  නම් (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  නැතහොත් නම් (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      නම් (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          නම් (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT const *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              නම් (ignore_missing && ignorable_fts_error (ent))
                ආපසු true;
            }

          නම් (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              ආපසු false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          නම් (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      නම් (excluded
          || (! opt_count_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          නම් (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT const *e = fts_read (fts);
              affirm (e == ent);
            }

          ආපසු true;
        }

      switch (info)
        {
        pak_sa FTS_D:
          ආපසු true;

        pak_sa FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          bha_nga;

        pak_sa FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          නම් (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              ආපසු false;
            }
          ආපසු true;
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
  dui_to_print = dui;

  නම් (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, parimaa_na *dulvl);
    }
  නැතහොත්
    {
      නම් (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      නැතහොත් නම් (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          නම් (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             parimaa_na *dulvl);

          සඳහා (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      නැතහොත් /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_add (&dui_to_print, &dulvl[prev_level].ent);
          නම් (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  නම් (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  නම් ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.size;
      නම් (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_size (&dui_to_print, file, quote_output);
    }

  ආපසු ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

static bool
du_files (var_na **files, පූර්ණ_සංඛ්‍යා bit_flags)
{
  bool ok = true;

  නම් (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, NULL);

      අතරතුර (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          නම් (ent == NULL)
            {
              නම් (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              bha_nga;
            }

#නම් GNULIB_FTS_DEBUG
          නම් (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      නම් (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  ආපසු ok;
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  var_na *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  var_na *files_from = NULL;

  /* Bit flags that control how fts works.  */
  පූර්ණ_සංඛ්‍යා bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  පූර්ණ_සංඛ්‍යා symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

  අතරතුර (true)
    {
      පූර්ණ_සංඛ්‍යා oi = -1;
      පූර්ණ_සංඛ්‍යා c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      නම් (c == -1)
        bha_nga;

      switch (c)
        {
#නම් GNULIB_FTS_DEBUG
        pak_sa FTS_DEBUG:
          fts_debug = true;
          bha_nga;
#endif

        pak_sa '0':
          opt_nul_terminate_output = true;
          bha_nga;

        pak_sa 'a':
          opt_all = true;
          bha_nga;

        pak_sa 'A':
          apparent_size = true;
          bha_nga;

        pak_sa 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          bha_nga;

        pak_sa 'c':
          print_grand_total = true;
          bha_nga;

        pak_sa 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          bha_nga;

        pak_sa HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          bha_nga;

        pak_sa 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          bha_nga;

        pak_sa 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            නම් (xstrtoimax (optarg, NULL, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            නැතහොත්
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          bha_nga;

        pak_sa 'm':
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          bha_nga;

        pak_sa 'l':
          opt_count_all = true;
          bha_nga;

        pak_sa 's':
          opt_summarize_only = true;
          bha_nga;

        pak_sa 't':
          {
            ga_nanaa strtol_error e;
            e = xstrtoimax (optarg, NULL, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            නම් (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            නම් (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          bha_nga;

        pak_sa 'x':
          bit_flags |= FTS_XDEV;
          bha_nga;

        pak_sa 'B':
          {
            ga_nanaa strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            නම් (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          bha_nga;

        pak_sa 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        pak_sa 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          bha_nga;

        pak_sa 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          bha_nga;

        pak_sa 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          bha_nga;

        pak_sa 'S':
          opt_separate_dirs = true;
          bha_nga;

        pak_sa 'X':
          නම් (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          bha_nga;

        pak_sa FILES0_FROM_OPTION:
          files_from = optarg;
          bha_nga;

        pak_sa EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          bha_nga;

        pak_sa INODES_OPTION:
          opt_inodes = true;
          bha_nga;

        pak_sa TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          bha_nga;

        pak_sa TIME_STYLE_OPTION:
          time_style = optarg;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          ok = false;
        }
    }

  නම් (!ok)
    usage (EXIT_FAILURE);

  නම් (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      පූර්ණ_සංඛ්‍යා qs = getenv_quoting_style ();
      නම් (qs < 0)
        qs = shell_escape_quoting_style;
      නම් (qs != literal_quoting_style)
        {
          set_quoting_style (NULL, qs);
          quote_output = true;
        }
    }

  නම් (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  නම් (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  නම් (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  නම් (opt_summarize_only)
    max_depth = 0;

  නම් (opt_inodes)
    {
      නම් (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  නම් (opt_time)
    {
      නම් (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          නම් (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          නැතහොත් නම් (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              var_na const *p = strchr (time_style, '\n');
              නම් (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          නැතහොත්
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              static var_na const posix_prefix[] = "posix-";
              static const size_t prefix_len = parimaa_na posix_prefix - 1;
              අතරතුර (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      නම් (*time_style == '+')
        time_format = time_style + 1;
      නැතහොත්
        {
          switch (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (var_na const *) time_style_types,
                                     parimaa_na (*time_style_types), EXIT_FAILURE))
            {
            pak_sa full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              bha_nga;

            pak_sa long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              bha_nga;

            pak_sa iso_time_style:
              time_format = "%Y-%m-%d";
              bha_nga;
            }
        }
    }

  sa_nracanaa argv_iterator *ai;
  නම් (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      නම් (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      නම් (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  නැතහොත්
    {
      var_na **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  නම් (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  නම් (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  නම් (opt_count_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  static var_na *temp_argv[] = { NULL, NULL };

  අතරතුර (true)
    {
      bool skip_file = false;
      ga_nanaa argv_iter_err ai_err;
      var_na *file_name = argv_iter (ai, &ai_err);
      නම් (!file_name)
        {
          switch (ai_err)
            {
            pak_sa AI_ERR_EOF:
              goto argv_iter_done;
            pak_sa AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              goto argv_iter_done;
            pak_sa AI_ERR_MEM:
              xalloc_die ();
            pak_sa AI_ERR_OK: default:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      නම් (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      නම් (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          නම් (files_from == NULL)
            error (0, 0, "%s", _("invalid zero-length file name"));
          නැතහොත්
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_number = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_number, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      නම් (skip_file)
        ok = false;
      නැතහොත්
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  නම් (di_mnt)
    di_set_free (di_mnt);

  නම් (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  නම් (print_grand_total)
    print_size (&tot_dui, _("total"), false);

  ආපසු ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
