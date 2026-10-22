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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <pthread.h>
#ضمن <sys/resource.h>
#ضمن <sys/types.h>
#ضمن <sys/wait.h>
#ضمن <signal.h>
#ضمن <spawn.h>
#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "filevercmp.h"
#ضمن "findprog.h"
#ضمن "flexmember.h"
#ضمن "hard-locale.h"
#ضمن "hash.h"
#ضمن "heap.h"
#ضمن "ignore-value.h"
#ضمن "mbswidth.h"
#ضمن "nproc.h"
#ضمن "physmem.h"
#ضمن "posixver.h"
#ضمن "quote.h"
#ضمن "randread.h"
#ضمن "readtokens0.h"
#ضمن "stdlib--.h"
#ضمن "strnumcmp.h"
#ضمن "term-sig.h"
#ضمن "unistd--.h"
#ضمن "xmemcoll.h"
#ضمن "xnanosleep.h"
#ضمن "xstrtol.h"
#ضمن "xstrtol-error.h"

#ifndef RLIMIT_DATA
بنية rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#אם HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#אם !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#אם !defined OPEN_MAX
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
تعداد { SUBTHREAD_LINES_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_LINES_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
تعداد { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
تعداد
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

تعداد
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

تعداد
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_ROOT = 1
  };

/* The representation of the decimal point in the current locale.  */
ساكن حرف decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
ساكن صحيح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساكن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساكن bool hard_LC_COLLATE;
#אם HAVE_NL_LANGINFO
ساكن bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
تعداد blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
بنية line
{
  حرف *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  حرف *keybeg;			/* Start of first key. */
  حرف *keylim;			/* Limit of first key. */
};

/* Input buffers. */
بنية buffer
{
  حرف *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t left;			/* Number of bytes left from previous reads. */
  size_t line_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
بنية sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
بنية keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ثابت *ignore;		/* Boolean array of characters to ignore. */
  حرف ثابت *translate;	/* Translation applied to characters. */
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
  بنية keyfield *next;	/* Next keyfield to try. */
};

بنية month
{
  حرف ثابت *name;
  صحيح val;
};

/* Binary merge tree node. */
بنية merge_node
{
  بنية line *lo;              /* Lines to merge from LO child node. */
  بنية line *hi;              /* Lines to merge from HI child node. */
  بنية line *end_lo;          /* End of available lines from LO. */
  بنية line *end_hi;          /* End of available lines from HI. */
  بنية line **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  بنية merge_node *parent;    /* Parent node. */
  بنية merge_node *lo_child;  /* LO child node. */
  بنية merge_node *hi_child;  /* HI child node. */
  unsigned صحيح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
بنية merge_node_queue
{
  بنية heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساكن بنية line saved_line;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ساكن bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ساكن bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ساكن bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ساكن حرف fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ساكن بنية month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + حجم (بنية line))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ساكن size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ساكن size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define INPUT_FILE_SIZE_GUESS (128 * 1024)

ساكن بنية
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  حرف ثابت **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
ساكن bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ساكن bool stable;

/* An int value outside char range.  */
تعداد { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
تعداد { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ساكن صحيح tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ساكن bool unique;

/* Nonzero if any of the input files are the standard input. */
ساكن bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ساكن بنية keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ساكن حرف ثابت *compress_program;

/* Annotate the output with extra info to aid the user.  */
ساكن bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ساكن unsigned صحيح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ساكن bool default_SIGPIPE;

/* The list of temporary files. */
بنية tempnode
{
  بنية tempnode *متطاير next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  حرف state;
  حرف name[FLEXIBLE_ARRAY_MEMBER];
};
ساكن بنية tempnode *متطاير temphead;
ساكن بنية tempnode *متطاير *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ساكن فراغ
cleanup (فراغ)
{
  עבור (بنية tempnode ثابت *node = temphead; node; node = node->next)
    unlink (node->name);
  temphead = عدم;
}

/* Handle interrupts and hangups. */

ساكن فراغ
sighandler (صحيح sig)
{
  cleanup ();

  إشارة (sig, SIG_DFL);
  ارفع (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ساكن فراغ
sort_die (حرف ثابت *message, حرف ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  אם (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
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
         ignore leading blanks when finding sort keys in each line\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالة\n\
         fold lower حالة to upper حالة characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical value\n\
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
         compare according to string numerical value;\n\
         see full documentation עבור supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical keys.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; עבור more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check עבור sorted input; עשה not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but עשה not report first bad line\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the line used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ input from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, اقرأ names from standard input\n\
"));
      oputs (_("\
  -k, --key=KEYDEF\n\
         sort via a key; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; עשה not sort\n\
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
         use SIZE עבור main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR עבור temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the number of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of lines with equal keys;\n\
         with -c, check עבור strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] עבור start and stop position, where F is a\n\
field number and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the line's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options עבור that key.  If no key is given, use\n\
the entire line as the key.  Use --debug to diagnose incorrect key usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (افتراضي), and so on עבור M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte values.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
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

ساكن حرف ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ساكن بنية option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_argument, عدم, 'b'},
  {"check", optional_argument, عدم, CHECK_OPTION},
  {"compress-program", required_argument, عدم, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, عدم, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, عدم, 'd'},
  {"ignore-case", no_argument, عدم, 'f'},
  {"files0-from", required_argument, عدم, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, عدم, 'g'},
  {"ignore-nonprinting", no_argument, عدم, 'i'},
  {"key", required_argument, عدم, 'k'},
  {"merge", no_argument, عدم, 'm'},
  {"month-sort", no_argument, عدم, 'M'},
  {"numeric-sort", no_argument, عدم, 'n'},
  {"human-numeric-sort", no_argument, عدم, 'h'},
  {"version-sort", no_argument, عدم, 'V'},
  {"random-sort", no_argument, عدم, 'R'},
  {"random-source", required_argument, عدم, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, عدم, SORT_OPTION},
  {"output", required_argument, عدم, 'o'},
  {"reverse", no_argument, عدم, 'r'},
  {"stable", no_argument, عدم, 's'},
  {"batch-size", required_argument, عدم, NMERGE_OPTION},
  {"buffer-size", required_argument, عدم, 'S'},
  {"field-separator", required_argument, عدم, 't'},
  {"temporary-directory", required_argument, عدم, 'T'},
  {"unique", no_argument, عدم, 'u'},
  {"zero-terminated", no_argument, عدم, 'z'},
  {"parallel", required_argument, عدم, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ساكن حرف ثابت *ثابت check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE عدم
#undef  _ct_
};
ساكن حرف ثابت check_types[] =
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

ساكن حرف ثابت *ثابت sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE عدم
#undef  _st_
};
ساكن حرف ثابت sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
ساكن sigset_t caught_signals;

/* Critical section status.  */
بنية cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ساكن فراغ
cs_enter (بنية cs_status *status)
{
  صحيح ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
ساكن فراغ
cs_leave (بنية cs_status ثابت *status)
{
  אם (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, عدم);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
تعداد { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
بنية sortfile
{
  /* The file's name.  */
  حرف ثابت *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  بنية tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ساكن Hash_table *proctab;

تعداد { INIT_PROCTAB_SIZE = 47 };

ساكن size_t
proctab_hasher (فراغ ثابت *entry, size_t tabsize)
{
  بنية tempnode ثابت *node = entry;
  החזר node->pid % tabsize;
}

ساكن bool
proctab_comparator (فراغ ثابت *e1, فراغ ثابت *e2)
{
  بنية tempnode ثابت *n1 = e1;
  بنية tempnode ثابت *n2 = e2;
  החזר n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ساكن pid_t nprocs;

ساكن bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ساكن pid_t
reap (pid_t pid)
{
  صحيح status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  אם (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  אחרת אם (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      אם (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  החזר cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساكن فراغ
register_proc (بنية tempnode *temp)
{
  אם (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, عدم,
                                 proctab_hasher,
                                 proctab_comparator,
                                 عدم);
      אם (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  אם (! hash_insert (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساكن bool
delete_proc (pid_t pid)
{
  بنية tempnode test;

  test.pid = pid;
  بنية tempnode *node = hash_remove (proctab, &test);
  אם (! node)
    החזר false;
  node->state = REAPED;
  החזר true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ساكن فراغ
wait_proc (pid_t pid)
{
  אם (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ساكن فراغ
reap_exited (فراغ)
{
  כלעוד (0 < nprocs && reap (0))
    استمر;
}

/* Reap at least one exited child, waiting if necessary.  */

ساكن فراغ
reap_some (فراغ)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ساكن فراغ
reap_all (فراغ)
{
  כלעוד (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ساكن فراغ
exit_cleanup (فراغ)
{
  אם (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      بنية cs_status cs;
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

ساكن بنية tempnode *
create_temp_file (صحيح *pfd, bool survive_fd_exhaustion)
{
  ساكن حرف ثابت slashbase[] = "/sortXXXXXX";
  ساكن idx_t temp_dir_index;
  صحيح fd;
  صحيح saved_errno;
  حرف ثابت *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  بنية tempnode *node =
    xmalloc (FLEXSIZEOF (بنية tempnode, name, len + حجم slashbase));
  حرف *file = node->name;
  بنية cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, حجم slashbase);
  node->next = عدم;
  אם (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  אם (0 <= fd)
    {
      *temptail = node;
      temptail = &node->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  אם (fd < 0)
    {
      אם (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      حرر (node);
      node = عدم;
    }

  *pfd = fd;
  החזר node;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ساكن بنية stat *
get_outstatus (فراغ)
{
  ساكن صحيح outstat_errno;
  ساكن بنية stat outstat;
  אם (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  החזר outstat_errno < 0 ? &outstat : عدم;
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

ساكن FILE *
stream_open (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp;

  אם (*how == 'r')
    {
      אם (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      אחרת
        {
          صحيح fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? عدم : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  אחרת אם (*how == 'w')
    {
      אם (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          صحيح ftruncate_errno = errno;
          بنية stat *outst = get_outstatus ();
          אם (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  אחרת
    affirm (!"unexpected mode passed to stream_open");

  החזר fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ساكن FILE *
xfopen (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp = stream_open (file, how);
  אם (!fp)
    sort_die (_("open failed"), file);
  החזר fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ساكن فراغ
xfclose (FILE *fp, حرف ثابت *file)
{
  بدل (fileno (fp))
    {
    حالة STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      اكسر;

    حالة STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      אם (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      اكسر;

    افتراضي:
      אם (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      اكسر;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ساكن فراغ
move_fd (صحيح oldfd, صحيح newfd)
{
  אם (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_value (dup2 (oldfd, newfd));
      ignore_value (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساكن صحيح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحيح oldfd, صحيح newfd)
{
  صحيح result = 0;
  אם (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      אם (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  החזר result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساكن حرف ثابت *
get_resolved_compress_program (فراغ)
{
  /* Use a cache, to perform the search only once.  */
  ساكن حرف ثابت *resolved_compress_program_cache /* = NULL */;

  אם (resolved_compress_program_cache == عدم)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), عدم, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  החזר resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ساكن صحيح
pipe_child (pid_t *pid, صحيح pipefds[2], صحيح tempfd, bool decompress,
            size_t tries)
{
  حرف ثابت *resolved_compress_program;
  بنية tempnode *saved_temphead;
  مزدوج wait_retry = 0.25;
  بنية cs_status cs;
  صحيح result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  אם (resolved_compress_program == عدم)
    החזר errno;

  אם ((result = posix_spawnattr_init (&attr)))
    החזר result;
  אם ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      החזר result;
    }

  אם (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحيح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      החזר saved_errno;
    }

  אם ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      החזר result;
    }

  حرف ثابت *ثابت argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : عدم,
      عدم
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  אם (nmerge + 1 < nprocs)
    reap_some ();

  כלעוד (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = عدم;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (حرف * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      אם (result != EAGAIN)
        اكسر;
      אחרת
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

  אם (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  אחרת
    ++nprocs;

  החזר result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ساكن بنية tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  صحيح tempfd;
  بنية tempnode *node = create_temp_file (&tempfd, survive_fd_exhaustion);
  אם (! node)
    החזר عدم;

  node->state = UNCOMPRESSED;

  אם (compress_program)
    {
      صحيح pipefds[2];
      ساكن صحيح last_result = 0;

      صحيح result = pipe_child (&node->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      אם (result)
        {
          אם (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      אחרת
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (node);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  אם (! *pfp)
    sort_die (_("couldn't create temporary file"), node->name);

  החזר node;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساكن بنية tempnode *
create_temp (FILE **pfp)
{
  החזר maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ساكن FILE *
open_temp (بنية tempnode *temp)
{
  صحيح tempfd, pipefds[2];
  FILE *fp = عدم;

  אם (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  אם (tempfd < 0)
    החזר عدم;

  pid_t child;
  صحيح result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  אם (result)
    {
      אם (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  אחרת
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      אם (! fp)
        {
          صحيح saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  החזר fp;
}

/* Append DIR to the array of temporary directory names.  */
ساكن فراغ
add_temp_dir (حرف ثابت *dir)
{
  אם (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              حجم *temp_dirs.dirs);

  idx_t ثابت dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ساكن فراغ
zaptemp (حرف ثابت *name)
{
  بنية tempnode *متطاير *pnode;
  بنية tempnode *node;
  بنية tempnode *next;
  صحيح unlink_status;
  صحيح unlink_errno = 0;
  بنية cs_status cs;

  עבור (pnode = &temphead; (node = *pnode)->name != name; pnode = &node->next)
    استمر;

  אם (node->state == UNREAPED)
    wait_proc (node->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = node->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  אם (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  אם (! next)
    temptail = pnode;
  حرر (node);
}

#אם HAVE_NL_LANGINFO

ساكن صحيح
struct_month_cmp (فراغ ثابت *m1, فراغ ثابت *m2)
{
  بنية month ثابت *month1 = m1;
  بنية month ثابت *month2 = m2;
  החזר strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

ساكن فراغ
inittables (فراغ)
{
  עבור (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#אם HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  אם (hard_LC_TIME)
    {
      עבור (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          עבור (j = k = 0; j < s_len; j++)
            אם (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, حجم *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ساكن فراغ
specify_nmerge (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  بنية rlimit rlimit;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned صحيح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  אם (e == LONGINT_OK)
    {
      nmerge = n;
      אם (nmerge != n)
        e = LONGINT_OVERFLOW;
      אחרת
        {
          אם (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          אחרת אם (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          אחרת
            החזר;
        }
    }

  אם (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  אחרת
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ساكن فراغ
specify_sort_size (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  حرف *suffix;
  تعداد strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  אם (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  אם (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    بدل (suffix[0])
      {
      حالة 'b':
        e = LONGINT_OK;
        اكسر;

      حالة '%':
        {
          مزدوج mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          אם (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          אחרת
            e = LONGINT_OVERFLOW;
        }
        اكسر;
      }

  אם (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      אם (n < sort_size)
        החזר;

      sort_size = n;
      אם (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          החזר;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ساكن size_t
specify_nthreads (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t nthreads;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &nthreads, "");
  אם (e == LONGINT_OVERFLOW)
    החזר SIZE_MAX;
  אם (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  אם (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  אם (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  החזר nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ساكن size_t
default_sort_size (فراغ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  بنية rlimit rlimit;
  אם (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  אם (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  אם (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  مزدوج avail = physmem_available ();
  مزدوج total = physmem_total ();
  مزدوج mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  אם (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  אם (mem < size)
    size = mem;
  החזר MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساكن bool
line_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = حجم (بنية line);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  אם (ckd_add (&aligned, size, padding))
    החזר false;

  *alloc = aligned;
  החזר true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساكن size_t
default_initial_sort_size (size_t line_bytes)
{
  size_t size = line_bytes + 2;
  size_t input_size;

  אם (ckd_mul (&input_size, INPUT_FILE_SIZE_GUESS, line_bytes + 1)
      || ckd_add (&size, size, input_size))
    החזר SIZE_MAX;

  החזר MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساكن bool
input_size_buffer_bytes (uintmax_t file_size, size_t line_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_input_byte = line_bytes + 1;

  אם (SIZE_MAX / worst_case_per_input_byte < file_size)
    החזר false;

  size_t size = file_size * worst_case_per_input_byte;
  אם (SIZE_MAX - size < 1)
    החזר false;

  *worst_case = size + 1;
  החזר true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ساكن فراغ
sort_buffer_policy (FILE *ثابت *fps, size_t nfps,
                    حرف *ثابت *files, size_t nfiles,
                    size_t line_bytes,
                    بنية sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (line_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_input_byte = line_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_input_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  עבור (size_t i = 0; i < nfiles; i++)
    {
      بنية stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      אם (size_bound <= size)
        {
          policy->initial = size_bound;
          החזר;
        }

      אם ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      אם (known_size)
        file_size = st.st_size;
      אחרת
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          אם (sort_size)
            {
              policy->initial = size_bound;
              החזר;
            }
          file_size = INPUT_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      אם (! input_size_buffer_bytes (file_size, line_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          החזר;
        }

      אם (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          החזר;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساكن فراغ
initbuf (بنية buffer *buf, size_t line_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  כלעוד (true)
    {
      אם (! line_aligned_size (&alloc))
        buf->buf = عدم;
      אחרת
        buf->buf = malloc (alloc);
      אם (buf->buf)
        اكسر;
      alloc /= 2;
      אם (alloc <= line_bytes + 1)
        xalloc_die ();
    }

  buf->line_bytes = line_bytes;
  buf->alloc = alloc;
  buf->used = buf->left = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساكن مضمن بنية line *
buffer_linelim (بنية buffer ثابت *buf)
{
  فراغ *linelim = buf->buf + buf->alloc;
  החזר linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ساكن bool
try_growbuf (بنية buffer *buf, size_t alloc)
{
  אם (! line_aligned_size (&alloc) || alloc <= buf->alloc)
    החזר false;

  حرف *newbuf = malloc (alloc);
  אם (! newbuf)
    החזר false;

  حرف *oldbuf = buf->buf;
  بنية line *old_linelim = buffer_linelim (buf);
  بنية line *old_line = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  بنية line *new_linelim = (فراغ *) (newbuf + alloc);
  بنية line *new_line = new_linelim - buf->nlines;
  memcpy (new_line, old_line, buf->nlines * حجم *new_line);

  עבור (بنية line *line = new_line; line < new_linelim; line++)
    {
      line->text = newbuf + (line->text - oldbuf);
      אם (keylist)
        {
          line->keybeg = newbuf + (line->keybeg - oldbuf);
          line->keylim = newbuf + (line->keylim - oldbuf);
        }
    }

  حرر (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  החזר true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساكن bool
maybe_growbuf (بنية buffer *buf, بنية sort_buffer_policy *policy)
{
  אם (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    החזר false;

  size_t alloc;
  אם (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  אחרת
    alloc = policy->limit;

  אם (try_growbuf (buf, alloc))
    החזר true;

  policy->growth_failed = true;
  החזר false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ساكن حرف *
begfield (بنية line ثابت *line, بنية keyfield ثابت *key)
{
  حرف *ptr = line->text, *lim = ptr + line->length - 1;
  size_t sword = key->sword;
  size_t schar = key->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  אם (tab != TAB_DEFAULT)
    כלעוד (ptr < lim && sword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        אם (ptr < lim)
          ++ptr;
      }
  אחרת
    כלעוד (ptr < lim && sword--)
      {
        כלעוד (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        כלעוד (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  אם (key->skipsblanks)
    כלעוד (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  אם (schar < remaining_bytes)
    ptr += schar;
  אחרת
    ptr = lim;

  החזר ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
ساكن حرف *
limfield (بنية line ثابت *line, بنية keyfield ثابت *key)
{
  حرف *ptr = line->text, *lim = ptr + line->length - 1;
  size_t eword = key->eword, echar = key->echar;

  אם (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  אם (tab != TAB_DEFAULT)
    כלעוד (ptr < lim && eword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        אם (ptr < lim && (eword || echar))
          ++ptr;
      }
  אחרת
    כלעוד (ptr < lim && eword--)
      {
        כלעוד (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        כלעוד (ptr < lim && !blanks[to_uchar (*ptr)])
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
  אם (tab != TAB_DEFAULT)
    {
      حرف *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      אם (newlim)
        lim = newlim;
    }
  אחרת
    {
      حرف *newlim;
      newlim = ptr;
      כלעוד (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      כלעוד (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  אם (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      אם (key->skipeblanks)
        כלעוד (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      אם (echar < remaining_bytes)
        ptr += echar;
      אחרת
        ptr = lim;
    }

  החזר ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ساكن bool
fillbuf (بنية buffer *buf, FILE *fp, حرف ثابت *file,
         بنية sort_buffer_policy *policy)
{
  بنية keyfield ثابت *key = keylist;
  حرف eol = eolchar;
  size_t line_bytes = buf->line_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  אם (buf->eof)
    החזר false;

  אם (buf->used != buf->left)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->left, buf->left);
      buf->used = buf->left;
      buf->nlines = 0;
    }

  כלעוד (true)
    {
      حرف *ptr = buf->buf + buf->used;
      بنية line *linelim = buffer_linelim (buf);
      بنية line *line = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * line_bytes - ptr;
      حرف *line_start = buf->nlines ? line->text + line->length : buf->buf;

      כלעוד (line_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (line_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + bytes_read;
          حرف *p;
          avail -= bytes_read;

          אם (bytes_read != readsize)
            {
              אם (ferror (fp))
                sort_die (_("read failed"), file);
              אם (feof (fp))
                {
                  buf->eof = true;
                  אם (buf->buf == ptrlim)
                    החזר false;
                  אם (line_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          כלעוד ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              line--;
              line->text = line_start;
              line->length = ptr - line_start;
              mergesize = MAX (mergesize, line->length);
              avail -= line_bytes;

              אם (key)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  line->keylim = (key->eword == SIZE_MAX
                                  ? p
                                  : limfield (line, key));

                  אם (key->sword != SIZE_MAX)
                    line->keybeg = begfield (line, key);
                  אחרת
                    {
                      אם (key->skipsblanks)
                        כלעוד (blanks[to_uchar (*line_start)])
                          line_start++;
                      line->keybeg = line_start;
                    }
                }

              line_start = ptr;
            }

          ptr = ptrlim;
          אם (buf->eof)
            اكسر;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - line;
      אם (buf->nlines != 0)
        {
          אם (! buf->eof && maybe_growbuf (buf, policy))
            استمر;
          buf->left = ptr - line_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          החזר true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t line_alloc = buf->alloc / حجم (بنية line);
        buf->buf = xpalloc (buf->buf, &line_alloc, 1, -1, حجم (بنية line));
        buf->alloc = line_alloc * حجم (بنية line);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ساكن حرف ثابت unit_order[UCHAR_LIM] =
  {
#אם ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#אחרת
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
ساكن حرف
traverse_raw_number (حرف ثابت **number)
{
  حرف ثابت *p = *number;
  حرف ch;
  حرف max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  כלעוד (c_isdigit (ch = *p++))
    {
      אם (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      אם (ends_with_thousands_sep)
        ++p;
    }

  אם (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *number = p - 2;
      החזר max_digit;
    }

  אם (ch == decimal_point)
    כלעוד (c_isdigit (ch = *p++))
      אם (max_digit < ch)
        max_digit = ch;

  *number = p - 1;
  החזר max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
ساكن صحيح
find_unit_order (حرف ثابت *number)
{
  bool minus_sign = (*number == '-');
  حرف ثابت *p = number + minus_sign;
  حرف max_digit = traverse_raw_number (&p);
  אם ('0' < max_digit)
    {
      unsigned حرف ch = *p;
      صحيح order = unit_order[ch];
      החזר (minus_sign ? -order : order);
    }
  אחרת
    החזר 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
ساكن صحيح
human_numcompare (حرف ثابت *a, حرف ثابت *b)
{
  כלעוד (blanks[to_uchar (*a)])
    a++;
  כלעוד (blanks[to_uchar (*b)])
    b++;

  صحيح diff = find_unit_order (a) - find_unit_order (b);
  החזר (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
ساكن صحيح
numcompare (حرف ثابت *a, حرف ثابت *b)
{
  כלעוד (blanks[to_uchar (*a)])
    a++;
  כלעוד (blanks[to_uchar (*b)])
    b++;

  החזר strnumcmp (a, b, decimal_point, thousands_sep);
}

ساكن صحيح
nan_compare (long مزدوج a, long مزدوج b)
{
  حرف buf[2][حجم "-nan""()" + CHAR_BIT * حجم a];
  snprintf (buf[0], حجم buf[0], "%Lf", a);
  snprintf (buf[1], حجم buf[1], "%Lf", b);
  החזר strcmp (buf[0], buf[1]);
}

ساكن صحيح
general_numcompare (حرف ثابت *sa, حرف ثابت *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  حرف *ea;
  حرف *eb;
  long مزدوج a = strtold (sa, &ea);
  long مزدوج b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  אם (sa == ea)
    החזר sb == eb ? 0 : -1;
  אם (sb == eb)
    החזר 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  החזר (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ساكن صحيح
getmonth (حرف ثابت *month, حرف **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  כלעוד (blanks[to_uchar (*month)])
    month++;

  עשה
    {
      size_t ix = (lo + hi) / 2;
      حرف ثابت *m = month;
      حرف ثابت *n = monthtab[ix].name;

      עבור (;; m++, n++)
        {
          אם (!*n)
            {
              אם (ea)
                *ea = (حرف *) m;
              החזר monthtab[ix].val;
            }
          אם (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              اكسر;
            }
          אחרת אם (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              اكسر;
            }
        }
    }
  כלעוד (lo < hi);

  החזר 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#אם DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# אם 14 <= __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#ضمن "md5.h"

#אם DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# אם 14 <= __GNUC__
#  pragma GCC diagnostic pop
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
ساكن فراغ
link_failure (فراغ)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ساكن فراغ *
symbol_address (فراغ *handle, حرف ثابت *symbol)
{
  فراغ *address = dlsym (handle, symbol);
  אם (!address)
    link_failure ();
  החזר address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
ساكن فراغ
link_libcrypto (فراغ)
{
#אם DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  فراغ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  אם (!handle)
    link_failure ();
  ptr_MD5_Init = symbol_address (handle, "MD5_Init");
  ptr_MD5_Update = symbol_address (handle, "MD5_Update");
  ptr_MD5_Final = symbol_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
ساكن بنية md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ساكن فراغ
random_md5_state_init (حرف ثابت *random_source)
{
  unsigned حرف buf[MD5_DIGEST_SIZE];
  بنية randread_source *r = randread_new (random_source, حجم buf);
  אם (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, حجم buf);
  אם (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, حجم buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ساكن size_t
xstrxfrm (حرف *restrict dest, حرف ثابت *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  אם (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  החזר translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ساكن صحيح
compare_random (حرف *restrict texta, size_t lena,
                حرف *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  صحيح xfrm_diff = 0;

  حرف stackbuf[4000];
  حرف *buf = stackbuf;
  size_t bufsize = حجم stackbuf;
  فراغ *allocated = عدم;
  uint32_t dig[2][MD5_DIGEST_SIZE / حجم (uint32_t)];
  بنية md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  אם (hard_LC_COLLATE)
    {
      حرف ثابت *lima = texta + lena;
      حرف ثابت *limb = textb + lenb;

      כלעוד (true)
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
          אם (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              حرر (allocated);
              buf = allocated = malloc (bufsize);
              אם (! buf)
                {
                  buf = stackbuf;
                  bufsize = حجم stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : عدم), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          אם (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              אם (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              حرر (allocated);
              buf = allocated = xmalloc (bufsize);
              אם (texta < lima)
                strxfrm (buf, texta, sizea);
              אם (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          אם (texta < lima)
            texta += strlen (texta) + 1;
          אם (textb < limb)
            textb += strlen (textb) + 1;
          אם (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              اكسر;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          אם (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              אם (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  صحيح diff = memcmp (dig[0], dig[1], حجم dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  אם (! diff)
    {
      אם (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          אם (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  حرر (allocated);

  החזר diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ساكن size_t
debug_width (حرف ثابت *text, حرف ثابت *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  כלעוד (text < lim)
    width += (*text++ == '\t');
  החזר width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساكن فراغ
mark_key (size_t offset, size_t width)
{
  כלעוד (offset--)
    putchar (' ');

  אם (!width)
    printf (_("^ no match for key\n"));
  אחרת
    {
      עשה
        putchar ('_');
      כלעוד (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساكن مضمن bool
key_numeric (بنية keyfield ثابت *key)
{
  החזר key->numeric || key->general_numeric || key->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساكن فراغ
debug_key (بنية line ثابت *line, بنية keyfield ثابت *key)
{
  حرف *text = line->text;
  حرف *beg = text;
  حرف *lim = text + line->length - 1;

  אם (key)
    {
      אם (key->sword != SIZE_MAX)
        beg = begfield (line, key);
      אם (key->eword != SIZE_MAX)
        {
          lim = limfield (line, key);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      אם ((key->skipsblanks && key->sword == SIZE_MAX)
          || key->month || key_numeric (key))
        {
          حرف saved = *lim;
          *lim = '\0';

          כלעוד (blanks[to_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          אם (lim < beg)
            tighter_lim = lim;
          אחרת אם (key->month)
            getmonth (beg, &tighter_lim);
          אחרת אם (key->general_numeric)
            ignore_value (strtold (beg, &tighter_lim));
          אחרת אם (key->numeric || key->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_number (&p);
              אם ('0' <= max_digit)
                {
                  unsigned حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (key->human_numeric && unit_order[ch]);
                }
            }
          אחרת
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_key (offset, width);
}

/* Debug LINE by underlining its keys.  */

ساكن فراغ
debug_line (بنية line ثابت *line)
{
  بنية keyfield ثابت *key = keylist;

  עשה
    debug_key (line, key);
  כלעוד (key && ((key = key->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساكن bool
default_key_compare (بنية keyfield ثابت *key)
{
  החזר ! (key->ignore
            || key->translate
            || key->skipsblanks
            || key->skipeblanks
            || key_numeric (key)
            || key->month
            || key->version
            || key->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساكن فراغ
key_to_opts (بنية keyfield ثابت *key, حرف *opts)
{
  אם (key->skipsblanks || key->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  אם (key->ignore == nondictionary)
    *opts++ = 'd';
  אם (key->translate)
    *opts++ = 'f';
  אם (key->general_numeric)
    *opts++ = 'g';
  אם (key->human_numeric)
    *opts++ = 'h';
  אם (key->ignore == nonprinting)
    *opts++ = 'i';
  אם (key->month)
    *opts++ = 'M';
  אם (key->numeric)
    *opts++ = 'n';
  אם (key->random)
    *opts++ = 'R';
  אם (key->reverse)
    *opts++ = 'r';
  אם (key->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساكن فراغ
key_warnings (بنية keyfield ثابت *gkey, bool gkey_only)
{
  بنية keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  עבור (بنية keyfield *key = keylist; key; key = key->next, keynum++)
    {
      אם (key_numeric (key))
        {
          אם (key->general_numeric)
            general_numeric_field = true;
          אחרת
            basic_numeric_field = true;
        }

      אם (key->traditional_used)
        {
          size_t sword = key->sword;
          size_t eword = key->eword;
          حرف tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          حرف obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          حرف nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          حرف *po = obuf;
          حرف *pn = nbuf;

          אם (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          אם (key->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (key->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = key->sword != SIZE_MAX && key->eword < key->sword;
      אם (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = key_numeric (key) || key->month;
      bool line_offset = key->eword == 0 && key->echar != 0; /* -k1.x,1.y  */
      אם (!zero_width && !gkey_only && tab == TAB_DEFAULT && !line_offset
          && ((!key->skipsblanks && !implicit_skip)
              || (!key->skipsblanks && key->schar)
              || (!key->skipeblanks && key->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      אם (!gkey_only && key_numeric (key))
        {
          size_t sword = key->sword + 1;
          size_t eword = key->eword + 1;
          אם (!sword)
            sword++;
          אם (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              אם (key->general_numeric)
                general_numeric_field_span = true;
              אחרת
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      אם (ugkey.ignore && (ugkey.ignore == key->ignore))
        ugkey.ignore = عدم;
      אם (ugkey.translate && (ugkey.translate == key->translate))
        ugkey.translate = عدم;
      ugkey.skipsblanks &= !key->skipsblanks;
      ugkey.skipeblanks &= !key->skipeblanks;
      ugkey.month &= !key->month;
      ugkey.numeric &= !key->numeric;
      ugkey.general_numeric &= !key->general_numeric;
      ugkey.human_numeric &= !key->human_numeric;
      ugkey.random &= !key->random;
      ugkey.version &= !key->version;
      ugkey.reverse &= !key->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool number_locale_warned = false;
  אם (basic_numeric_field_span)
    {
      אם (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          number_locale_warned = true;
        }
    }
  אם (basic_numeric_field_span || general_numeric_field_span)
    {
      אם (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_point, 0})));
          number_locale_warned = true;
        }
      אחרת אם (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      אחרת אם (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  אם ((basic_numeric_field || general_numeric_field) && ! number_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_point, 0})));

    }

  אם (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  אם (!default_key_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      אם (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      key_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  אם (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ساكن صحيح
diff_reversed (صحيح diff, bool reversed)
{
  החזר reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ساكن صحيح
keycompare (بنية line ثابت *a, بنية line ثابت *b)
{
  بنية keyfield *key = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحيح diff;

  כלעוד (true)
    {
      حرف ثابت *translate = key->translate;
      bool ثابت *ignore = key->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      אם (hard_LC_COLLATE || key_numeric (key)
          || key->month || key->random || key->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          حرف *ta = texta;
          حرف *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          حرف enda = ta[tlena];
          حرف endb = tb[tlenb];

          فراغ *allocated = عدم;
          حرف stackbuf[4000];

          אם (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              אם (size <= حجم stackbuf)
                ta = stackbuf;
              אחרת
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              עבור (tlena = i = 0; i < lena; i++)
                אם (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              עבור (tlenb = i = 0; i < lenb; i++)
                אם (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          אם (key->numeric)
            diff = numcompare (ta, tb);
          אחרת אם (key->general_numeric)
            diff = general_numcompare (ta, tb);
          אחרת אם (key->human_numeric)
            diff = human_numcompare (ta, tb);
          אחרת אם (key->month)
            diff = getmonth (ta, عدم) - getmonth (tb, عدم);
          אחרת אם (key->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          אחרת אם (key->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          אחרת
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              אם (tlena == 0)
                diff = - NONZERO (tlenb);
              אחרת אם (tlenb == 0)
                diff = 1;
              אחרת
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          حرر (allocated);
        }
      אחרת אם (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  עשה									\
    {									\
          כלעוד (true)							\
            {								\
              כלעוד (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              כלעוד (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              אם (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  اكسر;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              אם (diff)							\
                اكسر;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  כלעוד (0)

          אם (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          אחרת
            CMP_WITH_IGNORE (*texta, *textb);
        }
      אחרת
        {
          size_t lenmin = MIN (lena, lenb);
          אם (lenmin == 0)
            diff = 0;
          אחרת אם (translate)
            {
              size_t i = 0;
              עשה
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  אם (diff)
                    اكسر;
                  i++;
                }
              כלעוד (i < lenmin);
            }
          אחרת
            diff = memcmp (texta, textb, lenmin);

          אם (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      אם (diff)
        اكسر;

      key = key->next;
      אם (! key)
        החזר 0;

      /* Find the beginning and limit of the next field.  */
      אם (key->eword != SIZE_MAX)
        lima = limfield (a, key), limb = limfield (b, key);
      אחרת
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      אם (key->sword != SIZE_MAX)
        texta = begfield (a, key), textb = begfield (b, key);
      אחרת
        {
          texta = a->text, textb = b->text;
          אם (key->skipsblanks)
            {
              כלעוד (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              כלעוד (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  החזר diff_reversed (diff, key->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساكن صحيح
compare (بنية line ثابت *a, بنية line ثابت *b)
{
  صحيح diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  אם (keylist)
    {
      diff = keycompare (a, b);
      אם (diff || unique || stable)
        החזר diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  אם (alen == 0)
    diff = - NONZERO (blen);
  אחרת אם (blen == 0)
    diff = 1;
  אחרת אם (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  אחרת
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      אם (!diff)
        diff = _GL_CMP (alen, blen);
    }

  החזר diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساكن فراغ
write_line (بنية line ثابت *line, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = line->text;
  size_t n_bytes = line->length;
  حرف *ebuf = buf + n_bytes;

  אם (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      حرف ثابت *c = buf;

      כלעוד (c < ebuf)
        {
          حرف wc = *c++;
          אם (wc == '\t')
            wc = '>';
          אחרת אם (c == ebuf)
            wc = '\n';
          אם (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_line (line);
    }
  אחרת
    {
      ebuf[-1] = eolchar;
      אם (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ساكن bool
check (حرف ثابت *file_name, حرف checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  بنية buffer buf;		/* Input buffer. */
  بنية line temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t line_number = 0;
  بنية keyfield ثابت *key = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (بنية line),
           MAX (merge_buffer_size, sort_size));
  temp.text = عدم;

  כלעוד (fillbuf (&buf, fp, file_name, عدم))
    {
      بنية line ثابت *line = buffer_linelim (&buf);
      بنية line ثابت *linebase = line - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      אם (alloc && nonunique <= compare (&temp, line - 1))
        {
        found_disorder:
          {
            אם (checkonly == 'c')
              {
                بنية line ثابت *disorder_line = line - 1;
                uintmax_t disorder_line_number =
                  buffer_linelim (&buf) - disorder_line + line_number;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_line_number);
                write_line (disorder_line, stderr, _("standard error"));
              }

            ordered = false;
            اكسر;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      כלעוד (linebase < --line)
        אם (nonunique <= compare (line, line - 1))
          اذهب found_disorder;

      line_number += buf.nlines;

      /* Save the last line of the buffer.  */
      אם (alloc < line->length)
        {
          עשה
            {
              alloc *= 2;
              אם (! alloc)
                {
                  alloc = line->length;
                  اكسر;
                }
            }
          כלעוד (alloc < line->length);

          حرر (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, line->text, line->length);
      temp.length = line->length;
      אם (key)
        {
          temp.keybeg = temp.text + (line->keybeg - line->text);
          temp.keylim = temp.text + (line->keylim - line->text);
        }
    }

  xfclose (fp, file_name);
  حرر (buf.buf);
  حرر (temp.text);
  החזר ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساكن size_t
open_input_files (بنية sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحيح i;

  /* Open as many input files as we can.  */
  עבור (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      אם (!fps[i])
        اكسر;
    }

  החזר i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ساكن فراغ
mergefps (بنية sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, حرف ثابت *output_file, FILE **fps)
{
  بنية buffer *buffer = xnmalloc (nfiles, حجم *buffer);
                                /* Input buffers for each file. */
  بنية line saved;		/* Saved line storage for unique check. */
  بنية line ثابت *savedline = عدم;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  بنية line ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  بنية line ثابت **base = xnmalloc (nfiles, حجم *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  بنية keyfield ثابت *key = keylist;
  saved.text = عدم;

  /* Read initial lines from each input file. */
  עבור (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], حجم (بنية line),
               MAX (merge_buffer_size, sort_size / nfiles));
      אם (fillbuf (&buffer[i], fps[i], files[i].name, عدم))
        {
          بنية line ثابت *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      אחרת
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          אם (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          حرر (buffer[i].buf);
          --nfiles;
          עבור (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  עבור (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  עבור (size_t i = 1; i < nfiles; ++i)
    אם (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  כלעוד (nfiles)
    {
      بنية line ثابت *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      אם (unique)
        {
          אם (savedline && compare (savedline, smallest))
            {
              savedline = عدم;
              write_line (&saved, ofp, output_file);
            }
          אם (!savedline)
            {
              savedline = &saved;
              אם (savealloc < smallest->length)
                {
                  עשה
                    אם (! savealloc)
                      {
                        savealloc = smallest->length;
                        اكسر;
                      }
                  כלעוד ((savealloc *= 2) < smallest->length);

                  حرر (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              אם (key)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      אחרת
        write_line (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      אם (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      אחרת
        {
          אם (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       عدم))
            {
              بنية line ثابت *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          אחרת
            {
              /* We reached EOF on fps[ord[0]].  */
              עבור (size_t i = 1; i < nfiles; ++i)
                אם (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              אם (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              حرر (buffer[ord[0]].buf);
              עבור (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              עבור (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              استمر;
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
        size_t count_of_smaller_lines;

        כלעוד (lo < hi)
          {
            صحيح cmp = compare (cur[ord0], cur[ord[probe]]);
            אם (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            אחרת
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        count_of_smaller_lines = lo - 1;
        עבור (size_t j = 0; j < count_of_smaller_lines; j++)
          ord[j] = ord[j + 1];
        ord[count_of_smaller_lines] = ord0;
      }
    }

  אם (unique && savedline)
    {
      write_line (&saved, ofp, output_file);
      حرر (saved.text);
    }

  xfclose (ofp, output_file);
  حرر (fps);
  حرر (buffer);
  حرر (ord);
  حرر (base);
  حرر (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ساكن size_t
mergefiles (بنية sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, حرف ثابت *output_file)
{
  FILE **fps;
  size_t nopened = open_input_files (files, nfiles, &fps);
  אם (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  החזר nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساكن فراغ
mergelines (بنية line *restrict t, size_t nlines,
            بنية line ثابت *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية line *hi = t - nlo;

  כלעוד (true)
    אם (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        אם (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            החזר;
          }
      }
    אחרת
      {
        *--t = *--hi;
        אם (! --nhi)
          {
            עשה
              *--t = *--lo;
            כלעוד (--nlo);

            החזר;
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

ساكن فراغ
sequential_sort (بنية line *restrict lines, size_t nlines,
                 بنية line *restrict temp, bool to_temp)
{
  אם (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحيح swap = (0 < compare (&lines[-1], &lines[-2]));
      אם (to_temp)
        {
          temp[-1] = lines[-1 - swap];
          temp[-2] = lines[-2 + swap];
        }
      אחרת אם (swap)
        {
          temp[-1] = lines[-1];
          lines[-1] = lines[-2];
          lines[-2] = temp[-1];
        }
    }
  אחרת
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      بنية line *lo = lines;
      بنية line *hi = lines - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      אם (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      אחרת אם (!to_temp)
        temp[-1] = lo[-1];

      بنية line *dest;
      بنية line ثابت *sorted_lo;
      אם (to_temp)
        {
          dest = temp;
          sorted_lo = lines;
        }
      אחרת
        {
          dest = lines;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساكن بنية merge_node *init_node (بنية merge_node *restrict,
                                     بنية merge_node *restrict,
                                     بنية line *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساكن بنية merge_node *
merge_tree_init (size_t nthreads, size_t nlines, بنية line *dest)
{
  بنية merge_node *merge_tree = xmalloc (2 * حجم *merge_tree * nthreads);

  بنية merge_node *root = merge_tree;
  root->lo = root->hi = root->end_lo = root->end_hi = عدم;
  root->dest = عدم;
  root->nlo = root->nhi = nlines;
  root->parent = عدم;
  root->level = MERGE_END;
  root->queued = false;
  pthread_mutex_init (&root->lock, عدم);

  init_node (root, root + 1, dest, nthreads, nlines, false);
  החזר merge_tree;
}

/* Destroy the merge tree. */
ساكن فراغ
merge_tree_destroy (size_t nthreads, بنية merge_node *merge_tree)
{
  size_t n_nodes = nthreads * 2;
  بنية merge_node *node = merge_tree;

  כלעוד (n_nodes--)
    {
      pthread_mutex_destroy (&node->lock);
      node++;
    }

  حرر (merge_tree);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساكن بنية merge_node *
init_node (بنية merge_node *restrict parent,
           بنية merge_node *restrict node_pool,
           بنية line *dest, size_t nthreads,
           size_t total_lines, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية line *lo = dest - total_lines;
  بنية line *hi = lo - nlo;
  بنية line **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  بنية merge_node *node = node_pool++;
  node->lo = node->end_lo = lo;
  node->hi = node->end_hi = hi;
  node->dest = parent_end;
  node->nlo = nlo;
  node->nhi = nhi;
  node->parent = parent;
  node->level = parent->level + 1;
  node->queued = false;
  pthread_mutex_init (&node->lock, عدم);

  אם (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      node->lo_child = node_pool;
      node_pool = init_node (node, node_pool, lo, lo_threads,
                             total_lines, true);
      node->hi_child = node_pool;
      node_pool = init_node (node, node_pool, hi, hi_threads,
                             total_lines, false);
    }
  אחרת
    {
      node->lo_child = عدم;
      node->hi_child = عدم;
    }
  החזר node_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساكن صحيح
compare_nodes (فراغ ثابت *a, فراغ ثابت *b)
{
  بنية merge_node ثابت *nodea = a;
  بنية merge_node ثابت *nodeb = b;
  אם (nodea->level == nodeb->level)
      החזר (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  החזר nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساكن مضمن فراغ
lock_node (بنية merge_node *node)
{
  pthread_mutex_lock (&node->lock);
}

/* Unlock a merge tree NODE. */

ساكن مضمن فراغ
unlock_node (بنية merge_node *node)
{
  pthread_mutex_unlock (&node->lock);
}

/* Destroy merge QUEUE. */

ساكن فراغ
queue_destroy (بنية merge_node_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساكن فراغ
queue_init (بنية merge_node_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, عدم);
  pthread_cond_init (&queue->cond, عدم);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ساكن فراغ
queue_insert (بنية merge_node_queue *queue, بنية merge_node *node)
{
  pthread_mutex_lock (&queue->mutex);
  heap_insert (queue->priority_queue, node);
  node->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساكن بنية merge_node *
queue_pop (بنية merge_node_queue *queue)
{
  بنية merge_node *node;
  pthread_mutex_lock (&queue->mutex);
  כלעוד (! (node = heap_remove_top (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_node (node);
  node->queued = false;
  החזר node;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساكن فراغ
write_unique (بنية line ثابت *line, FILE *tfp, حرف ثابت *temp_output)
{
  אם (unique)
    {
      אם (saved_line.text && ! compare (line, &saved_line))
        החזר;
      saved_line = *line;
    }

  write_line (line, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساكن فراغ
mergelines_node (بنية merge_node *restrict node, size_t total_lines,
                 FILE *tfp, حرف ثابت *temp_output)
{
  بنية line *lo_orig = node->lo;
  بنية line *hi_orig = node->hi;
  size_t to_merge = MAX_MERGE (total_lines, node->level);
  size_t merged_lo;
  size_t merged_hi;

  אם (node->level > MERGE_ROOT)
    {
      /* Merge to destination buffer. */
      بنية line *dest = *node->dest;
      כלעוד (node->lo != node->end_lo && node->hi != node->end_hi && to_merge--)
        אם (compare (node->lo - 1, node->hi - 1) <= 0)
          *--dest = *--node->lo;
        אחרת
          *--dest = *--node->hi;

      merged_lo = lo_orig - node->lo;
      merged_hi = hi_orig - node->hi;

      אם (node->nhi == merged_hi)
        כלעוד (node->lo != node->end_lo && to_merge--)
          *--dest = *--node->lo;
      אחרת אם (node->nlo == merged_lo)
        כלעוד (node->hi != node->end_hi && to_merge--)
          *--dest = *--node->hi;
      *node->dest = dest;
    }
  אחרת
    {
      /* Merge directly to output. */
      כלעוד (node->lo != node->end_lo && node->hi != node->end_hi && to_merge--)
        {
          אם (compare (node->lo - 1, node->hi - 1) <= 0)
            write_unique (--node->lo, tfp, temp_output);
          אחרת
            write_unique (--node->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - node->lo;
      merged_hi = hi_orig - node->hi;

      אם (node->nhi == merged_hi)
        {
          כלעוד (node->lo != node->end_lo && to_merge--)
            write_unique (--node->lo, tfp, temp_output);
        }
      אחרת אם (node->nlo == merged_lo)
        {
          כלעוד (node->hi != node->end_hi && to_merge--)
            write_unique (--node->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - node->lo;
  merged_hi = hi_orig - node->hi;
  node->nlo -= merged_lo;
  node->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساكن فراغ
queue_check_insert (بنية merge_node_queue *queue, بنية merge_node *node)
{
  אם (! node->queued)
    {
      bool lo_avail = (node->lo - node->end_lo) != 0;
      bool hi_avail = (node->hi - node->end_hi) != 0;
      אם (lo_avail ? hi_avail || ! node->nhi : hi_avail && ! node->nlo)
        queue_insert (queue, node);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساكن فراغ
queue_check_insert_parent (بنية merge_node_queue *queue,
                           بنية merge_node *node)
{
  אם (node->level > MERGE_ROOT)
    {
      lock_node (node->parent);
      queue_check_insert (queue, node->parent);
      unlock_node (node->parent);
    }
  אחרת אם (node->nlo + node->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_insert (queue, node->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساكن فراغ
merge_loop (بنية merge_node_queue *queue,
            size_t total_lines, FILE *tfp, حرف ثابت *temp_output)
{
  כלעוד (true)
    {
      بنية merge_node *node = queue_pop (queue);

      אם (node->level == MERGE_END)
        {
          unlock_node (node);
          /* Reinsert so other threads can pop it. */
          queue_insert (queue, node);
          اكسر;
        }
      mergelines_node (node, total_lines, tfp, temp_output);
      queue_check_insert (queue, node);
      queue_check_insert_parent (queue, node);

      unlock_node (node);
    }
}


ساكن فراغ sortlines (بنية line *restrict, size_t, size_t,
                       بنية merge_node *, بنية merge_node_queue *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

بنية thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  بنية line *lines;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت total_lines;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  بنية merge_node *ثابت node;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  بنية merge_node_queue *ثابت queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  حرف ثابت *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ساكن فراغ *
sortlines_thread (فراغ *data)
{
  بنية thread_args ثابت *args = data;
  sortlines (args->lines, args->nthreads, args->total_lines,
             args->node, args->queue, args->tfp,
             args->output_temp);
  החזר عدم;
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

ساكن فراغ
sortlines (بنية line *restrict lines, size_t nthreads,
           size_t total_lines, بنية merge_node *node,
           بنية merge_node_queue *queue, FILE *tfp, حرف ثابت *temp_output)
{
  size_t nlines = node->nlo + node->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  بنية thread_args args = {lines, lo_threads, total_lines,
                             node->lo_child, queue, tfp, temp_output};

  אם (nthreads > 1 && SUBTHREAD_LINES_HEURISTIC <= nlines
      && pthread_create (&thread, عدم, sortlines_thread, &args) == 0)
    {
      sortlines (lines - node->nlo, hi_threads, total_lines,
                 node->hi_child, queue, tfp, temp_output);
      pthread_join (thread, عدم);
    }
  אחרת
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = node->nlo;
      size_t nhi = node->nhi;
      بنية line *temp = lines - total_lines;
      אם (1 < nhi)
        sequential_sort (lines - nlo, nhi, temp - nlo / 2, false);
      אם (1 < nlo)
        sequential_sort (lines, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      node->lo = lines;
      node->hi = lines - nlo;
      node->end_lo = lines - nlo;
      node->end_hi = lines - nlo - nhi;

      queue_insert (queue, node);
      merge_loop (queue, total_lines, tfp, temp_output);
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

ساكن فراغ
avoid_trashing_input (بنية sortfile *files, size_t ntemps,
                      size_t nfiles, حرف ثابت *outfile)
{
  بنية tempnode *tempcopy = عدم;

  עבור (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      بنية stat instat;

      אם (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      אחרת
        {
          بنية stat *outst = get_outstatus ();
          אם (!outst)
            اكسر;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      אם (same)
        {
          אם (! tempcopy)
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

ساكن فراغ
check_inputs (حرف *ثابت *files, size_t nfiles)
{
  עבור (size_t i = 0; i < nfiles; i++)
    {
      אם (streq (files[i], "-"))
        استمر;

      אם (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ساكن فراغ
check_output (حرف ثابت *outfile)
{
  אם (outfile)
    {
      صحيح oflags = O_WRONLY | O_BINARY | O_CLOEXEC | O_CREAT;
      صحيح outfd = open (outfile, oflags, MODE_RW_UGO);
      אם (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ساكن فراغ
merge (بنية sortfile *files, size_t ntemps, size_t nfiles,
       حرف ثابت *output_file)
{
  כלעוד (nmerge < nfiles)
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
      עבור (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          بنية tempnode *temp = create_temp (&tfp);
          size_t num_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, num_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += num_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      אם (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          بنية tempnode *temp = create_temp (&tfp);
          size_t num_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, num_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += num_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * حجم *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_input (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  כלעוד (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_input_files (files, nfiles, &fps);

      אם (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          אם (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              اكسر;
            }
          אם (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      אחרת אם (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      بنية tempnode *temp;
      עשה
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      כלעוד (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * حجم *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ساكن فراغ
sort (حرف *ثابت *files, size_t nfiles, حرف ثابت *output_file,
      size_t nthreads)
{
  بنية buffer buf;
  بنية sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  כלעוד (nfiles)
    {
      حرف ثابت *temp_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_line;
      אם (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          כלעוד (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_line = (mult * حجم (بنية line));
        }
      אחרת
        bytes_per_line = حجم (بنية line) * 3 / 2;

      אם (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_line,
                              &policy);
          initbuf (&buf, bytes_per_line, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      כלעוד (fillbuf (&buf, fp, file, &policy))
        {
          بنية line *line;

          אם (buf.eof && nfiles
              && (bytes_per_line + 1
                  < (buf.alloc - buf.used - bytes_per_line * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.left = buf.used;
              اكسر;
            }

          saved_line.text = عدم;
          line = buffer_linelim (&buf);
          אם (buf.eof && !nfiles && !ntemps && !buf.left)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          אחרת
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          אם (1 < buf.nlines)
            {
              אם (nthreads > 1)
                {
                  بنية merge_node_queue queue;
                  queue_init (&queue, nthreads);
                  بنية merge_node *merge_tree =
                    merge_tree_init (nthreads, buf.nlines, line);

                  sortlines (line, nthreads, buf.nlines, merge_tree + 1,
                             &queue, tfp, temp_output);

                  merge_tree_destroy (nthreads, merge_tree);
                  queue_destroy (&queue);
                }
              אחרת
                {
                  sequential_sort (line, buf.nlines,
                                   line - buf.nlines, false);
                  עבור (size_t i = 0; i < buf.nlines; i++)
                    write_unique (line - i - 1, tfp, temp_output);
                }
            }
          אחרת
            write_unique (line - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          אם (output_file_created)
            اذهب finish;
        }
      xfclose (fp, file);
    }

 finish:
  حرر (buf.buf);

  אם (! output_file_created)
    {
      بنية tempnode *node = temphead;
      بنية sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      עבור (size_t i = 0; node; i++)
        {
          tempfiles[i].name = node->name;
          tempfiles[i].temp = node;
          node = node->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      حرر (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساكن فراغ
insertkey (بنية keyfield *key_arg)
{
  بنية keyfield **p;
  بنية keyfield *key = xmemdup (key_arg, حجم *key);

  עבור (p = &keylist; *p; p = &(*p)->next)
    استمر;
  *p = key;
  key->next = عدم;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ساكن فراغ
badfieldspec (حرف ثابت *spec, حرف ثابت *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ساكن فراغ
incompatible_options (حرف ثابت *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ساكن فراغ
check_ordering_compatibility (فراغ)
{
  עבור (بنية keyfield *key = keylist; key; key = key->next)
    אם (1 < (key->numeric + key->general_numeric + key->human_numeric
             + key->month + (key->version | key->random | !!key->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        key->skipsblanks = key->skipeblanks = key->reverse = false;
        key_to_opts (key, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساكن حرف ثابت *
parse_field_count (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
{
  حرف *suffix;
  uintmax_t n;

  بدل (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    حالة LONGINT_OK:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      אם (*val == n)
        اكسر;
      FALLTHROUGH;
    حالة LONGINT_OVERFLOW:
    حالة LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      اكسر;

    حالة LONGINT_INVALID:
      אם (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      החזר عدم;
    }

  החזר suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ساكن حرف *
set_ordering (حرف ثابت *s, بنية keyfield *key, تعداد blanktype blanktype)
{
  כלעוד (*s)
    {
      بدل (*s)
        {
        حالة 'b':
          אם (blanktype == bl_start || blanktype == bl_both)
            key->skipsblanks = true;
          אם (blanktype == bl_end || blanktype == bl_both)
            key->skipeblanks = true;
          اكسر;
        حالة 'd':
          key->ignore = nondictionary;
          اكسر;
        حالة 'f':
          key->translate = fold_toupper;
          اكسر;
        حالة 'g':
          key->general_numeric = true;
          اكسر;
        حالة 'h':
          key->human_numeric = true;
          اكسر;
        حالة 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          אם (! key->ignore)
            key->ignore = nonprinting;
          اكسر;
        حالة 'M':
          key->month = true;
          اكسر;
        حالة 'n':
          key->numeric = true;
          اكسر;
        حالة 'R':
          key->random = true;
          اكسر;
        حالة 'r':
          key->reverse = true;
          اكسر;
        حالة 'V':
          key->version = true;
          اكسر;
        افتراضي:
          החזר (حرف *) s;
        }
      ++s;
    }
  החזר (حرف *) s;
}

/* Initialize KEY.  */

ساكن بنية keyfield *
key_init (بنية keyfield *key)
{
  memset (key, 0, حجم *key);
  key->eword = SIZE_MAX;
  החזר key;
}

صحيح
main (صحيح argc, حرف **argv)
{
  بنية keyfield *key;
  بنية keyfield key_buf;
  بنية keyfield gkey;
  bool gkey_only = false;
  حرف ثابت *s;
  صحيح c = 0;
  حرف checkonly = 0;
  bool mergeonly = false;
  حرف *random_source = عدم;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  صحيح posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  حرف **files;
  حرف *files_from = عدم;
  بنية Tokens tok;
  حرف ثابت *outfile = عدم;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#אם HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    بنية lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    אם (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    אם (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    אם (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    تعداد { nsigs = countof (term_sig) };

    بنية sigaction act;

    sigemptyset (&caught_signals);
    עבור (size_t i = 0; i < nsigs; i++)
      {
        אם (term_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        sigaction (term_sig[i], عدم, &act);
        אם (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    עבור (size_t i = 0; i < nsigs; i++)
      {
        אם (term_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        אם (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, عدم);
      }
  }
  إشارة (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  key_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, حجم *files);

  כלעוד (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      صحيح oi = -1;

      אם (c == -1
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
          אם (argc <= optind)
            اكسر;
          files[nfiles++] = argv[optind++];
        }
      אחרת بدل (c)
        {
        حالة 1:
          key = عدم;
          אם (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              אם (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  key = key_init (&key_buf);
                  s = parse_field_count (optarg + 1, &key->sword, عدم);
                  אם (s && *s == '.')
                    s = parse_field_count (s + 1, &key->schar, عدم);
                  אם (! (key->sword || key->schar))
                    key->sword = SIZE_MAX;
                  אם (! s || *set_ordering (s, key, bl_start))
                    key = عدم;
                  אחרת
                    {
                      אם (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_field_count (optarg1 + 1, &key->eword,
                                             N_("invalid number after '-'"));
                          אם (*s == '.')
                            s = parse_field_count (s + 1, &key->echar,
                                               N_("invalid number after '.'"));
                          אם (!key->echar && key->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              key->eword--;
                            }
                          אם (*set_ordering (s, key, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      key->traditional_used = true;
                      insertkey (key);
                    }
                }
            }
          אם (! key)
            files[nfiles++] = optarg;
          اكسر;

        حالة SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        حالة 'b':
        حالة 'd':
        حالة 'f':
        حالة 'g':
        حالة 'h':
        حالة 'i':
        حالة 'M':
        حالة 'n':
        حالة 'r':
        حالة 'R':
        حالة 'V':
          {
            حرف str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          اكسر;

        حالة CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        حالة 'c':
        حالة 'C':
          אם (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          اكسر;

        حالة COMPRESS_PROGRAM_OPTION:
          אם (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          اكسر;

        حالة DEBUG_PROGRAM_OPTION:
          debug = true;
          اكسر;

        حالة FILES0_FROM_OPTION:
          files_from = optarg;
          اكسر;

        حالة 'k':
          key = key_init (&key_buf);

          /* Get POS1. */
          s = parse_field_count (optarg, &key->sword,
                                 N_("invalid number at field start"));
          אם (! key->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          אם (*s == '.')
            {
              s = parse_field_count (s + 1, &key->schar,
                                     N_("invalid number after '.'"));
              אם (! key->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          אם (! (key->sword || key->schar))
            key->sword = SIZE_MAX;
          s = set_ordering (s, key, bl_start);
          אם (*s != ',')
            {
              key->eword = SIZE_MAX;
              key->echar = 0;
            }
          אחרת
            {
              /* Get POS2. */
              s = parse_field_count (s + 1, &key->eword,
                                     N_("invalid number after ','"));
              אם (! key->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              אם (*s == '.')
                {
                  s = parse_field_count (s + 1, &key->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, key, bl_end);
            }
          אם (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (key);
          اكسر;

        حالة 'm':
          mergeonly = true;
          اكسر;

        حالة NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          اكسر;

        حالة 'o':
          אם (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          اكسر;

        حالة RANDOM_SOURCE_OPTION:
          אם (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          اكسر;

        حالة 's':
          stable = true;
          اكسر;

        حالة 'S':
          specify_sort_size (oi, c, optarg);
          اكسر;

        حالة 't':
          {
            حرف newtab = optarg[0];
            אם (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            אם (optarg[1])
              {
                אם (streq (optarg, "\\0"))
                  newtab = '\0';
                אחרת
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            אם (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          اكسر;

        حالة 'T':
          add_temp_dir (optarg);
          اكسر;

        حالة PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          اكسر;

        حالة 'u':
          unique = true;
          اكسر;

        حالة 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          אם (optarg == argv[optind - 1])
            {
              حرف ثابت *p;
              עבור (p = optarg; c_isdigit (*p); p++)
                استمر;
              optind -= (*p != '\0');
            }
          اكسر;

        حالة 'z':
          eolchar = 0;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (إشارة (SIGPIPE, SIG_IGN) == SIG_DFL);

  אם (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      אם (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      אם (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      אם (tok.n_tok)
        {
          حرر (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          עבור (size_t i = 0; i < nfiles; i++)
            {
              אם (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              אחרת אם (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long صحيح file_number = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_number);
                }
            }
        }
      אחרת
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  עבור (key = keylist; key; key = key->next)
    {
      אם (default_key_compare (key) && !key->reverse)
        {
          key->ignore = gkey.ignore;
          key->translate = gkey.translate;
          key->skipsblanks = gkey.skipsblanks;
          key->skipeblanks = gkey.skipeblanks;
          key->month = gkey.month;
          key->numeric = gkey.numeric;
          key->general_numeric = gkey.general_numeric;
          key->human_numeric = gkey.human_numeric;
          key->version = gkey.version;
          key->random = gkey.random;
          key->reverse = gkey.reverse;
        }

      need_random |= key->random;
    }

  אם (!keylist && !default_key_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  אם (debug)
    {
      אם (checkonly || outfile)
        {
          ساكن حرف opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      אם (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      אם (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      אם (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, عدم)));
      אחרת
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      key_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  אם (need_random)
    random_md5_state_init (random_source);

  אם (temp_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  אם (nfiles == 0)
    {
      nfiles = 1;
      حرر (files);
      files = xmalloc (حجم *files);
      *files = (حرف *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  אם (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  אם (checkonly)
    {
      אם (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      אם (outfile)
        {
          ساكن حرف opts[] = {0, 'o', 0};
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

  אם (mergeonly)
    {
      بنية sortfile *sortfiles = xcalloc (nfiles, حجم *sortfiles);

      עבור (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  אחרת
    {
      אם (!nthreads)
        {
          unsigned long صحيح np = num_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (بنية merge_node));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  אם (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
