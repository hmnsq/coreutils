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
संरचना rlimit { माप_प्रकार rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sort"

#define AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#जर HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#जर !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#जर !defined OPEN_MAX
# define OPEN_MAX 20
#endif

#define UCHAR_LIM (अच_अक्षर_उच्च + 1)

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
गणना { SUBTHREAD_ओळी_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_ओळी_HEURISTIC);

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
    MERGE_मूळ = 1
  };

/* The representation of the decimal point in the current locale.  */
स्थिर अक्षर decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
स्थिर पूर्णांक thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
स्थिर bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
स्थिर bool hard_LC_COLLATE;
#जर HAVE_NL_LANGINFO
स्थिर bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
गणना blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
स्थिर अक्षर eolchar = '\n';

/* Lines are held in memory as counted strings. */
संरचना ओळ
{
  अक्षर *text;			/* Text of the line. */
  माप_प्रकार length;		/* Length including final newline. */
  अक्षर *keybeg;			/* Start of first key. */
  अक्षर *keylim;			/* Limit of first key. */
};

/* Input buffers. */
संरचना buffer
{
  अक्षर *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  माप_प्रकार used;			/* Number of bytes used for input data.  */
  माप_प्रकार nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  माप_प्रकार डावा;			/* Number of bytes left from previous reads. */
  माप_प्रकार ओळ_bytes;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
संरचना sort_buffer_policy
{
  माप_प्रकार initial;
  माप_प्रकार limit;
  bool growth_failed;
};

/* Sort key.  */
संरचना keyfield
{
  माप_प्रकार sword;			/* Zero-origin 'word' to start at. */
  माप_प्रकार schar;			/* Additional characters to skip. */
  माप_प्रकार eword;			/* Zero-origin last 'word' of key. */
  माप_प्रकार echar;			/* Additional characters in field. */
  bool स्थिरांक *ignore;		/* Boolean array of characters to ignore. */
  अक्षर स्थिरांक *translate;	/* Translation applied to characters. */
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
  अक्षर स्थिरांक *name;
  पूर्णांक val;
};

/* Binary merge tree node. */
संरचना merge_गाठ
{
  संरचना ओळ *lo;              /* Lines to merge from LO child node. */
  संरचना ओळ *hi;              /* Lines to merge from HI child node. */
  संरचना ओळ *end_lo;          /* End of available lines from LO. */
  संरचना ओळ *end_hi;          /* End of available lines from HI. */
  संरचना ओळ **dest;           /* Pointer to destination of merge. */
  माप_प्रकार nlo;                   /* Total Lines remaining from LO. */
  माप_प्रकार nhi;                   /* Total lines remaining from HI. */
  संरचना merge_गाठ *parent;    /* Parent node. */
  संरचना merge_गाठ *lo_child;  /* LO child node. */
  संरचना merge_गाठ *hi_child;  /* HI child node. */
  अचिन्हित पूर्णांक level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
संरचना merge_गाठ_queue
{
  संरचना heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
स्थिर संरचना ओळ saved_ओळ;

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
स्थिर अक्षर fold_toupper[UCHAR_LIM];

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
#define MIN_MERGE_BUFFER_SIZE (2 + आकार (संरचना ओळ))

/* Minimum sort size; the code might not work with smaller sizes.  */
#define MIN_SORT_SIZE (nmerge * MIN_MERGE_BUFFER_SIZE)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
स्थिर माप_प्रकार merge_buffer_size = MAX (MIN_MERGE_BUFFER_SIZE, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
स्थिर माप_प्रकार sort_size;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#define आदान_FILE_SIZE_GUESS (128 * 1024)

स्थिर संरचना
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  अक्षर स्थिरांक **dirs COUNTED_BY (n_dirs);
} temp_dirs;



/* Flag to reverse the order of all comparisons. */
स्थिर bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
स्थिर bool stable;

/* An int value outside char range.  */
गणना { NON_CHAR = अक्षर_उच्च + 1 };

/* If TAB has this value, blanks separate fields.  */
गणना { TAB_DEFAULT = अक्षर_उच्च + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
स्थिर पूर्णांक tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
स्थिर bool unique;

/* Nonzero if any of the input files are the standard input. */
स्थिर bool have_read_stdin;

/* List of key field comparisons to be tried.  */
स्थिर संरचना keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
स्थिर अक्षर स्थिरांक *compress_program;

/* Annotate the output with extra info to aid the user.  */
स्थिर bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
स्थिर अचिन्हित पूर्णांक nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
स्थिर bool default_SIGPIPE;

/* The list of temporary files. */
संरचना tempnode
{
  संरचना tempnode *अस्थिर next;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  अक्षर state;
  अक्षर name[FLEXIBLE_ARRAY_MEMBER];
};
स्थिर संरचना tempnode *अस्थिर temphead;
स्थिर संरचना tempnode *अस्थिर *temptail = &temphead;

/* Clean up any remaining temporary files.  */

स्थिर शून्य
cleanup (शून्य)
{
  साठी (संरचना tempnode स्थिरांक *गाठ = temphead; गाठ; गाठ = गाठ->next)
    unlink (गाठ->name);
  temphead = रिक्त;
}

/* Handle interrupts and hangups. */

स्थिर शून्य
sighandler (पूर्णांक sig)
{
  cleanup ();

  संकेत (sig, संक_पूर्व);
  उठवा (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

स्थिर शून्य
sort_die (अक्षर स्थिरांक *message, अक्षर स्थिरांक *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  जर (त्रुटि_सं == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, त्रुटि_सं, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Write sorted concatenation of all खाता(s) to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      ख_माला_दो (_("\
Ordering options:\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding sort किल्ल्या in each ओळ\n\
"));
      oputs (_("\
  -d, --dictionary-order\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-स्थिती\n\
         fold lower स्थिती to upper स्थिती characters\n\
"));
      oputs (_("\
  -g, --general-numeric-sort\n\
         compare according to general numerical मूल्य\n\
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
         compare according to string numerical मूल्य;\n\
         see full documentation साठी supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical किल्ल्या.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-source=खाता\n\
         get random bytes from खाता\n\
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
      ख_माला_दो (_("\
\n\
Other options:\n\
\n\
"), मानक_निकास);
      oputs (_("\
      --batch-size=NMERGE\n\
         merge at most NMERGE inputs at once; साठी more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check साठी sorted आदान; करा not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but करा not report first bad ओळ\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the ओळ used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ आदान from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, पठ names from standard आदान\n\
"));
      oputs (_("\
  -k, --किल्ली=KEYDEF\n\
         sort via a किल्ली; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; करा not sort\n\
"));
      oputs (_("\
  -o, --output=खाता\n\
         लिख result to खाता instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --buffer-size=SIZE\n\
         use SIZE साठी main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=सूची\n\
         use सूची साठी temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the संख्या of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of ओळी with equal किल्ल्या;\n\
         with -c, check साठी strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] साठी start and stop position, where F is a\n\
field संख्या and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the ओळ's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options साठी that किल्ली.  If no किल्ली is given, use\n\
the entire ओळ as the किल्ली.  Use --debug to diagnose incorrect किल्ली usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), मानक_निकास);
      ख_माला_दो (_("\
% 1% of memory, b 1, K 1024 (पूर्वनिर्धारित), and so on साठी M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte मूल्ये.\n\
"), मानक_निकास );
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  CHECK_OPTION = अक्षर_उच्च + 1,
  COMPRESS_PROGRAM_OPTION,
  DEBUG_PROGRAM_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_SOURCE_OPTION,
  SORT_OPTION,
  PARALLEL_OPTION
};

स्थिर अक्षर स्थिरांक short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

स्थिर संरचना option स्थिरांक long_options[] =
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

स्थिर अक्षर स्थिरांक *स्थिरांक check_args[] =
{
#define _ct_(_s, _c) _s,
  CHECK_TABLE रिक्त
#undef  _ct_
};
स्थिर अक्षर स्थिरांक check_types[] =
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

स्थिर अक्षर स्थिरांक *स्थिरांक sort_args[] =
{
#define _st_(_s, _c) _s,
  SORT_TABLE रिक्त
#undef  _st_
};
स्थिर अक्षर स्थिरांक sort_types[] =
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
  पूर्णांक ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
स्थिर शून्य
cs_leave (संरचना cs_status स्थिरांक *status)
{
  जर (status->valid)
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
  अक्षर स्थिरांक *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  संरचना tempnode *temp;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
स्थिर Hash_table *proctab;

गणना { INIT_PROCTAB_SIZE = 47 };

स्थिर माप_प्रकार
proctab_hasher (शून्य स्थिरांक *entry, माप_प्रकार tabsize)
{
  संरचना tempnode स्थिरांक *गाठ = entry;
  परत गाठ->pid % tabsize;
}

स्थिर bool
proctab_comparator (शून्य स्थिरांक *e1, शून्य स्थिरांक *e2)
{
  संरचना tempnode स्थिरांक *n1 = e1;
  संरचना tempnode स्थिरांक *n2 = e2;
  परत n1->pid == n2->pid;
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
  पूर्णांक status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  जर (cpid < 0)
    error (SORT_FAILURE, त्रुटि_सं, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  अन्यथा जर (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      जर (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  परत cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

स्थिर शून्य
register_proc (संरचना tempnode *temp)
{
  जर (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, रिक्त,
                                 proctab_hasher,
                                 proctab_comparator,
                                 रिक्त);
      जर (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  जर (! hash_घाला (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

स्थिर bool
delete_proc (pid_t pid)
{
  संरचना tempnode test;

  test.pid = pid;
  संरचना tempnode *गाठ = hash_remove (proctab, &test);
  जर (! गाठ)
    परत false;
  गाठ->state = REAPED;
  परत true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

स्थिर शून्य
wait_proc (pid_t pid)
{
  जर (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

स्थिर शून्य
reap_exited (शून्य)
{
  पर्यंत (0 < nprocs && reap (0))
    सुरूठेवा;
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
  पर्यंत (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

स्थिर शून्य
exit_cleanup (शून्य)
{
  जर (temphead)
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
create_temp_file (पूर्णांक *pfd, bool survive_fd_exhaustion)
{
  स्थिर अक्षर स्थिरांक slashbase[] = "/sortXXXXXX";
  स्थिर idx_t temp_dir_index;
  पूर्णांक fd;
  पूर्णांक saved_errno;
  अक्षर स्थिरांक *temp_dir = temp_dirs.dirs[temp_dir_index];
  माप_प्रकार len = म_माप (temp_dir);
  संरचना tempnode *गाठ =
    xmalloc (FLEXSIZEOF (संरचना tempnode, name, len + आकार slashbase));
  अक्षर *file = गाठ->name;
  संरचना cs_status cs;

  स_नकल (file, temp_dir, len);
  स_नकल (file + len, slashbase, आकार slashbase);
  गाठ->next = रिक्त;
  जर (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  जर (0 <= fd)
    {
      *temptail = गाठ;
      temptail = &गाठ->next;
    }
  saved_errno = त्रुटि_सं;
  cs_leave (&cs);
  त्रुटि_सं = saved_errno;

  जर (fd < 0)
    {
      जर (! (survive_fd_exhaustion && त्रुटि_सं == EMFILE))
        error (SORT_FAILURE, त्रुटि_सं, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      मुक्त (गाठ);
      गाठ = रिक्त;
    }

  *pfd = fd;
  परत गाठ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

स्थिर संरचना stat *
get_outstatus (शून्य)
{
  स्थिर पूर्णांक outstat_errno;
  स्थिर संरचना stat outstat;
  जर (outstat_errno == 0)
    outstat_errno = ख_स्थिति (STDOUT_FILENO, &outstat) == 0 ? -1 : त्रुटि_सं;
  परत outstat_errno < 0 ? &outstat : रिक्त;
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

स्थिर खाता *
stream_open (अक्षर स्थिरांक *file, अक्षर स्थिरांक *how)
{
  खाता *fp;

  जर (*how == 'r')
    {
      जर (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = मानक_निवेश;
        }
      अन्यथा
        {
          पूर्णांक fd = खोलो (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? रिक्त : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  अन्यथा जर (*how == 'w')
    {
      जर (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          पूर्णांक ftruncate_errno = त्रुटि_सं;
          संरचना stat *outst = get_outstatus ();
          जर (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = मानक_निकास;
    }
  अन्यथा
    affirm (!"unexpected mode passed to stream_open");

  परत fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

स्थिर खाता *
xfopen (अक्षर स्थिरांक *file, अक्षर स्थिरांक *how)
{
  खाता *fp = stream_open (file, how);
  जर (!fp)
    sort_die (_("open failed"), file);
  परत fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

स्थिर शून्य
xfclose (खाता *fp, अक्षर स्थिरांक *file)
{
  निवड (fileno (fp))
    {
    स्थिती STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      साफ_त्रुटि (fp);
      थांबा;

    स्थिती STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      जर (ख_साफ (fp) != 0)
        sort_die (_("fflush failed"), file);
      थांबा;

    पूर्वनिर्धारित:
      जर (ख_बंद (fp) != 0)
        sort_die (_("close failed"), file);
      थांबा;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

स्थिर शून्य
move_fd (पूर्णांक oldfd, पूर्णांक newfd)
{
  जर (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_मूल्य (dup2 (oldfd, newfd));
      ignore_मूल्य (बंद (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

स्थिर पूर्णांक
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  पूर्णांक oldfd, पूर्णांक newfd)
{
  पूर्णांक result = 0;
  जर (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      जर (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  परत result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

स्थिर अक्षर स्थिरांक *
get_resolved_compress_program (शून्य)
{
  /* Use a cache, to perform the search only once.  */
  स्थिर अक्षर स्थिरांक *resolved_compress_program_cache /* = NULL */;

  जर (resolved_compress_program_cache == रिक्त)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, दो_पर्या ("PATH"), रिक्त, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  परत resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

स्थिर पूर्णांक
pipe_child (pid_t *pid, पूर्णांक pipefds[2], पूर्णांक tempfd, bool decompress,
            माप_प्रकार tries)
{
  अक्षर स्थिरांक *resolved_compress_program;
  संरचना tempnode *saved_temphead;
  द्विगुण wait_retry = 0.25;
  संरचना cs_status cs;
  पूर्णांक result;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  जर (resolved_compress_program == रिक्त)
    परत त्रुटि_सं;

  जर ((result = posix_spawnattr_init (&attr)))
    परत result;
  जर ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      परत result;
    }

  जर (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      परत saved_errno;
    }

  जर ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      बंद (pipefds[0]);
      बंद (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      परत result;
    }

  अक्षर स्थिरांक *स्थिरांक argv[] =
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

  जर (nmerge + 1 < nprocs)
    reap_some ();

  पर्यंत (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = रिक्त;

      result = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (अक्षर * स्थिरांक *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      जर (result != EAGAIN)
        थांबा;
      अन्यथा
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

  जर (result)
    {
      बंद (pipefds[0]);
      बंद (pipefds[1]);
    }
  अन्यथा
    ++nprocs;

  परत result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

स्थिर संरचना tempnode *
maybe_create_temp (खाता **pfp, bool survive_fd_exhaustion)
{
  पूर्णांक tempfd;
  संरचना tempnode *गाठ = create_temp_file (&tempfd, survive_fd_exhaustion);
  जर (! गाठ)
    परत रिक्त;

  गाठ->state = UNCOMPRESSED;

  जर (compress_program)
    {
      पूर्णांक pipefds[2];
      स्थिर पूर्णांक last_result = 0;

      पूर्णांक result = pipe_child (&गाठ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      जर (result)
        {
          जर (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      अन्यथा
        {
          बंद (tempfd);
          बंद (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (गाठ);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  जर (! *pfp)
    sort_die (_("couldn't create temporary file"), गाठ->name);

  परत गाठ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

स्थिर संरचना tempnode *
create_temp (खाता **pfp)
{
  परत maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

स्थिर खाता *
open_temp (संरचना tempnode *temp)
{
  पूर्णांक tempfd, pipefds[2];
  खाता *fp = रिक्त;

  जर (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = खोलो (temp->name, O_RDONLY);
  जर (tempfd < 0)
    परत रिक्त;

  pid_t child;
  पूर्णांक result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  जर (result)
    {
      जर (result != EMFILE)
        error (SORT_FAILURE, result, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      बंद (tempfd);
      त्रुटि_सं = EMFILE;
    }
  अन्यथा
    {
      temp->pid = child;
      register_proc (temp);
      बंद (tempfd);
      बंद (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      जर (! fp)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          बंद (pipefds[0]);
          त्रुटि_सं = saved_errno;
        }
    }

  परत fp;
}

/* Append DIR to the array of temporary directory names.  */
स्थिर शून्य
add_temp_dir (अक्षर स्थिरांक *dir)
{
  जर (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
    temp_dirs.dirs = xpalloc (temp_dirs.dirs, &temp_dirs.n_dirs_alloc, 1, -1,
                              आकार *temp_dirs.dirs);

  idx_t स्थिरांक dir_idx = temp_dirs.n_dirs++;
  temp_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

स्थिर शून्य
zaptemp (अक्षर स्थिरांक *name)
{
  संरचना tempnode *अस्थिर *pnode;
  संरचना tempnode *गाठ;
  संरचना tempnode *next;
  पूर्णांक unlink_status;
  पूर्णांक unlink_errno = 0;
  संरचना cs_status cs;

  साठी (pnode = &temphead; (गाठ = *pnode)->name != name; pnode = &गाठ->next)
    सुरूठेवा;

  जर (गाठ->state == UNREAPED)
    wait_proc (गाठ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = गाठ->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = त्रुटि_सं;
  *pnode = next;
  cs_leave (&cs);

  जर (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  जर (! next)
    temptail = pnode;
  मुक्त (गाठ);
}

#जर HAVE_NL_LANGINFO

स्थिर पूर्णांक
struct_month_cmp (शून्य स्थिरांक *m1, शून्य स्थिरांक *m2)
{
  संरचना month स्थिरांक *month1 = m1;
  संरचना month स्थिरांक *month2 = m2;
  परत म_भेद (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

स्थिर शून्य
inittables (शून्य)
{
  साठी (माप_प्रकार i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! है_अक्षर_अंक (i);
      nonprinting[i] = ! है_छाप (i);
      fold_toupper[i] = बड़े (i);
    }

#जर HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  जर (hard_LC_TIME)
    {
      साठी (माप_प्रकार i = 0; i < MONTHS_PER_YEAR; i++)
        {
          अक्षर स्थिरांक *s;
          माप_प्रकार s_len;
          माप_प्रकार j, k;
          अक्षर *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = म_माप (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          साठी (j = k = 0; j < s_len; j++)
            जर (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      क्विक (monthtab, MONTHS_PER_YEAR, आकार *monthtab, struct_month_cmp);
    }
#endif
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
स्थिर शून्य
specify_nmerge (पूर्णांक oi, अक्षर c, अक्षर स्थिरांक *s)
{
  uintmax_t n;
  संरचना rlimit rlimit;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  अचिन्हित पूर्णांक max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  जर (e == LONGINT_OK)
    {
      nmerge = n;
      जर (nmerge != n)
        e = LONGINT_OVERFLOW;
      अन्यथा
        {
          जर (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          अन्यथा जर (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          अन्यथा
            परत;
        }
    }

  जर (e == LONGINT_OVERFLOW)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (SORT_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  अन्यथा
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
स्थिर शून्य
specify_sort_size (पूर्णांक oi, अक्षर c, अक्षर स्थिरांक *s)
{
  uintmax_t n;
  अक्षर *suffix;
  गणना strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  जर (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  जर (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    निवड (suffix[0])
      {
      स्थिती 'b':
        e = LONGINT_OK;
        थांबा;

      स्थिती '%':
        {
          द्विगुण mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          जर (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          अन्यथा
            e = LONGINT_OVERFLOW;
        }
        थांबा;
      }

  जर (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      जर (n < sort_size)
        परत;

      sort_size = n;
      जर (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          परत;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
स्थिर माप_प्रकार
specify_nthreads (पूर्णांक oi, अक्षर c, अक्षर स्थिरांक *s)
{
  uintmax_t nthreads;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &nthreads, "");
  जर (e == LONGINT_OVERFLOW)
    परत SIZE_MAX;
  जर (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  जर (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  जर (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  परत nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
स्थिर माप_प्रकार
default_sort_size (शून्य)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  माप_प्रकार size = SIZE_MAX;
  संरचना rlimit rlimit;
  जर (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  जर (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  जर (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  द्विगुण avail = physmem_available ();
  द्विगुण total = physmem_total ();
  द्विगुण mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  जर (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  जर (mem < size)
    size = mem;
  परत MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

स्थिर bool
ओळ_aligned_size (माप_प्रकार *alloc)
{
  माप_प्रकार size = *alloc;
  माप_प्रकार alignment = आकार (संरचना ओळ);
  माप_प्रकार padding = alignment - size % alignment;
  माप_प्रकार aligned;

  जर (ckd_add (&aligned, size, padding))
    परत false;

  *alloc = aligned;
  परत true;
}

/* Return the default initial allocation for a growable sort buffer.  */

स्थिर माप_प्रकार
default_initial_sort_size (माप_प्रकार ओळ_bytes)
{
  माप_प्रकार size = ओळ_bytes + 2;
  माप_प्रकार आदान_size;

  जर (ckd_mul (&आदान_size, आदान_FILE_SIZE_GUESS, ओळ_bytes + 1)
      || ckd_add (&size, size, आदान_size))
    परत SIZE_MAX;

  परत MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

स्थिर bool
आदान_size_buffer_bytes (uintmax_t file_size, माप_प्रकार ओळ_bytes,
                         माप_प्रकार *worst_case)
{
  माप_प्रकार worst_case_per_आदान_byte = ओळ_bytes + 1;

  जर (SIZE_MAX / worst_case_per_आदान_byte < file_size)
    परत false;

  माप_प्रकार size = file_size * worst_case_per_आदान_byte;
  जर (SIZE_MAX - size < 1)
    परत false;

  *worst_case = size + 1;
  परत true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

स्थिर शून्य
sort_buffer_policy (खाता *स्थिरांक *fps, माप_प्रकार nfps,
                    अक्षर *स्थिरांक *files, माप_प्रकार nfiles,
                    माप_प्रकार ओळ_bytes,
                    संरचना sort_buffer_policy *policy)
{
  माप_प्रकार size_bound = sort_size ? sort_size : default_sort_size ();
  माप_प्रकार initial_bound =
    sort_size ? size_bound : default_initial_sort_size (ओळ_bytes);

  /* In the worst case, each input byte is a newline.  */
  माप_प्रकार worst_case_per_आदान_byte = ओळ_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  माप_प्रकार size = worst_case_per_आदान_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  साठी (माप_प्रकार i = 0; i < nfiles; i++)
    {
      संरचना stat st;
      off_t file_size;
      माप_प्रकार worst_case;
      bool known_size;

      जर (size_bound <= size)
        {
          policy->initial = size_bound;
          परत;
        }

      जर ((i < nfps ? ख_स्थिति (fileno (fps[i]), &st)
           : streq (files[i], "-") ? ख_स्थिति (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      जर (known_size)
        file_size = st.st_size;
      अन्यथा
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          जर (sort_size)
            {
              policy->initial = size_bound;
              परत;
            }
          file_size = आदान_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      जर (! आदान_size_buffer_bytes (file_size, ओळ_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          परत;
        }

      जर (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          परत;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

स्थिर शून्य
initbuf (संरचना buffer *buf, माप_प्रकार ओळ_bytes, माप_प्रकार alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  पर्यंत (true)
    {
      जर (! ओळ_aligned_size (&alloc))
        buf->buf = रिक्त;
      अन्यथा
        buf->buf = दो_स्मृति (alloc);
      जर (buf->buf)
        थांबा;
      alloc /= 2;
      जर (alloc <= ओळ_bytes + 1)
        xalloc_die ();
    }

  buf->ओळ_bytes = ओळ_bytes;
  buf->alloc = alloc;
  buf->used = buf->डावा = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

स्थिर ओळीत संरचना ओळ *
buffer_linelim (संरचना buffer स्थिरांक *buf)
{
  शून्य *linelim = buf->buf + buf->alloc;
  परत linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

स्थिर bool
try_growbuf (संरचना buffer *buf, माप_प्रकार alloc)
{
  जर (! ओळ_aligned_size (&alloc) || alloc <= buf->alloc)
    परत false;

  अक्षर *newbuf = दो_स्मृति (alloc);
  जर (! newbuf)
    परत false;

  अक्षर *oldbuf = buf->buf;
  संरचना ओळ *old_linelim = buffer_linelim (buf);
  संरचना ओळ *old_ओळ = old_linelim - buf->nlines;

  स_नकल (newbuf, oldbuf, buf->used);

  संरचना ओळ *new_linelim = (शून्य *) (newbuf + alloc);
  संरचना ओळ *new_ओळ = new_linelim - buf->nlines;
  स_नकल (new_ओळ, old_ओळ, buf->nlines * आकार *new_ओळ);

  साठी (संरचना ओळ *ओळ = new_ओळ; ओळ < new_linelim; ओळ++)
    {
      ओळ->text = newbuf + (ओळ->text - oldbuf);
      जर (keylist)
        {
          ओळ->keybeg = newbuf + (ओळ->keybeg - oldbuf);
          ओळ->keylim = newbuf + (ओळ->keylim - oldbuf);
        }
    }

  मुक्त (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  परत true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

स्थिर bool
maybe_growbuf (संरचना buffer *buf, संरचना sort_buffer_policy *policy)
{
  जर (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    परत false;

  माप_प्रकार alloc;
  जर (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  अन्यथा
    alloc = policy->limit;

  जर (try_growbuf (buf, alloc))
    परत true;

  policy->growth_failed = true;
  परत false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

स्थिर अक्षर *
begfield (संरचना ओळ स्थिरांक *ओळ, संरचना keyfield स्थिरांक *किल्ली)
{
  अक्षर *ptr = ओळ->text, *lim = ptr + ओळ->length - 1;
  माप_प्रकार sword = किल्ली->sword;
  माप_प्रकार schar = किल्ली->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  जर (tab != TAB_DEFAULT)
    पर्यंत (ptr < lim && sword--)
      {
        अक्षर *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        जर (ptr < lim)
          ++ptr;
      }
  अन्यथा
    पर्यंत (ptr < lim && sword--)
      {
        पर्यंत (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        पर्यंत (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  जर (किल्ली->skipsblanks)
    पर्यंत (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  माप_प्रकार remaining_bytes = lim - ptr;
  जर (schar < remaining_bytes)
    ptr += schar;
  अन्यथा
    ptr = lim;

  परत ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
स्थिर अक्षर *
limfield (संरचना ओळ स्थिरांक *ओळ, संरचना keyfield स्थिरांक *किल्ली)
{
  अक्षर *ptr = ओळ->text, *lim = ptr + ओळ->length - 1;
  माप_प्रकार eword = किल्ली->eword, echar = किल्ली->echar;

  जर (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  जर (tab != TAB_DEFAULT)
    पर्यंत (ptr < lim && eword--)
      {
        अक्षर *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        जर (ptr < lim && (eword || echar))
          ++ptr;
      }
  अन्यथा
    पर्यंत (ptr < lim && eword--)
      {
        पर्यंत (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        पर्यंत (ptr < lim && !blanks[to_uchar (*ptr)])
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
  जर (tab != TAB_DEFAULT)
    {
      अक्षर *newlim;
      newlim = स_प्रथम (ptr, tab, lim - ptr);
      जर (newlim)
        lim = newlim;
    }
  अन्यथा
    {
      अक्षर *newlim;
      newlim = ptr;
      पर्यंत (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      पर्यंत (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  जर (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      जर (किल्ली->skipeblanks)
        पर्यंत (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      माप_प्रकार remaining_bytes = lim - ptr;
      जर (echar < remaining_bytes)
        ptr += echar;
      अन्यथा
        ptr = lim;
    }

  परत ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

स्थिर bool
fillbuf (संरचना buffer *buf, खाता *fp, अक्षर स्थिरांक *file,
         संरचना sort_buffer_policy *policy)
{
  संरचना keyfield स्थिरांक *किल्ली = keylist;
  अक्षर eol = eolchar;
  माप_प्रकार ओळ_bytes = buf->ओळ_bytes;
  माप_प्रकार mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  जर (buf->eof)
    परत false;

  जर (buf->used != buf->डावा)
    {
      स_हटाओ (buf->buf, buf->buf + buf->used - buf->डावा, buf->डावा);
      buf->used = buf->डावा;
      buf->nlines = 0;
    }

  पर्यंत (true)
    {
      अक्षर *ptr = buf->buf + buf->used;
      संरचना ओळ *linelim = buffer_linelim (buf);
      संरचना ओळ *ओळ = linelim - buf->nlines;
      माप_प्रकार avail = (अक्षर *) linelim - buf->nlines * ओळ_bytes - ptr;
      अक्षर *ओळ_start = buf->nlines ? ओळ->text + ओळ->length : buf->buf;

      पर्यंत (ओळ_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          माप_प्रकार readsize = (avail - 1) / (ओळ_bytes + 1);
          माप_प्रकार bytes_read = ख_पढ़ो (ptr, 1, readsize, fp);
          अक्षर *ptrlim = ptr + bytes_read;
          अक्षर *p;
          avail -= bytes_read;

          जर (bytes_read != readsize)
            {
              जर (ख_त्रुटि (fp))
                sort_die (_("read failed"), file);
              जर (ख_पूर्ण (fp))
                {
                  buf->eof = true;
                  जर (buf->buf == ptrlim)
                    परत false;
                  जर (ओळ_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          पर्यंत ((p = स_प्रथम (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              ओळ--;
              ओळ->text = ओळ_start;
              ओळ->length = ptr - ओळ_start;
              mergesize = MAX (mergesize, ओळ->length);
              avail -= ओळ_bytes;

              जर (किल्ली)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  ओळ->keylim = (किल्ली->eword == SIZE_MAX
                                  ? p
                                  : limfield (ओळ, किल्ली));

                  जर (किल्ली->sword != SIZE_MAX)
                    ओळ->keybeg = begfield (ओळ, किल्ली);
                  अन्यथा
                    {
                      जर (किल्ली->skipsblanks)
                        पर्यंत (blanks[to_uchar (*ओळ_start)])
                          ओळ_start++;
                      ओळ->keybeg = ओळ_start;
                    }
                }

              ओळ_start = ptr;
            }

          ptr = ptrlim;
          जर (buf->eof)
            थांबा;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - ओळ;
      जर (buf->nlines != 0)
        {
          जर (! buf->eof && maybe_growbuf (buf, policy))
            सुरूठेवा;
          buf->डावा = ptr - ओळ_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          परत true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t ओळ_alloc = buf->alloc / आकार (संरचना ओळ);
        buf->buf = xpalloc (buf->buf, &ओळ_alloc, 1, -1, आकार (संरचना ओळ));
        buf->alloc = ओळ_alloc * आकार (संरचना ओळ);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
स्थिर अक्षर स्थिरांक unit_order[UCHAR_LIM] =
  {
#जर ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#अन्यथा
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
स्थिर अक्षर
traverse_raw_संख्या (अक्षर स्थिरांक **संख्या)
{
  अक्षर स्थिरांक *p = *संख्या;
  अक्षर ch;
  अक्षर max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  पर्यंत (c_isdigit (ch = *p++))
    {
      जर (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      जर (ends_with_thousands_sep)
        ++p;
    }

  जर (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *संख्या = p - 2;
      परत max_digit;
    }

  जर (ch == decimal_point)
    पर्यंत (c_isdigit (ch = *p++))
      जर (max_digit < ch)
        max_digit = ch;

  *संख्या = p - 1;
  परत max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
स्थिर पूर्णांक
find_unit_order (अक्षर स्थिरांक *संख्या)
{
  bool minus_sign = (*संख्या == '-');
  अक्षर स्थिरांक *p = संख्या + minus_sign;
  अक्षर max_digit = traverse_raw_संख्या (&p);
  जर ('0' < max_digit)
    {
      अचिन्हित अक्षर ch = *p;
      पूर्णांक order = unit_order[ch];
      परत (minus_sign ? -order : order);
    }
  अन्यथा
    परत 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
स्थिर पूर्णांक
human_numcompare (अक्षर स्थिरांक *a, अक्षर स्थिरांक *b)
{
  पर्यंत (blanks[to_uchar (*a)])
    a++;
  पर्यंत (blanks[to_uchar (*b)])
    b++;

  पूर्णांक diff = find_unit_order (a) - find_unit_order (b);
  परत (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
स्थिर पूर्णांक
numcompare (अक्षर स्थिरांक *a, अक्षर स्थिरांक *b)
{
  पर्यंत (blanks[to_uchar (*a)])
    a++;
  पर्यंत (blanks[to_uchar (*b)])
    b++;

  परत strnumcmp (a, b, decimal_point, thousands_sep);
}

स्थिर पूर्णांक
nan_compare (दीर्घ द्विगुण a, दीर्घ द्विगुण b)
{
  अक्षर buf[2][आकार "-nan""()" + अक्षर_बिट * आकार a];
  snprintf (buf[0], आकार buf[0], "%Lf", a);
  snprintf (buf[1], आकार buf[1], "%Lf", b);
  परत म_भेद (buf[0], buf[1]);
}

स्थिर पूर्णांक
general_numcompare (अक्षर स्थिरांक *sa, अक्षर स्थिरांक *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  अक्षर *ea;
  अक्षर *eb;
  दीर्घ द्विगुण a = strtold (sa, &ea);
  दीर्घ द्विगुण b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  जर (sa == ea)
    परत sb == eb ? 0 : -1;
  जर (sb == eb)
    परत 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  परत (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

स्थिर पूर्णांक
getmonth (अक्षर स्थिरांक *month, अक्षर **ea)
{
  माप_प्रकार lo = 0;
  माप_प्रकार hi = MONTHS_PER_YEAR;

  पर्यंत (blanks[to_uchar (*month)])
    month++;

  करा
    {
      माप_प्रकार ix = (lo + hi) / 2;
      अक्षर स्थिरांक *m = month;
      अक्षर स्थिरांक *n = monthtab[ix].name;

      साठी (;; m++, n++)
        {
          जर (!*n)
            {
              जर (ea)
                *ea = (अक्षर *) m;
              परत monthtab[ix].val;
            }
          जर (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              थांबा;
            }
          अन्यथा जर (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              थांबा;
            }
        }
    }
  पर्यंत (lo < hi);

  परत 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#जर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# जर 14 <= __GNUC__
#  pragma GCC diagnostic ढकला
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#अन्तर्भाव "md5.h"

#जर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# जर 14 <= __GNUC__
#  pragma GCC diagnostic काढा
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
चिन्ह_address (शून्य *handle, अक्षर स्थिरांक *चिन्ह)
{
  शून्य *address = dlsym (handle, चिन्ह);
  जर (!address)
    link_failure ();
  परत address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
स्थिर शून्य
link_libcrypto (शून्य)
{
#जर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  शून्य *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  जर (!handle)
    link_failure ();
  ptr_MD5_Init = चिन्ह_address (handle, "MD5_Init");
  ptr_MD5_Update = चिन्ह_address (handle, "MD5_Update");
  ptr_MD5_Final = चिन्ह_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
स्थिर संरचना md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

स्थिर शून्य
random_md5_state_init (अक्षर स्थिरांक *random_source)
{
  अचिन्हित अक्षर buf[MD5_DIGEST_SIZE];
  संरचना randread_source *r = randread_new (random_source, आकार buf);
  जर (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, आकार buf);
  जर (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, आकार buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

स्थिर माप_प्रकार
xstrxfrm (अक्षर *restrict dest, अक्षर स्थिरांक *restrict src, माप_प्रकार destsize)
{
  त्रुटि_सं = 0;
  माप_प्रकार translated_size = strxfrm (dest, src, destsize);

  जर (त्रुटि_सं)
    {
      error (0, त्रुटि_सं, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  परत translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

स्थिर पूर्णांक
compare_random (अक्षर *restrict texta, माप_प्रकार lena,
                अक्षर *restrict textb, माप_प्रकार lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  पूर्णांक xfrm_diff = 0;

  अक्षर stackbuf[4000];
  अक्षर *buf = stackbuf;
  माप_प्रकार bufsize = आकार stackbuf;
  शून्य *allocated = रिक्त;
  uint32_t dig[2][MD5_DIGEST_SIZE / आकार (uint32_t)];
  संरचना md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  जर (hard_LC_COLLATE)
    {
      अक्षर स्थिरांक *lima = texta + lena;
      अक्षर स्थिरांक *limb = textb + lenb;

      पर्यंत (true)
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
          माप_प्रकार guess_bufsize = 3 * (lena + lenb) + 2;
          जर (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              मुक्त (allocated);
              buf = allocated = दो_स्मृति (bufsize);
              जर (! buf)
                {
                  buf = stackbuf;
                  bufsize = आकार stackbuf;
                }
            }

          माप_प्रकार sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          माप_प्रकार sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : रिक्त), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          जर (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              जर (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              मुक्त (allocated);
              buf = allocated = xmalloc (bufsize);
              जर (texta < lima)
                strxfrm (buf, texta, sizea);
              जर (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          जर (texta < lima)
            texta += म_माप (texta) + 1;
          जर (textb < limb)
            textb += म_माप (textb) + 1;
          जर (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              थांबा;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          जर (! xfrm_diff)
            {
              xfrm_diff = स_भेद (buf, buf + sizea, MIN (sizea, sizeb));
              जर (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  पूर्णांक diff = स_भेद (dig[0], dig[1], आकार dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  जर (! diff)
    {
      जर (! xfrm_diff)
        {
          xfrm_diff = स_भेद (texta, textb, MIN (lena, lenb));
          जर (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  मुक्त (allocated);

  परत diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

स्थिर माप_प्रकार
debug_width (अक्षर स्थिरांक *text, अक्षर स्थिरांक *lim)
{
  माप_प्रकार width = mbsnwidth (text, lim - text, 0);
  पर्यंत (text < lim)
    width += (*text++ == '\t');
  परत width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

स्थिर शून्य
mark_किल्ली (माप_प्रकार offset, माप_प्रकार width)
{
  पर्यंत (offset--)
    अक्षर_दो (' ');

  जर (!width)
    म_लिखो (_("^ no match for key\n"));
  अन्यथा
    {
      करा
        अक्षर_दो ('_');
      पर्यंत (--width);

      अक्षर_दो ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

स्थिर ओळीत bool
किल्ली_numeric (संरचना keyfield स्थिरांक *किल्ली)
{
  परत किल्ली->numeric || किल्ली->general_numeric || किल्ली->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

स्थिर शून्य
debug_किल्ली (संरचना ओळ स्थिरांक *ओळ, संरचना keyfield स्थिरांक *किल्ली)
{
  अक्षर *text = ओळ->text;
  अक्षर *beg = text;
  अक्षर *lim = text + ओळ->length - 1;

  जर (किल्ली)
    {
      जर (किल्ली->sword != SIZE_MAX)
        beg = begfield (ओळ, किल्ली);
      जर (किल्ली->eword != SIZE_MAX)
        {
          lim = limfield (ओळ, किल्ली);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      जर ((किल्ली->skipsblanks && किल्ली->sword == SIZE_MAX)
          || किल्ली->month || किल्ली_numeric (किल्ली))
        {
          अक्षर saved = *lim;
          *lim = '\0';

          पर्यंत (blanks[to_uchar (*beg)])
            beg++;

          अक्षर *tighter_lim = beg;

          जर (lim < beg)
            tighter_lim = lim;
          अन्यथा जर (किल्ली->month)
            getmonth (beg, &tighter_lim);
          अन्यथा जर (किल्ली->general_numeric)
            ignore_मूल्य (strtold (beg, &tighter_lim));
          अन्यथा जर (किल्ली->numeric || किल्ली->human_numeric)
            {
              अक्षर स्थिरांक *p = beg + (beg < lim && *beg == '-');
              अक्षर max_digit = traverse_raw_संख्या (&p);
              जर ('0' <= max_digit)
                {
                  अचिन्हित अक्षर ch = *p;
                  tighter_lim = (अक्षर *) p
                    + (किल्ली->human_numeric && unit_order[ch]);
                }
            }
          अन्यथा
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  माप_प्रकार offset = debug_width (text, beg);
  माप_प्रकार width = debug_width (beg, lim);
  mark_किल्ली (offset, width);
}

/* Debug LINE by underlining its keys.  */

स्थिर शून्य
debug_ओळ (संरचना ओळ स्थिरांक *ओळ)
{
  संरचना keyfield स्थिरांक *किल्ली = keylist;

  करा
    debug_किल्ली (ओळ, किल्ली);
  पर्यंत (किल्ली && ((किल्ली = किल्ली->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

स्थिर bool
default_किल्ली_compare (संरचना keyfield स्थिरांक *किल्ली)
{
  परत ! (किल्ली->ignore
            || किल्ली->translate
            || किल्ली->skipsblanks
            || किल्ली->skipeblanks
            || किल्ली_numeric (किल्ली)
            || किल्ली->month
            || किल्ली->version
            || किल्ली->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

स्थिर शून्य
किल्ली_to_opts (संरचना keyfield स्थिरांक *किल्ली, अक्षर *opts)
{
  जर (किल्ली->skipsblanks || किल्ली->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  जर (किल्ली->ignore == nondictionary)
    *opts++ = 'd';
  जर (किल्ली->translate)
    *opts++ = 'f';
  जर (किल्ली->general_numeric)
    *opts++ = 'g';
  जर (किल्ली->human_numeric)
    *opts++ = 'h';
  जर (किल्ली->ignore == nonprinting)
    *opts++ = 'i';
  जर (किल्ली->month)
    *opts++ = 'M';
  जर (किल्ली->numeric)
    *opts++ = 'n';
  जर (किल्ली->random)
    *opts++ = 'R';
  जर (किल्ली->reverse)
    *opts++ = 'r';
  जर (किल्ली->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

स्थिर शून्य
किल्ली_warnings (संरचना keyfield स्थिरांक *gkey, bool gkey_only)
{
  संरचना keyfield ugkey = *gkey;
  अचिन्हित दीर्घ keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  साठी (संरचना keyfield *किल्ली = keylist; किल्ली; किल्ली = किल्ली->next, keynum++)
    {
      जर (किल्ली_numeric (किल्ली))
        {
          जर (किल्ली->general_numeric)
            general_numeric_field = true;
          अन्यथा
            basic_numeric_field = true;
        }

      जर (किल्ली->traditional_used)
        {
          माप_प्रकार sword = किल्ली->sword;
          माप_प्रकार eword = किल्ली->eword;
          अक्षर tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          अक्षर obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          अक्षर nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          अक्षर *po = obuf;
          अक्षर *pn = nbuf;

          जर (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          जर (किल्ली->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (किल्ली->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = किल्ली->sword != SIZE_MAX && किल्ली->eword < किल्ली->sword;
      जर (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = किल्ली_numeric (किल्ली) || किल्ली->month;
      bool ओळ_offset = किल्ली->eword == 0 && किल्ली->echar != 0; /* -k1.x,1.y  */
      जर (!zero_width && !gkey_only && tab == TAB_DEFAULT && !ओळ_offset
          && ((!किल्ली->skipsblanks && !implicit_skip)
              || (!किल्ली->skipsblanks && किल्ली->schar)
              || (!किल्ली->skipeblanks && किल्ली->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      जर (!gkey_only && किल्ली_numeric (किल्ली))
        {
          माप_प्रकार sword = किल्ली->sword + 1;
          माप_प्रकार eword = किल्ली->eword + 1;
          जर (!sword)
            sword++;
          जर (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              जर (किल्ली->general_numeric)
                general_numeric_field_span = true;
              अन्यथा
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      जर (ugkey.ignore && (ugkey.ignore == किल्ली->ignore))
        ugkey.ignore = रिक्त;
      जर (ugkey.translate && (ugkey.translate == किल्ली->translate))
        ugkey.translate = रिक्त;
      ugkey.skipsblanks &= !किल्ली->skipsblanks;
      ugkey.skipeblanks &= !किल्ली->skipeblanks;
      ugkey.month &= !किल्ली->month;
      ugkey.numeric &= !किल्ली->numeric;
      ugkey.general_numeric &= !किल्ली->general_numeric;
      ugkey.human_numeric &= !किल्ली->human_numeric;
      ugkey.random &= !किल्ली->random;
      ugkey.version &= !किल्ली->version;
      ugkey.reverse &= !किल्ली->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool संख्या_locale_warned = false;
  जर (basic_numeric_field_span)
    {
      जर (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((अक्षर []) {thousands_sep, 0})));
          संख्या_locale_warned = true;
        }
    }
  जर (basic_numeric_field_span || general_numeric_field_span)
    {
      जर (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((अक्षर []) {decimal_point, 0})));
          संख्या_locale_warned = true;
        }
      अन्यथा जर (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((अक्षर []) {tab, 0})));
        }
      अन्यथा जर (general_numeric_field_span && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((अक्षर []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  जर ((basic_numeric_field || general_numeric_field) && ! संख्या_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((अक्षर []) {decimal_point, 0})));

    }

  जर (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  जर (!default_किल्ली_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      जर (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      अक्षर opts[आकार short_options];
      किल्ली_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (म_माप (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  जर (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

स्थिर पूर्णांक
diff_reversed (पूर्णांक diff, bool reversed)
{
  परत reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

स्थिर पूर्णांक
keycompare (संरचना ओळ स्थिरांक *a, संरचना ओळ स्थिरांक *b)
{
  संरचना keyfield *किल्ली = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  अक्षर *texta = a->keybeg;
  अक्षर *textb = b->keybeg;
  अक्षर *lima = a->keylim;
  अक्षर *limb = b->keylim;

  पूर्णांक diff;

  पर्यंत (true)
    {
      अक्षर स्थिरांक *translate = किल्ली->translate;
      bool स्थिरांक *ignore = किल्ली->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      माप_प्रकार lena = lima - texta;
      माप_प्रकार lenb = limb - textb;

      जर (hard_LC_COLLATE || किल्ली_numeric (किल्ली)
          || किल्ली->month || किल्ली->random || किल्ली->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          अक्षर *ta = texta;
          अक्षर *tb = textb;
          माप_प्रकार tlena = lena;
          माप_प्रकार tlenb = lenb;
          अक्षर enda = ta[tlena];
          अक्षर endb = tb[tlenb];

          शून्य *allocated = रिक्त;
          अक्षर stackbuf[4000];

          जर (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              माप_प्रकार i;

              /* Allocate space for copies.  */
              माप_प्रकार size = lena + 1 + lenb + 1;
              जर (size <= आकार stackbuf)
                ta = stackbuf;
              अन्यथा
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              साठी (tlena = i = 0; i < lena; i++)
                जर (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              साठी (tlenb = i = 0; i < lenb; i++)
                जर (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          जर (किल्ली->numeric)
            diff = numcompare (ta, tb);
          अन्यथा जर (किल्ली->general_numeric)
            diff = general_numcompare (ta, tb);
          अन्यथा जर (किल्ली->human_numeric)
            diff = human_numcompare (ta, tb);
          अन्यथा जर (किल्ली->month)
            diff = getmonth (ta, रिक्त) - getmonth (tb, रिक्त);
          अन्यथा जर (किल्ली->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          अन्यथा जर (किल्ली->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          अन्यथा
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              जर (tlena == 0)
                diff = - NONZERO (tlenb);
              अन्यथा जर (tlenb == 0)
                diff = 1;
              अन्यथा
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          मुक्त (allocated);
        }
      अन्यथा जर (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  करा									\
    {									\
          पर्यंत (true)							\
            {								\
              पर्यंत (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              पर्यंत (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              जर (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  थांबा;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              जर (diff)							\
                थांबा;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  पर्यंत (0)

          जर (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          अन्यथा
            CMP_WITH_IGNORE (*texta, *textb);
        }
      अन्यथा
        {
          माप_प्रकार lenmin = MIN (lena, lenb);
          जर (lenmin == 0)
            diff = 0;
          अन्यथा जर (translate)
            {
              माप_प्रकार i = 0;
              करा
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  जर (diff)
                    थांबा;
                  i++;
                }
              पर्यंत (i < lenmin);
            }
          अन्यथा
            diff = स_भेद (texta, textb, lenmin);

          जर (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      जर (diff)
        थांबा;

      किल्ली = किल्ली->next;
      जर (! किल्ली)
        परत 0;

      /* Find the beginning and limit of the next field.  */
      जर (किल्ली->eword != SIZE_MAX)
        lima = limfield (a, किल्ली), limb = limfield (b, किल्ली);
      अन्यथा
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      जर (किल्ली->sword != SIZE_MAX)
        texta = begfield (a, किल्ली), textb = begfield (b, किल्ली);
      अन्यथा
        {
          texta = a->text, textb = b->text;
          जर (किल्ली->skipsblanks)
            {
              पर्यंत (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              पर्यंत (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  परत diff_reversed (diff, किल्ली->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

स्थिर पूर्णांक
compare (संरचना ओळ स्थिरांक *a, संरचना ओळ स्थिरांक *b)
{
  पूर्णांक diff;
  माप_प्रकार alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  जर (keylist)
    {
      diff = keycompare (a, b);
      जर (diff || unique || stable)
        परत diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  जर (alen == 0)
    diff = - NONZERO (blen);
  अन्यथा जर (blen == 0)
    diff = 1;
  अन्यथा जर (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->text, alen + 1, b->text, blen + 1);
    }
  अन्यथा
    {
      diff = स_भेद (a->text, b->text, MIN (alen, blen));
      जर (!diff)
        diff = _GL_CMP (alen, blen);
    }

  परत diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

स्थिर शून्य
write_ओळ (संरचना ओळ स्थिरांक *ओळ, खाता *fp, अक्षर स्थिरांक *output_file)
{
  अक्षर *buf = ओळ->text;
  माप_प्रकार n_bytes = ओळ->length;
  अक्षर *ebuf = buf + n_bytes;

  जर (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      अक्षर स्थिरांक *c = buf;

      पर्यंत (c < ebuf)
        {
          अक्षर wc = *c++;
          जर (wc == '\t')
            wc = '>';
          अन्यथा जर (c == ebuf)
            wc = '\n';
          जर (ख_अक्षर_दो (wc, fp) == खातापूर्ण)
            sort_die (_("write failed"), output_file);
        }

      debug_ओळ (ओळ);
    }
  अन्यथा
    {
      ebuf[-1] = eolchar;
      जर (ख_डालो (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

स्थिर bool
check (अक्षर स्थिरांक *file_name, अक्षर checkonly)
{
  खाता *fp = xfopen (file_name, "r");
  संरचना buffer buf;		/* Input buffer. */
  संरचना ओळ temp;		/* Copy of previous line. */
  माप_प्रकार alloc = 0;
  uintmax_t ओळ_संख्या = 0;
  संरचना keyfield स्थिरांक *किल्ली = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, आकार (संरचना ओळ),
           MAX (merge_buffer_size, sort_size));
  temp.text = रिक्त;

  पर्यंत (fillbuf (&buf, fp, file_name, रिक्त))
    {
      संरचना ओळ स्थिरांक *ओळ = buffer_linelim (&buf);
      संरचना ओळ स्थिरांक *linebase = ओळ - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      जर (alloc && nonunique <= compare (&temp, ओळ - 1))
        {
        found_disorder:
          {
            जर (checkonly == 'c')
              {
                संरचना ओळ स्थिरांक *disorder_ओळ = ओळ - 1;
                uintmax_t disorder_ओळ_संख्या =
                  buffer_linelim (&buf) - disorder_ओळ + ओळ_संख्या;
                ख_लिखो (मानक_त्रुटि, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_ओळ_संख्या);
                write_ओळ (disorder_ओळ, मानक_त्रुटि, _("standard error"));
              }

            ordered = false;
            थांबा;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      पर्यंत (linebase < --ओळ)
        जर (nonunique <= compare (ओळ, ओळ - 1))
          जा found_disorder;

      ओळ_संख्या += buf.nlines;

      /* Save the last line of the buffer.  */
      जर (alloc < ओळ->length)
        {
          करा
            {
              alloc *= 2;
              जर (! alloc)
                {
                  alloc = ओळ->length;
                  थांबा;
                }
            }
          पर्यंत (alloc < ओळ->length);

          मुक्त (temp.text);
          temp.text = xmalloc (alloc);
        }
      स_नकल (temp.text, ओळ->text, ओळ->length);
      temp.length = ओळ->length;
      जर (किल्ली)
        {
          temp.keybeg = temp.text + (ओळ->keybeg - ओळ->text);
          temp.keylim = temp.text + (ओळ->keylim - ओळ->text);
        }
    }

  xfclose (fp, file_name);
  मुक्त (buf.buf);
  मुक्त (temp.text);
  परत ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

स्थिर माप_प्रकार
open_आदान_files (संरचना sortfile *files, माप_प्रकार nfiles, खाता ***pfps)
{
  खाता **fps = *pfps = xnmalloc (nfiles, आकार *fps);
  पूर्णांक i;

  /* Open as many input files as we can.  */
  साठी (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      जर (!fps[i])
        थांबा;
    }

  परत i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

स्थिर शून्य
mergefps (संरचना sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
          खाता *ofp, अक्षर स्थिरांक *output_file, खाता **fps)
{
  संरचना buffer *buffer = xnmalloc (nfiles, आकार *buffer);
                                /* Input buffers for each file. */
  संरचना ओळ saved;		/* Saved line storage for unique check. */
  संरचना ओळ स्थिरांक *savedline = रिक्त;
                                /* &saved if there is a saved line. */
  माप_प्रकार savealloc = 0;		/* Size allocated for the saved line. */
  संरचना ओळ स्थिरांक **cur = xnmalloc (nfiles, आकार *cur);
                                /* Current line in each line table. */
  संरचना ओळ स्थिरांक **base = xnmalloc (nfiles, आकार *base);
                                /* Base of each line table.  */
  माप_प्रकार *ord = xnmalloc (nfiles, आकार *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  माप_प्रकार t;
  संरचना keyfield स्थिरांक *किल्ली = keylist;
  saved.text = रिक्त;

  /* Read initial lines from each input file. */
  साठी (माप_प्रकार i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], आकार (संरचना ओळ),
               MAX (merge_buffer_size, sort_size / nfiles));
      जर (fillbuf (&buffer[i], fps[i], files[i].name, रिक्त))
        {
          संरचना ओळ स्थिरांक *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      अन्यथा
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          जर (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          मुक्त (buffer[i].buf);
          --nfiles;
          साठी (माप_प्रकार j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  साठी (माप_प्रकार i = 0; i < nfiles; ++i)
    ord[i] = i;
  साठी (माप_प्रकार i = 1; i < nfiles; ++i)
    जर (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  पर्यंत (nfiles)
    {
      संरचना ओळ स्थिरांक *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      जर (unique)
        {
          जर (savedline && compare (savedline, smallest))
            {
              savedline = रिक्त;
              write_ओळ (&saved, ofp, output_file);
            }
          जर (!savedline)
            {
              savedline = &saved;
              जर (savealloc < smallest->length)
                {
                  करा
                    जर (! savealloc)
                      {
                        savealloc = smallest->length;
                        थांबा;
                      }
                  पर्यंत ((savealloc *= 2) < smallest->length);

                  मुक्त (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              स_नकल (saved.text, smallest->text, saved.length);
              जर (किल्ली)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      अन्यथा
        write_ओळ (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      जर (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      अन्यथा
        {
          जर (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       रिक्त))
            {
              संरचना ओळ स्थिरांक *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          अन्यथा
            {
              /* We reached EOF on fps[ord[0]].  */
              साठी (माप_प्रकार i = 1; i < nfiles; ++i)
                जर (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              जर (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              मुक्त (buffer[ord[0]].buf);
              साठी (माप_प्रकार i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              साठी (माप_प्रकार i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              सुरूठेवा;
            }
        }

      /* The new line just read in may be larger than other lines
         already in main memory; push it back in the queue until we
         encounter a line larger than it.  Optimize for the common
         case where the new line is smallest.  */
      {
        माप_प्रकार lo = 1;
        माप_प्रकार hi = nfiles;
        माप_प्रकार probe = lo;
        माप_प्रकार ord0 = ord[0];
        माप_प्रकार मोजणी_of_smaller_ओळी;

        पर्यंत (lo < hi)
          {
            पूर्णांक cmp = compare (cur[ord0], cur[ord[probe]]);
            जर (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            अन्यथा
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        मोजणी_of_smaller_ओळी = lo - 1;
        साठी (माप_प्रकार j = 0; j < मोजणी_of_smaller_ओळी; j++)
          ord[j] = ord[j + 1];
        ord[मोजणी_of_smaller_ओळी] = ord0;
      }
    }

  जर (unique && savedline)
    {
      write_ओळ (&saved, ofp, output_file);
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

स्थिर माप_प्रकार
mergefiles (संरचना sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
            खाता *ofp, अक्षर स्थिरांक *output_file)
{
  खाता **fps;
  माप_प्रकार nopened = open_आदान_files (files, nfiles, &fps);
  जर (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  परत nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

स्थिर शून्य
mergelines (संरचना ओळ *restrict t, माप_प्रकार nlines,
            संरचना ओळ स्थिरांक *restrict lo)
{
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  संरचना ओळ *hi = t - nlo;

  पर्यंत (true)
    जर (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        जर (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            परत;
          }
      }
    अन्यथा
      {
        *--t = *--hi;
        जर (! --nhi)
          {
            करा
              *--t = *--lo;
            पर्यंत (--nlo);

            परत;
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
sequential_sort (संरचना ओळ *restrict ओळी, माप_प्रकार nlines,
                 संरचना ओळ *restrict temp, bool to_temp)
{
  जर (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      पूर्णांक swap = (0 < compare (&ओळी[-1], &ओळी[-2]));
      जर (to_temp)
        {
          temp[-1] = ओळी[-1 - swap];
          temp[-2] = ओळी[-2 + swap];
        }
      अन्यथा जर (swap)
        {
          temp[-1] = ओळी[-1];
          ओळी[-1] = ओळी[-2];
          ओळी[-2] = temp[-1];
        }
    }
  अन्यथा
    {
      माप_प्रकार nlo = nlines / 2;
      माप_प्रकार nhi = nlines - nlo;
      संरचना ओळ *lo = ओळी;
      संरचना ओळ *hi = ओळी - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      जर (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      अन्यथा जर (!to_temp)
        temp[-1] = lo[-1];

      संरचना ओळ *dest;
      संरचना ओळ स्थिरांक *sorted_lo;
      जर (to_temp)
        {
          dest = temp;
          sorted_lo = ओळी;
        }
      अन्यथा
        {
          dest = ओळी;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

स्थिर संरचना merge_गाठ *init_गाठ (संरचना merge_गाठ *restrict,
                                     संरचना merge_गाठ *restrict,
                                     संरचना ओळ *, माप_प्रकार, माप_प्रकार, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
स्थिर संरचना merge_गाठ *
merge_वृक्ष_init (माप_प्रकार nthreads, माप_प्रकार nlines, संरचना ओळ *dest)
{
  संरचना merge_गाठ *merge_वृक्ष = xmalloc (2 * आकार *merge_वृक्ष * nthreads);

  संरचना merge_गाठ *मूळ = merge_वृक्ष;
  मूळ->lo = मूळ->hi = मूळ->end_lo = मूळ->end_hi = रिक्त;
  मूळ->dest = रिक्त;
  मूळ->nlo = मूळ->nhi = nlines;
  मूळ->parent = रिक्त;
  मूळ->level = MERGE_END;
  मूळ->queued = false;
  pthread_mutex_init (&मूळ->lock, रिक्त);

  init_गाठ (मूळ, मूळ + 1, dest, nthreads, nlines, false);
  परत merge_वृक्ष;
}

/* Destroy the merge tree. */
स्थिर शून्य
merge_वृक्ष_destroy (माप_प्रकार nthreads, संरचना merge_गाठ *merge_वृक्ष)
{
  माप_प्रकार n_nodes = nthreads * 2;
  संरचना merge_गाठ *गाठ = merge_वृक्ष;

  पर्यंत (n_nodes--)
    {
      pthread_mutex_destroy (&गाठ->lock);
      गाठ++;
    }

  मुक्त (merge_वृक्ष);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

स्थिर संरचना merge_गाठ *
init_गाठ (संरचना merge_गाठ *restrict parent,
           संरचना merge_गाठ *restrict गाठ_pool,
           संरचना ओळ *dest, माप_प्रकार nthreads,
           माप_प्रकार total_ओळी, bool is_lo_child)
{
  माप_प्रकार nlines = (is_lo_child ? parent->nlo : parent->nhi);
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  संरचना ओळ *lo = dest - total_ओळी;
  संरचना ओळ *hi = lo - nlo;
  संरचना ओळ **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  संरचना merge_गाठ *गाठ = गाठ_pool++;
  गाठ->lo = गाठ->end_lo = lo;
  गाठ->hi = गाठ->end_hi = hi;
  गाठ->dest = parent_end;
  गाठ->nlo = nlo;
  गाठ->nhi = nhi;
  गाठ->parent = parent;
  गाठ->level = parent->level + 1;
  गाठ->queued = false;
  pthread_mutex_init (&गाठ->lock, रिक्त);

  जर (nthreads > 1)
    {
      माप_प्रकार lo_threads = nthreads / 2;
      माप_प्रकार hi_threads = nthreads - lo_threads;
      गाठ->lo_child = गाठ_pool;
      गाठ_pool = init_गाठ (गाठ, गाठ_pool, lo, lo_threads,
                             total_ओळी, true);
      गाठ->hi_child = गाठ_pool;
      गाठ_pool = init_गाठ (गाठ, गाठ_pool, hi, hi_threads,
                             total_ओळी, false);
    }
  अन्यथा
    {
      गाठ->lo_child = रिक्त;
      गाठ->hi_child = रिक्त;
    }
  परत गाठ_pool;
}


/* Compare two merge nodes A and B for priority.  */

स्थिर पूर्णांक
compare_nodes (शून्य स्थिरांक *a, शून्य स्थिरांक *b)
{
  संरचना merge_गाठ स्थिरांक *nodea = a;
  संरचना merge_गाठ स्थिरांक *nodeb = b;
  जर (nodea->level == nodeb->level)
      परत (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  परत nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

स्थिर ओळीत शून्य
lock_गाठ (संरचना merge_गाठ *गाठ)
{
  pthread_mutex_lock (&गाठ->lock);
}

/* Unlock a merge tree NODE. */

स्थिर ओळीत शून्य
unlock_गाठ (संरचना merge_गाठ *गाठ)
{
  pthread_mutex_unlock (&गाठ->lock);
}

/* Destroy merge QUEUE. */

स्थिर शून्य
queue_destroy (संरचना merge_गाठ_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

स्थिर शून्य
queue_init (संरचना merge_गाठ_queue *queue, माप_प्रकार nthreads)
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
queue_घाला (संरचना merge_गाठ_queue *queue, संरचना merge_गाठ *गाठ)
{
  pthread_mutex_lock (&queue->mutex);
  heap_घाला (queue->priority_queue, गाठ);
  गाठ->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

स्थिर संरचना merge_गाठ *
queue_काढा (संरचना merge_गाठ_queue *queue)
{
  संरचना merge_गाठ *गाठ;
  pthread_mutex_lock (&queue->mutex);
  पर्यंत (! (गाठ = heap_remove_शीर्ष (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_गाठ (गाठ);
  गाठ->queued = false;
  परत गाठ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

स्थिर शून्य
write_unique (संरचना ओळ स्थिरांक *ओळ, खाता *tfp, अक्षर स्थिरांक *temp_output)
{
  जर (unique)
    {
      जर (saved_ओळ.text && ! compare (ओळ, &saved_ओळ))
        परत;
      saved_ओळ = *ओळ;
    }

  write_ओळ (ओळ, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

स्थिर शून्य
mergelines_गाठ (संरचना merge_गाठ *restrict गाठ, माप_प्रकार total_ओळी,
                 खाता *tfp, अक्षर स्थिरांक *temp_output)
{
  संरचना ओळ *lo_orig = गाठ->lo;
  संरचना ओळ *hi_orig = गाठ->hi;
  माप_प्रकार to_merge = MAX_MERGE (total_ओळी, गाठ->level);
  माप_प्रकार merged_lo;
  माप_प्रकार merged_hi;

  जर (गाठ->level > MERGE_मूळ)
    {
      /* Merge to destination buffer. */
      संरचना ओळ *dest = *गाठ->dest;
      पर्यंत (गाठ->lo != गाठ->end_lo && गाठ->hi != गाठ->end_hi && to_merge--)
        जर (compare (गाठ->lo - 1, गाठ->hi - 1) <= 0)
          *--dest = *--गाठ->lo;
        अन्यथा
          *--dest = *--गाठ->hi;

      merged_lo = lo_orig - गाठ->lo;
      merged_hi = hi_orig - गाठ->hi;

      जर (गाठ->nhi == merged_hi)
        पर्यंत (गाठ->lo != गाठ->end_lo && to_merge--)
          *--dest = *--गाठ->lo;
      अन्यथा जर (गाठ->nlo == merged_lo)
        पर्यंत (गाठ->hi != गाठ->end_hi && to_merge--)
          *--dest = *--गाठ->hi;
      *गाठ->dest = dest;
    }
  अन्यथा
    {
      /* Merge directly to output. */
      पर्यंत (गाठ->lo != गाठ->end_lo && गाठ->hi != गाठ->end_hi && to_merge--)
        {
          जर (compare (गाठ->lo - 1, गाठ->hi - 1) <= 0)
            write_unique (--गाठ->lo, tfp, temp_output);
          अन्यथा
            write_unique (--गाठ->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - गाठ->lo;
      merged_hi = hi_orig - गाठ->hi;

      जर (गाठ->nhi == merged_hi)
        {
          पर्यंत (गाठ->lo != गाठ->end_lo && to_merge--)
            write_unique (--गाठ->lo, tfp, temp_output);
        }
      अन्यथा जर (गाठ->nlo == merged_lo)
        {
          पर्यंत (गाठ->hi != गाठ->end_hi && to_merge--)
            write_unique (--गाठ->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - गाठ->lo;
  merged_hi = hi_orig - गाठ->hi;
  गाठ->nlo -= merged_lo;
  गाठ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

स्थिर शून्य
queue_check_घाला (संरचना merge_गाठ_queue *queue, संरचना merge_गाठ *गाठ)
{
  जर (! गाठ->queued)
    {
      bool lo_avail = (गाठ->lo - गाठ->end_lo) != 0;
      bool hi_avail = (गाठ->hi - गाठ->end_hi) != 0;
      जर (lo_avail ? hi_avail || ! गाठ->nhi : hi_avail && ! गाठ->nlo)
        queue_घाला (queue, गाठ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

स्थिर शून्य
queue_check_घाला_parent (संरचना merge_गाठ_queue *queue,
                           संरचना merge_गाठ *गाठ)
{
  जर (गाठ->level > MERGE_मूळ)
    {
      lock_गाठ (गाठ->parent);
      queue_check_घाला (queue, गाठ->parent);
      unlock_गाठ (गाठ->parent);
    }
  अन्यथा जर (गाठ->nlo + गाठ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_घाला (queue, गाठ->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

स्थिर शून्य
merge_loop (संरचना merge_गाठ_queue *queue,
            माप_प्रकार total_ओळी, खाता *tfp, अक्षर स्थिरांक *temp_output)
{
  पर्यंत (true)
    {
      संरचना merge_गाठ *गाठ = queue_काढा (queue);

      जर (गाठ->level == MERGE_END)
        {
          unlock_गाठ (गाठ);
          /* Reinsert so other threads can pop it. */
          queue_घाला (queue, गाठ);
          थांबा;
        }
      mergelines_गाठ (गाठ, total_ओळी, tfp, temp_output);
      queue_check_घाला (queue, गाठ);
      queue_check_घाला_parent (queue, गाठ);

      unlock_गाठ (गाठ);
    }
}


स्थिर शून्य sortlines (संरचना ओळ *restrict, माप_प्रकार, माप_प्रकार,
                       संरचना merge_गाठ *, संरचना merge_गाठ_queue *,
                       खाता *, अक्षर स्थिरांक *);

/* Thread arguments for sortlines_thread. */

संरचना thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  संरचना ओळ *ओळी;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  माप_प्रकार nthreads;

  /* Number of lines in LINES and DEST.  */
  माप_प्रकार स्थिरांक total_ओळी;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  संरचना merge_गाठ *स्थिरांक गाठ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  संरचना merge_गाठ_queue *स्थिरांक queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  खाता *tfp;
  अक्षर स्थिरांक *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

स्थिर शून्य *
sortlines_thread (शून्य *data)
{
  संरचना thread_args स्थिरांक *args = data;
  sortlines (args->ओळी, args->nthreads, args->total_ओळी,
             args->गाठ, args->queue, args->tfp,
             args->output_temp);
  परत रिक्त;
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
sortlines (संरचना ओळ *restrict ओळी, माप_प्रकार nthreads,
           माप_प्रकार total_ओळी, संरचना merge_गाठ *गाठ,
           संरचना merge_गाठ_queue *queue, खाता *tfp, अक्षर स्थिरांक *temp_output)
{
  माप_प्रकार nlines = गाठ->nlo + गाठ->nhi;

  /* Calculate thread arguments. */
  माप_प्रकार lo_threads = nthreads / 2;
  माप_प्रकार hi_threads = nthreads - lo_threads;
  pthread_t thread;
  संरचना thread_args args = {ओळी, lo_threads, total_ओळी,
                             गाठ->lo_child, queue, tfp, temp_output};

  जर (nthreads > 1 && SUBTHREAD_ओळी_HEURISTIC <= nlines
      && pthread_create (&thread, रिक्त, sortlines_thread, &args) == 0)
    {
      sortlines (ओळी - गाठ->nlo, hi_threads, total_ओळी,
                 गाठ->hi_child, queue, tfp, temp_output);
      pthread_join (thread, रिक्त);
    }
  अन्यथा
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      माप_प्रकार nlo = गाठ->nlo;
      माप_प्रकार nhi = गाठ->nhi;
      संरचना ओळ *temp = ओळी - total_ओळी;
      जर (1 < nhi)
        sequential_sort (ओळी - nlo, nhi, temp - nlo / 2, false);
      जर (1 < nlo)
        sequential_sort (ओळी, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      गाठ->lo = ओळी;
      गाठ->hi = ओळी - nlo;
      गाठ->end_lo = ओळी - nlo;
      गाठ->end_hi = ओळी - nlo - nhi;

      queue_घाला (queue, गाठ);
      merge_loop (queue, total_ओळी, tfp, temp_output);
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
avoid_trashing_आदान (संरचना sortfile *files, माप_प्रकार ntemps,
                      माप_प्रकार nfiles, अक्षर स्थिरांक *outfile)
{
  संरचना tempnode *tempcopy = रिक्त;

  साठी (माप_प्रकार i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      संरचना stat instat;

      जर (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      अन्यथा
        {
          संरचना stat *outst = get_outstatus ();
          जर (!outst)
            थांबा;

          same = (((is_stdin
                    ? ख_स्थिति (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      जर (same)
        {
          जर (! tempcopy)
            {
              खाता *tftp;
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
check_inputs (अक्षर *स्थिरांक *files, माप_प्रकार nfiles)
{
  साठी (माप_प्रकार i = 0; i < nfiles; i++)
    {
      जर (streq (files[i], "-"))
        सुरूठेवा;

      जर (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

स्थिर शून्य
check_output (अक्षर स्थिरांक *outfile)
{
  जर (outfile)
    {
      पूर्णांक oflags = O_WRONLY | O_द्विमान | O_CLOEXEC | O_CREAT;
      पूर्णांक outfd = खोलो (outfile, oflags, MODE_RW_UGO);
      जर (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

स्थिर शून्य
merge (संरचना sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
       अक्षर स्थिरांक *output_file)
{
  पर्यंत (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      माप_प्रकार in;

      /* Number of output files generated so far.  */
      माप_प्रकार out;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      माप_प्रकार remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      माप_प्रकार cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      साठी (out = in = 0; nmerge <= nfiles - in; out++)
        {
          खाता *tfp;
          संरचना tempnode *temp = create_temp (&tfp);
          माप_प्रकार अंक_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, अंक_merged);
          files[out].name = temp->name;
          files[out].temp = temp;
          in += अंक_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      जर (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          माप_प्रकार nshortmerge = remainder - cheap_slots + 1;
          खाता *tfp;
          संरचना tempnode *temp = create_temp (&tfp);
          माप_प्रकार अंक_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, temp->name);
          ntemps -= MIN (ntemps, अंक_merged);
          files[out].name = temp->name;
          files[out++].temp = temp;
          in += अंक_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      स_हटाओ (&files[out], &files[in], (nfiles - in) * आकार *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_आदान (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  पर्यंत (true)
    {
      /* Merge directly into the output file if possible.  */
      खाता **fps;
      माप_प्रकार nopened = open_आदान_files (files, nfiles, &fps);

      जर (nopened == nfiles)
        {
          खाता *ofp = stream_open (output_file, "w");
          जर (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              थांबा;
            }
          जर (त्रुटि_सं != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      अन्यथा जर (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      खाता *tfp;
      संरचना tempnode *temp;
      करा
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      पर्यंत (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      स_हटाओ (&files[1], &files[nopened], (nfiles - nopened) * आकार *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

स्थिर शून्य
sort (अक्षर *स्थिरांक *files, माप_प्रकार nfiles, अक्षर स्थिरांक *output_file,
      माप_प्रकार nthreads)
{
  संरचना buffer buf;
  संरचना sort_buffer_policy policy;
  माप_प्रकार ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  पर्यंत (nfiles)
    {
      अक्षर स्थिरांक *temp_output;
      अक्षर स्थिरांक *file = *files;
      खाता *fp = xfopen (file, "r");
      खाता *tfp;

      माप_प्रकार bytes_per_ओळ;
      जर (nthreads > 1)
        {
          /* Get log P. */
          माप_प्रकार tmp = 1;
          माप_प्रकार mult = 1;
          पर्यंत (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_ओळ = (mult * आकार (संरचना ओळ));
        }
      अन्यथा
        bytes_per_ओळ = आकार (संरचना ओळ) * 3 / 2;

      जर (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_ओळ,
                              &policy);
          initbuf (&buf, bytes_per_ओळ, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      पर्यंत (fillbuf (&buf, fp, file, &policy))
        {
          संरचना ओळ *ओळ;

          जर (buf.eof && nfiles
              && (bytes_per_ओळ + 1
                  < (buf.alloc - buf.used - bytes_per_ओळ * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.डावा = buf.used;
              थांबा;
            }

          saved_ओळ.text = रिक्त;
          ओळ = buffer_linelim (&buf);
          जर (buf.eof && !nfiles && !ntemps && !buf.डावा)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              temp_output = output_file;
              output_file_created = true;
            }
          अन्यथा
            {
              ++ntemps;
              temp_output = create_temp (&tfp)->name;
            }
          जर (1 < buf.nlines)
            {
              जर (nthreads > 1)
                {
                  संरचना merge_गाठ_queue queue;
                  queue_init (&queue, nthreads);
                  संरचना merge_गाठ *merge_वृक्ष =
                    merge_वृक्ष_init (nthreads, buf.nlines, ओळ);

                  sortlines (ओळ, nthreads, buf.nlines, merge_वृक्ष + 1,
                             &queue, tfp, temp_output);

                  merge_वृक्ष_destroy (nthreads, merge_वृक्ष);
                  queue_destroy (&queue);
                }
              अन्यथा
                {
                  sequential_sort (ओळ, buf.nlines,
                                   ओळ - buf.nlines, false);
                  साठी (माप_प्रकार i = 0; i < buf.nlines; i++)
                    write_unique (ओळ - i - 1, tfp, temp_output);
                }
            }
          अन्यथा
            write_unique (ओळ - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          जर (output_file_created)
            जा finish;
        }
      xfclose (fp, file);
    }

 finish:
  मुक्त (buf.buf);

  जर (! output_file_created)
    {
      संरचना tempnode *गाठ = temphead;
      संरचना sortfile *tempfiles = xnmalloc (ntemps, आकार *tempfiles);
      साठी (माप_प्रकार i = 0; गाठ; i++)
        {
          tempfiles[i].name = गाठ->name;
          tempfiles[i].temp = गाठ;
          गाठ = गाठ->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      मुक्त (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

स्थिर शून्य
insertkey (संरचना keyfield *किल्ली_arg)
{
  संरचना keyfield **p;
  संरचना keyfield *किल्ली = xmemdup (किल्ली_arg, आकार *किल्ली);

  साठी (p = &keylist; *p; p = &(*p)->next)
    सुरूठेवा;
  *p = किल्ली;
  किल्ली->next = रिक्त;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

स्थिर शून्य
badfieldspec (अक्षर स्थिरांक *spec, अक्षर स्थिरांक *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

स्थिर शून्य
incompatible_options (अक्षर स्थिरांक *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

स्थिर शून्य
check_ordering_compatibility (शून्य)
{
  साठी (संरचना keyfield *किल्ली = keylist; किल्ली; किल्ली = किल्ली->next)
    जर (1 < (किल्ली->numeric + किल्ली->general_numeric + किल्ली->human_numeric
             + किल्ली->month + (किल्ली->version | किल्ली->random | !!किल्ली->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        अक्षर opts[आकार short_options];
        /* Clear flags we're not interested in.  */
        किल्ली->skipsblanks = किल्ली->skipeblanks = किल्ली->reverse = false;
        किल्ली_to_opts (किल्ली, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

स्थिर अक्षर स्थिरांक *
parse_field_मोजणी (अक्षर स्थिरांक *string, माप_प्रकार *val, अक्षर स्थिरांक *msgid)
{
  अक्षर *suffix;
  uintmax_t n;

  निवड (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    स्थिती LONGINT_OK:
    स्थिती LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      जर (*val == n)
        थांबा;
      FALLTHROUGH;
    स्थिती LONGINT_OVERFLOW:
    स्थिती LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      थांबा;

    स्थिती LONGINT_INVALID:
      जर (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      परत रिक्त;
    }

  परत suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

स्थिर अक्षर *
set_ordering (अक्षर स्थिरांक *s, संरचना keyfield *किल्ली, गणना blanktype blanktype)
{
  पर्यंत (*s)
    {
      निवड (*s)
        {
        स्थिती 'b':
          जर (blanktype == bl_start || blanktype == bl_both)
            किल्ली->skipsblanks = true;
          जर (blanktype == bl_end || blanktype == bl_both)
            किल्ली->skipeblanks = true;
          थांबा;
        स्थिती 'd':
          किल्ली->ignore = nondictionary;
          थांबा;
        स्थिती 'f':
          किल्ली->translate = fold_toupper;
          थांबा;
        स्थिती 'g':
          किल्ली->general_numeric = true;
          थांबा;
        स्थिती 'h':
          किल्ली->human_numeric = true;
          थांबा;
        स्थिती 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          जर (! किल्ली->ignore)
            किल्ली->ignore = nonprinting;
          थांबा;
        स्थिती 'M':
          किल्ली->month = true;
          थांबा;
        स्थिती 'n':
          किल्ली->numeric = true;
          थांबा;
        स्थिती 'R':
          किल्ली->random = true;
          थांबा;
        स्थिती 'r':
          किल्ली->reverse = true;
          थांबा;
        स्थिती 'V':
          किल्ली->version = true;
          थांबा;
        पूर्वनिर्धारित:
          परत (अक्षर *) s;
        }
      ++s;
    }
  परत (अक्षर *) s;
}

/* Initialize KEY.  */

स्थिर संरचना keyfield *
किल्ली_init (संरचना keyfield *किल्ली)
{
  स_रखो (किल्ली, 0, आकार *किल्ली);
  किल्ली->eword = SIZE_MAX;
  परत किल्ली;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  संरचना keyfield *किल्ली;
  संरचना keyfield किल्ली_buf;
  संरचना keyfield gkey;
  bool gkey_only = false;
  अक्षर स्थिरांक *s;
  पूर्णांक c = 0;
  अक्षर checkonly = 0;
  bool mergeonly = false;
  अक्षर *random_source = रिक्त;
  bool need_random = false;
  माप_प्रकार nthreads = 0;
  माप_प्रकार nfiles = 0;
  bool posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != रिक्त);
  पूर्णांक posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  अक्षर **files;
  अक्षर *files_from = रिक्त;
  संरचना Tokens tok;
  अक्षर स्थिरांक *outfile = रिक्त;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#जर HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    संरचना स्था_बदल स्थिरांक *locale = क्षेत्र_बदलो ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    जर (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    जर (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    जर (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    गणना { nsigs = countof (term_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_signals);
    साठी (माप_प्रकार i = 0; i < nsigs; i++)
      {
        जर (term_sig[i] == SIGPIPE)
          सुरूठेवा;  /* Handled below.  */
        sigaction (term_sig[i], रिक्त, &act);
        जर (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    साठी (माप_प्रकार i = 0; i < nsigs; i++)
      {
        जर (term_sig[i] == SIGPIPE)
          सुरूठेवा;  /* Handled below.  */
        जर (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, रिक्त);
      }
  }
  संकेत (SIGCHLD, संक_पूर्व); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  निकास_पर (exit_cleanup);

  किल्ली_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, आकार *files);

  पर्यंत (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      पूर्णांक oi = -1;

      जर (c == -1
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
          जर (argc <= optind)
            थांबा;
          files[nfiles++] = argv[optind++];
        }
      अन्यथा निवड (c)
        {
        स्थिती 1:
          किल्ली = रिक्त;
          जर (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              जर (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  किल्ली = किल्ली_init (&किल्ली_buf);
                  s = parse_field_मोजणी (optarg + 1, &किल्ली->sword, रिक्त);
                  जर (s && *s == '.')
                    s = parse_field_मोजणी (s + 1, &किल्ली->schar, रिक्त);
                  जर (! (किल्ली->sword || किल्ली->schar))
                    किल्ली->sword = SIZE_MAX;
                  जर (! s || *set_ordering (s, किल्ली, bl_start))
                    किल्ली = रिक्त;
                  अन्यथा
                    {
                      जर (minus_pos_usage)
                        {
                          अक्षर स्थिरांक *optarg1 = argv[optind++];
                          s = parse_field_मोजणी (optarg1 + 1, &किल्ली->eword,
                                             N_("invalid number after '-'"));
                          जर (*s == '.')
                            s = parse_field_मोजणी (s + 1, &किल्ली->echar,
                                               N_("invalid number after '.'"));
                          जर (!किल्ली->echar && किल्ली->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              किल्ली->eword--;
                            }
                          जर (*set_ordering (s, किल्ली, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      किल्ली->traditional_used = true;
                      insertkey (किल्ली);
                    }
                }
            }
          जर (! किल्ली)
            files[nfiles++] = optarg;
          थांबा;

        स्थिती SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        स्थिती 'b':
        स्थिती 'd':
        स्थिती 'f':
        स्थिती 'g':
        स्थिती 'h':
        स्थिती 'i':
        स्थिती 'M':
        स्थिती 'n':
        स्थिती 'r':
        स्थिती 'R':
        स्थिती 'V':
          {
            अक्षर str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          थांबा;

        स्थिती CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        स्थिती 'c':
        स्थिती 'C':
          जर (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          थांबा;

        स्थिती COMPRESS_PROGRAM_OPTION:
          जर (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          थांबा;

        स्थिती DEBUG_PROGRAM_OPTION:
          debug = true;
          थांबा;

        स्थिती FILES0_FROM_OPTION:
          files_from = optarg;
          थांबा;

        स्थिती 'k':
          किल्ली = किल्ली_init (&किल्ली_buf);

          /* Get POS1. */
          s = parse_field_मोजणी (optarg, &किल्ली->sword,
                                 N_("invalid number at field start"));
          जर (! किल्ली->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          जर (*s == '.')
            {
              s = parse_field_मोजणी (s + 1, &किल्ली->schar,
                                     N_("invalid number after '.'"));
              जर (! किल्ली->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          जर (! (किल्ली->sword || किल्ली->schar))
            किल्ली->sword = SIZE_MAX;
          s = set_ordering (s, किल्ली, bl_start);
          जर (*s != ',')
            {
              किल्ली->eword = SIZE_MAX;
              किल्ली->echar = 0;
            }
          अन्यथा
            {
              /* Get POS2. */
              s = parse_field_मोजणी (s + 1, &किल्ली->eword,
                                     N_("invalid number after ','"));
              जर (! किल्ली->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              जर (*s == '.')
                {
                  s = parse_field_मोजणी (s + 1, &किल्ली->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, किल्ली, bl_end);
            }
          जर (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (किल्ली);
          थांबा;

        स्थिती 'm':
          mergeonly = true;
          थांबा;

        स्थिती NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          थांबा;

        स्थिती 'o':
          जर (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          थांबा;

        स्थिती RANDOM_SOURCE_OPTION:
          जर (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          थांबा;

        स्थिती 's':
          stable = true;
          थांबा;

        स्थिती 'S':
          specify_sort_size (oi, c, optarg);
          थांबा;

        स्थिती 't':
          {
            अक्षर newtab = optarg[0];
            जर (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            जर (optarg[1])
              {
                जर (streq (optarg, "\\0"))
                  newtab = '\0';
                अन्यथा
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (SORT_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            जर (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          थांबा;

        स्थिती 'T':
          add_temp_dir (optarg);
          थांबा;

        स्थिती PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          थांबा;

        स्थिती 'u':
          unique = true;
          थांबा;

        स्थिती 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          जर (optarg == argv[optind - 1])
            {
              अक्षर स्थिरांक *p;
              साठी (p = optarg; c_isdigit (*p); p++)
                सुरूठेवा;
              optind -= (*p != '\0');
            }
          थांबा;

        स्थिती 'z':
          eolchar = 0;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (संकेत (SIGPIPE, संक_छोड़ो) == संक_पूर्व);

  जर (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      जर (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      खाता *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      जर (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      जर (tok.n_tok)
        {
          मुक्त (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          साठी (माप_प्रकार i = 0; i < nfiles; i++)
            {
              जर (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              अन्यथा जर (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  अचिन्हित दीर्घ पूर्णांक file_संख्या = i + 1;
                  error (SORT_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_संख्या);
                }
            }
        }
      अन्यथा
        error (SORT_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  साठी (किल्ली = keylist; किल्ली; किल्ली = किल्ली->next)
    {
      जर (default_किल्ली_compare (किल्ली) && !किल्ली->reverse)
        {
          किल्ली->ignore = gkey.ignore;
          किल्ली->translate = gkey.translate;
          किल्ली->skipsblanks = gkey.skipsblanks;
          किल्ली->skipeblanks = gkey.skipeblanks;
          किल्ली->month = gkey.month;
          किल्ली->numeric = gkey.numeric;
          किल्ली->general_numeric = gkey.general_numeric;
          किल्ली->human_numeric = gkey.human_numeric;
          किल्ली->version = gkey.version;
          किल्ली->random = gkey.random;
          किल्ली->reverse = gkey.reverse;
        }

      need_random |= किल्ली->random;
    }

  जर (!keylist && !default_किल्ली_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  जर (debug)
    {
      जर (checkonly || outfile)
        {
          स्थिर अक्षर opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      जर (locale_ok)
        locale_ok = !! रखो_क्षेत्र (LC_COLLATE, "");
      जर (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      जर (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (रखो_क्षेत्र (LC_COLLATE, रिक्त)));
      अन्यथा
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      किल्ली_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  जर (need_random)
    random_md5_state_init (random_source);

  जर (temp_dirs.n_dirs == 0)
    {
      अक्षर स्थिरांक *tmp_dir = दो_पर्या ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  जर (nfiles == 0)
    {
      nfiles = 1;
      मुक्त (files);
      files = xmalloc (आकार *files);
      *files = (अक्षर *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  जर (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  जर (checkonly)
    {
      जर (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      जर (outfile)
        {
          स्थिर अक्षर opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      निकास (check (files[0], checkonly) ? निकास_सफल : SORT_OUT_OF_ORDER);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  जर (mergeonly)
    {
      संरचना sortfile *sortfiles = xcalloc (nfiles, आकार *sortfiles);

      साठी (माप_प्रकार i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  अन्यथा
    {
      जर (!nthreads)
        {
          अचिन्हित दीर्घ पूर्णांक np = अंक_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      माप_प्रकार nthreads_max = SIZE_MAX / (2 * आकार (संरचना merge_गाठ));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  जर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    sort_die (_("close failed"), "-");

  main_exit (निकास_सफल);
}
