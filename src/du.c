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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argv-iter.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "di-set.h"
#அந்தர்பாவ "exclude.h"
#அந்தர்பாவ "fts-missing.h"
#அந்தர்பாவ "human.h"
#அந்தர்பாவ "mountlist.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "show-date.h"
#அந்தர்பாவ "stat-size.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xfts.h"
#அந்தர்பாவ "xmemdup0.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
நிலையான கட்டமைப்பு di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
நிலையான கட்டமைப்பு di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
நிலையான idx_t prev_level;

/* Define a class for collecting directory information. */
கட்டமைப்பு duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  கட்டமைப்பு timespec tmax;
};

/* Initialize directory data.  */
நிலையான உள்வரி வெறுமை
duinfo_init (கட்டமைப்பு duinfo *a)
{
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
நிலையான உள்வரி வெறுமை
duinfo_set (கட்டமைப்பு duinfo *a, uintmax_t size, கட்டமைப்பு timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
நிலையான உள்வரி வெறுமை
duinfo_add (கட்டமைப்பு duinfo *a, கட்டமைப்பு duinfo மாறிலி *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  எனில் (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
கட்டமைப்பு dulevel
{
  /* Entries in this directory.  */
  கட்டமைப்பு duinfo ent;

  /* Total for subdirectories.  */
  கட்டமைப்பு duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
நிலையான bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
நிலையான bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
நிலையான bool opt_மொத்தம்_all = false;

/* If true, hash all files to look for hard links.  */
நிலையான bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
நிலையான bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
நிலையான bool quote_output;

/* If true, print a grand total at the end.  */
நிலையான bool print_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
நிலையான bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
நிலையான idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
நிலையான intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
நிலையான முழு human_output_opts;

/* Output inodes count instead of blocks used.  */
நிலையான bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
நிலையான bool opt_time = false;

/* Type of time to display. controlled by --time.  */

எண்ணிக்கை time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

நிலையான எண்ணிக்கை time_type time_type = time_mtime;

/* User specified date / time style */
நிலையான எழுத்து மாறிலி *time_style = வெற்று;

/* Format used to display date / time. Controlled by --time-style */
நிலையான எழுத்து மாறிலி *time_format = வெற்று;

/* The local time zone rules, as per the TZ environment variable.  */
நிலையான timezone_t localtz;

/* The units to use when printing sizes.  */
நிலையான uintmax_t output_block_size;

/* File name patterns to exclude.  */
நிலையான கட்டமைப்பு exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
நிலையான கட்டமைப்பு duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#எனில் GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"all", no_argument, வெற்று, 'a'},
  {"apparent-size", no_argument, வெற்று, 'A'},
  {"block-size", required_argument, வெற்று, 'B'},
  {"bytes", no_argument, வெற்று, 'b'},
  {"count-links", no_argument, வெற்று, 'l'},
#எனில் GNULIB_FTS_DEBUG
  {"-debug", no_argument, வெற்று, FTS_DEBUG},
#endif
  {"dereference", no_argument, வெற்று, 'L'},
  {"dereference-args", no_argument, வெற்று, 'D'},
  {"exclude", required_argument, வெற்று, EXCLUDE_OPTION},
  {"exclude-from", required_argument, வெற்று, 'X'},
  {"files0-from", required_argument, வெற்று, FILES0_FROM_OPTION},
  {"human-readable", no_argument, வெற்று, 'h'},
  {"inodes", no_argument, வெற்று, INODES_OPTION},
  {"si", no_argument, வெற்று, HUMAN_SI_OPTION},
  {"max-depth", required_argument, வெற்று, 'd'},
  {"null", no_argument, வெற்று, '0'},
  {"no-dereference", no_argument, வெற்று, 'P'},
  {"one-file-system", no_argument, வெற்று, 'x'},
  {"separate-dirs", no_argument, வெற்று, 'S'},
  {"summarize", no_argument, வெற்று, 's'},
  {"total", no_argument, வெற்று, 'c'},
  {"threshold", required_argument, வெற்று, 't'},
  {"time", optional_argument, வெற்று, TIME_OPTION},
  {"time-style", required_argument, வெற்று, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

நிலையான எழுத்து மாறிலி *மாறிலி time_args[] =
{
  "atime", "access", "use", "ctime", "status", வெற்று
};
நிலையான எண்ணிக்கை time_type மாறிலி time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
எண்ணிக்கை time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

நிலையான எழுத்து மாறிலி *மாறிலி time_style_args[] =
{
  "full-iso", "long-iso", "iso", வெற்று
};
நிலையான எண்ணிக்கை time_style மாறிலி time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively க்கு directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output வரி with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         லிக counts க்கு all files, not just directories\n\
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
         dereference only symlinks that are listed on the command வரி\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the total க்கு a directory (or file, with --all)\n\
         only எனில் it is N or fewer levels below the command\n\
         வரி argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  எனில் F is -, பட names from standard உள்ளீடு\n\
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
  -l, --மொத்தம்-links\n\
         மொத்தம் sizes many times எனில் hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the இயல்பு)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         க்கு directories செய் not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total க்கு each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE எனில் positive,\n\
         or entries greater than SIZE எனில் negative\n\
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
Also the TIME_STYLE environment variable sets the இயல்பு style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
நிலையான bool
hash_ins (கட்டமைப்பு di_set *di_set, ino_t ino, dev_t dev)
{
  முழு inserted = di_set_செருகு (di_set, dev, ino);
  எனில் (inserted < 0)
    xalloc_die ();
  திரும்பு inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

நிலையான வெறுமை
print_only_size (uintmax_t n_bytes)
{
  எழுத்து buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

நிலையான வெறுமை
print_size (மாறிலி கட்டமைப்பு duinfo *pdui, எழுத்து மாறிலி *string, bool quote)
{
  print_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  எனில் (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      எனில் (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          எழுத்து buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  எனில் (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

நிலையான வெறுமை
fill_mount_table (வெறுமை)
{
  கட்டமைப்பு mount_entry *mnt_ent = read_file_system_list (false);
  போது (mnt_ent)
    {
      கட்டமைப்பு mount_entry *mnt_free;
      எனில் (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          கட்டமைப்பு stat buf;
          எனில் (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          இல்லையெனில்
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

நிலையான bool
mount_point_in_fts_cycle (FTSENT மாறிலி *ent)
{
  FTSENT மாறிலி *cycle_ent = ent->fts_cycle;

  எனில் (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      எனில் (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  போது (ent && ent != cycle_ent)
    {
      எனில் (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          திரும்பு true;
        }
      ent = ent->fts_parent;
    }

  திரும்பு false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

நிலையான bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  கட்டமைப்பு duinfo dui;
  கட்டமைப்பு duinfo dui_to_print;
  நிலையான idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  நிலையான கட்டமைப்பு dulevel *dulvl;

  எழுத்து மாறிலி *file = ent->fts_path;
  மாறிலி கட்டமைப்பு stat *sb = ent->fts_statp;
  முழு info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  எனில் (ignore_missing && ignorable_fts_error (ent))
    திரும்பு true;

  எனில் (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  இல்லையெனில் எனில் (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      எனில் (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          எனில் (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT மாறிலி *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              எனில் (ignore_missing && ignorable_fts_error (ent))
                திரும்பு true;
            }

          எனில் (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              திரும்பு false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          எனில் (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      எனில் (excluded
          || (! opt_மொத்தம்_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          எனில் (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT மாறிலி *e = fts_read (fts);
              affirm (e == ent);
            }

          திரும்பு true;
        }

      தேர்வு (info)
        {
        நிலை FTS_D:
          திரும்பு true;

        நிலை FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          நிறுத்து;

        நிலை FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          எனில் (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              திரும்பு false;
            }
          திரும்பு true;
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

  எனில் (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, அளவு *dulvl);
    }
  இல்லையெனில்
    {
      எனில் (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      இல்லையெனில் எனில் (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          எனில் (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             அளவு *dulvl);

          க்கு (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      இல்லையெனில் /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_add (&dui_to_print, &dulvl[prev_level].ent);
          எனில் (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  எனில் (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  எனில் ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.size;
      எனில் (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_size (&dui_to_print, file, quote_output);
    }

  திரும்பு ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

நிலையான bool
du_files (எழுத்து **files, முழு bit_flags)
{
  bool ok = true;

  எனில் (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, வெற்று);

      போது (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          எனில் (ent == வெற்று)
            {
              எனில் (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              நிறுத்து;
            }

#எனில் GNULIB_FTS_DEBUG
          எனில் (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      எனில் (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  திரும்பு ok;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  எழுத்து *files_from = வெற்று;

  /* Bit flags that control how fts works.  */
  முழு bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  முழு symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

  போது (true)
    {
      முழு oi = -1;
      முழு c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
#எனில் GNULIB_FTS_DEBUG
        நிலை FTS_DEBUG:
          fts_debug = true;
          நிறுத்து;
#endif

        நிலை '0':
          opt_nul_terminate_output = true;
          நிறுத்து;

        நிலை 'a':
          opt_all = true;
          நிறுத்து;

        நிலை 'A':
          apparent_size = true;
          நிறுத்து;

        நிலை 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          நிறுத்து;

        நிலை 'c':
          print_grand_total = true;
          நிறுத்து;

        நிலை 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          நிறுத்து;

        நிலை HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          நிறுத்து;

        நிலை 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          நிறுத்து;

        நிலை 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            எனில் (xstrtoimax (optarg, வெற்று, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            இல்லையெனில்
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          நிறுத்து;

        நிலை 'm':
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          நிறுத்து;

        நிலை 'l':
          opt_மொத்தம்_all = true;
          நிறுத்து;

        நிலை 's':
          opt_summarize_only = true;
          நிறுத்து;

        நிலை 't':
          {
            எண்ணிக்கை strtol_error e;
            e = xstrtoimax (optarg, வெற்று, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            எனில் (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            எனில் (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          நிறுத்து;

        நிலை 'x':
          bit_flags |= FTS_XDEV;
          நிறுத்து;

        நிலை 'B':
          {
            எண்ணிக்கை strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            எனில் (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          நிறுத்து;

        நிலை 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        நிலை 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          நிறுத்து;

        நிலை 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          நிறுத்து;

        நிலை 'S':
          opt_separate_dirs = true;
          நிறுத்து;

        நிலை 'X':
          எனில் (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          நிறுத்து;

        நிலை FILES0_FROM_OPTION:
          files_from = optarg;
          நிறுத்து;

        நிலை EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          நிறுத்து;

        நிலை INODES_OPTION:
          opt_inodes = true;
          நிறுத்து;

        நிலை TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          நிறுத்து;

        நிலை TIME_STYLE_OPTION:
          time_style = optarg;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          ok = false;
        }
    }

  எனில் (!ok)
    usage (EXIT_FAILURE);

  எனில் (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      முழு qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
      எனில் (qs != literal_quoting_style)
        {
          set_quoting_style (வெற்று, qs);
          quote_output = true;
        }
    }

  எனில் (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  எனில் (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  எனில் (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  எனில் (opt_summarize_only)
    max_depth = 0;

  எனில் (opt_inodes)
    {
      எனில் (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  எனில் (opt_time)
    {
      எனில் (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          எனில் (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          இல்லையெனில் எனில் (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              எழுத்து மாறிலி *p = strchr (time_style, '\n');
              எனில் (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          இல்லையெனில்
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              நிலையான எழுத்து மாறிலி posix_prefix[] = "posix-";
              நிலையான மாறிலி size_t prefix_len = அளவு posix_prefix - 1;
              போது (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      எனில் (*time_style == '+')
        time_format = time_style + 1;
      இல்லையெனில்
        {
          தேர்வு (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (எழுத்து மாறிலி *) time_style_types,
                                     அளவு (*time_style_types), EXIT_FAILURE))
            {
            நிலை full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              நிறுத்து;

            நிலை long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              நிறுத்து;

            நிலை iso_time_style:
              time_format = "%Y-%m-%d";
              நிறுத்து;
            }
        }
    }

  கட்டமைப்பு argv_iterator *ai;
  எனில் (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      எனில் (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      எனில் (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  இல்லையெனில்
    {
      எழுத்து **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  எனில் (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  எனில் (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  எனில் (opt_மொத்தம்_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  நிலையான எழுத்து *temp_argv[] = { வெற்று, வெற்று };

  போது (true)
    {
      bool skip_file = false;
      எண்ணிக்கை argv_iter_err ai_err;
      எழுத்து *file_name = argv_iter (ai, &ai_err);
      எனில் (!file_name)
        {
          தேர்வு (ai_err)
            {
            நிலை AI_ERR_EOF:
              செல் argv_iter_done;
            நிலை AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              செல் argv_iter_done;
            நிலை AI_ERR_MEM:
              xalloc_die ();
            நிலை AI_ERR_OK: இயல்பு:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      எனில் (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      எனில் (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          எனில் (files_from == வெற்று)
            error (0, 0, "%s", _("invalid zero-length file name"));
          இல்லையெனில்
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_எண் = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_எண், _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      எனில் (skip_file)
        ok = false;
      இல்லையெனில்
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  எனில் (di_mnt)
    di_set_free (di_mnt);

  எனில் (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  எனில் (print_grand_total)
    print_size (&tot_dui, _("total"), false);

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
