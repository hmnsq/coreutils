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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <pthread.h>
#അന്തര്ഭാവ <sys/resource.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <spawn.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "fadvise.h"
#അന്തര്ഭാവ "filevercmp.h"
#അന്തര്ഭാവ "findprog.h"
#അന്തര്ഭാവ "flexmember.h"
#അന്തര്ഭാവ "hard-locale.h"
#അന്തര്ഭാവ "hash.h"
#അന്തര്ഭാവ "heap.h"
#അന്തര്ഭാവ "ignore-value.h"
#അന്തര്ഭാവ "mbswidth.h"
#അന്തര്ഭാവ "nproc.h"
#അന്തര്ഭാവ "physmem.h"
#അന്തര്ഭാവ "posixver.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "randread.h"
#അന്തര്ഭാവ "readtokens0.h"
#അന്തര്ഭാവ "stdlib--.h"
#അന്തര്ഭാവ "strnumcmp.h"
#അന്തര്ഭാവ "term-sig.h"
#അന്തര്ഭാവ "unistd--.h"
#അന്തര്ഭാവ "xmemcoll.h"
#അന്തര്ഭാവ "xnanosleep.h"
#അന്തര്ഭാവ "xstrtol.h"
#അന്തര്ഭാവ "xstrtol-error.h"

#ifndef RLIMIT_DATA
ഘടന rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#എങ്കിൽ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#എങ്കിൽ !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#എങ്കിൽ !defined OPEN_MAX
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
എണ്ണൽ { SUBTHREAD_വരികൾ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_വരികൾ_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
എണ്ണൽ { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
എണ്ണൽ
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

എണ്ണൽ
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

എണ്ണൽ
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_വേര് = 1
  };

/* The representation of the decimal point in the current locale.  */
സ്ഥിതം അക്ഷരം decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
സ്ഥിതം bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
സ്ഥിതം bool hard_LC_COLLATE;
#എങ്കിൽ HAVE_NL_LANGINFO
സ്ഥിതം bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
എണ്ണൽ blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
സ്ഥിതം അക്ഷരം eolchar = '\n';

/* Lines are held in memory as counted strings. */
ഘടന വരി
{
  അക്ഷരം *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  അക്ഷരം *keybeg;			/* Start of first key. */
  അക്ഷരം *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ഘടന buffer
{
  അക്ഷരം *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ഇടത്;			/* Number of bytes left from previous reads. */
  size_t വരി_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ഘടന sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
ഘടന keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool സ്ഥിരം *ignore;		/* Boolean array of characters to ignore. */
  അക്ഷരം സ്ഥിരം *translate;	/* Translation applied to characters. */
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
  ഘടന keyfield *next;	/* Next keyfield to try. */
};

ഘടന month
{
  അക്ഷരം സ്ഥിരം *name;
  പൂർണ്ണസംഖ്യ val;
};

/* Binary merge tree node. */
ഘടന merge_കണ്ണി
{
  ഘടന വരി *lo;              /* Lines to merge from LO child node. */
  ഘടന വരി *hi;              /* Lines to merge from HI child node. */
  ഘടന വരി *end_lo;          /* End of available lines from LO. */
  ഘടന വരി *end_hi;          /* End of available lines from HI. */
  ഘടന വരി **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ഘടന merge_കണ്ണി *parent;    /* Parent node. */
  ഘടന merge_കണ്ണി *lo_child;  /* LO child node. */
  ഘടന merge_കണ്ണി *hi_child;  /* HI child node. */
  unsigned പൂർണ്ണസംഖ്യ level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ഘടന merge_കണ്ണി_queue
{
  ഘടന heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
സ്ഥിതം ഘടന വരി saved_വരി;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
സ്ഥിതം bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
സ്ഥിതം bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
സ്ഥിതം bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
സ്ഥിതം അക്ഷരം fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
സ്ഥിതം ഘടന month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + വലുപ്പം (ഘടന വരി))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
സ്ഥിതം size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
സ്ഥിതം size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define നിവേശം_FILE_SIZE_GUESS (128 * 1024)

സ്ഥിതം ഘടന
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  അക്ഷരം സ്ഥിരം **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
സ്ഥിതം bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
സ്ഥിതം bool stable;

/* An int value outside char range.  */
എണ്ണൽ { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
എണ്ണൽ { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
സ്ഥിതം പൂർണ്ണസംഖ്യ tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
സ്ഥിതം bool unique;

/* Nonzero if any of the input files are the standard input. */
സ്ഥിതം bool have_read_stdin;

/* List of key field comparisons to be tried.  */
സ്ഥിതം ഘടന keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം *compress_program;

/* Annotate the output with extra info to aid the user.  */
സ്ഥിതം bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
സ്ഥിതം bool default_SIGPIPE;

/* The list of temporary files. */
ഘടന tempnode
{
  ഘടന tempnode *അസ്ഥിര next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  അക്ഷരം state;
  അക്ഷരം name[FLEXIBLE_ARRAY_MEMBER];
};
സ്ഥിതം ഘടന tempnode *അസ്ഥിര temphead;
സ്ഥിതം ഘടന tempnode *അസ്ഥിര *temptail = &temphead;

/* Clean up any remaining temporary files.  */

സ്ഥിതം ശൂന്യം
cleanup (ശൂന്യം)
{
  വേണ്ടി (ഘടന tempnode സ്ഥിരം *കണ്ണി = temphead; കണ്ണി; കണ്ണി = കണ്ണി->next)
    unlink (കണ്ണി->name);
  temphead = ശൂന്യ;
}

/* Handle interrupts and hangups. */

സ്ഥിതം ശൂന്യം
sighandler (പൂർണ്ണസംഖ്യ sig)
{
  cleanup ();

  സൂചന (sig, SIG_DFL);
  ഉയർത്തുക (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

സ്ഥിതം ശൂന്യം
sort_die (അക്ഷരം സ്ഥിരം *message, അക്ഷരം സ്ഥിരം *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  എങ്കിൽ (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         ignore leading blanks when finding sort താക്കോലുകൾ in each വരി\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-അവസ്ഥ\n\
         fold lower അവസ്ഥ to upper അവസ്ഥ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical മൂല്യം\n\
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
         compare according to string numerical മൂല്യം;\n\
         see full documentation വേണ്ടി supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical താക്കോലുകൾ.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; വേണ്ടി more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check വേണ്ടി sorted നിവേശം; ചെയ്യുക not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but ചെയ്യുക not report first bad വരി\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the വരി used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         പഠ നിവേശം from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, പഠ names from standard നിവേശം\n\
"));
      oputs (_("\
  -k, --താക്കോൽ=KEYDEF\n\
         sort via a താക്കോൽ; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; ചെയ്യുക not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         ലിഖ result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE വേണ്ടി പ്രധാന memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR വേണ്ടി temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the സംഖ്യ of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of വരികൾ with equal താക്കോലുകൾ;\n\
         with -c, check വേണ്ടി strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] വേണ്ടി start and stop position, where F is a\n\
field സംഖ്യ and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the വരി's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options വേണ്ടി that താക്കോൽ.  If no താക്കോൽ is given, use\n\
the entire വരി as the താക്കോൽ.  Use --debug to diagnose incorrect താക്കോൽ usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (സ്ഥിരസ്ഥിതി), and so on വേണ്ടി M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte മൂല്യങ്ങൾ.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
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

സ്ഥിതം അക്ഷരം സ്ഥിരം short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"ignore-leading-blanks", no_argument, ശൂന്യ, 'b'},
  {"check", optional_argument, ശൂന്യ, CHECK_OPTION},
  {"compress-program", required_argument, ശൂന്യ, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, ശൂന്യ, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, ശൂന്യ, 'd'},
  {"ignore-case", no_argument, ശൂന്യ, 'f'},
  {"files0-from", required_argument, ശൂന്യ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, ശൂന്യ, 'g'},
  {"ignore-nonprinting", no_argument, ശൂന്യ, 'i'},
  {"key", required_argument, ശൂന്യ, 'k'},
  {"merge", no_argument, ശൂന്യ, 'm'},
  {"month-sort", no_argument, ശൂന്യ, 'M'},
  {"numeric-sort", no_argument, ശൂന്യ, 'n'},
  {"human-numeric-sort", no_argument, ശൂന്യ, 'h'},
  {"version-sort", no_argument, ശൂന്യ, 'V'},
  {"random-sort", no_argument, ശൂന്യ, 'R'},
  {"random-source", required_argument, ശൂന്യ, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, ശൂന്യ, SORT_OPTION},
  {"output", required_argument, ശൂന്യ, 'o'},
  {"reverse", no_argument, ശൂന്യ, 'r'},
  {"stable", no_argument, ശൂന്യ, 's'},
  {"batch-size", required_argument, ശൂന്യ, NMERGE_OPTION},
  {"buffer-size", required_argument, ശൂന്യ, 'S'},
  {"field-separator", required_argument, ശൂന്യ, 't'},
  {"temporary-directory", required_argument, ശൂന്യ, 'T'},
  {"unique", no_argument, ശൂന്യ, 'u'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {"parallel", required_argument, ശൂന്യ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE ശൂന്യ
#undef  _ct_
};
സ്ഥിതം അക്ഷരം സ്ഥിരം check_types[] =
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

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE ശൂന്യ
#undef  _st_
};
സ്ഥിതം അക്ഷരം സ്ഥിരം sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
സ്ഥിതം sigset_t caught_signals;

/* Critical section status.  */
ഘടന cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
സ്ഥിതം ശൂന്യം
cs_enter (ഘടന cs_status *status)
{
  പൂർണ്ണസംഖ്യ ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
സ്ഥിതം ശൂന്യം
cs_leave (ഘടന cs_status സ്ഥിരം *status)
{
  എങ്കിൽ (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, ശൂന്യ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
എണ്ണൽ { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ഘടന sortfile
{
  /* The file's name.  */
  അക്ഷരം സ്ഥിരം *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ഘടന tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
സ്ഥിതം Hash_table *proctab;

എണ്ണൽ { INIT_PROCTAB_SIZE = 47 };

സ്ഥിതം size_t
proctab_hasher (ശൂന്യം സ്ഥിരം *entry, size_t tabsize)
{
  ഘടന tempnode സ്ഥിരം *കണ്ണി = entry;
  തിരികെ കണ്ണി->pid % tabsize;
}

സ്ഥിതം bool
proctab_comparator (ശൂന്യം സ്ഥിരം *e1, ശൂന്യം സ്ഥിരം *e2)
{
  ഘടന tempnode സ്ഥിരം *n1 = e1;
  ഘടന tempnode സ്ഥിരം *n2 = e2;
  തിരികെ n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
സ്ഥിതം pid_t nprocs;

സ്ഥിതം bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

സ്ഥിതം pid_t
reap (pid_t pid)
{
  പൂർണ്ണസംഖ്യ status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  എങ്കിൽ (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  അല്ലെങ്കിൽ എങ്കിൽ (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      എങ്കിൽ (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  തിരികെ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

സ്ഥിതം ശൂന്യം
register_proc (ഘടന tempnode *temp)
{
  എങ്കിൽ (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, ശൂന്യ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ശൂന്യ);
      എങ്കിൽ (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  എങ്കിൽ (! hash_തിരുകുക (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

സ്ഥിതം bool
delete_proc (pid_t pid)
{
  ഘടന tempnode test;

  test.pid = pid;
  ഘടന tempnode *കണ്ണി = hash_remove (proctab, &test);
  എങ്കിൽ (! കണ്ണി)
    തിരികെ false;
  കണ്ണി->state = REAPED;
  തിരികെ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

സ്ഥിതം ശൂന്യം
wait_proc (pid_t pid)
{
  എങ്കിൽ (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

സ്ഥിതം ശൂന്യം
reap_exited (ശൂന്യം)
{
  ആകുമ്പോൾ (0 < nprocs && reap (0))
    തുടരുക;
}

/* Reap at least one exited child, waiting if necessary.  */

സ്ഥിതം ശൂന്യം
reap_some (ശൂന്യം)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

സ്ഥിതം ശൂന്യം
reap_all (ശൂന്യം)
{
  ആകുമ്പോൾ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

സ്ഥിതം ശൂന്യം
exit_cleanup (ശൂന്യം)
{
  എങ്കിൽ (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ഘടന cs_status cs;
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

സ്ഥിതം ഘടന tempnode *
create_temp_file (പൂർണ്ണസംഖ്യ *pfd, bool survive_fd_exhaustion)
{
  സ്ഥിതം അക്ഷരം സ്ഥിരം slashbase[] = "/sortXXXXXX";
  സ്ഥിതം idx_t temp_dir_index;
  പൂർണ്ണസംഖ്യ fd;
  പൂർണ്ണസംഖ്യ saved_errno;
  അക്ഷരം സ്ഥിരം *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  ഘടന tempnode *കണ്ണി =
    xmalloc (FLEXSIZEOF (ഘടന tempnode, name, len + വലുപ്പം slashbase));
  അക്ഷരം *file = കണ്ണി->name;
  ഘടന cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, വലുപ്പം slashbase);
  കണ്ണി->next = ശൂന്യ;
  എങ്കിൽ (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  എങ്കിൽ (0 <= fd)
    {
      *temptail = കണ്ണി;
      temptail = &കണ്ണി->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  എങ്കിൽ (fd < 0)
    {
      എങ്കിൽ (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      സ്വതന്ത്രം (കണ്ണി);
      കണ്ണി = ശൂന്യ;
    }

  *pfd = fd;
  തിരികെ കണ്ണി;
}

/* Return a pointer to stdout status, or NULL on failure.  */

സ്ഥിതം ഘടന stat *
get_outstatus (ശൂന്യം)
{
  സ്ഥിതം പൂർണ്ണസംഖ്യ outstat_errno;
  സ്ഥിതം ഘടന stat outstat;
  എങ്കിൽ (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  തിരികെ outstat_errno < 0 ? &outstat : ശൂന്യ;
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

സ്ഥിതം FILE *
stream_open (അക്ഷരം സ്ഥിരം *file, അക്ഷരം സ്ഥിരം *how)
{
  FILE *fp;

  എങ്കിൽ (*how == 'r')
    {
      എങ്കിൽ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ശൂന്യ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (*how == 'w')
    {
      എങ്കിൽ (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          പൂർണ്ണസംഖ്യ ftruncate_errno = errno;
          ഘടന stat *outst = get_outstatus ();
          എങ്കിൽ (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  അല്ലെങ്കിൽ
    affirm (!"unexpected mode passed to stream_open");

  തിരികെ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

സ്ഥിതം FILE *
xfopen (അക്ഷരം സ്ഥിരം *file, അക്ഷരം സ്ഥിരം *how)
{
  FILE *fp = stream_open (file, how);
  എങ്കിൽ (!fp)
    sort_die (_("open failed"), file);
  തിരികെ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

സ്ഥിതം ശൂന്യം
xfclose (FILE *fp, അക്ഷരം സ്ഥിരം *file)
{
  തിരഞ്ഞെടുപ്പ് (fileno (fp))
    {
    അവസ്ഥ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      നിർത്തുക;

    അവസ്ഥ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      എങ്കിൽ (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      എങ്കിൽ (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      നിർത്തുക;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

സ്ഥിതം ശൂന്യം
move_fd (പൂർണ്ണസംഖ്യ oldfd, പൂർണ്ണസംഖ്യ newfd)
{
  എങ്കിൽ (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_മൂല്യം (dup2 (oldfd, newfd));
      ignore_മൂല്യം (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  പൂർണ്ണസംഖ്യ oldfd, പൂർണ്ണസംഖ്യ newfd)
{
  പൂർണ്ണസംഖ്യ result = 0;
  എങ്കിൽ (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      എങ്കിൽ (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  തിരികെ result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
get_resolved_compress_program (ശൂന്യം)
{
  /* Use a cache, to perform the search only once.  */
  സ്ഥിതം അക്ഷരം സ്ഥിരം *resolved_compress_program_cache /* = NULL */;

  എങ്കിൽ (resolved_compress_program_cache == ശൂന്യ)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), ശൂന്യ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  തിരികെ resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
pipe_child (pid_t *pid, പൂർണ്ണസംഖ്യ pipefds[2], പൂർണ്ണസംഖ്യ tempfd, bool decompress,
            size_t tries)
{
  അക്ഷരം സ്ഥിരം *resolved_compress_program;
  ഘടന tempnode *saved_temphead;
  ഇരട്ട wait_retry = 0.25;
  ഘടന cs_status cs;
  പൂർണ്ണസംഖ്യ result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  എങ്കിൽ (resolved_compress_program == ശൂന്യ)
    തിരികെ errno;

  എങ്കിൽ ((result = posix_spawnattr_init (&attr)))
    തിരികെ result;
  എങ്കിൽ ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      തിരികെ result;
    }

  എങ്കിൽ (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      പൂർണ്ണസംഖ്യ saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      തിരികെ saved_errno;
    }

  എങ്കിൽ ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      തിരികെ result;
    }

  അക്ഷരം സ്ഥിരം *സ്ഥിരം argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : ശൂന്യ,
      ശൂന്യ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  എങ്കിൽ (nmerge + 1 < nprocs)
    reap_some ();

  ആകുമ്പോൾ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ശൂന്യ;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (അക്ഷരം * സ്ഥിരം *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      എങ്കിൽ (result != EAGAIN)
        നിർത്തുക;
      അല്ലെങ്കിൽ
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

  എങ്കിൽ (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  അല്ലെങ്കിൽ
    ++nprocs;

  തിരികെ result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

സ്ഥിതം ഘടന tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  പൂർണ്ണസംഖ്യ tempfd;
  ഘടന tempnode *കണ്ണി = create_temp_file (&tempfd, survive_fd_exhaustion);
  എങ്കിൽ (! കണ്ണി)
    തിരികെ ശൂന്യ;

  കണ്ണി->state = UNCOMPRESSED;

  എങ്കിൽ (compress_program)
    {
      പൂർണ്ണസംഖ്യ pipefds[2];
      സ്ഥിതം പൂർണ്ണസംഖ്യ last_result = 0;

      പൂർണ്ണസംഖ്യ result = pipe_child (&കണ്ണി->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      എങ്കിൽ (result)
        {
          എങ്കിൽ (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      അല്ലെങ്കിൽ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (കണ്ണി);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  എങ്കിൽ (! *pfp)
    sort_die (_("couldn't create temporary file"), കണ്ണി->name);

  തിരികെ കണ്ണി;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

സ്ഥിതം ഘടന tempnode *
create_temp (FILE **pfp)
{
  തിരികെ maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

സ്ഥിതം FILE *
open_temp (ഘടന tempnode *temp)
{
  പൂർണ്ണസംഖ്യ tempfd, pipefds[2];
  FILE *fp = ശൂന്യ;

  എങ്കിൽ (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  എങ്കിൽ (tempfd < 0)
    തിരികെ ശൂന്യ;

  pid_t child;
  പൂർണ്ണസംഖ്യ result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  എങ്കിൽ (result)
    {
      എങ്കിൽ (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  അല്ലെങ്കിൽ
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      എങ്കിൽ (! fp)
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  തിരികെ fp;
}

/* Append DIR to the array of temporary directory names.  */
സ്ഥിതം ശൂന്യം
add_temp_dir (അക്ഷരം സ്ഥിരം *dir)
{
  എങ്കിൽ (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              വലുപ്പം *temp_dirs.dirs);

  idx_t സ്ഥിരം dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

സ്ഥിതം ശൂന്യം
zaptemp (അക്ഷരം സ്ഥിരം *name)
{
  ഘടന tempnode *അസ്ഥിര *pnode;
  ഘടന tempnode *കണ്ണി;
  ഘടന tempnode *next;
  പൂർണ്ണസംഖ്യ unlink_status;
  പൂർണ്ണസംഖ്യ unlink_errno = 0;
  ഘടന cs_status cs;

  വേണ്ടി (pnode = &temphead; (കണ്ണി = *pnode)->name != name; pnode = &കണ്ണി->next)
    തുടരുക;

  എങ്കിൽ (കണ്ണി->state == UNREAPED)
    wait_proc (കണ്ണി->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = കണ്ണി->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  എങ്കിൽ (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  എങ്കിൽ (! next)
    temptail = pnode;
  സ്വതന്ത്രം (കണ്ണി);
}

#എങ്കിൽ HAVE_NL_LANGINFO

സ്ഥിതം പൂർണ്ണസംഖ്യ
struct_month_cmp (ശൂന്യം സ്ഥിരം *m1, ശൂന്യം സ്ഥിരം *m2)
{
  ഘടന month സ്ഥിരം *month1 = m1;
  ഘടന month സ്ഥിരം *month2 = m2;
  തിരികെ strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

സ്ഥിതം ശൂന്യം
inittables (ശൂന്യം)
{
  വേണ്ടി (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#എങ്കിൽ HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  എങ്കിൽ (hard_LC_TIME)
    {
      വേണ്ടി (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          അക്ഷരം സ്ഥിരം *s;
          size_t s_len;
          size_t j, k;
          അക്ഷരം *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          വേണ്ടി (j = k = 0; j < s_len; j++)
            എങ്കിൽ (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, വലുപ്പം *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
സ്ഥിതം ശൂന്യം
specify_nmerge (പൂർണ്ണസംഖ്യ oi, അക്ഷരം c, അക്ഷരം സ്ഥിരം *s)
{
  uintmax_t n;
  ഘടന rlimit rlimit;
  എണ്ണൽ strtol_error e = xstrtoumax (s, ശൂന്യ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned പൂർണ്ണസംഖ്യ max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  എങ്കിൽ (e == LONGINT_OK)
    {
      nmerge = n;
      എങ്കിൽ (nmerge != n)
        e = LONGINT_OVERFLOW;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          അല്ലെങ്കിൽ എങ്കിൽ (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          അല്ലെങ്കിൽ
            തിരികെ;
        }
    }

  എങ്കിൽ (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  അല്ലെങ്കിൽ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
സ്ഥിതം ശൂന്യം
specify_sort_size (പൂർണ്ണസംഖ്യ oi, അക്ഷരം c, അക്ഷരം സ്ഥിരം *s)
{
  uintmax_t n;
  അക്ഷരം *suffix;
  എണ്ണൽ strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  എങ്കിൽ (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  എങ്കിൽ (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    തിരഞ്ഞെടുപ്പ് (suffix[0])
      {
      അവസ്ഥ 'b':
        e = LONGINT_OK;
        നിർത്തുക;

      അവസ്ഥ '%':
        {
          ഇരട്ട mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          എങ്കിൽ (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          അല്ലെങ്കിൽ
            e = LONGINT_OVERFLOW;
        }
        നിർത്തുക;
      }

  എങ്കിൽ (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      എങ്കിൽ (n < sort_size)
        തിരികെ;

      sort_size = n;
      എങ്കിൽ (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          തിരികെ;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
സ്ഥിതം size_t
specify_nthreads (പൂർണ്ണസംഖ്യ oi, അക്ഷരം c, അക്ഷരം സ്ഥിരം *s)
{
  uintmax_t nthreads;
  എണ്ണൽ strtol_error e = xstrtoumax (s, ശൂന്യ, 10, &nthreads, "");
  എങ്കിൽ (e == LONGINT_OVERFLOW)
    തിരികെ SIZE_MAX;
  എങ്കിൽ (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  എങ്കിൽ (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  എങ്കിൽ (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  തിരികെ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
സ്ഥിതം size_t
default_sort_size (ശൂന്യം)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  ഘടന rlimit rlimit;
  എങ്കിൽ (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  എങ്കിൽ (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  എങ്കിൽ (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ഇരട്ട avail = physmem_available ();
  ഇരട്ട total = physmem_total ();
  ഇരട്ട mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  എങ്കിൽ (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  എങ്കിൽ (mem < size)
    size = mem;
  തിരികെ MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

സ്ഥിതം bool
വരി_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = വലുപ്പം (ഘടന വരി);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  എങ്കിൽ (ckd_add (&aligned, size, padding))
    തിരികെ false;

  *alloc = aligned;
  തിരികെ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

സ്ഥിതം size_t
default_initial_sort_size (size_t വരി_bytes)
{
  size_t size = വരി_bytes + 2;
  size_t നിവേശം_size;

  എങ്കിൽ (ckd_mul (&നിവേശം_size, നിവേശം_FILE_SIZE_GUESS, വരി_bytes + 1)
      || ckd_add (&size, size, നിവേശം_size))
    തിരികെ SIZE_MAX;

  തിരികെ MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

സ്ഥിതം bool
നിവേശം_size_buffer_bytes (uintmax_t file_size, size_t വരി_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_നിവേശം_byte = വരി_bytes + 1;

  എങ്കിൽ (SIZE_MAX / worst_case_per_നിവേശം_byte < file_size)
    തിരികെ false;

  size_t size = file_size * worst_case_per_നിവേശം_byte;
  എങ്കിൽ (SIZE_MAX - size < 1)
    തിരികെ false;

  *worst_case = size + 1;
  തിരികെ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

സ്ഥിതം ശൂന്യം
sort_buffer_policy (FILE *സ്ഥിരം *fps, size_t nfps,
                    അക്ഷരം *സ്ഥിരം *files, size_t nfiles,
                    size_t വരി_bytes,
                    ഘടന sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (വരി_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_നിവേശം_byte = വരി_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_നിവേശം_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  വേണ്ടി (size_t i = 0; i < nfiles; i++)
    {
      ഘടന stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      എങ്കിൽ (size_bound <= size)
        {
          policy->initial = size_bound;
          തിരികെ;
        }

      എങ്കിൽ ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      എങ്കിൽ (known_size)
        file_size = st.st_size;
      അല്ലെങ്കിൽ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          എങ്കിൽ (sort_size)
            {
              policy->initial = size_bound;
              തിരികെ;
            }
          file_size = നിവേശം_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      എങ്കിൽ (! നിവേശം_size_buffer_bytes (file_size, വരി_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          തിരികെ;
        }

      എങ്കിൽ (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          തിരികെ;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

സ്ഥിതം ശൂന്യം
initbuf (ഘടന buffer *buf, size_t വരി_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ആകുമ്പോൾ (true)
    {
      എങ്കിൽ (! വരി_aligned_size (&alloc))
        buf->buf = ശൂന്യ;
      അല്ലെങ്കിൽ
        buf->buf = malloc (alloc);
      എങ്കിൽ (buf->buf)
        നിർത്തുക;
      alloc /= 2;
      എങ്കിൽ (alloc <= വരി_bytes + 1)
        xalloc_die ();
    }

  buf->വരി_bytes = വരി_bytes;
  buf->alloc = alloc;
  buf->used = buf->ഇടത് = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

സ്ഥിതം വരിയിൽ ഘടന വരി *
buffer_linelim (ഘടന buffer സ്ഥിരം *buf)
{
  ശൂന്യം *linelim = buf->buf + buf->alloc;
  തിരികെ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

സ്ഥിതം bool
try_growbuf (ഘടന buffer *buf, size_t alloc)
{
  എങ്കിൽ (! വരി_aligned_size (&alloc) || alloc <= buf->alloc)
    തിരികെ false;

  അക്ഷരം *newbuf = malloc (alloc);
  എങ്കിൽ (! newbuf)
    തിരികെ false;

  അക്ഷരം *oldbuf = buf->buf;
  ഘടന വരി *old_linelim = buffer_linelim (buf);
  ഘടന വരി *old_വരി = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  ഘടന വരി *new_linelim = (ശൂന്യം *) (newbuf + alloc);
  ഘടന വരി *new_വരി = new_linelim - buf->nlines;
  memcpy (new_വരി, old_വരി, buf->nlines * വലുപ്പം *new_വരി);

  വേണ്ടി (ഘടന വരി *വരി = new_വരി; വരി < new_linelim; വരി++)
    {
      വരി->text = newbuf + (വരി->text - oldbuf);
      എങ്കിൽ (keylist)
        {
          വരി->keybeg = newbuf + (വരി->keybeg - oldbuf);
          വരി->keylim = newbuf + (വരി->keylim - oldbuf);
        }
    }

  സ്വതന്ത്രം (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  തിരികെ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

സ്ഥിതം bool
maybe_growbuf (ഘടന buffer *buf, ഘടന sort_buffer_policy *policy)
{
  എങ്കിൽ (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    തിരികെ false;

  size_t alloc;
  എങ്കിൽ (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  അല്ലെങ്കിൽ
    alloc = policy->limit;

  എങ്കിൽ (try_growbuf (buf, alloc))
    തിരികെ true;

  policy->growth_failed = true;
  തിരികെ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

സ്ഥിതം അക്ഷരം *
begfield (ഘടന വരി സ്ഥിരം *വരി, ഘടന keyfield സ്ഥിരം *താക്കോൽ)
{
  അക്ഷരം *ptr = വരി->text, *lim = ptr + വരി->length - 1;
  size_t sword = താക്കോൽ->sword;
  size_t schar = താക്കോൽ->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  എങ്കിൽ (tab != TAB_DEFAULT)
    ആകുമ്പോൾ (ptr < lim && sword--)
      {
        അക്ഷരം *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        എങ്കിൽ (ptr < lim)
          ++ptr;
      }
  അല്ലെങ്കിൽ
    ആകുമ്പോൾ (ptr < lim && sword--)
      {
        ആകുമ്പോൾ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ആകുമ്പോൾ (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  എങ്കിൽ (താക്കോൽ->skipsblanks)
    ആകുമ്പോൾ (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  എങ്കിൽ (schar < remaining_bytes)
    ptr += schar;
  അല്ലെങ്കിൽ
    ptr = lim;

  തിരികെ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
സ്ഥിതം അക്ഷരം *
limfield (ഘടന വരി സ്ഥിരം *വരി, ഘടന keyfield സ്ഥിരം *താക്കോൽ)
{
  അക്ഷരം *ptr = വരി->text, *lim = ptr + വരി->length - 1;
  size_t eword = താക്കോൽ->eword, echar = താക്കോൽ->echar;

  എങ്കിൽ (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  എങ്കിൽ (tab != TAB_DEFAULT)
    ആകുമ്പോൾ (ptr < lim && eword--)
      {
        അക്ഷരം *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        എങ്കിൽ (ptr < lim && (eword || echar))
          ++ptr;
      }
  അല്ലെങ്കിൽ
    ആകുമ്പോൾ (ptr < lim && eword--)
      {
        ആകുമ്പോൾ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ആകുമ്പോൾ (ptr < lim && !blanks[to_uchar (*ptr)])
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
  എങ്കിൽ (tab != TAB_DEFAULT)
    {
      അക്ഷരം *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      എങ്കിൽ (newlim)
        lim = newlim;
    }
  അല്ലെങ്കിൽ
    {
      അക്ഷരം *newlim;
      newlim = ptr;
      ആകുമ്പോൾ (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      ആകുമ്പോൾ (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  എങ്കിൽ (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      എങ്കിൽ (താക്കോൽ->skipeblanks)
        ആകുമ്പോൾ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      എങ്കിൽ (echar < remaining_bytes)
        ptr += echar;
      അല്ലെങ്കിൽ
        ptr = lim;
    }

  തിരികെ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

സ്ഥിതം bool
fillbuf (ഘടന buffer *buf, FILE *fp, അക്ഷരം സ്ഥിരം *file,
         ഘടന sort_buffer_policy *policy)
{
  ഘടന keyfield സ്ഥിരം *താക്കോൽ = keylist;
  അക്ഷരം eol = eolchar;
  size_t വരി_bytes = buf->വരി_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  എങ്കിൽ (buf->eof)
    തിരികെ false;

  എങ്കിൽ (buf->used != buf->ഇടത്)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->ഇടത്, buf->ഇടത്);
      buf->used = buf->ഇടത്;
      buf->nlines = 0;
    }

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *ptr = buf->buf + buf->used;
      ഘടന വരി *linelim = buffer_linelim (buf);
      ഘടന വരി *വരി = linelim - buf->nlines;
      size_t avail = (അക്ഷരം *) linelim - buf->nlines * വരി_bytes - ptr;
      അക്ഷരം *വരി_start = buf->nlines ? വരി->text + വരി->length : buf->buf;

      ആകുമ്പോൾ (വരി_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (വരി_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          അക്ഷരം *ptrlim = ptr + bytes_read;
          അക്ഷരം *p;
          avail -= bytes_read;

          എങ്കിൽ (bytes_read != readsize)
            {
              എങ്കിൽ (ferror (fp))
                sort_die (_("read failed"), file);
              എങ്കിൽ (feof (fp))
                {
                  buf->eof = true;
                  എങ്കിൽ (buf->buf == ptrlim)
                    തിരികെ false;
                  എങ്കിൽ (വരി_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ആകുമ്പോൾ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              വരി--;
              വരി->text = വരി_start;
              വരി->length = ptr - വരി_start;
              mergesize = MAX (mergesize, വരി->length);
              avail -= വരി_bytes;

              എങ്കിൽ (താക്കോൽ)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  വരി->keylim = (താക്കോൽ->eword == SIZE_MAX
                                  ? p
                                  : limfield (വരി, താക്കോൽ));

                  എങ്കിൽ (താക്കോൽ->sword != SIZE_MAX)
                    വരി->keybeg = begfield (വരി, താക്കോൽ);
                  അല്ലെങ്കിൽ
                    {
                      എങ്കിൽ (താക്കോൽ->skipsblanks)
                        ആകുമ്പോൾ (blanks[to_uchar (*വരി_start)])
                          വരി_start++;
                      വരി->keybeg = വരി_start;
                    }
                }

              വരി_start = ptr;
            }

          ptr = ptrlim;
          എങ്കിൽ (buf->eof)
            നിർത്തുക;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - വരി;
      എങ്കിൽ (buf->nlines != 0)
        {
          എങ്കിൽ (! buf->eof && maybe_growbuf (buf, policy))
            തുടരുക;
          buf->ഇടത് = ptr - വരി_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          തിരികെ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t വരി_alloc = buf->alloc / വലുപ്പം (ഘടന വരി);
        buf->buf = xpalloc (buf->buf, &വരി_alloc, 1, -1, വലുപ്പം (ഘടന വരി));
        buf->alloc = വരി_alloc * വലുപ്പം (ഘടന വരി);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
സ്ഥിതം അക്ഷരം സ്ഥിരം unit_order[UCHAR_LIM] =
  {
#എങ്കിൽ ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#അല്ലെങ്കിൽ
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
സ്ഥിതം അക്ഷരം
traverse_raw_സംഖ്യ (അക്ഷരം സ്ഥിരം **സംഖ്യ)
{
  അക്ഷരം സ്ഥിരം *p = *സംഖ്യ;
  അക്ഷരം ch;
  അക്ഷരം max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ആകുമ്പോൾ (c_isdigit (ch = *p++))
    {
      എങ്കിൽ (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      എങ്കിൽ (ends_with_thousands_sep)
        ++p;
    }

  എങ്കിൽ (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *സംഖ്യ = p - 2;
      തിരികെ max_digit;
    }

  എങ്കിൽ (ch == decimal_point)
    ആകുമ്പോൾ (c_isdigit (ch = *p++))
      എങ്കിൽ (max_digit < ch)
        max_digit = ch;

  *സംഖ്യ = p - 1;
  തിരികെ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
find_unit_order (അക്ഷരം സ്ഥിരം *സംഖ്യ)
{
  bool minus_sign = (*സംഖ്യ == '-');
  അക്ഷരം സ്ഥിരം *p = സംഖ്യ + minus_sign;
  അക്ഷരം max_digit = traverse_raw_സംഖ്യ (&p);
  എങ്കിൽ ('0' < max_digit)
    {
      unsigned അക്ഷരം ch = *p;
      പൂർണ്ണസംഖ്യ order = unit_order[ch];
      തിരികെ (minus_sign ? -order : order);
    }
  അല്ലെങ്കിൽ
    തിരികെ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
human_numcompare (അക്ഷരം സ്ഥിരം *a, അക്ഷരം സ്ഥിരം *b)
{
  ആകുമ്പോൾ (blanks[to_uchar (*a)])
    a++;
  ആകുമ്പോൾ (blanks[to_uchar (*b)])
    b++;

  പൂർണ്ണസംഖ്യ diff = find_unit_order (a) - find_unit_order (b);
  തിരികെ (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
സ്ഥിതം പൂർണ്ണസംഖ്യ
numcompare (അക്ഷരം സ്ഥിരം *a, അക്ഷരം സ്ഥിരം *b)
{
  ആകുമ്പോൾ (blanks[to_uchar (*a)])
    a++;
  ആകുമ്പോൾ (blanks[to_uchar (*b)])
    b++;

  തിരികെ strnumcmp (a, b, decimal_point, thousands_sep);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
nan_compare (long ഇരട്ട a, long ഇരട്ട b)
{
  അക്ഷരം buf[2][വലുപ്പം "-nan""()" + CHAR_BIT * വലുപ്പം a];
  snprintf (buf[0], വലുപ്പം buf[0], "%Lf", a);
  snprintf (buf[1], വലുപ്പം buf[1], "%Lf", b);
  തിരികെ strcmp (buf[0], buf[1]);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
general_numcompare (അക്ഷരം സ്ഥിരം *sa, അക്ഷരം സ്ഥിരം *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  അക്ഷരം *ea;
  അക്ഷരം *eb;
  long ഇരട്ട a = strtold (sa, &ea);
  long ഇരട്ട b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  എങ്കിൽ (sa == ea)
    തിരികെ sb == eb ? 0 : -1;
  എങ്കിൽ (sb == eb)
    തിരികെ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  തിരികെ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
getmonth (അക്ഷരം സ്ഥിരം *month, അക്ഷരം **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  ആകുമ്പോൾ (blanks[to_uchar (*month)])
    month++;

  ചെയ്യുക
    {
      size_t ix = (lo + hi) / 2;
      അക്ഷരം സ്ഥിരം *m = month;
      അക്ഷരം സ്ഥിരം *n = monthtab[ix].name;

      വേണ്ടി (;; m++, n++)
        {
          എങ്കിൽ (!*n)
            {
              എങ്കിൽ (ea)
                *ea = (അക്ഷരം *) m;
              തിരികെ monthtab[ix].val;
            }
          എങ്കിൽ (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              നിർത്തുക;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              നിർത്തുക;
            }
        }
    }
  ആകുമ്പോൾ (lo < hi);

  തിരികെ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#എങ്കിൽ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# എങ്കിൽ 14 <= __GNUC__
#  pragma GCC diagnostic തള്ളുക
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#അന്തര്ഭാവ "md5.h"

#എങ്കിൽ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# എങ്കിൽ 14 <= __GNUC__
#  pragma GCC diagnostic എടുക്കുക
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
സ്ഥിതം ശൂന്യം
link_failure (ശൂന്യം)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
സ്ഥിതം ശൂന്യം *
ചിഹ്നം_address (ശൂന്യം *handle, അക്ഷരം സ്ഥിരം *ചിഹ്നം)
{
  ശൂന്യം *address = dlsym (handle, ചിഹ്നം);
  എങ്കിൽ (!address)
    link_failure ();
  തിരികെ address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
സ്ഥിതം ശൂന്യം
link_libcrypto (ശൂന്യം)
{
#എങ്കിൽ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  ശൂന്യം *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  എങ്കിൽ (!handle)
    link_failure ();
  ptr_MD5_Init = ചിഹ്നം_address (handle, "MD5_Init");
  ptr_MD5_Update = ചിഹ്നം_address (handle, "MD5_Update");
  ptr_MD5_Final = ചിഹ്നം_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
സ്ഥിതം ഘടന md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

സ്ഥിതം ശൂന്യം
random_md5_state_init (അക്ഷരം സ്ഥിരം *random_source)
{
  unsigned അക്ഷരം buf[MD5_DIGEST_SIZE];
  ഘടന randread_source *r = randread_new (random_source, വലുപ്പം buf);
  എങ്കിൽ (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, വലുപ്പം buf);
  എങ്കിൽ (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, വലുപ്പം buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

സ്ഥിതം size_t
xstrxfrm (അക്ഷരം *restrict dest, അക്ഷരം സ്ഥിരം *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  എങ്കിൽ (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  തിരികെ translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_random (അക്ഷരം *restrict texta, size_t lena,
                അക്ഷരം *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  പൂർണ്ണസംഖ്യ xfrm_diff = 0;

  അക്ഷരം stackbuf[4000];
  അക്ഷരം *buf = stackbuf;
  size_t bufsize = വലുപ്പം stackbuf;
  ശൂന്യം *allocated = ശൂന്യ;
  uint32_t dig[2][MD5_DIGEST_SIZE / വലുപ്പം (uint32_t)];
  ഘടന md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  എങ്കിൽ (hard_LC_COLLATE)
    {
      അക്ഷരം സ്ഥിരം *lima = texta + lena;
      അക്ഷരം സ്ഥിരം *limb = textb + lenb;

      ആകുമ്പോൾ (true)
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
          എങ്കിൽ (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              സ്വതന്ത്രം (allocated);
              buf = allocated = malloc (bufsize);
              എങ്കിൽ (! buf)
                {
                  buf = stackbuf;
                  bufsize = വലുപ്പം stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : ശൂന്യ), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          എങ്കിൽ (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              എങ്കിൽ (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              സ്വതന്ത്രം (allocated);
              buf = allocated = xmalloc (bufsize);
              എങ്കിൽ (texta < lima)
                strxfrm (buf, texta, sizea);
              എങ്കിൽ (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          എങ്കിൽ (texta < lima)
            texta += strlen (texta) + 1;
          എങ്കിൽ (textb < limb)
            textb += strlen (textb) + 1;
          എങ്കിൽ (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              നിർത്തുക;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          എങ്കിൽ (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              എങ്കിൽ (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  പൂർണ്ണസംഖ്യ diff = memcmp (dig[0], dig[1], വലുപ്പം dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  എങ്കിൽ (! diff)
    {
      എങ്കിൽ (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          എങ്കിൽ (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  സ്വതന്ത്രം (allocated);

  തിരികെ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

സ്ഥിതം size_t
debug_width (അക്ഷരം സ്ഥിരം *text, അക്ഷരം സ്ഥിരം *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  ആകുമ്പോൾ (text < lim)
    width += (*text++ == '\t');
  തിരികെ width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

സ്ഥിതം ശൂന്യം
mark_താക്കോൽ (size_t offset, size_t width)
{
  ആകുമ്പോൾ (offset--)
    putchar (' ');

  എങ്കിൽ (!width)
    printf (_("^ no match for key\n"));
  അല്ലെങ്കിൽ
    {
      ചെയ്യുക
        putchar ('_');
      ആകുമ്പോൾ (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

സ്ഥിതം വരിയിൽ bool
താക്കോൽ_numeric (ഘടന keyfield സ്ഥിരം *താക്കോൽ)
{
  തിരികെ താക്കോൽ->numeric || താക്കോൽ->general_numeric || താക്കോൽ->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

സ്ഥിതം ശൂന്യം
debug_താക്കോൽ (ഘടന വരി സ്ഥിരം *വരി, ഘടന keyfield സ്ഥിരം *താക്കോൽ)
{
  അക്ഷരം *text = വരി->text;
  അക്ഷരം *beg = text;
  അക്ഷരം *lim = text + വരി->length - 1;

  എങ്കിൽ (താക്കോൽ)
    {
      എങ്കിൽ (താക്കോൽ->sword != SIZE_MAX)
        beg = begfield (വരി, താക്കോൽ);
      എങ്കിൽ (താക്കോൽ->eword != SIZE_MAX)
        {
          lim = limfield (വരി, താക്കോൽ);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      എങ്കിൽ ((താക്കോൽ->skipsblanks && താക്കോൽ->sword == SIZE_MAX)
          || താക്കോൽ->month || താക്കോൽ_numeric (താക്കോൽ))
        {
          അക്ഷരം saved = *lim;
          *lim = '\0';

          ആകുമ്പോൾ (blanks[to_uchar (*beg)])
            beg++;

          അക്ഷരം *tighter_lim = beg;

          എങ്കിൽ (lim < beg)
            tighter_lim = lim;
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->month)
            getmonth (beg, &tighter_lim);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->general_numeric)
            ignore_മൂല്യം (strtold (beg, &tighter_lim));
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->numeric || താക്കോൽ->human_numeric)
            {
              അക്ഷരം സ്ഥിരം *p = beg + (beg < lim && *beg == '-');
              അക്ഷരം max_digit = traverse_raw_സംഖ്യ (&p);
              എങ്കിൽ ('0' <= max_digit)
                {
                  unsigned അക്ഷരം ch = *p;
                  tighter_lim = (അക്ഷരം *) p
                    + (താക്കോൽ->human_numeric && unit_order[ch]);
                }
            }
          അല്ലെങ്കിൽ
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_താക്കോൽ (offset, width);
}

/* Debug LINE by underlining its keys.  */

സ്ഥിതം ശൂന്യം
debug_വരി (ഘടന വരി സ്ഥിരം *വരി)
{
  ഘടന keyfield സ്ഥിരം *താക്കോൽ = keylist;

  ചെയ്യുക
    debug_താക്കോൽ (വരി, താക്കോൽ);
  ആകുമ്പോൾ (താക്കോൽ && ((താക്കോൽ = താക്കോൽ->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

സ്ഥിതം bool
default_താക്കോൽ_compare (ഘടന keyfield സ്ഥിരം *താക്കോൽ)
{
  തിരികെ ! (താക്കോൽ->ignore
            || താക്കോൽ->translate
            || താക്കോൽ->skipsblanks
            || താക്കോൽ->skipeblanks
            || താക്കോൽ_numeric (താക്കോൽ)
            || താക്കോൽ->month
            || താക്കോൽ->version
            || താക്കോൽ->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

സ്ഥിതം ശൂന്യം
താക്കോൽ_to_opts (ഘടന keyfield സ്ഥിരം *താക്കോൽ, അക്ഷരം *opts)
{
  എങ്കിൽ (താക്കോൽ->skipsblanks || താക്കോൽ->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  എങ്കിൽ (താക്കോൽ->ignore == nondictionary)
    *opts++ = 'd';
  എങ്കിൽ (താക്കോൽ->translate)
    *opts++ = 'f';
  എങ്കിൽ (താക്കോൽ->general_numeric)
    *opts++ = 'g';
  എങ്കിൽ (താക്കോൽ->human_numeric)
    *opts++ = 'h';
  എങ്കിൽ (താക്കോൽ->ignore == nonprinting)
    *opts++ = 'i';
  എങ്കിൽ (താക്കോൽ->month)
    *opts++ = 'M';
  എങ്കിൽ (താക്കോൽ->numeric)
    *opts++ = 'n';
  എങ്കിൽ (താക്കോൽ->random)
    *opts++ = 'R';
  എങ്കിൽ (താക്കോൽ->reverse)
    *opts++ = 'r';
  എങ്കിൽ (താക്കോൽ->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

സ്ഥിതം ശൂന്യം
താക്കോൽ_warnings (ഘടന keyfield സ്ഥിരം *gkey, bool gkey_only)
{
  ഘടന keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  വേണ്ടി (ഘടന keyfield *താക്കോൽ = keylist; താക്കോൽ; താക്കോൽ = താക്കോൽ->next, keynum++)
    {
      എങ്കിൽ (താക്കോൽ_numeric (താക്കോൽ))
        {
          എങ്കിൽ (താക്കോൽ->general_numeric)
            general_numeric_field = true;
          അല്ലെങ്കിൽ
            basic_numeric_field = true;
        }

      എങ്കിൽ (താക്കോൽ->traditional_used)
        {
          size_t sword = താക്കോൽ->sword;
          size_t eword = താക്കോൽ->eword;
          അക്ഷരം tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          അക്ഷരം obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          അക്ഷരം nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          അക്ഷരം *po = obuf;
          അക്ഷരം *pn = nbuf;

          എങ്കിൽ (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          എങ്കിൽ (താക്കോൽ->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (താക്കോൽ->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = താക്കോൽ->sword != SIZE_MAX && താക്കോൽ->eword < താക്കോൽ->sword;
      എങ്കിൽ (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = താക്കോൽ_numeric (താക്കോൽ) || താക്കോൽ->month;
      bool വരി_offset = താക്കോൽ->eword == 0 && താക്കോൽ->echar != 0; /* -k1.x,1.y  */
      എങ്കിൽ (!zero_width && !gkey_only && tab == TAB_DEFAULT && !വരി_offset
          && ((!താക്കോൽ->skipsblanks && !implicit_skip)
              || (!താക്കോൽ->skipsblanks && താക്കോൽ->schar)
              || (!താക്കോൽ->skipeblanks && താക്കോൽ->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      എങ്കിൽ (!gkey_only && താക്കോൽ_numeric (താക്കോൽ))
        {
          size_t sword = താക്കോൽ->sword + 1;
          size_t eword = താക്കോൽ->eword + 1;
          എങ്കിൽ (!sword)
            sword++;
          എങ്കിൽ (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              എങ്കിൽ (താക്കോൽ->general_numeric)
                general_numeric_field_span = true;
              അല്ലെങ്കിൽ
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      എങ്കിൽ (ugkey.ignore && (ugkey.ignore == താക്കോൽ->ignore))
        ugkey.ignore = ശൂന്യ;
      എങ്കിൽ (ugkey.translate && (ugkey.translate == താക്കോൽ->translate))
        ugkey.translate = ശൂന്യ;
      ugkey.skipsblanks &= !താക്കോൽ->skipsblanks;
      ugkey.skipeblanks &= !താക്കോൽ->skipeblanks;
      ugkey.month &= !താക്കോൽ->month;
      ugkey.numeric &= !താക്കോൽ->numeric;
      ugkey.general_numeric &= !താക്കോൽ->general_numeric;
      ugkey.human_numeric &= !താക്കോൽ->human_numeric;
      ugkey.random &= !താക്കോൽ->random;
      ugkey.version &= !താക്കോൽ->version;
      ugkey.reverse &= !താക്കോൽ->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool സംഖ്യ_locale_warned = false;
  എങ്കിൽ (basic_numeric_field_span)
    {
      എങ്കിൽ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((അക്ഷരം []) {thousands_sep, 0})));
          സംഖ്യ_locale_warned = true;
        }
    }
  എങ്കിൽ (basic_numeric_field_span || general_numeric_field_span)
    {
      എങ്കിൽ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((അക്ഷരം []) {decimal_point, 0})));
          സംഖ്യ_locale_warned = true;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((അക്ഷരം []) {tab, 0})));
        }
      അല്ലെങ്കിൽ എങ്കിൽ (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((അക്ഷരം []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  എങ്കിൽ ((basic_numeric_field || general_numeric_field) && ! സംഖ്യ_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((അക്ഷരം []) {decimal_point, 0})));

    }

  എങ്കിൽ (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  എങ്കിൽ (!default_താക്കോൽ_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      എങ്കിൽ (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      അക്ഷരം opts[വലുപ്പം short_options];
      താക്കോൽ_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  എങ്കിൽ (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
diff_reversed (പൂർണ്ണസംഖ്യ diff, bool reversed)
{
  തിരികെ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

സ്ഥിതം പൂർണ്ണസംഖ്യ
keycompare (ഘടന വരി സ്ഥിരം *a, ഘടന വരി സ്ഥിരം *b)
{
  ഘടന keyfield *താക്കോൽ = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  അക്ഷരം *texta = a->keybeg;
  അക്ഷരം *textb = b->keybeg;
  അക്ഷരം *lima = a->keylim;
  അക്ഷരം *limb = b->keylim;

  പൂർണ്ണസംഖ്യ diff;

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം സ്ഥിരം *translate = താക്കോൽ->translate;
      bool സ്ഥിരം *ignore = താക്കോൽ->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      എങ്കിൽ (hard_LC_COLLATE || താക്കോൽ_numeric (താക്കോൽ)
          || താക്കോൽ->month || താക്കോൽ->random || താക്കോൽ->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          അക്ഷരം *ta = texta;
          അക്ഷരം *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          അക്ഷരം enda = ta[tlena];
          അക്ഷരം endb = tb[tlenb];

          ശൂന്യം *allocated = ശൂന്യ;
          അക്ഷരം stackbuf[4000];

          എങ്കിൽ (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              എങ്കിൽ (size <= വലുപ്പം stackbuf)
                ta = stackbuf;
              അല്ലെങ്കിൽ
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              വേണ്ടി (tlena = i = 0; i < lena; i++)
                എങ്കിൽ (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              വേണ്ടി (tlenb = i = 0; i < lenb; i++)
                എങ്കിൽ (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          എങ്കിൽ (താക്കോൽ->numeric)
            diff = numcompare (ta, tb);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->general_numeric)
            diff = general_numcompare (ta, tb);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->human_numeric)
            diff = human_numcompare (ta, tb);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->month)
            diff = getmonth (ta, ശൂന്യ) - getmonth (tb, ശൂന്യ);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          അല്ലെങ്കിൽ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              എങ്കിൽ (tlena == 0)
                diff = - NONZERO (tlenb);
              അല്ലെങ്കിൽ എങ്കിൽ (tlenb == 0)
                diff = 1;
              അല്ലെങ്കിൽ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          സ്വതന്ത്രം (allocated);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  ചെയ്യുക									\
    {									\
          ആകുമ്പോൾ (true)							\
            {								\
              ആകുമ്പോൾ (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              ആകുമ്പോൾ (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              എങ്കിൽ (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  നിർത്തുക;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              എങ്കിൽ (diff)							\
                നിർത്തുക;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ആകുമ്പോൾ (0)

          എങ്കിൽ (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          അല്ലെങ്കിൽ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      അല്ലെങ്കിൽ
        {
          size_t lenmin = MIN (lena, lenb);
          എങ്കിൽ (lenmin == 0)
            diff = 0;
          അല്ലെങ്കിൽ എങ്കിൽ (translate)
            {
              size_t i = 0;
              ചെയ്യുക
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  എങ്കിൽ (diff)
                    നിർത്തുക;
                  i++;
                }
              ആകുമ്പോൾ (i < lenmin);
            }
          അല്ലെങ്കിൽ
            diff = memcmp (texta, textb, lenmin);

          എങ്കിൽ (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      എങ്കിൽ (diff)
        നിർത്തുക;

      താക്കോൽ = താക്കോൽ->next;
      എങ്കിൽ (! താക്കോൽ)
        തിരികെ 0;

      /* Find the beginning and limit of the next field.  */
      എങ്കിൽ (താക്കോൽ->eword != SIZE_MAX)
        lima = limfield (a, താക്കോൽ), limb = limfield (b, താക്കോൽ);
      അല്ലെങ്കിൽ
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      എങ്കിൽ (താക്കോൽ->sword != SIZE_MAX)
        texta = begfield (a, താക്കോൽ), textb = begfield (b, താക്കോൽ);
      അല്ലെങ്കിൽ
        {
          texta = a->text, textb = b->text;
          എങ്കിൽ (താക്കോൽ->skipsblanks)
            {
              ആകുമ്പോൾ (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              ആകുമ്പോൾ (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  തിരികെ diff_reversed (diff, താക്കോൽ->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare (ഘടന വരി സ്ഥിരം *a, ഘടന വരി സ്ഥിരം *b)
{
  പൂർണ്ണസംഖ്യ diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  എങ്കിൽ (keylist)
    {
      diff = keycompare (a, b);
      എങ്കിൽ (diff || unique || stable)
        തിരികെ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  എങ്കിൽ (alen == 0)
    diff = - NONZERO (blen);
  അല്ലെങ്കിൽ എങ്കിൽ (blen == 0)
    diff = 1;
  അല്ലെങ്കിൽ എങ്കിൽ (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  അല്ലെങ്കിൽ
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      എങ്കിൽ (!diff)
        diff = _GL_CMP (alen, blen);
    }

  തിരികെ diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

സ്ഥിതം ശൂന്യം
write_വരി (ഘടന വരി സ്ഥിരം *വരി, FILE *fp, അക്ഷരം സ്ഥിരം *output_file)
{
  അക്ഷരം *buf = വരി->text;
  size_t n_bytes = വരി->length;
  അക്ഷരം *ebuf = buf + n_bytes;

  എങ്കിൽ (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      അക്ഷരം സ്ഥിരം *c = buf;

      ആകുമ്പോൾ (c < ebuf)
        {
          അക്ഷരം wc = *c++;
          എങ്കിൽ (wc == '\t')
            wc = '>';
          അല്ലെങ്കിൽ എങ്കിൽ (c == ebuf)
            wc = '\n';
          എങ്കിൽ (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_വരി (വരി);
    }
  അല്ലെങ്കിൽ
    {
      ebuf[-1] = eolchar;
      എങ്കിൽ (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

സ്ഥിതം bool
check (അക്ഷരം സ്ഥിരം *file_name, അക്ഷരം checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ഘടന buffer buf;		/* Input buffer. */
  ഘടന വരി temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t വരി_സംഖ്യ = 0;
  ഘടന keyfield സ്ഥിരം *താക്കോൽ = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, വലുപ്പം (ഘടന വരി),
           MAX (merge_buffer_size, sort_size));
  temp.text = ശൂന്യ;

  ആകുമ്പോൾ (fillbuf (&buf, fp, file_name, ശൂന്യ))
    {
      ഘടന വരി സ്ഥിരം *വരി = buffer_linelim (&buf);
      ഘടന വരി സ്ഥിരം *linebase = വരി - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      എങ്കിൽ (alloc && nonunique <= compare (&temp, വരി - 1))
        {
        found_disorder:
          {
            എങ്കിൽ (checkonly == 'c')
              {
                ഘടന വരി സ്ഥിരം *disorder_വരി = വരി - 1;
                uintmax_t disorder_വരി_സംഖ്യ =
                  buffer_linelim (&buf) - disorder_വരി + വരി_സംഖ്യ;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_വരി_സംഖ്യ);
                write_വരി (disorder_വരി, stderr, _("standard error"));
              }

            ordered = false;
            നിർത്തുക;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ആകുമ്പോൾ (linebase < --വരി)
        എങ്കിൽ (nonunique <= compare (വരി, വരി - 1))
          പോകുക found_disorder;

      വരി_സംഖ്യ += buf.nlines;

      /* Save the last line of the buffer.  */
      എങ്കിൽ (alloc < വരി->length)
        {
          ചെയ്യുക
            {
              alloc *= 2;
              എങ്കിൽ (! alloc)
                {
                  alloc = വരി->length;
                  നിർത്തുക;
                }
            }
          ആകുമ്പോൾ (alloc < വരി->length);

          സ്വതന്ത്രം (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, വരി->text, വരി->length);
      temp.length = വരി->length;
      എങ്കിൽ (താക്കോൽ)
        {
          temp.keybeg = temp.text + (വരി->keybeg - വരി->text);
          temp.keylim = temp.text + (വരി->keylim - വരി->text);
        }
    }

  xfclose (fp, file_name);
  സ്വതന്ത്രം (buf.buf);
  സ്വതന്ത്രം (temp.text);
  തിരികെ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

സ്ഥിതം size_t
open_നിവേശം_files (ഘടന sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, വലുപ്പം *fps);
  പൂർണ്ണസംഖ്യ i;

  /* Open as many input files as we can.  */
  വേണ്ടി (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      എങ്കിൽ (!fps[i])
        നിർത്തുക;
    }

  തിരികെ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

സ്ഥിതം ശൂന്യം
mergefps (ഘടന sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, അക്ഷരം സ്ഥിരം *output_file, FILE **fps)
{
  ഘടന buffer *buffer = xnmalloc (nfiles, വലുപ്പം *buffer);
                                /* Input buffers for each file. */
  ഘടന വരി saved;		/* Saved line storage for unique check. */
  ഘടന വരി സ്ഥിരം *savedline = ശൂന്യ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ഘടന വരി സ്ഥിരം **cur = xnmalloc (nfiles, വലുപ്പം *cur);
                                /* Current line in each line table. */
  ഘടന വരി സ്ഥിരം **base = xnmalloc (nfiles, വലുപ്പം *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, വലുപ്പം *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ഘടന keyfield സ്ഥിരം *താക്കോൽ = keylist;
  saved.text = ശൂന്യ;

  /* Read initial lines from each input file. */
  വേണ്ടി (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], വലുപ്പം (ഘടന വരി),
               MAX (merge_buffer_size, sort_size / nfiles));
      എങ്കിൽ (fillbuf (&buffer[i], fps[i], files[i].name, ശൂന്യ))
        {
          ഘടന വരി സ്ഥിരം *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      അല്ലെങ്കിൽ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          എങ്കിൽ (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          സ്വതന്ത്രം (buffer[i].buf);
          --nfiles;
          വേണ്ടി (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  വേണ്ടി (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  വേണ്ടി (size_t i = 1; i < nfiles; ++i)
    എങ്കിൽ (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ആകുമ്പോൾ (nfiles)
    {
      ഘടന വരി സ്ഥിരം *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      എങ്കിൽ (unique)
        {
          എങ്കിൽ (savedline && compare (savedline, smallest))
            {
              savedline = ശൂന്യ;
              write_വരി (&saved, ofp, output_file);
            }
          എങ്കിൽ (!savedline)
            {
              savedline = &saved;
              എങ്കിൽ (savealloc < smallest->length)
                {
                  ചെയ്യുക
                    എങ്കിൽ (! savealloc)
                      {
                        savealloc = smallest->length;
                        നിർത്തുക;
                      }
                  ആകുമ്പോൾ ((savealloc *= 2) < smallest->length);

                  സ്വതന്ത്രം (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              എങ്കിൽ (താക്കോൽ)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      അല്ലെങ്കിൽ
        write_വരി (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      എങ്കിൽ (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ശൂന്യ))
            {
              ഘടന വരി സ്ഥിരം *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          അല്ലെങ്കിൽ
            {
              /* We reached EOF on fps[ord[0]].  */
              വേണ്ടി (size_t i = 1; i < nfiles; ++i)
                എങ്കിൽ (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              എങ്കിൽ (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              സ്വതന്ത്രം (buffer[ord[0]].buf);
              വേണ്ടി (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              വേണ്ടി (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              തുടരുക;
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
        size_t എണ്ണം_of_smaller_വരികൾ;

        ആകുമ്പോൾ (lo < hi)
          {
            പൂർണ്ണസംഖ്യ cmp = compare (cur[ord0], cur[ord[probe]]);
            എങ്കിൽ (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            അല്ലെങ്കിൽ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        എണ്ണം_of_smaller_വരികൾ = lo - 1;
        വേണ്ടി (size_t j = 0; j < എണ്ണം_of_smaller_വരികൾ; j++)
          ord[j] = ord[j + 1];
        ord[എണ്ണം_of_smaller_വരികൾ] = ord0;
      }
    }

  എങ്കിൽ (unique && savedline)
    {
      write_വരി (&saved, ofp, output_file);
      സ്വതന്ത്രം (saved.text);
    }

  xfclose (ofp, output_file);
  സ്വതന്ത്രം (fps);
  സ്വതന്ത്രം (buffer);
  സ്വതന്ത്രം (ord);
  സ്വതന്ത്രം (base);
  സ്വതന്ത്രം (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

സ്ഥിതം size_t
mergefiles (ഘടന sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, അക്ഷരം സ്ഥിരം *output_file)
{
  FILE **fps;
  size_t nopened = open_നിവേശം_files (files, nfiles, &fps);
  എങ്കിൽ (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  തിരികെ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

സ്ഥിതം ശൂന്യം
mergelines (ഘടന വരി *restrict t, size_t nlines,
            ഘടന വരി സ്ഥിരം *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ഘടന വരി *hi = t - nlo;

  ആകുമ്പോൾ (true)
    എങ്കിൽ (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        എങ്കിൽ (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            തിരികെ;
          }
      }
    അല്ലെങ്കിൽ
      {
        *--t = *--hi;
        എങ്കിൽ (! --nhi)
          {
            ചെയ്യുക
              *--t = *--lo;
            ആകുമ്പോൾ (--nlo);

            തിരികെ;
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

സ്ഥിതം ശൂന്യം
sequential_sort (ഘടന വരി *restrict വരികൾ, size_t nlines,
                 ഘടന വരി *restrict temp, bool to_temp)
{
  എങ്കിൽ (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      പൂർണ്ണസംഖ്യ swap = (0 < compare (&വരികൾ[-1], &വരികൾ[-2]));
      എങ്കിൽ (to_temp)
        {
          temp[-1] = വരികൾ[-1 - swap];
          temp[-2] = വരികൾ[-2 + swap];
        }
      അല്ലെങ്കിൽ എങ്കിൽ (swap)
        {
          temp[-1] = വരികൾ[-1];
          വരികൾ[-1] = വരികൾ[-2];
          വരികൾ[-2] = temp[-1];
        }
    }
  അല്ലെങ്കിൽ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ഘടന വരി *lo = വരികൾ;
      ഘടന വരി *hi = വരികൾ - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      എങ്കിൽ (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      അല്ലെങ്കിൽ എങ്കിൽ (!to_temp)
        temp[-1] = lo[-1];

      ഘടന വരി *dest;
      ഘടന വരി സ്ഥിരം *sorted_lo;
      എങ്കിൽ (to_temp)
        {
          dest = temp;
          sorted_lo = വരികൾ;
        }
      അല്ലെങ്കിൽ
        {
          dest = വരികൾ;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

സ്ഥിതം ഘടന merge_കണ്ണി *init_കണ്ണി (ഘടന merge_കണ്ണി *restrict,
                                     ഘടന merge_കണ്ണി *restrict,
                                     ഘടന വരി *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
സ്ഥിതം ഘടന merge_കണ്ണി *
merge_വൃക്ഷം_init (size_t nthreads, size_t nlines, ഘടന വരി *dest)
{
  ഘടന merge_കണ്ണി *merge_വൃക്ഷം = xmalloc (2 * വലുപ്പം *merge_വൃക്ഷം * nthreads);

  ഘടന merge_കണ്ണി *വേര് = merge_വൃക്ഷം;
  വേര്->lo = വേര്->hi = വേര്->end_lo = വേര്->end_hi = ശൂന്യ;
  വേര്->dest = ശൂന്യ;
  വേര്->nlo = വേര്->nhi = nlines;
  വേര്->parent = ശൂന്യ;
  വേര്->level = MERGE_END;
  വേര്->queued = false;
  pthread_mutex_init (&വേര്->lock, ശൂന്യ);

  init_കണ്ണി (വേര്, വേര് + 1, dest, nthreads, nlines, false);
  തിരികെ merge_വൃക്ഷം;
}

/* Destroy the merge tree. */
സ്ഥിതം ശൂന്യം
merge_വൃക്ഷം_destroy (size_t nthreads, ഘടന merge_കണ്ണി *merge_വൃക്ഷം)
{
  size_t n_nodes = nthreads * 2;
  ഘടന merge_കണ്ണി *കണ്ണി = merge_വൃക്ഷം;

  ആകുമ്പോൾ (n_nodes--)
    {
      pthread_mutex_destroy (&കണ്ണി->lock);
      കണ്ണി++;
    }

  സ്വതന്ത്രം (merge_വൃക്ഷം);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

സ്ഥിതം ഘടന merge_കണ്ണി *
init_കണ്ണി (ഘടന merge_കണ്ണി *restrict parent,
           ഘടന merge_കണ്ണി *restrict കണ്ണി_pool,
           ഘടന വരി *dest, size_t nthreads,
           size_t total_വരികൾ, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ഘടന വരി *lo = dest - total_വരികൾ;
  ഘടന വരി *hi = lo - nlo;
  ഘടന വരി **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  ഘടന merge_കണ്ണി *കണ്ണി = കണ്ണി_pool++;
  കണ്ണി->lo = കണ്ണി->end_lo = lo;
  കണ്ണി->hi = കണ്ണി->end_hi = hi;
  കണ്ണി->dest = parent_end;
  കണ്ണി->nlo = nlo;
  കണ്ണി->nhi = nhi;
  കണ്ണി->parent = parent;
  കണ്ണി->level = parent->level + 1;
  കണ്ണി->queued = false;
  pthread_mutex_init (&കണ്ണി->lock, ശൂന്യ);

  എങ്കിൽ (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      കണ്ണി->lo_child = കണ്ണി_pool;
      കണ്ണി_pool = init_കണ്ണി (കണ്ണി, കണ്ണി_pool, lo, lo_threads,
                             total_വരികൾ, true);
      കണ്ണി->hi_child = കണ്ണി_pool;
      കണ്ണി_pool = init_കണ്ണി (കണ്ണി, കണ്ണി_pool, hi, hi_threads,
                             total_വരികൾ, false);
    }
  അല്ലെങ്കിൽ
    {
      കണ്ണി->lo_child = ശൂന്യ;
      കണ്ണി->hi_child = ശൂന്യ;
    }
  തിരികെ കണ്ണി_pool;
}


/* Compare two merge nodes A and B for priority.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_nodes (ശൂന്യം സ്ഥിരം *a, ശൂന്യം സ്ഥിരം *b)
{
  ഘടന merge_കണ്ണി സ്ഥിരം *nodea = a;
  ഘടന merge_കണ്ണി സ്ഥിരം *nodeb = b;
  എങ്കിൽ (nodea->level == nodeb->level)
      തിരികെ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  തിരികെ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

സ്ഥിതം വരിയിൽ ശൂന്യം
lock_കണ്ണി (ഘടന merge_കണ്ണി *കണ്ണി)
{
  pthread_mutex_lock (&കണ്ണി->lock);
}

/* Unlock a merge tree NODE. */

സ്ഥിതം വരിയിൽ ശൂന്യം
unlock_കണ്ണി (ഘടന merge_കണ്ണി *കണ്ണി)
{
  pthread_mutex_unlock (&കണ്ണി->lock);
}

/* Destroy merge QUEUE. */

സ്ഥിതം ശൂന്യം
queue_destroy (ഘടന merge_കണ്ണി_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

സ്ഥിതം ശൂന്യം
queue_init (ഘടന merge_കണ്ണി_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, ശൂന്യ);
  pthread_cond_init (&queue->cond, ശൂന്യ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

സ്ഥിതം ശൂന്യം
queue_തിരുകുക (ഘടന merge_കണ്ണി_queue *queue, ഘടന merge_കണ്ണി *കണ്ണി)
{
  pthread_mutex_lock (&queue->mutex);
  heap_തിരുകുക (queue->priority_queue, കണ്ണി);
  കണ്ണി->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

സ്ഥിതം ഘടന merge_കണ്ണി *
queue_എടുക്കുക (ഘടന merge_കണ്ണി_queue *queue)
{
  ഘടന merge_കണ്ണി *കണ്ണി;
  pthread_mutex_lock (&queue->mutex);
  ആകുമ്പോൾ (! (കണ്ണി = heap_remove_മുകൾ (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_കണ്ണി (കണ്ണി);
  കണ്ണി->queued = false;
  തിരികെ കണ്ണി;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

സ്ഥിതം ശൂന്യം
write_unique (ഘടന വരി സ്ഥിരം *വരി, FILE *tfp, അക്ഷരം സ്ഥിരം *temp_output)
{
  എങ്കിൽ (unique)
    {
      എങ്കിൽ (saved_വരി.text && ! compare (വരി, &saved_വരി))
        തിരികെ;
      saved_വരി = *വരി;
    }

  write_വരി (വരി, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

സ്ഥിതം ശൂന്യം
mergelines_കണ്ണി (ഘടന merge_കണ്ണി *restrict കണ്ണി, size_t total_വരികൾ,
                 FILE *tfp, അക്ഷരം സ്ഥിരം *temp_output)
{
  ഘടന വരി *lo_orig = കണ്ണി->lo;
  ഘടന വരി *hi_orig = കണ്ണി->hi;
  size_t to_merge = MAX_MERGE (total_വരികൾ, കണ്ണി->level);
  size_t merged_lo;
  size_t merged_hi;

  എങ്കിൽ (കണ്ണി->level > MERGE_വേര്)
    {
      /* Merge to destination buffer. */
      ഘടന വരി *dest = *കണ്ണി->dest;
      ആകുമ്പോൾ (കണ്ണി->lo != കണ്ണി->end_lo && കണ്ണി->hi != കണ്ണി->end_hi && to_merge--)
        എങ്കിൽ (compare (കണ്ണി->lo - 1, കണ്ണി->hi - 1) <= 0)
          *--dest = *--കണ്ണി->lo;
        അല്ലെങ്കിൽ
          *--dest = *--കണ്ണി->hi;

      merged_lo = lo_orig - കണ്ണി->lo;
      merged_hi = hi_orig - കണ്ണി->hi;

      എങ്കിൽ (കണ്ണി->nhi == merged_hi)
        ആകുമ്പോൾ (കണ്ണി->lo != കണ്ണി->end_lo && to_merge--)
          *--dest = *--കണ്ണി->lo;
      അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->nlo == merged_lo)
        ആകുമ്പോൾ (കണ്ണി->hi != കണ്ണി->end_hi && to_merge--)
          *--dest = *--കണ്ണി->hi;
      *കണ്ണി->dest = dest;
    }
  അല്ലെങ്കിൽ
    {
      /* Merge directly to output. */
      ആകുമ്പോൾ (കണ്ണി->lo != കണ്ണി->end_lo && കണ്ണി->hi != കണ്ണി->end_hi && to_merge--)
        {
          എങ്കിൽ (compare (കണ്ണി->lo - 1, കണ്ണി->hi - 1) <= 0)
            write_unique (--കണ്ണി->lo, tfp, temp_output);
          അല്ലെങ്കിൽ
            write_unique (--കണ്ണി->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - കണ്ണി->lo;
      merged_hi = hi_orig - കണ്ണി->hi;

      എങ്കിൽ (കണ്ണി->nhi == merged_hi)
        {
          ആകുമ്പോൾ (കണ്ണി->lo != കണ്ണി->end_lo && to_merge--)
            write_unique (--കണ്ണി->lo, tfp, temp_output);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->nlo == merged_lo)
        {
          ആകുമ്പോൾ (കണ്ണി->hi != കണ്ണി->end_hi && to_merge--)
            write_unique (--കണ്ണി->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - കണ്ണി->lo;
  merged_hi = hi_orig - കണ്ണി->hi;
  കണ്ണി->nlo -= merged_lo;
  കണ്ണി->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

സ്ഥിതം ശൂന്യം
queue_check_തിരുകുക (ഘടന merge_കണ്ണി_queue *queue, ഘടന merge_കണ്ണി *കണ്ണി)
{
  എങ്കിൽ (! കണ്ണി->queued)
    {
      bool lo_avail = (കണ്ണി->lo - കണ്ണി->end_lo) != 0;
      bool hi_avail = (കണ്ണി->hi - കണ്ണി->end_hi) != 0;
      എങ്കിൽ (lo_avail ? hi_avail || ! കണ്ണി->nhi : hi_avail && ! കണ്ണി->nlo)
        queue_തിരുകുക (queue, കണ്ണി);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

സ്ഥിതം ശൂന്യം
queue_check_തിരുകുക_parent (ഘടന merge_കണ്ണി_queue *queue,
                           ഘടന merge_കണ്ണി *കണ്ണി)
{
  എങ്കിൽ (കണ്ണി->level > MERGE_വേര്)
    {
      lock_കണ്ണി (കണ്ണി->parent);
      queue_check_തിരുകുക (queue, കണ്ണി->parent);
      unlock_കണ്ണി (കണ്ണി->parent);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->nlo + കണ്ണി->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_തിരുകുക (queue, കണ്ണി->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

സ്ഥിതം ശൂന്യം
merge_loop (ഘടന merge_കണ്ണി_queue *queue,
            size_t total_വരികൾ, FILE *tfp, അക്ഷരം സ്ഥിരം *temp_output)
{
  ആകുമ്പോൾ (true)
    {
      ഘടന merge_കണ്ണി *കണ്ണി = queue_എടുക്കുക (queue);

      എങ്കിൽ (കണ്ണി->level == MERGE_END)
        {
          unlock_കണ്ണി (കണ്ണി);
          /* Reinsert so other threads can pop it. */
          queue_തിരുകുക (queue, കണ്ണി);
          നിർത്തുക;
        }
      mergelines_കണ്ണി (കണ്ണി, total_വരികൾ, tfp, temp_output);
      queue_check_തിരുകുക (queue, കണ്ണി);
      queue_check_തിരുകുക_parent (queue, കണ്ണി);

      unlock_കണ്ണി (കണ്ണി);
    }
}


സ്ഥിതം ശൂന്യം sortlines (ഘടന വരി *restrict, size_t, size_t,
                       ഘടന merge_കണ്ണി *, ഘടന merge_കണ്ണി_queue *,
                       FILE *, അക്ഷരം സ്ഥിരം *);

/* Thread arguments for sortlines_thread. */

ഘടന thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ഘടന വരി *വരികൾ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t സ്ഥിരം total_വരികൾ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ഘടന merge_കണ്ണി *സ്ഥിരം കണ്ണി;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ഘടന merge_കണ്ണി_queue *സ്ഥിരം queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  അക്ഷരം സ്ഥിരം *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

സ്ഥിതം ശൂന്യം *
sortlines_thread (ശൂന്യം *data)
{
  ഘടന thread_args സ്ഥിരം *args = data;
  sortlines (args->വരികൾ, args->nthreads, args->total_വരികൾ,
             args->കണ്ണി, args->queue, args->tfp,
             args->output_temp);
  തിരികെ ശൂന്യ;
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

സ്ഥിതം ശൂന്യം
sortlines (ഘടന വരി *restrict വരികൾ, size_t nthreads,
           size_t total_വരികൾ, ഘടന merge_കണ്ണി *കണ്ണി,
           ഘടന merge_കണ്ണി_queue *queue, FILE *tfp, അക്ഷരം സ്ഥിരം *temp_output)
{
  size_t nlines = കണ്ണി->nlo + കണ്ണി->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  ഘടന thread_args args = {വരികൾ, lo_threads, total_വരികൾ,
                             കണ്ണി->lo_child, queue, tfp, temp_output};

  എങ്കിൽ (nthreads > 1 && SUBTHREAD_വരികൾ_HEURISTIC <= nlines
      && pthread_create (&thread, ശൂന്യ, sortlines_thread, &args) == 0)
    {
      sortlines (വരികൾ - കണ്ണി->nlo, hi_threads, total_വരികൾ,
                 കണ്ണി->hi_child, queue, tfp, temp_output);
      pthread_join (thread, ശൂന്യ);
    }
  അല്ലെങ്കിൽ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = കണ്ണി->nlo;
      size_t nhi = കണ്ണി->nhi;
      ഘടന വരി *temp = വരികൾ - total_വരികൾ;
      എങ്കിൽ (1 < nhi)
        sequential_sort (വരികൾ - nlo, nhi, temp - nlo / 2, false);
      എങ്കിൽ (1 < nlo)
        sequential_sort (വരികൾ, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      കണ്ണി->lo = വരികൾ;
      കണ്ണി->hi = വരികൾ - nlo;
      കണ്ണി->end_lo = വരികൾ - nlo;
      കണ്ണി->end_hi = വരികൾ - nlo - nhi;

      queue_തിരുകുക (queue, കണ്ണി);
      merge_loop (queue, total_വരികൾ, tfp, temp_output);
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

സ്ഥിതം ശൂന്യം
avoid_trashing_നിവേശം (ഘടന sortfile *files, size_t ntemps,
                      size_t nfiles, അക്ഷരം സ്ഥിരം *outfile)
{
  ഘടന tempnode *tempcopy = ശൂന്യ;

  വേണ്ടി (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ഘടന stat instat;

      എങ്കിൽ (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      അല്ലെങ്കിൽ
        {
          ഘടന stat *outst = get_outstatus ();
          എങ്കിൽ (!outst)
            നിർത്തുക;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      എങ്കിൽ (same)
        {
          എങ്കിൽ (! tempcopy)
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

സ്ഥിതം ശൂന്യം
check_inputs (അക്ഷരം *സ്ഥിരം *files, size_t nfiles)
{
  വേണ്ടി (size_t i = 0; i < nfiles; i++)
    {
      എങ്കിൽ (streq (files[i], "-"))
        തുടരുക;

      എങ്കിൽ (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

സ്ഥിതം ശൂന്യം
check_output (അക്ഷരം സ്ഥിരം *outfile)
{
  എങ്കിൽ (outfile)
    {
      പൂർണ്ണസംഖ്യ oflags = O_WRONLY | O_ദ്വിമാന | O_CLOEXEC | O_CREAT;
      പൂർണ്ണസംഖ്യ outfd = open (outfile, oflags, MODE_RW_UGO);
      എങ്കിൽ (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

സ്ഥിതം ശൂന്യം
merge (ഘടന sortfile *files, size_t ntemps, size_t nfiles,
       അക്ഷരം സ്ഥിരം *output_file)
{
  ആകുമ്പോൾ (nmerge < nfiles)
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
      വേണ്ടി (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          ഘടന tempnode *temp = create_temp (&tfp);
          size_t അക്കം_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, അക്കം_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += അക്കം_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      എങ്കിൽ (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ഘടന tempnode *temp = create_temp (&tfp);
          size_t അക്കം_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, അക്കം_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += അക്കം_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * വലുപ്പം *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_നിവേശം (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ആകുമ്പോൾ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_നിവേശം_files (files, nfiles, &fps);

      എങ്കിൽ (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          എങ്കിൽ (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              നിർത്തുക;
            }
          എങ്കിൽ (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      അല്ലെങ്കിൽ എങ്കിൽ (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ഘടന tempnode *temp;
      ചെയ്യുക
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      ആകുമ്പോൾ (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * വലുപ്പം *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

സ്ഥിതം ശൂന്യം
sort (അക്ഷരം *സ്ഥിരം *files, size_t nfiles, അക്ഷരം സ്ഥിരം *output_file,
      size_t nthreads)
{
  ഘടന buffer buf;
  ഘടന sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  ആകുമ്പോൾ (nfiles)
    {
      അക്ഷരം സ്ഥിരം *temp_output;
      അക്ഷരം സ്ഥിരം *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_വരി;
      എങ്കിൽ (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ആകുമ്പോൾ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_വരി = (mult * വലുപ്പം (ഘടന വരി));
        }
      അല്ലെങ്കിൽ
        bytes_per_വരി = വലുപ്പം (ഘടന വരി) * 3 / 2;

      എങ്കിൽ (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_വരി,
                              &policy);
          initbuf (&buf, bytes_per_വരി, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ആകുമ്പോൾ (fillbuf (&buf, fp, file, &policy))
        {
          ഘടന വരി *വരി;

          എങ്കിൽ (buf.eof && nfiles
              && (bytes_per_വരി + 1
                  < (buf.alloc - buf.used - bytes_per_വരി * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ഇടത് = buf.used;
              നിർത്തുക;
            }

          saved_വരി.text = ശൂന്യ;
          വരി = buffer_linelim (&buf);
          എങ്കിൽ (buf.eof && !nfiles && !ntemps && !buf.ഇടത്)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          അല്ലെങ്കിൽ
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          എങ്കിൽ (1 < buf.nlines)
            {
              എങ്കിൽ (nthreads > 1)
                {
                  ഘടന merge_കണ്ണി_queue queue;
                  queue_init (&queue, nthreads);
                  ഘടന merge_കണ്ണി *merge_വൃക്ഷം =
                    merge_വൃക്ഷം_init (nthreads, buf.nlines, വരി);

                  sortlines (വരി, nthreads, buf.nlines, merge_വൃക്ഷം + 1,
                             &queue, tfp, temp_output);

                  merge_വൃക്ഷം_destroy (nthreads, merge_വൃക്ഷം);
                  queue_destroy (&queue);
                }
              അല്ലെങ്കിൽ
                {
                  sequential_sort (വരി, buf.nlines,
                                   വരി - buf.nlines, false);
                  വേണ്ടി (size_t i = 0; i < buf.nlines; i++)
                    write_unique (വരി - i - 1, tfp, temp_output);
                }
            }
          അല്ലെങ്കിൽ
            write_unique (വരി - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          എങ്കിൽ (output_file_created)
            പോകുക finish;
        }
      xfclose (fp, file);
    }

 finish:
  സ്വതന്ത്രം (buf.buf);

  എങ്കിൽ (! output_file_created)
    {
      ഘടന tempnode *കണ്ണി = temphead;
      ഘടന sortfile *tempfiles = xnmalloc (ntemps, വലുപ്പം *tempfiles);
      വേണ്ടി (size_t i = 0; കണ്ണി; i++)
        {
          tempfiles[i].name = കണ്ണി->name;
          tempfiles[i].temp = കണ്ണി;
          കണ്ണി = കണ്ണി->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      സ്വതന്ത്രം (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

സ്ഥിതം ശൂന്യം
insertkey (ഘടന keyfield *താക്കോൽ_arg)
{
  ഘടന keyfield **p;
  ഘടന keyfield *താക്കോൽ = xmemdup (താക്കോൽ_arg, വലുപ്പം *താക്കോൽ);

  വേണ്ടി (p = &keylist; *p; p = &(*p)->next)
    തുടരുക;
  *p = താക്കോൽ;
  താക്കോൽ->next = ശൂന്യ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

സ്ഥിതം ശൂന്യം
badfieldspec (അക്ഷരം സ്ഥിരം *spec, അക്ഷരം സ്ഥിരം *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

സ്ഥിതം ശൂന്യം
incompatible_options (അക്ഷരം സ്ഥിരം *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

സ്ഥിതം ശൂന്യം
check_ordering_compatibility (ശൂന്യം)
{
  വേണ്ടി (ഘടന keyfield *താക്കോൽ = keylist; താക്കോൽ; താക്കോൽ = താക്കോൽ->next)
    എങ്കിൽ (1 < (താക്കോൽ->numeric + താക്കോൽ->general_numeric + താക്കോൽ->human_numeric
             + താക്കോൽ->month + (താക്കോൽ->version | താക്കോൽ->random | !!താക്കോൽ->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        അക്ഷരം opts[വലുപ്പം short_options];
        /* Clear flags we're not interested in.  */
        താക്കോൽ->skipsblanks = താക്കോൽ->skipeblanks = താക്കോൽ->reverse = false;
        താക്കോൽ_to_opts (താക്കോൽ, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *
parse_field_എണ്ണം (അക്ഷരം സ്ഥിരം *string, size_t *val, അക്ഷരം സ്ഥിരം *msgid)
{
  അക്ഷരം *suffix;
  uintmax_t n;

  തിരഞ്ഞെടുപ്പ് (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    അവസ്ഥ LONGINT_OK:
    അവസ്ഥ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      എങ്കിൽ (*val == n)
        നിർത്തുക;
      FALLTHROUGH;
    അവസ്ഥ LONGINT_OVERFLOW:
    അവസ്ഥ LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      നിർത്തുക;

    അവസ്ഥ LONGINT_INVALID:
      എങ്കിൽ (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      തിരികെ ശൂന്യ;
    }

  തിരികെ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

സ്ഥിതം അക്ഷരം *
set_ordering (അക്ഷരം സ്ഥിരം *s, ഘടന keyfield *താക്കോൽ, എണ്ണൽ blanktype blanktype)
{
  ആകുമ്പോൾ (*s)
    {
      തിരഞ്ഞെടുപ്പ് (*s)
        {
        അവസ്ഥ 'b':
          എങ്കിൽ (blanktype == bl_start || blanktype == bl_both)
            താക്കോൽ->skipsblanks = true;
          എങ്കിൽ (blanktype == bl_end || blanktype == bl_both)
            താക്കോൽ->skipeblanks = true;
          നിർത്തുക;
        അവസ്ഥ 'd':
          താക്കോൽ->ignore = nondictionary;
          നിർത്തുക;
        അവസ്ഥ 'f':
          താക്കോൽ->translate = fold_toupper;
          നിർത്തുക;
        അവസ്ഥ 'g':
          താക്കോൽ->general_numeric = true;
          നിർത്തുക;
        അവസ്ഥ 'h':
          താക്കോൽ->human_numeric = true;
          നിർത്തുക;
        അവസ്ഥ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          എങ്കിൽ (! താക്കോൽ->ignore)
            താക്കോൽ->ignore = nonprinting;
          നിർത്തുക;
        അവസ്ഥ 'M':
          താക്കോൽ->month = true;
          നിർത്തുക;
        അവസ്ഥ 'n':
          താക്കോൽ->numeric = true;
          നിർത്തുക;
        അവസ്ഥ 'R':
          താക്കോൽ->random = true;
          നിർത്തുക;
        അവസ്ഥ 'r':
          താക്കോൽ->reverse = true;
          നിർത്തുക;
        അവസ്ഥ 'V':
          താക്കോൽ->version = true;
          നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
          തിരികെ (അക്ഷരം *) s;
        }
      ++s;
    }
  തിരികെ (അക്ഷരം *) s;
}

/* Initialize KEY.  */

സ്ഥിതം ഘടന keyfield *
താക്കോൽ_init (ഘടന keyfield *താക്കോൽ)
{
  memset (താക്കോൽ, 0, വലുപ്പം *താക്കോൽ);
  താക്കോൽ->eword = SIZE_MAX;
  തിരികെ താക്കോൽ;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ഘടന keyfield *താക്കോൽ;
  ഘടന keyfield താക്കോൽ_buf;
  ഘടന keyfield gkey;
  bool gkey_only = false;
  അക്ഷരം സ്ഥിരം *s;
  പൂർണ്ണസംഖ്യ c = 0;
  അക്ഷരം checkonly = 0;
  bool mergeonly = false;
  അക്ഷരം *random_source = ശൂന്യ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ശൂന്യ);
  പൂർണ്ണസംഖ്യ posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  അക്ഷരം **files;
  അക്ഷരം *files_from = ശൂന്യ;
  ഘടന Tokens tok;
  അക്ഷരം സ്ഥിരം *outfile = ശൂന്യ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#എങ്കിൽ HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    ഘടന lconv സ്ഥിരം *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    എങ്കിൽ (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    എങ്കിൽ (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    എങ്കിൽ (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    എണ്ണൽ { nsigs = countof (term_sig) };

    ഘടന sigaction act;

    sigemptyset (&caught_signals);
    വേണ്ടി (size_t i = 0; i < nsigs; i++)
      {
        എങ്കിൽ (term_sig[i] == SIGPIPE)
          തുടരുക;  /* Handled below.  */
        sigaction (term_sig[i], ശൂന്യ, &act);
        എങ്കിൽ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    വേണ്ടി (size_t i = 0; i < nsigs; i++)
      {
        എങ്കിൽ (term_sig[i] == SIGPIPE)
          തുടരുക;  /* Handled below.  */
        എങ്കിൽ (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, ശൂന്യ);
      }
  }
  സൂചന (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  താക്കോൽ_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, വലുപ്പം *files);

  ആകുമ്പോൾ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      പൂർണ്ണസംഖ്യ oi = -1;

      എങ്കിൽ (c == -1
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
          എങ്കിൽ (argc <= optind)
            നിർത്തുക;
          files[nfiles++] = argv[optind++];
        }
      അല്ലെങ്കിൽ തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 1:
          താക്കോൽ = ശൂന്യ;
          എങ്കിൽ (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              എങ്കിൽ (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  താക്കോൽ = താക്കോൽ_init (&താക്കോൽ_buf);
                  s = parse_field_എണ്ണം (optarg + 1, &താക്കോൽ->sword, ശൂന്യ);
                  എങ്കിൽ (s && *s == '.')
                    s = parse_field_എണ്ണം (s + 1, &താക്കോൽ->schar, ശൂന്യ);
                  എങ്കിൽ (! (താക്കോൽ->sword || താക്കോൽ->schar))
                    താക്കോൽ->sword = SIZE_MAX;
                  എങ്കിൽ (! s || *set_ordering (s, താക്കോൽ, bl_start))
                    താക്കോൽ = ശൂന്യ;
                  അല്ലെങ്കിൽ
                    {
                      എങ്കിൽ (minus_pos_usage)
                        {
                          അക്ഷരം സ്ഥിരം *optarg1 = argv[optind++];
                          s = parse_field_എണ്ണം (optarg1 + 1, &താക്കോൽ->eword,
                                             N_("invalid number after '-'"));
                          എങ്കിൽ (*s == '.')
                            s = parse_field_എണ്ണം (s + 1, &താക്കോൽ->echar,
                                               N_("invalid number after '.'"));
                          എങ്കിൽ (!താക്കോൽ->echar && താക്കോൽ->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              താക്കോൽ->eword--;
                            }
                          എങ്കിൽ (*set_ordering (s, താക്കോൽ, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      താക്കോൽ->traditional_used = true;
                      insertkey (താക്കോൽ);
                    }
                }
            }
          എങ്കിൽ (! താക്കോൽ)
            files[nfiles++] = optarg;
          നിർത്തുക;

        അവസ്ഥ SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        അവസ്ഥ 'b':
        അവസ്ഥ 'd':
        അവസ്ഥ 'f':
        അവസ്ഥ 'g':
        അവസ്ഥ 'h':
        അവസ്ഥ 'i':
        അവസ്ഥ 'M':
        അവസ്ഥ 'n':
        അവസ്ഥ 'r':
        അവസ്ഥ 'R':
        അവസ്ഥ 'V':
          {
            അക്ഷരം str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          നിർത്തുക;

        അവസ്ഥ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        അവസ്ഥ 'c':
        അവസ്ഥ 'C':
          എങ്കിൽ (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          നിർത്തുക;

        അവസ്ഥ COMPRESS_PROGRAM_OPTION:
          എങ്കിൽ (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          നിർത്തുക;

        അവസ്ഥ DEBUG_PROGRAM_OPTION:
          debug = true;
          നിർത്തുക;

        അവസ്ഥ FILES0_FROM_OPTION:
          files_from = optarg;
          നിർത്തുക;

        അവസ്ഥ 'k':
          താക്കോൽ = താക്കോൽ_init (&താക്കോൽ_buf);

          /* Get POS1. */
          s = parse_field_എണ്ണം (optarg, &താക്കോൽ->sword,
                                 N_("invalid number at field start"));
          എങ്കിൽ (! താക്കോൽ->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          എങ്കിൽ (*s == '.')
            {
              s = parse_field_എണ്ണം (s + 1, &താക്കോൽ->schar,
                                     N_("invalid number after '.'"));
              എങ്കിൽ (! താക്കോൽ->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          എങ്കിൽ (! (താക്കോൽ->sword || താക്കോൽ->schar))
            താക്കോൽ->sword = SIZE_MAX;
          s = set_ordering (s, താക്കോൽ, bl_start);
          എങ്കിൽ (*s != ',')
            {
              താക്കോൽ->eword = SIZE_MAX;
              താക്കോൽ->echar = 0;
            }
          അല്ലെങ്കിൽ
            {
              /* Get POS2. */
              s = parse_field_എണ്ണം (s + 1, &താക്കോൽ->eword,
                                     N_("invalid number after ','"));
              എങ്കിൽ (! താക്കോൽ->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              എങ്കിൽ (*s == '.')
                {
                  s = parse_field_എണ്ണം (s + 1, &താക്കോൽ->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, താക്കോൽ, bl_end);
            }
          എങ്കിൽ (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (താക്കോൽ);
          നിർത്തുക;

        അവസ്ഥ 'm':
          mergeonly = true;
          നിർത്തുക;

        അവസ്ഥ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          നിർത്തുക;

        അവസ്ഥ 'o':
          എങ്കിൽ (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          നിർത്തുക;

        അവസ്ഥ RANDOM_SOURCE_OPTION:
          എങ്കിൽ (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          നിർത്തുക;

        അവസ്ഥ 's':
          stable = true;
          നിർത്തുക;

        അവസ്ഥ 'S':
          specify_sort_size (oi, c, optarg);
          നിർത്തുക;

        അവസ്ഥ 't':
          {
            അക്ഷരം newtab = optarg[0];
            എങ്കിൽ (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            എങ്കിൽ (optarg[1])
              {
                എങ്കിൽ (streq (optarg, "\\0"))
                  newtab = '\0';
                അല്ലെങ്കിൽ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            എങ്കിൽ (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          നിർത്തുക;

        അവസ്ഥ 'T':
          add_temp_dir (optarg);
          നിർത്തുക;

        അവസ്ഥ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          നിർത്തുക;

        അവസ്ഥ 'u':
          unique = true;
          നിർത്തുക;

        അവസ്ഥ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          എങ്കിൽ (optarg == argv[optind - 1])
            {
              അക്ഷരം സ്ഥിരം *p;
              വേണ്ടി (p = optarg; c_isdigit (*p); p++)
                തുടരുക;
              optind -= (*p != '\0');
            }
          നിർത്തുക;

        അവസ്ഥ 'z':
          eolchar = 0;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (സൂചന (SIGPIPE, SIG_IGN) == SIG_DFL);

  എങ്കിൽ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      എങ്കിൽ (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      എങ്കിൽ (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      എങ്കിൽ (tok.n_tok)
        {
          സ്വതന്ത്രം (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          വേണ്ടി (size_t i = 0; i < nfiles; i++)
            {
              എങ്കിൽ (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              അല്ലെങ്കിൽ എങ്കിൽ (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long പൂർണ്ണസംഖ്യ file_സംഖ്യ = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_സംഖ്യ);
                }
            }
        }
      അല്ലെങ്കിൽ
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  വേണ്ടി (താക്കോൽ = keylist; താക്കോൽ; താക്കോൽ = താക്കോൽ->next)
    {
      എങ്കിൽ (default_താക്കോൽ_compare (താക്കോൽ) && !താക്കോൽ->reverse)
        {
          താക്കോൽ->ignore = gkey.ignore;
          താക്കോൽ->translate = gkey.translate;
          താക്കോൽ->skipsblanks = gkey.skipsblanks;
          താക്കോൽ->skipeblanks = gkey.skipeblanks;
          താക്കോൽ->month = gkey.month;
          താക്കോൽ->numeric = gkey.numeric;
          താക്കോൽ->general_numeric = gkey.general_numeric;
          താക്കോൽ->human_numeric = gkey.human_numeric;
          താക്കോൽ->version = gkey.version;
          താക്കോൽ->random = gkey.random;
          താക്കോൽ->reverse = gkey.reverse;
        }

      need_random |= താക്കോൽ->random;
    }

  എങ്കിൽ (!keylist && !default_താക്കോൽ_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  എങ്കിൽ (debug)
    {
      എങ്കിൽ (checkonly || outfile)
        {
          സ്ഥിതം അക്ഷരം opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      എങ്കിൽ (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      എങ്കിൽ (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      എങ്കിൽ (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ശൂന്യ)));
      അല്ലെങ്കിൽ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      താക്കോൽ_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  എങ്കിൽ (need_random)
    random_md5_state_init (random_source);

  എങ്കിൽ (temp_dirs.n_dirs == 0)
    {
      അക്ഷരം സ്ഥിരം *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  എങ്കിൽ (nfiles == 0)
    {
      nfiles = 1;
      സ്വതന്ത്രം (files);
      files = xmalloc (വലുപ്പം *files);
      *files = (അക്ഷരം *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  എങ്കിൽ (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  എങ്കിൽ (checkonly)
    {
      എങ്കിൽ (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      എങ്കിൽ (outfile)
        {
          സ്ഥിതം അക്ഷരം opts[] = {0, 'o', 0};
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

  എങ്കിൽ (mergeonly)
    {
      ഘടന sortfile *sortfiles = xcalloc (nfiles, വലുപ്പം *sortfiles);

      വേണ്ടി (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (!nthreads)
        {
          unsigned long പൂർണ്ണസംഖ്യ np = അക്കം_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * വലുപ്പം (ഘടന merge_കണ്ണി));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  എങ്കിൽ (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
