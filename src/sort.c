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

#यदि HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif

#यदि !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#endif
#यदि !defined OPEN_MAX
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
गणना { SUBTHREAD_पंक्तियाँ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_पंक्तियाँ_HEURISTIC);

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
    MERGE_मूल = 1
  };

/* The representation of the decimal point in the current locale.  */
स्थिर वर्ण decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
स्थिर पूर्णाङ्क thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
स्थिर bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
स्थिर bool hard_LC_COLLATE;
#यदि HAVE_NL_LANGINFO
स्थिर bool hard_LC_TIME;
#endif

#define NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
गणना blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

/* Lines are held in memory as counted strings. */
संरचना पंक्ति
{
  वर्ण *text;			/* Text of the line. */
  माप_प्रकार length;		/* Length including final newline. */
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
  माप_प्रकार used;			/* Number of bytes used for input data.  */
  माप_प्रकार nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  माप_प्रकार बायाँ;			/* Number of bytes left from previous reads. */
  माप_प्रकार पंक्ति_bytes;		/* Number of bytes to reserve for each line. */
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
संरचना merge_गाँठ
{
  संरचना पंक्ति *lo;              /* Lines to merge from LO child node. */
  संरचना पंक्ति *hi;              /* Lines to merge from HI child node. */
  संरचना पंक्ति *end_lo;          /* End of available lines from LO. */
  संरचना पंक्ति *end_hi;          /* End of available lines from HI. */
  संरचना पंक्ति **dest;           /* Pointer to destination of merge. */
  माप_प्रकार nlo;                   /* Total Lines remaining from LO. */
  माप_प्रकार nhi;                   /* Total lines remaining from HI. */
  संरचना merge_गाँठ *parent;    /* Parent node. */
  संरचना merge_गाँठ *lo_child;  /* LO child node. */
  संरचना merge_गाँठ *hi_child;  /* HI child node. */
  अचिन्हित पूर्णाङ्क level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
संरचना merge_गाँठ_queue
{
  संरचना heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
स्थिर संरचना पंक्ति saved_पंक्ति;

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
#define MIN_MERGE_BUFFER_SIZE (2 + परिमाण (संरचना पंक्ति))

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
#define निवेश_FILE_SIZE_GUESS (128 * 1024)

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
गणना { NON_CHAR = अक्षर_उच्च + 1 };

/* If TAB has this value, blanks separate fields.  */
गणना { TAB_DEFAULT = अक्षर_उच्च + 1 };

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
स्थिर अचिन्हित पूर्णाङ्क nmerge = NMERGE_DEFAULT;

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
  प्रति (संरचना tempnode ध्रुव *गाँठ = temphead; गाँठ; गाँठ = गाँठ->next)
    unlink (गाँठ->name);
  temphead = रिक्त;
}

/* Handle interrupts and hangups. */

स्थिर शून्य
sighandler (पूर्णाङ्क sig)
{
  cleanup ();

  सङ्केत (sig, संक_पूर्व);
  उत्थापय (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

स्थिर शून्य
sort_die (वर्ण ध्रुव *message, वर्ण ध्रुव *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  यदि (त्रुटि_सं == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, त्रुटि_सं, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
         ignore leading blanks when finding sort कुंजियाँ in each पंक्ति\n\
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
         see full documentation प्रति supported strings\n\
"));
      oputs (_("\
  -R, --random-sort\n\
         shuffle, but group identical कुंजियाँ.  See also shuf(1)\n\
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
         merge at most NMERGE inputs at once; प्रति more use temp files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check प्रति sorted निवेश; कुरु not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but कुरु not report first bad पंक्ति\n\
"));
      oputs (_("\
      --compress-program=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part of the पंक्ति used to sort,\n\
         and warn about questionable usage to standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ निवेश from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, पठ names from standard निवेश\n\
"));
      oputs (_("\
  -k, --कुंजी=KEYDEF\n\
         sort via a कुंजी; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; कुरु not sort\n\
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
         use SIZE प्रति main memory buffer\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=सूची\n\
         use सूची प्रति temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the संख्या of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of पंक्तियाँ with equal कुंजियाँ;\n\
         with -c, check प्रति strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] प्रति start and stop position, where F is a\n\
field संख्या and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the पंक्ति's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options प्रति that कुंजी.  If no कुंजी is given, use\n\
the entire पंक्ति as the कुंजी.  Use --debug to diagnose incorrect कुंजी usage.\n\
\n\
SIZE may be followed by the following multiplicative suffixes:\n\
"), मानक_निकास);
      ख_माला_दो (_("\
% 1% of memory, b 1, K 1024 (स्वभाव), and so on प्रति M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects sort order.\n\
Set LC_ALL=C to get the traditional sort order that uses\n\
native byte मूल्यावली.\n\
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
  यदि (status->valid)
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

स्थिर माप_प्रकार
proctab_hasher (शून्य ध्रुव *entry, माप_प्रकार tabsize)
{
  संरचना tempnode ध्रुव *गाँठ = entry;
  प्रत्यागम गाँठ->pid % tabsize;
}

स्थिर bool
proctab_comparator (शून्य ध्रुव *e1, शून्य ध्रुव *e2)
{
  संरचना tempnode ध्रुव *n1 = e1;
  संरचना tempnode ध्रुव *n2 = e2;
  प्रत्यागम n1->pid == n2->pid;
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

  यदि (cpid < 0)
    error (SORT_FAILURE, त्रुटि_सं, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  अन्यथा यदि (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      यदि (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  प्रत्यागम cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

स्थिर शून्य
register_proc (संरचना tempnode *temp)
{
  यदि (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_SIZE, रिक्त,
                                 proctab_hasher,
                                 proctab_comparator,
                                 रिक्त);
      यदि (! proctab)
        xalloc_die ();
    }

  temp->state = UNREAPED;

  यदि (! hash_जोड़ो (proctab, temp))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

स्थिर bool
delete_proc (pid_t pid)
{
  संरचना tempnode test;

  test.pid = pid;
  संरचना tempnode *गाँठ = hash_remove (proctab, &test);
  यदि (! गाँठ)
    प्रत्यागम false;
  गाँठ->state = REAPED;
  प्रत्यागम true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

स्थिर शून्य
wait_proc (pid_t pid)
{
  यदि (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

स्थिर शून्य
reap_exited (शून्य)
{
  यावत् (0 < nprocs && reap (0))
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
  यावत् (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

स्थिर शून्य
exit_cleanup (शून्य)
{
  यदि (temphead)
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
  माप_प्रकार len = म_माप (temp_dir);
  संरचना tempnode *गाँठ =
    xmalloc (FLEXSIZEOF (संरचना tempnode, name, len + परिमाण slashbase));
  वर्ण *file = गाँठ->name;
  संरचना cs_status cs;

  स_नकल (file, temp_dir, len);
  स_नकल (file + len, slashbase, परिमाण slashbase);
  गाँठ->next = रिक्त;
  यदि (++temp_dir_index == temp_dirs.n_dirs)
    temp_dir_index = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  यदि (0 <= fd)
    {
      *temptail = गाँठ;
      temptail = &गाँठ->next;
    }
  saved_errno = त्रुटि_सं;
  cs_leave (&cs);
  त्रुटि_सं = saved_errno;

  यदि (fd < 0)
    {
      यदि (! (survive_fd_exhaustion && त्रुटि_सं == EMFILE))
        error (SORT_FAILURE, त्रुटि_सं, _("cannot create temporary file in %s"),
               quoteaf (temp_dir));
      मुक्त (गाँठ);
      गाँठ = रिक्त;
    }

  *pfd = fd;
  प्रत्यागम गाँठ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

स्थिर संरचना stat *
get_outstatus (शून्य)
{
  स्थिर पूर्णाङ्क outstat_errno;
  स्थिर संरचना stat outstat;
  यदि (outstat_errno == 0)
    outstat_errno = ख_स्थिति (STDOUT_FILENO, &outstat) == 0 ? -1 : त्रुटि_सं;
  प्रत्यागम outstat_errno < 0 ? &outstat : रिक्त;
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
stream_open (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  खाता *fp;

  यदि (*how == 'r')
    {
      यदि (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = मानक_निवेश;
        }
      अन्यथा
        {
          पूर्णाङ्क fd = खोलो (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? रिक्त : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  अन्यथा यदि (*how == 'w')
    {
      यदि (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          पूर्णाङ्क ftruncate_errno = त्रुटि_सं;
          संरचना stat *outst = get_outstatus ();
          यदि (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = मानक_निकास;
    }
  अन्यथा
    affirm (!"unexpected mode passed to stream_open");

  प्रत्यागम fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

स्थिर खाता *
xfopen (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  खाता *fp = stream_open (file, how);
  यदि (!fp)
    sort_die (_("open failed"), file);
  प्रत्यागम fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

स्थिर शून्य
xfclose (खाता *fp, वर्ण ध्रुव *file)
{
  विकल्प (fileno (fp))
    {
    पक्ष STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      साफ_त्रुटि (fp);
      भङ्ग;

    पक्ष STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      यदि (ख_साफ (fp) != 0)
        sort_die (_("fflush failed"), file);
      भङ्ग;

    स्वभाव:
      यदि (ख_बंद (fp) != 0)
        sort_die (_("close failed"), file);
      भङ्ग;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

स्थिर शून्य
move_fd (पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
  यदि (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_मूल्य (dup2 (oldfd, newfd));
      ignore_मूल्य (बंद (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

स्थिर पूर्णाङ्क
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  पूर्णाङ्क oldfd, पूर्णाङ्क newfd)
{
  पूर्णाङ्क result = 0;
  यदि (oldfd != newfd)
    {
      result = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      यदि (result == 0)
        result = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  प्रत्यागम result;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

स्थिर वर्ण ध्रुव *
get_resolved_compress_program (शून्य)
{
  /* Use a cache, to perform the search only once.  */
  स्थिर वर्ण ध्रुव *resolved_compress_program_cache /* = NULL */;

  यदि (resolved_compress_program_cache == रिक्त)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, दो_पर्या ("PATH"), रिक्त, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  प्रत्यागम resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

स्थिर पूर्णाङ्क
pipe_child (pid_t *pid, पूर्णाङ्क pipefds[2], पूर्णाङ्क tempfd, bool decompress,
            माप_प्रकार tries)
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
  यदि (resolved_compress_program == रिक्त)
    प्रत्यागम त्रुटि_सं;

  यदि ((result = posix_spawnattr_init (&attr)))
    प्रत्यागम result;
  यदि ((result = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (result = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      प्रत्यागम result;
    }

  यदि (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      पूर्णाङ्क saved_errno = त्रुटि_सं;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      प्रत्यागम saved_errno;
    }

  यदि ((result = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      प्रत्यागम result;
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

  यदि (nmerge + 1 < nprocs)
    reap_some ();

  यावत् (tries--)
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

      यदि (result != EAGAIN)
        भङ्ग;
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

  यदि (result)
    {
      बंद (pipefds[0]);
      बंद (pipefds[1]);
    }
  अन्यथा
    ++nprocs;

  प्रत्यागम result;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

स्थिर संरचना tempnode *
maybe_create_temp (खाता **pfp, bool survive_fd_exhaustion)
{
  पूर्णाङ्क tempfd;
  संरचना tempnode *गाँठ = create_temp_file (&tempfd, survive_fd_exhaustion);
  यदि (! गाँठ)
    प्रत्यागम रिक्त;

  गाँठ->state = UNCOMPRESSED;

  यदि (compress_program)
    {
      पूर्णाङ्क pipefds[2];
      स्थिर पूर्णाङ्क last_result = 0;

      पूर्णाङ्क result = pipe_child (&गाँठ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      यदि (result)
        {
          यदि (result != last_result)
            error (0, result, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      अन्यथा
        {
          बंद (tempfd);
          बंद (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (गाँठ);
        }

      last_result = result;
    }

  *pfp = fdopen (tempfd, "w");
  यदि (! *pfp)
    sort_die (_("couldn't create temporary file"), गाँठ->name);

  प्रत्यागम गाँठ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

स्थिर संरचना tempnode *
create_temp (खाता **pfp)
{
  प्रत्यागम maybe_create_temp (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

स्थिर खाता *
open_temp (संरचना tempnode *temp)
{
  पूर्णाङ्क tempfd, pipefds[2];
  खाता *fp = रिक्त;

  यदि (temp->state == UNREAPED)
    wait_proc (temp->pid);

  tempfd = खोलो (temp->name, O_RDONLY);
  यदि (tempfd < 0)
    प्रत्यागम रिक्त;

  pid_t child;
  पूर्णाङ्क result = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  यदि (result)
    {
      यदि (result != EMFILE)
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
      यदि (! fp)
        {
          पूर्णाङ्क saved_errno = त्रुटि_सं;
          बंद (pipefds[0]);
          त्रुटि_सं = saved_errno;
        }
    }

  प्रत्यागम fp;
}

/* Append DIR to the array of temporary directory names.  */
स्थिर शून्य
add_temp_dir (वर्ण ध्रुव *dir)
{
  यदि (temp_dirs.n_dirs == temp_dirs.n_dirs_alloc)
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
  संरचना tempnode *गाँठ;
  संरचना tempnode *next;
  पूर्णाङ्क unlink_status;
  पूर्णाङ्क unlink_errno = 0;
  संरचना cs_status cs;

  प्रति (pnode = &temphead; (गाँठ = *pnode)->name != name; pnode = &गाँठ->next)
    अनुवर्त;

  यदि (गाँठ->state == UNREAPED)
    wait_proc (गाँठ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  next = गाँठ->next;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = त्रुटि_सं;
  *pnode = next;
  cs_leave (&cs);

  यदि (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  यदि (! next)
    temptail = pnode;
  मुक्त (गाँठ);
}

#यदि HAVE_NL_LANGINFO

स्थिर पूर्णाङ्क
struct_month_cmp (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  संरचना month ध्रुव *month1 = m1;
  संरचना month ध्रुव *month2 = m2;
  प्रत्यागम म_भेद (month1->name, month2->name);
}

#endif

/* Initialize the character class tables. */

स्थिर शून्य
inittables (शून्य)
{
  प्रति (माप_प्रकार i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! है_अक्षर_अंक (i);
      nonprinting[i] = ! है_छाप (i);
      fold_toupper[i] = बड़े (i);
    }

#यदि HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  यदि (hard_LC_TIME)
    {
      प्रति (माप_प्रकार i = 0; i < MONTHS_PER_YEAR; i++)
        {
          वर्ण ध्रुव *s;
          माप_प्रकार s_len;
          माप_प्रकार j, k;
          वर्ण *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = म_माप (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          प्रति (j = k = 0; j < s_len; j++)
            यदि (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      क्विक (monthtab, MONTHS_PER_YEAR, परिमाण *monthtab, struct_month_cmp);
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
  अचिन्हित पूर्णाङ्क max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  यदि (e == LONGINT_OK)
    {
      nmerge = n;
      यदि (nmerge != n)
        e = LONGINT_OVERFLOW;
      अन्यथा
        {
          यदि (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          अन्यथा यदि (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          अन्यथा
            प्रत्यागम;
        }
    }

  यदि (e == LONGINT_OVERFLOW)
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
specify_sort_size (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t n;
  वर्ण *suffix;
  गणना strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  यदि (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  यदि (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    विकल्प (suffix[0])
      {
      पक्ष 'b':
        e = LONGINT_OK;
        भङ्ग;

      पक्ष '%':
        {
          द्विगुण mem = physmem_total () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          यदि (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          अन्यथा
            e = LONGINT_OVERFLOW;
        }
        भङ्ग;
      }

  यदि (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      यदि (n < sort_size)
        प्रत्यागम;

      sort_size = n;
      यदि (sort_size == n)
        {
          sort_size = MAX (sort_size, MIN_SORT_SIZE);
          प्रत्यागम;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
स्थिर माप_प्रकार
specify_nthreads (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t nthreads;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &nthreads, "");
  यदि (e == LONGINT_OVERFLOW)
    प्रत्यागम SIZE_MAX;
  यदि (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  यदि (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  यदि (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  प्रत्यागम nthreads;
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
  यदि (getrlimit (RLIMIT_DATA, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#ifdef RLIMIT_AS
  यदि (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < size)
    size = rlimit.rlim_cur;
#endif

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  size /= 2;

#ifdef RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  यदि (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < size)
    size = rlimit.rlim_cur / 16 * 15;
#endif

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  द्विगुण avail = physmem_available ();
  द्विगुण total = physmem_total ();
  द्विगुण mem = MAX (avail, total / 8);

  /* Leave a 1/4 margin for physical memory.  */
  यदि (total * 0.75 < size)
    size = total * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  यदि (mem < size)
    size = mem;
  प्रत्यागम MAX (size, MIN_SORT_SIZE);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

स्थिर bool
पंक्ति_aligned_size (माप_प्रकार *alloc)
{
  माप_प्रकार size = *alloc;
  माप_प्रकार alignment = परिमाण (संरचना पंक्ति);
  माप_प्रकार padding = alignment - size % alignment;
  माप_प्रकार aligned;

  यदि (ckd_add (&aligned, size, padding))
    प्रत्यागम false;

  *alloc = aligned;
  प्रत्यागम true;
}

/* Return the default initial allocation for a growable sort buffer.  */

स्थिर माप_प्रकार
default_initial_sort_size (माप_प्रकार पंक्ति_bytes)
{
  माप_प्रकार size = पंक्ति_bytes + 2;
  माप_प्रकार निवेश_size;

  यदि (ckd_mul (&निवेश_size, निवेश_FILE_SIZE_GUESS, पंक्ति_bytes + 1)
      || ckd_add (&size, size, निवेश_size))
    प्रत्यागम SIZE_MAX;

  प्रत्यागम MAX (size, MIN_SORT_SIZE);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

स्थिर bool
निवेश_size_buffer_bytes (uintmax_t file_size, माप_प्रकार पंक्ति_bytes,
                         माप_प्रकार *worst_case)
{
  माप_प्रकार worst_case_per_निवेश_byte = पंक्ति_bytes + 1;

  यदि (SIZE_MAX / worst_case_per_निवेश_byte < file_size)
    प्रत्यागम false;

  माप_प्रकार size = file_size * worst_case_per_निवेश_byte;
  यदि (SIZE_MAX - size < 1)
    प्रत्यागम false;

  *worst_case = size + 1;
  प्रत्यागम true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

स्थिर शून्य
sort_buffer_policy (खाता *ध्रुव *fps, माप_प्रकार nfps,
                    वर्ण *ध्रुव *files, माप_प्रकार nfiles,
                    माप_प्रकार पंक्ति_bytes,
                    संरचना sort_buffer_policy *policy)
{
  माप_प्रकार size_bound = sort_size ? sort_size : default_sort_size ();
  माप_प्रकार initial_bound =
    sort_size ? size_bound : default_initial_sort_size (पंक्ति_bytes);

  /* In the worst case, each input byte is a newline.  */
  माप_प्रकार worst_case_per_निवेश_byte = पंक्ति_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  माप_प्रकार size = worst_case_per_निवेश_byte + 1;

  initial_bound = MIN (initial_bound, size_bound);
  policy->limit = size_bound;
  policy->growth_failed = false;

  प्रति (माप_प्रकार i = 0; i < nfiles; i++)
    {
      संरचना stat st;
      off_t file_size;
      माप_प्रकार worst_case;
      bool known_size;

      यदि (size_bound <= size)
        {
          policy->initial = size_bound;
          प्रत्यागम;
        }

      यदि ((i < nfps ? ख_स्थिति (fileno (fps[i]), &st)
           : streq (files[i], "-") ? ख_स्थिति (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_size = usable_st_size (&st) && 0 < st.st_size;
      यदि (known_size)
        file_size = st.st_size;
      अन्यथा
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          यदि (sort_size)
            {
              policy->initial = size_bound;
              प्रत्यागम;
            }
          file_size = निवेश_FILE_SIZE_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      यदि (! निवेश_size_buffer_bytes (file_size, पंक्ति_bytes, &worst_case)
          || size_bound - size <= worst_case)
        {
          policy->initial = size_bound;
          प्रत्यागम;
        }

      यदि (! known_size && ! sort_size
          && (initial_bound <= size || initial_bound - size <= worst_case))
        {
          policy->initial = MAX (size, initial_bound);
          प्रत्यागम;
        }

      size += worst_case;
    }

  policy->initial = MAX (size, MIN_SORT_SIZE);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

स्थिर शून्य
initbuf (संरचना buffer *buf, माप_प्रकार पंक्ति_bytes, माप_प्रकार alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  यावत् (true)
    {
      यदि (! पंक्ति_aligned_size (&alloc))
        buf->buf = रिक्त;
      अन्यथा
        buf->buf = दो_स्मृति (alloc);
      यदि (buf->buf)
        भङ्ग;
      alloc /= 2;
      यदि (alloc <= पंक्ति_bytes + 1)
        xalloc_die ();
    }

  buf->पंक्ति_bytes = पंक्ति_bytes;
  buf->alloc = alloc;
  buf->used = buf->बायाँ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

स्थिर अन्तःस्थ संरचना पंक्ति *
buffer_linelim (संरचना buffer ध्रुव *buf)
{
  शून्य *linelim = buf->buf + buf->alloc;
  प्रत्यागम linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

स्थिर bool
try_growbuf (संरचना buffer *buf, माप_प्रकार alloc)
{
  यदि (! पंक्ति_aligned_size (&alloc) || alloc <= buf->alloc)
    प्रत्यागम false;

  वर्ण *newbuf = दो_स्मृति (alloc);
  यदि (! newbuf)
    प्रत्यागम false;

  वर्ण *oldbuf = buf->buf;
  संरचना पंक्ति *old_linelim = buffer_linelim (buf);
  संरचना पंक्ति *old_पंक्ति = old_linelim - buf->nlines;

  स_नकल (newbuf, oldbuf, buf->used);

  संरचना पंक्ति *new_linelim = (शून्य *) (newbuf + alloc);
  संरचना पंक्ति *new_पंक्ति = new_linelim - buf->nlines;
  स_नकल (new_पंक्ति, old_पंक्ति, buf->nlines * परिमाण *new_पंक्ति);

  प्रति (संरचना पंक्ति *पंक्ति = new_पंक्ति; पंक्ति < new_linelim; पंक्ति++)
    {
      पंक्ति->text = newbuf + (पंक्ति->text - oldbuf);
      यदि (keylist)
        {
          पंक्ति->keybeg = newbuf + (पंक्ति->keybeg - oldbuf);
          पंक्ति->keylim = newbuf + (पंक्ति->keylim - oldbuf);
        }
    }

  मुक्त (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  प्रत्यागम true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

स्थिर bool
maybe_growbuf (संरचना buffer *buf, संरचना sort_buffer_policy *policy)
{
  यदि (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    प्रत्यागम false;

  माप_प्रकार alloc;
  यदि (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  अन्यथा
    alloc = policy->limit;

  यदि (try_growbuf (buf, alloc))
    प्रत्यागम true;

  policy->growth_failed = true;
  प्रत्यागम false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

स्थिर वर्ण *
begfield (संरचना पंक्ति ध्रुव *पंक्ति, संरचना keyfield ध्रुव *कुंजी)
{
  वर्ण *ptr = पंक्ति->text, *lim = ptr + पंक्ति->length - 1;
  माप_प्रकार sword = कुंजी->sword;
  माप_प्रकार schar = कुंजी->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  यदि (tab != TAB_DEFAULT)
    यावत् (ptr < lim && sword--)
      {
        वर्ण *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        यदि (ptr < lim)
          ++ptr;
      }
  अन्यथा
    यावत् (ptr < lim && sword--)
      {
        यावत् (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        यावत् (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  यदि (कुंजी->skipsblanks)
    यावत् (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  माप_प्रकार remaining_bytes = lim - ptr;
  यदि (schar < remaining_bytes)
    ptr += schar;
  अन्यथा
    ptr = lim;

  प्रत्यागम ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
स्थिर वर्ण *
limfield (संरचना पंक्ति ध्रुव *पंक्ति, संरचना keyfield ध्रुव *कुंजी)
{
  वर्ण *ptr = पंक्ति->text, *lim = ptr + पंक्ति->length - 1;
  माप_प्रकार eword = कुंजी->eword, echar = कुंजी->echar;

  यदि (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  यदि (tab != TAB_DEFAULT)
    यावत् (ptr < lim && eword--)
      {
        वर्ण *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        यदि (ptr < lim && (eword || echar))
          ++ptr;
      }
  अन्यथा
    यावत् (ptr < lim && eword--)
      {
        यावत् (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        यावत् (ptr < lim && !blanks[to_uchar (*ptr)])
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
  यदि (tab != TAB_DEFAULT)
    {
      वर्ण *newlim;
      newlim = स_प्रथम (ptr, tab, lim - ptr);
      यदि (newlim)
        lim = newlim;
    }
  अन्यथा
    {
      वर्ण *newlim;
      newlim = ptr;
      यावत् (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      यावत् (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#endif

  यदि (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      यदि (कुंजी->skipeblanks)
        यावत् (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      माप_प्रकार remaining_bytes = lim - ptr;
      यदि (echar < remaining_bytes)
        ptr += echar;
      अन्यथा
        ptr = lim;
    }

  प्रत्यागम ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

स्थिर bool
fillbuf (संरचना buffer *buf, खाता *fp, वर्ण ध्रुव *file,
         संरचना sort_buffer_policy *policy)
{
  संरचना keyfield ध्रुव *कुंजी = keylist;
  वर्ण eol = eolchar;
  माप_प्रकार पंक्ति_bytes = buf->पंक्ति_bytes;
  माप_प्रकार mergesize = merge_buffer_size - MIN_MERGE_BUFFER_SIZE;

  यदि (buf->eof)
    प्रत्यागम false;

  यदि (buf->used != buf->बायाँ)
    {
      स_हटाओ (buf->buf, buf->buf + buf->used - buf->बायाँ, buf->बायाँ);
      buf->used = buf->बायाँ;
      buf->nlines = 0;
    }

  यावत् (true)
    {
      वर्ण *ptr = buf->buf + buf->used;
      संरचना पंक्ति *linelim = buffer_linelim (buf);
      संरचना पंक्ति *पंक्ति = linelim - buf->nlines;
      माप_प्रकार avail = (वर्ण *) linelim - buf->nlines * पंक्ति_bytes - ptr;
      वर्ण *पंक्ति_start = buf->nlines ? पंक्ति->text + पंक्ति->length : buf->buf;

      यावत् (पंक्ति_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          माप_प्रकार readsize = (avail - 1) / (पंक्ति_bytes + 1);
          माप_प्रकार bytes_read = ख_पढ़ो (ptr, 1, readsize, fp);
          वर्ण *ptrlim = ptr + bytes_read;
          वर्ण *p;
          avail -= bytes_read;

          यदि (bytes_read != readsize)
            {
              यदि (ख_त्रुटि (fp))
                sort_die (_("read failed"), file);
              यदि (ख_पूर्ण (fp))
                {
                  buf->eof = true;
                  यदि (buf->buf == ptrlim)
                    प्रत्यागम false;
                  यदि (पंक्ति_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          यावत् ((p = स_प्रथम (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              पंक्ति--;
              पंक्ति->text = पंक्ति_start;
              पंक्ति->length = ptr - पंक्ति_start;
              mergesize = MAX (mergesize, पंक्ति->length);
              avail -= पंक्ति_bytes;

              यदि (कुंजी)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  पंक्ति->keylim = (कुंजी->eword == SIZE_MAX
                                  ? p
                                  : limfield (पंक्ति, कुंजी));

                  यदि (कुंजी->sword != SIZE_MAX)
                    पंक्ति->keybeg = begfield (पंक्ति, कुंजी);
                  अन्यथा
                    {
                      यदि (कुंजी->skipsblanks)
                        यावत् (blanks[to_uchar (*पंक्ति_start)])
                          पंक्ति_start++;
                      पंक्ति->keybeg = पंक्ति_start;
                    }
                }

              पंक्ति_start = ptr;
            }

          ptr = ptrlim;
          यदि (buf->eof)
            भङ्ग;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = buffer_linelim (buf) - पंक्ति;
      यदि (buf->nlines != 0)
        {
          यदि (! buf->eof && maybe_growbuf (buf, policy))
            अनुवर्त;
          buf->बायाँ = ptr - पंक्ति_start;
          merge_buffer_size = mergesize + MIN_MERGE_BUFFER_SIZE;
          प्रत्यागम true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t पंक्ति_alloc = buf->alloc / परिमाण (संरचना पंक्ति);
        buf->buf = xpalloc (buf->buf, &पंक्ति_alloc, 1, -1, परिमाण (संरचना पंक्ति));
        buf->alloc = पंक्ति_alloc * परिमाण (संरचना पंक्ति);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
स्थिर वर्ण ध्रुव unit_order[UCHAR_LIM] =
  {
#यदि ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
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
स्थिर वर्ण
traverse_raw_संख्या (वर्ण ध्रुव **संख्या)
{
  वर्ण ध्रुव *p = *संख्या;
  वर्ण ch;
  वर्ण max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  यावत् (c_isdigit (ch = *p++))
    {
      यदि (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      यदि (ends_with_thousands_sep)
        ++p;
    }

  यदि (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *संख्या = p - 2;
      प्रत्यागम max_digit;
    }

  यदि (ch == decimal_point)
    यावत् (c_isdigit (ch = *p++))
      यदि (max_digit < ch)
        max_digit = ch;

  *संख्या = p - 1;
  प्रत्यागम max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
find_unit_order (वर्ण ध्रुव *संख्या)
{
  bool minus_sign = (*संख्या == '-');
  वर्ण ध्रुव *p = संख्या + minus_sign;
  वर्ण max_digit = traverse_raw_संख्या (&p);
  यदि ('0' < max_digit)
    {
      अचिन्हित वर्ण ch = *p;
      पूर्णाङ्क order = unit_order[ch];
      प्रत्यागम (minus_sign ? -order : order);
    }
  अन्यथा
    प्रत्यागम 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
human_numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  यावत् (blanks[to_uchar (*a)])
    a++;
  यावत् (blanks[to_uchar (*b)])
    b++;

  पूर्णाङ्क diff = find_unit_order (a) - find_unit_order (b);
  प्रत्यागम (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
स्थिर पूर्णाङ्क
numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  यावत् (blanks[to_uchar (*a)])
    a++;
  यावत् (blanks[to_uchar (*b)])
    b++;

  प्रत्यागम strnumcmp (a, b, decimal_point, thousands_sep);
}

स्थिर पूर्णाङ्क
nan_compare (दीर्घ द्विगुण a, दीर्घ द्विगुण b)
{
  वर्ण buf[2][परिमाण "-nan""()" + अक्षर_बिट * परिमाण a];
  snprintf (buf[0], परिमाण buf[0], "%Lf", a);
  snprintf (buf[1], परिमाण buf[1], "%Lf", b);
  प्रत्यागम म_भेद (buf[0], buf[1]);
}

स्थिर पूर्णाङ्क
general_numcompare (वर्ण ध्रुव *sa, वर्ण ध्रुव *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  वर्ण *ea;
  वर्ण *eb;
  दीर्घ द्विगुण a = strtold (sa, &ea);
  दीर्घ द्विगुण b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  यदि (sa == ea)
    प्रत्यागम sb == eb ? 0 : -1;
  यदि (sb == eb)
    प्रत्यागम 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  प्रत्यागम (a < b ? -1
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
  माप_प्रकार lo = 0;
  माप_प्रकार hi = MONTHS_PER_YEAR;

  यावत् (blanks[to_uchar (*month)])
    month++;

  कुरु
    {
      माप_प्रकार ix = (lo + hi) / 2;
      वर्ण ध्रुव *m = month;
      वर्ण ध्रुव *n = monthtab[ix].name;

      प्रति (;; m++, n++)
        {
          यदि (!*n)
            {
              यदि (ea)
                *ea = (वर्ण *) m;
              प्रत्यागम monthtab[ix].val;
            }
          यदि (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              भङ्ग;
            }
          अन्यथा यदि (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              भङ्ग;
            }
        }
    }
  यावत् (lo < hi);

  प्रत्यागम 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#यदि DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# यदि 14 <= __GNUC__
#  pragma GCC diagnostic धकेलो
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#endif

#अन्तर्भाव "md5.h"

#यदि DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# यदि 14 <= __GNUC__
#  pragma GCC diagnostic निकालो
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
प्रतीक_address (शून्य *handle, वर्ण ध्रुव *प्रतीक)
{
  शून्य *address = dlsym (handle, प्रतीक);
  यदि (!address)
    link_failure ();
  प्रत्यागम address;
}
#endif

/* Dynamically link the crypto library, if it needs linking.  */
स्थिर शून्य
link_libcrypto (शून्य)
{
#यदि DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  शून्य *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  यदि (!handle)
    link_failure ();
  ptr_MD5_Init = प्रतीक_address (handle, "MD5_Init");
  ptr_MD5_Update = प्रतीक_address (handle, "MD5_Update");
  ptr_MD5_Final = प्रतीक_address (handle, "MD5_Final");
#endif
}

/* A randomly chosen MD5 state, used for random comparison.  */
स्थिर संरचना md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

स्थिर शून्य
random_md5_state_init (वर्ण ध्रुव *random_source)
{
  अचिन्हित वर्ण buf[MD5_DIGEST_SIZE];
  संरचना randread_source *r = randread_new (random_source, परिमाण buf);
  यदि (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, परिमाण buf);
  यदि (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, परिमाण buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

स्थिर माप_प्रकार
xstrxfrm (वर्ण *restrict dest, वर्ण ध्रुव *restrict src, माप_प्रकार destsize)
{
  त्रुटि_सं = 0;
  माप_प्रकार translated_size = strxfrm (dest, src, destsize);

  यदि (त्रुटि_सं)
    {
      error (0, त्रुटि_सं, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  प्रत्यागम translated_size;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

स्थिर पूर्णाङ्क
compare_random (वर्ण *restrict texta, माप_प्रकार lena,
                वर्ण *restrict textb, माप_प्रकार lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  पूर्णाङ्क xfrm_diff = 0;

  वर्ण stackbuf[4000];
  वर्ण *buf = stackbuf;
  माप_प्रकार bufsize = परिमाण stackbuf;
  शून्य *allocated = रिक्त;
  uint32_t dig[2][MD5_DIGEST_SIZE / परिमाण (uint32_t)];
  संरचना md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  यदि (hard_LC_COLLATE)
    {
      वर्ण ध्रुव *lima = texta + lena;
      वर्ण ध्रुव *limb = textb + lenb;

      यावत् (true)
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
          यदि (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              मुक्त (allocated);
              buf = allocated = दो_स्मृति (bufsize);
              यदि (! buf)
                {
                  buf = stackbuf;
                  bufsize = परिमाण stackbuf;
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

          यदि (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              यदि (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              मुक्त (allocated);
              buf = allocated = xmalloc (bufsize);
              यदि (texta < lima)
                strxfrm (buf, texta, sizea);
              यदि (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          यदि (texta < lima)
            texta += म_माप (texta) + 1;
          यदि (textb < limb)
            textb += म_माप (textb) + 1;
          यदि (! (texta < lima || textb < limb))
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
          यदि (! xfrm_diff)
            {
              xfrm_diff = स_भेद (buf, buf + sizea, MIN (sizea, sizeb));
              यदि (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  पूर्णाङ्क diff = स_भेद (dig[0], dig[1], परिमाण dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  यदि (! diff)
    {
      यदि (! xfrm_diff)
        {
          xfrm_diff = स_भेद (texta, textb, MIN (lena, lenb));
          यदि (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  मुक्त (allocated);

  प्रत्यागम diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

स्थिर माप_प्रकार
debug_width (वर्ण ध्रुव *text, वर्ण ध्रुव *lim)
{
  माप_प्रकार width = mbsnwidth (text, lim - text, 0);
  यावत् (text < lim)
    width += (*text++ == '\t');
  प्रत्यागम width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

स्थिर शून्य
mark_कुंजी (माप_प्रकार offset, माप_प्रकार width)
{
  यावत् (offset--)
    अक्षर_दो (' ');

  यदि (!width)
    म_लिखो (_("^ no match for key\n"));
  अन्यथा
    {
      कुरु
        अक्षर_दो ('_');
      यावत् (--width);

      अक्षर_दो ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

स्थिर अन्तःस्थ bool
कुंजी_numeric (संरचना keyfield ध्रुव *कुंजी)
{
  प्रत्यागम कुंजी->numeric || कुंजी->general_numeric || कुंजी->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

स्थिर शून्य
debug_कुंजी (संरचना पंक्ति ध्रुव *पंक्ति, संरचना keyfield ध्रुव *कुंजी)
{
  वर्ण *text = पंक्ति->text;
  वर्ण *beg = text;
  वर्ण *lim = text + पंक्ति->length - 1;

  यदि (कुंजी)
    {
      यदि (कुंजी->sword != SIZE_MAX)
        beg = begfield (पंक्ति, कुंजी);
      यदि (कुंजी->eword != SIZE_MAX)
        {
          lim = limfield (पंक्ति, कुंजी);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      यदि ((कुंजी->skipsblanks && कुंजी->sword == SIZE_MAX)
          || कुंजी->month || कुंजी_numeric (कुंजी))
        {
          वर्ण saved = *lim;
          *lim = '\0';

          यावत् (blanks[to_uchar (*beg)])
            beg++;

          वर्ण *tighter_lim = beg;

          यदि (lim < beg)
            tighter_lim = lim;
          अन्यथा यदि (कुंजी->month)
            getmonth (beg, &tighter_lim);
          अन्यथा यदि (कुंजी->general_numeric)
            ignore_मूल्य (strtold (beg, &tighter_lim));
          अन्यथा यदि (कुंजी->numeric || कुंजी->human_numeric)
            {
              वर्ण ध्रुव *p = beg + (beg < lim && *beg == '-');
              वर्ण max_digit = traverse_raw_संख्या (&p);
              यदि ('0' <= max_digit)
                {
                  अचिन्हित वर्ण ch = *p;
                  tighter_lim = (वर्ण *) p
                    + (कुंजी->human_numeric && unit_order[ch]);
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
  mark_कुंजी (offset, width);
}

/* Debug LINE by underlining its keys.  */

स्थिर शून्य
debug_पंक्ति (संरचना पंक्ति ध्रुव *पंक्ति)
{
  संरचना keyfield ध्रुव *कुंजी = keylist;

  कुरु
    debug_कुंजी (पंक्ति, कुंजी);
  यावत् (कुंजी && ((कुंजी = कुंजी->next) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

स्थिर bool
default_कुंजी_compare (संरचना keyfield ध्रुव *कुंजी)
{
  प्रत्यागम ! (कुंजी->ignore
            || कुंजी->translate
            || कुंजी->skipsblanks
            || कुंजी->skipeblanks
            || कुंजी_numeric (कुंजी)
            || कुंजी->month
            || कुंजी->version
            || कुंजी->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

स्थिर शून्य
कुंजी_to_opts (संरचना keyfield ध्रुव *कुंजी, वर्ण *opts)
{
  यदि (कुंजी->skipsblanks || कुंजी->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  यदि (कुंजी->ignore == nondictionary)
    *opts++ = 'd';
  यदि (कुंजी->translate)
    *opts++ = 'f';
  यदि (कुंजी->general_numeric)
    *opts++ = 'g';
  यदि (कुंजी->human_numeric)
    *opts++ = 'h';
  यदि (कुंजी->ignore == nonprinting)
    *opts++ = 'i';
  यदि (कुंजी->month)
    *opts++ = 'M';
  यदि (कुंजी->numeric)
    *opts++ = 'n';
  यदि (कुंजी->random)
    *opts++ = 'R';
  यदि (कुंजी->reverse)
    *opts++ = 'r';
  यदि (कुंजी->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

स्थिर शून्य
कुंजी_warnings (संरचना keyfield ध्रुव *gkey, bool gkey_only)
{
  संरचना keyfield ugkey = *gkey;
  अचिन्हित दीर्घ keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  प्रति (संरचना keyfield *कुंजी = keylist; कुंजी; कुंजी = कुंजी->next, keynum++)
    {
      यदि (कुंजी_numeric (कुंजी))
        {
          यदि (कुंजी->general_numeric)
            general_numeric_field = true;
          अन्यथा
            basic_numeric_field = true;
        }

      यदि (कुंजी->traditional_used)
        {
          माप_प्रकार sword = कुंजी->sword;
          माप_प्रकार eword = कुंजी->eword;
          वर्ण tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          वर्ण obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          वर्ण nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          वर्ण *po = obuf;
          वर्ण *pn = nbuf;

          यदि (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          यदि (कुंजी->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (कुंजी->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_width = कुंजी->sword != SIZE_MAX && कुंजी->eword < कुंजी->sword;
      यदि (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = कुंजी_numeric (कुंजी) || कुंजी->month;
      bool पंक्ति_offset = कुंजी->eword == 0 && कुंजी->echar != 0; /* -k1.x,1.y  */
      यदि (!zero_width && !gkey_only && tab == TAB_DEFAULT && !पंक्ति_offset
          && ((!कुंजी->skipsblanks && !implicit_skip)
              || (!कुंजी->skipsblanks && कुंजी->schar)
              || (!कुंजी->skipeblanks && कुंजी->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      यदि (!gkey_only && कुंजी_numeric (कुंजी))
        {
          माप_प्रकार sword = कुंजी->sword + 1;
          माप_प्रकार eword = कुंजी->eword + 1;
          यदि (!sword)
            sword++;
          यदि (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              यदि (कुंजी->general_numeric)
                general_numeric_field_span = true;
              अन्यथा
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      यदि (ugkey.ignore && (ugkey.ignore == कुंजी->ignore))
        ugkey.ignore = रिक्त;
      यदि (ugkey.translate && (ugkey.translate == कुंजी->translate))
        ugkey.translate = रिक्त;
      ugkey.skipsblanks &= !कुंजी->skipsblanks;
      ugkey.skipeblanks &= !कुंजी->skipeblanks;
      ugkey.month &= !कुंजी->month;
      ugkey.numeric &= !कुंजी->numeric;
      ugkey.general_numeric &= !कुंजी->general_numeric;
      ugkey.human_numeric &= !कुंजी->human_numeric;
      ugkey.random &= !कुंजी->random;
      ugkey.version &= !कुंजी->version;
      ugkey.reverse &= !कुंजी->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool संख्या_locale_warned = false;
  यदि (basic_numeric_field_span)
    {
      यदि (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((वर्ण []) {thousands_sep, 0})));
          संख्या_locale_warned = true;
        }
    }
  यदि (basic_numeric_field_span || general_numeric_field_span)
    {
      यदि (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((वर्ण []) {decimal_point, 0})));
          संख्या_locale_warned = true;
        }
      अन्यथा यदि (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((वर्ण []) {tab, 0})));
        }
      अन्यथा यदि (general_numeric_field_span && tab == '+')
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
  यदि ((basic_numeric_field || general_numeric_field) && ! संख्या_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((वर्ण []) {decimal_point, 0})));

    }

  यदि (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  यदि (!default_कुंजी_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      यदि (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      वर्ण opts[परिमाण short_options];
      कुंजी_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (म_माप (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  यदि (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

स्थिर पूर्णाङ्क
diff_reversed (पूर्णाङ्क diff, bool reversed)
{
  प्रत्यागम reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

स्थिर पूर्णाङ्क
keycompare (संरचना पंक्ति ध्रुव *a, संरचना पंक्ति ध्रुव *b)
{
  संरचना keyfield *कुंजी = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  वर्ण *texta = a->keybeg;
  वर्ण *textb = b->keybeg;
  वर्ण *lima = a->keylim;
  वर्ण *limb = b->keylim;

  पूर्णाङ्क diff;

  यावत् (true)
    {
      वर्ण ध्रुव *translate = कुंजी->translate;
      bool ध्रुव *ignore = कुंजी->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      माप_प्रकार lena = lima - texta;
      माप_प्रकार lenb = limb - textb;

      यदि (hard_LC_COLLATE || कुंजी_numeric (कुंजी)
          || कुंजी->month || कुंजी->random || कुंजी->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          वर्ण *ta = texta;
          वर्ण *tb = textb;
          माप_प्रकार tlena = lena;
          माप_प्रकार tlenb = lenb;
          वर्ण enda = ta[tlena];
          वर्ण endb = tb[tlenb];

          शून्य *allocated = रिक्त;
          वर्ण stackbuf[4000];

          यदि (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              माप_प्रकार i;

              /* Allocate space for copies.  */
              माप_प्रकार size = lena + 1 + lenb + 1;
              यदि (size <= परिमाण stackbuf)
                ta = stackbuf;
              अन्यथा
                ta = allocated = xmalloc (size);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              प्रति (tlena = i = 0; i < lena; i++)
                यदि (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              प्रति (tlenb = i = 0; i < lenb; i++)
                यदि (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          यदि (कुंजी->numeric)
            diff = numcompare (ta, tb);
          अन्यथा यदि (कुंजी->general_numeric)
            diff = general_numcompare (ta, tb);
          अन्यथा यदि (कुंजी->human_numeric)
            diff = human_numcompare (ta, tb);
          अन्यथा यदि (कुंजी->month)
            diff = getmonth (ta, रिक्त) - getmonth (tb, रिक्त);
          अन्यथा यदि (कुंजी->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          अन्यथा यदि (कुंजी->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          अन्यथा
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              यदि (tlena == 0)
                diff = - NONZERO (tlenb);
              अन्यथा यदि (tlenb == 0)
                diff = 1;
              अन्यथा
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          मुक्त (allocated);
        }
      अन्यथा यदि (ignore)
        {
#define CMP_WITH_IGNORE(A, B)						\
  कुरु									\
    {									\
          यावत् (true)							\
            {								\
              यावत् (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              यावत् (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              यदि (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  भङ्ग;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              यदि (diff)							\
                भङ्ग;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  यावत् (0)

          यदि (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          अन्यथा
            CMP_WITH_IGNORE (*texta, *textb);
        }
      अन्यथा
        {
          माप_प्रकार lenmin = MIN (lena, lenb);
          यदि (lenmin == 0)
            diff = 0;
          अन्यथा यदि (translate)
            {
              माप_प्रकार i = 0;
              कुरु
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  यदि (diff)
                    भङ्ग;
                  i++;
                }
              यावत् (i < lenmin);
            }
          अन्यथा
            diff = स_भेद (texta, textb, lenmin);

          यदि (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      यदि (diff)
        भङ्ग;

      कुंजी = कुंजी->next;
      यदि (! कुंजी)
        प्रत्यागम 0;

      /* Find the beginning and limit of the next field.  */
      यदि (कुंजी->eword != SIZE_MAX)
        lima = limfield (a, कुंजी), limb = limfield (b, कुंजी);
      अन्यथा
        lima = a->text + a->length - 1, limb = b->text + b->length - 1;

      यदि (कुंजी->sword != SIZE_MAX)
        texta = begfield (a, कुंजी), textb = begfield (b, कुंजी);
      अन्यथा
        {
          texta = a->text, textb = b->text;
          यदि (कुंजी->skipsblanks)
            {
              यावत् (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              यावत् (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  प्रत्यागम diff_reversed (diff, कुंजी->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

स्थिर पूर्णाङ्क
compare (संरचना पंक्ति ध्रुव *a, संरचना पंक्ति ध्रुव *b)
{
  पूर्णाङ्क diff;
  माप_प्रकार alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  यदि (keylist)
    {
      diff = keycompare (a, b);
      यदि (diff || unique || stable)
        प्रत्यागम diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->length - 1, blen = b->length - 1;

  यदि (alen == 0)
    diff = - NONZERO (blen);
  अन्यथा यदि (blen == 0)
    diff = 1;
  अन्यथा यदि (hard_LC_COLLATE)
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
      यदि (!diff)
        diff = _GL_CMP (alen, blen);
    }

  प्रत्यागम diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

स्थिर शून्य
write_पंक्ति (संरचना पंक्ति ध्रुव *पंक्ति, खाता *fp, वर्ण ध्रुव *output_file)
{
  वर्ण *buf = पंक्ति->text;
  माप_प्रकार n_bytes = पंक्ति->length;
  वर्ण *ebuf = buf + n_bytes;

  यदि (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      वर्ण ध्रुव *c = buf;

      यावत् (c < ebuf)
        {
          वर्ण wc = *c++;
          यदि (wc == '\t')
            wc = '>';
          अन्यथा यदि (c == ebuf)
            wc = '\n';
          यदि (ख_अक्षर_दो (wc, fp) == खातापूर्ण)
            sort_die (_("write failed"), output_file);
        }

      debug_पंक्ति (पंक्ति);
    }
  अन्यथा
    {
      ebuf[-1] = eolchar;
      यदि (ख_डालो (buf, 1, n_bytes, fp) != n_bytes)
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
  खाता *fp = xfopen (file_name, "r");
  संरचना buffer buf;		/* Input buffer. */
  संरचना पंक्ति temp;		/* Copy of previous line. */
  माप_प्रकार alloc = 0;
  uintmax_t पंक्ति_संख्या = 0;
  संरचना keyfield ध्रुव *कुंजी = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, परिमाण (संरचना पंक्ति),
           MAX (merge_buffer_size, sort_size));
  temp.text = रिक्त;

  यावत् (fillbuf (&buf, fp, file_name, रिक्त))
    {
      संरचना पंक्ति ध्रुव *पंक्ति = buffer_linelim (&buf);
      संरचना पंक्ति ध्रुव *linebase = पंक्ति - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      यदि (alloc && nonunique <= compare (&temp, पंक्ति - 1))
        {
        found_disorder:
          {
            यदि (checkonly == 'c')
              {
                संरचना पंक्ति ध्रुव *disorder_पंक्ति = पंक्ति - 1;
                uintmax_t disorder_पंक्ति_संख्या =
                  buffer_linelim (&buf) - disorder_पंक्ति + पंक्ति_संख्या;
                ख_लिखो (मानक_त्रुटि, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_पंक्ति_संख्या);
                write_पंक्ति (disorder_पंक्ति, मानक_त्रुटि, _("standard error"));
              }

            ordered = false;
            भङ्ग;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      यावत् (linebase < --पंक्ति)
        यदि (nonunique <= compare (पंक्ति, पंक्ति - 1))
          गच्छ found_disorder;

      पंक्ति_संख्या += buf.nlines;

      /* Save the last line of the buffer.  */
      यदि (alloc < पंक्ति->length)
        {
          कुरु
            {
              alloc *= 2;
              यदि (! alloc)
                {
                  alloc = पंक्ति->length;
                  भङ्ग;
                }
            }
          यावत् (alloc < पंक्ति->length);

          मुक्त (temp.text);
          temp.text = xmalloc (alloc);
        }
      स_नकल (temp.text, पंक्ति->text, पंक्ति->length);
      temp.length = पंक्ति->length;
      यदि (कुंजी)
        {
          temp.keybeg = temp.text + (पंक्ति->keybeg - पंक्ति->text);
          temp.keylim = temp.text + (पंक्ति->keylim - पंक्ति->text);
        }
    }

  xfclose (fp, file_name);
  मुक्त (buf.buf);
  मुक्त (temp.text);
  प्रत्यागम ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

स्थिर माप_प्रकार
open_निवेश_files (संरचना sortfile *files, माप_प्रकार nfiles, खाता ***pfps)
{
  खाता **fps = *pfps = xnmalloc (nfiles, परिमाण *fps);
  पूर्णाङ्क i;

  /* Open as many input files as we can.  */
  प्रति (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].temp && files[i].temp->state != UNCOMPRESSED
                ? open_temp (files[i].temp)
                : stream_open (files[i].name, "r"));
      यदि (!fps[i])
        भङ्ग;
    }

  प्रत्यागम i;
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
          खाता *ofp, वर्ण ध्रुव *output_file, खाता **fps)
{
  संरचना buffer *buffer = xnmalloc (nfiles, परिमाण *buffer);
                                /* Input buffers for each file. */
  संरचना पंक्ति saved;		/* Saved line storage for unique check. */
  संरचना पंक्ति ध्रुव *savedline = रिक्त;
                                /* &saved if there is a saved line. */
  माप_प्रकार savealloc = 0;		/* Size allocated for the saved line. */
  संरचना पंक्ति ध्रुव **cur = xnmalloc (nfiles, परिमाण *cur);
                                /* Current line in each line table. */
  संरचना पंक्ति ध्रुव **base = xnmalloc (nfiles, परिमाण *base);
                                /* Base of each line table.  */
  माप_प्रकार *ord = xnmalloc (nfiles, परिमाण *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  माप_प्रकार t;
  संरचना keyfield ध्रुव *कुंजी = keylist;
  saved.text = रिक्त;

  /* Read initial lines from each input file. */
  प्रति (माप_प्रकार i = 0; i < nfiles; )
    {
      initbuf (&buffer[i], परिमाण (संरचना पंक्ति),
               MAX (merge_buffer_size, sort_size / nfiles));
      यदि (fillbuf (&buffer[i], fps[i], files[i].name, रिक्त))
        {
          संरचना पंक्ति ध्रुव *linelim = buffer_linelim (&buffer[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - buffer[i].nlines;
          i++;
        }
      अन्यथा
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          यदि (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          मुक्त (buffer[i].buf);
          --nfiles;
          प्रति (माप_प्रकार j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  प्रति (माप_प्रकार i = 0; i < nfiles; ++i)
    ord[i] = i;
  प्रति (माप_प्रकार i = 1; i < nfiles; ++i)
    यदि (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  यावत् (nfiles)
    {
      संरचना पंक्ति ध्रुव *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      यदि (unique)
        {
          यदि (savedline && compare (savedline, smallest))
            {
              savedline = रिक्त;
              write_पंक्ति (&saved, ofp, output_file);
            }
          यदि (!savedline)
            {
              savedline = &saved;
              यदि (savealloc < smallest->length)
                {
                  कुरु
                    यदि (! savealloc)
                      {
                        savealloc = smallest->length;
                        भङ्ग;
                      }
                  यावत् ((savealloc *= 2) < smallest->length);

                  मुक्त (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.length = smallest->length;
              स_नकल (saved.text, smallest->text, saved.length);
              यदि (कुंजी)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      अन्यथा
        write_पंक्ति (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      यदि (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      अन्यथा
        {
          यदि (fillbuf (&buffer[ord[0]], fps[ord[0]], files[ord[0]].name,
                       रिक्त))
            {
              संरचना पंक्ति ध्रुव *linelim = buffer_linelim (&buffer[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - buffer[ord[0]].nlines;
            }
          अन्यथा
            {
              /* We reached EOF on fps[ord[0]].  */
              प्रति (माप_प्रकार i = 1; i < nfiles; ++i)
                यदि (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              यदि (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              मुक्त (buffer[ord[0]].buf);
              प्रति (माप_प्रकार i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  buffer[i] = buffer[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              प्रति (माप_प्रकार i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              अनुवर्त;
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
        माप_प्रकार गिनती_of_smaller_पंक्तियाँ;

        यावत् (lo < hi)
          {
            पूर्णाङ्क cmp = compare (cur[ord0], cur[ord[probe]]);
            यदि (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            अन्यथा
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        गिनती_of_smaller_पंक्तियाँ = lo - 1;
        प्रति (माप_प्रकार j = 0; j < गिनती_of_smaller_पंक्तियाँ; j++)
          ord[j] = ord[j + 1];
        ord[गिनती_of_smaller_पंक्तियाँ] = ord0;
      }
    }

  यदि (unique && savedline)
    {
      write_पंक्ति (&saved, ofp, output_file);
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
            खाता *ofp, वर्ण ध्रुव *output_file)
{
  खाता **fps;
  माप_प्रकार nopened = open_निवेश_files (files, nfiles, &fps);
  यदि (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  प्रत्यागम nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

स्थिर शून्य
mergelines (संरचना पंक्ति *restrict t, माप_प्रकार nlines,
            संरचना पंक्ति ध्रुव *restrict lo)
{
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  संरचना पंक्ति *hi = t - nlo;

  यावत् (true)
    यदि (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        यदि (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            प्रत्यागम;
          }
      }
    अन्यथा
      {
        *--t = *--hi;
        यदि (! --nhi)
          {
            कुरु
              *--t = *--lo;
            यावत् (--nlo);

            प्रत्यागम;
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
sequential_sort (संरचना पंक्ति *restrict पंक्तियाँ, माप_प्रकार nlines,
                 संरचना पंक्ति *restrict temp, bool to_temp)
{
  यदि (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      पूर्णाङ्क swap = (0 < compare (&पंक्तियाँ[-1], &पंक्तियाँ[-2]));
      यदि (to_temp)
        {
          temp[-1] = पंक्तियाँ[-1 - swap];
          temp[-2] = पंक्तियाँ[-2 + swap];
        }
      अन्यथा यदि (swap)
        {
          temp[-1] = पंक्तियाँ[-1];
          पंक्तियाँ[-1] = पंक्तियाँ[-2];
          पंक्तियाँ[-2] = temp[-1];
        }
    }
  अन्यथा
    {
      माप_प्रकार nlo = nlines / 2;
      माप_प्रकार nhi = nlines - nlo;
      संरचना पंक्ति *lo = पंक्तियाँ;
      संरचना पंक्ति *hi = पंक्तियाँ - nlo;

      sequential_sort (hi, nhi, temp - (to_temp ? nlo : 0), to_temp);
      यदि (1 < nlo)
        sequential_sort (lo, nlo, temp, !to_temp);
      अन्यथा यदि (!to_temp)
        temp[-1] = lo[-1];

      संरचना पंक्ति *dest;
      संरचना पंक्ति ध्रुव *sorted_lo;
      यदि (to_temp)
        {
          dest = temp;
          sorted_lo = पंक्तियाँ;
        }
      अन्यथा
        {
          dest = पंक्तियाँ;
          sorted_lo = temp;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

स्थिर संरचना merge_गाँठ *init_गाँठ (संरचना merge_गाँठ *restrict,
                                     संरचना merge_गाँठ *restrict,
                                     संरचना पंक्ति *, माप_प्रकार, माप_प्रकार, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
स्थिर संरचना merge_गाँठ *
merge_वृक्ष_init (माप_प्रकार nthreads, माप_प्रकार nlines, संरचना पंक्ति *dest)
{
  संरचना merge_गाँठ *merge_वृक्ष = xmalloc (2 * परिमाण *merge_वृक्ष * nthreads);

  संरचना merge_गाँठ *मूल = merge_वृक्ष;
  मूल->lo = मूल->hi = मूल->end_lo = मूल->end_hi = रिक्त;
  मूल->dest = रिक्त;
  मूल->nlo = मूल->nhi = nlines;
  मूल->parent = रिक्त;
  मूल->level = MERGE_END;
  मूल->queued = false;
  pthread_mutex_init (&मूल->lock, रिक्त);

  init_गाँठ (मूल, मूल + 1, dest, nthreads, nlines, false);
  प्रत्यागम merge_वृक्ष;
}

/* Destroy the merge tree. */
स्थिर शून्य
merge_वृक्ष_destroy (माप_प्रकार nthreads, संरचना merge_गाँठ *merge_वृक्ष)
{
  माप_प्रकार n_nodes = nthreads * 2;
  संरचना merge_गाँठ *गाँठ = merge_वृक्ष;

  यावत् (n_nodes--)
    {
      pthread_mutex_destroy (&गाँठ->lock);
      गाँठ++;
    }

  मुक्त (merge_वृक्ष);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

स्थिर संरचना merge_गाँठ *
init_गाँठ (संरचना merge_गाँठ *restrict parent,
           संरचना merge_गाँठ *restrict गाँठ_pool,
           संरचना पंक्ति *dest, माप_प्रकार nthreads,
           माप_प्रकार total_पंक्तियाँ, bool is_lo_child)
{
  माप_प्रकार nlines = (is_lo_child ? parent->nlo : parent->nhi);
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  संरचना पंक्ति *lo = dest - total_पंक्तियाँ;
  संरचना पंक्ति *hi = lo - nlo;
  संरचना पंक्ति **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  संरचना merge_गाँठ *गाँठ = गाँठ_pool++;
  गाँठ->lo = गाँठ->end_lo = lo;
  गाँठ->hi = गाँठ->end_hi = hi;
  गाँठ->dest = parent_end;
  गाँठ->nlo = nlo;
  गाँठ->nhi = nhi;
  गाँठ->parent = parent;
  गाँठ->level = parent->level + 1;
  गाँठ->queued = false;
  pthread_mutex_init (&गाँठ->lock, रिक्त);

  यदि (nthreads > 1)
    {
      माप_प्रकार lo_threads = nthreads / 2;
      माप_प्रकार hi_threads = nthreads - lo_threads;
      गाँठ->lo_child = गाँठ_pool;
      गाँठ_pool = init_गाँठ (गाँठ, गाँठ_pool, lo, lo_threads,
                             total_पंक्तियाँ, true);
      गाँठ->hi_child = गाँठ_pool;
      गाँठ_pool = init_गाँठ (गाँठ, गाँठ_pool, hi, hi_threads,
                             total_पंक्तियाँ, false);
    }
  अन्यथा
    {
      गाँठ->lo_child = रिक्त;
      गाँठ->hi_child = रिक्त;
    }
  प्रत्यागम गाँठ_pool;
}


/* Compare two merge nodes A and B for priority.  */

स्थिर पूर्णाङ्क
compare_nodes (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  संरचना merge_गाँठ ध्रुव *nodea = a;
  संरचना merge_गाँठ ध्रुव *nodeb = b;
  यदि (nodea->level == nodeb->level)
      प्रत्यागम (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  प्रत्यागम nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

स्थिर अन्तःस्थ शून्य
lock_गाँठ (संरचना merge_गाँठ *गाँठ)
{
  pthread_mutex_lock (&गाँठ->lock);
}

/* Unlock a merge tree NODE. */

स्थिर अन्तःस्थ शून्य
unlock_गाँठ (संरचना merge_गाँठ *गाँठ)
{
  pthread_mutex_unlock (&गाँठ->lock);
}

/* Destroy merge QUEUE. */

स्थिर शून्य
queue_destroy (संरचना merge_गाँठ_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

स्थिर शून्य
queue_init (संरचना merge_गाँठ_queue *queue, माप_प्रकार nthreads)
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
queue_जोड़ो (संरचना merge_गाँठ_queue *queue, संरचना merge_गाँठ *गाँठ)
{
  pthread_mutex_lock (&queue->mutex);
  heap_जोड़ो (queue->priority_queue, गाँठ);
  गाँठ->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

स्थिर संरचना merge_गाँठ *
queue_निकालो (संरचना merge_गाँठ_queue *queue)
{
  संरचना merge_गाँठ *गाँठ;
  pthread_mutex_lock (&queue->mutex);
  यावत् (! (गाँठ = heap_remove_शीर्ष (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_गाँठ (गाँठ);
  गाँठ->queued = false;
  प्रत्यागम गाँठ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

स्थिर शून्य
write_unique (संरचना पंक्ति ध्रुव *पंक्ति, खाता *tfp, वर्ण ध्रुव *temp_output)
{
  यदि (unique)
    {
      यदि (saved_पंक्ति.text && ! compare (पंक्ति, &saved_पंक्ति))
        प्रत्यागम;
      saved_पंक्ति = *पंक्ति;
    }

  write_पंक्ति (पंक्ति, tfp, temp_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

स्थिर शून्य
mergelines_गाँठ (संरचना merge_गाँठ *restrict गाँठ, माप_प्रकार total_पंक्तियाँ,
                 खाता *tfp, वर्ण ध्रुव *temp_output)
{
  संरचना पंक्ति *lo_orig = गाँठ->lo;
  संरचना पंक्ति *hi_orig = गाँठ->hi;
  माप_प्रकार to_merge = MAX_MERGE (total_पंक्तियाँ, गाँठ->level);
  माप_प्रकार merged_lo;
  माप_प्रकार merged_hi;

  यदि (गाँठ->level > MERGE_मूल)
    {
      /* Merge to destination buffer. */
      संरचना पंक्ति *dest = *गाँठ->dest;
      यावत् (गाँठ->lo != गाँठ->end_lo && गाँठ->hi != गाँठ->end_hi && to_merge--)
        यदि (compare (गाँठ->lo - 1, गाँठ->hi - 1) <= 0)
          *--dest = *--गाँठ->lo;
        अन्यथा
          *--dest = *--गाँठ->hi;

      merged_lo = lo_orig - गाँठ->lo;
      merged_hi = hi_orig - गाँठ->hi;

      यदि (गाँठ->nhi == merged_hi)
        यावत् (गाँठ->lo != गाँठ->end_lo && to_merge--)
          *--dest = *--गाँठ->lo;
      अन्यथा यदि (गाँठ->nlo == merged_lo)
        यावत् (गाँठ->hi != गाँठ->end_hi && to_merge--)
          *--dest = *--गाँठ->hi;
      *गाँठ->dest = dest;
    }
  अन्यथा
    {
      /* Merge directly to output. */
      यावत् (गाँठ->lo != गाँठ->end_lo && गाँठ->hi != गाँठ->end_hi && to_merge--)
        {
          यदि (compare (गाँठ->lo - 1, गाँठ->hi - 1) <= 0)
            write_unique (--गाँठ->lo, tfp, temp_output);
          अन्यथा
            write_unique (--गाँठ->hi, tfp, temp_output);
        }

      merged_lo = lo_orig - गाँठ->lo;
      merged_hi = hi_orig - गाँठ->hi;

      यदि (गाँठ->nhi == merged_hi)
        {
          यावत् (गाँठ->lo != गाँठ->end_lo && to_merge--)
            write_unique (--गाँठ->lo, tfp, temp_output);
        }
      अन्यथा यदि (गाँठ->nlo == merged_lo)
        {
          यावत् (गाँठ->hi != गाँठ->end_hi && to_merge--)
            write_unique (--गाँठ->hi, tfp, temp_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - गाँठ->lo;
  merged_hi = hi_orig - गाँठ->hi;
  गाँठ->nlo -= merged_lo;
  गाँठ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

स्थिर शून्य
queue_check_जोड़ो (संरचना merge_गाँठ_queue *queue, संरचना merge_गाँठ *गाँठ)
{
  यदि (! गाँठ->queued)
    {
      bool lo_avail = (गाँठ->lo - गाँठ->end_lo) != 0;
      bool hi_avail = (गाँठ->hi - गाँठ->end_hi) != 0;
      यदि (lo_avail ? hi_avail || ! गाँठ->nhi : hi_avail && ! गाँठ->nlo)
        queue_जोड़ो (queue, गाँठ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

स्थिर शून्य
queue_check_जोड़ो_parent (संरचना merge_गाँठ_queue *queue,
                           संरचना merge_गाँठ *गाँठ)
{
  यदि (गाँठ->level > MERGE_मूल)
    {
      lock_गाँठ (गाँठ->parent);
      queue_check_जोड़ो (queue, गाँठ->parent);
      unlock_गाँठ (गाँठ->parent);
    }
  अन्यथा यदि (गाँठ->nlo + गाँठ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      queue_जोड़ो (queue, गाँठ->parent);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

स्थिर शून्य
merge_loop (संरचना merge_गाँठ_queue *queue,
            माप_प्रकार total_पंक्तियाँ, खाता *tfp, वर्ण ध्रुव *temp_output)
{
  यावत् (true)
    {
      संरचना merge_गाँठ *गाँठ = queue_निकालो (queue);

      यदि (गाँठ->level == MERGE_END)
        {
          unlock_गाँठ (गाँठ);
          /* Reinsert so other threads can pop it. */
          queue_जोड़ो (queue, गाँठ);
          भङ्ग;
        }
      mergelines_गाँठ (गाँठ, total_पंक्तियाँ, tfp, temp_output);
      queue_check_जोड़ो (queue, गाँठ);
      queue_check_जोड़ो_parent (queue, गाँठ);

      unlock_गाँठ (गाँठ);
    }
}


स्थिर शून्य sortlines (संरचना पंक्ति *restrict, माप_प्रकार, माप_प्रकार,
                       संरचना merge_गाँठ *, संरचना merge_गाँठ_queue *,
                       खाता *, वर्ण ध्रुव *);

/* Thread arguments for sortlines_thread. */

संरचना thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  संरचना पंक्ति *पंक्तियाँ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  माप_प्रकार nthreads;

  /* Number of lines in LINES and DEST.  */
  माप_प्रकार ध्रुव total_पंक्तियाँ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  संरचना merge_गाँठ *ध्रुव गाँठ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  संरचना merge_गाँठ_queue *ध्रुव queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  खाता *tfp;
  वर्ण ध्रुव *output_temp;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

स्थिर शून्य *
sortlines_thread (शून्य *data)
{
  संरचना thread_args ध्रुव *args = data;
  sortlines (args->पंक्तियाँ, args->nthreads, args->total_पंक्तियाँ,
             args->गाँठ, args->queue, args->tfp,
             args->output_temp);
  प्रत्यागम रिक्त;
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
sortlines (संरचना पंक्ति *restrict पंक्तियाँ, माप_प्रकार nthreads,
           माप_प्रकार total_पंक्तियाँ, संरचना merge_गाँठ *गाँठ,
           संरचना merge_गाँठ_queue *queue, खाता *tfp, वर्ण ध्रुव *temp_output)
{
  माप_प्रकार nlines = गाँठ->nlo + गाँठ->nhi;

  /* Calculate thread arguments. */
  माप_प्रकार lo_threads = nthreads / 2;
  माप_प्रकार hi_threads = nthreads - lo_threads;
  pthread_t thread;
  संरचना thread_args args = {पंक्तियाँ, lo_threads, total_पंक्तियाँ,
                             गाँठ->lo_child, queue, tfp, temp_output};

  यदि (nthreads > 1 && SUBTHREAD_पंक्तियाँ_HEURISTIC <= nlines
      && pthread_create (&thread, रिक्त, sortlines_thread, &args) == 0)
    {
      sortlines (पंक्तियाँ - गाँठ->nlo, hi_threads, total_पंक्तियाँ,
                 गाँठ->hi_child, queue, tfp, temp_output);
      pthread_join (thread, रिक्त);
    }
  अन्यथा
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      माप_प्रकार nlo = गाँठ->nlo;
      माप_प्रकार nhi = गाँठ->nhi;
      संरचना पंक्ति *temp = पंक्तियाँ - total_पंक्तियाँ;
      यदि (1 < nhi)
        sequential_sort (पंक्तियाँ - nlo, nhi, temp - nlo / 2, false);
      यदि (1 < nlo)
        sequential_sort (पंक्तियाँ, nlo, temp, false);

      /* Update merge NODE. No need to lock yet. */
      गाँठ->lo = पंक्तियाँ;
      गाँठ->hi = पंक्तियाँ - nlo;
      गाँठ->end_lo = पंक्तियाँ - nlo;
      गाँठ->end_hi = पंक्तियाँ - nlo - nhi;

      queue_जोड़ो (queue, गाँठ);
      merge_loop (queue, total_पंक्तियाँ, tfp, temp_output);
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
avoid_trashing_निवेश (संरचना sortfile *files, माप_प्रकार ntemps,
                      माप_प्रकार nfiles, वर्ण ध्रुव *outfile)
{
  संरचना tempnode *tempcopy = रिक्त;

  प्रति (माप_प्रकार i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      संरचना stat instat;

      यदि (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      अन्यथा
        {
          संरचना stat *outst = get_outstatus ();
          यदि (!outst)
            भङ्ग;

          same = (((is_stdin
                    ? ख_स्थिति (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      यदि (same)
        {
          यदि (! tempcopy)
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
check_inputs (वर्ण *ध्रुव *files, माप_प्रकार nfiles)
{
  प्रति (माप_प्रकार i = 0; i < nfiles; i++)
    {
      यदि (streq (files[i], "-"))
        अनुवर्त;

      यदि (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

स्थिर शून्य
check_output (वर्ण ध्रुव *outfile)
{
  यदि (outfile)
    {
      पूर्णाङ्क oflags = O_WRONLY | O_द्विआधारी | O_CLOEXEC | O_CREAT;
      पूर्णाङ्क outfd = खोलो (outfile, oflags, MODE_RW_UGO);
      यदि (outfd < 0)
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
       वर्ण ध्रुव *output_file)
{
  यावत् (nmerge < nfiles)
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
      प्रति (out = in = 0; nmerge <= nfiles - in; out++)
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

      यदि (cheap_slots < remainder)
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
      स_हटाओ (&files[out], &files[in], (nfiles - in) * परिमाण *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_निवेश (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  यावत् (true)
    {
      /* Merge directly into the output file if possible.  */
      खाता **fps;
      माप_प्रकार nopened = open_निवेश_files (files, nfiles, &fps);

      यदि (nopened == nfiles)
        {
          खाता *ofp = stream_open (output_file, "w");
          यदि (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              भङ्ग;
            }
          यदि (त्रुटि_सं != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      अन्यथा यदि (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      खाता *tfp;
      संरचना tempnode *temp;
      कुरु
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          temp = maybe_create_temp (&tfp, ! (nopened <= 2));
        }
      यावत् (!temp);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, temp->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = temp->name;
      files[0].temp = temp;

      स_हटाओ (&files[1], &files[nopened], (nfiles - nopened) * परिमाण *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

स्थिर शून्य
sort (वर्ण *ध्रुव *files, माप_प्रकार nfiles, वर्ण ध्रुव *output_file,
      माप_प्रकार nthreads)
{
  संरचना buffer buf;
  संरचना sort_buffer_policy policy;
  माप_प्रकार ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  यावत् (nfiles)
    {
      वर्ण ध्रुव *temp_output;
      वर्ण ध्रुव *file = *files;
      खाता *fp = xfopen (file, "r");
      खाता *tfp;

      माप_प्रकार bytes_per_पंक्ति;
      यदि (nthreads > 1)
        {
          /* Get log P. */
          माप_प्रकार tmp = 1;
          माप_प्रकार mult = 1;
          यावत् (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_पंक्ति = (mult * परिमाण (संरचना पंक्ति));
        }
      अन्यथा
        bytes_per_पंक्ति = परिमाण (संरचना पंक्ति) * 3 / 2;

      यदि (! buf.alloc)
        {
          sort_buffer_policy (&fp, 1, files, nfiles, bytes_per_पंक्ति,
                              &policy);
          initbuf (&buf, bytes_per_पंक्ति, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      यावत् (fillbuf (&buf, fp, file, &policy))
        {
          संरचना पंक्ति *पंक्ति;

          यदि (buf.eof && nfiles
              && (bytes_per_पंक्ति + 1
                  < (buf.alloc - buf.used - bytes_per_पंक्ति * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.बायाँ = buf.used;
              भङ्ग;
            }

          saved_पंक्ति.text = रिक्त;
          पंक्ति = buffer_linelim (&buf);
          यदि (buf.eof && !nfiles && !ntemps && !buf.बायाँ)
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
          यदि (1 < buf.nlines)
            {
              यदि (nthreads > 1)
                {
                  संरचना merge_गाँठ_queue queue;
                  queue_init (&queue, nthreads);
                  संरचना merge_गाँठ *merge_वृक्ष =
                    merge_वृक्ष_init (nthreads, buf.nlines, पंक्ति);

                  sortlines (पंक्ति, nthreads, buf.nlines, merge_वृक्ष + 1,
                             &queue, tfp, temp_output);

                  merge_वृक्ष_destroy (nthreads, merge_वृक्ष);
                  queue_destroy (&queue);
                }
              अन्यथा
                {
                  sequential_sort (पंक्ति, buf.nlines,
                                   पंक्ति - buf.nlines, false);
                  प्रति (माप_प्रकार i = 0; i < buf.nlines; i++)
                    write_unique (पंक्ति - i - 1, tfp, temp_output);
                }
            }
          अन्यथा
            write_unique (पंक्ति - 1, tfp, temp_output);

          xfclose (tfp, temp_output);

          यदि (output_file_created)
            गच्छ finish;
        }
      xfclose (fp, file);
    }

 finish:
  मुक्त (buf.buf);

  यदि (! output_file_created)
    {
      संरचना tempnode *गाँठ = temphead;
      संरचना sortfile *tempfiles = xnmalloc (ntemps, परिमाण *tempfiles);
      प्रति (माप_प्रकार i = 0; गाँठ; i++)
        {
          tempfiles[i].name = गाँठ->name;
          tempfiles[i].temp = गाँठ;
          गाँठ = गाँठ->next;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      मुक्त (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

स्थिर शून्य
insertkey (संरचना keyfield *कुंजी_arg)
{
  संरचना keyfield **p;
  संरचना keyfield *कुंजी = xmemdup (कुंजी_arg, परिमाण *कुंजी);

  प्रति (p = &keylist; *p; p = &(*p)->next)
    अनुवर्त;
  *p = कुंजी;
  कुंजी->next = रिक्त;
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
  प्रति (संरचना keyfield *कुंजी = keylist; कुंजी; कुंजी = कुंजी->next)
    यदि (1 < (कुंजी->numeric + कुंजी->general_numeric + कुंजी->human_numeric
             + कुंजी->month + (कुंजी->version | कुंजी->random | !!कुंजी->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        वर्ण opts[परिमाण short_options];
        /* Clear flags we're not interested in.  */
        कुंजी->skipsblanks = कुंजी->skipeblanks = कुंजी->reverse = false;
        कुंजी_to_opts (कुंजी, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

स्थिर वर्ण ध्रुव *
parse_field_गिनती (वर्ण ध्रुव *string, माप_प्रकार *val, वर्ण ध्रुव *msgid)
{
  वर्ण *suffix;
  uintmax_t n;

  विकल्प (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    पक्ष LONGINT_OK:
    पक्ष LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      यदि (*val == n)
        भङ्ग;
      FALLTHROUGH;
    पक्ष LONGINT_OVERFLOW:
    पक्ष LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      भङ्ग;

    पक्ष LONGINT_INVALID:
      यदि (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      प्रत्यागम रिक्त;
    }

  प्रत्यागम suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

स्थिर वर्ण *
set_ordering (वर्ण ध्रुव *s, संरचना keyfield *कुंजी, गणना blanktype blanktype)
{
  यावत् (*s)
    {
      विकल्प (*s)
        {
        पक्ष 'b':
          यदि (blanktype == bl_start || blanktype == bl_both)
            कुंजी->skipsblanks = true;
          यदि (blanktype == bl_end || blanktype == bl_both)
            कुंजी->skipeblanks = true;
          भङ्ग;
        पक्ष 'd':
          कुंजी->ignore = nondictionary;
          भङ्ग;
        पक्ष 'f':
          कुंजी->translate = fold_toupper;
          भङ्ग;
        पक्ष 'g':
          कुंजी->general_numeric = true;
          भङ्ग;
        पक्ष 'h':
          कुंजी->human_numeric = true;
          भङ्ग;
        पक्ष 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          यदि (! कुंजी->ignore)
            कुंजी->ignore = nonprinting;
          भङ्ग;
        पक्ष 'M':
          कुंजी->month = true;
          भङ्ग;
        पक्ष 'n':
          कुंजी->numeric = true;
          भङ्ग;
        पक्ष 'R':
          कुंजी->random = true;
          भङ्ग;
        पक्ष 'r':
          कुंजी->reverse = true;
          भङ्ग;
        पक्ष 'V':
          कुंजी->version = true;
          भङ्ग;
        स्वभाव:
          प्रत्यागम (वर्ण *) s;
        }
      ++s;
    }
  प्रत्यागम (वर्ण *) s;
}

/* Initialize KEY.  */

स्थिर संरचना keyfield *
कुंजी_init (संरचना keyfield *कुंजी)
{
  स_रखो (कुंजी, 0, परिमाण *कुंजी);
  कुंजी->eword = SIZE_MAX;
  प्रत्यागम कुंजी;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  संरचना keyfield *कुंजी;
  संरचना keyfield कुंजी_buf;
  संरचना keyfield gkey;
  bool gkey_only = false;
  वर्ण ध्रुव *s;
  पूर्णाङ्क c = 0;
  वर्ण checkonly = 0;
  bool mergeonly = false;
  वर्ण *random_source = रिक्त;
  bool need_random = false;
  माप_प्रकार nthreads = 0;
  माप_प्रकार nfiles = 0;
  bool posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != रिक्त);
  पूर्णाङ्क posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  वर्ण **files;
  वर्ण *files_from = रिक्त;
  संरचना Tokens tok;
  वर्ण ध्रुव *outfile = रिक्त;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#यदि HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#endif

  /* Get locale's representation of the decimal point.  */
  {
    संरचना स्था_बदल ध्रुव *locale = क्षेत्र_बदलो ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    यदि (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    यदि (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    यदि (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    गणना { nsigs = countof (term_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_signals);
    प्रति (माप_प्रकार i = 0; i < nsigs; i++)
      {
        यदि (term_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        sigaction (term_sig[i], रिक्त, &act);
        यदि (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    प्रति (माप_प्रकार i = 0; i < nsigs; i++)
      {
        यदि (term_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        यदि (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, रिक्त);
      }
  }
  सङ्केत (SIGCHLD, संक_पूर्व); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  निकास_पर (exit_cleanup);

  कुंजी_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, परिमाण *files);

  यावत् (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      पूर्णाङ्क oi = -1;

      यदि (c == -1
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
          यदि (argc <= optind)
            भङ्ग;
          files[nfiles++] = argv[optind++];
        }
      अन्यथा विकल्प (c)
        {
        पक्ष 1:
          कुंजी = रिक्त;
          यदि (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              यदि (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  कुंजी = कुंजी_init (&कुंजी_buf);
                  s = parse_field_गिनती (optarg + 1, &कुंजी->sword, रिक्त);
                  यदि (s && *s == '.')
                    s = parse_field_गिनती (s + 1, &कुंजी->schar, रिक्त);
                  यदि (! (कुंजी->sword || कुंजी->schar))
                    कुंजी->sword = SIZE_MAX;
                  यदि (! s || *set_ordering (s, कुंजी, bl_start))
                    कुंजी = रिक्त;
                  अन्यथा
                    {
                      यदि (minus_pos_usage)
                        {
                          वर्ण ध्रुव *optarg1 = argv[optind++];
                          s = parse_field_गिनती (optarg1 + 1, &कुंजी->eword,
                                             N_("invalid number after '-'"));
                          यदि (*s == '.')
                            s = parse_field_गिनती (s + 1, &कुंजी->echar,
                                               N_("invalid number after '.'"));
                          यदि (!कुंजी->echar && कुंजी->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              कुंजी->eword--;
                            }
                          यदि (*set_ordering (s, कुंजी, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      कुंजी->traditional_used = true;
                      insertkey (कुंजी);
                    }
                }
            }
          यदि (! कुंजी)
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
          यदि (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          भङ्ग;

        पक्ष COMPRESS_PROGRAM_OPTION:
          यदि (compress_program && !streq (compress_program, optarg))
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
          कुंजी = कुंजी_init (&कुंजी_buf);

          /* Get POS1. */
          s = parse_field_गिनती (optarg, &कुंजी->sword,
                                 N_("invalid number at field start"));
          यदि (! कुंजी->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          यदि (*s == '.')
            {
              s = parse_field_गिनती (s + 1, &कुंजी->schar,
                                     N_("invalid number after '.'"));
              यदि (! कुंजी->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          यदि (! (कुंजी->sword || कुंजी->schar))
            कुंजी->sword = SIZE_MAX;
          s = set_ordering (s, कुंजी, bl_start);
          यदि (*s != ',')
            {
              कुंजी->eword = SIZE_MAX;
              कुंजी->echar = 0;
            }
          अन्यथा
            {
              /* Get POS2. */
              s = parse_field_गिनती (s + 1, &कुंजी->eword,
                                     N_("invalid number after ','"));
              यदि (! कुंजी->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              यदि (*s == '.')
                {
                  s = parse_field_गिनती (s + 1, &कुंजी->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, कुंजी, bl_end);
            }
          यदि (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (कुंजी);
          भङ्ग;

        पक्ष 'm':
          mergeonly = true;
          भङ्ग;

        पक्ष NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          भङ्ग;

        पक्ष 'o':
          यदि (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          भङ्ग;

        पक्ष RANDOM_SOURCE_OPTION:
          यदि (random_source && !streq (random_source, optarg))
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
            यदि (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            यदि (optarg[1])
              {
                यदि (streq (optarg, "\\0"))
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
            यदि (tab != TAB_DEFAULT && tab != newtab)
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
          यदि (optarg == argv[optind - 1])
            {
              वर्ण ध्रुव *p;
              प्रति (p = optarg; c_isdigit (*p); p++)
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
  default_SIGPIPE = (सङ्केत (SIGPIPE, संक_छोड़ो) == संक_पूर्व);

  यदि (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      यदि (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      खाता *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      यदि (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      यदि (tok.n_tok)
        {
          मुक्त (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          प्रति (माप_प्रकार i = 0; i < nfiles; i++)
            {
              यदि (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              अन्यथा यदि (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  अचिन्हित दीर्घ पूर्णाङ्क file_संख्या = i + 1;
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
  प्रति (कुंजी = keylist; कुंजी; कुंजी = कुंजी->next)
    {
      यदि (default_कुंजी_compare (कुंजी) && !कुंजी->reverse)
        {
          कुंजी->ignore = gkey.ignore;
          कुंजी->translate = gkey.translate;
          कुंजी->skipsblanks = gkey.skipsblanks;
          कुंजी->skipeblanks = gkey.skipeblanks;
          कुंजी->month = gkey.month;
          कुंजी->numeric = gkey.numeric;
          कुंजी->general_numeric = gkey.general_numeric;
          कुंजी->human_numeric = gkey.human_numeric;
          कुंजी->version = gkey.version;
          कुंजी->random = gkey.random;
          कुंजी->reverse = gkey.reverse;
        }

      need_random |= कुंजी->random;
    }

  यदि (!keylist && !default_कुंजी_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  यदि (debug)
    {
      यदि (checkonly || outfile)
        {
          स्थिर वर्ण opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      यदि (locale_ok)
        locale_ok = !! रखो_क्षेत्र (LC_COLLATE, "");
      यदि (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      यदि (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (रखो_क्षेत्र (LC_COLLATE, रिक्त)));
      अन्यथा
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      कुंजी_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  यदि (need_random)
    random_md5_state_init (random_source);

  यदि (temp_dirs.n_dirs == 0)
    {
      वर्ण ध्रुव *tmp_dir = दो_पर्या ("TMPDIR");
      add_temp_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  यदि (nfiles == 0)
    {
      nfiles = 1;
      मुक्त (files);
      files = xmalloc (परिमाण *files);
      *files = (वर्ण *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  यदि (0 < sort_size)
    sort_size = MAX (sort_size, MIN_SORT_SIZE);

  यदि (checkonly)
    {
      यदि (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      यदि (outfile)
        {
          स्थिर वर्ण opts[] = {0, 'o', 0};
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

  यदि (mergeonly)
    {
      संरचना sortfile *sortfiles = xcalloc (nfiles, परिमाण *sortfiles);

      प्रति (माप_प्रकार i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  अन्यथा
    {
      यदि (!nthreads)
        {
          अचिन्हित दीर्घ पूर्णाङ्क np = अंक_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      माप_प्रकार nthreads_max = SIZE_MAX / (2 * परिमाण (संरचना merge_गाँठ));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  यदि (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    sort_die (_("close failed"), "-");

  main_exit (निकास_सफल);
}
