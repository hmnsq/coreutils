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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argv-iter.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "di-set.h"
#અન્તર્ભાવ "exclude.h"
#અન્તર્ભાવ "fts-missing.h"
#અન્તર્ભાવ "human.h"
#અન્તર્ભાવ "mountlist.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "show-date.h"
#અન્તર્ભાવ "stat-size.h"
#અન્તર્ભાવ "stat-time.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xfts.h"
#અન્તર્ભાવ "xmemdup0.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
સ્થિર સંરચના di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
સ્થિર સંરચના di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
સ્થિર idx_t prev_level;

/* Define a class for collecting directory information. */
સંરચના duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  સંરચના timespec tmax;
};

/* Initialize directory data.  */
સ્થિર અંતઃરેખ શૂન્ય
duinfo_init (સંરચના duinfo *a)
{
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
સ્થિર અંતઃરેખ શૂન્ય
duinfo_set (સંરચના duinfo *a, uintmax_t size, સંરચના timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
સ્થિર અંતઃરેખ શૂન્ય
duinfo_add (સંરચના duinfo *a, સંરચના duinfo અચલ *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  જો (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
સંરચના dulevel
{
  /* Entries in this directory.  */
  સંરચના duinfo ent;

  /* Total for subdirectories.  */
  સંરચના duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
સ્થિર bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
સ્થિર bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
સ્થિર bool opt_ગણતરી_all = false;

/* If true, hash all files to look for hard links.  */
સ્થિર bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
સ્થિર bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
સ્થિર bool quote_output;

/* If true, print a grand total at the end.  */
સ્થિર bool print_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
સ્થિર bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
સ્થિર idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
સ્થિર intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
સ્થિર પૂર્ણાંક human_output_opts;

/* Output inodes count instead of blocks used.  */
સ્થિર bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
સ્થિર bool opt_time = false;

/* Type of time to display. controlled by --time.  */

ગણના time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

સ્થિર ગણના time_type time_type = time_mtime;

/* User specified date / time style */
સ્થિર અક્ષર અચલ *time_style = ખાલી;

/* Format used to display date / time. Controlled by --time-style */
સ્થિર અક્ષર અચલ *time_format = ખાલી;

/* The local time zone rules, as per the TZ environment variable.  */
સ્થિર timezone_t localtz;

/* The units to use when printing sizes.  */
સ્થિર uintmax_t output_block_size;

/* File name patterns to exclude.  */
સ્થિર સંરચના exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
સ્થિર સંરચના duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#જો GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"all", no_argument, ખાલી, 'a'},
  {"apparent-size", no_argument, ખાલી, 'A'},
  {"block-size", required_argument, ખાલી, 'B'},
  {"bytes", no_argument, ખાલી, 'b'},
  {"count-links", no_argument, ખાલી, 'l'},
#જો GNULIB_FTS_DEBUG
  {"-debug", no_argument, ખાલી, FTS_DEBUG},
#endif
  {"dereference", no_argument, ખાલી, 'L'},
  {"dereference-args", no_argument, ખાલી, 'D'},
  {"exclude", required_argument, ખાલી, EXCLUDE_OPTION},
  {"exclude-from", required_argument, ખાલી, 'X'},
  {"files0-from", required_argument, ખાલી, FILES0_FROM_OPTION},
  {"human-readable", no_argument, ખાલી, 'h'},
  {"inodes", no_argument, ખાલી, INODES_OPTION},
  {"si", no_argument, ખાલી, HUMAN_SI_OPTION},
  {"max-depth", required_argument, ખાલી, 'd'},
  {"null", no_argument, ખાલી, '0'},
  {"no-dereference", no_argument, ખાલી, 'P'},
  {"one-file-system", no_argument, ખાલી, 'x'},
  {"separate-dirs", no_argument, ખાલી, 'S'},
  {"summarize", no_argument, ખાલી, 's'},
  {"total", no_argument, ખાલી, 'c'},
  {"threshold", required_argument, ખાલી, 't'},
  {"time", optional_argument, ખાલી, TIME_OPTION},
  {"time-style", required_argument, ખાલી, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

સ્થિર અક્ષર અચલ *અચલ time_args[] =
{
  "atime", "access", "use", "ctime", "status", ખાલી
};
સ્થિર ગણના time_type અચલ time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
ગણના time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

સ્થિર અક્ષર અચલ *અચલ time_style_args[] =
{
  "full-iso", "long-iso", "iso", ખાલી
};
સ્થિર ગણના time_style અચલ time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively માટે directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output લીટી with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         લિખ counts માટે all files, not just directories\n\
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
         dereference only symlinks that are listed on the command લીટી\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the total માટે a directory (or file, with --all)\n\
         only જો it is N or fewer levels below the command\n\
         લીટી argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  જો F is -, પઠ names from standard આગત\n\
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
  -l, --ગણતરી-links\n\
         ગણતરી sizes many times જો hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the મૂળભૂત)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         માટે directories કરો not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total માટે each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE જો positive,\n\
         or entries greater than SIZE જો negative\n\
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
Also the TIME_STYLE environment variable sets the મૂળભૂત style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
સ્થિર bool
hash_ins (સંરચના di_set *di_set, ino_t ino, dev_t dev)
{
  પૂર્ણાંક inserted = di_set_ઉમેરો (di_set, dev, ino);
  જો (inserted < 0)
    xalloc_die ();
  પાછા inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

સ્થિર શૂન્ય
print_only_size (uintmax_t n_bytes)
{
  અક્ષર buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

સ્થિર શૂન્ય
print_size (અચલ સંરચના duinfo *pdui, અક્ષર અચલ *string, bool quote)
{
  print_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  જો (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      જો (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          અક્ષર buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  જો (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

સ્થિર શૂન્ય
fill_mount_table (શૂન્ય)
{
  સંરચના mount_entry *mnt_ent = read_file_system_list (false);
  જ્યારે (mnt_ent)
    {
      સંરચના mount_entry *mnt_free;
      જો (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          સંરચના stat buf;
          જો (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          નહીંતર
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

સ્થિર bool
mount_point_in_fts_cycle (FTSENT અચલ *ent)
{
  FTSENT અચલ *cycle_ent = ent->fts_cycle;

  જો (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      જો (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  જ્યારે (ent && ent != cycle_ent)
    {
      જો (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          પાછા true;
        }
      ent = ent->fts_parent;
    }

  પાછા false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

સ્થિર bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  સંરચના duinfo dui;
  સંરચના duinfo dui_to_print;
  સ્થિર idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  સ્થિર સંરચના dulevel *dulvl;

  અક્ષર અચલ *file = ent->fts_path;
  અચલ સંરચના stat *sb = ent->fts_statp;
  પૂર્ણાંક info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  જો (ignore_missing && ignorable_fts_error (ent))
    પાછા true;

  જો (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  નહીંતર જો (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      જો (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          જો (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT અચલ *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              જો (ignore_missing && ignorable_fts_error (ent))
                પાછા true;
            }

          જો (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              પાછા false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          જો (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      જો (excluded
          || (! opt_ગણતરી_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          જો (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT અચલ *e = fts_read (fts);
              affirm (e == ent);
            }

          પાછા true;
        }

      પસંદગી (info)
        {
        સ્થિતિ FTS_D:
          પાછા true;

        સ્થિતિ FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          તોડો;

        સ્થિતિ FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          જો (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              પાછા false;
            }
          પાછા true;
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

  જો (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, કદ *dulvl);
    }
  નહીંતર
    {
      જો (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      નહીંતર જો (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          જો (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             કદ *dulvl);

          માટે (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      નહીંતર /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_add (&dui_to_print, &dulvl[prev_level].ent);
          જો (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  જો (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  જો ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.size;
      જો (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_size (&dui_to_print, file, quote_output);
    }

  પાછા ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

સ્થિર bool
du_files (અક્ષર **files, પૂર્ણાંક bit_flags)
{
  bool ok = true;

  જો (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, ખાલી);

      જ્યારે (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          જો (ent == ખાલી)
            {
              જો (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              તોડો;
            }

#જો GNULIB_FTS_DEBUG
          જો (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      જો (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  પાછા ok;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  અક્ષર *files_from = ખાલી;

  /* Bit flags that control how fts works.  */
  પૂર્ણાંક bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  પૂર્ણાંક symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

  જ્યારે (true)
    {
      પૂર્ણાંક oi = -1;
      પૂર્ણાંક c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
#જો GNULIB_FTS_DEBUG
        સ્થિતિ FTS_DEBUG:
          fts_debug = true;
          તોડો;
#endif

        સ્થિતિ '0':
          opt_nul_terminate_output = true;
          તોડો;

        સ્થિતિ 'a':
          opt_all = true;
          તોડો;

        સ્થિતિ 'A':
          apparent_size = true;
          તોડો;

        સ્થિતિ 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          તોડો;

        સ્થિતિ 'c':
          print_grand_total = true;
          તોડો;

        સ્થિતિ 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          તોડો;

        સ્થિતિ HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          તોડો;

        સ્થિતિ 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          તોડો;

        સ્થિતિ 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            જો (xstrtoimax (optarg, ખાલી, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            નહીંતર
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          તોડો;

        સ્થિતિ 'm':
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          તોડો;

        સ્થિતિ 'l':
          opt_ગણતરી_all = true;
          તોડો;

        સ્થિતિ 's':
          opt_summarize_only = true;
          તોડો;

        સ્થિતિ 't':
          {
            ગણના strtol_error e;
            e = xstrtoimax (optarg, ખાલી, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            જો (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            જો (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          તોડો;

        સ્થિતિ 'x':
          bit_flags |= FTS_XDEV;
          તોડો;

        સ્થિતિ 'B':
          {
            ગણના strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            જો (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          તોડો;

        સ્થિતિ 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        સ્થિતિ 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          તોડો;

        સ્થિતિ 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          તોડો;

        સ્થિતિ 'S':
          opt_separate_dirs = true;
          તોડો;

        સ્થિતિ 'X':
          જો (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          તોડો;

        સ્થિતિ FILES0_FROM_OPTION:
          files_from = optarg;
          તોડો;

        સ્થિતિ EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          તોડો;

        સ્થિતિ INODES_OPTION:
          opt_inodes = true;
          તોડો;

        સ્થિતિ TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          તોડો;

        સ્થિતિ TIME_STYLE_OPTION:
          time_style = optarg;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          ok = false;
        }
    }

  જો (!ok)
    usage (EXIT_FAILURE);

  જો (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      પૂર્ણાંક qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
      જો (qs != literal_quoting_style)
        {
          set_quoting_style (ખાલી, qs);
          quote_output = true;
        }
    }

  જો (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  જો (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  જો (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  જો (opt_summarize_only)
    max_depth = 0;

  જો (opt_inodes)
    {
      જો (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  જો (opt_time)
    {
      જો (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          જો (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          નહીંતર જો (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              અક્ષર અચલ *p = strchr (time_style, '\n');
              જો (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          નહીંતર
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              સ્થિર અક્ષર અચલ posix_prefix[] = "posix-";
              સ્થિર અચલ size_t prefix_len = કદ posix_prefix - 1;
              જ્યારે (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      જો (*time_style == '+')
        time_format = time_style + 1;
      નહીંતર
        {
          પસંદગી (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (અક્ષર અચલ *) time_style_types,
                                     કદ (*time_style_types), EXIT_FAILURE))
            {
            સ્થિતિ full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              તોડો;

            સ્થિતિ long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              તોડો;

            સ્થિતિ iso_time_style:
              time_format = "%Y-%m-%d";
              તોડો;
            }
        }
    }

  સંરચના argv_iterator *ai;
  જો (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      જો (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      જો (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  નહીંતર
    {
      અક્ષર **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  જો (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  જો (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  જો (opt_ગણતરી_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  સ્થિર અક્ષર *temp_argv[] = { ખાલી, ખાલી };

  જ્યારે (true)
    {
      bool skip_file = false;
      ગણના argv_iter_err ai_err;
      અક્ષર *file_name = argv_iter (ai, &ai_err);
      જો (!file_name)
        {
          પસંદગી (ai_err)
            {
            સ્થિતિ AI_ERR_EOF:
              જાઓ argv_iter_done;
            સ્થિતિ AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              જાઓ argv_iter_done;
            સ્થિતિ AI_ERR_MEM:
              xalloc_die ();
            સ્થિતિ AI_ERR_OK: મૂળભૂત:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      જો (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      જો (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          જો (files_from == ખાલી)
            error (0, 0, "%s", _("invalid zero-length file name"));
          નહીંતર
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_સંખ્યા = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_સંખ્યા, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      જો (skip_file)
        ok = false;
      નહીંતર
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  જો (di_mnt)
    di_set_free (di_mnt);

  જો (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  જો (print_grand_total)
    print_size (&tot_dui, _("total"), false);

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
