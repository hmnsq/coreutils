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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "argv-iter.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "di-set.h"
#अन्तर्भाव "exclude.h"
#अन्तर्भाव "fts-missing.h"
#अन्तर्भाव "human.h"
#अन्तर्भाव "mountlist.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "show-date.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "xfts.h"
#अन्तर्भाव "xmemdup0.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "du"

#define AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
स्थिर संरचना di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
स्थिर संरचना di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
स्थिर idx_t prev_level;

/* Define a class for collecting directory information. */
संरचना duinfo
{
  /* Size of files in directory.  */
  uintmax_t size;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  संरचना timespec tmax;
};

/* Initialize directory data.  */
स्थिर अन्तःस्थ शून्य
duinfo_init (संरचना duinfo *a)
{
  a->size = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (समय_प्रकार);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
स्थिर अन्तःस्थ शून्य
duinfo_set (संरचना duinfo *a, uintmax_t size, संरचना timespec tmax)
{
  a->size = size;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
स्थिर अन्तःस्थ शून्य
duinfo_add (संरचना duinfo *a, संरचना duinfo ध्रुव *b)
{
  uintmax_t sum = a->size + b->size;
  a->size = a->size <= sum ? sum : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  यदि (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
संरचना dulevel
{
  /* Entries in this directory.  */
  संरचना duinfo ent;

  /* Total for subdirectories.  */
  संरचना duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
स्थिर bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
स्थिर bool apparent_size = false;

/* If true, count each hard link of files with multiple links.  */
स्थिर bool opt_गिनती_all = false;

/* If true, hash all files to look for hard links.  */
स्थिर bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
स्थिर bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
स्थिर bool quote_output;

/* If true, print a grand total at the end.  */
स्थिर bool print_grand_total = false;

/* If nonzero, do not add sizes of subdirectories.  */
स्थिर bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
स्थिर idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
स्थिर intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
स्थिर पूर्णाङ्क human_output_opts;

/* Output inodes count instead of blocks used.  */
स्थिर bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
स्थिर bool opt_time = false;

/* Type of time to display. controlled by --time.  */

गणना time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

स्थिर गणना time_type time_type = time_mtime;

/* User specified date / time style */
स्थिर वर्ण ध्रुव *time_style = रिक्त;

/* Format used to display date / time. Controlled by --time-style */
स्थिर वर्ण ध्रुव *time_format = रिक्त;

/* The local time zone rules, as per the TZ environment variable.  */
स्थिर timezone_t localtz;

/* The units to use when printing sizes.  */
स्थिर uintmax_t output_block_size;

/* File name patterns to exclude.  */
स्थिर संरचना exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
स्थिर संरचना duinfo tot_dui;

#define IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  EXCLUDE_OPTION = अक्षर_उच्च + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#यदि GNULIB_FTS_DEBUG
  FTS_DEBUG,
#endif
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"all", no_argument, रिक्त, 'a'},
  {"apparent-size", no_argument, रिक्त, 'A'},
  {"block-size", required_argument, रिक्त, 'B'},
  {"bytes", no_argument, रिक्त, 'b'},
  {"count-links", no_argument, रिक्त, 'l'},
#यदि GNULIB_FTS_DEBUG
  {"-debug", no_argument, रिक्त, FTS_DEBUG},
#endif
  {"dereference", no_argument, रिक्त, 'L'},
  {"dereference-args", no_argument, रिक्त, 'D'},
  {"exclude", required_argument, रिक्त, EXCLUDE_OPTION},
  {"exclude-from", required_argument, रिक्त, 'X'},
  {"files0-from", required_argument, रिक्त, FILES0_FROM_OPTION},
  {"human-readable", no_argument, रिक्त, 'h'},
  {"inodes", no_argument, रिक्त, INODES_OPTION},
  {"si", no_argument, रिक्त, HUMAN_SI_OPTION},
  {"max-depth", required_argument, रिक्त, 'd'},
  {"null", no_argument, रिक्त, '0'},
  {"no-dereference", no_argument, रिक्त, 'P'},
  {"one-file-system", no_argument, रिक्त, 'x'},
  {"separate-dirs", no_argument, रिक्त, 'S'},
  {"summarize", no_argument, रिक्त, 's'},
  {"total", no_argument, रिक्त, 'c'},
  {"threshold", required_argument, रिक्त, 't'},
  {"time", optional_argument, रिक्त, TIME_OPTION},
  {"time-style", required_argument, रिक्त, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

स्थिर वर्ण ध्रुव *ध्रुव time_args[] =
{
  "atime", "access", "use", "ctime", "status", रिक्त
};
स्थिर गणना time_type ध्रुव time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
गणना time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

स्थिर वर्ण ध्रुव *ध्रुव time_style_args[] =
{
  "full-iso", "long-iso", "iso", रिक्त
};
स्थिर गणना time_style ध्रुव time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      ख_माला_दो (_("\
Summarize device usage of the set of FILEs, recursively प्रति directories.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         लिख counts प्रति all files, not just directories\n\
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
         dereference only symlinks that are listed on the command पंक्ति\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the total प्रति a directory (or file, with --all)\n\
         only यदि it is N or fewer levels below the command\n\
         पंक्ति argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  यदि F is -, पठ names from standard निवेश\n\
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
  -l, --गिनती-links\n\
         गिनती sizes many बार यदि hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-size=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the स्वभाव)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         प्रति directories कुरु not include size of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a total प्रति each argument\n\
"));
      oputs (_("\
  -t, --threshold=SIZE\n\
         exclude entries smaller than SIZE यदि positive,\n\
         or entries greater than SIZE यदि negative\n\
"));
      oputs (_("\
      --समय\n\
         show समय of the last modification of any file in the directory,\n\
         or any of its subdirectories\n\
"));
      oputs (_("\
      --समय=WORD\n\
         show समय as WORD instead of modification समय:\n\
         atime, access, use, स_समय or status\n\
"));
      oputs (_("\
      --समय-style=STYLE\n\
         समय/date format with --समय; see TIME_STYLE below\n\
"));
      oputs (_("\
  -X, --exclude-from=खाता\n\
         exclude files that match any pattern in खाता\n\
"));
      oputs (_("\
      --exclude=PATTERN\n\
         exclude files that match PATTERN\n\
"));
      oputs (_("\
  -x, --one-file-प्रणाली\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_size_note ();
      ख_माला_दो (_("\
\n\
The --समय-style STYLE argument can be full-iso, दीर्घ-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like in date(1).\n\
Also the TIME_STYLE environment variable sets the स्वभाव style to use.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
स्थिर bool
hash_ins (संरचना di_set *di_set, ino_t ino, dev_t dev)
{
  पूर्णाङ्क inserted = di_set_जोड़ो (di_set, dev, ino);
  यदि (inserted < 0)
    xalloc_die ();
  प्रत्यागम inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

स्थिर शून्य
print_only_size (uintmax_t n_bytes)
{
  वर्ण buf[LONGEST_HUMAN_READABLE + 1];
  ख_माला_दो ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_size)),
         मानक_निकास);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

स्थिर शून्य
print_size (ध्रुव संरचना duinfo *pdui, वर्ण ध्रुव *string, bool quote)
{
  print_only_size (opt_inodes
                   ? pdui->inodes
                   : pdui->size);

  यदि (opt_time)
    {
      अक्षर_दो ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      यदि (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          वर्ण buf[INT_BUFSIZE_BOUND (intmax_t)];
          ख_माला_दो (timetostr (pdui->tmax.tv_sec, buf), मानक_निकास);
        }
    }
  अक्षर_दो ('\t');
  ख_माला_दो (quote ? quoteN (string) : string, मानक_निकास);
  अक्षर_दो (opt_nul_terminate_output ? '\0' : '\n');
  यदि (ख_साफ (मानक_निकास) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

स्थिर शून्य
fill_mount_table (शून्य)
{
  संरचना mount_entry *mnt_ent = read_file_system_list (false);
  यावत् (mnt_ent)
    {
      संरचना mount_entry *mnt_free;
      यदि (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          संरचना stat buf;
          यदि (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          अन्यथा
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

स्थिर bool
mount_point_in_fts_cycle (FTSENT ध्रुव *ent)
{
  FTSENT ध्रुव *cycle_ent = ent->fts_cycle;

  यदि (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      यदि (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  यावत् (ent && ent != cycle_ent)
    {
      यदि (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          प्रत्यागम true;
        }
      ent = ent->fts_parent;
    }

  प्रत्यागम false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

स्थिर bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  संरचना duinfo dui;
  संरचना duinfo dui_to_print;
  स्थिर idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  स्थिर संरचना dulevel *dulvl;

  वर्ण ध्रुव *file = ent->fts_path;
  ध्रुव संरचना stat *sb = ent->fts_statp;
  पूर्णाङ्क info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  यदि (ignore_missing && ignorable_fts_error (ent))
    प्रत्यागम true;

  यदि (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  अन्यथा यदि (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      यदि (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          यदि (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT ध्रुव *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              यदि (ignore_missing && ignorable_fts_error (ent))
                प्रत्यागम true;
            }

          यदि (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              प्रत्यागम false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          यदि (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      यदि (excluded
          || (! opt_गिनती_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          यदि (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT ध्रुव *e = fts_read (fts);
              affirm (e == ent);
            }

          प्रत्यागम true;
        }

      विकल्प (info)
        {
        पक्ष FTS_D:
          प्रत्यागम true;

        पक्ष FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          भङ्ग;

        पक्ष FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          यदि (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              प्रत्यागम false;
            }
          प्रत्यागम true;
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

  यदि (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, परिमाण *dulvl);
    }
  अन्यथा
    {
      यदि (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      अन्यथा यदि (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          यदि (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             परिमाण *dulvl);

          प्रति (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      अन्यथा /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_add (&dui_to_print, &dulvl[prev_level].ent);
          यदि (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[prev_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  यदि (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  यदि ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.size;
      यदि (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_size (&dui_to_print, file, quote_output);
    }

  प्रत्यागम ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

स्थिर bool
du_files (वर्ण **files, पूर्णाङ्क bit_flags)
{
  bool ok = true;

  यदि (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, रिक्त);

      यावत् (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          यदि (ent == रिक्त)
            {
              यदि (त्रुटि_सं != 0)
                {
                  error (0, त्रुटि_सं, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              भङ्ग;
            }

#यदि GNULIB_FTS_DEBUG
          यदि (fts_debug)
            fts_cross_check (fts);
#endif

          ok &= process_file (fts, ent);
        }

      यदि (fts_close (fts) != 0)
        {
          error (0, त्रुटि_सं, _("fts_close failed"));
          ok = false;
        }
    }

  प्रत्यागम ok;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  वर्ण *files_from = रिक्त;

  /* Bit flags that control how fts works.  */
  पूर्णाङ्क bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  पूर्णाङ्क symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  exclude = new_exclude ();

  human_options (दो_पर्या ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_size);

  यावत् (true)
    {
      पूर्णाङ्क oi = -1;
      पूर्णाङ्क c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      यदि (c == -1)
        भङ्ग;

      विकल्प (c)
        {
#यदि GNULIB_FTS_DEBUG
        पक्ष FTS_DEBUG:
          fts_debug = true;
          भङ्ग;
#endif

        पक्ष '0':
          opt_nul_terminate_output = true;
          भङ्ग;

        पक्ष 'a':
          opt_all = true;
          भङ्ग;

        पक्ष 'A':
          apparent_size = true;
          भङ्ग;

        पक्ष 'b':
          apparent_size = true;
          human_output_opts = 0;
          output_block_size = 1;
          भङ्ग;

        पक्ष 'c':
          print_grand_total = true;
          भङ्ग;

        पक्ष 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_size = 1;
          भङ्ग;

        पक्ष HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_size = 1;
          भङ्ग;

        पक्ष 'k':
          human_output_opts = 0;
          output_block_size = 1024;
          भङ्ग;

        पक्ष 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            यदि (xstrtoimax (optarg, रिक्त, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            अन्यथा
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          भङ्ग;

        पक्ष 'm':
          human_output_opts = 0;
          output_block_size = 1024 * 1024;
          भङ्ग;

        पक्ष 'l':
          opt_गिनती_all = true;
          भङ्ग;

        पक्ष 's':
          opt_summarize_only = true;
          भङ्ग;

        पक्ष 't':
          {
            गणना strtol_error e;
            e = xstrtoimax (optarg, रिक्त, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            यदि (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            यदि (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (निकास_त्रुटि, 0, _("invalid --threshold argument '-0'"));
              }
          }
          भङ्ग;

        पक्ष 'x':
          bit_flags |= FTS_XDEV;
          भङ्ग;

        पक्ष 'B':
          {
            गणना strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_size);
            यदि (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          भङ्ग;

        पक्ष 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        पक्ष 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          भङ्ग;

        पक्ष 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          भङ्ग;

        पक्ष 'S':
          opt_separate_dirs = true;
          भङ्ग;

        पक्ष 'X':
          यदि (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, त्रुटि_सं, "%s", quotef (optarg));
              ok = false;
            }
          भङ्ग;

        पक्ष FILES0_FROM_OPTION:
          files_from = optarg;
          भङ्ग;

        पक्ष EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          भङ्ग;

        पक्ष INODES_OPTION:
          opt_inodes = true;
          भङ्ग;

        पक्ष TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (दो_पर्या ("TZ"));
          भङ्ग;

        पक्ष TIME_STYLE_OPTION:
          time_style = optarg;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          ok = false;
        }
    }

  यदि (!ok)
    usage (निकास_त्रुटि);

  यदि (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
      यदि (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  यदि (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (निकास_त्रुटि);
    }

  यदि (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  यदि (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (निकास_त्रुटि);
    }

  यदि (opt_summarize_only)
    max_depth = 0;

  यदि (opt_inodes)
    {
      यदि (apparent_size)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_size = 1;
    }

  /* Process time style if printing last times.  */
  यदि (opt_time)
    {
      यदि (! time_style)
        {
          time_style = दो_पर्या ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          यदि (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          अन्यथा यदि (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              वर्ण ध्रुव *p = म_अक्षर (time_style, '\n');
              यदि (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          अन्यथा
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              स्थिर वर्ण ध्रुव posix_prefix[] = "posix-";
              स्थिर ध्रुव माप_प्रकार prefix_len = परिमाण posix_prefix - 1;
              यावत् (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      यदि (*time_style == '+')
        time_format = time_style + 1;
      अन्यथा
        {
          विकल्प (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (वर्ण ध्रुव *) time_style_types,
                                     परिमाण (*time_style_types), निकास_त्रुटि))
            {
            पक्ष full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              भङ्ग;

            पक्ष long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              भङ्ग;

            पक्ष iso_time_style:
              time_format = "%Y-%m-%d";
              भङ्ग;
            }
        }
    }

  संरचना argv_iterator *ai;
  यदि (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      यदि (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (निकास_त्रुटि);
        }

      यदि (! (streq (files_from, "-") || ख_व_खोलो (files_from, "r", मानक_निवेश)))
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (मानक_निवेश);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  अन्यथा
    {
      वर्ण **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  यदि (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  यदि (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  यदि (opt_गिनती_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  स्थिर वर्ण *temp_argv[] = { रिक्त, रिक्त };

  यावत् (true)
    {
      bool skip_file = false;
      गणना argv_iter_err ai_err;
      वर्ण *file_name = argv_iter (ai, &ai_err);
      यदि (!file_name)
        {
          विकल्प (ai_err)
            {
            पक्ष AI_ERR_EOF:
              गच्छ argv_iter_done;
            पक्ष AI_ERR_READ:
              error (0, त्रुटि_सं, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              गच्छ argv_iter_done;
            पक्ष AI_ERR_MEM:
              xalloc_die ();
            पक्ष AI_ERR_OK: स्वभाव:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      यदि (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      यदि (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          यदि (files_from == रिक्त)
            error (0, 0, "%s", _("invalid zero-length file name"));
          अन्यथा
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_संख्या = argv_iter_n_args (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_संख्या, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      यदि (skip_file)
        ok = false;
      अन्यथा
        {
          temp_argv[0] = file_name;
          ok &= du_files (temp_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  यदि (di_mnt)
    di_set_free (di_mnt);

  यदि (files_from && (ख_त्रुटि (मानक_निवेश) || ख_बंद (मानक_निवेश) != 0) && ok)
    error (निकास_त्रुटि, 0, _("error reading %s"), quoteaf (files_from));

  यदि (print_grand_total)
    print_size (&tot_dui, _("total"), false);

  प्रत्यागम ok ? निकास_सफल : निकास_त्रुटि;
}
