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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argv-iter.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "di-set.h"
#ଅନ୍ତର୍ଭାଵ "exclude.h"
#ଅନ୍ତର୍ଭାଵ "fts-missing.h"
#ଅନ୍ତର୍ଭାଵ "human.h"
#ଅନ୍ତର୍ଭାଵ "mountlist.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "show-date.h"
#ଅନ୍ତର୍ଭାଵ "stat-size.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "xfts.h"
#ଅନ୍ତର୍ଭାଵ "xmemdup0.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ସ୍ଥିର ସଂରଚନା di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ସ୍ଥିର ସଂରଚନା di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ସ୍ଥିର idx_t prev_level;

/* Define a class for collecting directory information. */
ସଂରଚନା duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  ସଂରଚନା timespec tmax;
};

/* Initialize directory data.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
duinfo_init (ସଂରଚନା duinfo *a)
{
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
duinfo_set (ସଂରଚନା duinfo *a, uintmax_t size, ସଂରଚନା timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
duinfo_add (ସଂରଚନା duinfo *a, ସଂରଚନା duinfo ଅଚଳ *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  ଯଦି (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
ସଂରଚନା dulevel
{
  /* Entries in this directory.  */
  ସଂରଚନା duinfo ent;

  /* Total for subdirectories.  */
  ସଂରଚନା duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
ସ୍ଥିର bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
ସ୍ଥିର bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
ସ୍ଥିର bool opt_ଗଣନା_all = false;

/* If true, hash all files to look for hard links.  */
ସ୍ଥିର bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ସ୍ଥିର bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ସ୍ଥିର bool quote_output;

/* If true, print a grand total at the end.  */
ସ୍ଥିର bool print_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
ସ୍ଥିର bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ସ୍ଥିର idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ସ୍ଥିର intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା human_output_opts;

/* Output inodes count instead of blocks used.  */
ସ୍ଥିର bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ସ୍ଥିର bool opt_time = false;

/* Type of time to display. controlled by --time.  */

ଗଣନା time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

ସ୍ଥିର ଗଣନା time_type time_type = time_mtime;

/* User specified date / time style */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *time_style = ଖାଲି;

/* Format used to display date / time. Controlled by --time-style */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *time_format = ଖାଲି;

/* The local time zone rules, as per the TZ environment variable.  */
ସ୍ଥିର timezone_t localtz;

/* The units to use when printing sizes.  */
ସ୍ଥିର uintmax_t output_block_size;

/* File name patterns to exclude.  */
ସ୍ଥିର ସଂରଚନା exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ସ୍ଥିର ସଂରଚନା duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#ଯଦି GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"all", no_argument, ଖାଲି, 'a'},
  {"apparent-size", no_argument, ଖାଲି, 'A'},
  {"block-size", required_argument, ଖାଲି, 'B'},
  {"bytes", no_argument, ଖାଲି, 'b'},
  {"count-links", no_argument, ଖାଲି, 'l'},
#ଯଦି GNULIB_FTS_DEBUG
  {"-debug", no_argument, ଖାଲି, FTS_DEBUG},
#endif
  {"dereference", no_argument, ଖାଲି, 'L'},
  {"dereference-args", no_argument, ଖାଲି, 'D'},
  {"exclude", required_argument, ଖାଲି, EXCLUDE_OPTION},
  {"exclude-from", required_argument, ଖାଲି, 'X'},
  {"files0-from", required_argument, ଖାଲି, FILES0_FROM_OPTION},
  {"human-readable", no_argument, ଖାଲି, 'h'},
  {"inodes", no_argument, ଖାଲି, INODES_OPTION},
  {"si", no_argument, ଖାଲି, HUMAN_SI_OPTION},
  {"max-depth", required_argument, ଖାଲି, 'd'},
  {"null", no_argument, ଖାଲି, '0'},
  {"no-dereference", no_argument, ଖାଲି, 'P'},
  {"one-file-system", no_argument, ଖାଲି, 'x'},
  {"separate-dirs", no_argument, ଖାଲି, 'S'},
  {"summarize", no_argument, ଖାଲି, 's'},
  {"total", no_argument, ଖାଲି, 'c'},
  {"threshold", required_argument, ଖାଲି, 't'},
  {"time", optional_argument, ଖାଲି, TIME_OPTION},
  {"time-style", required_argument, ଖାଲି, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ time_args[] =
{
  "atime", "access", "use", "ctime", "status", ଖାଲି
};
ସ୍ଥିର ଗଣନା time_type ଅଚଳ time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
ଗଣନା time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ time_style_args[] =
{
  "full-iso", "long-iso", "iso", ଖାଲି
};
ସ୍ଥିର ଗଣନା time_style ଅଚଳ time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      fputs (_("\
Summarize device usage of the set of FILEs, recursively ପାଇଁ directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output ଧାଡ଼ି with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         ଲିଖ counts ପାଇଁ all files, not just directories\n\
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
         dereference only symlinks that are listed on the command ଧାଡ଼ି\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the total ପାଇଁ a directory (or file, with --all)\n\
         only ଯଦି it is N or fewer levels below the command\n\
         ଧାଡ଼ି argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  ଯଦି F is -, ପଠ names from standard ନିବେଶ\n\
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
  -l, --ଗଣନା_2-links\n\
         ଗଣନା_2 sizes many times ଯଦି hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         ପାଇଁ directories କର not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total ପାଇଁ each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE ଯଦି positive,\n\
         or entries greater than SIZE ଯଦି negative\n\
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
Also the TIME_STYLE environment variable sets the ପୂର୍ବନିର୍ଦ୍ଧାରିତ style to use.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ସ୍ଥିର bool
hash_ins (ସଂରଚନା di_set *di_set, ino_t ino, dev_t dev)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା inserted = di_set_ଭର୍ତ୍ତି (di_set, dev, ino);
  ଯଦି (inserted < 0)
    xalloc_die ();
  ଫେରନ୍ତୁ inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_only_size (uintmax_t n_bytes)
{
  ଅକ୍ଷର buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_size (ଅଚଳ ସଂରଚନା duinfo *pdui, ଅକ୍ଷର ଅଚଳ *string, bool quote)
{
  print_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  ଯଦି (opt_time)
    {
      putchar ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      ଯଦି (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  putchar ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  putchar (opt_nul_terminate_output ? '\0' : '\n');
  ଯଦି (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ସ୍ଥିର ଶୂନ୍ୟ
fill_mount_table (ଶୂନ୍ୟ)
{
  ସଂରଚନା mount_entry *mnt_ent = read_file_system_list (false);
  ଯେତେବେଳେ (mnt_ent)
    {
      ସଂରଚନା mount_entry *mnt_free;
      ଯଦି (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          ସଂରଚନା stat buf;
          ଯଦି (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          ଅନ୍ୟଥା
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

ସ୍ଥିର bool
mount_point_in_fts_cycle (FTSENT ଅଚଳ *ent)
{
  FTSENT ଅଚଳ *cycle_ent = ent->fts_cycle;

  ଯଦି (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      ଯଦି (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  ଯେତେବେଳେ (ent && ent != cycle_ent)
    {
      ଯଦି (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          ଫେରନ୍ତୁ true;
        }
      ent = ent->fts_parent;
    }

  ଫେରନ୍ତୁ false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

ସ୍ଥିର bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  ସଂରଚନା duinfo dui;
  ସଂରଚନା duinfo dui_to_print;
  ସ୍ଥିର idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  ସ୍ଥିର ସଂରଚନା dulevel *dulvl;

  ଅକ୍ଷର ଅଚଳ *file = ent->fts_path;
  ଅଚଳ ସଂରଚନା stat *sb = ent->fts_statp;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  ଯଦି (ignore_missing && ignorable_fts_error (ent))
    ଫେରନ୍ତୁ true;

  ଯଦି (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  ଅନ୍ୟଥା ଯଦି (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      ଯଦି (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          ଯଦି (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT ଅଚଳ *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              ଯଦି (ignore_missing && ignorable_fts_error (ent))
                ଫେରନ୍ତୁ true;
            }

          ଯଦି (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              ଫେରନ୍ତୁ false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          ଯଦି (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      ଯଦି (excluded
          || (! opt_ଗଣନା_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          ଯଦି (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT ଅଚଳ *e = fts_read (fts);
              affirm (e == ent);
            }

          ଫେରନ୍ତୁ true;
        }

      ବାଛ (info)
        {
        ସ୍ଥିତି FTS_D:
          ଫେରନ୍ତୁ true;

        ସ୍ଥିତି FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          ଯଦି (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              ଫେରନ୍ତୁ false;
            }
          ଫେରନ୍ତୁ true;
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

  ଯଦି (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, ଆକାର *dulvl);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      ଅନ୍ୟଥା ଯଦି (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          ଯଦି (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             ଆକାର *dulvl);

          ପାଇଁ (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      ଅନ୍ୟଥା /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_add (&dui_to_print, &dulvl[prev_level].ent);
          ଯଦି (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  ଯଦି (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  ଯଦି ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.size;
      ଯଦି (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_size (&dui_to_print, file, quote_output);
    }

  ଫେରନ୍ତୁ ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ସ୍ଥିର bool
du_files (ଅକ୍ଷର **files, ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags)
{
  bool ok = true;

  ଯଦି (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, ଖାଲି);

      ଯେତେବେଳେ (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ଯଦି (ent == ଖାଲି)
            {
              ଯଦି (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              ଭାଙ୍ଗ;
            }

#ଯଦି GNULIB_FTS_DEBUG
          ଯଦି (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      ଯଦି (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  ଫେରନ୍ତୁ ok;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  ଅକ୍ଷର *files_from = ଖାଲି;

  /* Bit flags that control how fts works.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi = -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
#ଯଦି GNULIB_FTS_DEBUG
        ସ୍ଥିତି FTS_DEBUG:
          fts_debug = true;
          ଭାଙ୍ଗ;
#endif

        ସ୍ଥିତି '0':
          opt_nul_terminate_output = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'a':
          opt_all = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'A':
          apparent_size = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
          print_grand_total = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            ଯଦି (xstrtoimax (optarg, ଖାଲି, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            ଅନ୍ୟଥା
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'l':
          opt_ଗଣନା_all = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          opt_summarize_only = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          {
            ଗଣନା strtol_error e;
            e = xstrtoimax (optarg, ଖାଲି, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            ଯଦି (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            ଯଦି (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'x':
          bit_flags |= FTS_XDEV;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'B':
          {
            ଗଣନା strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            ଯଦି (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        ସ୍ଥିତି 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'S':
          opt_separate_dirs = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'X':
          ଯଦି (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FILES0_FROM_OPTION:
          files_from = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି INODES_OPTION:
          opt_inodes = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (getenv ("TZ"));
          ଭାଙ୍ଗ;

        ସ୍ଥିତି TIME_STYLE_OPTION:
          time_style = optarg;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ok = false;
        }
    }

  ଯଦି (!ok)
    usage (EXIT_FAILURE);

  ଯଦି (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = getenv_quoting_style ();
      ଯଦି (qs < 0)
        qs = shell_escape_quoting_style;
      ଯଦି (qs != literal_quoting_style)
        {
          set_quoting_style (ଖାଲି, qs);
          quote_output = true;
        }
    }

  ଯଦି (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  ଯଦି (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (EXIT_FAILURE);
    }

  ଯଦି (opt_summarize_only)
    max_depth = 0;

  ଯଦି (opt_inodes)
    {
      ଯଦି (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  ଯଦି (opt_time)
    {
      ଯଦି (! time_style)
        {
          time_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          ଯଦି (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          ଅନ୍ୟଥା ଯଦି (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              ଅକ୍ଷର ଅଚଳ *p = strchr (time_style, '\n');
              ଯଦି (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          ଅନ୍ୟଥା
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ସ୍ଥିର ଅକ୍ଷର ଅଚଳ posix_prefix[] = "posix-";
              ସ୍ଥିର ଅଚଳ size_t prefix_len = ଆକାର posix_prefix - 1;
              ଯେତେବେଳେ (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      ଯଦି (*time_style == '+')
        time_format = time_style + 1;
      ଅନ୍ୟଥା
        {
          ବାଛ (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (ଅକ୍ଷର ଅଚଳ *) time_style_types,
                                     ଆକାର (*time_style_types), EXIT_FAILURE))
            {
            ସ୍ଥିତି full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              ଭାଙ୍ଗ;

            ସ୍ଥିତି long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              ଭାଙ୍ଗ;

            ସ୍ଥିତି iso_time_style:
              time_format = "%Y-%m-%d";
              ଭାଙ୍ଗ;
            }
        }
    }

  ସଂରଚନା argv_iterator *ai;
  ଯଦି (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ଯଦି (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ଯଦି (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  ଯଦି (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  ଯଦି (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  ଯଦି (opt_ଗଣନା_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  ସ୍ଥିର ଅକ୍ଷର *temp_argv[] = { ଖାଲି, ଖାଲି };

  ଯେତେବେଳେ (true)
    {
      bool skip_file = false;
      ଗଣନା argv_iter_err ai_err;
      ଅକ୍ଷର *file_name = argv_iter (ai, &ai_err);
      ଯଦି (!file_name)
        {
          ବାଛ (ai_err)
            {
            ସ୍ଥିତି AI_ERR_EOF:
              ଯାଅ argv_iter_done;
            ସ୍ଥିତି AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              ଯାଅ argv_iter_done;
            ସ୍ଥିତି AI_ERR_MEM:
              xalloc_die ();
            ସ୍ଥିତି AI_ERR_OK: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      ଯଦି (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      ଯଦି (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ଯଦି (files_from == ଖାଲି)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ଅନ୍ୟଥା
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_ସଂଖ୍ୟା = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_ସଂଖ୍ୟା, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ଯଦି (skip_file)
        ok = false;
      ଅନ୍ୟଥା
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  ଯଦି (di_mnt)
    di_set_free (di_mnt);

  ଯଦି (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  ଯଦି (print_grand_total)
    print_size (&tot_dui, _("total"), false);

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
