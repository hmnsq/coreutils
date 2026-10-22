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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argv-iter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "di-set.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "exclude.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fts-missing.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "human.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mountlist.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "show-date.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xmemdup0.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "du"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ਸਥਿਰ ਬਣਤਰ di_ਸੈੱਟ *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ਸਥਿਰ ਬਣਤਰ di_ਸੈੱਟ *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ਸਥਿਰ idx_t prev_level;

/* Define a class for collecting directory information. */
ਬਣਤਰ duinfo
{
  /* Size of files in directory.  */
  uintmax_t ਮਾਪ;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  ਬਣਤਰ timespec tmax;
};

/* Initialize directory data.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
duinfo_init (ਬਣਤਰ duinfo *a)
{
  a->ਮਾਪ = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
duinfo_ਸੈੱਟ (ਬਣਤਰ duinfo *a, uintmax_t ਮਾਪ, ਬਣਤਰ timespec tmax)
{
  a->ਮਾਪ = ਮਾਪ;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
duinfo_ਜੋੜੋ (ਬਣਤਰ duinfo *a, ਬਣਤਰ duinfo ਅਚਲ *b)
{
  uintmax_t ਜੋੜ = a->ਮਾਪ + b->ਮਾਪ;
  a->ਮਾਪ = a->ਮਾਪ <= ਜੋੜ ? ਜੋੜ : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  ਜੇ (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
ਬਣਤਰ dulevel
{
  /* Entries in this directory.  */
  ਬਣਤਰ duinfo ent;

  /* Total for subdirectories.  */
  ਬਣਤਰ duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
ਸਥਿਰ bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
ਸਥਿਰ bool apparent_ਮਾਪ = false;

/* If true, count each hard link of files with multiple links.  */
ਸਥਿਰ bool opt_ਗਿਣਤੀ_all = false;

/* If true, hash all files to look for hard links.  */
ਸਥਿਰ bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ਸਥਿਰ bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ਸਥਿਰ bool quote_output;

/* If true, print a grand total at the end.  */
ਸਥਿਰ bool print_grand_ਕੁੱਲ = false;

/* If nonzero, do not add sizes of subdirectories.  */
ਸਥਿਰ bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ਸਥਿਰ idx_t max_ਡੂੰਘਾਈ = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ਸਥਿਰ intmax_t opt_ਹੱਦ = 0;

/* Human-readable options for output.  */
ਸਥਿਰ ਪੂਰਨ human_output_opts;

/* Output inodes count instead of blocks used.  */
ਸਥਿਰ bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ਸਥਿਰ bool opt_ਸਮਾਂ = false;

/* Type of time to display. controlled by --time.  */

ਗਿਣਤੀ ਸਮਾਂ_type
  {
    ਸਮਾਂ_mtime,			/* default */
    ਸਮਾਂ_ctime,
    ਸਮਾਂ_atime
  };

ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_type ਸਮਾਂ_type = ਸਮਾਂ_mtime;

/* User specified date / time style */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਸਮਾਂ_style = ਨਿਰਾ;

/* Format used to display date / time. Controlled by --time-style */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਸਮਾਂ_format = ਨਿਰਾ;

/* The local time zone rules, as per the TZ environment variable.  */
ਸਥਿਰ timezone_t localtz;

/* The units to use when printing sizes.  */
ਸਥਿਰ uintmax_t output_ਖੰਡ_ਮਾਪ;

/* File name patterns to exclude.  */
ਸਥਿਰ ਬਣਤਰ exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ਸਥਿਰ ਬਣਤਰ duinfo tot_dui;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#ਜੇ GNULIB_FTS_DEBUG
  FTS_DEBUG,
#ਜੇ_ਅੰਤ
  ਸਮਾਂ_OPTION,
  ਸਮਾਂ_STYLE_OPTION,
  INODES_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"apparent-size", no_ਦਲੀਲ, ਨਿਰਾ, 'A'},
  {"block-size", required_ਦਲੀਲ, ਨਿਰਾ, 'B'},
  {"bytes", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"count-links", no_ਦਲੀਲ, ਨਿਰਾ, 'l'},
#ਜੇ GNULIB_FTS_DEBUG
  {"-debug", no_ਦਲੀਲ, ਨਿਰਾ, FTS_DEBUG},
#ਜੇ_ਅੰਤ
  {"dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"dereference-args", no_ਦਲੀਲ, ਨਿਰਾ, 'D'},
  {"exclude", required_ਦਲੀਲ, ਨਿਰਾ, EXCLUDE_OPTION},
  {"exclude-from", required_ਦਲੀਲ, ਨਿਰਾ, 'X'},
  {"files0-from", required_ਦਲੀਲ, ਨਿਰਾ, FILES0_FROM_OPTION},
  {"human-readable", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"inodes", no_ਦਲੀਲ, ਨਿਰਾ, INODES_OPTION},
  {"si", no_ਦਲੀਲ, ਨਿਰਾ, HUMAN_SI_OPTION},
  {"max-depth", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"null", no_ਦਲੀਲ, ਨਿਰਾ, '0'},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'P'},
  {"one-file-system", no_ਦਲੀਲ, ਨਿਰਾ, 'x'},
  {"separate-dirs", no_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"summarize", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"total", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"threshold", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"time", optional_ਦਲੀਲ, ਨਿਰਾ, ਸਮਾਂ_OPTION},
  {"time-style", required_ਦਲੀਲ, ਨਿਰਾ, ਸਮਾਂ_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_ਦਲੀਲਾਂ[] =
{
  "atime", "access", "use", "ctime", "status", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_type ਅਚਲ ਸਮਾਂ_types[] =
{
  ਸਮਾਂ_atime, ਸਮਾਂ_atime, ਸਮਾਂ_atime, ਸਮਾਂ_ctime, ਸਮਾਂ_ctime
};
ARGMATCH_VERIFY (ਸਮਾਂ_ਦਲੀਲਾਂ, ਸਮਾਂ_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
ਗਿਣਤੀ ਸਮਾਂ_style
  {
    ਭਰਿਆ_iso_ਸਮਾਂ_style,       /* --time-style=full-iso */
    long_iso_ਸਮਾਂ_style,       /* --time-style=long-iso */
    iso_ਸਮਾਂ_style	       /* --time-style=iso */
  };

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_style_ਦਲੀਲਾਂ[] =
{
  "full-iso", "long-iso", "iso", ਨਿਰਾ
};
ਸਥਿਰ ਗਿਣਤੀ ਸਮਾਂ_style ਅਚਲ ਸਮਾਂ_style_types[] =
{
  ਭਰਿਆ_iso_ਸਮਾਂ_style, long_iso_ਸਮਾਂ_style, iso_ਸਮਾਂ_style
};
ARGMATCH_VERIFY (ਸਮਾਂ_style_ਦਲੀਲਾਂ, ਸਮਾਂ_style_types);

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Summarize device usage ਦਾ the ਸੈੱਟ_2 ਦਾ FILEs, recursively ਲਈ directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         ਲਿਖ counts ਲਈ all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-ਮਾਪ\n\
         print apparent sizes rather than device usage;\n\
         although the apparent ਮਾਪ is usually smaller, it may be\n\
         larger due ਮੰਜ਼ਿਲ holes ਵਿੱਚ ('sparse') files,\n\
         ਅੰਦਰੂਨੀ fragmentation, ਅਸਿੱਧਾ ਖੰਡਾਂ, etc.\n\
"));
      oputs (_("\
  -B, --ਖੰਡ-ਮਾਪ=ਮਾਪ_2\n\
         scale sizes by ਮਾਪ_2 before printing them; See ਮਾਪ_2 format ਹੇਠਾਂ;\n\
         E.g., '-BM' prints sizes ਵਿੱਚ units ਦਾ 1,048,576 ਬਾਈਟਾਂ\n\
"));
      oputs (_("\
  -b, --ਬਾਈਟਾਂ\n\
         equivalent ਮੰਜ਼ਿਲ '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --ਕੁੱਲ\n\
         produce a grand ਕੁੱਲ\n\
"));
      oputs (_("\
  -D, --dereference-ਦਲੀਲਾਂ\n\
         dereference only symlinks that are listed on the command ਸਤਰ\n\
"));
      oputs (_("\
  -d, --max-ਡੂੰਘਾਈ=N\n\
         print the ਕੁੱਲ ਲਈ a directory (or file, with --all)\n\
         only ਜੇ it is N or fewer ਪੱਧਰ ਹੇਠਾਂ the command\n\
         ਸਤਰ ਦਲੀਲ;  --max-ਡੂੰਘਾਈ=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage ਦਾ the NUL-terminated file ਨਾਂ\n\
         specified ਵਿੱਚ file F;  ਜੇ F is -, ਪਠ ਨਾਂ from standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -H\n\
         equivalent ਮੰਜ਼ਿਲ --dereference-ਦਲੀਲਾਂ (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes ਵਿੱਚ human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead ਦਾ ਖੰਡ usage\n\
"));
      oputs (_("\
  -k\n\
         like --ਖੰਡ-ਮਾਪ=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --ਗਿਣਤੀ_2-links\n\
         ਗਿਣਤੀ_2 sizes many times ਜੇ hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --ਖੰਡ-ਮਾਪ=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the ਮੂਲ)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         ਲਈ directories ਕਰੋ not include ਮਾਪ ਦਾ subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers ਦਾ 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a ਕੁੱਲ ਲਈ each ਦਲੀਲ\n\
"));
      oputs (_("\
  -t, --ਹੱਦ=ਮਾਪ_2\n\
         exclude entries smaller than ਮਾਪ_2 ਜੇ ਸਕਾਰਾਤਮਕ,\n\
         or entries greater than ਮਾਪ_2 ਜੇ negative\n\
"));
      oputs (_("\
      --ਸਮਾਂ\n\
         show ਸਮਾਂ ਦਾ the ਆਖਰੀ modification ਦਾ any file ਵਿੱਚ the directory,\n\
         or any ਦਾ its subdirectories\n\
"));
      oputs (_("\
      --ਸਮਾਂ=ਸ਼ਬਦ\n\
         show ਸਮਾਂ as ਸ਼ਬਦ instead ਦਾ modification ਸਮਾਂ:\n\
         atime, access, use, ctime or ਸਥਿਤੀ\n\
"));
      oputs (_("\
      --ਸਮਾਂ-style=STYLE\n\
         ਸਮਾਂ/date format with --ਸਮਾਂ; see ਸਮਾਂ_STYLE ਹੇਠਾਂ\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that ਮੇਲ any ਨਮੂਨਾ_2 ਵਿੱਚ FILE\n\
"));
      oputs (_("\
      --exclude=ਨਮੂਨਾ\n\
         exclude files that ਮੇਲ ਨਮੂਨਾ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_ਮਾਪ_note ();
      fputs (_("\
\n\
The --ਸਮਾਂ-style STYLE ਦਲੀਲ can be ਭਰਿਆ-iso, ਲੰਮਾ-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like ਵਿੱਚ date(1).\n\
Also the ਸਮਾਂ_STYLE environment variable sets the ਮੂਲ style ਮੰਜ਼ਿਲ use.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ਸਥਿਰ bool
hash_ins (ਬਣਤਰ di_ਸੈੱਟ *di_ਸੈੱਟ, ino_t ino, dev_t dev)
{
  ਪੂਰਨ inserted = di_ਸੈੱਟ_ਪਾਓ (di_ਸੈੱਟ, dev, ino);
  ਜੇ (inserted < 0)
    xalloc_die ();
  ਵਾਪਸ inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ਸਥਿਰ ਖਾਲੀ
print_only_ਮਾਪ (uintmax_t n_ਬਾਈਟਾਂ)
{
  ਅੱਖਰ buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_ਬਾਈਟਾਂ == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_ਬਾਈਟਾਂ, buf, human_output_opts,
                            1, output_ਖੰਡ_ਮਾਪ)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ਸਥਿਰ ਖਾਲੀ
print_ਮਾਪ (ਅਚਲ ਬਣਤਰ duinfo *pdui, ਅੱਖਰ ਅਚਲ *string, bool quote)
{
  print_only_ਮਾਪ (opt_inodes
                   ? pdui->inodes
                   : pdui->ਮਾਪ);

  ਜੇ (opt_ਸਮਾਂ)
    {
      ਅੱਖਰ_ਲਿਖੋ ('\t');
      bool ok = show_date (ਸਮਾਂ_format, pdui->tmax, localtz);
      ਜੇ (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          ਅੱਖਰ buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  ਅੱਖਰ_ਲਿਖੋ ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  ਅੱਖਰ_ਲਿਖੋ (opt_nul_terminate_output ? '\0' : '\n');
  ਜੇ (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ਸਥਿਰ ਖਾਲੀ
fill_mount_ਸਾਰਣੀ (ਖਾਲੀ)
{
  ਬਣਤਰ mount_ਇੰਦਰਾਜ *mnt_ent = read_file_system_list (false);
  ਜਦੋਂ (mnt_ent)
    {
      ਬਣਤਰ mount_ਇੰਦਰਾਜ *mnt_free;
      ਜੇ (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          ਬਣਤਰ stat buf;
          ਜੇ (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          ਨਹੀਂਤਾਂ
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_ਅਗਲਾ;
      free_mount_ਇੰਦਰਾਜ (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

ਸਥਿਰ bool
mount_ਬਿੰਦੂ_ਵਿੱਚ_fts_ਚੱਕਰ (FTSENT ਅਚਲ *ent)
{
  FTSENT ਅਚਲ *ਚੱਕਰ_ent = ent->fts_ਚੱਕਰ;

  ਜੇ (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_ਸੈੱਟ_alloc ();
      ਜੇ (!di_mnt)
        xalloc_die ();

      fill_mount_ਸਾਰਣੀ ();
    }

  ਜਦੋਂ (ent && ent != ਚੱਕਰ_ent)
    {
      ਜੇ (di_ਸੈੱਟ_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          ਵਾਪਸ true;
        }
      ent = ent->fts_ਮਾਪਾ;
    }

  ਵਾਪਸ false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

ਸਥਿਰ bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  ਬਣਤਰ duinfo dui;
  ਬਣਤਰ duinfo dui_ਮੰਜ਼ਿਲ_print;
  ਸਥਿਰ idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  ਸਥਿਰ ਬਣਤਰ dulevel *dulvl;

  ਅੱਖਰ ਅਚਲ *file = ent->fts_ਰਸਤਾ;
  ਅਚਲ ਬਣਤਰ stat *sb = ent->fts_statp;
  ਪੂਰਨ info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_ਇੰਦਰਾਜ (ent);

  ਜੇ (ignore_missing && ignorable_fts_error (ent))
    ਵਾਪਸ true;

  ਜੇ (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  ਨਹੀਂਤਾਂ ਜੇ (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      ਜੇ (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          ਜੇ (info == FTS_NSOK)
            {
              fts_ਸੈੱਟ (fts, ent, FTS_AGAIN);
              MAYBE_ਅਣਵਰਤਿਆ FTSENT ਅਚਲ *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              ਜੇ (ignore_missing && ignorable_fts_error (ent))
                ਵਾਪਸ true;
            }

          ਜੇ (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              ਵਾਪਸ false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          ਜੇ (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      ਜੇ (excluded
          || (! opt_ਗਿਣਤੀ_all
              && (hash_all || (! S_ISDIR (sb->st_ਬਹੁਲਕ) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          ਜੇ (info == FTS_D)
            {
              fts_ਸੈੱਟ (fts, ent, FTS_SKIP);
              MAYBE_ਅਣਵਰਤਿਆ FTSENT ਅਚਲ *e = fts_read (fts);
              affirm (e == ent);
            }

          ਵਾਪਸ true;
        }

      ਚੋਣ (info)
        {
        ਹਾਲਤ FTS_D:
          ਵਾਪਸ true;

        ਹਾਲਤ FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          ਰੋਕੋ;

        ਹਾਲਤ FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          ਜੇ (ਚੱਕਰ_warning_required (fts, ent)
              && ! mount_ਬਿੰਦੂ_ਵਿੱਚ_fts_ਚੱਕਰ (ent))
            {
              emit_ਚੱਕਰ_warning (file);
              ਵਾਪਸ false;
            }
          ਵਾਪਸ true;
        }
    }

  duinfo_ਸੈੱਟ (&dui,
              (apparent_ਮਾਪ
               ? (usable_st_ਮਾਪ (sb) ? MAX (0, sb->st_ਮਾਪ) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (ਸਮਾਂ_type == ਸਮਾਂ_mtime ? get_stat_mtime (sb)
               : ਸਮਾਂ_type == ਸਮਾਂ_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_ਮੰਜ਼ਿਲ_print = dui;

  ਜੇ (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, ਆਕਾਰ *dulvl);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      ਨਹੀਂਤਾਂ ਜੇ (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          ਜੇ (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             ਆਕਾਰ *dulvl);

          ਲਈ (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      ਨਹੀਂਤਾਂ /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_ਜੋੜੋ (&dui_ਮੰਜ਼ਿਲ_print, &dulvl[prev_level].ent);
          ਜੇ (!opt_separate_dirs)
            duinfo_ਜੋੜੋ (&dui_ਮੰਜ਼ਿਲ_print, &dulvl[prev_level].subdir);
          duinfo_ਜੋੜੋ (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_ਜੋੜੋ (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  ਜੇ (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_ਜੋੜੋ (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_ਜੋੜੋ (&tot_dui, &dui);

  ਜੇ ((IS_DIR_TYPE (info) && level <= max_ਡੂੰਘਾਈ)
      || (opt_all && level <= max_ਡੂੰਘਾਈ)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_ਮੰਜ਼ਿਲ_print.inodes : dui_ਮੰਜ਼ਿਲ_print.ਮਾਪ;
      ਜੇ (opt_ਹੱਦ < 0
          ? v <= -opt_ਹੱਦ
          : v >= opt_ਹੱਦ)
        print_ਮਾਪ (&dui_ਮੰਜ਼ਿਲ_print, file, quote_output);
    }

  ਵਾਪਸ ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ਸਥਿਰ bool
du_files (ਅੱਖਰ **files, ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags)
{
  bool ok = true;

  ਜੇ (*files)
    {
      FTS *fts = xfts_open (files, ਦੋਆਧਾਰੀ_ਅੰਕ_flags, ਨਿਰਾ);

      ਜਦੋਂ (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ਜੇ (ent == ਨਿਰਾ)
            {
              ਜੇ (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_ਰਸਤਾ));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              ਰੋਕੋ;
            }

#ਜੇ GNULIB_FTS_DEBUG
          ਜੇ (fts_debug)
            fts_cross_check (fts);
#ਜੇ_ਅੰਤ

          ok &= process_file (fts, ent);
        }

      ਜੇ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  ਵਾਪਸ ok;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *cwd_only[2];
  bool max_ਡੂੰਘਾਈ_specified = false;
  bool ok = true;
  ਅੱਖਰ *files_from = ਨਿਰਾ;

  /* Bit flags that control how fts works.  */
  ਪੂਰਨ ਦੋਆਧਾਰੀ_ਅੰਕ_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  ਪੂਰਨ symlink_deref_ਬਿੱਟ = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_ਖੰਡ_ਮਾਪ);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ oi = -1;
      ਪੂਰਨ c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
#ਜੇ GNULIB_FTS_DEBUG
        ਹਾਲਤ FTS_DEBUG:
          fts_debug = true;
          ਰੋਕੋ;
#ਜੇ_ਅੰਤ

        ਹਾਲਤ '0':
          opt_nul_terminate_output = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'a':
          opt_all = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'A':
          apparent_ਮਾਪ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'b':
          apparent_ਮਾਪ = true;
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          print_grand_ਕੁੱਲ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'h':
          human_output_opts = human_autoscale | human_SI | human_ਅਧਾਰ_1024;
          output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;

        ਹਾਲਤ HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_ਖੰਡ_ਮਾਪ = 1;
          ਰੋਕੋ;

        ਹਾਲਤ 'k':
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1024;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            ਜੇ (xstrtoimax (optarg, ਨਿਰਾ, 0, &tmp, "") == LONGINT_OK
                && ! ckd_ਜੋੜੋ (&max_ਡੂੰਘਾਈ, tmp, 0) && 0 <= max_ਡੂੰਘਾਈ)
              max_ਡੂੰਘਾਈ_specified = true;
            ਨਹੀਂਤਾਂ
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          human_output_opts = 0;
          output_ਖੰਡ_ਮਾਪ = 1024 * 1024;
          ਰੋਕੋ;

        ਹਾਲਤ 'l':
          opt_ਗਿਣਤੀ_all = true;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          opt_summarize_only = true;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          {
            ਗਿਣਤੀ strtol_error e;
            e = xstrtoimax (optarg, ਨਿਰਾ, 0, &opt_ਹੱਦ,
                            "kKmMGTPEZYRQ0");
            ਜੇ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            ਜੇ (opt_ਹੱਦ == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'x':
          ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= FTS_XDEV;
          ਰੋਕੋ;

        ਹਾਲਤ 'B':
          {
            ਗਿਣਤੀ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ਖੰਡ_ਮਾਪ);
            ਜੇ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        ਹਾਲਤ 'D':
          symlink_deref_ਬਿੱਟ = FTS_COMFOLLOW | FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'L': /* --dereference */
          symlink_deref_ਬਿੱਟ = FTS_LOGICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'P': /* --no-dereference */
          symlink_deref_ਬਿੱਟ = FTS_PHYSICAL;
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          opt_separate_dirs = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'X':
          ਜੇ (ਜੋੜੋ_exclude_file (ਜੋੜੋ_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          ਰੋਕੋ;

        ਹਾਲਤ FILES0_FROM_OPTION:
          files_from = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ EXCLUDE_OPTION:
          ਜੋੜੋ_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          ਰੋਕੋ;

        ਹਾਲਤ INODES_OPTION:
          opt_inodes = true;
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮਾਂ_OPTION:
          opt_ਸਮਾਂ = true;
          ਸਮਾਂ_type =
            (optarg
             ? XARGMATCH ("--time", optarg, ਸਮਾਂ_ਦਲੀਲਾਂ, ਸਮਾਂ_types)
             : ਸਮਾਂ_mtime);
          localtz = tzalloc (getenv ("TZ"));
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮਾਂ_STYLE_OPTION:
          ਸਮਾਂ_style = optarg;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          ok = false;
        }
    }

  ਜੇ (!ok)
    usage (EXIT_FAILURE);

  ਜੇ (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      ਪੂਰਨ qs = getenv_quoting_style ();
      ਜੇ (qs < 0)
        qs = shell_escape_quoting_style;
      ਜੇ (qs != literal_quoting_style)
        {
          ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
          quote_output = true;
        }
    }

  ਜੇ (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (opt_summarize_only && max_ਡੂੰਘਾਈ_specified && max_ਡੂੰਘਾਈ == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  ਜੇ (opt_summarize_only && max_ਡੂੰਘਾਈ_specified && max_ਡੂੰਘਾਈ != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_ਡੂੰਘਾਈ);
      usage (EXIT_FAILURE);
    }

  ਜੇ (opt_summarize_only)
    max_ਡੂੰਘਾਈ = 0;

  ਜੇ (opt_inodes)
    {
      ਜੇ (apparent_ਮਾਪ)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_ਖੰਡ_ਮਾਪ = 1;
    }

  /* Process time style if printing last times.  */
  ਜੇ (opt_ਸਮਾਂ)
    {
      ਜੇ (! ਸਮਾਂ_style)
        {
          ਸਮਾਂ_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          ਜੇ (! ਸਮਾਂ_style || streq (ਸਮਾਂ_style, "locale"))
            ਸਮਾਂ_style = "long-iso";
          ਨਹੀਂਤਾਂ ਜੇ (*ਸਮਾਂ_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              ਅੱਖਰ ਅਚਲ *p = strchr (ਸਮਾਂ_style, '\n');
              ਜੇ (p)
                ਸਮਾਂ_style = xmemdup0 (ਸਮਾਂ_style, p - ਸਮਾਂ_style);
            }
          ਨਹੀਂਤਾਂ
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ਸਥਿਰ ਅੱਖਰ ਅਚਲ posix_ਅਗੇਤਰ[] = "posix-";
              ਸਥਿਰ ਅਚਲ size_t ਅਗੇਤਰ_len = ਆਕਾਰ posix_ਅਗੇਤਰ - 1;
              ਜਦੋਂ (STREQ_LEN (ਸਮਾਂ_style, posix_ਅਗੇਤਰ, ਅਗੇਤਰ_len))
                ਸਮਾਂ_style += ਅਗੇਤਰ_len;
            }
        }

      ਜੇ (*ਸਮਾਂ_style == '+')
        ਸਮਾਂ_format = ਸਮਾਂ_style + 1;
      ਨਹੀਂਤਾਂ
        {
          ਚੋਣ (x_timestyle_ਮੇਲ (ਸਮਾਂ_style, /*allow_posix=*/ false,
                                     ਸਮਾਂ_style_ਦਲੀਲਾਂ,
                                     (ਅੱਖਰ ਅਚਲ *) ਸਮਾਂ_style_types,
                                     ਆਕਾਰ (*ਸਮਾਂ_style_types), EXIT_FAILURE))
            {
            ਹਾਲਤ ਭਰਿਆ_iso_ਸਮਾਂ_style:
              ਸਮਾਂ_format = "%Y-%m-%d %H:%M:%S.%N %z";
              ਰੋਕੋ;

            ਹਾਲਤ long_iso_ਸਮਾਂ_style:
              ਸਮਾਂ_format = "%Y-%m-%d %H:%M";
              ਰੋਕੋ;

            ਹਾਲਤ iso_ਸਮਾਂ_style:
              ਸਮਾਂ_format = "%Y-%m-%d";
              ਰੋਕੋ;
            }
        }
    }

  ਬਣਤਰ argv_iterator *ai;
  ਜੇ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ਜੇ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ਜੇ (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_ਧਾਰਾ (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_ਬਿੱਟ == FTS_LOGICAL);
    }

  ਜੇ (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_ਸੈੱਟ_alloc ();
  ਜੇ (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  ਜੇ (opt_ਗਿਣਤੀ_all || ! hash_all)
    ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= FTS_TIGHT_ਚੱਕਰ_CHECK;

  ਦੋਆਧਾਰੀ_ਅੰਕ_flags |= symlink_deref_ਬਿੱਟ;
  ਸਥਿਰ ਅੱਖਰ *ਆਰਜ਼ੀ_argv[] = { ਨਿਰਾ, ਨਿਰਾ };

  ਜਦੋਂ (true)
    {
      bool skip_file = false;
      ਗਿਣਤੀ argv_iter_err ai_err;
      ਅੱਖਰ *file_name = argv_iter (ai, &ai_err);
      ਜੇ (!file_name)
        {
          ਚੋਣ (ai_err)
            {
            ਹਾਲਤ AI_ERR_EOF:
              ਜਾਓ argv_iter_ਪੂਰਾ;
            ਹਾਲਤ AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              ਜਾਓ argv_iter_ਪੂਰਾ;
            ਹਾਲਤ AI_ERR_MEM:
              xalloc_die ();
            ਹਾਲਤ AI_ERR_OK: ਮੂਲ:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      ਜੇ (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      ਜੇ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ਜੇ (files_from == ਨਿਰਾ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ਨਹੀਂਤਾਂ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_ਸੰਖਿਆ = argv_iter_n_ਦਲੀਲਾਂ (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_ਸੰਖਿਆ, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ਜੇ (skip_file)
        ok = false;
      ਨਹੀਂਤਾਂ
        {
          ਆਰਜ਼ੀ_argv[0] = file_name;
          ok &= du_files (ਆਰਜ਼ੀ_argv, ਦੋਆਧਾਰੀ_ਅੰਕ_flags);
        }
    }
 argv_iter_ਪੂਰਾ:

  argv_iter_free (ai);
  di_ਸੈੱਟ_free (di_files);
  ਜੇ (di_mnt)
    di_ਸੈੱਟ_free (di_mnt);

  ਜੇ (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  ਜੇ (print_grand_ਕੁੱਲ)
    print_ਮਾਪ (&tot_dui, _("total"), false);

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
