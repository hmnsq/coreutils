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

#ifndef RLIMIT_DATA
ಸಂರಚನಾ rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ಯದಿ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#ಯದಿ !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#ಯದಿ !defined OPEN_MAX
# define OPEN_MAX 20
#endif

#define UCHAR_LIM (UCHAR_MAX + 1)

#ifndef DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#endif

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#define MAX_MERGE(total, level) (((total) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
ಗಣನಾ { SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ಗಣನಾ { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
ಗಣನಾ
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

ಗಣನಾ
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

ಗಣನಾ
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_ಬೇರು = 1
  };

/* The representation of the decimal point in the current locale.  */
ಸ್ಥಿರ ವರ್ಣ decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ಸ್ಥಿರ bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ಸ್ಥಿರ bool hard_LC_COLLATE;
#ಯದಿ HAVE_NL_LANGINFO
ಸ್ಥಿರ bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ಗಣನಾ blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
ಸ್ಥಿರ ವರ್ಣ eolchar = '\n';

/* Lines are held in memory as counted strings. */
ಸಂರಚನಾ ಸಾಲು
{
  ವರ್ಣ *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  ವರ್ಣ *keybeg;			/* Start of first key. */
  ವರ್ಣ *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ಸಂರಚನಾ buffer
{
  ವರ್ಣ *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ಎಡ;			/* Number of bytes left from previous reads. */
  size_t ಸಾಲು_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ಸಂರಚನಾ sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
ಸಂರಚನಾ keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ಧ್ರುವ *ignore;		/* Boolean array of characters to ignore. */
  ವರ್ಣ ಧ್ರುವ *translate;	/* Translation applied to characters. */
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
  bool reverse;			/* Reverse the sense of comparison. */
  bool version;			/* sort by version number */
  bool traditional_used;	/* Traditional key option format is used. */
  ಸಂರಚನಾ keyfield *next;	/* Next keyfield to try. */
};

ಸಂರಚನಾ month
{
  ವರ್ಣ ಧ್ರುವ *name;
  ಪೂರ್ಣಾಙ್ಕ val;
};

/* Binary merge tree node. */
ಸಂರಚನಾ merge_ಗಂಟು
{
  ಸಂರಚನಾ ಸಾಲು *lo;              /* Lines to merge from LO child node. */
  ಸಂರಚನಾ ಸಾಲು *hi;              /* Lines to merge from HI child node. */
  ಸಂರಚನಾ ಸಾಲು *end_lo;          /* End of available lines from LO. */
  ಸಂರಚನಾ ಸಾಲು *end_hi;          /* End of available lines from HI. */
  ಸಂರಚನಾ ಸಾಲು **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ಸಂರಚನಾ merge_ಗಂಟು *parent;    /* Parent node. */
  ಸಂರಚನಾ merge_ಗಂಟು *lo_child;  /* LO child node. */
  ಸಂರಚನಾ merge_ಗಂಟು *hi_child;  /* HI child node. */
  unsigned ಪೂರ್ಣಾಙ್ಕ level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ಸಂರಚನಾ merge_ಗಂಟು_queue
{
  ಸಂರಚನಾ heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು saved_ಸಾಲು;

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
ಸ್ಥಿರ ವರ್ಣ fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ಸ್ಥಿರ ಸಂರಚನಾ month monthtab[] =
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
#define NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#define MIN_MERGE_BUFFER_SIZE (2 + ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ಸ್ಥಿರ size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ಸ್ಥಿರ size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define ಒಳಹರಿವು_FILE_SIZE_GUESS (128 * 1024)

ಸ್ಥಿರ ಸಂರಚನಾ
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  ವರ್ಣ ಧ್ರುವ **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
ಸ್ಥಿರ bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ಸ್ಥಿರ bool stable;

/* An int value outside char range.  */
ಗಣನಾ { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ಗಣನಾ { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ಸ್ಥಿರ bool unique;

/* Nonzero if any of the input files are the standard input. */
ಸ್ಥಿರ bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ಸ್ಥಿರ ಸಂರಚನಾ keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *compress_program;

/* Annotate the output with extra info to aid the user.  */
ಸ್ಥಿರ bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ಸ್ಥಿರ bool default_SIGPIPE;

/* The list of temporary files. */
ಸಂರಚನಾ tempnode
{
  ಸಂರಚನಾ tempnode *ಚಞ್ಚಲ next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  ವರ್ಣ state;
  ವರ್ಣ name[FLEXIBLE_ARRAY_MEMBER];
};
ಸ್ಥಿರ ಸಂರಚನಾ tempnode *ಚಞ್ಚಲ temphead;
ಸ್ಥಿರ ಸಂರಚನಾ tempnode *ಚಞ್ಚಲ *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಸಂರಚನಾ tempnode ಧ್ರುವ *ಗಂಟು = temphead; ಗಂಟು; ಗಂಟು = ಗಂಟು->next)
    unlink (ಗಂಟು->name);
  temphead = ರಿಕ್ತ;
}

/* Handle interrupts and hangups. */

ಸ್ಥಿರ ಶೂನ್ಯ
sighandler (ಪೂರ್ಣಾಙ್ಕ sig)
{
  cleanup ();

  ಸಙ್ಕೇತ (sig, SIG_DFL);
  ಉತ್ಥಾಪಯ (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sort_die (ವರ್ಣ ಧ್ರುವ *message, ವರ್ಣ ಧ್ರುವ *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ಯದಿ (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      fputs (_("\
Write sorted concatenation of all FILE(s) to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding sort ಕೀಲಿಗಳು in each ಸಾಲು\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-ಪಕ್ಷ\n\
         fold lower ಪಕ್ಷ to upper ಪಕ್ಷ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical ಮೌಲ್ಯ\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-sort\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-sort\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-sort\n\
         compare according to string numerical ಮೌಲ್ಯ;\n\
         see full documentation ಪ್ರತಿ supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical ಕೀಲಿಗಳು.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-source=FILE\n\
         get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse the result of comparisons\n\
"));
      oputs (_("\
      --sort=WORD\n\
         sort according to WORD:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, version -V\n\
"));
      oputs (_("\
  -V, --version-sort\n\
         natural sort of (version) numbers within text\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-size=NMERGE\n\
         merge at most NMERGE inputs at once; ಪ್ರತಿ more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check ಪ್ರತಿ sorted ಒಳಹರಿವು; ಕುರು not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but ಕುರು not report first bad ಸಾಲು\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the ಸಾಲು used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ಪಠ ಒಳಹರಿವು from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, ಪಠ names from standard ಒಳಹರಿವು\n\
"));
      oputs (_("\
  -k, --ಕೀಲಿ=KEYDEF\n\
         sort via a ಕೀಲಿ; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; ಕುರು not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         ಲಿಖ result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE ಪ್ರತಿ main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR ಪ್ರತಿ temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the ಸಂಖ್ಯೆ of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of ಸಾಲುಗಳು with equal ಕೀಲಿಗಳು;\n\
         with -c, check ಪ್ರತಿ strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] ಪ್ರತಿ start and stop position, where F is a\n\
field ಸಂಖ್ಯೆ and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the ಸಾಲು's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options ಪ್ರತಿ that ಕೀಲಿ.  If no ಕೀಲಿ is given, use\n\
the entire ಸಾಲು as the ಕೀಲಿ.  Use --debug to diagnose incorrect ಕೀಲಿ usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (ಸ್ವಭಾವ), and so on ಪ್ರತಿ M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte ಮೌಲ್ಯಗಳು.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_PROGRAM_OPTION,
  DEBUG_PROGRAM_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_SOURCE_OPTION,
  SORT_OPTION,
  PARALLEL_OPTION
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"ignore-leading-blanks", no_argument, ರಿಕ್ತ, 'b'},
  {"check", optional_argument, ರಿಕ್ತ, CHECK_OPTION},
  {"compress-program", required_argument, ರಿಕ್ತ, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, ರಿಕ್ತ, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, ರಿಕ್ತ, 'd'},
  {"ignore-case", no_argument, ರಿಕ್ತ, 'f'},
  {"files0-from", required_argument, ರಿಕ್ತ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, ರಿಕ್ತ, 'g'},
  {"ignore-nonprinting", no_argument, ರಿಕ್ತ, 'i'},
  {"key", required_argument, ರಿಕ್ತ, 'k'},
  {"merge", no_argument, ರಿಕ್ತ, 'm'},
  {"month-sort", no_argument, ರಿಕ್ತ, 'M'},
  {"numeric-sort", no_argument, ರಿಕ್ತ, 'n'},
  {"human-numeric-sort", no_argument, ರಿಕ್ತ, 'h'},
  {"version-sort", no_argument, ರಿಕ್ತ, 'V'},
  {"random-sort", no_argument, ರಿಕ್ತ, 'R'},
  {"random-source", required_argument, ರಿಕ್ತ, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, ರಿಕ್ತ, SORT_OPTION},
  {"output", required_argument, ರಿಕ್ತ, 'o'},
  {"reverse", no_argument, ರಿಕ್ತ, 'r'},
  {"stable", no_argument, ರಿಕ್ತ, 's'},
  {"batch-size", required_argument, ರಿಕ್ತ, NMERGE_OPTION},
  {"buffer-size", required_argument, ರಿಕ್ತ, 'S'},
  {"field-separator", required_argument, ರಿಕ್ತ, 't'},
  {"temporary-directory", required_argument, ರಿಕ್ತ, 'T'},
  {"unique", no_argument, ರಿಕ್ತ, 'u'},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {"parallel", required_argument, ರಿಕ್ತ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE ರಿಕ್ತ
#undef  _ct_
};
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ check_types[] =
{
#define _ct_(_s, _c) _c,
  CHECK_TABLE
#undef  _ct_
};

#define SORT_TABLE \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE ರಿಕ್ತ
#undef  _st_
};
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
ಸ್ಥಿರ sigset_t caught_signals;

/* Critical section status.  */
ಸಂರಚನಾ cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ಸ್ಥಿರ ಶೂನ್ಯ
cs_enter (ಸಂರಚನಾ cs_status *status)
{
  ಪೂರ್ಣಾಙ್ಕ ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
ಸ್ಥಿರ ಶೂನ್ಯ
cs_leave (ಸಂರಚನಾ cs_status ಧ್ರುವ *status)
{
  ಯದಿ (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, ರಿಕ್ತ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ಗಣನಾ { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ಸಂರಚನಾ sortfile
{
  /* The file's name.  */
  ವರ್ಣ ಧ್ರುವ *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ಸಂರಚನಾ tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ಸ್ಥಿರ Hash_table *proctab;

ಗಣನಾ { INIT_PROCTAB_SIZE = 47 };

ಸ್ಥಿರ size_t
proctab_hasher (ಶೂನ್ಯ ಧ್ರುವ *entry, size_t tabsize)
{
  ಸಂರಚನಾ tempnode ಧ್ರುವ *ಗಂಟು = entry;
  ಪ್ರತ್ಯಾಗಮ ಗಂಟು->pid % tabsize;
}

ಸ್ಥಿರ bool
proctab_comparator (ಶೂನ್ಯ ಧ್ರುವ *e1, ಶೂನ್ಯ ಧ್ರುವ *e2)
{
  ಸಂರಚನಾ tempnode ಧ್ರುವ *n1 = e1;
  ಸಂರಚನಾ tempnode ಧ್ರುವ *n2 = e2;
  ಪ್ರತ್ಯಾಗಮ n1->pid == n2->pid;
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
  ಪೂರ್ಣಾಙ್ಕ status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  ಯದಿ (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  ಅನ್ಯಥಾ ಯದಿ (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ಯದಿ (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  ಪ್ರತ್ಯಾಗಮ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ಸ್ಥಿರ ಶೂನ್ಯ
register_proc (ಸಂರಚನಾ tempnode *temp)
{
  ಯದಿ (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, ರಿಕ್ತ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ರಿಕ್ತ);
      ಯದಿ (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  ಯದಿ (! hash_ಸೇರಿಸು (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ಸ್ಥಿರ bool
delete_proc (pid_t pid)
{
  ಸಂರಚನಾ tempnode test;

  test.pid = pid;
  ಸಂರಚನಾ tempnode *ಗಂಟು = hash_remove (proctab, &test);
  ಯದಿ (! ಗಂಟು)
    ಪ್ರತ್ಯಾಗಮ false;
  ಗಂಟು->state = REAPED;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ಸ್ಥಿರ ಶೂನ್ಯ
wait_proc (pid_t pid)
{
  ಯದಿ (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ಸ್ಥಿರ ಶೂನ್ಯ
reap_exited (ಶೂನ್ಯ)
{
  ಯಾವತ್ (0 < nprocs && reap (0))
    ಅನುವರ್ತ;
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
  ಯಾವತ್ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ಸ್ಥಿರ ಶೂನ್ಯ
exit_cleanup (ಶೂನ್ಯ)
{
  ಯದಿ (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ಸಂರಚನಾ cs_status cs;
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

ಸ್ಥಿರ ಸಂರಚನಾ tempnode *
create_temp_file (ಪೂರ್ಣಾಙ್ಕ *pfd, bool survive_fd_exhaustion)
{
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ slashbase[] = "/sortXXXXXX";
  ಸ್ಥಿರ idx_t temp_dir_index;
  ಪೂರ್ಣಾಙ್ಕ fd;
  ಪೂರ್ಣಾಙ್ಕ saved_errno;
  ವರ್ಣ ಧ್ರುವ *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  ಸಂರಚನಾ tempnode *ಗಂಟು =
    xmalloc (FLEXSIZEOF (ಸಂರಚನಾ tempnode, name, len + ಪರಿಮಾಣ slashbase));
  ವರ್ಣ *file = ಗಂಟು->name;
  ಸಂರಚನಾ cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, ಪರಿಮಾಣ slashbase);
  ಗಂಟು->next = ರಿಕ್ತ;
  ಯದಿ (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ಯದಿ (0 <= fd)
    {
      *temptail = ಗಂಟು;
      temptail = &ಗಂಟು->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ಯದಿ (fd < 0)
    {
      ಯದಿ (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      ಮುಕ್ತ (ಗಂಟು);
      ಗಂಟು = ರಿಕ್ತ;
    }

  *pfd = fd;
  ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ಸ್ಥಿರ ಸಂರಚನಾ stat *
get_outstatus (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ outstat_errno;
  ಸ್ಥಿರ ಸಂರಚನಾ stat outstat;
  ಯದಿ (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  ಪ್ರತ್ಯಾಗಮ outstat_errno < 0 ? &outstat : ರಿಕ್ತ;
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
stream_open (ವರ್ಣ ಧ್ರುವ *file, ವರ್ಣ ಧ್ರುವ *how)
{
  FILE *fp;

  ಯದಿ (*how == 'r')
    {
      ಯದಿ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ರಿಕ್ತ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ಅನ್ಯಥಾ ಯದಿ (*how == 'w')
    {
      ಯದಿ (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          ಪೂರ್ಣಾಙ್ಕ ftruncate_errno = errno;
          ಸಂರಚನಾ stat *outst = get_outstatus ();
          ಯದಿ (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ಅನ್ಯಥಾ
    affirm (!"unexpected mode passed to stream_open");

  ಪ್ರತ್ಯಾಗಮ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ಸ್ಥಿರ FILE *
xfopen (ವರ್ಣ ಧ್ರುವ *file, ವರ್ಣ ಧ್ರುವ *how)
{
  FILE *fp = stream_open (file, how);
  ಯದಿ (!fp)
    sort_die (_("open failed"), file);
  ಪ್ರತ್ಯಾಗಮ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ಸ್ಥಿರ ಶೂನ್ಯ
xfclose (FILE *fp, ವರ್ಣ ಧ್ರುವ *file)
{
  ವಿಕಲ್ಪ (fileno (fp))
    {
    ಪಕ್ಷ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ಭಙ್ಗ;

    ಪಕ್ಷ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ಯದಿ (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಯದಿ (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      ಭಙ್ಗ;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ಸ್ಥಿರ ಶೂನ್ಯ
move_fd (ಪೂರ್ಣಾಙ್ಕ oldfd, ಪೂರ್ಣಾಙ್ಕ newfd)
{
  ಯದಿ (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_ಮೌಲ್ಯ (dup2 (oldfd, newfd));
      ignore_ಮೌಲ್ಯ (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  ಪೂರ್ಣಾಙ್ಕ oldfd, ಪೂರ್ಣಾಙ್ಕ newfd)
{
  ಪೂರ್ಣಾಙ್ಕ result = 0;
  ಯದಿ (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ಯದಿ (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  ಪ್ರತ್ಯಾಗಮ result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
get_resolved_compress_program (ಶೂನ್ಯ)
{
  /* Use a cache, to perform the search only once.  */
  ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *resolved_compress_program_cache /* = NULL */;

  ಯದಿ (resolved_compress_program_cache == ರಿಕ್ತ)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), ರಿಕ್ತ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  ಪ್ರತ್ಯಾಗಮ resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
pipe_child (pid_t *pid, ಪೂರ್ಣಾಙ್ಕ pipefds[2], ಪೂರ್ಣಾಙ್ಕ tempfd, bool decompress,
            size_t tries)
{
  ವರ್ಣ ಧ್ರುವ *resolved_compress_program;
  ಸಂರಚನಾ tempnode *saved_temphead;
  ದ್ವಿಗುಣ wait_retry = 0.25;
  ಸಂರಚನಾ cs_status cs;
  ಪೂರ್ಣಾಙ್ಕ result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  ಯದಿ (resolved_compress_program == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ errno;

  ಯದಿ ((result = posix_spawnattr_init (&attr)))
    ಪ್ರತ್ಯಾಗಮ result;
  ಯದಿ ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      ಪ್ರತ್ಯಾಗಮ result;
    }

  ಯದಿ (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ಪ್ರತ್ಯಾಗಮ saved_errno;
    }

  ಯದಿ ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (result = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((result = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (result = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (result = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((result = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (result = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (result = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ಪ್ರತ್ಯಾಗಮ result;
    }

  ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : ರಿಕ್ತ,
      ರಿಕ್ತ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ಯದಿ (nmerge + 1 < nprocs)
    reap_some ();

  ಯಾವತ್ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ರಿಕ್ತ;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (ವರ್ಣ * ಧ್ರುವ *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ಯದಿ (result != EAGAIN)
        ಭಙ್ಗ;
      ಅನ್ಯಥಾ
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

  ಯದಿ (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ಅನ್ಯಥಾ
    ++nprocs;

  ಪ್ರತ್ಯಾಗಮ result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ಸ್ಥಿರ ಸಂರಚನಾ tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  ಪೂರ್ಣಾಙ್ಕ tempfd;
  ಸಂರಚನಾ tempnode *ಗಂಟು = create_temp_file (&tempfd, survive_fd_exhaustion);
  ಯದಿ (! ಗಂಟು)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಗಂಟು->state = UNCOMPRESSED;

  ಯದಿ (compress_program)
    {
      ಪೂರ್ಣಾಙ್ಕ pipefds[2];
      ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ last_result = 0;

      ಪೂರ್ಣಾಙ್ಕ result = pipe_child (&ಗಂಟು->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ಯದಿ (result)
        {
          ಯದಿ (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      ಅನ್ಯಥಾ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (ಗಂಟು);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  ಯದಿ (! *pfp)
    sort_die (_("couldn't create temporary file"), ಗಂಟು->name);

  ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ಸ್ಥಿರ ಸಂರಚನಾ tempnode *
create_temp (FILE **pfp)
{
  ಪ್ರತ್ಯಾಗಮ maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ಸ್ಥಿರ FILE *
open_temp (ಸಂರಚನಾ tempnode *temp)
{
  ಪೂರ್ಣಾಙ್ಕ tempfd, pipefds[2];
  FILE *fp = ರಿಕ್ತ;

  ಯದಿ (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  ಯದಿ (tempfd < 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  pid_t child;
  ಪೂರ್ಣಾಙ್ಕ result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ಯದಿ (result)
    {
      ಯದಿ (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  ಅನ್ಯಥಾ
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ಯದಿ (! fp)
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  ಪ್ರತ್ಯಾಗಮ fp;
}

/* Append DIR to the array of temporary directory names.  */
ಸ್ಥಿರ ಶೂನ್ಯ
add_temp_dir (ವರ್ಣ ಧ್ರುವ *dir)
{
  ಯದಿ (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              ಪರಿಮಾಣ *temp_dirs.dirs);

  idx_t ಧ್ರುವ dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ಸ್ಥಿರ ಶೂನ್ಯ
zaptemp (ವರ್ಣ ಧ್ರುವ *name)
{
  ಸಂರಚನಾ tempnode *ಚಞ್ಚಲ *pnode;
  ಸಂರಚನಾ tempnode *ಗಂಟು;
  ಸಂರಚನಾ tempnode *next;
  ಪೂರ್ಣಾಙ್ಕ unlink_status;
  ಪೂರ್ಣಾಙ್ಕ unlink_errno = 0;
  ಸಂರಚನಾ cs_status cs;

  ಪ್ರತಿ (pnode = &temphead; (ಗಂಟು = *pnode)->name != name; pnode = &ಗಂಟು->next)
    ಅನುವರ್ತ;

  ಯದಿ (ಗಂಟು->state == UNREAPED)
    wait_proc (ಗಂಟು->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = ಗಂಟು->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  ಯದಿ (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ಯದಿ (! next)
    temptail = pnode;
  ಮುಕ್ತ (ಗಂಟು);
}

#ಯದಿ HAVE_NL_LANGINFO

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
struct_month_cmp (ಶೂನ್ಯ ಧ್ರುವ *m1, ಶೂನ್ಯ ಧ್ರುವ *m2)
{
  ಸಂರಚನಾ month ಧ್ರುವ *month1 = m1;
  ಸಂರಚನಾ month ಧ್ರುವ *month2 = m2;
  ಪ್ರತ್ಯಾಗಮ strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

ಸ್ಥಿರ ಶೂನ್ಯ
inittables (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#ಯದಿ HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ಯದಿ (hard_LC_TIME)
    {
      ಪ್ರತಿ (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          ವರ್ಣ ಧ್ರುವ *s;
          size_t s_len;
          size_t j, k;
          ವರ್ಣ *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          ಪ್ರತಿ (j = k = 0; j < s_len; j++)
            ಯದಿ (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, ಪರಿಮಾಣ *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ಸ್ಥಿರ ಶೂನ್ಯ
specify_nmerge (ಪೂರ್ಣಾಙ್ಕ oi, ವರ್ಣ c, ವರ್ಣ ಧ್ರುವ *s)
{
  uintmax_t n;
  ಸಂರಚನಾ rlimit rlimit;
  ಗಣನಾ strtol_error e = xstrtoumax (s, ರಿಕ್ತ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned ಪೂರ್ಣಾಙ್ಕ max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ಯದಿ (e == LONGINT_OK)
    {
      nmerge = n;
      ಯದಿ (nmerge != n)
        e = LONGINT_OVERFLOW;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ಅನ್ಯಥಾ ಯದಿ (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

  ಯದಿ (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ಅನ್ಯಥಾ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ಸ್ಥಿರ ಶೂನ್ಯ
specify_sort_size (ಪೂರ್ಣಾಙ್ಕ oi, ವರ್ಣ c, ವರ್ಣ ಧ್ರುವ *s)
{
  uintmax_t n;
  ವರ್ಣ *suffix;
  ಗಣನಾ strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ಯದಿ (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ಯದಿ (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    ವಿಕಲ್ಪ (suffix[0])
      {
      ಪಕ್ಷ 'b':
        e = LONGINT_OK;
        ಭಙ್ಗ;

      ಪಕ್ಷ '%':
        {
          ದ್ವಿಗುಣ mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ಯದಿ (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ಅನ್ಯಥಾ
            e = LONGINT_OVERFLOW;
        }
        ಭಙ್ಗ;
      }

  ಯದಿ (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ಯದಿ (n < sort_size)
        ಪ್ರತ್ಯಾಗಮ;

      sort_size = n;
      ಯದಿ (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          ಪ್ರತ್ಯಾಗಮ;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ಸ್ಥಿರ size_t
specify_nthreads (ಪೂರ್ಣಾಙ್ಕ oi, ವರ್ಣ c, ವರ್ಣ ಧ್ರುವ *s)
{
  uintmax_t nthreads;
  ಗಣನಾ strtol_error e = xstrtoumax (s, ರಿಕ್ತ, 10, &nthreads, "");
  ಯದಿ (e == LONGINT_OVERFLOW)
    ಪ್ರತ್ಯಾಗಮ SIZE_MAX;
  ಯದಿ (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ಯದಿ (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ಯದಿ (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  ಪ್ರತ್ಯಾಗಮ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ಸ್ಥಿರ size_t
default_sort_size (ಶೂನ್ಯ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  ಸಂರಚನಾ rlimit rlimit;
  ಯದಿ (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  ಯದಿ (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ಯದಿ (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ದ್ವಿಗುಣ avail = physmem_available ();
  ದ್ವಿಗುಣ total = physmem_total ();
  ದ್ವಿಗುಣ mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ಯದಿ (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ಯದಿ (mem < size)
    size = mem;
  ಪ್ರತ್ಯಾಗಮ MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ಸ್ಥಿರ bool
ಸಾಲು_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  ಯದಿ (ckd_add (&aligned, size, padding))
    ಪ್ರತ್ಯಾಗಮ false;

  *alloc = aligned;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ಸ್ಥಿರ size_t
default_initial_sort_size (size_t ಸಾಲು_bytes)
{
  size_t size = ಸಾಲು_bytes + 2;
  size_t ಒಳಹರಿವು_size;

  ಯದಿ (ckd_mul (&ಒಳಹರಿವು_size, ಒಳಹರಿವು_FILE_SIZE_GUESS, ಸಾಲು_bytes + 1)
      || ckd_add (&size, size, ಒಳಹರಿವು_size))
    ಪ್ರತ್ಯಾಗಮ SIZE_MAX;

  ಪ್ರತ್ಯಾಗಮ MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ಸ್ಥಿರ bool
ಒಳಹರಿವು_size_buffer_bytes (uintmax_t file_size, size_t ಸಾಲು_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_ಒಳಹರಿವು_byte = ಸಾಲು_bytes + 1;

  ಯದಿ (SIZE_MAX / worst_case_per_ಒಳಹರಿವು_byte < file_size)
    ಪ್ರತ್ಯಾಗಮ false;

  size_t size = file_size * worst_case_per_ಒಳಹರಿವು_byte;
  ಯದಿ (SIZE_MAX - size < 1)
    ಪ್ರತ್ಯಾಗಮ false;

  *worst_case = size + 1;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ಸ್ಥಿರ ಶೂನ್ಯ
sort_buffer_policy (FILE *ಧ್ರುವ *fps, size_t nfps,
                    ವರ್ಣ *ಧ್ರುವ *files, size_t nfiles,
                    size_t ಸಾಲು_bytes,
                    ಸಂರಚನಾ sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (ಸಾಲು_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_ಒಳಹರಿವು_byte = ಸಾಲು_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_ಒಳಹರಿವು_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  ಪ್ರತಿ (size_t i = 0; i < nfiles; i++)
    {
      ಸಂರಚನಾ stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      ಯದಿ (size_bound <= size)
        {
          policy->initial = size_bound;
          ಪ್ರತ್ಯಾಗಮ;
        }

      ಯದಿ ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      ಯದಿ (known_size)
        file_size = st.st_size;
      ಅನ್ಯಥಾ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ಯದಿ (sort_size)
            {
              policy->initial = size_bound;
              ಪ್ರತ್ಯಾಗಮ;
            }
          file_size = ಒಳಹರಿವು_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ಯದಿ (! ಒಳಹರಿವು_size_buffer_bytes (file_size, ಸಾಲು_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          ಪ್ರತ್ಯಾಗಮ;
        }

      ಯದಿ (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          ಪ್ರತ್ಯಾಗಮ;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ಸ್ಥಿರ ಶೂನ್ಯ
initbuf (ಸಂರಚನಾ buffer *buf, size_t ಸಾಲು_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ಯಾವತ್ (true)
    {
      ಯದಿ (! ಸಾಲು_aligned_size (&alloc))
        buf->buf = ರಿಕ್ತ;
      ಅನ್ಯಥಾ
        buf->buf = malloc (alloc);
      ಯದಿ (buf->buf)
        ಭಙ್ಗ;
      alloc /= 2;
      ಯದಿ (alloc <= ಸಾಲು_bytes + 1)
        xalloc_die ();
    }

  buf->ಸಾಲು_bytes = ಸಾಲು_bytes;
  buf->alloc = alloc;
  buf->used = buf->ಎಡ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ ಸಾಲು *
buffer_linelim (ಸಂರಚನಾ buffer ಧ್ರುವ *buf)
{
  ಶೂನ್ಯ *linelim = buf->buf + buf->alloc;
  ಪ್ರತ್ಯಾಗಮ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ಸ್ಥಿರ bool
try_growbuf (ಸಂರಚನಾ buffer *buf, size_t alloc)
{
  ಯದಿ (! ಸಾಲು_aligned_size (&alloc) || alloc <= buf->alloc)
    ಪ್ರತ್ಯಾಗಮ false;

  ವರ್ಣ *newbuf = malloc (alloc);
  ಯದಿ (! newbuf)
    ಪ್ರತ್ಯಾಗಮ false;

  ವರ್ಣ *oldbuf = buf->buf;
  ಸಂರಚನಾ ಸಾಲು *old_linelim = buffer_linelim (buf);
  ಸಂರಚನಾ ಸಾಲು *old_ಸಾಲು = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  ಸಂರಚನಾ ಸಾಲು *new_linelim = (ಶೂನ್ಯ *) (newbuf + alloc);
  ಸಂರಚನಾ ಸಾಲು *new_ಸಾಲು = new_linelim - buf->nlines;
  memcpy (new_ಸಾಲು, old_ಸಾಲು, buf->nlines * ಪರಿಮಾಣ *new_ಸಾಲು);

  ಪ್ರತಿ (ಸಂರಚನಾ ಸಾಲು *ಸಾಲು = new_ಸಾಲು; ಸಾಲು < new_linelim; ಸಾಲು++)
    {
      ಸಾಲು->text = newbuf + (ಸಾಲು->text - oldbuf);
      ಯದಿ (keylist)
        {
          ಸಾಲು->keybeg = newbuf + (ಸಾಲು->keybeg - oldbuf);
          ಸಾಲು->keylim = newbuf + (ಸಾಲು->keylim - oldbuf);
        }
    }

  ಮುಕ್ತ (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  ಪ್ರತ್ಯಾಗಮ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ಸ್ಥಿರ bool
maybe_growbuf (ಸಂರಚನಾ buffer *buf, ಸಂರಚನಾ sort_buffer_policy *policy)
{
  ಯದಿ (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    ಪ್ರತ್ಯಾಗಮ false;

  size_t alloc;
  ಯದಿ (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  ಅನ್ಯಥಾ
    alloc = policy->limit;

  ಯದಿ (try_growbuf (buf, alloc))
    ಪ್ರತ್ಯಾಗಮ true;

  policy->growth_failed = true;
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ಸ್ಥಿರ ವರ್ಣ *
begfield (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ)
{
  ವರ್ಣ *ptr = ಸಾಲು->text, *lim = ptr + ಸಾಲು->length - 1;
  size_t sword = ಕೀಲಿ->sword;
  size_t schar = ಕೀಲಿ->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ಯದಿ (tab != TAB_DEFAULT)
    ಯಾವತ್ (ptr < lim && sword--)
      {
        ವರ್ಣ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ಯದಿ (ptr < lim)
          ++ptr;
      }
  ಅನ್ಯಥಾ
    ಯಾವತ್ (ptr < lim && sword--)
      {
        ಯಾವತ್ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ಯಾವತ್ (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ಯದಿ (ಕೀಲಿ->skipsblanks)
    ಯಾವತ್ (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  ಯದಿ (schar < remaining_bytes)
    ptr += schar;
  ಅನ್ಯಥಾ
    ptr = lim;

  ಪ್ರತ್ಯಾಗಮ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
ಸ್ಥಿರ ವರ್ಣ *
limfield (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ)
{
  ವರ್ಣ *ptr = ಸಾಲು->text, *lim = ptr + ಸಾಲು->length - 1;
  size_t eword = ಕೀಲಿ->eword, echar = ಕೀಲಿ->echar;

  ಯದಿ (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ಯದಿ (tab != TAB_DEFAULT)
    ಯಾವತ್ (ptr < lim && eword--)
      {
        ವರ್ಣ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ಯದಿ (ptr < lim && (eword || echar))
          ++ptr;
      }
  ಅನ್ಯಥಾ
    ಯಾವತ್ (ptr < lim && eword--)
      {
        ಯಾವತ್ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ಯಾವತ್ (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

#ifdef POSIX_UNSPECIFIED
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
  ಯದಿ (tab != TAB_DEFAULT)
    {
      ವರ್ಣ *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ಯದಿ (newlim)
        lim = newlim;
    }
  ಅನ್ಯಥಾ
    {
      ವರ್ಣ *newlim;
      newlim = ptr;
      ಯಾವತ್ (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      ಯಾವತ್ (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  ಯದಿ (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ಯದಿ (ಕೀಲಿ->skipeblanks)
        ಯಾವತ್ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      ಯದಿ (echar < remaining_bytes)
        ptr += echar;
      ಅನ್ಯಥಾ
        ptr = lim;
    }

  ಪ್ರತ್ಯಾಗಮ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ಸ್ಥಿರ bool
fillbuf (ಸಂರಚನಾ buffer *buf, FILE *fp, ವರ್ಣ ಧ್ರುವ *file,
         ಸಂರಚನಾ sort_buffer_policy *policy)
{
  ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ = keylist;
  ವರ್ಣ eol = eolchar;
  size_t ಸಾಲು_bytes = buf->ಸಾಲು_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  ಯದಿ (buf->eof)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (buf->used != buf->ಎಡ)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->ಎಡ, buf->ಎಡ);
      buf->used = buf->ಎಡ;
      buf->nlines = 0;
    }

  ಯಾವತ್ (true)
    {
      ವರ್ಣ *ptr = buf->buf + buf->used;
      ಸಂರಚನಾ ಸಾಲು *linelim = buffer_linelim (buf);
      ಸಂರಚನಾ ಸಾಲು *ಸಾಲು = linelim - buf->nlines;
      size_t avail = (ವರ್ಣ *) linelim - buf->nlines * ಸಾಲು_bytes - ptr;
      ವರ್ಣ *ಸಾಲು_start = buf->nlines ? ಸಾಲು->text + ಸಾಲು->length : buf->buf;

      ಯಾವತ್ (ಸಾಲು_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (ಸಾಲು_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          ವರ್ಣ *ptrlim = ptr + bytes_read;
          ವರ್ಣ *p;
          avail -= bytes_read;

          ಯದಿ (bytes_read != readsize)
            {
              ಯದಿ (ferror (fp))
                sort_die (_("read failed"), file);
              ಯದಿ (feof (fp))
                {
                  buf->eof = true;
                  ಯದಿ (buf->buf == ptrlim)
                    ಪ್ರತ್ಯಾಗಮ false;
                  ಯದಿ (ಸಾಲು_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ಯಾವತ್ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              ಸಾಲು--;
              ಸಾಲು->text = ಸಾಲು_start;
              ಸಾಲು->length = ptr - ಸಾಲು_start;
              mergesize = MAX (mergesize, ಸಾಲು->length);
              avail -= ಸಾಲು_bytes;

              ಯದಿ (ಕೀಲಿ)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  ಸಾಲು->keylim = (ಕೀಲಿ->eword == SIZE_MAX
                                  ? p
                                  : limfield (ಸಾಲು, ಕೀಲಿ));

                  ಯದಿ (ಕೀಲಿ->sword != SIZE_MAX)
                    ಸಾಲು->keybeg = begfield (ಸಾಲು, ಕೀಲಿ);
                  ಅನ್ಯಥಾ
                    {
                      ಯದಿ (ಕೀಲಿ->skipsblanks)
                        ಯಾವತ್ (blanks[to_uchar (*ಸಾಲು_start)])
                          ಸಾಲು_start++;
                      ಸಾಲು->keybeg = ಸಾಲು_start;
                    }
                }

              ಸಾಲು_start = ptr;
            }

          ptr = ptrlim;
          ಯದಿ (buf->eof)
            ಭಙ್ಗ;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - ಸಾಲು;
      ಯದಿ (buf->nlines != 0)
        {
          ಯದಿ (! buf->eof && maybe_growbuf (buf, policy))
            ಅನುವರ್ತ;
          buf->ಎಡ = ptr - ಸಾಲು_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          ಪ್ರತ್ಯಾಗಮ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t ಸಾಲು_alloc = buf->alloc / ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು);
        buf->buf = xpalloc (buf->buf, &ಸಾಲು_alloc, 1, -1, ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು));
        buf->alloc = ಸಾಲು_alloc * ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ unit_order[UCHAR_LIM] =
  {
#ಯದಿ ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ಅನ್ಯಥಾ
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
#endif
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ಸ್ಥಿರ ವರ್ಣ
traverse_raw_ಸಂಖ್ಯೆ (ವರ್ಣ ಧ್ರುವ **ಸಂಖ್ಯೆ)
{
  ವರ್ಣ ಧ್ರುವ *p = *ಸಂಖ್ಯೆ;
  ವರ್ಣ ch;
  ವರ್ಣ max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ಯಾವತ್ (c_isdigit (ch = *p++))
    {
      ಯದಿ (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      ಯದಿ (ends_with_thousands_sep)
        ++p;
    }

  ಯದಿ (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *ಸಂಖ್ಯೆ = p - 2;
      ಪ್ರತ್ಯಾಗಮ max_digit;
    }

  ಯದಿ (ch == decimal_point)
    ಯಾವತ್ (c_isdigit (ch = *p++))
      ಯದಿ (max_digit < ch)
        max_digit = ch;

  *ಸಂಖ್ಯೆ = p - 1;
  ಪ್ರತ್ಯಾಗಮ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
find_unit_order (ವರ್ಣ ಧ್ರುವ *ಸಂಖ್ಯೆ)
{
  bool minus_sign = (*ಸಂಖ್ಯೆ == '-');
  ವರ್ಣ ಧ್ರುವ *p = ಸಂಖ್ಯೆ + minus_sign;
  ವರ್ಣ max_digit = traverse_raw_ಸಂಖ್ಯೆ (&p);
  ಯದಿ ('0' < max_digit)
    {
      unsigned ವರ್ಣ ch = *p;
      ಪೂರ್ಣಾಙ್ಕ order = unit_order[ch];
      ಪ್ರತ್ಯಾಗಮ (minus_sign ? -order : order);
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
human_numcompare (ವರ್ಣ ಧ್ರುವ *a, ವರ್ಣ ಧ್ರುವ *b)
{
  ಯಾವತ್ (blanks[to_uchar (*a)])
    a++;
  ಯಾವತ್ (blanks[to_uchar (*b)])
    b++;

  ಪೂರ್ಣಾಙ್ಕ diff = find_unit_order (a) - find_unit_order (b);
  ಪ್ರತ್ಯಾಗಮ (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
numcompare (ವರ್ಣ ಧ್ರುವ *a, ವರ್ಣ ಧ್ರುವ *b)
{
  ಯಾವತ್ (blanks[to_uchar (*a)])
    a++;
  ಯಾವತ್ (blanks[to_uchar (*b)])
    b++;

  ಪ್ರತ್ಯಾಗಮ strnumcmp (a, b, decimal_point, thousands_sep);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
nan_compare (long ದ್ವಿಗುಣ a, long ದ್ವಿಗುಣ b)
{
  ವರ್ಣ buf[2][ಪರಿಮಾಣ "-nan""()" + CHAR_BIT * ಪರಿಮಾಣ a];
  snprintf (buf[0], ಪರಿಮಾಣ buf[0], "%Lf", a);
  snprintf (buf[1], ಪರಿಮಾಣ buf[1], "%Lf", b);
  ಪ್ರತ್ಯಾಗಮ strcmp (buf[0], buf[1]);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
general_numcompare (ವರ್ಣ ಧ್ರುವ *sa, ವರ್ಣ ಧ್ರುವ *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  ವರ್ಣ *ea;
  ವರ್ಣ *eb;
  long ದ್ವಿಗುಣ a = strtold (sa, &ea);
  long ದ್ವಿಗುಣ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ಯದಿ (sa == ea)
    ಪ್ರತ್ಯಾಗಮ sb == eb ? 0 : -1;
  ಯದಿ (sb == eb)
    ಪ್ರತ್ಯಾಗಮ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  ಪ್ರತ್ಯಾಗಮ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
getmonth (ವರ್ಣ ಧ್ರುವ *month, ವರ್ಣ **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  ಯಾವತ್ (blanks[to_uchar (*month)])
    month++;

  ಕುರು
    {
      size_t ix = (lo + hi) / 2;
      ವರ್ಣ ಧ್ರುವ *m = month;
      ವರ್ಣ ಧ್ರುವ *n = monthtab[ix].name;

      ಪ್ರತಿ (;; m++, n++)
        {
          ಯದಿ (!*n)
            {
              ಯದಿ (ea)
                *ea = (ವರ್ಣ *) m;
              ಪ್ರತ್ಯಾಗಮ monthtab[ix].val;
            }
          ಯದಿ (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              ಭಙ್ಗ;
            }
          ಅನ್ಯಥಾ ಯದಿ (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              ಭಙ್ಗ;
            }
        }
    }
  ಯಾವತ್ (lo < hi);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ಯದಿ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ಯದಿ 14 <= __GNUC__
#  pragma GCC diagnostic ತಳ್ಳು
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#ಅನ್ತರ್ಭಾವ "md5.h"

#ಯದಿ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ಯದಿ 14 <= __GNUC__
#  pragma GCC diagnostic ತೆಗೆ
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_PRIORITY_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ಸ್ಥಿರ ಶೂನ್ಯ
link_failure (ಶೂನ್ಯ)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ಸ್ಥಿರ ಶೂನ್ಯ *
ಚಿಹ್ನೆ_address (ಶೂನ್ಯ *handle, ವರ್ಣ ಧ್ರುವ *ಚಿಹ್ನೆ)
{
  ಶೂನ್ಯ *address = dlsym (handle, ಚಿಹ್ನೆ);
  ಯದಿ (!address)
    link_failure ();
  ಪ್ರತ್ಯಾಗಮ address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
ಸ್ಥಿರ ಶೂನ್ಯ
link_libcrypto (ಶೂನ್ಯ)
{
#ಯದಿ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  ಶೂನ್ಯ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ಯದಿ (!handle)
    link_failure ();
  ptr_MD5_Init = ಚಿಹ್ನೆ_address (handle, "MD5_Init");
  ptr_MD5_Update = ಚಿಹ್ನೆ_address (handle, "MD5_Update");
  ptr_MD5_Final = ಚಿಹ್ನೆ_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
ಸ್ಥಿರ ಸಂರಚನಾ md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ಸ್ಥಿರ ಶೂನ್ಯ
random_md5_state_init (ವರ್ಣ ಧ್ರುವ *random_source)
{
  unsigned ವರ್ಣ buf[MD5_DIGEST_SIZE];
  ಸಂರಚನಾ randread_source *r = randread_new (random_source, ಪರಿಮಾಣ buf);
  ಯದಿ (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, ಪರಿಮಾಣ buf);
  ಯದಿ (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, ಪರಿಮಾಣ buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ಸ್ಥಿರ size_t
xstrxfrm (ವರ್ಣ *restrict dest, ವರ್ಣ ಧ್ರುವ *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  ಯದಿ (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  ಪ್ರತ್ಯಾಗಮ translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_random (ವರ್ಣ *restrict texta, size_t lena,
                ವರ್ಣ *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  ಪೂರ್ಣಾಙ್ಕ xfrm_diff = 0;

  ವರ್ಣ stackbuf[4000];
  ವರ್ಣ *buf = stackbuf;
  size_t bufsize = ಪರಿಮಾಣ stackbuf;
  ಶೂನ್ಯ *allocated = ರಿಕ್ತ;
  uint32_t dig[2][MD5_DIGEST_SIZE / ಪರಿಮಾಣ (uint32_t)];
  ಸಂರಚನಾ md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ಯದಿ (hard_LC_COLLATE)
    {
      ವರ್ಣ ಧ್ರುವ *lima = texta + lena;
      ವರ್ಣ ಧ್ರುವ *limb = textb + lenb;

      ಯಾವತ್ (true)
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
          ಯದಿ (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              ಮುಕ್ತ (allocated);
              buf = allocated = malloc (bufsize);
              ಯದಿ (! buf)
                {
                  buf = stackbuf;
                  bufsize = ಪರಿಮಾಣ stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : ರಿಕ್ತ), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          ಯದಿ (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ಯದಿ (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              ಮುಕ್ತ (allocated);
              buf = allocated = xmalloc (bufsize);
              ಯದಿ (texta < lima)
                strxfrm (buf, texta, sizea);
              ಯದಿ (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ಯದಿ (texta < lima)
            texta += strlen (texta) + 1;
          ಯದಿ (textb < limb)
            textb += strlen (textb) + 1;
          ಯದಿ (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ಭಙ್ಗ;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ಯದಿ (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ಯದಿ (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  ಪೂರ್ಣಾಙ್ಕ diff = memcmp (dig[0], dig[1], ಪರಿಮಾಣ dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ಯದಿ (! diff)
    {
      ಯದಿ (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ಯದಿ (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  ಮುಕ್ತ (allocated);

  ಪ್ರತ್ಯಾಗಮ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ಸ್ಥಿರ size_t
debug_width (ವರ್ಣ ಧ್ರುವ *text, ವರ್ಣ ಧ್ರುವ *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  ಯಾವತ್ (text < lim)
    width += (*text++ == '\t');
  ಪ್ರತ್ಯಾಗಮ width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mark_ಕೀಲಿ (size_t offset, size_t width)
{
  ಯಾವತ್ (offset--)
    putchar (' ');

  ಯದಿ (!width)
    printf (_("^ no match for key\n"));
  ಅನ್ಯಥಾ
    {
      ಕುರು
        putchar ('_');
      ಯಾವತ್ (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
ಕೀಲಿ_numeric (ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ)
{
  ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ->numeric || ಕೀಲಿ->general_numeric || ಕೀಲಿ->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ಸ್ಥಿರ ಶೂನ್ಯ
debug_ಕೀಲಿ (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ)
{
  ವರ್ಣ *text = ಸಾಲು->text;
  ವರ್ಣ *beg = text;
  ವರ್ಣ *lim = text + ಸಾಲು->length - 1;

  ಯದಿ (ಕೀಲಿ)
    {
      ಯದಿ (ಕೀಲಿ->sword != SIZE_MAX)
        beg = begfield (ಸಾಲು, ಕೀಲಿ);
      ಯದಿ (ಕೀಲಿ->eword != SIZE_MAX)
        {
          lim = limfield (ಸಾಲು, ಕೀಲಿ);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ಯದಿ ((ಕೀಲಿ->skipsblanks && ಕೀಲಿ->sword == SIZE_MAX)
          || ಕೀಲಿ->month || ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          ವರ್ಣ saved = *lim;
          *lim = '\0';

          ಯಾವತ್ (blanks[to_uchar (*beg)])
            beg++;

          ವರ್ಣ *tighter_lim = beg;

          ಯದಿ (lim < beg)
            tighter_lim = lim;
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->month)
            getmonth (beg, &tighter_lim);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->general_numeric)
            ignore_ಮೌಲ್ಯ (strtold (beg, &tighter_lim));
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->numeric || ಕೀಲಿ->human_numeric)
            {
              ವರ್ಣ ಧ್ರುವ *p = beg + (beg < lim && *beg == '-');
              ವರ್ಣ max_digit = traverse_raw_ಸಂಖ್ಯೆ (&p);
              ಯದಿ ('0' <= max_digit)
                {
                  unsigned ವರ್ಣ ch = *p;
                  tighter_lim = (ವರ್ಣ *) p
                    + (ಕೀಲಿ->human_numeric && unit_order[ch]);
                }
            }
          ಅನ್ಯಥಾ
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_ಕೀಲಿ (offset, width);
}

/* Debug LINE by underlining its keys.  */

ಸ್ಥಿರ ಶೂನ್ಯ
debug_ಸಾಲು (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು)
{
  ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ = keylist;

  ಕುರು
    debug_ಕೀಲಿ (ಸಾಲು, ಕೀಲಿ);
  ಯಾವತ್ (ಕೀಲಿ && ((ಕೀಲಿ = ಕೀಲಿ->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ಸ್ಥಿರ bool
default_ಕೀಲಿ_compare (ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ)
{
  ಪ್ರತ್ಯಾಗಮ ! (ಕೀಲಿ->ignore
            || ಕೀಲಿ->translate
            || ಕೀಲಿ->skipsblanks
            || ಕೀಲಿ->skipeblanks
            || ಕೀಲಿ_numeric (ಕೀಲಿ)
            || ಕೀಲಿ->month
            || ಕೀಲಿ->version
            || ಕೀಲಿ->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೀಲಿ_to_opts (ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ, ವರ್ಣ *opts)
{
  ಯದಿ (ಕೀಲಿ->skipsblanks || ಕೀಲಿ->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ಯದಿ (ಕೀಲಿ->ignore == nondictionary)
    *opts++ = 'd';
  ಯದಿ (ಕೀಲಿ->translate)
    *opts++ = 'f';
  ಯದಿ (ಕೀಲಿ->general_numeric)
    *opts++ = 'g';
  ಯದಿ (ಕೀಲಿ->human_numeric)
    *opts++ = 'h';
  ಯದಿ (ಕೀಲಿ->ignore == nonprinting)
    *opts++ = 'i';
  ಯದಿ (ಕೀಲಿ->month)
    *opts++ = 'M';
  ಯದಿ (ಕೀಲಿ->numeric)
    *opts++ = 'n';
  ಯದಿ (ಕೀಲಿ->random)
    *opts++ = 'R';
  ಯದಿ (ಕೀಲಿ->reverse)
    *opts++ = 'r';
  ಯದಿ (ಕೀಲಿ->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೀಲಿ_warnings (ಸಂರಚನಾ keyfield ಧ್ರುವ *gkey, bool gkey_only)
{
  ಸಂರಚನಾ keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  ಪ್ರತಿ (ಸಂರಚನಾ keyfield *ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->next, keynum++)
    {
      ಯದಿ (ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          ಯದಿ (ಕೀಲಿ->general_numeric)
            general_numeric_field = true;
          ಅನ್ಯಥಾ
            basic_numeric_field = true;
        }

      ಯದಿ (ಕೀಲಿ->traditional_used)
        {
          size_t sword = ಕೀಲಿ->sword;
          size_t eword = ಕೀಲಿ->eword;
          ವರ್ಣ tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          ವರ್ಣ obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          ವರ್ಣ nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          ವರ್ಣ *po = obuf;
          ವರ್ಣ *pn = nbuf;

          ಯದಿ (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ಯದಿ (ಕೀಲಿ->eword != SIZE_MAX)
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
      bool zero_width = ಕೀಲಿ->sword != SIZE_MAX && ಕೀಲಿ->eword < ಕೀಲಿ->sword;
      ಯದಿ (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ಕೀಲಿ_numeric (ಕೀಲಿ) || ಕೀಲಿ->month;
      bool ಸಾಲು_offset = ಕೀಲಿ->eword == 0 && ಕೀಲಿ->echar != 0; /* -k1.x,1.y  */
      ಯದಿ (!zero_width && !gkey_only && tab == TAB_DEFAULT && !ಸಾಲು_offset
          && ((!ಕೀಲಿ->skipsblanks && !implicit_skip)
              || (!ಕೀಲಿ->skipsblanks && ಕೀಲಿ->schar)
              || (!ಕೀಲಿ->skipeblanks && ಕೀಲಿ->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ಯದಿ (!gkey_only && ಕೀಲಿ_numeric (ಕೀಲಿ))
        {
          size_t sword = ಕೀಲಿ->sword + 1;
          size_t eword = ಕೀಲಿ->eword + 1;
          ಯದಿ (!sword)
            sword++;
          ಯದಿ (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ಯದಿ (ಕೀಲಿ->general_numeric)
                general_numeric_field_span = true;
              ಅನ್ಯಥಾ
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ಯದಿ (ugkey.ignore && (ugkey.ignore == ಕೀಲಿ->ignore))
        ugkey.ignore = ರಿಕ್ತ;
      ಯದಿ (ugkey.translate && (ugkey.translate == ಕೀಲಿ->translate))
        ugkey.translate = ರಿಕ್ತ;
      ugkey.skipsblanks &= !ಕೀಲಿ->skipsblanks;
      ugkey.skipeblanks &= !ಕೀಲಿ->skipeblanks;
      ugkey.month &= !ಕೀಲಿ->month;
      ugkey.numeric &= !ಕೀಲಿ->numeric;
      ugkey.general_numeric &= !ಕೀಲಿ->general_numeric;
      ugkey.human_numeric &= !ಕೀಲಿ->human_numeric;
      ugkey.random &= !ಕೀಲಿ->random;
      ugkey.version &= !ಕೀಲಿ->version;
      ugkey.reverse &= !ಕೀಲಿ->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool ಸಂಖ್ಯೆ_locale_warned = false;
  ಯದಿ (basic_numeric_field_span)
    {
      ಯದಿ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((ವರ್ಣ []) {thousands_sep, 0})));
          ಸಂಖ್ಯೆ_locale_warned = true;
        }
    }
  ಯದಿ (basic_numeric_field_span || general_numeric_field_span)
    {
      ಯದಿ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((ವರ್ಣ []) {decimal_point, 0})));
          ಸಂಖ್ಯೆ_locale_warned = true;
        }
      ಅನ್ಯಥಾ ಯದಿ (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((ವರ್ಣ []) {tab, 0})));
        }
      ಅನ್ಯಥಾ ಯದಿ (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((ವರ್ಣ []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ಯದಿ ((basic_numeric_field || general_numeric_field) && ! ಸಂಖ್ಯೆ_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((ವರ್ಣ []) {decimal_point, 0})));

    }

  ಯದಿ (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ಯದಿ (!default_ಕೀಲಿ_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      ಯದಿ (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      ವರ್ಣ opts[ಪರಿಮಾಣ short_options];
      ಕೀಲಿ_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  ಯದಿ (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
diff_reversed (ಪೂರ್ಣಾಙ್ಕ diff, bool reversed)
{
  ಪ್ರತ್ಯಾಗಮ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
keycompare (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *a, ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *b)
{
  ಸಂರಚನಾ keyfield *ಕೀಲಿ = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  ವರ್ಣ *texta = a->keybeg;
  ವರ್ಣ *textb = b->keybeg;
  ವರ್ಣ *lima = a->keylim;
  ವರ್ಣ *limb = b->keylim;

  ಪೂರ್ಣಾಙ್ಕ diff;

  ಯಾವತ್ (true)
    {
      ವರ್ಣ ಧ್ರುವ *translate = ಕೀಲಿ->translate;
      bool ಧ್ರುವ *ignore = ಕೀಲಿ->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ಯದಿ (hard_LC_COLLATE || ಕೀಲಿ_numeric (ಕೀಲಿ)
          || ಕೀಲಿ->month || ಕೀಲಿ->random || ಕೀಲಿ->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          ವರ್ಣ *ta = texta;
          ವರ್ಣ *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          ವರ್ಣ enda = ta[tlena];
          ವರ್ಣ endb = tb[tlenb];

          ಶೂನ್ಯ *allocated = ರಿಕ್ತ;
          ವರ್ಣ stackbuf[4000];

          ಯದಿ (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              ಯದಿ (size <= ಪರಿಮಾಣ stackbuf)
                ta = stackbuf;
              ಅನ್ಯಥಾ
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              ಪ್ರತಿ (tlena = i = 0; i < lena; i++)
                ಯದಿ (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              ಪ್ರತಿ (tlenb = i = 0; i < lenb; i++)
                ಯದಿ (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ಯದಿ (ಕೀಲಿ->numeric)
            diff = numcompare (ta, tb);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->general_numeric)
            diff = general_numcompare (ta, tb);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->human_numeric)
            diff = human_numcompare (ta, tb);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->month)
            diff = getmonth (ta, ರಿಕ್ತ) - getmonth (tb, ರಿಕ್ತ);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ಅನ್ಯಥಾ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ಯದಿ (tlena == 0)
                diff = - NONZERO (tlenb);
              ಅನ್ಯಥಾ ಯದಿ (tlenb == 0)
                diff = 1;
              ಅನ್ಯಥಾ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          ಮುಕ್ತ (allocated);
        }
      ಅನ್ಯಥಾ ಯದಿ (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  ಕುರು									\
    {									\
          ಯಾವತ್ (true)							\
            {								\
              ಯಾವತ್ (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              ಯಾವತ್ (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              ಯದಿ (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ಭಙ್ಗ;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              ಯದಿ (diff)							\
                ಭಙ್ಗ;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ಯಾವತ್ (0)

          ಯದಿ (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          ಅನ್ಯಥಾ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ಅನ್ಯಥಾ
        {
          size_t lenmin = MIN (lena, lenb);
          ಯದಿ (lenmin == 0)
            diff = 0;
          ಅನ್ಯಥಾ ಯದಿ (translate)
            {
              size_t i = 0;
              ಕುರು
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  ಯದಿ (diff)
                    ಭಙ್ಗ;
                  i++;
                }
              ಯಾವತ್ (i < lenmin);
            }
          ಅನ್ಯಥಾ
            diff = memcmp (texta, textb, lenmin);

          ಯದಿ (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ಯದಿ (diff)
        ಭಙ್ಗ;

      ಕೀಲಿ = ಕೀಲಿ->next;
      ಯದಿ (! ಕೀಲಿ)
        ಪ್ರತ್ಯಾಗಮ 0;

      /* Find the beginning and limit of the next field.  */
      ಯದಿ (ಕೀಲಿ->eword != SIZE_MAX)
        lima = limfield (a, ಕೀಲಿ), limb = limfield (b, ಕೀಲಿ);
      ಅನ್ಯಥಾ
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      ಯದಿ (ಕೀಲಿ->sword != SIZE_MAX)
        texta = begfield (a, ಕೀಲಿ), textb = begfield (b, ಕೀಲಿ);
      ಅನ್ಯಥಾ
        {
          texta = a->text, textb = b->text;
          ಯದಿ (ಕೀಲಿ->skipsblanks)
            {
              ಯಾವತ್ (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              ಯಾವತ್ (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ diff_reversed (diff, ಕೀಲಿ->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *a, ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *b)
{
  ಪೂರ್ಣಾಙ್ಕ diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ಯದಿ (keylist)
    {
      diff = keycompare (a, b);
      ಯದಿ (diff || unique || stable)
        ಪ್ರತ್ಯಾಗಮ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  ಯದಿ (alen == 0)
    diff = - NONZERO (blen);
  ಅನ್ಯಥಾ ಯದಿ (blen == 0)
    diff = 1;
  ಅನ್ಯಥಾ ಯದಿ (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  ಅನ್ಯಥಾ
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      ಯದಿ (!diff)
        diff = _GL_CMP (alen, blen);
    }

  ಪ್ರತ್ಯಾಗಮ diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಸಾಲು (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, FILE *fp, ವರ್ಣ ಧ್ರುವ *output_file)
{
  ವರ್ಣ *buf = ಸಾಲು->text;
  size_t n_bytes = ಸಾಲು->length;
  ವರ್ಣ *ebuf = buf + n_bytes;

  ಯದಿ (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      ವರ್ಣ ಧ್ರುವ *c = buf;

      ಯಾವತ್ (c < ebuf)
        {
          ವರ್ಣ wc = *c++;
          ಯದಿ (wc == '\t')
            wc = '>';
          ಅನ್ಯಥಾ ಯದಿ (c == ebuf)
            wc = '\n';
          ಯದಿ (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_ಸಾಲು (ಸಾಲು);
    }
  ಅನ್ಯಥಾ
    {
      ebuf[-1] = eolchar;
      ಯದಿ (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ಸ್ಥಿರ bool
check (ವರ್ಣ ಧ್ರುವ *file_name, ವರ್ಣ checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ಸಂರಚನಾ buffer buf;		/* Input buffer. */
  ಸಂರಚನಾ ಸಾಲು temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t ಸಾಲು_ಸಂಖ್ಯೆ = 0;
  ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು),
           MAX (merge_buffer_size, sort_size));
  temp.text = ರಿಕ್ತ;

  ಯಾವತ್ (fillbuf (&buf, fp, file_name, ರಿಕ್ತ))
    {
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು = buffer_linelim (&buf);
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *linebase = ಸಾಲು - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ಯದಿ (alloc && nonunique <= compare (&temp, ಸಾಲು - 1))
        {
        found_disorder:
          {
            ಯದಿ (checkonly == 'c')
              {
                ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *disorder_ಸಾಲು = ಸಾಲು - 1;
                uintmax_t disorder_ಸಾಲು_ಸಂಖ್ಯೆ =
                  buffer_linelim (&buf) - disorder_ಸಾಲು + ಸಾಲು_ಸಂಖ್ಯೆ;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_ಸಾಲು_ಸಂಖ್ಯೆ);
                write_ಸಾಲು (disorder_ಸಾಲು, stderr, _("standard error"));
              }

            ordered = false;
            ಭಙ್ಗ;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ಯಾವತ್ (linebase < --ಸಾಲು)
        ಯದಿ (nonunique <= compare (ಸಾಲು, ಸಾಲು - 1))
          ಗಚ್ಛ found_disorder;

      ಸಾಲು_ಸಂಖ್ಯೆ += buf.nlines;

      /* Save the last line of the buffer.  */
      ಯದಿ (alloc < ಸಾಲು->length)
        {
          ಕುರು
            {
              alloc *= 2;
              ಯದಿ (! alloc)
                {
                  alloc = ಸಾಲು->length;
                  ಭಙ್ಗ;
                }
            }
          ಯಾವತ್ (alloc < ಸಾಲು->length);

          ಮುಕ್ತ (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, ಸಾಲು->text, ಸಾಲು->length);
      temp.length = ಸಾಲು->length;
      ಯದಿ (ಕೀಲಿ)
        {
          temp.keybeg = temp.text + (ಸಾಲು->keybeg - ಸಾಲು->text);
          temp.keylim = temp.text + (ಸಾಲು->keylim - ಸಾಲು->text);
        }
    }

  xfclose (fp, file_name);
  ಮುಕ್ತ (buf.buf);
  ಮುಕ್ತ (temp.text);
  ಪ್ರತ್ಯಾಗಮ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ಸ್ಥಿರ size_t
open_ಒಳಹರಿವು_files (ಸಂರಚನಾ sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, ಪರಿಮಾಣ *fps);
  ಪೂರ್ಣಾಙ್ಕ i;

  /* Open as many input files as we can.  */
  ಪ್ರತಿ (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      ಯದಿ (!fps[i])
        ಭಙ್ಗ;
    }

  ಪ್ರತ್ಯಾಗಮ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergefps (ಸಂರಚನಾ sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, ವರ್ಣ ಧ್ರುವ *output_file, FILE **fps)
{
  ಸಂರಚನಾ buffer *buffer = xnmalloc (nfiles, ಪರಿಮಾಣ *buffer);
                                /* Input buffers for each file. */
  ಸಂರಚನಾ ಸಾಲು saved;		/* Saved line storage for unique check. */
  ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *savedline = ರಿಕ್ತ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ **cur = xnmalloc (nfiles, ಪರಿಮಾಣ *cur);
                                /* Current line in each line table. */
  ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ **base = xnmalloc (nfiles, ಪರಿಮಾಣ *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, ಪರಿಮಾಣ *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ಸಂರಚನಾ keyfield ಧ್ರುವ *ಕೀಲಿ = keylist;
  saved.text = ರಿಕ್ತ;

  /* Read initial lines from each input file. */
  ಪ್ರತಿ (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು),
               MAX (merge_buffer_size, sort_size / nfiles));
      ಯದಿ (fillbuf (&buffer[i], fps[i], files[i].name, ರಿಕ್ತ))
        {
          ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      ಅನ್ಯಥಾ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ಯದಿ (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          ಮುಕ್ತ (buffer[i].buf);
          --nfiles;
          ಪ್ರತಿ (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  ಪ್ರತಿ (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  ಪ್ರತಿ (size_t i = 1; i < nfiles; ++i)
    ಯದಿ (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ಯಾವತ್ (nfiles)
    {
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ಯದಿ (unique)
        {
          ಯದಿ (savedline && compare (savedline, smallest))
            {
              savedline = ರಿಕ್ತ;
              write_ಸಾಲು (&saved, ofp, output_file);
            }
          ಯದಿ (!savedline)
            {
              savedline = &saved;
              ಯದಿ (savealloc < smallest->length)
                {
                  ಕುರು
                    ಯದಿ (! savealloc)
                      {
                        savealloc = smallest->length;
                        ಭಙ್ಗ;
                      }
                  ಯಾವತ್ ((savealloc *= 2) < smallest->length);

                  ಮುಕ್ತ (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              ಯದಿ (ಕೀಲಿ)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      ಅನ್ಯಥಾ
        write_ಸಾಲು (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ಯದಿ (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ರಿಕ್ತ))
            {
              ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          ಅನ್ಯಥಾ
            {
              /* We reached EOF on fps[ord[0]].  */
              ಪ್ರತಿ (size_t i = 1; i < nfiles; ++i)
                ಯದಿ (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ಯದಿ (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              ಮುಕ್ತ (buffer[ord[0]].buf);
              ಪ್ರತಿ (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              ಪ್ರತಿ (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ಅನುವರ್ತ;
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
        size_t ಎಣಿಕೆ_of_smaller_ಸಾಲುಗಳು;

        ಯಾವತ್ (lo < hi)
          {
            ಪೂರ್ಣಾಙ್ಕ cmp = compare (cur[ord0], cur[ord[probe]]);
            ಯದಿ (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ಅನ್ಯಥಾ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ಎಣಿಕೆ_of_smaller_ಸಾಲುಗಳು = lo - 1;
        ಪ್ರತಿ (size_t j = 0; j < ಎಣಿಕೆ_of_smaller_ಸಾಲುಗಳು; j++)
          ord[j] = ord[j + 1];
        ord[ಎಣಿಕೆ_of_smaller_ಸಾಲುಗಳು] = ord0;
      }
    }

  ಯದಿ (unique && savedline)
    {
      write_ಸಾಲು (&saved, ofp, output_file);
      ಮುಕ್ತ (saved.text);
    }

  xfclose (ofp, output_file);
  ಮುಕ್ತ (fps);
  ಮುಕ್ತ (buffer);
  ಮುಕ್ತ (ord);
  ಮುಕ್ತ (base);
  ಮುಕ್ತ (cur);
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
mergefiles (ಸಂರಚನಾ sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, ವರ್ಣ ಧ್ರುವ *output_file)
{
  FILE **fps;
  size_t nopened = open_ಒಳಹರಿವು_files (files, nfiles, &fps);
  ಯದಿ (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  ಪ್ರತ್ಯಾಗಮ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergelines (ಸಂರಚನಾ ಸಾಲು *restrict t, size_t nlines,
            ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ಸಂರಚನಾ ಸಾಲು *hi = t - nlo;

  ಯಾವತ್ (true)
    ಯದಿ (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ಯದಿ (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            ಪ್ರತ್ಯಾಗಮ;
          }
      }
    ಅನ್ಯಥಾ
      {
        *--t = *--hi;
        ಯದಿ (! --nhi)
          {
            ಕುರು
              *--t = *--lo;
            ಯಾವತ್ (--nlo);

            ಪ್ರತ್ಯಾಗಮ;
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
sequential_sort (ಸಂರಚನಾ ಸಾಲು *restrict ಸಾಲುಗಳು, size_t nlines,
                 ಸಂರಚನಾ ಸಾಲು *restrict temp, bool to_temp)
{
  ಯದಿ (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      ಪೂರ್ಣಾಙ್ಕ swap = (0 < compare (&ಸಾಲುಗಳು[-1], &ಸಾಲುಗಳು[-2]));
      ಯದಿ (to_temp)
        {
          temp[-1] = ಸಾಲುಗಳು[-1 - swap];
          temp[-2] = ಸಾಲುಗಳು[-2 + swap];
        }
      ಅನ್ಯಥಾ ಯದಿ (swap)
        {
          temp[-1] = ಸಾಲುಗಳು[-1];
          ಸಾಲುಗಳು[-1] = ಸಾಲುಗಳು[-2];
          ಸಾಲುಗಳು[-2] = temp[-1];
        }
    }
  ಅನ್ಯಥಾ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ಸಂರಚನಾ ಸಾಲು *lo = ಸಾಲುಗಳು;
      ಸಂರಚನಾ ಸಾಲು *hi = ಸಾಲುಗಳು - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      ಯದಿ (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      ಅನ್ಯಥಾ ಯದಿ (!to_temp)
        temp[-1] = lo[-1];

      ಸಂರಚನಾ ಸಾಲು *dest;
      ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *sorted_lo;
      ಯದಿ (to_temp)
        {
          dest = temp;
          sorted_lo = ಸಾಲುಗಳು;
        }
      ಅನ್ಯಥಾ
        {
          dest = ಸಾಲುಗಳು;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ಸ್ಥಿರ ಸಂರಚನಾ merge_ಗಂಟು *init_ಗಂಟು (ಸಂರಚನಾ merge_ಗಂಟು *restrict,
                                     ಸಂರಚನಾ merge_ಗಂಟು *restrict,
                                     ಸಂರಚನಾ ಸಾಲು *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ಸ್ಥಿರ ಸಂರಚನಾ merge_ಗಂಟು *
merge_ವೃಕ್ಷ_init (size_t nthreads, size_t nlines, ಸಂರಚನಾ ಸಾಲು *dest)
{
  ಸಂರಚನಾ merge_ಗಂಟು *merge_ವೃಕ್ಷ = xmalloc (2 * ಪರಿಮಾಣ *merge_ವೃಕ್ಷ * nthreads);

  ಸಂರಚನಾ merge_ಗಂಟು *ಬೇರು = merge_ವೃಕ್ಷ;
  ಬೇರು->lo = ಬೇರು->hi = ಬೇರು->end_lo = ಬೇರು->end_hi = ರಿಕ್ತ;
  ಬೇರು->dest = ರಿಕ್ತ;
  ಬೇರು->nlo = ಬೇರು->nhi = nlines;
  ಬೇರು->parent = ರಿಕ್ತ;
  ಬೇರು->level = MERGE_END;
  ಬೇರು->queued = false;
  pthread_mutex_init (&ಬೇರು->lock, ರಿಕ್ತ);

  init_ಗಂಟು (ಬೇರು, ಬೇರು + 1, dest, nthreads, nlines, false);
  ಪ್ರತ್ಯಾಗಮ merge_ವೃಕ್ಷ;
}

/* Destroy the merge tree. */
ಸ್ಥಿರ ಶೂನ್ಯ
merge_ವೃಕ್ಷ_destroy (size_t nthreads, ಸಂರಚನಾ merge_ಗಂಟು *merge_ವೃಕ್ಷ)
{
  size_t n_nodes = nthreads * 2;
  ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು = merge_ವೃಕ್ಷ;

  ಯಾವತ್ (n_nodes--)
    {
      pthread_mutex_destroy (&ಗಂಟು->lock);
      ಗಂಟು++;
    }

  ಮುಕ್ತ (merge_ವೃಕ್ಷ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ಸ್ಥಿರ ಸಂರಚನಾ merge_ಗಂಟು *
init_ಗಂಟು (ಸಂರಚನಾ merge_ಗಂಟು *restrict parent,
           ಸಂರಚನಾ merge_ಗಂಟು *restrict ಗಂಟು_pool,
           ಸಂರಚನಾ ಸಾಲು *dest, size_t nthreads,
           size_t total_ಸಾಲುಗಳು, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ಸಂರಚನಾ ಸಾಲು *lo = dest - total_ಸಾಲುಗಳು;
  ಸಂರಚನಾ ಸಾಲು *hi = lo - nlo;
  ಸಂರಚನಾ ಸಾಲು **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು = ಗಂಟು_pool++;
  ಗಂಟು->lo = ಗಂಟು->end_lo = lo;
  ಗಂಟು->hi = ಗಂಟು->end_hi = hi;
  ಗಂಟು->dest = parent_end;
  ಗಂಟು->nlo = nlo;
  ಗಂಟು->nhi = nhi;
  ಗಂಟು->parent = parent;
  ಗಂಟು->level = parent->level + 1;
  ಗಂಟು->queued = false;
  pthread_mutex_init (&ಗಂಟು->lock, ರಿಕ್ತ);

  ಯದಿ (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      ಗಂಟು->lo_child = ಗಂಟು_pool;
      ಗಂಟು_pool = init_ಗಂಟು (ಗಂಟು, ಗಂಟು_pool, lo, lo_threads,
                             total_ಸಾಲುಗಳು, true);
      ಗಂಟು->hi_child = ಗಂಟು_pool;
      ಗಂಟು_pool = init_ಗಂಟು (ಗಂಟು, ಗಂಟು_pool, hi, hi_threads,
                             total_ಸಾಲುಗಳು, false);
    }
  ಅನ್ಯಥಾ
    {
      ಗಂಟು->lo_child = ರಿಕ್ತ;
      ಗಂಟು->hi_child = ರಿಕ್ತ;
    }
  ಪ್ರತ್ಯಾಗಮ ಗಂಟು_pool;
}


/* Compare two merge nodes A and B for priority.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_nodes (ಶೂನ್ಯ ಧ್ರುವ *a, ಶೂನ್ಯ ಧ್ರುವ *b)
{
  ಸಂರಚನಾ merge_ಗಂಟು ಧ್ರುವ *nodea = a;
  ಸಂರಚನಾ merge_ಗಂಟು ಧ್ರುವ *nodeb = b;
  ಯದಿ (nodea->level == nodeb->level)
      ಪ್ರತ್ಯಾಗಮ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  ಪ್ರತ್ಯಾಗಮ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
lock_ಗಂಟು (ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_lock (&ಗಂಟು->lock);
}

/* Unlock a merge tree NODE. */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
unlock_ಗಂಟು (ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_unlock (&ಗಂಟು->lock);
}

/* Destroy merge QUEUE. */

ಸ್ಥಿರ ಶೂನ್ಯ
queue_destroy (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ಸ್ಥಿರ ಶೂನ್ಯ
queue_init (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, ರಿಕ್ತ);
  pthread_cond_init (&queue->cond, ರಿಕ್ತ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ಸ್ಥಿರ ಶೂನ್ಯ
queue_ಸೇರಿಸು (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue, ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು)
{
  pthread_mutex_lock (&queue->mutex);
  heap_ಸೇರಿಸು (queue->priority_queue, ಗಂಟು);
  ಗಂಟು->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ಸ್ಥಿರ ಸಂರಚನಾ merge_ಗಂಟು *
queue_ತೆಗೆ (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue)
{
  ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು;
  pthread_mutex_lock (&queue->mutex);
  ಯಾವತ್ (! (ಗಂಟು = heap_remove_ತುದಿ (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_ಗಂಟು (ಗಂಟು);
  ಗಂಟು->queued = false;
  ಪ್ರತ್ಯಾಗಮ ಗಂಟು;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ಸ್ಥಿರ ಶೂನ್ಯ
write_unique (ಸಂರಚನಾ ಸಾಲು ಧ್ರುವ *ಸಾಲು, FILE *tfp, ವರ್ಣ ಧ್ರುವ *temp_output)
{
  ಯದಿ (unique)
    {
      ಯದಿ (saved_ಸಾಲು.text && ! compare (ಸಾಲು, &saved_ಸಾಲು))
        ಪ್ರತ್ಯಾಗಮ;
      saved_ಸಾಲು = *ಸಾಲು;
    }

  write_ಸಾಲು (ಸಾಲು, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
mergelines_ಗಂಟು (ಸಂರಚನಾ merge_ಗಂಟು *restrict ಗಂಟು, size_t total_ಸಾಲುಗಳು,
                 FILE *tfp, ವರ್ಣ ಧ್ರುವ *temp_output)
{
  ಸಂರಚನಾ ಸಾಲು *lo_orig = ಗಂಟು->lo;
  ಸಂರಚನಾ ಸಾಲು *hi_orig = ಗಂಟು->hi;
  size_t to_merge = MAX_MERGE (total_ಸಾಲುಗಳು, ಗಂಟು->level);
  size_t merged_lo;
  size_t merged_hi;

  ಯದಿ (ಗಂಟು->level > MERGE_ಬೇರು)
    {
      /* Merge to destination buffer. */
      ಸಂರಚನಾ ಸಾಲು *dest = *ಗಂಟು->dest;
      ಯಾವತ್ (ಗಂಟು->lo != ಗಂಟು->end_lo && ಗಂಟು->hi != ಗಂಟು->end_hi && to_merge--)
        ಯದಿ (compare (ಗಂಟು->lo - 1, ಗಂಟು->hi - 1) <= 0)
          *--dest = *--ಗಂಟು->lo;
        ಅನ್ಯಥಾ
          *--dest = *--ಗಂಟು->hi;

      merged_lo = lo_orig - ಗಂಟು->lo;
      merged_hi = hi_orig - ಗಂಟು->hi;

      ಯದಿ (ಗಂಟು->nhi == merged_hi)
        ಯಾವತ್ (ಗಂಟು->lo != ಗಂಟು->end_lo && to_merge--)
          *--dest = *--ಗಂಟು->lo;
      ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->nlo == merged_lo)
        ಯಾವತ್ (ಗಂಟು->hi != ಗಂಟು->end_hi && to_merge--)
          *--dest = *--ಗಂಟು->hi;
      *ಗಂಟು->dest = dest;
    }
  ಅನ್ಯಥಾ
    {
      /* Merge directly to output. */
      ಯಾವತ್ (ಗಂಟು->lo != ಗಂಟು->end_lo && ಗಂಟು->hi != ಗಂಟು->end_hi && to_merge--)
        {
          ಯದಿ (compare (ಗಂಟು->lo - 1, ಗಂಟು->hi - 1) <= 0)
            write_unique (--ಗಂಟು->lo, tfp, temp_output);
          ಅನ್ಯಥಾ
            write_unique (--ಗಂಟು->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - ಗಂಟು->lo;
      merged_hi = hi_orig - ಗಂಟು->hi;

      ಯದಿ (ಗಂಟು->nhi == merged_hi)
        {
          ಯಾವತ್ (ಗಂಟು->lo != ಗಂಟು->end_lo && to_merge--)
            write_unique (--ಗಂಟು->lo, tfp, temp_output);
        }
      ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->nlo == merged_lo)
        {
          ಯಾವತ್ (ಗಂಟು->hi != ಗಂಟು->end_hi && to_merge--)
            write_unique (--ಗಂಟು->hi, tfp, temp_output);
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
queue_check_ಸೇರಿಸು (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue, ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು)
{
  ಯದಿ (! ಗಂಟು->queued)
    {
      bool lo_avail = (ಗಂಟು->lo - ಗಂಟು->end_lo) != 0;
      bool hi_avail = (ಗಂಟು->hi - ಗಂಟು->end_hi) != 0;
      ಯದಿ (lo_avail ? hi_avail || ! ಗಂಟು->nhi : hi_avail && ! ಗಂಟು->nlo)
        queue_ಸೇರಿಸು (queue, ಗಂಟು);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ಸ್ಥಿರ ಶೂನ್ಯ
queue_check_ಸೇರಿಸು_parent (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue,
                           ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು)
{
  ಯದಿ (ಗಂಟು->level > MERGE_ಬೇರು)
    {
      lock_ಗಂಟು (ಗಂಟು->parent);
      queue_check_ಸೇರಿಸು (queue, ಗಂಟು->parent);
      unlock_ಗಂಟು (ಗಂಟು->parent);
    }
  ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->nlo + ಗಂಟು->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_ಸೇರಿಸು (queue, ಗಂಟು->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
merge_loop (ಸಂರಚನಾ merge_ಗಂಟು_queue *queue,
            size_t total_ಸಾಲುಗಳು, FILE *tfp, ವರ್ಣ ಧ್ರುವ *temp_output)
{
  ಯಾವತ್ (true)
    {
      ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು = queue_ತೆಗೆ (queue);

      ಯದಿ (ಗಂಟು->level == MERGE_END)
        {
          unlock_ಗಂಟು (ಗಂಟು);
          /* Reinsert so other threads can pop it. */
          queue_ಸೇರಿಸು (queue, ಗಂಟು);
          ಭಙ್ಗ;
        }
      mergelines_ಗಂಟು (ಗಂಟು, total_ಸಾಲುಗಳು, tfp, temp_output);
      queue_check_ಸೇರಿಸು (queue, ಗಂಟು);
      queue_check_ಸೇರಿಸು_parent (queue, ಗಂಟು);

      unlock_ಗಂಟು (ಗಂಟು);
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ sortlines (ಸಂರಚನಾ ಸಾಲು *restrict, size_t, size_t,
                       ಸಂರಚನಾ merge_ಗಂಟು *, ಸಂರಚನಾ merge_ಗಂಟು_queue *,
                       FILE *, ವರ್ಣ ಧ್ರುವ *);

/* Thread arguments for sortlines_thread. */

ಸಂರಚನಾ thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ಸಂರಚನಾ ಸಾಲು *ಸಾಲುಗಳು;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ಧ್ರುವ total_ಸಾಲುಗಳು;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ಸಂರಚನಾ merge_ಗಂಟು *ಧ್ರುವ ಗಂಟು;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ಸಂರಚನಾ merge_ಗಂಟು_queue *ಧ್ರುವ queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  ವರ್ಣ ಧ್ರುವ *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ಸ್ಥಿರ ಶೂನ್ಯ *
sortlines_thread (ಶೂನ್ಯ *data)
{
  ಸಂರಚನಾ thread_args ಧ್ರುವ *args = data;
  sortlines (args->ಸಾಲುಗಳು, args->nthreads, args->total_ಸಾಲುಗಳು,
             args->ಗಂಟು, args->queue, args->tfp,
             args->output_temp);
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
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
sortlines (ಸಂರಚನಾ ಸಾಲು *restrict ಸಾಲುಗಳು, size_t nthreads,
           size_t total_ಸಾಲುಗಳು, ಸಂರಚನಾ merge_ಗಂಟು *ಗಂಟು,
           ಸಂರಚನಾ merge_ಗಂಟು_queue *queue, FILE *tfp, ವರ್ಣ ಧ್ರುವ *temp_output)
{
  size_t nlines = ಗಂಟು->nlo + ಗಂಟು->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  ಸಂರಚನಾ thread_args args = {ಸಾಲುಗಳು, lo_threads, total_ಸಾಲುಗಳು,
                             ಗಂಟು->lo_child, queue, tfp, temp_output};

  ಯದಿ (nthreads > 1 && SUBTHREAD_ಸಾಲುಗಳು_HEURISTIC <= nlines
      && pthread_create (&thread, ರಿಕ್ತ, sortlines_thread, &args) == 0)
    {
      sortlines (ಸಾಲುಗಳು - ಗಂಟು->nlo, hi_threads, total_ಸಾಲುಗಳು,
                 ಗಂಟು->hi_child, queue, tfp, temp_output);
      pthread_join (thread, ರಿಕ್ತ);
    }
  ಅನ್ಯಥಾ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ಗಂಟು->nlo;
      size_t nhi = ಗಂಟು->nhi;
      ಸಂರಚನಾ ಸಾಲು *temp = ಸಾಲುಗಳು - total_ಸಾಲುಗಳು;
      ಯದಿ (1 < nhi)
        sequential_sort (ಸಾಲುಗಳು - nlo, nhi, temp - nlo / 2, false);
      ಯದಿ (1 < nlo)
        sequential_sort (ಸಾಲುಗಳು, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      ಗಂಟು->lo = ಸಾಲುಗಳು;
      ಗಂಟು->hi = ಸಾಲುಗಳು - nlo;
      ಗಂಟು->end_lo = ಸಾಲುಗಳು - nlo;
      ಗಂಟು->end_hi = ಸಾಲುಗಳು - nlo - nhi;

      queue_ಸೇರಿಸು (queue, ಗಂಟು);
      merge_loop (queue, total_ಸಾಲುಗಳು, tfp, temp_output);
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
avoid_trashing_ಒಳಹರಿವು (ಸಂರಚನಾ sortfile *files, size_t ntemps,
                      size_t nfiles, ವರ್ಣ ಧ್ರುವ *outfile)
{
  ಸಂರಚನಾ tempnode *tempcopy = ರಿಕ್ತ;

  ಪ್ರತಿ (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ಸಂರಚನಾ stat instat;

      ಯದಿ (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ಅನ್ಯಥಾ
        {
          ಸಂರಚನಾ stat *outst = get_outstatus ();
          ಯದಿ (!outst)
            ಭಙ್ಗ;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ಯದಿ (same)
        {
          ಯದಿ (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_temp (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].temp = tempcopy;
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
check_inputs (ವರ್ಣ *ಧ್ರುವ *files, size_t nfiles)
{
  ಪ್ರತಿ (size_t i = 0; i < nfiles; i++)
    {
      ಯದಿ (streq (files[i], "-"))
        ಅನುವರ್ತ;

      ಯದಿ (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_output (ವರ್ಣ ಧ್ರುವ *outfile)
{
  ಯದಿ (outfile)
    {
      ಪೂರ್ಣಾಙ್ಕ oflags = O_WRONLY | O_ದ್ವಿಮಾನ | O_CLOEXEC | O_CREAT;
      ಪೂರ್ಣಾಙ್ಕ outfd = open (outfile, oflags, MODE_RW_UGO);
      ಯದಿ (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ಸ್ಥಿರ ಶೂನ್ಯ
merge (ಸಂರಚನಾ sortfile *files, size_t ntemps, size_t nfiles,
       ವರ್ಣ ಧ್ರುವ *output_file)
{
  ಯಾವತ್ (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t in;

      /* Number of output files generated so far.  */
      size_t out;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      ಪ್ರತಿ (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          ಸಂರಚನಾ tempnode *temp = create_temp (&tfp);
          size_t ಅಂಕೆ_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, ಅಂಕೆ_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += ಅಂಕೆ_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      ಯದಿ (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ಸಂರಚನಾ tempnode *temp = create_temp (&tfp);
          size_t ಅಂಕೆ_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, ಅಂಕೆ_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += ಅಂಕೆ_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * ಪರಿಮಾಣ *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_ಒಳಹರಿವು (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ಯಾವತ್ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ಒಳಹರಿವು_files (files, nfiles, &fps);

      ಯದಿ (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          ಯದಿ (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ಭಙ್ಗ;
            }
          ಯದಿ (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      ಅನ್ಯಥಾ ಯದಿ (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ಸಂರಚನಾ tempnode *temp;
      ಕುರು
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      ಯಾವತ್ (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * ಪರಿಮಾಣ *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ಸ್ಥಿರ ಶೂನ್ಯ
sort (ವರ್ಣ *ಧ್ರುವ *files, size_t nfiles, ವರ್ಣ ಧ್ರುವ *output_file,
      size_t nthreads)
{
  ಸಂರಚನಾ buffer buf;
  ಸಂರಚನಾ sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  ಯಾವತ್ (nfiles)
    {
      ವರ್ಣ ಧ್ರುವ *temp_output;
      ವರ್ಣ ಧ್ರುವ *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_ಸಾಲು;
      ಯದಿ (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ಯಾವತ್ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_ಸಾಲು = (mult * ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು));
        }
      ಅನ್ಯಥಾ
        bytes_per_ಸಾಲು = ಪರಿಮಾಣ (ಸಂರಚನಾ ಸಾಲು) * 3 / 2;

      ಯದಿ (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_ಸಾಲು,
                              &policy);
          initbuf (&buf, bytes_per_ಸಾಲು, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ಯಾವತ್ (fillbuf (&buf, fp, file, &policy))
        {
          ಸಂರಚನಾ ಸಾಲು *ಸಾಲು;

          ಯದಿ (buf.eof && nfiles
              && (bytes_per_ಸಾಲು + 1
                  < (buf.alloc - buf.used - bytes_per_ಸಾಲು * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ಎಡ = buf.used;
              ಭಙ್ಗ;
            }

          saved_ಸಾಲು.text = ರಿಕ್ತ;
          ಸಾಲು = buffer_linelim (&buf);
          ಯದಿ (buf.eof && !nfiles && !ntemps && !buf.ಎಡ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          ಅನ್ಯಥಾ
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          ಯದಿ (1 < buf.nlines)
            {
              ಯದಿ (nthreads > 1)
                {
                  ಸಂರಚನಾ merge_ಗಂಟು_queue queue;
                  queue_init (&queue, nthreads);
                  ಸಂರಚನಾ merge_ಗಂಟು *merge_ವೃಕ್ಷ =
                    merge_ವೃಕ್ಷ_init (nthreads, buf.nlines, ಸಾಲು);

                  sortlines (ಸಾಲು, nthreads, buf.nlines, merge_ವೃಕ್ಷ + 1,
                             &queue, tfp, temp_output);

                  merge_ವೃಕ್ಷ_destroy (nthreads, merge_ವೃಕ್ಷ);
                  queue_destroy (&queue);
                }
              ಅನ್ಯಥಾ
                {
                  sequential_sort (ಸಾಲು, buf.nlines,
                                   ಸಾಲು - buf.nlines, false);
                  ಪ್ರತಿ (size_t i = 0; i < buf.nlines; i++)
                    write_unique (ಸಾಲು - i - 1, tfp, temp_output);
                }
            }
          ಅನ್ಯಥಾ
            write_unique (ಸಾಲು - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          ಯದಿ (output_file_created)
            ಗಚ್ಛ finish;
        }
      xfclose (fp, file);
    }

 finish:
  ಮುಕ್ತ (buf.buf);

  ಯದಿ (! output_file_created)
    {
      ಸಂರಚನಾ tempnode *ಗಂಟು = temphead;
      ಸಂರಚನಾ sortfile *tempfiles = xnmalloc (ntemps, ಪರಿಮಾಣ *tempfiles);
      ಪ್ರತಿ (size_t i = 0; ಗಂಟು; i++)
        {
          tempfiles[i].name = ಗಂಟು->name;
          tempfiles[i].temp = ಗಂಟು;
          ಗಂಟು = ಗಂಟು->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      ಮುಕ್ತ (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ಸ್ಥಿರ ಶೂನ್ಯ
insertkey (ಸಂರಚನಾ keyfield *ಕೀಲಿ_arg)
{
  ಸಂರಚನಾ keyfield **p;
  ಸಂರಚನಾ keyfield *ಕೀಲಿ = xmemdup (ಕೀಲಿ_arg, ಪರಿಮಾಣ *ಕೀಲಿ);

  ಪ್ರತಿ (p = &keylist; *p; p = &(*p)->next)
    ಅನುವರ್ತ;
  *p = ಕೀಲಿ;
  ಕೀಲಿ->next = ರಿಕ್ತ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ಸ್ಥಿರ ಶೂನ್ಯ
badfieldspec (ವರ್ಣ ಧ್ರುವ *spec, ವರ್ಣ ಧ್ರುವ *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
incompatible_options (ವರ್ಣ ಧ್ರುವ *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ಸ್ಥಿರ ಶೂನ್ಯ
check_ordering_compatibility (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (ಸಂರಚನಾ keyfield *ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->next)
    ಯದಿ (1 < (ಕೀಲಿ->numeric + ಕೀಲಿ->general_numeric + ಕೀಲಿ->human_numeric
             + ಕೀಲಿ->month + (ಕೀಲಿ->version | ಕೀಲಿ->random | !!ಕೀಲಿ->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        ವರ್ಣ opts[ಪರಿಮಾಣ short_options];
        /* Clear flags we're not interested in.  */
        ಕೀಲಿ->skipsblanks = ಕೀಲಿ->skipeblanks = ಕೀಲಿ->reverse = false;
        ಕೀಲಿ_to_opts (ಕೀಲಿ, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *
parse_field_ಎಣಿಕೆ (ವರ್ಣ ಧ್ರುವ *string, size_t *val, ವರ್ಣ ಧ್ರುವ *msgid)
{
  ವರ್ಣ *suffix;
  uintmax_t n;

  ವಿಕಲ್ಪ (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    ಪಕ್ಷ LONGINT_OK:
    ಪಕ್ಷ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ಯದಿ (*val == n)
        ಭಙ್ಗ;
      FALLTHROUGH;
    ಪಕ್ಷ LONGINT_OVERFLOW:
    ಪಕ್ಷ LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ಭಙ್ಗ;

    ಪಕ್ಷ LONGINT_INVALID:
      ಯದಿ (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ಪ್ರತ್ಯಾಗಮ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ಸ್ಥಿರ ವರ್ಣ *
set_ordering (ವರ್ಣ ಧ್ರುವ *s, ಸಂರಚನಾ keyfield *ಕೀಲಿ, ಗಣನಾ blanktype blanktype)
{
  ಯಾವತ್ (*s)
    {
      ವಿಕಲ್ಪ (*s)
        {
        ಪಕ್ಷ 'b':
          ಯದಿ (blanktype == bl_start || blanktype == bl_both)
            ಕೀಲಿ->skipsblanks = true;
          ಯದಿ (blanktype == bl_end || blanktype == bl_both)
            ಕೀಲಿ->skipeblanks = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'd':
          ಕೀಲಿ->ignore = nondictionary;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
          ಕೀಲಿ->translate = fold_toupper;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'g':
          ಕೀಲಿ->general_numeric = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'h':
          ಕೀಲಿ->human_numeric = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ಯದಿ (! ಕೀಲಿ->ignore)
            ಕೀಲಿ->ignore = nonprinting;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'M':
          ಕೀಲಿ->month = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          ಕೀಲಿ->numeric = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'R':
          ಕೀಲಿ->random = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'r':
          ಕೀಲಿ->reverse = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'V':
          ಕೀಲಿ->version = true;
          ಭಙ್ಗ;
        ಸ್ವಭಾವ:
          ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) s;
        }
      ++s;
    }
  ಪ್ರತ್ಯಾಗಮ (ವರ್ಣ *) s;
}

/* Initialize KEY.  */

ಸ್ಥಿರ ಸಂರಚನಾ keyfield *
ಕೀಲಿ_init (ಸಂರಚನಾ keyfield *ಕೀಲಿ)
{
  memset (ಕೀಲಿ, 0, ಪರಿಮಾಣ *ಕೀಲಿ);
  ಕೀಲಿ->eword = SIZE_MAX;
  ಪ್ರತ್ಯಾಗಮ ಕೀಲಿ;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಸಂರಚನಾ keyfield *ಕೀಲಿ;
  ಸಂರಚನಾ keyfield ಕೀಲಿ_buf;
  ಸಂರಚನಾ keyfield gkey;
  bool gkey_only = false;
  ವರ್ಣ ಧ್ರುವ *s;
  ಪೂರ್ಣಾಙ್ಕ c = 0;
  ವರ್ಣ checkonly = 0;
  bool mergeonly = false;
  ವರ್ಣ *random_source = ರಿಕ್ತ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ);
  ಪೂರ್ಣಾಙ್ಕ posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  ವರ್ಣ **files;
  ವರ್ಣ *files_from = ರಿಕ್ತ;
  ಸಂರಚನಾ Tokens tok;
  ವರ್ಣ ಧ್ರುವ *outfile = ರಿಕ್ತ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ಯದಿ HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    ಸಂರಚನಾ lconv ಧ್ರುವ *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    ಯದಿ (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ಯದಿ (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ಯದಿ (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ಗಣನಾ { nsigs = countof (term_sig) };

    ಸಂರಚನಾ sigaction act;

    sigemptyset (&caught_signals);
    ಪ್ರತಿ (size_t i = 0; i < nsigs; i++)
      {
        ಯದಿ (term_sig[i] == SIGPIPE)
          ಅನುವರ್ತ;  /* Handled below.  */
        sigaction (term_sig[i], ರಿಕ್ತ, &act);
        ಯದಿ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    ಪ್ರತಿ (size_t i = 0; i < nsigs; i++)
      {
        ಯದಿ (term_sig[i] == SIGPIPE)
          ಅನುವರ್ತ;  /* Handled below.  */
        ಯದಿ (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, ರಿಕ್ತ);
      }
  }
  ಸಙ್ಕೇತ (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ಕೀಲಿ_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, ಪರಿಮಾಣ *files);

  ಯಾವತ್ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      ಪೂರ್ಣಾಙ್ಕ oi = -1;

      ಯದಿ (c == -1
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
          ಯದಿ (argc <= optind)
            ಭಙ್ಗ;
          files[nfiles++] = argv[optind++];
        }
      ಅನ್ಯಥಾ ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 1:
          ಕೀಲಿ = ರಿಕ್ತ;
          ಯದಿ (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ಯದಿ (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ಕೀಲಿ = ಕೀಲಿ_init (&ಕೀಲಿ_buf);
                  s = parse_field_ಎಣಿಕೆ (optarg + 1, &ಕೀಲಿ->sword, ರಿಕ್ತ);
                  ಯದಿ (s && *s == '.')
                    s = parse_field_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->schar, ರಿಕ್ತ);
                  ಯದಿ (! (ಕೀಲಿ->sword || ಕೀಲಿ->schar))
                    ಕೀಲಿ->sword = SIZE_MAX;
                  ಯದಿ (! s || *set_ordering (s, ಕೀಲಿ, bl_start))
                    ಕೀಲಿ = ರಿಕ್ತ;
                  ಅನ್ಯಥಾ
                    {
                      ಯದಿ (minus_pos_usage)
                        {
                          ವರ್ಣ ಧ್ರುವ *optarg1 = argv[optind++];
                          s = parse_field_ಎಣಿಕೆ (optarg1 + 1, &ಕೀಲಿ->eword,
                                             N_("invalid number after '-'"));
                          ಯದಿ (*s == '.')
                            s = parse_field_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->echar,
                                               N_("invalid number after '.'"));
                          ಯದಿ (!ಕೀಲಿ->echar && ಕೀಲಿ->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ಕೀಲಿ->eword--;
                            }
                          ಯದಿ (*set_ordering (s, ಕೀಲಿ, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ಕೀಲಿ->traditional_used = true;
                      insertkey (ಕೀಲಿ);
                    }
                }
            }
          ಯದಿ (! ಕೀಲಿ)
            files[nfiles++] = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        ಪಕ್ಷ 'b':
        ಪಕ್ಷ 'd':
        ಪಕ್ಷ 'f':
        ಪಕ್ಷ 'g':
        ಪಕ್ಷ 'h':
        ಪಕ್ಷ 'i':
        ಪಕ್ಷ 'M':
        ಪಕ್ಷ 'n':
        ಪಕ್ಷ 'r':
        ಪಕ್ಷ 'R':
        ಪಕ್ಷ 'V':
          {
            ವರ್ಣ str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        ಪಕ್ಷ 'c':
        ಪಕ್ಷ 'C':
          ಯದಿ (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ಭಙ್ಗ;

        ಪಕ್ಷ COMPRESS_PROGRAM_OPTION:
          ಯದಿ (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ DEBUG_PROGRAM_OPTION:
          debug = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ FILES0_FROM_OPTION:
          files_from = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'k':
          ಕೀಲಿ = ಕೀಲಿ_init (&ಕೀಲಿ_buf);

          /* Get POS1. */
          s = parse_field_ಎಣಿಕೆ (optarg, &ಕೀಲಿ->sword,
                                 N_("invalid number at field start"));
          ಯದಿ (! ಕೀಲಿ->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ಯದಿ (*s == '.')
            {
              s = parse_field_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->schar,
                                     N_("invalid number after '.'"));
              ಯದಿ (! ಕೀಲಿ->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ಯದಿ (! (ಕೀಲಿ->sword || ಕೀಲಿ->schar))
            ಕೀಲಿ->sword = SIZE_MAX;
          s = set_ordering (s, ಕೀಲಿ, bl_start);
          ಯದಿ (*s != ',')
            {
              ಕೀಲಿ->eword = SIZE_MAX;
              ಕೀಲಿ->echar = 0;
            }
          ಅನ್ಯಥಾ
            {
              /* Get POS2. */
              s = parse_field_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->eword,
                                     N_("invalid number after ','"));
              ಯದಿ (! ಕೀಲಿ->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ಯದಿ (*s == '.')
                {
                  s = parse_field_ಎಣಿಕೆ (s + 1, &ಕೀಲಿ->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, ಕೀಲಿ, bl_end);
            }
          ಯದಿ (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ಕೀಲಿ);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'm':
          mergeonly = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'o':
          ಯದಿ (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ RANDOM_SOURCE_OPTION:
          ಯದಿ (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ 's':
          stable = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'S':
          specify_sort_size (oi, c, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          {
            ವರ್ಣ newtab = optarg[0];
            ಯದಿ (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            ಯದಿ (optarg[1])
              {
                ಯದಿ (streq (optarg, "\\0"))
                  newtab = '\0';
                ಅನ್ಯಥಾ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ಯದಿ (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'T':
          add_temp_dir (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ 'u':
          unique = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ಯದಿ (optarg == argv[optind - 1])
            {
              ವರ್ಣ ಧ್ರುವ *p;
              ಪ್ರತಿ (p = optarg; c_isdigit (*p); p++)
                ಅನುವರ್ತ;
              optind -= (*p != '\0');
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ 'z':
          eolchar = 0;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (ಸಙ್ಕೇತ (SIGPIPE, SIG_IGN) == SIG_DFL);

  ಯದಿ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ಯದಿ (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ಯದಿ (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      ಯದಿ (tok.n_tok)
        {
          ಮುಕ್ತ (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          ಪ್ರತಿ (size_t i = 0; i < nfiles; i++)
            {
              ಯದಿ (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ಅನ್ಯಥಾ ಯದಿ (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long ಪೂರ್ಣಾಙ್ಕ file_ಸಂಖ್ಯೆ = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_ಸಂಖ್ಯೆ);
                }
            }
        }
      ಅನ್ಯಥಾ
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  ಪ್ರತಿ (ಕೀಲಿ = keylist; ಕೀಲಿ; ಕೀಲಿ = ಕೀಲಿ->next)
    {
      ಯದಿ (default_ಕೀಲಿ_compare (ಕೀಲಿ) && !ಕೀಲಿ->reverse)
        {
          ಕೀಲಿ->ignore = gkey.ignore;
          ಕೀಲಿ->translate = gkey.translate;
          ಕೀಲಿ->skipsblanks = gkey.skipsblanks;
          ಕೀಲಿ->skipeblanks = gkey.skipeblanks;
          ಕೀಲಿ->month = gkey.month;
          ಕೀಲಿ->numeric = gkey.numeric;
          ಕೀಲಿ->general_numeric = gkey.general_numeric;
          ಕೀಲಿ->human_numeric = gkey.human_numeric;
          ಕೀಲಿ->version = gkey.version;
          ಕೀಲಿ->random = gkey.random;
          ಕೀಲಿ->reverse = gkey.reverse;
        }

      need_random |= ಕೀಲಿ->random;
    }

  ಯದಿ (!keylist && !default_ಕೀಲಿ_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ಯದಿ (debug)
    {
      ಯದಿ (checkonly || outfile)
        {
          ಸ್ಥಿರ ವರ್ಣ opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ಯದಿ (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ಯದಿ (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ಯದಿ (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ರಿಕ್ತ)));
      ಅನ್ಯಥಾ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ಕೀಲಿ_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  ಯದಿ (need_random)
    random_md5_state_init (random_source);

  ಯದಿ (temp_dirs.n_dirs == 0)
    {
      ವರ್ಣ ಧ್ರುವ *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ಯದಿ (nfiles == 0)
    {
      nfiles = 1;
      ಮುಕ್ತ (files);
      files = xmalloc (ಪರಿಮಾಣ *files);
      *files = (ವರ್ಣ *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ಯದಿ (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  ಯದಿ (checkonly)
    {
      ಯದಿ (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ಯದಿ (outfile)
        {
          ಸ್ಥಿರ ವರ್ಣ opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      exit (check (files[0], checkonly) ? EXIT_SUCCESS : SORT_OUT_OF_ORDER);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  ಯದಿ (mergeonly)
    {
      ಸಂರಚನಾ sortfile *sortfiles = xcalloc (nfiles, ಪರಿಮಾಣ *sortfiles);

      ಪ್ರತಿ (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (!nthreads)
        {
          unsigned long ಪೂರ್ಣಾಙ್ಕ np = ಅಂಕೆ_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * ಪರಿಮಾಣ (ಸಂರಚನಾ merge_ಗಂಟು));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  ಯದಿ (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
