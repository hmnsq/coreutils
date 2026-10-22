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

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#إذا !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#إذا !defined OPEN_MAX
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
تعداد { SUBTHREAD_قۇرلار_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_قۇرلار_HEURISTIC);

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
    MERGE_يىلتىز = 1
  };

/* The representation of the decimal point in the current locale.  */
ساكن حرف decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
ساكن صحيح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساكن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساكن bool hard_LC_COLLATE;
#إذا HAVE_NL_LANGINFO
ساكن bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
تعداد blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
بنية قۇر
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
  size_t سول;			/* Number of bytes left from previous reads. */
  size_t قۇر_bytes;		/* Number of bytes to reserve for each line. */
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
بنية merge_تۈگۈن
{
  بنية قۇر *lo;              /* Lines to merge from LO child node. */
  بنية قۇر *hi;              /* Lines to merge from HI child node. */
  بنية قۇر *end_lo;          /* End of available lines from LO. */
  بنية قۇر *end_hi;          /* End of available lines from HI. */
  بنية قۇر **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  بنية merge_تۈگۈن *parent;    /* Parent node. */
  بنية merge_تۈگۈن *lo_child;  /* LO child node. */
  بنية merge_تۈگۈن *hi_child;  /* HI child node. */
  unsigned صحيح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
بنية merge_تۈگۈن_queue
{
  بنية heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساكن بنية قۇر saved_قۇر;

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
#define MIN_MERGE_BUFFER_SIZE (2 + حجم (بنية قۇر))

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
#define كىرگۈزۈش_FILE_SIZE_GUESS (128 * 1024)

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
  لكل (بنية tempnode ثابت *تۈگۈن = temphead; تۈگۈن; تۈگۈن = تۈگۈن->next)
    unlink (تۈگۈن->name);
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
  إذا (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         ignore leading blanks when finding sort ئاچقۇچلار in each قۇر\n\
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
         compare according to general numerical قىممەت\n\
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
         compare according to string numerical قىممەت;\n\
         see full documentation لكل supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical ئاچقۇچلار.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; لكل more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check لكل sorted كىرگۈزۈش; افعل not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but افعل not report first bad قۇر\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the قۇر used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ كىرگۈزۈش from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, اقرأ names from standard كىرگۈزۈش\n\
"));
      oputs (_("\
  -k, --ئاچقۇچ=KEYDEF\n\
         sort via a ئاچقۇچ; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; افعل not sort\n\
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
         use SIZE لكل main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR لكل temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the سان of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of قۇرلار with equal ئاچقۇچلار;\n\
         with -c, check لكل strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         قۇر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] لكل start and stop position, where F is a\n\
field سان and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the قۇر's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options لكل that ئاچقۇچ.  If no ئاچقۇچ is given, use\n\
the entire قۇر as the ئاچقۇچ.  Use --debug to diagnose incorrect ئاچقۇچ usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (افتراضي), and so on لكل M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte قىممەتلەر.\n\
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
  إذا (status->valid)
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
  بنية tempnode ثابت *تۈگۈن = entry;
  أرجع تۈگۈن->pid % tabsize;
}

ساكن bool
proctab_comparator (فراغ ثابت *e1, فراغ ثابت *e2)
{
  بنية tempnode ثابت *n1 = e1;
  بنية tempnode ثابت *n2 = e2;
  أرجع n1->pid == n2->pid;
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

  إذا (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  وإلا إذا (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      إذا (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  أرجع cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساكن فراغ
register_proc (بنية tempnode *temp)
{
  إذا (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, عدم,
                                 proctab_hasher,
                                 proctab_comparator,
                                 عدم);
      إذا (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  إذا (! hash_قىستۇرۇش (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساكن bool
delete_proc (pid_t pid)
{
  بنية tempnode test;

  test.pid = pid;
  بنية tempnode *تۈگۈن = hash_remove (proctab, &test);
  إذا (! تۈگۈن)
    أرجع false;
  تۈگۈن->state = REAPED;
  أرجع true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ساكن فراغ
wait_proc (pid_t pid)
{
  إذا (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ساكن فراغ
reap_exited (فراغ)
{
  طالما (0 < nprocs && reap (0))
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
  طالما (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ساكن فراغ
exit_cleanup (فراغ)
{
  إذا (temphead)
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
  بنية tempnode *تۈگۈن =
    xmalloc (FLEXSIZEOF (بنية tempnode, name, len + حجم slashbase));
  حرف *file = تۈگۈن->name;
  بنية cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, حجم slashbase);
  تۈگۈن->next = عدم;
  إذا (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  إذا (0 <= fd)
    {
      *temptail = تۈگۈن;
      temptail = &تۈگۈن->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  إذا (fd < 0)
    {
      إذا (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      حرر (تۈگۈن);
      تۈگۈن = عدم;
    }

  *pfd = fd;
  أرجع تۈگۈن;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ساكن بنية stat *
get_outstatus (فراغ)
{
  ساكن صحيح outstat_errno;
  ساكن بنية stat outstat;
  إذا (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  أرجع outstat_errno < 0 ? &outstat : عدم;
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

  إذا (*how == 'r')
    {
      إذا (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      وإلا
        {
          صحيح fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? عدم : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  وإلا إذا (*how == 'w')
    {
      إذا (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          صحيح ftruncate_errno = errno;
          بنية stat *outst = get_outstatus ();
          إذا (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  وإلا
    affirm (!"unexpected mode passed to stream_open");

  أرجع fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ساكن FILE *
xfopen (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp = stream_open (file, how);
  إذا (!fp)
    sort_die (_("open failed"), file);
  أرجع fp;
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
      إذا (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      اكسر;

    افتراضي:
      إذا (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      اكسر;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ساكن فراغ
move_fd (صحيح oldfd, صحيح newfd)
{
  إذا (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_قىممەت (dup2 (oldfd, newfd));
      ignore_قىممەت (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساكن صحيح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحيح oldfd, صحيح newfd)
{
  صحيح result = 0;
  إذا (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      إذا (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  أرجع result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساكن حرف ثابت *
get_resolved_compress_program (فراغ)
{
  /* Use a cache, to perform the search only once.  */
  ساكن حرف ثابت *resolved_compress_program_cache /* = NULL */;

  إذا (resolved_compress_program_cache == عدم)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), عدم, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  أرجع resolved_compress_program_cache;
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
  إذا (resolved_compress_program == عدم)
    أرجع errno;

  إذا ((result = posix_spawnattr_init (&attr)))
    أرجع result;
  إذا ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      أرجع result;
    }

  إذا (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحيح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع saved_errno;
    }

  إذا ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      أرجع result;
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

  إذا (nmerge + 1 < nprocs)
    reap_some ();

  طالما (tries--)
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

      إذا (result != EAGAIN)
        اكسر;
      وإلا
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

  إذا (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  وإلا
    ++nprocs;

  أرجع result;
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
  بنية tempnode *تۈگۈن = create_temp_file (&tempfd, survive_fd_exhaustion);
  إذا (! تۈگۈن)
    أرجع عدم;

  تۈگۈن->state = UNCOMPRESSED;

  إذا (compress_program)
    {
      صحيح pipefds[2];
      ساكن صحيح last_result = 0;

      صحيح result = pipe_child (&تۈگۈن->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      إذا (result)
        {
          إذا (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      وإلا
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (تۈگۈن);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  إذا (! *pfp)
    sort_die (_("couldn't create temporary file"), تۈگۈن->name);

  أرجع تۈگۈن;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساكن بنية tempnode *
create_temp (FILE **pfp)
{
  أرجع maybe_create_temp (pfp, false);
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

  إذا (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  إذا (tempfd < 0)
    أرجع عدم;

  pid_t child;
  صحيح result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  إذا (result)
    {
      إذا (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  وإلا
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      إذا (! fp)
        {
          صحيح saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  أرجع fp;
}

/* Append DIR to the array of temporary directory names.  */
ساكن فراغ
add_temp_dir (حرف ثابت *dir)
{
  إذا (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
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
  بنية tempnode *تۈگۈن;
  بنية tempnode *next;
  صحيح unlink_status;
  صحيح unlink_errno = 0;
  بنية cs_status cs;

  لكل (pnode = &temphead; (تۈگۈن = *pnode)->name != name; pnode = &تۈگۈن->next)
    استمر;

  إذا (تۈگۈن->state == UNREAPED)
    wait_proc (تۈگۈن->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = تۈگۈن->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  إذا (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  إذا (! next)
    temptail = pnode;
  حرر (تۈگۈن);
}

#إذا HAVE_NL_LANGINFO

ساكن صحيح
struct_month_cmp (فراغ ثابت *m1, فراغ ثابت *m2)
{
  بنية month ثابت *month1 = m1;
  بنية month ثابت *month2 = m2;
  أرجع strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

ساكن فراغ
inittables (فراغ)
{
  لكل (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#إذا HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  إذا (hard_LC_TIME)
    {
      لكل (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          لكل (j = k = 0; j < s_len; j++)
            إذا (! isblank (to_uchar (s[j])))
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

  إذا (e == LONGINT_OK)
    {
      nmerge = n;
      إذا (nmerge != n)
        e = LONGINT_OVERFLOW;
      وإلا
        {
          إذا (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          وإلا إذا (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          وإلا
            أرجع;
        }
    }

  إذا (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  وإلا
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
  إذا (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  إذا (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    بدل (suffix[0])
      {
      حالة 'b':
        e = LONGINT_OK;
        اكسر;

      حالة '%':
        {
          مزدوج mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          إذا (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          وإلا
            e = LONGINT_OVERFLOW;
        }
        اكسر;
      }

  إذا (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      إذا (n < sort_size)
        أرجع;

      sort_size = n;
      إذا (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          أرجع;
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
  إذا (e == LONGINT_OVERFLOW)
    أرجع SIZE_MAX;
  إذا (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  إذا (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  إذا (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  أرجع nthreads;
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
  إذا (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  إذا (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  إذا (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  مزدوج avail = physmem_available ();
  مزدوج total = physmem_total ();
  مزدوج mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  إذا (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  إذا (mem < size)
    size = mem;
  أرجع MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساكن bool
قۇر_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = حجم (بنية قۇر);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  إذا (ckd_add (&aligned, size, padding))
    أرجع false;

  *alloc = aligned;
  أرجع true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساكن size_t
default_initial_sort_size (size_t قۇر_bytes)
{
  size_t size = قۇر_bytes + 2;
  size_t كىرگۈزۈش_size;

  إذا (ckd_mul (&كىرگۈزۈش_size, كىرگۈزۈش_FILE_SIZE_GUESS, قۇر_bytes + 1)
      || ckd_add (&size, size, كىرگۈزۈش_size))
    أرجع SIZE_MAX;

  أرجع MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساكن bool
كىرگۈزۈش_size_buffer_bytes (uintmax_t file_size, size_t قۇر_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_كىرگۈزۈش_byte = قۇر_bytes + 1;

  إذا (SIZE_MAX / worst_case_per_كىرگۈزۈش_byte < file_size)
    أرجع false;

  size_t size = file_size * worst_case_per_كىرگۈزۈش_byte;
  إذا (SIZE_MAX - size < 1)
    أرجع false;

  *worst_case = size + 1;
  أرجع true;
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
                    size_t قۇر_bytes,
                    بنية sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (قۇر_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_كىرگۈزۈش_byte = قۇر_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_كىرگۈزۈش_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  لكل (size_t i = 0; i < nfiles; i++)
    {
      بنية stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      إذا (size_bound <= size)
        {
          policy->initial = size_bound;
          أرجع;
        }

      إذا ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      إذا (known_size)
        file_size = st.st_size;
      وإلا
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          إذا (sort_size)
            {
              policy->initial = size_bound;
              أرجع;
            }
          file_size = كىرگۈزۈش_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      إذا (! كىرگۈزۈش_size_buffer_bytes (file_size, قۇر_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          أرجع;
        }

      إذا (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          أرجع;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساكن فراغ
initbuf (بنية buffer *buf, size_t قۇر_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  طالما (true)
    {
      إذا (! قۇر_aligned_size (&alloc))
        buf->buf = عدم;
      وإلا
        buf->buf = malloc (alloc);
      إذا (buf->buf)
        اكسر;
      alloc /= 2;
      إذا (alloc <= قۇر_bytes + 1)
        xalloc_die ();
    }

  buf->قۇر_bytes = قۇر_bytes;
  buf->alloc = alloc;
  buf->used = buf->سول = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساكن مضمن بنية قۇر *
buffer_linelim (بنية buffer ثابت *buf)
{
  فراغ *linelim = buf->buf + buf->alloc;
  أرجع linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ساكن bool
try_growbuf (بنية buffer *buf, size_t alloc)
{
  إذا (! قۇر_aligned_size (&alloc) || alloc <= buf->alloc)
    أرجع false;

  حرف *newbuf = malloc (alloc);
  إذا (! newbuf)
    أرجع false;

  حرف *oldbuf = buf->buf;
  بنية قۇر *old_linelim = buffer_linelim (buf);
  بنية قۇر *old_قۇر = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  بنية قۇر *new_linelim = (فراغ *) (newbuf + alloc);
  بنية قۇر *new_قۇر = new_linelim - buf->nlines;
  memcpy (new_قۇر, old_قۇر, buf->nlines * حجم *new_قۇر);

  لكل (بنية قۇر *قۇر = new_قۇر; قۇر < new_linelim; قۇر++)
    {
      قۇر->text = newbuf + (قۇر->text - oldbuf);
      إذا (keylist)
        {
          قۇر->keybeg = newbuf + (قۇر->keybeg - oldbuf);
          قۇر->keylim = newbuf + (قۇر->keylim - oldbuf);
        }
    }

  حرر (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  أرجع true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساكن bool
maybe_growbuf (بنية buffer *buf, بنية sort_buffer_policy *policy)
{
  إذا (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    أرجع false;

  size_t alloc;
  إذا (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  وإلا
    alloc = policy->limit;

  إذا (try_growbuf (buf, alloc))
    أرجع true;

  policy->growth_failed = true;
  أرجع false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ساكن حرف *
begfield (بنية قۇر ثابت *قۇر, بنية keyfield ثابت *ئاچقۇچ)
{
  حرف *ptr = قۇر->text, *lim = ptr + قۇر->length - 1;
  size_t sword = ئاچقۇچ->sword;
  size_t schar = ئاچقۇچ->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  إذا (tab != TAB_DEFAULT)
    طالما (ptr < lim && sword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        إذا (ptr < lim)
          ++ptr;
      }
  وإلا
    طالما (ptr < lim && sword--)
      {
        طالما (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  إذا (ئاچقۇچ->skipsblanks)
    طالما (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  إذا (schar < remaining_bytes)
    ptr += schar;
  وإلا
    ptr = lim;

  أرجع ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
ساكن حرف *
limfield (بنية قۇر ثابت *قۇر, بنية keyfield ثابت *ئاچقۇچ)
{
  حرف *ptr = قۇر->text, *lim = ptr + قۇر->length - 1;
  size_t eword = ئاچقۇچ->eword, echar = ئاچقۇچ->echar;

  إذا (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  إذا (tab != TAB_DEFAULT)
    طالما (ptr < lim && eword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        إذا (ptr < lim && (eword || echar))
          ++ptr;
      }
  وإلا
    طالما (ptr < lim && eword--)
      {
        طالما (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[to_uchar (*ptr)])
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
  إذا (tab != TAB_DEFAULT)
    {
      حرف *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      إذا (newlim)
        lim = newlim;
    }
  وإلا
    {
      حرف *newlim;
      newlim = ptr;
      طالما (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      طالما (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  إذا (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      إذا (ئاچقۇچ->skipeblanks)
        طالما (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      إذا (echar < remaining_bytes)
        ptr += echar;
      وإلا
        ptr = lim;
    }

  أرجع ptr;
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
  بنية keyfield ثابت *ئاچقۇچ = keylist;
  حرف eol = eolchar;
  size_t قۇر_bytes = buf->قۇر_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  إذا (buf->eof)
    أرجع false;

  إذا (buf->used != buf->سول)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->سول, buf->سول);
      buf->used = buf->سول;
      buf->nlines = 0;
    }

  طالما (true)
    {
      حرف *ptr = buf->buf + buf->used;
      بنية قۇر *linelim = buffer_linelim (buf);
      بنية قۇر *قۇر = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * قۇر_bytes - ptr;
      حرف *قۇر_start = buf->nlines ? قۇر->text + قۇر->length : buf->buf;

      طالما (قۇر_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (قۇر_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + bytes_read;
          حرف *p;
          avail -= bytes_read;

          إذا (bytes_read != readsize)
            {
              إذا (ferror (fp))
                sort_die (_("read failed"), file);
              إذا (feof (fp))
                {
                  buf->eof = true;
                  إذا (buf->buf == ptrlim)
                    أرجع false;
                  إذا (قۇر_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          طالما ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              قۇر--;
              قۇر->text = قۇر_start;
              قۇر->length = ptr - قۇر_start;
              mergesize = MAX (mergesize, قۇر->length);
              avail -= قۇر_bytes;

              إذا (ئاچقۇچ)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  قۇر->keylim = (ئاچقۇچ->eword == SIZE_MAX
                                  ? p
                                  : limfield (قۇر, ئاچقۇچ));

                  إذا (ئاچقۇچ->sword != SIZE_MAX)
                    قۇر->keybeg = begfield (قۇر, ئاچقۇچ);
                  وإلا
                    {
                      إذا (ئاچقۇچ->skipsblanks)
                        طالما (blanks[to_uchar (*قۇر_start)])
                          قۇر_start++;
                      قۇر->keybeg = قۇر_start;
                    }
                }

              قۇر_start = ptr;
            }

          ptr = ptrlim;
          إذا (buf->eof)
            اكسر;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - قۇر;
      إذا (buf->nlines != 0)
        {
          إذا (! buf->eof && maybe_growbuf (buf, policy))
            استمر;
          buf->سول = ptr - قۇر_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          أرجع true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t قۇر_alloc = buf->alloc / حجم (بنية قۇر);
        buf->buf = xpalloc (buf->buf, &قۇر_alloc, 1, -1, حجم (بنية قۇر));
        buf->alloc = قۇر_alloc * حجم (بنية قۇر);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ساكن حرف ثابت unit_order[UCHAR_LIM] =
  {
#إذا ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#وإلا
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
traverse_raw_سان (حرف ثابت **سان)
{
  حرف ثابت *p = *سان;
  حرف ch;
  حرف max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  طالما (c_isdigit (ch = *p++))
    {
      إذا (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      إذا (ends_with_thousands_sep)
        ++p;
    }

  إذا (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *سان = p - 2;
      أرجع max_digit;
    }

  إذا (ch == decimal_point)
    طالما (c_isdigit (ch = *p++))
      إذا (max_digit < ch)
        max_digit = ch;

  *سان = p - 1;
  أرجع max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
ساكن صحيح
find_unit_order (حرف ثابت *سان)
{
  bool minus_sign = (*سان == '-');
  حرف ثابت *p = سان + minus_sign;
  حرف max_digit = traverse_raw_سان (&p);
  إذا ('0' < max_digit)
    {
      unsigned حرف ch = *p;
      صحيح order = unit_order[ch];
      أرجع (minus_sign ? -order : order);
    }
  وإلا
    أرجع 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
ساكن صحيح
human_numcompare (حرف ثابت *a, حرف ثابت *b)
{
  طالما (blanks[to_uchar (*a)])
    a++;
  طالما (blanks[to_uchar (*b)])
    b++;

  صحيح diff = find_unit_order (a) - find_unit_order (b);
  أرجع (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
ساكن صحيح
numcompare (حرف ثابت *a, حرف ثابت *b)
{
  طالما (blanks[to_uchar (*a)])
    a++;
  طالما (blanks[to_uchar (*b)])
    b++;

  أرجع strnumcmp (a, b, decimal_point, thousands_sep);
}

ساكن صحيح
nan_compare (long مزدوج a, long مزدوج b)
{
  حرف buf[2][حجم "-nan""()" + CHAR_BIT * حجم a];
  snprintf (buf[0], حجم buf[0], "%Lf", a);
  snprintf (buf[1], حجم buf[1], "%Lf", b);
  أرجع strcmp (buf[0], buf[1]);
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
  إذا (sa == ea)
    أرجع sb == eb ? 0 : -1;
  إذا (sb == eb)
    أرجع 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  أرجع (a < b ? -1
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

  طالما (blanks[to_uchar (*month)])
    month++;

  افعل
    {
      size_t ix = (lo + hi) / 2;
      حرف ثابت *m = month;
      حرف ثابت *n = monthtab[ix].name;

      لكل (;; m++, n++)
        {
          إذا (!*n)
            {
              إذا (ea)
                *ea = (حرف *) m;
              أرجع monthtab[ix].val;
            }
          إذا (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              اكسر;
            }
          وإلا إذا (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              اكسر;
            }
        }
    }
  طالما (lo < hi);

  أرجع 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# إذا 14 <= __GNUC__
#  pragma GCC diagnostic ئىتتىرىش
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#ضمن "md5.h"

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# إذا 14 <= __GNUC__
#  pragma GCC diagnostic چىقىرىش
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
بەلگە_address (فراغ *handle, حرف ثابت *بەلگە)
{
  فراغ *address = dlsym (handle, بەلگە);
  إذا (!address)
    link_failure ();
  أرجع address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
ساكن فراغ
link_libcrypto (فراغ)
{
#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  فراغ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  إذا (!handle)
    link_failure ();
  ptr_MD5_Init = بەلگە_address (handle, "MD5_Init");
  ptr_MD5_Update = بەلگە_address (handle, "MD5_Update");
  ptr_MD5_Final = بەلگە_address (handle, "MD5_Final");
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
  إذا (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, حجم buf);
  إذا (randread_free (r) != 0)
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

  إذا (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  أرجع translated_size;
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

  إذا (hard_LC_COLLATE)
    {
      حرف ثابت *lima = texta + lena;
      حرف ثابت *limb = textb + lenb;

      طالما (true)
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
          إذا (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              حرر (allocated);
              buf = allocated = malloc (bufsize);
              إذا (! buf)
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

          إذا (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              إذا (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              حرر (allocated);
              buf = allocated = xmalloc (bufsize);
              إذا (texta < lima)
                strxfrm (buf, texta, sizea);
              إذا (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          إذا (texta < lima)
            texta += strlen (texta) + 1;
          إذا (textb < limb)
            textb += strlen (textb) + 1;
          إذا (! (texta < lima || textb < limb))
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
          إذا (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              إذا (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  صحيح diff = memcmp (dig[0], dig[1], حجم dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  إذا (! diff)
    {
      إذا (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          إذا (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  حرر (allocated);

  أرجع diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ساكن size_t
debug_width (حرف ثابت *text, حرف ثابت *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  طالما (text < lim)
    width += (*text++ == '\t');
  أرجع width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساكن فراغ
mark_ئاچقۇچ (size_t offset, size_t width)
{
  طالما (offset--)
    putchar (' ');

  إذا (!width)
    printf (_("^ no match for key\n"));
  وإلا
    {
      افعل
        putchar ('_');
      طالما (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساكن مضمن bool
ئاچقۇچ_numeric (بنية keyfield ثابت *ئاچقۇچ)
{
  أرجع ئاچقۇچ->numeric || ئاچقۇچ->general_numeric || ئاچقۇچ->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساكن فراغ
debug_ئاچقۇچ (بنية قۇر ثابت *قۇر, بنية keyfield ثابت *ئاچقۇچ)
{
  حرف *text = قۇر->text;
  حرف *beg = text;
  حرف *lim = text + قۇر->length - 1;

  إذا (ئاچقۇچ)
    {
      إذا (ئاچقۇچ->sword != SIZE_MAX)
        beg = begfield (قۇر, ئاچقۇچ);
      إذا (ئاچقۇچ->eword != SIZE_MAX)
        {
          lim = limfield (قۇر, ئاچقۇچ);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      إذا ((ئاچقۇچ->skipsblanks && ئاچقۇچ->sword == SIZE_MAX)
          || ئاچقۇچ->month || ئاچقۇچ_numeric (ئاچقۇچ))
        {
          حرف saved = *lim;
          *lim = '\0';

          طالما (blanks[to_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          إذا (lim < beg)
            tighter_lim = lim;
          وإلا إذا (ئاچقۇچ->month)
            getmonth (beg, &tighter_lim);
          وإلا إذا (ئاچقۇچ->general_numeric)
            ignore_قىممەت (strtold (beg, &tighter_lim));
          وإلا إذا (ئاچقۇچ->numeric || ئاچقۇچ->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_سان (&p);
              إذا ('0' <= max_digit)
                {
                  unsigned حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (ئاچقۇچ->human_numeric && unit_order[ch]);
                }
            }
          وإلا
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_ئاچقۇچ (offset, width);
}

/* Debug LINE by underlining its keys.  */

ساكن فراغ
debug_قۇر (بنية قۇر ثابت *قۇر)
{
  بنية keyfield ثابت *ئاچقۇچ = keylist;

  افعل
    debug_ئاچقۇچ (قۇر, ئاچقۇچ);
  طالما (ئاچقۇچ && ((ئاچقۇچ = ئاچقۇچ->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساكن bool
default_ئاچقۇچ_compare (بنية keyfield ثابت *ئاچقۇچ)
{
  أرجع ! (ئاچقۇچ->ignore
            || ئاچقۇچ->translate
            || ئاچقۇچ->skipsblanks
            || ئاچقۇچ->skipeblanks
            || ئاچقۇچ_numeric (ئاچقۇچ)
            || ئاچقۇچ->month
            || ئاچقۇچ->version
            || ئاچقۇچ->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساكن فراغ
ئاچقۇچ_to_opts (بنية keyfield ثابت *ئاچقۇچ, حرف *opts)
{
  إذا (ئاچقۇچ->skipsblanks || ئاچقۇچ->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  إذا (ئاچقۇچ->ignore == nondictionary)
    *opts++ = 'd';
  إذا (ئاچقۇچ->translate)
    *opts++ = 'f';
  إذا (ئاچقۇچ->general_numeric)
    *opts++ = 'g';
  إذا (ئاچقۇچ->human_numeric)
    *opts++ = 'h';
  إذا (ئاچقۇچ->ignore == nonprinting)
    *opts++ = 'i';
  إذا (ئاچقۇچ->month)
    *opts++ = 'M';
  إذا (ئاچقۇچ->numeric)
    *opts++ = 'n';
  إذا (ئاچقۇچ->random)
    *opts++ = 'R';
  إذا (ئاچقۇچ->reverse)
    *opts++ = 'r';
  إذا (ئاچقۇچ->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساكن فراغ
ئاچقۇچ_warnings (بنية keyfield ثابت *gkey, bool gkey_only)
{
  بنية keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  لكل (بنية keyfield *ئاچقۇچ = keylist; ئاچقۇچ; ئاچقۇچ = ئاچقۇچ->next, keynum++)
    {
      إذا (ئاچقۇچ_numeric (ئاچقۇچ))
        {
          إذا (ئاچقۇچ->general_numeric)
            general_numeric_field = true;
          وإلا
            basic_numeric_field = true;
        }

      إذا (ئاچقۇچ->traditional_used)
        {
          size_t sword = ئاچقۇچ->sword;
          size_t eword = ئاچقۇچ->eword;
          حرف tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          حرف obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          حرف nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          حرف *po = obuf;
          حرف *pn = nbuf;

          إذا (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          إذا (ئاچقۇچ->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ئاچقۇچ->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = ئاچقۇچ->sword != SIZE_MAX && ئاچقۇچ->eword < ئاچقۇچ->sword;
      إذا (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ئاچقۇچ_numeric (ئاچقۇچ) || ئاچقۇچ->month;
      bool قۇر_offset = ئاچقۇچ->eword == 0 && ئاچقۇچ->echar != 0; /* -k1.x,1.y  */
      إذا (!zero_width && !gkey_only && tab == TAB_DEFAULT && !قۇر_offset
          && ((!ئاچقۇچ->skipsblanks && !implicit_skip)
              || (!ئاچقۇچ->skipsblanks && ئاچقۇچ->schar)
              || (!ئاچقۇچ->skipeblanks && ئاچقۇچ->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      إذا (!gkey_only && ئاچقۇچ_numeric (ئاچقۇچ))
        {
          size_t sword = ئاچقۇچ->sword + 1;
          size_t eword = ئاچقۇچ->eword + 1;
          إذا (!sword)
            sword++;
          إذا (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              إذا (ئاچقۇچ->general_numeric)
                general_numeric_field_span = true;
              وإلا
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      إذا (ugkey.ignore && (ugkey.ignore == ئاچقۇچ->ignore))
        ugkey.ignore = عدم;
      إذا (ugkey.translate && (ugkey.translate == ئاچقۇچ->translate))
        ugkey.translate = عدم;
      ugkey.skipsblanks &= !ئاچقۇچ->skipsblanks;
      ugkey.skipeblanks &= !ئاچقۇچ->skipeblanks;
      ugkey.month &= !ئاچقۇچ->month;
      ugkey.numeric &= !ئاچقۇچ->numeric;
      ugkey.general_numeric &= !ئاچقۇچ->general_numeric;
      ugkey.human_numeric &= !ئاچقۇچ->human_numeric;
      ugkey.random &= !ئاچقۇچ->random;
      ugkey.version &= !ئاچقۇچ->version;
      ugkey.reverse &= !ئاچقۇچ->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool سان_locale_warned = false;
  إذا (basic_numeric_field_span)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          سان_locale_warned = true;
        }
    }
  إذا (basic_numeric_field_span || general_numeric_field_span)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_point, 0})));
          سان_locale_warned = true;
        }
      وإلا إذا (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      وإلا إذا (general_numeric_field_span && tab == '+')
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
  إذا ((basic_numeric_field || general_numeric_field) && ! سان_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_point, 0})));

    }

  إذا (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  إذا (!default_ئاچقۇچ_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      إذا (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      ئاچقۇچ_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  إذا (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ساكن صحيح
diff_reversed (صحيح diff, bool reversed)
{
  أرجع reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ساكن صحيح
keycompare (بنية قۇر ثابت *a, بنية قۇر ثابت *b)
{
  بنية keyfield *ئاچقۇچ = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحيح diff;

  طالما (true)
    {
      حرف ثابت *translate = ئاچقۇچ->translate;
      bool ثابت *ignore = ئاچقۇچ->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      إذا (hard_LC_COLLATE || ئاچقۇچ_numeric (ئاچقۇچ)
          || ئاچقۇچ->month || ئاچقۇچ->random || ئاچقۇچ->version)
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

          إذا (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              إذا (size <= حجم stackbuf)
                ta = stackbuf;
              وإلا
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              لكل (tlena = i = 0; i < lena; i++)
                إذا (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              لكل (tlenb = i = 0; i < lenb; i++)
                إذا (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          إذا (ئاچقۇچ->numeric)
            diff = numcompare (ta, tb);
          وإلا إذا (ئاچقۇچ->general_numeric)
            diff = general_numcompare (ta, tb);
          وإلا إذا (ئاچقۇچ->human_numeric)
            diff = human_numcompare (ta, tb);
          وإلا إذا (ئاچقۇچ->month)
            diff = getmonth (ta, عدم) - getmonth (tb, عدم);
          وإلا إذا (ئاچقۇچ->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          وإلا إذا (ئاچقۇچ->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          وإلا
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              إذا (tlena == 0)
                diff = - NONZERO (tlenb);
              وإلا إذا (tlenb == 0)
                diff = 1;
              وإلا
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          حرر (allocated);
        }
      وإلا إذا (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  افعل									\
    {									\
          طالما (true)							\
            {								\
              طالما (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              طالما (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              إذا (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  اكسر;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              إذا (diff)							\
                اكسر;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  طالما (0)

          إذا (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          وإلا
            CMP_WITH_IGNORE (*texta, *textb);
        }
      وإلا
        {
          size_t lenmin = MIN (lena, lenb);
          إذا (lenmin == 0)
            diff = 0;
          وإلا إذا (translate)
            {
              size_t i = 0;
              افعل
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  إذا (diff)
                    اكسر;
                  i++;
                }
              طالما (i < lenmin);
            }
          وإلا
            diff = memcmp (texta, textb, lenmin);

          إذا (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      إذا (diff)
        اكسر;

      ئاچقۇچ = ئاچقۇچ->next;
      إذا (! ئاچقۇچ)
        أرجع 0;

      /* Find the beginning and limit of the next field.  */
      إذا (ئاچقۇچ->eword != SIZE_MAX)
        lima = limfield (a, ئاچقۇچ), limb = limfield (b, ئاچقۇچ);
      وإلا
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      إذا (ئاچقۇچ->sword != SIZE_MAX)
        texta = begfield (a, ئاچقۇچ), textb = begfield (b, ئاچقۇچ);
      وإلا
        {
          texta = a->text, textb = b->text;
          إذا (ئاچقۇچ->skipsblanks)
            {
              طالما (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              طالما (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  أرجع diff_reversed (diff, ئاچقۇچ->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساكن صحيح
compare (بنية قۇر ثابت *a, بنية قۇر ثابت *b)
{
  صحيح diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  إذا (keylist)
    {
      diff = keycompare (a, b);
      إذا (diff || unique || stable)
        أرجع diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  إذا (alen == 0)
    diff = - NONZERO (blen);
  وإلا إذا (blen == 0)
    diff = 1;
  وإلا إذا (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  وإلا
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      إذا (!diff)
        diff = _GL_CMP (alen, blen);
    }

  أرجع diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساكن فراغ
write_قۇر (بنية قۇر ثابت *قۇر, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = قۇر->text;
  size_t n_bytes = قۇر->length;
  حرف *ebuf = buf + n_bytes;

  إذا (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      حرف ثابت *c = buf;

      طالما (c < ebuf)
        {
          حرف wc = *c++;
          إذا (wc == '\t')
            wc = '>';
          وإلا إذا (c == ebuf)
            wc = '\n';
          إذا (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_قۇر (قۇر);
    }
  وإلا
    {
      ebuf[-1] = eolchar;
      إذا (fwrite (buf, 1, n_bytes, fp) != n_bytes)
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
  بنية قۇر temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t قۇر_سان = 0;
  بنية keyfield ثابت *ئاچقۇچ = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (بنية قۇر),
           MAX (merge_buffer_size, sort_size));
  temp.text = عدم;

  طالما (fillbuf (&buf, fp, file_name, عدم))
    {
      بنية قۇر ثابت *قۇر = buffer_linelim (&buf);
      بنية قۇر ثابت *linebase = قۇر - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      إذا (alloc && nonunique <= compare (&temp, قۇر - 1))
        {
        found_disorder:
          {
            إذا (checkonly == 'c')
              {
                بنية قۇر ثابت *disorder_قۇر = قۇر - 1;
                uintmax_t disorder_قۇر_سان =
                  buffer_linelim (&buf) - disorder_قۇر + قۇر_سان;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_قۇر_سان);
                write_قۇر (disorder_قۇر, stderr, _("standard error"));
              }

            ordered = false;
            اكسر;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      طالما (linebase < --قۇر)
        إذا (nonunique <= compare (قۇر, قۇر - 1))
          اذهب found_disorder;

      قۇر_سان += buf.nlines;

      /* Save the last line of the buffer.  */
      إذا (alloc < قۇر->length)
        {
          افعل
            {
              alloc *= 2;
              إذا (! alloc)
                {
                  alloc = قۇر->length;
                  اكسر;
                }
            }
          طالما (alloc < قۇر->length);

          حرر (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, قۇر->text, قۇر->length);
      temp.length = قۇر->length;
      إذا (ئاچقۇچ)
        {
          temp.keybeg = temp.text + (قۇر->keybeg - قۇر->text);
          temp.keylim = temp.text + (قۇر->keylim - قۇر->text);
        }
    }

  xfclose (fp, file_name);
  حرر (buf.buf);
  حرر (temp.text);
  أرجع ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساكن size_t
open_كىرگۈزۈش_files (بنية sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحيح i;

  /* Open as many input files as we can.  */
  لكل (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      إذا (!fps[i])
        اكسر;
    }

  أرجع i;
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
  بنية قۇر saved;		/* Saved line storage for unique check. */
  بنية قۇر ثابت *savedline = عدم;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  بنية قۇر ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  بنية قۇر ثابت **base = xnmalloc (nfiles, حجم *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  بنية keyfield ثابت *ئاچقۇچ = keylist;
  saved.text = عدم;

  /* Read initial lines from each input file. */
  لكل (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], حجم (بنية قۇر),
               MAX (merge_buffer_size, sort_size / nfiles));
      إذا (fillbuf (&buffer[i], fps[i], files[i].name, عدم))
        {
          بنية قۇر ثابت *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      وإلا
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          إذا (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          حرر (buffer[i].buf);
          --nfiles;
          لكل (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  لكل (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  لكل (size_t i = 1; i < nfiles; ++i)
    إذا (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  طالما (nfiles)
    {
      بنية قۇر ثابت *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      إذا (unique)
        {
          إذا (savedline && compare (savedline, smallest))
            {
              savedline = عدم;
              write_قۇر (&saved, ofp, output_file);
            }
          إذا (!savedline)
            {
              savedline = &saved;
              إذا (savealloc < smallest->length)
                {
                  افعل
                    إذا (! savealloc)
                      {
                        savealloc = smallest->length;
                        اكسر;
                      }
                  طالما ((savealloc *= 2) < smallest->length);

                  حرر (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              إذا (ئاچقۇچ)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      وإلا
        write_قۇر (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      إذا (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      وإلا
        {
          إذا (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       عدم))
            {
              بنية قۇر ثابت *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          وإلا
            {
              /* We reached EOF on fps[ord[0]].  */
              لكل (size_t i = 1; i < nfiles; ++i)
                إذا (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              إذا (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              حرر (buffer[ord[0]].buf);
              لكل (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              لكل (size_t i = 0; i < nfiles; ++i)
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
        size_t سانى_of_smaller_قۇرلار;

        طالما (lo < hi)
          {
            صحيح cmp = compare (cur[ord0], cur[ord[probe]]);
            إذا (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            وإلا
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        سانى_of_smaller_قۇرلار = lo - 1;
        لكل (size_t j = 0; j < سانى_of_smaller_قۇرلار; j++)
          ord[j] = ord[j + 1];
        ord[سانى_of_smaller_قۇرلار] = ord0;
      }
    }

  إذا (unique && savedline)
    {
      write_قۇر (&saved, ofp, output_file);
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
  size_t nopened = open_كىرگۈزۈش_files (files, nfiles, &fps);
  إذا (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  أرجع nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساكن فراغ
mergelines (بنية قۇر *restrict t, size_t nlines,
            بنية قۇر ثابت *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية قۇر *hi = t - nlo;

  طالما (true)
    إذا (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        إذا (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            أرجع;
          }
      }
    وإلا
      {
        *--t = *--hi;
        إذا (! --nhi)
          {
            افعل
              *--t = *--lo;
            طالما (--nlo);

            أرجع;
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
sequential_sort (بنية قۇر *restrict قۇرلار, size_t nlines,
                 بنية قۇر *restrict temp, bool to_temp)
{
  إذا (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحيح swap = (0 < compare (&قۇرلار[-1], &قۇرلار[-2]));
      إذا (to_temp)
        {
          temp[-1] = قۇرلار[-1 - swap];
          temp[-2] = قۇرلار[-2 + swap];
        }
      وإلا إذا (swap)
        {
          temp[-1] = قۇرلار[-1];
          قۇرلار[-1] = قۇرلار[-2];
          قۇرلار[-2] = temp[-1];
        }
    }
  وإلا
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      بنية قۇر *lo = قۇرلار;
      بنية قۇر *hi = قۇرلار - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      إذا (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      وإلا إذا (!to_temp)
        temp[-1] = lo[-1];

      بنية قۇر *dest;
      بنية قۇر ثابت *sorted_lo;
      إذا (to_temp)
        {
          dest = temp;
          sorted_lo = قۇرلار;
        }
      وإلا
        {
          dest = قۇرلار;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساكن بنية merge_تۈگۈن *init_تۈگۈن (بنية merge_تۈگۈن *restrict,
                                     بنية merge_تۈگۈن *restrict,
                                     بنية قۇر *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساكن بنية merge_تۈگۈن *
merge_دەرەخ_init (size_t nthreads, size_t nlines, بنية قۇر *dest)
{
  بنية merge_تۈگۈن *merge_دەرەخ = xmalloc (2 * حجم *merge_دەرەخ * nthreads);

  بنية merge_تۈگۈن *يىلتىز = merge_دەرەخ;
  يىلتىز->lo = يىلتىز->hi = يىلتىز->end_lo = يىلتىز->end_hi = عدم;
  يىلتىز->dest = عدم;
  يىلتىز->nlo = يىلتىز->nhi = nlines;
  يىلتىز->parent = عدم;
  يىلتىز->level = MERGE_END;
  يىلتىز->queued = false;
  pthread_mutex_init (&يىلتىز->lock, عدم);

  init_تۈگۈن (يىلتىز, يىلتىز + 1, dest, nthreads, nlines, false);
  أرجع merge_دەرەخ;
}

/* Destroy the merge tree. */
ساكن فراغ
merge_دەرەخ_destroy (size_t nthreads, بنية merge_تۈگۈن *merge_دەرەخ)
{
  size_t n_nodes = nthreads * 2;
  بنية merge_تۈگۈن *تۈگۈن = merge_دەرەخ;

  طالما (n_nodes--)
    {
      pthread_mutex_destroy (&تۈگۈن->lock);
      تۈگۈن++;
    }

  حرر (merge_دەرەخ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساكن بنية merge_تۈگۈن *
init_تۈگۈن (بنية merge_تۈگۈن *restrict parent,
           بنية merge_تۈگۈن *restrict تۈگۈن_pool,
           بنية قۇر *dest, size_t nthreads,
           size_t total_قۇرلار, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية قۇر *lo = dest - total_قۇرلار;
  بنية قۇر *hi = lo - nlo;
  بنية قۇر **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  بنية merge_تۈگۈن *تۈگۈن = تۈگۈن_pool++;
  تۈگۈن->lo = تۈگۈن->end_lo = lo;
  تۈگۈن->hi = تۈگۈن->end_hi = hi;
  تۈگۈن->dest = parent_end;
  تۈگۈن->nlo = nlo;
  تۈگۈن->nhi = nhi;
  تۈگۈن->parent = parent;
  تۈگۈن->level = parent->level + 1;
  تۈگۈن->queued = false;
  pthread_mutex_init (&تۈگۈن->lock, عدم);

  إذا (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      تۈگۈن->lo_child = تۈگۈن_pool;
      تۈگۈن_pool = init_تۈگۈن (تۈگۈن, تۈگۈن_pool, lo, lo_threads,
                             total_قۇرلار, true);
      تۈگۈن->hi_child = تۈگۈن_pool;
      تۈگۈن_pool = init_تۈگۈن (تۈگۈن, تۈگۈن_pool, hi, hi_threads,
                             total_قۇرلار, false);
    }
  وإلا
    {
      تۈگۈن->lo_child = عدم;
      تۈگۈن->hi_child = عدم;
    }
  أرجع تۈگۈن_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساكن صحيح
compare_nodes (فراغ ثابت *a, فراغ ثابت *b)
{
  بنية merge_تۈگۈن ثابت *nodea = a;
  بنية merge_تۈگۈن ثابت *nodeb = b;
  إذا (nodea->level == nodeb->level)
      أرجع (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  أرجع nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساكن مضمن فراغ
lock_تۈگۈن (بنية merge_تۈگۈن *تۈگۈن)
{
  pthread_mutex_lock (&تۈگۈن->lock);
}

/* Unlock a merge tree NODE. */

ساكن مضمن فراغ
unlock_تۈگۈن (بنية merge_تۈگۈن *تۈگۈن)
{
  pthread_mutex_unlock (&تۈگۈن->lock);
}

/* Destroy merge QUEUE. */

ساكن فراغ
queue_destroy (بنية merge_تۈگۈن_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساكن فراغ
queue_init (بنية merge_تۈگۈن_queue *queue, size_t nthreads)
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
queue_قىستۇرۇش (بنية merge_تۈگۈن_queue *queue, بنية merge_تۈگۈن *تۈگۈن)
{
  pthread_mutex_lock (&queue->mutex);
  heap_قىستۇرۇش (queue->priority_queue, تۈگۈن);
  تۈگۈن->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساكن بنية merge_تۈگۈن *
queue_چىقىرىش (بنية merge_تۈگۈن_queue *queue)
{
  بنية merge_تۈگۈن *تۈگۈن;
  pthread_mutex_lock (&queue->mutex);
  طالما (! (تۈگۈن = heap_remove_چوققا (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_تۈگۈن (تۈگۈن);
  تۈگۈن->queued = false;
  أرجع تۈگۈن;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساكن فراغ
write_unique (بنية قۇر ثابت *قۇر, FILE *tfp, حرف ثابت *temp_output)
{
  إذا (unique)
    {
      إذا (saved_قۇر.text && ! compare (قۇر, &saved_قۇر))
        أرجع;
      saved_قۇر = *قۇر;
    }

  write_قۇر (قۇر, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساكن فراغ
mergelines_تۈگۈن (بنية merge_تۈگۈن *restrict تۈگۈن, size_t total_قۇرلار,
                 FILE *tfp, حرف ثابت *temp_output)
{
  بنية قۇر *lo_orig = تۈگۈن->lo;
  بنية قۇر *hi_orig = تۈگۈن->hi;
  size_t to_merge = MAX_MERGE (total_قۇرلار, تۈگۈن->level);
  size_t merged_lo;
  size_t merged_hi;

  إذا (تۈگۈن->level > MERGE_يىلتىز)
    {
      /* Merge to destination buffer. */
      بنية قۇر *dest = *تۈگۈن->dest;
      طالما (تۈگۈن->lo != تۈگۈن->end_lo && تۈگۈن->hi != تۈگۈن->end_hi && to_merge--)
        إذا (compare (تۈگۈن->lo - 1, تۈگۈن->hi - 1) <= 0)
          *--dest = *--تۈگۈن->lo;
        وإلا
          *--dest = *--تۈگۈن->hi;

      merged_lo = lo_orig - تۈگۈن->lo;
      merged_hi = hi_orig - تۈگۈن->hi;

      إذا (تۈگۈن->nhi == merged_hi)
        طالما (تۈگۈن->lo != تۈگۈن->end_lo && to_merge--)
          *--dest = *--تۈگۈن->lo;
      وإلا إذا (تۈگۈن->nlo == merged_lo)
        طالما (تۈگۈن->hi != تۈگۈن->end_hi && to_merge--)
          *--dest = *--تۈگۈن->hi;
      *تۈگۈن->dest = dest;
    }
  وإلا
    {
      /* Merge directly to output. */
      طالما (تۈگۈن->lo != تۈگۈن->end_lo && تۈگۈن->hi != تۈگۈن->end_hi && to_merge--)
        {
          إذا (compare (تۈگۈن->lo - 1, تۈگۈن->hi - 1) <= 0)
            write_unique (--تۈگۈن->lo, tfp, temp_output);
          وإلا
            write_unique (--تۈگۈن->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - تۈگۈن->lo;
      merged_hi = hi_orig - تۈگۈن->hi;

      إذا (تۈگۈن->nhi == merged_hi)
        {
          طالما (تۈگۈن->lo != تۈگۈن->end_lo && to_merge--)
            write_unique (--تۈگۈن->lo, tfp, temp_output);
        }
      وإلا إذا (تۈگۈن->nlo == merged_lo)
        {
          طالما (تۈگۈن->hi != تۈگۈن->end_hi && to_merge--)
            write_unique (--تۈگۈن->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - تۈگۈن->lo;
  merged_hi = hi_orig - تۈگۈن->hi;
  تۈگۈن->nlo -= merged_lo;
  تۈگۈن->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساكن فراغ
queue_check_قىستۇرۇش (بنية merge_تۈگۈن_queue *queue, بنية merge_تۈگۈن *تۈگۈن)
{
  إذا (! تۈگۈن->queued)
    {
      bool lo_avail = (تۈگۈن->lo - تۈگۈن->end_lo) != 0;
      bool hi_avail = (تۈگۈن->hi - تۈگۈن->end_hi) != 0;
      إذا (lo_avail ? hi_avail || ! تۈگۈن->nhi : hi_avail && ! تۈگۈن->nlo)
        queue_قىستۇرۇش (queue, تۈگۈن);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساكن فراغ
queue_check_قىستۇرۇش_parent (بنية merge_تۈگۈن_queue *queue,
                           بنية merge_تۈگۈن *تۈگۈن)
{
  إذا (تۈگۈن->level > MERGE_يىلتىز)
    {
      lock_تۈگۈن (تۈگۈن->parent);
      queue_check_قىستۇرۇش (queue, تۈگۈن->parent);
      unlock_تۈگۈن (تۈگۈن->parent);
    }
  وإلا إذا (تۈگۈن->nlo + تۈگۈن->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_قىستۇرۇش (queue, تۈگۈن->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساكن فراغ
merge_loop (بنية merge_تۈگۈن_queue *queue,
            size_t total_قۇرلار, FILE *tfp, حرف ثابت *temp_output)
{
  طالما (true)
    {
      بنية merge_تۈگۈن *تۈگۈن = queue_چىقىرىش (queue);

      إذا (تۈگۈن->level == MERGE_END)
        {
          unlock_تۈگۈن (تۈگۈن);
          /* Reinsert so other threads can pop it. */
          queue_قىستۇرۇش (queue, تۈگۈن);
          اكسر;
        }
      mergelines_تۈگۈن (تۈگۈن, total_قۇرلار, tfp, temp_output);
      queue_check_قىستۇرۇش (queue, تۈگۈن);
      queue_check_قىستۇرۇش_parent (queue, تۈگۈن);

      unlock_تۈگۈن (تۈگۈن);
    }
}


ساكن فراغ sortlines (بنية قۇر *restrict, size_t, size_t,
                       بنية merge_تۈگۈن *, بنية merge_تۈگۈن_queue *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

بنية thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  بنية قۇر *قۇرلار;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت total_قۇرلار;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  بنية merge_تۈگۈن *ثابت تۈگۈن;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  بنية merge_تۈگۈن_queue *ثابت queue;

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
  sortlines (args->قۇرلار, args->nthreads, args->total_قۇرلار,
             args->تۈگۈن, args->queue, args->tfp,
             args->output_temp);
  أرجع عدم;
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
sortlines (بنية قۇر *restrict قۇرلار, size_t nthreads,
           size_t total_قۇرلار, بنية merge_تۈگۈن *تۈگۈن,
           بنية merge_تۈگۈن_queue *queue, FILE *tfp, حرف ثابت *temp_output)
{
  size_t nlines = تۈگۈن->nlo + تۈگۈن->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  بنية thread_args args = {قۇرلار, lo_threads, total_قۇرلار,
                             تۈگۈن->lo_child, queue, tfp, temp_output};

  إذا (nthreads > 1 && SUBTHREAD_قۇرلار_HEURISTIC <= nlines
      && pthread_create (&thread, عدم, sortlines_thread, &args) == 0)
    {
      sortlines (قۇرلار - تۈگۈن->nlo, hi_threads, total_قۇرلار,
                 تۈگۈن->hi_child, queue, tfp, temp_output);
      pthread_join (thread, عدم);
    }
  وإلا
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = تۈگۈن->nlo;
      size_t nhi = تۈگۈن->nhi;
      بنية قۇر *temp = قۇرلار - total_قۇرلار;
      إذا (1 < nhi)
        sequential_sort (قۇرلار - nlo, nhi, temp - nlo / 2, false);
      إذا (1 < nlo)
        sequential_sort (قۇرلار, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      تۈگۈن->lo = قۇرلار;
      تۈگۈن->hi = قۇرلار - nlo;
      تۈگۈن->end_lo = قۇرلار - nlo;
      تۈگۈن->end_hi = قۇرلار - nlo - nhi;

      queue_قىستۇرۇش (queue, تۈگۈن);
      merge_loop (queue, total_قۇرلار, tfp, temp_output);
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
avoid_trashing_كىرگۈزۈش (بنية sortfile *files, size_t ntemps,
                      size_t nfiles, حرف ثابت *outfile)
{
  بنية tempnode *tempcopy = عدم;

  لكل (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      بنية stat instat;

      إذا (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      وإلا
        {
          بنية stat *outst = get_outstatus ();
          إذا (!outst)
            اكسر;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      إذا (same)
        {
          إذا (! tempcopy)
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
  لكل (size_t i = 0; i < nfiles; i++)
    {
      إذا (streq (files[i], "-"))
        استمر;

      إذا (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ساكن فراغ
check_output (حرف ثابت *outfile)
{
  إذا (outfile)
    {
      صحيح oflags = O_WRONLY | O_ئىككىلىك | O_CLOEXEC | O_CREAT;
      صحيح outfd = open (outfile, oflags, MODE_RW_UGO);
      إذا (outfd < 0)
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
  طالما (nmerge < nfiles)
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
      لكل (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          بنية tempnode *temp = create_temp (&tfp);
          size_t رەقەم_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, رەقەم_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += رەقەم_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      إذا (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          بنية tempnode *temp = create_temp (&tfp);
          size_t رەقەم_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, رەقەم_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += رەقەم_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * حجم *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_كىرگۈزۈش (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  طالما (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_كىرگۈزۈش_files (files, nfiles, &fps);

      إذا (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          إذا (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              اكسر;
            }
          إذا (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      وإلا إذا (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      بنية tempnode *temp;
      افعل
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      طالما (!temp);

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

  طالما (nfiles)
    {
      حرف ثابت *temp_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_قۇر;
      إذا (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          طالما (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_قۇر = (mult * حجم (بنية قۇر));
        }
      وإلا
        bytes_per_قۇر = حجم (بنية قۇر) * 3 / 2;

      إذا (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_قۇر,
                              &policy);
          initbuf (&buf, bytes_per_قۇر, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      طالما (fillbuf (&buf, fp, file, &policy))
        {
          بنية قۇر *قۇر;

          إذا (buf.eof && nfiles
              && (bytes_per_قۇر + 1
                  < (buf.alloc - buf.used - bytes_per_قۇر * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.سول = buf.used;
              اكسر;
            }

          saved_قۇر.text = عدم;
          قۇر = buffer_linelim (&buf);
          إذا (buf.eof && !nfiles && !ntemps && !buf.سول)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          وإلا
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          إذا (1 < buf.nlines)
            {
              إذا (nthreads > 1)
                {
                  بنية merge_تۈگۈن_queue queue;
                  queue_init (&queue, nthreads);
                  بنية merge_تۈگۈن *merge_دەرەخ =
                    merge_دەرەخ_init (nthreads, buf.nlines, قۇر);

                  sortlines (قۇر, nthreads, buf.nlines, merge_دەرەخ + 1,
                             &queue, tfp, temp_output);

                  merge_دەرەخ_destroy (nthreads, merge_دەرەخ);
                  queue_destroy (&queue);
                }
              وإلا
                {
                  sequential_sort (قۇر, buf.nlines,
                                   قۇر - buf.nlines, false);
                  لكل (size_t i = 0; i < buf.nlines; i++)
                    write_unique (قۇر - i - 1, tfp, temp_output);
                }
            }
          وإلا
            write_unique (قۇر - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          إذا (output_file_created)
            اذهب finish;
        }
      xfclose (fp, file);
    }

 finish:
  حرر (buf.buf);

  إذا (! output_file_created)
    {
      بنية tempnode *تۈگۈن = temphead;
      بنية sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      لكل (size_t i = 0; تۈگۈن; i++)
        {
          tempfiles[i].name = تۈگۈن->name;
          tempfiles[i].temp = تۈگۈن;
          تۈگۈن = تۈگۈن->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      حرر (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساكن فراغ
insertkey (بنية keyfield *ئاچقۇچ_arg)
{
  بنية keyfield **p;
  بنية keyfield *ئاچقۇچ = xmemdup (ئاچقۇچ_arg, حجم *ئاچقۇچ);

  لكل (p = &keylist; *p; p = &(*p)->next)
    استمر;
  *p = ئاچقۇچ;
  ئاچقۇچ->next = عدم;
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
  لكل (بنية keyfield *ئاچقۇچ = keylist; ئاچقۇچ; ئاچقۇچ = ئاچقۇچ->next)
    إذا (1 < (ئاچقۇچ->numeric + ئاچقۇچ->general_numeric + ئاچقۇچ->human_numeric
             + ئاچقۇچ->month + (ئاچقۇچ->version | ئاچقۇچ->random | !!ئاچقۇچ->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        ئاچقۇچ->skipsblanks = ئاچقۇچ->skipeblanks = ئاچقۇچ->reverse = false;
        ئاچقۇچ_to_opts (ئاچقۇچ, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساكن حرف ثابت *
parse_field_سانى (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
{
  حرف *suffix;
  uintmax_t n;

  بدل (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    حالة LONGINT_OK:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      إذا (*val == n)
        اكسر;
      FALLTHROUGH;
    حالة LONGINT_OVERFLOW:
    حالة LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      اكسر;

    حالة LONGINT_INVALID:
      إذا (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      أرجع عدم;
    }

  أرجع suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ساكن حرف *
set_ordering (حرف ثابت *s, بنية keyfield *ئاچقۇچ, تعداد blanktype blanktype)
{
  طالما (*s)
    {
      بدل (*s)
        {
        حالة 'b':
          إذا (blanktype == bl_start || blanktype == bl_both)
            ئاچقۇچ->skipsblanks = true;
          إذا (blanktype == bl_end || blanktype == bl_both)
            ئاچقۇچ->skipeblanks = true;
          اكسر;
        حالة 'd':
          ئاچقۇچ->ignore = nondictionary;
          اكسر;
        حالة 'f':
          ئاچقۇچ->translate = fold_toupper;
          اكسر;
        حالة 'g':
          ئاچقۇچ->general_numeric = true;
          اكسر;
        حالة 'h':
          ئاچقۇچ->human_numeric = true;
          اكسر;
        حالة 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          إذا (! ئاچقۇچ->ignore)
            ئاچقۇچ->ignore = nonprinting;
          اكسر;
        حالة 'M':
          ئاچقۇچ->month = true;
          اكسر;
        حالة 'n':
          ئاچقۇچ->numeric = true;
          اكسر;
        حالة 'R':
          ئاچقۇچ->random = true;
          اكسر;
        حالة 'r':
          ئاچقۇچ->reverse = true;
          اكسر;
        حالة 'V':
          ئاچقۇچ->version = true;
          اكسر;
        افتراضي:
          أرجع (حرف *) s;
        }
      ++s;
    }
  أرجع (حرف *) s;
}

/* Initialize KEY.  */

ساكن بنية keyfield *
ئاچقۇچ_init (بنية keyfield *ئاچقۇچ)
{
  memset (ئاچقۇچ, 0, حجم *ئاچقۇچ);
  ئاچقۇچ->eword = SIZE_MAX;
  أرجع ئاچقۇچ;
}

صحيح
main (صحيح argc, حرف **argv)
{
  بنية keyfield *ئاچقۇچ;
  بنية keyfield ئاچقۇچ_buf;
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
#إذا HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    بنية lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    إذا (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    إذا (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    إذا (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    تعداد { nsigs = countof (term_sig) };

    بنية sigaction act;

    sigemptyset (&caught_signals);
    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (term_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        sigaction (term_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (term_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        إذا (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, عدم);
      }
  }
  إشارة (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ئاچقۇچ_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, حجم *files);

  طالما (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      صحيح oi = -1;

      إذا (c == -1
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
          إذا (argc <= optind)
            اكسر;
          files[nfiles++] = argv[optind++];
        }
      وإلا بدل (c)
        {
        حالة 1:
          ئاچقۇچ = عدم;
          إذا (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              إذا (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ئاچقۇچ = ئاچقۇچ_init (&ئاچقۇچ_buf);
                  s = parse_field_سانى (optarg + 1, &ئاچقۇچ->sword, عدم);
                  إذا (s && *s == '.')
                    s = parse_field_سانى (s + 1, &ئاچقۇچ->schar, عدم);
                  إذا (! (ئاچقۇچ->sword || ئاچقۇچ->schar))
                    ئاچقۇچ->sword = SIZE_MAX;
                  إذا (! s || *set_ordering (s, ئاچقۇچ, bl_start))
                    ئاچقۇچ = عدم;
                  وإلا
                    {
                      إذا (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_field_سانى (optarg1 + 1, &ئاچقۇچ->eword,
                                             N_("invalid number after '-'"));
                          إذا (*s == '.')
                            s = parse_field_سانى (s + 1, &ئاچقۇچ->echar,
                                               N_("invalid number after '.'"));
                          إذا (!ئاچقۇچ->echar && ئاچقۇچ->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ئاچقۇچ->eword--;
                            }
                          إذا (*set_ordering (s, ئاچقۇچ, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ئاچقۇچ->traditional_used = true;
                      insertkey (ئاچقۇچ);
                    }
                }
            }
          إذا (! ئاچقۇچ)
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
          إذا (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          اكسر;

        حالة COMPRESS_PROGRAM_OPTION:
          إذا (compress_program && !streq (compress_program, optarg))
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
          ئاچقۇچ = ئاچقۇچ_init (&ئاچقۇچ_buf);

          /* Get POS1. */
          s = parse_field_سانى (optarg, &ئاچقۇچ->sword,
                                 N_("invalid number at field start"));
          إذا (! ئاچقۇچ->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          إذا (*s == '.')
            {
              s = parse_field_سانى (s + 1, &ئاچقۇچ->schar,
                                     N_("invalid number after '.'"));
              إذا (! ئاچقۇچ->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          إذا (! (ئاچقۇچ->sword || ئاچقۇچ->schar))
            ئاچقۇچ->sword = SIZE_MAX;
          s = set_ordering (s, ئاچقۇچ, bl_start);
          إذا (*s != ',')
            {
              ئاچقۇچ->eword = SIZE_MAX;
              ئاچقۇچ->echar = 0;
            }
          وإلا
            {
              /* Get POS2. */
              s = parse_field_سانى (s + 1, &ئاچقۇچ->eword,
                                     N_("invalid number after ','"));
              إذا (! ئاچقۇچ->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              إذا (*s == '.')
                {
                  s = parse_field_سانى (s + 1, &ئاچقۇچ->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, ئاچقۇچ, bl_end);
            }
          إذا (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ئاچقۇچ);
          اكسر;

        حالة 'm':
          mergeonly = true;
          اكسر;

        حالة NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          اكسر;

        حالة 'o':
          إذا (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          اكسر;

        حالة RANDOM_SOURCE_OPTION:
          إذا (random_source && !streq (random_source, optarg))
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
            إذا (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            إذا (optarg[1])
              {
                إذا (streq (optarg, "\\0"))
                  newtab = '\0';
                وإلا
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            إذا (tab != TAB_DEFAULT && tab != newtab)
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
          إذا (optarg == argv[optind - 1])
            {
              حرف ثابت *p;
              لكل (p = optarg; c_isdigit (*p); p++)
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

  إذا (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      إذا (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      إذا (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      إذا (tok.n_tok)
        {
          حرر (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          لكل (size_t i = 0; i < nfiles; i++)
            {
              إذا (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              وإلا إذا (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long صحيح file_سان = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_سان);
                }
            }
        }
      وإلا
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  لكل (ئاچقۇچ = keylist; ئاچقۇچ; ئاچقۇچ = ئاچقۇچ->next)
    {
      إذا (default_ئاچقۇچ_compare (ئاچقۇچ) && !ئاچقۇچ->reverse)
        {
          ئاچقۇچ->ignore = gkey.ignore;
          ئاچقۇچ->translate = gkey.translate;
          ئاچقۇچ->skipsblanks = gkey.skipsblanks;
          ئاچقۇچ->skipeblanks = gkey.skipeblanks;
          ئاچقۇچ->month = gkey.month;
          ئاچقۇچ->numeric = gkey.numeric;
          ئاچقۇچ->general_numeric = gkey.general_numeric;
          ئاچقۇچ->human_numeric = gkey.human_numeric;
          ئاچقۇچ->version = gkey.version;
          ئاچقۇچ->random = gkey.random;
          ئاچقۇچ->reverse = gkey.reverse;
        }

      need_random |= ئاچقۇچ->random;
    }

  إذا (!keylist && !default_ئاچقۇچ_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  إذا (debug)
    {
      إذا (checkonly || outfile)
        {
          ساكن حرف opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      إذا (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      إذا (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      إذا (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, عدم)));
      وإلا
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ئاچقۇچ_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  إذا (need_random)
    random_md5_state_init (random_source);

  إذا (temp_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  إذا (nfiles == 0)
    {
      nfiles = 1;
      حرر (files);
      files = xmalloc (حجم *files);
      *files = (حرف *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  إذا (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  إذا (checkonly)
    {
      إذا (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      إذا (outfile)
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

  إذا (mergeonly)
    {
      بنية sortfile *sortfiles = xcalloc (nfiles, حجم *sortfiles);

      لكل (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  وإلا
    {
      إذا (!nthreads)
        {
          unsigned long صحيح np = رەقەم_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (بنية merge_تۈگۈن));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
