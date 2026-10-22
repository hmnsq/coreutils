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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <pthread.h>
#அந்தர்பாவ <sys/resource.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <spawn.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "filevercmp.h"
#அந்தர்பாவ "findprog.h"
#அந்தர்பாவ "flexmember.h"
#அந்தர்பாவ "hard-locale.h"
#அந்தர்பாவ "hash.h"
#அந்தர்பாவ "heap.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "mbswidth.h"
#அந்தர்பாவ "nproc.h"
#அந்தர்பாவ "physmem.h"
#அந்தர்பாவ "posixver.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "randread.h"
#அந்தர்பாவ "readtokens0.h"
#அந்தர்பாவ "stdlib--.h"
#அந்தர்பாவ "strnumcmp.h"
#அந்தர்பாவ "term-sig.h"
#அந்தர்பாவ "unistd--.h"
#அந்தர்பாவ "xmemcoll.h"
#அந்தர்பாவ "xnanosleep.h"
#அந்தர்பாவ "xstrtol.h"
#அந்தர்பாவ "xstrtol-error.h"

#ifndef RLIMIT_DATA
கட்டமைப்பு rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#எனில் HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#எனில் !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#எனில் !defined OPEN_MAX
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
எண்ணிக்கை { SUBTHREAD_வரிகள்_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_வரிகள்_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
எண்ணிக்கை { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
எண்ணிக்கை
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

எண்ணிக்கை
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

எண்ணிக்கை
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_வேர் = 1
  };

/* The representation of the decimal point in the current locale.  */
நிலையான எழுத்து decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
நிலையான முழு thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
நிலையான bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
நிலையான bool hard_LC_COLLATE;
#எனில் HAVE_NL_LANGINFO
நிலையான bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
எண்ணிக்கை blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
நிலையான எழுத்து eolchar = '\n';

/* Lines are held in memory as counted strings. */
கட்டமைப்பு வரி
{
  எழுத்து *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  எழுத்து *keybeg;			/* Start of first key. */
  எழுத்து *keylim;			/* Limit of first key. */
};

/* Input buffers. */
கட்டமைப்பு buffer
{
  எழுத்து *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t இடது;			/* Number of bytes left from previous reads. */
  size_t வரி_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
கட்டமைப்பு sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
கட்டமைப்பு keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool மாறிலி *ignore;		/* Boolean array of characters to ignore. */
  எழுத்து மாறிலி *translate;	/* Translation applied to characters. */
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
  கட்டமைப்பு keyfield *next;	/* Next keyfield to try. */
};

கட்டமைப்பு month
{
  எழுத்து மாறிலி *name;
  முழு val;
};

/* Binary merge tree node. */
கட்டமைப்பு merge_முனை
{
  கட்டமைப்பு வரி *lo;              /* Lines to merge from LO child node. */
  கட்டமைப்பு வரி *hi;              /* Lines to merge from HI child node. */
  கட்டமைப்பு வரி *end_lo;          /* End of available lines from LO. */
  கட்டமைப்பு வரி *end_hi;          /* End of available lines from HI. */
  கட்டமைப்பு வரி **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  கட்டமைப்பு merge_முனை *parent;    /* Parent node. */
  கட்டமைப்பு merge_முனை *lo_child;  /* LO child node. */
  கட்டமைப்பு merge_முனை *hi_child;  /* HI child node. */
  unsigned முழு level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
கட்டமைப்பு merge_முனை_queue
{
  கட்டமைப்பு heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
நிலையான கட்டமைப்பு வரி saved_வரி;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
நிலையான bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
நிலையான bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
நிலையான bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
நிலையான எழுத்து fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
நிலையான கட்டமைப்பு month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + அளவு (கட்டமைப்பு வரி))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
நிலையான size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
நிலையான size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define உள்ளீடு_FILE_SIZE_GUESS (128 * 1024)

நிலையான கட்டமைப்பு
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  எழுத்து மாறிலி **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
நிலையான bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
நிலையான bool stable;

/* An int value outside char range.  */
எண்ணிக்கை { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
எண்ணிக்கை { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
நிலையான முழு tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
நிலையான bool unique;

/* Nonzero if any of the input files are the standard input. */
நிலையான bool have_read_stdin;

/* List of key field comparisons to be tried.  */
நிலையான கட்டமைப்பு keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
நிலையான எழுத்து மாறிலி *compress_program;

/* Annotate the output with extra info to aid the user.  */
நிலையான bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
நிலையான unsigned முழு nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
நிலையான bool default_SIGPIPE;

/* The list of temporary files. */
கட்டமைப்பு tempnode
{
  கட்டமைப்பு tempnode *நிலையற்ற next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  எழுத்து state;
  எழுத்து name[FLEXIBLE_ARRAY_MEMBER];
};
நிலையான கட்டமைப்பு tempnode *நிலையற்ற temphead;
நிலையான கட்டமைப்பு tempnode *நிலையற்ற *temptail = &temphead;

/* Clean up any remaining temporary files.  */

நிலையான வெறுமை
cleanup (வெறுமை)
{
  க்கு (கட்டமைப்பு tempnode மாறிலி *முனை = temphead; முனை; முனை = முனை->next)
    unlink (முனை->name);
  temphead = வெற்று;
}

/* Handle interrupts and hangups. */

நிலையான வெறுமை
sighandler (முழு sig)
{
  cleanup ();

  சமிக்ஞை (sig, SIG_DFL);
  எழுப்பு (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

நிலையான வெறுமை
sort_die (எழுத்து மாறிலி *message, எழுத்து மாறிலி *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  எனில் (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         ignore leading blanks when finding sort சாவிகள் in each வரி\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-நிலை\n\
         fold lower நிலை to upper நிலை characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical மதிப்பு\n\
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
         compare according to string numerical மதிப்பு;\n\
         see full documentation க்கு supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical சாவிகள்.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; க்கு more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check க்கு sorted உள்ளீடு; செய் not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but செய் not report first bad வரி\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the வரி used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         பட உள்ளீடு from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, பட names from standard உள்ளீடு\n\
"));
      oputs (_("\
  -k, --சாவி=KEYDEF\n\
         sort via a சாவி; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; செய் not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         லிக result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE க்கு முதன்மை memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR க்கு temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the எண் of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of வரிகள் with equal சாவிகள்;\n\
         with -c, check க்கு strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] க்கு start and stop position, where F is a\n\
field எண் and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the வரி's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options க்கு that சாவி.  If no சாவி is given, use\n\
the entire வரி as the சாவி.  Use --debug to diagnose incorrect சாவி usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (இயல்பு), and so on க்கு M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte மதிப்புகள்.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
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

நிலையான எழுத்து மாறிலி short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"ignore-leading-blanks", no_argument, வெற்று, 'b'},
  {"check", optional_argument, வெற்று, CHECK_OPTION},
  {"compress-program", required_argument, வெற்று, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, வெற்று, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, வெற்று, 'd'},
  {"ignore-case", no_argument, வெற்று, 'f'},
  {"files0-from", required_argument, வெற்று, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, வெற்று, 'g'},
  {"ignore-nonprinting", no_argument, வெற்று, 'i'},
  {"key", required_argument, வெற்று, 'k'},
  {"merge", no_argument, வெற்று, 'm'},
  {"month-sort", no_argument, வெற்று, 'M'},
  {"numeric-sort", no_argument, வெற்று, 'n'},
  {"human-numeric-sort", no_argument, வெற்று, 'h'},
  {"version-sort", no_argument, வெற்று, 'V'},
  {"random-sort", no_argument, வெற்று, 'R'},
  {"random-source", required_argument, வெற்று, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, வெற்று, SORT_OPTION},
  {"output", required_argument, வெற்று, 'o'},
  {"reverse", no_argument, வெற்று, 'r'},
  {"stable", no_argument, வெற்று, 's'},
  {"batch-size", required_argument, வெற்று, NMERGE_OPTION},
  {"buffer-size", required_argument, வெற்று, 'S'},
  {"field-separator", required_argument, வெற்று, 't'},
  {"temporary-directory", required_argument, வெற்று, 'T'},
  {"unique", no_argument, வெற்று, 'u'},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {"parallel", required_argument, வெற்று, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

நிலையான எழுத்து மாறிலி *மாறிலி check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE வெற்று
#undef  _ct_
};
நிலையான எழுத்து மாறிலி check_types[] =
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

நிலையான எழுத்து மாறிலி *மாறிலி sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE வெற்று
#undef  _st_
};
நிலையான எழுத்து மாறிலி sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
நிலையான sigset_t caught_signals;

/* Critical section status.  */
கட்டமைப்பு cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
நிலையான வெறுமை
cs_enter (கட்டமைப்பு cs_status *status)
{
  முழு ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
நிலையான வெறுமை
cs_leave (கட்டமைப்பு cs_status மாறிலி *status)
{
  எனில் (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, வெற்று);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
எண்ணிக்கை { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
கட்டமைப்பு sortfile
{
  /* The file's name.  */
  எழுத்து மாறிலி *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  கட்டமைப்பு tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
நிலையான Hash_table *proctab;

எண்ணிக்கை { INIT_PROCTAB_SIZE = 47 };

நிலையான size_t
proctab_hasher (வெறுமை மாறிலி *entry, size_t tabsize)
{
  கட்டமைப்பு tempnode மாறிலி *முனை = entry;
  திரும்பு முனை->pid % tabsize;
}

நிலையான bool
proctab_comparator (வெறுமை மாறிலி *e1, வெறுமை மாறிலி *e2)
{
  கட்டமைப்பு tempnode மாறிலி *n1 = e1;
  கட்டமைப்பு tempnode மாறிலி *n2 = e2;
  திரும்பு n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
நிலையான pid_t nprocs;

நிலையான bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

நிலையான pid_t
reap (pid_t pid)
{
  முழு status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  எனில் (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  இல்லையெனில் எனில் (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      எனில் (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  திரும்பு cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

நிலையான வெறுமை
register_proc (கட்டமைப்பு tempnode *temp)
{
  எனில் (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, வெற்று,
                                 proctab_hasher,
                                 proctab_comparator,
                                 வெற்று);
      எனில் (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  எனில் (! hash_செருகு (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

நிலையான bool
delete_proc (pid_t pid)
{
  கட்டமைப்பு tempnode test;

  test.pid = pid;
  கட்டமைப்பு tempnode *முனை = hash_remove (proctab, &test);
  எனில் (! முனை)
    திரும்பு false;
  முனை->state = REAPED;
  திரும்பு true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

நிலையான வெறுமை
wait_proc (pid_t pid)
{
  எனில் (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

நிலையான வெறுமை
reap_exited (வெறுமை)
{
  போது (0 < nprocs && reap (0))
    தொடர்;
}

/* Reap at least one exited child, waiting if necessary.  */

நிலையான வெறுமை
reap_some (வெறுமை)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

நிலையான வெறுமை
reap_all (வெறுமை)
{
  போது (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

நிலையான வெறுமை
exit_cleanup (வெறுமை)
{
  எனில் (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      கட்டமைப்பு cs_status cs;
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

நிலையான கட்டமைப்பு tempnode *
create_temp_file (முழு *pfd, bool survive_fd_exhaustion)
{
  நிலையான எழுத்து மாறிலி slashbase[] = "/sortXXXXXX";
  நிலையான idx_t temp_dir_index;
  முழு fd;
  முழு saved_errno;
  எழுத்து மாறிலி *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  கட்டமைப்பு tempnode *முனை =
    xmalloc (FLEXSIZEOF (கட்டமைப்பு tempnode, name, len + அளவு slashbase));
  எழுத்து *file = முனை->name;
  கட்டமைப்பு cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, அளவு slashbase);
  முனை->next = வெற்று;
  எனில் (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  எனில் (0 <= fd)
    {
      *temptail = முனை;
      temptail = &முனை->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  எனில் (fd < 0)
    {
      எனில் (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      விடுவி (முனை);
      முனை = வெற்று;
    }

  *pfd = fd;
  திரும்பு முனை;
}

/* Return a pointer to stdout status, or NULL on failure.  */

நிலையான கட்டமைப்பு stat *
get_outstatus (வெறுமை)
{
  நிலையான முழு outstat_errno;
  நிலையான கட்டமைப்பு stat outstat;
  எனில் (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  திரும்பு outstat_errno < 0 ? &outstat : வெற்று;
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

நிலையான FILE *
stream_open (எழுத்து மாறிலி *file, எழுத்து மாறிலி *how)
{
  FILE *fp;

  எனில் (*how == 'r')
    {
      எனில் (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      இல்லையெனில்
        {
          முழு fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? வெற்று : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  இல்லையெனில் எனில் (*how == 'w')
    {
      எனில் (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          முழு ftruncate_errno = errno;
          கட்டமைப்பு stat *outst = get_outstatus ();
          எனில் (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  இல்லையெனில்
    affirm (!"unexpected mode passed to stream_open");

  திரும்பு fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

நிலையான FILE *
xfopen (எழுத்து மாறிலி *file, எழுத்து மாறிலி *how)
{
  FILE *fp = stream_open (file, how);
  எனில் (!fp)
    sort_die (_("open failed"), file);
  திரும்பு fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

நிலையான வெறுமை
xfclose (FILE *fp, எழுத்து மாறிலி *file)
{
  தேர்வு (fileno (fp))
    {
    நிலை STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      நிறுத்து;

    நிலை STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      எனில் (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      நிறுத்து;

    இயல்பு:
      எனில் (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      நிறுத்து;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

நிலையான வெறுமை
move_fd (முழு oldfd, முழு newfd)
{
  எனில் (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_மதிப்பு (dup2 (oldfd, newfd));
      ignore_மதிப்பு (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

நிலையான முழு
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  முழு oldfd, முழு newfd)
{
  முழு result = 0;
  எனில் (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      எனில் (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  திரும்பு result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

நிலையான எழுத்து மாறிலி *
get_resolved_compress_program (வெறுமை)
{
  /* Use a cache, to perform the search only once.  */
  நிலையான எழுத்து மாறிலி *resolved_compress_program_cache /* = NULL */;

  எனில் (resolved_compress_program_cache == வெற்று)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), வெற்று, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  திரும்பு resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

நிலையான முழு
pipe_child (pid_t *pid, முழு pipefds[2], முழு tempfd, bool decompress,
            size_t tries)
{
  எழுத்து மாறிலி *resolved_compress_program;
  கட்டமைப்பு tempnode *saved_temphead;
  இரட்டை wait_retry = 0.25;
  கட்டமைப்பு cs_status cs;
  முழு result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  எனில் (resolved_compress_program == வெற்று)
    திரும்பு errno;

  எனில் ((result = posix_spawnattr_init (&attr)))
    திரும்பு result;
  எனில் ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      திரும்பு result;
    }

  எனில் (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      முழு saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      திரும்பு saved_errno;
    }

  எனில் ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      திரும்பு result;
    }

  எழுத்து மாறிலி *மாறிலி argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : வெற்று,
      வெற்று
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  எனில் (nmerge + 1 < nprocs)
    reap_some ();

  போது (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = வெற்று;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (எழுத்து * மாறிலி *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      எனில் (result != EAGAIN)
        நிறுத்து;
      இல்லையெனில்
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

  எனில் (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  இல்லையெனில்
    ++nprocs;

  திரும்பு result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

நிலையான கட்டமைப்பு tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  முழு tempfd;
  கட்டமைப்பு tempnode *முனை = create_temp_file (&tempfd, survive_fd_exhaustion);
  எனில் (! முனை)
    திரும்பு வெற்று;

  முனை->state = UNCOMPRESSED;

  எனில் (compress_program)
    {
      முழு pipefds[2];
      நிலையான முழு last_result = 0;

      முழு result = pipe_child (&முனை->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      எனில் (result)
        {
          எனில் (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      இல்லையெனில்
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (முனை);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  எனில் (! *pfp)
    sort_die (_("couldn't create temporary file"), முனை->name);

  திரும்பு முனை;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

நிலையான கட்டமைப்பு tempnode *
create_temp (FILE **pfp)
{
  திரும்பு maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

நிலையான FILE *
open_temp (கட்டமைப்பு tempnode *temp)
{
  முழு tempfd, pipefds[2];
  FILE *fp = வெற்று;

  எனில் (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  எனில் (tempfd < 0)
    திரும்பு வெற்று;

  pid_t child;
  முழு result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  எனில் (result)
    {
      எனில் (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  இல்லையெனில்
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      எனில் (! fp)
        {
          முழு saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  திரும்பு fp;
}

/* Append DIR to the array of temporary directory names.  */
நிலையான வெறுமை
add_temp_dir (எழுத்து மாறிலி *dir)
{
  எனில் (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              அளவு *temp_dirs.dirs);

  idx_t மாறிலி dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

நிலையான வெறுமை
zaptemp (எழுத்து மாறிலி *name)
{
  கட்டமைப்பு tempnode *நிலையற்ற *pnode;
  கட்டமைப்பு tempnode *முனை;
  கட்டமைப்பு tempnode *next;
  முழு unlink_status;
  முழு unlink_errno = 0;
  கட்டமைப்பு cs_status cs;

  க்கு (pnode = &temphead; (முனை = *pnode)->name != name; pnode = &முனை->next)
    தொடர்;

  எனில் (முனை->state == UNREAPED)
    wait_proc (முனை->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = முனை->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  எனில் (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  எனில் (! next)
    temptail = pnode;
  விடுவி (முனை);
}

#எனில் HAVE_NL_LANGINFO

நிலையான முழு
struct_month_cmp (வெறுமை மாறிலி *m1, வெறுமை மாறிலி *m2)
{
  கட்டமைப்பு month மாறிலி *month1 = m1;
  கட்டமைப்பு month மாறிலி *month2 = m2;
  திரும்பு strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

நிலையான வெறுமை
inittables (வெறுமை)
{
  க்கு (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#எனில் HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  எனில் (hard_LC_TIME)
    {
      க்கு (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          எழுத்து மாறிலி *s;
          size_t s_len;
          size_t j, k;
          எழுத்து *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          க்கு (j = k = 0; j < s_len; j++)
            எனில் (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, அளவு *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
நிலையான வெறுமை
specify_nmerge (முழு oi, எழுத்து c, எழுத்து மாறிலி *s)
{
  uintmax_t n;
  கட்டமைப்பு rlimit rlimit;
  எண்ணிக்கை strtol_error e = xstrtoumax (s, வெற்று, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned முழு max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  எனில் (e == LONGINT_OK)
    {
      nmerge = n;
      எனில் (nmerge != n)
        e = LONGINT_OVERFLOW;
      இல்லையெனில்
        {
          எனில் (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          இல்லையெனில் எனில் (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          இல்லையெனில்
            திரும்பு;
        }
    }

  எனில் (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  இல்லையெனில்
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
நிலையான வெறுமை
specify_sort_size (முழு oi, எழுத்து c, எழுத்து மாறிலி *s)
{
  uintmax_t n;
  எழுத்து *suffix;
  எண்ணிக்கை strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  எனில் (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  எனில் (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    தேர்வு (suffix[0])
      {
      நிலை 'b':
        e = LONGINT_OK;
        நிறுத்து;

      நிலை '%':
        {
          இரட்டை mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          எனில் (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          இல்லையெனில்
            e = LONGINT_OVERFLOW;
        }
        நிறுத்து;
      }

  எனில் (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      எனில் (n < sort_size)
        திரும்பு;

      sort_size = n;
      எனில் (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          திரும்பு;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
நிலையான size_t
specify_nthreads (முழு oi, எழுத்து c, எழுத்து மாறிலி *s)
{
  uintmax_t nthreads;
  எண்ணிக்கை strtol_error e = xstrtoumax (s, வெற்று, 10, &nthreads, "");
  எனில் (e == LONGINT_OVERFLOW)
    திரும்பு SIZE_MAX;
  எனில் (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  எனில் (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  எனில் (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  திரும்பு nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
நிலையான size_t
default_sort_size (வெறுமை)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  கட்டமைப்பு rlimit rlimit;
  எனில் (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  எனில் (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  எனில் (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  இரட்டை avail = physmem_available ();
  இரட்டை total = physmem_total ();
  இரட்டை mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  எனில் (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  எனில் (mem < size)
    size = mem;
  திரும்பு MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

நிலையான bool
வரி_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = அளவு (கட்டமைப்பு வரி);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  எனில் (ckd_add (&aligned, size, padding))
    திரும்பு false;

  *alloc = aligned;
  திரும்பு true;
}

/* Return the default initial allocation for a growable sort buffer.  */

நிலையான size_t
default_initial_sort_size (size_t வரி_bytes)
{
  size_t size = வரி_bytes + 2;
  size_t உள்ளீடு_size;

  எனில் (ckd_mul (&உள்ளீடு_size, உள்ளீடு_FILE_SIZE_GUESS, வரி_bytes + 1)
      || ckd_add (&size, size, உள்ளீடு_size))
    திரும்பு SIZE_MAX;

  திரும்பு MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

நிலையான bool
உள்ளீடு_size_buffer_bytes (uintmax_t file_size, size_t வரி_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_உள்ளீடு_byte = வரி_bytes + 1;

  எனில் (SIZE_MAX / worst_case_per_உள்ளீடு_byte < file_size)
    திரும்பு false;

  size_t size = file_size * worst_case_per_உள்ளீடு_byte;
  எனில் (SIZE_MAX - size < 1)
    திரும்பு false;

  *worst_case = size + 1;
  திரும்பு true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

நிலையான வெறுமை
sort_buffer_policy (FILE *மாறிலி *fps, size_t nfps,
                    எழுத்து *மாறிலி *files, size_t nfiles,
                    size_t வரி_bytes,
                    கட்டமைப்பு sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (வரி_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_உள்ளீடு_byte = வரி_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_உள்ளீடு_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  க்கு (size_t i = 0; i < nfiles; i++)
    {
      கட்டமைப்பு stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      எனில் (size_bound <= size)
        {
          policy->initial = size_bound;
          திரும்பு;
        }

      எனில் ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      எனில் (known_size)
        file_size = st.st_size;
      இல்லையெனில்
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          எனில் (sort_size)
            {
              policy->initial = size_bound;
              திரும்பு;
            }
          file_size = உள்ளீடு_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      எனில் (! உள்ளீடு_size_buffer_bytes (file_size, வரி_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          திரும்பு;
        }

      எனில் (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          திரும்பு;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

நிலையான வெறுமை
initbuf (கட்டமைப்பு buffer *buf, size_t வரி_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  போது (true)
    {
      எனில் (! வரி_aligned_size (&alloc))
        buf->buf = வெற்று;
      இல்லையெனில்
        buf->buf = malloc (alloc);
      எனில் (buf->buf)
        நிறுத்து;
      alloc /= 2;
      எனில் (alloc <= வரி_bytes + 1)
        xalloc_die ();
    }

  buf->வரி_bytes = வரி_bytes;
  buf->alloc = alloc;
  buf->used = buf->இடது = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

நிலையான உள்வரி கட்டமைப்பு வரி *
buffer_linelim (கட்டமைப்பு buffer மாறிலி *buf)
{
  வெறுமை *linelim = buf->buf + buf->alloc;
  திரும்பு linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

நிலையான bool
try_growbuf (கட்டமைப்பு buffer *buf, size_t alloc)
{
  எனில் (! வரி_aligned_size (&alloc) || alloc <= buf->alloc)
    திரும்பு false;

  எழுத்து *newbuf = malloc (alloc);
  எனில் (! newbuf)
    திரும்பு false;

  எழுத்து *oldbuf = buf->buf;
  கட்டமைப்பு வரி *old_linelim = buffer_linelim (buf);
  கட்டமைப்பு வரி *old_வரி = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  கட்டமைப்பு வரி *new_linelim = (வெறுமை *) (newbuf + alloc);
  கட்டமைப்பு வரி *new_வரி = new_linelim - buf->nlines;
  memcpy (new_வரி, old_வரி, buf->nlines * அளவு *new_வரி);

  க்கு (கட்டமைப்பு வரி *வரி = new_வரி; வரி < new_linelim; வரி++)
    {
      வரி->text = newbuf + (வரி->text - oldbuf);
      எனில் (keylist)
        {
          வரி->keybeg = newbuf + (வரி->keybeg - oldbuf);
          வரி->keylim = newbuf + (வரி->keylim - oldbuf);
        }
    }

  விடுவி (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  திரும்பு true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

நிலையான bool
maybe_growbuf (கட்டமைப்பு buffer *buf, கட்டமைப்பு sort_buffer_policy *policy)
{
  எனில் (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    திரும்பு false;

  size_t alloc;
  எனில் (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  இல்லையெனில்
    alloc = policy->limit;

  எனில் (try_growbuf (buf, alloc))
    திரும்பு true;

  policy->growth_failed = true;
  திரும்பு false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

நிலையான எழுத்து *
begfield (கட்டமைப்பு வரி மாறிலி *வரி, கட்டமைப்பு keyfield மாறிலி *சாவி)
{
  எழுத்து *ptr = வரி->text, *lim = ptr + வரி->length - 1;
  size_t sword = சாவி->sword;
  size_t schar = சாவி->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  எனில் (tab != TAB_DEFAULT)
    போது (ptr < lim && sword--)
      {
        எழுத்து *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        எனில் (ptr < lim)
          ++ptr;
      }
  இல்லையெனில்
    போது (ptr < lim && sword--)
      {
        போது (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        போது (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  எனில் (சாவி->skipsblanks)
    போது (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  எனில் (schar < remaining_bytes)
    ptr += schar;
  இல்லையெனில்
    ptr = lim;

  திரும்பு ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
நிலையான எழுத்து *
limfield (கட்டமைப்பு வரி மாறிலி *வரி, கட்டமைப்பு keyfield மாறிலி *சாவி)
{
  எழுத்து *ptr = வரி->text, *lim = ptr + வரி->length - 1;
  size_t eword = சாவி->eword, echar = சாவி->echar;

  எனில் (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  எனில் (tab != TAB_DEFAULT)
    போது (ptr < lim && eword--)
      {
        எழுத்து *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        எனில் (ptr < lim && (eword || echar))
          ++ptr;
      }
  இல்லையெனில்
    போது (ptr < lim && eword--)
      {
        போது (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        போது (ptr < lim && !blanks[to_uchar (*ptr)])
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
  எனில் (tab != TAB_DEFAULT)
    {
      எழுத்து *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      எனில் (newlim)
        lim = newlim;
    }
  இல்லையெனில்
    {
      எழுத்து *newlim;
      newlim = ptr;
      போது (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      போது (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  எனில் (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      எனில் (சாவி->skipeblanks)
        போது (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      எனில் (echar < remaining_bytes)
        ptr += echar;
      இல்லையெனில்
        ptr = lim;
    }

  திரும்பு ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

நிலையான bool
fillbuf (கட்டமைப்பு buffer *buf, FILE *fp, எழுத்து மாறிலி *file,
         கட்டமைப்பு sort_buffer_policy *policy)
{
  கட்டமைப்பு keyfield மாறிலி *சாவி = keylist;
  எழுத்து eol = eolchar;
  size_t வரி_bytes = buf->வரி_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  எனில் (buf->eof)
    திரும்பு false;

  எனில் (buf->used != buf->இடது)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->இடது, buf->இடது);
      buf->used = buf->இடது;
      buf->nlines = 0;
    }

  போது (true)
    {
      எழுத்து *ptr = buf->buf + buf->used;
      கட்டமைப்பு வரி *linelim = buffer_linelim (buf);
      கட்டமைப்பு வரி *வரி = linelim - buf->nlines;
      size_t avail = (எழுத்து *) linelim - buf->nlines * வரி_bytes - ptr;
      எழுத்து *வரி_start = buf->nlines ? வரி->text + வரி->length : buf->buf;

      போது (வரி_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (வரி_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          எழுத்து *ptrlim = ptr + bytes_read;
          எழுத்து *p;
          avail -= bytes_read;

          எனில் (bytes_read != readsize)
            {
              எனில் (ferror (fp))
                sort_die (_("read failed"), file);
              எனில் (feof (fp))
                {
                  buf->eof = true;
                  எனில் (buf->buf == ptrlim)
                    திரும்பு false;
                  எனில் (வரி_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          போது ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              வரி--;
              வரி->text = வரி_start;
              வரி->length = ptr - வரி_start;
              mergesize = MAX (mergesize, வரி->length);
              avail -= வரி_bytes;

              எனில் (சாவி)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  வரி->keylim = (சாவி->eword == SIZE_MAX
                                  ? p
                                  : limfield (வரி, சாவி));

                  எனில் (சாவி->sword != SIZE_MAX)
                    வரி->keybeg = begfield (வரி, சாவி);
                  இல்லையெனில்
                    {
                      எனில் (சாவி->skipsblanks)
                        போது (blanks[to_uchar (*வரி_start)])
                          வரி_start++;
                      வரி->keybeg = வரி_start;
                    }
                }

              வரி_start = ptr;
            }

          ptr = ptrlim;
          எனில் (buf->eof)
            நிறுத்து;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - வரி;
      எனில் (buf->nlines != 0)
        {
          எனில் (! buf->eof && maybe_growbuf (buf, policy))
            தொடர்;
          buf->இடது = ptr - வரி_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          திரும்பு true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t வரி_alloc = buf->alloc / அளவு (கட்டமைப்பு வரி);
        buf->buf = xpalloc (buf->buf, &வரி_alloc, 1, -1, அளவு (கட்டமைப்பு வரி));
        buf->alloc = வரி_alloc * அளவு (கட்டமைப்பு வரி);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
நிலையான எழுத்து மாறிலி unit_order[UCHAR_LIM] =
  {
#எனில் ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#இல்லையெனில்
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
நிலையான எழுத்து
traverse_raw_எண் (எழுத்து மாறிலி **எண்)
{
  எழுத்து மாறிலி *p = *எண்;
  எழுத்து ch;
  எழுத்து max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  போது (c_isdigit (ch = *p++))
    {
      எனில் (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      எனில் (ends_with_thousands_sep)
        ++p;
    }

  எனில் (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *எண் = p - 2;
      திரும்பு max_digit;
    }

  எனில் (ch == decimal_point)
    போது (c_isdigit (ch = *p++))
      எனில் (max_digit < ch)
        max_digit = ch;

  *எண் = p - 1;
  திரும்பு max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
நிலையான முழு
find_unit_order (எழுத்து மாறிலி *எண்)
{
  bool minus_sign = (*எண் == '-');
  எழுத்து மாறிலி *p = எண் + minus_sign;
  எழுத்து max_digit = traverse_raw_எண் (&p);
  எனில் ('0' < max_digit)
    {
      unsigned எழுத்து ch = *p;
      முழு order = unit_order[ch];
      திரும்பு (minus_sign ? -order : order);
    }
  இல்லையெனில்
    திரும்பு 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
நிலையான முழு
human_numcompare (எழுத்து மாறிலி *a, எழுத்து மாறிலி *b)
{
  போது (blanks[to_uchar (*a)])
    a++;
  போது (blanks[to_uchar (*b)])
    b++;

  முழு diff = find_unit_order (a) - find_unit_order (b);
  திரும்பு (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
நிலையான முழு
numcompare (எழுத்து மாறிலி *a, எழுத்து மாறிலி *b)
{
  போது (blanks[to_uchar (*a)])
    a++;
  போது (blanks[to_uchar (*b)])
    b++;

  திரும்பு strnumcmp (a, b, decimal_point, thousands_sep);
}

நிலையான முழு
nan_compare (long இரட்டை a, long இரட்டை b)
{
  எழுத்து buf[2][அளவு "-nan""()" + CHAR_BIT * அளவு a];
  snprintf (buf[0], அளவு buf[0], "%Lf", a);
  snprintf (buf[1], அளவு buf[1], "%Lf", b);
  திரும்பு strcmp (buf[0], buf[1]);
}

நிலையான முழு
general_numcompare (எழுத்து மாறிலி *sa, எழுத்து மாறிலி *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  எழுத்து *ea;
  எழுத்து *eb;
  long இரட்டை a = strtold (sa, &ea);
  long இரட்டை b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  எனில் (sa == ea)
    திரும்பு sb == eb ? 0 : -1;
  எனில் (sb == eb)
    திரும்பு 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  திரும்பு (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

நிலையான முழு
getmonth (எழுத்து மாறிலி *month, எழுத்து **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  போது (blanks[to_uchar (*month)])
    month++;

  செய்
    {
      size_t ix = (lo + hi) / 2;
      எழுத்து மாறிலி *m = month;
      எழுத்து மாறிலி *n = monthtab[ix].name;

      க்கு (;; m++, n++)
        {
          எனில் (!*n)
            {
              எனில் (ea)
                *ea = (எழுத்து *) m;
              திரும்பு monthtab[ix].val;
            }
          எனில் (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              நிறுத்து;
            }
          இல்லையெனில் எனில் (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              நிறுத்து;
            }
        }
    }
  போது (lo < hi);

  திரும்பு 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#எனில் DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# எனில் 14 <= __GNUC__
#  pragma GCC diagnostic தள்ளு
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#அந்தர்பாவ "md5.h"

#எனில் DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# எனில் 14 <= __GNUC__
#  pragma GCC diagnostic எடு
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
நிலையான வெறுமை
link_failure (வெறுமை)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
நிலையான வெறுமை *
குறியீடு_address (வெறுமை *handle, எழுத்து மாறிலி *குறியீடு)
{
  வெறுமை *address = dlsym (handle, குறியீடு);
  எனில் (!address)
    link_failure ();
  திரும்பு address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
நிலையான வெறுமை
link_libcrypto (வெறுமை)
{
#எனில் DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  வெறுமை *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  எனில் (!handle)
    link_failure ();
  ptr_MD5_Init = குறியீடு_address (handle, "MD5_Init");
  ptr_MD5_Update = குறியீடு_address (handle, "MD5_Update");
  ptr_MD5_Final = குறியீடு_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
நிலையான கட்டமைப்பு md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

நிலையான வெறுமை
random_md5_state_init (எழுத்து மாறிலி *random_source)
{
  unsigned எழுத்து buf[MD5_DIGEST_SIZE];
  கட்டமைப்பு randread_source *r = randread_new (random_source, அளவு buf);
  எனில் (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, அளவு buf);
  எனில் (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, அளவு buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

நிலையான size_t
xstrxfrm (எழுத்து *restrict dest, எழுத்து மாறிலி *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  எனில் (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  திரும்பு translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

நிலையான முழு
compare_random (எழுத்து *restrict texta, size_t lena,
                எழுத்து *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  முழு xfrm_diff = 0;

  எழுத்து stackbuf[4000];
  எழுத்து *buf = stackbuf;
  size_t bufsize = அளவு stackbuf;
  வெறுமை *allocated = வெற்று;
  uint32_t dig[2][MD5_DIGEST_SIZE / அளவு (uint32_t)];
  கட்டமைப்பு md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  எனில் (hard_LC_COLLATE)
    {
      எழுத்து மாறிலி *lima = texta + lena;
      எழுத்து மாறிலி *limb = textb + lenb;

      போது (true)
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
          எனில் (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              விடுவி (allocated);
              buf = allocated = malloc (bufsize);
              எனில் (! buf)
                {
                  buf = stackbuf;
                  bufsize = அளவு stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : வெற்று), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          எனில் (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              எனில் (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              விடுவி (allocated);
              buf = allocated = xmalloc (bufsize);
              எனில் (texta < lima)
                strxfrm (buf, texta, sizea);
              எனில் (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          எனில் (texta < lima)
            texta += strlen (texta) + 1;
          எனில் (textb < limb)
            textb += strlen (textb) + 1;
          எனில் (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              நிறுத்து;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          எனில் (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              எனில் (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  முழு diff = memcmp (dig[0], dig[1], அளவு dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  எனில் (! diff)
    {
      எனில் (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          எனில் (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  விடுவி (allocated);

  திரும்பு diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

நிலையான size_t
debug_width (எழுத்து மாறிலி *text, எழுத்து மாறிலி *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  போது (text < lim)
    width += (*text++ == '\t');
  திரும்பு width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

நிலையான வெறுமை
mark_சாவி (size_t offset, size_t width)
{
  போது (offset--)
    putchar (' ');

  எனில் (!width)
    printf (_("^ no match for key\n"));
  இல்லையெனில்
    {
      செய்
        putchar ('_');
      போது (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

நிலையான உள்வரி bool
சாவி_numeric (கட்டமைப்பு keyfield மாறிலி *சாவி)
{
  திரும்பு சாவி->numeric || சாவி->general_numeric || சாவி->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

நிலையான வெறுமை
debug_சாவி (கட்டமைப்பு வரி மாறிலி *வரி, கட்டமைப்பு keyfield மாறிலி *சாவி)
{
  எழுத்து *text = வரி->text;
  எழுத்து *beg = text;
  எழுத்து *lim = text + வரி->length - 1;

  எனில் (சாவி)
    {
      எனில் (சாவி->sword != SIZE_MAX)
        beg = begfield (வரி, சாவி);
      எனில் (சாவி->eword != SIZE_MAX)
        {
          lim = limfield (வரி, சாவி);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      எனில் ((சாவி->skipsblanks && சாவி->sword == SIZE_MAX)
          || சாவி->month || சாவி_numeric (சாவி))
        {
          எழுத்து saved = *lim;
          *lim = '\0';

          போது (blanks[to_uchar (*beg)])
            beg++;

          எழுத்து *tighter_lim = beg;

          எனில் (lim < beg)
            tighter_lim = lim;
          இல்லையெனில் எனில் (சாவி->month)
            getmonth (beg, &tighter_lim);
          இல்லையெனில் எனில் (சாவி->general_numeric)
            ignore_மதிப்பு (strtold (beg, &tighter_lim));
          இல்லையெனில் எனில் (சாவி->numeric || சாவி->human_numeric)
            {
              எழுத்து மாறிலி *p = beg + (beg < lim && *beg == '-');
              எழுத்து max_digit = traverse_raw_எண் (&p);
              எனில் ('0' <= max_digit)
                {
                  unsigned எழுத்து ch = *p;
                  tighter_lim = (எழுத்து *) p
                    + (சாவி->human_numeric && unit_order[ch]);
                }
            }
          இல்லையெனில்
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_சாவி (offset, width);
}

/* Debug LINE by underlining its keys.  */

நிலையான வெறுமை
debug_வரி (கட்டமைப்பு வரி மாறிலி *வரி)
{
  கட்டமைப்பு keyfield மாறிலி *சாவி = keylist;

  செய்
    debug_சாவி (வரி, சாவி);
  போது (சாவி && ((சாவி = சாவி->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

நிலையான bool
default_சாவி_compare (கட்டமைப்பு keyfield மாறிலி *சாவி)
{
  திரும்பு ! (சாவி->ignore
            || சாவி->translate
            || சாவி->skipsblanks
            || சாவி->skipeblanks
            || சாவி_numeric (சாவி)
            || சாவி->month
            || சாவி->version
            || சாவி->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

நிலையான வெறுமை
சாவி_to_opts (கட்டமைப்பு keyfield மாறிலி *சாவி, எழுத்து *opts)
{
  எனில் (சாவி->skipsblanks || சாவி->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  எனில் (சாவி->ignore == nondictionary)
    *opts++ = 'd';
  எனில் (சாவி->translate)
    *opts++ = 'f';
  எனில் (சாவி->general_numeric)
    *opts++ = 'g';
  எனில் (சாவி->human_numeric)
    *opts++ = 'h';
  எனில் (சாவி->ignore == nonprinting)
    *opts++ = 'i';
  எனில் (சாவி->month)
    *opts++ = 'M';
  எனில் (சாவி->numeric)
    *opts++ = 'n';
  எனில் (சாவி->random)
    *opts++ = 'R';
  எனில் (சாவி->reverse)
    *opts++ = 'r';
  எனில் (சாவி->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

நிலையான வெறுமை
சாவி_warnings (கட்டமைப்பு keyfield மாறிலி *gkey, bool gkey_only)
{
  கட்டமைப்பு keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  க்கு (கட்டமைப்பு keyfield *சாவி = keylist; சாவி; சாவி = சாவி->next, keynum++)
    {
      எனில் (சாவி_numeric (சாவி))
        {
          எனில் (சாவி->general_numeric)
            general_numeric_field = true;
          இல்லையெனில்
            basic_numeric_field = true;
        }

      எனில் (சாவி->traditional_used)
        {
          size_t sword = சாவி->sword;
          size_t eword = சாவி->eword;
          எழுத்து tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          எழுத்து obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          எழுத்து nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          எழுத்து *po = obuf;
          எழுத்து *pn = nbuf;

          எனில் (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          எனில் (சாவி->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (சாவி->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = சாவி->sword != SIZE_MAX && சாவி->eword < சாவி->sword;
      எனில் (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = சாவி_numeric (சாவி) || சாவி->month;
      bool வரி_offset = சாவி->eword == 0 && சாவி->echar != 0; /* -k1.x,1.y  */
      எனில் (!zero_width && !gkey_only && tab == TAB_DEFAULT && !வரி_offset
          && ((!சாவி->skipsblanks && !implicit_skip)
              || (!சாவி->skipsblanks && சாவி->schar)
              || (!சாவி->skipeblanks && சாவி->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      எனில் (!gkey_only && சாவி_numeric (சாவி))
        {
          size_t sword = சாவி->sword + 1;
          size_t eword = சாவி->eword + 1;
          எனில் (!sword)
            sword++;
          எனில் (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              எனில் (சாவி->general_numeric)
                general_numeric_field_span = true;
              இல்லையெனில்
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      எனில் (ugkey.ignore && (ugkey.ignore == சாவி->ignore))
        ugkey.ignore = வெற்று;
      எனில் (ugkey.translate && (ugkey.translate == சாவி->translate))
        ugkey.translate = வெற்று;
      ugkey.skipsblanks &= !சாவி->skipsblanks;
      ugkey.skipeblanks &= !சாவி->skipeblanks;
      ugkey.month &= !சாவி->month;
      ugkey.numeric &= !சாவி->numeric;
      ugkey.general_numeric &= !சாவி->general_numeric;
      ugkey.human_numeric &= !சாவி->human_numeric;
      ugkey.random &= !சாவி->random;
      ugkey.version &= !சாவி->version;
      ugkey.reverse &= !சாவி->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool எண்_locale_warned = false;
  எனில் (basic_numeric_field_span)
    {
      எனில் (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((எழுத்து []) {thousands_sep, 0})));
          எண்_locale_warned = true;
        }
    }
  எனில் (basic_numeric_field_span || general_numeric_field_span)
    {
      எனில் (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((எழுத்து []) {decimal_point, 0})));
          எண்_locale_warned = true;
        }
      இல்லையெனில் எனில் (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((எழுத்து []) {tab, 0})));
        }
      இல்லையெனில் எனில் (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((எழுத்து []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  எனில் ((basic_numeric_field || general_numeric_field) && ! எண்_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((எழுத்து []) {decimal_point, 0})));

    }

  எனில் (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  எனில் (!default_சாவி_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      எனில் (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      எழுத்து opts[அளவு short_options];
      சாவி_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  எனில் (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

நிலையான முழு
diff_reversed (முழு diff, bool reversed)
{
  திரும்பு reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

நிலையான முழு
keycompare (கட்டமைப்பு வரி மாறிலி *a, கட்டமைப்பு வரி மாறிலி *b)
{
  கட்டமைப்பு keyfield *சாவி = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  எழுத்து *texta = a->keybeg;
  எழுத்து *textb = b->keybeg;
  எழுத்து *lima = a->keylim;
  எழுத்து *limb = b->keylim;

  முழு diff;

  போது (true)
    {
      எழுத்து மாறிலி *translate = சாவி->translate;
      bool மாறிலி *ignore = சாவி->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      எனில் (hard_LC_COLLATE || சாவி_numeric (சாவி)
          || சாவி->month || சாவி->random || சாவி->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          எழுத்து *ta = texta;
          எழுத்து *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          எழுத்து enda = ta[tlena];
          எழுத்து endb = tb[tlenb];

          வெறுமை *allocated = வெற்று;
          எழுத்து stackbuf[4000];

          எனில் (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              எனில் (size <= அளவு stackbuf)
                ta = stackbuf;
              இல்லையெனில்
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              க்கு (tlena = i = 0; i < lena; i++)
                எனில் (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              க்கு (tlenb = i = 0; i < lenb; i++)
                எனில் (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          எனில் (சாவி->numeric)
            diff = numcompare (ta, tb);
          இல்லையெனில் எனில் (சாவி->general_numeric)
            diff = general_numcompare (ta, tb);
          இல்லையெனில் எனில் (சாவி->human_numeric)
            diff = human_numcompare (ta, tb);
          இல்லையெனில் எனில் (சாவி->month)
            diff = getmonth (ta, வெற்று) - getmonth (tb, வெற்று);
          இல்லையெனில் எனில் (சாவி->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          இல்லையெனில் எனில் (சாவி->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          இல்லையெனில்
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              எனில் (tlena == 0)
                diff = - NONZERO (tlenb);
              இல்லையெனில் எனில் (tlenb == 0)
                diff = 1;
              இல்லையெனில்
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          விடுவி (allocated);
        }
      இல்லையெனில் எனில் (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  செய்									\
    {									\
          போது (true)							\
            {								\
              போது (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              போது (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              எனில் (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  நிறுத்து;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              எனில் (diff)							\
                நிறுத்து;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  போது (0)

          எனில் (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          இல்லையெனில்
            CMP_WITH_IGNORE (*texta, *textb);
        }
      இல்லையெனில்
        {
          size_t lenmin = MIN (lena, lenb);
          எனில் (lenmin == 0)
            diff = 0;
          இல்லையெனில் எனில் (translate)
            {
              size_t i = 0;
              செய்
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  எனில் (diff)
                    நிறுத்து;
                  i++;
                }
              போது (i < lenmin);
            }
          இல்லையெனில்
            diff = memcmp (texta, textb, lenmin);

          எனில் (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      எனில் (diff)
        நிறுத்து;

      சாவி = சாவி->next;
      எனில் (! சாவி)
        திரும்பு 0;

      /* Find the beginning and limit of the next field.  */
      எனில் (சாவி->eword != SIZE_MAX)
        lima = limfield (a, சாவி), limb = limfield (b, சாவி);
      இல்லையெனில்
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      எனில் (சாவி->sword != SIZE_MAX)
        texta = begfield (a, சாவி), textb = begfield (b, சாவி);
      இல்லையெனில்
        {
          texta = a->text, textb = b->text;
          எனில் (சாவி->skipsblanks)
            {
              போது (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              போது (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  திரும்பு diff_reversed (diff, சாவி->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

நிலையான முழு
compare (கட்டமைப்பு வரி மாறிலி *a, கட்டமைப்பு வரி மாறிலி *b)
{
  முழு diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  எனில் (keylist)
    {
      diff = keycompare (a, b);
      எனில் (diff || unique || stable)
        திரும்பு diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  எனில் (alen == 0)
    diff = - NONZERO (blen);
  இல்லையெனில் எனில் (blen == 0)
    diff = 1;
  இல்லையெனில் எனில் (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  இல்லையெனில்
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      எனில் (!diff)
        diff = _GL_CMP (alen, blen);
    }

  திரும்பு diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

நிலையான வெறுமை
write_வரி (கட்டமைப்பு வரி மாறிலி *வரி, FILE *fp, எழுத்து மாறிலி *output_file)
{
  எழுத்து *buf = வரி->text;
  size_t n_bytes = வரி->length;
  எழுத்து *ebuf = buf + n_bytes;

  எனில் (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      எழுத்து மாறிலி *c = buf;

      போது (c < ebuf)
        {
          எழுத்து wc = *c++;
          எனில் (wc == '\t')
            wc = '>';
          இல்லையெனில் எனில் (c == ebuf)
            wc = '\n';
          எனில் (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_வரி (வரி);
    }
  இல்லையெனில்
    {
      ebuf[-1] = eolchar;
      எனில் (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

நிலையான bool
check (எழுத்து மாறிலி *file_name, எழுத்து checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  கட்டமைப்பு buffer buf;		/* Input buffer. */
  கட்டமைப்பு வரி temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t வரி_எண் = 0;
  கட்டமைப்பு keyfield மாறிலி *சாவி = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, அளவு (கட்டமைப்பு வரி),
           MAX (merge_buffer_size, sort_size));
  temp.text = வெற்று;

  போது (fillbuf (&buf, fp, file_name, வெற்று))
    {
      கட்டமைப்பு வரி மாறிலி *வரி = buffer_linelim (&buf);
      கட்டமைப்பு வரி மாறிலி *linebase = வரி - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      எனில் (alloc && nonunique <= compare (&temp, வரி - 1))
        {
        found_disorder:
          {
            எனில் (checkonly == 'c')
              {
                கட்டமைப்பு வரி மாறிலி *disorder_வரி = வரி - 1;
                uintmax_t disorder_வரி_எண் =
                  buffer_linelim (&buf) - disorder_வரி + வரி_எண்;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_வரி_எண்);
                write_வரி (disorder_வரி, stderr, _("standard error"));
              }

            ordered = false;
            நிறுத்து;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      போது (linebase < --வரி)
        எனில் (nonunique <= compare (வரி, வரி - 1))
          செல் found_disorder;

      வரி_எண் += buf.nlines;

      /* Save the last line of the buffer.  */
      எனில் (alloc < வரி->length)
        {
          செய்
            {
              alloc *= 2;
              எனில் (! alloc)
                {
                  alloc = வரி->length;
                  நிறுத்து;
                }
            }
          போது (alloc < வரி->length);

          விடுவி (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, வரி->text, வரி->length);
      temp.length = வரி->length;
      எனில் (சாவி)
        {
          temp.keybeg = temp.text + (வரி->keybeg - வரி->text);
          temp.keylim = temp.text + (வரி->keylim - வரி->text);
        }
    }

  xfclose (fp, file_name);
  விடுவி (buf.buf);
  விடுவி (temp.text);
  திரும்பு ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

நிலையான size_t
open_உள்ளீடு_files (கட்டமைப்பு sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, அளவு *fps);
  முழு i;

  /* Open as many input files as we can.  */
  க்கு (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      எனில் (!fps[i])
        நிறுத்து;
    }

  திரும்பு i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

நிலையான வெறுமை
mergefps (கட்டமைப்பு sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, எழுத்து மாறிலி *output_file, FILE **fps)
{
  கட்டமைப்பு buffer *buffer = xnmalloc (nfiles, அளவு *buffer);
                                /* Input buffers for each file. */
  கட்டமைப்பு வரி saved;		/* Saved line storage for unique check. */
  கட்டமைப்பு வரி மாறிலி *savedline = வெற்று;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  கட்டமைப்பு வரி மாறிலி **cur = xnmalloc (nfiles, அளவு *cur);
                                /* Current line in each line table. */
  கட்டமைப்பு வரி மாறிலி **base = xnmalloc (nfiles, அளவு *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, அளவு *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  கட்டமைப்பு keyfield மாறிலி *சாவி = keylist;
  saved.text = வெற்று;

  /* Read initial lines from each input file. */
  க்கு (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], அளவு (கட்டமைப்பு வரி),
               MAX (merge_buffer_size, sort_size / nfiles));
      எனில் (fillbuf (&buffer[i], fps[i], files[i].name, வெற்று))
        {
          கட்டமைப்பு வரி மாறிலி *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      இல்லையெனில்
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          எனில் (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          விடுவி (buffer[i].buf);
          --nfiles;
          க்கு (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  க்கு (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  க்கு (size_t i = 1; i < nfiles; ++i)
    எனில் (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  போது (nfiles)
    {
      கட்டமைப்பு வரி மாறிலி *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      எனில் (unique)
        {
          எனில் (savedline && compare (savedline, smallest))
            {
              savedline = வெற்று;
              write_வரி (&saved, ofp, output_file);
            }
          எனில் (!savedline)
            {
              savedline = &saved;
              எனில் (savealloc < smallest->length)
                {
                  செய்
                    எனில் (! savealloc)
                      {
                        savealloc = smallest->length;
                        நிறுத்து;
                      }
                  போது ((savealloc *= 2) < smallest->length);

                  விடுவி (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              எனில் (சாவி)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      இல்லையெனில்
        write_வரி (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      எனில் (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      இல்லையெனில்
        {
          எனில் (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       வெற்று))
            {
              கட்டமைப்பு வரி மாறிலி *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          இல்லையெனில்
            {
              /* We reached EOF on fps[ord[0]].  */
              க்கு (size_t i = 1; i < nfiles; ++i)
                எனில் (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              எனில் (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              விடுவி (buffer[ord[0]].buf);
              க்கு (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              க்கு (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              தொடர்;
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
        size_t மொத்தம்_of_smaller_வரிகள்;

        போது (lo < hi)
          {
            முழு cmp = compare (cur[ord0], cur[ord[probe]]);
            எனில் (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            இல்லையெனில்
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        மொத்தம்_of_smaller_வரிகள் = lo - 1;
        க்கு (size_t j = 0; j < மொத்தம்_of_smaller_வரிகள்; j++)
          ord[j] = ord[j + 1];
        ord[மொத்தம்_of_smaller_வரிகள்] = ord0;
      }
    }

  எனில் (unique && savedline)
    {
      write_வரி (&saved, ofp, output_file);
      விடுவி (saved.text);
    }

  xfclose (ofp, output_file);
  விடுவி (fps);
  விடுவி (buffer);
  விடுவி (ord);
  விடுவி (base);
  விடுவி (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

நிலையான size_t
mergefiles (கட்டமைப்பு sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, எழுத்து மாறிலி *output_file)
{
  FILE **fps;
  size_t nopened = open_உள்ளீடு_files (files, nfiles, &fps);
  எனில் (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  திரும்பு nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

நிலையான வெறுமை
mergelines (கட்டமைப்பு வரி *restrict t, size_t nlines,
            கட்டமைப்பு வரி மாறிலி *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  கட்டமைப்பு வரி *hi = t - nlo;

  போது (true)
    எனில் (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        எனில் (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            திரும்பு;
          }
      }
    இல்லையெனில்
      {
        *--t = *--hi;
        எனில் (! --nhi)
          {
            செய்
              *--t = *--lo;
            போது (--nlo);

            திரும்பு;
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

நிலையான வெறுமை
sequential_sort (கட்டமைப்பு வரி *restrict வரிகள், size_t nlines,
                 கட்டமைப்பு வரி *restrict temp, bool to_temp)
{
  எனில் (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      முழு swap = (0 < compare (&வரிகள்[-1], &வரிகள்[-2]));
      எனில் (to_temp)
        {
          temp[-1] = வரிகள்[-1 - swap];
          temp[-2] = வரிகள்[-2 + swap];
        }
      இல்லையெனில் எனில் (swap)
        {
          temp[-1] = வரிகள்[-1];
          வரிகள்[-1] = வரிகள்[-2];
          வரிகள்[-2] = temp[-1];
        }
    }
  இல்லையெனில்
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      கட்டமைப்பு வரி *lo = வரிகள்;
      கட்டமைப்பு வரி *hi = வரிகள் - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      எனில் (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      இல்லையெனில் எனில் (!to_temp)
        temp[-1] = lo[-1];

      கட்டமைப்பு வரி *dest;
      கட்டமைப்பு வரி மாறிலி *sorted_lo;
      எனில் (to_temp)
        {
          dest = temp;
          sorted_lo = வரிகள்;
        }
      இல்லையெனில்
        {
          dest = வரிகள்;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

நிலையான கட்டமைப்பு merge_முனை *init_முனை (கட்டமைப்பு merge_முனை *restrict,
                                     கட்டமைப்பு merge_முனை *restrict,
                                     கட்டமைப்பு வரி *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
நிலையான கட்டமைப்பு merge_முனை *
merge_மரம்_init (size_t nthreads, size_t nlines, கட்டமைப்பு வரி *dest)
{
  கட்டமைப்பு merge_முனை *merge_மரம் = xmalloc (2 * அளவு *merge_மரம் * nthreads);

  கட்டமைப்பு merge_முனை *வேர் = merge_மரம்;
  வேர்->lo = வேர்->hi = வேர்->end_lo = வேர்->end_hi = வெற்று;
  வேர்->dest = வெற்று;
  வேர்->nlo = வேர்->nhi = nlines;
  வேர்->parent = வெற்று;
  வேர்->level = MERGE_END;
  வேர்->queued = false;
  pthread_mutex_init (&வேர்->lock, வெற்று);

  init_முனை (வேர், வேர் + 1, dest, nthreads, nlines, false);
  திரும்பு merge_மரம்;
}

/* Destroy the merge tree. */
நிலையான வெறுமை
merge_மரம்_destroy (size_t nthreads, கட்டமைப்பு merge_முனை *merge_மரம்)
{
  size_t n_nodes = nthreads * 2;
  கட்டமைப்பு merge_முனை *முனை = merge_மரம்;

  போது (n_nodes--)
    {
      pthread_mutex_destroy (&முனை->lock);
      முனை++;
    }

  விடுவி (merge_மரம்);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

நிலையான கட்டமைப்பு merge_முனை *
init_முனை (கட்டமைப்பு merge_முனை *restrict parent,
           கட்டமைப்பு merge_முனை *restrict முனை_pool,
           கட்டமைப்பு வரி *dest, size_t nthreads,
           size_t total_வரிகள், bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  கட்டமைப்பு வரி *lo = dest - total_வரிகள்;
  கட்டமைப்பு வரி *hi = lo - nlo;
  கட்டமைப்பு வரி **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  கட்டமைப்பு merge_முனை *முனை = முனை_pool++;
  முனை->lo = முனை->end_lo = lo;
  முனை->hi = முனை->end_hi = hi;
  முனை->dest = parent_end;
  முனை->nlo = nlo;
  முனை->nhi = nhi;
  முனை->parent = parent;
  முனை->level = parent->level + 1;
  முனை->queued = false;
  pthread_mutex_init (&முனை->lock, வெற்று);

  எனில் (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      முனை->lo_child = முனை_pool;
      முனை_pool = init_முனை (முனை, முனை_pool, lo, lo_threads,
                             total_வரிகள், true);
      முனை->hi_child = முனை_pool;
      முனை_pool = init_முனை (முனை, முனை_pool, hi, hi_threads,
                             total_வரிகள், false);
    }
  இல்லையெனில்
    {
      முனை->lo_child = வெற்று;
      முனை->hi_child = வெற்று;
    }
  திரும்பு முனை_pool;
}


/* Compare two merge nodes A and B for priority.  */

நிலையான முழு
compare_nodes (வெறுமை மாறிலி *a, வெறுமை மாறிலி *b)
{
  கட்டமைப்பு merge_முனை மாறிலி *nodea = a;
  கட்டமைப்பு merge_முனை மாறிலி *nodeb = b;
  எனில் (nodea->level == nodeb->level)
      திரும்பு (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  திரும்பு nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

நிலையான உள்வரி வெறுமை
lock_முனை (கட்டமைப்பு merge_முனை *முனை)
{
  pthread_mutex_lock (&முனை->lock);
}

/* Unlock a merge tree NODE. */

நிலையான உள்வரி வெறுமை
unlock_முனை (கட்டமைப்பு merge_முனை *முனை)
{
  pthread_mutex_unlock (&முனை->lock);
}

/* Destroy merge QUEUE. */

நிலையான வெறுமை
queue_destroy (கட்டமைப்பு merge_முனை_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

நிலையான வெறுமை
queue_init (கட்டமைப்பு merge_முனை_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, வெற்று);
  pthread_cond_init (&queue->cond, வெற்று);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

நிலையான வெறுமை
queue_செருகு (கட்டமைப்பு merge_முனை_queue *queue, கட்டமைப்பு merge_முனை *முனை)
{
  pthread_mutex_lock (&queue->mutex);
  heap_செருகு (queue->priority_queue, முனை);
  முனை->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

நிலையான கட்டமைப்பு merge_முனை *
queue_எடு (கட்டமைப்பு merge_முனை_queue *queue)
{
  கட்டமைப்பு merge_முனை *முனை;
  pthread_mutex_lock (&queue->mutex);
  போது (! (முனை = heap_remove_உச்சி (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_முனை (முனை);
  முனை->queued = false;
  திரும்பு முனை;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

நிலையான வெறுமை
write_unique (கட்டமைப்பு வரி மாறிலி *வரி, FILE *tfp, எழுத்து மாறிலி *temp_output)
{
  எனில் (unique)
    {
      எனில் (saved_வரி.text && ! compare (வரி, &saved_வரி))
        திரும்பு;
      saved_வரி = *வரி;
    }

  write_வரி (வரி, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

நிலையான வெறுமை
mergelines_முனை (கட்டமைப்பு merge_முனை *restrict முனை, size_t total_வரிகள்,
                 FILE *tfp, எழுத்து மாறிலி *temp_output)
{
  கட்டமைப்பு வரி *lo_orig = முனை->lo;
  கட்டமைப்பு வரி *hi_orig = முனை->hi;
  size_t to_merge = MAX_MERGE (total_வரிகள், முனை->level);
  size_t merged_lo;
  size_t merged_hi;

  எனில் (முனை->level > MERGE_வேர்)
    {
      /* Merge to destination buffer. */
      கட்டமைப்பு வரி *dest = *முனை->dest;
      போது (முனை->lo != முனை->end_lo && முனை->hi != முனை->end_hi && to_merge--)
        எனில் (compare (முனை->lo - 1, முனை->hi - 1) <= 0)
          *--dest = *--முனை->lo;
        இல்லையெனில்
          *--dest = *--முனை->hi;

      merged_lo = lo_orig - முனை->lo;
      merged_hi = hi_orig - முனை->hi;

      எனில் (முனை->nhi == merged_hi)
        போது (முனை->lo != முனை->end_lo && to_merge--)
          *--dest = *--முனை->lo;
      இல்லையெனில் எனில் (முனை->nlo == merged_lo)
        போது (முனை->hi != முனை->end_hi && to_merge--)
          *--dest = *--முனை->hi;
      *முனை->dest = dest;
    }
  இல்லையெனில்
    {
      /* Merge directly to output. */
      போது (முனை->lo != முனை->end_lo && முனை->hi != முனை->end_hi && to_merge--)
        {
          எனில் (compare (முனை->lo - 1, முனை->hi - 1) <= 0)
            write_unique (--முனை->lo, tfp, temp_output);
          இல்லையெனில்
            write_unique (--முனை->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - முனை->lo;
      merged_hi = hi_orig - முனை->hi;

      எனில் (முனை->nhi == merged_hi)
        {
          போது (முனை->lo != முனை->end_lo && to_merge--)
            write_unique (--முனை->lo, tfp, temp_output);
        }
      இல்லையெனில் எனில் (முனை->nlo == merged_lo)
        {
          போது (முனை->hi != முனை->end_hi && to_merge--)
            write_unique (--முனை->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - முனை->lo;
  merged_hi = hi_orig - முனை->hi;
  முனை->nlo -= merged_lo;
  முனை->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

நிலையான வெறுமை
queue_check_செருகு (கட்டமைப்பு merge_முனை_queue *queue, கட்டமைப்பு merge_முனை *முனை)
{
  எனில் (! முனை->queued)
    {
      bool lo_avail = (முனை->lo - முனை->end_lo) != 0;
      bool hi_avail = (முனை->hi - முனை->end_hi) != 0;
      எனில் (lo_avail ? hi_avail || ! முனை->nhi : hi_avail && ! முனை->nlo)
        queue_செருகு (queue, முனை);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

நிலையான வெறுமை
queue_check_செருகு_parent (கட்டமைப்பு merge_முனை_queue *queue,
                           கட்டமைப்பு merge_முனை *முனை)
{
  எனில் (முனை->level > MERGE_வேர்)
    {
      lock_முனை (முனை->parent);
      queue_check_செருகு (queue, முனை->parent);
      unlock_முனை (முனை->parent);
    }
  இல்லையெனில் எனில் (முனை->nlo + முனை->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_செருகு (queue, முனை->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

நிலையான வெறுமை
merge_loop (கட்டமைப்பு merge_முனை_queue *queue,
            size_t total_வரிகள், FILE *tfp, எழுத்து மாறிலி *temp_output)
{
  போது (true)
    {
      கட்டமைப்பு merge_முனை *முனை = queue_எடு (queue);

      எனில் (முனை->level == MERGE_END)
        {
          unlock_முனை (முனை);
          /* Reinsert so other threads can pop it. */
          queue_செருகு (queue, முனை);
          நிறுத்து;
        }
      mergelines_முனை (முனை, total_வரிகள், tfp, temp_output);
      queue_check_செருகு (queue, முனை);
      queue_check_செருகு_parent (queue, முனை);

      unlock_முனை (முனை);
    }
}


நிலையான வெறுமை sortlines (கட்டமைப்பு வரி *restrict, size_t, size_t,
                       கட்டமைப்பு merge_முனை *, கட்டமைப்பு merge_முனை_queue *,
                       FILE *, எழுத்து மாறிலி *);

/* Thread arguments for sortlines_thread. */

கட்டமைப்பு thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  கட்டமைப்பு வரி *வரிகள்;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t மாறிலி total_வரிகள்;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  கட்டமைப்பு merge_முனை *மாறிலி முனை;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  கட்டமைப்பு merge_முனை_queue *மாறிலி queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  எழுத்து மாறிலி *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

நிலையான வெறுமை *
sortlines_thread (வெறுமை *data)
{
  கட்டமைப்பு thread_args மாறிலி *args = data;
  sortlines (args->வரிகள், args->nthreads, args->total_வரிகள்,
             args->முனை, args->queue, args->tfp,
             args->output_temp);
  திரும்பு வெற்று;
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

நிலையான வெறுமை
sortlines (கட்டமைப்பு வரி *restrict வரிகள், size_t nthreads,
           size_t total_வரிகள், கட்டமைப்பு merge_முனை *முனை,
           கட்டமைப்பு merge_முனை_queue *queue, FILE *tfp, எழுத்து மாறிலி *temp_output)
{
  size_t nlines = முனை->nlo + முனை->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  கட்டமைப்பு thread_args args = {வரிகள், lo_threads, total_வரிகள்,
                             முனை->lo_child, queue, tfp, temp_output};

  எனில் (nthreads > 1 && SUBTHREAD_வரிகள்_HEURISTIC <= nlines
      && pthread_create (&thread, வெற்று, sortlines_thread, &args) == 0)
    {
      sortlines (வரிகள் - முனை->nlo, hi_threads, total_வரிகள்,
                 முனை->hi_child, queue, tfp, temp_output);
      pthread_join (thread, வெற்று);
    }
  இல்லையெனில்
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = முனை->nlo;
      size_t nhi = முனை->nhi;
      கட்டமைப்பு வரி *temp = வரிகள் - total_வரிகள்;
      எனில் (1 < nhi)
        sequential_sort (வரிகள் - nlo, nhi, temp - nlo / 2, false);
      எனில் (1 < nlo)
        sequential_sort (வரிகள், nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      முனை->lo = வரிகள்;
      முனை->hi = வரிகள் - nlo;
      முனை->end_lo = வரிகள் - nlo;
      முனை->end_hi = வரிகள் - nlo - nhi;

      queue_செருகு (queue, முனை);
      merge_loop (queue, total_வரிகள், tfp, temp_output);
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

நிலையான வெறுமை
avoid_trashing_உள்ளீடு (கட்டமைப்பு sortfile *files, size_t ntemps,
                      size_t nfiles, எழுத்து மாறிலி *outfile)
{
  கட்டமைப்பு tempnode *tempcopy = வெற்று;

  க்கு (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      கட்டமைப்பு stat instat;

      எனில் (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      இல்லையெனில்
        {
          கட்டமைப்பு stat *outst = get_outstatus ();
          எனில் (!outst)
            நிறுத்து;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      எனில் (same)
        {
          எனில் (! tempcopy)
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

நிலையான வெறுமை
check_inputs (எழுத்து *மாறிலி *files, size_t nfiles)
{
  க்கு (size_t i = 0; i < nfiles; i++)
    {
      எனில் (streq (files[i], "-"))
        தொடர்;

      எனில் (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

நிலையான வெறுமை
check_output (எழுத்து மாறிலி *outfile)
{
  எனில் (outfile)
    {
      முழு oflags = O_WRONLY | O_இருமம் | O_CLOEXEC | O_CREAT;
      முழு outfd = open (outfile, oflags, MODE_RW_UGO);
      எனில் (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

நிலையான வெறுமை
merge (கட்டமைப்பு sortfile *files, size_t ntemps, size_t nfiles,
       எழுத்து மாறிலி *output_file)
{
  போது (nmerge < nfiles)
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
      க்கு (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          கட்டமைப்பு tempnode *temp = create_temp (&tfp);
          size_t இலக்கம்_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, இலக்கம்_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += இலக்கம்_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      எனில் (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          கட்டமைப்பு tempnode *temp = create_temp (&tfp);
          size_t இலக்கம்_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, இலக்கம்_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += இலக்கம்_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * அளவு *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_உள்ளீடு (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  போது (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_உள்ளீடு_files (files, nfiles, &fps);

      எனில் (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          எனில் (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              நிறுத்து;
            }
          எனில் (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      இல்லையெனில் எனில் (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      கட்டமைப்பு tempnode *temp;
      செய்
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      போது (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * அளவு *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

நிலையான வெறுமை
sort (எழுத்து *மாறிலி *files, size_t nfiles, எழுத்து மாறிலி *output_file,
      size_t nthreads)
{
  கட்டமைப்பு buffer buf;
  கட்டமைப்பு sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  போது (nfiles)
    {
      எழுத்து மாறிலி *temp_output;
      எழுத்து மாறிலி *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_வரி;
      எனில் (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          போது (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_வரி = (mult * அளவு (கட்டமைப்பு வரி));
        }
      இல்லையெனில்
        bytes_per_வரி = அளவு (கட்டமைப்பு வரி) * 3 / 2;

      எனில் (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_வரி,
                              &policy);
          initbuf (&buf, bytes_per_வரி, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      போது (fillbuf (&buf, fp, file, &policy))
        {
          கட்டமைப்பு வரி *வரி;

          எனில் (buf.eof && nfiles
              && (bytes_per_வரி + 1
                  < (buf.alloc - buf.used - bytes_per_வரி * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.இடது = buf.used;
              நிறுத்து;
            }

          saved_வரி.text = வெற்று;
          வரி = buffer_linelim (&buf);
          எனில் (buf.eof && !nfiles && !ntemps && !buf.இடது)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          இல்லையெனில்
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          எனில் (1 < buf.nlines)
            {
              எனில் (nthreads > 1)
                {
                  கட்டமைப்பு merge_முனை_queue queue;
                  queue_init (&queue, nthreads);
                  கட்டமைப்பு merge_முனை *merge_மரம் =
                    merge_மரம்_init (nthreads, buf.nlines, வரி);

                  sortlines (வரி, nthreads, buf.nlines, merge_மரம் + 1,
                             &queue, tfp, temp_output);

                  merge_மரம்_destroy (nthreads, merge_மரம்);
                  queue_destroy (&queue);
                }
              இல்லையெனில்
                {
                  sequential_sort (வரி, buf.nlines,
                                   வரி - buf.nlines, false);
                  க்கு (size_t i = 0; i < buf.nlines; i++)
                    write_unique (வரி - i - 1, tfp, temp_output);
                }
            }
          இல்லையெனில்
            write_unique (வரி - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          எனில் (output_file_created)
            செல் finish;
        }
      xfclose (fp, file);
    }

 finish:
  விடுவி (buf.buf);

  எனில் (! output_file_created)
    {
      கட்டமைப்பு tempnode *முனை = temphead;
      கட்டமைப்பு sortfile *tempfiles = xnmalloc (ntemps, அளவு *tempfiles);
      க்கு (size_t i = 0; முனை; i++)
        {
          tempfiles[i].name = முனை->name;
          tempfiles[i].temp = முனை;
          முனை = முனை->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      விடுவி (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

நிலையான வெறுமை
insertkey (கட்டமைப்பு keyfield *சாவி_arg)
{
  கட்டமைப்பு keyfield **p;
  கட்டமைப்பு keyfield *சாவி = xmemdup (சாவி_arg, அளவு *சாவி);

  க்கு (p = &keylist; *p; p = &(*p)->next)
    தொடர்;
  *p = சாவி;
  சாவி->next = வெற்று;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

நிலையான வெறுமை
badfieldspec (எழுத்து மாறிலி *spec, எழுத்து மாறிலி *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

நிலையான வெறுமை
incompatible_options (எழுத்து மாறிலி *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

நிலையான வெறுமை
check_ordering_compatibility (வெறுமை)
{
  க்கு (கட்டமைப்பு keyfield *சாவி = keylist; சாவி; சாவி = சாவி->next)
    எனில் (1 < (சாவி->numeric + சாவி->general_numeric + சாவி->human_numeric
             + சாவி->month + (சாவி->version | சாவி->random | !!சாவி->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        எழுத்து opts[அளவு short_options];
        /* Clear flags we're not interested in.  */
        சாவி->skipsblanks = சாவி->skipeblanks = சாவி->reverse = false;
        சாவி_to_opts (சாவி, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

நிலையான எழுத்து மாறிலி *
parse_field_மொத்தம் (எழுத்து மாறிலி *string, size_t *val, எழுத்து மாறிலி *msgid)
{
  எழுத்து *suffix;
  uintmax_t n;

  தேர்வு (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    நிலை LONGINT_OK:
    நிலை LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      எனில் (*val == n)
        நிறுத்து;
      FALLTHROUGH;
    நிலை LONGINT_OVERFLOW:
    நிலை LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      நிறுத்து;

    நிலை LONGINT_INVALID:
      எனில் (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      திரும்பு வெற்று;
    }

  திரும்பு suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

நிலையான எழுத்து *
set_ordering (எழுத்து மாறிலி *s, கட்டமைப்பு keyfield *சாவி, எண்ணிக்கை blanktype blanktype)
{
  போது (*s)
    {
      தேர்வு (*s)
        {
        நிலை 'b':
          எனில் (blanktype == bl_start || blanktype == bl_both)
            சாவி->skipsblanks = true;
          எனில் (blanktype == bl_end || blanktype == bl_both)
            சாவி->skipeblanks = true;
          நிறுத்து;
        நிலை 'd':
          சாவி->ignore = nondictionary;
          நிறுத்து;
        நிலை 'f':
          சாவி->translate = fold_toupper;
          நிறுத்து;
        நிலை 'g':
          சாவி->general_numeric = true;
          நிறுத்து;
        நிலை 'h':
          சாவி->human_numeric = true;
          நிறுத்து;
        நிலை 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          எனில் (! சாவி->ignore)
            சாவி->ignore = nonprinting;
          நிறுத்து;
        நிலை 'M':
          சாவி->month = true;
          நிறுத்து;
        நிலை 'n':
          சாவி->numeric = true;
          நிறுத்து;
        நிலை 'R':
          சாவி->random = true;
          நிறுத்து;
        நிலை 'r':
          சாவி->reverse = true;
          நிறுத்து;
        நிலை 'V':
          சாவி->version = true;
          நிறுத்து;
        இயல்பு:
          திரும்பு (எழுத்து *) s;
        }
      ++s;
    }
  திரும்பு (எழுத்து *) s;
}

/* Initialize KEY.  */

நிலையான கட்டமைப்பு keyfield *
சாவி_init (கட்டமைப்பு keyfield *சாவி)
{
  memset (சாவி, 0, அளவு *சாவி);
  சாவி->eword = SIZE_MAX;
  திரும்பு சாவி;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  கட்டமைப்பு keyfield *சாவி;
  கட்டமைப்பு keyfield சாவி_buf;
  கட்டமைப்பு keyfield gkey;
  bool gkey_only = false;
  எழுத்து மாறிலி *s;
  முழு c = 0;
  எழுத்து checkonly = 0;
  bool mergeonly = false;
  எழுத்து *random_source = வெற்று;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != வெற்று);
  முழு posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  எழுத்து **files;
  எழுத்து *files_from = வெற்று;
  கட்டமைப்பு Tokens tok;
  எழுத்து மாறிலி *outfile = வெற்று;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#எனில் HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    கட்டமைப்பு lconv மாறிலி *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    எனில் (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    எனில் (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    எனில் (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    எண்ணிக்கை { nsigs = countof (term_sig) };

    கட்டமைப்பு sigaction act;

    sigemptyset (&caught_signals);
    க்கு (size_t i = 0; i < nsigs; i++)
      {
        எனில் (term_sig[i] == SIGPIPE)
          தொடர்;  /* Handled below.  */
        sigaction (term_sig[i], வெற்று, &act);
        எனில் (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    க்கு (size_t i = 0; i < nsigs; i++)
      {
        எனில் (term_sig[i] == SIGPIPE)
          தொடர்;  /* Handled below.  */
        எனில் (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, வெற்று);
      }
  }
  சமிக்ஞை (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  சாவி_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, அளவு *files);

  போது (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      முழு oi = -1;

      எனில் (c == -1
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
          எனில் (argc <= optind)
            நிறுத்து;
          files[nfiles++] = argv[optind++];
        }
      இல்லையெனில் தேர்வு (c)
        {
        நிலை 1:
          சாவி = வெற்று;
          எனில் (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              எனில் (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  சாவி = சாவி_init (&சாவி_buf);
                  s = parse_field_மொத்தம் (optarg + 1, &சாவி->sword, வெற்று);
                  எனில் (s && *s == '.')
                    s = parse_field_மொத்தம் (s + 1, &சாவி->schar, வெற்று);
                  எனில் (! (சாவி->sword || சாவி->schar))
                    சாவி->sword = SIZE_MAX;
                  எனில் (! s || *set_ordering (s, சாவி, bl_start))
                    சாவி = வெற்று;
                  இல்லையெனில்
                    {
                      எனில் (minus_pos_usage)
                        {
                          எழுத்து மாறிலி *optarg1 = argv[optind++];
                          s = parse_field_மொத்தம் (optarg1 + 1, &சாவி->eword,
                                             N_("invalid number after '-'"));
                          எனில் (*s == '.')
                            s = parse_field_மொத்தம் (s + 1, &சாவி->echar,
                                               N_("invalid number after '.'"));
                          எனில் (!சாவி->echar && சாவி->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              சாவி->eword--;
                            }
                          எனில் (*set_ordering (s, சாவி, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      சாவி->traditional_used = true;
                      insertkey (சாவி);
                    }
                }
            }
          எனில் (! சாவி)
            files[nfiles++] = optarg;
          நிறுத்து;

        நிலை SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        நிலை 'b':
        நிலை 'd':
        நிலை 'f':
        நிலை 'g':
        நிலை 'h':
        நிலை 'i':
        நிலை 'M':
        நிலை 'n':
        நிலை 'r':
        நிலை 'R':
        நிலை 'V':
          {
            எழுத்து str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          நிறுத்து;

        நிலை CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        நிலை 'c':
        நிலை 'C':
          எனில் (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          நிறுத்து;

        நிலை COMPRESS_PROGRAM_OPTION:
          எனில் (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          நிறுத்து;

        நிலை DEBUG_PROGRAM_OPTION:
          debug = true;
          நிறுத்து;

        நிலை FILES0_FROM_OPTION:
          files_from = optarg;
          நிறுத்து;

        நிலை 'k':
          சாவி = சாவி_init (&சாவி_buf);

          /* Get POS1. */
          s = parse_field_மொத்தம் (optarg, &சாவி->sword,
                                 N_("invalid number at field start"));
          எனில் (! சாவி->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          எனில் (*s == '.')
            {
              s = parse_field_மொத்தம் (s + 1, &சாவி->schar,
                                     N_("invalid number after '.'"));
              எனில் (! சாவி->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          எனில் (! (சாவி->sword || சாவி->schar))
            சாவி->sword = SIZE_MAX;
          s = set_ordering (s, சாவி, bl_start);
          எனில் (*s != ',')
            {
              சாவி->eword = SIZE_MAX;
              சாவி->echar = 0;
            }
          இல்லையெனில்
            {
              /* Get POS2. */
              s = parse_field_மொத்தம் (s + 1, &சாவி->eword,
                                     N_("invalid number after ','"));
              எனில் (! சாவி->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              எனில் (*s == '.')
                {
                  s = parse_field_மொத்தம் (s + 1, &சாவி->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, சாவி, bl_end);
            }
          எனில் (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (சாவி);
          நிறுத்து;

        நிலை 'm':
          mergeonly = true;
          நிறுத்து;

        நிலை NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          நிறுத்து;

        நிலை 'o':
          எனில் (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          நிறுத்து;

        நிலை RANDOM_SOURCE_OPTION:
          எனில் (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          நிறுத்து;

        நிலை 's':
          stable = true;
          நிறுத்து;

        நிலை 'S':
          specify_sort_size (oi, c, optarg);
          நிறுத்து;

        நிலை 't':
          {
            எழுத்து newtab = optarg[0];
            எனில் (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            எனில் (optarg[1])
              {
                எனில் (streq (optarg, "\\0"))
                  newtab = '\0';
                இல்லையெனில்
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            எனில் (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          நிறுத்து;

        நிலை 'T':
          add_temp_dir (optarg);
          நிறுத்து;

        நிலை PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          நிறுத்து;

        நிலை 'u':
          unique = true;
          நிறுத்து;

        நிலை 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          எனில் (optarg == argv[optind - 1])
            {
              எழுத்து மாறிலி *p;
              க்கு (p = optarg; c_isdigit (*p); p++)
                தொடர்;
              optind -= (*p != '\0');
            }
          நிறுத்து;

        நிலை 'z':
          eolchar = 0;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (சமிக்ஞை (SIGPIPE, SIG_IGN) == SIG_DFL);

  எனில் (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      எனில் (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      எனில் (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      எனில் (tok.n_tok)
        {
          விடுவி (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          க்கு (size_t i = 0; i < nfiles; i++)
            {
              எனில் (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              இல்லையெனில் எனில் (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long முழு file_எண் = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_எண்);
                }
            }
        }
      இல்லையெனில்
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  க்கு (சாவி = keylist; சாவி; சாவி = சாவி->next)
    {
      எனில் (default_சாவி_compare (சாவி) && !சாவி->reverse)
        {
          சாவி->ignore = gkey.ignore;
          சாவி->translate = gkey.translate;
          சாவி->skipsblanks = gkey.skipsblanks;
          சாவி->skipeblanks = gkey.skipeblanks;
          சாவி->month = gkey.month;
          சாவி->numeric = gkey.numeric;
          சாவி->general_numeric = gkey.general_numeric;
          சாவி->human_numeric = gkey.human_numeric;
          சாவி->version = gkey.version;
          சாவி->random = gkey.random;
          சாவி->reverse = gkey.reverse;
        }

      need_random |= சாவி->random;
    }

  எனில் (!keylist && !default_சாவி_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  எனில் (debug)
    {
      எனில் (checkonly || outfile)
        {
          நிலையான எழுத்து opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      எனில் (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      எனில் (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      எனில் (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, வெற்று)));
      இல்லையெனில்
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      சாவி_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  எனில் (need_random)
    random_md5_state_init (random_source);

  எனில் (temp_dirs.n_dirs == 0)
    {
      எழுத்து மாறிலி *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  எனில் (nfiles == 0)
    {
      nfiles = 1;
      விடுவி (files);
      files = xmalloc (அளவு *files);
      *files = (எழுத்து *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  எனில் (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  எனில் (checkonly)
    {
      எனில் (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      எனில் (outfile)
        {
          நிலையான எழுத்து opts[] = {0, 'o', 0};
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

  எனில் (mergeonly)
    {
      கட்டமைப்பு sortfile *sortfiles = xcalloc (nfiles, அளவு *sortfiles);

      க்கு (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  இல்லையெனில்
    {
      எனில் (!nthreads)
        {
          unsigned long முழு np = இலக்கம்_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * அளவு (கட்டமைப்பு merge_முனை));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  எனில் (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
