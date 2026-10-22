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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"
#समावेश "argv-iter.h"
#समावेश "assure.h"
#समावेश "di-set.h"
#समावेश "exclude.h"
#समावेश "fts-missing.h"
#समावेश "human.h"
#समावेश "mountlist.h"
#समावेश "quote.h"
#समावेश "show-date.h"
#समावेश "stat-size.h"
#समावेश "stat-time.h"
#समावेश "stdio--.h"
#समावेश "xfts.h"
#समावेश "xmemdup0.h"
#समावेश "xstrtol.h"
#समावेश "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "du"

#घोषणा AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
अटल काष्ठा di_set *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
अटल काष्ठा di_set *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
अटल idx_t पूर्व_level;

/* Define a class for collecting directory information. */
काष्ठा duinfo
{
  /* Size of files in directory.  */
  uintmax_t आकार;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  काष्ठा timespec tmax;
};

/* Initialize directory data.  */
अटल अंतरभूत व्योम
duinfo_init (काष्ठा duinfo *a)
{
  a->आकार = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (समय_प्रकार);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
अटल अंतरभूत व्योम
duinfo_set (काष्ठा duinfo *a, uintmax_t आकार, काष्ठा timespec tmax)
{
  a->आकार = आकार;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
अटल अंतरभूत व्योम
duinfo_add (काष्ठा duinfo *a, काष्ठा duinfo स्थिर *b)
{
  uintmax_t योग = a->आकार + b->आकार;
  a->आकार = a->आकार <= योग ? योग : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  अगर (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
काष्ठा dulevel
{
  /* Entries in this directory.  */
  काष्ठा duinfo ent;

  /* Total for subdirectories.  */
  काष्ठा duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
अटल bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
अटल bool apparent_आकार = false;

/* If true, count each hard link of files with multiple links.  */
अटल bool opt_गिनती_all = false;

/* If true, hash all files to look for hard links.  */
अटल bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
अटल bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
अटल bool quote_output;

/* If true, print a grand total at the end.  */
अटल bool print_grand_योग = false;

/* If nonzero, do not add sizes of subdirectories.  */
अटल bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
अटल idx_t max_depth = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
अटल intmax_t opt_threshold = 0;

/* Human-readable options for output.  */
अटल पूर्णांक human_output_opts;

/* Output inodes count instead of blocks used.  */
अटल bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
अटल bool opt_time = false;

/* Type of time to display. controlled by --time.  */

क्रमागत time_type
  {
    time_mtime,			/* default */
    time_ctime,
    time_atime
  };

अटल क्रमागत time_type time_type = time_mtime;

/* User specified date / time style */
अटल अक्षर स्थिर *time_style = शून्य;

/* Format used to display date / time. Controlled by --time-style */
अटल अक्षर स्थिर *time_format = शून्य;

/* The local time zone rules, as per the TZ environment variable.  */
अटल timezone_t localtz;

/* The units to use when printing sizes.  */
अटल uintmax_t output_block_आकार;

/* File name patterns to exclude.  */
अटल काष्ठा exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
अटल काष्ठा duinfo tot_dui;

#घोषणा IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  EXCLUDE_OPTION = अक्षर_उच्च + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#अगर GNULIB_FTS_DEBUG
  FTS_DEBUG,
#पूर्ण_अगर
  TIME_OPTION,
  TIME_STYLE_OPTION,
  INODES_OPTION
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"all", no_argument, शून्य, 'a'},
  {"apparent-size", no_argument, शून्य, 'A'},
  {"block-size", required_argument, शून्य, 'B'},
  {"bytes", no_argument, शून्य, 'b'},
  {"count-links", no_argument, शून्य, 'l'},
#अगर GNULIB_FTS_DEBUG
  {"-debug", no_argument, शून्य, FTS_DEBUG},
#पूर्ण_अगर
  {"dereference", no_argument, शून्य, 'L'},
  {"dereference-args", no_argument, शून्य, 'D'},
  {"exclude", required_argument, शून्य, EXCLUDE_OPTION},
  {"exclude-from", required_argument, शून्य, 'X'},
  {"files0-from", required_argument, शून्य, FILES0_FROM_OPTION},
  {"human-readable", no_argument, शून्य, 'h'},
  {"inodes", no_argument, शून्य, INODES_OPTION},
  {"si", no_argument, शून्य, HUMAN_SI_OPTION},
  {"max-depth", required_argument, शून्य, 'd'},
  {"null", no_argument, शून्य, '0'},
  {"no-dereference", no_argument, शून्य, 'P'},
  {"one-file-system", no_argument, शून्य, 'x'},
  {"separate-dirs", no_argument, शून्य, 'S'},
  {"summarize", no_argument, शून्य, 's'},
  {"total", no_argument, शून्य, 'c'},
  {"threshold", required_argument, शून्य, 't'},
  {"time", optional_argument, शून्य, TIME_OPTION},
  {"time-style", required_argument, शून्य, TIME_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

अटल अक्षर स्थिर *स्थिर time_args[] =
{
  "atime", "access", "use", "ctime", "status", शून्य
};
अटल क्रमागत time_type स्थिर time_types[] =
{
  time_atime, time_atime, time_atime, time_ctime, time_ctime
};
ARGMATCH_VERIFY (time_args, time_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
क्रमागत time_style
  {
    full_iso_time_style,       /* --time-style=full-iso */
    long_iso_time_style,       /* --time-style=long-iso */
    iso_time_style	       /* --time-style=iso */
  };

अटल अक्षर स्थिर *स्थिर time_style_args[] =
{
  "full-iso", "long-iso", "iso", शून्य
};
अटल क्रमागत time_style स्थिर time_style_types[] =
{
  full_iso_time_style, long_iso_time_style, iso_time_style
};
ARGMATCH_VERIFY (time_style_args, time_style_types);

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), program_name, program_name);
      ख_माला_दो (_("\
Summarize device usage of the set of FILEs, recursively क्रम directories.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         ग_लिखो counts क्रम all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-आकार\n\
         print apparent sizes rather than device usage;\n\
         although the apparent आकार is usually smaller, it may be\n\
         larger due to holes in ('sparse') files,\n\
         internal fragmentation, indirect blocks, etc.\n\
"));
      oputs (_("\
  -B, --block-आकार=आकार_2\n\
         scale sizes by आकार_2 before printing them; See आकार_2 format below;\n\
         E.g., '-BM' prints sizes in units of 1,048,576 bytes\n\
"));
      oputs (_("\
  -b, --bytes\n\
         equivalent to '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --योग_2\n\
         produce a grand योग_2\n\
"));
      oputs (_("\
  -D, --dereference-args\n\
         dereference only symlinks that are listed on the command पंक्ति\n\
"));
      oputs (_("\
  -d, --max-depth=N\n\
         print the योग_2 क्रम a directory (or file, with --all)\n\
         only अगर it is N or fewer levels below the command\n\
         पंक्ति argument;  --max-depth=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage of the NUL-terminated file names\n\
         specified in file F;  अगर F is -, पढ़ो names from standard निवेश\n\
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
         like --block-आकार=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --गिनती-links\n\
         गिनती sizes many बार अगर hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --block-आकार=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the शेष)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         क्रम directories करो not include आकार of subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers of 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a योग_2 क्रम each argument\n\
"));
      oputs (_("\
  -t, --threshold=आकार_2\n\
         exclude entries smaller than आकार_2 अगर positive,\n\
         or entries greater than आकार_2 अगर negative\n\
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
      emit_आकार_note ();
      ख_माला_दो (_("\
\n\
The --समय-style STYLE argument can be full-iso, दीर्घ-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like in date(1).\n\
Also the TIME_STYLE environment variable sets the शेष style to use.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
अटल bool
hash_ins (काष्ठा di_set *di_set, ino_t ino, dev_t dev)
{
  पूर्णांक inserted = di_set_जोड़ो (di_set, dev, ino);
  अगर (inserted < 0)
    xalloc_die ();
  वापस inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

अटल व्योम
print_only_आकार (uintmax_t n_bytes)
{
  अक्षर buf[LONGEST_HUMAN_READABLE + 1];
  ख_माला_दो ((n_bytes == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_bytes, buf, human_output_opts,
                            1, output_block_आकार)),
         मानक_निकास);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

अटल व्योम
print_आकार (स्थिर काष्ठा duinfo *pdui, अक्षर स्थिर *string, bool quote)
{
  print_only_आकार (opt_inodes
                   ? pdui->inodes
                   : pdui->आकार);

  अगर (opt_time)
    {
      अक्षर_दो ('\t');
      bool ok = show_date (time_format, pdui->tmax, localtz);
      अगर (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];
          ख_माला_दो (timetostr (pdui->tmax.tv_sec, buf), मानक_निकास);
        }
    }
  अक्षर_दो ('\t');
  ख_माला_दो (quote ? quoteN (string) : string, मानक_निकास);
  अक्षर_दो (opt_nul_terminate_output ? '\0' : '\n');
  अगर (ख_साफ (मानक_निकास) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

अटल व्योम
fill_mount_table (व्योम)
{
  काष्ठा mount_entry *mnt_ent = read_file_system_list (false);
  जबतक (mnt_ent)
    {
      काष्ठा mount_entry *mnt_free;
      अगर (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          काष्ठा stat buf;
          अगर (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          अन्यथा
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_अग्र;
      free_mount_entry (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

अटल bool
mount_point_in_fts_cycle (FTSENT स्थिर *ent)
{
  FTSENT स्थिर *cycle_ent = ent->fts_cycle;

  अगर (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_set_alloc ();
      अगर (!di_mnt)
        xalloc_die ();

      fill_mount_table ();
    }

  जबतक (ent && ent != cycle_ent)
    {
      अगर (di_set_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          वापस true;
        }
      ent = ent->fts_parent;
    }

  वापस false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

अटल bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  काष्ठा duinfo dui;
  काष्ठा duinfo dui_to_print;
  अटल idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  अटल काष्ठा dulevel *dulvl;

  अक्षर स्थिर *file = ent->fts_path;
  स्थिर काष्ठा stat *sb = ent->fts_statp;
  पूर्णांक info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_entry (ent);

  अगर (ignore_missing && ignorable_fts_error (ent))
    वापस true;

  अगर (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  अन्यथा अगर (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      अगर (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          अगर (info == FTS_NSOK)
            {
              fts_set (fts, ent, FTS_AGAIN);
              MAYBE_UNUSED FTSENT स्थिर *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              अगर (ignore_missing && ignorable_fts_error (ent))
                वापस true;
            }

          अगर (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              वापस false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          अगर (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      अगर (excluded
          || (! opt_गिनती_all
              && (hash_all || (! S_ISDIR (sb->st_mode) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          अगर (info == FTS_D)
            {
              fts_set (fts, ent, FTS_SKIP);
              MAYBE_UNUSED FTSENT स्थिर *e = fts_read (fts);
              affirm (e == ent);
            }

          वापस true;
        }

      चयन (info)
        {
        हाल FTS_D:
          वापस true;

        हाल FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          अवरोध;

        हाल FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          अगर (cycle_warning_required (fts, ent)
              && ! mount_point_in_fts_cycle (ent))
            {
              emit_cycle_warning (file);
              वापस false;
            }
          वापस true;
        }
    }

  duinfo_set (&dui,
              (apparent_आकार
               ? (usable_st_आकार (sb) ? MAX (0, sb->st_आकार) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (time_type == time_mtime ? get_stat_mtime (sb)
               : time_type == time_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_to_print = dui;

  अगर (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, माप *dulvl);
    }
  अन्यथा
    {
      अगर (level == पूर्व_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      अन्यथा अगर (level > पूर्व_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          अगर (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             माप *dulvl);

          क्रम (idx_t i = पूर्व_level + 1; i <= level; i++)
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
          affirm (level == पूर्व_level - 1);
          duinfo_add (&dui_to_print, &dulvl[पूर्व_level].ent);
          अगर (!opt_separate_dirs)
            duinfo_add (&dui_to_print, &dulvl[पूर्व_level].subdir);
          duinfo_add (&dulvl[level].subdir, &dulvl[पूर्व_level].ent);
          duinfo_add (&dulvl[level].subdir, &dulvl[पूर्व_level].subdir);
        }
    }

  पूर्व_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  अगर (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_add (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_add (&tot_dui, &dui);

  अगर ((IS_DIR_TYPE (info) && level <= max_depth)
      || (opt_all && level <= max_depth)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_to_print.inodes : dui_to_print.आकार;
      अगर (opt_threshold < 0
          ? v <= -opt_threshold
          : v >= opt_threshold)
        print_आकार (&dui_to_print, file, quote_output);
    }

  वापस ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

अटल bool
du_files (अक्षर **files, पूर्णांक bit_flags)
{
  bool ok = true;

  अगर (*files)
    {
      FTS *fts = xfts_open (files, bit_flags, शून्य);

      जबतक (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          अगर (ent == शून्य)
            {
              अगर (त्रुटि_सं != 0)
                {
                  error (0, त्रुटि_सं, _("fts_read failed: %s"),
                         quotef (fts->fts_path));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              पूर्व_level = 0;
              अवरोध;
            }

#अगर GNULIB_FTS_DEBUG
          अगर (fts_debug)
            fts_cross_check (fts);
#पूर्ण_अगर

          ok &= process_file (fts, ent);
        }

      अगर (fts_close (fts) != 0)
        {
          error (0, त्रुटि_सं, _("fts_close failed"));
          ok = false;
        }
    }

  वापस ok;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *cwd_only[2];
  bool max_depth_specified = false;
  bool ok = true;
  अक्षर *files_from = शून्य;

  /* Bit flags that control how fts works.  */
  पूर्णांक bit_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  पूर्णांक symlink_deref_bits = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  exclude = new_exclude ();

  human_options (दो_पर्या ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_block_आकार);

  जबतक (true)
    {
      पूर्णांक oi = -1;
      पूर्णांक c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
#अगर GNULIB_FTS_DEBUG
        हाल FTS_DEBUG:
          fts_debug = true;
          अवरोध;
#पूर्ण_अगर

        हाल '0':
          opt_nul_terminate_output = true;
          अवरोध;

        हाल 'a':
          opt_all = true;
          अवरोध;

        हाल 'A':
          apparent_आकार = true;
          अवरोध;

        हाल 'b':
          apparent_आकार = true;
          human_output_opts = 0;
          output_block_आकार = 1;
          अवरोध;

        हाल 'c':
          print_grand_योग = true;
          अवरोध;

        हाल 'h':
          human_output_opts = human_autoscale | human_SI | human_base_1024;
          output_block_आकार = 1;
          अवरोध;

        हाल HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_block_आकार = 1;
          अवरोध;

        हाल 'k':
          human_output_opts = 0;
          output_block_आकार = 1024;
          अवरोध;

        हाल 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            अगर (xstrtoimax (optarg, शून्य, 0, &tmp, "") == LONGINT_OK
                && ! ckd_add (&max_depth, tmp, 0) && 0 <= max_depth)
              max_depth_specified = true;
            अन्यथा
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          अवरोध;

        हाल 'm':
          human_output_opts = 0;
          output_block_आकार = 1024 * 1024;
          अवरोध;

        हाल 'l':
          opt_गिनती_all = true;
          अवरोध;

        हाल 's':
          opt_summarize_only = true;
          अवरोध;

        हाल 't':
          {
            क्रमागत strtol_error e;
            e = xstrtoimax (optarg, शून्य, 0, &opt_threshold,
                            "kKmMGTPEZYRQ0");
            अगर (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            अगर (opt_threshold == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (निकास_त्रुटि, 0, _("invalid --threshold argument '-0'"));
              }
          }
          अवरोध;

        हाल 'x':
          bit_flags |= FTS_XDEV;
          अवरोध;

        हाल 'B':
          {
            क्रमागत strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_block_आकार);
            अगर (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          अवरोध;

        हाल 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        हाल 'D':
          symlink_deref_bits = FTS_COMFOLLOW | FTS_PHYSICAL;
          अवरोध;

        हाल 'L': /* --dereference */
          symlink_deref_bits = FTS_LOGICAL;
          अवरोध;

        हाल 'P': /* --no-dereference */
          symlink_deref_bits = FTS_PHYSICAL;
          अवरोध;

        हाल 'S':
          opt_separate_dirs = true;
          अवरोध;

        हाल 'X':
          अगर (add_exclude_file (add_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, त्रुटि_सं, "%s", quotef (optarg));
              ok = false;
            }
          अवरोध;

        हाल FILES0_FROM_OPTION:
          files_from = optarg;
          अवरोध;

        हाल EXCLUDE_OPTION:
          add_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          अवरोध;

        हाल INODES_OPTION:
          opt_inodes = true;
          अवरोध;

        हाल TIME_OPTION:
          opt_time = true;
          time_type =
            (optarg
             ? XARGMATCH ("--time", optarg, time_args, time_types)
             : time_mtime);
          localtz = tzalloc (दो_पर्या ("TZ"));
          अवरोध;

        हाल TIME_STYLE_OPTION:
          time_style = optarg;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          ok = false;
        }
    }

  अगर (!ok)
    usage (निकास_त्रुटि);

  अगर (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
      अगर (qs != literal_quoting_style)
        {
          set_quoting_style (शून्य, qs);
          quote_output = true;
        }
    }

  अगर (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (निकास_त्रुटि);
    }

  अगर (opt_summarize_only && max_depth_specified && max_depth == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  अगर (opt_summarize_only && max_depth_specified && max_depth != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_depth);
      usage (निकास_त्रुटि);
    }

  अगर (opt_summarize_only)
    max_depth = 0;

  अगर (opt_inodes)
    {
      अगर (apparent_आकार)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_block_आकार = 1;
    }

  /* Process time style if printing last times.  */
  अगर (opt_time)
    {
      अगर (! time_style)
        {
          time_style = दो_पर्या ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          अगर (! time_style || streq (time_style, "locale"))
            time_style = "long-iso";
          अन्यथा अगर (*time_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              अक्षर स्थिर *p = म_अक्षर (time_style, '\n');
              अगर (p)
                time_style = xmemdup0 (time_style, p - time_style);
            }
          अन्यथा
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              अटल अक्षर स्थिर posix_prefix[] = "posix-";
              अटल स्थिर माप_प्रकार prefix_len = माप posix_prefix - 1;
              जबतक (STREQ_LEN (time_style, posix_prefix, prefix_len))
                time_style += prefix_len;
            }
        }

      अगर (*time_style == '+')
        time_format = time_style + 1;
      अन्यथा
        {
          चयन (x_timestyle_match (time_style, /*allow_posix=*/ false,
                                     time_style_args,
                                     (अक्षर स्थिर *) time_style_types,
                                     माप (*time_style_types), निकास_त्रुटि))
            {
            हाल full_iso_time_style:
              time_format = "%Y-%m-%d %H:%M:%S.%N %z";
              अवरोध;

            हाल long_iso_time_style:
              time_format = "%Y-%m-%d %H:%M";
              अवरोध;

            हाल iso_time_style:
              time_format = "%Y-%m-%d";
              अवरोध;
            }
        }
    }

  काष्ठा argv_iterator *ai;
  अगर (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      अगर (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (निकास_त्रुटि);
        }

      अगर (! (streq (files_from, "-") || ख_व_खोलो (files_from, "r", मानक_निवेश)))
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_stream (मानक_निवेश);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  अन्यथा
    {
      अक्षर **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_bits == FTS_LOGICAL);
    }

  अगर (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_set_alloc ();
  अगर (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  अगर (opt_गिनती_all || ! hash_all)
    bit_flags |= FTS_TIGHT_CYCLE_CHECK;

  bit_flags |= symlink_deref_bits;
  अटल अक्षर *अस्थायी_argv[] = { शून्य, शून्य };

  जबतक (true)
    {
      bool skip_file = false;
      क्रमागत argv_iter_err ai_err;
      अक्षर *file_name = argv_iter (ai, &ai_err);
      अगर (!file_name)
        {
          चयन (ai_err)
            {
            हाल AI_ERR_EOF:
              जाओ argv_iter_done;
            हाल AI_ERR_READ:
              error (0, त्रुटि_सं, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              जाओ argv_iter_done;
            हाल AI_ERR_MEM:
              xalloc_die ();
            हाल AI_ERR_OK: शेष:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      अगर (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      अगर (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          अगर (files_from == शून्य)
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

      अगर (skip_file)
        ok = false;
      अन्यथा
        {
          अस्थायी_argv[0] = file_name;
          ok &= du_files (अस्थायी_argv, bit_flags);
        }
    }
 argv_iter_done:

  argv_iter_free (ai);
  di_set_free (di_files);
  अगर (di_mnt)
    di_set_free (di_mnt);

  अगर (files_from && (ख_त्रुटि (मानक_निवेश) || ख_बंद (मानक_निवेश) != 0) && ok)
    error (निकास_त्रुटि, 0, _("error reading %s"), quoteaf (files_from));

  अगर (print_grand_योग)
    print_आकार (&tot_dui, _("total"), false);

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
