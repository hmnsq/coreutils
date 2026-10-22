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

#include <config.h>

#include <getopt.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <spawn.h>
#include "system.h"
#include "argmatch.h"
#include "assure.h"
#include "c-ctype.h"
#include "fadvise.h"
#include "filevercmp.h"
#include "findprog.h"
#include "flexmember.h"
#include "hard-locale.h"
#include "hash.h"
#include "heap.h"
#include "ignore-value.h"
#include "mbswidth.h"
#include "nproc.h"
#include "physmem.h"
#include "posixver.h"
#include "quote.h"
#include "randread.h"
#include "readtokens0.h"
#include "stdlib--.h"
#include "strnumcmp.h"
#include "term-sig.h"
#include "unistd--.h"
#include "xmemcoll.h"
#include "xnanosleep.h"
#include "xstrtol.h"
#include "xstrtol-error.h"

#ifndef RLIMIT_DATA
sa_nracanaa rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ပေႃး HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#ပေႃး !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#ပေႃး !defined OPEN_MAX
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
ga_nanaa { SUBTHREAD_LINES_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_LINES_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ga_nanaa { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
ga_nanaa
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

ga_nanaa
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

ga_nanaa
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_ROOT = 1
  };

/* The representation of the decimal point in the current locale.  */
static var_na decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
static puur_naa_nka thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
static bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
static bool hard_LC_COLLATE;
#ပေႃး HAVE_NL_LANGINFO
static bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ga_nanaa blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
static var_na eolchar = '\n';

/* Lines are held in memory as counted strings. */
sa_nracanaa line
{
  var_na *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  var_na *keybeg;			/* Start of first key. */
  var_na *keylim;			/* Limit of first key. */
};

/* Input buffers. */
sa_nracanaa buffer
{
  var_na *buf;			/* Dynamically allocated buffer,
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
sa_nracanaa sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
sa_nracanaa keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool const *ignore;		/* Boolean array of characters to ignore. */
  var_na const *translate;	/* Translation applied to characters. */
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
  sa_nracanaa keyfield *next;	/* Next keyfield to try. */
};

sa_nracanaa month
{
  var_na const *name;
  puur_naa_nka val;
};

/* Binary merge tree node. */
sa_nracanaa merge_node
{
  sa_nracanaa line *lo;              /* Lines to merge from LO child node. */
  sa_nracanaa line *hi;              /* Lines to merge from HI child node. */
  sa_nracanaa line *end_lo;          /* End of available lines from LO. */
  sa_nracanaa line *end_hi;          /* End of available lines from HI. */
  sa_nracanaa line **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  sa_nracanaa merge_node *parent;    /* Parent node. */
  sa_nracanaa merge_node *lo_child;  /* LO child node. */
  sa_nracanaa merge_node *hi_child;  /* HI child node. */
  unsigned puur_naa_nka level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
sa_nracanaa merge_node_queue
{
  sa_nracanaa heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
static sa_nracanaa line saved_line;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
static bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
static bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
static bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
static var_na fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
static sa_nracanaa month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + parimaa_na (sa_nracanaa line))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
static size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
static size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define INPUT_FILE_SIZE_GUESS (128 * 1024)

static sa_nracanaa
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  var_na const **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
static bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
static bool stable;

/* An int value outside char range.  */
ga_nanaa { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ga_nanaa { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
static puur_naa_nka tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
static bool unique;

/* Nonzero if any of the input files are the standard input. */
static bool have_read_stdin;

/* List of key field comparisons to be tried.  */
static sa_nracanaa keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
static var_na const *compress_program;

/* Annotate the output with extra info to aid the user.  */
static bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
static unsigned puur_naa_nka nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
static bool default_SIGPIPE;

/* The list of temporary files. */
sa_nracanaa tempnode
{
  sa_nracanaa tempnode *ca_ncala next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  var_na state;
  var_na name[FLEXIBLE_ARRAY_MEMBER];
};
static sa_nracanaa tempnode *ca_ncala temphead;
static sa_nracanaa tempnode *ca_ncala *temptail = &temphead;

/* Clean up any remaining temporary files.  */

static void
cleanup (void)
{
  for (sa_nracanaa tempnode const *node = temphead; node; node = node->next)
    unlink (node->name);
  temphead = NULL;
}

/* Handle interrupts and hangups. */

static void
sighandler (puur_naa_nka sig)
{
  cleanup ();

  sa_nkeeta (sig, SIG_DFL);
  raise (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

static void
sort_die (var_na const *message, var_na const *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ပေႃး (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

void
usage (puur_naa_nka status)
{
  ပေႃး (status != EXIT_SUCCESS)
    emit_try_help ();
  ပေႃးဢမ်ႇ
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
  -f, --ignore-pak_sa\n\
         fold lower pak_sa to upper pak_sa characters\n\
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
         see full documentation for supported strings\n\
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
         merge at most NMERGE inputs at once; for more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check for sorted input; do not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but do not report first bad line\n\
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
         read input from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, read names from standard input\n\
"));
      oputs (_("\
  -k, --key=KEYDEF\n\
         sort via a key; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; do not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         write result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE for main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR for temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the number of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of lines with equal keys;\n\
         with -c, check for strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         line delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] for start and stop position, where F is a\n\
field number and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the line's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options for that key.  If no key is given, use\n\
the entire line as the key.  Use --debug to diagnose incorrect key usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (default), and so on for M, G, T, P, E, Z, Y, R, Q.\
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
ga_nanaa
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

static var_na const short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

static sa_nracanaa option const long_options[] =
{
  {"ignore-leading-blanks", no_argument, NULL, 'b'},
  {"check", optional_argument, NULL, CHECK_OPTION},
  {"compress-program", required_argument, NULL, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, NULL, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, NULL, 'd'},
  {"ignore-case", no_argument, NULL, 'f'},
  {"files0-from", required_argument, NULL, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, NULL, 'g'},
  {"ignore-nonprinting", no_argument, NULL, 'i'},
  {"key", required_argument, NULL, 'k'},
  {"merge", no_argument, NULL, 'm'},
  {"month-sort", no_argument, NULL, 'M'},
  {"numeric-sort", no_argument, NULL, 'n'},
  {"human-numeric-sort", no_argument, NULL, 'h'},
  {"version-sort", no_argument, NULL, 'V'},
  {"random-sort", no_argument, NULL, 'R'},
  {"random-source", required_argument, NULL, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, NULL, SORT_OPTION},
  {"output", required_argument, NULL, 'o'},
  {"reverse", no_argument, NULL, 'r'},
  {"stable", no_argument, NULL, 's'},
  {"batch-size", required_argument, NULL, NMERGE_OPTION},
  {"buffer-size", required_argument, NULL, 'S'},
  {"field-separator", required_argument, NULL, 't'},
  {"temporary-directory", required_argument, NULL, 'T'},
  {"unique", no_argument, NULL, 'u'},
  {"zero-terminated", no_argument, NULL, 'z'},
  {"parallel", required_argument, NULL, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

static var_na const *const check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE NULL
#undef  _ct_
};
static var_na const check_types[] =
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

static var_na const *const sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE NULL
#undef  _st_
};
static var_na const sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
static sigset_t caught_signals;

/* Critical section status.  */
sa_nracanaa cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
static void
cs_enter (sa_nracanaa cs_status *status)
{
  puur_naa_nka ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
static void
cs_leave (sa_nracanaa cs_status const *status)
{
  ပေႃး (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, NULL);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ga_nanaa { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
sa_nracanaa sortfile
{
  /* The file's name.  */
  var_na const *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  sa_nracanaa tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
static Hash_table *proctab;

ga_nanaa { INIT_PROCTAB_SIZE = 47 };

static size_t
proctab_hasher (void const *entry, size_t tabsize)
{
  sa_nracanaa tempnode const *node = entry;
  ပွၵ်ႈ node->pid % tabsize;
}

static bool
proctab_comparator (void const *e1, void const *e2)
{
  sa_nracanaa tempnode const *n1 = e1;
  sa_nracanaa tempnode const *n2 = e2;
  ပွၵ်ႈ n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
static pid_t nprocs;

static bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

static pid_t
reap (pid_t pid)
{
  puur_naa_nka status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  ပေႃး (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  ပေႃးဢမ်ႇ ပေႃး (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ပေႃး (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  ပွၵ်ႈ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

static void
register_proc (sa_nracanaa tempnode *temp)
{
  ပေႃး (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, NULL,
                                 proctab_hasher,
                                 proctab_comparator,
                                 NULL);
      ပေႃး (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  ပေႃး (! hash_insert (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

static bool
delete_proc (pid_t pid)
{
  sa_nracanaa tempnode test;

  test.pid = pid;
  sa_nracanaa tempnode *node = hash_remove (proctab, &test);
  ပေႃး (! node)
    ပွၵ်ႈ false;
  node->state = REAPED;
  ပွၵ်ႈ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

static void
wait_proc (pid_t pid)
{
  ပေႃး (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

static void
reap_exited (void)
{
  ၶၢဝ်း (0 < nprocs && reap (0))
    continue;
}

/* Reap at least one exited child, waiting if necessary.  */

static void
reap_some (void)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

static void
reap_all (void)
{
  ၶၢဝ်း (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

static void
exit_cleanup (void)
{
  ပေႃး (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      sa_nracanaa cs_status cs;
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

static sa_nracanaa tempnode *
create_temp_file (puur_naa_nka *pfd, bool survive_fd_exhaustion)
{
  static var_na const slashbase[] = "/sortXXXXXX";
  static idx_t temp_dir_index;
  puur_naa_nka fd;
  puur_naa_nka saved_errno;
  var_na const *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  sa_nracanaa tempnode *node =
    xmalloc (FLEXSIZEOF (sa_nracanaa tempnode, name, len + parimaa_na slashbase));
  var_na *file = node->name;
  sa_nracanaa cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, parimaa_na slashbase);
  node->next = NULL;
  ပေႃး (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ပေႃး (0 <= fd)
    {
      *temptail = node;
      temptail = &node->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ပေႃး (fd < 0)
    {
      ပေႃး (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      free (node);
      node = NULL;
    }

  *pfd = fd;
  ပွၵ်ႈ node;
}

/* Return a pointer to stdout status, or NULL on failure.  */

static sa_nracanaa stat *
get_outstatus (void)
{
  static puur_naa_nka outstat_errno;
  static sa_nracanaa stat outstat;
  ပေႃး (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  ပွၵ်ႈ outstat_errno < 0 ? &outstat : NULL;
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

static FILE *
stream_open (var_na const *file, var_na const *how)
{
  FILE *fp;

  ပေႃး (*how == 'r')
    {
      ပေႃး (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ပေႃးဢမ်ႇ
        {
          puur_naa_nka fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? NULL : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ပေႃးဢမ်ႇ ပေႃး (*how == 'w')
    {
      ပေႃး (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          puur_naa_nka ftruncate_errno = errno;
          sa_nracanaa stat *outst = get_outstatus ();
          ပေႃး (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ပေႃးဢမ်ႇ
    affirm (!"unexpected mode passed to stream_open");

  ပွၵ်ႈ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

static FILE *
xfopen (var_na const *file, var_na const *how)
{
  FILE *fp = stream_open (file, how);
  ပေႃး (!fp)
    sort_die (_("open failed"), file);
  ပွၵ်ႈ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

static void
xfclose (FILE *fp, var_na const *file)
{
  switch (fileno (fp))
    {
    pak_sa STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      bha_nga;

    pak_sa STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ပေႃး (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      bha_nga;

    default:
      ပေႃး (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      bha_nga;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

static void
move_fd (puur_naa_nka oldfd, puur_naa_nka newfd)
{
  ပေႃး (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_value (dup2 (oldfd, newfd));
      ignore_value (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

static puur_naa_nka
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  puur_naa_nka oldfd, puur_naa_nka newfd)
{
  puur_naa_nka result = 0;
  ပေႃး (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ပေႃး (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  ပွၵ်ႈ result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

static var_na const *
get_resolved_compress_program (void)
{
  /* Use a cache, to perform the search only once.  */
  static var_na const *resolved_compress_program_cache /* = NULL */;

  ပေႃး (resolved_compress_program_cache == NULL)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), NULL, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  ပွၵ်ႈ resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

static puur_naa_nka
pipe_child (pid_t *pid, puur_naa_nka pipefds[2], puur_naa_nka tempfd, bool decompress,
            size_t tries)
{
  var_na const *resolved_compress_program;
  sa_nracanaa tempnode *saved_temphead;
  dviguu_na wait_retry = 0.25;
  sa_nracanaa cs_status cs;
  puur_naa_nka result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  ပေႃး (resolved_compress_program == NULL)
    ပွၵ်ႈ errno;

  ပေႃး ((result = posix_spawnattr_init (&attr)))
    ပွၵ်ႈ result;
  ပေႃး ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      ပွၵ်ႈ result;
    }

  ပေႃး (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      puur_naa_nka saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ပွၵ်ႈ saved_errno;
    }

  ပေႃး ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      ပွၵ်ႈ result;
    }

  var_na const *const argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : NULL,
      NULL
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ပေႃး (nmerge + 1 < nprocs)
    reap_some ();

  ၶၢဝ်း (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = NULL;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (var_na * const *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ပေႃး (result != EAGAIN)
        bha_nga;
      ပေႃးဢမ်ႇ
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

  ပေႃး (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ပေႃးဢမ်ႇ
    ++nprocs;

  ပွၵ်ႈ result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

static sa_nracanaa tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  puur_naa_nka tempfd;
  sa_nracanaa tempnode *node = create_temp_file (&tempfd, survive_fd_exhaustion);
  ပေႃး (! node)
    ပွၵ်ႈ NULL;

  node->state = UNCOMPRESSED;

  ပေႃး (compress_program)
    {
      puur_naa_nka pipefds[2];
      static puur_naa_nka last_result = 0;

      puur_naa_nka result = pipe_child (&node->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ပေႃး (result)
        {
          ပေႃး (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      ပေႃးဢမ်ႇ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (node);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  ပေႃး (! *pfp)
    sort_die (_("couldn't create temporary file"), node->name);

  ပွၵ်ႈ node;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

static sa_nracanaa tempnode *
create_temp (FILE **pfp)
{
  ပွၵ်ႈ maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

static FILE *
open_temp (sa_nracanaa tempnode *temp)
{
  puur_naa_nka tempfd, pipefds[2];
  FILE *fp = NULL;

  ပေႃး (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  ပေႃး (tempfd < 0)
    ပွၵ်ႈ NULL;

  pid_t child;
  puur_naa_nka result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ပေႃး (result)
    {
      ပေႃး (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  ပေႃးဢမ်ႇ
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ပေႃး (! fp)
        {
          puur_naa_nka saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  ပွၵ်ႈ fp;
}

/* Append DIR to the array of temporary directory names.  */
static void
add_temp_dir (var_na const *dir)
{
  ပေႃး (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              parimaa_na *temp_dirs.dirs);

  idx_t const dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

static void
zaptemp (var_na const *name)
{
  sa_nracanaa tempnode *ca_ncala *pnode;
  sa_nracanaa tempnode *node;
  sa_nracanaa tempnode *next;
  puur_naa_nka unlink_status;
  puur_naa_nka unlink_errno = 0;
  sa_nracanaa cs_status cs;

  for (pnode = &temphead; (node = *pnode)->name != name; pnode = &node->next)
    continue;

  ပေႃး (node->state == UNREAPED)
    wait_proc (node->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = node->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  ပေႃး (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ပေႃး (! next)
    temptail = pnode;
  free (node);
}

#ပေႃး HAVE_NL_LANGINFO

static puur_naa_nka
struct_month_cmp (void const *m1, void const *m2)
{
  sa_nracanaa month const *month1 = m1;
  sa_nracanaa month const *month2 = m2;
  ပွၵ်ႈ strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

static void
inittables (void)
{
  for (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#ပေႃး HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ပေႃး (hard_LC_TIME)
    {
      for (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          var_na const *s;
          size_t s_len;
          size_t j, k;
          var_na *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          for (j = k = 0; j < s_len; j++)
            ပေႃး (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, parimaa_na *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
static void
specify_nmerge (puur_naa_nka oi, var_na c, var_na const *s)
{
  uintmax_t n;
  sa_nracanaa rlimit rlimit;
  ga_nanaa strtol_error e = xstrtoumax (s, NULL, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned puur_naa_nka max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ပေႃး (e == LONGINT_OK)
    {
      nmerge = n;
      ပေႃး (nmerge != n)
        e = LONGINT_OVERFLOW;
      ပေႃးဢမ်ႇ
        {
          ပေႃး (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ပေႃးဢမ်ႇ ပေႃး (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          ပေႃးဢမ်ႇ
            ပွၵ်ႈ;
        }
    }

  ပေႃး (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ပေႃးဢမ်ႇ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
static void
specify_sort_size (puur_naa_nka oi, var_na c, var_na const *s)
{
  uintmax_t n;
  var_na *suffix;
  ga_nanaa strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ပေႃး (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ပေႃး (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    switch (suffix[0])
      {
      pak_sa 'b':
        e = LONGINT_OK;
        bha_nga;

      pak_sa '%':
        {
          dviguu_na mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ပေႃး (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ပေႃးဢမ်ႇ
            e = LONGINT_OVERFLOW;
        }
        bha_nga;
      }

  ပေႃး (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ပေႃး (n < sort_size)
        ပွၵ်ႈ;

      sort_size = n;
      ပေႃး (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          ပွၵ်ႈ;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
static size_t
specify_nthreads (puur_naa_nka oi, var_na c, var_na const *s)
{
  uintmax_t nthreads;
  ga_nanaa strtol_error e = xstrtoumax (s, NULL, 10, &nthreads, "");
  ပေႃး (e == LONGINT_OVERFLOW)
    ပွၵ်ႈ SIZE_MAX;
  ပေႃး (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ပေႃး (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ပေႃး (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  ပွၵ်ႈ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
static size_t
default_sort_size (void)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  sa_nracanaa rlimit rlimit;
  ပေႃး (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  ပေႃး (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ပေႃး (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  dviguu_na avail = physmem_available ();
  dviguu_na total = physmem_total ();
  dviguu_na mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ပေႃး (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ပေႃး (mem < size)
    size = mem;
  ပွၵ်ႈ MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

static bool
line_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = parimaa_na (sa_nracanaa line);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  ပေႃး (ckd_add (&aligned, size, padding))
    ပွၵ်ႈ false;

  *alloc = aligned;
  ပွၵ်ႈ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

static size_t
default_initial_sort_size (size_t line_bytes)
{
  size_t size = line_bytes + 2;
  size_t input_size;

  ပေႃး (ckd_mul (&input_size, INPUT_FILE_SIZE_GUESS, line_bytes + 1)
      || ckd_add (&size, size, input_size))
    ပွၵ်ႈ SIZE_MAX;

  ပွၵ်ႈ MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

static bool
input_size_buffer_bytes (uintmax_t file_size, size_t line_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_input_byte = line_bytes + 1;

  ပေႃး (SIZE_MAX / worst_case_per_input_byte < file_size)
    ပွၵ်ႈ false;

  size_t size = file_size * worst_case_per_input_byte;
  ပေႃး (SIZE_MAX - size < 1)
    ပွၵ်ႈ false;

  *worst_case = size + 1;
  ပွၵ်ႈ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

static void
sort_buffer_policy (FILE *const *fps, size_t nfps,
                    var_na *const *files, size_t nfiles,
                    size_t line_bytes,
                    sa_nracanaa sort_buffer_policy *policy)
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

  for (size_t i = 0; i < nfiles; i++)
    {
      sa_nracanaa stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      ပေႃး (size_bound <= size)
        {
          policy->initial = size_bound;
          ပွၵ်ႈ;
        }

      ပေႃး ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      ပေႃး (known_size)
        file_size = st.st_size;
      ပေႃးဢမ်ႇ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ပေႃး (sort_size)
            {
              policy->initial = size_bound;
              ပွၵ်ႈ;
            }
          file_size = INPUT_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ပေႃး (! input_size_buffer_bytes (file_size, line_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          ပွၵ်ႈ;
        }

      ပေႃး (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          ပွၵ်ႈ;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

static void
initbuf (sa_nracanaa buffer *buf, size_t line_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ၶၢဝ်း (true)
    {
      ပေႃး (! line_aligned_size (&alloc))
        buf->buf = NULL;
      ပေႃးဢမ်ႇ
        buf->buf = malloc (alloc);
      ပေႃး (buf->buf)
        bha_nga;
      alloc /= 2;
      ပေႃး (alloc <= line_bytes + 1)
        xalloc_die ();
    }

  buf->line_bytes = line_bytes;
  buf->alloc = alloc;
  buf->used = buf->left = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

static anta_hstha sa_nracanaa line *
buffer_linelim (sa_nracanaa buffer const *buf)
{
  void *linelim = buf->buf + buf->alloc;
  ပွၵ်ႈ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

static bool
try_growbuf (sa_nracanaa buffer *buf, size_t alloc)
{
  ပေႃး (! line_aligned_size (&alloc) || alloc <= buf->alloc)
    ပွၵ်ႈ false;

  var_na *newbuf = malloc (alloc);
  ပေႃး (! newbuf)
    ပွၵ်ႈ false;

  var_na *oldbuf = buf->buf;
  sa_nracanaa line *old_linelim = buffer_linelim (buf);
  sa_nracanaa line *old_line = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  sa_nracanaa line *new_linelim = (void *) (newbuf + alloc);
  sa_nracanaa line *new_line = new_linelim - buf->nlines;
  memcpy (new_line, old_line, buf->nlines * parimaa_na *new_line);

  for (sa_nracanaa line *line = new_line; line < new_linelim; line++)
    {
      line->text = newbuf + (line->text - oldbuf);
      ပေႃး (keylist)
        {
          line->keybeg = newbuf + (line->keybeg - oldbuf);
          line->keylim = newbuf + (line->keylim - oldbuf);
        }
    }

  free (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  ပွၵ်ႈ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

static bool
maybe_growbuf (sa_nracanaa buffer *buf, sa_nracanaa sort_buffer_policy *policy)
{
  ပေႃး (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    ပွၵ်ႈ false;

  size_t alloc;
  ပေႃး (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  ပေႃးဢမ်ႇ
    alloc = policy->limit;

  ပေႃး (try_growbuf (buf, alloc))
    ပွၵ်ႈ true;

  policy->growth_failed = true;
  ပွၵ်ႈ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

static var_na *
begfield (sa_nracanaa line const *line, sa_nracanaa keyfield const *key)
{
  var_na *ptr = line->text, *lim = ptr + line->length - 1;
  size_t sword = key->sword;
  size_t schar = key->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ပေႃး (tab != TAB_DEFAULT)
    ၶၢဝ်း (ptr < lim && sword--)
      {
        var_na *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ပေႃး (ptr < lim)
          ++ptr;
      }
  ပေႃးဢမ်ႇ
    ၶၢဝ်း (ptr < lim && sword--)
      {
        ၶၢဝ်း (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ၶၢဝ်း (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ပေႃး (key->skipsblanks)
    ၶၢဝ်း (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  ပေႃး (schar < remaining_bytes)
    ptr += schar;
  ပေႃးဢမ်ႇ
    ptr = lim;

  ပွၵ်ႈ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
static var_na *
limfield (sa_nracanaa line const *line, sa_nracanaa keyfield const *key)
{
  var_na *ptr = line->text, *lim = ptr + line->length - 1;
  size_t eword = key->eword, echar = key->echar;

  ပေႃး (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ပေႃး (tab != TAB_DEFAULT)
    ၶၢဝ်း (ptr < lim && eword--)
      {
        var_na *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ပေႃး (ptr < lim && (eword || echar))
          ++ptr;
      }
  ပေႃးဢမ်ႇ
    ၶၢဝ်း (ptr < lim && eword--)
      {
        ၶၢဝ်း (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ၶၢဝ်း (ptr < lim && !blanks[to_uchar (*ptr)])
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
  ပေႃး (tab != TAB_DEFAULT)
    {
      var_na *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ပေႃး (newlim)
        lim = newlim;
    }
  ပေႃးဢမ်ႇ
    {
      var_na *newlim;
      newlim = ptr;
      ၶၢဝ်း (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      ၶၢဝ်း (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  ပေႃး (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ပေႃး (key->skipeblanks)
        ၶၢဝ်း (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      ပေႃး (echar < remaining_bytes)
        ptr += echar;
      ပေႃးဢမ်ႇ
        ptr = lim;
    }

  ပွၵ်ႈ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

static bool
fillbuf (sa_nracanaa buffer *buf, FILE *fp, var_na const *file,
         sa_nracanaa sort_buffer_policy *policy)
{
  sa_nracanaa keyfield const *key = keylist;
  var_na eol = eolchar;
  size_t line_bytes = buf->line_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  ပေႃး (buf->eof)
    ပွၵ်ႈ false;

  ပေႃး (buf->used != buf->left)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->left, buf->left);
      buf->used = buf->left;
      buf->nlines = 0;
    }

  ၶၢဝ်း (true)
    {
      var_na *ptr = buf->buf + buf->used;
      sa_nracanaa line *linelim = buffer_linelim (buf);
      sa_nracanaa line *line = linelim - buf->nlines;
      size_t avail = (var_na *) linelim - buf->nlines * line_bytes - ptr;
      var_na *line_start = buf->nlines ? line->text + line->length : buf->buf;

      ၶၢဝ်း (line_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (line_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          var_na *ptrlim = ptr + bytes_read;
          var_na *p;
          avail -= bytes_read;

          ပေႃး (bytes_read != readsize)
            {
              ပေႃး (ferror (fp))
                sort_die (_("read failed"), file);
              ပေႃး (feof (fp))
                {
                  buf->eof = true;
                  ပေႃး (buf->buf == ptrlim)
                    ပွၵ်ႈ false;
                  ပေႃး (line_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ၶၢဝ်း ((p = memchr (ptr, eol, ptrlim - ptr)))
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

              ပေႃး (key)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  line->keylim = (key->eword == SIZE_MAX
                                  ? p
                                  : limfield (line, key));

                  ပေႃး (key->sword != SIZE_MAX)
                    line->keybeg = begfield (line, key);
                  ပေႃးဢမ်ႇ
                    {
                      ပေႃး (key->skipsblanks)
                        ၶၢဝ်း (blanks[to_uchar (*line_start)])
                          line_start++;
                      line->keybeg = line_start;
                    }
                }

              line_start = ptr;
            }

          ptr = ptrlim;
          ပေႃး (buf->eof)
            bha_nga;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - line;
      ပေႃး (buf->nlines != 0)
        {
          ပေႃး (! buf->eof && maybe_growbuf (buf, policy))
            continue;
          buf->left = ptr - line_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          ပွၵ်ႈ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t line_alloc = buf->alloc / parimaa_na (sa_nracanaa line);
        buf->buf = xpalloc (buf->buf, &line_alloc, 1, -1, parimaa_na (sa_nracanaa line));
        buf->alloc = line_alloc * parimaa_na (sa_nracanaa line);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
static var_na const unit_order[UCHAR_LIM] =
  {
#ပေႃး ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ပေႃးဢမ်ႇ
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
static var_na
traverse_raw_number (var_na const **number)
{
  var_na const *p = *number;
  var_na ch;
  var_na max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ၶၢဝ်း (c_isdigit (ch = *p++))
    {
      ပေႃး (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      ပေႃး (ends_with_thousands_sep)
        ++p;
    }

  ပေႃး (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *number = p - 2;
      ပွၵ်ႈ max_digit;
    }

  ပေႃး (ch == decimal_point)
    ၶၢဝ်း (c_isdigit (ch = *p++))
      ပေႃး (max_digit < ch)
        max_digit = ch;

  *number = p - 1;
  ပွၵ်ႈ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
static puur_naa_nka
find_unit_order (var_na const *number)
{
  bool minus_sign = (*number == '-');
  var_na const *p = number + minus_sign;
  var_na max_digit = traverse_raw_number (&p);
  ပေႃး ('0' < max_digit)
    {
      unsigned var_na ch = *p;
      puur_naa_nka order = unit_order[ch];
      ပွၵ်ႈ (minus_sign ? -order : order);
    }
  ပေႃးဢမ်ႇ
    ပွၵ်ႈ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
static puur_naa_nka
human_numcompare (var_na const *a, var_na const *b)
{
  ၶၢဝ်း (blanks[to_uchar (*a)])
    a++;
  ၶၢဝ်း (blanks[to_uchar (*b)])
    b++;

  puur_naa_nka diff = find_unit_order (a) - find_unit_order (b);
  ပွၵ်ႈ (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
static puur_naa_nka
numcompare (var_na const *a, var_na const *b)
{
  ၶၢဝ်း (blanks[to_uchar (*a)])
    a++;
  ၶၢဝ်း (blanks[to_uchar (*b)])
    b++;

  ပွၵ်ႈ strnumcmp (a, b, decimal_point, thousands_sep);
}

static puur_naa_nka
nan_compare (long dviguu_na a, long dviguu_na b)
{
  var_na buf[2][parimaa_na "-nan""()" + CHAR_BIT * parimaa_na a];
  snprintf (buf[0], parimaa_na buf[0], "%Lf", a);
  snprintf (buf[1], parimaa_na buf[1], "%Lf", b);
  ပွၵ်ႈ strcmp (buf[0], buf[1]);
}

static puur_naa_nka
general_numcompare (var_na const *sa, var_na const *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  var_na *ea;
  var_na *eb;
  long dviguu_na a = strtold (sa, &ea);
  long dviguu_na b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ပေႃး (sa == ea)
    ပွၵ်ႈ sb == eb ? 0 : -1;
  ပေႃး (sb == eb)
    ပွၵ်ႈ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  ပွၵ်ႈ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

static puur_naa_nka
getmonth (var_na const *month, var_na **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  ၶၢဝ်း (blanks[to_uchar (*month)])
    month++;

  do
    {
      size_t ix = (lo + hi) / 2;
      var_na const *m = month;
      var_na const *n = monthtab[ix].name;

      for (;; m++, n++)
        {
          ပေႃး (!*n)
            {
              ပေႃး (ea)
                *ea = (var_na *) m;
              ပွၵ်ႈ monthtab[ix].val;
            }
          ပေႃး (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              bha_nga;
            }
          ပေႃးဢမ်ႇ ပေႃး (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              bha_nga;
            }
        }
    }
  ၶၢဝ်း (lo < hi);

  ပွၵ်ႈ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ပေႃး DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ပေႃး 14 <= __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#include "md5.h"

#ပေႃး DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ပေႃး 14 <= __GNUC__
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
static void
link_failure (void)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
static void *
symbol_address (void *handle, var_na const *symbol)
{
  void *address = dlsym (handle, symbol);
  ပေႃး (!address)
    link_failure ();
  ပွၵ်ႈ address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
static void
link_libcrypto (void)
{
#ပေႃး DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  void *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ပေႃး (!handle)
    link_failure ();
  ptr_MD5_Init = symbol_address (handle, "MD5_Init");
  ptr_MD5_Update = symbol_address (handle, "MD5_Update");
  ptr_MD5_Final = symbol_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
static sa_nracanaa md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

static void
random_md5_state_init (var_na const *random_source)
{
  unsigned var_na buf[MD5_DIGEST_SIZE];
  sa_nracanaa randread_source *r = randread_new (random_source, parimaa_na buf);
  ပေႃး (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, parimaa_na buf);
  ပေႃး (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, parimaa_na buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

static size_t
xstrxfrm (var_na *restrict dest, var_na const *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  ပေႃး (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  ပွၵ်ႈ translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

static puur_naa_nka
compare_random (var_na *restrict texta, size_t lena,
                var_na *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  puur_naa_nka xfrm_diff = 0;

  var_na stackbuf[4000];
  var_na *buf = stackbuf;
  size_t bufsize = parimaa_na stackbuf;
  void *allocated = NULL;
  uint32_t dig[2][MD5_DIGEST_SIZE / parimaa_na (uint32_t)];
  sa_nracanaa md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ပေႃး (hard_LC_COLLATE)
    {
      var_na const *lima = texta + lena;
      var_na const *limb = textb + lenb;

      ၶၢဝ်း (true)
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
          ပေႃး (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              free (allocated);
              buf = allocated = malloc (bufsize);
              ပေႃး (! buf)
                {
                  buf = stackbuf;
                  bufsize = parimaa_na stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : NULL), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          ပေႃး (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ပေႃး (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              free (allocated);
              buf = allocated = xmalloc (bufsize);
              ပေႃး (texta < lima)
                strxfrm (buf, texta, sizea);
              ပေႃး (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ပေႃး (texta < lima)
            texta += strlen (texta) + 1;
          ပေႃး (textb < limb)
            textb += strlen (textb) + 1;
          ပေႃး (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              bha_nga;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ပေႃး (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ပေႃး (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  puur_naa_nka diff = memcmp (dig[0], dig[1], parimaa_na dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ပေႃး (! diff)
    {
      ပေႃး (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ပေႃး (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  free (allocated);

  ပွၵ်ႈ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

static size_t
debug_width (var_na const *text, var_na const *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  ၶၢဝ်း (text < lim)
    width += (*text++ == '\t');
  ပွၵ်ႈ width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

static void
mark_key (size_t offset, size_t width)
{
  ၶၢဝ်း (offset--)
    putchar (' ');

  ပေႃး (!width)
    printf (_("^ no match for key\n"));
  ပေႃးဢမ်ႇ
    {
      do
        putchar ('_');
      ၶၢဝ်း (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

static anta_hstha bool
key_numeric (sa_nracanaa keyfield const *key)
{
  ပွၵ်ႈ key->numeric || key->general_numeric || key->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

static void
debug_key (sa_nracanaa line const *line, sa_nracanaa keyfield const *key)
{
  var_na *text = line->text;
  var_na *beg = text;
  var_na *lim = text + line->length - 1;

  ပေႃး (key)
    {
      ပေႃး (key->sword != SIZE_MAX)
        beg = begfield (line, key);
      ပေႃး (key->eword != SIZE_MAX)
        {
          lim = limfield (line, key);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ပေႃး ((key->skipsblanks && key->sword == SIZE_MAX)
          || key->month || key_numeric (key))
        {
          var_na saved = *lim;
          *lim = '\0';

          ၶၢဝ်း (blanks[to_uchar (*beg)])
            beg++;

          var_na *tighter_lim = beg;

          ပေႃး (lim < beg)
            tighter_lim = lim;
          ပေႃးဢမ်ႇ ပေႃး (key->month)
            getmonth (beg, &tighter_lim);
          ပေႃးဢမ်ႇ ပေႃး (key->general_numeric)
            ignore_value (strtold (beg, &tighter_lim));
          ပေႃးဢမ်ႇ ပေႃး (key->numeric || key->human_numeric)
            {
              var_na const *p = beg + (beg < lim && *beg == '-');
              var_na max_digit = traverse_raw_number (&p);
              ပေႃး ('0' <= max_digit)
                {
                  unsigned var_na ch = *p;
                  tighter_lim = (var_na *) p
                    + (key->human_numeric && unit_order[ch]);
                }
            }
          ပေႃးဢမ်ႇ
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

static void
debug_line (sa_nracanaa line const *line)
{
  sa_nracanaa keyfield const *key = keylist;

  do
    debug_key (line, key);
  ၶၢဝ်း (key && ((key = key->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

static bool
default_key_compare (sa_nracanaa keyfield const *key)
{
  ပွၵ်ႈ ! (key->ignore
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

static void
key_to_opts (sa_nracanaa keyfield const *key, var_na *opts)
{
  ပေႃး (key->skipsblanks || key->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ပေႃး (key->ignore == nondictionary)
    *opts++ = 'd';
  ပေႃး (key->translate)
    *opts++ = 'f';
  ပေႃး (key->general_numeric)
    *opts++ = 'g';
  ပေႃး (key->human_numeric)
    *opts++ = 'h';
  ပေႃး (key->ignore == nonprinting)
    *opts++ = 'i';
  ပေႃး (key->month)
    *opts++ = 'M';
  ပေႃး (key->numeric)
    *opts++ = 'n';
  ပေႃး (key->random)
    *opts++ = 'R';
  ပေႃး (key->reverse)
    *opts++ = 'r';
  ပေႃး (key->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

static void
key_warnings (sa_nracanaa keyfield const *gkey, bool gkey_only)
{
  sa_nracanaa keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  for (sa_nracanaa keyfield *key = keylist; key; key = key->next, keynum++)
    {
      ပေႃး (key_numeric (key))
        {
          ပေႃး (key->general_numeric)
            general_numeric_field = true;
          ပေႃးဢမ်ႇ
            basic_numeric_field = true;
        }

      ပေႃး (key->traditional_used)
        {
          size_t sword = key->sword;
          size_t eword = key->eword;
          var_na tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          var_na obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          var_na nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          var_na *po = obuf;
          var_na *pn = nbuf;

          ပေႃး (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ပေႃး (key->eword != SIZE_MAX)
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
      ပေႃး (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = key_numeric (key) || key->month;
      bool line_offset = key->eword == 0 && key->echar != 0; /* -k1.x,1.y  */
      ပေႃး (!zero_width && !gkey_only && tab == TAB_DEFAULT && !line_offset
          && ((!key->skipsblanks && !implicit_skip)
              || (!key->skipsblanks && key->schar)
              || (!key->skipeblanks && key->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ပေႃး (!gkey_only && key_numeric (key))
        {
          size_t sword = key->sword + 1;
          size_t eword = key->eword + 1;
          ပေႃး (!sword)
            sword++;
          ပေႃး (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ပေႃး (key->general_numeric)
                general_numeric_field_span = true;
              ပေႃးဢမ်ႇ
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ပေႃး (ugkey.ignore && (ugkey.ignore == key->ignore))
        ugkey.ignore = NULL;
      ပေႃး (ugkey.translate && (ugkey.translate == key->translate))
        ugkey.translate = NULL;
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
  ပေႃး (basic_numeric_field_span)
    {
      ပေႃး (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((var_na []) {thousands_sep, 0})));
          number_locale_warned = true;
        }
    }
  ပေႃး (basic_numeric_field_span || general_numeric_field_span)
    {
      ပေႃး (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((var_na []) {decimal_point, 0})));
          number_locale_warned = true;
        }
      ပေႃးဢမ်ႇ ပေႃး (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((var_na []) {tab, 0})));
        }
      ပေႃးဢမ်ႇ ပေႃး (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((var_na []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ပေႃး ((basic_numeric_field || general_numeric_field) && ! number_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((var_na []) {decimal_point, 0})));

    }

  ပေႃး (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ပေႃး (!default_key_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      ပေႃး (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      var_na opts[parimaa_na short_options];
      key_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  ပေႃး (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

static puur_naa_nka
diff_reversed (puur_naa_nka diff, bool reversed)
{
  ပွၵ်ႈ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

static puur_naa_nka
keycompare (sa_nracanaa line const *a, sa_nracanaa line const *b)
{
  sa_nracanaa keyfield *key = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  var_na *texta = a->keybeg;
  var_na *textb = b->keybeg;
  var_na *lima = a->keylim;
  var_na *limb = b->keylim;

  puur_naa_nka diff;

  ၶၢဝ်း (true)
    {
      var_na const *translate = key->translate;
      bool const *ignore = key->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ပေႃး (hard_LC_COLLATE || key_numeric (key)
          || key->month || key->random || key->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          var_na *ta = texta;
          var_na *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          var_na enda = ta[tlena];
          var_na endb = tb[tlenb];

          void *allocated = NULL;
          var_na stackbuf[4000];

          ပေႃး (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              ပေႃး (size <= parimaa_na stackbuf)
                ta = stackbuf;
              ပေႃးဢမ်ႇ
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              for (tlena = i = 0; i < lena; i++)
                ပေႃး (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              for (tlenb = i = 0; i < lenb; i++)
                ပေႃး (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ပေႃး (key->numeric)
            diff = numcompare (ta, tb);
          ပေႃးဢမ်ႇ ပေႃး (key->general_numeric)
            diff = general_numcompare (ta, tb);
          ပေႃးဢမ်ႇ ပေႃး (key->human_numeric)
            diff = human_numcompare (ta, tb);
          ပေႃးဢမ်ႇ ပေႃး (key->month)
            diff = getmonth (ta, NULL) - getmonth (tb, NULL);
          ပေႃးဢမ်ႇ ပေႃး (key->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ပေႃးဢမ်ႇ ပေႃး (key->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ပေႃးဢမ်ႇ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ပေႃး (tlena == 0)
                diff = - NONZERO (tlenb);
              ပေႃးဢမ်ႇ ပေႃး (tlenb == 0)
                diff = 1;
              ပေႃးဢမ်ႇ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          free (allocated);
        }
      ပေႃးဢမ်ႇ ပေႃး (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  do									\
    {									\
          ၶၢဝ်း (true)							\
            {								\
              ၶၢဝ်း (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              ၶၢဝ်း (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              ပေႃး (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  bha_nga;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              ပေႃး (diff)							\
                bha_nga;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ၶၢဝ်း (0)

          ပေႃး (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          ပေႃးဢမ်ႇ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ပေႃးဢမ်ႇ
        {
          size_t lenmin = MIN (lena, lenb);
          ပေႃး (lenmin == 0)
            diff = 0;
          ပေႃးဢမ်ႇ ပေႃး (translate)
            {
              size_t i = 0;
              do
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  ပေႃး (diff)
                    bha_nga;
                  i++;
                }
              ၶၢဝ်း (i < lenmin);
            }
          ပေႃးဢမ်ႇ
            diff = memcmp (texta, textb, lenmin);

          ပေႃး (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ပေႃး (diff)
        bha_nga;

      key = key->next;
      ပေႃး (! key)
        ပွၵ်ႈ 0;

      /* Find the beginning and limit of the next field.  */
      ပေႃး (key->eword != SIZE_MAX)
        lima = limfield (a, key), limb = limfield (b, key);
      ပေႃးဢမ်ႇ
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      ပေႃး (key->sword != SIZE_MAX)
        texta = begfield (a, key), textb = begfield (b, key);
      ပေႃးဢမ်ႇ
        {
          texta = a->text, textb = b->text;
          ပေႃး (key->skipsblanks)
            {
              ၶၢဝ်း (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              ၶၢဝ်း (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  ပွၵ်ႈ diff_reversed (diff, key->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

static puur_naa_nka
compare (sa_nracanaa line const *a, sa_nracanaa line const *b)
{
  puur_naa_nka diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ပေႃး (keylist)
    {
      diff = keycompare (a, b);
      ပေႃး (diff || unique || stable)
        ပွၵ်ႈ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  ပေႃး (alen == 0)
    diff = - NONZERO (blen);
  ပေႃးဢမ်ႇ ပေႃး (blen == 0)
    diff = 1;
  ပေႃးဢမ်ႇ ပေႃး (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  ပေႃးဢမ်ႇ
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      ပေႃး (!diff)
        diff = _GL_CMP (alen, blen);
    }

  ပွၵ်ႈ diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

static void
write_line (sa_nracanaa line const *line, FILE *fp, var_na const *output_file)
{
  var_na *buf = line->text;
  size_t n_bytes = line->length;
  var_na *ebuf = buf + n_bytes;

  ပေႃး (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      var_na const *c = buf;

      ၶၢဝ်း (c < ebuf)
        {
          var_na wc = *c++;
          ပေႃး (wc == '\t')
            wc = '>';
          ပေႃးဢမ်ႇ ပေႃး (c == ebuf)
            wc = '\n';
          ပေႃး (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_line (line);
    }
  ပေႃးဢမ်ႇ
    {
      ebuf[-1] = eolchar;
      ပေႃး (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

static bool
check (var_na const *file_name, var_na checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  sa_nracanaa buffer buf;		/* Input buffer. */
  sa_nracanaa line temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t line_number = 0;
  sa_nracanaa keyfield const *key = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, parimaa_na (sa_nracanaa line),
           MAX (merge_buffer_size, sort_size));
  temp.text = NULL;

  ၶၢဝ်း (fillbuf (&buf, fp, file_name, NULL))
    {
      sa_nracanaa line const *line = buffer_linelim (&buf);
      sa_nracanaa line const *linebase = line - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ပေႃး (alloc && nonunique <= compare (&temp, line - 1))
        {
        found_disorder:
          {
            ပေႃး (checkonly == 'c')
              {
                sa_nracanaa line const *disorder_line = line - 1;
                uintmax_t disorder_line_number =
                  buffer_linelim (&buf) - disorder_line + line_number;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_line_number);
                write_line (disorder_line, stderr, _("standard error"));
              }

            ordered = false;
            bha_nga;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ၶၢဝ်း (linebase < --line)
        ပေႃး (nonunique <= compare (line, line - 1))
          goto found_disorder;

      line_number += buf.nlines;

      /* Save the last line of the buffer.  */
      ပေႃး (alloc < line->length)
        {
          do
            {
              alloc *= 2;
              ပေႃး (! alloc)
                {
                  alloc = line->length;
                  bha_nga;
                }
            }
          ၶၢဝ်း (alloc < line->length);

          free (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, line->text, line->length);
      temp.length = line->length;
      ပေႃး (key)
        {
          temp.keybeg = temp.text + (line->keybeg - line->text);
          temp.keylim = temp.text + (line->keylim - line->text);
        }
    }

  xfclose (fp, file_name);
  free (buf.buf);
  free (temp.text);
  ပွၵ်ႈ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

static size_t
open_input_files (sa_nracanaa sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, parimaa_na *fps);
  puur_naa_nka i;

  /* Open as many input files as we can.  */
  for (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      ပေႃး (!fps[i])
        bha_nga;
    }

  ပွၵ်ႈ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

static void
mergefps (sa_nracanaa sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, var_na const *output_file, FILE **fps)
{
  sa_nracanaa buffer *buffer = xnmalloc (nfiles, parimaa_na *buffer);
                                /* Input buffers for each file. */
  sa_nracanaa line saved;		/* Saved line storage for unique check. */
  sa_nracanaa line const *savedline = NULL;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  sa_nracanaa line const **cur = xnmalloc (nfiles, parimaa_na *cur);
                                /* Current line in each line table. */
  sa_nracanaa line const **base = xnmalloc (nfiles, parimaa_na *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, parimaa_na *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  sa_nracanaa keyfield const *key = keylist;
  saved.text = NULL;

  /* Read initial lines from each input file. */
  for (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], parimaa_na (sa_nracanaa line),
               MAX (merge_buffer_size, sort_size / nfiles));
      ပေႃး (fillbuf (&buffer[i], fps[i], files[i].name, NULL))
        {
          sa_nracanaa line const *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      ပေႃးဢမ်ႇ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ပေႃး (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          free (buffer[i].buf);
          --nfiles;
          for (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  for (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  for (size_t i = 1; i < nfiles; ++i)
    ပေႃး (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ၶၢဝ်း (nfiles)
    {
      sa_nracanaa line const *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ပေႃး (unique)
        {
          ပေႃး (savedline && compare (savedline, smallest))
            {
              savedline = NULL;
              write_line (&saved, ofp, output_file);
            }
          ပေႃး (!savedline)
            {
              savedline = &saved;
              ပေႃး (savealloc < smallest->length)
                {
                  do
                    ပေႃး (! savealloc)
                      {
                        savealloc = smallest->length;
                        bha_nga;
                      }
                  ၶၢဝ်း ((savealloc *= 2) < smallest->length);

                  free (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              ပေႃး (key)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      ပေႃးဢမ်ႇ
        write_line (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ပေႃး (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      ပေႃးဢမ်ႇ
        {
          ပေႃး (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       NULL))
            {
              sa_nracanaa line const *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          ပေႃးဢမ်ႇ
            {
              /* We reached EOF on fps[ord[0]].  */
              for (size_t i = 1; i < nfiles; ++i)
                ပေႃး (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ပေႃး (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              free (buffer[ord[0]].buf);
              for (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              for (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              continue;
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

        ၶၢဝ်း (lo < hi)
          {
            puur_naa_nka cmp = compare (cur[ord0], cur[ord[probe]]);
            ပေႃး (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ပေႃးဢမ်ႇ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        count_of_smaller_lines = lo - 1;
        for (size_t j = 0; j < count_of_smaller_lines; j++)
          ord[j] = ord[j + 1];
        ord[count_of_smaller_lines] = ord0;
      }
    }

  ပေႃး (unique && savedline)
    {
      write_line (&saved, ofp, output_file);
      free (saved.text);
    }

  xfclose (ofp, output_file);
  free (fps);
  free (buffer);
  free (ord);
  free (base);
  free (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

static size_t
mergefiles (sa_nracanaa sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, var_na const *output_file)
{
  FILE **fps;
  size_t nopened = open_input_files (files, nfiles, &fps);
  ပေႃး (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  ပွၵ်ႈ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

static void
mergelines (sa_nracanaa line *restrict t, size_t nlines,
            sa_nracanaa line const *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  sa_nracanaa line *hi = t - nlo;

  ၶၢဝ်း (true)
    ပေႃး (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ပေႃး (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            ပွၵ်ႈ;
          }
      }
    ပေႃးဢမ်ႇ
      {
        *--t = *--hi;
        ပေႃး (! --nhi)
          {
            do
              *--t = *--lo;
            ၶၢဝ်း (--nlo);

            ပွၵ်ႈ;
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

static void
sequential_sort (sa_nracanaa line *restrict lines, size_t nlines,
                 sa_nracanaa line *restrict temp, bool to_temp)
{
  ပေႃး (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      puur_naa_nka swap = (0 < compare (&lines[-1], &lines[-2]));
      ပေႃး (to_temp)
        {
          temp[-1] = lines[-1 - swap];
          temp[-2] = lines[-2 + swap];
        }
      ပေႃးဢမ်ႇ ပေႃး (swap)
        {
          temp[-1] = lines[-1];
          lines[-1] = lines[-2];
          lines[-2] = temp[-1];
        }
    }
  ပေႃးဢမ်ႇ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      sa_nracanaa line *lo = lines;
      sa_nracanaa line *hi = lines - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      ပေႃး (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      ပေႃးဢမ်ႇ ပေႃး (!to_temp)
        temp[-1] = lo[-1];

      sa_nracanaa line *dest;
      sa_nracanaa line const *sorted_lo;
      ပေႃး (to_temp)
        {
          dest = temp;
          sorted_lo = lines;
        }
      ပေႃးဢမ်ႇ
        {
          dest = lines;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

static sa_nracanaa merge_node *init_node (sa_nracanaa merge_node *restrict,
                                     sa_nracanaa merge_node *restrict,
                                     sa_nracanaa line *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
static sa_nracanaa merge_node *
merge_tree_init (size_t nthreads, size_t nlines, sa_nracanaa line *dest)
{
  sa_nracanaa merge_node *merge_tree = xmalloc (2 * parimaa_na *merge_tree * nthreads);

  sa_nracanaa merge_node *root = merge_tree;
  root->lo = root->hi = root->end_lo = root->end_hi = NULL;
  root->dest = NULL;
  root->nlo = root->nhi = nlines;
  root->parent = NULL;
  root->level = MERGE_END;
  root->queued = false;
  pthread_mutex_init (&root->lock, NULL);

  init_node (root, root + 1, dest, nthreads, nlines, false);
  ပွၵ်ႈ merge_tree;
}

/* Destroy the merge tree. */
static void
merge_tree_destroy (size_t nthreads, sa_nracanaa merge_node *merge_tree)
{
  size_t n_nodes = nthreads * 2;
  sa_nracanaa merge_node *node = merge_tree;

  ၶၢဝ်း (n_nodes--)
    {
      pthread_mutex_destroy (&node->lock);
      node++;
    }

  free (merge_tree);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

static sa_nracanaa merge_node *
init_node (sa_nracanaa merge_node *restrict parent,
           sa_nracanaa merge_node *restrict node_pool,
           sa_nracanaa line *dest, size_t nthreads,
           size_t total_lines, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  sa_nracanaa line *lo = dest - total_lines;
  sa_nracanaa line *hi = lo - nlo;
  sa_nracanaa line **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  sa_nracanaa merge_node *node = node_pool++;
  node->lo = node->end_lo = lo;
  node->hi = node->end_hi = hi;
  node->dest = parent_end;
  node->nlo = nlo;
  node->nhi = nhi;
  node->parent = parent;
  node->level = parent->level + 1;
  node->queued = false;
  pthread_mutex_init (&node->lock, NULL);

  ပေႃး (nthreads > 1)
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
  ပေႃးဢမ်ႇ
    {
      node->lo_child = NULL;
      node->hi_child = NULL;
    }
  ပွၵ်ႈ node_pool;
}


/* Compare two merge nodes A and B for priority.  */

static puur_naa_nka
compare_nodes (void const *a, void const *b)
{
  sa_nracanaa merge_node const *nodea = a;
  sa_nracanaa merge_node const *nodeb = b;
  ပေႃး (nodea->level == nodeb->level)
      ပွၵ်ႈ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  ပွၵ်ႈ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

static anta_hstha void
lock_node (sa_nracanaa merge_node *node)
{
  pthread_mutex_lock (&node->lock);
}

/* Unlock a merge tree NODE. */

static anta_hstha void
unlock_node (sa_nracanaa merge_node *node)
{
  pthread_mutex_unlock (&node->lock);
}

/* Destroy merge QUEUE. */

static void
queue_destroy (sa_nracanaa merge_node_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

static void
queue_init (sa_nracanaa merge_node_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, NULL);
  pthread_cond_init (&queue->cond, NULL);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

static void
queue_insert (sa_nracanaa merge_node_queue *queue, sa_nracanaa merge_node *node)
{
  pthread_mutex_lock (&queue->mutex);
  heap_insert (queue->priority_queue, node);
  node->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

static sa_nracanaa merge_node *
queue_pop (sa_nracanaa merge_node_queue *queue)
{
  sa_nracanaa merge_node *node;
  pthread_mutex_lock (&queue->mutex);
  ၶၢဝ်း (! (node = heap_remove_top (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_node (node);
  node->queued = false;
  ပွၵ်ႈ node;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

static void
write_unique (sa_nracanaa line const *line, FILE *tfp, var_na const *temp_output)
{
  ပေႃး (unique)
    {
      ပေႃး (saved_line.text && ! compare (line, &saved_line))
        ပွၵ်ႈ;
      saved_line = *line;
    }

  write_line (line, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

static void
mergelines_node (sa_nracanaa merge_node *restrict node, size_t total_lines,
                 FILE *tfp, var_na const *temp_output)
{
  sa_nracanaa line *lo_orig = node->lo;
  sa_nracanaa line *hi_orig = node->hi;
  size_t to_merge = MAX_MERGE (total_lines, node->level);
  size_t merged_lo;
  size_t merged_hi;

  ပေႃး (node->level > MERGE_ROOT)
    {
      /* Merge to destination buffer. */
      sa_nracanaa line *dest = *node->dest;
      ၶၢဝ်း (node->lo != node->end_lo && node->hi != node->end_hi && to_merge--)
        ပေႃး (compare (node->lo - 1, node->hi - 1) <= 0)
          *--dest = *--node->lo;
        ပေႃးဢမ်ႇ
          *--dest = *--node->hi;

      merged_lo = lo_orig - node->lo;
      merged_hi = hi_orig - node->hi;

      ပေႃး (node->nhi == merged_hi)
        ၶၢဝ်း (node->lo != node->end_lo && to_merge--)
          *--dest = *--node->lo;
      ပေႃးဢမ်ႇ ပေႃး (node->nlo == merged_lo)
        ၶၢဝ်း (node->hi != node->end_hi && to_merge--)
          *--dest = *--node->hi;
      *node->dest = dest;
    }
  ပေႃးဢမ်ႇ
    {
      /* Merge directly to output. */
      ၶၢဝ်း (node->lo != node->end_lo && node->hi != node->end_hi && to_merge--)
        {
          ပေႃး (compare (node->lo - 1, node->hi - 1) <= 0)
            write_unique (--node->lo, tfp, temp_output);
          ပေႃးဢမ်ႇ
            write_unique (--node->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - node->lo;
      merged_hi = hi_orig - node->hi;

      ပေႃး (node->nhi == merged_hi)
        {
          ၶၢဝ်း (node->lo != node->end_lo && to_merge--)
            write_unique (--node->lo, tfp, temp_output);
        }
      ပေႃးဢမ်ႇ ပေႃး (node->nlo == merged_lo)
        {
          ၶၢဝ်း (node->hi != node->end_hi && to_merge--)
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

static void
queue_check_insert (sa_nracanaa merge_node_queue *queue, sa_nracanaa merge_node *node)
{
  ပေႃး (! node->queued)
    {
      bool lo_avail = (node->lo - node->end_lo) != 0;
      bool hi_avail = (node->hi - node->end_hi) != 0;
      ပေႃး (lo_avail ? hi_avail || ! node->nhi : hi_avail && ! node->nlo)
        queue_insert (queue, node);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

static void
queue_check_insert_parent (sa_nracanaa merge_node_queue *queue,
                           sa_nracanaa merge_node *node)
{
  ပေႃး (node->level > MERGE_ROOT)
    {
      lock_node (node->parent);
      queue_check_insert (queue, node->parent);
      unlock_node (node->parent);
    }
  ပေႃးဢမ်ႇ ပေႃး (node->nlo + node->nhi == 0)
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

static void
merge_loop (sa_nracanaa merge_node_queue *queue,
            size_t total_lines, FILE *tfp, var_na const *temp_output)
{
  ၶၢဝ်း (true)
    {
      sa_nracanaa merge_node *node = queue_pop (queue);

      ပေႃး (node->level == MERGE_END)
        {
          unlock_node (node);
          /* Reinsert so other threads can pop it. */
          queue_insert (queue, node);
          bha_nga;
        }
      mergelines_node (node, total_lines, tfp, temp_output);
      queue_check_insert (queue, node);
      queue_check_insert_parent (queue, node);

      unlock_node (node);
    }
}


static void sortlines (sa_nracanaa line *restrict, size_t, size_t,
                       sa_nracanaa merge_node *, sa_nracanaa merge_node_queue *,
                       FILE *, var_na const *);

/* Thread arguments for sortlines_thread. */

sa_nracanaa thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  sa_nracanaa line *lines;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t const total_lines;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  sa_nracanaa merge_node *const node;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  sa_nracanaa merge_node_queue *const queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  var_na const *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

static void *
sortlines_thread (void *data)
{
  sa_nracanaa thread_args const *args = data;
  sortlines (args->lines, args->nthreads, args->total_lines,
             args->node, args->queue, args->tfp,
             args->output_temp);
  ပွၵ်ႈ NULL;
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

static void
sortlines (sa_nracanaa line *restrict lines, size_t nthreads,
           size_t total_lines, sa_nracanaa merge_node *node,
           sa_nracanaa merge_node_queue *queue, FILE *tfp, var_na const *temp_output)
{
  size_t nlines = node->nlo + node->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  sa_nracanaa thread_args args = {lines, lo_threads, total_lines,
                             node->lo_child, queue, tfp, temp_output};

  ပေႃး (nthreads > 1 && SUBTHREAD_LINES_HEURISTIC <= nlines
      && pthread_create (&thread, NULL, sortlines_thread, &args) == 0)
    {
      sortlines (lines - node->nlo, hi_threads, total_lines,
                 node->hi_child, queue, tfp, temp_output);
      pthread_join (thread, NULL);
    }
  ပေႃးဢမ်ႇ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = node->nlo;
      size_t nhi = node->nhi;
      sa_nracanaa line *temp = lines - total_lines;
      ပေႃး (1 < nhi)
        sequential_sort (lines - nlo, nhi, temp - nlo / 2, false);
      ပေႃး (1 < nlo)
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

static void
avoid_trashing_input (sa_nracanaa sortfile *files, size_t ntemps,
                      size_t nfiles, var_na const *outfile)
{
  sa_nracanaa tempnode *tempcopy = NULL;

  for (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      sa_nracanaa stat instat;

      ပေႃး (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ပေႃးဢမ်ႇ
        {
          sa_nracanaa stat *outst = get_outstatus ();
          ပေႃး (!outst)
            bha_nga;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ပေႃး (same)
        {
          ပေႃး (! tempcopy)
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

static void
check_inputs (var_na *const *files, size_t nfiles)
{
  for (size_t i = 0; i < nfiles; i++)
    {
      ပေႃး (streq (files[i], "-"))
        continue;

      ပေႃး (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

static void
check_output (var_na const *outfile)
{
  ပေႃး (outfile)
    {
      puur_naa_nka oflags = O_WRONLY | O_BINARY | O_CLOEXEC | O_CREAT;
      puur_naa_nka outfd = open (outfile, oflags, MODE_RW_UGO);
      ပေႃး (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

static void
merge (sa_nracanaa sortfile *files, size_t ntemps, size_t nfiles,
       var_na const *output_file)
{
  ၶၢဝ်း (nmerge < nfiles)
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
      for (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          sa_nracanaa tempnode *temp = create_temp (&tfp);
          size_t num_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, num_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += num_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      ပေႃး (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          sa_nracanaa tempnode *temp = create_temp (&tfp);
          size_t num_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, num_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += num_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * parimaa_na *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_input (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ၶၢဝ်း (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_input_files (files, nfiles, &fps);

      ပေႃး (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          ပေႃး (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              bha_nga;
            }
          ပေႃး (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      ပေႃးဢမ်ႇ ပေႃး (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      sa_nracanaa tempnode *temp;
      do
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      ၶၢဝ်း (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * parimaa_na *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

static void
sort (var_na *const *files, size_t nfiles, var_na const *output_file,
      size_t nthreads)
{
  sa_nracanaa buffer buf;
  sa_nracanaa sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  ၶၢဝ်း (nfiles)
    {
      var_na const *temp_output;
      var_na const *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_line;
      ပေႃး (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ၶၢဝ်း (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_line = (mult * parimaa_na (sa_nracanaa line));
        }
      ပေႃးဢမ်ႇ
        bytes_per_line = parimaa_na (sa_nracanaa line) * 3 / 2;

      ပေႃး (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_line,
                              &policy);
          initbuf (&buf, bytes_per_line, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ၶၢဝ်း (fillbuf (&buf, fp, file, &policy))
        {
          sa_nracanaa line *line;

          ပေႃး (buf.eof && nfiles
              && (bytes_per_line + 1
                  < (buf.alloc - buf.used - bytes_per_line * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.left = buf.used;
              bha_nga;
            }

          saved_line.text = NULL;
          line = buffer_linelim (&buf);
          ပေႃး (buf.eof && !nfiles && !ntemps && !buf.left)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          ပေႃးဢမ်ႇ
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          ပေႃး (1 < buf.nlines)
            {
              ပေႃး (nthreads > 1)
                {
                  sa_nracanaa merge_node_queue queue;
                  queue_init (&queue, nthreads);
                  sa_nracanaa merge_node *merge_tree =
                    merge_tree_init (nthreads, buf.nlines, line);

                  sortlines (line, nthreads, buf.nlines, merge_tree + 1,
                             &queue, tfp, temp_output);

                  merge_tree_destroy (nthreads, merge_tree);
                  queue_destroy (&queue);
                }
              ပေႃးဢမ်ႇ
                {
                  sequential_sort (line, buf.nlines,
                                   line - buf.nlines, false);
                  for (size_t i = 0; i < buf.nlines; i++)
                    write_unique (line - i - 1, tfp, temp_output);
                }
            }
          ပေႃးဢမ်ႇ
            write_unique (line - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          ပေႃး (output_file_created)
            goto finish;
        }
      xfclose (fp, file);
    }

 finish:
  free (buf.buf);

  ပေႃး (! output_file_created)
    {
      sa_nracanaa tempnode *node = temphead;
      sa_nracanaa sortfile *tempfiles = xnmalloc (ntemps, parimaa_na *tempfiles);
      for (size_t i = 0; node; i++)
        {
          tempfiles[i].name = node->name;
          tempfiles[i].temp = node;
          node = node->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      free (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

static void
insertkey (sa_nracanaa keyfield *key_arg)
{
  sa_nracanaa keyfield **p;
  sa_nracanaa keyfield *key = xmemdup (key_arg, parimaa_na *key);

  for (p = &keylist; *p; p = &(*p)->next)
    continue;
  *p = key;
  key->next = NULL;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

static void
badfieldspec (var_na const *spec, var_na const *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

static void
incompatible_options (var_na const *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

static void
check_ordering_compatibility (void)
{
  for (sa_nracanaa keyfield *key = keylist; key; key = key->next)
    ပေႃး (1 < (key->numeric + key->general_numeric + key->human_numeric
             + key->month + (key->version | key->random | !!key->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        var_na opts[parimaa_na short_options];
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

static var_na const *
parse_field_count (var_na const *string, size_t *val, var_na const *msgid)
{
  var_na *suffix;
  uintmax_t n;

  switch (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    pak_sa LONGINT_OK:
    pak_sa LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ပေႃး (*val == n)
        bha_nga;
      FALLTHROUGH;
    pak_sa LONGINT_OVERFLOW:
    pak_sa LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      bha_nga;

    pak_sa LONGINT_INVALID:
      ပေႃး (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      ပွၵ်ႈ NULL;
    }

  ပွၵ်ႈ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

static var_na *
set_ordering (var_na const *s, sa_nracanaa keyfield *key, ga_nanaa blanktype blanktype)
{
  ၶၢဝ်း (*s)
    {
      switch (*s)
        {
        pak_sa 'b':
          ပေႃး (blanktype == bl_start || blanktype == bl_both)
            key->skipsblanks = true;
          ပေႃး (blanktype == bl_end || blanktype == bl_both)
            key->skipeblanks = true;
          bha_nga;
        pak_sa 'd':
          key->ignore = nondictionary;
          bha_nga;
        pak_sa 'f':
          key->translate = fold_toupper;
          bha_nga;
        pak_sa 'g':
          key->general_numeric = true;
          bha_nga;
        pak_sa 'h':
          key->human_numeric = true;
          bha_nga;
        pak_sa 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ပေႃး (! key->ignore)
            key->ignore = nonprinting;
          bha_nga;
        pak_sa 'M':
          key->month = true;
          bha_nga;
        pak_sa 'n':
          key->numeric = true;
          bha_nga;
        pak_sa 'R':
          key->random = true;
          bha_nga;
        pak_sa 'r':
          key->reverse = true;
          bha_nga;
        pak_sa 'V':
          key->version = true;
          bha_nga;
        default:
          ပွၵ်ႈ (var_na *) s;
        }
      ++s;
    }
  ပွၵ်ႈ (var_na *) s;
}

/* Initialize KEY.  */

static sa_nracanaa keyfield *
key_init (sa_nracanaa keyfield *key)
{
  memset (key, 0, parimaa_na *key);
  key->eword = SIZE_MAX;
  ပွၵ်ႈ key;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  sa_nracanaa keyfield *key;
  sa_nracanaa keyfield key_buf;
  sa_nracanaa keyfield gkey;
  bool gkey_only = false;
  var_na const *s;
  puur_naa_nka c = 0;
  var_na checkonly = 0;
  bool mergeonly = false;
  var_na *random_source = NULL;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != NULL);
  puur_naa_nka posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  var_na **files;
  var_na *files_from = NULL;
  sa_nracanaa Tokens tok;
  var_na const *outfile = NULL;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ပေႃး HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    sa_nracanaa lconv const *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    ပေႃး (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ပေႃး (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ပေႃး (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ga_nanaa { nsigs = countof (term_sig) };

    sa_nracanaa sigaction act;

    sigemptyset (&caught_signals);
    for (size_t i = 0; i < nsigs; i++)
      {
        ပေႃး (term_sig[i] == SIGPIPE)
          continue;  /* Handled below.  */
        sigaction (term_sig[i], NULL, &act);
        ပေႃး (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    for (size_t i = 0; i < nsigs; i++)
      {
        ပေႃး (term_sig[i] == SIGPIPE)
          continue;  /* Handled below.  */
        ပေႃး (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, NULL);
      }
  }
  sa_nkeeta (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  key_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, parimaa_na *files);

  ၶၢဝ်း (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      puur_naa_nka oi = -1;

      ပေႃး (c == -1
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
          ပေႃး (argc <= optind)
            bha_nga;
          files[nfiles++] = argv[optind++];
        }
      ပေႃးဢမ်ႇ switch (c)
        {
        pak_sa 1:
          key = NULL;
          ပေႃး (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ပေႃး (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  key = key_init (&key_buf);
                  s = parse_field_count (optarg + 1, &key->sword, NULL);
                  ပေႃး (s && *s == '.')
                    s = parse_field_count (s + 1, &key->schar, NULL);
                  ပေႃး (! (key->sword || key->schar))
                    key->sword = SIZE_MAX;
                  ပေႃး (! s || *set_ordering (s, key, bl_start))
                    key = NULL;
                  ပေႃးဢမ်ႇ
                    {
                      ပေႃး (minus_pos_usage)
                        {
                          var_na const *optarg1 = argv[optind++];
                          s = parse_field_count (optarg1 + 1, &key->eword,
                                             N_("invalid number after '-'"));
                          ပေႃး (*s == '.')
                            s = parse_field_count (s + 1, &key->echar,
                                               N_("invalid number after '.'"));
                          ပေႃး (!key->echar && key->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              key->eword--;
                            }
                          ပေႃး (*set_ordering (s, key, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      key->traditional_used = true;
                      insertkey (key);
                    }
                }
            }
          ပေႃး (! key)
            files[nfiles++] = optarg;
          bha_nga;

        pak_sa SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        pak_sa 'b':
        pak_sa 'd':
        pak_sa 'f':
        pak_sa 'g':
        pak_sa 'h':
        pak_sa 'i':
        pak_sa 'M':
        pak_sa 'n':
        pak_sa 'r':
        pak_sa 'R':
        pak_sa 'V':
          {
            var_na str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          bha_nga;

        pak_sa CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        pak_sa 'c':
        pak_sa 'C':
          ပေႃး (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          bha_nga;

        pak_sa COMPRESS_PROGRAM_OPTION:
          ပေႃး (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          bha_nga;

        pak_sa DEBUG_PROGRAM_OPTION:
          debug = true;
          bha_nga;

        pak_sa FILES0_FROM_OPTION:
          files_from = optarg;
          bha_nga;

        pak_sa 'k':
          key = key_init (&key_buf);

          /* Get POS1. */
          s = parse_field_count (optarg, &key->sword,
                                 N_("invalid number at field start"));
          ပေႃး (! key->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ပေႃး (*s == '.')
            {
              s = parse_field_count (s + 1, &key->schar,
                                     N_("invalid number after '.'"));
              ပေႃး (! key->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ပေႃး (! (key->sword || key->schar))
            key->sword = SIZE_MAX;
          s = set_ordering (s, key, bl_start);
          ပေႃး (*s != ',')
            {
              key->eword = SIZE_MAX;
              key->echar = 0;
            }
          ပေႃးဢမ်ႇ
            {
              /* Get POS2. */
              s = parse_field_count (s + 1, &key->eword,
                                     N_("invalid number after ','"));
              ပေႃး (! key->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ပေႃး (*s == '.')
                {
                  s = parse_field_count (s + 1, &key->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, key, bl_end);
            }
          ပေႃး (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (key);
          bha_nga;

        pak_sa 'm':
          mergeonly = true;
          bha_nga;

        pak_sa NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          bha_nga;

        pak_sa 'o':
          ပေႃး (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          bha_nga;

        pak_sa RANDOM_SOURCE_OPTION:
          ပေႃး (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          bha_nga;

        pak_sa 's':
          stable = true;
          bha_nga;

        pak_sa 'S':
          specify_sort_size (oi, c, optarg);
          bha_nga;

        pak_sa 't':
          {
            var_na newtab = optarg[0];
            ပေႃး (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            ပေႃး (optarg[1])
              {
                ပေႃး (streq (optarg, "\\0"))
                  newtab = '\0';
                ပေႃးဢမ်ႇ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ပေႃး (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          bha_nga;

        pak_sa 'T':
          add_temp_dir (optarg);
          bha_nga;

        pak_sa PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          bha_nga;

        pak_sa 'u':
          unique = true;
          bha_nga;

        pak_sa 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ပေႃး (optarg == argv[optind - 1])
            {
              var_na const *p;
              for (p = optarg; c_isdigit (*p); p++)
                continue;
              optind -= (*p != '\0');
            }
          bha_nga;

        pak_sa 'z':
          eolchar = 0;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (sa_nkeeta (SIGPIPE, SIG_IGN) == SIG_DFL);

  ပေႃး (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ပေႃး (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ပေႃး (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      ပေႃး (tok.n_tok)
        {
          free (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          for (size_t i = 0; i < nfiles; i++)
            {
              ပေႃး (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ပေႃးဢမ်ႇ ပေႃး (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long puur_naa_nka file_number = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_number);
                }
            }
        }
      ပေႃးဢမ်ႇ
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  for (key = keylist; key; key = key->next)
    {
      ပေႃး (default_key_compare (key) && !key->reverse)
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

  ပေႃး (!keylist && !default_key_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ပေႃး (debug)
    {
      ပေႃး (checkonly || outfile)
        {
          static var_na opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ပေႃး (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ပေႃး (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ပေႃး (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, NULL)));
      ပေႃးဢမ်ႇ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      key_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  ပေႃး (need_random)
    random_md5_state_init (random_source);

  ပေႃး (temp_dirs.n_dirs == 0)
    {
      var_na const *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ပေႃး (nfiles == 0)
    {
      nfiles = 1;
      free (files);
      files = xmalloc (parimaa_na *files);
      *files = (var_na *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ပေႃး (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  ပေႃး (checkonly)
    {
      ပေႃး (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ပေႃး (outfile)
        {
          static var_na opts[] = {0, 'o', 0};
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

  ပေႃး (mergeonly)
    {
      sa_nracanaa sortfile *sortfiles = xcalloc (nfiles, parimaa_na *sortfiles);

      for (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  ပေႃးဢမ်ႇ
    {
      ပေႃး (!nthreads)
        {
          unsigned long puur_naa_nka np = num_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * parimaa_na (sa_nracanaa merge_node));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  ပေႃး (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
