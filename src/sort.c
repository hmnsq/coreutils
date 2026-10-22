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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <pthread.h>
#ଅନ୍ତର୍ଭାଵ <sys/resource.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <spawn.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "filevercmp.h"
#ଅନ୍ତର୍ଭାଵ "findprog.h"
#ଅନ୍ତର୍ଭାଵ "flexmember.h"
#ଅନ୍ତର୍ଭାଵ "hard-locale.h"
#ଅନ୍ତର୍ଭାଵ "hash.h"
#ଅନ୍ତର୍ଭାଵ "heap.h"
#ଅନ୍ତର୍ଭାଵ "ignore-value.h"
#ଅନ୍ତର୍ଭାଵ "mbswidth.h"
#ଅନ୍ତର୍ଭାଵ "nproc.h"
#ଅନ୍ତର୍ଭାଵ "physmem.h"
#ଅନ୍ତର୍ଭାଵ "posixver.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "randread.h"
#ଅନ୍ତର୍ଭାଵ "readtokens0.h"
#ଅନ୍ତର୍ଭାଵ "stdlib--.h"
#ଅନ୍ତର୍ଭାଵ "strnumcmp.h"
#ଅନ୍ତର୍ଭାଵ "term-sig.h"
#ଅନ୍ତର୍ଭାଵ "unistd--.h"
#ଅନ୍ତର୍ଭାଵ "xmemcoll.h"
#ଅନ୍ତର୍ଭାଵ "xnanosleep.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol-error.h"

#ifndef RLIMIT_DATA
ସଂରଚନା rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ଯଦି HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#ଯଦି !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#ଯଦି !defined OPEN_MAX
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
ଗଣନା { SUBTHREAD_ଧାଡ଼ିଗୁଡ଼ିକ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_ଧାଡ଼ିଗୁଡ଼ିକ_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ଗଣନା { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
ଗଣନା
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

ଗଣନା
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

ଗଣନା
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_ମୂଳ = 1
  };

/* The representation of the decimal point in the current locale.  */
ସ୍ଥିର ଅକ୍ଷର decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ସ୍ଥିର bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ସ୍ଥିର bool hard_LC_COLLATE;
#ଯଦି HAVE_NL_LANGINFO
ସ୍ଥିର bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ଗଣନା blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
ସ୍ଥିର ଅକ୍ଷର eolchar = '\n';

/* Lines are held in memory as counted strings. */
ସଂରଚନା ଧାଡ଼ି
{
  ଅକ୍ଷର *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  ଅକ୍ଷର *keybeg;			/* Start of first key. */
  ଅକ୍ଷର *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ସଂରଚନା buffer
{
  ଅକ୍ଷର *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ବାମ;			/* Number of bytes left from previous reads. */
  size_t ଧାଡ଼ି_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ସଂରଚନା sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
ସଂରଚନା keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ଅଚଳ *ignore;		/* Boolean array of characters to ignore. */
  ଅକ୍ଷର ଅଚଳ *translate;	/* Translation applied to characters. */
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
  ସଂରଚନା keyfield *next;	/* Next keyfield to try. */
};

ସଂରଚନା month
{
  ଅକ୍ଷର ଅଚଳ *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;
};

/* Binary merge tree node. */
ସଂରଚନା merge_ଗଣ୍ଠି
{
  ସଂରଚନା ଧାଡ଼ି *lo;              /* Lines to merge from LO child node. */
  ସଂରଚନା ଧାଡ଼ି *hi;              /* Lines to merge from HI child node. */
  ସଂରଚନା ଧାଡ଼ି *end_lo;          /* End of available lines from LO. */
  ସଂରଚନା ଧାଡ଼ି *end_hi;          /* End of available lines from HI. */
  ସଂରଚନା ଧାଡ଼ି **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ସଂରଚନା merge_ଗଣ୍ଠି *parent;    /* Parent node. */
  ସଂରଚନା merge_ଗଣ୍ଠି *lo_child;  /* LO child node. */
  ସଂରଚନା merge_ଗଣ୍ଠି *hi_child;  /* HI child node. */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ସଂରଚନା merge_ଗଣ୍ଠି_queue
{
  ସଂରଚନା heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ସ୍ଥିର ସଂରଚନା ଧାଡ଼ି saved_ଧାଡ଼ି;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ସ୍ଥିର bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ସ୍ଥିର bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ସ୍ଥିର bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ସ୍ଥିର ଅକ୍ଷର fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ସ୍ଥିର ସଂରଚନା month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + ଆକାର (ସଂରଚନା ଧାଡ଼ି))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ସ୍ଥିର size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ସ୍ଥିର size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define ନିବେଶ_FILE_SIZE_GUESS (128 * 1024)

ସ୍ଥିର ସଂରଚନା
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  ଅକ୍ଷର ଅଚଳ **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
ସ୍ଥିର bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ସ୍ଥିର bool stable;

/* An int value outside char range.  */
ଗଣନା { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ଗଣନା { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ସ୍ଥିର bool unique;

/* Nonzero if any of the input files are the standard input. */
ସ୍ଥିର bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ସ୍ଥିର ସଂରଚନା keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *compress_program;

/* Annotate the output with extra info to aid the user.  */
ସ୍ଥିର bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ସ୍ଥିର bool default_SIGPIPE;

/* The list of temporary files. */
ସଂରଚନା tempnode
{
  ସଂରଚନା tempnode *ଅସ୍ଥିର next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  ଅକ୍ଷର state;
  ଅକ୍ଷର name[FLEXIBLE_ARRAY_MEMBER];
};
ସ୍ଥିର ସଂରଚନା tempnode *ଅସ୍ଥିର temphead;
ସ୍ଥିର ସଂରଚନା tempnode *ଅସ୍ଥିର *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ସ୍ଥିର ଶୂନ୍ୟ
cleanup (ଶୂନ୍ୟ)
{
  ପାଇଁ (ସଂରଚନା tempnode ଅଚଳ *ଗଣ୍ଠି = temphead; ଗଣ୍ଠି; ଗଣ୍ଠି = ଗଣ୍ଠି->next)
    unlink (ଗଣ୍ଠି->name);
  temphead = ଖାଲି;
}

/* Handle interrupts and hangups. */

ସ୍ଥିର ଶୂନ୍ୟ
sighandler (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  cleanup ();

  ସଙ୍କେତ (sig, SIG_DFL);
  ଉଠାଅ (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ସ୍ଥିର ଶୂନ୍ୟ
sort_die (ଅକ୍ଷର ଅଚଳ *message, ଅକ୍ଷର ଅଚଳ *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ଯଦି (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         ignore leading blanks when finding sort ଚାବିଗୁଡ଼ିକ in each ଧାଡ଼ି\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-ସ୍ଥିତି\n\
         fold lower ସ୍ଥିତି to upper ସ୍ଥିତି characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical ମୂଲ୍ୟ\n\
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
         compare according to string numerical ମୂଲ୍ୟ;\n\
         see full documentation ପାଇଁ supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical ଚାବିଗୁଡ଼ିକ.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; ପାଇଁ more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check ପାଇଁ sorted ନିବେଶ; କର not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but କର not report first bad ଧାଡ଼ି\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the ଧାଡ଼ି used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ପଠ ନିବେଶ from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, ପଠ names from standard ନିବେଶ\n\
"));
      oputs (_("\
  -k, --ଚାବି=KEYDEF\n\
         sort via a ଚାବି; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; କର not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         ଲିଖ result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE ପାଇଁ ମୁଖ୍ୟ memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR ପାଇଁ temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the ସଂଖ୍ୟା of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of ଧାଡ଼ିଗୁଡ଼ିକ with equal ଚାବିଗୁଡ଼ିକ;\n\
         with -c, check ପାଇଁ strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] ପାଇଁ start and stop position, where F is a\n\
field ସଂଖ୍ୟା and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the ଧାଡ଼ି's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options ପାଇଁ that ଚାବି.  If no ଚାବି is given, use\n\
the entire ଧାଡ଼ି as the ଚାବି.  Use --debug to diagnose incorrect ଚାବି usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (ପୂର୍ବନିର୍ଦ୍ଧାରିତ), and so on ପାଇଁ M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte ମୂଲ୍ୟଗୁଡ଼ିକ.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"ignore-leading-blanks", no_argument, ଖାଲି, 'b'},
  {"check", optional_argument, ଖାଲି, CHECK_OPTION},
  {"compress-program", required_argument, ଖାଲି, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, ଖାଲି, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, ଖାଲି, 'd'},
  {"ignore-case", no_argument, ଖାଲି, 'f'},
  {"files0-from", required_argument, ଖାଲି, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, ଖାଲି, 'g'},
  {"ignore-nonprinting", no_argument, ଖାଲି, 'i'},
  {"key", required_argument, ଖାଲି, 'k'},
  {"merge", no_argument, ଖାଲି, 'm'},
  {"month-sort", no_argument, ଖାଲି, 'M'},
  {"numeric-sort", no_argument, ଖାଲି, 'n'},
  {"human-numeric-sort", no_argument, ଖାଲି, 'h'},
  {"version-sort", no_argument, ଖାଲି, 'V'},
  {"random-sort", no_argument, ଖାଲି, 'R'},
  {"random-source", required_argument, ଖାଲି, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, ଖାଲି, SORT_OPTION},
  {"output", required_argument, ଖାଲି, 'o'},
  {"reverse", no_argument, ଖାଲି, 'r'},
  {"stable", no_argument, ଖାଲି, 's'},
  {"batch-size", required_argument, ଖାଲି, NMERGE_OPTION},
  {"buffer-size", required_argument, ଖାଲି, 'S'},
  {"field-separator", required_argument, ଖାଲି, 't'},
  {"temporary-directory", required_argument, ଖାଲି, 'T'},
  {"unique", no_argument, ଖାଲି, 'u'},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {"parallel", required_argument, ଖାଲି, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE ଖାଲି
#undef  _ct_
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ check_types[] =
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

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE ଖାଲି
#undef  _st_
};
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
ସ୍ଥିର sigset_t caught_signals;

/* Critical section status.  */
ସଂରଚନା cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ସ୍ଥିର ଶୂନ୍ୟ
cs_enter (ସଂରଚନା cs_status *status)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
ସ୍ଥିର ଶୂନ୍ୟ
cs_leave (ସଂରଚନା cs_status ଅଚଳ *status)
{
  ଯଦି (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, ଖାଲି);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ଗଣନା { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ସଂରଚନା sortfile
{
  /* The file's name.  */
  ଅକ୍ଷର ଅଚଳ *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ସଂରଚନା tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ସ୍ଥିର Hash_table *proctab;

ଗଣନା { INIT_PROCTAB_SIZE = 47 };

ସ୍ଥିର size_t
proctab_hasher (ଶୂନ୍ୟ ଅଚଳ *entry, size_t tabsize)
{
  ସଂରଚନା tempnode ଅଚଳ *ଗଣ୍ଠି = entry;
  ଫେରନ୍ତୁ ଗଣ୍ଠି->pid % tabsize;
}

ସ୍ଥିର bool
proctab_comparator (ଶୂନ୍ୟ ଅଚଳ *e1, ଶୂନ୍ୟ ଅଚଳ *e2)
{
  ସଂରଚନା tempnode ଅଚଳ *n1 = e1;
  ସଂରଚନା tempnode ଅଚଳ *n2 = e2;
  ଫେରନ୍ତୁ n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ସ୍ଥିର pid_t nprocs;

ସ୍ଥିର bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ସ୍ଥିର pid_t
reap (pid_t pid)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  ଯଦି (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  ଅନ୍ୟଥା ଯଦି (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ଯଦି (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  ଫେରନ୍ତୁ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ସ୍ଥିର ଶୂନ୍ୟ
register_proc (ସଂରଚନା tempnode *temp)
{
  ଯଦି (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, ଖାଲି,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ଖାଲି);
      ଯଦି (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  ଯଦି (! hash_ଭର୍ତ୍ତି (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ସ୍ଥିର bool
delete_proc (pid_t pid)
{
  ସଂରଚନା tempnode test;

  test.pid = pid;
  ସଂରଚନା tempnode *ଗଣ୍ଠି = hash_remove (proctab, &test);
  ଯଦି (! ଗଣ୍ଠି)
    ଫେରନ୍ତୁ false;
  ଗଣ୍ଠି->state = REAPED;
  ଫେରନ୍ତୁ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ସ୍ଥିର ଶୂନ୍ୟ
wait_proc (pid_t pid)
{
  ଯଦି (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ସ୍ଥିର ଶୂନ୍ୟ
reap_exited (ଶୂନ୍ୟ)
{
  ଯେତେବେଳେ (0 < nprocs && reap (0))
    ଜାରି;
}

/* Reap at least one exited child, waiting if necessary.  */

ସ୍ଥିର ଶୂନ୍ୟ
reap_some (ଶୂନ୍ୟ)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ସ୍ଥିର ଶୂନ୍ୟ
reap_all (ଶୂନ୍ୟ)
{
  ଯେତେବେଳେ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ସ୍ଥିର ଶୂନ୍ୟ
exit_cleanup (ଶୂନ୍ୟ)
{
  ଯଦି (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ସଂରଚନା cs_status cs;
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

ସ୍ଥିର ସଂରଚନା tempnode *
create_temp_file (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pfd, bool survive_fd_exhaustion)
{
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ slashbase[] = "/sortXXXXXX";
  ସ୍ଥିର idx_t temp_dir_index;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno;
  ଅକ୍ଷର ଅଚଳ *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  ସଂରଚନା tempnode *ଗଣ୍ଠି =
    xmalloc (FLEXSIZEOF (ସଂରଚନା tempnode, name, len + ଆକାର slashbase));
  ଅକ୍ଷର *file = ଗଣ୍ଠି->name;
  ସଂରଚନା cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, ଆକାର slashbase);
  ଗଣ୍ଠି->next = ଖାଲି;
  ଯଦି (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ଯଦି (0 <= fd)
    {
      *temptail = ଗଣ୍ଠି;
      temptail = &ଗଣ୍ଠି->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ଯଦି (fd < 0)
    {
      ଯଦି (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      ମୁକ୍ତ (ଗଣ୍ଠି);
      ଗଣ୍ଠି = ଖାଲି;
    }

  *pfd = fd;
  ଫେରନ୍ତୁ ଗଣ୍ଠି;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ସ୍ଥିର ସଂରଚନା stat *
get_outstatus (ଶୂନ୍ୟ)
{
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା outstat_errno;
  ସ୍ଥିର ସଂରଚନା stat outstat;
  ଯଦି (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  ଫେରନ୍ତୁ outstat_errno < 0 ? &outstat : ଖାଲି;
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

ସ୍ଥିର FILE *
stream_open (ଅକ୍ଷର ଅଚଳ *file, ଅକ୍ଷର ଅଚଳ *how)
{
  FILE *fp;

  ଯଦି (*how == 'r')
    {
      ଯଦି (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ଖାଲି : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ଅନ୍ୟଥା ଯଦି (*how == 'w')
    {
      ଯଦି (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ftruncate_errno = errno;
          ସଂରଚନା stat *outst = get_outstatus ();
          ଯଦି (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ଅନ୍ୟଥା
    affirm (!"unexpected mode passed to stream_open");

  ଫେରନ୍ତୁ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ସ୍ଥିର FILE *
xfopen (ଅକ୍ଷର ଅଚଳ *file, ଅକ୍ଷର ଅଚଳ *how)
{
  FILE *fp = stream_open (file, how);
  ଯଦି (!fp)
    sort_die (_("open failed"), file);
  ଫେରନ୍ତୁ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ସ୍ଥିର ଶୂନ୍ୟ
xfclose (FILE *fp, ଅକ୍ଷର ଅଚଳ *file)
{
  ବାଛ (fileno (fp))
    {
    ସ୍ଥିତି STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ଯଦି (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଯଦି (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      ଭାଙ୍ଗ;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ସ୍ଥିର ଶୂନ୍ୟ
move_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd)
{
  ଯଦି (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_ମୂଲ୍ୟ (dup2 (oldfd, newfd));
      ignore_ମୂଲ୍ୟ (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା oldfd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା newfd)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = 0;
  ଯଦି (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ଯଦି (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  ଫେରନ୍ତୁ result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
get_resolved_compress_program (ଶୂନ୍ୟ)
{
  /* Use a cache, to perform the search only once.  */
  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *resolved_compress_program_cache /* = NULL */;

  ଯଦି (resolved_compress_program_cache == ଖାଲି)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), ଖାଲି, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  ଫେରନ୍ତୁ resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
pipe_child (pid_t *pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefds[2], ପୂର୍ଣ୍ଣସଂଖ୍ୟା tempfd, bool decompress,
            size_t tries)
{
  ଅକ୍ଷର ଅଚଳ *resolved_compress_program;
  ସଂରଚନା tempnode *saved_temphead;
  ଦ୍ୱିଗୁଣ wait_retry = 0.25;
  ସଂରଚନା cs_status cs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  ଯଦି (resolved_compress_program == ଖାଲି)
    ଫେରନ୍ତୁ errno;

  ଯଦି ((result = posix_spawnattr_init (&attr)))
    ଫେରନ୍ତୁ result;
  ଯଦି ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      ଫେରନ୍ତୁ result;
    }

  ଯଦି (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ଫେରନ୍ତୁ saved_errno;
    }

  ଯଦି ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      ଫେରନ୍ତୁ result;
    }

  ଅକ୍ଷର ଅଚଳ *ଅଚଳ argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : ଖାଲି,
      ଖାଲି
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ଯଦି (nmerge + 1 < nprocs)
    reap_some ();

  ଯେତେବେଳେ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ଖାଲି;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (ଅକ୍ଷର * ଅଚଳ *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ଯଦି (result != EAGAIN)
        ଭାଙ୍ଗ;
      ଅନ୍ୟଥା
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

  ଯଦି (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ଅନ୍ୟଥା
    ++nprocs;

  ଫେରନ୍ତୁ result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ସ୍ଥିର ସଂରଚନା tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tempfd;
  ସଂରଚନା tempnode *ଗଣ୍ଠି = create_temp_file (&tempfd, survive_fd_exhaustion);
  ଯଦି (! ଗଣ୍ଠି)
    ଫେରନ୍ତୁ ଖାଲି;

  ଗଣ୍ଠି->state = UNCOMPRESSED;

  ଯଦି (compress_program)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefds[2];
      ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_result = 0;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = pipe_child (&ଗଣ୍ଠି->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ଯଦି (result)
        {
          ଯଦି (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      ଅନ୍ୟଥା
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (ଗଣ୍ଠି);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  ଯଦି (! *pfp)
    sort_die (_("couldn't create temporary file"), ଗଣ୍ଠି->name);

  ଫେରନ୍ତୁ ଗଣ୍ଠି;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ସ୍ଥିର ସଂରଚନା tempnode *
create_temp (FILE **pfp)
{
  ଫେରନ୍ତୁ maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ସ୍ଥିର FILE *
open_temp (ସଂରଚନା tempnode *temp)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା tempfd, pipefds[2];
  FILE *fp = ଖାଲି;

  ଯଦି (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  ଯଦି (tempfd < 0)
    ଫେରନ୍ତୁ ଖାଲି;

  pid_t child;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ଯଦି (result)
    {
      ଯଦି (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  ଅନ୍ୟଥା
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ଯଦି (! fp)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  ଫେରନ୍ତୁ fp;
}

/* Append DIR to the array of temporary directory names.  */
ସ୍ଥିର ଶୂନ୍ୟ
add_temp_dir (ଅକ୍ଷର ଅଚଳ *dir)
{
  ଯଦି (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              ଆକାର *temp_dirs.dirs);

  idx_t ଅଚଳ dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ସ୍ଥିର ଶୂନ୍ୟ
zaptemp (ଅକ୍ଷର ଅଚଳ *name)
{
  ସଂରଚନା tempnode *ଅସ୍ଥିର *pnode;
  ସଂରଚନା tempnode *ଗଣ୍ଠି;
  ସଂରଚନା tempnode *next;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unlink_status;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା unlink_errno = 0;
  ସଂରଚନା cs_status cs;

  ପାଇଁ (pnode = &temphead; (ଗଣ୍ଠି = *pnode)->name != name; pnode = &ଗଣ୍ଠି->next)
    ଜାରି;

  ଯଦି (ଗଣ୍ଠି->state == UNREAPED)
    wait_proc (ଗଣ୍ଠି->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = ଗଣ୍ଠି->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  ଯଦି (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ଯଦି (! next)
    temptail = pnode;
  ମୁକ୍ତ (ଗଣ୍ଠି);
}

#ଯଦି HAVE_NL_LANGINFO

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
struct_month_cmp (ଶୂନ୍ୟ ଅଚଳ *m1, ଶୂନ୍ୟ ଅଚଳ *m2)
{
  ସଂରଚନା month ଅଚଳ *month1 = m1;
  ସଂରଚନା month ଅଚଳ *month2 = m2;
  ଫେରନ୍ତୁ strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

ସ୍ଥିର ଶୂନ୍ୟ
inittables (ଶୂନ୍ୟ)
{
  ପାଇଁ (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#ଯଦି HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ଯଦି (hard_LC_TIME)
    {
      ପାଇଁ (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          ଅକ୍ଷର ଅଚଳ *s;
          size_t s_len;
          size_t j, k;
          ଅକ୍ଷର *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          ପାଇଁ (j = k = 0; j < s_len; j++)
            ଯଦି (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, ଆକାର *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ସ୍ଥିର ଶୂନ୍ୟ
specify_nmerge (ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi, ଅକ୍ଷର c, ଅକ୍ଷର ଅଚଳ *s)
{
  uintmax_t n;
  ସଂରଚନା rlimit rlimit;
  ଗଣନା strtol_error e = xstrtoumax (s, ଖାଲି, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ଯଦି (e == LONGINT_OK)
    {
      nmerge = n;
      ଯଦି (nmerge != n)
        e = LONGINT_OVERFLOW;
      ଅନ୍ୟଥା
        {
          ଯଦି (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ଅନ୍ୟଥା ଯଦି (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          ଅନ୍ୟଥା
            ଫେରନ୍ତୁ;
        }
    }

  ଯଦି (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ଅନ୍ୟଥା
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ସ୍ଥିର ଶୂନ୍ୟ
specify_sort_size (ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi, ଅକ୍ଷର c, ଅକ୍ଷର ଅଚଳ *s)
{
  uintmax_t n;
  ଅକ୍ଷର *suffix;
  ଗଣନା strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ଯଦି (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ଯଦି (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    ବାଛ (suffix[0])
      {
      ସ୍ଥିତି 'b':
        e = LONGINT_OK;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି '%':
        {
          ଦ୍ୱିଗୁଣ mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ଯଦି (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ଅନ୍ୟଥା
            e = LONGINT_OVERFLOW;
        }
        ଭାଙ୍ଗ;
      }

  ଯଦି (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ଯଦି (n < sort_size)
        ଫେରନ୍ତୁ;

      sort_size = n;
      ଯଦି (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          ଫେରନ୍ତୁ;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ସ୍ଥିର size_t
specify_nthreads (ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi, ଅକ୍ଷର c, ଅକ୍ଷର ଅଚଳ *s)
{
  uintmax_t nthreads;
  ଗଣନା strtol_error e = xstrtoumax (s, ଖାଲି, 10, &nthreads, "");
  ଯଦି (e == LONGINT_OVERFLOW)
    ଫେରନ୍ତୁ SIZE_MAX;
  ଯଦି (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ଯଦି (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ଯଦି (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  ଫେରନ୍ତୁ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ସ୍ଥିର size_t
default_sort_size (ଶୂନ୍ୟ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  ସଂରଚନା rlimit rlimit;
  ଯଦି (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  ଯଦି (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ଯଦି (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ଦ୍ୱିଗୁଣ avail = physmem_available ();
  ଦ୍ୱିଗୁଣ total = physmem_total ();
  ଦ୍ୱିଗୁଣ mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ଯଦି (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ଯଦି (mem < size)
    size = mem;
  ଫେରନ୍ତୁ MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ସ୍ଥିର bool
ଧାଡ଼ି_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = ଆକାର (ସଂରଚନା ଧାଡ଼ି);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  ଯଦି (ckd_add (&aligned, size, padding))
    ଫେରନ୍ତୁ false;

  *alloc = aligned;
  ଫେରନ୍ତୁ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ସ୍ଥିର size_t
default_initial_sort_size (size_t ଧାଡ଼ି_bytes)
{
  size_t size = ଧାଡ଼ି_bytes + 2;
  size_t ନିବେଶ_size;

  ଯଦି (ckd_mul (&ନିବେଶ_size, ନିବେଶ_FILE_SIZE_GUESS, ଧାଡ଼ି_bytes + 1)
      || ckd_add (&size, size, ନିବେଶ_size))
    ଫେରନ୍ତୁ SIZE_MAX;

  ଫେରନ୍ତୁ MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ସ୍ଥିର bool
ନିବେଶ_size_buffer_bytes (uintmax_t file_size, size_t ଧାଡ଼ି_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_ନିବେଶ_byte = ଧାଡ଼ି_bytes + 1;

  ଯଦି (SIZE_MAX / worst_case_per_ନିବେଶ_byte < file_size)
    ଫେରନ୍ତୁ false;

  size_t size = file_size * worst_case_per_ନିବେଶ_byte;
  ଯଦି (SIZE_MAX - size < 1)
    ଫେରନ୍ତୁ false;

  *worst_case = size + 1;
  ଫେରନ୍ତୁ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ସ୍ଥିର ଶୂନ୍ୟ
sort_buffer_policy (FILE *ଅଚଳ *fps, size_t nfps,
                    ଅକ୍ଷର *ଅଚଳ *files, size_t nfiles,
                    size_t ଧାଡ଼ି_bytes,
                    ସଂରଚନା sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (ଧାଡ଼ି_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_ନିବେଶ_byte = ଧାଡ଼ି_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_ନିବେଶ_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  ପାଇଁ (size_t i = 0; i < nfiles; i++)
    {
      ସଂରଚନା stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      ଯଦି (size_bound <= size)
        {
          policy->initial = size_bound;
          ଫେରନ୍ତୁ;
        }

      ଯଦି ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      ଯଦି (known_size)
        file_size = st.st_size;
      ଅନ୍ୟଥା
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ଯଦି (sort_size)
            {
              policy->initial = size_bound;
              ଫେରନ୍ତୁ;
            }
          file_size = ନିବେଶ_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ଯଦି (! ନିବେଶ_size_buffer_bytes (file_size, ଧାଡ଼ି_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          ଫେରନ୍ତୁ;
        }

      ଯଦି (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          ଫେରନ୍ତୁ;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ସ୍ଥିର ଶୂନ୍ୟ
initbuf (ସଂରଚନା buffer *buf, size_t ଧାଡ଼ି_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ଯେତେବେଳେ (true)
    {
      ଯଦି (! ଧାଡ଼ି_aligned_size (&alloc))
        buf->buf = ଖାଲି;
      ଅନ୍ୟଥା
        buf->buf = malloc (alloc);
      ଯଦି (buf->buf)
        ଭାଙ୍ଗ;
      alloc /= 2;
      ଯଦି (alloc <= ଧାଡ଼ି_bytes + 1)
        xalloc_die ();
    }

  buf->ଧାଡ଼ି_bytes = ଧାଡ଼ି_bytes;
  buf->alloc = alloc;
  buf->used = buf->ବାମ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା ଧାଡ଼ି *
buffer_linelim (ସଂରଚନା buffer ଅଚଳ *buf)
{
  ଶୂନ୍ୟ *linelim = buf->buf + buf->alloc;
  ଫେରନ୍ତୁ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ସ୍ଥିର bool
try_growbuf (ସଂରଚନା buffer *buf, size_t alloc)
{
  ଯଦି (! ଧାଡ଼ି_aligned_size (&alloc) || alloc <= buf->alloc)
    ଫେରନ୍ତୁ false;

  ଅକ୍ଷର *newbuf = malloc (alloc);
  ଯଦି (! newbuf)
    ଫେରନ୍ତୁ false;

  ଅକ୍ଷର *oldbuf = buf->buf;
  ସଂରଚନା ଧାଡ଼ି *old_linelim = buffer_linelim (buf);
  ସଂରଚନା ଧାଡ଼ି *old_ଧାଡ଼ି = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  ସଂରଚନା ଧାଡ଼ି *new_linelim = (ଶୂନ୍ୟ *) (newbuf + alloc);
  ସଂରଚନା ଧାଡ଼ି *new_ଧାଡ଼ି = new_linelim - buf->nlines;
  memcpy (new_ଧାଡ଼ି, old_ଧାଡ଼ି, buf->nlines * ଆକାର *new_ଧାଡ଼ି);

  ପାଇଁ (ସଂରଚନା ଧାଡ଼ି *ଧାଡ଼ି = new_ଧାଡ଼ି; ଧାଡ଼ି < new_linelim; ଧାଡ଼ି++)
    {
      ଧାଡ଼ି->text = newbuf + (ଧାଡ଼ି->text - oldbuf);
      ଯଦି (keylist)
        {
          ଧାଡ଼ି->keybeg = newbuf + (ଧାଡ଼ି->keybeg - oldbuf);
          ଧାଡ଼ି->keylim = newbuf + (ଧାଡ଼ି->keylim - oldbuf);
        }
    }

  ମୁକ୍ତ (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  ଫେରନ୍ତୁ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ସ୍ଥିର bool
maybe_growbuf (ସଂରଚନା buffer *buf, ସଂରଚନା sort_buffer_policy *policy)
{
  ଯଦି (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    ଫେରନ୍ତୁ false;

  size_t alloc;
  ଯଦି (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  ଅନ୍ୟଥା
    alloc = policy->limit;

  ଯଦି (try_growbuf (buf, alloc))
    ଫେରନ୍ତୁ true;

  policy->growth_failed = true;
  ଫେରନ୍ତୁ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ସ୍ଥିର ଅକ୍ଷର *
begfield (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି, ସଂରଚନା keyfield ଅଚଳ *ଚାବି)
{
  ଅକ୍ଷର *ptr = ଧାଡ଼ି->text, *lim = ptr + ଧାଡ଼ି->length - 1;
  size_t sword = ଚାବି->sword;
  size_t schar = ଚାବି->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ଯଦି (tab != TAB_DEFAULT)
    ଯେତେବେଳେ (ptr < lim && sword--)
      {
        ଅକ୍ଷର *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ଯଦି (ptr < lim)
          ++ptr;
      }
  ଅନ୍ୟଥା
    ଯେତେବେଳେ (ptr < lim && sword--)
      {
        ଯେତେବେଳେ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ଯେତେବେଳେ (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ଯଦି (ଚାବି->skipsblanks)
    ଯେତେବେଳେ (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  ଯଦି (schar < remaining_bytes)
    ptr += schar;
  ଅନ୍ୟଥା
    ptr = lim;

  ଫେରନ୍ତୁ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
ସ୍ଥିର ଅକ୍ଷର *
limfield (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି, ସଂରଚନା keyfield ଅଚଳ *ଚାବି)
{
  ଅକ୍ଷର *ptr = ଧାଡ଼ି->text, *lim = ptr + ଧାଡ଼ି->length - 1;
  size_t eword = ଚାବି->eword, echar = ଚାବି->echar;

  ଯଦି (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ଯଦି (tab != TAB_DEFAULT)
    ଯେତେବେଳେ (ptr < lim && eword--)
      {
        ଅକ୍ଷର *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ଯଦି (ptr < lim && (eword || echar))
          ++ptr;
      }
  ଅନ୍ୟଥା
    ଯେତେବେଳେ (ptr < lim && eword--)
      {
        ଯେତେବେଳେ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        ଯେତେବେଳେ (ptr < lim && !blanks[to_uchar (*ptr)])
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
  ଯଦି (tab != TAB_DEFAULT)
    {
      ଅକ୍ଷର *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ଯଦି (newlim)
        lim = newlim;
    }
  ଅନ୍ୟଥା
    {
      ଅକ୍ଷର *newlim;
      newlim = ptr;
      ଯେତେବେଳେ (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      ଯେତେବେଳେ (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  ଯଦି (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ଯଦି (ଚାବି->skipeblanks)
        ଯେତେବେଳେ (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      ଯଦି (echar < remaining_bytes)
        ptr += echar;
      ଅନ୍ୟଥା
        ptr = lim;
    }

  ଫେରନ୍ତୁ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ସ୍ଥିର bool
fillbuf (ସଂରଚନା buffer *buf, FILE *fp, ଅକ୍ଷର ଅଚଳ *file,
         ସଂରଚନା sort_buffer_policy *policy)
{
  ସଂରଚନା keyfield ଅଚଳ *ଚାବି = keylist;
  ଅକ୍ଷର eol = eolchar;
  size_t ଧାଡ଼ି_bytes = buf->ଧାଡ଼ି_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  ଯଦି (buf->eof)
    ଫେରନ୍ତୁ false;

  ଯଦି (buf->used != buf->ବାମ)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->ବାମ, buf->ବାମ);
      buf->used = buf->ବାମ;
      buf->nlines = 0;
    }

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *ptr = buf->buf + buf->used;
      ସଂରଚନା ଧାଡ଼ି *linelim = buffer_linelim (buf);
      ସଂରଚନା ଧାଡ଼ି *ଧାଡ଼ି = linelim - buf->nlines;
      size_t avail = (ଅକ୍ଷର *) linelim - buf->nlines * ଧାଡ଼ି_bytes - ptr;
      ଅକ୍ଷର *ଧାଡ଼ି_start = buf->nlines ? ଧାଡ଼ି->text + ଧାଡ଼ି->length : buf->buf;

      ଯେତେବେଳେ (ଧାଡ଼ି_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (ଧାଡ଼ି_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          ଅକ୍ଷର *ptrlim = ptr + bytes_read;
          ଅକ୍ଷର *p;
          avail -= bytes_read;

          ଯଦି (bytes_read != readsize)
            {
              ଯଦି (ferror (fp))
                sort_die (_("read failed"), file);
              ଯଦି (feof (fp))
                {
                  buf->eof = true;
                  ଯଦି (buf->buf == ptrlim)
                    ଫେରନ୍ତୁ false;
                  ଯଦି (ଧାଡ଼ି_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ଯେତେବେଳେ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              ଧାଡ଼ି--;
              ଧାଡ଼ି->text = ଧାଡ଼ି_start;
              ଧାଡ଼ି->length = ptr - ଧାଡ଼ି_start;
              mergesize = MAX (mergesize, ଧାଡ଼ି->length);
              avail -= ଧାଡ଼ି_bytes;

              ଯଦି (ଚାବି)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  ଧାଡ଼ି->keylim = (ଚାବି->eword == SIZE_MAX
                                  ? p
                                  : limfield (ଧାଡ଼ି, ଚାବି));

                  ଯଦି (ଚାବି->sword != SIZE_MAX)
                    ଧାଡ଼ି->keybeg = begfield (ଧାଡ଼ି, ଚାବି);
                  ଅନ୍ୟଥା
                    {
                      ଯଦି (ଚାବି->skipsblanks)
                        ଯେତେବେଳେ (blanks[to_uchar (*ଧାଡ଼ି_start)])
                          ଧାଡ଼ି_start++;
                      ଧାଡ଼ି->keybeg = ଧାଡ଼ି_start;
                    }
                }

              ଧାଡ଼ି_start = ptr;
            }

          ptr = ptrlim;
          ଯଦି (buf->eof)
            ଭାଙ୍ଗ;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - ଧାଡ଼ି;
      ଯଦି (buf->nlines != 0)
        {
          ଯଦି (! buf->eof && maybe_growbuf (buf, policy))
            ଜାରି;
          buf->ବାମ = ptr - ଧାଡ଼ି_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          ଫେରନ୍ତୁ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t ଧାଡ଼ି_alloc = buf->alloc / ଆକାର (ସଂରଚନା ଧାଡ଼ି);
        buf->buf = xpalloc (buf->buf, &ଧାଡ଼ି_alloc, 1, -1, ଆକାର (ସଂରଚନା ଧାଡ଼ି));
        buf->alloc = ଧାଡ଼ି_alloc * ଆକାର (ସଂରଚନା ଧାଡ଼ି);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ unit_order[UCHAR_LIM] =
  {
#ଯଦି ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ଅନ୍ୟଥା
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
ସ୍ଥିର ଅକ୍ଷର
traverse_raw_ସଂଖ୍ୟା (ଅକ୍ଷର ଅଚଳ **ସଂଖ୍ୟା)
{
  ଅକ୍ଷର ଅଚଳ *p = *ସଂଖ୍ୟା;
  ଅକ୍ଷର ch;
  ଅକ୍ଷର max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ଯେତେବେଳେ (c_isdigit (ch = *p++))
    {
      ଯଦି (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      ଯଦି (ends_with_thousands_sep)
        ++p;
    }

  ଯଦି (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *ସଂଖ୍ୟା = p - 2;
      ଫେରନ୍ତୁ max_digit;
    }

  ଯଦି (ch == decimal_point)
    ଯେତେବେଳେ (c_isdigit (ch = *p++))
      ଯଦି (max_digit < ch)
        max_digit = ch;

  *ସଂଖ୍ୟା = p - 1;
  ଫେରନ୍ତୁ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
find_unit_order (ଅକ୍ଷର ଅଚଳ *ସଂଖ୍ୟା)
{
  bool minus_sign = (*ସଂଖ୍ୟା == '-');
  ଅକ୍ଷର ଅଚଳ *p = ସଂଖ୍ୟା + minus_sign;
  ଅକ୍ଷର max_digit = traverse_raw_ସଂଖ୍ୟା (&p);
  ଯଦି ('0' < max_digit)
    {
      unsigned ଅକ୍ଷର ch = *p;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା order = unit_order[ch];
      ଫେରନ୍ତୁ (minus_sign ? -order : order);
    }
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
human_numcompare (ଅକ୍ଷର ଅଚଳ *a, ଅକ୍ଷର ଅଚଳ *b)
{
  ଯେତେବେଳେ (blanks[to_uchar (*a)])
    a++;
  ଯେତେବେଳେ (blanks[to_uchar (*b)])
    b++;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା diff = find_unit_order (a) - find_unit_order (b);
  ଫେରନ୍ତୁ (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
numcompare (ଅକ୍ଷର ଅଚଳ *a, ଅକ୍ଷର ଅଚଳ *b)
{
  ଯେତେବେଳେ (blanks[to_uchar (*a)])
    a++;
  ଯେତେବେଳେ (blanks[to_uchar (*b)])
    b++;

  ଫେରନ୍ତୁ strnumcmp (a, b, decimal_point, thousands_sep);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
nan_compare (long ଦ୍ୱିଗୁଣ a, long ଦ୍ୱିଗୁଣ b)
{
  ଅକ୍ଷର buf[2][ଆକାର "-nan""()" + CHAR_BIT * ଆକାର a];
  snprintf (buf[0], ଆକାର buf[0], "%Lf", a);
  snprintf (buf[1], ଆକାର buf[1], "%Lf", b);
  ଫେରନ୍ତୁ strcmp (buf[0], buf[1]);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
general_numcompare (ଅକ୍ଷର ଅଚଳ *sa, ଅକ୍ଷର ଅଚଳ *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  ଅକ୍ଷର *ea;
  ଅକ୍ଷର *eb;
  long ଦ୍ୱିଗୁଣ a = strtold (sa, &ea);
  long ଦ୍ୱିଗୁଣ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ଯଦି (sa == ea)
    ଫେରନ୍ତୁ sb == eb ? 0 : -1;
  ଯଦି (sb == eb)
    ଫେରନ୍ତୁ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  ଫେରନ୍ତୁ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
getmonth (ଅକ୍ଷର ଅଚଳ *month, ଅକ୍ଷର **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  ଯେତେବେଳେ (blanks[to_uchar (*month)])
    month++;

  କର
    {
      size_t ix = (lo + hi) / 2;
      ଅକ୍ଷର ଅଚଳ *m = month;
      ଅକ୍ଷର ଅଚଳ *n = monthtab[ix].name;

      ପାଇଁ (;; m++, n++)
        {
          ଯଦି (!*n)
            {
              ଯଦି (ea)
                *ea = (ଅକ୍ଷର *) m;
              ଫେରନ୍ତୁ monthtab[ix].val;
            }
          ଯଦି (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              ଭାଙ୍ଗ;
            }
          ଅନ୍ୟଥା ଯଦି (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              ଭାଙ୍ଗ;
            }
        }
    }
  ଯେତେବେଳେ (lo < hi);

  ଫେରନ୍ତୁ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ଯଦି DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ଯଦି 14 <= __GNUC__
#  pragma GCC diagnostic ଠେଲ
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#ଅନ୍ତର୍ଭାଵ "md5.h"

#ଯଦି DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ଯଦି 14 <= __GNUC__
#  pragma GCC diagnostic କାଢ଼
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
ସ୍ଥିର ଶୂନ୍ୟ
link_failure (ଶୂନ୍ୟ)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ସ୍ଥିର ଶୂନ୍ୟ *
ପ୍ରତୀକ_address (ଶୂନ୍ୟ *handle, ଅକ୍ଷର ଅଚଳ *ପ୍ରତୀକ)
{
  ଶୂନ୍ୟ *address = dlsym (handle, ପ୍ରତୀକ);
  ଯଦି (!address)
    link_failure ();
  ଫେରନ୍ତୁ address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
ସ୍ଥିର ଶୂନ୍ୟ
link_libcrypto (ଶୂନ୍ୟ)
{
#ଯଦି DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  ଶୂନ୍ୟ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ଯଦି (!handle)
    link_failure ();
  ptr_MD5_Init = ପ୍ରତୀକ_address (handle, "MD5_Init");
  ptr_MD5_Update = ପ୍ରତୀକ_address (handle, "MD5_Update");
  ptr_MD5_Final = ପ୍ରତୀକ_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
ସ୍ଥିର ସଂରଚନା md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ସ୍ଥିର ଶୂନ୍ୟ
random_md5_state_init (ଅକ୍ଷର ଅଚଳ *random_source)
{
  unsigned ଅକ୍ଷର buf[MD5_DIGEST_SIZE];
  ସଂରଚନା randread_source *r = randread_new (random_source, ଆକାର buf);
  ଯଦି (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, ଆକାର buf);
  ଯଦି (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, ଆକାର buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ସ୍ଥିର size_t
xstrxfrm (ଅକ୍ଷର *restrict dest, ଅକ୍ଷର ଅଚଳ *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  ଯଦି (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  ଫେରନ୍ତୁ translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_random (ଅକ୍ଷର *restrict texta, size_t lena,
                ଅକ୍ଷର *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା xfrm_diff = 0;

  ଅକ୍ଷର stackbuf[4000];
  ଅକ୍ଷର *buf = stackbuf;
  size_t bufsize = ଆକାର stackbuf;
  ଶୂନ୍ୟ *allocated = ଖାଲି;
  uint32_t dig[2][MD5_DIGEST_SIZE / ଆକାର (uint32_t)];
  ସଂରଚନା md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ଯଦି (hard_LC_COLLATE)
    {
      ଅକ୍ଷର ଅଚଳ *lima = texta + lena;
      ଅକ୍ଷର ଅଚଳ *limb = textb + lenb;

      ଯେତେବେଳେ (true)
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
          ଯଦି (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              ମୁକ୍ତ (allocated);
              buf = allocated = malloc (bufsize);
              ଯଦି (! buf)
                {
                  buf = stackbuf;
                  bufsize = ଆକାର stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : ଖାଲି), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          ଯଦି (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ଯଦି (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              ମୁକ୍ତ (allocated);
              buf = allocated = xmalloc (bufsize);
              ଯଦି (texta < lima)
                strxfrm (buf, texta, sizea);
              ଯଦି (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ଯଦି (texta < lima)
            texta += strlen (texta) + 1;
          ଯଦି (textb < limb)
            textb += strlen (textb) + 1;
          ଯଦି (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ଭାଙ୍ଗ;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ଯଦି (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ଯଦି (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା diff = memcmp (dig[0], dig[1], ଆକାର dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ଯଦି (! diff)
    {
      ଯଦି (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ଯଦି (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  ମୁକ୍ତ (allocated);

  ଫେରନ୍ତୁ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ସ୍ଥିର size_t
debug_width (ଅକ୍ଷର ଅଚଳ *text, ଅକ୍ଷର ଅଚଳ *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  ଯେତେବେଳେ (text < lim)
    width += (*text++ == '\t');
  ଫେରନ୍ତୁ width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ସ୍ଥିର ଶୂନ୍ୟ
mark_ଚାବି (size_t offset, size_t width)
{
  ଯେତେବେଳେ (offset--)
    putchar (' ');

  ଯଦି (!width)
    printf (_("^ no match for key\n"));
  ଅନ୍ୟଥା
    {
      କର
        putchar ('_');
      ଯେତେବେଳେ (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
ଚାବି_numeric (ସଂରଚନା keyfield ଅଚଳ *ଚାବି)
{
  ଫେରନ୍ତୁ ଚାବି->numeric || ଚାବି->general_numeric || ଚାବି->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ସ୍ଥିର ଶୂନ୍ୟ
debug_ଚାବି (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି, ସଂରଚନା keyfield ଅଚଳ *ଚାବି)
{
  ଅକ୍ଷର *text = ଧାଡ଼ି->text;
  ଅକ୍ଷର *beg = text;
  ଅକ୍ଷର *lim = text + ଧାଡ଼ି->length - 1;

  ଯଦି (ଚାବି)
    {
      ଯଦି (ଚାବି->sword != SIZE_MAX)
        beg = begfield (ଧାଡ଼ି, ଚାବି);
      ଯଦି (ଚାବି->eword != SIZE_MAX)
        {
          lim = limfield (ଧାଡ଼ି, ଚାବି);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ଯଦି ((ଚାବି->skipsblanks && ଚାବି->sword == SIZE_MAX)
          || ଚାବି->month || ଚାବି_numeric (ଚାବି))
        {
          ଅକ୍ଷର saved = *lim;
          *lim = '\0';

          ଯେତେବେଳେ (blanks[to_uchar (*beg)])
            beg++;

          ଅକ୍ଷର *tighter_lim = beg;

          ଯଦି (lim < beg)
            tighter_lim = lim;
          ଅନ୍ୟଥା ଯଦି (ଚାବି->month)
            getmonth (beg, &tighter_lim);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->general_numeric)
            ignore_ମୂଲ୍ୟ (strtold (beg, &tighter_lim));
          ଅନ୍ୟଥା ଯଦି (ଚାବି->numeric || ଚାବି->human_numeric)
            {
              ଅକ୍ଷର ଅଚଳ *p = beg + (beg < lim && *beg == '-');
              ଅକ୍ଷର max_digit = traverse_raw_ସଂଖ୍ୟା (&p);
              ଯଦି ('0' <= max_digit)
                {
                  unsigned ଅକ୍ଷର ch = *p;
                  tighter_lim = (ଅକ୍ଷର *) p
                    + (ଚାବି->human_numeric && unit_order[ch]);
                }
            }
          ଅନ୍ୟଥା
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_ଚାବି (offset, width);
}

/* Debug LINE by underlining its keys.  */

ସ୍ଥିର ଶୂନ୍ୟ
debug_ଧାଡ଼ି (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି)
{
  ସଂରଚନା keyfield ଅଚଳ *ଚାବି = keylist;

  କର
    debug_ଚାବି (ଧାଡ଼ି, ଚାବି);
  ଯେତେବେଳେ (ଚାବି && ((ଚାବି = ଚାବି->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ସ୍ଥିର bool
default_ଚାବି_compare (ସଂରଚନା keyfield ଅଚଳ *ଚାବି)
{
  ଫେରନ୍ତୁ ! (ଚାବି->ignore
            || ଚାବି->translate
            || ଚାବି->skipsblanks
            || ଚାବି->skipeblanks
            || ଚାବି_numeric (ଚାବି)
            || ଚାବି->month
            || ଚାବି->version
            || ଚାବି->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ସ୍ଥିର ଶୂନ୍ୟ
ଚାବି_to_opts (ସଂରଚନା keyfield ଅଚଳ *ଚାବି, ଅକ୍ଷର *opts)
{
  ଯଦି (ଚାବି->skipsblanks || ଚାବି->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ଯଦି (ଚାବି->ignore == nondictionary)
    *opts++ = 'd';
  ଯଦି (ଚାବି->translate)
    *opts++ = 'f';
  ଯଦି (ଚାବି->general_numeric)
    *opts++ = 'g';
  ଯଦି (ଚାବି->human_numeric)
    *opts++ = 'h';
  ଯଦି (ଚାବି->ignore == nonprinting)
    *opts++ = 'i';
  ଯଦି (ଚାବି->month)
    *opts++ = 'M';
  ଯଦି (ଚାବି->numeric)
    *opts++ = 'n';
  ଯଦି (ଚାବି->random)
    *opts++ = 'R';
  ଯଦି (ଚାବି->reverse)
    *opts++ = 'r';
  ଯଦି (ଚାବି->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ସ୍ଥିର ଶୂନ୍ୟ
ଚାବି_warnings (ସଂରଚନା keyfield ଅଚଳ *gkey, bool gkey_only)
{
  ସଂରଚନା keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  ପାଇଁ (ସଂରଚନା keyfield *ଚାବି = keylist; ଚାବି; ଚାବି = ଚାବି->next, keynum++)
    {
      ଯଦି (ଚାବି_numeric (ଚାବି))
        {
          ଯଦି (ଚାବି->general_numeric)
            general_numeric_field = true;
          ଅନ୍ୟଥା
            basic_numeric_field = true;
        }

      ଯଦି (ଚାବି->traditional_used)
        {
          size_t sword = ଚାବି->sword;
          size_t eword = ଚାବି->eword;
          ଅକ୍ଷର tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          ଅକ୍ଷର obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          ଅକ୍ଷର nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          ଅକ୍ଷର *po = obuf;
          ଅକ୍ଷର *pn = nbuf;

          ଯଦି (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ଯଦି (ଚାବି->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ଚାବି->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = ଚାବି->sword != SIZE_MAX && ଚାବି->eword < ଚାବି->sword;
      ଯଦି (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ଚାବି_numeric (ଚାବି) || ଚାବି->month;
      bool ଧାଡ଼ି_offset = ଚାବି->eword == 0 && ଚାବି->echar != 0; /* -k1.x,1.y  */
      ଯଦି (!zero_width && !gkey_only && tab == TAB_DEFAULT && !ଧାଡ଼ି_offset
          && ((!ଚାବି->skipsblanks && !implicit_skip)
              || (!ଚାବି->skipsblanks && ଚାବି->schar)
              || (!ଚାବି->skipeblanks && ଚାବି->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ଯଦି (!gkey_only && ଚାବି_numeric (ଚାବି))
        {
          size_t sword = ଚାବି->sword + 1;
          size_t eword = ଚାବି->eword + 1;
          ଯଦି (!sword)
            sword++;
          ଯଦି (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ଯଦି (ଚାବି->general_numeric)
                general_numeric_field_span = true;
              ଅନ୍ୟଥା
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ଯଦି (ugkey.ignore && (ugkey.ignore == ଚାବି->ignore))
        ugkey.ignore = ଖାଲି;
      ଯଦି (ugkey.translate && (ugkey.translate == ଚାବି->translate))
        ugkey.translate = ଖାଲି;
      ugkey.skipsblanks &= !ଚାବି->skipsblanks;
      ugkey.skipeblanks &= !ଚାବି->skipeblanks;
      ugkey.month &= !ଚାବି->month;
      ugkey.numeric &= !ଚାବି->numeric;
      ugkey.general_numeric &= !ଚାବି->general_numeric;
      ugkey.human_numeric &= !ଚାବି->human_numeric;
      ugkey.random &= !ଚାବି->random;
      ugkey.version &= !ଚାବି->version;
      ugkey.reverse &= !ଚାବି->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool ସଂଖ୍ୟା_locale_warned = false;
  ଯଦି (basic_numeric_field_span)
    {
      ଯଦି (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((ଅକ୍ଷର []) {thousands_sep, 0})));
          ସଂଖ୍ୟା_locale_warned = true;
        }
    }
  ଯଦି (basic_numeric_field_span || general_numeric_field_span)
    {
      ଯଦି (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((ଅକ୍ଷର []) {decimal_point, 0})));
          ସଂଖ୍ୟା_locale_warned = true;
        }
      ଅନ୍ୟଥା ଯଦି (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((ଅକ୍ଷର []) {tab, 0})));
        }
      ଅନ୍ୟଥା ଯଦି (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((ଅକ୍ଷର []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ଯଦି ((basic_numeric_field || general_numeric_field) && ! ସଂଖ୍ୟା_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((ଅକ୍ଷର []) {decimal_point, 0})));

    }

  ଯଦି (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ଯଦି (!default_ଚାବି_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      ଯଦି (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      ଅକ୍ଷର opts[ଆକାର short_options];
      ଚାବି_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  ଯଦି (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
diff_reversed (ପୂର୍ଣ୍ଣସଂଖ୍ୟା diff, bool reversed)
{
  ଫେରନ୍ତୁ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
keycompare (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *a, ସଂରଚନା ଧାଡ଼ି ଅଚଳ *b)
{
  ସଂରଚନା keyfield *ଚାବି = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  ଅକ୍ଷର *texta = a->keybeg;
  ଅକ୍ଷର *textb = b->keybeg;
  ଅକ୍ଷର *lima = a->keylim;
  ଅକ୍ଷର *limb = b->keylim;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା diff;

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର ଅଚଳ *translate = ଚାବି->translate;
      bool ଅଚଳ *ignore = ଚାବି->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ଯଦି (hard_LC_COLLATE || ଚାବି_numeric (ଚାବି)
          || ଚାବି->month || ଚାବି->random || ଚାବି->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          ଅକ୍ଷର *ta = texta;
          ଅକ୍ଷର *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          ଅକ୍ଷର enda = ta[tlena];
          ଅକ୍ଷର endb = tb[tlenb];

          ଶୂନ୍ୟ *allocated = ଖାଲି;
          ଅକ୍ଷର stackbuf[4000];

          ଯଦି (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              ଯଦି (size <= ଆକାର stackbuf)
                ta = stackbuf;
              ଅନ୍ୟଥା
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              ପାଇଁ (tlena = i = 0; i < lena; i++)
                ଯଦି (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              ପାଇଁ (tlenb = i = 0; i < lenb; i++)
                ଯଦି (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ଯଦି (ଚାବି->numeric)
            diff = numcompare (ta, tb);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->general_numeric)
            diff = general_numcompare (ta, tb);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->human_numeric)
            diff = human_numcompare (ta, tb);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->month)
            diff = getmonth (ta, ଖାଲି) - getmonth (tb, ଖାଲି);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ଅନ୍ୟଥା ଯଦି (ଚାବି->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ଅନ୍ୟଥା
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ଯଦି (tlena == 0)
                diff = - NONZERO (tlenb);
              ଅନ୍ୟଥା ଯଦି (tlenb == 0)
                diff = 1;
              ଅନ୍ୟଥା
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          ମୁକ୍ତ (allocated);
        }
      ଅନ୍ୟଥା ଯଦି (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  କର									\
    {									\
          ଯେତେବେଳେ (true)							\
            {								\
              ଯେତେବେଳେ (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              ଯେତେବେଳେ (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              ଯଦି (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ଭାଙ୍ଗ;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              ଯଦି (diff)							\
                ଭାଙ୍ଗ;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ଯେତେବେଳେ (0)

          ଯଦି (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          ଅନ୍ୟଥା
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ଅନ୍ୟଥା
        {
          size_t lenmin = MIN (lena, lenb);
          ଯଦି (lenmin == 0)
            diff = 0;
          ଅନ୍ୟଥା ଯଦି (translate)
            {
              size_t i = 0;
              କର
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  ଯଦି (diff)
                    ଭାଙ୍ଗ;
                  i++;
                }
              ଯେତେବେଳେ (i < lenmin);
            }
          ଅନ୍ୟଥା
            diff = memcmp (texta, textb, lenmin);

          ଯଦି (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ଯଦି (diff)
        ଭାଙ୍ଗ;

      ଚାବି = ଚାବି->next;
      ଯଦି (! ଚାବି)
        ଫେରନ୍ତୁ 0;

      /* Find the beginning and limit of the next field.  */
      ଯଦି (ଚାବି->eword != SIZE_MAX)
        lima = limfield (a, ଚାବି), limb = limfield (b, ଚାବି);
      ଅନ୍ୟଥା
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      ଯଦି (ଚାବି->sword != SIZE_MAX)
        texta = begfield (a, ଚାବି), textb = begfield (b, ଚାବି);
      ଅନ୍ୟଥା
        {
          texta = a->text, textb = b->text;
          ଯଦି (ଚାବି->skipsblanks)
            {
              ଯେତେବେଳେ (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              ଯେତେବେଳେ (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  ଫେରନ୍ତୁ diff_reversed (diff, ଚାବି->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *a, ସଂରଚନା ଧାଡ଼ି ଅଚଳ *b)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ଯଦି (keylist)
    {
      diff = keycompare (a, b);
      ଯଦି (diff || unique || stable)
        ଫେରନ୍ତୁ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  ଯଦି (alen == 0)
    diff = - NONZERO (blen);
  ଅନ୍ୟଥା ଯଦି (blen == 0)
    diff = 1;
  ଅନ୍ୟଥା ଯଦି (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  ଅନ୍ୟଥା
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      ଯଦି (!diff)
        diff = _GL_CMP (alen, blen);
    }

  ଫେରନ୍ତୁ diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ସ୍ଥିର ଶୂନ୍ୟ
write_ଧାଡ଼ି (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି, FILE *fp, ଅକ୍ଷର ଅଚଳ *output_file)
{
  ଅକ୍ଷର *buf = ଧାଡ଼ି->text;
  size_t n_bytes = ଧାଡ଼ି->length;
  ଅକ୍ଷର *ebuf = buf + n_bytes;

  ଯଦି (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      ଅକ୍ଷର ଅଚଳ *c = buf;

      ଯେତେବେଳେ (c < ebuf)
        {
          ଅକ୍ଷର wc = *c++;
          ଯଦି (wc == '\t')
            wc = '>';
          ଅନ୍ୟଥା ଯଦି (c == ebuf)
            wc = '\n';
          ଯଦି (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_ଧାଡ଼ି (ଧାଡ଼ି);
    }
  ଅନ୍ୟଥା
    {
      ebuf[-1] = eolchar;
      ଯଦି (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ସ୍ଥିର bool
check (ଅକ୍ଷର ଅଚଳ *file_name, ଅକ୍ଷର checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ସଂରଚନା buffer buf;		/* Input buffer. */
  ସଂରଚନା ଧାଡ଼ି temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t ଧାଡ଼ି_ସଂଖ୍ୟା = 0;
  ସଂରଚନା keyfield ଅଚଳ *ଚାବି = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, ଆକାର (ସଂରଚନା ଧାଡ଼ି),
           MAX (merge_buffer_size, sort_size));
  temp.text = ଖାଲି;

  ଯେତେବେଳେ (fillbuf (&buf, fp, file_name, ଖାଲି))
    {
      ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି = buffer_linelim (&buf);
      ସଂରଚନା ଧାଡ଼ି ଅଚଳ *linebase = ଧାଡ଼ି - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ଯଦି (alloc && nonunique <= compare (&temp, ଧାଡ଼ି - 1))
        {
        found_disorder:
          {
            ଯଦି (checkonly == 'c')
              {
                ସଂରଚନା ଧାଡ଼ି ଅଚଳ *disorder_ଧାଡ଼ି = ଧାଡ଼ି - 1;
                uintmax_t disorder_ଧାଡ଼ି_ସଂଖ୍ୟା =
                  buffer_linelim (&buf) - disorder_ଧାଡ଼ି + ଧାଡ଼ି_ସଂଖ୍ୟା;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_ଧାଡ଼ି_ସଂଖ୍ୟା);
                write_ଧାଡ଼ି (disorder_ଧାଡ଼ି, stderr, _("standard error"));
              }

            ordered = false;
            ଭାଙ୍ଗ;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ଯେତେବେଳେ (linebase < --ଧାଡ଼ି)
        ଯଦି (nonunique <= compare (ଧାଡ଼ି, ଧାଡ଼ି - 1))
          ଯାଅ found_disorder;

      ଧାଡ଼ି_ସଂଖ୍ୟା += buf.nlines;

      /* Save the last line of the buffer.  */
      ଯଦି (alloc < ଧାଡ଼ି->length)
        {
          କର
            {
              alloc *= 2;
              ଯଦି (! alloc)
                {
                  alloc = ଧାଡ଼ି->length;
                  ଭାଙ୍ଗ;
                }
            }
          ଯେତେବେଳେ (alloc < ଧାଡ଼ି->length);

          ମୁକ୍ତ (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, ଧାଡ଼ି->text, ଧାଡ଼ି->length);
      temp.length = ଧାଡ଼ି->length;
      ଯଦି (ଚାବି)
        {
          temp.keybeg = temp.text + (ଧାଡ଼ି->keybeg - ଧାଡ଼ି->text);
          temp.keylim = temp.text + (ଧାଡ଼ି->keylim - ଧାଡ଼ି->text);
        }
    }

  xfclose (fp, file_name);
  ମୁକ୍ତ (buf.buf);
  ମୁକ୍ତ (temp.text);
  ଫେରନ୍ତୁ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ସ୍ଥିର size_t
open_ନିବେଶ_files (ସଂରଚନା sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, ଆକାର *fps);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  /* Open as many input files as we can.  */
  ପାଇଁ (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      ଯଦି (!fps[i])
        ଭାଙ୍ଗ;
    }

  ଫେରନ୍ତୁ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ସ୍ଥିର ଶୂନ୍ୟ
mergefps (ସଂରଚନା sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, ଅକ୍ଷର ଅଚଳ *output_file, FILE **fps)
{
  ସଂରଚନା buffer *buffer = xnmalloc (nfiles, ଆକାର *buffer);
                                /* Input buffers for each file. */
  ସଂରଚନା ଧାଡ଼ି saved;		/* Saved line storage for unique check. */
  ସଂରଚନା ଧାଡ଼ି ଅଚଳ *savedline = ଖାଲି;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ସଂରଚନା ଧାଡ଼ି ଅଚଳ **cur = xnmalloc (nfiles, ଆକାର *cur);
                                /* Current line in each line table. */
  ସଂରଚନା ଧାଡ଼ି ଅଚଳ **base = xnmalloc (nfiles, ଆକାର *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, ଆକାର *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ସଂରଚନା keyfield ଅଚଳ *ଚାବି = keylist;
  saved.text = ଖାଲି;

  /* Read initial lines from each input file. */
  ପାଇଁ (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], ଆକାର (ସଂରଚନା ଧାଡ଼ି),
               MAX (merge_buffer_size, sort_size / nfiles));
      ଯଦି (fillbuf (&buffer[i], fps[i], files[i].name, ଖାଲି))
        {
          ସଂରଚନା ଧାଡ଼ି ଅଚଳ *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      ଅନ୍ୟଥା
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ଯଦି (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          ମୁକ୍ତ (buffer[i].buf);
          --nfiles;
          ପାଇଁ (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  ପାଇଁ (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  ପାଇଁ (size_t i = 1; i < nfiles; ++i)
    ଯଦି (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ଯେତେବେଳେ (nfiles)
    {
      ସଂରଚନା ଧାଡ଼ି ଅଚଳ *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ଯଦି (unique)
        {
          ଯଦି (savedline && compare (savedline, smallest))
            {
              savedline = ଖାଲି;
              write_ଧାଡ଼ି (&saved, ofp, output_file);
            }
          ଯଦି (!savedline)
            {
              savedline = &saved;
              ଯଦି (savealloc < smallest->length)
                {
                  କର
                    ଯଦି (! savealloc)
                      {
                        savealloc = smallest->length;
                        ଭାଙ୍ଗ;
                      }
                  ଯେତେବେଳେ ((savealloc *= 2) < smallest->length);

                  ମୁକ୍ତ (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              ଯଦି (ଚାବି)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      ଅନ୍ୟଥା
        write_ଧାଡ଼ି (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ଯଦି (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      ଅନ୍ୟଥା
        {
          ଯଦି (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ଖାଲି))
            {
              ସଂରଚନା ଧାଡ଼ି ଅଚଳ *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          ଅନ୍ୟଥା
            {
              /* We reached EOF on fps[ord[0]].  */
              ପାଇଁ (size_t i = 1; i < nfiles; ++i)
                ଯଦି (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ଯଦି (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              ମୁକ୍ତ (buffer[ord[0]].buf);
              ପାଇଁ (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              ପାଇଁ (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ଜାରି;
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
        size_t ଗଣନା_of_smaller_ଧାଡ଼ିଗୁଡ଼ିକ;

        ଯେତେବେଳେ (lo < hi)
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp = compare (cur[ord0], cur[ord[probe]]);
            ଯଦି (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ଅନ୍ୟଥା
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ଗଣନା_of_smaller_ଧାଡ଼ିଗୁଡ଼ିକ = lo - 1;
        ପାଇଁ (size_t j = 0; j < ଗଣନା_of_smaller_ଧାଡ଼ିଗୁଡ଼ିକ; j++)
          ord[j] = ord[j + 1];
        ord[ଗଣନା_of_smaller_ଧାଡ଼ିଗୁଡ଼ିକ] = ord0;
      }
    }

  ଯଦି (unique && savedline)
    {
      write_ଧାଡ଼ି (&saved, ofp, output_file);
      ମୁକ୍ତ (saved.text);
    }

  xfclose (ofp, output_file);
  ମୁକ୍ତ (fps);
  ମୁକ୍ତ (buffer);
  ମୁକ୍ତ (ord);
  ମୁକ୍ତ (base);
  ମୁକ୍ତ (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ସ୍ଥିର size_t
mergefiles (ସଂରଚନା sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, ଅକ୍ଷର ଅଚଳ *output_file)
{
  FILE **fps;
  size_t nopened = open_ନିବେଶ_files (files, nfiles, &fps);
  ଯଦି (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  ଫେରନ୍ତୁ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ସ୍ଥିର ଶୂନ୍ୟ
mergelines (ସଂରଚନା ଧାଡ଼ି *restrict t, size_t nlines,
            ସଂରଚନା ଧାଡ଼ି ଅଚଳ *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ସଂରଚନା ଧାଡ଼ି *hi = t - nlo;

  ଯେତେବେଳେ (true)
    ଯଦି (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ଯଦି (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            ଫେରନ୍ତୁ;
          }
      }
    ଅନ୍ୟଥା
      {
        *--t = *--hi;
        ଯଦି (! --nhi)
          {
            କର
              *--t = *--lo;
            ଯେତେବେଳେ (--nlo);

            ଫେରନ୍ତୁ;
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

ସ୍ଥିର ଶୂନ୍ୟ
sequential_sort (ସଂରଚନା ଧାଡ଼ି *restrict ଧାଡ଼ିଗୁଡ଼ିକ, size_t nlines,
                 ସଂରଚନା ଧାଡ଼ି *restrict temp, bool to_temp)
{
  ଯଦି (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା swap = (0 < compare (&ଧାଡ଼ିଗୁଡ଼ିକ[-1], &ଧାଡ଼ିଗୁଡ଼ିକ[-2]));
      ଯଦି (to_temp)
        {
          temp[-1] = ଧାଡ଼ିଗୁଡ଼ିକ[-1 - swap];
          temp[-2] = ଧାଡ଼ିଗୁଡ଼ିକ[-2 + swap];
        }
      ଅନ୍ୟଥା ଯଦି (swap)
        {
          temp[-1] = ଧାଡ଼ିଗୁଡ଼ିକ[-1];
          ଧାଡ଼ିଗୁଡ଼ିକ[-1] = ଧାଡ଼ିଗୁଡ଼ିକ[-2];
          ଧାଡ଼ିଗୁଡ଼ିକ[-2] = temp[-1];
        }
    }
  ଅନ୍ୟଥା
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ସଂରଚନା ଧାଡ଼ି *lo = ଧାଡ଼ିଗୁଡ଼ିକ;
      ସଂରଚନା ଧାଡ଼ି *hi = ଧାଡ଼ିଗୁଡ଼ିକ - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      ଯଦି (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      ଅନ୍ୟଥା ଯଦି (!to_temp)
        temp[-1] = lo[-1];

      ସଂରଚନା ଧାଡ଼ି *dest;
      ସଂରଚନା ଧାଡ଼ି ଅଚଳ *sorted_lo;
      ଯଦି (to_temp)
        {
          dest = temp;
          sorted_lo = ଧାଡ଼ିଗୁଡ଼ିକ;
        }
      ଅନ୍ୟଥା
        {
          dest = ଧାଡ଼ିଗୁଡ଼ିକ;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ସ୍ଥିର ସଂରଚନା merge_ଗଣ୍ଠି *init_ଗଣ୍ଠି (ସଂରଚନା merge_ଗଣ୍ଠି *restrict,
                                     ସଂରଚନା merge_ଗଣ୍ଠି *restrict,
                                     ସଂରଚନା ଧାଡ଼ି *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ସ୍ଥିର ସଂରଚନା merge_ଗଣ୍ଠି *
merge_ବୃକ୍ଷ_init (size_t nthreads, size_t nlines, ସଂରଚନା ଧାଡ଼ି *dest)
{
  ସଂରଚନା merge_ଗଣ୍ଠି *merge_ବୃକ୍ଷ = xmalloc (2 * ଆକାର *merge_ବୃକ୍ଷ * nthreads);

  ସଂରଚନା merge_ଗଣ୍ଠି *ମୂଳ = merge_ବୃକ୍ଷ;
  ମୂଳ->lo = ମୂଳ->hi = ମୂଳ->end_lo = ମୂଳ->end_hi = ଖାଲି;
  ମୂଳ->dest = ଖାଲି;
  ମୂଳ->nlo = ମୂଳ->nhi = nlines;
  ମୂଳ->parent = ଖାଲି;
  ମୂଳ->level = MERGE_END;
  ମୂଳ->queued = false;
  pthread_mutex_init (&ମୂଳ->lock, ଖାଲି);

  init_ଗଣ୍ଠି (ମୂଳ, ମୂଳ + 1, dest, nthreads, nlines, false);
  ଫେରନ୍ତୁ merge_ବୃକ୍ଷ;
}

/* Destroy the merge tree. */
ସ୍ଥିର ଶୂନ୍ୟ
merge_ବୃକ୍ଷ_destroy (size_t nthreads, ସଂରଚନା merge_ଗଣ୍ଠି *merge_ବୃକ୍ଷ)
{
  size_t n_nodes = nthreads * 2;
  ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି = merge_ବୃକ୍ଷ;

  ଯେତେବେଳେ (n_nodes--)
    {
      pthread_mutex_destroy (&ଗଣ୍ଠି->lock);
      ଗଣ୍ଠି++;
    }

  ମୁକ୍ତ (merge_ବୃକ୍ଷ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ସ୍ଥିର ସଂରଚନା merge_ଗଣ୍ଠି *
init_ଗଣ୍ଠି (ସଂରଚନା merge_ଗଣ୍ଠି *restrict parent,
           ସଂରଚନା merge_ଗଣ୍ଠି *restrict ଗଣ୍ଠି_pool,
           ସଂରଚନା ଧାଡ଼ି *dest, size_t nthreads,
           size_t total_ଧାଡ଼ିଗୁଡ଼ିକ, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ସଂରଚନା ଧାଡ଼ି *lo = dest - total_ଧାଡ଼ିଗୁଡ଼ିକ;
  ସଂରଚନା ଧାଡ଼ି *hi = lo - nlo;
  ସଂରଚନା ଧାଡ଼ି **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି = ଗଣ୍ଠି_pool++;
  ଗଣ୍ଠି->lo = ଗଣ୍ଠି->end_lo = lo;
  ଗଣ୍ଠି->hi = ଗଣ୍ଠି->end_hi = hi;
  ଗଣ୍ଠି->dest = parent_end;
  ଗଣ୍ଠି->nlo = nlo;
  ଗଣ୍ଠି->nhi = nhi;
  ଗଣ୍ଠି->parent = parent;
  ଗଣ୍ଠି->level = parent->level + 1;
  ଗଣ୍ଠି->queued = false;
  pthread_mutex_init (&ଗଣ୍ଠି->lock, ଖାଲି);

  ଯଦି (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      ଗଣ୍ଠି->lo_child = ଗଣ୍ଠି_pool;
      ଗଣ୍ଠି_pool = init_ଗଣ୍ଠି (ଗଣ୍ଠି, ଗଣ୍ଠି_pool, lo, lo_threads,
                             total_ଧାଡ଼ିଗୁଡ଼ିକ, true);
      ଗଣ୍ଠି->hi_child = ଗଣ୍ଠି_pool;
      ଗଣ୍ଠି_pool = init_ଗଣ୍ଠି (ଗଣ୍ଠି, ଗଣ୍ଠି_pool, hi, hi_threads,
                             total_ଧାଡ଼ିଗୁଡ଼ିକ, false);
    }
  ଅନ୍ୟଥା
    {
      ଗଣ୍ଠି->lo_child = ଖାଲି;
      ଗଣ୍ଠି->hi_child = ଖାଲି;
    }
  ଫେରନ୍ତୁ ଗଣ୍ଠି_pool;
}


/* Compare two merge nodes A and B for priority.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_nodes (ଶୂନ୍ୟ ଅଚଳ *a, ଶୂନ୍ୟ ଅଚଳ *b)
{
  ସଂରଚନା merge_ଗଣ୍ଠି ଅଚଳ *nodea = a;
  ସଂରଚନା merge_ଗଣ୍ଠି ଅଚଳ *nodeb = b;
  ଯଦି (nodea->level == nodeb->level)
      ଫେରନ୍ତୁ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  ଫେରନ୍ତୁ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
lock_ଗଣ୍ଠି (ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
  pthread_mutex_lock (&ଗଣ୍ଠି->lock);
}

/* Unlock a merge tree NODE. */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
unlock_ଗଣ୍ଠି (ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
  pthread_mutex_unlock (&ଗଣ୍ଠି->lock);
}

/* Destroy merge QUEUE. */

ସ୍ଥିର ଶୂନ୍ୟ
queue_destroy (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ସ୍ଥିର ଶୂନ୍ୟ
queue_init (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, ଖାଲି);
  pthread_cond_init (&queue->cond, ଖାଲି);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ସ୍ଥିର ଶୂନ୍ୟ
queue_ଭର୍ତ୍ତି (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue, ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
  pthread_mutex_lock (&queue->mutex);
  heap_ଭର୍ତ୍ତି (queue->priority_queue, ଗଣ୍ଠି);
  ଗଣ୍ଠି->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ସ୍ଥିର ସଂରଚନା merge_ଗଣ୍ଠି *
queue_କାଢ଼ (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue)
{
  ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି;
  pthread_mutex_lock (&queue->mutex);
  ଯେତେବେଳେ (! (ଗଣ୍ଠି = heap_remove_ଶୀର୍ଷ (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_ଗଣ୍ଠି (ଗଣ୍ଠି);
  ଗଣ୍ଠି->queued = false;
  ଫେରନ୍ତୁ ଗଣ୍ଠି;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ସ୍ଥିର ଶୂନ୍ୟ
write_unique (ସଂରଚନା ଧାଡ଼ି ଅଚଳ *ଧାଡ଼ି, FILE *tfp, ଅକ୍ଷର ଅଚଳ *temp_output)
{
  ଯଦି (unique)
    {
      ଯଦି (saved_ଧାଡ଼ି.text && ! compare (ଧାଡ଼ି, &saved_ଧାଡ଼ି))
        ଫେରନ୍ତୁ;
      saved_ଧାଡ଼ି = *ଧାଡ଼ି;
    }

  write_ଧାଡ଼ି (ଧାଡ଼ି, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ସ୍ଥିର ଶୂନ୍ୟ
mergelines_ଗଣ୍ଠି (ସଂରଚନା merge_ଗଣ୍ଠି *restrict ଗଣ୍ଠି, size_t total_ଧାଡ଼ିଗୁଡ଼ିକ,
                 FILE *tfp, ଅକ୍ଷର ଅଚଳ *temp_output)
{
  ସଂରଚନା ଧାଡ଼ି *lo_orig = ଗଣ୍ଠି->lo;
  ସଂରଚନା ଧାଡ଼ି *hi_orig = ଗଣ୍ଠି->hi;
  size_t to_merge = MAX_MERGE (total_ଧାଡ଼ିଗୁଡ଼ିକ, ଗଣ୍ଠି->level);
  size_t merged_lo;
  size_t merged_hi;

  ଯଦି (ଗଣ୍ଠି->level > MERGE_ମୂଳ)
    {
      /* Merge to destination buffer. */
      ସଂରଚନା ଧାଡ଼ି *dest = *ଗଣ୍ଠି->dest;
      ଯେତେବେଳେ (ଗଣ୍ଠି->lo != ଗଣ୍ଠି->end_lo && ଗଣ୍ଠି->hi != ଗଣ୍ଠି->end_hi && to_merge--)
        ଯଦି (compare (ଗଣ୍ଠି->lo - 1, ଗଣ୍ଠି->hi - 1) <= 0)
          *--dest = *--ଗଣ୍ଠି->lo;
        ଅନ୍ୟଥା
          *--dest = *--ଗଣ୍ଠି->hi;

      merged_lo = lo_orig - ଗଣ୍ଠି->lo;
      merged_hi = hi_orig - ଗଣ୍ଠି->hi;

      ଯଦି (ଗଣ୍ଠି->nhi == merged_hi)
        ଯେତେବେଳେ (ଗଣ୍ଠି->lo != ଗଣ୍ଠି->end_lo && to_merge--)
          *--dest = *--ଗଣ୍ଠି->lo;
      ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->nlo == merged_lo)
        ଯେତେବେଳେ (ଗଣ୍ଠି->hi != ଗଣ୍ଠି->end_hi && to_merge--)
          *--dest = *--ଗଣ୍ଠି->hi;
      *ଗଣ୍ଠି->dest = dest;
    }
  ଅନ୍ୟଥା
    {
      /* Merge directly to output. */
      ଯେତେବେଳେ (ଗଣ୍ଠି->lo != ଗଣ୍ଠି->end_lo && ଗଣ୍ଠି->hi != ଗଣ୍ଠି->end_hi && to_merge--)
        {
          ଯଦି (compare (ଗଣ୍ଠି->lo - 1, ଗଣ୍ଠି->hi - 1) <= 0)
            write_unique (--ଗଣ୍ଠି->lo, tfp, temp_output);
          ଅନ୍ୟଥା
            write_unique (--ଗଣ୍ଠି->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - ଗଣ୍ଠି->lo;
      merged_hi = hi_orig - ଗଣ୍ଠି->hi;

      ଯଦି (ଗଣ୍ଠି->nhi == merged_hi)
        {
          ଯେତେବେଳେ (ଗଣ୍ଠି->lo != ଗଣ୍ଠି->end_lo && to_merge--)
            write_unique (--ଗଣ୍ଠି->lo, tfp, temp_output);
        }
      ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->nlo == merged_lo)
        {
          ଯେତେବେଳେ (ଗଣ୍ଠି->hi != ଗଣ୍ଠି->end_hi && to_merge--)
            write_unique (--ଗଣ୍ଠି->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ଗଣ୍ଠି->lo;
  merged_hi = hi_orig - ଗଣ୍ଠି->hi;
  ଗଣ୍ଠି->nlo -= merged_lo;
  ଗଣ୍ଠି->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ସ୍ଥିର ଶୂନ୍ୟ
queue_check_ଭର୍ତ୍ତି (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue, ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
  ଯଦି (! ଗଣ୍ଠି->queued)
    {
      bool lo_avail = (ଗଣ୍ଠି->lo - ଗଣ୍ଠି->end_lo) != 0;
      bool hi_avail = (ଗଣ୍ଠି->hi - ଗଣ୍ଠି->end_hi) != 0;
      ଯଦି (lo_avail ? hi_avail || ! ଗଣ୍ଠି->nhi : hi_avail && ! ଗଣ୍ଠି->nlo)
        queue_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ସ୍ଥିର ଶୂନ୍ୟ
queue_check_ଭର୍ତ୍ତି_parent (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue,
                           ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି)
{
  ଯଦି (ଗଣ୍ଠି->level > MERGE_ମୂଳ)
    {
      lock_ଗଣ୍ଠି (ଗଣ୍ଠି->parent);
      queue_check_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି->parent);
      unlock_ଗଣ୍ଠି (ଗଣ୍ଠି->parent);
    }
  ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->nlo + ଗଣ୍ଠି->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ସ୍ଥିର ଶୂନ୍ୟ
merge_loop (ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue,
            size_t total_ଧାଡ଼ିଗୁଡ଼ିକ, FILE *tfp, ଅକ୍ଷର ଅଚଳ *temp_output)
{
  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି = queue_କାଢ଼ (queue);

      ଯଦି (ଗଣ୍ଠି->level == MERGE_END)
        {
          unlock_ଗଣ୍ଠି (ଗଣ୍ଠି);
          /* Reinsert so other threads can pop it. */
          queue_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି);
          ଭାଙ୍ଗ;
        }
      mergelines_ଗଣ୍ଠି (ଗଣ୍ଠି, total_ଧାଡ଼ିଗୁଡ଼ିକ, tfp, temp_output);
      queue_check_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି);
      queue_check_ଭର୍ତ୍ତି_parent (queue, ଗଣ୍ଠି);

      unlock_ଗଣ୍ଠି (ଗଣ୍ଠି);
    }
}


ସ୍ଥିର ଶୂନ୍ୟ sortlines (ସଂରଚନା ଧାଡ଼ି *restrict, size_t, size_t,
                       ସଂରଚନା merge_ଗଣ୍ଠି *, ସଂରଚନା merge_ଗଣ୍ଠି_queue *,
                       FILE *, ଅକ୍ଷର ଅଚଳ *);

/* Thread arguments for sortlines_thread. */

ସଂରଚନା thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ସଂରଚନା ଧାଡ଼ି *ଧାଡ଼ିଗୁଡ଼ିକ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ଅଚଳ total_ଧାଡ଼ିଗୁଡ଼ିକ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ସଂରଚନା merge_ଗଣ୍ଠି *ଅଚଳ ଗଣ୍ଠି;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ସଂରଚନା merge_ଗଣ୍ଠି_queue *ଅଚଳ queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  ଅକ୍ଷର ଅଚଳ *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ସ୍ଥିର ଶୂନ୍ୟ *
sortlines_thread (ଶୂନ୍ୟ *data)
{
  ସଂରଚନା thread_args ଅଚଳ *args = data;
  sortlines (args->ଧାଡ଼ିଗୁଡ଼ିକ, args->nthreads, args->total_ଧାଡ଼ିଗୁଡ଼ିକ,
             args->ଗଣ୍ଠି, args->queue, args->tfp,
             args->output_temp);
  ଫେରନ୍ତୁ ଖାଲି;
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

ସ୍ଥିର ଶୂନ୍ୟ
sortlines (ସଂରଚନା ଧାଡ଼ି *restrict ଧାଡ଼ିଗୁଡ଼ିକ, size_t nthreads,
           size_t total_ଧାଡ଼ିଗୁଡ଼ିକ, ସଂରଚନା merge_ଗଣ୍ଠି *ଗଣ୍ଠି,
           ସଂରଚନା merge_ଗଣ୍ଠି_queue *queue, FILE *tfp, ଅକ୍ଷର ଅଚଳ *temp_output)
{
  size_t nlines = ଗଣ୍ଠି->nlo + ଗଣ୍ଠି->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  ସଂରଚନା thread_args args = {ଧାଡ଼ିଗୁଡ଼ିକ, lo_threads, total_ଧାଡ଼ିଗୁଡ଼ିକ,
                             ଗଣ୍ଠି->lo_child, queue, tfp, temp_output};

  ଯଦି (nthreads > 1 && SUBTHREAD_ଧାଡ଼ିଗୁଡ଼ିକ_HEURISTIC <= nlines
      && pthread_create (&thread, ଖାଲି, sortlines_thread, &args) == 0)
    {
      sortlines (ଧାଡ଼ିଗୁଡ଼ିକ - ଗଣ୍ଠି->nlo, hi_threads, total_ଧାଡ଼ିଗୁଡ଼ିକ,
                 ଗଣ୍ଠି->hi_child, queue, tfp, temp_output);
      pthread_join (thread, ଖାଲି);
    }
  ଅନ୍ୟଥା
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ଗଣ୍ଠି->nlo;
      size_t nhi = ଗଣ୍ଠି->nhi;
      ସଂରଚନା ଧାଡ଼ି *temp = ଧାଡ଼ିଗୁଡ଼ିକ - total_ଧାଡ଼ିଗୁଡ଼ିକ;
      ଯଦି (1 < nhi)
        sequential_sort (ଧାଡ଼ିଗୁଡ଼ିକ - nlo, nhi, temp - nlo / 2, false);
      ଯଦି (1 < nlo)
        sequential_sort (ଧାଡ଼ିଗୁଡ଼ିକ, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      ଗଣ୍ଠି->lo = ଧାଡ଼ିଗୁଡ଼ିକ;
      ଗଣ୍ଠି->hi = ଧାଡ଼ିଗୁଡ଼ିକ - nlo;
      ଗଣ୍ଠି->end_lo = ଧାଡ଼ିଗୁଡ଼ିକ - nlo;
      ଗଣ୍ଠି->end_hi = ଧାଡ଼ିଗୁଡ଼ିକ - nlo - nhi;

      queue_ଭର୍ତ୍ତି (queue, ଗଣ୍ଠି);
      merge_loop (queue, total_ଧାଡ଼ିଗୁଡ଼ିକ, tfp, temp_output);
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

ସ୍ଥିର ଶୂନ୍ୟ
avoid_trashing_ନିବେଶ (ସଂରଚନା sortfile *files, size_t ntemps,
                      size_t nfiles, ଅକ୍ଷର ଅଚଳ *outfile)
{
  ସଂରଚନା tempnode *tempcopy = ଖାଲି;

  ପାଇଁ (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ସଂରଚନା stat instat;

      ଯଦି (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ଅନ୍ୟଥା
        {
          ସଂରଚନା stat *outst = get_outstatus ();
          ଯଦି (!outst)
            ଭାଙ୍ଗ;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ଯଦି (same)
        {
          ଯଦି (! tempcopy)
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

ସ୍ଥିର ଶୂନ୍ୟ
check_inputs (ଅକ୍ଷର *ଅଚଳ *files, size_t nfiles)
{
  ପାଇଁ (size_t i = 0; i < nfiles; i++)
    {
      ଯଦି (streq (files[i], "-"))
        ଜାରି;

      ଯଦି (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ସ୍ଥିର ଶୂନ୍ୟ
check_output (ଅକ୍ଷର ଅଚଳ *outfile)
{
  ଯଦି (outfile)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oflags = O_WRONLY | O_ଦ୍ୱିମିକ | O_CLOEXEC | O_CREAT;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା outfd = open (outfile, oflags, MODE_RW_UGO);
      ଯଦି (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ସ୍ଥିର ଶୂନ୍ୟ
merge (ସଂରଚନା sortfile *files, size_t ntemps, size_t nfiles,
       ଅକ୍ଷର ଅଚଳ *output_file)
{
  ଯେତେବେଳେ (nmerge < nfiles)
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
      ପାଇଁ (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          ସଂରଚନା tempnode *temp = create_temp (&tfp);
          size_t ଅଙ୍କ_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, ଅଙ୍କ_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += ଅଙ୍କ_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      ଯଦି (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ସଂରଚନା tempnode *temp = create_temp (&tfp);
          size_t ଅଙ୍କ_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, ଅଙ୍କ_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += ଅଙ୍କ_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * ଆକାର *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_ନିବେଶ (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ଯେତେବେଳେ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ନିବେଶ_files (files, nfiles, &fps);

      ଯଦି (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          ଯଦି (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ଭାଙ୍ଗ;
            }
          ଯଦି (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      ଅନ୍ୟଥା ଯଦି (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ସଂରଚନା tempnode *temp;
      କର
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      ଯେତେବେଳେ (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * ଆକାର *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ସ୍ଥିର ଶୂନ୍ୟ
sort (ଅକ୍ଷର *ଅଚଳ *files, size_t nfiles, ଅକ୍ଷର ଅଚଳ *output_file,
      size_t nthreads)
{
  ସଂରଚନା buffer buf;
  ସଂରଚନା sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  ଯେତେବେଳେ (nfiles)
    {
      ଅକ୍ଷର ଅଚଳ *temp_output;
      ଅକ୍ଷର ଅଚଳ *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_ଧାଡ଼ି;
      ଯଦି (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ଯେତେବେଳେ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_ଧାଡ଼ି = (mult * ଆକାର (ସଂରଚନା ଧାଡ଼ି));
        }
      ଅନ୍ୟଥା
        bytes_per_ଧାଡ଼ି = ଆକାର (ସଂରଚନା ଧାଡ଼ି) * 3 / 2;

      ଯଦି (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_ଧାଡ଼ି,
                              &policy);
          initbuf (&buf, bytes_per_ଧାଡ଼ି, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ଯେତେବେଳେ (fillbuf (&buf, fp, file, &policy))
        {
          ସଂରଚନା ଧାଡ଼ି *ଧାଡ଼ି;

          ଯଦି (buf.eof && nfiles
              && (bytes_per_ଧାଡ଼ି + 1
                  < (buf.alloc - buf.used - bytes_per_ଧାଡ଼ି * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ବାମ = buf.used;
              ଭାଙ୍ଗ;
            }

          saved_ଧାଡ଼ି.text = ଖାଲି;
          ଧାଡ଼ି = buffer_linelim (&buf);
          ଯଦି (buf.eof && !nfiles && !ntemps && !buf.ବାମ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          ଅନ୍ୟଥା
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          ଯଦି (1 < buf.nlines)
            {
              ଯଦି (nthreads > 1)
                {
                  ସଂରଚନା merge_ଗଣ୍ଠି_queue queue;
                  queue_init (&queue, nthreads);
                  ସଂରଚନା merge_ଗଣ୍ଠି *merge_ବୃକ୍ଷ =
                    merge_ବୃକ୍ଷ_init (nthreads, buf.nlines, ଧାଡ଼ି);

                  sortlines (ଧାଡ଼ି, nthreads, buf.nlines, merge_ବୃକ୍ଷ + 1,
                             &queue, tfp, temp_output);

                  merge_ବୃକ୍ଷ_destroy (nthreads, merge_ବୃକ୍ଷ);
                  queue_destroy (&queue);
                }
              ଅନ୍ୟଥା
                {
                  sequential_sort (ଧାଡ଼ି, buf.nlines,
                                   ଧାଡ଼ି - buf.nlines, false);
                  ପାଇଁ (size_t i = 0; i < buf.nlines; i++)
                    write_unique (ଧାଡ଼ି - i - 1, tfp, temp_output);
                }
            }
          ଅନ୍ୟଥା
            write_unique (ଧାଡ଼ି - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          ଯଦି (output_file_created)
            ଯାଅ finish;
        }
      xfclose (fp, file);
    }

 finish:
  ମୁକ୍ତ (buf.buf);

  ଯଦି (! output_file_created)
    {
      ସଂରଚନା tempnode *ଗଣ୍ଠି = temphead;
      ସଂରଚନା sortfile *tempfiles = xnmalloc (ntemps, ଆକାର *tempfiles);
      ପାଇଁ (size_t i = 0; ଗଣ୍ଠି; i++)
        {
          tempfiles[i].name = ଗଣ୍ଠି->name;
          tempfiles[i].temp = ଗଣ୍ଠି;
          ଗଣ୍ଠି = ଗଣ୍ଠି->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      ମୁକ୍ତ (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ସ୍ଥିର ଶୂନ୍ୟ
insertkey (ସଂରଚନା keyfield *ଚାବି_arg)
{
  ସଂରଚନା keyfield **p;
  ସଂରଚନା keyfield *ଚାବି = xmemdup (ଚାବି_arg, ଆକାର *ଚାବି);

  ପାଇଁ (p = &keylist; *p; p = &(*p)->next)
    ଜାରି;
  *p = ଚାବି;
  ଚାବି->next = ଖାଲି;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ସ୍ଥିର ଶୂନ୍ୟ
badfieldspec (ଅକ୍ଷର ଅଚଳ *spec, ଅକ୍ଷର ଅଚଳ *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ସ୍ଥିର ଶୂନ୍ୟ
incompatible_options (ଅକ୍ଷର ଅଚଳ *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ସ୍ଥିର ଶୂନ୍ୟ
check_ordering_compatibility (ଶୂନ୍ୟ)
{
  ପାଇଁ (ସଂରଚନା keyfield *ଚାବି = keylist; ଚାବି; ଚାବି = ଚାବି->next)
    ଯଦି (1 < (ଚାବି->numeric + ଚାବି->general_numeric + ଚାବି->human_numeric
             + ଚାବି->month + (ଚାବି->version | ଚାବି->random | !!ଚାବି->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        ଅକ୍ଷର opts[ଆକାର short_options];
        /* Clear flags we're not interested in.  */
        ଚାବି->skipsblanks = ଚାବି->skipeblanks = ଚାବି->reverse = false;
        ଚାବି_to_opts (ଚାବି, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
parse_field_ଗଣନା (ଅକ୍ଷର ଅଚଳ *string, size_t *val, ଅକ୍ଷର ଅଚଳ *msgid)
{
  ଅକ୍ଷର *suffix;
  uintmax_t n;

  ବାଛ (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    ସ୍ଥିତି LONGINT_OK:
    ସ୍ଥିତି LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ଯଦି (*val == n)
        ଭାଙ୍ଗ;
      FALLTHROUGH;
    ସ୍ଥିତି LONGINT_OVERFLOW:
    ସ୍ଥିତି LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି LONGINT_INVALID:
      ଯଦି (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଫେରନ୍ତୁ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ସ୍ଥିର ଅକ୍ଷର *
set_ordering (ଅକ୍ଷର ଅଚଳ *s, ସଂରଚନା keyfield *ଚାବି, ଗଣନା blanktype blanktype)
{
  ଯେତେବେଳେ (*s)
    {
      ବାଛ (*s)
        {
        ସ୍ଥିତି 'b':
          ଯଦି (blanktype == bl_start || blanktype == bl_both)
            ଚାବି->skipsblanks = true;
          ଯଦି (blanktype == bl_end || blanktype == bl_both)
            ଚାବି->skipeblanks = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'd':
          ଚାବି->ignore = nondictionary;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
          ଚାବି->translate = fold_toupper;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'g':
          ଚାବି->general_numeric = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'h':
          ଚାବି->human_numeric = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ଯଦି (! ଚାବି->ignore)
            ଚାବି->ignore = nonprinting;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'M':
          ଚାବି->month = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          ଚାବି->numeric = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'R':
          ଚାବି->random = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'r':
          ଚାବି->reverse = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'V':
          ଚାବି->version = true;
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଫେରନ୍ତୁ (ଅକ୍ଷର *) s;
        }
      ++s;
    }
  ଫେରନ୍ତୁ (ଅକ୍ଷର *) s;
}

/* Initialize KEY.  */

ସ୍ଥିର ସଂରଚନା keyfield *
ଚାବି_init (ସଂରଚନା keyfield *ଚାବି)
{
  memset (ଚାବି, 0, ଆକାର *ଚାବି);
  ଚାବି->eword = SIZE_MAX;
  ଫେରନ୍ତୁ ଚାବି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ସଂରଚନା keyfield *ଚାବି;
  ସଂରଚନା keyfield ଚାବି_buf;
  ସଂରଚନା keyfield gkey;
  bool gkey_only = false;
  ଅକ୍ଷର ଅଚଳ *s;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = 0;
  ଅକ୍ଷର checkonly = 0;
  bool mergeonly = false;
  ଅକ୍ଷର *random_source = ଖାଲି;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ଖାଲି);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  ଅକ୍ଷର **files;
  ଅକ୍ଷର *files_from = ଖାଲି;
  ସଂରଚନା Tokens tok;
  ଅକ୍ଷର ଅଚଳ *outfile = ଖାଲି;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ଯଦି HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    ସଂରଚନା lconv ଅଚଳ *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    ଯଦି (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ଯଦି (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ଯଦି (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ଗଣନା { nsigs = countof (term_sig) };

    ସଂରଚନା sigaction act;

    sigemptyset (&caught_signals);
    ପାଇଁ (size_t i = 0; i < nsigs; i++)
      {
        ଯଦି (term_sig[i] == SIGPIPE)
          ଜାରି;  /* Handled below.  */
        sigaction (term_sig[i], ଖାଲି, &act);
        ଯଦି (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    ପାଇଁ (size_t i = 0; i < nsigs; i++)
      {
        ଯଦି (term_sig[i] == SIGPIPE)
          ଜାରି;  /* Handled below.  */
        ଯଦି (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, ଖାଲି);
      }
  }
  ସଙ୍କେତ (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ଚାବି_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, ଆକାର *files);

  ଯେତେବେଳେ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା oi = -1;

      ଯଦି (c == -1
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
          ଯଦି (argc <= optind)
            ଭାଙ୍ଗ;
          files[nfiles++] = argv[optind++];
        }
      ଅନ୍ୟଥା ବାଛ (c)
        {
        ସ୍ଥିତି 1:
          ଚାବି = ଖାଲି;
          ଯଦି (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ଯଦି (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ଚାବି = ଚାବି_init (&ଚାବି_buf);
                  s = parse_field_ଗଣନା (optarg + 1, &ଚାବି->sword, ଖାଲି);
                  ଯଦି (s && *s == '.')
                    s = parse_field_ଗଣନା (s + 1, &ଚାବି->schar, ଖାଲି);
                  ଯଦି (! (ଚାବି->sword || ଚାବି->schar))
                    ଚାବି->sword = SIZE_MAX;
                  ଯଦି (! s || *set_ordering (s, ଚାବି, bl_start))
                    ଚାବି = ଖାଲି;
                  ଅନ୍ୟଥା
                    {
                      ଯଦି (minus_pos_usage)
                        {
                          ଅକ୍ଷର ଅଚଳ *optarg1 = argv[optind++];
                          s = parse_field_ଗଣନା (optarg1 + 1, &ଚାବି->eword,
                                             N_("invalid number after '-'"));
                          ଯଦି (*s == '.')
                            s = parse_field_ଗଣନା (s + 1, &ଚାବି->echar,
                                               N_("invalid number after '.'"));
                          ଯଦି (!ଚାବି->echar && ଚାବି->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ଚାବି->eword--;
                            }
                          ଯଦି (*set_ordering (s, ଚାବି, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ଚାବି->traditional_used = true;
                      insertkey (ଚାବି);
                    }
                }
            }
          ଯଦି (! ଚାବି)
            files[nfiles++] = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        ସ୍ଥିତି 'b':
        ସ୍ଥିତି 'd':
        ସ୍ଥିତି 'f':
        ସ୍ଥିତି 'g':
        ସ୍ଥିତି 'h':
        ସ୍ଥିତି 'i':
        ସ୍ଥିତି 'M':
        ସ୍ଥିତି 'n':
        ସ୍ଥିତି 'r':
        ସ୍ଥିତି 'R':
        ସ୍ଥିତି 'V':
          {
            ଅକ୍ଷର str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        ସ୍ଥିତି 'c':
        ସ୍ଥିତି 'C':
          ଯଦି (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି COMPRESS_PROGRAM_OPTION:
          ଯଦି (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEBUG_PROGRAM_OPTION:
          debug = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି FILES0_FROM_OPTION:
          files_from = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'k':
          ଚାବି = ଚାବି_init (&ଚାବି_buf);

          /* Get POS1. */
          s = parse_field_ଗଣନା (optarg, &ଚାବି->sword,
                                 N_("invalid number at field start"));
          ଯଦି (! ଚାବି->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ଯଦି (*s == '.')
            {
              s = parse_field_ଗଣନା (s + 1, &ଚାବି->schar,
                                     N_("invalid number after '.'"));
              ଯଦି (! ଚାବି->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ଯଦି (! (ଚାବି->sword || ଚାବି->schar))
            ଚାବି->sword = SIZE_MAX;
          s = set_ordering (s, ଚାବି, bl_start);
          ଯଦି (*s != ',')
            {
              ଚାବି->eword = SIZE_MAX;
              ଚାବି->echar = 0;
            }
          ଅନ୍ୟଥା
            {
              /* Get POS2. */
              s = parse_field_ଗଣନା (s + 1, &ଚାବି->eword,
                                     N_("invalid number after ','"));
              ଯଦି (! ଚାବି->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ଯଦି (*s == '.')
                {
                  s = parse_field_ଗଣନା (s + 1, &ଚାବି->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, ଚାବି, bl_end);
            }
          ଯଦି (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ଚାବି);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
          mergeonly = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'o':
          ଯଦି (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RANDOM_SOURCE_OPTION:
          ଯଦି (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          stable = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'S':
          specify_sort_size (oi, c, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          {
            ଅକ୍ଷର newtab = optarg[0];
            ଯଦି (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            ଯଦି (optarg[1])
              {
                ଯଦି (streq (optarg, "\\0"))
                  newtab = '\0';
                ଅନ୍ୟଥା
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ଯଦି (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'T':
          add_temp_dir (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'u':
          unique = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ଯଦି (optarg == argv[optind - 1])
            {
              ଅକ୍ଷର ଅଚଳ *p;
              ପାଇଁ (p = optarg; c_isdigit (*p); p++)
                ଜାରି;
              optind -= (*p != '\0');
            }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          eolchar = 0;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (ସଙ୍କେତ (SIGPIPE, SIG_IGN) == SIG_DFL);

  ଯଦି (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ଯଦି (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ଯଦି (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      ଯଦି (tok.n_tok)
        {
          ମୁକ୍ତ (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          ପାଇଁ (size_t i = 0; i < nfiles; i++)
            {
              ଯଦି (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ଅନ୍ୟଥା ଯଦି (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_ସଂଖ୍ୟା = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_ସଂଖ୍ୟା);
                }
            }
        }
      ଅନ୍ୟଥା
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  ପାଇଁ (ଚାବି = keylist; ଚାବି; ଚାବି = ଚାବି->next)
    {
      ଯଦି (default_ଚାବି_compare (ଚାବି) && !ଚାବି->reverse)
        {
          ଚାବି->ignore = gkey.ignore;
          ଚାବି->translate = gkey.translate;
          ଚାବି->skipsblanks = gkey.skipsblanks;
          ଚାବି->skipeblanks = gkey.skipeblanks;
          ଚାବି->month = gkey.month;
          ଚାବି->numeric = gkey.numeric;
          ଚାବି->general_numeric = gkey.general_numeric;
          ଚାବି->human_numeric = gkey.human_numeric;
          ଚାବି->version = gkey.version;
          ଚାବି->random = gkey.random;
          ଚାବି->reverse = gkey.reverse;
        }

      need_random |= ଚାବି->random;
    }

  ଯଦି (!keylist && !default_ଚାବି_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ଯଦି (debug)
    {
      ଯଦି (checkonly || outfile)
        {
          ସ୍ଥିର ଅକ୍ଷର opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ଯଦି (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ଯଦି (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ଯଦି (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ଖାଲି)));
      ଅନ୍ୟଥା
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ଚାବି_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  ଯଦି (need_random)
    random_md5_state_init (random_source);

  ଯଦି (temp_dirs.n_dirs == 0)
    {
      ଅକ୍ଷର ଅଚଳ *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ଯଦି (nfiles == 0)
    {
      nfiles = 1;
      ମୁକ୍ତ (files);
      files = xmalloc (ଆକାର *files);
      *files = (ଅକ୍ଷର *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ଯଦି (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  ଯଦି (checkonly)
    {
      ଯଦି (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ଯଦି (outfile)
        {
          ସ୍ଥିର ଅକ୍ଷର opts[] = {0, 'o', 0};
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

  ଯଦି (mergeonly)
    {
      ସଂରଚନା sortfile *sortfiles = xcalloc (nfiles, ଆକାର *sortfiles);

      ପାଇଁ (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (!nthreads)
        {
          unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା np = ଅଙ୍କ_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * ଆକାର (ସଂରଚନା merge_ଗଣ୍ଠି));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  ଯଦି (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
