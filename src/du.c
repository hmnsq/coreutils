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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argv-iter.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "di-set.h"
#ಅನ್ತರ್ಭಾವ "exclude.h"
#ಅನ್ತರ್ಭಾವ "fts-missing.h"
#ಅನ್ತರ್ಭಾವ "human.h"
#ಅನ್ತರ್ಭಾವ "mountlist.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "show-date.h"
#ಅನ್ತರ್ಭಾವ "stat-size.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"
#ಅನ್ತರ್ಭಾವ "xmemdup0.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "du"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Paul Eggert"), \
  proper_name ("Jim Meyering")

/* A set of dev/ino pairs to help identify files and directories
   whose sizes have already been counted.  */
ಸ್ಥಿರ ರಚನೆ di_ಗಣ *di_files;

/* A set containing a dev/ino pair for each local mount point directory.  */
ಸ್ಥಿರ ರಚನೆ di_ಗಣ *di_mnt;

/* Keep track of the preceding "level" (depth in hierarchy)
   from one call of process_file to the next.  */
ಸ್ಥಿರ idx_t prev_level;

/* Define a class for collecting directory information. */
ರಚನೆ duinfo
{
  /* Size of files in directory.  */
  uintmax_t ಗಾತ್ರ_2;

  /* Number of inodes in directory.  */
  uintmax_t inodes;

  /* Latest timestamp found.  If tmax.tv_sec == TYPE_MINIMUM (time_t)
     && tmax.tv_nsec < 0, no timestamp has been found.  */
  ರಚನೆ timespec tmax;
};

/* Initialize directory data.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
duinfo_init (ರಚನೆ duinfo *a)
{
  a->ಗಾತ್ರ_2 = 0;
  a->inodes = 0;
  a->tmax.tv_sec = TYPE_MINIMUM (time_t);
  a->tmax.tv_nsec = -1;
}

/* Set directory data.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
duinfo_ಗಣ (ರಚನೆ duinfo *a, uintmax_t ಗಾತ್ರ_2, ರಚನೆ timespec tmax)
{
  a->ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
  a->inodes = 1;
  a->tmax = tmax;
}

/* Accumulate directory data.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
duinfo_ಕೂಡಿಸು (ರಚನೆ duinfo *a, ರಚನೆ duinfo ಸ್ಥಿರಾಂಕ *b)
{
  uintmax_t ಮೊತ್ತ = a->ಗಾತ್ರ_2 + b->ಗಾತ್ರ_2;
  a->ಗಾತ್ರ_2 = a->ಗಾತ್ರ_2 <= ಮೊತ್ತ ? ಮೊತ್ತ : UINTMAX_MAX;
  a->inodes = a->inodes + b->inodes;
  ಆದರೆ (timespec_cmp (a->tmax, b->tmax) < 0)
    a->tmax = b->tmax;
}

/* A structure for per-directory level information.  */
ರಚನೆ dulevel
{
  /* Entries in this directory.  */
  ರಚನೆ duinfo ent;

  /* Total for subdirectories.  */
  ರಚನೆ duinfo subdir;
};

/* If true, display counts for all files, not just directories.  */
ಸ್ಥಿರ bool opt_all = false;

/* If true, rather than using the device usage of each file,
   use the apparent size (stat.st_size if usable, 0 otherwise).  */
ಸ್ಥಿರ bool apparent_ಗಾತ್ರ = false;

/* If true, count each hard link of files with multiple links.  */
ಸ್ಥಿರ bool opt_ಎಣಿಕೆ_all = false;

/* If true, hash all files to look for hard links.  */
ಸ್ಥಿರ bool hash_all;

/* If true, output the NUL byte instead of a newline at the end of each line. */
ಸ್ಥಿರ bool opt_nul_terminate_output = false;

/* If true, quote output pathnames according to the selected quoting style.  */
ಸ್ಥಿರ bool quote_output;

/* If true, print a grand total at the end.  */
ಸ್ಥಿರ bool print_grand_ಒಟ್ಟು = false;

/* If nonzero, do not add sizes of subdirectories.  */
ಸ್ಥಿರ bool opt_separate_dirs = false;

/* Show the total for each directory (and file if --all) that is at
   most MAX_DEPTH levels down from the root of the hierarchy.  The root
   is at level 0, so 'du --max-depth=0' is equivalent to 'du -s'.  */
ಸ್ಥಿರ idx_t max_ಆಳ = IDX_MAX;

/* Only output entries with at least this SIZE if positive,
   or at most if negative.  See --threshold option.  */
ಸ್ಥಿರ intmax_t opt_ಮಿತಿ = 0;

/* Human-readable options for output.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ human_output_opts;

/* Output inodes count instead of blocks used.  */
ಸ್ಥಿರ bool opt_inodes = false;

/* If true, print most recently modified date, using the specified format.  */
ಸ್ಥಿರ bool opt_ಸಮಯ = false;

/* Type of time to display. controlled by --time.  */

ಎಣಿಕೆ ಸಮಯ_type
  {
    ಸಮಯ_mtime,			/* default */
    ಸಮಯ_ctime,
    ಸಮಯ_atime
  };

ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_type ಸಮಯ_type = ಸಮಯ_mtime;

/* User specified date / time style */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಮಯ_style = ಖಾಲಿ;

/* Format used to display date / time. Controlled by --time-style */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಮಯ_format = ಖಾಲಿ;

/* The local time zone rules, as per the TZ environment variable.  */
ಸ್ಥಿರ timezone_t localtz;

/* The units to use when printing sizes.  */
ಸ್ಥಿರ uintmax_t output_ಖಂಡ_ಗಾತ್ರ;

/* File name patterns to exclude.  */
ಸ್ಥಿರ ರಚನೆ exclude *exclude;

/* Grand total size of all args, in bytes. Also latest modified date. */
ಸ್ಥಿರ ರಚನೆ duinfo tot_dui;

#ವ್ಯಾಖ್ಯಾನಿಸು IS_DIR_TYPE(Type)	\
  ((Type) == FTS_DP		\
   || (Type) == FTS_DNR)

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  EXCLUDE_OPTION = CHAR_MAX + 1,
  FILES0_FROM_OPTION,
  HUMAN_SI_OPTION,
#ಆದರೆ GNULIB_FTS_DEBUG
  FTS_DEBUG,
#ಆದರೆ_ಅಂತ್ಯ
  ಸಮಯ_OPTION,
  ಸಮಯ_STYLE_OPTION,
  INODES_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"apparent-size", no_ವಾದ, ಖಾಲಿ, 'A'},
  {"block-size", required_ವಾದ, ಖಾಲಿ, 'B'},
  {"bytes", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"count-links", no_ವಾದ, ಖಾಲಿ, 'l'},
#ಆದರೆ GNULIB_FTS_DEBUG
  {"-debug", no_ವಾದ, ಖಾಲಿ, FTS_DEBUG},
#ಆದರೆ_ಅಂತ್ಯ
  {"dereference", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"dereference-args", no_ವಾದ, ಖಾಲಿ, 'D'},
  {"exclude", required_ವಾದ, ಖಾಲಿ, EXCLUDE_OPTION},
  {"exclude-from", required_ವಾದ, ಖಾಲಿ, 'X'},
  {"files0-from", required_ವಾದ, ಖಾಲಿ, FILES0_FROM_OPTION},
  {"human-readable", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"inodes", no_ವಾದ, ಖಾಲಿ, INODES_OPTION},
  {"si", no_ವಾದ, ಖಾಲಿ, HUMAN_SI_OPTION},
  {"max-depth", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"null", no_ವಾದ, ಖಾಲಿ, '0'},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'P'},
  {"one-file-system", no_ವಾದ, ಖಾಲಿ, 'x'},
  {"separate-dirs", no_ವಾದ, ಖಾಲಿ, 'S'},
  {"summarize", no_ವಾದ, ಖಾಲಿ, 's'},
  {"total", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"threshold", required_ವಾದ, ಖಾಲಿ, 't'},
  {"time", optional_ವಾದ, ಖಾಲಿ, ಸಮಯ_OPTION},
  {"time-style", required_ವಾದ, ಖಾಲಿ, ಸಮಯ_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_ವಾದಗಳು[] =
{
  "atime", "access", "use", "ctime", "status", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_type ಸ್ಥಿರಾಂಕ ಸಮಯ_types[] =
{
  ಸಮಯ_atime, ಸಮಯ_atime, ಸಮಯ_atime, ಸಮಯ_ctime, ಸಮಯ_ctime
};
ARGMATCH_VERIFY (ಸಮಯ_ವಾದಗಳು, ಸಮಯ_types);

/* 'full-iso' uses full ISO-style dates and times.  'long-iso' uses longer
   ISO-style timestamps, though shorter than 'full-iso'.  'iso' uses shorter
   ISO-style timestamps.  */
ಎಣಿಕೆ ಸಮಯ_style
  {
    ತುಂಬಿದ_iso_ಸಮಯ_style,       /* --time-style=full-iso */
    long_iso_ಸಮಯ_style,       /* --time-style=long-iso */
    iso_ಸಮಯ_style	       /* --time-style=iso */
  };

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_style_ವಾದಗಳು[] =
{
  "full-iso", "long-iso", "iso", ಖಾಲಿ
};
ಸ್ಥಿರ ಎಣಿಕೆ ಸಮಯ_style ಸ್ಥಿರಾಂಕ ಸಮಯ_style_types[] =
{
  ತುಂಬಿದ_iso_ಸಮಯ_style, long_iso_ಸಮಯ_style, iso_ಸಮಯ_style
};
ARGMATCH_VERIFY (ಸಮಯ_style_ವಾದಗಳು, ಸಮಯ_style_types);

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"), ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Summarize device usage ದ the ಗಣ_2 ದ FILEs, recursively ಗಾಗಿ directories.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -0, --null\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
  -a, --all\n\
         ಲಿಖ counts ಗಾಗಿ all files, not just directories\n\
"));
      oputs (_("\
  -A, --apparent-ಗಾತ್ರ_2\n\
         print apparent sizes rather than device usage;\n\
         although the apparent ಗಾತ್ರ_2 is usually smaller, it may be\n\
         larger due ಗಮ್ಯ holes ಒಳ ('sparse') files,\n\
         ಆಂತರಿಕ fragmentation, ಪರೋಕ್ಷ ಖಂಡಗಳು, etc.\n\
"));
      oputs (_("\
  -B, --ಖಂಡ-ಗಾತ್ರ_2=ಗಾತ್ರ_3\n\
         scale sizes by ಗಾತ್ರ_3 before printing them; See ಗಾತ್ರ_3 format ಕೆಳಗೆ;\n\
         E.g., '-BM' prints sizes ಒಳ units ದ 1,048,576 ಬೈಟುಗಳು\n\
"));
      oputs (_("\
  -b, --ಬೈಟುಗಳು\n\
         equivalent ಗಮ್ಯ '--apparent-size --block-size=1'\n\
"));
      oputs (_("\
  -c, --ಒಟ್ಟು\n\
         produce a grand ಒಟ್ಟು\n\
"));
      oputs (_("\
  -D, --dereference-ವಾದಗಳು\n\
         dereference only symlinks that are listed on the command ಸಾಲು\n\
"));
      oputs (_("\
  -d, --max-ಆಳ=N\n\
         print the ಒಟ್ಟು ಗಾಗಿ a directory (or file, with --all)\n\
         only ಆದರೆ it is N or fewer ಮಟ್ಟಗಳು ಕೆಳಗೆ the command\n\
         ಸಾಲು ವಾದ;  --max-ಆಳ=0 is the same as --summarize\n\
"));
      oputs (_("\
      --files0-from=F\n\
         summarize device usage ದ the NUL-terminated file ಹೆಸರುಗಳು\n\
         specified ಒಳ file F;  ಆದರೆ F is -, ಪಠ ಹೆಸರುಗಳು from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -H\n\
         equivalent ಗಮ್ಯ --dereference-ವಾದಗಳು (-D)\n\
"));
      oputs (_("\
  -h, --human-readable\n\
         print sizes ಒಳ human readable format (e.g., 1K 234M 2G)\n\
"));
      oputs (_("\
      --inodes\n\
         list inode usage information instead ದ ಖಂಡ usage\n\
"));
      oputs (_("\
  -k\n\
         like --ಖಂಡ-ಗಾತ್ರ_2=1K\n\
"));
      oputs (_("\
  -L, --dereference\n\
         dereference all symbolic links\n\
"));
      oputs (_("\
  -l, --ಎಣಿಕೆ_2-links\n\
         ಎಣಿಕೆ_2 sizes many times ಆದರೆ hard linked\n\
"));
      oputs (_("\
  -m\n\
         like --ಖಂಡ-ಗಾತ್ರ_2=1M\n\
"));
      oputs (_("\
  -P, --no-dereference\n\
         don't follow any symbolic links (this is the ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
  -S, --separate-dirs\n\
         ಗಾಗಿ directories ಮಾಡು not include ಗಾತ್ರ_2 ದ subdirectories\n\
"));
      oputs (_("\
      --si\n\
         like -h, but use powers ದ 1000 not 1024\n\
"));
      oputs (_("\
  -s, --summarize\n\
         display only a ಒಟ್ಟು ಗಾಗಿ each ವಾದ\n\
"));
      oputs (_("\
  -t, --ಮಿತಿ=ಗಾತ್ರ_3\n\
         exclude entries smaller than ಗಾತ್ರ_3 ಆದರೆ ಧನಾತ್ಮಕ,\n\
         or entries greater than ಗಾತ್ರ_3 ಆದರೆ negative\n\
"));
      oputs (_("\
      --ಸಮಯ\n\
         show ಸಮಯ ದ the ಕೊನೆಯ modification ದ any file ಒಳ the directory,\n\
         or any ದ its subdirectories\n\
"));
      oputs (_("\
      --ಸಮಯ=ಶಬ್ದ\n\
         show ಸಮಯ as ಶಬ್ದ instead ದ modification ಸಮಯ:\n\
         atime, access, use, ctime or ಸ್ಥಿತಿ_2\n\
"));
      oputs (_("\
      --ಸಮಯ-style=STYLE\n\
         ಸಮಯ/date format with --ಸಮಯ; see ಸಮಯ_STYLE ಕೆಳಗೆ\n\
"));
      oputs (_("\
  -X, --exclude-from=FILE\n\
         exclude files that ಹೊಂದಿಕೆ any ನಮೂನೆ_2 ಒಳ FILE\n\
"));
      oputs (_("\
      --exclude=ನಮೂನೆ\n\
         exclude files that ಹೊಂದಿಕೆ ನಮೂನೆ\n\
"));
      oputs (_("\
  -x, --one-file-system\n\
         skip directories on different file systems\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_blocksize_note ("DU");
      emit_ಗಾತ್ರ_note ();
      fputs (_("\
\n\
The --ಸಮಯ-style STYLE ವಾದ can be ತುಂಬಿದ-iso, ದೀರ್ಘ-iso, iso, or +FORMAT.\n\
FORMAT is interpreted like ಒಳ date(1).\n\
Also the ಸಮಯ_STYLE environment variable sets the ಪೂರ್ವನಿಯೋಜಿತ style ಗಮ್ಯ use.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Try to insert the INO/DEV pair into DI_SET.
   Return true if the pair is successfully inserted,
   false if the pair was already there.  */
ಸ್ಥಿರ bool
hash_ins (ರಚನೆ di_ಗಣ *di_ಗಣ, ino_t ino, dev_t dev)
{
  ಪೂರ್ಣಾಂಕ inserted = di_ಗಣ_ಸೇರಿಸು (di_ಗಣ, dev, ino);
  ಆದರೆ (inserted < 0)
    xalloc_die ();
  ಹಿಂತಿರುಗಿ inserted;
}

/* Print N_BYTES.  Convert it to a readable value before printing.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_only_ಗಾತ್ರ (uintmax_t n_ಬೈಟುಗಳು)
{
  ಅಕ್ಷರ buf[LONGEST_HUMAN_READABLE + 1];
  fputs ((n_ಬೈಟುಗಳು == UINTMAX_MAX
          ? _("Infinity")
          : human_readable (n_ಬೈಟುಗಳು, buf, human_output_opts,
                            1, output_ಖಂಡ_ಗಾತ್ರ)),
         stdout);
}

/* Print size (and optionally time) indicated by *PDUI, followed by STRING,
   quoting STRING if QUOTE is true.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಗಾತ್ರ (ಸ್ಥಿರಾಂಕ ರಚನೆ duinfo *pdui, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string, bool quote)
{
  print_only_ಗಾತ್ರ (opt_inodes
                   ? pdui->inodes
                   : pdui->ಗಾತ್ರ_2);

  ಆದರೆ (opt_ಸಮಯ)
    {
      ಅಕ್ಷರ_ಬರೆ ('\t');
      bool ok = show_date (ಸಮಯ_format, pdui->tmax, localtz);
      ಆದರೆ (!ok)
        {
          /* If failed to format date, print raw seconds instead.  */
          ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (intmax_t)];
          fputs (timetostr (pdui->tmax.tv_sec, buf), stdout);
        }
    }
  ಅಕ್ಷರ_ಬರೆ ('\t');
  fputs (quote ? quoteN (string) : string, stdout);
  ಅಕ್ಷರ_ಬರೆ (opt_nul_terminate_output ? '\0' : '\n');
  ಆದರೆ (fflush (stdout) < 0)
    write_error ();
}

/* Fill the di_mnt set with local mount point dev/ino pairs.  */

ಸ್ಥಿರ ಶೂನ್ಯ
fill_mount_ಕೋಷ್ಟಕ (ಶೂನ್ಯ)
{
  ರಚನೆ mount_ನಮೂದು *mnt_ent = read_file_system_list (false);
  ಆಗ (mnt_ent)
    {
      ರಚನೆ mount_ನಮೂದು *mnt_free;
      ಆದರೆ (!mnt_ent->me_remote && !mnt_ent->me_dummy)
        {
          ರಚನೆ stat buf;
          ಆದರೆ (!stat (mnt_ent->me_mountdir, &buf))
            hash_ins (di_mnt, buf.st_ino, buf.st_dev);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Ignore stat failure.  False positives are too common.
                 E.g., "Permission denied" on /run/user/<name>/gvfs.  */
            }
        }

      mnt_free = mnt_ent;
      mnt_ent = mnt_ent->me_ಮುಂದಿನ;
      free_mount_ನಮೂದು (mnt_free);
    }
}

/* This function checks whether any of the directories in the cycle that
   fts detected is a mount point.  */

ಸ್ಥಿರ bool
mount_ಬಿಂದು_ಒಳ_fts_ಚಕ್ರ (FTSENT ಸ್ಥಿರಾಂಕ *ent)
{
  FTSENT ಸ್ಥಿರಾಂಕ *ಚಕ್ರ_ent = ent->fts_ಚಕ್ರ;

  ಆದರೆ (!di_mnt)
    {
      /* Initialize the set of dev,inode pairs.  */
      di_mnt = di_ಗಣ_alloc ();
      ಆದರೆ (!di_mnt)
        xalloc_die ();

      fill_mount_ಕೋಷ್ಟಕ ();
    }

  ಆಗ (ent && ent != ಚಕ್ರ_ent)
    {
      ಆದರೆ (di_ಗಣ_lookup (di_mnt, ent->fts_statp->st_dev,
                         ent->fts_statp->st_ino) > 0)
        {
          ಹಿಂತಿರುಗಿ true;
        }
      ent = ent->fts_ಪೋಷಕ;
    }

  ಹಿಂತಿರುಗಿ false;
}

/* This function is called once for every file system object that fts
   encounters.  fts does a depth-first traversal.  This function knows
   that and accumulates per-directory totals based on changes in
   the depth of the current entry.  It returns true on success.  */

ಸ್ಥಿರ bool
process_file (FTS *fts, FTSENT *ent)
{
  bool ok = true;
  ರಚನೆ duinfo dui;
  ರಚನೆ duinfo dui_ಗಮ್ಯ_print;
  ಸ್ಥಿರ idx_t n_alloc;
  /* First element of the structure contains:
     The sum of the sizes of all entries in the single directory
     at the corresponding level.  Although this does include the sizes
     corresponding to each subdirectory, it does not include the size of
     any file in a subdirectory. Also corresponding last modified date.
     Second element of the structure contains:
     The sum of the sizes of all entries in the hierarchy at or below the
     directory at the specified level.  */
  ಸ್ಥಿರ ರಚನೆ dulevel *dulvl;

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = ent->fts_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ರಚನೆ stat *sb = ent->fts_statp;
  ಪೂರ್ಣಾಂಕ info = ent->fts_info;
  bool ignore_missing = ignore_missing_fts_ನಮೂದು (ent);

  ಆದರೆ (ignore_missing && ignorable_fts_error (ent))
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (info == FTS_DNR)
    {
      /* An error occurred, but the size is known, so count it.  */
      error (0, ent->fts_errno, _("cannot read directory %s"), quoteaf (file));
      ok = false;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info != FTS_DP)
    {
      bool excluded = excluded_file_name (exclude, file);
      ಆದರೆ (! excluded)
        {
          /* Make the stat buffer *SB valid, or fail noisily.  */

          ಆದರೆ (info == FTS_NSOK)
            {
              fts_ಗಣ (fts, ent, FTS_AGAIN);
              MAYBE_ಬಳಸದ FTSENT ಸ್ಥಿರಾಂಕ *e = fts_read (fts);
              affirm (e == ent);
              info = ent->fts_info;

              ಆದರೆ (ignore_missing && ignorable_fts_error (ent))
                ಹಿಂತಿರುಗಿ true;
            }

          ಆದರೆ (info == FTS_NS || info == FTS_SLNONE)
            {
              error (0, ent->fts_errno, _("cannot access %s"), quoteaf (file));
              ಹಿಂತಿರುಗಿ false;
            }

          /* The --one-file-system (-x) option cannot exclude anything
             specified on the command-line.  By definition, it can exclude
             a file or directory only when its device number is different
             from that of its just-processed parent directory, and du does
             not process the parent of a command-line argument.  */
          ಆದರೆ (fts->fts_options & FTS_XDEV
              && FTS_ROOTLEVEL < ent->fts_level
              && fts->fts_dev != sb->st_dev)
            excluded = true;
        }

      ಆದರೆ (excluded
          || (! opt_ಎಣಿಕೆ_all
              && (hash_all || (! S_ISDIR (sb->st_ಬಹುಲಕ) && 1 < sb->st_nlink))
              && ! hash_ins (di_files, sb->st_ino, sb->st_dev)))
        {
          /* If ignoring a directory in preorder, skip its children.
             Ignore the next fts_read output too, as it's a postorder
             visit to the same directory.  */
          ಆದರೆ (info == FTS_D)
            {
              fts_ಗಣ (fts, ent, FTS_SKIP);
              MAYBE_ಬಳಸದ FTSENT ಸ್ಥಿರಾಂಕ *e = fts_read (fts);
              affirm (e == ent);
            }

          ಹಿಂತಿರುಗಿ true;
        }

      ಆಯ್ಕೆ (info)
        {
        ಸ್ಥಿತಿ FTS_D:
          ಹಿಂತಿರುಗಿ true;

        ಸ್ಥಿತಿ FTS_ERR:
          /* An error occurred, but the size is known, so count it.  */
          error (0, ent->fts_errno, "%s", quotef (file));
          ok = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FTS_DC:
          /* If not following symlinks and not a (bind) mount point.  */
          ಆದರೆ (ಚಕ್ರ_warning_required (fts, ent)
              && ! mount_ಬಿಂದು_ಒಳ_fts_ಚಕ್ರ (ent))
            {
              emit_ಚಕ್ರ_warning (file);
              ಹಿಂತಿರುಗಿ false;
            }
          ಹಿಂತಿರುಗಿ true;
        }
    }

  duinfo_ಗಣ (&dui,
              (apparent_ಗಾತ್ರ
               ? (usable_st_ಗಾತ್ರ (sb) ? MAX (0, sb->st_ಗಾತ್ರ) : 0)
               : (uintmax_t) STP_NBLOCKS (sb) * ST_NBLOCKSIZE),
              (ಸಮಯ_type == ಸಮಯ_mtime ? get_stat_mtime (sb)
               : ಸಮಯ_type == ಸಮಯ_atime ? get_stat_atime (sb)
               : get_stat_ctime (sb)));

  idx_t level = ent->fts_level;
  dui_ಗಮ್ಯ_print = dui;

  ಆದರೆ (n_alloc == 0)
    {
      n_alloc = level + 10;
      dulvl = xcalloc (n_alloc, ಗಾತ್ರ *dulvl);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (level == prev_level)
        {
          /* This is usually the most common case.  Do nothing.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (level > prev_level)
        {
          /* Descending the hierarchy.
             Clear the accumulators for *all* levels between prev_level
             and the current one.  The depth may change dramatically,
             e.g., from 1 to 10.  */

          ಆದರೆ (n_alloc <= level)
            dulvl = xpalloc (dulvl, &n_alloc, level - n_alloc + 1, -1,
                             ಗಾತ್ರ *dulvl);

          ಗಾಗಿ (idx_t i = prev_level + 1; i <= level; i++)
            {
              duinfo_init (&dulvl[i].ent);
              duinfo_init (&dulvl[i].subdir);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ /* level < prev_level */
        {
          /* Ascending the hierarchy.
             Process a directory only after all entries in that
             directory have been processed.  When the depth decreases,
             propagate sums from the children (prev_level) to the parent.
             Here, the current level is always one smaller than the
             previous one.  */
          affirm (level == prev_level - 1);
          duinfo_ಕೂಡಿಸು (&dui_ಗಮ್ಯ_print, &dulvl[prev_level].ent);
          ಆದರೆ (!opt_separate_dirs)
            duinfo_ಕೂಡಿಸು (&dui_ಗಮ್ಯ_print, &dulvl[prev_level].subdir);
          duinfo_ಕೂಡಿಸು (&dulvl[level].subdir, &dulvl[prev_level].ent);
          duinfo_ಕೂಡಿಸು (&dulvl[level].subdir, &dulvl[prev_level].subdir);
        }
    }

  prev_level = level;

  /* Let the size of a directory entry contribute to the total for the
     containing directory, unless --separate-dirs (-S) is specified.  */
  ಆದರೆ (! (opt_separate_dirs && IS_DIR_TYPE (info)))
    duinfo_ಕೂಡಿಸು (&dulvl[level].ent, &dui);

  /* Even if this directory is unreadable or we can't chdir into it,
     do let its size contribute to the total. */
  duinfo_ಕೂಡಿಸು (&tot_dui, &dui);

  ಆದರೆ ((IS_DIR_TYPE (info) && level <= max_ಆಳ)
      || (opt_all && level <= max_ಆಳ)
      || level == 0)
    {
      /* Print or elide this entry according to the --threshold option.  */
      uintmax_t v = opt_inodes ? dui_ಗಮ್ಯ_print.inodes : dui_ಗಮ್ಯ_print.ಗಾತ್ರ_2;
      ಆದರೆ (opt_ಮಿತಿ < 0
          ? v <= -opt_ಮಿತಿ
          : v >= opt_ಮಿತಿ)
        print_ಗಾತ್ರ (&dui_ಗಮ್ಯ_print, file, quote_output);
    }

  ಹಿಂತಿರುಗಿ ok;
}

/* Recursively print the sizes of the directories (and, if selected, files)
   named in FILES, the last entry of which is null.
   BIT_FLAGS controls how fts works.
   Return true if successful.  */

ಸ್ಥಿರ bool
du_files (ಅಕ್ಷರ **files, ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags)
{
  bool ok = true;

  ಆದರೆ (*files)
    {
      FTS *fts = xfts_open (files, ಬಿಟ್_flags, ಖಾಲಿ);

      ಆಗ (true)
        {
          FTSENT *ent;

          ent = fts_read (fts);
          ಆದರೆ (ent == ಖಾಲಿ)
            {
              ಆದರೆ (errno != 0)
                {
                  error (0, errno, _("fts_read failed: %s"),
                         quotef (fts->fts_ಮಾರ್ಗ));
                  ok = false;
                }

              /* When exiting this loop early, be careful to reset the
                 global, prev_level, used in process_file.  Otherwise, its
                 (level == prev_level - 1) assertion could fail.  */
              prev_level = 0;
              ನಿಲ್ಲಿಸು;
            }

#ಆದರೆ GNULIB_FTS_DEBUG
          ಆದರೆ (fts_debug)
            fts_cross_check (fts);
#ಆದರೆ_ಅಂತ್ಯ

          ok &= process_file (fts, ent);
        }

      ಆದರೆ (fts_close (fts) != 0)
        {
          error (0, errno, _("fts_close failed"));
          ok = false;
        }
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *cwd_only[2];
  bool max_ಆಳ_specified = false;
  bool ok = true;
  ಅಕ್ಷರ *files_from = ಖಾಲಿ;

  /* Bit flags that control how fts works.  */
  ಪೂರ್ಣಾಂಕ ಬಿಟ್_flags = FTS_NOSTAT;

  /* Select one of the three FTS_ options that control if/when
     to follow a symlink.  */
  ಪೂರ್ಣಾಂಕ symlink_deref_ಬಿಟ್ಟುಗಳು = FTS_PHYSICAL;

  /* If true, display only a total for each argument. */
  bool opt_summarize_only = false;

  cwd_only[0] = bad_cast (".");
  cwd_only[1] = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  exclude = new_exclude ();

  human_options (getenv ("DU_BLOCK_SIZE"),
                 &human_output_opts, &output_ಖಂಡ_ಗಾತ್ರ);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ oi = -1;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "0aAbd:chHklmst:xB:DLPSX:",
                           long_options, &oi);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
#ಆದರೆ GNULIB_FTS_DEBUG
        ಸ್ಥಿತಿ FTS_DEBUG:
          fts_debug = true;
          ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

        ಸ್ಥಿತಿ '0':
          opt_nul_terminate_output = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'a':
          opt_all = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'A':
          apparent_ಗಾತ್ರ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'b':
          apparent_ಗಾತ್ರ = true;
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          print_grand_ಒಟ್ಟು = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h':
          human_output_opts = human_autoscale | human_SI | human_ತಳ_1024;
          output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ HUMAN_SI_OPTION:
          human_output_opts = human_autoscale | human_SI;
          output_ಖಂಡ_ಗಾತ್ರ = 1;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'k':
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1024;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':		/* --max-depth=N */
          {
            intmax_t tmp;
            ಆದರೆ (xstrtoimax (optarg, ಖಾಲಿ, 0, &tmp, "") == LONGINT_OK
                && ! ckd_ಕೂಡಿಸು (&max_ಆಳ, tmp, 0) && 0 <= max_ಆಳ)
              max_ಆಳ_specified = true;
            ಇಲ್ಲದಿದ್ದರೆ
              {
                error (0, 0, _("invalid maximum depth %s"),
                       quote (optarg));
                ok = false;
              }
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          human_output_opts = 0;
          output_ಖಂಡ_ಗಾತ್ರ = 1024 * 1024;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'l':
          opt_ಎಣಿಕೆ_all = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          opt_summarize_only = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          {
            ಎಣಿಕೆ strtol_error e;
            e = xstrtoimax (optarg, ಖಾಲಿ, 0, &opt_ಮಿತಿ,
                            "kKmMGTPEZYRQ0");
            ಆದರೆ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
            ಆದರೆ (opt_ಮಿತಿ == 0 && *optarg == '-')
              {
                /* Do not allow -0, as this wouldn't make sense anyway.  */
                error (EXIT_FAILURE, 0, _("invalid --threshold argument '-0'"));
              }
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'x':
          ಬಿಟ್_flags |= FTS_XDEV;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'B':
          {
            ಎಣಿಕೆ strtol_error e = human_options (optarg, &human_output_opts,
                                                 &output_ಖಂಡ_ಗಾತ್ರ);
            ಆದರೆ (e != LONGINT_OK)
              xstrtol_fatal (e, oi, c, long_options, optarg);
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'H':  /* NOTE: before 2008-12, -H was equivalent to --si.  */
        ಸ್ಥಿತಿ 'D':
          symlink_deref_ಬಿಟ್ಟುಗಳು = FTS_COMFOLLOW | FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'L': /* --dereference */
          symlink_deref_ಬಿಟ್ಟುಗಳು = FTS_LOGICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'P': /* --no-dereference */
          symlink_deref_ಬಿಟ್ಟುಗಳು = FTS_PHYSICAL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          opt_separate_dirs = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'X':
          ಆದರೆ (ಕೂಡಿಸು_exclude_file (ಕೂಡಿಸು_exclude, exclude, optarg,
                                EXCLUDE_WILDCARDS, '\n'))
            {
              error (0, errno, "%s", quotef (optarg));
              ok = false;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FILES0_FROM_OPTION:
          files_from = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ EXCLUDE_OPTION:
          ಕೂಡಿಸು_exclude (exclude, optarg, EXCLUDE_WILDCARDS);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ INODES_OPTION:
          opt_inodes = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸಮಯ_OPTION:
          opt_ಸಮಯ = true;
          ಸಮಯ_type =
            (optarg
             ? XARGMATCH ("--time", optarg, ಸಮಯ_ವಾದಗಳು, ಸಮಯ_types)
             : ಸಮಯ_mtime);
          localtz = tzalloc (getenv ("TZ"));
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸಮಯ_STYLE_OPTION:
          ಸಮಯ_style = optarg;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          ok = false;
        }
    }

  ಆದರೆ (!ok)
    usage (EXIT_FAILURE);

  ಆದರೆ (!opt_nul_terminate_output && isatty (STDOUT_FILENO))
    {
      ಪೂರ್ಣಾಂಕ qs = getenv_quoting_style ();
      ಆದರೆ (qs < 0)
        qs = shell_escape_quoting_style;
      ಆದರೆ (qs != literal_quoting_style)
        {
          ಗಣ_quoting_style (ಖಾಲಿ, qs);
          quote_output = true;
        }
    }

  ಆದರೆ (opt_all && opt_summarize_only)
    {
      error (0, 0, _("cannot both summarize and show all entries"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (opt_summarize_only && max_ಆಳ_specified && max_ಆಳ == 0)
    {
      error (0, 0,
             _("warning: summarizing is the same as using --max-depth=0"));
    }

  ಆದರೆ (opt_summarize_only && max_ಆಳ_specified && max_ಆಳ != 0)
    {
      error (0, 0, _("warning: summarizing conflicts with --max-depth=%td"),
             max_ಆಳ);
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (opt_summarize_only)
    max_ಆಳ = 0;

  ಆದರೆ (opt_inodes)
    {
      ಆದರೆ (apparent_ಗಾತ್ರ)
        {
          error (0, 0, _("warning: options --apparent-size and -b are "
                         "ineffective with --inodes"));
        }
      output_ಖಂಡ_ಗಾತ್ರ = 1;
    }

  /* Process time style if printing last times.  */
  ಆದರೆ (opt_ಸಮಯ)
    {
      ಆದರೆ (! ಸಮಯ_style)
        {
          ಸಮಯ_style = getenv ("TIME_STYLE");

          /* Ignore TIMESTYLE="locale", for compatibility with ls.  */
          ಆದರೆ (! ಸಮಯ_style || streq (ಸಮಯ_style, "locale"))
            ಸಮಯ_style = "long-iso";
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*ಸಮಯ_style == '+')
            {
              /* Ignore anything after a newline, for compatibility
                 with ls.  */
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = strchr (ಸಮಯ_style, '\n');
              ಆದರೆ (p)
                ಸಮಯ_style = xmemdup0 (ಸಮಯ_style, p - ಸಮಯ_style);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Ignore "posix-" prefix, for compatibility with ls.  */
              ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ posix_ಪೂರ್ವಪ್ರತ್ಯಯ[] = "posix-";
              ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ size_t ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಗಾತ್ರ posix_ಪೂರ್ವಪ್ರತ್ಯಯ - 1;
              ಆಗ (STREQ_LEN (ಸಮಯ_style, posix_ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_len))
                ಸಮಯ_style += ಪೂರ್ವಪ್ರತ್ಯಯ_len;
            }
        }

      ಆದರೆ (*ಸಮಯ_style == '+')
        ಸಮಯ_format = ಸಮಯ_style + 1;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆಯ್ಕೆ (x_timestyle_ಹೊಂದಿಕೆ (ಸಮಯ_style, /*allow_posix=*/ false,
                                     ಸಮಯ_style_ವಾದಗಳು,
                                     (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಸಮಯ_style_types,
                                     ಗಾತ್ರ (*ಸಮಯ_style_types), EXIT_FAILURE))
            {
            ಸ್ಥಿತಿ ತುಂಬಿದ_iso_ಸಮಯ_style:
              ಸಮಯ_format = "%Y-%m-%d %H:%M:%S.%N %z";
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ long_iso_ಸಮಯ_style:
              ಸಮಯ_format = "%Y-%m-%d %H:%M";
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ iso_ಸಮಯ_style:
              ಸಮಯ_format = "%Y-%m-%d";
              ನಿಲ್ಲಿಸು;
            }
        }
    }

  ರಚನೆ argv_iterator *ai;
  ಆದರೆ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ಆದರೆ (optind < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (EXIT_FAILURE);
        }

      ಆದರೆ (! (streq (files_from, "-") || freopen (files_from, "r", stdin)))
        error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
               quoteaf (files_from));

      ai = argv_iter_init_ಹರಿವು (stdin);

      /* It's not easy here to count the arguments, so assume the
         worst.  */
      hash_all = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ **files = (optind < argc ? argv + optind : cwd_only);
      ai = argv_iter_init_argv (files);

      /* Hash all dev,ino pairs if there are multiple arguments, or if
         following non-command-line symlinks, because in either case a
         file with just one hard link might be seen more than once.  */
      hash_all = (optind + 1 < argc || symlink_deref_ಬಿಟ್ಟುಗಳು == FTS_LOGICAL);
    }

  ಆದರೆ (!ai)
    xalloc_die ();

  /* Initialize the set of dev,inode pairs.  */
  di_files = di_ಗಣ_alloc ();
  ಆದರೆ (!di_files)
    xalloc_die ();

  /* If not hashing everything, process_file won't find cycles on its
     own, so ask fts_read to check for them accurately.  */
  ಆದರೆ (opt_ಎಣಿಕೆ_all || ! hash_all)
    ಬಿಟ್_flags |= FTS_TIGHT_ಚಕ್ರ_CHECK;

  ಬಿಟ್_flags |= symlink_deref_ಬಿಟ್ಟುಗಳು;
  ಸ್ಥಿರ ಅಕ್ಷರ *ತಾತ್ಕಾಲಿಕ_argv[] = { ಖಾಲಿ, ಖಾಲಿ };

  ಆಗ (true)
    {
      bool skip_file = false;
      ಎಣಿಕೆ argv_iter_err ai_err;
      ಅಕ್ಷರ *file_name = argv_iter (ai, &ai_err);
      ಆದರೆ (!file_name)
        {
          ಆಯ್ಕೆ (ai_err)
            {
            ಸ್ಥಿತಿ AI_ERR_EOF:
              ಹೋಗು argv_iter_ಮುಗಿದ;
            ಸ್ಥಿತಿ AI_ERR_READ:
              error (0, errno, _("%s: read error"),
                     quotef (files_from));
              ok = false;
              ಹೋಗು argv_iter_ಮುಗಿದ;
            ಸ್ಥಿತಿ AI_ERR_MEM:
              xalloc_die ();
            ಸ್ಥಿತಿ AI_ERR_OK: ಪೂರ್ವನಿಯೋಜಿತ:
              affirm (!"unexpected error code from argv_iter");
            }
        }
      ಆದರೆ (files_from && streq (files_from, "-") && streq (file_name, "-"))
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
      ಆದರೆ (!file_name[0])
        {
          /* Diagnose a zero-length file name.  When it's one
             among many, knowing the record number may help.
             FIXME: currently print the record number only with
             --files0-from=FILE.  Maybe do it for argv, too?  */
          ಆದರೆ (files_from == ಖಾಲಿ)
            error (0, 0, "%s", _("invalid zero-length file name"));
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Using the standard 'filename:line-number:' prefix here is
                 not totally appropriate, since NUL is the separator, not NL,
                 but it might be better than nothing.  */
              idx_t file_ಸಂಖ್ಯೆ = argv_iter_n_ವಾದಗಳು (ai);
              error (0, 0, "%s:%td: %s", quotef (files_from),
                     file_ಸಂಖ್ಯೆ, _("invalid zero-length file name"));
            }
          skip_file = true;
        }

      ಆದರೆ (skip_file)
        ok = false;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ತಾತ್ಕಾಲಿಕ_argv[0] = file_name;
          ok &= du_files (ತಾತ್ಕಾಲಿಕ_argv, ಬಿಟ್_flags);
        }
    }
 argv_iter_ಮುಗಿದ:

  argv_iter_free (ai);
  di_ಗಣ_free (di_files);
  ಆದರೆ (di_mnt)
    di_ಗಣ_free (di_mnt);

  ಆದರೆ (files_from && (ferror (stdin) || fclose (stdin) != 0) && ok)
    error (EXIT_FAILURE, 0, _("error reading %s"), quoteaf (files_from));

  ಆದರೆ (print_grand_ಒಟ್ಟು)
    print_ಗಾತ್ರ (&tot_dui, _("total"), false);

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
