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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <pthread.h>
#अन्तर्भाव <sys/resource.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <spawn.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fadvise.h"
#अन्तर्भाव "filevercmp.h"
#अन्तर्भाव "findprog.h"
#अन्तर्भाव "flexmember.h"
#अन्तर्भाव "hard-locale.h"
#अन्तर्भाव "hash.h"
#अन्तर्भाव "heap.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "mbswidth.h"
#अन्तर्भाव "nproc.h"
#अन्तर्भाव "physmem.h"
#अन्तर्भाव "posixver.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "randread.h"
#अन्तर्भाव "readtokens0.h"
#अन्तर्भाव "stdlib--.h"
#अन्तर्भाव "strnumcmp.h"
#अन्तर्भाव "term-sig.h"
#अन्तर्भाव "unistd--.h"
#अन्तर्भाव "xmemcoll.h"
#अन्तर्भाव "xnanosleep.h"
#अन्तर्भाव "xstrtol.h"
#अन्तर्भाव "xstrtol-error.h"

#ifndef RLIMIT_DATA
संरचना rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#सचे HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#सचे !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#सचे !defined OPEN_MAX
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
गणना { SUBTHREAD_पङ्क्तयः_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_पङ्क्तयः_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
गणना { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
गणना
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

गणना
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

गणना
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_मूलम् = 1
  };

/* The representation of the decimal point in the current locale.  */
स्थिर वर्ण decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
स्थिर पूर्णाङ्क thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
स्थिर bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
स्थिर bool hard_LC_COLLATE;
#सचे HAVE_NL_LANGINFO
स्थिर bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
गणना blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

/* Lines are held in memory as counted strings. */
संरचना पङ्क्तिः
{
  वर्ण *text;			/* Text of the line. */
  size_t length;		/* Length including final newline. */
  वर्ण *keybeg;			/* Start of first key. */
  वर्ण *keylim;			/* Limit of first key. */
};

/* Input buffers. */
संरचना buffer
{
  वर्ण *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t used;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t वामः;			/* Number of bytes left from previous reads. */
  size_t पङ्क्तिः_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
संरचना sort_buffer_policy
{
  size_t initial;
  size_t limit;
  bool growth_failed;
};

/* Sort key.  */
संरचना keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ध्रुव *ignore;		/* Boolean array of characters to ignore. */
  वर्ण ध्रुव *translate;	/* Translation applied to characters. */
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
  संरचना keyfield *next;	/* Next keyfield to try. */
};

संरचना month
{
  वर्ण ध्रुव *name;
  पूर्णाङ्क val;
};

/* Binary merge tree node. */
संरचना merge_ग्रन्थिः
{
  संरचना पङ्क्तिः *lo;              /* Lines to merge from LO child node. */
  संरचना पङ्क्तिः *hi;              /* Lines to merge from HI child node. */
  संरचना पङ्क्तिः *end_lo;          /* End of available lines from LO. */
  संरचना पङ्क्तिः *end_hi;          /* End of available lines from HI. */
  संरचना पङ्क्तिः **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  संरचना merge_ग्रन्थिः *parent;    /* Parent node. */
  संरचना merge_ग्रन्थिः *lo_child;  /* LO child node. */
  संरचना merge_ग्रन्थिः *hi_child;  /* HI child node. */
  unsigned पूर्णाङ्क level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
संरचना merge_ग्रन्थिः_queue
{
  संरचना heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
स्थिर संरचना पङ्क्तिः saved_पङ्क्तिः;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
स्थिर bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
स्थिर bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
स्थिर bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
स्थिर वर्ण fold_toupper[UCHAR_LIM];

#define MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
स्थिर संरचना month monthtab[] =
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
#define MIN_MERGE_BUFFER_SIZE (2 + परिमाण (संरचना पङ्क्तिः))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
स्थिर size_t merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
स्थिर size_t sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define निवेशः_FILE_SIZE_GUESS (128 * 1024)

स्थिर संरचना
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  वर्ण ध्रुव **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
स्थिर bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
स्थिर bool stable;

/* An int value outside char range.  */
गणना { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
गणना { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
स्थिर पूर्णाङ्क tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
स्थिर bool unique;

/* Nonzero if any of the input files are the standard input. */
स्थिर bool have_read_stdin;

/* List of key field comparisons to be tried.  */
स्थिर संरचना keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
स्थिर वर्ण ध्रुव *compress_program;

/* Annotate the output with extra info to aid the user.  */
स्थिर bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
स्थिर unsigned पूर्णाङ्क nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
स्थिर bool default_SIGPIPE;

/* The list of temporary files. */
संरचना tempnode
{
  संरचना tempnode *चञ्चल next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  वर्ण state;
  वर्ण name[FLEXIBLE_ARRAY_MEMBER];
};
स्थिर संरचना tempnode *चञ्चल temphead;
स्थिर संरचना tempnode *चञ्चल *temptail = &temphead;

/* Clean up any remaining temporary files.  */

स्थिर शून्य
cleanup (शून्य)
{
  अत्थाय (संरचना tempnode ध्रुव *ग्रन्थिः = temphead; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->next)
    unlink (ग्रन्थिः->name);
  temphead = रिक्त;
}

/* Handle interrupts and hangups. */

स्थिर शून्य
sighandler (पूर्णाङ्क sig)
{
  cleanup ();

  सङ्केत (sig, SIG_DFL);
  उत्थापय (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

स्थिर शून्य
sort_die (वर्ण ध्रुव *message, वर्ण ध्रुव *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  सचे (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, errno, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
         ignore leading blanks when finding sort कुञ्चिकाः in each पङ्क्तिः\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-पक्ष\n\
         fold lower पक्ष to upper पक्ष characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical मानम्\n\
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
         compare according to string numerical मानम्;\n\
         see full documentation अत्थाय supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical कुञ्चिकाः.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; अत्थाय more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check अत्थाय sorted निवेशः; करोहि not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but करोहि not report first bad पङ्क्तिः\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the पङ्क्तिः used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ निवेशः from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, पठ names from standard निवेशः\n\
"));
      oputs (_("\
  -k, --कुञ्चिका=KEYDEF\n\
         sort via a कुञ्चिका; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; करोहि not sort\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         लिख result to FILE instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE अत्थाय main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR अत्थाय temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the सङ्ख्या of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of पङ्क्तयः with equal कुञ्चिकाः;\n\
         with -c, check अत्थाय strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] अत्थाय start and stop position, where F is a\n\
field सङ्ख्या and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the पङ्क्तिः's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options अत्थाय that कुञ्चिका.  If no कुञ्चिका is given, use\n\
the entire पङ्क्तिः as the कुञ्चिका.  Use --debug to diagnose incorrect कुञ्चिका usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% of memory, b 1, K 1024 (स्वभाव), and so on अत्थाय M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte मानानि.\n\
"), stdout );
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
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

स्थिर वर्ण ध्रुव short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"ignore-leading-blanks", no_argument, रिक्त, 'b'},
  {"check", optional_argument, रिक्त, CHECK_OPTION},
  {"compress-program", required_argument, रिक्त, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, रिक्त, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, रिक्त, 'd'},
  {"ignore-case", no_argument, रिक्त, 'f'},
  {"files0-from", required_argument, रिक्त, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, रिक्त, 'g'},
  {"ignore-nonprinting", no_argument, रिक्त, 'i'},
  {"key", required_argument, रिक्त, 'k'},
  {"merge", no_argument, रिक्त, 'm'},
  {"month-sort", no_argument, रिक्त, 'M'},
  {"numeric-sort", no_argument, रिक्त, 'n'},
  {"human-numeric-sort", no_argument, रिक्त, 'h'},
  {"version-sort", no_argument, रिक्त, 'V'},
  {"random-sort", no_argument, रिक्त, 'R'},
  {"random-source", required_argument, रिक्त, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, रिक्त, SORT_OPTION},
  {"output", required_argument, रिक्त, 'o'},
  {"reverse", no_argument, रिक्त, 'r'},
  {"stable", no_argument, रिक्त, 's'},
  {"batch-size", required_argument, रिक्त, NMERGE_OPTION},
  {"buffer-size", required_argument, रिक्त, 'S'},
  {"field-separator", required_argument, रिक्त, 't'},
  {"temporary-directory", required_argument, रिक्त, 'T'},
  {"unique", no_argument, रिक्त, 'u'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {"parallel", required_argument, रिक्त, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

#define CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

स्थिर वर्ण ध्रुव *ध्रुव check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE रिक्त
#undef  _ct_
};
स्थिर वर्ण ध्रुव check_types[] =
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

स्थिर वर्ण ध्रुव *ध्रुव sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE रिक्त
#undef  _st_
};
स्थिर वर्ण ध्रुव sort_types[] =
{
#define _st_(_s, _c) _c,
  SORT_TABLE
#undef  _st_
};

/* The set of signals that are caught.  */
स्थिर sigset_t caught_signals;

/* Critical section status.  */
संरचना cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
स्थिर शून्य
cs_enter (संरचना cs_status *status)
{
  पूर्णाङ्क ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
स्थिर शून्य
cs_leave (संरचना cs_status ध्रुव *status)
{
  सचे (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, रिक्त);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
गणना { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
संरचना sortfile
{
  /* The file's name.  */
  वर्ण ध्रुव *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  संरचना tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
स्थिर Hash_table *proctab;

गणना { INIT_PROCTAB_SIZE = 47 };

स्थिर size_t
proctab_hasher (शून्य ध्रुव *entry, size_t tabsize)
{
  संरचना tempnode ध्रुव *ग्रन्थिः = entry;
  पटिनिवत्त ग्रन्थिः->pid % tabsize;
}

स्थिर bool
proctab_comparator (शून्य ध्रुव *e1, शून्य ध्रुव *e2)
{
  संरचना tempnode ध्रुव *n1 = e1;
  संरचना tempnode ध्रुव *n2 = e2;
  पटिनिवत्त n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
स्थिर pid_t nprocs;

स्थिर bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

स्थिर pid_t
reap (pid_t pid)
{
  पूर्णाङ्क status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  सचे (cpid < 0)
    error (SORT_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  अञ्ञथा सचे (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      सचे (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  पटिनिवत्त cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

स्थिर शून्य
register_proc (संरचना tempnode *temp)
{
  सचे (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, रिक्त,
                                 proctab_hasher,
                                 proctab_comparator,
                                 रिक्त);
      सचे (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  सचे (! hash_निवेशय (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

स्थिर bool
delete_proc (pid_t pid)
{
  संरचना tempnode test;

  test.pid = pid;
  संरचना tempnode *ग्रन्थिः = hash_remove (proctab, &test);
  सचे (! ग्रन्थिः)
    पटिनिवत्त false;
  ग्रन्थिः->state = REAPED;
  पटिनिवत्त true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

स्थिर शून्य
wait_proc (pid_t pid)
{
  सचे (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

स्थिर शून्य
reap_exited (शून्य)
{
  याव (0 < nprocs && reap (0))
    अनुवर्त;
}

/* Reap at least one exited child, waiting if necessary.  */

स्थिर शून्य
reap_some (शून्य)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

स्थिर शून्य
reap_all (शून्य)
{
  याव (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

स्थिर शून्य
exit_cleanup (शून्य)
{
  सचे (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      संरचना cs_status cs;
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

स्थिर संरचना tempnode *
create_temp_file (पूर्णाङ्क *pfd, bool survive_fd_exhaustion)
{
  स्थिर वर्ण ध्रुव slashbase[] = "/sortXXXXXX";
  स्थिर idx_t temp_dir_index;
  पूर्णाङ्क fd;
  पूर्णाङ्क saved_errno;
  वर्ण ध्रुव *temp_dir = temp_dirs.dirs[temp_dir_index];
  size_t len = strlen (temp_dir);
  संरचना tempnode *ग्रन्थिः =
    xmalloc (FLEXSIZEOF (संरचना tempnode, name, len + परिमाण slashbase));
  वर्ण *file = ग्रन्थिः->name;
  संरचना cs_status cs;

  memcpy (file, temp_dir, len);
  memcpy (file + len, slashbase, परिमाण slashbase);
  ग्रन्थिः->next = रिक्त;
  सचे (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  सचे (0 <= fd)
    {
      *temptail = ग्रन्थिः;
      temptail = &ग्रन्थिः->next;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  सचे (fd < 0)
    {
      सचे (! (survive_fd_exhaustion && errno == EMFILE))
        error (SORT_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      मुक्त (ग्रन्थिः);
      ग्रन्थिः = रिक्त;
    }

  *pfd = fd;
  पटिनिवत्त ग्रन्थिः;
}

/* Return a pointer to stdout status, or NULL on failure.  */

स्थिर संरचना stat *
get_outstatus (शून्य)
{
  स्थिर पूर्णाङ्क outstat_errno;
  स्थिर संरचना stat outstat;
  सचे (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  पटिनिवत्त outstat_errno < 0 ? &outstat : रिक्त;
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

स्थिर FILE *
stream_open (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  FILE *fp;

  सचे (*how == 'r')
    {
      सचे (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      अञ्ञथा
        {
          पूर्णाङ्क fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? रिक्त : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  अञ्ञथा सचे (*how == 'w')
    {
      सचे (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          पूर्णाङ्क ftruncate_errno = errno;
          संरचना stat *outst = get_outstatus ();
          सचे (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  अञ्ञथा
    affirm (!"unexpected mode passed to stream_open");

  पटिनिवत्त fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

स्थिर FILE *
xfopen (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  FILE *fp = stream_open (file, how);
  सचे (!fp)
    sort_die (_("open failed"), file);
  पटिनिवत्त fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

स्थिर शून्य
xfclose (FILE *fp, वर्ण ध्रुव *file)
{
  विकल्प (fileno (fp))
    {
    पक्ष STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      भङ्ग;

    पक्ष STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      सचे (fflush (fp) != 0)
        sort_die (_("fflush failed"), file);
      भङ्ग;

    स्वभाव:
      सचे (fclose (fp) != 0)
        sort_die (_("close failed"), file);
      भङ्ग;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

स्थिर शून्य
move_fd (पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
  सचे (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_मानम् (dup2 (oldfd, newfd));
      ignore_मानम् (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

स्थिर पूर्णाङ्क
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
  पूर्णाङ्क result = 0;
  सचे (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      सचे (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  पटिनिवत्त result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

स्थिर वर्ण ध्रुव *
get_resolved_compress_program (शून्य)
{
  /* Use a cache, to perform the search only once.  */
  स्थिर वर्ण ध्रुव *resolved_compress_program_cache /* = NULL */;

  सचे (resolved_compress_program_cache == रिक्त)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, getenv ("PATH"), रिक्त, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  पटिनिवत्त resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

स्थिर पूर्णाङ्क
pipe_child (pid_t *pid, पूर्णाङ्क pipefds[2], पूर्णाङ्क tempfd, bool decompress,
            size_t tries)
{
  वर्ण ध्रुव *resolved_compress_program;
  संरचना tempnode *saved_temphead;
  द्विगुण wait_retry = 0.25;
  संरचना cs_status cs;
  पूर्णाङ्क result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  सचे (resolved_compress_program == रिक्त)
    पटिनिवत्त errno;

  सचे ((result = posix_spawnattr_init (&attr)))
    पटिनिवत्त result;
  सचे ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      पटिनिवत्त result;
    }

  सचे (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      पूर्णाङ्क saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      पटिनिवत्त saved_errno;
    }

  सचे ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      पटिनिवत्त result;
    }

  वर्ण ध्रुव *ध्रुव argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : रिक्त,
      रिक्त
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  सचे (nmerge + 1 < nprocs)
    reap_some ();

  याव (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = रिक्त;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (वर्ण * ध्रुव *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      सचे (result != EAGAIN)
        भङ्ग;
      अञ्ञथा
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

  सचे (result)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  अञ्ञथा
    ++nprocs;

  पटिनिवत्त result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

स्थिर संरचना tempnode *
maybe_create_temp (FILE **pfp, bool survive_fd_exhaustion)
{
  पूर्णाङ्क tempfd;
  संरचना tempnode *ग्रन्थिः = create_temp_file (&tempfd, survive_fd_exhaustion);
  सचे (! ग्रन्थिः)
    पटिनिवत्त रिक्त;

  ग्रन्थिः->state = UNCOMPRESSED;

  सचे (compress_program)
    {
      पूर्णाङ्क pipefds[2];
      स्थिर पूर्णाङ्क last_result = 0;

      पूर्णाङ्क result = pipe_child (&ग्रन्थिः->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      सचे (result)
        {
          सचे (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      अञ्ञथा
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (ग्रन्थिः);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  सचे (! *pfp)
    sort_die (_("couldn't create temporary file"), ग्रन्थिः->name);

  पटिनिवत्त ग्रन्थिः;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

स्थिर संरचना tempnode *
create_temp (FILE **pfp)
{
  पटिनिवत्त maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

स्थिर FILE *
open_temp (संरचना tempnode *temp)
{
  पूर्णाङ्क tempfd, pipefds[2];
  FILE *fp = रिक्त;

  सचे (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = open (temp->name, O_RDONLY);
  सचे (tempfd < 0)
    पटिनिवत्त रिक्त;

  pid_t child;
  पूर्णाङ्क result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  सचे (result)
    {
      सचे (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      close (tempfd);
      errno = EMFILE;
    }
  अञ्ञथा
    {
      temp->pid = child;
      register_proc (temp);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      सचे (! fp)
        {
          पूर्णाङ्क saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  पटिनिवत्त fp;
}

/* Append DIR to the array of temporary directory names.  */
स्थिर शून्य
add_temp_dir (वर्ण ध्रुव *dir)
{
  सचे (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              परिमाण *temp_dirs.dirs);

  idx_t ध्रुव dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

स्थिर शून्य
zaptemp (वर्ण ध्रुव *name)
{
  संरचना tempnode *चञ्चल *pnode;
  संरचना tempnode *ग्रन्थिः;
  संरचना tempnode *next;
  पूर्णाङ्क unlink_status;
  पूर्णाङ्क unlink_errno = 0;
  संरचना cs_status cs;

  अत्थाय (pnode = &temphead; (ग्रन्थिः = *pnode)->name != name; pnode = &ग्रन्थिः->next)
    अनुवर्त;

  सचे (ग्रन्थिः->state == UNREAPED)
    wait_proc (ग्रन्थिः->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = ग्रन्थिः->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = errno;
  *pnode = next;
  cs_leave (&cs);

  सचे (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  सचे (! next)
    temptail = pnode;
  मुक्त (ग्रन्थिः);
}

#सचे HAVE_NL_LANGINFO

स्थिर पूर्णाङ्क
struct_month_cmp (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  संरचना month ध्रुव *month1 = m1;
  संरचना month ध्रुव *month2 = m2;
  पटिनिवत्त strcmp (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

स्थिर शून्य
inittables (शून्य)
{
  अत्थाय (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! isalnum (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = toupper (i);
    }

#सचे HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  सचे (hard_LC_TIME)
    {
      अत्थाय (size_t i = 0; i < MONTHS_PER_YEAR; i++)
        {
          वर्ण ध्रुव *s;
          size_t s_len;
          size_t j, k;
          वर्ण *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = strlen (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          अत्थाय (j = k = 0; j < s_len; j++)
            सचे (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_PER_YEAR, परिमाण *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
स्थिर शून्य
specify_nmerge (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t n;
  संरचना rlimit rlimit;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  unsigned पूर्णाङ्क max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  सचे (e == LONGINT_OK)
    {
      nmerge = n;
      सचे (nmerge != n)
        e = LONGINT_OVERFLOW;
      अञ्ञथा
        {
          सचे (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          अञ्ञथा सचे (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          अञ्ञथा
            पटिनिवत्त;
        }
    }

  सचे (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  अञ्ञथा
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
स्थिर शून्य
specify_sort_size (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t n;
  वर्ण *suffix;
  गणना strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  सचे (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  सचे (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    विकल्प (suffix[0])
      {
      पक्ष 'b':
        e = LONGINT_OK;
        भङ्ग;

      पक्ष '%':
        {
          द्विगुण mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          सचे (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          अञ्ञथा
            e = LONGINT_OVERFLOW;
        }
        भङ्ग;
      }

  सचे (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      सचे (n < sort_size)
        पटिनिवत्त;

      sort_size = n;
      सचे (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          पटिनिवत्त;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
स्थिर size_t
specify_nthreads (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t nthreads;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &nthreads, "");
  सचे (e == LONGINT_OVERFLOW)
    पटिनिवत्त SIZE_MAX;
  सचे (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  सचे (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  सचे (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  पटिनिवत्त nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
स्थिर size_t
default_sort_size (शून्य)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t size = SIZE_MAX;
  संरचना rlimit rlimit;
  सचे (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  सचे (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  सचे (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  द्विगुण avail = physmem_available ();
  द्विगुण total = physmem_total ();
  द्विगुण mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  सचे (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  सचे (mem < size)
    size = mem;
  पटिनिवत्त MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

स्थिर bool
पङ्क्तिः_aligned_size (size_t *alloc)
{
  size_t size = *alloc;
  size_t alignment = परिमाण (संरचना पङ्क्तिः);
  size_t padding = alignment - size % alignment;
  size_t aligned;

  सचे (ckd_add (&aligned, size, padding))
    पटिनिवत्त false;

  *alloc = aligned;
  पटिनिवत्त true;
}

/* Return the default initial allocation for a growable sort buffer.  */

स्थिर size_t
default_initial_sort_size (size_t पङ्क्तिः_bytes)
{
  size_t size = पङ्क्तिः_bytes + 2;
  size_t निवेशः_size;

  सचे (ckd_mul (&निवेशः_size, निवेशः_FILE_SIZE_GUESS, पङ्क्तिः_bytes + 1)
      || ckd_add (&size, size, निवेशः_size))
    पटिनिवत्त SIZE_MAX;

  पटिनिवत्त MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

स्थिर bool
निवेशः_size_buffer_bytes (uintmax_t file_size, size_t पङ्क्तिः_bytes,
                         size_t *worst_case)
{
  size_t worst_case_per_निवेशः_byte = पङ्क्तिः_bytes + 1;

  सचे (SIZE_MAX / worst_case_per_निवेशः_byte < file_size)
    पटिनिवत्त false;

  size_t size = file_size * worst_case_per_निवेशः_byte;
  सचे (SIZE_MAX - size < 1)
    पटिनिवत्त false;

  *worst_case = size + 1;
  पटिनिवत्त true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

स्थिर शून्य
sort_buffer_policy (FILE *ध्रुव *fps, size_t nfps,
                    वर्ण *ध्रुव *files, size_t nfiles,
                    size_t पङ्क्तिः_bytes,
                    संरचना sort_buffer_policy *policy)
{
  size_t size_bound = sort_size ? sort_size : default_sort_size ();
  size_t initial_bound =
    sort_size ? size_bound : default_initial_sort_size (पङ्क्तिः_bytes);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_per_निवेशः_byte = पङ्क्तिः_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t size = worst_case_per_निवेशः_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  अत्थाय (size_t i = 0; i < nfiles; i++)
    {
      संरचना stat st;
      off_t file_size;
      size_t worst_case;
      bool known_size;

      सचे (size_bound <= size)
        {
          policy->initial = size_bound;
          पटिनिवत्त;
        }

      सचे ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      सचे (known_size)
        file_size = st.st_size;
      अञ्ञथा
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          सचे (sort_size)
            {
              policy->initial = size_bound;
              पटिनिवत्त;
            }
          file_size = निवेशः_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      सचे (! निवेशः_size_buffer_bytes (file_size, पङ्क्तिः_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          पटिनिवत्त;
        }

      सचे (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          पटिनिवत्त;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

स्थिर शून्य
initbuf (संरचना buffer *buf, size_t पङ्क्तिः_bytes, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  याव (true)
    {
      सचे (! पङ्क्तिः_aligned_size (&alloc))
        buf->buf = रिक्त;
      अञ्ञथा
        buf->buf = malloc (alloc);
      सचे (buf->buf)
        भङ्ग;
      alloc /= 2;
      सचे (alloc <= पङ्क्तिः_bytes + 1)
        xalloc_die ();
    }

  buf->पङ्क्तिः_bytes = पङ्क्तिः_bytes;
  buf->alloc = alloc;
  buf->used = buf->वामः = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

स्थिर अन्तःस्थ संरचना पङ्क्तिः *
buffer_linelim (संरचना buffer ध्रुव *buf)
{
  शून्य *linelim = buf->buf + buf->alloc;
  पटिनिवत्त linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

स्थिर bool
try_growbuf (संरचना buffer *buf, size_t alloc)
{
  सचे (! पङ्क्तिः_aligned_size (&alloc) || alloc <= buf->alloc)
    पटिनिवत्त false;

  वर्ण *newbuf = malloc (alloc);
  सचे (! newbuf)
    पटिनिवत्त false;

  वर्ण *oldbuf = buf->buf;
  संरचना पङ्क्तिः *old_linelim = buffer_linelim (buf);
  संरचना पङ्क्तिः *old_पङ्क्तिः = old_linelim - buf->nlines;

  memcpy (newbuf, oldbuf, buf->used);

  संरचना पङ्क्तिः *new_linelim = (शून्य *) (newbuf + alloc);
  संरचना पङ्क्तिः *new_पङ्क्तिः = new_linelim - buf->nlines;
  memcpy (new_पङ्क्तिः, old_पङ्क्तिः, buf->nlines * परिमाण *new_पङ्क्तिः);

  अत्थाय (संरचना पङ्क्तिः *पङ्क्तिः = new_पङ्क्तिः; पङ्क्तिः < new_linelim; पङ्क्तिः++)
    {
      पङ्क्तिः->text = newbuf + (पङ्क्तिः->text - oldbuf);
      सचे (keylist)
        {
          पङ्क्तिः->keybeg = newbuf + (पङ्क्तिः->keybeg - oldbuf);
          पङ्क्तिः->keylim = newbuf + (पङ्क्तिः->keylim - oldbuf);
        }
    }

  मुक्त (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  पटिनिवत्त true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

स्थिर bool
maybe_growbuf (संरचना buffer *buf, संरचना sort_buffer_policy *policy)
{
  सचे (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    पटिनिवत्त false;

  size_t alloc;
  सचे (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  अञ्ञथा
    alloc = policy->limit;

  सचे (try_growbuf (buf, alloc))
    पटिनिवत्त true;

  policy->growth_failed = true;
  पटिनिवत्त false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

स्थिर वर्ण *
begfield (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *ptr = पङ्क्तिः->text, *lim = ptr + पङ्क्तिः->length - 1;
  size_t sword = कुञ्चिका->sword;
  size_t schar = कुञ्चिका->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  सचे (tab != TAB_DEFAULT)
    याव (ptr < lim && sword--)
      {
        वर्ण *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        सचे (ptr < lim)
          ++ptr;
      }
  अञ्ञथा
    याव (ptr < lim && sword--)
      {
        याव (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        याव (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  सचे (कुञ्चिका->skipsblanks)
    याव (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_bytes = lim - ptr;
  सचे (schar < remaining_bytes)
    ptr += schar;
  अञ्ञथा
    ptr = lim;

  पटिनिवत्त ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
स्थिर वर्ण *
limfield (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *ptr = पङ्क्तिः->text, *lim = ptr + पङ्क्तिः->length - 1;
  size_t eword = कुञ्चिका->eword, echar = कुञ्चिका->echar;

  सचे (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  सचे (tab != TAB_DEFAULT)
    याव (ptr < lim && eword--)
      {
        वर्ण *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        सचे (ptr < lim && (eword || echar))
          ++ptr;
      }
  अञ्ञथा
    याव (ptr < lim && eword--)
      {
        याव (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        याव (ptr < lim && !blanks[to_uchar (*ptr)])
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
  सचे (tab != TAB_DEFAULT)
    {
      वर्ण *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      सचे (newlim)
        lim = newlim;
    }
  अञ्ञथा
    {
      वर्ण *newlim;
      newlim = ptr;
      याव (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      याव (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  सचे (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      सचे (कुञ्चिका->skipeblanks)
        याव (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_bytes = lim - ptr;
      सचे (echar < remaining_bytes)
        ptr += echar;
      अञ्ञथा
        ptr = lim;
    }

  पटिनिवत्त ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

स्थिर bool
fillbuf (संरचना buffer *buf, FILE *fp, वर्ण ध्रुव *file,
         संरचना sort_buffer_policy *policy)
{
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  वर्ण eol = eolchar;
  size_t पङ्क्तिः_bytes = buf->पङ्क्तिः_bytes;
  size_t mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  सचे (buf->eof)
    पटिनिवत्त false;

  सचे (buf->used != buf->वामः)
    {
      memmove (buf->buf, buf->buf + buf->used - buf->वामः, buf->वामः);
      buf->used = buf->वामः;
      buf->nlines = 0;
    }

  याव (true)
    {
      वर्ण *ptr = buf->buf + buf->used;
      संरचना पङ्क्तिः *linelim = buffer_linelim (buf);
      संरचना पङ्क्तिः *पङ्क्तिः = linelim - buf->nlines;
      size_t avail = (वर्ण *) linelim - buf->nlines * पङ्क्तिः_bytes - ptr;
      वर्ण *पङ्क्तिः_start = buf->nlines ? पङ्क्तिः->text + पङ्क्तिः->length : buf->buf;

      याव (पङ्क्तिः_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (पङ्क्तिः_bytes + 1);
          size_t bytes_read = fread (ptr, 1, readsize, fp);
          वर्ण *ptrlim = ptr + bytes_read;
          वर्ण *p;
          avail -= bytes_read;

          सचे (bytes_read != readsize)
            {
              सचे (ferror (fp))
                sort_die (_("read failed"), file);
              सचे (feof (fp))
                {
                  buf->eof = true;
                  सचे (buf->buf == ptrlim)
                    पटिनिवत्त false;
                  सचे (पङ्क्तिः_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          याव ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              पङ्क्तिः--;
              पङ्क्तिः->text = पङ्क्तिः_start;
              पङ्क्तिः->length = ptr - पङ्क्तिः_start;
              mergesize = MAX (mergesize, पङ्क्तिः->length);
              avail -= पङ्क्तिः_bytes;

              सचे (कुञ्चिका)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  पङ्क्तिः->keylim = (कुञ्चिका->eword == SIZE_MAX
                                  ? p
                                  : limfield (पङ्क्तिः, कुञ्चिका));

                  सचे (कुञ्चिका->sword != SIZE_MAX)
                    पङ्क्तिः->keybeg = begfield (पङ्क्तिः, कुञ्चिका);
                  अञ्ञथा
                    {
                      सचे (कुञ्चिका->skipsblanks)
                        याव (blanks[to_uchar (*पङ्क्तिः_start)])
                          पङ्क्तिः_start++;
                      पङ्क्तिः->keybeg = पङ्क्तिः_start;
                    }
                }

              पङ्क्तिः_start = ptr;
            }

          ptr = ptrlim;
          सचे (buf->eof)
            भङ्ग;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - पङ्क्तिः;
      सचे (buf->nlines != 0)
        {
          सचे (! buf->eof && maybe_growbuf (buf, policy))
            अनुवर्त;
          buf->वामः = ptr - पङ्क्तिः_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          पटिनिवत्त true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t पङ्क्तिः_alloc = buf->alloc / परिमाण (संरचना पङ्क्तिः);
        buf->buf = xpalloc (buf->buf, &पङ्क्तिः_alloc, 1, -1, परिमाण (संरचना पङ्क्तिः));
        buf->alloc = पङ्क्तिः_alloc * परिमाण (संरचना पङ्क्तिः);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
स्थिर वर्ण ध्रुव unit_order[UCHAR_LIM] =
  {
#सचे ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#अञ्ञथा
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
स्थिर वर्ण
traverse_raw_सङ्ख्या (वर्ण ध्रुव **सङ्ख्या)
{
  वर्ण ध्रुव *p = *सङ्ख्या;
  वर्ण ch;
  वर्ण max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  याव (c_isdigit (ch = *p++))
    {
      सचे (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      सचे (ends_with_thousands_sep)
        ++p;
    }

  सचे (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *सङ्ख्या = p - 2;
      पटिनिवत्त max_digit;
    }

  सचे (ch == decimal_point)
    याव (c_isdigit (ch = *p++))
      सचे (max_digit < ch)
        max_digit = ch;

  *सङ्ख्या = p - 1;
  पटिनिवत्त max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
find_unit_order (वर्ण ध्रुव *सङ्ख्या)
{
  bool minus_sign = (*सङ्ख्या == '-');
  वर्ण ध्रुव *p = सङ्ख्या + minus_sign;
  वर्ण max_digit = traverse_raw_सङ्ख्या (&p);
  सचे ('0' < max_digit)
    {
      unsigned वर्ण ch = *p;
      पूर्णाङ्क order = unit_order[ch];
      पटिनिवत्त (minus_sign ? -order : order);
    }
  अञ्ञथा
    पटिनिवत्त 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
human_numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  याव (blanks[to_uchar (*a)])
    a++;
  याव (blanks[to_uchar (*b)])
    b++;

  पूर्णाङ्क diff = find_unit_order (a) - find_unit_order (b);
  पटिनिवत्त (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  याव (blanks[to_uchar (*a)])
    a++;
  याव (blanks[to_uchar (*b)])
    b++;

  पटिनिवत्त strnumcmp (a, b, decimal_point, thousands_sep);
}

स्थिर पूर्णाङ्क
nan_compare (long द्विगुण a, long द्विगुण b)
{
  वर्ण buf[2][परिमाण "-nan""()" + CHAR_BIT * परिमाण a];
  snprintf (buf[0], परिमाण buf[0], "%Lf", a);
  snprintf (buf[1], परिमाण buf[1], "%Lf", b);
  पटिनिवत्त strcmp (buf[0], buf[1]);
}

स्थिर पूर्णाङ्क
general_numcompare (वर्ण ध्रुव *sa, वर्ण ध्रुव *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  वर्ण *ea;
  वर्ण *eb;
  long द्विगुण a = strtold (sa, &ea);
  long द्विगुण b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  सचे (sa == ea)
    पटिनिवत्त sb == eb ? 0 : -1;
  सचे (sb == eb)
    पटिनिवत्त 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  पटिनिवत्त (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

स्थिर पूर्णाङ्क
getmonth (वर्ण ध्रुव *month, वर्ण **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_PER_YEAR;

  याव (blanks[to_uchar (*month)])
    month++;

  करोहि
    {
      size_t ix = (lo + hi) / 2;
      वर्ण ध्रुव *m = month;
      वर्ण ध्रुव *n = monthtab[ix].name;

      अत्थाय (;; m++, n++)
        {
          सचे (!*n)
            {
              सचे (ea)
                *ea = (वर्ण *) m;
              पटिनिवत्त monthtab[ix].val;
            }
          सचे (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              भङ्ग;
            }
          अञ्ञथा सचे (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              भङ्ग;
            }
        }
    }
  याव (lo < hi);

  पटिनिवत्त 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#सचे DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# सचे 14 <= __GNUC__
#  pragma GCC diagnostic प्रक्षिप
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#अन्तर्भाव "md5.h"

#सचे DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# सचे 14 <= __GNUC__
#  pragma GCC diagnostic निष्कासय
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
स्थिर शून्य
link_failure (शून्य)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
स्थिर शून्य *
चिह्नम्_address (शून्य *handle, वर्ण ध्रुव *चिह्नम्)
{
  शून्य *address = dlsym (handle, चिह्नम्);
  सचे (!address)
    link_failure ();
  पटिनिवत्त address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
स्थिर शून्य
link_libcrypto (शून्य)
{
#सचे DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  शून्य *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  सचे (!handle)
    link_failure ();
  ptr_MD5_Init = चिह्नम्_address (handle, "MD5_Init");
  ptr_MD5_Update = चिह्नम्_address (handle, "MD5_Update");
  ptr_MD5_Final = चिह्नम्_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
स्थिर संरचना md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

स्थिर शून्य
random_md5_state_init (वर्ण ध्रुव *random_source)
{
  unsigned वर्ण buf[MD5_DIGEST_SIZE];
  संरचना randread_source *r = randread_new (random_source, परिमाण buf);
  सचे (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, परिमाण buf);
  सचे (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, परिमाण buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

स्थिर size_t
xstrxfrm (वर्ण *restrict dest, वर्ण ध्रुव *restrict src, size_t destsize)
{
  errno = 0;
  size_t translated_size = strxfrm (dest, src, destsize);

  सचे (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  पटिनिवत्त translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

स्थिर पूर्णाङ्क
compare_random (वर्ण *restrict texta, size_t lena,
                वर्ण *restrict textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  पूर्णाङ्क xfrm_diff = 0;

  वर्ण stackbuf[4000];
  वर्ण *buf = stackbuf;
  size_t bufsize = परिमाण stackbuf;
  शून्य *allocated = रिक्त;
  uint32_t dig[2][MD5_DIGEST_SIZE / परिमाण (uint32_t)];
  संरचना md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  सचे (hard_LC_COLLATE)
    {
      वर्ण ध्रुव *lima = texta + lena;
      वर्ण ध्रुव *limb = textb + lenb;

      याव (true)
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
          सचे (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              मुक्त (allocated);
              buf = allocated = malloc (bufsize);
              सचे (! buf)
                {
                  buf = stackbuf;
                  bufsize = परिमाण stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : रिक्त), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          सचे (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              सचे (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              मुक्त (allocated);
              buf = allocated = xmalloc (bufsize);
              सचे (texta < lima)
                strxfrm (buf, texta, sizea);
              सचे (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          सचे (texta < lima)
            texta += strlen (texta) + 1;
          सचे (textb < limb)
            textb += strlen (textb) + 1;
          सचे (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              भङ्ग;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          सचे (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              सचे (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  पूर्णाङ्क diff = memcmp (dig[0], dig[1], परिमाण dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  सचे (! diff)
    {
      सचे (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          सचे (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  मुक्त (allocated);

  पटिनिवत्त diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

स्थिर size_t
debug_width (वर्ण ध्रुव *text, वर्ण ध्रुव *lim)
{
  size_t width = mbsnwidth (text, lim - text, 0);
  याव (text < lim)
    width += (*text++ == '\t');
  पटिनिवत्त width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

स्थिर शून्य
mark_कुञ्चिका (size_t offset, size_t width)
{
  याव (offset--)
    putchar (' ');

  सचे (!width)
    printf (_("^ no match for key\n"));
  अञ्ञथा
    {
      करोहि
        putchar ('_');
      याव (--width);

      putchar ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

स्थिर अन्तःस्थ bool
कुञ्चिका_numeric (संरचना keyfield ध्रुव *कुञ्चिका)
{
  पटिनिवत्त कुञ्चिका->numeric || कुञ्चिका->general_numeric || कुञ्चिका->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

स्थिर शून्य
debug_कुञ्चिका (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *text = पङ्क्तिः->text;
  वर्ण *beg = text;
  वर्ण *lim = text + पङ्क्तिः->length - 1;

  सचे (कुञ्चिका)
    {
      सचे (कुञ्चिका->sword != SIZE_MAX)
        beg = begfield (पङ्क्तिः, कुञ्चिका);
      सचे (कुञ्चिका->eword != SIZE_MAX)
        {
          lim = limfield (पङ्क्तिः, कुञ्चिका);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      सचे ((कुञ्चिका->skipsblanks && कुञ्चिका->sword == SIZE_MAX)
          || कुञ्चिका->month || कुञ्चिका_numeric (कुञ्चिका))
        {
          वर्ण saved = *lim;
          *lim = '\0';

          याव (blanks[to_uchar (*beg)])
            beg++;

          वर्ण *tighter_lim = beg;

          सचे (lim < beg)
            tighter_lim = lim;
          अञ्ञथा सचे (कुञ्चिका->month)
            getmonth (beg, &tighter_lim);
          अञ्ञथा सचे (कुञ्चिका->general_numeric)
            ignore_मानम् (strtold (beg, &tighter_lim));
          अञ्ञथा सचे (कुञ्चिका->numeric || कुञ्चिका->human_numeric)
            {
              वर्ण ध्रुव *p = beg + (beg < lim && *beg == '-');
              वर्ण max_digit = traverse_raw_सङ्ख्या (&p);
              सचे ('0' <= max_digit)
                {
                  unsigned वर्ण ch = *p;
                  tighter_lim = (वर्ण *) p
                    + (कुञ्चिका->human_numeric && unit_order[ch]);
                }
            }
          अञ्ञथा
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t offset = debug_width (text, beg);
  size_t width = debug_width (beg, lim);
  mark_कुञ्चिका (offset, width);
}

/* Debug LINE by underlining its keys.  */

स्थिर शून्य
debug_पङ्क्तिः (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः)
{
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;

  करोहि
    debug_कुञ्चिका (पङ्क्तिः, कुञ्चिका);
  याव (कुञ्चिका && ((कुञ्चिका = कुञ्चिका->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

स्थिर bool
default_कुञ्चिका_compare (संरचना keyfield ध्रुव *कुञ्चिका)
{
  पटिनिवत्त ! (कुञ्चिका->ignore
            || कुञ्चिका->translate
            || कुञ्चिका->skipsblanks
            || कुञ्चिका->skipeblanks
            || कुञ्चिका_numeric (कुञ्चिका)
            || कुञ्चिका->month
            || कुञ्चिका->version
            || कुञ्चिका->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

स्थिर शून्य
कुञ्चिका_to_opts (संरचना keyfield ध्रुव *कुञ्चिका, वर्ण *opts)
{
  सचे (कुञ्चिका->skipsblanks || कुञ्चिका->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  सचे (कुञ्चिका->ignore == nondictionary)
    *opts++ = 'd';
  सचे (कुञ्चिका->translate)
    *opts++ = 'f';
  सचे (कुञ्चिका->general_numeric)
    *opts++ = 'g';
  सचे (कुञ्चिका->human_numeric)
    *opts++ = 'h';
  सचे (कुञ्चिका->ignore == nonprinting)
    *opts++ = 'i';
  सचे (कुञ्चिका->month)
    *opts++ = 'M';
  सचे (कुञ्चिका->numeric)
    *opts++ = 'n';
  सचे (कुञ्चिका->random)
    *opts++ = 'R';
  सचे (कुञ्चिका->reverse)
    *opts++ = 'r';
  सचे (कुञ्चिका->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

स्थिर शून्य
कुञ्चिका_warnings (संरचना keyfield ध्रुव *gkey, bool gkey_only)
{
  संरचना keyfield ugkey = *gkey;
  unsigned long keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  अत्थाय (संरचना keyfield *कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->next, keynum++)
    {
      सचे (कुञ्चिका_numeric (कुञ्चिका))
        {
          सचे (कुञ्चिका->general_numeric)
            general_numeric_field = true;
          अञ्ञथा
            basic_numeric_field = true;
        }

      सचे (कुञ्चिका->traditional_used)
        {
          size_t sword = कुञ्चिका->sword;
          size_t eword = कुञ्चिका->eword;
          वर्ण tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          वर्ण obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          वर्ण nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          वर्ण *po = obuf;
          वर्ण *pn = nbuf;

          सचे (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          सचे (कुञ्चिका->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (कुञ्चिका->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = कुञ्चिका->sword != SIZE_MAX && कुञ्चिका->eword < कुञ्चिका->sword;
      सचे (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = कुञ्चिका_numeric (कुञ्चिका) || कुञ्चिका->month;
      bool पङ्क्तिः_offset = कुञ्चिका->eword == 0 && कुञ्चिका->echar != 0; /* -k1.x,1.y  */
      सचे (!zero_width && !gkey_only && tab == TAB_DEFAULT && !पङ्क्तिः_offset
          && ((!कुञ्चिका->skipsblanks && !implicit_skip)
              || (!कुञ्चिका->skipsblanks && कुञ्चिका->schar)
              || (!कुञ्चिका->skipeblanks && कुञ्चिका->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      सचे (!gkey_only && कुञ्चिका_numeric (कुञ्चिका))
        {
          size_t sword = कुञ्चिका->sword + 1;
          size_t eword = कुञ्चिका->eword + 1;
          सचे (!sword)
            sword++;
          सचे (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              सचे (कुञ्चिका->general_numeric)
                general_numeric_field_span = true;
              अञ्ञथा
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      सचे (ugkey.ignore && (ugkey.ignore == कुञ्चिका->ignore))
        ugkey.ignore = रिक्त;
      सचे (ugkey.translate && (ugkey.translate == कुञ्चिका->translate))
        ugkey.translate = रिक्त;
      ugkey.skipsblanks &= !कुञ्चिका->skipsblanks;
      ugkey.skipeblanks &= !कुञ्चिका->skipeblanks;
      ugkey.month &= !कुञ्चिका->month;
      ugkey.numeric &= !कुञ्चिका->numeric;
      ugkey.general_numeric &= !कुञ्चिका->general_numeric;
      ugkey.human_numeric &= !कुञ्चिका->human_numeric;
      ugkey.random &= !कुञ्चिका->random;
      ugkey.version &= !कुञ्चिका->version;
      ugkey.reverse &= !कुञ्चिका->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool सङ्ख्या_locale_warned = false;
  सचे (basic_numeric_field_span)
    {
      सचे (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((वर्ण []) {thousands_sep, 0})));
          सङ्ख्या_locale_warned = true;
        }
    }
  सचे (basic_numeric_field_span || general_numeric_field_span)
    {
      सचे (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((वर्ण []) {decimal_point, 0})));
          सङ्ख्या_locale_warned = true;
        }
      अञ्ञथा सचे (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((वर्ण []) {tab, 0})));
        }
      अञ्ञथा सचे (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((वर्ण []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  सचे ((basic_numeric_field || general_numeric_field) && ! सङ्ख्या_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((वर्ण []) {decimal_point, 0})));

    }

  सचे (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  सचे (!default_कुञ्चिका_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      सचे (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      वर्ण opts[परिमाण short_options];
      कुञ्चिका_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (strlen (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  सचे (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

स्थिर पूर्णाङ्क
diff_reversed (पूर्णाङ्क diff, bool reversed)
{
  पटिनिवत्त reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

स्थिर पूर्णाङ्क
keycompare (संरचना पङ्क्तिः ध्रुव *a, संरचना पङ्क्तिः ध्रुव *b)
{
  संरचना keyfield *कुञ्चिका = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  वर्ण *texta = a->keybeg;
  वर्ण *textb = b->keybeg;
  वर्ण *lima = a->keylim;
  वर्ण *limb = b->keylim;

  पूर्णाङ्क diff;

  याव (true)
    {
      वर्ण ध्रुव *translate = कुञ्चिका->translate;
      bool ध्रुव *ignore = कुञ्चिका->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      सचे (hard_LC_COLLATE || कुञ्चिका_numeric (कुञ्चिका)
          || कुञ्चिका->month || कुञ्चिका->random || कुञ्चिका->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          वर्ण *ta = texta;
          वर्ण *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          वर्ण enda = ta[tlena];
          वर्ण endb = tb[tlenb];

          शून्य *allocated = रिक्त;
          वर्ण stackbuf[4000];

          सचे (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t size = lena + 1 + lenb + 1;
              सचे (size <= परिमाण stackbuf)
                ta = stackbuf;
              अञ्ञथा
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              अत्थाय (tlena = i = 0; i < lena; i++)
                सचे (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              अत्थाय (tlenb = i = 0; i < lenb; i++)
                सचे (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          सचे (कुञ्चिका->numeric)
            diff = numcompare (ta, tb);
          अञ्ञथा सचे (कुञ्चिका->general_numeric)
            diff = general_numcompare (ta, tb);
          अञ्ञथा सचे (कुञ्चिका->human_numeric)
            diff = human_numcompare (ta, tb);
          अञ्ञथा सचे (कुञ्चिका->month)
            diff = getmonth (ta, रिक्त) - getmonth (tb, रिक्त);
          अञ्ञथा सचे (कुञ्चिका->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          अञ्ञथा सचे (कुञ्चिका->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          अञ्ञथा
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              सचे (tlena == 0)
                diff = - NONZERO (tlenb);
              अञ्ञथा सचे (tlenb == 0)
                diff = 1;
              अञ्ञथा
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          मुक्त (allocated);
        }
      अञ्ञथा सचे (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  करोहि									\
    {									\
          याव (true)							\
            {								\
              याव (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              याव (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              सचे (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  भङ्ग;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              सचे (diff)							\
                भङ्ग;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  याव (0)

          सचे (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          अञ्ञथा
            CMP_WITH_IGNORE (*texta, *textb);
        }
      अञ्ञथा
        {
          size_t lenmin = MIN (lena, lenb);
          सचे (lenmin == 0)
            diff = 0;
          अञ्ञथा सचे (translate)
            {
              size_t i = 0;
              करोहि
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  सचे (diff)
                    भङ्ग;
                  i++;
                }
              याव (i < lenmin);
            }
          अञ्ञथा
            diff = memcmp (texta, textb, lenmin);

          सचे (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      सचे (diff)
        भङ्ग;

      कुञ्चिका = कुञ्चिका->next;
      सचे (! कुञ्चिका)
        पटिनिवत्त 0;

      /* Find the beginning and limit of the next field.  */
      सचे (कुञ्चिका->eword != SIZE_MAX)
        lima = limfield (a, कुञ्चिका), limb = limfield (b, कुञ्चिका);
      अञ्ञथा
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      सचे (कुञ्चिका->sword != SIZE_MAX)
        texta = begfield (a, कुञ्चिका), textb = begfield (b, कुञ्चिका);
      अञ्ञथा
        {
          texta = a->text, textb = b->text;
          सचे (कुञ्चिका->skipsblanks)
            {
              याव (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              याव (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  पटिनिवत्त diff_reversed (diff, कुञ्चिका->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

स्थिर पूर्णाङ्क
compare (संरचना पङ्क्तिः ध्रुव *a, संरचना पङ्क्तिः ध्रुव *b)
{
  पूर्णाङ्क diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  सचे (keylist)
    {
      diff = keycompare (a, b);
      सचे (diff || unique || stable)
        पटिनिवत्त diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  सचे (alen == 0)
    diff = - NONZERO (blen);
  अञ्ञथा सचे (blen == 0)
    diff = 1;
  अञ्ञथा सचे (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  अञ्ञथा
    {
      diff = memcmp (a->text, b->text, MIN (alen, blen));
      सचे (!diff)
        diff = _GL_CMP (alen, blen);
    }

  पटिनिवत्त diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

स्थिर शून्य
write_पङ्क्तिः (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, FILE *fp, वर्ण ध्रुव *output_file)
{
  वर्ण *buf = पङ्क्तिः->text;
  size_t n_bytes = पङ्क्तिः->length;
  वर्ण *ebuf = buf + n_bytes;

  सचे (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      वर्ण ध्रुव *c = buf;

      याव (c < ebuf)
        {
          वर्ण wc = *c++;
          सचे (wc == '\t')
            wc = '>';
          अञ्ञथा सचे (c == ebuf)
            wc = '\n';
          सचे (fputc (wc, fp) == EOF)
            sort_die (_("write failed"), output_file);
        }

      debug_पङ्क्तिः (पङ्क्तिः);
    }
  अञ्ञथा
    {
      ebuf[-1] = eolchar;
      सचे (fwrite (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

स्थिर bool
check (वर्ण ध्रुव *file_name, वर्ण checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  संरचना buffer buf;		/* Input buffer. */
  संरचना पङ्क्तिः temp;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t पङ्क्तिः_सङ्ख्या = 0;
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, परिमाण (संरचना पङ्क्तिः),
           MAX (merge_buffer_size, sort_size));
  temp.text = रिक्त;

  याव (fillbuf (&buf, fp, file_name, रिक्त))
    {
      संरचना पङ्क्तिः ध्रुव *पङ्क्तिः = buffer_linelim (&buf);
      संरचना पङ्क्तिः ध्रुव *linebase = पङ्क्तिः - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      सचे (alloc && nonunique <= compare (&temp, पङ्क्तिः - 1))
        {
        found_disorder:
          {
            सचे (checkonly == 'c')
              {
                संरचना पङ्क्तिः ध्रुव *disorder_पङ्क्तिः = पङ्क्तिः - 1;
                uintmax_t disorder_पङ्क्तिः_सङ्ख्या =
                  buffer_linelim (&buf) - disorder_पङ्क्तिः + पङ्क्तिः_सङ्ख्या;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_पङ्क्तिः_सङ्ख्या);
                write_पङ्क्तिः (disorder_पङ्क्तिः, stderr, _("standard error"));
              }

            ordered = false;
            भङ्ग;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      याव (linebase < --पङ्क्तिः)
        सचे (nonunique <= compare (पङ्क्तिः, पङ्क्तिः - 1))
          गच्छ found_disorder;

      पङ्क्तिः_सङ्ख्या += buf.nlines;

      /* Save the last line of the buffer.  */
      सचे (alloc < पङ्क्तिः->length)
        {
          करोहि
            {
              alloc *= 2;
              सचे (! alloc)
                {
                  alloc = पङ्क्तिः->length;
                  भङ्ग;
                }
            }
          याव (alloc < पङ्क्तिः->length);

          मुक्त (temp.text);
          temp.text = xmalloc (alloc);
        }
      memcpy (temp.text, पङ्क्तिः->text, पङ्क्तिः->length);
      temp.length = पङ्क्तिः->length;
      सचे (कुञ्चिका)
        {
          temp.keybeg = temp.text + (पङ्क्तिः->keybeg - पङ्क्तिः->text);
          temp.keylim = temp.text + (पङ्क्तिः->keylim - पङ्क्तिः->text);
        }
    }

  xfclose (fp, file_name);
  मुक्त (buf.buf);
  मुक्त (temp.text);
  पटिनिवत्त ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

स्थिर size_t
open_निवेशः_files (संरचना sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, परिमाण *fps);
  पूर्णाङ्क i;

  /* Open as many input files as we can.  */
  अत्थाय (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      सचे (!fps[i])
        भङ्ग;
    }

  पटिनिवत्त i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

स्थिर शून्य
mergefps (संरचना sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, वर्ण ध्रुव *output_file, FILE **fps)
{
  संरचना buffer *buffer = xnmalloc (nfiles, परिमाण *buffer);
                                /* Input buffers for each file. */
  संरचना पङ्क्तिः saved;		/* Saved line storage for unique check. */
  संरचना पङ्क्तिः ध्रुव *savedline = रिक्त;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  संरचना पङ्क्तिः ध्रुव **cur = xnmalloc (nfiles, परिमाण *cur);
                                /* Current line in each line table. */
  संरचना पङ्क्तिः ध्रुव **base = xnmalloc (nfiles, परिमाण *base);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, परिमाण *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  saved.text = रिक्त;

  /* Read initial lines from each input file. */
  अत्थाय (size_t i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], परिमाण (संरचना पङ्क्तिः),
               MAX (merge_buffer_size, sort_size / nfiles));
      सचे (fillbuf (&buffer[i], fps[i], files[i].name, रिक्त))
        {
          संरचना पङ्क्तिः ध्रुव *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      अञ्ञथा
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          सचे (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          मुक्त (buffer[i].buf);
          --nfiles;
          अत्थाय (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  अत्थाय (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  अत्थाय (size_t i = 1; i < nfiles; ++i)
    सचे (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  याव (nfiles)
    {
      संरचना पङ्क्तिः ध्रुव *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      सचे (unique)
        {
          सचे (savedline && compare (savedline, smallest))
            {
              savedline = रिक्त;
              write_पङ्क्तिः (&saved, ofp, output_file);
            }
          सचे (!savedline)
            {
              savedline = &saved;
              सचे (savealloc < smallest->length)
                {
                  करोहि
                    सचे (! savealloc)
                      {
                        savealloc = smallest->length;
                        भङ्ग;
                      }
                  याव ((savealloc *= 2) < smallest->length);

                  मुक्त (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              memcpy (saved.text, smallest->text, saved.length);
              सचे (कुञ्चिका)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      अञ्ञथा
        write_पङ्क्तिः (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      सचे (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      अञ्ञथा
        {
          सचे (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       रिक्त))
            {
              संरचना पङ्क्तिः ध्रुव *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          अञ्ञथा
            {
              /* We reached EOF on fps[ord[0]].  */
              अत्थाय (size_t i = 1; i < nfiles; ++i)
                सचे (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              सचे (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              मुक्त (buffer[ord[0]].buf);
              अत्थाय (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              अत्थाय (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              अनुवर्त;
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
        size_t गणनम्_of_smaller_पङ्क्तयः;

        याव (lo < hi)
          {
            पूर्णाङ्क cmp = compare (cur[ord0], cur[ord[probe]]);
            सचे (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            अञ्ञथा
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        गणनम्_of_smaller_पङ्क्तयः = lo - 1;
        अत्थाय (size_t j = 0; j < गणनम्_of_smaller_पङ्क्तयः; j++)
          ord[j] = ord[j + 1];
        ord[गणनम्_of_smaller_पङ्क्तयः] = ord0;
      }
    }

  सचे (unique && savedline)
    {
      write_पङ्क्तिः (&saved, ofp, output_file);
      मुक्त (saved.text);
    }

  xfclose (ofp, output_file);
  मुक्त (fps);
  मुक्त (buffer);
  मुक्त (ord);
  मुक्त (base);
  मुक्त (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

स्थिर size_t
mergefiles (संरचना sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, वर्ण ध्रुव *output_file)
{
  FILE **fps;
  size_t nopened = open_निवेशः_files (files, nfiles, &fps);
  सचे (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  पटिनिवत्त nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

स्थिर शून्य
mergelines (संरचना पङ्क्तिः *restrict t, size_t nlines,
            संरचना पङ्क्तिः ध्रुव *restrict lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  संरचना पङ्क्तिः *hi = t - nlo;

  याव (true)
    सचे (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        सचे (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            पटिनिवत्त;
          }
      }
    अञ्ञथा
      {
        *--t = *--hi;
        सचे (! --nhi)
          {
            करोहि
              *--t = *--lo;
            याव (--nlo);

            पटिनिवत्त;
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

स्थिर शून्य
sequential_sort (संरचना पङ्क्तिः *restrict पङ्क्तयः, size_t nlines,
                 संरचना पङ्क्तिः *restrict temp, bool to_temp)
{
  सचे (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      पूर्णाङ्क swap = (0 < compare (&पङ्क्तयः[-1], &पङ्क्तयः[-2]));
      सचे (to_temp)
        {
          temp[-1] = पङ्क्तयः[-1 - swap];
          temp[-2] = पङ्क्तयः[-2 + swap];
        }
      अञ्ञथा सचे (swap)
        {
          temp[-1] = पङ्क्तयः[-1];
          पङ्क्तयः[-1] = पङ्क्तयः[-2];
          पङ्क्तयः[-2] = temp[-1];
        }
    }
  अञ्ञथा
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      संरचना पङ्क्तिः *lo = पङ्क्तयः;
      संरचना पङ्क्तिः *hi = पङ्क्तयः - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      सचे (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      अञ्ञथा सचे (!to_temp)
        temp[-1] = lo[-1];

      संरचना पङ्क्तिः *dest;
      संरचना पङ्क्तिः ध्रुव *sorted_lo;
      सचे (to_temp)
        {
          dest = temp;
          sorted_lo = पङ्क्तयः;
        }
      अञ्ञथा
        {
          dest = पङ्क्तयः;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

स्थिर संरचना merge_ग्रन्थिः *init_ग्रन्थिः (संरचना merge_ग्रन्थिः *restrict,
                                     संरचना merge_ग्रन्थिः *restrict,
                                     संरचना पङ्क्तिः *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
स्थिर संरचना merge_ग्रन्थिः *
merge_वृक्षः_init (size_t nthreads, size_t nlines, संरचना पङ्क्तिः *dest)
{
  संरचना merge_ग्रन्थिः *merge_वृक्षः = xmalloc (2 * परिमाण *merge_वृक्षः * nthreads);

  संरचना merge_ग्रन्थिः *मूलम् = merge_वृक्षः;
  मूलम्->lo = मूलम्->hi = मूलम्->end_lo = मूलम्->end_hi = रिक्त;
  मूलम्->dest = रिक्त;
  मूलम्->nlo = मूलम्->nhi = nlines;
  मूलम्->parent = रिक्त;
  मूलम्->level = MERGE_END;
  मूलम्->queued = false;
  pthread_mutex_init (&मूलम्->lock, रिक्त);

  init_ग्रन्थिः (मूलम्, मूलम् + 1, dest, nthreads, nlines, false);
  पटिनिवत्त merge_वृक्षः;
}

/* Destroy the merge tree. */
स्थिर शून्य
merge_वृक्षः_destroy (size_t nthreads, संरचना merge_ग्रन्थिः *merge_वृक्षः)
{
  size_t n_nodes = nthreads * 2;
  संरचना merge_ग्रन्थिः *ग्रन्थिः = merge_वृक्षः;

  याव (n_nodes--)
    {
      pthread_mutex_destroy (&ग्रन्थिः->lock);
      ग्रन्थिः++;
    }

  मुक्त (merge_वृक्षः);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

स्थिर संरचना merge_ग्रन्थिः *
init_ग्रन्थिः (संरचना merge_ग्रन्थिः *restrict parent,
           संरचना merge_ग्रन्थिः *restrict ग्रन्थिः_pool,
           संरचना पङ्क्तिः *dest, size_t nthreads,
           size_t total_पङ्क्तयः, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  संरचना पङ्क्तिः *lo = dest - total_पङ्क्तयः;
  संरचना पङ्क्तिः *hi = lo - nlo;
  संरचना पङ्क्तिः **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  संरचना merge_ग्रन्थिः *ग्रन्थिः = ग्रन्थिः_pool++;
  ग्रन्थिः->lo = ग्रन्थिः->end_lo = lo;
  ग्रन्थिः->hi = ग्रन्थिः->end_hi = hi;
  ग्रन्थिः->dest = parent_end;
  ग्रन्थिः->nlo = nlo;
  ग्रन्थिः->nhi = nhi;
  ग्रन्थिः->parent = parent;
  ग्रन्थिः->level = parent->level + 1;
  ग्रन्थिः->queued = false;
  pthread_mutex_init (&ग्रन्थिः->lock, रिक्त);

  सचे (nthreads > 1)
    {
      size_t lo_threads = nthreads / 2;
      size_t hi_threads = nthreads - lo_threads;
      ग्रन्थिः->lo_child = ग्रन्थिः_pool;
      ग्रन्थिः_pool = init_ग्रन्थिः (ग्रन्थिः, ग्रन्थिः_pool, lo, lo_threads,
                             total_पङ्क्तयः, true);
      ग्रन्थिः->hi_child = ग्रन्थिः_pool;
      ग्रन्थिः_pool = init_ग्रन्थिः (ग्रन्थिः, ग्रन्थिः_pool, hi, hi_threads,
                             total_पङ्क्तयः, false);
    }
  अञ्ञथा
    {
      ग्रन्थिः->lo_child = रिक्त;
      ग्रन्थिः->hi_child = रिक्त;
    }
  पटिनिवत्त ग्रन्थिः_pool;
}


/* Compare two merge nodes A and B for priority.  */

स्थिर पूर्णाङ्क
compare_nodes (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  संरचना merge_ग्रन्थिः ध्रुव *nodea = a;
  संरचना merge_ग्रन्थिः ध्रुव *nodeb = b;
  सचे (nodea->level == nodeb->level)
      पटिनिवत्त (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  पटिनिवत्त nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

स्थिर अन्तःस्थ शून्य
lock_ग्रन्थिः (संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_lock (&ग्रन्थिः->lock);
}

/* Unlock a merge tree NODE. */

स्थिर अन्तःस्थ शून्य
unlock_ग्रन्थिः (संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_unlock (&ग्रन्थिः->lock);
}

/* Destroy merge QUEUE. */

स्थिर शून्य
queue_destroy (संरचना merge_ग्रन्थिः_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

स्थिर शून्य
queue_init (संरचना merge_ग्रन्थिः_queue *queue, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, रिक्त);
  pthread_cond_init (&queue->cond, रिक्त);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

स्थिर शून्य
queue_निवेशय (संरचना merge_ग्रन्थिः_queue *queue, संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_lock (&queue->mutex);
  heap_निवेशय (queue->priority_queue, ग्रन्थिः);
  ग्रन्थिः->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

स्थिर संरचना merge_ग्रन्थिः *
queue_निष्कासय (संरचना merge_ग्रन्थिः_queue *queue)
{
  संरचना merge_ग्रन्थिः *ग्रन्थिः;
  pthread_mutex_lock (&queue->mutex);
  याव (! (ग्रन्थिः = heap_remove_शिखरम् (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_ग्रन्थिः (ग्रन्थिः);
  ग्रन्थिः->queued = false;
  पटिनिवत्त ग्रन्थिः;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

स्थिर शून्य
write_unique (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, FILE *tfp, वर्ण ध्रुव *temp_output)
{
  सचे (unique)
    {
      सचे (saved_पङ्क्तिः.text && ! compare (पङ्क्तिः, &saved_पङ्क्तिः))
        पटिनिवत्त;
      saved_पङ्क्तिः = *पङ्क्तिः;
    }

  write_पङ्क्तिः (पङ्क्तिः, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

स्थिर शून्य
mergelines_ग्रन्थिः (संरचना merge_ग्रन्थिः *restrict ग्रन्थिः, size_t total_पङ्क्तयः,
                 FILE *tfp, वर्ण ध्रुव *temp_output)
{
  संरचना पङ्क्तिः *lo_orig = ग्रन्थिः->lo;
  संरचना पङ्क्तिः *hi_orig = ग्रन्थिः->hi;
  size_t to_merge = MAX_MERGE (total_पङ्क्तयः, ग्रन्थिः->level);
  size_t merged_lo;
  size_t merged_hi;

  सचे (ग्रन्थिः->level > MERGE_मूलम्)
    {
      /* Merge to destination buffer. */
      संरचना पङ्क्तिः *dest = *ग्रन्थिः->dest;
      याव (ग्रन्थिः->lo != ग्रन्थिः->end_lo && ग्रन्थिः->hi != ग्रन्थिः->end_hi && to_merge--)
        सचे (compare (ग्रन्थिः->lo - 1, ग्रन्थिः->hi - 1) <= 0)
          *--dest = *--ग्रन्थिः->lo;
        अञ्ञथा
          *--dest = *--ग्रन्थिः->hi;

      merged_lo = lo_orig - ग्रन्थिः->lo;
      merged_hi = hi_orig - ग्रन्थिः->hi;

      सचे (ग्रन्थिः->nhi == merged_hi)
        याव (ग्रन्थिः->lo != ग्रन्थिः->end_lo && to_merge--)
          *--dest = *--ग्रन्थिः->lo;
      अञ्ञथा सचे (ग्रन्थिः->nlo == merged_lo)
        याव (ग्रन्थिः->hi != ग्रन्थिः->end_hi && to_merge--)
          *--dest = *--ग्रन्थिः->hi;
      *ग्रन्थिः->dest = dest;
    }
  अञ्ञथा
    {
      /* Merge directly to output. */
      याव (ग्रन्थिः->lo != ग्रन्थिः->end_lo && ग्रन्थिः->hi != ग्रन्थिः->end_hi && to_merge--)
        {
          सचे (compare (ग्रन्थिः->lo - 1, ग्रन्थिः->hi - 1) <= 0)
            write_unique (--ग्रन्थिः->lo, tfp, temp_output);
          अञ्ञथा
            write_unique (--ग्रन्थिः->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - ग्रन्थिः->lo;
      merged_hi = hi_orig - ग्रन्थिः->hi;

      सचे (ग्रन्थिः->nhi == merged_hi)
        {
          याव (ग्रन्थिः->lo != ग्रन्थिः->end_lo && to_merge--)
            write_unique (--ग्रन्थिः->lo, tfp, temp_output);
        }
      अञ्ञथा सचे (ग्रन्थिः->nlo == merged_lo)
        {
          याव (ग्रन्थिः->hi != ग्रन्थिः->end_hi && to_merge--)
            write_unique (--ग्रन्थिः->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ग्रन्थिः->lo;
  merged_hi = hi_orig - ग्रन्थिः->hi;
  ग्रन्थिः->nlo -= merged_lo;
  ग्रन्थिः->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

स्थिर शून्य
queue_check_निवेशय (संरचना merge_ग्रन्थिः_queue *queue, संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  सचे (! ग्रन्थिः->queued)
    {
      bool lo_avail = (ग्रन्थिः->lo - ग्रन्थिः->end_lo) != 0;
      bool hi_avail = (ग्रन्थिः->hi - ग्रन्थिः->end_hi) != 0;
      सचे (lo_avail ? hi_avail || ! ग्रन्थिः->nhi : hi_avail && ! ग्रन्थिः->nlo)
        queue_निवेशय (queue, ग्रन्थिः);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

स्थिर शून्य
queue_check_निवेशय_parent (संरचना merge_ग्रन्थिः_queue *queue,
                           संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  सचे (ग्रन्थिः->level > MERGE_मूलम्)
    {
      lock_ग्रन्थिः (ग्रन्थिः->parent);
      queue_check_निवेशय (queue, ग्रन्थिः->parent);
      unlock_ग्रन्थिः (ग्रन्थिः->parent);
    }
  अञ्ञथा सचे (ग्रन्थिः->nlo + ग्रन्थिः->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_निवेशय (queue, ग्रन्थिः->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

स्थिर शून्य
merge_loop (संरचना merge_ग्रन्थिः_queue *queue,
            size_t total_पङ्क्तयः, FILE *tfp, वर्ण ध्रुव *temp_output)
{
  याव (true)
    {
      संरचना merge_ग्रन्थिः *ग्रन्थिः = queue_निष्कासय (queue);

      सचे (ग्रन्थिः->level == MERGE_END)
        {
          unlock_ग्रन्थिः (ग्रन्थिः);
          /* Reinsert so other threads can pop it. */
          queue_निवेशय (queue, ग्रन्थिः);
          भङ्ग;
        }
      mergelines_ग्रन्थिः (ग्रन्थिः, total_पङ्क्तयः, tfp, temp_output);
      queue_check_निवेशय (queue, ग्रन्थिः);
      queue_check_निवेशय_parent (queue, ग्रन्थिः);

      unlock_ग्रन्थिः (ग्रन्थिः);
    }
}


स्थिर शून्य sortlines (संरचना पङ्क्तिः *restrict, size_t, size_t,
                       संरचना merge_ग्रन्थिः *, संरचना merge_ग्रन्थिः_queue *,
                       FILE *, वर्ण ध्रुव *);

/* Thread arguments for sortlines_thread. */

संरचना thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  संरचना पङ्क्तिः *पङ्क्तयः;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ध्रुव total_पङ्क्तयः;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  संरचना merge_ग्रन्थिः *ध्रुव ग्रन्थिः;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  संरचना merge_ग्रन्थिः_queue *ध्रुव queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  वर्ण ध्रुव *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

स्थिर शून्य *
sortlines_thread (शून्य *data)
{
  संरचना thread_args ध्रुव *args = data;
  sortlines (args->पङ्क्तयः, args->nthreads, args->total_पङ्क्तयः,
             args->ग्रन्थिः, args->queue, args->tfp,
             args->output_temp);
  पटिनिवत्त रिक्त;
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

स्थिर शून्य
sortlines (संरचना पङ्क्तिः *restrict पङ्क्तयः, size_t nthreads,
           size_t total_पङ्क्तयः, संरचना merge_ग्रन्थिः *ग्रन्थिः,
           संरचना merge_ग्रन्थिः_queue *queue, FILE *tfp, वर्ण ध्रुव *temp_output)
{
  size_t nlines = ग्रन्थिः->nlo + ग्रन्थिः->nhi;

  /* Calculate thread arguments. */
  size_t lo_threads = nthreads / 2;
  size_t hi_threads = nthreads - lo_threads;
  pthread_t thread;
  संरचना thread_args args = {पङ्क्तयः, lo_threads, total_पङ्क्तयः,
                             ग्रन्थिः->lo_child, queue, tfp, temp_output};

  सचे (nthreads > 1 && SUBTHREAD_पङ्क्तयः_HEURISTIC <= nlines
      && pthread_create (&thread, रिक्त, sortlines_thread, &args) == 0)
    {
      sortlines (पङ्क्तयः - ग्रन्थिः->nlo, hi_threads, total_पङ्क्तयः,
                 ग्रन्थिः->hi_child, queue, tfp, temp_output);
      pthread_join (thread, रिक्त);
    }
  अञ्ञथा
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ग्रन्थिः->nlo;
      size_t nhi = ग्रन्थिः->nhi;
      संरचना पङ्क्तिः *temp = पङ्क्तयः - total_पङ्क्तयः;
      सचे (1 < nhi)
        sequential_sort (पङ्क्तयः - nlo, nhi, temp - nlo / 2, false);
      सचे (1 < nlo)
        sequential_sort (पङ्क्तयः, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      ग्रन्थिः->lo = पङ्क्तयः;
      ग्रन्थिः->hi = पङ्क्तयः - nlo;
      ग्रन्थिः->end_lo = पङ्क्तयः - nlo;
      ग्रन्थिः->end_hi = पङ्क्तयः - nlo - nhi;

      queue_निवेशय (queue, ग्रन्थिः);
      merge_loop (queue, total_पङ्क्तयः, tfp, temp_output);
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

स्थिर शून्य
avoid_trashing_निवेशः (संरचना sortfile *files, size_t ntemps,
                      size_t nfiles, वर्ण ध्रुव *outfile)
{
  संरचना tempnode *tempcopy = रिक्त;

  अत्थाय (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      संरचना stat instat;

      सचे (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      अञ्ञथा
        {
          संरचना stat *outst = get_outstatus ();
          सचे (!outst)
            भङ्ग;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      सचे (same)
        {
          सचे (! tempcopy)
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

स्थिर शून्य
check_inputs (वर्ण *ध्रुव *files, size_t nfiles)
{
  अत्थाय (size_t i = 0; i < nfiles; i++)
    {
      सचे (streq (files[i], "-"))
        अनुवर्त;

      सचे (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

स्थिर शून्य
check_output (वर्ण ध्रुव *outfile)
{
  सचे (outfile)
    {
      पूर्णाङ्क oflags = O_WRONLY | O_द्विमान | O_CLOEXEC | O_CREAT;
      पूर्णाङ्क outfd = open (outfile, oflags, MODE_RW_UGO);
      सचे (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

स्थिर शून्य
merge (संरचना sortfile *files, size_t ntemps, size_t nfiles,
       वर्ण ध्रुव *output_file)
{
  याव (nmerge < nfiles)
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
      अत्थाय (out = in = 0; nmerge <= nfiles - in; out++)
        {
          FILE *tfp;
          संरचना tempnode *temp = create_temp (&tfp);
          size_t अङ्कः_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, अङ्कः_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += अङ्कः_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      सचे (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          संरचना tempnode *temp = create_temp (&tfp);
          size_t अङ्कः_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, अङ्कः_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += अङ्कः_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[out], &files[in], (nfiles - in) * परिमाण *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_निवेशः (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  याव (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_निवेशः_files (files, nfiles, &fps);

      सचे (nopened == nfiles)
        {
          FILE *ofp = stream_open (output_file, "w");
          सचे (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              भङ्ग;
            }
          सचे (errno != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      अञ्ञथा सचे (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      संरचना tempnode *temp;
      करोहि
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      याव (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * परिमाण *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

स्थिर शून्य
sort (वर्ण *ध्रुव *files, size_t nfiles, वर्ण ध्रुव *output_file,
      size_t nthreads)
{
  संरचना buffer buf;
  संरचना sort_buffer_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  याव (nfiles)
    {
      वर्ण ध्रुव *temp_output;
      वर्ण ध्रुव *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t bytes_per_पङ्क्तिः;
      सचे (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          याव (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_पङ्क्तिः = (mult * परिमाण (संरचना पङ्क्तिः));
        }
      अञ्ञथा
        bytes_per_पङ्क्तिः = परिमाण (संरचना पङ्क्तिः) * 3 / 2;

      सचे (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_पङ्क्तिः,
                              &policy);
          initbuf (&buf, bytes_per_पङ्क्तिः, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      याव (fillbuf (&buf, fp, file, &policy))
        {
          संरचना पङ्क्तिः *पङ्क्तिः;

          सचे (buf.eof && nfiles
              && (bytes_per_पङ्क्तिः + 1
                  < (buf.alloc - buf.used - bytes_per_पङ्क्तिः * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.वामः = buf.used;
              भङ्ग;
            }

          saved_पङ्क्तिः.text = रिक्त;
          पङ्क्तिः = buffer_linelim (&buf);
          सचे (buf.eof && !nfiles && !ntemps && !buf.वामः)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          अञ्ञथा
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          सचे (1 < buf.nlines)
            {
              सचे (nthreads > 1)
                {
                  संरचना merge_ग्रन्थिः_queue queue;
                  queue_init (&queue, nthreads);
                  संरचना merge_ग्रन्थिः *merge_वृक्षः =
                    merge_वृक्षः_init (nthreads, buf.nlines, पङ्क्तिः);

                  sortlines (पङ्क्तिः, nthreads, buf.nlines, merge_वृक्षः + 1,
                             &queue, tfp, temp_output);

                  merge_वृक्षः_destroy (nthreads, merge_वृक्षः);
                  queue_destroy (&queue);
                }
              अञ्ञथा
                {
                  sequential_sort (पङ्क्तिः, buf.nlines,
                                   पङ्क्तिः - buf.nlines, false);
                  अत्थाय (size_t i = 0; i < buf.nlines; i++)
                    write_unique (पङ्क्तिः - i - 1, tfp, temp_output);
                }
            }
          अञ्ञथा
            write_unique (पङ्क्तिः - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          सचे (output_file_created)
            गच्छ finish;
        }
      xfclose (fp, file);
    }

 finish:
  मुक्त (buf.buf);

  सचे (! output_file_created)
    {
      संरचना tempnode *ग्रन्थिः = temphead;
      संरचना sortfile *tempfiles = xnmalloc (ntemps, परिमाण *tempfiles);
      अत्थाय (size_t i = 0; ग्रन्थिः; i++)
        {
          tempfiles[i].name = ग्रन्थिः->name;
          tempfiles[i].temp = ग्रन्थिः;
          ग्रन्थिः = ग्रन्थिः->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      मुक्त (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

स्थिर शून्य
insertkey (संरचना keyfield *कुञ्चिका_arg)
{
  संरचना keyfield **p;
  संरचना keyfield *कुञ्चिका = xmemdup (कुञ्चिका_arg, परिमाण *कुञ्चिका);

  अत्थाय (p = &keylist; *p; p = &(*p)->next)
    अनुवर्त;
  *p = कुञ्चिका;
  कुञ्चिका->next = रिक्त;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

स्थिर शून्य
badfieldspec (वर्ण ध्रुव *spec, वर्ण ध्रुव *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

स्थिर शून्य
incompatible_options (वर्ण ध्रुव *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

स्थिर शून्य
check_ordering_compatibility (शून्य)
{
  अत्थाय (संरचना keyfield *कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->next)
    सचे (1 < (कुञ्चिका->numeric + कुञ्चिका->general_numeric + कुञ्चिका->human_numeric
             + कुञ्चिका->month + (कुञ्चिका->version | कुञ्चिका->random | !!कुञ्चिका->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        वर्ण opts[परिमाण short_options];
        /* Clear flags we're not interested in.  */
        कुञ्चिका->skipsblanks = कुञ्चिका->skipeblanks = कुञ्चिका->reverse = false;
        कुञ्चिका_to_opts (कुञ्चिका, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

स्थिर वर्ण ध्रुव *
parse_field_गणनम् (वर्ण ध्रुव *string, size_t *val, वर्ण ध्रुव *msgid)
{
  वर्ण *suffix;
  uintmax_t n;

  विकल्प (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    पक्ष LONGINT_OK:
    पक्ष LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      सचे (*val == n)
        भङ्ग;
      FALLTHROUGH;
    पक्ष LONGINT_OVERFLOW:
    पक्ष LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      भङ्ग;

    पक्ष LONGINT_INVALID:
      सचे (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      पटिनिवत्त रिक्त;
    }

  पटिनिवत्त suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

स्थिर वर्ण *
set_ordering (वर्ण ध्रुव *s, संरचना keyfield *कुञ्चिका, गणना blanktype blanktype)
{
  याव (*s)
    {
      विकल्प (*s)
        {
        पक्ष 'b':
          सचे (blanktype == bl_start || blanktype == bl_both)
            कुञ्चिका->skipsblanks = true;
          सचे (blanktype == bl_end || blanktype == bl_both)
            कुञ्चिका->skipeblanks = true;
          भङ्ग;
        पक्ष 'd':
          कुञ्चिका->ignore = nondictionary;
          भङ्ग;
        पक्ष 'f':
          कुञ्चिका->translate = fold_toupper;
          भङ्ग;
        पक्ष 'g':
          कुञ्चिका->general_numeric = true;
          भङ्ग;
        पक्ष 'h':
          कुञ्चिका->human_numeric = true;
          भङ्ग;
        पक्ष 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          सचे (! कुञ्चिका->ignore)
            कुञ्चिका->ignore = nonprinting;
          भङ्ग;
        पक्ष 'M':
          कुञ्चिका->month = true;
          भङ्ग;
        पक्ष 'n':
          कुञ्चिका->numeric = true;
          भङ्ग;
        पक्ष 'R':
          कुञ्चिका->random = true;
          भङ्ग;
        पक्ष 'r':
          कुञ्चिका->reverse = true;
          भङ्ग;
        पक्ष 'V':
          कुञ्चिका->version = true;
          भङ्ग;
        स्वभाव:
          पटिनिवत्त (वर्ण *) s;
        }
      ++s;
    }
  पटिनिवत्त (वर्ण *) s;
}

/* Initialize KEY.  */

स्थिर संरचना keyfield *
कुञ्चिका_init (संरचना keyfield *कुञ्चिका)
{
  memset (कुञ्चिका, 0, परिमाण *कुञ्चिका);
  कुञ्चिका->eword = SIZE_MAX;
  पटिनिवत्त कुञ्चिका;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  संरचना keyfield *कुञ्चिका;
  संरचना keyfield कुञ्चिका_buf;
  संरचना keyfield gkey;
  bool gkey_only = false;
  वर्ण ध्रुव *s;
  पूर्णाङ्क c = 0;
  वर्ण checkonly = 0;
  bool mergeonly = false;
  वर्ण *random_source = रिक्त;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != रिक्त);
  पूर्णाङ्क posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  वर्ण **files;
  वर्ण *files_from = रिक्त;
  संरचना Tokens tok;
  वर्ण ध्रुव *outfile = रिक्त;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#सचे HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    संरचना lconv ध्रुव *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    सचे (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    सचे (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    सचे (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    गणना { nsigs = countof (term_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_signals);
    अत्थाय (size_t i = 0; i < nsigs; i++)
      {
        सचे (term_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        sigaction (term_sig[i], रिक्त, &act);
        सचे (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    अत्थाय (size_t i = 0; i < nsigs; i++)
      {
        सचे (term_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        सचे (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, रिक्त);
      }
  }
  सङ्केत (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  कुञ्चिका_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, परिमाण *files);

  याव (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      पूर्णाङ्क oi = -1;

      सचे (c == -1
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
          सचे (argc <= optind)
            भङ्ग;
          files[nfiles++] = argv[optind++];
        }
      अञ्ञथा विकल्प (c)
        {
        पक्ष 1:
          कुञ्चिका = रिक्त;
          सचे (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              सचे (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  कुञ्चिका = कुञ्चिका_init (&कुञ्चिका_buf);
                  s = parse_field_गणनम् (optarg + 1, &कुञ्चिका->sword, रिक्त);
                  सचे (s && *s == '.')
                    s = parse_field_गणनम् (s + 1, &कुञ्चिका->schar, रिक्त);
                  सचे (! (कुञ्चिका->sword || कुञ्चिका->schar))
                    कुञ्चिका->sword = SIZE_MAX;
                  सचे (! s || *set_ordering (s, कुञ्चिका, bl_start))
                    कुञ्चिका = रिक्त;
                  अञ्ञथा
                    {
                      सचे (minus_pos_usage)
                        {
                          वर्ण ध्रुव *optarg1 = argv[optind++];
                          s = parse_field_गणनम् (optarg1 + 1, &कुञ्चिका->eword,
                                             N_("invalid number after '-'"));
                          सचे (*s == '.')
                            s = parse_field_गणनम् (s + 1, &कुञ्चिका->echar,
                                               N_("invalid number after '.'"));
                          सचे (!कुञ्चिका->echar && कुञ्चिका->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              कुञ्चिका->eword--;
                            }
                          सचे (*set_ordering (s, कुञ्चिका, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      कुञ्चिका->traditional_used = true;
                      insertkey (कुञ्चिका);
                    }
                }
            }
          सचे (! कुञ्चिका)
            files[nfiles++] = optarg;
          भङ्ग;

        पक्ष SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        पक्ष 'b':
        पक्ष 'd':
        पक्ष 'f':
        पक्ष 'g':
        पक्ष 'h':
        पक्ष 'i':
        पक्ष 'M':
        पक्ष 'n':
        पक्ष 'r':
        पक्ष 'R':
        पक्ष 'V':
          {
            वर्ण str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          भङ्ग;

        पक्ष CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        पक्ष 'c':
        पक्ष 'C':
          सचे (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          भङ्ग;

        पक्ष COMPRESS_PROGRAM_OPTION:
          सचे (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          भङ्ग;

        पक्ष DEBUG_PROGRAM_OPTION:
          debug = true;
          भङ्ग;

        पक्ष FILES0_FROM_OPTION:
          files_from = optarg;
          भङ्ग;

        पक्ष 'k':
          कुञ्चिका = कुञ्चिका_init (&कुञ्चिका_buf);

          /* Get POS1. */
          s = parse_field_गणनम् (optarg, &कुञ्चिका->sword,
                                 N_("invalid number at field start"));
          सचे (! कुञ्चिका->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          सचे (*s == '.')
            {
              s = parse_field_गणनम् (s + 1, &कुञ्चिका->schar,
                                     N_("invalid number after '.'"));
              सचे (! कुञ्चिका->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          सचे (! (कुञ्चिका->sword || कुञ्चिका->schar))
            कुञ्चिका->sword = SIZE_MAX;
          s = set_ordering (s, कुञ्चिका, bl_start);
          सचे (*s != ',')
            {
              कुञ्चिका->eword = SIZE_MAX;
              कुञ्चिका->echar = 0;
            }
          अञ्ञथा
            {
              /* Get POS2. */
              s = parse_field_गणनम् (s + 1, &कुञ्चिका->eword,
                                     N_("invalid number after ','"));
              सचे (! कुञ्चिका->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              सचे (*s == '.')
                {
                  s = parse_field_गणनम् (s + 1, &कुञ्चिका->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, कुञ्चिका, bl_end);
            }
          सचे (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (कुञ्चिका);
          भङ्ग;

        पक्ष 'm':
          mergeonly = true;
          भङ्ग;

        पक्ष NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          भङ्ग;

        पक्ष 'o':
          सचे (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          भङ्ग;

        पक्ष RANDOM_SOURCE_OPTION:
          सचे (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          भङ्ग;

        पक्ष 's':
          stable = true;
          भङ्ग;

        पक्ष 'S':
          specify_sort_size (oi, c, optarg);
          भङ्ग;

        पक्ष 't':
          {
            वर्ण newtab = optarg[0];
            सचे (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            सचे (optarg[1])
              {
                सचे (streq (optarg, "\\0"))
                  newtab = '\0';
                अञ्ञथा
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            सचे (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          भङ्ग;

        पक्ष 'T':
          add_temp_dir (optarg);
          भङ्ग;

        पक्ष PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          भङ्ग;

        पक्ष 'u':
          unique = true;
          भङ्ग;

        पक्ष 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          सचे (optarg == argv[optind - 1])
            {
              वर्ण ध्रुव *p;
              अत्थाय (p = optarg; c_isdigit (*p); p++)
                अनुवर्त;
              optind -= (*p != '\0');
            }
          भङ्ग;

        पक्ष 'z':
          eolchar = 0;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (सङ्केत (SIGPIPE, SIG_IGN) == SIG_DFL);

  सचे (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      सचे (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      FILE *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      सचे (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      सचे (tok.n_tok)
        {
          मुक्त (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          अत्थाय (size_t i = 0; i < nfiles; i++)
            {
              सचे (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              अञ्ञथा सचे (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  unsigned long पूर्णाङ्क file_सङ्ख्या = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_सङ्ख्या);
                }
            }
        }
      अञ्ञथा
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  अत्थाय (कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->next)
    {
      सचे (default_कुञ्चिका_compare (कुञ्चिका) && !कुञ्चिका->reverse)
        {
          कुञ्चिका->ignore = gkey.ignore;
          कुञ्चिका->translate = gkey.translate;
          कुञ्चिका->skipsblanks = gkey.skipsblanks;
          कुञ्चिका->skipeblanks = gkey.skipeblanks;
          कुञ्चिका->month = gkey.month;
          कुञ्चिका->numeric = gkey.numeric;
          कुञ्चिका->general_numeric = gkey.general_numeric;
          कुञ्चिका->human_numeric = gkey.human_numeric;
          कुञ्चिका->version = gkey.version;
          कुञ्चिका->random = gkey.random;
          कुञ्चिका->reverse = gkey.reverse;
        }

      need_random |= कुञ्चिका->random;
    }

  सचे (!keylist && !default_कुञ्चिका_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  सचे (debug)
    {
      सचे (checkonly || outfile)
        {
          स्थिर वर्ण opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      सचे (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      सचे (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      सचे (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, रिक्त)));
      अञ्ञथा
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      कुञ्चिका_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  सचे (need_random)
    random_md5_state_init (random_source);

  सचे (temp_dirs.n_dirs == 0)
    {
      वर्ण ध्रुव *tmp_dir = getenv ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  सचे (nfiles == 0)
    {
      nfiles = 1;
      मुक्त (files);
      files = xmalloc (परिमाण *files);
      *files = (वर्ण *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  सचे (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  सचे (checkonly)
    {
      सचे (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      सचे (outfile)
        {
          स्थिर वर्ण opts[] = {0, 'o', 0};
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

  सचे (mergeonly)
    {
      संरचना sortfile *sortfiles = xcalloc (nfiles, परिमाण *sortfiles);

      अत्थाय (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  अञ्ञथा
    {
      सचे (!nthreads)
        {
          unsigned long पूर्णाङ्क np = अङ्कः_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * परिमाण (संरचना merge_ग्रन्थिः));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  सचे (have_read_stdin && fclose (stdin) == EOF)
    sort_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
