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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <pthread.h>
#અન્તર્ભાવ <sys/resource.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <spawn.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "argmatch.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "filevercmp.h"
#અન્તર્ભાવ "findprog.h"
#અન્તર્ભાવ "flexmember.h"
#અન્તર્ભાવ "hard-locale.h"
#અન્તર્ભાવ "hash.h"
#અન્તર્ભાવ "heap.h"
#અન્તર્ભાવ "ignore-value.h"
#અન્તર્ભાવ "mbswidth.h"
#અન્તર્ભાવ "nproc.h"
#અન્તર્ભાવ "physmem.h"
#અન્તર્ભાવ "posixver.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "randread.h"
#અન્તર્ભાવ "readtokens0.h"
#અન્તર્ભાવ "stdlib--.h"
#અન્તર્ભાવ "strnumcmp.h"
#અન્તર્ભાવ "term-sig.h"
#અન્તર્ભાવ "unistd--.h"
#અન્તર્ભાવ "xmemcoll.h"
#અન્તર્ભાવ "xnanosleep.h"
#અન્તર્ભાવ "xstrtol.h"
#અન્તર્ભાવ "xstrtol-error.h"

#ifndef RLIMIT_DATA
સંરચના rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#જો HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#જો !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#જો !defined OPEN_MAX
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
ગણના { SUBTHREAD_લીટીઓ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_લીટીઓ_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ગણના { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
ગણના
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

ગણના
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

ગણના
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_મૂળ = 1
  };

/* The representation of the decimal point in the current locale.  */
સ્થિર અક્ષર decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
સ્થિર પૂર્ણાંક thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
સ્થિર bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
સ્થિર bool hard_LC_COLLATE;
#જો HAVE_NL_LANGINFO
સ્થિર bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ગણના blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
સ્થિર અક્ષર eolchar = '\n';

/* Lines are held in memory as counted strings. */
સંરચના લીટી
{
  અક્ષર *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  અક્ષર *keybeg;			/* Start of first key. */
  અક્ષર *keylim;			/* Limit of first key. */
};

/* Input buffers. */
સંરચના buffer
{
  અક્ષર *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ડાબું;			/* Number of bytes left from previous reads. */
  size_t લીટી_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
સંરચના sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
સંરચના keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool અચલ *ignore;		/* Boolean array of characters to ignore. */
  અક્ષર અચલ *translate;	/* Translation applied to characters. */
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
  સંરચના keyfield *next;	/* Next keyfield to try. */
};

સંરચના month
{
  અક્ષર અચલ *name;
  પૂર્ણાંક val;
};

/* Binary merge tree node. */
સંરચના merge_ગાંઠ
{
  સંરચના લીટી *lo;              /* Lines to merge from LO child node. */
  સંરચના લીટી *hi;              /* Lines to merge from HI child node. */
  સંરચના લીટી *end_lo;          /* End of available lines from LO. */
  સંરચના લીટી *end_hi;          /* End of available lines from HI. */
  સંરચના લીટી **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  સંરચના merge_ગાંઠ *parent;    /* Parent node. */
  સંરચના merge_ગાંઠ *lo_child;  /* LO child node. */
  સંરચના merge_ગાંઠ *hi_child;  /* HI child node. */
  unsigned પૂર્ણાંક level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
સંરચના merge_ગાંઠ_queue
{
  સંરચના heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
સ્થિર સંરચના લીટી saved_લીટી;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
સ્થિર bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
સ્થિર bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
સ્થિર bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
સ્થિર અક્ષર fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
સ્થિર સંરચના month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + કદ (સંરચના લીટી))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
સ્થિર size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
સ્થિર size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define આગત_FILE_SIZE_GUESS (128 * 1024)

સ્થિર સંરચના
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  અક્ષર અચલ **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
સ્થિર bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
સ્થિર bool stable;

/* An int value outside char range.  */
ગણના { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ગણના { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
સ્થિર પૂર્ણાંક tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
સ્થિર bool unique;

/* Nonzero if any of the input files are the standard input. */
સ્થિર bool have_read_stdin;

/* List of key field comparisons to be tried.  */
સ્થિર સંરચના keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
સ્થિર અક્ષર અચલ *compress_program;

/* Annotate the output with extra info to aid the user.  */
સ્થિર bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
સ્થિર unsigned પૂર્ણાંક nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
સ્થિર bool default_SIGPIPE;

/* The list of temporary files. */
સંરચના tempnode
{
  સંરચના tempnode *અસ્થિર next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  અક્ષર state;
  અક્ષર name[FLEXIBLE_ARRAY_MEMBER];
};
સ્થિર સંરચના tempnode *અસ્થિર temphead;
સ્થિર સંરચના tempnode *અસ્થિર *temptail = &temphead;

/* Clean up any remaining temporary files.  */

સ્થિર શૂન્ય
cleanup (શૂન્ય)
{
  માટે (સંરચના tempnode અચલ *ગાંઠ = temphead; ગાંઠ; ગાંઠ = ગાંઠ->next)
    unlink (ગાંઠ->name);
  temphead = ખાલી;
}

/* Handle interrupts and hangups. */

સ્થિર શૂન્ય
sighandler (પૂર્ણાંક sig)
{
  cleanup ();

  સંકેત (sig, SIG_DFL);
  ઉઠાવો (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

સ્થિર શૂન્ય
sort_die (અક્ષર અચલ *message, અક્ષર અચલ *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  જો (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         ignore leading blanks when finding sort ચાવીઓ in each લીટી\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-સ્થિતિ\n\
         fold lower સ્થિતિ to upper સ્થિતિ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical કિંમત\n\
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
         compare according to string numerical કિંમત;\n\
         see full documentation માટે supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical ચાવીઓ.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; માટે more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check માટે sorted આગત; કરો not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but કરો not report first bad લીટી\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the લીટી used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         પઠ આગત from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, પઠ names from standard આગત\n\
"));
      oputs (_("\
  -k, --ચાવી=KEYDEF\n\
         sort via a ચાવી; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; કરો not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         લિખ result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE માટે મુખ્ય memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR માટે temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the સંખ્યા of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of લીટીઓ with equal ચાવીઓ;\n\
         with -c, check માટે strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] માટે start and stop position, where F is a\n\
field સંખ્યા and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the લીટી's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options માટે that ચાવી.  If no ચાવી is given, use\n\
the entire લીટી as the ચાવી.  Use --debug to diagnose incorrect ચાવી usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (મૂળભૂત), and so on માટે M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte કિંમતો.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
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

સ્થિર અક્ષર અચલ short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

સ્થિર સંરચના option અચલ long_options[] =
{
  {"ignore-leading-blanks", no_argument, ખાલી, 'b'},
  {"check", optional_argument, ખાલી, CHECK_OPTION},
  {"compress-program", required_argument, ખાલી, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, ખાલી, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, ખાલી, 'd'},
  {"ignore-case", no_argument, ખાલી, 'f'},
  {"files0-from", required_argument, ખાલી, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, ખાલી, 'g'},
  {"ignore-nonprinting", no_argument, ખાલી, 'i'},
  {"key", required_argument, ખાલી, 'k'},
  {"merge", no_argument, ખાલી, 'm'},
  {"month-sort", no_argument, ખાલી, 'M'},
  {"numeric-sort", no_argument, ખાલી, 'n'},
  {"human-numeric-sort", no_argument, ખાલી, 'h'},
  {"version-sort", no_argument, ખાલી, 'V'},
  {"random-sort", no_argument, ખાલી, 'R'},
  {"random-source", required_argument, ખાલી, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, ખાલી, SORT_OPTION},
  {"output", required_argument, ખાલી, 'o'},
  {"reverse", no_argument, ખાલી, 'r'},
  {"stable", no_argument, ખાલી, 's'},
  {"batch-size", required_argument, ખાલી, NMERGE_OPTION},
  {"buffer-size", required_argument, ખાલી, 'S'},
  {"field-separator", required_argument, ખાલી, 't'},
  {"temporary-directory", required_argument, ખાલી, 'T'},
  {"unique", no_argument, ખાલી, 'u'},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {"parallel", required_argument, ખાલી, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

સ્થિર અક્ષર અચલ *અચલ check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE ખાલી
#undef  _ct_
};
સ્થિર અક્ષર અચલ check_types[] =
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

સ્થિર અક્ષર અચલ *અચલ sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE ખાલી
#undef  _st_
};
સ્થિર અક્ષર અચલ sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
સ્થિર sigset_t caught_signals;

/* Critical section status.  */
સંરચના cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
સ્થિર શૂન્ય
cs_enter (સંરચના cs_status *status)
{
  પૂર્ણાંક ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
સ્થિર શૂન્ય
cs_leave (સંરચના cs_status અચલ *status)
{
  જો (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, ખાલી);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ગણના { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
સંરચના sortfile
{
  /* The file's name.  */
  અક્ષર અચલ *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  સંરચના tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
સ્થિર Hash_table *proctab;

ગણના { INIT_PROCTAB_SIZE = 47 };

સ્થિર size_t
proctab_hasher (શૂન્ય અચલ *entry, size_t tabsize)
{
  સંરચના tempnode અચલ *ગાંઠ = entry;
  પાછા ગાંઠ->pid % tabsize;
}

સ્થિર bool
proctab_comparator (શૂન્ય અચલ *e1, શૂન્ય અચલ *e2)
{
  સંરચના tempnode અચલ *n1 = e1;
  સંરચના tempnode અચલ *n2 = e2;
  પાછા n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
સ્થિર pid_t nprocs;

સ્થિર bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

સ્થિર pid_t
reap (pid_t pid)
{
  પૂર્ણાંક status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  જો (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  નહીંતર જો (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      જો (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  પાછા cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

સ્થિર શૂન્ય
register_proc (સંરચના tempnode *temp)
{
  જો (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, ખાલી,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ખાલી);
      જો (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  જો (! hash_ઉમેરો (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

સ્થિર bool
delete_proc (pid_t pid)
{
  સંરચના tempnode test;

  test.pid = pid;
  સંરચના tempnode *ગાંઠ = hash_remove (proctab, &test);
  જો (! ગાંઠ)
    પાછા false;
  ગાંઠ->state = REAPED;
  પાછા true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

સ્થિર શૂન્ય
wait_proc (pid_t pid)
{
  જો (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

સ્થિર શૂન્ય
reap_exited (શૂન્ય)
{
  જ્યારે (0 < nprocs && reap (0))
    ચાલુ;
}

/* Reap at least one exited child, waiting if necessary.  */

સ્થિર શૂન્ય
reap_some (શૂન્ય)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

સ્થિર શૂન્ય
reap_all (શૂન્ય)
{
  જ્યારે (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

સ્થિર શૂન્ય
exit_cleanup (શૂન્ય)
{
  જો (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      સંરચના cs_status cs;
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

સ્થિર સંરચના tempnode *
create_temp_file (પૂર્ણાંક *pfd, bool survive_fd_exhaustion)
{
  સ્થિર અક્ષર અચલ slashbase[] = "/sortXXXXXX";
  સ્થિર idx_t temp_dir_index;
  પૂર્ણાંક fd;
  પૂર્ણાંક saved_errno;
  અક્ષર અચલ *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  સંરચના tempnode *ગાંઠ =
    xmalloc (FLEXSIZEOF (સંરચના tempnode, name, len + કદ slashbase));
  અક્ષર *file = ગાંઠ->name;
  સંરચના cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, કદ slashbase);
  ગાંઠ->next = ખાલી;
  જો (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  જો (0 <= fd)
    {
      *temptail = ગાંઠ;
      temptail = &ગાંઠ->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  જો (fd < 0)
    {
      જો (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      મુક્ત (ગાંઠ);
      ગાંઠ = ખાલી;
    }

  *pfd = fd;
  પાછા ગાંઠ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

સ્થિર સંરચના stat *
get_outstatus (શૂન્ય)
{
  સ્થિર પૂર્ણાંક outstat_errno;
  સ્થિર સંરચના stat outstat;
  જો (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  પાછા outstat_errno < 0 ? &outstat : ખાલી;
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

સ્થિર FILE *
stream_open (અક્ષર અચલ *file, અક્ષર અચલ *how)
{
  FILE *fp;

  જો (*how == 'r')
    {
      જો (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      નહીંતર
        {
          પૂર્ણાંક fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ખાલી : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  નહીંતર જો (*how == 'w')
    {
      જો (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          પૂર્ણાંક ftruncate_errno = errno;
          સંરચના stat *outst = get_outstatus ();
          જો (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  નહીંતર
    affirm (!"unexpected mode passed to stream_open");

  પાછા fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

સ્થિર FILE *
xfopen (અક્ષર અચલ *file, અક્ષર અચલ *how)
{
  FILE *fp = stream_open (file, how);
  જો (!fp)
    sort_die (_("open failed"), file);
  પાછા fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

સ્થિર શૂન્ય
xfclose (FILE *fp, અક્ષર અચલ *file)
{
  પસંદગી (fileno (fp))
    {
    સ્થિતિ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      તોડો;

    સ્થિતિ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      જો (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      તોડો;

    મૂળભૂત:
      જો (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      તોડો;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

સ્થિર શૂન્ય
move_fd (પૂર્ણાંક oldfd, પૂર્ણાંક newfd)
{
  જો (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_કિંમત (dup2 (oldfd, newfd));
      ignore_કિંમત (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

સ્થિર પૂર્ણાંક
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  પૂર્ણાંક oldfd, પૂર્ણાંક newfd)
{
  પૂર્ણાંક result = 0;
  જો (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      જો (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  પાછા result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

સ્થિર અક્ષર અચલ *
get_resolved_compress_program (શૂન્ય)
{
  /* Use a cache, to perform the search only once.  */
  સ્થિર અક્ષર અચલ *resolved_compress_program_cache /* = NULL */;

  જો (resolved_compress_program_cache == ખાલી)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), ખાલી, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  પાછા resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

સ્થિર પૂર્ણાંક
pipe_child (pid_t *pid, પૂર્ણાંક pipefds[2], પૂર્ણાંક tempfd, bool decompress,
            size_t tries)
{
  અક્ષર અચલ *resolved_compress_program;
  સંરચના tempnode *saved_temphead;
  દ્વિગુણ wait_retry = 0.25;
  સંરચના cs_status cs;
  પૂર્ણાંક result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  જો (resolved_compress_program == ખાલી)
    પાછા errno;

  જો ((result = posix_spawnattr_init (&attr)))
    પાછા result;
  જો ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      પાછા result;
    }

  જો (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      પૂર્ણાંક saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      પાછા saved_errno;
    }

  જો ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      પાછા result;
    }

  અક્ષર અચલ *અચલ argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : ખાલી,
      ખાલી
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  જો (nmerge + 1 < nprocs)
    reap_some ();

  જ્યારે (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ખાલી;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (અક્ષર * અચલ *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      જો (result != EAGAIN)
        તોડો;
      નહીંતર
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

  જો (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  નહીંતર
    ++nprocs;

  પાછા result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

સ્થિર સંરચના tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  પૂર્ણાંક tempfd;
  સંરચના tempnode *ગાંઠ = create_temp_file (&tempfd, survive_fd_exhaustion);
  જો (! ગાંઠ)
    પાછા ખાલી;

  ગાંઠ->state = UNCOMPRESSED;

  જો (compress_program)
    {
      પૂર્ણાંક pipefds[2];
      સ્થિર પૂર્ણાંક last_result = 0;

      પૂર્ણાંક result = pipe_child (&ગાંઠ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      જો (result)
        {
          જો (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      નહીંતર
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (ગાંઠ);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  જો (! *pfp)
    sort_die (_("couldn't create temporary file"), ગાંઠ->name);

  પાછા ગાંઠ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

સ્થિર સંરચના tempnode *
create_temp (FILE **pfp)
{
  પાછા maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

સ્થિર FILE *
open_temp (સંરચના tempnode *temp)
{
  પૂર્ણાંક tempfd, pipefds[2];
  FILE *fp = ખાલી;

  જો (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  જો (tempfd < 0)
    પાછા ખાલી;

  pid_t child;
  પૂર્ણાંક result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  જો (result)
    {
      જો (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  નહીંતર
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      જો (! fp)
        {
          પૂર્ણાંક saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  પાછા fp;
}

/* Append DIR to the array of temporary directory names.  */
સ્થિર શૂન્ય
add_temp_dir (અક્ષર અચલ *dir)
{
  જો (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              કદ *temp_dirs.dirs);

  idx_t અચલ dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

સ્થિર શૂન્ય
zaptemp (અક્ષર અચલ *name)
{
  સંરચના tempnode *અસ્થિર *pnode;
  સંરચના tempnode *ગાંઠ;
  સંરચના tempnode *next;
  પૂર્ણાંક unlink_status;
  પૂર્ણાંક unlink_errno = 0;
  સંરચના cs_status cs;

  માટે (pnode = &temphead; (ગાંઠ = *pnode)->name != name; pnode = &ગાંઠ->next)
    ચાલુ;

  જો (ગાંઠ->state == UNREAPED)
    wait_proc (ગાંઠ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = ગાંઠ->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  જો (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  જો (! next)
    temptail = pnode;
  મુક્ત (ગાંઠ);
}

#જો HAVE_NL_LANGINFO

સ્થિર પૂર્ણાંક
struct_month_cmp (શૂન્ય અચલ *m1, શૂન્ય અચલ *m2)
{
  સંરચના month અચલ *month1 = m1;
  સંરચના month અચલ *month2 = m2;
  પાછા strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

સ્થિર શૂન્ય
inittables (શૂન્ય)
{
  માટે (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#જો HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  જો (hard_LC_TIME)
    {
      માટે (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          અક્ષર અચલ *s;
          size_t s_len;
          size_t j, k;
          અક્ષર *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          માટે (j = k = 0; j < s_len; j++)
            જો (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, કદ *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
સ્થિર શૂન્ય
specify_nmerge (પૂર્ણાંક oi, અક્ષર c, અક્ષર અચલ *s)
{
  uintmax_t n;
  સંરચના rlimit rlimit;
  ગણના strtol_error e = xstrtoumax (s, ખાલી, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned પૂર્ણાંક max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  જો (e == LONGINT_OK)
    {
      nmerge = n;
      જો (nmerge != n)
        e = LONGINT_OVERFLOW;
      નહીંતર
        {
          જો (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          નહીંતર જો (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          નહીંતર
            પાછા;
        }
    }

  જો (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  નહીંતર
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
સ્થિર શૂન્ય
specify_sort_size (પૂર્ણાંક oi, અક્ષર c, અક્ષર અચલ *s)
{
  uintmax_t n;
  અક્ષર *suffix;
  ગણના strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  જો (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  જો (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    પસંદગી (suffix[0])
      {
      સ્થિતિ 'b':
        e = LONGINT_OK;
        તોડો;

      સ્થિતિ '%':
        {
          દ્વિગુણ mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          જો (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          નહીંતર
            e = LONGINT_OVERFLOW;
        }
        તોડો;
      }

  જો (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      જો (n < sort_size)
        પાછા;

      sort_size = n;
      જો (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          પાછા;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
સ્થિર size_t
specify_nthreads (પૂર્ણાંક oi, અક્ષર c, અક્ષર અચલ *s)
{
  uintmax_t nthreads;
  ગણના strtol_error e = xstrtoumax (s, ખાલી, 10, &nthreads, "");
  જો (e == LONGINT_OVERFLOW)
    પાછા SIZE_MAX;
  જો (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  જો (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  જો (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  પાછા nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
સ્થિર size_t
default_sort_size (શૂન્ય)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  સંરચના rlimit rlimit;
  જો (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  જો (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  જો (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  દ્વિગુણ avail = physmem_available ();
  દ્વિગુણ total = physmem_total ();
  દ્વિગુણ mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  જો (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  જો (mem < size)
    size = mem;
  પાછા MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

સ્થિર bool
લીટી_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = કદ (સંરચના લીટી);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  જો (ckd_add (&aligned, size, padding))
    પાછા false;

  *alloc = aligned;
  પાછા true;
}

/* Return the default initial allocation for a growable sort buffer.  */

સ્થિર size_t
default_initial_sort_size (size_t લીટી_bytes)
{
  size_t size = લીટી_bytes + 2;
  size_t આગત_size;

  જો (ckd_mul (&આગત_size, આગત_FILE_SIZE_GUESS, લીટી_bytes + 1)
      || ckd_add (&size, size, આગત_size))
    પાછા SIZE_MAX;

  પાછા MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

સ્થિર bool
આગત_size_buffer_bytes (uintmax_t file_size, size_t લીટી_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_આગત_byte = લીટી_bytes + 1;

  જો (SIZE_MAX / worst_case_per_આગત_byte < file_size)
    પાછા false;

  size_t size = file_size * worst_case_per_આગત_byte;
  જો (SIZE_MAX - size < 1)
    પાછા false;

  *worst_case = size + 1;
  પાછા true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

સ્થિર શૂન્ય
sort_buffer_policy (FILE *અચલ *fps, size_t nfps,
                    અક્ષર *અચલ *files, size_t nfiles,
                    size_t લીટી_bytes,
                    સંરચના sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (લીટી_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_આગત_byte = લીટી_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_આગત_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  માટે (size_t i = 0; i < nfiles; i++)
    {
      સંરચના stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      જો (size_bound <= size)
        {
          policy->initial = size_bound;
          પાછા;
        }

      જો ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      જો (known_size)
        file_size = st.st_size;
      નહીંતર
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          જો (sort_size)
            {
              policy->initial = size_bound;
              પાછા;
            }
          file_size = આગત_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      જો (! આગત_size_buffer_bytes (file_size, લીટી_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          પાછા;
        }

      જો (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          પાછા;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

સ્થિર શૂન્ય
initbuf (સંરચના buffer *buf, size_t લીટી_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  જ્યારે (true)
    {
      જો (! લીટી_aligned_size (&alloc))
        buf->buf = ખાલી;
      નહીંતર
        buf->buf = malloc (alloc);
      જો (buf->buf)
        તોડો;
      alloc /= 2;
      જો (alloc <= લીટી_bytes + 1)
        xalloc_die ();
    }

  buf->લીટી_bytes = લીટી_bytes;
  buf->alloc = alloc;
  buf->used = buf->ડાબું = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

સ્થિર અંતઃરેખ સંરચના લીટી *
buffer_linelim (સંરચના buffer અચલ *buf)
{
  શૂન્ય *linelim = buf->buf + buf->alloc;
  પાછા linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

સ્થિર bool
try_growbuf (સંરચના buffer *buf, size_t alloc)
{
  જો (! લીટી_aligned_size (&alloc) || alloc <= buf->alloc)
    પાછા false;

  અક્ષર *newbuf = malloc (alloc);
  જો (! newbuf)
    પાછા false;

  અક્ષર *oldbuf = buf->buf;
  સંરચના લીટી *old_linelim = buffer_linelim (buf);
  સંરચના લીટી *old_લીટી = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  સંરચના લીટી *new_linelim = (શૂન્ય *) (newbuf + alloc);
  સંરચના લીટી *new_લીટી = new_linelim - buf->nlines;
  memcpy (new_લીટી, old_લીટી, buf->nlines * કદ *new_લીટી);

  માટે (સંરચના લીટી *લીટી = new_લીટી; લીટી < new_linelim; લીટી++)
    {
      લીટી->text = newbuf + (લીટી->text - oldbuf);
      જો (keylist)
        {
          લીટી->keybeg = newbuf + (લીટી->keybeg - oldbuf);
          લીટી->keylim = newbuf + (લીટી->keylim - oldbuf);
        }
    }

  મુક્ત (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  પાછા true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

સ્થિર bool
maybe_growbuf (સંરચના buffer *buf, સંરચના sort_buffer_policy *policy)
{
  જો (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    પાછા false;

  size_t alloc;
  જો (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  નહીંતર
    alloc = policy->limit;

  જો (try_growbuf (buf, alloc))
    પાછા true;

  policy->growth_failed = true;
  પાછા false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

સ્થિર અક્ષર *
begfield (સંરચના લીટી અચલ *લીટી, સંરચના keyfield અચલ *ચાવી)
{
  અક્ષર *ptr = લીટી->text, *lim = ptr + લીટી->length - 1;
  size_t sword = ચાવી->sword;
  size_t schar = ચાવી->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  જો (tab != TAB_DEFAULT)
    જ્યારે (ptr < lim && sword--)
      {
        અક્ષર *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        જો (ptr < lim)
          ++ptr;
      }
  નહીંતર
    જ્યારે (ptr < lim && sword--)
      {
        જ્યારે (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        જ્યારે (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  જો (ચાવી->skipsblanks)
    જ્યારે (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  જો (schar < remaining_bytes)
    ptr += schar;
  નહીંતર
    ptr = lim;

  પાછા ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
સ્થિર અક્ષર *
limfield (સંરચના લીટી અચલ *લીટી, સંરચના keyfield અચલ *ચાવી)
{
  અક્ષર *ptr = લીટી->text, *lim = ptr + લીટી->length - 1;
  size_t eword = ચાવી->eword, echar = ચાવી->echar;

  જો (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  જો (tab != TAB_DEFAULT)
    જ્યારે (ptr < lim && eword--)
      {
        અક્ષર *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        જો (ptr < lim && (eword || echar))
          ++ptr;
      }
  નહીંતર
    જ્યારે (ptr < lim && eword--)
      {
        જ્યારે (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        જ્યારે (ptr < lim && !blanks[to_uchar (*ptr)])
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
  જો (tab != TAB_DEFAULT)
    {
      અક્ષર *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      જો (newlim)
        lim = newlim;
    }
  નહીંતર
    {
      અક્ષર *newlim;
      newlim = ptr;
      જ્યારે (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      જ્યારે (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  જો (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      જો (ચાવી->skipeblanks)
        જ્યારે (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      જો (echar < remaining_bytes)
        ptr += echar;
      નહીંતર
        ptr = lim;
    }

  પાછા ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

સ્થિર bool
fillbuf (સંરચના buffer *buf, FILE *fp, અક્ષર અચલ *file,
         સંરચના sort_buffer_policy *policy)
{
  સંરચના keyfield અચલ *ચાવી = keylist;
  અક્ષર eol = eolchar;
  size_t લીટી_bytes = buf->લીટી_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  જો (buf->eof)
    પાછા false;

  જો (buf->used != buf->ડાબું)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->ડાબું, buf->ડાબું);
      buf->used = buf->ડાબું;
      buf->nlines = 0;
    }

  જ્યારે (true)
    {
      અક્ષર *ptr = buf->buf + buf->used;
      સંરચના લીટી *linelim = buffer_linelim (buf);
      સંરચના લીટી *લીટી = linelim - buf->nlines;
      size_t avail = (અક્ષર *) linelim - buf->nlines * લીટી_bytes - ptr;
      અક્ષર *લીટી_start = buf->nlines ? લીટી->text + લીટી->length : buf->buf;

      જ્યારે (લીટી_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (લીટી_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          અક્ષર *ptrlim = ptr + bytes_read;
          અક્ષર *p;
          avail -= bytes_read;

          જો (bytes_read != readsize)
            {
              જો (ferror (fp))
                sort_die (_("read failed"), file);
              જો (feof (fp))
                {
                  buf->eof = true;
                  જો (buf->buf == ptrlim)
                    પાછા false;
                  જો (લીટી_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          જ્યારે ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              લીટી--;
              લીટી->text = લીટી_start;
              લીટી->length = ptr - લીટી_start;
              mergesize = MAX (mergesize, લીટી->length);
              avail -= લીટી_bytes;

              જો (ચાવી)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  લીટી->keylim = (ચાવી->eword == SIZE_MAX
                                  ? p
                                  : limfield (લીટી, ચાવી));

                  જો (ચાવી->sword != SIZE_MAX)
                    લીટી->keybeg = begfield (લીટી, ચાવી);
                  નહીંતર
                    {
                      જો (ચાવી->skipsblanks)
                        જ્યારે (blanks[to_uchar (*લીટી_start)])
                          લીટી_start++;
                      લીટી->keybeg = લીટી_start;
                    }
                }

              લીટી_start = ptr;
            }

          ptr = ptrlim;
          જો (buf->eof)
            તોડો;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - લીટી;
      જો (buf->nlines != 0)
        {
          જો (! buf->eof && maybe_growbuf (buf, policy))
            ચાલુ;
          buf->ડાબું = ptr - લીટી_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          પાછા true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t લીટી_alloc = buf->alloc / કદ (સંરચના લીટી);
        buf->buf = xpalloc (buf->buf, &લીટી_alloc, 1, -1, કદ (સંરચના લીટી));
        buf->alloc = લીટી_alloc * કદ (સંરચના લીટી);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
સ્થિર અક્ષર અચલ unit_order[UCHAR_LIM] =
  {
#જો ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#નહીંતર
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
સ્થિર અક્ષર
traverse_raw_સંખ્યા (અક્ષર અચલ **સંખ્યા)
{
  અક્ષર અચલ *p = *સંખ્યા;
  અક્ષર ch;
  અક્ષર max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  જ્યારે (c_isdigit (ch = *p++))
    {
      જો (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      જો (ends_with_thousands_sep)
        ++p;
    }

  જો (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *સંખ્યા = p - 2;
      પાછા max_digit;
    }

  જો (ch == decimal_point)
    જ્યારે (c_isdigit (ch = *p++))
      જો (max_digit < ch)
        max_digit = ch;

  *સંખ્યા = p - 1;
  પાછા max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
find_unit_order (અક્ષર અચલ *સંખ્યા)
{
  bool minus_sign = (*સંખ્યા == '-');
  અક્ષર અચલ *p = સંખ્યા + minus_sign;
  અક્ષર max_digit = traverse_raw_સંખ્યા (&p);
  જો ('0' < max_digit)
    {
      unsigned અક્ષર ch = *p;
      પૂર્ણાંક order = unit_order[ch];
      પાછા (minus_sign ? -order : order);
    }
  નહીંતર
    પાછા 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
human_numcompare (અક્ષર અચલ *a, અક્ષર અચલ *b)
{
  જ્યારે (blanks[to_uchar (*a)])
    a++;
  જ્યારે (blanks[to_uchar (*b)])
    b++;

  પૂર્ણાંક diff = find_unit_order (a) - find_unit_order (b);
  પાછા (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
સ્થિર પૂર્ણાંક
numcompare (અક્ષર અચલ *a, અક્ષર અચલ *b)
{
  જ્યારે (blanks[to_uchar (*a)])
    a++;
  જ્યારે (blanks[to_uchar (*b)])
    b++;

  પાછા strnumcmp (a, b, decimal_point, thousands_sep);
}

સ્થિર પૂર્ણાંક
nan_compare (long દ્વિગુણ a, long દ્વિગુણ b)
{
  અક્ષર buf[2][કદ "-nan""()" + CHAR_BIT * કદ a];
  snprintf (buf[0], કદ buf[0], "%Lf", a);
  snprintf (buf[1], કદ buf[1], "%Lf", b);
  પાછા strcmp (buf[0], buf[1]);
}

સ્થિર પૂર્ણાંક
general_numcompare (અક્ષર અચલ *sa, અક્ષર અચલ *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  અક્ષર *ea;
  અક્ષર *eb;
  long દ્વિગુણ a = strtold (sa, &ea);
  long દ્વિગુણ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  જો (sa == ea)
    પાછા sb == eb ? 0 : -1;
  જો (sb == eb)
    પાછા 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  પાછા (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

સ્થિર પૂર્ણાંક
getmonth (અક્ષર અચલ *month, અક્ષર **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  જ્યારે (blanks[to_uchar (*month)])
    month++;

  કરો
    {
      size_t ix = (lo + hi) / 2;
      અક્ષર અચલ *m = month;
      અક્ષર અચલ *n = monthtab[ix].name;

      માટે (;; m++, n++)
        {
          જો (!*n)
            {
              જો (ea)
                *ea = (અક્ષર *) m;
              પાછા monthtab[ix].val;
            }
          જો (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              તોડો;
            }
          નહીંતર જો (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              તોડો;
            }
        }
    }
  જ્યારે (lo < hi);

  પાછા 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#જો DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# જો 14 <= __GNUC__
#  pragma GCC diagnostic ધકેલો
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#અન્તર્ભાવ "md5.h"

#જો DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# જો 14 <= __GNUC__
#  pragma GCC diagnostic કાઢો
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
સ્થિર શૂન્ય
link_failure (શૂન્ય)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
સ્થિર શૂન્ય *
પ્રતીક_address (શૂન્ય *handle, અક્ષર અચલ *પ્રતીક)
{
  શૂન્ય *address = dlsym (handle, પ્રતીક);
  જો (!address)
    link_failure ();
  પાછા address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
સ્થિર શૂન્ય
link_libcrypto (શૂન્ય)
{
#જો DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  શૂન્ય *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  જો (!handle)
    link_failure ();
  ptr_MD5_Init = પ્રતીક_address (handle, "MD5_Init");
  ptr_MD5_Update = પ્રતીક_address (handle, "MD5_Update");
  ptr_MD5_Final = પ્રતીક_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
સ્થિર સંરચના md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

સ્થિર શૂન્ય
random_md5_state_init (અક્ષર અચલ *random_source)
{
  unsigned અક્ષર buf[MD5_DIGEST_SIZE];
  સંરચના randread_source *r = randread_new (random_source, કદ buf);
  જો (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, કદ buf);
  જો (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, કદ buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

સ્થિર size_t
xstrxfrm (અક્ષર *restrict dest, અક્ષર અચલ *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  જો (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  પાછા translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

સ્થિર પૂર્ણાંક
compare_random (અક્ષર *restrict texta, size_t lena,
                અક્ષર *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  પૂર્ણાંક xfrm_diff = 0;

  અક્ષર stackbuf[4000];
  અક્ષર *buf = stackbuf;
  size_t bufsize = કદ stackbuf;
  શૂન્ય *allocated = ખાલી;
  uint32_t dig[2][MD5_DIGEST_SIZE / કદ (uint32_t)];
  સંરચના md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  જો (hard_LC_COLLATE)
    {
      અક્ષર અચલ *lima = texta + lena;
      અક્ષર અચલ *limb = textb + lenb;

      જ્યારે (true)
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
          જો (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              મુક્ત (allocated);
              buf = allocated = malloc (bufsize);
              જો (! buf)
                {
                  buf = stackbuf;
                  bufsize = કદ stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : ખાલી), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          જો (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              જો (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              મુક્ત (allocated);
              buf = allocated = xmalloc (bufsize);
              જો (texta < lima)
                strxfrm (buf, texta, sizea);
              જો (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          જો (texta < lima)
            texta += strlen (texta) + 1;
          જો (textb < limb)
            textb += strlen (textb) + 1;
          જો (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              તોડો;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          જો (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              જો (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  પૂર્ણાંક diff = memcmp (dig[0], dig[1], કદ dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  જો (! diff)
    {
      જો (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          જો (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  મુક્ત (allocated);

  પાછા diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

સ્થિર size_t
debug_width (અક્ષર અચલ *text, અક્ષર અચલ *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  જ્યારે (text < lim)
    width += (*text++ == '\t');
  પાછા width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

સ્થિર શૂન્ય
mark_ચાવી (size_t offset, size_t width)
{
  જ્યારે (offset--)
    putchar (' ');

  જો (!width)
    printf (_("^ no match for key\n"));
  નહીંતર
    {
      કરો
        putchar ('_');
      જ્યારે (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

સ્થિર અંતઃરેખ bool
ચાવી_numeric (સંરચના keyfield અચલ *ચાવી)
{
  પાછા ચાવી->numeric || ચાવી->general_numeric || ચાવી->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

સ્થિર શૂન્ય
debug_ચાવી (સંરચના લીટી અચલ *લીટી, સંરચના keyfield અચલ *ચાવી)
{
  અક્ષર *text = લીટી->text;
  અક્ષર *beg = text;
  અક્ષર *lim = text + લીટી->length - 1;

  જો (ચાવી)
    {
      જો (ચાવી->sword != SIZE_MAX)
        beg = begfield (લીટી, ચાવી);
      જો (ચાવી->eword != SIZE_MAX)
        {
          lim = limfield (લીટી, ચાવી);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      જો ((ચાવી->skipsblanks && ચાવી->sword == SIZE_MAX)
          || ચાવી->month || ચાવી_numeric (ચાવી))
        {
          અક્ષર saved = *lim;
          *lim = '\0';

          જ્યારે (blanks[to_uchar (*beg)])
            beg++;

          અક્ષર *tighter_lim = beg;

          જો (lim < beg)
            tighter_lim = lim;
          નહીંતર જો (ચાવી->month)
            getmonth (beg, &tighter_lim);
          નહીંતર જો (ચાવી->general_numeric)
            ignore_કિંમત (strtold (beg, &tighter_lim));
          નહીંતર જો (ચાવી->numeric || ચાવી->human_numeric)
            {
              અક્ષર અચલ *p = beg + (beg < lim && *beg == '-');
              અક્ષર max_digit = traverse_raw_સંખ્યા (&p);
              જો ('0' <= max_digit)
                {
                  unsigned અક્ષર ch = *p;
                  tighter_lim = (અક્ષર *) p
                    + (ચાવી->human_numeric && unit_order[ch]);
                }
            }
          નહીંતર
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_ચાવી (offset, width);
}

/* Debug LINE by underlining its keys.  */

સ્થિર શૂન્ય
debug_લીટી (સંરચના લીટી અચલ *લીટી)
{
  સંરચના keyfield અચલ *ચાવી = keylist;

  કરો
    debug_ચાવી (લીટી, ચાવી);
  જ્યારે (ચાવી && ((ચાવી = ચાવી->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

સ્થિર bool
default_ચાવી_compare (સંરચના keyfield અચલ *ચાવી)
{
  પાછા ! (ચાવી->ignore
            || ચાવી->translate
            || ચાવી->skipsblanks
            || ચાવી->skipeblanks
            || ચાવી_numeric (ચાવી)
            || ચાવી->month
            || ચાવી->version
            || ચાવી->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

સ્થિર શૂન્ય
ચાવી_to_opts (સંરચના keyfield અચલ *ચાવી, અક્ષર *opts)
{
  જો (ચાવી->skipsblanks || ચાવી->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  જો (ચાવી->ignore == nondictionary)
    *opts++ = 'd';
  જો (ચાવી->translate)
    *opts++ = 'f';
  જો (ચાવી->general_numeric)
    *opts++ = 'g';
  જો (ચાવી->human_numeric)
    *opts++ = 'h';
  જો (ચાવી->ignore == nonprinting)
    *opts++ = 'i';
  જો (ચાવી->month)
    *opts++ = 'M';
  જો (ચાવી->numeric)
    *opts++ = 'n';
  જો (ચાવી->random)
    *opts++ = 'R';
  જો (ચાવી->reverse)
    *opts++ = 'r';
  જો (ચાવી->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

સ્થિર શૂન્ય
ચાવી_warnings (સંરચના keyfield અચલ *gkey, bool gkey_only)
{
  સંરચના keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  માટે (સંરચના keyfield *ચાવી = keylist; ચાવી; ચાવી = ચાવી->next, keynum++)
    {
      જો (ચાવી_numeric (ચાવી))
        {
          જો (ચાવી->general_numeric)
            general_numeric_field = true;
          નહીંતર
            basic_numeric_field = true;
        }

      જો (ચાવી->traditional_used)
        {
          size_t sword = ચાવી->sword;
          size_t eword = ચાવી->eword;
          અક્ષર tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          અક્ષર obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          અક્ષર nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          અક્ષર *po = obuf;
          અક્ષર *pn = nbuf;

          જો (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          જો (ચાવી->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ચાવી->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = ચાવી->sword != SIZE_MAX && ચાવી->eword < ચાવી->sword;
      જો (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ચાવી_numeric (ચાવી) || ચાવી->month;
      bool લીટી_offset = ચાવી->eword == 0 && ચાવી->echar != 0; /* -k1.x,1.y  */
      જો (!zero_width && !gkey_only && tab == TAB_DEFAULT && !લીટી_offset
          && ((!ચાવી->skipsblanks && !implicit_skip)
              || (!ચાવી->skipsblanks && ચાવી->schar)
              || (!ચાવી->skipeblanks && ચાવી->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      જો (!gkey_only && ચાવી_numeric (ચાવી))
        {
          size_t sword = ચાવી->sword + 1;
          size_t eword = ચાવી->eword + 1;
          જો (!sword)
            sword++;
          જો (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              જો (ચાવી->general_numeric)
                general_numeric_field_span = true;
              નહીંતર
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      જો (ugkey.ignore && (ugkey.ignore == ચાવી->ignore))
        ugkey.ignore = ખાલી;
      જો (ugkey.translate && (ugkey.translate == ચાવી->translate))
        ugkey.translate = ખાલી;
      ugkey.skipsblanks &= !ચાવી->skipsblanks;
      ugkey.skipeblanks &= !ચાવી->skipeblanks;
      ugkey.month &= !ચાવી->month;
      ugkey.numeric &= !ચાવી->numeric;
      ugkey.general_numeric &= !ચાવી->general_numeric;
      ugkey.human_numeric &= !ચાવી->human_numeric;
      ugkey.random &= !ચાવી->random;
      ugkey.version &= !ચાવી->version;
      ugkey.reverse &= !ચાવી->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool સંખ્યા_locale_warned = false;
  જો (basic_numeric_field_span)
    {
      જો (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((અક્ષર []) {thousands_sep, 0})));
          સંખ્યા_locale_warned = true;
        }
    }
  જો (basic_numeric_field_span || general_numeric_field_span)
    {
      જો (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((અક્ષર []) {decimal_point, 0})));
          સંખ્યા_locale_warned = true;
        }
      નહીંતર જો (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((અક્ષર []) {tab, 0})));
        }
      નહીંતર જો (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((અક્ષર []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  જો ((basic_numeric_field || general_numeric_field) && ! સંખ્યા_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((અક્ષર []) {decimal_point, 0})));

    }

  જો (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  જો (!default_ચાવી_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      જો (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      અક્ષર opts[કદ short_options];
      ચાવી_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  જો (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

સ્થિર પૂર્ણાંક
diff_reversed (પૂર્ણાંક diff, bool reversed)
{
  પાછા reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

સ્થિર પૂર્ણાંક
keycompare (સંરચના લીટી અચલ *a, સંરચના લીટી અચલ *b)
{
  સંરચના keyfield *ચાવી = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  અક્ષર *texta = a->keybeg;
  અક્ષર *textb = b->keybeg;
  અક્ષર *lima = a->keylim;
  અક્ષર *limb = b->keylim;

  પૂર્ણાંક diff;

  જ્યારે (true)
    {
      અક્ષર અચલ *translate = ચાવી->translate;
      bool અચલ *ignore = ચાવી->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      જો (hard_LC_COLLATE || ચાવી_numeric (ચાવી)
          || ચાવી->month || ચાવી->random || ચાવી->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          અક્ષર *ta = texta;
          અક્ષર *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          અક્ષર enda = ta[tlena];
          અક્ષર endb = tb[tlenb];

          શૂન્ય *allocated = ખાલી;
          અક્ષર stackbuf[4000];

          જો (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              જો (size <= કદ stackbuf)
                ta = stackbuf;
              નહીંતર
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              માટે (tlena = i = 0; i < lena; i++)
                જો (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              માટે (tlenb = i = 0; i < lenb; i++)
                જો (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          જો (ચાવી->numeric)
            diff = numcompare (ta, tb);
          નહીંતર જો (ચાવી->general_numeric)
            diff = general_numcompare (ta, tb);
          નહીંતર જો (ચાવી->human_numeric)
            diff = human_numcompare (ta, tb);
          નહીંતર જો (ચાવી->month)
            diff = getmonth (ta, ખાલી) - getmonth (tb, ખાલી);
          નહીંતર જો (ચાવી->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          નહીંતર જો (ચાવી->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          નહીંતર
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              જો (tlena == 0)
                diff = - NONZERO (tlenb);
              નહીંતર જો (tlenb == 0)
                diff = 1;
              નહીંતર
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          મુક્ત (allocated);
        }
      નહીંતર જો (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  કરો									\
    {									\
          જ્યારે (true)							\
            {								\
              જ્યારે (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              જ્યારે (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              જો (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  તોડો;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              જો (diff)							\
                તોડો;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  જ્યારે (0)

          જો (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          નહીંતર
            CMP_WITH_IGNORE (*texta, *textb);
        }
      નહીંતર
        {
          size_t lenmin = MIN (lena, lenb);
          જો (lenmin == 0)
            diff = 0;
          નહીંતર જો (translate)
            {
              size_t i = 0;
              કરો
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  જો (diff)
                    તોડો;
                  i++;
                }
              જ્યારે (i < lenmin);
            }
          નહીંતર
            diff = memcmp (texta, textb, lenmin);

          જો (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      જો (diff)
        તોડો;

      ચાવી = ચાવી->next;
      જો (! ચાવી)
        પાછા 0;

      /* Find the beginning and limit of the next field.  */
      જો (ચાવી->eword != SIZE_MAX)
        lima = limfield (a, ચાવી), limb = limfield (b, ચાવી);
      નહીંતર
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      જો (ચાવી->sword != SIZE_MAX)
        texta = begfield (a, ચાવી), textb = begfield (b, ચાવી);
      નહીંતર
        {
          texta = a->text, textb = b->text;
          જો (ચાવી->skipsblanks)
            {
              જ્યારે (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              જ્યારે (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  પાછા diff_reversed (diff, ચાવી->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

સ્થિર પૂર્ણાંક
compare (સંરચના લીટી અચલ *a, સંરચના લીટી અચલ *b)
{
  પૂર્ણાંક diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  જો (keylist)
    {
      diff = keycompare (a, b);
      જો (diff || unique || stable)
        પાછા diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  જો (alen == 0)
    diff = - NONZERO (blen);
  નહીંતર જો (blen == 0)
    diff = 1;
  નહીંતર જો (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  નહીંતર
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      જો (!diff)
        diff = _GL_CMP (alen, blen);
    }

  પાછા diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

સ્થિર શૂન્ય
write_લીટી (સંરચના લીટી અચલ *લીટી, FILE *fp, અક્ષર અચલ *output_file)
{
  અક્ષર *buf = લીટી->text;
  size_t n_bytes = લીટી->length;
  અક્ષર *ebuf = buf + n_bytes;

  જો (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      અક્ષર અચલ *c = buf;

      જ્યારે (c < ebuf)
        {
          અક્ષર wc = *c++;
          જો (wc == '\t')
            wc = '>';
          નહીંતર જો (c == ebuf)
            wc = '\n';
          જો (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_લીટી (લીટી);
    }
  નહીંતર
    {
      ebuf[-1] = eolchar;
      જો (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

સ્થિર bool
check (અક્ષર અચલ *file_name, અક્ષર checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  સંરચના buffer buf;		/* Input buffer. */
  સંરચના લીટી temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t લીટી_સંખ્યા = 0;
  સંરચના keyfield અચલ *ચાવી = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, કદ (સંરચના લીટી),
           MAX (merge_buffer_size, sort_size));
  temp.text = ખાલી;

  જ્યારે (fillbuf (&buf, fp, file_name, ખાલી))
    {
      સંરચના લીટી અચલ *લીટી = buffer_linelim (&buf);
      સંરચના લીટી અચલ *linebase = લીટી - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      જો (alloc && nonunique <= compare (&temp, લીટી - 1))
        {
        found_disorder:
          {
            જો (checkonly == 'c')
              {
                સંરચના લીટી અચલ *disorder_લીટી = લીટી - 1;
                uintmax_t disorder_લીટી_સંખ્યા =
                  buffer_linelim (&buf) - disorder_લીટી + લીટી_સંખ્યા;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_લીટી_સંખ્યા);
                write_લીટી (disorder_લીટી, stderr, _("standard error"));
              }

            ordered = false;
            તોડો;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      જ્યારે (linebase < --લીટી)
        જો (nonunique <= compare (લીટી, લીટી - 1))
          જાઓ found_disorder;

      લીટી_સંખ્યા += buf.nlines;

      /* Save the last line of the buffer.  */
      જો (alloc < લીટી->length)
        {
          કરો
            {
              alloc *= 2;
              જો (! alloc)
                {
                  alloc = લીટી->length;
                  તોડો;
                }
            }
          જ્યારે (alloc < લીટી->length);

          મુક્ત (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, લીટી->text, લીટી->length);
      temp.length = લીટી->length;
      જો (ચાવી)
        {
          temp.keybeg = temp.text + (લીટી->keybeg - લીટી->text);
          temp.keylim = temp.text + (લીટી->keylim - લીટી->text);
        }
    }

  xfclose (fp, file_name);
  મુક્ત (buf.buf);
  મુક્ત (temp.text);
  પાછા ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

સ્થિર size_t
open_આગત_files (સંરચના sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, કદ *fps);
  પૂર્ણાંક i;

  /* Open as many input files as we can.  */
  માટે (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      જો (!fps[i])
        તોડો;
    }

  પાછા i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

સ્થિર શૂન્ય
mergefps (સંરચના sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, અક્ષર અચલ *output_file, FILE **fps)
{
  સંરચના buffer *buffer = xnmalloc (nfiles, કદ *buffer);
                                /* Input buffers for each file. */
  સંરચના લીટી saved;		/* Saved line storage for unique check. */
  સંરચના લીટી અચલ *savedline = ખાલી;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  સંરચના લીટી અચલ **cur = xnmalloc (nfiles, કદ *cur);
                                /* Current line in each line table. */
  સંરચના લીટી અચલ **base = xnmalloc (nfiles, કદ *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, કદ *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  સંરચના keyfield અચલ *ચાવી = keylist;
  saved.text = ખાલી;

  /* Read initial lines from each input file. */
  માટે (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], કદ (સંરચના લીટી),
               MAX (merge_buffer_size, sort_size / nfiles));
      જો (fillbuf (&buffer[i], fps[i], files[i].name, ખાલી))
        {
          સંરચના લીટી અચલ *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      નહીંતર
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          જો (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          મુક્ત (buffer[i].buf);
          --nfiles;
          માટે (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  માટે (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  માટે (size_t i = 1; i < nfiles; ++i)
    જો (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  જ્યારે (nfiles)
    {
      સંરચના લીટી અચલ *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      જો (unique)
        {
          જો (savedline && compare (savedline, smallest))
            {
              savedline = ખાલી;
              write_લીટી (&saved, ofp, output_file);
            }
          જો (!savedline)
            {
              savedline = &saved;
              જો (savealloc < smallest->length)
                {
                  કરો
                    જો (! savealloc)
                      {
                        savealloc = smallest->length;
                        તોડો;
                      }
                  જ્યારે ((savealloc *= 2) < smallest->length);

                  મુક્ત (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              જો (ચાવી)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      નહીંતર
        write_લીટી (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      જો (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      નહીંતર
        {
          જો (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ખાલી))
            {
              સંરચના લીટી અચલ *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          નહીંતર
            {
              /* We reached EOF on fps[ord[0]].  */
              માટે (size_t i = 1; i < nfiles; ++i)
                જો (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              જો (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              મુક્ત (buffer[ord[0]].buf);
              માટે (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              માટે (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ચાલુ;
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
        size_t ગણતરી_of_smaller_લીટીઓ;

        જ્યારે (lo < hi)
          {
            પૂર્ણાંક cmp = compare (cur[ord0], cur[ord[probe]]);
            જો (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            નહીંતર
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ગણતરી_of_smaller_લીટીઓ = lo - 1;
        માટે (size_t j = 0; j < ગણતરી_of_smaller_લીટીઓ; j++)
          ord[j] = ord[j + 1];
        ord[ગણતરી_of_smaller_લીટીઓ] = ord0;
      }
    }

  જો (unique && savedline)
    {
      write_લીટી (&saved, ofp, output_file);
      મુક્ત (saved.text);
    }

  xfclose (ofp, output_file);
  મુક્ત (fps);
  મુક્ત (buffer);
  મુક્ત (ord);
  મુક્ત (base);
  મુક્ત (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

સ્થિર size_t
mergefiles (સંરચના sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, અક્ષર અચલ *output_file)
{
  FILE **fps;
  size_t nopened = open_આગત_files (files, nfiles, &fps);
  જો (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  પાછા nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

સ્થિર શૂન્ય
mergelines (સંરચના લીટી *restrict t, size_t nlines,
            સંરચના લીટી અચલ *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  સંરચના લીટી *hi = t - nlo;

  જ્યારે (true)
    જો (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        જો (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            પાછા;
          }
      }
    નહીંતર
      {
        *--t = *--hi;
        જો (! --nhi)
          {
            કરો
              *--t = *--lo;
            જ્યારે (--nlo);

            પાછા;
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

સ્થિર શૂન્ય
sequential_sort (સંરચના લીટી *restrict લીટીઓ, size_t nlines,
                 સંરચના લીટી *restrict temp, bool to_temp)
{
  જો (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      પૂર્ણાંક swap = (0 < compare (&લીટીઓ[-1], &લીટીઓ[-2]));
      જો (to_temp)
        {
          temp[-1] = લીટીઓ[-1 - swap];
          temp[-2] = લીટીઓ[-2 + swap];
        }
      નહીંતર જો (swap)
        {
          temp[-1] = લીટીઓ[-1];
          લીટીઓ[-1] = લીટીઓ[-2];
          લીટીઓ[-2] = temp[-1];
        }
    }
  નહીંતર
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      સંરચના લીટી *lo = લીટીઓ;
      સંરચના લીટી *hi = લીટીઓ - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      જો (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      નહીંતર જો (!to_temp)
        temp[-1] = lo[-1];

      સંરચના લીટી *dest;
      સંરચના લીટી અચલ *sorted_lo;
      જો (to_temp)
        {
          dest = temp;
          sorted_lo = લીટીઓ;
        }
      નહીંતર
        {
          dest = લીટીઓ;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

સ્થિર સંરચના merge_ગાંઠ *init_ગાંઠ (સંરચના merge_ગાંઠ *restrict,
                                     સંરચના merge_ગાંઠ *restrict,
                                     સંરચના લીટી *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
સ્થિર સંરચના merge_ગાંઠ *
merge_વૃક્ષ_init (size_t nthreads, size_t nlines, સંરચના લીટી *dest)
{
  સંરચના merge_ગાંઠ *merge_વૃક્ષ = xmalloc (2 * કદ *merge_વૃક્ષ * nthreads);

  સંરચના merge_ગાંઠ *મૂળ = merge_વૃક્ષ;
  મૂળ->lo = મૂળ->hi = મૂળ->end_lo = મૂળ->end_hi = ખાલી;
  મૂળ->dest = ખાલી;
  મૂળ->nlo = મૂળ->nhi = nlines;
  મૂળ->parent = ખાલી;
  મૂળ->level = MERGE_END;
  મૂળ->queued = false;
  pthread_mutex_init (&મૂળ->lock, ખાલી);

  init_ગાંઠ (મૂળ, મૂળ + 1, dest, nthreads, nlines, false);
  પાછા merge_વૃક્ષ;
}

/* Destroy the merge tree. */
સ્થિર શૂન્ય
merge_વૃક્ષ_destroy (size_t nthreads, સંરચના merge_ગાંઠ *merge_વૃક્ષ)
{
  size_t n_nodes = nthreads * 2;
  સંરચના merge_ગાંઠ *ગાંઠ = merge_વૃક્ષ;

  જ્યારે (n_nodes--)
    {
      pthread_mutex_destroy (&ગાંઠ->lock);
      ગાંઠ++;
    }

  મુક્ત (merge_વૃક્ષ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

સ્થિર સંરચના merge_ગાંઠ *
init_ગાંઠ (સંરચના merge_ગાંઠ *restrict parent,
           સંરચના merge_ગાંઠ *restrict ગાંઠ_pool,
           સંરચના લીટી *dest, size_t nthreads,
           size_t total_લીટીઓ, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  સંરચના લીટી *lo = dest - total_લીટીઓ;
  સંરચના લીટી *hi = lo - nlo;
  સંરચના લીટી **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  સંરચના merge_ગાંઠ *ગાંઠ = ગાંઠ_pool++;
  ગાંઠ->lo = ગાંઠ->end_lo = lo;
  ગાંઠ->hi = ગાંઠ->end_hi = hi;
  ગાંઠ->dest = parent_end;
  ગાંઠ->nlo = nlo;
  ગાંઠ->nhi = nhi;
  ગાંઠ->parent = parent;
  ગાંઠ->level = parent->level + 1;
  ગાંઠ->queued = false;
  pthread_mutex_init (&ગાંઠ->lock, ખાલી);

  જો (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      ગાંઠ->lo_child = ગાંઠ_pool;
      ગાંઠ_pool = init_ગાંઠ (ગાંઠ, ગાંઠ_pool, lo, lo_threads,
                             total_લીટીઓ, true);
      ગાંઠ->hi_child = ગાંઠ_pool;
      ગાંઠ_pool = init_ગાંઠ (ગાંઠ, ગાંઠ_pool, hi, hi_threads,
                             total_લીટીઓ, false);
    }
  નહીંતર
    {
      ગાંઠ->lo_child = ખાલી;
      ગાંઠ->hi_child = ખાલી;
    }
  પાછા ગાંઠ_pool;
}


/* Compare two merge nodes A and B for priority.  */

સ્થિર પૂર્ણાંક
compare_nodes (શૂન્ય અચલ *a, શૂન્ય અચલ *b)
{
  સંરચના merge_ગાંઠ અચલ *nodea = a;
  સંરચના merge_ગાંઠ અચલ *nodeb = b;
  જો (nodea->level == nodeb->level)
      પાછા (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  પાછા nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

સ્થિર અંતઃરેખ શૂન્ય
lock_ગાંઠ (સંરચના merge_ગાંઠ *ગાંઠ)
{
  pthread_mutex_lock (&ગાંઠ->lock);
}

/* Unlock a merge tree NODE. */

સ્થિર અંતઃરેખ શૂન્ય
unlock_ગાંઠ (સંરચના merge_ગાંઠ *ગાંઠ)
{
  pthread_mutex_unlock (&ગાંઠ->lock);
}

/* Destroy merge QUEUE. */

સ્થિર શૂન્ય
queue_destroy (સંરચના merge_ગાંઠ_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

સ્થિર શૂન્ય
queue_init (સંરચના merge_ગાંઠ_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, ખાલી);
  pthread_cond_init (&queue->cond, ખાલી);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

સ્થિર શૂન્ય
queue_ઉમેરો (સંરચના merge_ગાંઠ_queue *queue, સંરચના merge_ગાંઠ *ગાંઠ)
{
  pthread_mutex_lock (&queue->mutex);
  heap_ઉમેરો (queue->priority_queue, ગાંઠ);
  ગાંઠ->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

સ્થિર સંરચના merge_ગાંઠ *
queue_કાઢો (સંરચના merge_ગાંઠ_queue *queue)
{
  સંરચના merge_ગાંઠ *ગાંઠ;
  pthread_mutex_lock (&queue->mutex);
  જ્યારે (! (ગાંઠ = heap_remove_ટોચ (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_ગાંઠ (ગાંઠ);
  ગાંઠ->queued = false;
  પાછા ગાંઠ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

સ્થિર શૂન્ય
write_unique (સંરચના લીટી અચલ *લીટી, FILE *tfp, અક્ષર અચલ *temp_output)
{
  જો (unique)
    {
      જો (saved_લીટી.text && ! compare (લીટી, &saved_લીટી))
        પાછા;
      saved_લીટી = *લીટી;
    }

  write_લીટી (લીટી, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

સ્થિર શૂન્ય
mergelines_ગાંઠ (સંરચના merge_ગાંઠ *restrict ગાંઠ, size_t total_લીટીઓ,
                 FILE *tfp, અક્ષર અચલ *temp_output)
{
  સંરચના લીટી *lo_orig = ગાંઠ->lo;
  સંરચના લીટી *hi_orig = ગાંઠ->hi;
  size_t to_merge = MAX_MERGE (total_લીટીઓ, ગાંઠ->level);
  size_t merged_lo;
  size_t merged_hi;

  જો (ગાંઠ->level > MERGE_મૂળ)
    {
      /* Merge to destination buffer. */
      સંરચના લીટી *dest = *ગાંઠ->dest;
      જ્યારે (ગાંઠ->lo != ગાંઠ->end_lo && ગાંઠ->hi != ગાંઠ->end_hi && to_merge--)
        જો (compare (ગાંઠ->lo - 1, ગાંઠ->hi - 1) <= 0)
          *--dest = *--ગાંઠ->lo;
        નહીંતર
          *--dest = *--ગાંઠ->hi;

      merged_lo = lo_orig - ગાંઠ->lo;
      merged_hi = hi_orig - ગાંઠ->hi;

      જો (ગાંઠ->nhi == merged_hi)
        જ્યારે (ગાંઠ->lo != ગાંઠ->end_lo && to_merge--)
          *--dest = *--ગાંઠ->lo;
      નહીંતર જો (ગાંઠ->nlo == merged_lo)
        જ્યારે (ગાંઠ->hi != ગાંઠ->end_hi && to_merge--)
          *--dest = *--ગાંઠ->hi;
      *ગાંઠ->dest = dest;
    }
  નહીંતર
    {
      /* Merge directly to output. */
      જ્યારે (ગાંઠ->lo != ગાંઠ->end_lo && ગાંઠ->hi != ગાંઠ->end_hi && to_merge--)
        {
          જો (compare (ગાંઠ->lo - 1, ગાંઠ->hi - 1) <= 0)
            write_unique (--ગાંઠ->lo, tfp, temp_output);
          નહીંતર
            write_unique (--ગાંઠ->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - ગાંઠ->lo;
      merged_hi = hi_orig - ગાંઠ->hi;

      જો (ગાંઠ->nhi == merged_hi)
        {
          જ્યારે (ગાંઠ->lo != ગાંઠ->end_lo && to_merge--)
            write_unique (--ગાંઠ->lo, tfp, temp_output);
        }
      નહીંતર જો (ગાંઠ->nlo == merged_lo)
        {
          જ્યારે (ગાંઠ->hi != ગાંઠ->end_hi && to_merge--)
            write_unique (--ગાંઠ->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ગાંઠ->lo;
  merged_hi = hi_orig - ગાંઠ->hi;
  ગાંઠ->nlo -= merged_lo;
  ગાંઠ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

સ્થિર શૂન્ય
queue_check_ઉમેરો (સંરચના merge_ગાંઠ_queue *queue, સંરચના merge_ગાંઠ *ગાંઠ)
{
  જો (! ગાંઠ->queued)
    {
      bool lo_avail = (ગાંઠ->lo - ગાંઠ->end_lo) != 0;
      bool hi_avail = (ગાંઠ->hi - ગાંઠ->end_hi) != 0;
      જો (lo_avail ? hi_avail || ! ગાંઠ->nhi : hi_avail && ! ગાંઠ->nlo)
        queue_ઉમેરો (queue, ગાંઠ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

સ્થિર શૂન્ય
queue_check_ઉમેરો_parent (સંરચના merge_ગાંઠ_queue *queue,
                           સંરચના merge_ગાંઠ *ગાંઠ)
{
  જો (ગાંઠ->level > MERGE_મૂળ)
    {
      lock_ગાંઠ (ગાંઠ->parent);
      queue_check_ઉમેરો (queue, ગાંઠ->parent);
      unlock_ગાંઠ (ગાંઠ->parent);
    }
  નહીંતર જો (ગાંઠ->nlo + ગાંઠ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_ઉમેરો (queue, ગાંઠ->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

સ્થિર શૂન્ય
merge_loop (સંરચના merge_ગાંઠ_queue *queue,
            size_t total_લીટીઓ, FILE *tfp, અક્ષર અચલ *temp_output)
{
  જ્યારે (true)
    {
      સંરચના merge_ગાંઠ *ગાંઠ = queue_કાઢો (queue);

      જો (ગાંઠ->level == MERGE_END)
        {
          unlock_ગાંઠ (ગાંઠ);
          /* Reinsert so other threads can pop it. */
          queue_ઉમેરો (queue, ગાંઠ);
          તોડો;
        }
      mergelines_ગાંઠ (ગાંઠ, total_લીટીઓ, tfp, temp_output);
      queue_check_ઉમેરો (queue, ગાંઠ);
      queue_check_ઉમેરો_parent (queue, ગાંઠ);

      unlock_ગાંઠ (ગાંઠ);
    }
}


સ્થિર શૂન્ય sortlines (સંરચના લીટી *restrict, size_t, size_t,
                       સંરચના merge_ગાંઠ *, સંરચના merge_ગાંઠ_queue *,
                       FILE *, અક્ષર અચલ *);

/* Thread arguments for sortlines_thread. */

સંરચના thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  સંરચના લીટી *લીટીઓ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t અચલ total_લીટીઓ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  સંરચના merge_ગાંઠ *અચલ ગાંઠ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  સંરચના merge_ગાંઠ_queue *અચલ queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  અક્ષર અચલ *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

સ્થિર શૂન્ય *
sortlines_thread (શૂન્ય *data)
{
  સંરચના thread_args અચલ *args = data;
  sortlines (args->લીટીઓ, args->nthreads, args->total_લીટીઓ,
             args->ગાંઠ, args->queue, args->tfp,
             args->output_temp);
  પાછા ખાલી;
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

સ્થિર શૂન્ય
sortlines (સંરચના લીટી *restrict લીટીઓ, size_t nthreads,
           size_t total_લીટીઓ, સંરચના merge_ગાંઠ *ગાંઠ,
           સંરચના merge_ગાંઠ_queue *queue, FILE *tfp, અક્ષર અચલ *temp_output)
{
  size_t nlines = ગાંઠ->nlo + ગાંઠ->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  સંરચના thread_args args = {લીટીઓ, lo_threads, total_લીટીઓ,
                             ગાંઠ->lo_child, queue, tfp, temp_output};

  જો (nthreads > 1 && SUBTHREAD_લીટીઓ_HEURISTIC <= nlines
      && pthread_create (&thread, ખાલી, sortlines_thread, &args) == 0)
    {
      sortlines (લીટીઓ - ગાંઠ->nlo, hi_threads, total_લીટીઓ,
                 ગાંઠ->hi_child, queue, tfp, temp_output);
      pthread_join (thread, ખાલી);
    }
  નહીંતર
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ગાંઠ->nlo;
      size_t nhi = ગાંઠ->nhi;
      સંરચના લીટી *temp = લીટીઓ - total_લીટીઓ;
      જો (1 < nhi)
        sequential_sort (લીટીઓ - nlo, nhi, temp - nlo / 2, false);
      જો (1 < nlo)
        sequential_sort (લીટીઓ, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      ગાંઠ->lo = લીટીઓ;
      ગાંઠ->hi = લીટીઓ - nlo;
      ગાંઠ->end_lo = લીટીઓ - nlo;
      ગાંઠ->end_hi = લીટીઓ - nlo - nhi;

      queue_ઉમેરો (queue, ગાંઠ);
      merge_loop (queue, total_લીટીઓ, tfp, temp_output);
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

સ્થિર શૂન્ય
avoid_trashing_આગત (સંરચના sortfile *files, size_t ntemps,
                      size_t nfiles, અક્ષર અચલ *outfile)
{
  સંરચના tempnode *tempcopy = ખાલી;

  માટે (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      સંરચના stat instat;

      જો (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      નહીંતર
        {
          સંરચના stat *outst = get_outstatus ();
          જો (!outst)
            તોડો;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      જો (same)
        {
          જો (! tempcopy)
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

સ્થિર શૂન્ય
check_inputs (અક્ષર *અચલ *files, size_t nfiles)
{
  માટે (size_t i = 0; i < nfiles; i++)
    {
      જો (streq (files[i], "-"))
        ચાલુ;

      જો (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

સ્થિર શૂન્ય
check_output (અક્ષર અચલ *outfile)
{
  જો (outfile)
    {
      પૂર્ણાંક oflags = O_WRONLY | O_દ્વિઅંકી | O_CLOEXEC | O_CREAT;
      પૂર્ણાંક outfd = open (outfile, oflags, MODE_RW_UGO);
      જો (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

સ્થિર શૂન્ય
merge (સંરચના sortfile *files, size_t ntemps, size_t nfiles,
       અક્ષર અચલ *output_file)
{
  જ્યારે (nmerge < nfiles)
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
      માટે (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          સંરચના tempnode *temp = create_temp (&tfp);
          size_t અંક_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, અંક_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += અંક_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      જો (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          સંરચના tempnode *temp = create_temp (&tfp);
          size_t અંક_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, અંક_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += અંક_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * કદ *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_આગત (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  જ્યારે (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_આગત_files (files, nfiles, &fps);

      જો (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          જો (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              તોડો;
            }
          જો (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      નહીંતર જો (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      સંરચના tempnode *temp;
      કરો
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      જ્યારે (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * કદ *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

સ્થિર શૂન્ય
sort (અક્ષર *અચલ *files, size_t nfiles, અક્ષર અચલ *output_file,
      size_t nthreads)
{
  સંરચના buffer buf;
  સંરચના sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  જ્યારે (nfiles)
    {
      અક્ષર અચલ *temp_output;
      અક્ષર અચલ *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_લીટી;
      જો (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          જ્યારે (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_લીટી = (mult * કદ (સંરચના લીટી));
        }
      નહીંતર
        bytes_per_લીટી = કદ (સંરચના લીટી) * 3 / 2;

      જો (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_લીટી,
                              &policy);
          initbuf (&buf, bytes_per_લીટી, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      જ્યારે (fillbuf (&buf, fp, file, &policy))
        {
          સંરચના લીટી *લીટી;

          જો (buf.eof && nfiles
              && (bytes_per_લીટી + 1
                  < (buf.alloc - buf.used - bytes_per_લીટી * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ડાબું = buf.used;
              તોડો;
            }

          saved_લીટી.text = ખાલી;
          લીટી = buffer_linelim (&buf);
          જો (buf.eof && !nfiles && !ntemps && !buf.ડાબું)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          નહીંતર
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          જો (1 < buf.nlines)
            {
              જો (nthreads > 1)
                {
                  સંરચના merge_ગાંઠ_queue queue;
                  queue_init (&queue, nthreads);
                  સંરચના merge_ગાંઠ *merge_વૃક્ષ =
                    merge_વૃક્ષ_init (nthreads, buf.nlines, લીટી);

                  sortlines (લીટી, nthreads, buf.nlines, merge_વૃક્ષ + 1,
                             &queue, tfp, temp_output);

                  merge_વૃક્ષ_destroy (nthreads, merge_વૃક્ષ);
                  queue_destroy (&queue);
                }
              નહીંતર
                {
                  sequential_sort (લીટી, buf.nlines,
                                   લીટી - buf.nlines, false);
                  માટે (size_t i = 0; i < buf.nlines; i++)
                    write_unique (લીટી - i - 1, tfp, temp_output);
                }
            }
          નહીંતર
            write_unique (લીટી - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          જો (output_file_created)
            જાઓ finish;
        }
      xfclose (fp, file);
    }

 finish:
  મુક્ત (buf.buf);

  જો (! output_file_created)
    {
      સંરચના tempnode *ગાંઠ = temphead;
      સંરચના sortfile *tempfiles = xnmalloc (ntemps, કદ *tempfiles);
      માટે (size_t i = 0; ગાંઠ; i++)
        {
          tempfiles[i].name = ગાંઠ->name;
          tempfiles[i].temp = ગાંઠ;
          ગાંઠ = ગાંઠ->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      મુક્ત (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

સ્થિર શૂન્ય
insertkey (સંરચના keyfield *ચાવી_arg)
{
  સંરચના keyfield **p;
  સંરચના keyfield *ચાવી = xmemdup (ચાવી_arg, કદ *ચાવી);

  માટે (p = &keylist; *p; p = &(*p)->next)
    ચાલુ;
  *p = ચાવી;
  ચાવી->next = ખાલી;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

સ્થિર શૂન્ય
badfieldspec (અક્ષર અચલ *spec, અક્ષર અચલ *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

સ્થિર શૂન્ય
incompatible_options (અક્ષર અચલ *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

સ્થિર શૂન્ય
check_ordering_compatibility (શૂન્ય)
{
  માટે (સંરચના keyfield *ચાવી = keylist; ચાવી; ચાવી = ચાવી->next)
    જો (1 < (ચાવી->numeric + ચાવી->general_numeric + ચાવી->human_numeric
             + ચાવી->month + (ચાવી->version | ચાવી->random | !!ચાવી->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        અક્ષર opts[કદ short_options];
        /* Clear flags we're not interested in.  */
        ચાવી->skipsblanks = ચાવી->skipeblanks = ચાવી->reverse = false;
        ચાવી_to_opts (ચાવી, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

સ્થિર અક્ષર અચલ *
parse_field_ગણતરી (અક્ષર અચલ *string, size_t *val, અક્ષર અચલ *msgid)
{
  અક્ષર *suffix;
  uintmax_t n;

  પસંદગી (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    સ્થિતિ LONGINT_OK:
    સ્થિતિ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      જો (*val == n)
        તોડો;
      FALLTHROUGH;
    સ્થિતિ LONGINT_OVERFLOW:
    સ્થિતિ LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      તોડો;

    સ્થિતિ LONGINT_INVALID:
      જો (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      પાછા ખાલી;
    }

  પાછા suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

સ્થિર અક્ષર *
set_ordering (અક્ષર અચલ *s, સંરચના keyfield *ચાવી, ગણના blanktype blanktype)
{
  જ્યારે (*s)
    {
      પસંદગી (*s)
        {
        સ્થિતિ 'b':
          જો (blanktype == bl_start || blanktype == bl_both)
            ચાવી->skipsblanks = true;
          જો (blanktype == bl_end || blanktype == bl_both)
            ચાવી->skipeblanks = true;
          તોડો;
        સ્થિતિ 'd':
          ચાવી->ignore = nondictionary;
          તોડો;
        સ્થિતિ 'f':
          ચાવી->translate = fold_toupper;
          તોડો;
        સ્થિતિ 'g':
          ચાવી->general_numeric = true;
          તોડો;
        સ્થિતિ 'h':
          ચાવી->human_numeric = true;
          તોડો;
        સ્થિતિ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          જો (! ચાવી->ignore)
            ચાવી->ignore = nonprinting;
          તોડો;
        સ્થિતિ 'M':
          ચાવી->month = true;
          તોડો;
        સ્થિતિ 'n':
          ચાવી->numeric = true;
          તોડો;
        સ્થિતિ 'R':
          ચાવી->random = true;
          તોડો;
        સ્થિતિ 'r':
          ચાવી->reverse = true;
          તોડો;
        સ્થિતિ 'V':
          ચાવી->version = true;
          તોડો;
        મૂળભૂત:
          પાછા (અક્ષર *) s;
        }
      ++s;
    }
  પાછા (અક્ષર *) s;
}

/* Initialize KEY.  */

સ્થિર સંરચના keyfield *
ચાવી_init (સંરચના keyfield *ચાવી)
{
  memset (ચાવી, 0, કદ *ચાવી);
  ચાવી->eword = SIZE_MAX;
  પાછા ચાવી;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  સંરચના keyfield *ચાવી;
  સંરચના keyfield ચાવી_buf;
  સંરચના keyfield gkey;
  bool gkey_only = false;
  અક્ષર અચલ *s;
  પૂર્ણાંક c = 0;
  અક્ષર checkonly = 0;
  bool mergeonly = false;
  અક્ષર *random_source = ખાલી;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ખાલી);
  પૂર્ણાંક posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  અક્ષર **files;
  અક્ષર *files_from = ખાલી;
  સંરચના Tokens tok;
  અક્ષર અચલ *outfile = ખાલી;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#જો HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    સંરચના lconv અચલ *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    જો (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    જો (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    જો (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ગણના { nsigs = countof (term_sig) };

    સંરચના sigaction act;

    sigemptyset (&caught_signals);
    માટે (size_t i = 0; i < nsigs; i++)
      {
        જો (term_sig[i] == SIGPIPE)
          ચાલુ;  /* Handled below.  */
        sigaction (term_sig[i], ખાલી, &act);
        જો (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    માટે (size_t i = 0; i < nsigs; i++)
      {
        જો (term_sig[i] == SIGPIPE)
          ચાલુ;  /* Handled below.  */
        જો (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, ખાલી);
      }
  }
  સંકેત (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ચાવી_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, કદ *files);

  જ્યારે (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      પૂર્ણાંક oi = -1;

      જો (c == -1
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
          જો (argc <= optind)
            તોડો;
          files[nfiles++] = argv[optind++];
        }
      નહીંતર પસંદગી (c)
        {
        સ્થિતિ 1:
          ચાવી = ખાલી;
          જો (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              જો (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ચાવી = ચાવી_init (&ચાવી_buf);
                  s = parse_field_ગણતરી (optarg + 1, &ચાવી->sword, ખાલી);
                  જો (s && *s == '.')
                    s = parse_field_ગણતરી (s + 1, &ચાવી->schar, ખાલી);
                  જો (! (ચાવી->sword || ચાવી->schar))
                    ચાવી->sword = SIZE_MAX;
                  જો (! s || *set_ordering (s, ચાવી, bl_start))
                    ચાવી = ખાલી;
                  નહીંતર
                    {
                      જો (minus_pos_usage)
                        {
                          અક્ષર અચલ *optarg1 = argv[optind++];
                          s = parse_field_ગણતરી (optarg1 + 1, &ચાવી->eword,
                                             N_("invalid number after '-'"));
                          જો (*s == '.')
                            s = parse_field_ગણતરી (s + 1, &ચાવી->echar,
                                               N_("invalid number after '.'"));
                          જો (!ચાવી->echar && ચાવી->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ચાવી->eword--;
                            }
                          જો (*set_ordering (s, ચાવી, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ચાવી->traditional_used = true;
                      insertkey (ચાવી);
                    }
                }
            }
          જો (! ચાવી)
            files[nfiles++] = optarg;
          તોડો;

        સ્થિતિ SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        સ્થિતિ 'b':
        સ્થિતિ 'd':
        સ્થિતિ 'f':
        સ્થિતિ 'g':
        સ્થિતિ 'h':
        સ્થિતિ 'i':
        સ્થિતિ 'M':
        સ્થિતિ 'n':
        સ્થિતિ 'r':
        સ્થિતિ 'R':
        સ્થિતિ 'V':
          {
            અક્ષર str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          તોડો;

        સ્થિતિ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        સ્થિતિ 'c':
        સ્થિતિ 'C':
          જો (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          તોડો;

        સ્થિતિ COMPRESS_PROGRAM_OPTION:
          જો (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          તોડો;

        સ્થિતિ DEBUG_PROGRAM_OPTION:
          debug = true;
          તોડો;

        સ્થિતિ FILES0_FROM_OPTION:
          files_from = optarg;
          તોડો;

        સ્થિતિ 'k':
          ચાવી = ચાવી_init (&ચાવી_buf);

          /* Get POS1. */
          s = parse_field_ગણતરી (optarg, &ચાવી->sword,
                                 N_("invalid number at field start"));
          જો (! ચાવી->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          જો (*s == '.')
            {
              s = parse_field_ગણતરી (s + 1, &ચાવી->schar,
                                     N_("invalid number after '.'"));
              જો (! ચાવી->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          જો (! (ચાવી->sword || ચાવી->schar))
            ચાવી->sword = SIZE_MAX;
          s = set_ordering (s, ચાવી, bl_start);
          જો (*s != ',')
            {
              ચાવી->eword = SIZE_MAX;
              ચાવી->echar = 0;
            }
          નહીંતર
            {
              /* Get POS2. */
              s = parse_field_ગણતરી (s + 1, &ચાવી->eword,
                                     N_("invalid number after ','"));
              જો (! ચાવી->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              જો (*s == '.')
                {
                  s = parse_field_ગણતરી (s + 1, &ચાવી->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, ચાવી, bl_end);
            }
          જો (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ચાવી);
          તોડો;

        સ્થિતિ 'm':
          mergeonly = true;
          તોડો;

        સ્થિતિ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          તોડો;

        સ્થિતિ 'o':
          જો (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          તોડો;

        સ્થિતિ RANDOM_SOURCE_OPTION:
          જો (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          તોડો;

        સ્થિતિ 's':
          stable = true;
          તોડો;

        સ્થિતિ 'S':
          specify_sort_size (oi, c, optarg);
          તોડો;

        સ્થિતિ 't':
          {
            અક્ષર newtab = optarg[0];
            જો (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            જો (optarg[1])
              {
                જો (streq (optarg, "\\0"))
                  newtab = '\0';
                નહીંતર
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            જો (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          તોડો;

        સ્થિતિ 'T':
          add_temp_dir (optarg);
          તોડો;

        સ્થિતિ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          તોડો;

        સ્થિતિ 'u':
          unique = true;
          તોડો;

        સ્થિતિ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          જો (optarg == argv[optind - 1])
            {
              અક્ષર અચલ *p;
              માટે (p = optarg; c_isdigit (*p); p++)
                ચાલુ;
              optind -= (*p != '\0');
            }
          તોડો;

        સ્થિતિ 'z':
          eolchar = 0;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (સંકેત (SIGPIPE, SIG_IGN) == SIG_DFL);

  જો (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      જો (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      જો (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      જો (tok.n_tok)
        {
          મુક્ત (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          માટે (size_t i = 0; i < nfiles; i++)
            {
              જો (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              નહીંતર જો (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long પૂર્ણાંક file_સંખ્યા = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_સંખ્યા);
                }
            }
        }
      નહીંતર
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  માટે (ચાવી = keylist; ચાવી; ચાવી = ચાવી->next)
    {
      જો (default_ચાવી_compare (ચાવી) && !ચાવી->reverse)
        {
          ચાવી->ignore = gkey.ignore;
          ચાવી->translate = gkey.translate;
          ચાવી->skipsblanks = gkey.skipsblanks;
          ચાવી->skipeblanks = gkey.skipeblanks;
          ચાવી->month = gkey.month;
          ચાવી->numeric = gkey.numeric;
          ચાવી->general_numeric = gkey.general_numeric;
          ચાવી->human_numeric = gkey.human_numeric;
          ચાવી->version = gkey.version;
          ચાવી->random = gkey.random;
          ચાવી->reverse = gkey.reverse;
        }

      need_random |= ચાવી->random;
    }

  જો (!keylist && !default_ચાવી_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  જો (debug)
    {
      જો (checkonly || outfile)
        {
          સ્થિર અક્ષર opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      જો (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      જો (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      જો (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ખાલી)));
      નહીંતર
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ચાવી_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  જો (need_random)
    random_md5_state_init (random_source);

  જો (temp_dirs.n_dirs == 0)
    {
      અક્ષર અચલ *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  જો (nfiles == 0)
    {
      nfiles = 1;
      મુક્ત (files);
      files = xmalloc (કદ *files);
      *files = (અક્ષર *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  જો (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  જો (checkonly)
    {
      જો (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      જો (outfile)
        {
          સ્થિર અક્ષર opts[] = {0, 'o', 0};
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

  જો (mergeonly)
    {
      સંરચના sortfile *sortfiles = xcalloc (nfiles, કદ *sortfiles);

      માટે (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  નહીંતર
    {
      જો (!nthreads)
        {
          unsigned long પૂર્ણાંક np = અંક_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * કદ (સંરચના merge_ગાંઠ));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  જો (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
