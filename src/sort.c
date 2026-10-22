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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <pthread.h>
#অন্তর্ভাব <sys/resource.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <spawn.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "filevercmp.h"
#অন্তর্ভাব "findprog.h"
#অন্তর্ভাব "flexmember.h"
#অন্তর্ভাব "hard-locale.h"
#অন্তর্ভাব "hash.h"
#অন্তর্ভাব "heap.h"
#অন্তর্ভাব "ignore-value.h"
#অন্তর্ভাব "mbswidth.h"
#অন্তর্ভাব "nproc.h"
#অন্তর্ভাব "physmem.h"
#অন্তর্ভাব "posixver.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "randread.h"
#অন্তর্ভাব "readtokens0.h"
#অন্তর্ভাব "stdlib--.h"
#অন্তর্ভাব "strnumcmp.h"
#অন্তর্ভাব "term-sig.h"
#অন্তর্ভাব "unistd--.h"
#অন্তর্ভাব "xmemcoll.h"
#অন্তর্ভাব "xnanosleep.h"
#অন্তর্ভাব "xstrtol.h"
#অন্তর্ভাব "xstrtol-error.h"

#ifndef RLIMIT_DATA
সংরচনা rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#যদি HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#যদি !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#যদি !defined OPEN_MAX
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
গণনা { SUBTHREAD_সারিগুলি_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_সারিগুলি_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
গণনা { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
গণনা
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

গণনা
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

গণনা
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_মূল = 1
  };

/* The representation of the decimal point in the current locale.  */
স্থির বর্ণ decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
স্থির পূর্ণাঙ্ক thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
স্থির bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
স্থির bool hard_LC_COLLATE;
#যদি HAVE_NL_LANGINFO
স্থির bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
গণনা blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
স্থির বর্ণ eolchar = '\n';

/* Lines are held in memory as counted strings. */
সংরচনা সারি
{
  বর্ণ *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  বর্ণ *keybeg;			/* Start of first key. */
  বর্ণ *keylim;			/* Limit of first key. */
};

/* Input buffers. */
সংরচনা buffer
{
  বর্ণ *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t বাম;			/* Number of bytes left from previous reads. */
  size_t সারি_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
সংরচনা sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
সংরচনা keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ধ্রুব *ignore;		/* Boolean array of characters to ignore. */
  বর্ণ ধ্রুব *translate;	/* Translation applied to characters. */
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
  সংরচনা keyfield *next;	/* Next keyfield to try. */
};

সংরচনা month
{
  বর্ণ ধ্রুব *name;
  পূর্ণাঙ্ক val;
};

/* Binary merge tree node. */
সংরচনা merge_গ্রন্থি
{
  সংরচনা সারি *lo;              /* Lines to merge from LO child node. */
  সংরচনা সারি *hi;              /* Lines to merge from HI child node. */
  সংরচনা সারি *end_lo;          /* End of available lines from LO. */
  সংরচনা সারি *end_hi;          /* End of available lines from HI. */
  সংরচনা সারি **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  সংরচনা merge_গ্রন্থি *parent;    /* Parent node. */
  সংরচনা merge_গ্রন্থি *lo_child;  /* LO child node. */
  সংরচনা merge_গ্রন্থি *hi_child;  /* HI child node. */
  unsigned পূর্ণাঙ্ক level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
সংরচনা merge_গ্রন্থি_queue
{
  সংরচনা heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
স্থির সংরচনা সারি saved_সারি;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
স্থির bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
স্থির bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
স্থির bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
স্থির বর্ণ fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
স্থির সংরচনা month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + পরিমাণ (সংরচনা সারি))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
স্থির size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
স্থির size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define প্রবেশ_FILE_SIZE_GUESS (128 * 1024)

স্থির সংরচনা
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  বর্ণ ধ্রুব **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
স্থির bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
স্থির bool stable;

/* An int value outside char range.  */
গণনা { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
গণনা { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
স্থির পূর্ণাঙ্ক tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
স্থির bool unique;

/* Nonzero if any of the input files are the standard input. */
স্থির bool have_read_stdin;

/* List of key field comparisons to be tried.  */
স্থির সংরচনা keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
স্থির বর্ণ ধ্রুব *compress_program;

/* Annotate the output with extra info to aid the user.  */
স্থির bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
স্থির unsigned পূর্ণাঙ্ক nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
স্থির bool default_SIGPIPE;

/* The list of temporary files. */
সংরচনা tempnode
{
  সংরচনা tempnode *চঞ্চল next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  বর্ণ state;
  বর্ণ name[FLEXIBLE_ARRAY_MEMBER];
};
স্থির সংরচনা tempnode *চঞ্চল temphead;
স্থির সংরচনা tempnode *চঞ্চল *temptail = &temphead;

/* Clean up any remaining temporary files.  */

স্থির শূন্য
cleanup (শূন্য)
{
  প্রতি (সংরচনা tempnode ধ্রুব *গ্রন্থি = temphead; গ্রন্থি; গ্রন্থি = গ্রন্থি->next)
    unlink (গ্রন্থি->name);
  temphead = রিক্ত;
}

/* Handle interrupts and hangups. */

স্থির শূন্য
sighandler (পূর্ণাঙ্ক sig)
{
  cleanup ();

  সঙ্কেত (sig, SIG_DFL);
  উত্থাপয (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

স্থির শূন্য
sort_die (বর্ণ ধ্রুব *message, বর্ণ ধ্রুব *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  যদি (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         ignore leading blanks when finding sort চাবিগুলি in each সারি\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-পক্ষ\n\
         fold lower পক্ষ to upper পক্ষ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical মান\n\
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
         compare according to string numerical মান;\n\
         see full documentation প্রতি supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical চাবিগুলি.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; প্রতি more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check প্রতি sorted প্রবেশ; কুরু not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but কুরু not report first bad সারি\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the সারি used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         পঠ প্রবেশ from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, পঠ names from standard প্রবেশ\n\
"));
      oputs (_("\
  -k, --চাবি=KEYDEF\n\
         sort via a চাবি; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; কুরু not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         লিখ result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE প্রতি main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR প্রতি temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the সংখ্যা of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of সারিগুলি with equal চাবিগুলি;\n\
         with -c, check প্রতি strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] প্রতি start and stop position, where F is a\n\
field সংখ্যা and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the সারি's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options প্রতি that চাবি.  If no চাবি is given, use\n\
the entire সারি as the চাবি.  Use --debug to diagnose incorrect চাবি usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (স্বভাব), and so on প্রতি M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte মানসমূহ.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
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

স্থির বর্ণ ধ্রুব short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"ignore-leading-blanks", no_argument, রিক্ত, 'b'},
  {"check", optional_argument, রিক্ত, CHECK_OPTION},
  {"compress-program", required_argument, রিক্ত, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, রিক্ত, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, রিক্ত, 'd'},
  {"ignore-case", no_argument, রিক্ত, 'f'},
  {"files0-from", required_argument, রিক্ত, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, রিক্ত, 'g'},
  {"ignore-nonprinting", no_argument, রিক্ত, 'i'},
  {"key", required_argument, রিক্ত, 'k'},
  {"merge", no_argument, রিক্ত, 'm'},
  {"month-sort", no_argument, রিক্ত, 'M'},
  {"numeric-sort", no_argument, রিক্ত, 'n'},
  {"human-numeric-sort", no_argument, রিক্ত, 'h'},
  {"version-sort", no_argument, রিক্ত, 'V'},
  {"random-sort", no_argument, রিক্ত, 'R'},
  {"random-source", required_argument, রিক্ত, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, রিক্ত, SORT_OPTION},
  {"output", required_argument, রিক্ত, 'o'},
  {"reverse", no_argument, রিক্ত, 'r'},
  {"stable", no_argument, রিক্ত, 's'},
  {"batch-size", required_argument, রিক্ত, NMERGE_OPTION},
  {"buffer-size", required_argument, রিক্ত, 'S'},
  {"field-separator", required_argument, রিক্ত, 't'},
  {"temporary-directory", required_argument, রিক্ত, 'T'},
  {"unique", no_argument, রিক্ত, 'u'},
  {"zero-terminated", no_argument, রিক্ত, 'z'},
  {"parallel", required_argument, রিক্ত, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

স্থির বর্ণ ধ্রুব *ধ্রুব check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE রিক্ত
#undef  _ct_
};
স্থির বর্ণ ধ্রুব check_types[] =
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

স্থির বর্ণ ধ্রুব *ধ্রুব sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE রিক্ত
#undef  _st_
};
স্থির বর্ণ ধ্রুব sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
স্থির sigset_t caught_signals;

/* Critical section status.  */
সংরচনা cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
স্থির শূন্য
cs_enter (সংরচনা cs_status *status)
{
  পূর্ণাঙ্ক ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
স্থির শূন্য
cs_leave (সংরচনা cs_status ধ্রুব *status)
{
  যদি (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, রিক্ত);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
গণনা { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
সংরচনা sortfile
{
  /* The file's name.  */
  বর্ণ ধ্রুব *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  সংরচনা tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
স্থির Hash_table *proctab;

গণনা { INIT_PROCTAB_SIZE = 47 };

স্থির size_t
proctab_hasher (শূন্য ধ্রুব *entry, size_t tabsize)
{
  সংরচনা tempnode ধ্রুব *গ্রন্থি = entry;
  প্রত্যাগম গ্রন্থি->pid % tabsize;
}

স্থির bool
proctab_comparator (শূন্য ধ্রুব *e1, শূন্য ধ্রুব *e2)
{
  সংরচনা tempnode ধ্রুব *n1 = e1;
  সংরচনা tempnode ধ্রুব *n2 = e2;
  প্রত্যাগম n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
স্থির pid_t nprocs;

স্থির bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

স্থির pid_t
reap (pid_t pid)
{
  পূর্ণাঙ্ক status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  যদি (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  অন্যথা যদি (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      যদি (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  প্রত্যাগম cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

স্থির শূন্য
register_proc (সংরচনা tempnode *temp)
{
  যদি (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, রিক্ত,
                                 proctab_hasher,
                                 proctab_comparator,
                                 রিক্ত);
      যদি (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  যদি (! hash_ঢোকাও (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

স্থির bool
delete_proc (pid_t pid)
{
  সংরচনা tempnode test;

  test.pid = pid;
  সংরচনা tempnode *গ্রন্থি = hash_remove (proctab, &test);
  যদি (! গ্রন্থি)
    প্রত্যাগম false;
  গ্রন্থি->state = REAPED;
  প্রত্যাগম true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

স্থির শূন্য
wait_proc (pid_t pid)
{
  যদি (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

স্থির শূন্য
reap_exited (শূন্য)
{
  যাবত্ (0 < nprocs && reap (0))
    অনুবর্ত;
}

/* Reap at least one exited child, waiting if necessary.  */

স্থির শূন্য
reap_some (শূন্য)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

স্থির শূন্য
reap_all (শূন্য)
{
  যাবত্ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

স্থির শূন্য
exit_cleanup (শূন্য)
{
  যদি (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      সংরচনা cs_status cs;
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

স্থির সংরচনা tempnode *
create_temp_file (পূর্ণাঙ্ক *pfd, bool survive_fd_exhaustion)
{
  স্থির বর্ণ ধ্রুব slashbase[] = "/sortXXXXXX";
  স্থির idx_t temp_dir_index;
  পূর্ণাঙ্ক fd;
  পূর্ণাঙ্ক saved_errno;
  বর্ণ ধ্রুব *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  সংরচনা tempnode *গ্রন্থি =
    xmalloc (FLEXSIZEOF (সংরচনা tempnode, name, len + পরিমাণ slashbase));
  বর্ণ *file = গ্রন্থি->name;
  সংরচনা cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, পরিমাণ slashbase);
  গ্রন্থি->next = রিক্ত;
  যদি (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  যদি (0 <= fd)
    {
      *temptail = গ্রন্থি;
      temptail = &গ্রন্থি->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  যদি (fd < 0)
    {
      যদি (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      মুক্ত (গ্রন্থি);
      গ্রন্থি = রিক্ত;
    }

  *pfd = fd;
  প্রত্যাগম গ্রন্থি;
}

/* Return a pointer to stdout status, or NULL on failure.  */

স্থির সংরচনা stat *
get_outstatus (শূন্য)
{
  স্থির পূর্ণাঙ্ক outstat_errno;
  স্থির সংরচনা stat outstat;
  যদি (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  প্রত্যাগম outstat_errno < 0 ? &outstat : রিক্ত;
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

স্থির FILE *
stream_open (বর্ণ ধ্রুব *file, বর্ণ ধ্রুব *how)
{
  FILE *fp;

  যদি (*how == 'r')
    {
      যদি (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      অন্যথা
        {
          পূর্ণাঙ্ক fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? রিক্ত : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  অন্যথা যদি (*how == 'w')
    {
      যদি (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          পূর্ণাঙ্ক ftruncate_errno = errno;
          সংরচনা stat *outst = get_outstatus ();
          যদি (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  অন্যথা
    affirm (!"unexpected mode passed to stream_open");

  প্রত্যাগম fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

স্থির FILE *
xfopen (বর্ণ ধ্রুব *file, বর্ণ ধ্রুব *how)
{
  FILE *fp = stream_open (file, how);
  যদি (!fp)
    sort_die (_("open failed"), file);
  প্রত্যাগম fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

স্থির শূন্য
xfclose (FILE *fp, বর্ণ ধ্রুব *file)
{
  বিকল্প (fileno (fp))
    {
    পক্ষ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ভঙ্গ;

    পক্ষ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      যদি (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      ভঙ্গ;

    স্বভাব:
      যদি (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      ভঙ্গ;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

স্থির শূন্য
move_fd (পূর্ণাঙ্ক oldfd, পূর্ণাঙ্ক newfd)
{
  যদি (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_মান (dup2 (oldfd, newfd));
      ignore_মান (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

স্থির পূর্ণাঙ্ক
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  পূর্ণাঙ্ক oldfd, পূর্ণাঙ্ক newfd)
{
  পূর্ণাঙ্ক result = 0;
  যদি (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      যদি (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  প্রত্যাগম result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

স্থির বর্ণ ধ্রুব *
get_resolved_compress_program (শূন্য)
{
  /* Use a cache, to perform the search only once.  */
  স্থির বর্ণ ধ্রুব *resolved_compress_program_cache /* = NULL */;

  যদি (resolved_compress_program_cache == রিক্ত)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), রিক্ত, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  প্রত্যাগম resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

স্থির পূর্ণাঙ্ক
pipe_child (pid_t *pid, পূর্ণাঙ্ক pipefds[2], পূর্ণাঙ্ক tempfd, bool decompress,
            size_t tries)
{
  বর্ণ ধ্রুব *resolved_compress_program;
  সংরচনা tempnode *saved_temphead;
  দ্বিগুণ wait_retry = 0.25;
  সংরচনা cs_status cs;
  পূর্ণাঙ্ক result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  যদি (resolved_compress_program == রিক্ত)
    প্রত্যাগম errno;

  যদি ((result = posix_spawnattr_init (&attr)))
    প্রত্যাগম result;
  যদি ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      প্রত্যাগম result;
    }

  যদি (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      পূর্ণাঙ্ক saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      প্রত্যাগম saved_errno;
    }

  যদি ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      প্রত্যাগম result;
    }

  বর্ণ ধ্রুব *ধ্রুব argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : রিক্ত,
      রিক্ত
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  যদি (nmerge + 1 < nprocs)
    reap_some ();

  যাবত্ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = রিক্ত;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (বর্ণ * ধ্রুব *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      যদি (result != EAGAIN)
        ভঙ্গ;
      অন্যথা
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

  যদি (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  অন্যথা
    ++nprocs;

  প্রত্যাগম result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

স্থির সংরচনা tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  পূর্ণাঙ্ক tempfd;
  সংরচনা tempnode *গ্রন্থি = create_temp_file (&tempfd, survive_fd_exhaustion);
  যদি (! গ্রন্থি)
    প্রত্যাগম রিক্ত;

  গ্রন্থি->state = UNCOMPRESSED;

  যদি (compress_program)
    {
      পূর্ণাঙ্ক pipefds[2];
      স্থির পূর্ণাঙ্ক last_result = 0;

      পূর্ণাঙ্ক result = pipe_child (&গ্রন্থি->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      যদি (result)
        {
          যদি (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      অন্যথা
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (গ্রন্থি);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  যদি (! *pfp)
    sort_die (_("couldn't create temporary file"), গ্রন্থি->name);

  প্রত্যাগম গ্রন্থি;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

স্থির সংরচনা tempnode *
create_temp (FILE **pfp)
{
  প্রত্যাগম maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

স্থির FILE *
open_temp (সংরচনা tempnode *temp)
{
  পূর্ণাঙ্ক tempfd, pipefds[2];
  FILE *fp = রিক্ত;

  যদি (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  যদি (tempfd < 0)
    প্রত্যাগম রিক্ত;

  pid_t child;
  পূর্ণাঙ্ক result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  যদি (result)
    {
      যদি (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  অন্যথা
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      যদি (! fp)
        {
          পূর্ণাঙ্ক saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  প্রত্যাগম fp;
}

/* Append DIR to the array of temporary directory names.  */
স্থির শূন্য
add_temp_dir (বর্ণ ধ্রুব *dir)
{
  যদি (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              পরিমাণ *temp_dirs.dirs);

  idx_t ধ্রুব dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

স্থির শূন্য
zaptemp (বর্ণ ধ্রুব *name)
{
  সংরচনা tempnode *চঞ্চল *pnode;
  সংরচনা tempnode *গ্রন্থি;
  সংরচনা tempnode *next;
  পূর্ণাঙ্ক unlink_status;
  পূর্ণাঙ্ক unlink_errno = 0;
  সংরচনা cs_status cs;

  প্রতি (pnode = &temphead; (গ্রন্থি = *pnode)->name != name; pnode = &গ্রন্থি->next)
    অনুবর্ত;

  যদি (গ্রন্থি->state == UNREAPED)
    wait_proc (গ্রন্থি->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = গ্রন্থি->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  যদি (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  যদি (! next)
    temptail = pnode;
  মুক্ত (গ্রন্থি);
}

#যদি HAVE_NL_LANGINFO

স্থির পূর্ণাঙ্ক
struct_month_cmp (শূন্য ধ্রুব *m1, শূন্য ধ্রুব *m2)
{
  সংরচনা month ধ্রুব *month1 = m1;
  সংরচনা month ধ্রুব *month2 = m2;
  প্রত্যাগম strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

স্থির শূন্য
inittables (শূন্য)
{
  প্রতি (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#যদি HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  যদি (hard_LC_TIME)
    {
      প্রতি (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          বর্ণ ধ্রুব *s;
          size_t s_len;
          size_t j, k;
          বর্ণ *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          প্রতি (j = k = 0; j < s_len; j++)
            যদি (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, পরিমাণ *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
স্থির শূন্য
specify_nmerge (পূর্ণাঙ্ক oi, বর্ণ c, বর্ণ ধ্রুব *s)
{
  uintmax_t n;
  সংরচনা rlimit rlimit;
  গণনা strtol_error e = xstrtoumax (s, রিক্ত, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned পূর্ণাঙ্ক max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  যদি (e == LONGINT_OK)
    {
      nmerge = n;
      যদি (nmerge != n)
        e = LONGINT_OVERFLOW;
      অন্যথা
        {
          যদি (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          অন্যথা যদি (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          অন্যথা
            প্রত্যাগম;
        }
    }

  যদি (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  অন্যথা
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
স্থির শূন্য
specify_sort_size (পূর্ণাঙ্ক oi, বর্ণ c, বর্ণ ধ্রুব *s)
{
  uintmax_t n;
  বর্ণ *suffix;
  গণনা strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  যদি (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  যদি (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    বিকল্প (suffix[0])
      {
      পক্ষ 'b':
        e = LONGINT_OK;
        ভঙ্গ;

      পক্ষ '%':
        {
          দ্বিগুণ mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          যদি (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          অন্যথা
            e = LONGINT_OVERFLOW;
        }
        ভঙ্গ;
      }

  যদি (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      যদি (n < sort_size)
        প্রত্যাগম;

      sort_size = n;
      যদি (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          প্রত্যাগম;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
স্থির size_t
specify_nthreads (পূর্ণাঙ্ক oi, বর্ণ c, বর্ণ ধ্রুব *s)
{
  uintmax_t nthreads;
  গণনা strtol_error e = xstrtoumax (s, রিক্ত, 10, &nthreads, "");
  যদি (e == LONGINT_OVERFLOW)
    প্রত্যাগম SIZE_MAX;
  যদি (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  যদি (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  যদি (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  প্রত্যাগম nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
স্থির size_t
default_sort_size (শূন্য)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  সংরচনা rlimit rlimit;
  যদি (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  যদি (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  যদি (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  দ্বিগুণ avail = physmem_available ();
  দ্বিগুণ total = physmem_total ();
  দ্বিগুণ mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  যদি (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  যদি (mem < size)
    size = mem;
  প্রত্যাগম MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

স্থির bool
সারি_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = পরিমাণ (সংরচনা সারি);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  যদি (ckd_add (&aligned, size, padding))
    প্রত্যাগম false;

  *alloc = aligned;
  প্রত্যাগম true;
}

/* Return the default initial allocation for a growable sort buffer.  */

স্থির size_t
default_initial_sort_size (size_t সারি_bytes)
{
  size_t size = সারি_bytes + 2;
  size_t প্রবেশ_size;

  যদি (ckd_mul (&প্রবেশ_size, প্রবেশ_FILE_SIZE_GUESS, সারি_bytes + 1)
      || ckd_add (&size, size, প্রবেশ_size))
    প্রত্যাগম SIZE_MAX;

  প্রত্যাগম MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

স্থির bool
প্রবেশ_size_buffer_bytes (uintmax_t file_size, size_t সারি_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_প্রবেশ_byte = সারি_bytes + 1;

  যদি (SIZE_MAX / worst_case_per_প্রবেশ_byte < file_size)
    প্রত্যাগম false;

  size_t size = file_size * worst_case_per_প্রবেশ_byte;
  যদি (SIZE_MAX - size < 1)
    প্রত্যাগম false;

  *worst_case = size + 1;
  প্রত্যাগম true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

স্থির শূন্য
sort_buffer_policy (FILE *ধ্রুব *fps, size_t nfps,
                    বর্ণ *ধ্রুব *files, size_t nfiles,
                    size_t সারি_bytes,
                    সংরচনা sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (সারি_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_প্রবেশ_byte = সারি_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_প্রবেশ_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  প্রতি (size_t i = 0; i < nfiles; i++)
    {
      সংরচনা stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      যদি (size_bound <= size)
        {
          policy->initial = size_bound;
          প্রত্যাগম;
        }

      যদি ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      যদি (known_size)
        file_size = st.st_size;
      অন্যথা
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          যদি (sort_size)
            {
              policy->initial = size_bound;
              প্রত্যাগম;
            }
          file_size = প্রবেশ_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      যদি (! প্রবেশ_size_buffer_bytes (file_size, সারি_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          প্রত্যাগম;
        }

      যদি (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          প্রত্যাগম;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

স্থির শূন্য
initbuf (সংরচনা buffer *buf, size_t সারি_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  যাবত্ (true)
    {
      যদি (! সারি_aligned_size (&alloc))
        buf->buf = রিক্ত;
      অন্যথা
        buf->buf = malloc (alloc);
      যদি (buf->buf)
        ভঙ্গ;
      alloc /= 2;
      যদি (alloc <= সারি_bytes + 1)
        xalloc_die ();
    }

  buf->সারি_bytes = সারি_bytes;
  buf->alloc = alloc;
  buf->used = buf->বাম = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

স্থির অন্তঃস্থ সংরচনা সারি *
buffer_linelim (সংরচনা buffer ধ্রুব *buf)
{
  শূন্য *linelim = buf->buf + buf->alloc;
  প্রত্যাগম linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

স্থির bool
try_growbuf (সংরচনা buffer *buf, size_t alloc)
{
  যদি (! সারি_aligned_size (&alloc) || alloc <= buf->alloc)
    প্রত্যাগম false;

  বর্ণ *newbuf = malloc (alloc);
  যদি (! newbuf)
    প্রত্যাগম false;

  বর্ণ *oldbuf = buf->buf;
  সংরচনা সারি *old_linelim = buffer_linelim (buf);
  সংরচনা সারি *old_সারি = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  সংরচনা সারি *new_linelim = (শূন্য *) (newbuf + alloc);
  সংরচনা সারি *new_সারি = new_linelim - buf->nlines;
  memcpy (new_সারি, old_সারি, buf->nlines * পরিমাণ *new_সারি);

  প্রতি (সংরচনা সারি *সারি = new_সারি; সারি < new_linelim; সারি++)
    {
      সারি->text = newbuf + (সারি->text - oldbuf);
      যদি (keylist)
        {
          সারি->keybeg = newbuf + (সারি->keybeg - oldbuf);
          সারি->keylim = newbuf + (সারি->keylim - oldbuf);
        }
    }

  মুক্ত (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  প্রত্যাগম true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

স্থির bool
maybe_growbuf (সংরচনা buffer *buf, সংরচনা sort_buffer_policy *policy)
{
  যদি (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    প্রত্যাগম false;

  size_t alloc;
  যদি (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  অন্যথা
    alloc = policy->limit;

  যদি (try_growbuf (buf, alloc))
    প্রত্যাগম true;

  policy->growth_failed = true;
  প্রত্যাগম false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

স্থির বর্ণ *
begfield (সংরচনা সারি ধ্রুব *সারি, সংরচনা keyfield ধ্রুব *চাবি)
{
  বর্ণ *ptr = সারি->text, *lim = ptr + সারি->length - 1;
  size_t sword = চাবি->sword;
  size_t schar = চাবি->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  যদি (tab != TAB_DEFAULT)
    যাবত্ (ptr < lim && sword--)
      {
        বর্ণ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        যদি (ptr < lim)
          ++ptr;
      }
  অন্যথা
    যাবত্ (ptr < lim && sword--)
      {
        যাবত্ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        যাবত্ (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  যদি (চাবি->skipsblanks)
    যাবত্ (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  যদি (schar < remaining_bytes)
    ptr += schar;
  অন্যথা
    ptr = lim;

  প্রত্যাগম ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
স্থির বর্ণ *
limfield (সংরচনা সারি ধ্রুব *সারি, সংরচনা keyfield ধ্রুব *চাবি)
{
  বর্ণ *ptr = সারি->text, *lim = ptr + সারি->length - 1;
  size_t eword = চাবি->eword, echar = চাবি->echar;

  যদি (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  যদি (tab != TAB_DEFAULT)
    যাবত্ (ptr < lim && eword--)
      {
        বর্ণ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        যদি (ptr < lim && (eword || echar))
          ++ptr;
      }
  অন্যথা
    যাবত্ (ptr < lim && eword--)
      {
        যাবত্ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        যাবত্ (ptr < lim && !blanks[to_uchar (*ptr)])
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
  যদি (tab != TAB_DEFAULT)
    {
      বর্ণ *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      যদি (newlim)
        lim = newlim;
    }
  অন্যথা
    {
      বর্ণ *newlim;
      newlim = ptr;
      যাবত্ (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      যাবত্ (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  যদি (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      যদি (চাবি->skipeblanks)
        যাবত্ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      যদি (echar < remaining_bytes)
        ptr += echar;
      অন্যথা
        ptr = lim;
    }

  প্রত্যাগম ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

স্থির bool
fillbuf (সংরচনা buffer *buf, FILE *fp, বর্ণ ধ্রুব *file,
         সংরচনা sort_buffer_policy *policy)
{
  সংরচনা keyfield ধ্রুব *চাবি = keylist;
  বর্ণ eol = eolchar;
  size_t সারি_bytes = buf->সারি_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  যদি (buf->eof)
    প্রত্যাগম false;

  যদি (buf->used != buf->বাম)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->বাম, buf->বাম);
      buf->used = buf->বাম;
      buf->nlines = 0;
    }

  যাবত্ (true)
    {
      বর্ণ *ptr = buf->buf + buf->used;
      সংরচনা সারি *linelim = buffer_linelim (buf);
      সংরচনা সারি *সারি = linelim - buf->nlines;
      size_t avail = (বর্ণ *) linelim - buf->nlines * সারি_bytes - ptr;
      বর্ণ *সারি_start = buf->nlines ? সারি->text + সারি->length : buf->buf;

      যাবত্ (সারি_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (সারি_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          বর্ণ *ptrlim = ptr + bytes_read;
          বর্ণ *p;
          avail -= bytes_read;

          যদি (bytes_read != readsize)
            {
              যদি (ferror (fp))
                sort_die (_("read failed"), file);
              যদি (feof (fp))
                {
                  buf->eof = true;
                  যদি (buf->buf == ptrlim)
                    প্রত্যাগম false;
                  যদি (সারি_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          যাবত্ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              সারি--;
              সারি->text = সারি_start;
              সারি->length = ptr - সারি_start;
              mergesize = MAX (mergesize, সারি->length);
              avail -= সারি_bytes;

              যদি (চাবি)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  সারি->keylim = (চাবি->eword == SIZE_MAX
                                  ? p
                                  : limfield (সারি, চাবি));

                  যদি (চাবি->sword != SIZE_MAX)
                    সারি->keybeg = begfield (সারি, চাবি);
                  অন্যথা
                    {
                      যদি (চাবি->skipsblanks)
                        যাবত্ (blanks[to_uchar (*সারি_start)])
                          সারি_start++;
                      সারি->keybeg = সারি_start;
                    }
                }

              সারি_start = ptr;
            }

          ptr = ptrlim;
          যদি (buf->eof)
            ভঙ্গ;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - সারি;
      যদি (buf->nlines != 0)
        {
          যদি (! buf->eof && maybe_growbuf (buf, policy))
            অনুবর্ত;
          buf->বাম = ptr - সারি_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          প্রত্যাগম true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t সারি_alloc = buf->alloc / পরিমাণ (সংরচনা সারি);
        buf->buf = xpalloc (buf->buf, &সারি_alloc, 1, -1, পরিমাণ (সংরচনা সারি));
        buf->alloc = সারি_alloc * পরিমাণ (সংরচনা সারি);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
স্থির বর্ণ ধ্রুব unit_order[UCHAR_LIM] =
  {
#যদি ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#অন্যথা
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
স্থির বর্ণ
traverse_raw_সংখ্যা (বর্ণ ধ্রুব **সংখ্যা)
{
  বর্ণ ধ্রুব *p = *সংখ্যা;
  বর্ণ ch;
  বর্ণ max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  যাবত্ (c_isdigit (ch = *p++))
    {
      যদি (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      যদি (ends_with_thousands_sep)
        ++p;
    }

  যদি (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *সংখ্যা = p - 2;
      প্রত্যাগম max_digit;
    }

  যদি (ch == decimal_point)
    যাবত্ (c_isdigit (ch = *p++))
      যদি (max_digit < ch)
        max_digit = ch;

  *সংখ্যা = p - 1;
  প্রত্যাগম max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
স্থির পূর্ণাঙ্ক
find_unit_order (বর্ণ ধ্রুব *সংখ্যা)
{
  bool minus_sign = (*সংখ্যা == '-');
  বর্ণ ধ্রুব *p = সংখ্যা + minus_sign;
  বর্ণ max_digit = traverse_raw_সংখ্যা (&p);
  যদি ('0' < max_digit)
    {
      unsigned বর্ণ ch = *p;
      পূর্ণাঙ্ক order = unit_order[ch];
      প্রত্যাগম (minus_sign ? -order : order);
    }
  অন্যথা
    প্রত্যাগম 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
স্থির পূর্ণাঙ্ক
human_numcompare (বর্ণ ধ্রুব *a, বর্ণ ধ্রুব *b)
{
  যাবত্ (blanks[to_uchar (*a)])
    a++;
  যাবত্ (blanks[to_uchar (*b)])
    b++;

  পূর্ণাঙ্ক diff = find_unit_order (a) - find_unit_order (b);
  প্রত্যাগম (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
স্থির পূর্ণাঙ্ক
numcompare (বর্ণ ধ্রুব *a, বর্ণ ধ্রুব *b)
{
  যাবত্ (blanks[to_uchar (*a)])
    a++;
  যাবত্ (blanks[to_uchar (*b)])
    b++;

  প্রত্যাগম strnumcmp (a, b, decimal_point, thousands_sep);
}

স্থির পূর্ণাঙ্ক
nan_compare (long দ্বিগুণ a, long দ্বিগুণ b)
{
  বর্ণ buf[2][পরিমাণ "-nan""()" + CHAR_BIT * পরিমাণ a];
  snprintf (buf[0], পরিমাণ buf[0], "%Lf", a);
  snprintf (buf[1], পরিমাণ buf[1], "%Lf", b);
  প্রত্যাগম strcmp (buf[0], buf[1]);
}

স্থির পূর্ণাঙ্ক
general_numcompare (বর্ণ ধ্রুব *sa, বর্ণ ধ্রুব *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  বর্ণ *ea;
  বর্ণ *eb;
  long দ্বিগুণ a = strtold (sa, &ea);
  long দ্বিগুণ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  যদি (sa == ea)
    প্রত্যাগম sb == eb ? 0 : -1;
  যদি (sb == eb)
    প্রত্যাগম 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  প্রত্যাগম (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

স্থির পূর্ণাঙ্ক
getmonth (বর্ণ ধ্রুব *month, বর্ণ **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  যাবত্ (blanks[to_uchar (*month)])
    month++;

  কুরু
    {
      size_t ix = (lo + hi) / 2;
      বর্ণ ধ্রুব *m = month;
      বর্ণ ধ্রুব *n = monthtab[ix].name;

      প্রতি (;; m++, n++)
        {
          যদি (!*n)
            {
              যদি (ea)
                *ea = (বর্ণ *) m;
              প্রত্যাগম monthtab[ix].val;
            }
          যদি (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              ভঙ্গ;
            }
          অন্যথা যদি (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              ভঙ্গ;
            }
        }
    }
  যাবত্ (lo < hi);

  প্রত্যাগম 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#যদি DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# যদি 14 <= __GNUC__
#  pragma GCC diagnostic ঠেলো
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#অন্তর্ভাব "md5.h"

#যদি DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# যদি 14 <= __GNUC__
#  pragma GCC diagnostic বের_করো
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
স্থির শূন্য
link_failure (শূন্য)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
স্থির শূন্য *
প্রতীক_address (শূন্য *handle, বর্ণ ধ্রুব *প্রতীক)
{
  শূন্য *address = dlsym (handle, প্রতীক);
  যদি (!address)
    link_failure ();
  প্রত্যাগম address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
স্থির শূন্য
link_libcrypto (শূন্য)
{
#যদি DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  শূন্য *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  যদি (!handle)
    link_failure ();
  ptr_MD5_Init = প্রতীক_address (handle, "MD5_Init");
  ptr_MD5_Update = প্রতীক_address (handle, "MD5_Update");
  ptr_MD5_Final = প্রতীক_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
স্থির সংরচনা md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

স্থির শূন্য
random_md5_state_init (বর্ণ ধ্রুব *random_source)
{
  unsigned বর্ণ buf[MD5_DIGEST_SIZE];
  সংরচনা randread_source *r = randread_new (random_source, পরিমাণ buf);
  যদি (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, পরিমাণ buf);
  যদি (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, পরিমাণ buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

স্থির size_t
xstrxfrm (বর্ণ *restrict dest, বর্ণ ধ্রুব *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  যদি (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  প্রত্যাগম translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

স্থির পূর্ণাঙ্ক
compare_random (বর্ণ *restrict texta, size_t lena,
                বর্ণ *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  পূর্ণাঙ্ক xfrm_diff = 0;

  বর্ণ stackbuf[4000];
  বর্ণ *buf = stackbuf;
  size_t bufsize = পরিমাণ stackbuf;
  শূন্য *allocated = রিক্ত;
  uint32_t dig[2][MD5_DIGEST_SIZE / পরিমাণ (uint32_t)];
  সংরচনা md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  যদি (hard_LC_COLLATE)
    {
      বর্ণ ধ্রুব *lima = texta + lena;
      বর্ণ ধ্রুব *limb = textb + lenb;

      যাবত্ (true)
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
          যদি (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              মুক্ত (allocated);
              buf = allocated = malloc (bufsize);
              যদি (! buf)
                {
                  buf = stackbuf;
                  bufsize = পরিমাণ stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : রিক্ত), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          যদি (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              যদি (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              মুক্ত (allocated);
              buf = allocated = xmalloc (bufsize);
              যদি (texta < lima)
                strxfrm (buf, texta, sizea);
              যদি (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          যদি (texta < lima)
            texta += strlen (texta) + 1;
          যদি (textb < limb)
            textb += strlen (textb) + 1;
          যদি (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ভঙ্গ;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          যদি (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              যদি (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  পূর্ণাঙ্ক diff = memcmp (dig[0], dig[1], পরিমাণ dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  যদি (! diff)
    {
      যদি (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          যদি (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  মুক্ত (allocated);

  প্রত্যাগম diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

স্থির size_t
debug_width (বর্ণ ধ্রুব *text, বর্ণ ধ্রুব *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  যাবত্ (text < lim)
    width += (*text++ == '\t');
  প্রত্যাগম width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

স্থির শূন্য
mark_চাবি (size_t offset, size_t width)
{
  যাবত্ (offset--)
    putchar (' ');

  যদি (!width)
    printf (_("^ no match for key\n"));
  অন্যথা
    {
      কুরু
        putchar ('_');
      যাবত্ (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

স্থির অন্তঃস্থ bool
চাবি_numeric (সংরচনা keyfield ধ্রুব *চাবি)
{
  প্রত্যাগম চাবি->numeric || চাবি->general_numeric || চাবি->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

স্থির শূন্য
debug_চাবি (সংরচনা সারি ধ্রুব *সারি, সংরচনা keyfield ধ্রুব *চাবি)
{
  বর্ণ *text = সারি->text;
  বর্ণ *beg = text;
  বর্ণ *lim = text + সারি->length - 1;

  যদি (চাবি)
    {
      যদি (চাবি->sword != SIZE_MAX)
        beg = begfield (সারি, চাবি);
      যদি (চাবি->eword != SIZE_MAX)
        {
          lim = limfield (সারি, চাবি);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      যদি ((চাবি->skipsblanks && চাবি->sword == SIZE_MAX)
          || চাবি->month || চাবি_numeric (চাবি))
        {
          বর্ণ saved = *lim;
          *lim = '\0';

          যাবত্ (blanks[to_uchar (*beg)])
            beg++;

          বর্ণ *tighter_lim = beg;

          যদি (lim < beg)
            tighter_lim = lim;
          অন্যথা যদি (চাবি->month)
            getmonth (beg, &tighter_lim);
          অন্যথা যদি (চাবি->general_numeric)
            ignore_মান (strtold (beg, &tighter_lim));
          অন্যথা যদি (চাবি->numeric || চাবি->human_numeric)
            {
              বর্ণ ধ্রুব *p = beg + (beg < lim && *beg == '-');
              বর্ণ max_digit = traverse_raw_সংখ্যা (&p);
              যদি ('0' <= max_digit)
                {
                  unsigned বর্ণ ch = *p;
                  tighter_lim = (বর্ণ *) p
                    + (চাবি->human_numeric && unit_order[ch]);
                }
            }
          অন্যথা
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_চাবি (offset, width);
}

/* Debug LINE by underlining its keys.  */

স্থির শূন্য
debug_সারি (সংরচনা সারি ধ্রুব *সারি)
{
  সংরচনা keyfield ধ্রুব *চাবি = keylist;

  কুরু
    debug_চাবি (সারি, চাবি);
  যাবত্ (চাবি && ((চাবি = চাবি->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

স্থির bool
default_চাবি_compare (সংরচনা keyfield ধ্রুব *চাবি)
{
  প্রত্যাগম ! (চাবি->ignore
            || চাবি->translate
            || চাবি->skipsblanks
            || চাবি->skipeblanks
            || চাবি_numeric (চাবি)
            || চাবি->month
            || চাবি->version
            || চাবি->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

স্থির শূন্য
চাবি_to_opts (সংরচনা keyfield ধ্রুব *চাবি, বর্ণ *opts)
{
  যদি (চাবি->skipsblanks || চাবি->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  যদি (চাবি->ignore == nondictionary)
    *opts++ = 'd';
  যদি (চাবি->translate)
    *opts++ = 'f';
  যদি (চাবি->general_numeric)
    *opts++ = 'g';
  যদি (চাবি->human_numeric)
    *opts++ = 'h';
  যদি (চাবি->ignore == nonprinting)
    *opts++ = 'i';
  যদি (চাবি->month)
    *opts++ = 'M';
  যদি (চাবি->numeric)
    *opts++ = 'n';
  যদি (চাবি->random)
    *opts++ = 'R';
  যদি (চাবি->reverse)
    *opts++ = 'r';
  যদি (চাবি->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

স্থির শূন্য
চাবি_warnings (সংরচনা keyfield ধ্রুব *gkey, bool gkey_only)
{
  সংরচনা keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  প্রতি (সংরচনা keyfield *চাবি = keylist; চাবি; চাবি = চাবি->next, keynum++)
    {
      যদি (চাবি_numeric (চাবি))
        {
          যদি (চাবি->general_numeric)
            general_numeric_field = true;
          অন্যথা
            basic_numeric_field = true;
        }

      যদি (চাবি->traditional_used)
        {
          size_t sword = চাবি->sword;
          size_t eword = চাবি->eword;
          বর্ণ tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          বর্ণ obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          বর্ণ nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          বর্ণ *po = obuf;
          বর্ণ *pn = nbuf;

          যদি (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          যদি (চাবি->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (চাবি->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = চাবি->sword != SIZE_MAX && চাবি->eword < চাবি->sword;
      যদি (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = চাবি_numeric (চাবি) || চাবি->month;
      bool সারি_offset = চাবি->eword == 0 && চাবি->echar != 0; /* -k1.x,1.y  */
      যদি (!zero_width && !gkey_only && tab == TAB_DEFAULT && !সারি_offset
          && ((!চাবি->skipsblanks && !implicit_skip)
              || (!চাবি->skipsblanks && চাবি->schar)
              || (!চাবি->skipeblanks && চাবি->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      যদি (!gkey_only && চাবি_numeric (চাবি))
        {
          size_t sword = চাবি->sword + 1;
          size_t eword = চাবি->eword + 1;
          যদি (!sword)
            sword++;
          যদি (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              যদি (চাবি->general_numeric)
                general_numeric_field_span = true;
              অন্যথা
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      যদি (ugkey.ignore && (ugkey.ignore == চাবি->ignore))
        ugkey.ignore = রিক্ত;
      যদি (ugkey.translate && (ugkey.translate == চাবি->translate))
        ugkey.translate = রিক্ত;
      ugkey.skipsblanks &= !চাবি->skipsblanks;
      ugkey.skipeblanks &= !চাবি->skipeblanks;
      ugkey.month &= !চাবি->month;
      ugkey.numeric &= !চাবি->numeric;
      ugkey.general_numeric &= !চাবি->general_numeric;
      ugkey.human_numeric &= !চাবি->human_numeric;
      ugkey.random &= !চাবি->random;
      ugkey.version &= !চাবি->version;
      ugkey.reverse &= !চাবি->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool সংখ্যা_locale_warned = false;
  যদি (basic_numeric_field_span)
    {
      যদি (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((বর্ণ []) {thousands_sep, 0})));
          সংখ্যা_locale_warned = true;
        }
    }
  যদি (basic_numeric_field_span || general_numeric_field_span)
    {
      যদি (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((বর্ণ []) {decimal_point, 0})));
          সংখ্যা_locale_warned = true;
        }
      অন্যথা যদি (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((বর্ণ []) {tab, 0})));
        }
      অন্যথা যদি (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((বর্ণ []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  যদি ((basic_numeric_field || general_numeric_field) && ! সংখ্যা_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((বর্ণ []) {decimal_point, 0})));

    }

  যদি (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  যদি (!default_চাবি_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      যদি (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      বর্ণ opts[পরিমাণ short_options];
      চাবি_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  যদি (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

স্থির পূর্ণাঙ্ক
diff_reversed (পূর্ণাঙ্ক diff, bool reversed)
{
  প্রত্যাগম reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

স্থির পূর্ণাঙ্ক
keycompare (সংরচনা সারি ধ্রুব *a, সংরচনা সারি ধ্রুব *b)
{
  সংরচনা keyfield *চাবি = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  বর্ণ *texta = a->keybeg;
  বর্ণ *textb = b->keybeg;
  বর্ণ *lima = a->keylim;
  বর্ণ *limb = b->keylim;

  পূর্ণাঙ্ক diff;

  যাবত্ (true)
    {
      বর্ণ ধ্রুব *translate = চাবি->translate;
      bool ধ্রুব *ignore = চাবি->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      যদি (hard_LC_COLLATE || চাবি_numeric (চাবি)
          || চাবি->month || চাবি->random || চাবি->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          বর্ণ *ta = texta;
          বর্ণ *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          বর্ণ enda = ta[tlena];
          বর্ণ endb = tb[tlenb];

          শূন্য *allocated = রিক্ত;
          বর্ণ stackbuf[4000];

          যদি (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              যদি (size <= পরিমাণ stackbuf)
                ta = stackbuf;
              অন্যথা
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              প্রতি (tlena = i = 0; i < lena; i++)
                যদি (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              প্রতি (tlenb = i = 0; i < lenb; i++)
                যদি (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          যদি (চাবি->numeric)
            diff = numcompare (ta, tb);
          অন্যথা যদি (চাবি->general_numeric)
            diff = general_numcompare (ta, tb);
          অন্যথা যদি (চাবি->human_numeric)
            diff = human_numcompare (ta, tb);
          অন্যথা যদি (চাবি->month)
            diff = getmonth (ta, রিক্ত) - getmonth (tb, রিক্ত);
          অন্যথা যদি (চাবি->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          অন্যথা যদি (চাবি->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          অন্যথা
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              যদি (tlena == 0)
                diff = - NONZERO (tlenb);
              অন্যথা যদি (tlenb == 0)
                diff = 1;
              অন্যথা
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          মুক্ত (allocated);
        }
      অন্যথা যদি (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  কুরু									\
    {									\
          যাবত্ (true)							\
            {								\
              যাবত্ (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              যাবত্ (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              যদি (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ভঙ্গ;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              যদি (diff)							\
                ভঙ্গ;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  যাবত্ (0)

          যদি (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          অন্যথা
            CMP_WITH_IGNORE (*texta, *textb);
        }
      অন্যথা
        {
          size_t lenmin = MIN (lena, lenb);
          যদি (lenmin == 0)
            diff = 0;
          অন্যথা যদি (translate)
            {
              size_t i = 0;
              কুরু
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  যদি (diff)
                    ভঙ্গ;
                  i++;
                }
              যাবত্ (i < lenmin);
            }
          অন্যথা
            diff = memcmp (texta, textb, lenmin);

          যদি (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      যদি (diff)
        ভঙ্গ;

      চাবি = চাবি->next;
      যদি (! চাবি)
        প্রত্যাগম 0;

      /* Find the beginning and limit of the next field.  */
      যদি (চাবি->eword != SIZE_MAX)
        lima = limfield (a, চাবি), limb = limfield (b, চাবি);
      অন্যথা
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      যদি (চাবি->sword != SIZE_MAX)
        texta = begfield (a, চাবি), textb = begfield (b, চাবি);
      অন্যথা
        {
          texta = a->text, textb = b->text;
          যদি (চাবি->skipsblanks)
            {
              যাবত্ (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              যাবত্ (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  প্রত্যাগম diff_reversed (diff, চাবি->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

স্থির পূর্ণাঙ্ক
compare (সংরচনা সারি ধ্রুব *a, সংরচনা সারি ধ্রুব *b)
{
  পূর্ণাঙ্ক diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  যদি (keylist)
    {
      diff = keycompare (a, b);
      যদি (diff || unique || stable)
        প্রত্যাগম diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  যদি (alen == 0)
    diff = - NONZERO (blen);
  অন্যথা যদি (blen == 0)
    diff = 1;
  অন্যথা যদি (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  অন্যথা
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      যদি (!diff)
        diff = _GL_CMP (alen, blen);
    }

  প্রত্যাগম diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

স্থির শূন্য
write_সারি (সংরচনা সারি ধ্রুব *সারি, FILE *fp, বর্ণ ধ্রুব *output_file)
{
  বর্ণ *buf = সারি->text;
  size_t n_bytes = সারি->length;
  বর্ণ *ebuf = buf + n_bytes;

  যদি (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      বর্ণ ধ্রুব *c = buf;

      যাবত্ (c < ebuf)
        {
          বর্ণ wc = *c++;
          যদি (wc == '\t')
            wc = '>';
          অন্যথা যদি (c == ebuf)
            wc = '\n';
          যদি (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_সারি (সারি);
    }
  অন্যথা
    {
      ebuf[-1] = eolchar;
      যদি (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

স্থির bool
check (বর্ণ ধ্রুব *file_name, বর্ণ checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  সংরচনা buffer buf;		/* Input buffer. */
  সংরচনা সারি temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t সারি_সংখ্যা = 0;
  সংরচনা keyfield ধ্রুব *চাবি = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, পরিমাণ (সংরচনা সারি),
           MAX (merge_buffer_size, sort_size));
  temp.text = রিক্ত;

  যাবত্ (fillbuf (&buf, fp, file_name, রিক্ত))
    {
      সংরচনা সারি ধ্রুব *সারি = buffer_linelim (&buf);
      সংরচনা সারি ধ্রুব *linebase = সারি - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      যদি (alloc && nonunique <= compare (&temp, সারি - 1))
        {
        found_disorder:
          {
            যদি (checkonly == 'c')
              {
                সংরচনা সারি ধ্রুব *disorder_সারি = সারি - 1;
                uintmax_t disorder_সারি_সংখ্যা =
                  buffer_linelim (&buf) - disorder_সারি + সারি_সংখ্যা;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_সারি_সংখ্যা);
                write_সারি (disorder_সারি, stderr, _("standard error"));
              }

            ordered = false;
            ভঙ্গ;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      যাবত্ (linebase < --সারি)
        যদি (nonunique <= compare (সারি, সারি - 1))
          গচ্ছ found_disorder;

      সারি_সংখ্যা += buf.nlines;

      /* Save the last line of the buffer.  */
      যদি (alloc < সারি->length)
        {
          কুরু
            {
              alloc *= 2;
              যদি (! alloc)
                {
                  alloc = সারি->length;
                  ভঙ্গ;
                }
            }
          যাবত্ (alloc < সারি->length);

          মুক্ত (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, সারি->text, সারি->length);
      temp.length = সারি->length;
      যদি (চাবি)
        {
          temp.keybeg = temp.text + (সারি->keybeg - সারি->text);
          temp.keylim = temp.text + (সারি->keylim - সারি->text);
        }
    }

  xfclose (fp, file_name);
  মুক্ত (buf.buf);
  মুক্ত (temp.text);
  প্রত্যাগম ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

স্থির size_t
open_প্রবেশ_files (সংরচনা sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, পরিমাণ *fps);
  পূর্ণাঙ্ক i;

  /* Open as many input files as we can.  */
  প্রতি (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      যদি (!fps[i])
        ভঙ্গ;
    }

  প্রত্যাগম i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

স্থির শূন্য
mergefps (সংরচনা sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, বর্ণ ধ্রুব *output_file, FILE **fps)
{
  সংরচনা buffer *buffer = xnmalloc (nfiles, পরিমাণ *buffer);
                                /* Input buffers for each file. */
  সংরচনা সারি saved;		/* Saved line storage for unique check. */
  সংরচনা সারি ধ্রুব *savedline = রিক্ত;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  সংরচনা সারি ধ্রুব **cur = xnmalloc (nfiles, পরিমাণ *cur);
                                /* Current line in each line table. */
  সংরচনা সারি ধ্রুব **base = xnmalloc (nfiles, পরিমাণ *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, পরিমাণ *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  সংরচনা keyfield ধ্রুব *চাবি = keylist;
  saved.text = রিক্ত;

  /* Read initial lines from each input file. */
  প্রতি (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], পরিমাণ (সংরচনা সারি),
               MAX (merge_buffer_size, sort_size / nfiles));
      যদি (fillbuf (&buffer[i], fps[i], files[i].name, রিক্ত))
        {
          সংরচনা সারি ধ্রুব *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      অন্যথা
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          যদি (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          মুক্ত (buffer[i].buf);
          --nfiles;
          প্রতি (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  প্রতি (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  প্রতি (size_t i = 1; i < nfiles; ++i)
    যদি (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  যাবত্ (nfiles)
    {
      সংরচনা সারি ধ্রুব *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      যদি (unique)
        {
          যদি (savedline && compare (savedline, smallest))
            {
              savedline = রিক্ত;
              write_সারি (&saved, ofp, output_file);
            }
          যদি (!savedline)
            {
              savedline = &saved;
              যদি (savealloc < smallest->length)
                {
                  কুরু
                    যদি (! savealloc)
                      {
                        savealloc = smallest->length;
                        ভঙ্গ;
                      }
                  যাবত্ ((savealloc *= 2) < smallest->length);

                  মুক্ত (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              যদি (চাবি)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      অন্যথা
        write_সারি (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      যদি (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      অন্যথা
        {
          যদি (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       রিক্ত))
            {
              সংরচনা সারি ধ্রুব *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          অন্যথা
            {
              /* We reached EOF on fps[ord[0]].  */
              প্রতি (size_t i = 1; i < nfiles; ++i)
                যদি (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              যদি (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              মুক্ত (buffer[ord[0]].buf);
              প্রতি (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              প্রতি (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              অনুবর্ত;
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
        size_t গুনতি_of_smaller_সারিগুলি;

        যাবত্ (lo < hi)
          {
            পূর্ণাঙ্ক cmp = compare (cur[ord0], cur[ord[probe]]);
            যদি (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            অন্যথা
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        গুনতি_of_smaller_সারিগুলি = lo - 1;
        প্রতি (size_t j = 0; j < গুনতি_of_smaller_সারিগুলি; j++)
          ord[j] = ord[j + 1];
        ord[গুনতি_of_smaller_সারিগুলি] = ord0;
      }
    }

  যদি (unique && savedline)
    {
      write_সারি (&saved, ofp, output_file);
      মুক্ত (saved.text);
    }

  xfclose (ofp, output_file);
  মুক্ত (fps);
  মুক্ত (buffer);
  মুক্ত (ord);
  মুক্ত (base);
  মুক্ত (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

স্থির size_t
mergefiles (সংরচনা sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, বর্ণ ধ্রুব *output_file)
{
  FILE **fps;
  size_t nopened = open_প্রবেশ_files (files, nfiles, &fps);
  যদি (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  প্রত্যাগম nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

স্থির শূন্য
mergelines (সংরচনা সারি *restrict t, size_t nlines,
            সংরচনা সারি ধ্রুব *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  সংরচনা সারি *hi = t - nlo;

  যাবত্ (true)
    যদি (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        যদি (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            প্রত্যাগম;
          }
      }
    অন্যথা
      {
        *--t = *--hi;
        যদি (! --nhi)
          {
            কুরু
              *--t = *--lo;
            যাবত্ (--nlo);

            প্রত্যাগম;
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

স্থির শূন্য
sequential_sort (সংরচনা সারি *restrict সারিগুলি, size_t nlines,
                 সংরচনা সারি *restrict temp, bool to_temp)
{
  যদি (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      পূর্ণাঙ্ক swap = (0 < compare (&সারিগুলি[-1], &সারিগুলি[-2]));
      যদি (to_temp)
        {
          temp[-1] = সারিগুলি[-1 - swap];
          temp[-2] = সারিগুলি[-2 + swap];
        }
      অন্যথা যদি (swap)
        {
          temp[-1] = সারিগুলি[-1];
          সারিগুলি[-1] = সারিগুলি[-2];
          সারিগুলি[-2] = temp[-1];
        }
    }
  অন্যথা
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      সংরচনা সারি *lo = সারিগুলি;
      সংরচনা সারি *hi = সারিগুলি - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      যদি (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      অন্যথা যদি (!to_temp)
        temp[-1] = lo[-1];

      সংরচনা সারি *dest;
      সংরচনা সারি ধ্রুব *sorted_lo;
      যদি (to_temp)
        {
          dest = temp;
          sorted_lo = সারিগুলি;
        }
      অন্যথা
        {
          dest = সারিগুলি;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

স্থির সংরচনা merge_গ্রন্থি *init_গ্রন্থি (সংরচনা merge_গ্রন্থি *restrict,
                                     সংরচনা merge_গ্রন্থি *restrict,
                                     সংরচনা সারি *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
স্থির সংরচনা merge_গ্রন্থি *
merge_বৃক্ষ_init (size_t nthreads, size_t nlines, সংরচনা সারি *dest)
{
  সংরচনা merge_গ্রন্থি *merge_বৃক্ষ = xmalloc (2 * পরিমাণ *merge_বৃক্ষ * nthreads);

  সংরচনা merge_গ্রন্থি *মূল = merge_বৃক্ষ;
  মূল->lo = মূল->hi = মূল->end_lo = মূল->end_hi = রিক্ত;
  মূল->dest = রিক্ত;
  মূল->nlo = মূল->nhi = nlines;
  মূল->parent = রিক্ত;
  মূল->level = MERGE_END;
  মূল->queued = false;
  pthread_mutex_init (&মূল->lock, রিক্ত);

  init_গ্রন্থি (মূল, মূল + 1, dest, nthreads, nlines, false);
  প্রত্যাগম merge_বৃক্ষ;
}

/* Destroy the merge tree. */
স্থির শূন্য
merge_বৃক্ষ_destroy (size_t nthreads, সংরচনা merge_গ্রন্থি *merge_বৃক্ষ)
{
  size_t n_nodes = nthreads * 2;
  সংরচনা merge_গ্রন্থি *গ্রন্থি = merge_বৃক্ষ;

  যাবত্ (n_nodes--)
    {
      pthread_mutex_destroy (&গ্রন্থি->lock);
      গ্রন্থি++;
    }

  মুক্ত (merge_বৃক্ষ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

স্থির সংরচনা merge_গ্রন্থি *
init_গ্রন্থি (সংরচনা merge_গ্রন্থি *restrict parent,
           সংরচনা merge_গ্রন্থি *restrict গ্রন্থি_pool,
           সংরচনা সারি *dest, size_t nthreads,
           size_t total_সারিগুলি, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  সংরচনা সারি *lo = dest - total_সারিগুলি;
  সংরচনা সারি *hi = lo - nlo;
  সংরচনা সারি **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  সংরচনা merge_গ্রন্থি *গ্রন্থি = গ্রন্থি_pool++;
  গ্রন্থি->lo = গ্রন্থি->end_lo = lo;
  গ্রন্থি->hi = গ্রন্থি->end_hi = hi;
  গ্রন্থি->dest = parent_end;
  গ্রন্থি->nlo = nlo;
  গ্রন্থি->nhi = nhi;
  গ্রন্থি->parent = parent;
  গ্রন্থি->level = parent->level + 1;
  গ্রন্থি->queued = false;
  pthread_mutex_init (&গ্রন্থি->lock, রিক্ত);

  যদি (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      গ্রন্থি->lo_child = গ্রন্থি_pool;
      গ্রন্থি_pool = init_গ্রন্থি (গ্রন্থি, গ্রন্থি_pool, lo, lo_threads,
                             total_সারিগুলি, true);
      গ্রন্থি->hi_child = গ্রন্থি_pool;
      গ্রন্থি_pool = init_গ্রন্থি (গ্রন্থি, গ্রন্থি_pool, hi, hi_threads,
                             total_সারিগুলি, false);
    }
  অন্যথা
    {
      গ্রন্থি->lo_child = রিক্ত;
      গ্রন্থি->hi_child = রিক্ত;
    }
  প্রত্যাগম গ্রন্থি_pool;
}


/* Compare two merge nodes A and B for priority.  */

স্থির পূর্ণাঙ্ক
compare_nodes (শূন্য ধ্রুব *a, শূন্য ধ্রুব *b)
{
  সংরচনা merge_গ্রন্থি ধ্রুব *nodea = a;
  সংরচনা merge_গ্রন্থি ধ্রুব *nodeb = b;
  যদি (nodea->level == nodeb->level)
      প্রত্যাগম (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  প্রত্যাগম nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

স্থির অন্তঃস্থ শূন্য
lock_গ্রন্থি (সংরচনা merge_গ্রন্থি *গ্রন্থি)
{
  pthread_mutex_lock (&গ্রন্থি->lock);
}

/* Unlock a merge tree NODE. */

স্থির অন্তঃস্থ শূন্য
unlock_গ্রন্থি (সংরচনা merge_গ্রন্থি *গ্রন্থি)
{
  pthread_mutex_unlock (&গ্রন্থি->lock);
}

/* Destroy merge QUEUE. */

স্থির শূন্য
queue_destroy (সংরচনা merge_গ্রন্থি_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

স্থির শূন্য
queue_init (সংরচনা merge_গ্রন্থি_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, রিক্ত);
  pthread_cond_init (&queue->cond, রিক্ত);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

স্থির শূন্য
queue_ঢোকাও (সংরচনা merge_গ্রন্থি_queue *queue, সংরচনা merge_গ্রন্থি *গ্রন্থি)
{
  pthread_mutex_lock (&queue->mutex);
  heap_ঢোকাও (queue->priority_queue, গ্রন্থি);
  গ্রন্থি->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

স্থির সংরচনা merge_গ্রন্থি *
queue_বের_করো (সংরচনা merge_গ্রন্থি_queue *queue)
{
  সংরচনা merge_গ্রন্থি *গ্রন্থি;
  pthread_mutex_lock (&queue->mutex);
  যাবত্ (! (গ্রন্থি = heap_remove_শীর্ষ (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_গ্রন্থি (গ্রন্থি);
  গ্রন্থি->queued = false;
  প্রত্যাগম গ্রন্থি;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

স্থির শূন্য
write_unique (সংরচনা সারি ধ্রুব *সারি, FILE *tfp, বর্ণ ধ্রুব *temp_output)
{
  যদি (unique)
    {
      যদি (saved_সারি.text && ! compare (সারি, &saved_সারি))
        প্রত্যাগম;
      saved_সারি = *সারি;
    }

  write_সারি (সারি, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

স্থির শূন্য
mergelines_গ্রন্থি (সংরচনা merge_গ্রন্থি *restrict গ্রন্থি, size_t total_সারিগুলি,
                 FILE *tfp, বর্ণ ধ্রুব *temp_output)
{
  সংরচনা সারি *lo_orig = গ্রন্থি->lo;
  সংরচনা সারি *hi_orig = গ্রন্থি->hi;
  size_t to_merge = MAX_MERGE (total_সারিগুলি, গ্রন্থি->level);
  size_t merged_lo;
  size_t merged_hi;

  যদি (গ্রন্থি->level > MERGE_মূল)
    {
      /* Merge to destination buffer. */
      সংরচনা সারি *dest = *গ্রন্থি->dest;
      যাবত্ (গ্রন্থি->lo != গ্রন্থি->end_lo && গ্রন্থি->hi != গ্রন্থি->end_hi && to_merge--)
        যদি (compare (গ্রন্থি->lo - 1, গ্রন্থি->hi - 1) <= 0)
          *--dest = *--গ্রন্থি->lo;
        অন্যথা
          *--dest = *--গ্রন্থি->hi;

      merged_lo = lo_orig - গ্রন্থি->lo;
      merged_hi = hi_orig - গ্রন্থি->hi;

      যদি (গ্রন্থি->nhi == merged_hi)
        যাবত্ (গ্রন্থি->lo != গ্রন্থি->end_lo && to_merge--)
          *--dest = *--গ্রন্থি->lo;
      অন্যথা যদি (গ্রন্থি->nlo == merged_lo)
        যাবত্ (গ্রন্থি->hi != গ্রন্থি->end_hi && to_merge--)
          *--dest = *--গ্রন্থি->hi;
      *গ্রন্থি->dest = dest;
    }
  অন্যথা
    {
      /* Merge directly to output. */
      যাবত্ (গ্রন্থি->lo != গ্রন্থি->end_lo && গ্রন্থি->hi != গ্রন্থি->end_hi && to_merge--)
        {
          যদি (compare (গ্রন্থি->lo - 1, গ্রন্থি->hi - 1) <= 0)
            write_unique (--গ্রন্থি->lo, tfp, temp_output);
          অন্যথা
            write_unique (--গ্রন্থি->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - গ্রন্থি->lo;
      merged_hi = hi_orig - গ্রন্থি->hi;

      যদি (গ্রন্থি->nhi == merged_hi)
        {
          যাবত্ (গ্রন্থি->lo != গ্রন্থি->end_lo && to_merge--)
            write_unique (--গ্রন্থি->lo, tfp, temp_output);
        }
      অন্যথা যদি (গ্রন্থি->nlo == merged_lo)
        {
          যাবত্ (গ্রন্থি->hi != গ্রন্থি->end_hi && to_merge--)
            write_unique (--গ্রন্থি->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - গ্রন্থি->lo;
  merged_hi = hi_orig - গ্রন্থি->hi;
  গ্রন্থি->nlo -= merged_lo;
  গ্রন্থি->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

স্থির শূন্য
queue_check_ঢোকাও (সংরচনা merge_গ্রন্থি_queue *queue, সংরচনা merge_গ্রন্থি *গ্রন্থি)
{
  যদি (! গ্রন্থি->queued)
    {
      bool lo_avail = (গ্রন্থি->lo - গ্রন্থি->end_lo) != 0;
      bool hi_avail = (গ্রন্থি->hi - গ্রন্থি->end_hi) != 0;
      যদি (lo_avail ? hi_avail || ! গ্রন্থি->nhi : hi_avail && ! গ্রন্থি->nlo)
        queue_ঢোকাও (queue, গ্রন্থি);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

স্থির শূন্য
queue_check_ঢোকাও_parent (সংরচনা merge_গ্রন্থি_queue *queue,
                           সংরচনা merge_গ্রন্থি *গ্রন্থি)
{
  যদি (গ্রন্থি->level > MERGE_মূল)
    {
      lock_গ্রন্থি (গ্রন্থি->parent);
      queue_check_ঢোকাও (queue, গ্রন্থি->parent);
      unlock_গ্রন্থি (গ্রন্থি->parent);
    }
  অন্যথা যদি (গ্রন্থি->nlo + গ্রন্থি->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_ঢোকাও (queue, গ্রন্থি->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

স্থির শূন্য
merge_loop (সংরচনা merge_গ্রন্থি_queue *queue,
            size_t total_সারিগুলি, FILE *tfp, বর্ণ ধ্রুব *temp_output)
{
  যাবত্ (true)
    {
      সংরচনা merge_গ্রন্থি *গ্রন্থি = queue_বের_করো (queue);

      যদি (গ্রন্থি->level == MERGE_END)
        {
          unlock_গ্রন্থি (গ্রন্থি);
          /* Reinsert so other threads can pop it. */
          queue_ঢোকাও (queue, গ্রন্থি);
          ভঙ্গ;
        }
      mergelines_গ্রন্থি (গ্রন্থি, total_সারিগুলি, tfp, temp_output);
      queue_check_ঢোকাও (queue, গ্রন্থি);
      queue_check_ঢোকাও_parent (queue, গ্রন্থি);

      unlock_গ্রন্থি (গ্রন্থি);
    }
}


স্থির শূন্য sortlines (সংরচনা সারি *restrict, size_t, size_t,
                       সংরচনা merge_গ্রন্থি *, সংরচনা merge_গ্রন্থি_queue *,
                       FILE *, বর্ণ ধ্রুব *);

/* Thread arguments for sortlines_thread. */

সংরচনা thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  সংরচনা সারি *সারিগুলি;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ধ্রুব total_সারিগুলি;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  সংরচনা merge_গ্রন্থি *ধ্রুব গ্রন্থি;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  সংরচনা merge_গ্রন্থি_queue *ধ্রুব queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  বর্ণ ধ্রুব *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

স্থির শূন্য *
sortlines_thread (শূন্য *data)
{
  সংরচনা thread_args ধ্রুব *args = data;
  sortlines (args->সারিগুলি, args->nthreads, args->total_সারিগুলি,
             args->গ্রন্থি, args->queue, args->tfp,
             args->output_temp);
  প্রত্যাগম রিক্ত;
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

স্থির শূন্য
sortlines (সংরচনা সারি *restrict সারিগুলি, size_t nthreads,
           size_t total_সারিগুলি, সংরচনা merge_গ্রন্থি *গ্রন্থি,
           সংরচনা merge_গ্রন্থি_queue *queue, FILE *tfp, বর্ণ ধ্রুব *temp_output)
{
  size_t nlines = গ্রন্থি->nlo + গ্রন্থি->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  সংরচনা thread_args args = {সারিগুলি, lo_threads, total_সারিগুলি,
                             গ্রন্থি->lo_child, queue, tfp, temp_output};

  যদি (nthreads > 1 && SUBTHREAD_সারিগুলি_HEURISTIC <= nlines
      && pthread_create (&thread, রিক্ত, sortlines_thread, &args) == 0)
    {
      sortlines (সারিগুলি - গ্রন্থি->nlo, hi_threads, total_সারিগুলি,
                 গ্রন্থি->hi_child, queue, tfp, temp_output);
      pthread_join (thread, রিক্ত);
    }
  অন্যথা
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = গ্রন্থি->nlo;
      size_t nhi = গ্রন্থি->nhi;
      সংরচনা সারি *temp = সারিগুলি - total_সারিগুলি;
      যদি (1 < nhi)
        sequential_sort (সারিগুলি - nlo, nhi, temp - nlo / 2, false);
      যদি (1 < nlo)
        sequential_sort (সারিগুলি, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      গ্রন্থি->lo = সারিগুলি;
      গ্রন্থি->hi = সারিগুলি - nlo;
      গ্রন্থি->end_lo = সারিগুলি - nlo;
      গ্রন্থি->end_hi = সারিগুলি - nlo - nhi;

      queue_ঢোকাও (queue, গ্রন্থি);
      merge_loop (queue, total_সারিগুলি, tfp, temp_output);
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

স্থির শূন্য
avoid_trashing_প্রবেশ (সংরচনা sortfile *files, size_t ntemps,
                      size_t nfiles, বর্ণ ধ্রুব *outfile)
{
  সংরচনা tempnode *tempcopy = রিক্ত;

  প্রতি (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      সংরচনা stat instat;

      যদি (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      অন্যথা
        {
          সংরচনা stat *outst = get_outstatus ();
          যদি (!outst)
            ভঙ্গ;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      যদি (same)
        {
          যদি (! tempcopy)
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

স্থির শূন্য
check_inputs (বর্ণ *ধ্রুব *files, size_t nfiles)
{
  প্রতি (size_t i = 0; i < nfiles; i++)
    {
      যদি (streq (files[i], "-"))
        অনুবর্ত;

      যদি (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

স্থির শূন্য
check_output (বর্ণ ধ্রুব *outfile)
{
  যদি (outfile)
    {
      পূর্ণাঙ্ক oflags = O_WRONLY | O_দ্বিমিক | O_CLOEXEC | O_CREAT;
      পূর্ণাঙ্ক outfd = open (outfile, oflags, MODE_RW_UGO);
      যদি (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

স্থির শূন্য
merge (সংরচনা sortfile *files, size_t ntemps, size_t nfiles,
       বর্ণ ধ্রুব *output_file)
{
  যাবত্ (nmerge < nfiles)
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
      প্রতি (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          সংরচনা tempnode *temp = create_temp (&tfp);
          size_t অঙ্ক_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, অঙ্ক_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += অঙ্ক_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      যদি (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          সংরচনা tempnode *temp = create_temp (&tfp);
          size_t অঙ্ক_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, অঙ্ক_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += অঙ্ক_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * পরিমাণ *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_প্রবেশ (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  যাবত্ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_প্রবেশ_files (files, nfiles, &fps);

      যদি (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          যদি (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ভঙ্গ;
            }
          যদি (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      অন্যথা যদি (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      সংরচনা tempnode *temp;
      কুরু
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      যাবত্ (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * পরিমাণ *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

স্থির শূন্য
sort (বর্ণ *ধ্রুব *files, size_t nfiles, বর্ণ ধ্রুব *output_file,
      size_t nthreads)
{
  সংরচনা buffer buf;
  সংরচনা sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  যাবত্ (nfiles)
    {
      বর্ণ ধ্রুব *temp_output;
      বর্ণ ধ্রুব *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_সারি;
      যদি (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          যাবত্ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_সারি = (mult * পরিমাণ (সংরচনা সারি));
        }
      অন্যথা
        bytes_per_সারি = পরিমাণ (সংরচনা সারি) * 3 / 2;

      যদি (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_সারি,
                              &policy);
          initbuf (&buf, bytes_per_সারি, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      যাবত্ (fillbuf (&buf, fp, file, &policy))
        {
          সংরচনা সারি *সারি;

          যদি (buf.eof && nfiles
              && (bytes_per_সারি + 1
                  < (buf.alloc - buf.used - bytes_per_সারি * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.বাম = buf.used;
              ভঙ্গ;
            }

          saved_সারি.text = রিক্ত;
          সারি = buffer_linelim (&buf);
          যদি (buf.eof && !nfiles && !ntemps && !buf.বাম)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          অন্যথা
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          যদি (1 < buf.nlines)
            {
              যদি (nthreads > 1)
                {
                  সংরচনা merge_গ্রন্থি_queue queue;
                  queue_init (&queue, nthreads);
                  সংরচনা merge_গ্রন্থি *merge_বৃক্ষ =
                    merge_বৃক্ষ_init (nthreads, buf.nlines, সারি);

                  sortlines (সারি, nthreads, buf.nlines, merge_বৃক্ষ + 1,
                             &queue, tfp, temp_output);

                  merge_বৃক্ষ_destroy (nthreads, merge_বৃক্ষ);
                  queue_destroy (&queue);
                }
              অন্যথা
                {
                  sequential_sort (সারি, buf.nlines,
                                   সারি - buf.nlines, false);
                  প্রতি (size_t i = 0; i < buf.nlines; i++)
                    write_unique (সারি - i - 1, tfp, temp_output);
                }
            }
          অন্যথা
            write_unique (সারি - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          যদি (output_file_created)
            গচ্ছ finish;
        }
      xfclose (fp, file);
    }

 finish:
  মুক্ত (buf.buf);

  যদি (! output_file_created)
    {
      সংরচনা tempnode *গ্রন্থি = temphead;
      সংরচনা sortfile *tempfiles = xnmalloc (ntemps, পরিমাণ *tempfiles);
      প্রতি (size_t i = 0; গ্রন্থি; i++)
        {
          tempfiles[i].name = গ্রন্থি->name;
          tempfiles[i].temp = গ্রন্থি;
          গ্রন্থি = গ্রন্থি->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      মুক্ত (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

স্থির শূন্য
insertkey (সংরচনা keyfield *চাবি_arg)
{
  সংরচনা keyfield **p;
  সংরচনা keyfield *চাবি = xmemdup (চাবি_arg, পরিমাণ *চাবি);

  প্রতি (p = &keylist; *p; p = &(*p)->next)
    অনুবর্ত;
  *p = চাবি;
  চাবি->next = রিক্ত;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

স্থির শূন্য
badfieldspec (বর্ণ ধ্রুব *spec, বর্ণ ধ্রুব *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

স্থির শূন্য
incompatible_options (বর্ণ ধ্রুব *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

স্থির শূন্য
check_ordering_compatibility (শূন্য)
{
  প্রতি (সংরচনা keyfield *চাবি = keylist; চাবি; চাবি = চাবি->next)
    যদি (1 < (চাবি->numeric + চাবি->general_numeric + চাবি->human_numeric
             + চাবি->month + (চাবি->version | চাবি->random | !!চাবি->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        বর্ণ opts[পরিমাণ short_options];
        /* Clear flags we're not interested in.  */
        চাবি->skipsblanks = চাবি->skipeblanks = চাবি->reverse = false;
        চাবি_to_opts (চাবি, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

স্থির বর্ণ ধ্রুব *
parse_field_গুনতি (বর্ণ ধ্রুব *string, size_t *val, বর্ণ ধ্রুব *msgid)
{
  বর্ণ *suffix;
  uintmax_t n;

  বিকল্প (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    পক্ষ LONGINT_OK:
    পক্ষ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      যদি (*val == n)
        ভঙ্গ;
      FALLTHROUGH;
    পক্ষ LONGINT_OVERFLOW:
    পক্ষ LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ভঙ্গ;

    পক্ষ LONGINT_INVALID:
      যদি (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      প্রত্যাগম রিক্ত;
    }

  প্রত্যাগম suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

স্থির বর্ণ *
set_ordering (বর্ণ ধ্রুব *s, সংরচনা keyfield *চাবি, গণনা blanktype blanktype)
{
  যাবত্ (*s)
    {
      বিকল্প (*s)
        {
        পক্ষ 'b':
          যদি (blanktype == bl_start || blanktype == bl_both)
            চাবি->skipsblanks = true;
          যদি (blanktype == bl_end || blanktype == bl_both)
            চাবি->skipeblanks = true;
          ভঙ্গ;
        পক্ষ 'd':
          চাবি->ignore = nondictionary;
          ভঙ্গ;
        পক্ষ 'f':
          চাবি->translate = fold_toupper;
          ভঙ্গ;
        পক্ষ 'g':
          চাবি->general_numeric = true;
          ভঙ্গ;
        পক্ষ 'h':
          চাবি->human_numeric = true;
          ভঙ্গ;
        পক্ষ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          যদি (! চাবি->ignore)
            চাবি->ignore = nonprinting;
          ভঙ্গ;
        পক্ষ 'M':
          চাবি->month = true;
          ভঙ্গ;
        পক্ষ 'n':
          চাবি->numeric = true;
          ভঙ্গ;
        পক্ষ 'R':
          চাবি->random = true;
          ভঙ্গ;
        পক্ষ 'r':
          চাবি->reverse = true;
          ভঙ্গ;
        পক্ষ 'V':
          চাবি->version = true;
          ভঙ্গ;
        স্বভাব:
          প্রত্যাগম (বর্ণ *) s;
        }
      ++s;
    }
  প্রত্যাগম (বর্ণ *) s;
}

/* Initialize KEY.  */

স্থির সংরচনা keyfield *
চাবি_init (সংরচনা keyfield *চাবি)
{
  memset (চাবি, 0, পরিমাণ *চাবি);
  চাবি->eword = SIZE_MAX;
  প্রত্যাগম চাবি;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  সংরচনা keyfield *চাবি;
  সংরচনা keyfield চাবি_buf;
  সংরচনা keyfield gkey;
  bool gkey_only = false;
  বর্ণ ধ্রুব *s;
  পূর্ণাঙ্ক c = 0;
  বর্ণ checkonly = 0;
  bool mergeonly = false;
  বর্ণ *random_source = রিক্ত;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != রিক্ত);
  পূর্ণাঙ্ক posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  বর্ণ **files;
  বর্ণ *files_from = রিক্ত;
  সংরচনা Tokens tok;
  বর্ণ ধ্রুব *outfile = রিক্ত;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#যদি HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    সংরচনা lconv ধ্রুব *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    যদি (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    যদি (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    যদি (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    গণনা { nsigs = countof (term_sig) };

    সংরচনা sigaction act;

    sigemptyset (&caught_signals);
    প্রতি (size_t i = 0; i < nsigs; i++)
      {
        যদি (term_sig[i] == SIGPIPE)
          অনুবর্ত;  /* Handled below.  */
        sigaction (term_sig[i], রিক্ত, &act);
        যদি (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    প্রতি (size_t i = 0; i < nsigs; i++)
      {
        যদি (term_sig[i] == SIGPIPE)
          অনুবর্ত;  /* Handled below.  */
        যদি (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, রিক্ত);
      }
  }
  সঙ্কেত (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  চাবি_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, পরিমাণ *files);

  যাবত্ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      পূর্ণাঙ্ক oi = -1;

      যদি (c == -1
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
          যদি (argc <= optind)
            ভঙ্গ;
          files[nfiles++] = argv[optind++];
        }
      অন্যথা বিকল্প (c)
        {
        পক্ষ 1:
          চাবি = রিক্ত;
          যদি (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              যদি (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  চাবি = চাবি_init (&চাবি_buf);
                  s = parse_field_গুনতি (optarg + 1, &চাবি->sword, রিক্ত);
                  যদি (s && *s == '.')
                    s = parse_field_গুনতি (s + 1, &চাবি->schar, রিক্ত);
                  যদি (! (চাবি->sword || চাবি->schar))
                    চাবি->sword = SIZE_MAX;
                  যদি (! s || *set_ordering (s, চাবি, bl_start))
                    চাবি = রিক্ত;
                  অন্যথা
                    {
                      যদি (minus_pos_usage)
                        {
                          বর্ণ ধ্রুব *optarg1 = argv[optind++];
                          s = parse_field_গুনতি (optarg1 + 1, &চাবি->eword,
                                             N_("invalid number after '-'"));
                          যদি (*s == '.')
                            s = parse_field_গুনতি (s + 1, &চাবি->echar,
                                               N_("invalid number after '.'"));
                          যদি (!চাবি->echar && চাবি->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              চাবি->eword--;
                            }
                          যদি (*set_ordering (s, চাবি, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      চাবি->traditional_used = true;
                      insertkey (চাবি);
                    }
                }
            }
          যদি (! চাবি)
            files[nfiles++] = optarg;
          ভঙ্গ;

        পক্ষ SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        পক্ষ 'b':
        পক্ষ 'd':
        পক্ষ 'f':
        পক্ষ 'g':
        পক্ষ 'h':
        পক্ষ 'i':
        পক্ষ 'M':
        পক্ষ 'n':
        পক্ষ 'r':
        পক্ষ 'R':
        পক্ষ 'V':
          {
            বর্ণ str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          ভঙ্গ;

        পক্ষ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        পক্ষ 'c':
        পক্ষ 'C':
          যদি (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ভঙ্গ;

        পক্ষ COMPRESS_PROGRAM_OPTION:
          যদি (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          ভঙ্গ;

        পক্ষ DEBUG_PROGRAM_OPTION:
          debug = true;
          ভঙ্গ;

        পক্ষ FILES0_FROM_OPTION:
          files_from = optarg;
          ভঙ্গ;

        পক্ষ 'k':
          চাবি = চাবি_init (&চাবি_buf);

          /* Get POS1. */
          s = parse_field_গুনতি (optarg, &চাবি->sword,
                                 N_("invalid number at field start"));
          যদি (! চাবি->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          যদি (*s == '.')
            {
              s = parse_field_গুনতি (s + 1, &চাবি->schar,
                                     N_("invalid number after '.'"));
              যদি (! চাবি->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          যদি (! (চাবি->sword || চাবি->schar))
            চাবি->sword = SIZE_MAX;
          s = set_ordering (s, চাবি, bl_start);
          যদি (*s != ',')
            {
              চাবি->eword = SIZE_MAX;
              চাবি->echar = 0;
            }
          অন্যথা
            {
              /* Get POS2. */
              s = parse_field_গুনতি (s + 1, &চাবি->eword,
                                     N_("invalid number after ','"));
              যদি (! চাবি->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              যদি (*s == '.')
                {
                  s = parse_field_গুনতি (s + 1, &চাবি->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, চাবি, bl_end);
            }
          যদি (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (চাবি);
          ভঙ্গ;

        পক্ষ 'm':
          mergeonly = true;
          ভঙ্গ;

        পক্ষ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ভঙ্গ;

        পক্ষ 'o':
          যদি (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ভঙ্গ;

        পক্ষ RANDOM_SOURCE_OPTION:
          যদি (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          ভঙ্গ;

        পক্ষ 's':
          stable = true;
          ভঙ্গ;

        পক্ষ 'S':
          specify_sort_size (oi, c, optarg);
          ভঙ্গ;

        পক্ষ 't':
          {
            বর্ণ newtab = optarg[0];
            যদি (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            যদি (optarg[1])
              {
                যদি (streq (optarg, "\\0"))
                  newtab = '\0';
                অন্যথা
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            যদি (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ভঙ্গ;

        পক্ষ 'T':
          add_temp_dir (optarg);
          ভঙ্গ;

        পক্ষ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ভঙ্গ;

        পক্ষ 'u':
          unique = true;
          ভঙ্গ;

        পক্ষ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          যদি (optarg == argv[optind - 1])
            {
              বর্ণ ধ্রুব *p;
              প্রতি (p = optarg; c_isdigit (*p); p++)
                অনুবর্ত;
              optind -= (*p != '\0');
            }
          ভঙ্গ;

        পক্ষ 'z':
          eolchar = 0;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (সঙ্কেত (SIGPIPE, SIG_IGN) == SIG_DFL);

  যদি (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      যদি (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      যদি (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      যদি (tok.n_tok)
        {
          মুক্ত (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          প্রতি (size_t i = 0; i < nfiles; i++)
            {
              যদি (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              অন্যথা যদি (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long পূর্ণাঙ্ক file_সংখ্যা = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_সংখ্যা);
                }
            }
        }
      অন্যথা
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  প্রতি (চাবি = keylist; চাবি; চাবি = চাবি->next)
    {
      যদি (default_চাবি_compare (চাবি) && !চাবি->reverse)
        {
          চাবি->ignore = gkey.ignore;
          চাবি->translate = gkey.translate;
          চাবি->skipsblanks = gkey.skipsblanks;
          চাবি->skipeblanks = gkey.skipeblanks;
          চাবি->month = gkey.month;
          চাবি->numeric = gkey.numeric;
          চাবি->general_numeric = gkey.general_numeric;
          চাবি->human_numeric = gkey.human_numeric;
          চাবি->version = gkey.version;
          চাবি->random = gkey.random;
          চাবি->reverse = gkey.reverse;
        }

      need_random |= চাবি->random;
    }

  যদি (!keylist && !default_চাবি_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  যদি (debug)
    {
      যদি (checkonly || outfile)
        {
          স্থির বর্ণ opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      যদি (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      যদি (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      যদি (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, রিক্ত)));
      অন্যথা
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      চাবি_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  যদি (need_random)
    random_md5_state_init (random_source);

  যদি (temp_dirs.n_dirs == 0)
    {
      বর্ণ ধ্রুব *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  যদি (nfiles == 0)
    {
      nfiles = 1;
      মুক্ত (files);
      files = xmalloc (পরিমাণ *files);
      *files = (বর্ণ *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  যদি (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  যদি (checkonly)
    {
      যদি (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      যদি (outfile)
        {
          স্থির বর্ণ opts[] = {0, 'o', 0};
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

  যদি (mergeonly)
    {
      সংরচনা sortfile *sortfiles = xcalloc (nfiles, পরিমাণ *sortfiles);

      প্রতি (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  অন্যথা
    {
      যদি (!nthreads)
        {
          unsigned long পূর্ণাঙ্ক np = অঙ্ক_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * পরিমাণ (সংরচনা merge_গ্রন্থি));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  যদি (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
