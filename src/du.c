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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "argv-iter.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "di-set.h"
#అన్తర్భావ "exclude.h"
#అన్తర్భావ "fts-missing.h"
#అన్తర్భావ "human.h"
#అన్తర్భావ "mountlist.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "show-date.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xfts.h"
#అన్తర్భావ "xmemdup0.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "du"

#నిర్వచించు AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
స్థిర నిర్మాణం di_సమితి *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
స్థిర నిర్మాణం di_సమితి *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
స్థిర idx_t prev_level;

/* Define a class for collecting directory information. */
నిర్మాణం duinfo
{
  /* Size of files in directory.  */
  uintmax_t కొలత;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  నిర్మాణం timespec tmax;
};

/* Initialize directory data.  */
స్థిర అంతఃవరుస శూన్య
duinfo_init (నిర్మాణం duinfo *a)
{
  a->కొలత = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
స్థిర అంతఃవరుస శూన్య
duinfo_సమితి (నిర్మాణం duinfo *a, uintmax_t కొలత, నిర్మాణం timespec tmax)
{
  a->కొలత = కొలత;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
స్థిర అంతఃవరుస శూన్య
duinfo_కూడు (నిర్మాణం duinfo *a, నిర్మాణం duinfo స్థిరరాశి *b)
{
  uintmax_t కూడిక = a->కొలత + b->కొలత;
  a->కొలత = a->కొలత <= కూడిక ? కూడిక : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  ఒకవేళ (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
నిర్మాణం dulevel
{
  /* Entries in this directory.  */
  నిర్మాణం duinfo ent;

  /* Total for subdirectories.  */
  నిర్మాణం duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
స్థిర bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
స్థిర bool apparent_కొలత = false;

/* If true, count each hard link of files with multiple links.  */
స్థిర bool opt_లెక్క_all = false;

/* If true, hash all files to look for hard links.  */
స్థిర bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
స్థిర bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
స్థిర bool quote_output;

/* If true, print a grand total at the end.  */
స్థిర bool print_grand_మొత్తం = false;

/* If nonzero, do not add sizes of subdirectories.  */
స్థిర bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
స్థిర idx_t max_లోతు = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
స్థిర intmax_t opt_పరిమితి = 0;

/* Human-readable options for output.  */
స్థిర పూర్ణాంకం human_output_opts;

/* Output inodes count instead of blocks used.  */
స్థిర bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
స్థిర bool opt_సమయం = false;

/* Type of time to display. controlled by --time.  */

గణన సమయం_type
  {
    సమయం_mtime,			/* default */
    సమయం_ctime,
    సమయం_atime
  };

స్థిర గణన సమయం_type సమయం_type = సమయం_mtime;

/* User specified date / time style */
స్థిర అక్షరం స్థిరరాశి *సమయం_style = ఖాళీ;

/* Format used to display date / time. Controlled by --time-style */
స్థిర అక్షరం స్థిరరాశి *సమయం_format = ఖాళీ;

/* The local time zone rules, as per the TZ environment variable.  */
స్థిర timezone_t localtz;

/* The units to use when printing sizes.  */
స్థిర uintmax_t output_ఖండం_కొలత;

/* File name patterns to exclude.  */
స్థిర నిర్మాణం exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
స్థిర నిర్మాణం duinfo tot_dui;

#నిర్వచించు IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#ఒకవేళ GNULIB_FTS_DEBUG
  FTS_DEBUG,
#అయితే_ముగింపు
  సమయం_OPTION,
  సమయం_STYLE_OPTION,
  INODES_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"apparent-size", no_వాదం, ఖాళీ, 'A'},
  {"block-size", required_వాదం, ఖాళీ, 'B'},
  {"bytes", no_వాదం, ఖాళీ, 'b'},
  {"count-links", no_వాదం, ఖాళీ, 'l'},
#ఒకవేళ GNULIB_FTS_DEBUG
  {"-debug", no_వాదం, ఖాళీ, FTS_DEBUG},
#అయితే_ముగింపు
  {"dereference", no_వాదం, ఖాళీ, 'L'},
  {"dereference-args", no_వాదం, ఖాళీ, 'D'},
  {"exclude", required_వాదం, ఖాళీ, EXCLUDE_OPTION},
  {"exclude-from", required_వాదం, ఖాళీ, 'X'},
  {"files0-from", required_వాదం, ఖాళీ, FILES0_FROM_OPTION},
  {"human-readable", no_వాదం, ఖాళీ, 'h'},
  {"inodes", no_వాదం, ఖాళీ, INODES_OPTION},
  {"si", no_వాదం, ఖాళీ, HUMAN_SI_OPTION},
  {"max-depth", required_వాదం, ఖాళీ, 'd'},
  {"null", no_వాదం, ఖాళీ, '0'},
  {"no-dereference", no_వాదం, ఖాళీ, 'P'},
  {"one-file-system", no_వాదం, ఖాళీ, 'x'},
  {"separate-dirs", no_వాదం, ఖాళీ, 'S'},
  {"summarize", no_వాదం, ఖాళీ, 's'},
  {"total", no_వాదం, ఖాళీ, 'c'},
  {"threshold", required_వాదం, ఖాళీ, 't'},
  {"time", optional_వాదం, ఖాళీ, సమయం_OPTION},
  {"time-style", required_వాదం, ఖాళీ, సమయం_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_వాదనలు[] =
{
  "atime", "access", "use", "ctime", "status", ఖాళీ
};
స్థిర గణన సమయం_type స్థిరరాశి సమయం_types[] =
{
  సమయం_atime, సమయం_atime, సమయం_atime, సమయం_ctime, సమయం_ctime
};
ARGMATCH_VERIFY (సమయం_వాదనలు, సమయం_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
గణన సమయం_style
  {
    నిండిన_iso_సమయం_style,       /* --time-style=full-iso */
    long_iso_సమయం_style,       /* --time-style=long-iso */
    iso_సమయం_style	       /* --time-style=iso */
  };

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_style_వాదనలు[] =
{
  "full-iso", "long-iso", "iso", ఖాళీ
};
స్థిర గణన సమయం_style స్థిరరాశి సమయం_style_types[] =
{
  నిండిన_iso_సమయం_style, long_iso_సమయం_style, iso_సమయం_style
};
ARGMATCH_VERIFY (సమయం_style_వాదనలు, సమయం_style_types);

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Summarize device usage యొక్క the సమితి_2 యొక్క FILEs, recursively కొరకు directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         లిఖ counts కొరకు all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-కొలత\n\
         print apparent sizes rather than device usage;\n\
         although the apparent కొలత is usually smaller, it may be\n\
         larger due గమ్యం holes లోపల ('sparse') files,\n\
         అంతర్గత fragmentation, పరోక్ష ఖండాలు, etc.\n\
"));
      oputs (_("\
  -B, --ఖండం-కొలత=కొలత_2\n\
         scale sizes by కొలత_2 before printing them; See కొలత_2 format క్రింద;\n\
         E.g., '-BM' prints sizes లోపల units యొక్క 1,048,576 బైట్లు\n\
"));
      oputs (_("\
  -b, --బైట్లు\n\
         equivalent గమ్యం '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --మొత్తం\n\
         produce a grand మొత్తం\n\
"));
      oputs (_("\
  -D, --dereference-వాదనలు\n\
         dereference only symlinks that are listed on the command పంక్తి\n\
"));
      oputs (_("\
  -d, --max-లోతు=N\n\
         print the మొత్తం కొరకు a directory (or file, with --all)\n\
         only ఒకవేళ it is N or fewer స్థాయిలు క్రింద the command\n\
         పంక్తి వాదం;  --max-లోతు=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage యొక్క the NUL-terminated file పేర్లు\n\
         specified లోపల file F;  ఒకవేళ F is -, పఠ పేర్లు from standard ఆదానం\n\
"));
      oputs (_("\
  -H\n\
         equivalent గమ్యం --dereference-వాదనలు (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes లోపల human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead యొక్క ఖండం usage\n\
"));
      oputs (_("\
  -k\n\
         like --ఖండం-కొలత=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --లెక్క-links\n\
         లెక్క sizes many times ఒకవేళ hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --ఖండం-కొలత=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the అప్రమేయ)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         కొరకు directories చేయి not include కొలత యొక్క subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers యొక్క 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a మొత్తం కొరకు each వాదం\n\
"));
      oputs (_("\
  -t, --పరిమితి=కొలత_2\n\
         exclude entries smaller than కొలత_2 ఒకవేళ ధనాత్మక,\n\
         or entries greater than కొలత_2 ఒకవేళ negative\n\
"));
      oputs (_("\
      --సమయం\n\
         show సమయం యొక్క the చివరి modification యొక్క any file లోపల the directory,\n\
         or any యొక్క its subdirectories\n\
"));
      oputs (_("\
      --సమయం=మాట\n\
         show సమయం as మాట instead యొక్క modification సమయం:\n\
         atime, access, use, ctime or స్థితి_2\n\
"));
      oputs (_("\
      --సమయం-style=STYLE\n\
         సమయం/date format with --సమయం; see సమయం_STYLE క్రింద\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that సరిపోలిక any నమూనా_2 లోపల FILE\n\
"));
      oputs (_("\
      --exclude=నమూనా\n\
         exclude files that సరిపోలిక నమూనా\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_కొలత_note ();
      fputs (_("\
\n\
The --సమయం-style STYLE వాదం can be నిండిన-iso, దీర్ఘ-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like లోపల date(1).\n\
Also the సమయం_STYLE environment variable sets the అప్రమేయ style గమ్యం use.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
స్థిర bool
hash_ins (నిర్మాణం di_సమితి *di_సమితి, ino_t ino, dev_t dev)
{
  పూర్ణాంకం inserted = di_సమితి_చొప్పించు (di_సమితి, dev, ino);
  ఒకవేళ (inserted < 0)
    xalloc_die ();
  తిరిగి inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

స్థిర శూన్య
print_only_కొలత (uintmax_t n_బైట్లు)
{
  అక్షరం buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_బైట్లు == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_బైట్లు, buf, human_output_opts,
                            1, output_ఖండం_కొలత)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

స్థిర శూన్య
print_కొలత (స్థిరరాశి నిర్మాణం duinfo *pdui, అక్షరం స్థిరరాశి *string, bool quote)
{
  print_only_కొలత (opt_inodes
                   ? pdui->inodes
                   : pdui->కొలత);

  ఒకవేళ (opt_సమయం)
    {
      అక్షరం_రాయి ('\t');
      bool ok = show_date (సమయం_format, pdui->tmax, localtz);
      ఒకవేళ (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          అక్షరం buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  అక్షరం_రాయి ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  అక్షరం_రాయి (opt_nul_terminate_output ? '\0' : '\n');
  ఒకవేళ (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

స్థిర శూన్య
fill_mount_పట్టిక (శూన్య)
{
  నిర్మాణం mount_నమోదు *mnt_ent = read_file_system_list (false);
  అయితే (mnt_ent)
    {
      నిర్మాణం mount_నమోదు *mnt_free;
      ఒకవేళ (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          నిర్మాణం stat buf;
          ఒకవేళ (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          లేకపోతే
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_తదుపరి;
      free_mount_నమోదు (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

స్థిర bool
mount_బిందువు_లోపల_fts_చక్రం (FTSENT స్థిరరాశి *ent)
{
  FTSENT స్థిరరాశి *చక్రం_ent = ent->fts_చక్రం;

  ఒకవేళ (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_సమితి_alloc ();
      ఒకవేళ (!di_mnt)
        xalloc_die ();

      fill_mount_పట్టిక ();
    }

  అయితే (ent && ent != చక్రం_ent)
    {
      ఒకవేళ (di_సమితి_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          తిరిగి true;
        }
      ent = ent->fts_మాతృ;
    }

  తిరిగి false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

స్థిర bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  నిర్మాణం duinfo dui;
  నిర్మాణం duinfo dui_గమ్యం_print;
  స్థిర idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  స్థిర నిర్మాణం dulevel *dulvl;

  అక్షరం స్థిరరాశి *file = ent->fts_మార్గం;
  స్థిరరాశి నిర్మాణం stat *sb = ent->fts_statp;
  పూర్ణాంకం info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_నమోదు (ent);

  ఒకవేళ (ignore_missing && ignorable_fts_error (ent))
    తిరిగి true;

  ఒకవేళ (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  లేకపోతే ఒకవేళ (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      ఒకవేళ (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          ఒకవేళ (info == FTS_NSOK)
            {
              fts_సమితి (fts, ent, FTS_AGAIN);
              MAYBE_వాడని FTSENT స్థిరరాశి *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              ఒకవేళ (ignore_missing && ignorable_fts_error (ent))
                తిరిగి true;
            }

          ఒకవేళ (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              తిరిగి false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          ఒకవేళ (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      ఒకవేళ (excluded
          || (! opt_లెక్క_all
              && (hash_all || (! S_ISDIR (sb->st_బహుళకం) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          ఒకవేళ (info == FTS_D)
            {
              fts_సమితి (fts, ent, FTS_SKIP);
              MAYBE_వాడని FTSENT స్థిరరాశి *e = fts_read (fts);
              affirm (e == ent);
            }

          తిరిగి true;
        }

      ఎంపిక (info)
        {
        స్థితి FTS_D:
          తిరిగి true;

        స్థితి FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          ఆపు;

        స్థితి FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          ఒకవేళ (చక్రం_warning_required (fts, ent)
              && ! mount_బిందువు_లోపల_fts_చక్రం (ent))
            {
              emit_చక్రం_warning (file);
              తిరిగి false;
            }
          తిరిగి true;
        }
    }

  duinfo_సమితి (&dui,
              (apparent_కొలత
               ? (usable_st_కొలత (sb) ? MAX (0, sb->st_కొలత) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (సమయం_type == సమయం_mtime ? get_stat_mtime (sb)
               : సమయం_type == సమయం_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_గమ్యం_print = dui;

  ఒకవేళ (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, పరిమాణం *dulvl);
    }
  లేకపోతే
    {
      ఒకవేళ (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      లేకపోతే ఒకవేళ (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          ఒకవేళ (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             పరిమాణం *dulvl);

          కొరకు (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      లేకపోతే /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_కూడు (&dui_గమ్యం_print, &dulvl[prev_level].ent);
          ఒకవేళ (!opt_separate_dirs)
            duinfo_కూడు (&dui_గమ్యం_print, &dulvl[prev_level].subdir);
          duinfo_కూడు (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_కూడు (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  ఒకవేళ (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_కూడు (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_కూడు (&tot_dui, &dui);

  ఒకవేళ ((IS_DIR_TYPE (info) && level <= max_లోతు)
      || (opt_all && level <= max_లోతు)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_గమ్యం_print.inodes : dui_గమ్యం_print.కొలత;
      ఒకవేళ (opt_పరిమితి < 0
          ? v <= -opt_పరిమితి
          : v >= opt_పరిమితి)
        print_కొలత (&dui_గమ్యం_print, file, quote_output);
    }

  తిరిగి ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

స్థిర bool
du_files (అక్షరం **files, పూర్ణాంకం బిట్టు_flags)
{
  bool ok = true;

  ఒకవేళ (*files)
    {
      FTS *fts = xfts_open (files, బిట్టు_flags, ఖాళీ);

      అయితే (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ఒకవేళ (ent == ఖాళీ)
            {
              ఒకవేళ (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_మార్గం));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              ఆపు;
            }

#ఒకవేళ GNULIB_FTS_DEBUG
          ఒకవేళ (fts_debug)
            fts_cross_check (fts);
#అయితే_ముగింపు

          ok &= process_file (fts, ent);
        }

      ఒకవేళ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *cwd_only[2];
  bool max_లోతు_specified = false;
  bool ok = true;
  అక్షరం *files_from = ఖాళీ;

  /* Bit flags that control how fts works.  */
  పూర్ణాంకం బిట్టు_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  పూర్ణాంకం symlink_deref_బిట్లు = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_ఖండం_కొలత);

  అయితే (true)
    {
      పూర్ణాంకం oi = -1;
      పూర్ణాంకం c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
#ఒకవేళ GNULIB_FTS_DEBUG
        స్థితి FTS_DEBUG:
          fts_debug = true;
          ఆపు;
#అయితే_ముగింపు

        స్థితి '0':
          opt_nul_terminate_output = true;
          ఆపు;

        స్థితి 'a':
          opt_all = true;
          ఆపు;

        స్థితి 'A':
          apparent_కొలత = true;
          ఆపు;

        స్థితి 'b':
          apparent_కొలత = true;
          human_output_opts = 0;
          output_ఖండం_కొలత = 1;
          ఆపు;

        స్థితి 'c':
          print_grand_మొత్తం = true;
          ఆపు;

        స్థితి 'h':
          human_output_opts = human_autoscale | human_SI | human_పునాది_1024;
          output_ఖండం_కొలత = 1;
          ఆపు;

        స్థితి HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_ఖండం_కొలత = 1;
          ఆపు;

        స్థితి 'k':
          human_output_opts = 0;
          output_ఖండం_కొలత = 1024;
          ఆపు;

        స్థితి 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            ఒకవేళ (xstrtoimax (optarg, ఖాళీ, 0, &tmp, "") == LONGINT_OK
                && ! ckd_కూడు (&max_లోతు, tmp, 0) && 0 <= max_లోతు)
              max_లోతు_specified = true;
            లేకపోతే
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          ఆపు;

        స్థితి 'm':
          human_output_opts = 0;
          output_ఖండం_కొలత = 1024 * 1024;
          ఆపు;

        స్థితి 'l':
          opt_లెక్క_all = true;
          ఆపు;

        స్థితి 's':
          opt_summarize_only = true;
          ఆపు;

        స్థితి 't':
          {
            గణన strtol_error e;
            e = xstrtoimax (optarg, ఖాళీ, 0, &opt_పరిమితి,
                            "kKmMGTPEZYRQ0");
            ఒకవేళ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            ఒకవేళ (opt_పరిమితి == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          ఆపు;

        స్థితి 'x':
          బిట్టు_flags |= FTS_XDEV;
          ఆపు;

        స్థితి 'B':
          {
            గణన strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ఖండం_కొలత);
            ఒకవేళ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ఆపు;

        స్థితి 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        స్థితి 'D':
          symlink_deref_బిట్లు = FTS_COMFOLLOW | FTS_PHYSICAL;
          ఆపు;

        స్థితి 'L': /* --dereference */
          symlink_deref_బిట్లు = FTS_LOGICAL;
          ఆపు;

        స్థితి 'P': /* --no-dereference */
          symlink_deref_బిట్లు = FTS_PHYSICAL;
          ఆపు;

        స్థితి 'S':
          opt_separate_dirs = true;
          ఆపు;

        స్థితి 'X':
          ఒకవేళ (కూడు_exclude_file (కూడు_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          ఆపు;

        స్థితి FILES0_FROM_OPTION:
          files_from = optarg;
          ఆపు;

        స్థితి EXCLUDE_OPTION:
          కూడు_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          ఆపు;

        స్థితి INODES_OPTION:
          opt_inodes = true;
          ఆపు;

        స్థితి సమయం_OPTION:
          opt_సమయం = true;
          సమయం_type =
            (optarg
             ? XARGMATCH ("--time", optarg, సమయం_వాదనలు, సమయం_types)
             : సమయం_mtime);
          localtz = tzalloc (getenv ("TZ"));
          ఆపు;

        స్థితి సమయం_STYLE_OPTION:
          సమయం_style = optarg;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          ok = false;
        }
    }

  ఒకవేళ (!ok)
    usage (EXIT_FAILURE);

  ఒకవేళ (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      పూర్ణాంకం qs = getenv_quoting_style ();
      ఒకవేళ (qs < 0)
        qs = shell_escape_quoting_style;
      ఒకవేళ (qs != literal_quoting_style)
        {
          సమితి_quoting_style (ఖాళీ, qs);
          quote_output = true;
        }
    }

  ఒకవేళ (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (opt_summarize_only && max_లోతు_specified && max_లోతు == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  ఒకవేళ (opt_summarize_only && max_లోతు_specified && max_లోతు != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_లోతు);
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (opt_summarize_only)
    max_లోతు = 0;

  ఒకవేళ (opt_inodes)
    {
      ఒకవేళ (apparent_కొలత)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_ఖండం_కొలత = 1;
    }

  /* Process time style if printing last times.  */
  ఒకవేళ (opt_సమయం)
    {
      ఒకవేళ (! సమయం_style)
        {
          సమయం_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          ఒకవేళ (! సమయం_style || streq (సమయం_style, "locale"))
            సమయం_style = "long-iso";
          లేకపోతే ఒకవేళ (*సమయం_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              అక్షరం స్థిరరాశి *p = strchr (సమయం_style, '\n');
              ఒకవేళ (p)
                సమయం_style = xmemdup0 (సమయం_style, p - సమయం_style);
            }
          లేకపోతే
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              స్థిర అక్షరం స్థిరరాశి posix_ఉపసర్గ[] = "posix-";
              స్థిర స్థిరరాశి size_t ఉపసర్గ_len = పరిమాణం posix_ఉపసర్గ - 1;
              అయితే (STREQ_LEN (సమయం_style, posix_ఉపసర్గ, ఉపసర్గ_len))
                సమయం_style += ఉపసర్గ_len;
            }
        }

      ఒకవేళ (*సమయం_style == '+')
        సమయం_format = సమయం_style + 1;
      లేకపోతే
        {
          ఎంపిక (x_timestyle_సరిపోలిక (సమయం_style, /*allow_posix=*/ false,
                                     సమయం_style_వాదనలు,
                                     (అక్షరం స్థిరరాశి *) సమయం_style_types,
                                     పరిమాణం (*సమయం_style_types), EXIT_FAILURE))
            {
            స్థితి నిండిన_iso_సమయం_style:
              సమయం_format = "%Y-%m-%d %H:%M:%S.%N %z";
              ఆపు;

            స్థితి long_iso_సమయం_style:
              సమయం_format = "%Y-%m-%d %H:%M";
              ఆపు;

            స్థితి iso_సమయం_style:
              సమయం_format = "%Y-%m-%d";
              ఆపు;
            }
        }
    }

  నిర్మాణం argv_iterator *ai;
  ఒకవేళ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ఒకవేళ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ఒకవేళ (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_ప్రవాహం (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  లేకపోతే
    {
      అక్షరం **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_బిట్లు == FTS_LOGICAL);
    }

  ఒకవేళ (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_సమితి_alloc ();
  ఒకవేళ (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  ఒకవేళ (opt_లెక్క_all || ! hash_all)
    బిట్టు_flags |= FTS_TIGHT_చక్రం_CHECK;

  బిట్టు_flags |= symlink_deref_బిట్లు;
  స్థిర అక్షరం *తాత్కాలిక_argv[] = { ఖాళీ, ఖాళీ };

  అయితే (true)
    {
      bool skip_file = false;
      గణన argv_iter_err ai_err;
      అక్షరం *file_name = argv_iter (ai, &ai_err);
      ఒకవేళ (!file_name)
        {
          ఎంపిక (ai_err)
            {
            స్థితి AI_ERR_EOF:
              వెళ్ళు argv_iter_పూర్తి;
            స్థితి AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              వెళ్ళు argv_iter_పూర్తి;
            స్థితి AI_ERR_MEM:
              xalloc_die ();
            స్థితి AI_ERR_OK: అప్రమేయ:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      ఒకవేళ (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      ఒకవేళ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ఒకవేళ (files_from == ఖాళీ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          లేకపోతే
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_సంఖ్య = argv_iter_n_వాదనలు (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_సంఖ్య, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ఒకవేళ (skip_file)
        ok = false;
      లేకపోతే
        {
          తాత్కాలిక_argv[0] = file_name;
          ok &= du_files (తాత్కాలిక_argv, బిట్టు_flags);
        }
    }
 argv_iter_పూర్తి:

  argv_iter_free (ai);
  di_సమితి_free (di_files);
  ఒకవేళ (di_mnt)
    di_సమితి_free (di_mnt);

  ఒకవేళ (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  ఒకవేళ (print_grand_మొత్తం)
    print_కొలత (&tot_dui, _("total"), false);

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
