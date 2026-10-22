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

#شامل <config.h>

#شامل <getopt.h>
#شامل <pthread.h>
#شامل <sys/resource.h>
#شامل <sys/types.h>
#شامل <sys/wait.h>
#شامل <signal.h>
#شامل <spawn.h>
#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "filevercmp.h"
#شامل "findprog.h"
#شامل "flexmember.h"
#شامل "hard-locale.h"
#شامل "hash.h"
#شامل "heap.h"
#شامل "ignore-value.h"
#شامل "mbswidth.h"
#شامل "nproc.h"
#شامل "physmem.h"
#شامل "posixver.h"
#شامل "quote.h"
#شامل "randread.h"
#شامل "readtokens0.h"
#شامل "stdlib--.h"
#شامل "strnumcmp.h"
#شامل "term-sig.h"
#شامل "unistd--.h"
#شامل "xmemcoll.h"
#شامل "xnanosleep.h"
#شامل "xstrtol.h"
#شامل "xstrtol-error.h"

#ifndef RLIMIT_DATA
ساختار rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#اگر HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#اگر !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#اگر !defined OPEN_MAX
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
شمارش { SUBTHREAD_سطرها_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_سطرها_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
شمارش { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
شمارش
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

شمارش
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

شمارش
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_ریشه = 1
  };

/* The representation of the decimal point in the current locale.  */
ایستا نویسه decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
ایستا صحیح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ایستا bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ایستا bool hard_LC_COLLATE;
#اگر HAVE_NL_LANGINFO
ایستا bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
شمارش blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
ایستا نویسه eolchar = '\n';

/* Lines are held in memory as counted strings. */
ساختار سطر
{
  نویسه *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  نویسه *keybeg;			/* Start of first key. */
  نویسه *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ساختار buffer
{
  نویسه *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t چپ;			/* Number of bytes left from previous reads. */
  size_t سطر_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ساختار sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
ساختار keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ثابت *ignore;		/* Boolean array of characters to ignore. */
  نویسه ثابت *translate;	/* Translation applied to characters. */
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
  ساختار keyfield *next;	/* Next keyfield to try. */
};

ساختار month
{
  نویسه ثابت *name;
  صحیح val;
};

/* Binary merge tree node. */
ساختار merge_گره
{
  ساختار سطر *lo;              /* Lines to merge from LO child node. */
  ساختار سطر *hi;              /* Lines to merge from HI child node. */
  ساختار سطر *end_lo;          /* End of available lines from LO. */
  ساختار سطر *end_hi;          /* End of available lines from HI. */
  ساختار سطر **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ساختار merge_گره *parent;    /* Parent node. */
  ساختار merge_گره *lo_child;  /* LO child node. */
  ساختار merge_گره *hi_child;  /* HI child node. */
  unsigned صحیح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ساختار merge_گره_queue
{
  ساختار heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ایستا ساختار سطر saved_سطر;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ایستا bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ایستا bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ایستا bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ایستا نویسه fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ایستا ساختار month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + اندازه (ساختار سطر))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ایستا size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ایستا size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define درون_داد_FILE_SIZE_GUESS (128 * 1024)

ایستا ساختار
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  نویسه ثابت **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
ایستا bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ایستا bool stable;

/* An int value outside char range.  */
شمارش { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
شمارش { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ایستا صحیح tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ایستا bool unique;

/* Nonzero if any of the input files are the standard input. */
ایستا bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ایستا ساختار keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ایستا نویسه ثابت *compress_program;

/* Annotate the output with extra info to aid the user.  */
ایستا bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ایستا unsigned صحیح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ایستا bool default_SIGPIPE;

/* The list of temporary files. */
ساختار tempnode
{
  ساختار tempnode *فرار next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  نویسه state;
  نویسه name[FLEXIBLE_ARRAY_MEMBER];
};
ایستا ساختار tempnode *فرار temphead;
ایستا ساختار tempnode *فرار *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ایستا تهی
cleanup (تهی)
{
  برای (ساختار tempnode ثابت *گره = temphead; گره; گره = گره->next)
    unlink (گره->name);
  temphead = پوچ;
}

/* Handle interrupts and hangups. */

ایستا تهی
sighandler (صحیح sig)
{
  cleanup ();

  سیگنال (sig, SIG_DFL);
  بالاببر (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ایستا تهی
sort_die (نویسه ثابت *message, نویسه ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  اگر (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         ignore leading blanks when finding sort کلیدها in each سطر\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالت\n\
         fold lower حالت to upper حالت characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical مقدار\n\
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
         compare according to string numerical مقدار;\n\
         see full documentation برای supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical کلیدها.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; برای more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check برای sorted درون_داد; انجامبده not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but انجامبده not report first bad سطر\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the سطر used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ درون_داد from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, اقرأ names from standard درون_داد\n\
"));
      oputs (_("\
  -k, --کلید=KEYDEF\n\
         sort via a کلید; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; انجامبده not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         اكتب result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE برای اصلی memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR برای temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the عدد of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of سطرها with equal کلیدها;\n\
         with -c, check برای strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] برای start and stop position, where F is a\n\
field عدد and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the سطر's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options برای that کلید.  If no کلید is given, use\n\
the entire سطر as the کلید.  Use --debug to diagnose incorrect کلید usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (پیشفرض), and so on برای M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte مقادیر.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
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

ایستا نویسه ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ایستا ساختار option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_argument, پوچ, 'b'},
  {"check", optional_argument, پوچ, CHECK_OPTION},
  {"compress-program", required_argument, پوچ, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, پوچ, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, پوچ, 'd'},
  {"ignore-case", no_argument, پوچ, 'f'},
  {"files0-from", required_argument, پوچ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, پوچ, 'g'},
  {"ignore-nonprinting", no_argument, پوچ, 'i'},
  {"key", required_argument, پوچ, 'k'},
  {"merge", no_argument, پوچ, 'm'},
  {"month-sort", no_argument, پوچ, 'M'},
  {"numeric-sort", no_argument, پوچ, 'n'},
  {"human-numeric-sort", no_argument, پوچ, 'h'},
  {"version-sort", no_argument, پوچ, 'V'},
  {"random-sort", no_argument, پوچ, 'R'},
  {"random-source", required_argument, پوچ, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, پوچ, SORT_OPTION},
  {"output", required_argument, پوچ, 'o'},
  {"reverse", no_argument, پوچ, 'r'},
  {"stable", no_argument, پوچ, 's'},
  {"batch-size", required_argument, پوچ, NMERGE_OPTION},
  {"buffer-size", required_argument, پوچ, 'S'},
  {"field-separator", required_argument, پوچ, 't'},
  {"temporary-directory", required_argument, پوچ, 'T'},
  {"unique", no_argument, پوچ, 'u'},
  {"zero-terminated", no_argument, پوچ, 'z'},
  {"parallel", required_argument, پوچ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ایستا نویسه ثابت *ثابت check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE پوچ
#undef  _ct_
};
ایستا نویسه ثابت check_types[] =
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

ایستا نویسه ثابت *ثابت sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE پوچ
#undef  _st_
};
ایستا نویسه ثابت sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
ایستا sigset_t caught_signals;

/* Critical section status.  */
ساختار cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ایستا تهی
cs_enter (ساختار cs_status *status)
{
  صحیح ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
ایستا تهی
cs_leave (ساختار cs_status ثابت *status)
{
  اگر (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, پوچ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
شمارش { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ساختار sortfile
{
  /* The file's name.  */
  نویسه ثابت *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ساختار tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ایستا Hash_table *proctab;

شمارش { INIT_PROCTAB_SIZE = 47 };

ایستا size_t
proctab_hasher (تهی ثابت *entry, size_t tabsize)
{
  ساختار tempnode ثابت *گره = entry;
  بازگشت گره->pid % tabsize;
}

ایستا bool
proctab_comparator (تهی ثابت *e1, تهی ثابت *e2)
{
  ساختار tempnode ثابت *n1 = e1;
  ساختار tempnode ثابت *n2 = e2;
  بازگشت n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ایستا pid_t nprocs;

ایستا bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ایستا pid_t
reap (pid_t pid)
{
  صحیح status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  اگر (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  وگرنه اگر (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      اگر (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  بازگشت cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ایستا تهی
register_proc (ساختار tempnode *temp)
{
  اگر (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, پوچ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 پوچ);
      اگر (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  اگر (! hash_درج (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ایستا bool
delete_proc (pid_t pid)
{
  ساختار tempnode test;

  test.pid = pid;
  ساختار tempnode *گره = hash_remove (proctab, &test);
  اگر (! گره)
    بازگشت false;
  گره->state = REAPED;
  بازگشت true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ایستا تهی
wait_proc (pid_t pid)
{
  اگر (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ایستا تهی
reap_exited (تهی)
{
  وقتی (0 < nprocs && reap (0))
    ادامه;
}

/* Reap at least one exited child, waiting if necessary.  */

ایستا تهی
reap_some (تهی)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ایستا تهی
reap_all (تهی)
{
  وقتی (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ایستا تهی
exit_cleanup (تهی)
{
  اگر (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ساختار cs_status cs;
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

ایستا ساختار tempnode *
create_temp_file (صحیح *pfd, bool survive_fd_exhaustion)
{
  ایستا نویسه ثابت slashbase[] = "/sortXXXXXX";
  ایستا idx_t temp_dir_index;
  صحیح fd;
  صحیح saved_errno;
  نویسه ثابت *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  ساختار tempnode *گره =
    xmalloc (FLEXSIZEOF (ساختار tempnode, name, len + اندازه slashbase));
  نویسه *file = گره->name;
  ساختار cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, اندازه slashbase);
  گره->next = پوچ;
  اگر (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  اگر (0 <= fd)
    {
      *temptail = گره;
      temptail = &گره->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  اگر (fd < 0)
    {
      اگر (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      آزاد (گره);
      گره = پوچ;
    }

  *pfd = fd;
  بازگشت گره;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ایستا ساختار stat *
get_outstatus (تهی)
{
  ایستا صحیح outstat_errno;
  ایستا ساختار stat outstat;
  اگر (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  بازگشت outstat_errno < 0 ? &outstat : پوچ;
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

ایستا FILE *
stream_open (نویسه ثابت *file, نویسه ثابت *how)
{
  FILE *fp;

  اگر (*how == 'r')
    {
      اگر (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      وگرنه
        {
          صحیح fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? پوچ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  وگرنه اگر (*how == 'w')
    {
      اگر (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          صحیح ftruncate_errno = errno;
          ساختار stat *outst = get_outstatus ();
          اگر (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  وگرنه
    affirm (!"unexpected mode passed to stream_open");

  بازگشت fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ایستا FILE *
xfopen (نویسه ثابت *file, نویسه ثابت *how)
{
  FILE *fp = stream_open (file, how);
  اگر (!fp)
    sort_die (_("open failed"), file);
  بازگشت fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ایستا تهی
xfclose (FILE *fp, نویسه ثابت *file)
{
  انتخاب (fileno (fp))
    {
    حالت STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      بشکن;

    حالت STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      اگر (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      بشکن;

    پیشفرض:
      اگر (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      بشکن;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ایستا تهی
move_fd (صحیح oldfd, صحیح newfd)
{
  اگر (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_مقدار (dup2 (oldfd, newfd));
      ignore_مقدار (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ایستا صحیح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحیح oldfd, صحیح newfd)
{
  صحیح result = 0;
  اگر (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      اگر (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  بازگشت result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ایستا نویسه ثابت *
get_resolved_compress_program (تهی)
{
  /* Use a cache, to perform the search only once.  */
  ایستا نویسه ثابت *resolved_compress_program_cache /* = NULL */;

  اگر (resolved_compress_program_cache == پوچ)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), پوچ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  بازگشت resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ایستا صحیح
pipe_child (pid_t *pid, صحیح pipefds[2], صحیح tempfd, bool decompress,
            size_t tries)
{
  نویسه ثابت *resolved_compress_program;
  ساختار tempnode *saved_temphead;
  دوگانه wait_retry = 0.25;
  ساختار cs_status cs;
  صحیح result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  اگر (resolved_compress_program == پوچ)
    بازگشت errno;

  اگر ((result = posix_spawnattr_init (&attr)))
    بازگشت result;
  اگر ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      بازگشت result;
    }

  اگر (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحیح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      بازگشت saved_errno;
    }

  اگر ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      بازگشت result;
    }

  نویسه ثابت *ثابت argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : پوچ,
      پوچ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  اگر (nmerge + 1 < nprocs)
    reap_some ();

  وقتی (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = پوچ;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (نویسه * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      اگر (result != EAGAIN)
        بشکن;
      وگرنه
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

  اگر (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  وگرنه
    ++nprocs;

  بازگشت result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ایستا ساختار tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  صحیح tempfd;
  ساختار tempnode *گره = create_temp_file (&tempfd, survive_fd_exhaustion);
  اگر (! گره)
    بازگشت پوچ;

  گره->state = UNCOMPRESSED;

  اگر (compress_program)
    {
      صحیح pipefds[2];
      ایستا صحیح last_result = 0;

      صحیح result = pipe_child (&گره->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      اگر (result)
        {
          اگر (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      وگرنه
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (گره);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  اگر (! *pfp)
    sort_die (_("couldn't create temporary file"), گره->name);

  بازگشت گره;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ایستا ساختار tempnode *
create_temp (FILE **pfp)
{
  بازگشت maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ایستا FILE *
open_temp (ساختار tempnode *temp)
{
  صحیح tempfd, pipefds[2];
  FILE *fp = پوچ;

  اگر (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  اگر (tempfd < 0)
    بازگشت پوچ;

  pid_t child;
  صحیح result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  اگر (result)
    {
      اگر (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  وگرنه
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      اگر (! fp)
        {
          صحیح saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  بازگشت fp;
}

/* Append DIR to the array of temporary directory names.  */
ایستا تهی
add_temp_dir (نویسه ثابت *dir)
{
  اگر (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              اندازه *temp_dirs.dirs);

  idx_t ثابت dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ایستا تهی
zaptemp (نویسه ثابت *name)
{
  ساختار tempnode *فرار *pnode;
  ساختار tempnode *گره;
  ساختار tempnode *next;
  صحیح unlink_status;
  صحیح unlink_errno = 0;
  ساختار cs_status cs;

  برای (pnode = &temphead; (گره = *pnode)->name != name; pnode = &گره->next)
    ادامه;

  اگر (گره->state == UNREAPED)
    wait_proc (گره->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = گره->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  اگر (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  اگر (! next)
    temptail = pnode;
  آزاد (گره);
}

#اگر HAVE_NL_LANGINFO

ایستا صحیح
struct_month_cmp (تهی ثابت *m1, تهی ثابت *m2)
{
  ساختار month ثابت *month1 = m1;
  ساختار month ثابت *month2 = m2;
  بازگشت strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

ایستا تهی
inittables (تهی)
{
  برای (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#اگر HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  اگر (hard_LC_TIME)
    {
      برای (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          نویسه ثابت *s;
          size_t s_len;
          size_t j, k;
          نویسه *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          برای (j = k = 0; j < s_len; j++)
            اگر (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, اندازه *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ایستا تهی
specify_nmerge (صحیح oi, نویسه c, نویسه ثابت *s)
{
  uintmax_t n;
  ساختار rlimit rlimit;
  شمارش strtol_error e = xstrtoumax (s, پوچ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned صحیح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  اگر (e == LONGINT_OK)
    {
      nmerge = n;
      اگر (nmerge != n)
        e = LONGINT_OVERFLOW;
      وگرنه
        {
          اگر (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          وگرنه اگر (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          وگرنه
            بازگشت;
        }
    }

  اگر (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  وگرنه
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ایستا تهی
specify_sort_size (صحیح oi, نویسه c, نویسه ثابت *s)
{
  uintmax_t n;
  نویسه *suffix;
  شمارش strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  اگر (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  اگر (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    انتخاب (suffix[0])
      {
      حالت 'b':
        e = LONGINT_OK;
        بشکن;

      حالت '%':
        {
          دوگانه mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          اگر (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          وگرنه
            e = LONGINT_OVERFLOW;
        }
        بشکن;
      }

  اگر (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      اگر (n < sort_size)
        بازگشت;

      sort_size = n;
      اگر (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          بازگشت;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ایستا size_t
specify_nthreads (صحیح oi, نویسه c, نویسه ثابت *s)
{
  uintmax_t nthreads;
  شمارش strtol_error e = xstrtoumax (s, پوچ, 10, &nthreads, "");
  اگر (e == LONGINT_OVERFLOW)
    بازگشت SIZE_MAX;
  اگر (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  اگر (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  اگر (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  بازگشت nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ایستا size_t
default_sort_size (تهی)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  ساختار rlimit rlimit;
  اگر (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  اگر (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  اگر (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  دوگانه avail = physmem_available ();
  دوگانه total = physmem_total ();
  دوگانه mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  اگر (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  اگر (mem < size)
    size = mem;
  بازگشت MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ایستا bool
سطر_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = اندازه (ساختار سطر);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  اگر (ckd_add (&aligned, size, padding))
    بازگشت false;

  *alloc = aligned;
  بازگشت true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ایستا size_t
default_initial_sort_size (size_t سطر_bytes)
{
  size_t size = سطر_bytes + 2;
  size_t درون_داد_size;

  اگر (ckd_mul (&درون_داد_size, درون_داد_FILE_SIZE_GUESS, سطر_bytes + 1)
      || ckd_add (&size, size, درون_داد_size))
    بازگشت SIZE_MAX;

  بازگشت MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ایستا bool
درون_داد_size_buffer_bytes (uintmax_t file_size, size_t سطر_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_درون_داد_byte = سطر_bytes + 1;

  اگر (SIZE_MAX / worst_case_per_درون_داد_byte < file_size)
    بازگشت false;

  size_t size = file_size * worst_case_per_درون_داد_byte;
  اگر (SIZE_MAX - size < 1)
    بازگشت false;

  *worst_case = size + 1;
  بازگشت true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ایستا تهی
sort_buffer_policy (FILE *ثابت *fps, size_t nfps,
                    نویسه *ثابت *files, size_t nfiles,
                    size_t سطر_bytes,
                    ساختار sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (سطر_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_درون_داد_byte = سطر_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_درون_داد_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  برای (size_t i = 0; i < nfiles; i++)
    {
      ساختار stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      اگر (size_bound <= size)
        {
          policy->initial = size_bound;
          بازگشت;
        }

      اگر ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      اگر (known_size)
        file_size = st.st_size;
      وگرنه
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          اگر (sort_size)
            {
              policy->initial = size_bound;
              بازگشت;
            }
          file_size = درون_داد_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      اگر (! درون_داد_size_buffer_bytes (file_size, سطر_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          بازگشت;
        }

      اگر (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          بازگشت;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ایستا تهی
initbuf (ساختار buffer *buf, size_t سطر_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  وقتی (true)
    {
      اگر (! سطر_aligned_size (&alloc))
        buf->buf = پوچ;
      وگرنه
        buf->buf = malloc (alloc);
      اگر (buf->buf)
        بشکن;
      alloc /= 2;
      اگر (alloc <= سطر_bytes + 1)
        xalloc_die ();
    }

  buf->سطر_bytes = سطر_bytes;
  buf->alloc = alloc;
  buf->used = buf->چپ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ایستا درونخطی ساختار سطر *
buffer_linelim (ساختار buffer ثابت *buf)
{
  تهی *linelim = buf->buf + buf->alloc;
  بازگشت linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ایستا bool
try_growbuf (ساختار buffer *buf, size_t alloc)
{
  اگر (! سطر_aligned_size (&alloc) || alloc <= buf->alloc)
    بازگشت false;

  نویسه *newbuf = malloc (alloc);
  اگر (! newbuf)
    بازگشت false;

  نویسه *oldbuf = buf->buf;
  ساختار سطر *old_linelim = buffer_linelim (buf);
  ساختار سطر *old_سطر = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  ساختار سطر *new_linelim = (تهی *) (newbuf + alloc);
  ساختار سطر *new_سطر = new_linelim - buf->nlines;
  memcpy (new_سطر, old_سطر, buf->nlines * اندازه *new_سطر);

  برای (ساختار سطر *سطر = new_سطر; سطر < new_linelim; سطر++)
    {
      سطر->text = newbuf + (سطر->text - oldbuf);
      اگر (keylist)
        {
          سطر->keybeg = newbuf + (سطر->keybeg - oldbuf);
          سطر->keylim = newbuf + (سطر->keylim - oldbuf);
        }
    }

  آزاد (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  بازگشت true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ایستا bool
maybe_growbuf (ساختار buffer *buf, ساختار sort_buffer_policy *policy)
{
  اگر (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    بازگشت false;

  size_t alloc;
  اگر (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  وگرنه
    alloc = policy->limit;

  اگر (try_growbuf (buf, alloc))
    بازگشت true;

  policy->growth_failed = true;
  بازگشت false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ایستا نویسه *
begfield (ساختار سطر ثابت *سطر, ساختار keyfield ثابت *کلید)
{
  نویسه *ptr = سطر->text, *lim = ptr + سطر->length - 1;
  size_t sword = کلید->sword;
  size_t schar = کلید->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  اگر (tab != TAB_DEFAULT)
    وقتی (ptr < lim && sword--)
      {
        نویسه *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        اگر (ptr < lim)
          ++ptr;
      }
  وگرنه
    وقتی (ptr < lim && sword--)
      {
        وقتی (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        وقتی (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  اگر (کلید->skipsblanks)
    وقتی (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  اگر (schar < remaining_bytes)
    ptr += schar;
  وگرنه
    ptr = lim;

  بازگشت ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
ایستا نویسه *
limfield (ساختار سطر ثابت *سطر, ساختار keyfield ثابت *کلید)
{
  نویسه *ptr = سطر->text, *lim = ptr + سطر->length - 1;
  size_t eword = کلید->eword, echar = کلید->echar;

  اگر (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  اگر (tab != TAB_DEFAULT)
    وقتی (ptr < lim && eword--)
      {
        نویسه *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        اگر (ptr < lim && (eword || echar))
          ++ptr;
      }
  وگرنه
    وقتی (ptr < lim && eword--)
      {
        وقتی (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        وقتی (ptr < lim && !blanks[to_uchar (*ptr)])
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
  اگر (tab != TAB_DEFAULT)
    {
      نویسه *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      اگر (newlim)
        lim = newlim;
    }
  وگرنه
    {
      نویسه *newlim;
      newlim = ptr;
      وقتی (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      وقتی (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  اگر (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      اگر (کلید->skipeblanks)
        وقتی (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      اگر (echar < remaining_bytes)
        ptr += echar;
      وگرنه
        ptr = lim;
    }

  بازگشت ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ایستا bool
fillbuf (ساختار buffer *buf, FILE *fp, نویسه ثابت *file,
         ساختار sort_buffer_policy *policy)
{
  ساختار keyfield ثابت *کلید = keylist;
  نویسه eol = eolchar;
  size_t سطر_bytes = buf->سطر_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  اگر (buf->eof)
    بازگشت false;

  اگر (buf->used != buf->چپ)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->چپ, buf->چپ);
      buf->used = buf->چپ;
      buf->nlines = 0;
    }

  وقتی (true)
    {
      نویسه *ptr = buf->buf + buf->used;
      ساختار سطر *linelim = buffer_linelim (buf);
      ساختار سطر *سطر = linelim - buf->nlines;
      size_t avail = (نویسه *) linelim - buf->nlines * سطر_bytes - ptr;
      نویسه *سطر_start = buf->nlines ? سطر->text + سطر->length : buf->buf;

      وقتی (سطر_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (سطر_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          نویسه *ptrlim = ptr + bytes_read;
          نویسه *p;
          avail -= bytes_read;

          اگر (bytes_read != readsize)
            {
              اگر (ferror (fp))
                sort_die (_("read failed"), file);
              اگر (feof (fp))
                {
                  buf->eof = true;
                  اگر (buf->buf == ptrlim)
                    بازگشت false;
                  اگر (سطر_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          وقتی ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              سطر--;
              سطر->text = سطر_start;
              سطر->length = ptr - سطر_start;
              mergesize = MAX (mergesize, سطر->length);
              avail -= سطر_bytes;

              اگر (کلید)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  سطر->keylim = (کلید->eword == SIZE_MAX
                                  ? p
                                  : limfield (سطر, کلید));

                  اگر (کلید->sword != SIZE_MAX)
                    سطر->keybeg = begfield (سطر, کلید);
                  وگرنه
                    {
                      اگر (کلید->skipsblanks)
                        وقتی (blanks[to_uchar (*سطر_start)])
                          سطر_start++;
                      سطر->keybeg = سطر_start;
                    }
                }

              سطر_start = ptr;
            }

          ptr = ptrlim;
          اگر (buf->eof)
            بشکن;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - سطر;
      اگر (buf->nlines != 0)
        {
          اگر (! buf->eof && maybe_growbuf (buf, policy))
            ادامه;
          buf->چپ = ptr - سطر_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          بازگشت true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t سطر_alloc = buf->alloc / اندازه (ساختار سطر);
        buf->buf = xpalloc (buf->buf, &سطر_alloc, 1, -1, اندازه (ساختار سطر));
        buf->alloc = سطر_alloc * اندازه (ساختار سطر);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ایستا نویسه ثابت unit_order[UCHAR_LIM] =
  {
#اگر ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#وگرنه
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
ایستا نویسه
traverse_raw_عدد (نویسه ثابت **عدد)
{
  نویسه ثابت *p = *عدد;
  نویسه ch;
  نویسه max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  وقتی (c_isdigit (ch = *p++))
    {
      اگر (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      اگر (ends_with_thousands_sep)
        ++p;
    }

  اگر (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *عدد = p - 2;
      بازگشت max_digit;
    }

  اگر (ch == decimal_point)
    وقتی (c_isdigit (ch = *p++))
      اگر (max_digit < ch)
        max_digit = ch;

  *عدد = p - 1;
  بازگشت max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
ایستا صحیح
find_unit_order (نویسه ثابت *عدد)
{
  bool minus_sign = (*عدد == '-');
  نویسه ثابت *p = عدد + minus_sign;
  نویسه max_digit = traverse_raw_عدد (&p);
  اگر ('0' < max_digit)
    {
      unsigned نویسه ch = *p;
      صحیح order = unit_order[ch];
      بازگشت (minus_sign ? -order : order);
    }
  وگرنه
    بازگشت 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
ایستا صحیح
human_numcompare (نویسه ثابت *a, نویسه ثابت *b)
{
  وقتی (blanks[to_uchar (*a)])
    a++;
  وقتی (blanks[to_uchar (*b)])
    b++;

  صحیح diff = find_unit_order (a) - find_unit_order (b);
  بازگشت (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
ایستا صحیح
numcompare (نویسه ثابت *a, نویسه ثابت *b)
{
  وقتی (blanks[to_uchar (*a)])
    a++;
  وقتی (blanks[to_uchar (*b)])
    b++;

  بازگشت strnumcmp (a, b, decimal_point, thousands_sep);
}

ایستا صحیح
nan_compare (long دوگانه a, long دوگانه b)
{
  نویسه buf[2][اندازه "-nan""()" + CHAR_BIT * اندازه a];
  snprintf (buf[0], اندازه buf[0], "%Lf", a);
  snprintf (buf[1], اندازه buf[1], "%Lf", b);
  بازگشت strcmp (buf[0], buf[1]);
}

ایستا صحیح
general_numcompare (نویسه ثابت *sa, نویسه ثابت *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  نویسه *ea;
  نویسه *eb;
  long دوگانه a = strtold (sa, &ea);
  long دوگانه b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  اگر (sa == ea)
    بازگشت sb == eb ? 0 : -1;
  اگر (sb == eb)
    بازگشت 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  بازگشت (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ایستا صحیح
getmonth (نویسه ثابت *month, نویسه **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  وقتی (blanks[to_uchar (*month)])
    month++;

  انجامبده
    {
      size_t ix = (lo + hi) / 2;
      نویسه ثابت *m = month;
      نویسه ثابت *n = monthtab[ix].name;

      برای (;; m++, n++)
        {
          اگر (!*n)
            {
              اگر (ea)
                *ea = (نویسه *) m;
              بازگشت monthtab[ix].val;
            }
          اگر (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              بشکن;
            }
          وگرنه اگر (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              بشکن;
            }
        }
    }
  وقتی (lo < hi);

  بازگشت 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# اگر 14 <= __GNUC__
#  pragma GCC diagnostic افزودن
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#شامل "md5.h"

#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# اگر 14 <= __GNUC__
#  pragma GCC diagnostic برداشت
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
ایستا تهی
link_failure (تهی)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ایستا تهی *
نماد_address (تهی *handle, نویسه ثابت *نماد)
{
  تهی *address = dlsym (handle, نماد);
  اگر (!address)
    link_failure ();
  بازگشت address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
ایستا تهی
link_libcrypto (تهی)
{
#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  تهی *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  اگر (!handle)
    link_failure ();
  ptr_MD5_Init = نماد_address (handle, "MD5_Init");
  ptr_MD5_Update = نماد_address (handle, "MD5_Update");
  ptr_MD5_Final = نماد_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
ایستا ساختار md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ایستا تهی
random_md5_state_init (نویسه ثابت *random_source)
{
  unsigned نویسه buf[MD5_DIGEST_SIZE];
  ساختار randread_source *r = randread_new (random_source, اندازه buf);
  اگر (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, اندازه buf);
  اگر (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, اندازه buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ایستا size_t
xstrxfrm (نویسه *restrict dest, نویسه ثابت *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  اگر (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  بازگشت translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ایستا صحیح
compare_random (نویسه *restrict texta, size_t lena,
                نویسه *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  صحیح xfrm_diff = 0;

  نویسه stackbuf[4000];
  نویسه *buf = stackbuf;
  size_t bufsize = اندازه stackbuf;
  تهی *allocated = پوچ;
  uint32_t dig[2][MD5_DIGEST_SIZE / اندازه (uint32_t)];
  ساختار md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  اگر (hard_LC_COLLATE)
    {
      نویسه ثابت *lima = texta + lena;
      نویسه ثابت *limb = textb + lenb;

      وقتی (true)
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
          اگر (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              آزاد (allocated);
              buf = allocated = malloc (bufsize);
              اگر (! buf)
                {
                  buf = stackbuf;
                  bufsize = اندازه stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : پوچ), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          اگر (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              اگر (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              آزاد (allocated);
              buf = allocated = xmalloc (bufsize);
              اگر (texta < lima)
                strxfrm (buf, texta, sizea);
              اگر (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          اگر (texta < lima)
            texta += strlen (texta) + 1;
          اگر (textb < limb)
            textb += strlen (textb) + 1;
          اگر (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              بشکن;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          اگر (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              اگر (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  صحیح diff = memcmp (dig[0], dig[1], اندازه dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  اگر (! diff)
    {
      اگر (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          اگر (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  آزاد (allocated);

  بازگشت diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ایستا size_t
debug_width (نویسه ثابت *text, نویسه ثابت *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  وقتی (text < lim)
    width += (*text++ == '\t');
  بازگشت width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ایستا تهی
mark_کلید (size_t offset, size_t width)
{
  وقتی (offset--)
    putchar (' ');

  اگر (!width)
    printf (_("^ no match for key\n"));
  وگرنه
    {
      انجامبده
        putchar ('_');
      وقتی (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ایستا درونخطی bool
کلید_numeric (ساختار keyfield ثابت *کلید)
{
  بازگشت کلید->numeric || کلید->general_numeric || کلید->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ایستا تهی
debug_کلید (ساختار سطر ثابت *سطر, ساختار keyfield ثابت *کلید)
{
  نویسه *text = سطر->text;
  نویسه *beg = text;
  نویسه *lim = text + سطر->length - 1;

  اگر (کلید)
    {
      اگر (کلید->sword != SIZE_MAX)
        beg = begfield (سطر, کلید);
      اگر (کلید->eword != SIZE_MAX)
        {
          lim = limfield (سطر, کلید);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      اگر ((کلید->skipsblanks && کلید->sword == SIZE_MAX)
          || کلید->month || کلید_numeric (کلید))
        {
          نویسه saved = *lim;
          *lim = '\0';

          وقتی (blanks[to_uchar (*beg)])
            beg++;

          نویسه *tighter_lim = beg;

          اگر (lim < beg)
            tighter_lim = lim;
          وگرنه اگر (کلید->month)
            getmonth (beg, &tighter_lim);
          وگرنه اگر (کلید->general_numeric)
            ignore_مقدار (strtold (beg, &tighter_lim));
          وگرنه اگر (کلید->numeric || کلید->human_numeric)
            {
              نویسه ثابت *p = beg + (beg < lim && *beg == '-');
              نویسه max_digit = traverse_raw_عدد (&p);
              اگر ('0' <= max_digit)
                {
                  unsigned نویسه ch = *p;
                  tighter_lim = (نویسه *) p
                    + (کلید->human_numeric && unit_order[ch]);
                }
            }
          وگرنه
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_کلید (offset, width);
}

/* Debug LINE by underlining its keys.  */

ایستا تهی
debug_سطر (ساختار سطر ثابت *سطر)
{
  ساختار keyfield ثابت *کلید = keylist;

  انجامبده
    debug_کلید (سطر, کلید);
  وقتی (کلید && ((کلید = کلید->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ایستا bool
default_کلید_compare (ساختار keyfield ثابت *کلید)
{
  بازگشت ! (کلید->ignore
            || کلید->translate
            || کلید->skipsblanks
            || کلید->skipeblanks
            || کلید_numeric (کلید)
            || کلید->month
            || کلید->version
            || کلید->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ایستا تهی
کلید_to_opts (ساختار keyfield ثابت *کلید, نویسه *opts)
{
  اگر (کلید->skipsblanks || کلید->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  اگر (کلید->ignore == nondictionary)
    *opts++ = 'd';
  اگر (کلید->translate)
    *opts++ = 'f';
  اگر (کلید->general_numeric)
    *opts++ = 'g';
  اگر (کلید->human_numeric)
    *opts++ = 'h';
  اگر (کلید->ignore == nonprinting)
    *opts++ = 'i';
  اگر (کلید->month)
    *opts++ = 'M';
  اگر (کلید->numeric)
    *opts++ = 'n';
  اگر (کلید->random)
    *opts++ = 'R';
  اگر (کلید->reverse)
    *opts++ = 'r';
  اگر (کلید->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ایستا تهی
کلید_warnings (ساختار keyfield ثابت *gkey, bool gkey_only)
{
  ساختار keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  برای (ساختار keyfield *کلید = keylist; کلید; کلید = کلید->next, keynum++)
    {
      اگر (کلید_numeric (کلید))
        {
          اگر (کلید->general_numeric)
            general_numeric_field = true;
          وگرنه
            basic_numeric_field = true;
        }

      اگر (کلید->traditional_used)
        {
          size_t sword = کلید->sword;
          size_t eword = کلید->eword;
          نویسه tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          نویسه obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          نویسه nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          نویسه *po = obuf;
          نویسه *pn = nbuf;

          اگر (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          اگر (کلید->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (کلید->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = کلید->sword != SIZE_MAX && کلید->eword < کلید->sword;
      اگر (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = کلید_numeric (کلید) || کلید->month;
      bool سطر_offset = کلید->eword == 0 && کلید->echar != 0; /* -k1.x,1.y  */
      اگر (!zero_width && !gkey_only && tab == TAB_DEFAULT && !سطر_offset
          && ((!کلید->skipsblanks && !implicit_skip)
              || (!کلید->skipsblanks && کلید->schar)
              || (!کلید->skipeblanks && کلید->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      اگر (!gkey_only && کلید_numeric (کلید))
        {
          size_t sword = کلید->sword + 1;
          size_t eword = کلید->eword + 1;
          اگر (!sword)
            sword++;
          اگر (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              اگر (کلید->general_numeric)
                general_numeric_field_span = true;
              وگرنه
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      اگر (ugkey.ignore && (ugkey.ignore == کلید->ignore))
        ugkey.ignore = پوچ;
      اگر (ugkey.translate && (ugkey.translate == کلید->translate))
        ugkey.translate = پوچ;
      ugkey.skipsblanks &= !کلید->skipsblanks;
      ugkey.skipeblanks &= !کلید->skipeblanks;
      ugkey.month &= !کلید->month;
      ugkey.numeric &= !کلید->numeric;
      ugkey.general_numeric &= !کلید->general_numeric;
      ugkey.human_numeric &= !کلید->human_numeric;
      ugkey.random &= !کلید->random;
      ugkey.version &= !کلید->version;
      ugkey.reverse &= !کلید->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool عدد_locale_warned = false;
  اگر (basic_numeric_field_span)
    {
      اگر (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((نویسه []) {thousands_sep, 0})));
          عدد_locale_warned = true;
        }
    }
  اگر (basic_numeric_field_span || general_numeric_field_span)
    {
      اگر (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((نویسه []) {decimal_point, 0})));
          عدد_locale_warned = true;
        }
      وگرنه اگر (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((نویسه []) {tab, 0})));
        }
      وگرنه اگر (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((نویسه []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  اگر ((basic_numeric_field || general_numeric_field) && ! عدد_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((نویسه []) {decimal_point, 0})));

    }

  اگر (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  اگر (!default_کلید_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      اگر (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      نویسه opts[اندازه short_options];
      کلید_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  اگر (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ایستا صحیح
diff_reversed (صحیح diff, bool reversed)
{
  بازگشت reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ایستا صحیح
keycompare (ساختار سطر ثابت *a, ساختار سطر ثابت *b)
{
  ساختار keyfield *کلید = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  نویسه *texta = a->keybeg;
  نویسه *textb = b->keybeg;
  نویسه *lima = a->keylim;
  نویسه *limb = b->keylim;

  صحیح diff;

  وقتی (true)
    {
      نویسه ثابت *translate = کلید->translate;
      bool ثابت *ignore = کلید->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      اگر (hard_LC_COLLATE || کلید_numeric (کلید)
          || کلید->month || کلید->random || کلید->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          نویسه *ta = texta;
          نویسه *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          نویسه enda = ta[tlena];
          نویسه endb = tb[tlenb];

          تهی *allocated = پوچ;
          نویسه stackbuf[4000];

          اگر (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              اگر (size <= اندازه stackbuf)
                ta = stackbuf;
              وگرنه
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              برای (tlena = i = 0; i < lena; i++)
                اگر (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              برای (tlenb = i = 0; i < lenb; i++)
                اگر (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          اگر (کلید->numeric)
            diff = numcompare (ta, tb);
          وگرنه اگر (کلید->general_numeric)
            diff = general_numcompare (ta, tb);
          وگرنه اگر (کلید->human_numeric)
            diff = human_numcompare (ta, tb);
          وگرنه اگر (کلید->month)
            diff = getmonth (ta, پوچ) - getmonth (tb, پوچ);
          وگرنه اگر (کلید->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          وگرنه اگر (کلید->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          وگرنه
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              اگر (tlena == 0)
                diff = - NONZERO (tlenb);
              وگرنه اگر (tlenb == 0)
                diff = 1;
              وگرنه
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          آزاد (allocated);
        }
      وگرنه اگر (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  انجامبده									\
    {									\
          وقتی (true)							\
            {								\
              وقتی (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              وقتی (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              اگر (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  بشکن;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              اگر (diff)							\
                بشکن;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  وقتی (0)

          اگر (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          وگرنه
            CMP_WITH_IGNORE (*texta, *textb);
        }
      وگرنه
        {
          size_t lenmin = MIN (lena, lenb);
          اگر (lenmin == 0)
            diff = 0;
          وگرنه اگر (translate)
            {
              size_t i = 0;
              انجامبده
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  اگر (diff)
                    بشکن;
                  i++;
                }
              وقتی (i < lenmin);
            }
          وگرنه
            diff = memcmp (texta, textb, lenmin);

          اگر (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      اگر (diff)
        بشکن;

      کلید = کلید->next;
      اگر (! کلید)
        بازگشت 0;

      /* Find the beginning and limit of the next field.  */
      اگر (کلید->eword != SIZE_MAX)
        lima = limfield (a, کلید), limb = limfield (b, کلید);
      وگرنه
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      اگر (کلید->sword != SIZE_MAX)
        texta = begfield (a, کلید), textb = begfield (b, کلید);
      وگرنه
        {
          texta = a->text, textb = b->text;
          اگر (کلید->skipsblanks)
            {
              وقتی (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              وقتی (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  بازگشت diff_reversed (diff, کلید->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ایستا صحیح
compare (ساختار سطر ثابت *a, ساختار سطر ثابت *b)
{
  صحیح diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  اگر (keylist)
    {
      diff = keycompare (a, b);
      اگر (diff || unique || stable)
        بازگشت diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  اگر (alen == 0)
    diff = - NONZERO (blen);
  وگرنه اگر (blen == 0)
    diff = 1;
  وگرنه اگر (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  وگرنه
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      اگر (!diff)
        diff = _GL_CMP (alen, blen);
    }

  بازگشت diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ایستا تهی
write_سطر (ساختار سطر ثابت *سطر, FILE *fp, نویسه ثابت *output_file)
{
  نویسه *buf = سطر->text;
  size_t n_bytes = سطر->length;
  نویسه *ebuf = buf + n_bytes;

  اگر (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      نویسه ثابت *c = buf;

      وقتی (c < ebuf)
        {
          نویسه wc = *c++;
          اگر (wc == '\t')
            wc = '>';
          وگرنه اگر (c == ebuf)
            wc = '\n';
          اگر (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_سطر (سطر);
    }
  وگرنه
    {
      ebuf[-1] = eolchar;
      اگر (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ایستا bool
check (نویسه ثابت *file_name, نویسه checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ساختار buffer buf;		/* Input buffer. */
  ساختار سطر temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t سطر_عدد = 0;
  ساختار keyfield ثابت *کلید = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, اندازه (ساختار سطر),
           MAX (merge_buffer_size, sort_size));
  temp.text = پوچ;

  وقتی (fillbuf (&buf, fp, file_name, پوچ))
    {
      ساختار سطر ثابت *سطر = buffer_linelim (&buf);
      ساختار سطر ثابت *linebase = سطر - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      اگر (alloc && nonunique <= compare (&temp, سطر - 1))
        {
        found_disorder:
          {
            اگر (checkonly == 'c')
              {
                ساختار سطر ثابت *disorder_سطر = سطر - 1;
                uintmax_t disorder_سطر_عدد =
                  buffer_linelim (&buf) - disorder_سطر + سطر_عدد;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_سطر_عدد);
                write_سطر (disorder_سطر, stderr, _("standard error"));
              }

            ordered = false;
            بشکن;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      وقتی (linebase < --سطر)
        اگر (nonunique <= compare (سطر, سطر - 1))
          برو found_disorder;

      سطر_عدد += buf.nlines;

      /* Save the last line of the buffer.  */
      اگر (alloc < سطر->length)
        {
          انجامبده
            {
              alloc *= 2;
              اگر (! alloc)
                {
                  alloc = سطر->length;
                  بشکن;
                }
            }
          وقتی (alloc < سطر->length);

          آزاد (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, سطر->text, سطر->length);
      temp.length = سطر->length;
      اگر (کلید)
        {
          temp.keybeg = temp.text + (سطر->keybeg - سطر->text);
          temp.keylim = temp.text + (سطر->keylim - سطر->text);
        }
    }

  xfclose (fp, file_name);
  آزاد (buf.buf);
  آزاد (temp.text);
  بازگشت ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ایستا size_t
open_درون_داد_files (ساختار sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, اندازه *fps);
  صحیح i;

  /* Open as many input files as we can.  */
  برای (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      اگر (!fps[i])
        بشکن;
    }

  بازگشت i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ایستا تهی
mergefps (ساختار sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, نویسه ثابت *output_file, FILE **fps)
{
  ساختار buffer *buffer = xnmalloc (nfiles, اندازه *buffer);
                                /* Input buffers for each file. */
  ساختار سطر saved;		/* Saved line storage for unique check. */
  ساختار سطر ثابت *savedline = پوچ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ساختار سطر ثابت **cur = xnmalloc (nfiles, اندازه *cur);
                                /* Current line in each line table. */
  ساختار سطر ثابت **base = xnmalloc (nfiles, اندازه *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, اندازه *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ساختار keyfield ثابت *کلید = keylist;
  saved.text = پوچ;

  /* Read initial lines from each input file. */
  برای (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], اندازه (ساختار سطر),
               MAX (merge_buffer_size, sort_size / nfiles));
      اگر (fillbuf (&buffer[i], fps[i], files[i].name, پوچ))
        {
          ساختار سطر ثابت *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      وگرنه
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          اگر (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          آزاد (buffer[i].buf);
          --nfiles;
          برای (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  برای (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  برای (size_t i = 1; i < nfiles; ++i)
    اگر (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  وقتی (nfiles)
    {
      ساختار سطر ثابت *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      اگر (unique)
        {
          اگر (savedline && compare (savedline, smallest))
            {
              savedline = پوچ;
              write_سطر (&saved, ofp, output_file);
            }
          اگر (!savedline)
            {
              savedline = &saved;
              اگر (savealloc < smallest->length)
                {
                  انجامبده
                    اگر (! savealloc)
                      {
                        savealloc = smallest->length;
                        بشکن;
                      }
                  وقتی ((savealloc *= 2) < smallest->length);

                  آزاد (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              اگر (کلید)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      وگرنه
        write_سطر (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      اگر (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      وگرنه
        {
          اگر (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       پوچ))
            {
              ساختار سطر ثابت *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          وگرنه
            {
              /* We reached EOF on fps[ord[0]].  */
              برای (size_t i = 1; i < nfiles; ++i)
                اگر (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              اگر (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              آزاد (buffer[ord[0]].buf);
              برای (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              برای (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ادامه;
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
        size_t شمار_of_smaller_سطرها;

        وقتی (lo < hi)
          {
            صحیح cmp = compare (cur[ord0], cur[ord[probe]]);
            اگر (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            وگرنه
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        شمار_of_smaller_سطرها = lo - 1;
        برای (size_t j = 0; j < شمار_of_smaller_سطرها; j++)
          ord[j] = ord[j + 1];
        ord[شمار_of_smaller_سطرها] = ord0;
      }
    }

  اگر (unique && savedline)
    {
      write_سطر (&saved, ofp, output_file);
      آزاد (saved.text);
    }

  xfclose (ofp, output_file);
  آزاد (fps);
  آزاد (buffer);
  آزاد (ord);
  آزاد (base);
  آزاد (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ایستا size_t
mergefiles (ساختار sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, نویسه ثابت *output_file)
{
  FILE **fps;
  size_t nopened = open_درون_داد_files (files, nfiles, &fps);
  اگر (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  بازگشت nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ایستا تهی
mergelines (ساختار سطر *restrict t, size_t nlines,
            ساختار سطر ثابت *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ساختار سطر *hi = t - nlo;

  وقتی (true)
    اگر (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        اگر (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            بازگشت;
          }
      }
    وگرنه
      {
        *--t = *--hi;
        اگر (! --nhi)
          {
            انجامبده
              *--t = *--lo;
            وقتی (--nlo);

            بازگشت;
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

ایستا تهی
sequential_sort (ساختار سطر *restrict سطرها, size_t nlines,
                 ساختار سطر *restrict temp, bool to_temp)
{
  اگر (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحیح swap = (0 < compare (&سطرها[-1], &سطرها[-2]));
      اگر (to_temp)
        {
          temp[-1] = سطرها[-1 - swap];
          temp[-2] = سطرها[-2 + swap];
        }
      وگرنه اگر (swap)
        {
          temp[-1] = سطرها[-1];
          سطرها[-1] = سطرها[-2];
          سطرها[-2] = temp[-1];
        }
    }
  وگرنه
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ساختار سطر *lo = سطرها;
      ساختار سطر *hi = سطرها - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      اگر (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      وگرنه اگر (!to_temp)
        temp[-1] = lo[-1];

      ساختار سطر *dest;
      ساختار سطر ثابت *sorted_lo;
      اگر (to_temp)
        {
          dest = temp;
          sorted_lo = سطرها;
        }
      وگرنه
        {
          dest = سطرها;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ایستا ساختار merge_گره *init_گره (ساختار merge_گره *restrict,
                                     ساختار merge_گره *restrict,
                                     ساختار سطر *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ایستا ساختار merge_گره *
merge_درخت_init (size_t nthreads, size_t nlines, ساختار سطر *dest)
{
  ساختار merge_گره *merge_درخت = xmalloc (2 * اندازه *merge_درخت * nthreads);

  ساختار merge_گره *ریشه = merge_درخت;
  ریشه->lo = ریشه->hi = ریشه->end_lo = ریشه->end_hi = پوچ;
  ریشه->dest = پوچ;
  ریشه->nlo = ریشه->nhi = nlines;
  ریشه->parent = پوچ;
  ریشه->level = MERGE_END;
  ریشه->queued = false;
  pthread_mutex_init (&ریشه->lock, پوچ);

  init_گره (ریشه, ریشه + 1, dest, nthreads, nlines, false);
  بازگشت merge_درخت;
}

/* Destroy the merge tree. */
ایستا تهی
merge_درخت_destroy (size_t nthreads, ساختار merge_گره *merge_درخت)
{
  size_t n_nodes = nthreads * 2;
  ساختار merge_گره *گره = merge_درخت;

  وقتی (n_nodes--)
    {
      pthread_mutex_destroy (&گره->lock);
      گره++;
    }

  آزاد (merge_درخت);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ایستا ساختار merge_گره *
init_گره (ساختار merge_گره *restrict parent,
           ساختار merge_گره *restrict گره_pool,
           ساختار سطر *dest, size_t nthreads,
           size_t total_سطرها, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ساختار سطر *lo = dest - total_سطرها;
  ساختار سطر *hi = lo - nlo;
  ساختار سطر **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  ساختار merge_گره *گره = گره_pool++;
  گره->lo = گره->end_lo = lo;
  گره->hi = گره->end_hi = hi;
  گره->dest = parent_end;
  گره->nlo = nlo;
  گره->nhi = nhi;
  گره->parent = parent;
  گره->level = parent->level + 1;
  گره->queued = false;
  pthread_mutex_init (&گره->lock, پوچ);

  اگر (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      گره->lo_child = گره_pool;
      گره_pool = init_گره (گره, گره_pool, lo, lo_threads,
                             total_سطرها, true);
      گره->hi_child = گره_pool;
      گره_pool = init_گره (گره, گره_pool, hi, hi_threads,
                             total_سطرها, false);
    }
  وگرنه
    {
      گره->lo_child = پوچ;
      گره->hi_child = پوچ;
    }
  بازگشت گره_pool;
}


/* Compare two merge nodes A and B for priority.  */

ایستا صحیح
compare_nodes (تهی ثابت *a, تهی ثابت *b)
{
  ساختار merge_گره ثابت *nodea = a;
  ساختار merge_گره ثابت *nodeb = b;
  اگر (nodea->level == nodeb->level)
      بازگشت (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  بازگشت nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ایستا درونخطی تهی
lock_گره (ساختار merge_گره *گره)
{
  pthread_mutex_lock (&گره->lock);
}

/* Unlock a merge tree NODE. */

ایستا درونخطی تهی
unlock_گره (ساختار merge_گره *گره)
{
  pthread_mutex_unlock (&گره->lock);
}

/* Destroy merge QUEUE. */

ایستا تهی
queue_destroy (ساختار merge_گره_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ایستا تهی
queue_init (ساختار merge_گره_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, پوچ);
  pthread_cond_init (&queue->cond, پوچ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ایستا تهی
queue_درج (ساختار merge_گره_queue *queue, ساختار merge_گره *گره)
{
  pthread_mutex_lock (&queue->mutex);
  heap_درج (queue->priority_queue, گره);
  گره->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ایستا ساختار merge_گره *
queue_برداشت (ساختار merge_گره_queue *queue)
{
  ساختار merge_گره *گره;
  pthread_mutex_lock (&queue->mutex);
  وقتی (! (گره = heap_remove_بالاترین (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_گره (گره);
  گره->queued = false;
  بازگشت گره;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ایستا تهی
write_unique (ساختار سطر ثابت *سطر, FILE *tfp, نویسه ثابت *temp_output)
{
  اگر (unique)
    {
      اگر (saved_سطر.text && ! compare (سطر, &saved_سطر))
        بازگشت;
      saved_سطر = *سطر;
    }

  write_سطر (سطر, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ایستا تهی
mergelines_گره (ساختار merge_گره *restrict گره, size_t total_سطرها,
                 FILE *tfp, نویسه ثابت *temp_output)
{
  ساختار سطر *lo_orig = گره->lo;
  ساختار سطر *hi_orig = گره->hi;
  size_t to_merge = MAX_MERGE (total_سطرها, گره->level);
  size_t merged_lo;
  size_t merged_hi;

  اگر (گره->level > MERGE_ریشه)
    {
      /* Merge to destination buffer. */
      ساختار سطر *dest = *گره->dest;
      وقتی (گره->lo != گره->end_lo && گره->hi != گره->end_hi && to_merge--)
        اگر (compare (گره->lo - 1, گره->hi - 1) <= 0)
          *--dest = *--گره->lo;
        وگرنه
          *--dest = *--گره->hi;

      merged_lo = lo_orig - گره->lo;
      merged_hi = hi_orig - گره->hi;

      اگر (گره->nhi == merged_hi)
        وقتی (گره->lo != گره->end_lo && to_merge--)
          *--dest = *--گره->lo;
      وگرنه اگر (گره->nlo == merged_lo)
        وقتی (گره->hi != گره->end_hi && to_merge--)
          *--dest = *--گره->hi;
      *گره->dest = dest;
    }
  وگرنه
    {
      /* Merge directly to output. */
      وقتی (گره->lo != گره->end_lo && گره->hi != گره->end_hi && to_merge--)
        {
          اگر (compare (گره->lo - 1, گره->hi - 1) <= 0)
            write_unique (--گره->lo, tfp, temp_output);
          وگرنه
            write_unique (--گره->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - گره->lo;
      merged_hi = hi_orig - گره->hi;

      اگر (گره->nhi == merged_hi)
        {
          وقتی (گره->lo != گره->end_lo && to_merge--)
            write_unique (--گره->lo, tfp, temp_output);
        }
      وگرنه اگر (گره->nlo == merged_lo)
        {
          وقتی (گره->hi != گره->end_hi && to_merge--)
            write_unique (--گره->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - گره->lo;
  merged_hi = hi_orig - گره->hi;
  گره->nlo -= merged_lo;
  گره->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ایستا تهی
queue_check_درج (ساختار merge_گره_queue *queue, ساختار merge_گره *گره)
{
  اگر (! گره->queued)
    {
      bool lo_avail = (گره->lo - گره->end_lo) != 0;
      bool hi_avail = (گره->hi - گره->end_hi) != 0;
      اگر (lo_avail ? hi_avail || ! گره->nhi : hi_avail && ! گره->nlo)
        queue_درج (queue, گره);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ایستا تهی
queue_check_درج_parent (ساختار merge_گره_queue *queue,
                           ساختار merge_گره *گره)
{
  اگر (گره->level > MERGE_ریشه)
    {
      lock_گره (گره->parent);
      queue_check_درج (queue, گره->parent);
      unlock_گره (گره->parent);
    }
  وگرنه اگر (گره->nlo + گره->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_درج (queue, گره->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ایستا تهی
merge_loop (ساختار merge_گره_queue *queue,
            size_t total_سطرها, FILE *tfp, نویسه ثابت *temp_output)
{
  وقتی (true)
    {
      ساختار merge_گره *گره = queue_برداشت (queue);

      اگر (گره->level == MERGE_END)
        {
          unlock_گره (گره);
          /* Reinsert so other threads can pop it. */
          queue_درج (queue, گره);
          بشکن;
        }
      mergelines_گره (گره, total_سطرها, tfp, temp_output);
      queue_check_درج (queue, گره);
      queue_check_درج_parent (queue, گره);

      unlock_گره (گره);
    }
}


ایستا تهی sortlines (ساختار سطر *restrict, size_t, size_t,
                       ساختار merge_گره *, ساختار merge_گره_queue *,
                       FILE *, نویسه ثابت *);

/* Thread arguments for sortlines_thread. */

ساختار thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ساختار سطر *سطرها;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت total_سطرها;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ساختار merge_گره *ثابت گره;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ساختار merge_گره_queue *ثابت queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  نویسه ثابت *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ایستا تهی *
sortlines_thread (تهی *data)
{
  ساختار thread_args ثابت *args = data;
  sortlines (args->سطرها, args->nthreads, args->total_سطرها,
             args->گره, args->queue, args->tfp,
             args->output_temp);
  بازگشت پوچ;
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

ایستا تهی
sortlines (ساختار سطر *restrict سطرها, size_t nthreads,
           size_t total_سطرها, ساختار merge_گره *گره,
           ساختار merge_گره_queue *queue, FILE *tfp, نویسه ثابت *temp_output)
{
  size_t nlines = گره->nlo + گره->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  ساختار thread_args args = {سطرها, lo_threads, total_سطرها,
                             گره->lo_child, queue, tfp, temp_output};

  اگر (nthreads > 1 && SUBTHREAD_سطرها_HEURISTIC <= nlines
      && pthread_create (&thread, پوچ, sortlines_thread, &args) == 0)
    {
      sortlines (سطرها - گره->nlo, hi_threads, total_سطرها,
                 گره->hi_child, queue, tfp, temp_output);
      pthread_join (thread, پوچ);
    }
  وگرنه
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = گره->nlo;
      size_t nhi = گره->nhi;
      ساختار سطر *temp = سطرها - total_سطرها;
      اگر (1 < nhi)
        sequential_sort (سطرها - nlo, nhi, temp - nlo / 2, false);
      اگر (1 < nlo)
        sequential_sort (سطرها, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      گره->lo = سطرها;
      گره->hi = سطرها - nlo;
      گره->end_lo = سطرها - nlo;
      گره->end_hi = سطرها - nlo - nhi;

      queue_درج (queue, گره);
      merge_loop (queue, total_سطرها, tfp, temp_output);
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

ایستا تهی
avoid_trashing_درون_داد (ساختار sortfile *files, size_t ntemps,
                      size_t nfiles, نویسه ثابت *outfile)
{
  ساختار tempnode *tempcopy = پوچ;

  برای (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ساختار stat instat;

      اگر (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      وگرنه
        {
          ساختار stat *outst = get_outstatus ();
          اگر (!outst)
            بشکن;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      اگر (same)
        {
          اگر (! tempcopy)
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

ایستا تهی
check_inputs (نویسه *ثابت *files, size_t nfiles)
{
  برای (size_t i = 0; i < nfiles; i++)
    {
      اگر (streq (files[i], "-"))
        ادامه;

      اگر (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ایستا تهی
check_output (نویسه ثابت *outfile)
{
  اگر (outfile)
    {
      صحیح oflags = O_WRONLY | O_دودویی | O_CLOEXEC | O_CREAT;
      صحیح outfd = open (outfile, oflags, MODE_RW_UGO);
      اگر (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ایستا تهی
merge (ساختار sortfile *files, size_t ntemps, size_t nfiles,
       نویسه ثابت *output_file)
{
  وقتی (nmerge < nfiles)
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
      برای (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          ساختار tempnode *temp = create_temp (&tfp);
          size_t رقم_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, رقم_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += رقم_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      اگر (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ساختار tempnode *temp = create_temp (&tfp);
          size_t رقم_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, رقم_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += رقم_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * اندازه *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_درون_داد (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  وقتی (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_درون_داد_files (files, nfiles, &fps);

      اگر (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          اگر (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              بشکن;
            }
          اگر (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      وگرنه اگر (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ساختار tempnode *temp;
      انجامبده
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      وقتی (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * اندازه *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ایستا تهی
sort (نویسه *ثابت *files, size_t nfiles, نویسه ثابت *output_file,
      size_t nthreads)
{
  ساختار buffer buf;
  ساختار sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  وقتی (nfiles)
    {
      نویسه ثابت *temp_output;
      نویسه ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_سطر;
      اگر (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          وقتی (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_سطر = (mult * اندازه (ساختار سطر));
        }
      وگرنه
        bytes_per_سطر = اندازه (ساختار سطر) * 3 / 2;

      اگر (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_سطر,
                              &policy);
          initbuf (&buf, bytes_per_سطر, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      وقتی (fillbuf (&buf, fp, file, &policy))
        {
          ساختار سطر *سطر;

          اگر (buf.eof && nfiles
              && (bytes_per_سطر + 1
                  < (buf.alloc - buf.used - bytes_per_سطر * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.چپ = buf.used;
              بشکن;
            }

          saved_سطر.text = پوچ;
          سطر = buffer_linelim (&buf);
          اگر (buf.eof && !nfiles && !ntemps && !buf.چپ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          وگرنه
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          اگر (1 < buf.nlines)
            {
              اگر (nthreads > 1)
                {
                  ساختار merge_گره_queue queue;
                  queue_init (&queue, nthreads);
                  ساختار merge_گره *merge_درخت =
                    merge_درخت_init (nthreads, buf.nlines, سطر);

                  sortlines (سطر, nthreads, buf.nlines, merge_درخت + 1,
                             &queue, tfp, temp_output);

                  merge_درخت_destroy (nthreads, merge_درخت);
                  queue_destroy (&queue);
                }
              وگرنه
                {
                  sequential_sort (سطر, buf.nlines,
                                   سطر - buf.nlines, false);
                  برای (size_t i = 0; i < buf.nlines; i++)
                    write_unique (سطر - i - 1, tfp, temp_output);
                }
            }
          وگرنه
            write_unique (سطر - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          اگر (output_file_created)
            برو finish;
        }
      xfclose (fp, file);
    }

 finish:
  آزاد (buf.buf);

  اگر (! output_file_created)
    {
      ساختار tempnode *گره = temphead;
      ساختار sortfile *tempfiles = xnmalloc (ntemps, اندازه *tempfiles);
      برای (size_t i = 0; گره; i++)
        {
          tempfiles[i].name = گره->name;
          tempfiles[i].temp = گره;
          گره = گره->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      آزاد (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ایستا تهی
insertkey (ساختار keyfield *کلید_arg)
{
  ساختار keyfield **p;
  ساختار keyfield *کلید = xmemdup (کلید_arg, اندازه *کلید);

  برای (p = &keylist; *p; p = &(*p)->next)
    ادامه;
  *p = کلید;
  کلید->next = پوچ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ایستا تهی
badfieldspec (نویسه ثابت *spec, نویسه ثابت *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ایستا تهی
incompatible_options (نویسه ثابت *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ایستا تهی
check_ordering_compatibility (تهی)
{
  برای (ساختار keyfield *کلید = keylist; کلید; کلید = کلید->next)
    اگر (1 < (کلید->numeric + کلید->general_numeric + کلید->human_numeric
             + کلید->month + (کلید->version | کلید->random | !!کلید->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        نویسه opts[اندازه short_options];
        /* Clear flags we're not interested in.  */
        کلید->skipsblanks = کلید->skipeblanks = کلید->reverse = false;
        کلید_to_opts (کلید, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ایستا نویسه ثابت *
parse_field_شمار (نویسه ثابت *string, size_t *val, نویسه ثابت *msgid)
{
  نویسه *suffix;
  uintmax_t n;

  انتخاب (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    حالت LONGINT_OK:
    حالت LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      اگر (*val == n)
        بشکن;
      FALLTHROUGH;
    حالت LONGINT_OVERFLOW:
    حالت LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      بشکن;

    حالت LONGINT_INVALID:
      اگر (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      بازگشت پوچ;
    }

  بازگشت suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ایستا نویسه *
set_ordering (نویسه ثابت *s, ساختار keyfield *کلید, شمارش blanktype blanktype)
{
  وقتی (*s)
    {
      انتخاب (*s)
        {
        حالت 'b':
          اگر (blanktype == bl_start || blanktype == bl_both)
            کلید->skipsblanks = true;
          اگر (blanktype == bl_end || blanktype == bl_both)
            کلید->skipeblanks = true;
          بشکن;
        حالت 'd':
          کلید->ignore = nondictionary;
          بشکن;
        حالت 'f':
          کلید->translate = fold_toupper;
          بشکن;
        حالت 'g':
          کلید->general_numeric = true;
          بشکن;
        حالت 'h':
          کلید->human_numeric = true;
          بشکن;
        حالت 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          اگر (! کلید->ignore)
            کلید->ignore = nonprinting;
          بشکن;
        حالت 'M':
          کلید->month = true;
          بشکن;
        حالت 'n':
          کلید->numeric = true;
          بشکن;
        حالت 'R':
          کلید->random = true;
          بشکن;
        حالت 'r':
          کلید->reverse = true;
          بشکن;
        حالت 'V':
          کلید->version = true;
          بشکن;
        پیشفرض:
          بازگشت (نویسه *) s;
        }
      ++s;
    }
  بازگشت (نویسه *) s;
}

/* Initialize KEY.  */

ایستا ساختار keyfield *
کلید_init (ساختار keyfield *کلید)
{
  memset (کلید, 0, اندازه *کلید);
  کلید->eword = SIZE_MAX;
  بازگشت کلید;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  ساختار keyfield *کلید;
  ساختار keyfield کلید_buf;
  ساختار keyfield gkey;
  bool gkey_only = false;
  نویسه ثابت *s;
  صحیح c = 0;
  نویسه checkonly = 0;
  bool mergeonly = false;
  نویسه *random_source = پوچ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != پوچ);
  صحیح posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  نویسه **files;
  نویسه *files_from = پوچ;
  ساختار Tokens tok;
  نویسه ثابت *outfile = پوچ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#اگر HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    ساختار lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    اگر (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    اگر (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    اگر (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    شمارش { nsigs = countof (term_sig) };

    ساختار sigaction act;

    sigemptyset (&caught_signals);
    برای (size_t i = 0; i < nsigs; i++)
      {
        اگر (term_sig[i] == SIGPIPE)
          ادامه;  /* Handled below.  */
        sigaction (term_sig[i], پوچ, &act);
        اگر (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    برای (size_t i = 0; i < nsigs; i++)
      {
        اگر (term_sig[i] == SIGPIPE)
          ادامه;  /* Handled below.  */
        اگر (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, پوچ);
      }
  }
  سیگنال (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  کلید_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, اندازه *files);

  وقتی (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      صحیح oi = -1;

      اگر (c == -1
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
          اگر (argc <= optind)
            بشکن;
          files[nfiles++] = argv[optind++];
        }
      وگرنه انتخاب (c)
        {
        حالت 1:
          کلید = پوچ;
          اگر (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              اگر (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  کلید = کلید_init (&کلید_buf);
                  s = parse_field_شمار (optarg + 1, &کلید->sword, پوچ);
                  اگر (s && *s == '.')
                    s = parse_field_شمار (s + 1, &کلید->schar, پوچ);
                  اگر (! (کلید->sword || کلید->schar))
                    کلید->sword = SIZE_MAX;
                  اگر (! s || *set_ordering (s, کلید, bl_start))
                    کلید = پوچ;
                  وگرنه
                    {
                      اگر (minus_pos_usage)
                        {
                          نویسه ثابت *optarg1 = argv[optind++];
                          s = parse_field_شمار (optarg1 + 1, &کلید->eword,
                                             N_("invalid number after '-'"));
                          اگر (*s == '.')
                            s = parse_field_شمار (s + 1, &کلید->echar,
                                               N_("invalid number after '.'"));
                          اگر (!کلید->echar && کلید->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              کلید->eword--;
                            }
                          اگر (*set_ordering (s, کلید, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      کلید->traditional_used = true;
                      insertkey (کلید);
                    }
                }
            }
          اگر (! کلید)
            files[nfiles++] = optarg;
          بشکن;

        حالت SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        حالت 'b':
        حالت 'd':
        حالت 'f':
        حالت 'g':
        حالت 'h':
        حالت 'i':
        حالت 'M':
        حالت 'n':
        حالت 'r':
        حالت 'R':
        حالت 'V':
          {
            نویسه str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          بشکن;

        حالت CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        حالت 'c':
        حالت 'C':
          اگر (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          بشکن;

        حالت COMPRESS_PROGRAM_OPTION:
          اگر (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          بشکن;

        حالت DEBUG_PROGRAM_OPTION:
          debug = true;
          بشکن;

        حالت FILES0_FROM_OPTION:
          files_from = optarg;
          بشکن;

        حالت 'k':
          کلید = کلید_init (&کلید_buf);

          /* Get POS1. */
          s = parse_field_شمار (optarg, &کلید->sword,
                                 N_("invalid number at field start"));
          اگر (! کلید->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          اگر (*s == '.')
            {
              s = parse_field_شمار (s + 1, &کلید->schar,
                                     N_("invalid number after '.'"));
              اگر (! کلید->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          اگر (! (کلید->sword || کلید->schar))
            کلید->sword = SIZE_MAX;
          s = set_ordering (s, کلید, bl_start);
          اگر (*s != ',')
            {
              کلید->eword = SIZE_MAX;
              کلید->echar = 0;
            }
          وگرنه
            {
              /* Get POS2. */
              s = parse_field_شمار (s + 1, &کلید->eword,
                                     N_("invalid number after ','"));
              اگر (! کلید->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              اگر (*s == '.')
                {
                  s = parse_field_شمار (s + 1, &کلید->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, کلید, bl_end);
            }
          اگر (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (کلید);
          بشکن;

        حالت 'm':
          mergeonly = true;
          بشکن;

        حالت NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          بشکن;

        حالت 'o':
          اگر (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          بشکن;

        حالت RANDOM_SOURCE_OPTION:
          اگر (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          بشکن;

        حالت 's':
          stable = true;
          بشکن;

        حالت 'S':
          specify_sort_size (oi, c, optarg);
          بشکن;

        حالت 't':
          {
            نویسه newtab = optarg[0];
            اگر (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            اگر (optarg[1])
              {
                اگر (streq (optarg, "\\0"))
                  newtab = '\0';
                وگرنه
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            اگر (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          بشکن;

        حالت 'T':
          add_temp_dir (optarg);
          بشکن;

        حالت PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          بشکن;

        حالت 'u':
          unique = true;
          بشکن;

        حالت 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          اگر (optarg == argv[optind - 1])
            {
              نویسه ثابت *p;
              برای (p = optarg; c_isdigit (*p); p++)
                ادامه;
              optind -= (*p != '\0');
            }
          بشکن;

        حالت 'z':
          eolchar = 0;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (سیگنال (SIGPIPE, SIG_IGN) == SIG_DFL);

  اگر (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      اگر (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      اگر (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      اگر (tok.n_tok)
        {
          آزاد (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          برای (size_t i = 0; i < nfiles; i++)
            {
              اگر (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              وگرنه اگر (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long صحیح file_عدد = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_عدد);
                }
            }
        }
      وگرنه
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  برای (کلید = keylist; کلید; کلید = کلید->next)
    {
      اگر (default_کلید_compare (کلید) && !کلید->reverse)
        {
          کلید->ignore = gkey.ignore;
          کلید->translate = gkey.translate;
          کلید->skipsblanks = gkey.skipsblanks;
          کلید->skipeblanks = gkey.skipeblanks;
          کلید->month = gkey.month;
          کلید->numeric = gkey.numeric;
          کلید->general_numeric = gkey.general_numeric;
          کلید->human_numeric = gkey.human_numeric;
          کلید->version = gkey.version;
          کلید->random = gkey.random;
          کلید->reverse = gkey.reverse;
        }

      need_random |= کلید->random;
    }

  اگر (!keylist && !default_کلید_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  اگر (debug)
    {
      اگر (checkonly || outfile)
        {
          ایستا نویسه opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      اگر (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      اگر (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      اگر (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, پوچ)));
      وگرنه
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      کلید_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  اگر (need_random)
    random_md5_state_init (random_source);

  اگر (temp_dirs.n_dirs == 0)
    {
      نویسه ثابت *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  اگر (nfiles == 0)
    {
      nfiles = 1;
      آزاد (files);
      files = xmalloc (اندازه *files);
      *files = (نویسه *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  اگر (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  اگر (checkonly)
    {
      اگر (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      اگر (outfile)
        {
          ایستا نویسه opts[] = {0, 'o', 0};
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

  اگر (mergeonly)
    {
      ساختار sortfile *sortfiles = xcalloc (nfiles, اندازه *sortfiles);

      برای (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  وگرنه
    {
      اگر (!nthreads)
        {
          unsigned long صحیح np = رقم_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * اندازه (ساختار merge_گره));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
