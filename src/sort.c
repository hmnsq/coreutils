/* sort - sort lines of text (with all kinds of options).
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written December 1988 by Mike Haertel.
   The author may be reached (Email) at the address mike@gnu.ai.mit.edu,
   or (US mail) as Mike Haertel c/o Free Software Foundation.

   Ørn E. Hansen added NLS support in 1997.  */

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <spawn.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "filevercmp.h"
#ಅನ್ತರ್ಭಾವ "findprog.h"
#ಅನ್ತರ್ಭಾವ "flexmember.h"
#ಅನ್ತರ್ಭಾವ "hard-locale.h"
#ಅನ್ತರ್ಭಾವ "hash.h"
#ಅನ್ತರ್ಭಾವ "heap.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "mbswidth.h"
#ಅನ್ತರ್ಭಾವ "nproc.h"
#ಅನ್ತರ್ಭಾವ "physmem.h"
#ಅನ್ತರ್ಭಾವ "posixver.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "randread.h"
#ಅನ್ತರ್ಭಾವ "readtokens0.h"
#ಅನ್ತರ್ಭಾವ "stdlib--.h"
#ಅನ್ತರ್ಭಾವ "strnumcmp.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"
#ಅನ್ತರ್ಭಾವ "unistd--.h"
#ಅನ್ತರ್ಭಾವ "xmemcoll.h"
#ಅನ್ತರ್ಭಾವ "xnanosleep.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"
#ಅನ್ತರ್ಭಾವ "xstrtol-error.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RLIMIT_ದತ್ತಾಂಶ
ರಚನೆ rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#ಆದರೆ_ಅಂತ್ಯ

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "sort"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ಆದರೆ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined OPEN_MAX
# define OPEN_MAX 20
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು UCHAR_LIM (UCHAR_MAX + 1)

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#ಆದರೆ_ಅಂತ್ಯ

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ವಿಲೀನ(ಒಟ್ಟು, level) (((ಒಟ್ಟು) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
ಎಣಿಕೆ { SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ಎಣಿಕೆ { DEFAULT_MAX_ಎಳೆಗಳು = 8 };

/* Exit statuses.  */
ಎಣಿಕೆ
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    ವಿಂಗಡಿಸು_ಹೊರಹೋಗು_ದ_ಕ್ರಮ = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    ವಿಂಗಡಿಸು_FAILURE = 2
  };

ಎಣಿಕೆ
  {
    /* The number of times we should try to spawn a compression process
       (we retry if the posix_spawnp call fails with EAGAIN).  We don't _need_
       to compress temp files, this is just to reduce file system access, so
       this number can be small.  Each retry doubles in duration.  */
    MAX_TRIES_COMPRESS = 4,

    /* The number of times we should try to spawn a decompression process.
       If we can't spawn a decompression process, we can't sort, so this
       number should be big.  Each retry doubles in duration.  */
    MAX_TRIES_DECOMPRESS = 9
  };

ಎಣಿಕೆ
  {
    /* Level of the end-of-merge node, one level above the root. */
    ವಿಲೀನ_ಕೊನೆ = 0,

    /* Level of the root node in merge tree. */
    ವಿಲೀನ_ಬೇರು = 1
  };

/* The representation of the decimal point in the current locale.  */
ಸ್ಥಿರ ಅಕ್ಷರ decimal_ಬಿಂದು;

/* Thousands separator; if outside char range, there is no separator.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ಸ್ಥಿರ bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ಸ್ಥಿರ bool hard_LC_COLLATE;
#ಆದರೆ HAVE_NL_LANGINFO
ಸ್ಥಿರ bool hard_LC_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ಎಣಿಕೆ blanktype { bl_ಆರಂಭ, bl_ಕೊನೆ, bl_both };

/* The character marking end of line. Default to \n. */
ಸ್ಥಿರ ಅಕ್ಷರ eolchar = '\n';

/* Lines are held in memory as counted strings. */
ರಚನೆ ಸಾಲು
{
  ಅಕ್ಷರ *ಪಠ್ಯ;			/* Text of the line. */
  size_t ಉದ್ದಳತೆ;		/* Length including final newline. */
  ಅಕ್ಷರ *keybeg;			/* Start of first key. */
  ಅಕ್ಷರ *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ರಚನೆ ಬಫರ್
{
  ಅಕ್ಷರ *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t ಬಳಸಿದ;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ಎಡ;			/* Number of bytes left from previous reads. */
  size_t ಸಾಲು_ಬೈಟುಗಳು;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ರಚನೆ ವಿಂಗಡಿಸು_ಬಫರ್_policy
{
  size_t initial;
  size_t ಎಲ್ಲೆ;
  bool growth_failed;
};

/* Sort key.  */
ರಚನೆ keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ಸ್ಥಿರಾಂಕ *ignore;		/* Boolean array of characters to ignore. */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate;	/* Translation applied to characters. */
  bool skipsblanks;		/* Skip leading blanks when finding start.  */
  bool skipeblanks;		/* Skip leading blanks when finding end.  */
  bool numeric;			/* Flag for numeric comparison.  Handle
                                   strings of digits with optional decimal
                                   point, but no exponential notation. */
  bool random;			/* Sort by random hash of key.  */
  bool general_numeric;		/* Flag for general, numeric comparison.
                                   Handle numbers in exponential notation. */
  bool human_numeric;		/* Flag for sorting by human readable
                                   units with either SI or IEC prefixes. */
  bool month;			/* Flag for comparison by month name. */
  bool ಹಿಮ್ಮುಖ;			/* Reverse the sense of comparison. */
  bool ಆವೃತ್ತಿ_3;			/* sort by version number */
  bool traditional_ಬಳಸಿದ;	/* Traditional key option format is used. */
  ರಚನೆ keyfield *ಮುಂದಿನ;	/* Next keyfield to try. */
};

ರಚನೆ month
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;
  ಪೂರ್ಣಾಂಕ val;
};

/* Binary merge tree node. */
ರಚನೆ ವಿಲೀನ_ಗಂಟು
{
  ರಚನೆ ಸಾಲು *lo;              /* Lines to merge from LO child node. */
  ರಚನೆ ಸಾಲು *hi;              /* Lines to merge from HI child node. */
  ರಚನೆ ಸಾಲು *ಕೊನೆ_lo;          /* End of available lines from LO. */
  ರಚನೆ ಸಾಲು *ಕೊನೆ_hi;          /* End of available lines from HI. */
  ರಚನೆ ಸಾಲು **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಪೋಷಕ;    /* Parent node. */
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *lo_ಮಗು;  /* LO child node. */
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *hi_ಮಗು;  /* HI child node. */
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t ಬೀಗ;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ
{
  ರಚನೆ ರಾಶಿ_2 *ಆದ್ಯತೆ_ಸರತಿ;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ಸ್ಥಿರ ರಚನೆ ಸಾಲು saved_ಸಾಲು;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ಸ್ಥಿರ bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ಸ್ಥಿರ bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ಸ್ಥಿರ bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ಸ್ಥಿರ ಅಕ್ಷರ fold_toupper[UCHAR_LIM];

#ವ್ಯಾಖ್ಯಾನಿಸು MONTHS_ಪ್ರತಿ_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ಸ್ಥಿರ ರಚನೆ month monthtab[] =
{
  {"APR", 4},
  {"AUG", 8},
  {"DEC", 12},
  {"FEB", 2},
  {"JAN", 1},
  {"JUL", 7},
  {"JUN", 6},
  {"MAR", 3},
  {"MAY", 5},
  {"NOV", 11},
  {"OCT", 10},
  {"SEP", 9}
};

/* During the merge phase, the number of files to merge at once. */
#ವ್ಯಾಖ್ಯಾನಿಸು NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MIN_ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ (2 + ಗಾತ್ರ (ರಚನೆ ಸಾಲು))

/* Minimum sort size; the code might not work with smaller sizes.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ (nmerge * MIN_ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ಸ್ಥಿರ size_t ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ = MAX (MIN_ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ಸ್ಥಿರ size_t ವಿಂಗಡಿಸು_ಗಾತ್ರ;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಒಳಹರಿವು_FILE_ಗಾತ್ರ_GUESS (128 * 1024)

ಸ್ಥಿರ ರಚನೆ
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **dirs COUNTED_BY (n_dirs);
} ತಾತ್ಕಾಲಿಕ_dirs;



/* Flag to reverse the order of all comparisons. */
ಸ್ಥಿರ bool ಹಿಮ್ಮುಖ;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ಸ್ಥಿರ bool stable;

/* An int value outside char range.  */
ಎಣಿಕೆ { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ಎಣಿಕೆ { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ಸ್ಥಿರ bool unique;

/* Nonzero if any of the input files are the standard input. */
ಸ್ಥಿರ bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ಸ್ಥಿರ ರಚನೆ keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *compress_ಕಾರ್ಯಕ್ರಮ;

/* Annotate the output with extra info to aid the user.  */
ಸ್ಥಿರ bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ಸ್ಥಿರ bool default_SIGPIPE;

/* The list of temporary files. */
ರಚನೆ tempnode
{
  ರಚನೆ tempnode *ಅಸ್ಥಿರ ಮುಂದಿನ;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  ಅಕ್ಷರ state;
  ಅಕ್ಷರ name[FLEXIBLE_ARRAY_MEMBER];
};
ಸ್ಥಿರ ರಚನೆ tempnode *ಅಸ್ಥಿರ temphead;
ಸ್ಥಿರ ರಚನೆ tempnode *ಅಸ್ಥಿರ *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ರಚನೆ tempnode ಸ್ಥಿರಾಂಕ *ಗಂಟು = temphead; ಗಂಟು; ಗಂಟು = ಗಂಟು->ಮುಂದಿನ)
    unlink (ಗಂಟು->name);
  temphead = ಖಾಲಿ;
}

/* Handle interrupts and hangups. */

ಸ್ಥಿರ ಶೂನ್ಯ
sighandler (ಪೂರ್ಣಾಂಕ sig)
{
  cleanup ();

  ಸಂಕೇತ (sig, SIG_DFL);
  ಎತ್ತು (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು_die (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂದೇಶ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ಆದರೆ (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (ವಿಂಗಡಿಸು_FAILURE, errno, "%s: %s", ಸಂದೇಶ_2,
         quotef (file ? file : _("standard output")));
}

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
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Write sorted concatenation ದ all FILE(s) ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding ವಿಂಗಡಿಸು ಕೀಲಿಗಳು ಒಳ each ಸಾಲು\n\
"));
      oputs (_("\
  -d, --dictionary-ಕ್ರಮ\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-ಸ್ಥಿತಿ\n\
         fold ಅಧೋ ಸ್ಥಿತಿ ಗಮ್ಯ ಊರ್ಧ್ವ ಸ್ಥಿತಿ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-ವಿಂಗಡಿಸು\n\
         compare according ಗಮ್ಯ general numerical ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-ವಿಂಗಡಿಸು\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-ವಿಂಗಡಿಸು\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-ವಿಂಗಡಿಸು\n\
         compare according ಗಮ್ಯ string numerical ಮೌಲ್ಯ;\n\
         see ತುಂಬಿದ documentation ಗಾಗಿ supported ಅಕ್ಷರಸರಣಿಗಳು\n\
"));
      oputs (_("\
  -R, --random-ವಿಂಗಡಿಸು\n\
         shuffle, but ಗುಂಪು identical ಕೀಲಿಗಳು.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-ಮೂಲ_2=FILE\n\
         get random ಬೈಟುಗಳು from FILE\n\
"));
      oputs (_("\
  -r, --ಹಿಮ್ಮುಖ\n\
         ಹಿಮ್ಮುಖ the ಫಲಿತಾಂಶ ದ comparisons\n\
"));
      oputs (_("\
      --ವಿಂಗಡಿಸು=ಶಬ್ದ\n\
         ವಿಂಗಡಿಸು according ಗಮ್ಯ ಶಬ್ದ:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, ಆವೃತ್ತಿ_3 -V\n\
"));
      oputs (_("\
  -V, --ಆವೃತ್ತಿ_3-ವಿಂಗಡಿಸು\n\
         natural ವಿಂಗಡಿಸು ದ (ಆವೃತ್ತಿ_3) numbers within ಪಠ್ಯ\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-ಗಾತ್ರ_2=NMERGE\n\
         ವಿಲೀನ at most NMERGE inputs at once; ಗಾಗಿ ಇನ್ನಷ್ಟು use ತಾತ್ಕಾಲಿಕ files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-ಮೊದಲ\n\
         check ಗಾಗಿ sorted ಒಳಹರಿವು; ಮಾಡು not ವಿಂಗಡಿಸು\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but ಮಾಡು not report ಮೊದಲ bad ಸಾಲು\n\
"));
      oputs (_("\
      --compress-ಕಾರ್ಯಕ್ರಮ=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part ದ the ಸಾಲು ಬಳಸಿದ ಗಮ್ಯ ವಿಂಗಡಿಸು,\n\
         and warn about questionable usage ಗಮ್ಯ standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ಪಠ ಒಳಹರಿವು from the files specified by NUL-terminated ಹೆಸರುಗಳು ಒಳ file F;\
\n\
         If F is -, ಪಠ ಹೆಸರುಗಳು from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -k, --ಕೀಲಿ=KEYDEF\n\
         ವಿಂಗಡಿಸು ಮೂಲಕ a ಕೀಲಿ; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --ವಿಲೀನ\n\
         ವಿಲೀನ already sorted files; ಮಾಡು not ವಿಂಗಡಿಸು\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         ಲಿಖ ಫಲಿತಾಂಶ ಗಮ್ಯ FILE instead ದ standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize ವಿಂಗಡಿಸು by disabling ಕೊನೆಯ-resort comparison\n\
"));
      oputs (_("\
  -S, --ಬಫರ್-ಗಾತ್ರ_2=ಗಾತ್ರ_3\n\
         use ಗಾತ್ರ_3 ಗಾಗಿ ಮುಖ್ಯ ಸ್ಮರಣೆ ಬಫರ್\n\
"));
      oputs (_("\
  -t, --ಕ್ಷೇತ್ರ-separator=SEP\n\
         use SEP instead ದ non-blank ಗಮ್ಯ blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR ಗಾಗಿ temporaries, not $TMPDIR or %s;\n\
         ಅಪವರ್ತ್ಯ options specify ಅಪವರ್ತ್ಯ directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the ಸಂಖ್ಯೆ ದ sorts ಸತತ_2 concurrently ಗಮ್ಯ N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the ಮೊದಲ ದ ಸಾಲುಗಳು with equal ಕೀಲಿಗಳು;\n\
         with -c, check ಗಾಗಿ strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] ಗಾಗಿ ಆರಂಭ and stop position, where F is a\n\
ಕ್ಷೇತ್ರ ಸಂಖ್ಯೆ and C a character position ಒಳ the ಕ್ಷೇತ್ರ; both are origin 1, and\n\
the stop position defaults ಗಮ್ಯ the ಸಾಲು's ಕೊನೆ.  If neither -t nor -b is ಒಳ\n\
effect, characters ಒಳ a ಕ್ಷೇತ್ರ are counted from the beginning ದ the preceding\n\
whitespace.  OPTS is one or ಇನ್ನಷ್ಟು single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options ಗಾಗಿ that ಕೀಲಿ.  If no ಕೀಲಿ is given, use\n\
the entire ಸಾಲು as the ಕೀಲಿ.  Use --debug ಗಮ್ಯ diagnose incorrect ಕೀಲಿ usage.\n\
\n\
ಗಾತ್ರ_3 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% ದ ಸ್ಮರಣೆ, b 1, K 1024 (ಪೂರ್ವನಿಯೋಜಿತ), and so on ಗಾಗಿ M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects ವಿಂಗಡಿಸು ಕ್ರಮ.\n\
ಗಣ_3 LC_ALL=C ಗಮ್ಯ get the traditional ವಿಂಗಡಿಸು ಕ್ರಮ that ಬಳಸುತ್ತದೆ\n\
native byte ಮೌಲ್ಯಗಳು.\n\
"), stdout );
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_ಕಾರ್ಯಕ್ರಮ_OPTION,
  DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_ಮೂಲ_OPTION,
  ವಿಂಗಡಿಸು_OPTION,
  PARALLEL_OPTION
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"ignore-leading-blanks", no_ವಾದ, ಖಾಲಿ, 'b'},
  {"check", optional_ವಾದ, ಖಾಲಿ, CHECK_OPTION},
  {"compress-program", required_ವಾದ, ಖಾಲಿ, COMPRESS_ಕಾರ್ಯಕ್ರಮ_OPTION},
  {"debug", no_ವಾದ, ಖಾಲಿ, DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION},
  {"dictionary-order", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"ignore-case", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"files0-from", required_ವಾದ, ಖಾಲಿ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_ವಾದ, ಖಾಲಿ, 'g'},
  {"ignore-nonprinting", no_ವಾದ, ಖಾಲಿ, 'i'},
  {"key", required_ವಾದ, ಖಾಲಿ, 'k'},
  {"merge", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"month-sort", no_ವಾದ, ಖಾಲಿ, 'M'},
  {"numeric-sort", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"human-numeric-sort", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"version-sort", no_ವಾದ, ಖಾಲಿ, 'V'},
  {"random-sort", no_ವಾದ, ಖಾಲಿ, 'R'},
  {"random-source", required_ವಾದ, ಖಾಲಿ, RANDOM_ಮೂಲ_OPTION},
  {"sort", required_ವಾದ, ಖಾಲಿ, ವಿಂಗಡಿಸು_OPTION},
  {"output", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"reverse", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"stable", no_ವಾದ, ಖಾಲಿ, 's'},
  {"batch-size", required_ವಾದ, ಖಾಲಿ, NMERGE_OPTION},
  {"buffer-size", required_ವಾದ, ಖಾಲಿ, 'S'},
  {"field-separator", required_ವಾದ, ಖಾಲಿ, 't'},
  {"temporary-directory", required_ವಾದ, ಖಾಲಿ, 'T'},
  {"unique", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {"parallel", required_ವಾದ, ಖಾಲಿ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0},
};

#ವ್ಯಾಖ್ಯಾನಿಸು CHECK_ಕೋಷ್ಟಕ \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ check_ವಾದಗಳು[] =
{
#ವ್ಯಾಖ್ಯಾನಿಸು _ct_(_s, _c) _s,
  CHECK_ಕೋಷ್ಟಕ ಖಾಲಿ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  _ct_
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ check_types[] =
{
#ವ್ಯಾಖ್ಯಾನಿಸು _ct_(_s, _c) _c,
  CHECK_ಕೋಷ್ಟಕ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  _ct_
};

#ವ್ಯಾಖ್ಯಾನಿಸು ವಿಂಗಡಿಸು_ಕೋಷ್ಟಕ \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ವಿಂಗಡಿಸು_ವಾದಗಳು[] =
{
#ವ್ಯಾಖ್ಯಾನಿಸು _st_(_s, _c) _s,
  ವಿಂಗಡಿಸು_ಕೋಷ್ಟಕ ಖಾಲಿ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  _st_
};
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ ವಿಂಗಡಿಸು_types[] =
{
#ವ್ಯಾಖ್ಯಾನಿಸು _st_(_s, _c) _c,
  ವಿಂಗಡಿಸು_ಕೋಷ್ಟಕ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು  _st_
};

/* The set of signals that are caught.  */
ಸ್ಥಿರ sigset_t caught_ಸಂಕೇತಗಳು;

/* Critical section status.  */
ರಚನೆ cs_ಸ್ಥಿತಿ
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ಸ್ಥಿರ ಶೂನ್ಯ
cs_enter (ರಚನೆ cs_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2)
{
  ಪೂರ್ಣಾಂಕ ret = pthread_sigmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &ಸ್ಥಿತಿ_2->sigs);
  ಸ್ಥಿತಿ_2->valid = ret == 0;
}

/* Leave a critical section.  */
ಸ್ಥಿರ ಶೂನ್ಯ
cs_leave (ರಚನೆ cs_ಸ್ಥಿತಿ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &ಸ್ಥಿತಿ_2->sigs, ಖಾಲಿ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ಎಣಿಕೆ { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ರಚನೆ sortfile
{
  /* The file's name.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ಸ್ಥಿರ Hash_ಕೋಷ್ಟಕ *proctab;

ಎಣಿಕೆ { INIT_PROCTAB_ಗಾತ್ರ = 47 };

ಸ್ಥಿರ size_t
proctab_hasher (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *ನಮೂದು, size_t tabsize)
{
  ರಚನೆ tempnode ಸ್ಥಿರಾಂಕ *ಗಂಟು = ನಮೂದು;
  ಹಿಂತಿರುಗಿ ಗಂಟು->pid % tabsize;
}

ಸ್ಥಿರ bool
proctab_comparator (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *e1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *e2)
{
  ರಚನೆ tempnode ಸ್ಥಿರಾಂಕ *n1 = e1;
  ರಚನೆ tempnode ಸ್ಥಿರಾಂಕ *n2 = e2;
  ಹಿಂತಿರುಗಿ n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ಸ್ಥಿರ pid_t nprocs;

ಸ್ಥಿರ bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ಸ್ಥಿರ pid_t
reap (pid_t pid)
{
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
  pid_t cpid = waitpid ((pid ? pid : -1), &ಸ್ಥಿತಿ_2, (pid ? 0 : WNOHANG));

  ಆದರೆ (cpid < 0)
    error (ವಿಂಗಡಿಸು_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_ಕಾರ್ಯಕ್ರಮ));
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ಆದರೆ (! WIFEXITED (ಸ್ಥಿತಿ_2) || WEXITSTATUS (ಸ್ಥಿತಿ_2))
        error (ವಿಂಗಡಿಸು_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_ಕಾರ್ಯಕ್ರಮ));
      --nprocs;
    }

  ಹಿಂತಿರುಗಿ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ರಿಜಿಸ್ಟರ್_proc (ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ)
{
  ಆದರೆ (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_ಗಾತ್ರ, ಖಾಲಿ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ಖಾಲಿ);
      ಆದರೆ (! proctab)
        xalloc_die ();
    }

  ತಾತ್ಕಾಲಿಕ->state = UNREAPED;

  ಆದರೆ (! hash_ಸೇರಿಸು (proctab, ತಾತ್ಕಾಲಿಕ))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ಸ್ಥಿರ bool
delete_proc (pid_t pid)
{
  ರಚನೆ tempnode test;

  test.pid = pid;
  ರಚನೆ tempnode *ಗಂಟು = hash_remove (proctab, &test);
  ಆದರೆ (! ಗಂಟು)
    ಹಿಂತಿರುಗಿ false;
  ಗಂಟು->state = REAPED;
  ಹಿಂತಿರುಗಿ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ಸ್ಥಿರ ಶೂನ್ಯ
wait_proc (pid_t pid)
{
  ಆದರೆ (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ಸ್ಥಿರ ಶೂನ್ಯ
reap_exited (ಶೂನ್ಯ)
{
  ಆಗ (0 < nprocs && reap (0))
    ಮುಂದುವರಿ;
}

/* Reap at least one exited child, waiting if necessary.  */

ಸ್ಥಿರ ಶೂನ್ಯ
reap_some (ಶೂನ್ಯ)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ಸ್ಥಿರ ಶೂನ್ಯ
reap_all (ಶೂನ್ಯ)
{
  ಆಗ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ಸ್ಥಿರ ಶೂನ್ಯ
exit_cleanup (ಶೂನ್ಯ)
{
  ಆದರೆ (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ರಚನೆ cs_ಸ್ಥಿತಿ cs;
      cs_enter (&cs);
      cleanup ();
      cs_leave (&cs);
    }

  close_stdout ();
}

/* Create a new temporary file, returning its newly allocated tempnode.
   Store into *PFD the file descriptor open for writing.
   If the creation fails, return NULL and store -1 into *PFD if the
   failure is due to file descriptor exhaustion and
   SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ಸ್ಥಿರ ರಚನೆ tempnode *
create_ತಾತ್ಕಾಲಿಕ_file (ಪೂರ್ಣಾಂಕ *pfd, bool survive_fd_exhaustion)
{
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ slashbase[] = "/sortXXXXXX";
  ಸ್ಥಿರ idx_t ತಾತ್ಕಾಲಿಕ_dir_ಸೂಚ್ಯಂಕ;
  ಪೂರ್ಣಾಂಕ fd;
  ಪೂರ್ಣಾಂಕ saved_errno;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_dir = ತಾತ್ಕಾಲಿಕ_dirs.dirs[ತಾತ್ಕಾಲಿಕ_dir_ಸೂಚ್ಯಂಕ];
  size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ತಾತ್ಕಾಲಿಕ_dir);
  ರಚನೆ tempnode *ಗಂಟು =
    xmalloc (FLEXSIZEOF (ರಚನೆ tempnode, name, len + ಗಾತ್ರ slashbase));
  ಅಕ್ಷರ *file = ಗಂಟು->name;
  ರಚನೆ cs_ಸ್ಥಿತಿ cs;

  ಸ್ಮರಣೆ_ನಕಲು (file, ತಾತ್ಕಾಲಿಕ_dir, len);
  ಸ್ಮರಣೆ_ನಕಲು (file + len, slashbase, ಗಾತ್ರ slashbase);
  ಗಂಟು->ಮುಂದಿನ = ಖಾಲಿ;
  ಆದರೆ (++ತಾತ್ಕಾಲಿಕ_dir_ಸೂಚ್ಯಂಕ == ತಾತ್ಕಾಲಿಕ_dirs.n_dirs)
    ತಾತ್ಕಾಲಿಕ_dir_ಸೂಚ್ಯಂಕ = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ಆದರೆ (0 <= fd)
    {
      *temptail = ಗಂಟು;
      temptail = &ಗಂಟು->ಮುಂದಿನ;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ಆದರೆ (fd < 0)
    {
      ಆದರೆ (! (survive_fd_exhaustion && errno == EMFILE))
        error (ವಿಂಗಡಿಸು_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (ತಾತ್ಕಾಲಿಕ_dir));
      ಬಿಡುಗಡೆ (ಗಂಟು);
      ಗಂಟು = ಖಾಲಿ;
    }

  *pfd = fd;
  ಹಿಂತಿರುಗಿ ಗಂಟು;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ಸ್ಥಿರ ರಚನೆ stat *
get_outstatus (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ outstat_errno;
  ಸ್ಥಿರ ರಚನೆ stat outstat;
  ಆದರೆ (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  ಹಿಂತಿರುಗಿ outstat_errno < 0 ? &outstat : ಖಾಲಿ;
}

/* Return a stream for FILE, opened with mode HOW.  If HOW is "w",
   the file is already open on standard output, and needs to be
   truncated unless FILE is null.  When opening for input, "-"
   means standard input.  To avoid confusion, do not return file
   descriptors STDIN_FILENO, STDOUT_FILENO, or STDERR_FILENO when
   opening an ordinary FILE.  Return NULL if unsuccessful.

   Use fadvise to specify an access pattern for input files.
   There are a few hints we could possibly provide,
   and after careful testing it was decided that
   specifying FADVISE_SEQUENTIAL was not detrimental
   to any cases.  On Linux 2.6.31, this option doubles
   the size of read ahead performed and thus was seen to
   benefit these cases:
     Merging
     Sorting with a smaller internal buffer
     Reading from faster flash devices

   In _addition_ one could also specify other hints...

   FADVISE_WILLNEED was tested, but Linux 2.6.31
   at least uses that to _synchronously_ prepopulate the cache
   with the specified range.  While sort does need to
   read all of its input before outputting, a synchronous
   read of the whole file up front precludes any processing
   that sort could do in parallel with the system doing
   read ahead of the data. This was seen to have negative effects
   in a couple of cases:
     Merging
     Sorting with a smaller internal buffer
   This option was seen to shorten the runtime for sort
   on a multicore system with lots of RAM and other processes
   competing for CPU.  It could be argued that more explicit
   scheduling hints with 'nice' et. al. are more appropriate
   for this situation.

   FADVISE_NOREUSE is a possibility as it could lower
   the priority of input data in the cache as sort will
   only need to process it once.  However its functionality
   has changed over Linux kernel versions and as of 2.6.31
   it does nothing and thus we can't depend on what it might
   do in future.

   FADVISE_DONTNEED is not appropriate for user specified
   input files, but for temp files we do want to drop the
   cache immediately after processing.  This is done implicitly
   however when the files are unlinked.  */

ಸ್ಥಿರ FILE *
ಹರಿವು_open (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *how)
{
  FILE *fp;

  ಆದರೆ (*how == 'r')
    {
      ಆದರೆ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ಖಾಲಿ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*how == 'w')
    {
      ಆದರೆ (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          ಪೂರ್ಣಾಂಕ ftruncate_errno = errno;
          ರಚನೆ stat *outst = get_outstatus ();
          ಆದರೆ (!outst || S_ISREG (outst->st_ಬಹುಲಕ) || S_TYPEISSHM (outst))
            error (ವಿಂಗಡಿಸು_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    affirm (!"unexpected mode passed to stream_open");

  ಹಿಂತಿರುಗಿ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ಸ್ಥಿರ FILE *
xfopen (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *how)
{
  FILE *fp = ಹರಿವು_open (file, how);
  ಆದರೆ (!fp)
    ವಿಂಗಡಿಸು_die (_("open failed"), file);
  ಹಿಂತಿರುಗಿ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xfclose (FILE *fp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಆಯ್ಕೆ (fileno (fp))
    {
    ಸ್ಥಿತಿ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ಆದರೆ (fflush (fp) != 0)
        ವಿಂಗಡಿಸು_die (_("fflush failed"), file);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಆದರೆ (fclose (fp) != 0)
        ವಿಂಗಡಿಸು_die (_("close failed"), file);
      ನಿಲ್ಲಿಸು;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ಸ್ಥಿರ ಶೂನ್ಯ
move_fd (ಪೂರ್ಣಾಂಕ oldfd, ಪೂರ್ಣಾಂಕ newfd)
{
  ಆದರೆ (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_ಮೌಲ್ಯ (dup2 (oldfd, newfd));
      ignore_ಮೌಲ್ಯ (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  ಪೂರ್ಣಾಂಕ oldfd, ಪೂರ್ಣಾಂಕ newfd)
{
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = 0;
  ಆದರೆ (oldfd != newfd)
    {
      ಫಲಿತಾಂಶ = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ಆದರೆ (ಫಲಿತಾಂಶ == 0)
        ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
get_resolved_compress_ಕಾರ್ಯಕ್ರಮ (ಶೂನ್ಯ)
{
  /* Use a cache, to perform the search only once.  */
  ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *resolved_compress_ಕಾರ್ಯಕ್ರಮ_cache /* = NULL */;

  ಆದರೆ (resolved_compress_ಕಾರ್ಯಕ್ರಮ_cache == ಖಾಲಿ)
    {
      resolved_compress_ಕಾರ್ಯಕ್ರಮ_cache =
        ಕಂಡುಹಿಡಿ_ಒಳ_given_ಮಾರ್ಗ (compress_ಕಾರ್ಯಕ್ರಮ, getenv ("PATH"), ಖಾಲಿ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  ಹಿಂತಿರುಗಿ resolved_compress_ಕಾರ್ಯಕ್ರಮ_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
pipe_ಮಗು (pid_t *pid, ಪೂರ್ಣಾಂಕ pipefds[2], ಪೂರ್ಣಾಂಕ tempfd, bool decompress,
            size_t tries)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *resolved_compress_ಕಾರ್ಯಕ್ರಮ;
  ರಚನೆ tempnode *saved_temphead;
  ದ್ವಿಗುಣ wait_retry = 0.25;
  ರಚನೆ cs_ಸ್ಥಿತಿ cs;
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_ಕಾರ್ಯಕ್ರಮ = get_resolved_compress_ಕಾರ್ಯಕ್ರಮ ();
  ಆದರೆ (resolved_compress_ಕಾರ್ಯಕ್ರಮ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ errno;

  ಆದರೆ ((ಫಲಿತಾಂಶ = posix_spawnattr_init (&attr)))
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
  ಆದರೆ ((ಫಲಿತಾಂಶ = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

  ಆದರೆ (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ಹಿಂತಿರುಗಿ saved_errno;
    }

  ಆದರೆ ((ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((ಫಲಿತಾಂಶ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (ಫಲಿತಾಂಶ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ argv[] =
    {
      resolved_compress_ಕಾರ್ಯಕ್ರಮ,
      decompress ? "-d" : ಖಾಲಿ,
      ಖಾಲಿ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ಆದರೆ (nmerge + 1 < nprocs)
    reap_some ();

  ಆಗ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ಖಾಲಿ;

      ಫಲಿತಾಂಶ = posix_spawnp (pid, resolved_compress_ಕಾರ್ಯಕ್ರಮ, &actions, &attr,
                             (ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ಆದರೆ (ಫಲಿತಾಂಶ != EAGAIN)
        ನಿಲ್ಲಿಸು;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* [v]fork/clone are indicating resource constraints,
             so back-off for a while and retry.  */
          xnanosleep (wait_retry);
          wait_retry *= 2;
          reap_exited ();
        }
    }

  posix_spawnattr_destroy (&attr);
  posix_spawn_file_actions_destroy (&actions);

  ಆದರೆ (ಫಲಿತಾಂಶ)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ++nprocs;

  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ಸ್ಥಿರ ರಚನೆ tempnode *
maybe_create_ತಾತ್ಕಾಲಿಕ (FILE **pfp, bool survive_fd_exhaustion)
{
  ಪೂರ್ಣಾಂಕ tempfd;
  ರಚನೆ tempnode *ಗಂಟು = create_ತಾತ್ಕಾಲಿಕ_file (&tempfd, survive_fd_exhaustion);
  ಆದರೆ (! ಗಂಟು)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಗಂಟು->state = UNCOMPRESSED;

  ಆದರೆ (compress_ಕಾರ್ಯಕ್ರಮ)
    {
      ಪೂರ್ಣಾಂಕ pipefds[2];
      ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಫಲಿತಾಂಶ = 0;

      ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = pipe_ಮಗು (&ಗಂಟು->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ಆದರೆ (ಫಲಿತಾಂಶ)
        {
          ಆದರೆ (ಫಲಿತಾಂಶ != ಕೊನೆಯ_ಫಲಿತಾಂಶ)
            error (0, ಫಲಿತಾಂಶ, _("could not run compress program %s"),
                   quoteaf (compress_ಕಾರ್ಯಕ್ರಮ));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          ರಿಜಿಸ್ಟರ್_proc (ಗಂಟು);
        }

      ಕೊನೆಯ_ಫಲಿತಾಂಶ = ಫಲಿತಾಂಶ;
    }

  *pfp = fdopen (tempfd, "w");
  ಆದರೆ (! *pfp)
    ವಿಂಗಡಿಸು_die (_("couldn't create temporary file"), ಗಂಟು->name);

  ಹಿಂತಿರುಗಿ ಗಂಟು;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ಸ್ಥಿರ ರಚನೆ tempnode *
create_ತಾತ್ಕಾಲಿಕ (FILE **pfp)
{
  ಹಿಂತಿರುಗಿ maybe_create_ತಾತ್ಕಾಲಿಕ (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ಸ್ಥಿರ FILE *
open_ತಾತ್ಕಾಲಿಕ (ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ)
{
  ಪೂರ್ಣಾಂಕ tempfd, pipefds[2];
  FILE *fp = ಖಾಲಿ;

  ಆದರೆ (ತಾತ್ಕಾಲಿಕ->state == UNREAPED)
    wait_proc (ತಾತ್ಕಾಲಿಕ->pid);

  tempfd = open (ತಾತ್ಕಾಲಿಕ->name, O_RDONLY);
  ಆದರೆ (tempfd < 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  pid_t ಮಗು;
  ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ = pipe_ಮಗು (&ಮಗು, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ಆದರೆ (ಫಲಿತಾಂಶ)
    {
      ಆದರೆ (ಫಲಿತಾಂಶ != EMFILE)
        error (ವಿಂಗಡಿಸು_FAILURE, ಫಲಿತಾಂಶ, _("could not run compress program %s -d"),
               quoteaf (compress_ಕಾರ್ಯಕ್ರಮ));
      close (tempfd);
      errno = EMFILE;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ತಾತ್ಕಾಲಿಕ->pid = ಮಗು;
      ರಿಜಿಸ್ಟರ್_proc (ತಾತ್ಕಾಲಿಕ);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ಆದರೆ (! fp)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  ಹಿಂತಿರುಗಿ fp;
}

/* Append DIR to the array of temporary directory names.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ_dir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir)
{
  ಆದರೆ (ತಾತ್ಕಾಲಿಕ_dirs.n_dirs == ತಾತ್ಕಾಲಿಕ_dirs.n_dirs_alloc)
    ತಾತ್ಕಾಲಿಕ_dirs.dirs = xpalloc (ತಾತ್ಕಾಲಿಕ_dirs.dirs, &ತಾತ್ಕಾಲಿಕ_dirs.n_dirs_alloc, 1, -1,
                              ಗಾತ್ರ *ತಾತ್ಕಾಲಿಕ_dirs.dirs);

  idx_t ಸ್ಥಿರಾಂಕ dir_idx = ತಾತ್ಕಾಲಿಕ_dirs.n_dirs++;
  ತಾತ್ಕಾಲಿಕ_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
zaptemp (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ರಚನೆ tempnode *ಅಸ್ಥಿರ *pnode;
  ರಚನೆ tempnode *ಗಂಟು;
  ರಚನೆ tempnode *ಮುಂದಿನ;
  ಪೂರ್ಣಾಂಕ unlink_ಸ್ಥಿತಿ;
  ಪೂರ್ಣಾಂಕ unlink_errno = 0;
  ರಚನೆ cs_ಸ್ಥಿತಿ cs;

  ಗಾಗಿ (pnode = &temphead; (ಗಂಟು = *pnode)->name != name; pnode = &ಗಂಟು->ಮುಂದಿನ)
    ಮುಂದುವರಿ;

  ಆದರೆ (ಗಂಟು->state == UNREAPED)
    wait_proc (ಗಂಟು->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  ಮುಂದಿನ = ಗಂಟು->ಮುಂದಿನ;
  cs_enter (&cs);
  unlink_ಸ್ಥಿತಿ = unlink (name);
  unlink_errno = errno;
  *pnode = ಮುಂದಿನ;
  cs_leave (&cs);

  ಆದರೆ (unlink_ಸ್ಥಿತಿ != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ಆದರೆ (! ಮುಂದಿನ)
    temptail = pnode;
  ಬಿಡುಗಡೆ (ಗಂಟು);
}

#ಆದರೆ HAVE_NL_LANGINFO

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
struct_month_cmp (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m1, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *m2)
{
  ರಚನೆ month ಸ್ಥಿರಾಂಕ *month1 = m1;
  ರಚನೆ month ಸ್ಥಿರಾಂಕ *month2 = m2;
  ಹಿಂತಿರುಗಿ strcmp (month1->name, month2->name);
}

#ಆದರೆ_ಅಂತ್ಯ

/* Initialize the character class tables. */

ಸ್ಥಿರ ಶೂನ್ಯ
inittables (ಶೂನ್ಯ)
{
  ಗಾಗಿ (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! ಅಕ್ಷರವೇ_ಅಂಕಿಯೇ (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = ದೊಡ್ಡ_ಅಕ್ಷರ (i);
    }

#ಆದರೆ HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ಆದರೆ (hard_LC_ಸಮಯ)
    {
      ಗಾಗಿ (size_t i = 0; i < MONTHS_ಪ್ರತಿ_YEAR; i++)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s;
          size_t s_len;
          size_t j, k;
          ಅಕ್ಷರ *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          ಗಾಗಿ (j = k = 0; j < s_len; j++)
            ಆದರೆ (! isblank (ಗಮ್ಯ_uchar (s[j])))
              name[k++] = fold_toupper[ಗಮ್ಯ_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_ಪ್ರತಿ_YEAR, ಗಾತ್ರ *monthtab, struct_month_cmp);
    }
#ಆದರೆ_ಅಂತ್ಯ
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ಸ್ಥಿರ ಶೂನ್ಯ
specify_nmerge (ಪೂರ್ಣಾಂಕ oi, ಅಕ್ಷರ c, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  uintmax_t n;
  ರಚನೆ rlimit rlimit;
  ಎಣಿಕೆ strtol_error e = xstrtoumax (s, ಖಾಲಿ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ಆದರೆ (e == LONGINT_OK)
    {
      nmerge = n;
      ಆದರೆ (nmerge != n)
        e = LONGINT_ಅತಿಪ್ರವಾಹ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (ವಿಂಗಡಿಸು_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (max_nmerge < nmerge)
            {
              e = LONGINT_ಅತಿಪ್ರವಾಹ;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ;
        }
    }

  ಆದರೆ (e == LONGINT_ಅತಿಪ್ರವಾಹ)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (ವಿಂಗಡಿಸು_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ಸ್ಥಿರ ಶೂನ್ಯ
specify_ವಿಂಗಡಿಸು_ಗಾತ್ರ (ಪೂರ್ಣಾಂಕ oi, ಅಕ್ಷರ c, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  uintmax_t n;
  ಅಕ್ಷರ *suffix;
  ಎಣಿಕೆ strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ಆದರೆ (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_ಅತಿಪ್ರವಾಹ;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ಆದರೆ (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    ಆಯ್ಕೆ (suffix[0])
      {
      ಸ್ಥಿತಿ 'b':
        e = LONGINT_OK;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ '%':
        {
          ದ್ವಿಗುಣ mem = physmem_ಒಟ್ಟು () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ಆದರೆ (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            e = LONGINT_ಅತಿಪ್ರವಾಹ;
        }
        ನಿಲ್ಲಿಸು;
      }

  ಆದರೆ (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ಆದರೆ (n < ವಿಂಗಡಿಸು_ಗಾತ್ರ)
        ಹಿಂತಿರುಗಿ;

      ವಿಂಗಡಿಸು_ಗಾತ್ರ = n;
      ಆದರೆ (ವಿಂಗಡಿಸು_ಗಾತ್ರ == n)
        {
          ವಿಂಗಡಿಸು_ಗಾತ್ರ = MAX (ವಿಂಗಡಿಸು_ಗಾತ್ರ, MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ);
          ಹಿಂತಿರುಗಿ;
        }

      e = LONGINT_ಅತಿಪ್ರವಾಹ;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ಸ್ಥಿರ size_t
specify_nthreads (ಪೂರ್ಣಾಂಕ oi, ಅಕ್ಷರ c, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  uintmax_t nthreads;
  ಎಣಿಕೆ strtol_error e = xstrtoumax (s, ಖಾಲಿ, 10, &nthreads, "");
  ಆದರೆ (e == LONGINT_ಅತಿಪ್ರವಾಹ)
    ಹಿಂತಿರುಗಿ SIZE_MAX;
  ಆದರೆ (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ಆದರೆ (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ಆದರೆ (nthreads == 0)
    error (ವಿಂಗಡಿಸು_FAILURE, 0, _("number in parallel must be nonzero"));
  ಹಿಂತಿರುಗಿ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ಸ್ಥಿರ size_t
default_ವಿಂಗಡಿಸು_ಗಾತ್ರ (ಶೂನ್ಯ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t ಗಾತ್ರ_2 = SIZE_MAX;
  ರಚನೆ rlimit rlimit;
  ಆದರೆ (getrlimit (RLIMIT_ದತ್ತಾಂಶ, &rlimit) == 0 && rlimit.rlim_cur < ಗಾತ್ರ_2)
    ಗಾತ್ರ_2 = rlimit.rlim_cur;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RLIMIT_AS
  ಆದರೆ (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < ಗಾತ್ರ_2)
    ಗಾತ್ರ_2 = rlimit.rlim_cur;
#ಆದರೆ_ಅಂತ್ಯ

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  ಗಾತ್ರ_2 /= 2;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ಆದರೆ (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < ಗಾತ್ರ_2)
    ಗಾತ್ರ_2 = rlimit.rlim_cur / 16 * 15;
#ಆದರೆ_ಅಂತ್ಯ

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ದ್ವಿಗುಣ avail = physmem_ಲಭ್ಯ ();
  ದ್ವಿಗುಣ ಒಟ್ಟು = physmem_ಒಟ್ಟು ();
  ದ್ವಿಗುಣ mem = MAX (avail, ಒಟ್ಟು / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ಆದರೆ (ಒಟ್ಟು * 0.75 < ಗಾತ್ರ_2)
    ಗಾತ್ರ_2 = ಒಟ್ಟು * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ಆದರೆ (mem < ಗಾತ್ರ_2)
    ಗಾತ್ರ_2 = mem;
  ಹಿಂತಿರುಗಿ MAX (ಗಾತ್ರ_2, MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ಸ್ಥಿರ bool
ಸಾಲು_aligned_ಗಾತ್ರ (size_t *alloc)
{
  size_t ಗಾತ್ರ_2 = *alloc;
  size_t alignment = ಗಾತ್ರ (ರಚನೆ ಸಾಲು);
  size_t padding = alignment - ಗಾತ್ರ_2 % alignment;
  size_t aligned;

  ಆದರೆ (ckd_ಕೂಡಿಸು (&aligned, ಗಾತ್ರ_2, padding))
    ಹಿಂತಿರುಗಿ false;

  *alloc = aligned;
  ಹಿಂತಿರುಗಿ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ಸ್ಥಿರ size_t
default_initial_ವಿಂಗಡಿಸು_ಗಾತ್ರ (size_t ಸಾಲು_ಬೈಟುಗಳು)
{
  size_t ಗಾತ್ರ_2 = ಸಾಲು_ಬೈಟುಗಳು + 2;
  size_t ಒಳಹರಿವು_ಗಾತ್ರ;

  ಆದರೆ (ckd_mul (&ಒಳಹರಿವು_ಗಾತ್ರ, ಒಳಹರಿವು_FILE_ಗಾತ್ರ_GUESS, ಸಾಲು_ಬೈಟುಗಳು + 1)
      || ckd_ಕೂಡಿಸು (&ಗಾತ್ರ_2, ಗಾತ್ರ_2, ಒಳಹರಿವು_ಗಾತ್ರ))
    ಹಿಂತಿರುಗಿ SIZE_MAX;

  ಹಿಂತಿರುಗಿ MAX (ಗಾತ್ರ_2, MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ಸ್ಥಿರ bool
ಒಳಹರಿವು_ಗಾತ್ರ_ಬಫರ್_ಬೈಟುಗಳು (uintmax_t file_ಗಾತ್ರ, size_t ಸಾಲು_ಬೈಟುಗಳು,
                         size_t *worst_case)
{
  size_t worst_case_ಪ್ರತಿ_ಒಳಹರಿವು_byte = ಸಾಲು_ಬೈಟುಗಳು + 1;

  ಆದರೆ (SIZE_MAX / worst_case_ಪ್ರತಿ_ಒಳಹರಿವು_byte < file_ಗಾತ್ರ)
    ಹಿಂತಿರುಗಿ false;

  size_t ಗಾತ್ರ_2 = file_ಗಾತ್ರ * worst_case_ಪ್ರತಿ_ಒಳಹರಿವು_byte;
  ಆದರೆ (SIZE_MAX - ಗಾತ್ರ_2 < 1)
    ಹಿಂತಿರುಗಿ false;

  *worst_case = ಗಾತ್ರ_2 + 1;
  ಹಿಂತಿರುಗಿ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು_ಬಫರ್_policy (FILE *ಸ್ಥಿರಾಂಕ *fps, size_t nfps,
                    ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *files, size_t nfiles,
                    size_t ಸಾಲು_ಬೈಟುಗಳು,
                    ರಚನೆ ವಿಂಗಡಿಸು_ಬಫರ್_policy *policy)
{
  size_t ಗಾತ್ರ_bound = ವಿಂಗಡಿಸು_ಗಾತ್ರ ? ವಿಂಗಡಿಸು_ಗಾತ್ರ : default_ವಿಂಗಡಿಸು_ಗಾತ್ರ ();
  size_t initial_bound =
    ವಿಂಗಡಿಸು_ಗಾತ್ರ ? ಗಾತ್ರ_bound : default_initial_ವಿಂಗಡಿಸು_ಗಾತ್ರ (ಸಾಲು_ಬೈಟುಗಳು);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_ಪ್ರತಿ_ಒಳಹರಿವು_byte = ಸಾಲು_ಬೈಟುಗಳು + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t ಗಾತ್ರ_2 = worst_case_ಪ್ರತಿ_ಒಳಹರಿವು_byte + 1;

  initial_bound = MIN (initial_bound, ಗಾತ್ರ_bound);
  policy->ಎಲ್ಲೆ = ಗಾತ್ರ_bound;
  policy->growth_failed = false;

  ಗಾಗಿ (size_t i = 0; i < nfiles; i++)
    {
      ರಚನೆ stat st;
      off_t file_ಗಾತ್ರ;
      size_t worst_case;
      bool ತಿಳಿದ_ಗಾತ್ರ;

      ಆದರೆ (ಗಾತ್ರ_bound <= ಗಾತ್ರ_2)
        {
          policy->initial = ಗಾತ್ರ_bound;
          ಹಿಂತಿರುಗಿ;
        }

      ಆದರೆ ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        ವಿಂಗಡಿಸು_die (_("stat failed"), files[i]);

      ತಿಳಿದ_ಗಾತ್ರ = usable_st_ಗಾತ್ರ (&st) && 0 < st.st_ಗಾತ್ರ;
      ಆದರೆ (ತಿಳಿದ_ಗಾತ್ರ)
        file_ಗಾತ್ರ = st.st_ಗಾತ್ರ;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ಆದರೆ (ವಿಂಗಡಿಸು_ಗಾತ್ರ)
            {
              policy->initial = ಗಾತ್ರ_bound;
              ಹಿಂತಿರುಗಿ;
            }
          file_ಗಾತ್ರ = ಒಳಹರಿವು_FILE_ಗಾತ್ರ_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ಆದರೆ (! ಒಳಹರಿವು_ಗಾತ್ರ_ಬಫರ್_ಬೈಟುಗಳು (file_ಗಾತ್ರ, ಸಾಲು_ಬೈಟುಗಳು, &worst_case)
          || ಗಾತ್ರ_bound - ಗಾತ್ರ_2 <= worst_case)
        {
          policy->initial = ಗಾತ್ರ_bound;
          ಹಿಂತಿರುಗಿ;
        }

      ಆದರೆ (! ತಿಳಿದ_ಗಾತ್ರ && ! ವಿಂಗಡಿಸು_ಗಾತ್ರ
          && (initial_bound <= ಗಾತ್ರ_2 || initial_bound - ಗಾತ್ರ_2 <= worst_case))
        {
          policy->initial = MAX (ಗಾತ್ರ_2, initial_bound);
          ಹಿಂತಿರುಗಿ;
        }

      ಗಾತ್ರ_2 += worst_case;
    }

  policy->initial = MAX (ಗಾತ್ರ_2, MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ಸ್ಥಿರ ಶೂನ್ಯ
initbuf (ರಚನೆ ಬಫರ್ *buf, size_t ಸಾಲು_ಬೈಟುಗಳು, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ಆಗ (true)
    {
      ಆದರೆ (! ಸಾಲು_aligned_ಗಾತ್ರ (&alloc))
        buf->buf = ಖಾಲಿ;
      ಇಲ್ಲದಿದ್ದರೆ
        buf->buf = ಸ್ಮರಣೆ_ಕೊಡು (alloc);
      ಆದರೆ (buf->buf)
        ನಿಲ್ಲಿಸು;
      alloc /= 2;
      ಆದರೆ (alloc <= ಸಾಲು_ಬೈಟುಗಳು + 1)
        xalloc_die ();
    }

  buf->ಸಾಲು_ಬೈಟುಗಳು = ಸಾಲು_ಬೈಟುಗಳು;
  buf->alloc = alloc;
  buf->ಬಳಸಿದ = buf->ಎಡ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ ಸಾಲು *
ಬಫರ್_linelim (ರಚನೆ ಬಫರ್ ಸ್ಥಿರಾಂಕ *buf)
{
  ಶೂನ್ಯ *linelim = buf->buf + buf->alloc;
  ಹಿಂತಿರುಗಿ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ಸ್ಥಿರ bool
try_growbuf (ರಚನೆ ಬಫರ್ *buf, size_t alloc)
{
  ಆದರೆ (! ಸಾಲು_aligned_ಗಾತ್ರ (&alloc) || alloc <= buf->alloc)
    ಹಿಂತಿರುಗಿ false;

  ಅಕ್ಷರ *newbuf = ಸ್ಮರಣೆ_ಕೊಡು (alloc);
  ಆದರೆ (! newbuf)
    ಹಿಂತಿರುಗಿ false;

  ಅಕ್ಷರ *oldbuf = buf->buf;
  ರಚನೆ ಸಾಲು *old_linelim = ಬಫರ್_linelim (buf);
  ರಚನೆ ಸಾಲು *old_ಸಾಲು = old_linelim - buf->nlines;

  ಸ್ಮರಣೆ_ನಕಲು (newbuf, oldbuf, buf->ಬಳಸಿದ);

  ರಚನೆ ಸಾಲು *new_linelim = (ಶೂನ್ಯ *) (newbuf + alloc);
  ರಚನೆ ಸಾಲು *new_ಸಾಲು = new_linelim - buf->nlines;
  ಸ್ಮರಣೆ_ನಕಲು (new_ಸಾಲು, old_ಸಾಲು, buf->nlines * ಗಾತ್ರ *new_ಸಾಲು);

  ಗಾಗಿ (ರಚನೆ ಸಾಲು *ಸಾಲು = new_ಸಾಲು; ಸಾಲು < new_linelim; ಸಾಲು++)
    {
      ಸಾಲು->ಪಠ್ಯ = newbuf + (ಸಾಲು->ಪಠ್ಯ - oldbuf);
      ಆದರೆ (keylist)
        {
          ಸಾಲು->keybeg = newbuf + (ಸಾಲು->keybeg - oldbuf);
          ಸಾಲು->keylim = newbuf + (ಸಾಲು->keylim - oldbuf);
        }
    }

  ಬಿಡುಗಡೆ (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  ಹಿಂತಿರುಗಿ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ಸ್ಥಿರ bool
maybe_growbuf (ರಚನೆ ಬಫರ್ *buf, ರಚನೆ ವಿಂಗಡಿಸು_ಬಫರ್_policy *policy)
{
  ಆದರೆ (! policy || policy->growth_failed || policy->ಎಲ್ಲೆ <= buf->alloc)
    ಹಿಂತಿರುಗಿ false;

  size_t alloc;
  ಆದರೆ (buf->alloc <= policy->ಎಲ್ಲೆ / 3)
    alloc = buf->alloc * 3;
  ಇಲ್ಲದಿದ್ದರೆ
    alloc = policy->ಎಲ್ಲೆ;

  ಆದರೆ (try_growbuf (buf, alloc))
    ಹಿಂತಿರುಗಿ true;

  policy->growth_failed = true;
  ಹಿಂತಿರುಗಿ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ಸ್ಥಿರ ಅಕ್ಷರ *
begfield (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಅಕ್ಷರ *ptr = ಸಾಲು->ಪಠ್ಯ, *lim = ptr + ಸಾಲು->ಉದ್ದಳತೆ - 1;
  size_t sword = ಕೀಲಿ->sword;
  size_t schar = ಕೀಲಿ->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ಆದರೆ (tab != TAB_DEFAULT)
    ಆಗ (ptr < lim && sword--)
      {
        ಅಕ್ಷರ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ಆದರೆ (ptr < lim)
          ++ptr;
      }
  ಇಲ್ಲದಿದ್ದರೆ
    ಆಗ (ptr < lim && sword--)
      {
        ಆಗ (ptr < lim && blanks[ಗಮ್ಯ_uchar (*ptr)])
          ++ptr;
        ಆಗ (ptr < lim && !blanks[ಗಮ್ಯ_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ಆದರೆ (ಕೀಲಿ->skipsblanks)
    ಆಗ (ptr < lim && blanks[ಗಮ್ಯ_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t ಉಳಿದ_ಬೈಟುಗಳು = lim - ptr;
  ಆದರೆ (schar < ಉಳಿದ_ಬೈಟುಗಳು)
    ptr += schar;
  ಇಲ್ಲದಿದ್ದರೆ
    ptr = lim;

  ಹಿಂತಿರುಗಿ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಅಕ್ಷರ *
limfield (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಅಕ್ಷರ *ptr = ಸಾಲು->ಪಠ್ಯ, *lim = ptr + ಸಾಲು->ಉದ್ದಳತೆ - 1;
  size_t eword = ಕೀಲಿ->eword, echar = ಕೀಲಿ->echar;

  ಆದರೆ (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ಆದರೆ (tab != TAB_DEFAULT)
    ಆಗ (ptr < lim && eword--)
      {
        ಅಕ್ಷರ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ಆದರೆ (ptr < lim && (eword || echar))
          ++ptr;
      }
  ಇಲ್ಲದಿದ್ದರೆ
    ಆಗ (ptr < lim && eword--)
      {
        ಆಗ (ptr < lim && blanks[ಗಮ್ಯ_uchar (*ptr)])
          ++ptr;
        ಆಗ (ptr < lim && !blanks[ಗಮ್ಯ_uchar (*ptr)])
          ++ptr;
      }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ POSIX_UNSPECIFIED
  /* The following block of code makes GNU sort incompatible with
     standard Unix sort, so it's ifdef'd out for now.
     The POSIX spec isn't clear on how to interpret this.
     FIXME: request clarification.

     From: kwzh@gnu.ai.mit.edu (Karl Heuer)
     Date: Thu, 30 May 96 12:20:41 -0400
     [Translated to POSIX 1003.1-2001 terminology by Paul Eggert.]

     [...]I believe I've found another bug in 'sort'.

     $ cat /tmp/sort.in
     a b c 2 d
     pq rs 1 t
     $ textutils-1.15/src/sort -k1.7,1.7 </tmp/sort.in
     a b c 2 d
     pq rs 1 t
     $ /bin/sort -k1.7,1.7 </tmp/sort.in
     pq rs 1 t
     a b c 2 d

     Unix sort produced the answer I expected: sort on the single character
     in column 7.  GNU sort produced different results, because it disagrees
     on the interpretation of the key-end spec "M.N".  Unix sort reads this
     as "skip M-1 fields, then N-1 characters"; but GNU sort wants it to mean
     "skip M-1 fields, then either N-1 characters or the rest of the current
     field, whichever comes first".  This extra clause applies only to
     key-ends, not key-starts.
     */

  /* Make LIM point to the end of (one byte past) the current field.  */
  ಆದರೆ (tab != TAB_DEFAULT)
    {
      ಅಕ್ಷರ *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ಆದರೆ (newlim)
        lim = newlim;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ *newlim;
      newlim = ptr;
      ಆಗ (newlim < lim && blanks[ಗಮ್ಯ_uchar (*newlim)])
        ++newlim;
      ಆಗ (newlim < lim && !blanks[ಗಮ್ಯ_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ಆದರೆ (ಕೀಲಿ->skipeblanks)
        ಆಗ (ptr < lim && blanks[ಗಮ್ಯ_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t ಉಳಿದ_ಬೈಟುಗಳು = lim - ptr;
      ಆದರೆ (echar < ಉಳಿದ_ಬೈಟುಗಳು)
        ptr += echar;
      ಇಲ್ಲದಿದ್ದರೆ
        ptr = lim;
    }

  ಹಿಂತಿರುಗಿ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ಸ್ಥಿರ bool
fillbuf (ರಚನೆ ಬಫರ್ *buf, FILE *fp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file,
         ರಚನೆ ವಿಂಗಡಿಸು_ಬಫರ್_policy *policy)
{
  ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ = keylist;
  ಅಕ್ಷರ eol = eolchar;
  size_t ಸಾಲು_ಬೈಟುಗಳು = buf->ಸಾಲು_ಬೈಟುಗಳು;
  size_t mergesize = ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ - MIN_ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ;

  ಆದರೆ (buf->eof)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (buf->ಬಳಸಿದ != buf->ಎಡ)
    {
      memmove (buf->buf, buf->buf + buf->ಬಳಸಿದ - buf->ಎಡ, buf->ಎಡ);
      buf->ಬಳಸಿದ = buf->ಎಡ;
      buf->nlines = 0;
    }

  ಆಗ (true)
    {
      ಅಕ್ಷರ *ptr = buf->buf + buf->ಬಳಸಿದ;
      ರಚನೆ ಸಾಲು *linelim = ಬಫರ್_linelim (buf);
      ರಚನೆ ಸಾಲು *ಸಾಲು = linelim - buf->nlines;
      size_t avail = (ಅಕ್ಷರ *) linelim - buf->nlines * ಸಾಲು_ಬೈಟುಗಳು - ptr;
      ಅಕ್ಷರ *ಸಾಲು_ಆರಂಭ = buf->nlines ? ಸಾಲು->ಪಠ್ಯ + ಸಾಲು->ಉದ್ದಳತೆ : buf->buf;

      ಆಗ (ಸಾಲು_ಬೈಟುಗಳು + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (ಸಾಲು_ಬೈಟುಗಳು + 1);
          size_t ಬೈಟುಗಳು_read = fread (ptr, 1, readsize, fp);
          ಅಕ್ಷರ *ptrlim = ptr + ಬೈಟುಗಳು_read;
          ಅಕ್ಷರ *p;
          avail -= ಬೈಟುಗಳು_read;

          ಆದರೆ (ಬೈಟುಗಳು_read != readsize)
            {
              ಆದರೆ (ferror (fp))
                ವಿಂಗಡಿಸು_die (_("read failed"), file);
              ಆದರೆ (feof (fp))
                {
                  buf->eof = true;
                  ಆದರೆ (buf->buf == ptrlim)
                    ಹಿಂತಿರುಗಿ false;
                  ಆದರೆ (ಸಾಲು_ಆರಂಭ != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ಆಗ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              ಸಾಲು--;
              ಸಾಲು->ಪಠ್ಯ = ಸಾಲು_ಆರಂಭ;
              ಸಾಲು->ಉದ್ದಳತೆ = ptr - ಸಾಲು_ಆರಂಭ;
              mergesize = MAX (mergesize, ಸಾಲು->ಉದ್ದಳತೆ);
              avail -= ಸಾಲು_ಬೈಟುಗಳು;

              ಆದರೆ (ಕೀಲಿ)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  ಸಾಲು->keylim = (ಕೀಲಿ->eword == SIZE_MAX
                                  ? p
                                  : limfield (ಸಾಲು, ಕೀಲಿ));

                  ಆದರೆ (ಕೀಲಿ->sword != SIZE_MAX)
                    ಸಾಲು->keybeg = begfield (ಸಾಲು, ಕೀಲಿ);
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      ಆದರೆ (ಕೀಲಿ->skipsblanks)
                        ಆಗ (blanks[ಗಮ್ಯ_uchar (*ಸಾಲು_ಆರಂಭ)])
                          ಸಾಲು_ಆರಂಭ++;
                      ಸಾಲು->keybeg = ಸಾಲು_ಆರಂಭ;
                    }
                }

              ಸಾಲು_ಆರಂಭ = ptr;
            }

          ptr = ptrlim;
          ಆದರೆ (buf->eof)
            ನಿಲ್ಲಿಸು;
        }

      buf->ಬಳಸಿದ = ptr - buf->buf;
      buf->nlines = ಬಫರ್_linelim (buf) - ಸಾಲು;
      ಆದರೆ (buf->nlines != 0)
        {
          ಆದರೆ (! buf->eof && maybe_growbuf (buf, policy))
            ಮುಂದುವರಿ;
          buf->ಎಡ = ptr - ಸಾಲು_ಆರಂಭ;
          ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ = mergesize + MIN_ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ;
          ಹಿಂತಿರುಗಿ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t ಸಾಲು_alloc = buf->alloc / ಗಾತ್ರ (ರಚನೆ ಸಾಲು);
        buf->buf = xpalloc (buf->buf, &ಸಾಲು_alloc, 1, -1, ಗಾತ್ರ (ರಚನೆ ಸಾಲು));
        buf->alloc = ಸಾಲು_alloc * ಗಾತ್ರ (ರಚನೆ ಸಾಲು);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ unit_ಕ್ರಮ[UCHAR_LIM] =
  {
#ಆದರೆ ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ಇಲ್ಲದಿದ್ದರೆ
    /* Generate the following table with this command:
       perl -e 'my %a=(k=>1,K=>1,M=>2,G=>3,T=>4,P=>5,E=>6,Z=>7,Y=>8,R=>9,Q=>10);
       foreach my $i (0..255) {my $c=chr($i); $a{$c} ||= 0;print "$a{$c}, "}'\
       |fmt  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 3,
    0, 0, 0, 1, 0, 2, 0, 0, 5, 10, 9, 0, 4, 0, 0, 0, 0, 8, 7, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#ಆದರೆ_ಅಂತ್ಯ
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ಸ್ಥಿರ ಅಕ್ಷರ
traverse_raw_ಸಂಖ್ಯೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ **ಸಂಖ್ಯೆ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = *ಸಂಖ್ಯೆ;
  ಅಕ್ಷರ ch;
  ಅಕ್ಷರ max_digit = '\0';
  bool ತುದಿಗಳು_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ಆಗ (c_isdigit (ch = *p++))
    {
      ಆದರೆ (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ತುದಿಗಳು_with_thousands_sep = (*p == thousands_sep);
      ಆದರೆ (ತುದಿಗಳು_with_thousands_sep)
        ++p;
    }

  ಆದರೆ (ತುದಿಗಳು_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *ಸಂಖ್ಯೆ = p - 2;
      ಹಿಂತಿರುಗಿ max_digit;
    }

  ಆದರೆ (ch == decimal_ಬಿಂದು)
    ಆಗ (c_isdigit (ch = *p++))
      ಆದರೆ (max_digit < ch)
        max_digit = ch;

  *ಸಂಖ್ಯೆ = p - 1;
  ಹಿಂತಿರುಗಿ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಕಂಡುಹಿಡಿ_unit_ಕ್ರಮ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಂಖ್ಯೆ)
{
  bool minus_sign = (*ಸಂಖ್ಯೆ == '-');
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = ಸಂಖ್ಯೆ + minus_sign;
  ಅಕ್ಷರ max_digit = traverse_raw_ಸಂಖ್ಯೆ (&p);
  ಆದರೆ ('0' < max_digit)
    {
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch = *p;
      ಪೂರ್ಣಾಂಕ ಕ್ರಮ = unit_ಕ್ರಮ[ch];
      ಹಿಂತಿರುಗಿ (minus_sign ? -ಕ್ರಮ : ಕ್ರಮ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
human_numcompare (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b)
{
  ಆಗ (blanks[ಗಮ್ಯ_uchar (*a)])
    a++;
  ಆಗ (blanks[ಗಮ್ಯ_uchar (*b)])
    b++;

  ಪೂರ್ಣಾಂಕ diff = ಕಂಡುಹಿಡಿ_unit_ಕ್ರಮ (a) - ಕಂಡುಹಿಡಿ_unit_ಕ್ರಮ (b);
  ಹಿಂತಿರುಗಿ (diff ? diff : strnumcmp (a, b, decimal_ಬಿಂದು, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
numcompare (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *a, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *b)
{
  ಆಗ (blanks[ಗಮ್ಯ_uchar (*a)])
    a++;
  ಆಗ (blanks[ಗಮ್ಯ_uchar (*b)])
    b++;

  ಹಿಂತಿರುಗಿ strnumcmp (a, b, decimal_ಬಿಂದು, thousands_sep);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
nan_compare (ದೀರ್ಘ ದ್ವಿಗುಣ a, ದೀರ್ಘ ದ್ವಿಗುಣ b)
{
  ಅಕ್ಷರ buf[2][ಗಾತ್ರ "-nan""()" + CHAR_BIT * ಗಾತ್ರ a];
  snprintf (buf[0], ಗಾತ್ರ buf[0], "%Lf", a);
  snprintf (buf[1], ಗಾತ್ರ buf[1], "%Lf", b);
  ಹಿಂತಿರುಗಿ strcmp (buf[0], buf[1]);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
general_numcompare (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sa, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  ಅಕ್ಷರ *ea;
  ಅಕ್ಷರ *eb;
  ದೀರ್ಘ ದ್ವಿಗುಣ a = strtold (sa, &ea);
  ದೀರ್ಘ ದ್ವಿಗುಣ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ಆದರೆ (sa == ea)
    ಹಿಂತಿರುಗಿ sb == eb ? 0 : -1;
  ಆದರೆ (sb == eb)
    ಹಿಂತಿರುಗಿ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  ಹಿಂತಿರುಗಿ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
getmonth (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *month, ಅಕ್ಷರ **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_ಪ್ರತಿ_YEAR;

  ಆಗ (blanks[ಗಮ್ಯ_uchar (*month)])
    month++;

  ಮಾಡು
    {
      size_t ix = (lo + hi) / 2;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *m = month;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *n = monthtab[ix].name;

      ಗಾಗಿ (;; m++, n++)
        {
          ಆದರೆ (!*n)
            {
              ಆದರೆ (ea)
                *ea = (ಅಕ್ಷರ *) m;
              ಹಿಂತಿರುಗಿ monthtab[ix].val;
            }
          ಆದರೆ (ಗಮ್ಯ_uchar (fold_toupper[ಗಮ್ಯ_uchar (*m)]) < ಗಮ್ಯ_uchar (*n))
            {
              hi = ix;
              ನಿಲ್ಲಿಸು;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಮ್ಯ_uchar (fold_toupper[ಗಮ್ಯ_uchar (*m)]) > ಗಮ್ಯ_uchar (*n))
            {
              lo = ix + 1;
              ನಿಲ್ಲಿಸು;
            }
        }
    }
  ಆಗ (lo < hi);

  ಹಿಂತಿರುಗಿ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ಆದರೆ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ಆದರೆ 14 <= __GNUC__
#  pragma GCC diagnostic ತಳ್ಳು
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "md5.h"

#ಆದರೆ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ಆದರೆ 14 <= __GNUC__
#  pragma GCC diagnostic ತೆಗೆ
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_ಆದ್ಯತೆ_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ಸ್ಥಿರ ಶೂನ್ಯ
link_failure (ಶೂನ್ಯ)
{
  error (ವಿಂಗಡಿಸು_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ಸ್ಥಿರ ಶೂನ್ಯ *
ಚಿಹ್ನೆ_ವಿಳಾಸ (ಶೂನ್ಯ *handle, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಚಿಹ್ನೆ)
{
  ಶೂನ್ಯ *ವಿಳಾಸ = dlsym (handle, ಚಿಹ್ನೆ);
  ಆದರೆ (!ವಿಳಾಸ)
    link_failure ();
  ಹಿಂತಿರುಗಿ ವಿಳಾಸ;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Dynamically link the crypto library, if it needs linking.  */
ಸ್ಥಿರ ಶೂನ್ಯ
link_libcrypto (ಶೂನ್ಯ)
{
#ಆದರೆ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  ಶೂನ್ಯ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ಆದರೆ (!handle)
    link_failure ();
  ptr_MD5_Init = ಚಿಹ್ನೆ_ವಿಳಾಸ (handle, "MD5_Init");
  ptr_MD5_Update = ಚಿಹ್ನೆ_ವಿಳಾಸ (handle, "MD5_Update");
  ptr_MD5_Final = ಚಿಹ್ನೆ_ವಿಳಾಸ (handle, "MD5_Final");
#ಆದರೆ_ಅಂತ್ಯ
}

/* A randomly chosen MD5 state, used for random comparison.  */
ಸ್ಥಿರ ರಚನೆ md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ಸ್ಥಿರ ಶೂನ್ಯ
random_md5_state_init (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *random_ಮೂಲ)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ buf[MD5_DIGEST_ಗಾತ್ರ];
  ರಚನೆ randread_ಮೂಲ *r = randread_new (random_ಮೂಲ, ಗಾತ್ರ buf);
  ಆದರೆ (! r)
    ವಿಂಗಡಿಸು_die (_("open failed"), random_ಮೂಲ ? random_ಮೂಲ : "getrandom");
  randread (r, buf, ಗಾತ್ರ buf);
  ಆದರೆ (randread_free (r) != 0)
    ವಿಂಗಡಿಸು_die (_("close failed"), random_ಮೂಲ);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_ಬೈಟುಗಳು (buf, ಗಾತ್ರ buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ಸ್ಥಿರ size_t
xstrxfrm (ಅಕ್ಷರ *ಸೀಮಿತ dest, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ src, size_t destsize)
{
  errno = 0;
  size_t translated_ಗಾತ್ರ = strxfrm (dest, src, destsize);

  ಆದರೆ (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (ವಿಂಗಡಿಸು_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  ಹಿಂತಿರುಗಿ translated_ಗಾತ್ರ;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_random (ಅಕ್ಷರ *ಸೀಮಿತ texta, size_t lena,
                ಅಕ್ಷರ *ಸೀಮಿತ textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  ಪೂರ್ಣಾಂಕ xfrm_diff = 0;

  ಅಕ್ಷರ stackbuf[4000];
  ಅಕ್ಷರ *buf = stackbuf;
  size_t bufsize = ಗಾತ್ರ stackbuf;
  ಶೂನ್ಯ *allocated = ಖಾಲಿ;
  uint32_t dig[2][MD5_DIGEST_ಗಾತ್ರ / ಗಾತ್ರ (uint32_t)];
  ರಚನೆ md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ಆದರೆ (hard_LC_COLLATE)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lima = texta + lena;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *limb = textb + lenb;

      ಆಗ (true)
        {
          /* Transform the text into the basis of comparison, so that byte
             strings that would otherwise considered to be equal are
             considered equal here even if their bytes differ.

             Each time through this loop, transform one
             null-terminated string's worth from TEXTA or from TEXTB
             or both.  That way, there's no need to store the
             transformation of the whole line, if it contains many
             null-terminated strings.  */

          /* Store the transformed data into a big-enough buffer.  */

          /* A 3X size guess avoids the overhead of calling strxfrm
             twice on typical implementations.  Don't worry about
             size_t overflow, as the guess need not be correct.  */
          size_t guess_bufsize = 3 * (lena + lenb) + 2;
          ಆದರೆ (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              ಬಿಡುಗಡೆ (allocated);
              buf = allocated = ಸ್ಮರಣೆ_ಕೊಡು (bufsize);
              ಆದರೆ (! buf)
                {
                  buf = stackbuf;
                  bufsize = ಗಾತ್ರ stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_ಹೊಂದುತ್ತದೆ = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_ಹೊಂದುತ್ತದೆ ? buf + sizea : ಖಾಲಿ), textb,
                          (a_ಹೊಂದುತ್ತದೆ ? bufsize - sizea : 0))
                + 1)
             : 0);

          ಆದರೆ (! (a_ಹೊಂದುತ್ತದೆ && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ಆದರೆ (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              ಬಿಡುಗಡೆ (allocated);
              buf = allocated = xmalloc (bufsize);
              ಆದರೆ (texta < lima)
                strxfrm (buf, texta, sizea);
              ಆದರೆ (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ಆದರೆ (texta < lima)
            texta += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (texta) + 1;
          ಆದರೆ (textb < limb)
            textb += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (textb) + 1;
          ಆದರೆ (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ನಿಲ್ಲಿಸು;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_ಬೈಟುಗಳು (buf, sizea, &s[0]);
          md5_process_ಬೈಟುಗಳು (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ಆದರೆ (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ಆದರೆ (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_ಬೈಟುಗಳು (texta, lena, &s[0]); md5_ಮುಕ್ತಾಯ_ctx (&s[0], dig[0]);
  md5_process_ಬೈಟುಗಳು (textb, lenb, &s[1]); md5_ಮುಕ್ತಾಯ_ctx (&s[1], dig[1]);
  ಪೂರ್ಣಾಂಕ diff = memcmp (dig[0], dig[1], ಗಾತ್ರ dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ಆದರೆ (! diff)
    {
      ಆದರೆ (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ಆದರೆ (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  ಬಿಡುಗಡೆ (allocated);

  ಹಿಂತಿರುಗಿ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ಸ್ಥಿರ size_t
debug_ಅಗಲ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪಠ್ಯ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *lim)
{
  size_t ಅಗಲ = mbsnwidth (ಪಠ್ಯ, lim - ಪಠ್ಯ, 0);
  ಆಗ (ಪಠ್ಯ < lim)
    ಅಗಲ += (*ಪಠ್ಯ++ == '\t');
  ಹಿಂತಿರುಗಿ ಅಗಲ;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mark_ಕೀಲಿ (size_t ಸ್ಥಾನಾಂತರ, size_t ಅಗಲ)
{
  ಆಗ (ಸ್ಥಾನಾಂತರ--)
    ಅಕ್ಷರ_ಬರೆ (' ');

  ಆದರೆ (!ಅಗಲ)
    ಬರೆ (_("^ no match for key\n"));
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಮಾಡು
        ಅಕ್ಷರ_ಬರೆ ('_');
      ಆಗ (--ಅಗಲ);

      ಅಕ್ಷರ_ಬರೆ ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಕೀಲಿ_numeric (ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಹಿಂತಿರುಗಿ ಕೀಲಿ->numeric || ಕೀಲಿ->general_numeric || ಕೀಲಿ->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
debug_ಕೀಲಿ (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಅಕ್ಷರ *ಪಠ್ಯ = ಸಾಲು->ಪಠ್ಯ;
  ಅಕ್ಷರ *beg = ಪಠ್ಯ;
  ಅಕ್ಷರ *lim = ಪಠ್ಯ + ಸಾಲು->ಉದ್ದಳತೆ - 1;

  ಆದರೆ (ಕೀಲಿ)
    {
      ಆದರೆ (ಕೀಲಿ->sword != SIZE_MAX)
        beg = begfield (ಸಾಲು, ಕೀಲಿ);
      ಆದರೆ (ಕೀಲಿ->eword != SIZE_MAX)
        {
          lim = limfield (ಸಾಲು, ಕೀಲಿ);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ಆದರೆ ((ಕೀಲಿ->skipsblanks && ಕೀಲಿ->sword == SIZE_MAX)
          || ಕೀಲಿ->month || ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          ಅಕ್ಷರ saved = *lim;
          *lim = '\0';

          ಆಗ (blanks[ಗಮ್ಯ_uchar (*beg)])
            beg++;

          ಅಕ್ಷರ *tighter_lim = beg;

          ಆದರೆ (lim < beg)
            tighter_lim = lim;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->month)
            getmonth (beg, &tighter_lim);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->general_numeric)
            ignore_ಮೌಲ್ಯ (strtold (beg, &tighter_lim));
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->numeric || ಕೀಲಿ->human_numeric)
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = beg + (beg < lim && *beg == '-');
              ಅಕ್ಷರ max_digit = traverse_raw_ಸಂಖ್ಯೆ (&p);
              ಆದರೆ ('0' <= max_digit)
                {
                  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch = *p;
                  tighter_lim = (ಅಕ್ಷರ *) p
                    + (ಕೀಲಿ->human_numeric && unit_ಕ್ರಮ[ch]);
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t ಸ್ಥಾನಾಂತರ = debug_ಅಗಲ (ಪಠ್ಯ, beg);
  size_t ಅಗಲ = debug_ಅಗಲ (beg, lim);
  mark_ಕೀಲಿ (ಸ್ಥಾನಾಂತರ, ಅಗಲ);
}

/* Debug LINE by underlining its keys.  */

ಸ್ಥಿರ ಶೂನ್ಯ
debug_ಸಾಲು (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು)
{
  ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ = keylist;

  ಮಾಡು
    debug_ಕೀಲಿ (ಸಾಲು, ಕೀಲಿ);
  ಆಗ (ಕೀಲಿ && ((ಕೀಲಿ = ಕೀಲಿ->ಮುಂದಿನ) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ಸ್ಥಿರ bool
default_ಕೀಲಿ_compare (ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ)
{
  ಹಿಂತಿರುಗಿ ! (ಕೀಲಿ->ignore
            || ಕೀಲಿ->translate
            || ಕೀಲಿ->skipsblanks
            || ಕೀಲಿ->skipeblanks
            || ಕೀಲಿ_numeric (ಕೀಲಿ)
            || ಕೀಲಿ->month
            || ಕೀಲಿ->ಆವೃತ್ತಿ_3
            || ಕೀಲಿ->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೀಲಿ_ಗಮ್ಯ_opts (ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ, ಅಕ್ಷರ *opts)
{
  ಆದರೆ (ಕೀಲಿ->skipsblanks || ಕೀಲಿ->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ಆದರೆ (ಕೀಲಿ->ignore == nondictionary)
    *opts++ = 'd';
  ಆದರೆ (ಕೀಲಿ->translate)
    *opts++ = 'f';
  ಆದರೆ (ಕೀಲಿ->general_numeric)
    *opts++ = 'g';
  ಆದರೆ (ಕೀಲಿ->human_numeric)
    *opts++ = 'h';
  ಆದರೆ (ಕೀಲಿ->ignore == nonprinting)
    *opts++ = 'i';
  ಆದರೆ (ಕೀಲಿ->month)
    *opts++ = 'M';
  ಆದರೆ (ಕೀಲಿ->numeric)
    *opts++ = 'n';
  ಆದರೆ (ಕೀಲಿ->random)
    *opts++ = 'R';
  ಆದರೆ (ಕೀಲಿ->ಹಿಮ್ಮುಖ)
    *opts++ = 'r';
  ಆದರೆ (ಕೀಲಿ->ಆವೃತ್ತಿ_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೀಲಿ_warnings (ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *gkey, bool gkey_only)
{
  ರಚನೆ keyfield ugkey = *gkey;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ keynum = 1;
  bool basic_numeric_ಕ್ಷೇತ್ರ = false;
  bool general_numeric_ಕ್ಷೇತ್ರ = false;
  bool basic_numeric_ಕ್ಷೇತ್ರ_ಹರವು = false;
  bool general_numeric_ಕ್ಷೇತ್ರ_ಹರವು = false;

  ಗಾಗಿ (ರಚನೆ keyfield *ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->ಮುಂದಿನ, keynum++)
    {
      ಆದರೆ (ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          ಆದರೆ (ಕೀಲಿ->general_numeric)
            general_numeric_ಕ್ಷೇತ್ರ = true;
          ಇಲ್ಲದಿದ್ದರೆ
            basic_numeric_ಕ್ಷೇತ್ರ = true;
        }

      ಆದರೆ (ಕೀಲಿ->traditional_ಬಳಸಿದ)
        {
          size_t sword = ಕೀಲಿ->sword;
          size_t eword = ಕೀಲಿ->eword;
          ಅಕ್ಷರ tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          ಅಕ್ಷರ obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          ಅಕ್ಷರ nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          ಅಕ್ಷರ *po = obuf;
          ಅಕ್ಷರ *pn = nbuf;

          ಆದರೆ (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ಆದರೆ (ಕೀಲಿ->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ಕೀಲಿ->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_ಅಗಲ = ಕೀಲಿ->sword != SIZE_MAX && ಕೀಲಿ->eword < ಕೀಲಿ->sword;
      ಆದರೆ (zero_ಅಗಲ)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ಕೀಲಿ_numeric (ಕೀಲಿ) || ಕೀಲಿ->month;
      bool ಸಾಲು_ಸ್ಥಾನಾಂತರ = ಕೀಲಿ->eword == 0 && ಕೀಲಿ->echar != 0; /* -k1.x,1.y  */
      ಆದರೆ (!zero_ಅಗಲ && !gkey_only && tab == TAB_DEFAULT && !ಸಾಲು_ಸ್ಥಾನಾಂತರ
          && ((!ಕೀಲಿ->skipsblanks && !implicit_skip)
              || (!ಕೀಲಿ->skipsblanks && ಕೀಲಿ->schar)
              || (!ಕೀಲಿ->skipeblanks && ಕೀಲಿ->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ಆದರೆ (!gkey_only && ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          size_t sword = ಕೀಲಿ->sword + 1;
          size_t eword = ಕೀಲಿ->eword + 1;
          ಆದರೆ (!sword)
            sword++;
          ಆದರೆ (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ಆದರೆ (ಕೀಲಿ->general_numeric)
                general_numeric_ಕ್ಷೇತ್ರ_ಹರವು = true;
              ಇಲ್ಲದಿದ್ದರೆ
                basic_numeric_ಕ್ಷೇತ್ರ_ಹರವು = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ಆದರೆ (ugkey.ignore && (ugkey.ignore == ಕೀಲಿ->ignore))
        ugkey.ignore = ಖಾಲಿ;
      ಆದರೆ (ugkey.translate && (ugkey.translate == ಕೀಲಿ->translate))
        ugkey.translate = ಖಾಲಿ;
      ugkey.skipsblanks &= !ಕೀಲಿ->skipsblanks;
      ugkey.skipeblanks &= !ಕೀಲಿ->skipeblanks;
      ugkey.month &= !ಕೀಲಿ->month;
      ugkey.numeric &= !ಕೀಲಿ->numeric;
      ugkey.general_numeric &= !ಕೀಲಿ->general_numeric;
      ugkey.human_numeric &= !ಕೀಲಿ->human_numeric;
      ugkey.random &= !ಕೀಲಿ->random;
      ugkey.ಆವೃತ್ತಿ_3 &= !ಕೀಲಿ->ಆವೃತ್ತಿ_3;
      ugkey.ಹಿಮ್ಮುಖ &= !ಕೀಲಿ->ಹಿಮ್ಮುಖ;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool ಸಂಖ್ಯೆ_locale_warned = false;
  ಆದರೆ (basic_numeric_ಕ್ಷೇತ್ರ_ಹರವು)
    {
      ಆದರೆ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (ಗಮ್ಯ_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((ಅಕ್ಷರ []) {thousands_sep, 0})));
          ಸಂಖ್ಯೆ_locale_warned = true;
        }
    }
  ಆದರೆ (basic_numeric_ಕ್ಷೇತ್ರ_ಹರವು || general_numeric_ಕ್ಷೇತ್ರ_ಹರವು)
    {
      ಆದರೆ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (ಗಮ್ಯ_uchar (decimal_ಬಿಂದು)))
          : tab == decimal_ಬಿಂದು)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((ಅಕ್ಷರ []) {decimal_ಬಿಂದು, 0})));
          ಸಂಖ್ಯೆ_locale_warned = true;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((ಅಕ್ಷರ []) {tab, 0})));
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (general_numeric_ಕ್ಷೇತ್ರ_ಹರವು && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((ಅಕ್ಷರ []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ಆದರೆ ((basic_numeric_ಕ್ಷೇತ್ರ || general_numeric_ಕ್ಷೇತ್ರ) && ! ಸಂಖ್ಯೆ_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((ಅಕ್ಷರ []) {decimal_ಬಿಂದು, 0})));

    }

  ಆದರೆ (basic_numeric_ಕ್ಷೇತ್ರ && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ಆದರೆ (!default_ಕೀಲಿ_compare (&ugkey)
      || (ugkey.ಹಿಮ್ಮುಖ && (stable || unique) && keylist))
    {
      bool ugkey_ಹಿಮ್ಮುಖ = ugkey.ಹಿಮ್ಮುಖ;
      ಆದರೆ (!(stable || unique))
        ugkey.ಹಿಮ್ಮುಖ = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      ಅಕ್ಷರ opts[ಗಾತ್ರ short_options];
      ಕೀಲಿ_ಗಮ್ಯ_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (opts))), opts);
      ugkey.ಹಿಮ್ಮುಖ = ugkey_ಹಿಮ್ಮುಖ;
    }
  ಆದರೆ (ugkey.ಹಿಮ್ಮುಖ && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
diff_reversed (ಪೂರ್ಣಾಂಕ diff, bool reversed)
{
  ಹಿಂತಿರುಗಿ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
keycompare (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *a, ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *b)
{
  ರಚನೆ keyfield *ಕೀಲಿ = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  ಅಕ್ಷರ *texta = a->keybeg;
  ಅಕ್ಷರ *textb = b->keybeg;
  ಅಕ್ಷರ *lima = a->keylim;
  ಅಕ್ಷರ *limb = b->keylim;

  ಪೂರ್ಣಾಂಕ diff;

  ಆಗ (true)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *translate = ಕೀಲಿ->translate;
      bool ಸ್ಥಿರಾಂಕ *ignore = ಕೀಲಿ->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ಆದರೆ (hard_LC_COLLATE || ಕೀಲಿ_numeric (ಕೀಲಿ)
          || ಕೀಲಿ->month || ಕೀಲಿ->random || ಕೀಲಿ->ಆವೃತ್ತಿ_3)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          ಅಕ್ಷರ *ta = texta;
          ಅಕ್ಷರ *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          ಅಕ್ಷರ enda = ta[tlena];
          ಅಕ್ಷರ endb = tb[tlenb];

          ಶೂನ್ಯ *allocated = ಖಾಲಿ;
          ಅಕ್ಷರ stackbuf[4000];

          ಆದರೆ (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t ಗಾತ್ರ_2 = lena + 1 + lenb + 1;
              ಆದರೆ (ಗಾತ್ರ_2 <= ಗಾತ್ರ stackbuf)
                ta = stackbuf;
              ಇಲ್ಲದಿದ್ದರೆ
                ta = allocated = xmalloc (ಗಾತ್ರ_2);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              ಗಾಗಿ (tlena = i = 0; i < lena; i++)
                ಆದರೆ (! (ignore && ignore[ಗಮ್ಯ_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[ಗಮ್ಯ_uchar (texta[i])]
                                 : texta[i]);

              ಗಾಗಿ (tlenb = i = 0; i < lenb; i++)
                ಆದರೆ (! (ignore && ignore[ಗಮ್ಯ_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[ಗಮ್ಯ_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ಆದರೆ (ಕೀಲಿ->numeric)
            diff = numcompare (ta, tb);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->general_numeric)
            diff = general_numcompare (ta, tb);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->human_numeric)
            diff = human_numcompare (ta, tb);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->month)
            diff = getmonth (ta, ಖಾಲಿ) - getmonth (tb, ಖಾಲಿ);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->ಆವೃತ್ತಿ_3)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ಆದರೆ (tlena == 0)
                diff = - NONZERO (tlenb);
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (tlenb == 0)
                diff = 1;
              ಇಲ್ಲದಿದ್ದರೆ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          ಬಿಡುಗಡೆ (allocated);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ignore)
        {
#ವ್ಯಾಖ್ಯಾನಿಸು CMP_WITH_IGNORE(A, B)						\
  ಮಾಡು									\
    {									\
          ಆಗ (true)							\
            {								\
              ಆಗ (texta < lima && ignore[ಗಮ್ಯ_uchar (*texta)])		\
                ++texta;						\
              ಆಗ (textb < limb && ignore[ಗಮ್ಯ_uchar (*textb)])		\
                ++textb;						\
              ಆದರೆ (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ನಿಲ್ಲಿಸು;						\
                }							\
              diff = ಗಮ್ಯ_uchar (A) - ಗಮ್ಯ_uchar (B);			\
              ಆದರೆ (diff)							\
                ನಿಲ್ಲಿಸು;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ಆಗ (0)

          ಆದರೆ (translate)
            CMP_WITH_IGNORE (translate[ಗಮ್ಯ_uchar (*texta)],
                             translate[ಗಮ್ಯ_uchar (*textb)]);
          ಇಲ್ಲದಿದ್ದರೆ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          size_t lenmin = MIN (lena, lenb);
          ಆದರೆ (lenmin == 0)
            diff = 0;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (translate)
            {
              size_t i = 0;
              ಮಾಡು
                {
                  diff = (ಗಮ್ಯ_uchar (translate[ಗಮ್ಯ_uchar (texta[i])])
                          - ಗಮ್ಯ_uchar (translate[ಗಮ್ಯ_uchar (textb[i])]));
                  ಆದರೆ (diff)
                    ನಿಲ್ಲಿಸು;
                  i++;
                }
              ಆಗ (i < lenmin);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            diff = memcmp (texta, textb, lenmin);

          ಆದರೆ (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ಆದರೆ (diff)
        ನಿಲ್ಲಿಸು;

      ಕೀಲಿ = ಕೀಲಿ->ಮುಂದಿನ;
      ಆದರೆ (! ಕೀಲಿ)
        ಹಿಂತಿರುಗಿ 0;

      /* Find the beginning and limit of the next field.  */
      ಆದರೆ (ಕೀಲಿ->eword != SIZE_MAX)
        lima = limfield (a, ಕೀಲಿ), limb = limfield (b, ಕೀಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        lima = a->ಪಠ್ಯ + a->ಉದ್ದಳತೆ - 1, limb = b->ಪಠ್ಯ + b->ಉದ್ದಳತೆ - 1;

      ಆದರೆ (ಕೀಲಿ->sword != SIZE_MAX)
        texta = begfield (a, ಕೀಲಿ), textb = begfield (b, ಕೀಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          texta = a->ಪಠ್ಯ, textb = b->ಪಠ್ಯ;
          ಆದರೆ (ಕೀಲಿ->skipsblanks)
            {
              ಆಗ (texta < lima && blanks[ಗಮ್ಯ_uchar (*texta)])
                ++texta;
              ಆಗ (textb < limb && blanks[ಗಮ್ಯ_uchar (*textb)])
                ++textb;
            }
        }
    }

  ಹಿಂತಿರುಗಿ diff_reversed (diff, ಕೀಲಿ->ಹಿಮ್ಮುಖ);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *a, ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *b)
{
  ಪೂರ್ಣಾಂಕ diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ಆದರೆ (keylist)
    {
      diff = keycompare (a, b);
      ಆದರೆ (diff || unique || stable)
        ಹಿಂತಿರುಗಿ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->ಉದ್ದಳತೆ - 1, blen = b->ಉದ್ದಳತೆ - 1;

  ಆದರೆ (alen == 0)
    diff = - NONZERO (blen);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (blen == 0)
    diff = 1;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->ಪಠ್ಯ, alen + 1, b->ಪಠ್ಯ, blen + 1);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      diff = memcmp (a->ಪಠ್ಯ, b->ಪಠ್ಯ, MIN (alen, blen));
      ಆದರೆ (!diff)
        diff = _GL_CMP (alen, blen);
    }

  ಹಿಂತಿರುಗಿ diff_reversed (diff, ಹಿಮ್ಮುಖ);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಸಾಲು (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, FILE *fp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file)
{
  ಅಕ್ಷರ *buf = ಸಾಲು->ಪಠ್ಯ;
  size_t n_ಬೈಟುಗಳು = ಸಾಲು->ಉದ್ದಳತೆ;
  ಅಕ್ಷರ *ebuf = buf + n_ಬೈಟುಗಳು;

  ಆದರೆ (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *c = buf;

      ಆಗ (c < ebuf)
        {
          ಅಕ್ಷರ wc = *c++;
          ಆದರೆ (wc == '\t')
            wc = '>';
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c == ebuf)
            wc = '\n';
          ಆದರೆ (fputc (wc, fp) == EOF)
            ವಿಂಗಡಿಸು_die (_("write failed"), output_file);
        }

      debug_ಸಾಲು (ಸಾಲು);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ebuf[-1] = eolchar;
      ಆದರೆ (fwrite (buf, 1, n_ಬೈಟುಗಳು, fp) != n_ಬೈಟುಗಳು)
        ವಿಂಗಡಿಸು_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ಸ್ಥಿರ bool
check (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_name, ಅಕ್ಷರ checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ರಚನೆ ಬಫರ್ buf;		/* Input buffer. */
  ರಚನೆ ಸಾಲು ತಾತ್ಕಾಲಿಕ;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t ಸಾಲು_ಸಂಖ್ಯೆ = 0;
  ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, ಗಾತ್ರ (ರಚನೆ ಸಾಲು),
           MAX (ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ, ವಿಂಗಡಿಸು_ಗಾತ್ರ));
  ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ = ಖಾಲಿ;

  ಆಗ (fillbuf (&buf, fp, file_name, ಖಾಲಿ))
    {
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು = ಬಫರ್_linelim (&buf);
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *linebase = ಸಾಲು - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ಆದರೆ (alloc && nonunique <= compare (&ತಾತ್ಕಾಲಿಕ, ಸಾಲು - 1))
        {
        ಸಿಕ್ಕಿತು_disorder:
          {
            ಆದರೆ (checkonly == 'c')
              {
                ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *disorder_ಸಾಲು = ಸಾಲು - 1;
                uintmax_t disorder_ಸಾಲು_ಸಂಖ್ಯೆ =
                  ಬಫರ್_linelim (&buf) - disorder_ಸಾಲು + ಸಾಲು_ಸಂಖ್ಯೆ;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         ಕಾರ್ಯಕ್ರಮ_name, file_name, disorder_ಸಾಲು_ಸಂಖ್ಯೆ);
                write_ಸಾಲು (disorder_ಸಾಲು, stderr, _("standard error"));
              }

            ordered = false;
            ನಿಲ್ಲಿಸು;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ಆಗ (linebase < --ಸಾಲು)
        ಆದರೆ (nonunique <= compare (ಸಾಲು, ಸಾಲು - 1))
          ಹೋಗು ಸಿಕ್ಕಿತು_disorder;

      ಸಾಲು_ಸಂಖ್ಯೆ += buf.nlines;

      /* Save the last line of the buffer.  */
      ಆದರೆ (alloc < ಸಾಲು->ಉದ್ದಳತೆ)
        {
          ಮಾಡು
            {
              alloc *= 2;
              ಆದರೆ (! alloc)
                {
                  alloc = ಸಾಲು->ಉದ್ದಳತೆ;
                  ನಿಲ್ಲಿಸು;
                }
            }
          ಆಗ (alloc < ಸಾಲು->ಉದ್ದಳತೆ);

          ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ);
          ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ = xmalloc (alloc);
        }
      ಸ್ಮರಣೆ_ನಕಲು (ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ, ಸಾಲು->ಪಠ್ಯ, ಸಾಲು->ಉದ್ದಳತೆ);
      ತಾತ್ಕಾಲಿಕ.ಉದ್ದಳತೆ = ಸಾಲು->ಉದ್ದಳತೆ;
      ಆದರೆ (ಕೀಲಿ)
        {
          ತಾತ್ಕಾಲಿಕ.keybeg = ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ + (ಸಾಲು->keybeg - ಸಾಲು->ಪಠ್ಯ);
          ತಾತ್ಕಾಲಿಕ.keylim = ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ + (ಸಾಲು->keylim - ಸಾಲು->ಪಠ್ಯ);
        }
    }

  xfclose (fp, file_name);
  ಬಿಡುಗಡೆ (buf.buf);
  ಬಿಡುಗಡೆ (ತಾತ್ಕಾಲಿಕ.ಪಠ್ಯ);
  ಹಿಂತಿರುಗಿ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ಸ್ಥಿರ size_t
open_ಒಳಹರಿವು_files (ರಚನೆ sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, ಗಾತ್ರ *fps);
  ಪೂರ್ಣಾಂಕ i;

  /* Open as many input files as we can.  */
  ಗಾಗಿ (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].ತಾತ್ಕಾಲಿಕ && files[i].ತಾತ್ಕಾಲಿಕ->state != UNCOMPRESSED
                ? open_ತಾತ್ಕಾಲಿಕ (files[i].ತಾತ್ಕಾಲಿಕ)
                : ಹರಿವು_open (files[i].name, "r"));
      ಆದರೆ (!fps[i])
        ನಿಲ್ಲಿಸು;
    }

  ಹಿಂತಿರುಗಿ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergefps (ರಚನೆ sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file, FILE **fps)
{
  ರಚನೆ ಬಫರ್ *ಬಫರ್ = xnmalloc (nfiles, ಗಾತ್ರ *ಬಫರ್);
                                /* Input buffers for each file. */
  ರಚನೆ ಸಾಲು saved;		/* Saved line storage for unique check. */
  ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *savedline = ಖಾಲಿ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ **cur = xnmalloc (nfiles, ಗಾತ್ರ *cur);
                                /* Current line in each line table. */
  ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ **ತಳ_2 = xnmalloc (nfiles, ಗಾತ್ರ *ತಳ_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, ಗಾತ್ರ *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ರಚನೆ keyfield ಸ್ಥಿರಾಂಕ *ಕೀಲಿ = keylist;
  saved.ಪಠ್ಯ = ಖಾಲಿ;

  /* Read initial lines from each input file. */
  ಗಾಗಿ (size_t i = 0; i < nfiles; )
    {
      initbuf (&ಬಫರ್[i], ಗಾತ್ರ (ರಚನೆ ಸಾಲು),
               MAX (ವಿಲೀನ_ಬಫರ್_ಗಾತ್ರ, ವಿಂಗಡಿಸು_ಗಾತ್ರ / nfiles));
      ಆದರೆ (fillbuf (&ಬಫರ್[i], fps[i], files[i].name, ಖಾಲಿ))
        {
          ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *linelim = ಬಫರ್_linelim (&ಬಫರ್[i]);
          cur[i] = linelim - 1;
          ತಳ_2[i] = linelim - ಬಫರ್[i].nlines;
          i++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ಆದರೆ (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          ಬಿಡುಗಡೆ (ಬಫರ್[i].buf);
          --nfiles;
          ಗಾಗಿ (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  ಗಾಗಿ (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  ಗಾಗಿ (size_t i = 1; i < nfiles; ++i)
    ಆದರೆ (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ಆಗ (nfiles)
    {
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಅತಿಚಿಕ್ಕ = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ಆದರೆ (unique)
        {
          ಆದರೆ (savedline && compare (savedline, ಅತಿಚಿಕ್ಕ))
            {
              savedline = ಖಾಲಿ;
              write_ಸಾಲು (&saved, ofp, output_file);
            }
          ಆದರೆ (!savedline)
            {
              savedline = &saved;
              ಆದರೆ (savealloc < ಅತಿಚಿಕ್ಕ->ಉದ್ದಳತೆ)
                {
                  ಮಾಡು
                    ಆದರೆ (! savealloc)
                      {
                        savealloc = ಅತಿಚಿಕ್ಕ->ಉದ್ದಳತೆ;
                        ನಿಲ್ಲಿಸು;
                      }
                  ಆಗ ((savealloc *= 2) < ಅತಿಚಿಕ್ಕ->ಉದ್ದಳತೆ);

                  ಬಿಡುಗಡೆ (saved.ಪಠ್ಯ);
                  saved.ಪಠ್ಯ = xmalloc (savealloc);
                }
              saved.ಉದ್ದಳತೆ = ಅತಿಚಿಕ್ಕ->ಉದ್ದಳತೆ;
              ಸ್ಮರಣೆ_ನಕಲು (saved.ಪಠ್ಯ, ಅತಿಚಿಕ್ಕ->ಪಠ್ಯ, saved.ಉದ್ದಳತೆ);
              ಆದರೆ (ಕೀಲಿ)
                {
                  saved.keybeg =
                    saved.ಪಠ್ಯ + (ಅತಿಚಿಕ್ಕ->keybeg - ಅತಿಚಿಕ್ಕ->ಪಠ್ಯ);
                  saved.keylim =
                    saved.ಪಠ್ಯ + (ಅತಿಚಿಕ್ಕ->keylim - ಅತಿಚಿಕ್ಕ->ಪಠ್ಯ);
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        write_ಸಾಲು (ಅತಿಚಿಕ್ಕ, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ಆದರೆ (ತಳ_2[ord[0]] < ಅತಿಚಿಕ್ಕ)
        cur[ord[0]] = ಅತಿಚಿಕ್ಕ - 1;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (fillbuf (&ಬಫರ್[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ಖಾಲಿ))
            {
              ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *linelim = ಬಫರ್_linelim (&ಬಫರ್[ord[0]]);
              cur[ord[0]] = linelim - 1;
              ತಳ_2[ord[0]] = linelim - ಬಫರ್[ord[0]].nlines;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* We reached EOF on fps[ord[0]].  */
              ಗಾಗಿ (size_t i = 1; i < nfiles; ++i)
                ಆದರೆ (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ಆದರೆ (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              ಬಿಡುಗಡೆ (ಬಫರ್[ord[0]].buf);
              ಗಾಗಿ (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  ಬಫರ್[i] = ಬಫರ್[i + 1];
                  cur[i] = cur[i + 1];
                  ತಳ_2[i] = ತಳ_2[i + 1];
                }
              ಗಾಗಿ (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ಮುಂದುವರಿ;
            }
        }

      /* The new line just read in may be larger than other lines
         already in main memory; push it back in the queue until we
         encounter a line larger than it.  Optimize for the common
         case where the new line is smallest.  */
      {
        size_t lo = 1;
        size_t hi = nfiles;
        size_t probe = lo;
        size_t ord0 = ord[0];
        size_t ಎಣಿಕೆ_ದ_smaller_ಸಾಲುಗಳು;

        ಆಗ (lo < hi)
          {
            ಪೂರ್ಣಾಂಕ cmp = compare (cur[ord0], cur[ord[probe]]);
            ಆದರೆ (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ಇಲ್ಲದಿದ್ದರೆ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ಎಣಿಕೆ_ದ_smaller_ಸಾಲುಗಳು = lo - 1;
        ಗಾಗಿ (size_t j = 0; j < ಎಣಿಕೆ_ದ_smaller_ಸಾಲುಗಳು; j++)
          ord[j] = ord[j + 1];
        ord[ಎಣಿಕೆ_ದ_smaller_ಸಾಲುಗಳು] = ord0;
      }
    }

  ಆದರೆ (unique && savedline)
    {
      write_ಸಾಲು (&saved, ofp, output_file);
      ಬಿಡುಗಡೆ (saved.ಪಠ್ಯ);
    }

  xfclose (ofp, output_file);
  ಬಿಡುಗಡೆ (fps);
  ಬಿಡುಗಡೆ (ಬಫರ್);
  ಬಿಡುಗಡೆ (ord);
  ಬಿಡುಗಡೆ (ತಳ_2);
  ಬಿಡುಗಡೆ (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ಸ್ಥಿರ size_t
mergefiles (ರಚನೆ sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file)
{
  FILE **fps;
  size_t nopened = open_ಒಳಹರಿವು_files (files, nfiles, &fps);
  ಆದರೆ (nopened < nfiles && nopened < 2)
    ವಿಂಗಡಿಸು_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  ಹಿಂತಿರುಗಿ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergelines (ರಚನೆ ಸಾಲು *ಸೀಮಿತ t, size_t nlines,
            ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸೀಮಿತ lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ರಚನೆ ಸಾಲು *hi = t - nlo;

  ಆಗ (true)
    ಆದರೆ (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ಆದರೆ (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            ಹಿಂತಿರುಗಿ;
          }
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        *--t = *--hi;
        ಆದರೆ (! --nhi)
          {
            ಮಾಡು
              *--t = *--lo;
            ಆಗ (--nlo);

            ಹಿಂತಿರುಗಿ;
          }
      }
}

/* Sort the array LINES with NLINES members, using TEMP for temporary space.
   Do this all within one thread.  NLINES must be at least 2.
   If TO_TEMP, put the sorted output into TEMP, and TEMP is as large as LINES.
   Otherwise the sort is in-place and TEMP is half-sized.
   The input and output arrays are in reverse order, and LINES and
   TEMP point just past the end of their respective arrays.

   Use a recursive divide-and-conquer algorithm, in the style
   suggested by Knuth volume 3 (2nd edition), exercise 5.2.4-23.  Use
   the optimization suggested by exercise 5.2.4-10; this requires room
   for only 1.5*N lines, rather than the usual 2*N lines.  Knuth
   writes that this memory optimization was originally published by
   D. A. Bell, Comp J. 1 (1958), 75.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sequential_ವಿಂಗಡಿಸು (ರಚನೆ ಸಾಲು *ಸೀಮಿತ ಸಾಲುಗಳು, size_t nlines,
                 ರಚನೆ ಸಾಲು *ಸೀಮಿತ ತಾತ್ಕಾಲಿಕ, bool ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ)
{
  ಆದರೆ (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      ಪೂರ್ಣಾಂಕ ಅದಲುಬದಲು = (0 < compare (&ಸಾಲುಗಳು[-1], &ಸಾಲುಗಳು[-2]));
      ಆದರೆ (ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ)
        {
          ತಾತ್ಕಾಲಿಕ[-1] = ಸಾಲುಗಳು[-1 - ಅದಲುಬದಲು];
          ತಾತ್ಕಾಲಿಕ[-2] = ಸಾಲುಗಳು[-2 + ಅದಲುಬದಲು];
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅದಲುಬದಲು)
        {
          ತಾತ್ಕಾಲಿಕ[-1] = ಸಾಲುಗಳು[-1];
          ಸಾಲುಗಳು[-1] = ಸಾಲುಗಳು[-2];
          ಸಾಲುಗಳು[-2] = ತಾತ್ಕಾಲಿಕ[-1];
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ರಚನೆ ಸಾಲು *lo = ಸಾಲುಗಳು;
      ರಚನೆ ಸಾಲು *hi = ಸಾಲುಗಳು - nlo;

      sequential_ವಿಂಗಡಿಸು (hi, nhi, ತಾತ್ಕಾಲಿಕ - (ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ ? nlo : 0), ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ);
      ಆದರೆ (1 < nlo)
        sequential_ವಿಂಗಡಿಸು (lo, nlo, ತಾತ್ಕಾಲಿಕ, !ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ)
        ತಾತ್ಕಾಲಿಕ[-1] = lo[-1];

      ರಚನೆ ಸಾಲು *dest;
      ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *sorted_lo;
      ಆದರೆ (ಗಮ್ಯ_ತಾತ್ಕಾಲಿಕ)
        {
          dest = ತಾತ್ಕಾಲಿಕ;
          sorted_lo = ಸಾಲುಗಳು;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          dest = ಸಾಲುಗಳು;
          sorted_lo = ತಾತ್ಕಾಲಿಕ;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_ಗಂಟು *init_ಗಂಟು (ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸೀಮಿತ,
                                     ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸೀಮಿತ,
                                     ರಚನೆ ಸಾಲು *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_ಗಂಟು *
ವಿಲೀನ_ವೃಕ್ಷ_init (size_t nthreads, size_t nlines, ರಚನೆ ಸಾಲು *dest)
{
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ವಿಲೀನ_ವೃಕ್ಷ = xmalloc (2 * ಗಾತ್ರ *ವಿಲೀನ_ವೃಕ್ಷ * nthreads);

  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಬೇರು = ವಿಲೀನ_ವೃಕ್ಷ;
  ಬೇರು->lo = ಬೇರು->hi = ಬೇರು->ಕೊನೆ_lo = ಬೇರು->ಕೊನೆ_hi = ಖಾಲಿ;
  ಬೇರು->dest = ಖಾಲಿ;
  ಬೇರು->nlo = ಬೇರು->nhi = nlines;
  ಬೇರು->ಪೋಷಕ = ಖಾಲಿ;
  ಬೇರು->level = ವಿಲೀನ_ಕೊನೆ;
  ಬೇರು->queued = false;
  pthread_mutex_init (&ಬೇರು->ಬೀಗ, ಖಾಲಿ);

  init_ಗಂಟು (ಬೇರು, ಬೇರು + 1, dest, nthreads, nlines, false);
  ಹಿಂತಿರುಗಿ ವಿಲೀನ_ವೃಕ್ಷ;
}

/* Destroy the merge tree. */
ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_ವೃಕ್ಷ_destroy (size_t nthreads, ರಚನೆ ವಿಲೀನ_ಗಂಟು *ವಿಲೀನ_ವೃಕ್ಷ)
{
  size_t n_nodes = nthreads * 2;
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು = ವಿಲೀನ_ವೃಕ್ಷ;

  ಆಗ (n_nodes--)
    {
      pthread_mutex_destroy (&ಗಂಟು->ಬೀಗ);
      ಗಂಟು++;
    }

  ಬಿಡುಗಡೆ (ವಿಲೀನ_ವೃಕ್ಷ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_ಗಂಟು *
init_ಗಂಟು (ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸೀಮಿತ ಪೋಷಕ,
           ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸೀಮಿತ ಗಂಟು_pool,
           ರಚನೆ ಸಾಲು *dest, size_t nthreads,
           size_t ಒಟ್ಟು_ಸಾಲುಗಳು, bool is_lo_ಮಗು)
{
  size_t nlines = (is_lo_ಮಗು ? ಪೋಷಕ->nlo : ಪೋಷಕ->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ರಚನೆ ಸಾಲು *lo = dest - ಒಟ್ಟು_ಸಾಲುಗಳು;
  ರಚನೆ ಸಾಲು *hi = lo - nlo;
  ರಚನೆ ಸಾಲು **ಪೋಷಕ_ಕೊನೆ = (is_lo_ಮಗು ? &ಪೋಷಕ->ಕೊನೆ_lo : &ಪೋಷಕ->ಕೊನೆ_hi);

  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು = ಗಂಟು_pool++;
  ಗಂಟು->lo = ಗಂಟು->ಕೊನೆ_lo = lo;
  ಗಂಟು->hi = ಗಂಟು->ಕೊನೆ_hi = hi;
  ಗಂಟು->dest = ಪೋಷಕ_ಕೊನೆ;
  ಗಂಟು->nlo = nlo;
  ಗಂಟು->nhi = nhi;
  ಗಂಟು->ಪೋಷಕ = ಪೋಷಕ;
  ಗಂಟು->level = ಪೋಷಕ->level + 1;
  ಗಂಟು->queued = false;
  pthread_mutex_init (&ಗಂಟು->ಬೀಗ, ಖಾಲಿ);

  ಆದರೆ (nthreads > 1)
    {
      size_t lo_ಎಳೆಗಳು = nthreads / 2;
      size_t hi_ಎಳೆಗಳು = nthreads - lo_ಎಳೆಗಳು;
      ಗಂಟು->lo_ಮಗು = ಗಂಟು_pool;
      ಗಂಟು_pool = init_ಗಂಟು (ಗಂಟು, ಗಂಟು_pool, lo, lo_ಎಳೆಗಳು,
                             ಒಟ್ಟು_ಸಾಲುಗಳು, true);
      ಗಂಟು->hi_ಮಗು = ಗಂಟು_pool;
      ಗಂಟು_pool = init_ಗಂಟು (ಗಂಟು, ಗಂಟು_pool, hi, hi_ಎಳೆಗಳು,
                             ಒಟ್ಟು_ಸಾಲುಗಳು, false);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಂಟು->lo_ಮಗು = ಖಾಲಿ;
      ಗಂಟು->hi_ಮಗು = ಖಾಲಿ;
    }
  ಹಿಂತಿರುಗಿ ಗಂಟು_pool;
}


/* Compare two merge nodes A and B for priority.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_nodes (ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *a, ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *b)
{
  ರಚನೆ ವಿಲೀನ_ಗಂಟು ಸ್ಥಿರಾಂಕ *nodea = a;
  ರಚನೆ ವಿಲೀನ_ಗಂಟು ಸ್ಥಿರಾಂಕ *nodeb = b;
  ಆದರೆ (nodea->level == nodeb->level)
      ಹಿಂತಿರುಗಿ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  ಹಿಂತಿರುಗಿ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಬೀಗ_ಗಂಟು (ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_ಬೀಗ (&ಗಂಟು->ಬೀಗ);
}

/* Unlock a merge tree NODE. */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
unlock_ಗಂಟು (ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_unlock (&ಗಂಟು->ಬೀಗ);
}

/* Destroy merge QUEUE. */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_destroy (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ)
{
  ರಾಶಿ_free (ಸರತಿ->ಆದ್ಯತೆ_ಸರತಿ);
  pthread_cond_destroy (&ಸರತಿ->cond);
  pthread_mutex_destroy (&ಸರತಿ->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_init (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  ಸರತಿ->ಆದ್ಯತೆ_ಸರತಿ = ರಾಶಿ_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&ಸರತಿ->mutex, ಖಾಲಿ);
  pthread_cond_init (&ಸರತಿ->cond, ಖಾಲಿ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_ಸೇರಿಸು (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ, ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_ಬೀಗ (&ಸರತಿ->mutex);
  ರಾಶಿ_ಸೇರಿಸು (ಸರತಿ->ಆದ್ಯತೆ_ಸರತಿ, ಗಂಟು);
  ಗಂಟು->queued = true;
  pthread_cond_signal (&ಸರತಿ->cond);
  pthread_mutex_unlock (&ಸರತಿ->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ಸ್ಥಿರ ರಚನೆ ವಿಲೀನ_ಗಂಟು *
ಸರತಿ_ತೆಗೆ (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ)
{
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು;
  pthread_mutex_ಬೀಗ (&ಸರತಿ->mutex);
  ಆಗ (! (ಗಂಟು = ರಾಶಿ_remove_ತುದಿ (ಸರತಿ->ಆದ್ಯತೆ_ಸರತಿ)))
    pthread_cond_wait (&ಸರತಿ->cond, &ಸರತಿ->mutex);
  pthread_mutex_unlock (&ಸರತಿ->mutex);
  ಬೀಗ_ಗಂಟು (ಗಂಟು);
  ಗಂಟು->queued = false;
  ಹಿಂತಿರುಗಿ ಗಂಟು;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_unique (ರಚನೆ ಸಾಲು ಸ್ಥಿರಾಂಕ *ಸಾಲು, FILE *tfp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_output)
{
  ಆದರೆ (unique)
    {
      ಆದರೆ (saved_ಸಾಲು.ಪಠ್ಯ && ! compare (ಸಾಲು, &saved_ಸಾಲು))
        ಹಿಂತಿರುಗಿ;
      saved_ಸಾಲು = *ಸಾಲು;
    }

  write_ಸಾಲು (ಸಾಲು, tfp, ತಾತ್ಕಾಲಿಕ_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergelines_ಗಂಟು (ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸೀಮಿತ ಗಂಟು, size_t ಒಟ್ಟು_ಸಾಲುಗಳು,
                 FILE *tfp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_output)
{
  ರಚನೆ ಸಾಲು *lo_orig = ಗಂಟು->lo;
  ರಚನೆ ಸಾಲು *hi_orig = ಗಂಟು->hi;
  size_t ಗಮ್ಯ_ವಿಲೀನ = MAX_ವಿಲೀನ (ಒಟ್ಟು_ಸಾಲುಗಳು, ಗಂಟು->level);
  size_t merged_lo;
  size_t merged_hi;

  ಆದರೆ (ಗಂಟು->level > ವಿಲೀನ_ಬೇರು)
    {
      /* Merge to destination buffer. */
      ರಚನೆ ಸಾಲು *dest = *ಗಂಟು->dest;
      ಆಗ (ಗಂಟು->lo != ಗಂಟು->ಕೊನೆ_lo && ಗಂಟು->hi != ಗಂಟು->ಕೊನೆ_hi && ಗಮ್ಯ_ವಿಲೀನ--)
        ಆದರೆ (compare (ಗಂಟು->lo - 1, ಗಂಟು->hi - 1) <= 0)
          *--dest = *--ಗಂಟು->lo;
        ಇಲ್ಲದಿದ್ದರೆ
          *--dest = *--ಗಂಟು->hi;

      merged_lo = lo_orig - ಗಂಟು->lo;
      merged_hi = hi_orig - ಗಂಟು->hi;

      ಆದರೆ (ಗಂಟು->nhi == merged_hi)
        ಆಗ (ಗಂಟು->lo != ಗಂಟು->ಕೊನೆ_lo && ಗಮ್ಯ_ವಿಲೀನ--)
          *--dest = *--ಗಂಟು->lo;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->nlo == merged_lo)
        ಆಗ (ಗಂಟು->hi != ಗಂಟು->ಕೊನೆ_hi && ಗಮ್ಯ_ವಿಲೀನ--)
          *--dest = *--ಗಂಟು->hi;
      *ಗಂಟು->dest = dest;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Merge directly to output. */
      ಆಗ (ಗಂಟು->lo != ಗಂಟು->ಕೊನೆ_lo && ಗಂಟು->hi != ಗಂಟು->ಕೊನೆ_hi && ಗಮ್ಯ_ವಿಲೀನ--)
        {
          ಆದರೆ (compare (ಗಂಟು->lo - 1, ಗಂಟು->hi - 1) <= 0)
            write_unique (--ಗಂಟು->lo, tfp, ತಾತ್ಕಾಲಿಕ_output);
          ಇಲ್ಲದಿದ್ದರೆ
            write_unique (--ಗಂಟು->hi, tfp, ತಾತ್ಕಾಲಿಕ_output);
        }

      merged_lo = lo_orig - ಗಂಟು->lo;
      merged_hi = hi_orig - ಗಂಟು->hi;

      ಆದರೆ (ಗಂಟು->nhi == merged_hi)
        {
          ಆಗ (ಗಂಟು->lo != ಗಂಟು->ಕೊನೆ_lo && ಗಮ್ಯ_ವಿಲೀನ--)
            write_unique (--ಗಂಟು->lo, tfp, ತಾತ್ಕಾಲಿಕ_output);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->nlo == merged_lo)
        {
          ಆಗ (ಗಂಟು->hi != ಗಂಟು->ಕೊನೆ_hi && ಗಮ್ಯ_ವಿಲೀನ--)
            write_unique (--ಗಂಟು->hi, tfp, ತಾತ್ಕಾಲಿಕ_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ಗಂಟು->lo;
  merged_hi = hi_orig - ಗಂಟು->hi;
  ಗಂಟು->nlo -= merged_lo;
  ಗಂಟು->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_check_ಸೇರಿಸು (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ, ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು)
{
  ಆದರೆ (! ಗಂಟು->queued)
    {
      bool lo_avail = (ಗಂಟು->lo - ಗಂಟು->ಕೊನೆ_lo) != 0;
      bool hi_avail = (ಗಂಟು->hi - ಗಂಟು->ಕೊನೆ_hi) != 0;
      ಆದರೆ (lo_avail ? hi_avail || ! ಗಂಟು->nhi : hi_avail && ! ಗಂಟು->nlo)
        ಸರತಿ_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿ_check_ಸೇರಿಸು_ಪೋಷಕ (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ,
                           ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು)
{
  ಆದರೆ (ಗಂಟು->level > ವಿಲೀನ_ಬೇರು)
    {
      ಬೀಗ_ಗಂಟು (ಗಂಟು->ಪೋಷಕ);
      ಸರತಿ_check_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು->ಪೋಷಕ);
      unlock_ಗಂಟು (ಗಂಟು->ಪೋಷಕ);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->nlo + ಗಂಟು->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      ಸರತಿ_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು->ಪೋಷಕ);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_loop (ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ,
            size_t ಒಟ್ಟು_ಸಾಲುಗಳು, FILE *tfp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_output)
{
  ಆಗ (true)
    {
      ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು = ಸರತಿ_ತೆಗೆ (ಸರತಿ);

      ಆದರೆ (ಗಂಟು->level == ವಿಲೀನ_ಕೊನೆ)
        {
          unlock_ಗಂಟು (ಗಂಟು);
          /* Reinsert so other threads can pop it. */
          ಸರತಿ_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು);
          ನಿಲ್ಲಿಸು;
        }
      mergelines_ಗಂಟು (ಗಂಟು, ಒಟ್ಟು_ಸಾಲುಗಳು, tfp, ತಾತ್ಕಾಲಿಕ_output);
      ಸರತಿ_check_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು);
      ಸರತಿ_check_ಸೇರಿಸು_ಪೋಷಕ (ಸರತಿ, ಗಂಟು);

      unlock_ಗಂಟು (ಗಂಟು);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ sortlines (ರಚನೆ ಸಾಲು *ಸೀಮಿತ, size_t, size_t,
                       ರಚನೆ ವಿಲೀನ_ಗಂಟು *, ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *,
                       FILE *, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *);

/* Thread arguments for sortlines_thread. */

ರಚನೆ thread_ವಾದಗಳು
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ರಚನೆ ಸಾಲು *ಸಾಲುಗಳು;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ಸ್ಥಿರಾಂಕ ಒಟ್ಟು_ಸಾಲುಗಳು;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಸ್ಥಿರಾಂಕ ಗಂಟು;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸ್ಥಿರಾಂಕ ಸರತಿ;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_ತಾತ್ಕಾಲಿಕ;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ಸ್ಥಿರ ಶೂನ್ಯ *
sortlines_thread (ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
  ರಚನೆ thread_ವಾದಗಳು ಸ್ಥಿರಾಂಕ *ವಾದಗಳು = ದತ್ತಾಂಶ;
  sortlines (ವಾದಗಳು->ಸಾಲುಗಳು, ವಾದಗಳು->nthreads, ವಾದಗಳು->ಒಟ್ಟು_ಸಾಲುಗಳು,
             ವಾದಗಳು->ಗಂಟು, ವಾದಗಳು->ಸರತಿ, ವಾದಗಳು->tfp,
             ವಾದಗಳು->output_ತಾತ್ಕಾಲಿಕ);
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/* Sort lines, possibly in parallel.  The arguments are as in struct
   thread_args above.

   The algorithm has three phases: node creation, sequential sort,
   and binary merge.

   During node creation, sortlines recursively visits each node in the
   binary merge tree and creates a NODE structure corresponding to all the
   future line merging NODE is responsible for. For each call to
   sortlines, half the available threads are assigned to each recursive
   call, until a leaf node having only 1 available thread is reached.

   Each leaf node then performs two sequential sorts, one on each half of
   the lines it is responsible for. It records in its NODE structure that
   there are two sorted sublists available to merge from, and inserts its
   NODE into the priority queue.

   The binary merge phase then begins. Each thread drops into a loop
   where the thread retrieves a NODE from the priority queue, merges lines
   available to that NODE, and potentially insert NODE or its parent back
   into the queue if there are sufficient available lines for them to
   merge. This continues until all lines at all nodes of the merge tree
   have been merged. */

ಸ್ಥಿರ ಶೂನ್ಯ
sortlines (ರಚನೆ ಸಾಲು *ಸೀಮಿತ ಸಾಲುಗಳು, size_t nthreads,
           size_t ಒಟ್ಟು_ಸಾಲುಗಳು, ರಚನೆ ವಿಲೀನ_ಗಂಟು *ಗಂಟು,
           ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ *ಸರತಿ, FILE *tfp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_output)
{
  size_t nlines = ಗಂಟು->nlo + ಗಂಟು->nhi;

  /* Calculate thread arguments. */
  size_t lo_ಎಳೆಗಳು = nthreads / 2;
  size_t hi_ಎಳೆಗಳು = nthreads - lo_ಎಳೆಗಳು;
  pthread_t thread;
  ರಚನೆ thread_ವಾದಗಳು ವಾದಗಳು = {ಸಾಲುಗಳು, lo_ಎಳೆಗಳು, ಒಟ್ಟು_ಸಾಲುಗಳು,
                             ಗಂಟು->lo_ಮಗು, ಸರತಿ, tfp, ತಾತ್ಕಾಲಿಕ_output};

  ಆದರೆ (nthreads > 1 && SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC <= nlines
      && pthread_create (&thread, ಖಾಲಿ, sortlines_thread, &ವಾದಗಳು) == 0)
    {
      sortlines (ಸಾಲುಗಳು - ಗಂಟು->nlo, hi_ಎಳೆಗಳು, ಒಟ್ಟು_ಸಾಲುಗಳು,
                 ಗಂಟು->hi_ಮಗು, ಸರತಿ, tfp, ತಾತ್ಕಾಲಿಕ_output);
      pthread_join (thread, ಖಾಲಿ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ಗಂಟು->nlo;
      size_t nhi = ಗಂಟು->nhi;
      ರಚನೆ ಸಾಲು *ತಾತ್ಕಾಲಿಕ = ಸಾಲುಗಳು - ಒಟ್ಟು_ಸಾಲುಗಳು;
      ಆದರೆ (1 < nhi)
        sequential_ವಿಂಗಡಿಸು (ಸಾಲುಗಳು - nlo, nhi, ತಾತ್ಕಾಲಿಕ - nlo / 2, false);
      ಆದರೆ (1 < nlo)
        sequential_ವಿಂಗಡಿಸು (ಸಾಲುಗಳು, nlo, ತಾತ್ಕಾಲಿಕ, false);

      /* Update merge NODE. No need to lock yet. */
      ಗಂಟು->lo = ಸಾಲುಗಳು;
      ಗಂಟು->hi = ಸಾಲುಗಳು - nlo;
      ಗಂಟು->ಕೊನೆ_lo = ಸಾಲುಗಳು - nlo;
      ಗಂಟು->ಕೊನೆ_hi = ಸಾಲುಗಳು - nlo - nhi;

      ಸರತಿ_ಸೇರಿಸು (ಸರತಿ, ಗಂಟು);
      ವಿಲೀನ_loop (ಸರತಿ, ಒಟ್ಟು_ಸಾಲುಗಳು, tfp, ತಾತ್ಕಾಲಿಕ_output);
    }
}

/* Scan through FILES[NTEMPS .. NFILES-1] looking for files that are
   the same as OUTFILE.  If found, replace each with the same
   temporary copy that can be merged into OUTFILE without destroying
   OUTFILE before it is completely read.  This temporary copy does not
   count as a merge temp, so don't worry about incrementing NTEMPS in
   the caller; final cleanup will remove it, not zaptemp.

   This test ensures that an otherwise-erroneous use like
   "sort -m -o FILE ... FILE ..." copies FILE before writing to it.
   It's not clear that POSIX requires this nicety.
   Detect common error cases, but don't try to catch obscure cases like
   "cat ... FILE ... | sort -m -o FILE"
   where traditional "sort" doesn't copy the input and where
   people should know that they're getting into trouble anyway.
   Catching these obscure cases would slow down performance in
   common cases.  */

ಸ್ಥಿರ ಶೂನ್ಯ
avoid_trashing_ಒಳಹರಿವು (ರಚನೆ sortfile *files, size_t ntemps,
                      size_t nfiles, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outfile)
{
  ರಚನೆ tempnode *tempcopy = ಖಾಲಿ;

  ಗಾಗಿ (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ರಚನೆ stat instat;

      ಆದರೆ (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ರಚನೆ stat *outst = get_outstatus ();
          ಆದರೆ (!outst)
            ನಿಲ್ಲಿಸು;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ಆದರೆ (same)
        {
          ಆದರೆ (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_ತಾತ್ಕಾಲಿಕ (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].ತಾತ್ಕಾಲಿಕ = tempcopy;
        }
    }
}

/* Scan the input files to ensure all are accessible.
   Otherwise exit with a diagnostic.

   This will catch common issues with permissions etc.
   but will fail to notice issues where you can open but not read,
   like when a directory is specified on some systems.
   Catching these obscure cases could slow down performance in
   common cases.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_inputs (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *files, size_t nfiles)
{
  ಗಾಗಿ (size_t i = 0; i < nfiles; i++)
    {
      ಆದರೆ (streq (files[i], "-"))
        ಮುಂದುವರಿ;

      ಆದರೆ (euidaccess (files[i], R_OK) != 0)
        ವಿಂಗಡಿಸು_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_output (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outfile)
{
  ಆದರೆ (outfile)
    {
      ಪೂರ್ಣಾಂಕ oflags = O_WRONLY | O_ದ್ವಿಮಾನ | O_CLOEXEC | O_CREAT;
      ಪೂರ್ಣಾಂಕ outfd = open (outfile, oflags, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (outfd < 0)
        ವಿಂಗಡಿಸು_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ (ರಚನೆ sortfile *files, size_t ntemps, size_t nfiles,
       ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file)
{
  ಆಗ (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t ಒಳ;

      /* Number of output files generated so far.  */
      size_t ಹೊರಹೋಗು;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      ಗಾಗಿ (ಹೊರಹೋಗು = ಒಳ = 0; nmerge <= nfiles - ಒಳ; ಹೊರಹೋಗು++)
        {
          FILE *tfp;
          ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ = create_ತಾತ್ಕಾಲಿಕ (&tfp);
          size_t ಅಂಕೆ_merged = mergefiles (&files[ಒಳ], MIN (ntemps, nmerge),
                                          nmerge, tfp, ತಾತ್ಕಾಲಿಕ->name);
          ntemps -= MIN (ntemps, ಅಂಕೆ_merged);
          files[ಹೊರಹೋಗು].name = ತಾತ್ಕಾಲಿಕ->name;
          files[ಹೊರಹೋಗು].ತಾತ್ಕಾಲಿಕ = ತಾತ್ಕಾಲಿಕ;
          ಒಳ += ಅಂಕೆ_merged;
        }

      remainder = nfiles - ಒಳ;
      cheap_slots = nmerge - ಹೊರಹೋಗು % nmerge;

      ಆದರೆ (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ = create_ತಾತ್ಕಾಲಿಕ (&tfp);
          size_t ಅಂಕೆ_merged = mergefiles (&files[ಒಳ], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, ತಾತ್ಕಾಲಿಕ->name);
          ntemps -= MIN (ntemps, ಅಂಕೆ_merged);
          files[ಹೊರಹೋಗು].name = ತಾತ್ಕಾಲಿಕ->name;
          files[ಹೊರಹೋಗು++].ತಾತ್ಕಾಲಿಕ = ತಾತ್ಕಾಲಿಕ;
          ಒಳ += ಅಂಕೆ_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[ಹೊರಹೋಗು], &files[ಒಳ], (nfiles - ಒಳ) * ಗಾತ್ರ *files);
      ntemps += ಹೊರಹೋಗು;
      nfiles -= ಒಳ - ಹೊರಹೋಗು;
    }

  avoid_trashing_ಒಳಹರಿವು (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ಆಗ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ಒಳಹರಿವು_files (files, nfiles, &fps);

      ಆದರೆ (nopened == nfiles)
        {
          FILE *ofp = ಹರಿವು_open (output_file, "w");
          ಆದರೆ (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ನಿಲ್ಲಿಸು;
            }
          ಆದರೆ (errno != EMFILE || nopened <= 2)
            ವಿಂಗಡಿಸು_die (_("open failed"), output_file);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nopened <= 2)
        ವಿಂಗಡಿಸು_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ರಚನೆ tempnode *ತಾತ್ಕಾಲಿಕ;
      ಮಾಡು
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          ತಾತ್ಕಾಲಿಕ = maybe_create_ತಾತ್ಕಾಲಿಕ (&tfp, ! (nopened <= 2));
        }
      ಆಗ (!ತಾತ್ಕಾಲಿಕ);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, ತಾತ್ಕಾಲಿಕ->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = ತಾತ್ಕಾಲಿಕ->name;
      files[0].ತಾತ್ಕಾಲಿಕ = ತಾತ್ಕಾಲಿಕ;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * ಗಾತ್ರ *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಂಗಡಿಸು (ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *files, size_t nfiles, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *output_file,
      size_t nthreads)
{
  ರಚನೆ ಬಫರ್ buf;
  ರಚನೆ ವಿಂಗಡಿಸು_ಬಫರ್_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.ಎಲ್ಲೆ = 0;
  policy.growth_failed = false;

  ಆಗ (nfiles)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ_output;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು;
      ಆದರೆ (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ಆಗ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು = (mult * ಗಾತ್ರ (ರಚನೆ ಸಾಲು));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು = ಗಾತ್ರ (ರಚನೆ ಸಾಲು) * 3 / 2;

      ಆದರೆ (! buf.alloc)
        {
          ವಿಂಗಡಿಸು_ಬಫರ್_policy (&fp, 1, files, nfiles, ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು,
                              &policy);
          initbuf (&buf, ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ಆಗ (fillbuf (&buf, fp, file, &policy))
        {
          ರಚನೆ ಸಾಲು *ಸಾಲು;

          ಆದರೆ (buf.eof && nfiles
              && (ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು + 1
                  < (buf.alloc - buf.ಬಳಸಿದ - ಬೈಟುಗಳು_ಪ್ರತಿ_ಸಾಲು * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ಎಡ = buf.ಬಳಸಿದ;
              ನಿಲ್ಲಿಸು;
            }

          saved_ಸಾಲು.ಪಠ್ಯ = ಖಾಲಿ;
          ಸಾಲು = ಬಫರ್_linelim (&buf);
          ಆದರೆ (buf.eof && !nfiles && !ntemps && !buf.ಎಡ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              ತಾತ್ಕಾಲಿಕ_output = output_file;
              output_file_created = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ++ntemps;
              ತಾತ್ಕಾಲಿಕ_output = create_ತಾತ್ಕಾಲಿಕ (&tfp)->name;
            }
          ಆದರೆ (1 < buf.nlines)
            {
              ಆದರೆ (nthreads > 1)
                {
                  ರಚನೆ ವಿಲೀನ_ಗಂಟು_ಸರತಿ ಸರತಿ;
                  ಸರತಿ_init (&ಸರತಿ, nthreads);
                  ರಚನೆ ವಿಲೀನ_ಗಂಟು *ವಿಲೀನ_ವೃಕ್ಷ =
                    ವಿಲೀನ_ವೃಕ್ಷ_init (nthreads, buf.nlines, ಸಾಲು);

                  sortlines (ಸಾಲು, nthreads, buf.nlines, ವಿಲೀನ_ವೃಕ್ಷ + 1,
                             &ಸರತಿ, tfp, ತಾತ್ಕಾಲಿಕ_output);

                  ವಿಲೀನ_ವೃಕ್ಷ_destroy (nthreads, ವಿಲೀನ_ವೃಕ್ಷ);
                  ಸರತಿ_destroy (&ಸರತಿ);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  sequential_ವಿಂಗಡಿಸು (ಸಾಲು, buf.nlines,
                                   ಸಾಲು - buf.nlines, false);
                  ಗಾಗಿ (size_t i = 0; i < buf.nlines; i++)
                    write_unique (ಸಾಲು - i - 1, tfp, ತಾತ್ಕಾಲಿಕ_output);
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            write_unique (ಸಾಲು - 1, tfp, ತಾತ್ಕಾಲಿಕ_output);

          xfclose (tfp, ತಾತ್ಕಾಲಿಕ_output);

          ಆದರೆ (output_file_created)
            ಹೋಗು ಮುಕ್ತಾಯ;
        }
      xfclose (fp, file);
    }

 ಮುಕ್ತಾಯ:
  ಬಿಡುಗಡೆ (buf.buf);

  ಆದರೆ (! output_file_created)
    {
      ರಚನೆ tempnode *ಗಂಟು = temphead;
      ರಚನೆ sortfile *tempfiles = xnmalloc (ntemps, ಗಾತ್ರ *tempfiles);
      ಗಾಗಿ (size_t i = 0; ಗಂಟು; i++)
        {
          tempfiles[i].name = ಗಂಟು->name;
          tempfiles[i].ತಾತ್ಕಾಲಿಕ = ಗಂಟು;
          ಗಂಟು = ಗಂಟು->ಮುಂದಿನ;
        }
      ವಿಲೀನ (tempfiles, ntemps, ntemps, output_file);
      ಬಿಡುಗಡೆ (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ಸ್ಥಿರ ಶೂನ್ಯ
insertkey (ರಚನೆ keyfield *ಕೀಲಿ_arg)
{
  ರಚನೆ keyfield **p;
  ರಚನೆ keyfield *ಕೀಲಿ = xmemdup (ಕೀಲಿ_arg, ಗಾತ್ರ *ಕೀಲಿ);

  ಗಾಗಿ (p = &keylist; *p; p = &(*p)->ಮುಂದಿನ)
    ಮುಂದುವರಿ;
  *p = ಕೀಲಿ;
  ಕೀಲಿ->ಮುಂದಿನ = ಖಾಲಿ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ಸ್ಥಿರ ಶೂನ್ಯ
badfieldspec (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid)
{
  error (ವಿಂಗಡಿಸು_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
incompatible_options (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *opts)
{
  error (ವಿಂಗಡಿಸು_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_ordering_compatibility (ಶೂನ್ಯ)
{
  ಗಾಗಿ (ರಚನೆ keyfield *ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->ಮುಂದಿನ)
    ಆದರೆ (1 < (ಕೀಲಿ->numeric + ಕೀಲಿ->general_numeric + ಕೀಲಿ->human_numeric
             + ಕೀಲಿ->month + (ಕೀಲಿ->ಆವೃತ್ತಿ_3 | ಕೀಲಿ->random | !!ಕೀಲಿ->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        ಅಕ್ಷರ opts[ಗಾತ್ರ short_options];
        /* Clear flags we're not interested in.  */
        ಕೀಲಿ->skipsblanks = ಕೀಲಿ->skipeblanks = ಕೀಲಿ->ಹಿಮ್ಮುಖ = false;
        ಕೀಲಿ_ಗಮ್ಯ_opts (ಕೀಲಿ, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string, size_t *val, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *msgid)
{
  ಅಕ್ಷರ *suffix;
  uintmax_t n;

  ಆಯ್ಕೆ (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    ಸ್ಥಿತಿ LONGINT_OK:
    ಸ್ಥಿತಿ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ಆದರೆ (*val == n)
        ನಿಲ್ಲಿಸು;
      FALLTHROUGH;
    ಸ್ಥಿತಿ LONGINT_ಅತಿಪ್ರವಾಹ:
    ಸ್ಥಿತಿ LONGINT_ಅತಿಪ್ರವಾಹ | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ LONGINT_INVALID:
      ಆದರೆ (msgid)
        error (ವಿಂಗಡಿಸು_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಹಿಂತಿರುಗಿ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ಸ್ಥಿರ ಅಕ್ಷರ *
ಗಣ_ordering (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, ರಚನೆ keyfield *ಕೀಲಿ, ಎಣಿಕೆ blanktype blanktype)
{
  ಆಗ (*s)
    {
      ಆಯ್ಕೆ (*s)
        {
        ಸ್ಥಿತಿ 'b':
          ಆದರೆ (blanktype == bl_ಆರಂಭ || blanktype == bl_both)
            ಕೀಲಿ->skipsblanks = true;
          ಆದರೆ (blanktype == bl_ಕೊನೆ || blanktype == bl_both)
            ಕೀಲಿ->skipeblanks = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'd':
          ಕೀಲಿ->ignore = nondictionary;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          ಕೀಲಿ->translate = fold_toupper;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'g':
          ಕೀಲಿ->general_numeric = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'h':
          ಕೀಲಿ->human_numeric = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ಆದರೆ (! ಕೀಲಿ->ignore)
            ಕೀಲಿ->ignore = nonprinting;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'M':
          ಕೀಲಿ->month = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          ಕೀಲಿ->numeric = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'R':
          ಕೀಲಿ->random = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'r':
          ಕೀಲಿ->ಹಿಮ್ಮುಖ = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'V':
          ಕೀಲಿ->ಆವೃತ್ತಿ_3 = true;
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) s;
        }
      ++s;
    }
  ಹಿಂತಿರುಗಿ (ಅಕ್ಷರ *) s;
}

/* Initialize KEY.  */

ಸ್ಥಿರ ರಚನೆ keyfield *
ಕೀಲಿ_init (ರಚನೆ keyfield *ಕೀಲಿ)
{
  memset (ಕೀಲಿ, 0, ಗಾತ್ರ *ಕೀಲಿ);
  ಕೀಲಿ->eword = SIZE_MAX;
  ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ರಚನೆ keyfield *ಕೀಲಿ;
  ರಚನೆ keyfield ಕೀಲಿ_buf;
  ರಚನೆ keyfield gkey;
  bool gkey_only = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s;
  ಪೂರ್ಣಾಂಕ c = 0;
  ಅಕ್ಷರ checkonly = 0;
  bool mergeonly = false;
  ಅಕ್ಷರ *random_ಮೂಲ = ಖಾಲಿ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ);
  ಪೂರ್ಣಾಂಕ posix_ver = posix2_ಆವೃತ್ತಿ ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  ಅಕ್ಷರ **files;
  ಅಕ್ಷರ *files_from = ಖಾಲಿ;
  ರಚನೆ ಟೋಕನುಗಳು tok;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outfile = ಖಾಲಿ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ವಿಂಗಡಿಸು_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ಆದರೆ HAVE_NL_LANGINFO
  hard_LC_ಸಮಯ = hard_locale (LC_ಸಮಯ);
#ಆದರೆ_ಅಂತ್ಯ

  /* Get locale's representation of the decimal point.  */
  {
    ರಚನೆ lconv ಸ್ಥಿರಾಂಕ *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_ಬಿಂದು = locale->decimal_ಬಿಂದು[0];
    ಆದರೆ (! decimal_ಬಿಂದು || locale->decimal_ಬಿಂದು[1])
      decimal_ಬಿಂದು = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ಆದರೆ (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ಆದರೆ (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ಎಣಿಕೆ { nsigs = countof (ಪದ_sig) };

    ರಚನೆ sigaction act;

    sigemptyset (&caught_ಸಂಕೇತಗಳು);
    ಗಾಗಿ (size_t i = 0; i < nsigs; i++)
      {
        ಆದರೆ (ಪದ_sig[i] == SIGPIPE)
          ಮುಂದುವರಿ;  /* Handled below.  */
        sigaction (ಪದ_sig[i], ಖಾಲಿ, &act);
        ಆದರೆ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_ಸಂಕೇತಗಳು, ಪದ_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_ಸಂಕೇತಗಳು;
    act.sa_flags = 0;

    ಗಾಗಿ (size_t i = 0; i < nsigs; i++)
      {
        ಆದರೆ (ಪದ_sig[i] == SIGPIPE)
          ಮುಂದುವರಿ;  /* Handled below.  */
        ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, ಪದ_sig[i]))
          sigaction (ಪದ_sig[i], &act, ಖಾಲಿ);
      }
  }
  ಸಂಕೇತ (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ಕೀಲಿ_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, ಗಾತ್ರ *files);

  ಆಗ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      ಪೂರ್ಣಾಂಕ oi = -1;

      ಆದರೆ (c == -1
          || (posixly_correct && nfiles != 0
              && ! (traditional_usage
                    && ! checkonly
                    && optind != argc
                    && argv[optind][0] == '-' && argv[optind][1] == 'o'
                    && (argv[optind][2] || optind + 1 != argc)))
          || ((c = getopt_long (argc, argv, short_options,
                                long_options, &oi))
              == -1))
        {
          ಆದರೆ (argc <= optind)
            ನಿಲ್ಲಿಸು;
          files[nfiles++] = argv[optind++];
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 1:
          ಕೀಲಿ = ಖಾಲಿ;
          ಆದರೆ (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ಆದರೆ (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ಕೀಲಿ = ಕೀಲಿ_init (&ಕೀಲಿ_buf);
                  s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (optarg + 1, &ಕೀಲಿ->sword, ಖಾಲಿ);
                  ಆದರೆ (s && *s == '.')
                    s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->schar, ಖಾಲಿ);
                  ಆದರೆ (! (ಕೀಲಿ->sword || ಕೀಲಿ->schar))
                    ಕೀಲಿ->sword = SIZE_MAX;
                  ಆದರೆ (! s || *ಗಣ_ordering (s, ಕೀಲಿ, bl_ಆರಂಭ))
                    ಕೀಲಿ = ಖಾಲಿ;
                  ಇಲ್ಲದಿದ್ದರೆ
                    {
                      ಆದರೆ (minus_pos_usage)
                        {
                          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *optarg1 = argv[optind++];
                          s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (optarg1 + 1, &ಕೀಲಿ->eword,
                                             N_("invalid number after '-'"));
                          ಆದರೆ (*s == '.')
                            s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->echar,
                                               N_("invalid number after '.'"));
                          ಆದರೆ (!ಕೀಲಿ->echar && ಕೀಲಿ->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ಕೀಲಿ->eword--;
                            }
                          ಆದರೆ (*ಗಣ_ordering (s, ಕೀಲಿ, bl_ಕೊನೆ))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ಕೀಲಿ->traditional_ಬಳಸಿದ = true;
                      insertkey (ಕೀಲಿ);
                    }
                }
            }
          ಆದರೆ (! ಕೀಲಿ)
            files[nfiles++] = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ವಿಂಗಡಿಸು_OPTION:
          c = XARGMATCH ("--sort", optarg, ವಿಂಗಡಿಸು_ವಾದಗಳು, ವಿಂಗಡಿಸು_types);
          FALLTHROUGH;
        ಸ್ಥಿತಿ 'b':
        ಸ್ಥಿತಿ 'd':
        ಸ್ಥಿತಿ 'f':
        ಸ್ಥಿತಿ 'g':
        ಸ್ಥಿತಿ 'h':
        ಸ್ಥಿತಿ 'i':
        ಸ್ಥಿತಿ 'M':
        ಸ್ಥಿತಿ 'n':
        ಸ್ಥಿತಿ 'r':
        ಸ್ಥಿತಿ 'R':
        ಸ್ಥಿತಿ 'V':
          {
            ಅಕ್ಷರ str[2];
            str[0] = c;
            str[1] = '\0';
            ಗಣ_ordering (str, &gkey, bl_both);
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_ವಾದಗಳು, check_types)
               : 'c');
          FALLTHROUGH;
        ಸ್ಥಿತಿ 'c':
        ಸ್ಥಿತಿ 'C':
          ಆದರೆ (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ COMPRESS_ಕಾರ್ಯಕ್ರಮ_OPTION:
          ಆದರೆ (compress_ಕಾರ್ಯಕ್ರಮ && !streq (compress_ಕಾರ್ಯಕ್ರಮ, optarg))
            error (ವಿಂಗಡಿಸು_FAILURE, 0, _("multiple compress programs specified"));
          compress_ಕಾರ್ಯಕ್ರಮ = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEBUG_ಕಾರ್ಯಕ್ರಮ_OPTION:
          debug = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ FILES0_FROM_OPTION:
          files_from = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'k':
          ಕೀಲಿ = ಕೀಲಿ_init (&ಕೀಲಿ_buf);

          /* Get POS1. */
          s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (optarg, &ಕೀಲಿ->sword,
                                 N_("invalid number at field start"));
          ಆದರೆ (! ಕೀಲಿ->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ಆದರೆ (*s == '.')
            {
              s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->schar,
                                     N_("invalid number after '.'"));
              ಆದರೆ (! ಕೀಲಿ->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ಆದರೆ (! (ಕೀಲಿ->sword || ಕೀಲಿ->schar))
            ಕೀಲಿ->sword = SIZE_MAX;
          s = ಗಣ_ordering (s, ಕೀಲಿ, bl_ಆರಂಭ);
          ಆದರೆ (*s != ',')
            {
              ಕೀಲಿ->eword = SIZE_MAX;
              ಕೀಲಿ->echar = 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Get POS2. */
              s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->eword,
                                     N_("invalid number after ','"));
              ಆದರೆ (! ಕೀಲಿ->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ಆದರೆ (*s == '.')
                {
                  s = parse_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->echar,
                                         N_("invalid number after '.'"));
                }
              s = ಗಣ_ordering (s, ಕೀಲಿ, bl_ಕೊನೆ);
            }
          ಆದರೆ (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ಕೀಲಿ);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          mergeonly = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'o':
          ಆದರೆ (outfile && !streq (outfile, optarg))
            error (ವಿಂಗಡಿಸು_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ RANDOM_ಮೂಲ_OPTION:
          ಆದರೆ (random_ಮೂಲ && !streq (random_ಮೂಲ, optarg))
            error (ವಿಂಗಡಿಸು_FAILURE, 0, _("multiple random sources specified"));
          random_ಮೂಲ = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 's':
          stable = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'S':
          specify_ವಿಂಗಡಿಸು_ಗಾತ್ರ (oi, c, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          {
            ಅಕ್ಷರ newtab = optarg[0];
            ಆದರೆ (! newtab)
              error (ವಿಂಗಡಿಸು_FAILURE, 0, _("empty tab"));
            ಆದರೆ (optarg[1])
              {
                ಆದರೆ (streq (optarg, "\\0"))
                  newtab = '\0';
                ಇಲ್ಲದಿದ್ದರೆ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (ವಿಂಗಡಿಸು_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ಆದರೆ (tab != TAB_DEFAULT && tab != newtab)
              error (ವಿಂಗಡಿಸು_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'T':
          ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ_dir (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'u':
          unique = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ಆದರೆ (optarg == argv[optind - 1])
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;
              ಗಾಗಿ (p = optarg; c_isdigit (*p); p++)
                ಮುಂದುವರಿ;
              optind -= (*p != '\0');
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'z':
          eolchar = 0;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (ವಿಂಗಡಿಸು_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (ಸಂಕೇತ (SIGPIPE, SIG_IGN) == SIG_DFL);

  ಆದರೆ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ಆದರೆ (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (ವಿಂಗಡಿಸು_FAILURE);
        }

      FILE *ಹರಿವು = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ಆದರೆ (! readtokens0 (ಹರಿವು, &tok))
        error (ವಿಂಗಡಿಸು_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (ಹರಿವು, files_from);

      ಆದರೆ (tok.n_tok)
        {
          ಬಿಡುಗಡೆ (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          ಗಾಗಿ (size_t i = 0; i < nfiles; i++)
            {
              ಆದರೆ (streq (files[i], "-"))
                error (ವಿಂಗಡಿಸು_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ file_ಸಂಖ್ಯೆ = i + 1;
                  error (ವಿಂಗಡಿಸು_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_ಸಂಖ್ಯೆ);
                }
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        error (ವಿಂಗಡಿಸು_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  ಗಾಗಿ (ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->ಮುಂದಿನ)
    {
      ಆದರೆ (default_ಕೀಲಿ_compare (ಕೀಲಿ) && !ಕೀಲಿ->ಹಿಮ್ಮುಖ)
        {
          ಕೀಲಿ->ignore = gkey.ignore;
          ಕೀಲಿ->translate = gkey.translate;
          ಕೀಲಿ->skipsblanks = gkey.skipsblanks;
          ಕೀಲಿ->skipeblanks = gkey.skipeblanks;
          ಕೀಲಿ->month = gkey.month;
          ಕೀಲಿ->numeric = gkey.numeric;
          ಕೀಲಿ->general_numeric = gkey.general_numeric;
          ಕೀಲಿ->human_numeric = gkey.human_numeric;
          ಕೀಲಿ->ಆವೃತ್ತಿ_3 = gkey.ಆವೃತ್ತಿ_3;
          ಕೀಲಿ->random = gkey.random;
          ಕೀಲಿ->ಹಿಮ್ಮುಖ = gkey.ಹಿಮ್ಮುಖ;
        }

      need_random |= ಕೀಲಿ->random;
    }

  ಆದರೆ (!keylist && !default_ಕೀಲಿ_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ಆದರೆ (debug)
    {
      ಆದರೆ (checkonly || outfile)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ಆದರೆ (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ಆದರೆ (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ಆದರೆ (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ಖಾಲಿ)));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ಕೀಲಿ_warnings (&gkey, gkey_only);
    }

  ಹಿಮ್ಮುಖ = gkey.ಹಿಮ್ಮುಖ;

  ಆದರೆ (need_random)
    random_md5_state_init (random_ಮೂಲ);

  ಆದರೆ (ತಾತ್ಕಾಲಿಕ_dirs.n_dirs == 0)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *tmp_dir = getenv ("TMPDIR");
      ಕೂಡಿಸು_ತಾತ್ಕಾಲಿಕ_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ಆದರೆ (nfiles == 0)
    {
      nfiles = 1;
      ಬಿಡುಗಡೆ (files);
      files = xmalloc (ಗಾತ್ರ *files);
      *files = (ಅಕ್ಷರ *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ಆದರೆ (0 < ವಿಂಗಡಿಸು_ಗಾತ್ರ)
    ವಿಂಗಡಿಸು_ಗಾತ್ರ = MAX (ವಿಂಗಡಿಸು_ಗಾತ್ರ, MIN_ವಿಂಗಡಿಸು_ಗಾತ್ರ);

  ಆದರೆ (checkonly)
    {
      ಆದರೆ (nfiles > 1)
        error (ವಿಂಗಡಿಸು_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ಆದರೆ (outfile)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      ನಿರ್ಗಮಿಸು (check (files[0], checkonly) ? EXIT_SUCCESS : ವಿಂಗಡಿಸು_ಹೊರಹೋಗು_ದ_ಕ್ರಮ);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  ಆದರೆ (mergeonly)
    {
      ರಚನೆ sortfile *sortfiles = xcalloc (nfiles, ಗಾತ್ರ *sortfiles);

      ಗಾಗಿ (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      ವಿಲೀನ (sortfiles, 0, nfiles, outfile);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (!nthreads)
        {
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ np = ಅಂಕೆ_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_ಎಳೆಗಳು);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * ಗಾತ್ರ (ರಚನೆ ವಿಲೀನ_ಗಂಟು));
      nthreads = MIN (nthreads, nthreads_max);

      ವಿಂಗಡಿಸು (files, nfiles, outfile, nthreads);
    }

  ಆದರೆ (have_read_stdin && fclose (stdin) == EOF)
    ವಿಂಗಡಿಸು_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
