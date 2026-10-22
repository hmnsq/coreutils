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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <pthread.h>
#समावेश <sys/resource.h>
#समावेश <sys/types.h>
#समावेश <sys/wait.h>
#समावेश <signal.h>
#समावेश <spawn.h>
#समावेश "system.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"
#समावेश "fadvise.h"
#समावेश "filevercmp.h"
#समावेश "findprog.h"
#समावेश "flexmember.h"
#समावेश "hard-locale.h"
#समावेश "hash.h"
#समावेश "heap.h"
#समावेश "ignore-value.h"
#समावेश "mbswidth.h"
#समावेश "nproc.h"
#समावेश "physmem.h"
#समावेश "posixver.h"
#समावेश "quote.h"
#समावेश "randread.h"
#समावेश "readtokens0.h"
#समावेश "stdlib--.h"
#समावेश "strnumcmp.h"
#समावेश "term-sig.h"
#समावेश "unistd--.h"
#समावेश "xmemcoll.h"
#समावेश "xnanosleep.h"
#समावेश "xstrtol.h"
#समावेश "xstrtol-error.h"

#अगर_अघोषित RLIMIT_तथ्य
काष्ठा rlimit { माप_प्रकार rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#पूर्ण_अगर

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "sort"

#घोषणा AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#अगर HAVE_LANGINFO_CODESET
# include <langinfo.h>
#पूर्ण_अगर

#अगर !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#पूर्ण_अगर
#अगर !defined OPEN_MAX
# define OPEN_MAX 20
#पूर्ण_अगर

#घोषणा UCHAR_LIM (अच_अक्षर_उच्च + 1)

#अगर_अघोषित DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#पूर्ण_अगर

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#घोषणा MAX_MERGE(योग_2, level) (((योग_2) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
क्रमागत { SUBTHREAD_पंक्तियाँ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_पंक्तियाँ_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
क्रमागत { DEFAULT_MAX_THREADS = 8 };

/* Exit statuses.  */
क्रमागत
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    SORT_OUT_OF_ORDER = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    SORT_FAILURE = 2
  };

क्रमागत
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

क्रमागत
  {
    /* Level of the end-of-merge node, one level above the root. */
    MERGE_END = 0,

    /* Level of the root node in merge tree. */
    MERGE_मूल = 1
  };

/* The representation of the decimal point in the current locale.  */
अटल अक्षर decimal_point;

/* Thousands separator; if outside char range, there is no separator.  */
अटल पूर्णांक thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
अटल bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
अटल bool hard_LC_COLLATE;
#अगर HAVE_NL_LANGINFO
अटल bool hard_LC_TIME;
#पूर्ण_अगर

#घोषणा NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
क्रमागत blanktype { bl_start, bl_end, bl_both };

/* The character marking end of line. Default to \n. */
अटल अक्षर eolchar = '\n';

/* Lines are held in memory as counted strings. */
काष्ठा पंक्ति
{
  अक्षर *text;			/* Text of the line. */
  माप_प्रकार लंबाई;		/* Length including final newline. */
  अक्षर *keybeg;			/* Start of first key. */
  अक्षर *keylim;			/* Limit of first key. */
};

/* Input buffers. */
काष्ठा बफर
{
  अक्षर *buf;			/* Dynamically allocated buffer,
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
काष्ठा sort_बफर_policy
{
  माप_प्रकार initial;
  माप_प्रकार limit;
  bool growth_failed;
};

/* Sort key.  */
काष्ठा keyfield
{
  माप_प्रकार sword;			/* Zero-origin 'word' to start at. */
  माप_प्रकार schar;			/* Additional characters to skip. */
  माप_प्रकार eword;			/* Zero-origin last 'word' of key. */
  माप_प्रकार echar;			/* Additional characters in field. */
  bool स्थिर *ignore;		/* Boolean array of characters to ignore. */
  अक्षर स्थिर *translate;	/* Translation applied to characters. */
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
  काष्ठा keyfield *अग्र;	/* Next keyfield to try. */
};

काष्ठा month
{
  अक्षर स्थिर *name;
  पूर्णांक val;
};

/* Binary merge tree node. */
काष्ठा merge_गाँठ
{
  काष्ठा पंक्ति *lo;              /* Lines to merge from LO child node. */
  काष्ठा पंक्ति *hi;              /* Lines to merge from HI child node. */
  काष्ठा पंक्ति *end_lo;          /* End of available lines from LO. */
  काष्ठा पंक्ति *end_hi;          /* End of available lines from HI. */
  काष्ठा पंक्ति **dest;           /* Pointer to destination of merge. */
  माप_प्रकार nlo;                   /* Total Lines remaining from LO. */
  माप_प्रकार nhi;                   /* Total lines remaining from HI. */
  काष्ठा merge_गाँठ *parent;    /* Parent node. */
  काष्ठा merge_गाँठ *lo_child;  /* LO child node. */
  काष्ठा merge_गाँठ *hi_child;  /* HI child node. */
  अचिन्हित पूर्णांक level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t lock;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
काष्ठा merge_गाँठ_queue
{
  काष्ठा heap *priority_queue;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
अटल काष्ठा पंक्ति saved_पंक्ति;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
अटल bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
अटल bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
अटल bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
अटल अक्षर fold_toupper[UCHAR_LIM];

#घोषणा MONTHS_PER_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
अटल काष्ठा month monthtab[] =
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
#घोषणा NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#घोषणा MIN_MERGE_बफर_आकार (2 + माप (काष्ठा पंक्ति))

/* Minimum sort size; the code might not work with smaller sizes.  */
#घोषणा MIN_SORT_आकार (nmerge * MIN_MERGE_बफर_आकार)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
अटल माप_प्रकार merge_बफर_आकार = MAX (MIN_MERGE_बफर_आकार, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
अटल माप_प्रकार sort_आकार;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#घोषणा निवेश_FILE_आकार_GUESS (128 * 1024)

अटल काष्ठा
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  अक्षर स्थिर **dirs COUNTED_BY (n_dirs);
} अस्थायी_dirs;



/* Flag to reverse the order of all comparisons. */
अटल bool reverse;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
अटल bool stable;

/* An int value outside char range.  */
क्रमागत { NON_CHAR = अक्षर_उच्च + 1 };

/* If TAB has this value, blanks separate fields.  */
क्रमागत { TAB_DEFAULT = अक्षर_उच्च + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
अटल पूर्णांक tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
अटल bool unique;

/* Nonzero if any of the input files are the standard input. */
अटल bool have_read_stdin;

/* List of key field comparisons to be tried.  */
अटल काष्ठा keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
अटल अक्षर स्थिर *compress_program;

/* Annotate the output with extra info to aid the user.  */
अटल bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
अटल अचिन्हित पूर्णांक nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
अटल bool default_SIGPIPE;

/* The list of temporary files. */
काष्ठा tempnode
{
  काष्ठा tempnode *अस्थिर अग्र;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  अक्षर state;
  अक्षर name[FLEXIBLE_ARRAY_MEMBER];
};
अटल काष्ठा tempnode *अस्थिर temphead;
अटल काष्ठा tempnode *अस्थिर *temptail = &temphead;

/* Clean up any remaining temporary files.  */

अटल व्योम
cleanup (व्योम)
{
  क्रम (काष्ठा tempnode स्थिर *गाँठ = temphead; गाँठ; गाँठ = गाँठ->अग्र)
    unlink (गाँठ->name);
  temphead = शून्य;
}

/* Handle interrupts and hangups. */

अटल व्योम
sighandler (पूर्णांक sig)
{
  cleanup ();

  संकेत (sig, संक_पूर्व);
  उठाओ (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

अटल व्योम
sort_die (अक्षर स्थिर *message, अक्षर स्थिर *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  अगर (त्रुटि_सं == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (SORT_FAILURE, त्रुटि_सं, "%s: %s", message,
         quotef (file ? file : _("standard output")));
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
  -f, --ignore-हाल\n\
         fold lower हाल to upper हाल characters\n\
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
         see full documentation क्रम supported strings\n\
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
         reverse the परिणाम of comparisons\n\
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
      --batch-आकार=NMERGE\n\
         merge at most NMERGE inputs at once; क्रम more use अस्थायी files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check क्रम sorted निवेश; करो not sort\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but करो not report first bad पंक्ति\n\
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
         पढ़ो निवेश from the files specified by NUL-terminated names in file F;\
\n\
         If F is -, पढ़ो names from standard निवेश\n\
"));
      oputs (_("\
  -k, --कुंजी=KEYDEF\n\
         sort via a कुंजी; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; करो not sort\n\
"));
      oputs (_("\
  -o, --output=खाता\n\
         ग_लिखो परिणाम to खाता instead of standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize sort by disabling last-resort comparison\n\
"));
      oputs (_("\
  -S, --बफर-आकार=आकार_2\n\
         use आकार_2 क्रम मुख्य memory बफर\n\
"));
      oputs (_("\
  -t, --field-separator=SEP\n\
         use SEP instead of non-blank to blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=सूची\n\
         use सूची क्रम temporaries, not $TMPDIR or %s;\n\
         multiple options specify multiple directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the संख्या of sorts run concurrently to N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first of पंक्तियाँ with equal कुंजियाँ;\n\
         with -c, check क्रम strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पंक्ति delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] क्रम start and stop position, where F is a\n\
field संख्या and C a character position in the field; both are origin 1, and\n\
the stop position defaults to the पंक्ति's end.  If neither -t nor -b is in\n\
effect, characters in a field are counted from the beginning of the preceding\n\
whitespace.  OPTS is one or more single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options क्रम that कुंजी.  If no कुंजी is given, use\n\
the entire पंक्ति as the कुंजी.  Use --debug to diagnose incorrect कुंजी usage.\n\
\n\
आकार_2 may be followed by the following multiplicative suffixes:\n\
"), मानक_निकास);
      ख_माला_दो (_("\
% 1% of memory, b 1, K 1024 (शेष), and so on क्रम M, G, T, P, E, Z, Y, R, Q.\
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
क्रमागत
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

अटल अक्षर स्थिर short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

अटल काष्ठा option स्थिर long_options[] =
{
  {"ignore-leading-blanks", no_argument, शून्य, 'b'},
  {"check", optional_argument, शून्य, CHECK_OPTION},
  {"compress-program", required_argument, शून्य, COMPRESS_PROGRAM_OPTION},
  {"debug", no_argument, शून्य, DEBUG_PROGRAM_OPTION},
  {"dictionary-order", no_argument, शून्य, 'd'},
  {"ignore-case", no_argument, शून्य, 'f'},
  {"files0-from", required_argument, शून्य, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_argument, शून्य, 'g'},
  {"ignore-nonprinting", no_argument, शून्य, 'i'},
  {"key", required_argument, शून्य, 'k'},
  {"merge", no_argument, शून्य, 'm'},
  {"month-sort", no_argument, शून्य, 'M'},
  {"numeric-sort", no_argument, शून्य, 'n'},
  {"human-numeric-sort", no_argument, शून्य, 'h'},
  {"version-sort", no_argument, शून्य, 'V'},
  {"random-sort", no_argument, शून्य, 'R'},
  {"random-source", required_argument, शून्य, RANDOM_SOURCE_OPTION},
  {"sort", required_argument, शून्य, SORT_OPTION},
  {"output", required_argument, शून्य, 'o'},
  {"reverse", no_argument, शून्य, 'r'},
  {"stable", no_argument, शून्य, 's'},
  {"batch-size", required_argument, शून्य, NMERGE_OPTION},
  {"buffer-size", required_argument, शून्य, 'S'},
  {"field-separator", required_argument, शून्य, 't'},
  {"temporary-directory", required_argument, शून्य, 'T'},
  {"unique", no_argument, शून्य, 'u'},
  {"zero-terminated", no_argument, शून्य, 'z'},
  {"parallel", required_argument, शून्य, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0},
};

#घोषणा CHECK_TABLE \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

अटल अक्षर स्थिर *स्थिर check_args[] =
{
#घोषणा _ct_(_s, _c) _s,
  CHECK_TABLE शून्य
#अघोषित  _ct_
};
अटल अक्षर स्थिर check_types[] =
{
#घोषणा _ct_(_s, _c) _c,
  CHECK_TABLE
#अघोषित  _ct_
};

#घोषणा SORT_TABLE \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

अटल अक्षर स्थिर *स्थिर sort_args[] =
{
#घोषणा _st_(_s, _c) _s,
  SORT_TABLE शून्य
#अघोषित  _st_
};
अटल अक्षर स्थिर sort_types[] =
{
#घोषणा _st_(_s, _c) _c,
  SORT_TABLE
#अघोषित  _st_
};

/* The set of signals that are caught.  */
अटल sigset_t caught_signals;

/* Critical section status.  */
काष्ठा cs_status
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
अटल व्योम
cs_enter (काष्ठा cs_status *status)
{
  पूर्णांक ret = pthread_sigmask (SIG_BLOCK, &caught_signals, &status->sigs);
  status->valid = ret == 0;
}

/* Leave a critical section.  */
अटल व्योम
cs_leave (काष्ठा cs_status स्थिर *status)
{
  अगर (status->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &status->sigs, शून्य);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
क्रमागत { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
काष्ठा sortfile
{
  /* The file's name.  */
  अक्षर स्थिर *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  काष्ठा tempnode *अस्थायी;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
अटल Hash_table *proctab;

क्रमागत { INIT_PROCTAB_आकार = 47 };

अटल माप_प्रकार
proctab_hasher (व्योम स्थिर *entry, माप_प्रकार tabsize)
{
  काष्ठा tempnode स्थिर *गाँठ = entry;
  वापस गाँठ->pid % tabsize;
}

अटल bool
proctab_comparator (व्योम स्थिर *e1, व्योम स्थिर *e2)
{
  काष्ठा tempnode स्थिर *n1 = e1;
  काष्ठा tempnode स्थिर *n2 = e2;
  वापस n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
अटल pid_t nprocs;

अटल bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

अटल pid_t
reap (pid_t pid)
{
  पूर्णांक status;
  pid_t cpid = waitpid ((pid ? pid : -1), &status, (pid ? 0 : WNOHANG));

  अगर (cpid < 0)
    error (SORT_FAILURE, त्रुटि_सं, _("waiting for %s [-d]"),
           quoteaf (compress_program));
  अन्यथा अगर (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      अगर (! WIFEXITED (status) || WEXITSTATUS (status))
        error (SORT_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_program));
      --nprocs;
    }

  वापस cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

अटल व्योम
register_proc (काष्ठा tempnode *अस्थायी)
{
  अगर (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_आकार, शून्य,
                                 proctab_hasher,
                                 proctab_comparator,
                                 शून्य);
      अगर (! proctab)
        xalloc_die ();
    }

  अस्थायी->state = UNREAPED;

  अगर (! hash_जोड़ो (proctab, अस्थायी))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

अटल bool
delete_proc (pid_t pid)
{
  काष्ठा tempnode test;

  test.pid = pid;
  काष्ठा tempnode *गाँठ = hash_remove (proctab, &test);
  अगर (! गाँठ)
    वापस false;
  गाँठ->state = REAPED;
  वापस true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

अटल व्योम
wait_proc (pid_t pid)
{
  अगर (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

अटल व्योम
reap_exited (व्योम)
{
  जबतक (0 < nprocs && reap (0))
    जारी;
}

/* Reap at least one exited child, waiting if necessary.  */

अटल व्योम
reap_some (व्योम)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

अटल व्योम
reap_all (व्योम)
{
  जबतक (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

अटल व्योम
exit_cleanup (व्योम)
{
  अगर (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      काष्ठा cs_status cs;
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

अटल काष्ठा tempnode *
create_अस्थायी_file (पूर्णांक *pfd, bool survive_fd_exhaustion)
{
  अटल अक्षर स्थिर slashbase[] = "/sortXXXXXX";
  अटल idx_t अस्थायी_dir_सूचकांक;
  पूर्णांक fd;
  पूर्णांक saved_errno;
  अक्षर स्थिर *अस्थायी_dir = अस्थायी_dirs.dirs[अस्थायी_dir_सूचकांक];
  माप_प्रकार len = म_माप (अस्थायी_dir);
  काष्ठा tempnode *गाँठ =
    xmalloc (FLEXSIZEOF (काष्ठा tempnode, name, len + माप slashbase));
  अक्षर *file = गाँठ->name;
  काष्ठा cs_status cs;

  स_नकल (file, अस्थायी_dir, len);
  स_नकल (file + len, slashbase, माप slashbase);
  गाँठ->अग्र = शून्य;
  अगर (++अस्थायी_dir_सूचकांक == अस्थायी_dirs.n_dirs)
    अस्थायी_dir_सूचकांक = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  अगर (0 <= fd)
    {
      *temptail = गाँठ;
      temptail = &गाँठ->अग्र;
    }
  saved_errno = त्रुटि_सं;
  cs_leave (&cs);
  त्रुटि_सं = saved_errno;

  अगर (fd < 0)
    {
      अगर (! (survive_fd_exhaustion && त्रुटि_सं == EMFILE))
        error (SORT_FAILURE, त्रुटि_सं, _("cannot create temporary file in %s"),
               quoteaf (अस्थायी_dir));
      मुक्त (गाँठ);
      गाँठ = शून्य;
    }

  *pfd = fd;
  वापस गाँठ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

अटल काष्ठा stat *
get_outstatus (व्योम)
{
  अटल पूर्णांक outstat_errno;
  अटल काष्ठा stat outstat;
  अगर (outstat_errno == 0)
    outstat_errno = ख_स्थिति (STDOUT_FILENO, &outstat) == 0 ? -1 : त्रुटि_सं;
  वापस outstat_errno < 0 ? &outstat : शून्य;
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

अटल खाता *
stream_open (अक्षर स्थिर *file, अक्षर स्थिर *how)
{
  खाता *fp;

  अगर (*how == 'r')
    {
      अगर (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = मानक_निवेश;
        }
      अन्यथा
        {
          पूर्णांक fd = खोलो (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? शून्य : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  अन्यथा अगर (*how == 'w')
    {
      अगर (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          पूर्णांक ftruncate_errno = त्रुटि_सं;
          काष्ठा stat *outst = get_outstatus ();
          अगर (!outst || S_ISREG (outst->st_mode) || S_TYPEISSHM (outst))
            error (SORT_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = मानक_निकास;
    }
  अन्यथा
    affirm (!"unexpected mode passed to stream_open");

  वापस fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

अटल खाता *
xfopen (अक्षर स्थिर *file, अक्षर स्थिर *how)
{
  खाता *fp = stream_open (file, how);
  अगर (!fp)
    sort_die (_("open failed"), file);
  वापस fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

अटल व्योम
xfclose (खाता *fp, अक्षर स्थिर *file)
{
  चयन (fileno (fp))
    {
    हाल STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      साफ_त्रुटि (fp);
      अवरोध;

    हाल STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      अगर (ख_साफ (fp) != 0)
        sort_die (_("fflush failed"), file);
      अवरोध;

    शेष:
      अगर (ख_बंद (fp) != 0)
        sort_die (_("close failed"), file);
      अवरोध;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

अटल व्योम
move_fd (पूर्णांक oldfd, पूर्णांक newfd)
{
  अगर (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_मूल्य (dup2 (oldfd, newfd));
      ignore_मूल्य (बंद (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

अटल पूर्णांक
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  पूर्णांक oldfd, पूर्णांक newfd)
{
  पूर्णांक परिणाम = 0;
  अगर (oldfd != newfd)
    {
      परिणाम = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      अगर (परिणाम == 0)
        परिणाम = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  वापस परिणाम;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

अटल अक्षर स्थिर *
get_resolved_compress_program (व्योम)
{
  /* Use a cache, to perform the search only once.  */
  अटल अक्षर स्थिर *resolved_compress_program_cache /* = NULL */;

  अगर (resolved_compress_program_cache == शून्य)
    {
      resolved_compress_program_cache =
        find_in_given_path (compress_program, दो_पर्या ("PATH"), शून्य, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  वापस resolved_compress_program_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

अटल पूर्णांक
pipe_child (pid_t *pid, पूर्णांक pipefds[2], पूर्णांक tempfd, bool decompress,
            माप_प्रकार tries)
{
  अक्षर स्थिर *resolved_compress_program;
  काष्ठा tempnode *saved_temphead;
  द्विगुन wait_retry = 0.25;
  काष्ठा cs_status cs;
  पूर्णांक परिणाम;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_program = get_resolved_compress_program ();
  अगर (resolved_compress_program == शून्य)
    वापस त्रुटि_सं;

  अगर ((परिणाम = posix_spawnattr_init (&attr)))
    वापस परिणाम;
  अगर ((परिणाम = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (परिणाम = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      वापस परिणाम;
    }

  अगर (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      वापस saved_errno;
    }

  अगर ((परिणाम = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (परिणाम = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((परिणाम = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (परिणाम = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (परिणाम = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((परिणाम = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (परिणाम = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (परिणाम = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      बंद (pipefds[0]);
      बंद (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      वापस परिणाम;
    }

  अक्षर स्थिर *स्थिर argv[] =
    {
      resolved_compress_program,
      decompress ? "-d" : शून्य,
      शून्य
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  अगर (nmerge + 1 < nprocs)
    reap_some ();

  जबतक (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = शून्य;

      परिणाम = posix_spawnp (pid, resolved_compress_program, &actions, &attr,
                             (अक्षर * स्थिर *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      अगर (परिणाम != EAGAIN)
        अवरोध;
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

  अगर (परिणाम)
    {
      बंद (pipefds[0]);
      बंद (pipefds[1]);
    }
  अन्यथा
    ++nprocs;

  वापस परिणाम;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

अटल काष्ठा tempnode *
maybe_create_अस्थायी (खाता **pfp, bool survive_fd_exhaustion)
{
  पूर्णांक tempfd;
  काष्ठा tempnode *गाँठ = create_अस्थायी_file (&tempfd, survive_fd_exhaustion);
  अगर (! गाँठ)
    वापस शून्य;

  गाँठ->state = UNCOMPRESSED;

  अगर (compress_program)
    {
      पूर्णांक pipefds[2];
      अटल पूर्णांक last_परिणाम = 0;

      पूर्णांक परिणाम = pipe_child (&गाँठ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      अगर (परिणाम)
        {
          अगर (परिणाम != last_परिणाम)
            error (0, परिणाम, _("could not run compress program %s"),
                   quoteaf (compress_program));
        }
      अन्यथा
        {
          बंद (tempfd);
          बंद (pipefds[0]);
          tempfd = pipefds[1];

          register_proc (गाँठ);
        }

      last_परिणाम = परिणाम;
    }

  *pfp = fdopen (tempfd, "w");
  अगर (! *pfp)
    sort_die (_("couldn't create temporary file"), गाँठ->name);

  वापस गाँठ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

अटल काष्ठा tempnode *
create_अस्थायी (खाता **pfp)
{
  वापस maybe_create_अस्थायी (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

अटल खाता *
open_अस्थायी (काष्ठा tempnode *अस्थायी)
{
  पूर्णांक tempfd, pipefds[2];
  खाता *fp = शून्य;

  अगर (अस्थायी->state == UNREAPED)
    wait_proc (अस्थायी->pid);

  tempfd = खोलो (अस्थायी->name, O_RDONLY);
  अगर (tempfd < 0)
    वापस शून्य;

  pid_t child;
  पूर्णांक परिणाम = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  अगर (परिणाम)
    {
      अगर (परिणाम != EMFILE)
        error (SORT_FAILURE, परिणाम, _("could not run compress program %s -d"),
               quoteaf (compress_program));
      बंद (tempfd);
      त्रुटि_सं = EMFILE;
    }
  अन्यथा
    {
      अस्थायी->pid = child;
      register_proc (अस्थायी);
      बंद (tempfd);
      बंद (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      अगर (! fp)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          बंद (pipefds[0]);
          त्रुटि_सं = saved_errno;
        }
    }

  वापस fp;
}

/* Append DIR to the array of temporary directory names.  */
अटल व्योम
add_अस्थायी_dir (अक्षर स्थिर *dir)
{
  अगर (अस्थायी_dirs.n_dirs == अस्थायी_dirs.n_dirs_alloc)
    अस्थायी_dirs.dirs = xpalloc (अस्थायी_dirs.dirs, &अस्थायी_dirs.n_dirs_alloc, 1, -1,
                              माप *अस्थायी_dirs.dirs);

  idx_t स्थिर dir_idx = अस्थायी_dirs.n_dirs++;
  अस्थायी_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

अटल व्योम
zaptemp (अक्षर स्थिर *name)
{
  काष्ठा tempnode *अस्थिर *pnode;
  काष्ठा tempnode *गाँठ;
  काष्ठा tempnode *अग्र;
  पूर्णांक unlink_status;
  पूर्णांक unlink_errno = 0;
  काष्ठा cs_status cs;

  क्रम (pnode = &temphead; (गाँठ = *pnode)->name != name; pnode = &गाँठ->अग्र)
    जारी;

  अगर (गाँठ->state == UNREAPED)
    wait_proc (गाँठ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  अग्र = गाँठ->अग्र;
  cs_enter (&cs);
  unlink_status = unlink (name);
  unlink_errno = त्रुटि_सं;
  *pnode = अग्र;
  cs_leave (&cs);

  अगर (unlink_status != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  अगर (! अग्र)
    temptail = pnode;
  मुक्त (गाँठ);
}

#अगर HAVE_NL_LANGINFO

अटल पूर्णांक
struct_month_cmp (व्योम स्थिर *m1, व्योम स्थिर *m2)
{
  काष्ठा month स्थिर *month1 = m1;
  काष्ठा month स्थिर *month2 = m2;
  वापस म_भेद (month1->name, month2->name);
}

#पूर्ण_अगर

/* Initialize the character class tables. */

अटल व्योम
inittables (व्योम)
{
  क्रम (माप_प्रकार i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! है_अक्षर_अंक (i);
      nonprinting[i] = ! है_छाप (i);
      fold_toupper[i] = बड़े (i);
    }

#अगर HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  अगर (hard_LC_TIME)
    {
      क्रम (माप_प्रकार i = 0; i < MONTHS_PER_YEAR; i++)
        {
          अक्षर स्थिर *s;
          माप_प्रकार s_len;
          माप_प्रकार j, k;
          अक्षर *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = म_माप (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          क्रम (j = k = 0; j < s_len; j++)
            अगर (! isblank (to_uchar (s[j])))
              name[k++] = fold_toupper[to_uchar (s[j])];
          name[k] = '\0';
        }
      क्विक (monthtab, MONTHS_PER_YEAR, माप *monthtab, struct_month_cmp);
    }
#पूर्ण_अगर
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
अटल व्योम
specify_nmerge (पूर्णांक oi, अक्षर c, अक्षर स्थिर *s)
{
  uintmax_t n;
  काष्ठा rlimit rlimit;
  क्रमागत strtol_error e = xstrtoumax (s, शून्य, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  अचिन्हित पूर्णांक max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  अगर (e == LONGINT_OK)
    {
      nmerge = n;
      अगर (nmerge != n)
        e = LONGINT_OVERFLOW;
      अन्यथा
        {
          अगर (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (SORT_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          अन्यथा अगर (max_nmerge < nmerge)
            {
              e = LONGINT_OVERFLOW;
            }
          अन्यथा
            वापस;
        }
    }

  अगर (e == LONGINT_OVERFLOW)
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
अटल व्योम
specify_sort_आकार (पूर्णांक oi, अक्षर c, अक्षर स्थिर *s)
{
  uintmax_t n;
  अक्षर *suffix;
  क्रमागत strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  अगर (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_OVERFLOW;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  अगर (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    चयन (suffix[0])
      {
      हाल 'b':
        e = LONGINT_OK;
        अवरोध;

      हाल '%':
        {
          द्विगुन mem = physmem_योग () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          अगर (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          अन्यथा
            e = LONGINT_OVERFLOW;
        }
        अवरोध;
      }

  अगर (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      अगर (n < sort_आकार)
        वापस;

      sort_आकार = n;
      अगर (sort_आकार == n)
        {
          sort_आकार = MAX (sort_आकार, MIN_SORT_आकार);
          वापस;
        }

      e = LONGINT_OVERFLOW;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
अटल माप_प्रकार
specify_nthreads (पूर्णांक oi, अक्षर c, अक्षर स्थिर *s)
{
  uintmax_t nthreads;
  क्रमागत strtol_error e = xstrtoumax (s, शून्य, 10, &nthreads, "");
  अगर (e == LONGINT_OVERFLOW)
    वापस SIZE_MAX;
  अगर (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  अगर (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  अगर (nthreads == 0)
    error (SORT_FAILURE, 0, _("number in parallel must be nonzero"));
  वापस nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
अटल माप_प्रकार
default_sort_आकार (व्योम)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  माप_प्रकार आकार = SIZE_MAX;
  काष्ठा rlimit rlimit;
  अगर (getrlimit (RLIMIT_तथ्य, &rlimit) == 0 && rlimit.rlim_cur < आकार)
    आकार = rlimit.rlim_cur;
#अगर_घोषित RLIMIT_AS
  अगर (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < आकार)
    आकार = rlimit.rlim_cur;
#पूर्ण_अगर

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  आकार /= 2;

#अगर_घोषित RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  अगर (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < आकार)
    आकार = rlimit.rlim_cur / 16 * 15;
#पूर्ण_अगर

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  द्विगुन avail = physmem_available ();
  द्विगुन योग_2 = physmem_योग ();
  द्विगुन mem = MAX (avail, योग_2 / 8);

  /* Leave a 1/4 margin for physical memory.  */
  अगर (योग_2 * 0.75 < आकार)
    आकार = योग_2 * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  अगर (mem < आकार)
    आकार = mem;
  वापस MAX (आकार, MIN_SORT_आकार);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

अटल bool
पंक्ति_aligned_आकार (माप_प्रकार *alloc)
{
  माप_प्रकार आकार = *alloc;
  माप_प्रकार alignment = माप (काष्ठा पंक्ति);
  माप_प्रकार padding = alignment - आकार % alignment;
  माप_प्रकार aligned;

  अगर (ckd_add (&aligned, आकार, padding))
    वापस false;

  *alloc = aligned;
  वापस true;
}

/* Return the default initial allocation for a growable sort buffer.  */

अटल माप_प्रकार
default_initial_sort_आकार (माप_प्रकार पंक्ति_bytes)
{
  माप_प्रकार आकार = पंक्ति_bytes + 2;
  माप_प्रकार निवेश_आकार;

  अगर (ckd_mul (&निवेश_आकार, निवेश_FILE_आकार_GUESS, पंक्ति_bytes + 1)
      || ckd_add (&आकार, आकार, निवेश_आकार))
    वापस SIZE_MAX;

  वापस MAX (आकार, MIN_SORT_आकार);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

अटल bool
निवेश_आकार_बफर_bytes (uintmax_t file_आकार, माप_प्रकार पंक्ति_bytes,
                         माप_प्रकार *worst_case)
{
  माप_प्रकार worst_case_per_निवेश_byte = पंक्ति_bytes + 1;

  अगर (SIZE_MAX / worst_case_per_निवेश_byte < file_आकार)
    वापस false;

  माप_प्रकार आकार = file_आकार * worst_case_per_निवेश_byte;
  अगर (SIZE_MAX - आकार < 1)
    वापस false;

  *worst_case = आकार + 1;
  वापस true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

अटल व्योम
sort_बफर_policy (खाता *स्थिर *fps, माप_प्रकार nfps,
                    अक्षर *स्थिर *files, माप_प्रकार nfiles,
                    माप_प्रकार पंक्ति_bytes,
                    काष्ठा sort_बफर_policy *policy)
{
  माप_प्रकार आकार_bound = sort_आकार ? sort_आकार : default_sort_आकार ();
  माप_प्रकार initial_bound =
    sort_आकार ? आकार_bound : default_initial_sort_आकार (पंक्ति_bytes);

  /* In the worst case, each input byte is a newline.  */
  माप_प्रकार worst_case_per_निवेश_byte = पंक्ति_bytes + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  माप_प्रकार आकार = worst_case_per_निवेश_byte + 1;

  initial_bound = MIN (initial_bound, आकार_bound);
  policy->limit = आकार_bound;
  policy->growth_failed = false;

  क्रम (माप_प्रकार i = 0; i < nfiles; i++)
    {
      काष्ठा stat st;
      off_t file_आकार;
      माप_प्रकार worst_case;
      bool known_आकार;

      अगर (आकार_bound <= आकार)
        {
          policy->initial = आकार_bound;
          वापस;
        }

      अगर ((i < nfps ? ख_स्थिति (fileno (fps[i]), &st)
           : streq (files[i], "-") ? ख_स्थिति (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        sort_die (_("stat failed"), files[i]);

      known_आकार = usable_st_आकार (&st) && 0 < st.st_आकार;
      अगर (known_आकार)
        file_आकार = st.st_आकार;
      अन्यथा
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          अगर (sort_आकार)
            {
              policy->initial = आकार_bound;
              वापस;
            }
          file_आकार = निवेश_FILE_आकार_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      अगर (! निवेश_आकार_बफर_bytes (file_आकार, पंक्ति_bytes, &worst_case)
          || आकार_bound - आकार <= worst_case)
        {
          policy->initial = आकार_bound;
          वापस;
        }

      अगर (! known_आकार && ! sort_आकार
          && (initial_bound <= आकार || initial_bound - आकार <= worst_case))
        {
          policy->initial = MAX (आकार, initial_bound);
          वापस;
        }

      आकार += worst_case;
    }

  policy->initial = MAX (आकार, MIN_SORT_आकार);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

अटल व्योम
initbuf (काष्ठा बफर *buf, माप_प्रकार पंक्ति_bytes, माप_प्रकार alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  जबतक (true)
    {
      अगर (! पंक्ति_aligned_आकार (&alloc))
        buf->buf = शून्य;
      अन्यथा
        buf->buf = दो_स्मृति (alloc);
      अगर (buf->buf)
        अवरोध;
      alloc /= 2;
      अगर (alloc <= पंक्ति_bytes + 1)
        xalloc_die ();
    }

  buf->पंक्ति_bytes = पंक्ति_bytes;
  buf->alloc = alloc;
  buf->used = buf->बायाँ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

अटल अंतरभूत काष्ठा पंक्ति *
बफर_linelim (काष्ठा बफर स्थिर *buf)
{
  व्योम *linelim = buf->buf + buf->alloc;
  वापस linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

अटल bool
try_growbuf (काष्ठा बफर *buf, माप_प्रकार alloc)
{
  अगर (! पंक्ति_aligned_आकार (&alloc) || alloc <= buf->alloc)
    वापस false;

  अक्षर *newbuf = दो_स्मृति (alloc);
  अगर (! newbuf)
    वापस false;

  अक्षर *oldbuf = buf->buf;
  काष्ठा पंक्ति *old_linelim = बफर_linelim (buf);
  काष्ठा पंक्ति *old_पंक्ति = old_linelim - buf->nlines;

  स_नकल (newbuf, oldbuf, buf->used);

  काष्ठा पंक्ति *new_linelim = (व्योम *) (newbuf + alloc);
  काष्ठा पंक्ति *new_पंक्ति = new_linelim - buf->nlines;
  स_नकल (new_पंक्ति, old_पंक्ति, buf->nlines * माप *new_पंक्ति);

  क्रम (काष्ठा पंक्ति *पंक्ति = new_पंक्ति; पंक्ति < new_linelim; पंक्ति++)
    {
      पंक्ति->text = newbuf + (पंक्ति->text - oldbuf);
      अगर (keylist)
        {
          पंक्ति->keybeg = newbuf + (पंक्ति->keybeg - oldbuf);
          पंक्ति->keylim = newbuf + (पंक्ति->keylim - oldbuf);
        }
    }

  मुक्त (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  वापस true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

अटल bool
maybe_growbuf (काष्ठा बफर *buf, काष्ठा sort_बफर_policy *policy)
{
  अगर (! policy || policy->growth_failed || policy->limit <= buf->alloc)
    वापस false;

  माप_प्रकार alloc;
  अगर (buf->alloc <= policy->limit / 3)
    alloc = buf->alloc * 3;
  अन्यथा
    alloc = policy->limit;

  अगर (try_growbuf (buf, alloc))
    वापस true;

  policy->growth_failed = true;
  वापस false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

अटल अक्षर *
begfield (काष्ठा पंक्ति स्थिर *पंक्ति, काष्ठा keyfield स्थिर *कुंजी)
{
  अक्षर *ptr = पंक्ति->text, *lim = ptr + पंक्ति->लंबाई - 1;
  माप_प्रकार sword = कुंजी->sword;
  माप_प्रकार schar = कुंजी->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  अगर (tab != TAB_DEFAULT)
    जबतक (ptr < lim && sword--)
      {
        अक्षर *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        अगर (ptr < lim)
          ++ptr;
      }
  अन्यथा
    जबतक (ptr < lim && sword--)
      {
        जबतक (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        जबतक (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  अगर (कुंजी->skipsblanks)
    जबतक (ptr < lim && blanks[to_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  माप_प्रकार remaining_bytes = lim - ptr;
  अगर (schar < remaining_bytes)
    ptr += schar;
  अन्यथा
    ptr = lim;

  वापस ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_PURE
अटल अक्षर *
limfield (काष्ठा पंक्ति स्थिर *पंक्ति, काष्ठा keyfield स्थिर *कुंजी)
{
  अक्षर *ptr = पंक्ति->text, *lim = ptr + पंक्ति->लंबाई - 1;
  माप_प्रकार eword = कुंजी->eword, echar = कुंजी->echar;

  अगर (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  अगर (tab != TAB_DEFAULT)
    जबतक (ptr < lim && eword--)
      {
        अक्षर *sep = स_प्रथम (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        अगर (ptr < lim && (eword || echar))
          ++ptr;
      }
  अन्यथा
    जबतक (ptr < lim && eword--)
      {
        जबतक (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;
        जबतक (ptr < lim && !blanks[to_uchar (*ptr)])
          ++ptr;
      }

#अगर_घोषित POSIX_UNSPECIFIED
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
  अगर (tab != TAB_DEFAULT)
    {
      अक्षर *newlim;
      newlim = स_प्रथम (ptr, tab, lim - ptr);
      अगर (newlim)
        lim = newlim;
    }
  अन्यथा
    {
      अक्षर *newlim;
      newlim = ptr;
      जबतक (newlim < lim && blanks[to_uchar (*newlim)])
        ++newlim;
      जबतक (newlim < lim && !blanks[to_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#पूर्ण_अगर

  अगर (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      अगर (कुंजी->skipeblanks)
        जबतक (ptr < lim && blanks[to_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      माप_प्रकार remaining_bytes = lim - ptr;
      अगर (echar < remaining_bytes)
        ptr += echar;
      अन्यथा
        ptr = lim;
    }

  वापस ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

अटल bool
fillbuf (काष्ठा बफर *buf, खाता *fp, अक्षर स्थिर *file,
         काष्ठा sort_बफर_policy *policy)
{
  काष्ठा keyfield स्थिर *कुंजी = keylist;
  अक्षर eol = eolchar;
  माप_प्रकार पंक्ति_bytes = buf->पंक्ति_bytes;
  माप_प्रकार mergesize = merge_बफर_आकार - MIN_MERGE_बफर_आकार;

  अगर (buf->eof)
    वापस false;

  अगर (buf->used != buf->बायाँ)
    {
      स_हटाओ (buf->buf, buf->buf + buf->used - buf->बायाँ, buf->बायाँ);
      buf->used = buf->बायाँ;
      buf->nlines = 0;
    }

  जबतक (true)
    {
      अक्षर *ptr = buf->buf + buf->used;
      काष्ठा पंक्ति *linelim = बफर_linelim (buf);
      काष्ठा पंक्ति *पंक्ति = linelim - buf->nlines;
      माप_प्रकार avail = (अक्षर *) linelim - buf->nlines * पंक्ति_bytes - ptr;
      अक्षर *पंक्ति_start = buf->nlines ? पंक्ति->text + पंक्ति->लंबाई : buf->buf;

      जबतक (पंक्ति_bytes + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          माप_प्रकार readsize = (avail - 1) / (पंक्ति_bytes + 1);
          माप_प्रकार bytes_read = ख_पढ़ो (ptr, 1, readsize, fp);
          अक्षर *ptrlim = ptr + bytes_read;
          अक्षर *p;
          avail -= bytes_read;

          अगर (bytes_read != readsize)
            {
              अगर (ख_त्रुटि (fp))
                sort_die (_("read failed"), file);
              अगर (ख_पूर्ण (fp))
                {
                  buf->eof = true;
                  अगर (buf->buf == ptrlim)
                    वापस false;
                  अगर (पंक्ति_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          जबतक ((p = स_प्रथम (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              पंक्ति--;
              पंक्ति->text = पंक्ति_start;
              पंक्ति->लंबाई = ptr - पंक्ति_start;
              mergesize = MAX (mergesize, पंक्ति->लंबाई);
              avail -= पंक्ति_bytes;

              अगर (कुंजी)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  पंक्ति->keylim = (कुंजी->eword == SIZE_MAX
                                  ? p
                                  : limfield (पंक्ति, कुंजी));

                  अगर (कुंजी->sword != SIZE_MAX)
                    पंक्ति->keybeg = begfield (पंक्ति, कुंजी);
                  अन्यथा
                    {
                      अगर (कुंजी->skipsblanks)
                        जबतक (blanks[to_uchar (*पंक्ति_start)])
                          पंक्ति_start++;
                      पंक्ति->keybeg = पंक्ति_start;
                    }
                }

              पंक्ति_start = ptr;
            }

          ptr = ptrlim;
          अगर (buf->eof)
            अवरोध;
        }

      buf->used = ptr - buf->buf;
      buf->nlines = बफर_linelim (buf) - पंक्ति;
      अगर (buf->nlines != 0)
        {
          अगर (! buf->eof && maybe_growbuf (buf, policy))
            जारी;
          buf->बायाँ = ptr - पंक्ति_start;
          merge_बफर_आकार = mergesize + MIN_MERGE_बफर_आकार;
          वापस true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t पंक्ति_alloc = buf->alloc / माप (काष्ठा पंक्ति);
        buf->buf = xpalloc (buf->buf, &पंक्ति_alloc, 1, -1, माप (काष्ठा पंक्ति));
        buf->alloc = पंक्ति_alloc * माप (काष्ठा पंक्ति);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
अटल अक्षर स्थिर unit_order[UCHAR_LIM] =
  {
#अगर ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
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
#पूर्ण_अगर
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
अटल अक्षर
traverse_raw_संख्या (अक्षर स्थिर **संख्या)
{
  अक्षर स्थिर *p = *संख्या;
  अक्षर ch;
  अक्षर max_digit = '\0';
  bool ends_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  जबतक (c_isdigit (ch = *p++))
    {
      अगर (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ends_with_thousands_sep = (*p == thousands_sep);
      अगर (ends_with_thousands_sep)
        ++p;
    }

  अगर (ends_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *संख्या = p - 2;
      वापस max_digit;
    }

  अगर (ch == decimal_point)
    जबतक (c_isdigit (ch = *p++))
      अगर (max_digit < ch)
        max_digit = ch;

  *संख्या = p - 1;
  वापस max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_PURE
अटल पूर्णांक
find_unit_order (अक्षर स्थिर *संख्या)
{
  bool minus_sign = (*संख्या == '-');
  अक्षर स्थिर *p = संख्या + minus_sign;
  अक्षर max_digit = traverse_raw_संख्या (&p);
  अगर ('0' < max_digit)
    {
      अचिन्हित अक्षर ch = *p;
      पूर्णांक order = unit_order[ch];
      वापस (minus_sign ? -order : order);
    }
  अन्यथा
    वापस 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_PURE
अटल पूर्णांक
human_numcompare (अक्षर स्थिर *a, अक्षर स्थिर *b)
{
  जबतक (blanks[to_uchar (*a)])
    a++;
  जबतक (blanks[to_uchar (*b)])
    b++;

  पूर्णांक diff = find_unit_order (a) - find_unit_order (b);
  वापस (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_PURE
अटल पूर्णांक
numcompare (अक्षर स्थिर *a, अक्षर स्थिर *b)
{
  जबतक (blanks[to_uchar (*a)])
    a++;
  जबतक (blanks[to_uchar (*b)])
    b++;

  वापस strnumcmp (a, b, decimal_point, thousands_sep);
}

अटल पूर्णांक
nan_compare (दीर्घ द्विगुन a, दीर्घ द्विगुन b)
{
  अक्षर buf[2][माप "-nan""()" + अक्षर_बिट * माप a];
  snprintf (buf[0], माप buf[0], "%Lf", a);
  snprintf (buf[1], माप buf[1], "%Lf", b);
  वापस म_भेद (buf[0], buf[1]);
}

अटल पूर्णांक
general_numcompare (अक्षर स्थिर *sa, अक्षर स्थिर *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  अक्षर *ea;
  अक्षर *eb;
  दीर्घ द्विगुन a = strtold (sa, &ea);
  दीर्घ द्विगुन b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  अगर (sa == ea)
    वापस sb == eb ? 0 : -1;
  अगर (sb == eb)
    वापस 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  वापस (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

अटल पूर्णांक
getmonth (अक्षर स्थिर *month, अक्षर **ea)
{
  माप_प्रकार lo = 0;
  माप_प्रकार hi = MONTHS_PER_YEAR;

  जबतक (blanks[to_uchar (*month)])
    month++;

  करो
    {
      माप_प्रकार ix = (lo + hi) / 2;
      अक्षर स्थिर *m = month;
      अक्षर स्थिर *n = monthtab[ix].name;

      क्रम (;; m++, n++)
        {
          अगर (!*n)
            {
              अगर (ea)
                *ea = (अक्षर *) m;
              वापस monthtab[ix].val;
            }
          अगर (to_uchar (fold_toupper[to_uchar (*m)]) < to_uchar (*n))
            {
              hi = ix;
              अवरोध;
            }
          अन्यथा अगर (to_uchar (fold_toupper[to_uchar (*m)]) > to_uchar (*n))
            {
              lo = ix + 1;
              अवरोध;
            }
        }
    }
  जबतक (lo < hi);

  वापस 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#अगर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# अगर 14 <= __GNUC__
#  pragma GCC diagnostic धकेलो
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#पूर्ण_अगर

#समावेश "md5.h"

#अगर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# अगर 14 <= __GNUC__
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
अटल व्योम
link_failure (व्योम)
{
  error (SORT_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
अटल व्योम *
प्रतीक_address (व्योम *handle, अक्षर स्थिर *प्रतीक)
{
  व्योम *address = dlsym (handle, प्रतीक);
  अगर (!address)
    link_failure ();
  वापस address;
}
#पूर्ण_अगर

/* Dynamically link the crypto library, if it needs linking.  */
अटल व्योम
link_libcrypto (व्योम)
{
#अगर DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  व्योम *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  अगर (!handle)
    link_failure ();
  ptr_MD5_Init = प्रतीक_address (handle, "MD5_Init");
  ptr_MD5_Update = प्रतीक_address (handle, "MD5_Update");
  ptr_MD5_Final = प्रतीक_address (handle, "MD5_Final");
#पूर्ण_अगर
}

/* A randomly chosen MD5 state, used for random comparison.  */
अटल काष्ठा md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

अटल व्योम
random_md5_state_init (अक्षर स्थिर *random_source)
{
  अचिन्हित अक्षर buf[MD5_DIGEST_आकार];
  काष्ठा randread_source *r = randread_new (random_source, माप buf);
  अगर (! r)
    sort_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, माप buf);
  अगर (randread_free (r) != 0)
    sort_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_bytes (buf, माप buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

अटल माप_प्रकार
xstrxfrm (अक्षर *restrict dest, अक्षर स्थिर *restrict src, माप_प्रकार destsize)
{
  त्रुटि_सं = 0;
  माप_प्रकार translated_आकार = strxfrm (dest, src, destsize);

  अगर (त्रुटि_सं)
    {
      error (0, त्रुटि_सं, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (SORT_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  वापस translated_आकार;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

अटल पूर्णांक
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
  माप_प्रकार bufsize = माप stackbuf;
  व्योम *allocated = शून्य;
  uint32_t dig[2][MD5_DIGEST_आकार / माप (uint32_t)];
  काष्ठा md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  अगर (hard_LC_COLLATE)
    {
      अक्षर स्थिर *lima = texta + lena;
      अक्षर स्थिर *limb = textb + lenb;

      जबतक (true)
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
          अगर (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              मुक्त (allocated);
              buf = allocated = दो_स्मृति (bufsize);
              अगर (! buf)
                {
                  buf = stackbuf;
                  bufsize = माप stackbuf;
                }
            }

          माप_प्रकार sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_fits = sizea <= bufsize;
          माप_प्रकार sizeb =
            (textb < limb
             ? (xstrxfrm ((a_fits ? buf + sizea : शून्य), textb,
                          (a_fits ? bufsize - sizea : 0))
                + 1)
             : 0);

          अगर (! (a_fits && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              अगर (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              मुक्त (allocated);
              buf = allocated = xmalloc (bufsize);
              अगर (texta < lima)
                strxfrm (buf, texta, sizea);
              अगर (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          अगर (texta < lima)
            texta += म_माप (texta) + 1;
          अगर (textb < limb)
            textb += म_माप (textb) + 1;
          अगर (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              अवरोध;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_bytes (buf, sizea, &s[0]);
          md5_process_bytes (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          अगर (! xfrm_diff)
            {
              xfrm_diff = स_भेद (buf, buf + sizea, MIN (sizea, sizeb));
              अगर (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_bytes (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_bytes (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  पूर्णांक diff = स_भेद (dig[0], dig[1], माप dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  अगर (! diff)
    {
      अगर (! xfrm_diff)
        {
          xfrm_diff = स_भेद (texta, textb, MIN (lena, lenb));
          अगर (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  मुक्त (allocated);

  वापस diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

अटल माप_प्रकार
debug_width (अक्षर स्थिर *text, अक्षर स्थिर *lim)
{
  माप_प्रकार width = mbsnwidth (text, lim - text, 0);
  जबतक (text < lim)
    width += (*text++ == '\t');
  वापस width;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

अटल व्योम
mark_कुंजी (माप_प्रकार offset, माप_प्रकार width)
{
  जबतक (offset--)
    अक्षर_दो (' ');

  अगर (!width)
    म_लिखो (_("^ no match for key\n"));
  अन्यथा
    {
      करो
        अक्षर_दो ('_');
      जबतक (--width);

      अक्षर_दो ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

अटल अंतरभूत bool
कुंजी_numeric (काष्ठा keyfield स्थिर *कुंजी)
{
  वापस कुंजी->numeric || कुंजी->general_numeric || कुंजी->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

अटल व्योम
debug_कुंजी (काष्ठा पंक्ति स्थिर *पंक्ति, काष्ठा keyfield स्थिर *कुंजी)
{
  अक्षर *text = पंक्ति->text;
  अक्षर *beg = text;
  अक्षर *lim = text + पंक्ति->लंबाई - 1;

  अगर (कुंजी)
    {
      अगर (कुंजी->sword != SIZE_MAX)
        beg = begfield (पंक्ति, कुंजी);
      अगर (कुंजी->eword != SIZE_MAX)
        {
          lim = limfield (पंक्ति, कुंजी);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      अगर ((कुंजी->skipsblanks && कुंजी->sword == SIZE_MAX)
          || कुंजी->month || कुंजी_numeric (कुंजी))
        {
          अक्षर saved = *lim;
          *lim = '\0';

          जबतक (blanks[to_uchar (*beg)])
            beg++;

          अक्षर *tighter_lim = beg;

          अगर (lim < beg)
            tighter_lim = lim;
          अन्यथा अगर (कुंजी->month)
            getmonth (beg, &tighter_lim);
          अन्यथा अगर (कुंजी->general_numeric)
            ignore_मूल्य (strtold (beg, &tighter_lim));
          अन्यथा अगर (कुंजी->numeric || कुंजी->human_numeric)
            {
              अक्षर स्थिर *p = beg + (beg < lim && *beg == '-');
              अक्षर max_digit = traverse_raw_संख्या (&p);
              अगर ('0' <= max_digit)
                {
                  अचिन्हित अक्षर ch = *p;
                  tighter_lim = (अक्षर *) p
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

अटल व्योम
debug_पंक्ति (काष्ठा पंक्ति स्थिर *पंक्ति)
{
  काष्ठा keyfield स्थिर *कुंजी = keylist;

  करो
    debug_कुंजी (पंक्ति, कुंजी);
  जबतक (कुंजी && ((कुंजी = कुंजी->अग्र) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

अटल bool
default_कुंजी_compare (काष्ठा keyfield स्थिर *कुंजी)
{
  वापस ! (कुंजी->ignore
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

अटल व्योम
कुंजी_to_opts (काष्ठा keyfield स्थिर *कुंजी, अक्षर *opts)
{
  अगर (कुंजी->skipsblanks || कुंजी->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  अगर (कुंजी->ignore == nondictionary)
    *opts++ = 'd';
  अगर (कुंजी->translate)
    *opts++ = 'f';
  अगर (कुंजी->general_numeric)
    *opts++ = 'g';
  अगर (कुंजी->human_numeric)
    *opts++ = 'h';
  अगर (कुंजी->ignore == nonprinting)
    *opts++ = 'i';
  अगर (कुंजी->month)
    *opts++ = 'M';
  अगर (कुंजी->numeric)
    *opts++ = 'n';
  अगर (कुंजी->random)
    *opts++ = 'R';
  अगर (कुंजी->reverse)
    *opts++ = 'r';
  अगर (कुंजी->version)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

अटल व्योम
कुंजी_warnings (काष्ठा keyfield स्थिर *gkey, bool gkey_only)
{
  काष्ठा keyfield ugkey = *gkey;
  अचिन्हित दीर्घ keynum = 1;
  bool basic_numeric_field = false;
  bool general_numeric_field = false;
  bool basic_numeric_field_span = false;
  bool general_numeric_field_span = false;

  क्रम (काष्ठा keyfield *कुंजी = keylist; कुंजी; कुंजी = कुंजी->अग्र, keynum++)
    {
      अगर (कुंजी_numeric (कुंजी))
        {
          अगर (कुंजी->general_numeric)
            general_numeric_field = true;
          अन्यथा
            basic_numeric_field = true;
        }

      अगर (कुंजी->traditional_used)
        {
          माप_प्रकार sword = कुंजी->sword;
          माप_प्रकार eword = कुंजी->eword;
          अक्षर tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          अक्षर obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          अक्षर nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          अक्षर *po = obuf;
          अक्षर *pn = nbuf;

          अगर (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          अगर (कुंजी->eword != SIZE_MAX)
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
      अगर (zero_width)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = कुंजी_numeric (कुंजी) || कुंजी->month;
      bool पंक्ति_offset = कुंजी->eword == 0 && कुंजी->echar != 0; /* -k1.x,1.y  */
      अगर (!zero_width && !gkey_only && tab == TAB_DEFAULT && !पंक्ति_offset
          && ((!कुंजी->skipsblanks && !implicit_skip)
              || (!कुंजी->skipsblanks && कुंजी->schar)
              || (!कुंजी->skipeblanks && कुंजी->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      अगर (!gkey_only && कुंजी_numeric (कुंजी))
        {
          माप_प्रकार sword = कुंजी->sword + 1;
          माप_प्रकार eword = कुंजी->eword + 1;
          अगर (!sword)
            sword++;
          अगर (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              अगर (कुंजी->general_numeric)
                general_numeric_field_span = true;
              अन्यथा
                basic_numeric_field_span = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      अगर (ugkey.ignore && (ugkey.ignore == कुंजी->ignore))
        ugkey.ignore = शून्य;
      अगर (ugkey.translate && (ugkey.translate == कुंजी->translate))
        ugkey.translate = शून्य;
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
  अगर (basic_numeric_field_span)
    {
      अगर (tab == TAB_DEFAULT
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
  अगर (basic_numeric_field_span || general_numeric_field_span)
    {
      अगर (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (to_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((अक्षर []) {decimal_point, 0})));
          संख्या_locale_warned = true;
        }
      अन्यथा अगर (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((अक्षर []) {tab, 0})));
        }
      अन्यथा अगर (general_numeric_field_span && tab == '+')
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
  अगर ((basic_numeric_field || general_numeric_field) && ! संख्या_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((अक्षर []) {decimal_point, 0})));

    }

  अगर (basic_numeric_field && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  अगर (!default_कुंजी_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      अगर (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      अक्षर opts[माप short_options];
      कुंजी_to_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (म_माप (opts))), opts);
      ugkey.reverse = ugkey_reverse;
    }
  अगर (ugkey.reverse && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

अटल पूर्णांक
diff_reversed (पूर्णांक diff, bool reversed)
{
  वापस reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

अटल पूर्णांक
keycompare (काष्ठा पंक्ति स्थिर *a, काष्ठा पंक्ति स्थिर *b)
{
  काष्ठा keyfield *कुंजी = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  अक्षर *texta = a->keybeg;
  अक्षर *textb = b->keybeg;
  अक्षर *lima = a->keylim;
  अक्षर *limb = b->keylim;

  पूर्णांक diff;

  जबतक (true)
    {
      अक्षर स्थिर *translate = कुंजी->translate;
      bool स्थिर *ignore = कुंजी->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      माप_प्रकार lena = lima - texta;
      माप_प्रकार lenb = limb - textb;

      अगर (hard_LC_COLLATE || कुंजी_numeric (कुंजी)
          || कुंजी->month || कुंजी->random || कुंजी->version)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          अक्षर *ta = texta;
          अक्षर *tb = textb;
          माप_प्रकार tlena = lena;
          माप_प्रकार tlenb = lenb;
          अक्षर enda = ta[tlena];
          अक्षर endb = tb[tlenb];

          व्योम *allocated = शून्य;
          अक्षर stackbuf[4000];

          अगर (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              माप_प्रकार i;

              /* Allocate space for copies.  */
              माप_प्रकार आकार = lena + 1 + lenb + 1;
              अगर (आकार <= माप stackbuf)
                ta = stackbuf;
              अन्यथा
                ta = allocated = xmalloc (आकार);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              क्रम (tlena = i = 0; i < lena; i++)
                अगर (! (ignore && ignore[to_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[to_uchar (texta[i])]
                                 : texta[i]);

              क्रम (tlenb = i = 0; i < lenb; i++)
                अगर (! (ignore && ignore[to_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[to_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          अगर (कुंजी->numeric)
            diff = numcompare (ta, tb);
          अन्यथा अगर (कुंजी->general_numeric)
            diff = general_numcompare (ta, tb);
          अन्यथा अगर (कुंजी->human_numeric)
            diff = human_numcompare (ta, tb);
          अन्यथा अगर (कुंजी->month)
            diff = getmonth (ta, शून्य) - getmonth (tb, शून्य);
          अन्यथा अगर (कुंजी->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          अन्यथा अगर (कुंजी->version)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          अन्यथा
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              अगर (tlena == 0)
                diff = - NONZERO (tlenb);
              अन्यथा अगर (tlenb == 0)
                diff = 1;
              अन्यथा
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          मुक्त (allocated);
        }
      अन्यथा अगर (ignore)
        {
#घोषणा CMP_WITH_IGNORE(A, B)						\
  करो									\
    {									\
          जबतक (true)							\
            {								\
              जबतक (texta < lima && ignore[to_uchar (*texta)])		\
                ++texta;						\
              जबतक (textb < limb && ignore[to_uchar (*textb)])		\
                ++textb;						\
              अगर (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  अवरोध;						\
                }							\
              diff = to_uchar (A) - to_uchar (B);			\
              अगर (diff)							\
                अवरोध;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  जबतक (0)

          अगर (translate)
            CMP_WITH_IGNORE (translate[to_uchar (*texta)],
                             translate[to_uchar (*textb)]);
          अन्यथा
            CMP_WITH_IGNORE (*texta, *textb);
        }
      अन्यथा
        {
          माप_प्रकार lenmin = MIN (lena, lenb);
          अगर (lenmin == 0)
            diff = 0;
          अन्यथा अगर (translate)
            {
              माप_प्रकार i = 0;
              करो
                {
                  diff = (to_uchar (translate[to_uchar (texta[i])])
                          - to_uchar (translate[to_uchar (textb[i])]));
                  अगर (diff)
                    अवरोध;
                  i++;
                }
              जबतक (i < lenmin);
            }
          अन्यथा
            diff = स_भेद (texta, textb, lenmin);

          अगर (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      अगर (diff)
        अवरोध;

      कुंजी = कुंजी->अग्र;
      अगर (! कुंजी)
        वापस 0;

      /* Find the beginning and limit of the next field.  */
      अगर (कुंजी->eword != SIZE_MAX)
        lima = limfield (a, कुंजी), limb = limfield (b, कुंजी);
      अन्यथा
        lima = a->text + a->लंबाई - 1, limb = b->text + b->लंबाई - 1;

      अगर (कुंजी->sword != SIZE_MAX)
        texta = begfield (a, कुंजी), textb = begfield (b, कुंजी);
      अन्यथा
        {
          texta = a->text, textb = b->text;
          अगर (कुंजी->skipsblanks)
            {
              जबतक (texta < lima && blanks[to_uchar (*texta)])
                ++texta;
              जबतक (textb < limb && blanks[to_uchar (*textb)])
                ++textb;
            }
        }
    }

  वापस diff_reversed (diff, कुंजी->reverse);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

अटल पूर्णांक
compare (काष्ठा पंक्ति स्थिर *a, काष्ठा पंक्ति स्थिर *b)
{
  पूर्णांक diff;
  माप_प्रकार alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  अगर (keylist)
    {
      diff = keycompare (a, b);
      अगर (diff || unique || stable)
        वापस diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->लंबाई - 1, blen = b->लंबाई - 1;

  अगर (alen == 0)
    diff = - NONZERO (blen);
  अन्यथा अगर (blen == 0)
    diff = 1;
  अन्यथा अगर (hard_LC_COLLATE)
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
      अगर (!diff)
        diff = _GL_CMP (alen, blen);
    }

  वापस diff_reversed (diff, reverse);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

अटल व्योम
write_पंक्ति (काष्ठा पंक्ति स्थिर *पंक्ति, खाता *fp, अक्षर स्थिर *output_file)
{
  अक्षर *buf = पंक्ति->text;
  माप_प्रकार n_bytes = पंक्ति->लंबाई;
  अक्षर *ebuf = buf + n_bytes;

  अगर (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      अक्षर स्थिर *c = buf;

      जबतक (c < ebuf)
        {
          अक्षर wc = *c++;
          अगर (wc == '\t')
            wc = '>';
          अन्यथा अगर (c == ebuf)
            wc = '\n';
          अगर (ख_अक्षर_दो (wc, fp) == खातापूर्ण)
            sort_die (_("write failed"), output_file);
        }

      debug_पंक्ति (पंक्ति);
    }
  अन्यथा
    {
      ebuf[-1] = eolchar;
      अगर (ख_डालो (buf, 1, n_bytes, fp) != n_bytes)
        sort_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

अटल bool
check (अक्षर स्थिर *file_name, अक्षर checkonly)
{
  खाता *fp = xfopen (file_name, "r");
  काष्ठा बफर buf;		/* Input buffer. */
  काष्ठा पंक्ति अस्थायी;		/* Copy of previous line. */
  माप_प्रकार alloc = 0;
  uintmax_t पंक्ति_संख्या = 0;
  काष्ठा keyfield स्थिर *कुंजी = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, माप (काष्ठा पंक्ति),
           MAX (merge_बफर_आकार, sort_आकार));
  अस्थायी.text = शून्य;

  जबतक (fillbuf (&buf, fp, file_name, शून्य))
    {
      काष्ठा पंक्ति स्थिर *पंक्ति = बफर_linelim (&buf);
      काष्ठा पंक्ति स्थिर *linebase = पंक्ति - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      अगर (alloc && nonunique <= compare (&अस्थायी, पंक्ति - 1))
        {
        found_disorder:
          {
            अगर (checkonly == 'c')
              {
                काष्ठा पंक्ति स्थिर *disorder_पंक्ति = पंक्ति - 1;
                uintmax_t disorder_पंक्ति_संख्या =
                  बफर_linelim (&buf) - disorder_पंक्ति + पंक्ति_संख्या;
                ख_लिखो (मानक_त्रुटि, _("%s: %s:%ju: disorder: "),
                         program_name, file_name, disorder_पंक्ति_संख्या);
                write_पंक्ति (disorder_पंक्ति, मानक_त्रुटि, _("standard error"));
              }

            ordered = false;
            अवरोध;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      जबतक (linebase < --पंक्ति)
        अगर (nonunique <= compare (पंक्ति, पंक्ति - 1))
          जाओ found_disorder;

      पंक्ति_संख्या += buf.nlines;

      /* Save the last line of the buffer.  */
      अगर (alloc < पंक्ति->लंबाई)
        {
          करो
            {
              alloc *= 2;
              अगर (! alloc)
                {
                  alloc = पंक्ति->लंबाई;
                  अवरोध;
                }
            }
          जबतक (alloc < पंक्ति->लंबाई);

          मुक्त (अस्थायी.text);
          अस्थायी.text = xmalloc (alloc);
        }
      स_नकल (अस्थायी.text, पंक्ति->text, पंक्ति->लंबाई);
      अस्थायी.लंबाई = पंक्ति->लंबाई;
      अगर (कुंजी)
        {
          अस्थायी.keybeg = अस्थायी.text + (पंक्ति->keybeg - पंक्ति->text);
          अस्थायी.keylim = अस्थायी.text + (पंक्ति->keylim - पंक्ति->text);
        }
    }

  xfclose (fp, file_name);
  मुक्त (buf.buf);
  मुक्त (अस्थायी.text);
  वापस ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

अटल माप_प्रकार
open_निवेश_files (काष्ठा sortfile *files, माप_प्रकार nfiles, खाता ***pfps)
{
  खाता **fps = *pfps = xnmalloc (nfiles, माप *fps);
  पूर्णांक i;

  /* Open as many input files as we can.  */
  क्रम (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].अस्थायी && files[i].अस्थायी->state != UNCOMPRESSED
                ? open_अस्थायी (files[i].अस्थायी)
                : stream_open (files[i].name, "r"));
      अगर (!fps[i])
        अवरोध;
    }

  वापस i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

अटल व्योम
mergefps (काष्ठा sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
          खाता *ofp, अक्षर स्थिर *output_file, खाता **fps)
{
  काष्ठा बफर *बफर = xnmalloc (nfiles, माप *बफर);
                                /* Input buffers for each file. */
  काष्ठा पंक्ति saved;		/* Saved line storage for unique check. */
  काष्ठा पंक्ति स्थिर *savedline = शून्य;
                                /* &saved if there is a saved line. */
  माप_प्रकार savealloc = 0;		/* Size allocated for the saved line. */
  काष्ठा पंक्ति स्थिर **cur = xnmalloc (nfiles, माप *cur);
                                /* Current line in each line table. */
  काष्ठा पंक्ति स्थिर **base = xnmalloc (nfiles, माप *base);
                                /* Base of each line table.  */
  माप_प्रकार *ord = xnmalloc (nfiles, माप *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  माप_प्रकार t;
  काष्ठा keyfield स्थिर *कुंजी = keylist;
  saved.text = शून्य;

  /* Read initial lines from each input file. */
  क्रम (माप_प्रकार i = 0; i < nfiles; )
    {
      initbuf (&बफर[i], माप (काष्ठा पंक्ति),
               MAX (merge_बफर_आकार, sort_आकार / nfiles));
      अगर (fillbuf (&बफर[i], fps[i], files[i].name, शून्य))
        {
          काष्ठा पंक्ति स्थिर *linelim = बफर_linelim (&बफर[i]);
          cur[i] = linelim - 1;
          base[i] = linelim - बफर[i].nlines;
          i++;
        }
      अन्यथा
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          अगर (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          मुक्त (बफर[i].buf);
          --nfiles;
          क्रम (माप_प्रकार j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  क्रम (माप_प्रकार i = 0; i < nfiles; ++i)
    ord[i] = i;
  क्रम (माप_प्रकार i = 1; i < nfiles; ++i)
    अगर (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  जबतक (nfiles)
    {
      काष्ठा पंक्ति स्थिर *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      अगर (unique)
        {
          अगर (savedline && compare (savedline, smallest))
            {
              savedline = शून्य;
              write_पंक्ति (&saved, ofp, output_file);
            }
          अगर (!savedline)
            {
              savedline = &saved;
              अगर (savealloc < smallest->लंबाई)
                {
                  करो
                    अगर (! savealloc)
                      {
                        savealloc = smallest->लंबाई;
                        अवरोध;
                      }
                  जबतक ((savealloc *= 2) < smallest->लंबाई);

                  मुक्त (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.लंबाई = smallest->लंबाई;
              स_नकल (saved.text, smallest->text, saved.लंबाई);
              अगर (कुंजी)
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
      अगर (base[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      अन्यथा
        {
          अगर (fillbuf (&बफर[ord[0]], fps[ord[0]], files[ord[0]].name,
                       शून्य))
            {
              काष्ठा पंक्ति स्थिर *linelim = बफर_linelim (&बफर[ord[0]]);
              cur[ord[0]] = linelim - 1;
              base[ord[0]] = linelim - बफर[ord[0]].nlines;
            }
          अन्यथा
            {
              /* We reached EOF on fps[ord[0]].  */
              क्रम (माप_प्रकार i = 1; i < nfiles; ++i)
                अगर (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              अगर (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              मुक्त (बफर[ord[0]].buf);
              क्रम (माप_प्रकार i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  बफर[i] = बफर[i + 1];
                  cur[i] = cur[i + 1];
                  base[i] = base[i + 1];
                }
              क्रम (माप_प्रकार i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              जारी;
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

        जबतक (lo < hi)
          {
            पूर्णांक cmp = compare (cur[ord0], cur[ord[probe]]);
            अगर (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            अन्यथा
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        गिनती_of_smaller_पंक्तियाँ = lo - 1;
        क्रम (माप_प्रकार j = 0; j < गिनती_of_smaller_पंक्तियाँ; j++)
          ord[j] = ord[j + 1];
        ord[गिनती_of_smaller_पंक्तियाँ] = ord0;
      }
    }

  अगर (unique && savedline)
    {
      write_पंक्ति (&saved, ofp, output_file);
      मुक्त (saved.text);
    }

  xfclose (ofp, output_file);
  मुक्त (fps);
  मुक्त (बफर);
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

अटल माप_प्रकार
mergefiles (काष्ठा sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
            खाता *ofp, अक्षर स्थिर *output_file)
{
  खाता **fps;
  माप_प्रकार nopened = open_निवेश_files (files, nfiles, &fps);
  अगर (nopened < nfiles && nopened < 2)
    sort_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  वापस nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

अटल व्योम
mergelines (काष्ठा पंक्ति *restrict t, माप_प्रकार nlines,
            काष्ठा पंक्ति स्थिर *restrict lo)
{
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  काष्ठा पंक्ति *hi = t - nlo;

  जबतक (true)
    अगर (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        अगर (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            वापस;
          }
      }
    अन्यथा
      {
        *--t = *--hi;
        अगर (! --nhi)
          {
            करो
              *--t = *--lo;
            जबतक (--nlo);

            वापस;
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

अटल व्योम
sequential_sort (काष्ठा पंक्ति *restrict पंक्तियाँ, माप_प्रकार nlines,
                 काष्ठा पंक्ति *restrict अस्थायी, bool to_अस्थायी)
{
  अगर (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      पूर्णांक swap = (0 < compare (&पंक्तियाँ[-1], &पंक्तियाँ[-2]));
      अगर (to_अस्थायी)
        {
          अस्थायी[-1] = पंक्तियाँ[-1 - swap];
          अस्थायी[-2] = पंक्तियाँ[-2 + swap];
        }
      अन्यथा अगर (swap)
        {
          अस्थायी[-1] = पंक्तियाँ[-1];
          पंक्तियाँ[-1] = पंक्तियाँ[-2];
          पंक्तियाँ[-2] = अस्थायी[-1];
        }
    }
  अन्यथा
    {
      माप_प्रकार nlo = nlines / 2;
      माप_प्रकार nhi = nlines - nlo;
      काष्ठा पंक्ति *lo = पंक्तियाँ;
      काष्ठा पंक्ति *hi = पंक्तियाँ - nlo;

      sequential_sort (hi, nhi, अस्थायी - (to_अस्थायी ? nlo : 0), to_अस्थायी);
      अगर (1 < nlo)
        sequential_sort (lo, nlo, अस्थायी, !to_अस्थायी);
      अन्यथा अगर (!to_अस्थायी)
        अस्थायी[-1] = lo[-1];

      काष्ठा पंक्ति *dest;
      काष्ठा पंक्ति स्थिर *sorted_lo;
      अगर (to_अस्थायी)
        {
          dest = अस्थायी;
          sorted_lo = पंक्तियाँ;
        }
      अन्यथा
        {
          dest = पंक्तियाँ;
          sorted_lo = अस्थायी;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

अटल काष्ठा merge_गाँठ *init_गाँठ (काष्ठा merge_गाँठ *restrict,
                                     काष्ठा merge_गाँठ *restrict,
                                     काष्ठा पंक्ति *, माप_प्रकार, माप_प्रकार, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
अटल काष्ठा merge_गाँठ *
merge_वृक्ष_init (माप_प्रकार nthreads, माप_प्रकार nlines, काष्ठा पंक्ति *dest)
{
  काष्ठा merge_गाँठ *merge_वृक्ष = xmalloc (2 * माप *merge_वृक्ष * nthreads);

  काष्ठा merge_गाँठ *मूल = merge_वृक्ष;
  मूल->lo = मूल->hi = मूल->end_lo = मूल->end_hi = शून्य;
  मूल->dest = शून्य;
  मूल->nlo = मूल->nhi = nlines;
  मूल->parent = शून्य;
  मूल->level = MERGE_END;
  मूल->queued = false;
  pthread_mutex_init (&मूल->lock, शून्य);

  init_गाँठ (मूल, मूल + 1, dest, nthreads, nlines, false);
  वापस merge_वृक्ष;
}

/* Destroy the merge tree. */
अटल व्योम
merge_वृक्ष_destroy (माप_प्रकार nthreads, काष्ठा merge_गाँठ *merge_वृक्ष)
{
  माप_प्रकार n_nodes = nthreads * 2;
  काष्ठा merge_गाँठ *गाँठ = merge_वृक्ष;

  जबतक (n_nodes--)
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

अटल काष्ठा merge_गाँठ *
init_गाँठ (काष्ठा merge_गाँठ *restrict parent,
           काष्ठा merge_गाँठ *restrict गाँठ_pool,
           काष्ठा पंक्ति *dest, माप_प्रकार nthreads,
           माप_प्रकार योग_पंक्तियाँ, bool is_lo_child)
{
  माप_प्रकार nlines = (is_lo_child ? parent->nlo : parent->nhi);
  माप_प्रकार nlo = nlines / 2;
  माप_प्रकार nhi = nlines - nlo;
  काष्ठा पंक्ति *lo = dest - योग_पंक्तियाँ;
  काष्ठा पंक्ति *hi = lo - nlo;
  काष्ठा पंक्ति **parent_end = (is_lo_child ? &parent->end_lo : &parent->end_hi);

  काष्ठा merge_गाँठ *गाँठ = गाँठ_pool++;
  गाँठ->lo = गाँठ->end_lo = lo;
  गाँठ->hi = गाँठ->end_hi = hi;
  गाँठ->dest = parent_end;
  गाँठ->nlo = nlo;
  गाँठ->nhi = nhi;
  गाँठ->parent = parent;
  गाँठ->level = parent->level + 1;
  गाँठ->queued = false;
  pthread_mutex_init (&गाँठ->lock, शून्य);

  अगर (nthreads > 1)
    {
      माप_प्रकार lo_threads = nthreads / 2;
      माप_प्रकार hi_threads = nthreads - lo_threads;
      गाँठ->lo_child = गाँठ_pool;
      गाँठ_pool = init_गाँठ (गाँठ, गाँठ_pool, lo, lo_threads,
                             योग_पंक्तियाँ, true);
      गाँठ->hi_child = गाँठ_pool;
      गाँठ_pool = init_गाँठ (गाँठ, गाँठ_pool, hi, hi_threads,
                             योग_पंक्तियाँ, false);
    }
  अन्यथा
    {
      गाँठ->lo_child = शून्य;
      गाँठ->hi_child = शून्य;
    }
  वापस गाँठ_pool;
}


/* Compare two merge nodes A and B for priority.  */

अटल पूर्णांक
compare_nodes (व्योम स्थिर *a, व्योम स्थिर *b)
{
  काष्ठा merge_गाँठ स्थिर *nodea = a;
  काष्ठा merge_गाँठ स्थिर *nodeb = b;
  अगर (nodea->level == nodeb->level)
      वापस (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  वापस nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

अटल अंतरभूत व्योम
lock_गाँठ (काष्ठा merge_गाँठ *गाँठ)
{
  pthread_mutex_lock (&गाँठ->lock);
}

/* Unlock a merge tree NODE. */

अटल अंतरभूत व्योम
unlock_गाँठ (काष्ठा merge_गाँठ *गाँठ)
{
  pthread_mutex_unlock (&गाँठ->lock);
}

/* Destroy merge QUEUE. */

अटल व्योम
queue_destroy (काष्ठा merge_गाँठ_queue *queue)
{
  heap_free (queue->priority_queue);
  pthread_cond_destroy (&queue->cond);
  pthread_mutex_destroy (&queue->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

अटल व्योम
queue_init (काष्ठा merge_गाँठ_queue *queue, माप_प्रकार nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  queue->priority_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, शून्य);
  pthread_cond_init (&queue->cond, शून्य);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

अटल व्योम
queue_जोड़ो (काष्ठा merge_गाँठ_queue *queue, काष्ठा merge_गाँठ *गाँठ)
{
  pthread_mutex_lock (&queue->mutex);
  heap_जोड़ो (queue->priority_queue, गाँठ);
  गाँठ->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

अटल काष्ठा merge_गाँठ *
queue_निकालो (काष्ठा merge_गाँठ_queue *queue)
{
  काष्ठा merge_गाँठ *गाँठ;
  pthread_mutex_lock (&queue->mutex);
  जबतक (! (गाँठ = heap_remove_शीर्ष (queue->priority_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  lock_गाँठ (गाँठ);
  गाँठ->queued = false;
  वापस गाँठ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

अटल व्योम
write_unique (काष्ठा पंक्ति स्थिर *पंक्ति, खाता *tfp, अक्षर स्थिर *अस्थायी_output)
{
  अगर (unique)
    {
      अगर (saved_पंक्ति.text && ! compare (पंक्ति, &saved_पंक्ति))
        वापस;
      saved_पंक्ति = *पंक्ति;
    }

  write_पंक्ति (पंक्ति, tfp, अस्थायी_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

अटल व्योम
mergelines_गाँठ (काष्ठा merge_गाँठ *restrict गाँठ, माप_प्रकार योग_पंक्तियाँ,
                 खाता *tfp, अक्षर स्थिर *अस्थायी_output)
{
  काष्ठा पंक्ति *lo_orig = गाँठ->lo;
  काष्ठा पंक्ति *hi_orig = गाँठ->hi;
  माप_प्रकार to_merge = MAX_MERGE (योग_पंक्तियाँ, गाँठ->level);
  माप_प्रकार merged_lo;
  माप_प्रकार merged_hi;

  अगर (गाँठ->level > MERGE_मूल)
    {
      /* Merge to destination buffer. */
      काष्ठा पंक्ति *dest = *गाँठ->dest;
      जबतक (गाँठ->lo != गाँठ->end_lo && गाँठ->hi != गाँठ->end_hi && to_merge--)
        अगर (compare (गाँठ->lo - 1, गाँठ->hi - 1) <= 0)
          *--dest = *--गाँठ->lo;
        अन्यथा
          *--dest = *--गाँठ->hi;

      merged_lo = lo_orig - गाँठ->lo;
      merged_hi = hi_orig - गाँठ->hi;

      अगर (गाँठ->nhi == merged_hi)
        जबतक (गाँठ->lo != गाँठ->end_lo && to_merge--)
          *--dest = *--गाँठ->lo;
      अन्यथा अगर (गाँठ->nlo == merged_lo)
        जबतक (गाँठ->hi != गाँठ->end_hi && to_merge--)
          *--dest = *--गाँठ->hi;
      *गाँठ->dest = dest;
    }
  अन्यथा
    {
      /* Merge directly to output. */
      जबतक (गाँठ->lo != गाँठ->end_lo && गाँठ->hi != गाँठ->end_hi && to_merge--)
        {
          अगर (compare (गाँठ->lo - 1, गाँठ->hi - 1) <= 0)
            write_unique (--गाँठ->lo, tfp, अस्थायी_output);
          अन्यथा
            write_unique (--गाँठ->hi, tfp, अस्थायी_output);
        }

      merged_lo = lo_orig - गाँठ->lo;
      merged_hi = hi_orig - गाँठ->hi;

      अगर (गाँठ->nhi == merged_hi)
        {
          जबतक (गाँठ->lo != गाँठ->end_lo && to_merge--)
            write_unique (--गाँठ->lo, tfp, अस्थायी_output);
        }
      अन्यथा अगर (गाँठ->nlo == merged_lo)
        {
          जबतक (गाँठ->hi != गाँठ->end_hi && to_merge--)
            write_unique (--गाँठ->hi, tfp, अस्थायी_output);
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

अटल व्योम
queue_check_जोड़ो (काष्ठा merge_गाँठ_queue *queue, काष्ठा merge_गाँठ *गाँठ)
{
  अगर (! गाँठ->queued)
    {
      bool lo_avail = (गाँठ->lo - गाँठ->end_lo) != 0;
      bool hi_avail = (गाँठ->hi - गाँठ->end_hi) != 0;
      अगर (lo_avail ? hi_avail || ! गाँठ->nhi : hi_avail && ! गाँठ->nlo)
        queue_जोड़ो (queue, गाँठ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

अटल व्योम
queue_check_जोड़ो_parent (काष्ठा merge_गाँठ_queue *queue,
                           काष्ठा merge_गाँठ *गाँठ)
{
  अगर (गाँठ->level > MERGE_मूल)
    {
      lock_गाँठ (गाँठ->parent);
      queue_check_जोड़ो (queue, गाँठ->parent);
      unlock_गाँठ (गाँठ->parent);
    }
  अन्यथा अगर (गाँठ->nlo + गाँठ->nhi == 0)
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

अटल व्योम
merge_loop (काष्ठा merge_गाँठ_queue *queue,
            माप_प्रकार योग_पंक्तियाँ, खाता *tfp, अक्षर स्थिर *अस्थायी_output)
{
  जबतक (true)
    {
      काष्ठा merge_गाँठ *गाँठ = queue_निकालो (queue);

      अगर (गाँठ->level == MERGE_END)
        {
          unlock_गाँठ (गाँठ);
          /* Reinsert so other threads can pop it. */
          queue_जोड़ो (queue, गाँठ);
          अवरोध;
        }
      mergelines_गाँठ (गाँठ, योग_पंक्तियाँ, tfp, अस्थायी_output);
      queue_check_जोड़ो (queue, गाँठ);
      queue_check_जोड़ो_parent (queue, गाँठ);

      unlock_गाँठ (गाँठ);
    }
}


अटल व्योम sortlines (काष्ठा पंक्ति *restrict, माप_प्रकार, माप_प्रकार,
                       काष्ठा merge_गाँठ *, काष्ठा merge_गाँठ_queue *,
                       खाता *, अक्षर स्थिर *);

/* Thread arguments for sortlines_thread. */

काष्ठा thread_args
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  काष्ठा पंक्ति *पंक्तियाँ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  माप_प्रकार nthreads;

  /* Number of lines in LINES and DEST.  */
  माप_प्रकार स्थिर योग_पंक्तियाँ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  काष्ठा merge_गाँठ *स्थिर गाँठ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  काष्ठा merge_गाँठ_queue *स्थिर queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  खाता *tfp;
  अक्षर स्थिर *output_अस्थायी;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

अटल व्योम *
sortlines_thread (व्योम *तथ्य)
{
  काष्ठा thread_args स्थिर *args = तथ्य;
  sortlines (args->पंक्तियाँ, args->nthreads, args->योग_पंक्तियाँ,
             args->गाँठ, args->queue, args->tfp,
             args->output_अस्थायी);
  वापस शून्य;
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

अटल व्योम
sortlines (काष्ठा पंक्ति *restrict पंक्तियाँ, माप_प्रकार nthreads,
           माप_प्रकार योग_पंक्तियाँ, काष्ठा merge_गाँठ *गाँठ,
           काष्ठा merge_गाँठ_queue *queue, खाता *tfp, अक्षर स्थिर *अस्थायी_output)
{
  माप_प्रकार nlines = गाँठ->nlo + गाँठ->nhi;

  /* Calculate thread arguments. */
  माप_प्रकार lo_threads = nthreads / 2;
  माप_प्रकार hi_threads = nthreads - lo_threads;
  pthread_t thread;
  काष्ठा thread_args args = {पंक्तियाँ, lo_threads, योग_पंक्तियाँ,
                             गाँठ->lo_child, queue, tfp, अस्थायी_output};

  अगर (nthreads > 1 && SUBTHREAD_पंक्तियाँ_HEURISTIC <= nlines
      && pthread_create (&thread, शून्य, sortlines_thread, &args) == 0)
    {
      sortlines (पंक्तियाँ - गाँठ->nlo, hi_threads, योग_पंक्तियाँ,
                 गाँठ->hi_child, queue, tfp, अस्थायी_output);
      pthread_join (thread, शून्य);
    }
  अन्यथा
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      माप_प्रकार nlo = गाँठ->nlo;
      माप_प्रकार nhi = गाँठ->nhi;
      काष्ठा पंक्ति *अस्थायी = पंक्तियाँ - योग_पंक्तियाँ;
      अगर (1 < nhi)
        sequential_sort (पंक्तियाँ - nlo, nhi, अस्थायी - nlo / 2, false);
      अगर (1 < nlo)
        sequential_sort (पंक्तियाँ, nlo, अस्थायी, false);

      /* Update merge NODE. No need to lock yet. */
      गाँठ->lo = पंक्तियाँ;
      गाँठ->hi = पंक्तियाँ - nlo;
      गाँठ->end_lo = पंक्तियाँ - nlo;
      गाँठ->end_hi = पंक्तियाँ - nlo - nhi;

      queue_जोड़ो (queue, गाँठ);
      merge_loop (queue, योग_पंक्तियाँ, tfp, अस्थायी_output);
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

अटल व्योम
avoid_trashing_निवेश (काष्ठा sortfile *files, माप_प्रकार ntemps,
                      माप_प्रकार nfiles, अक्षर स्थिर *outfile)
{
  काष्ठा tempnode *tempcopy = शून्य;

  क्रम (माप_प्रकार i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      काष्ठा stat instat;

      अगर (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      अन्यथा
        {
          काष्ठा stat *outst = get_outstatus ();
          अगर (!outst)
            अवरोध;

          same = (((is_stdin
                    ? ख_स्थिति (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      अगर (same)
        {
          अगर (! tempcopy)
            {
              खाता *tftp;
              tempcopy = create_अस्थायी (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].अस्थायी = tempcopy;
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

अटल व्योम
check_inputs (अक्षर *स्थिर *files, माप_प्रकार nfiles)
{
  क्रम (माप_प्रकार i = 0; i < nfiles; i++)
    {
      अगर (streq (files[i], "-"))
        जारी;

      अगर (euidaccess (files[i], R_OK) != 0)
        sort_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

अटल व्योम
check_output (अक्षर स्थिर *outfile)
{
  अगर (outfile)
    {
      पूर्णांक oflags = O_WRONLY | O_द्विआधारी | O_CLOEXEC | O_CREAT;
      पूर्णांक outfd = खोलो (outfile, oflags, MODE_RW_UGO);
      अगर (outfd < 0)
        sort_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

अटल व्योम
merge (काष्ठा sortfile *files, माप_प्रकार ntemps, माप_प्रकार nfiles,
       अक्षर स्थिर *output_file)
{
  जबतक (nmerge < nfiles)
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
      क्रम (out = in = 0; nmerge <= nfiles - in; out++)
        {
          खाता *tfp;
          काष्ठा tempnode *अस्थायी = create_अस्थायी (&tfp);
          माप_प्रकार अंक_merged = mergefiles (&files[in], MIN (ntemps, nmerge),
                                          nmerge, tfp, अस्थायी->name);
          ntemps -= MIN (ntemps, अंक_merged);
          files[out].name = अस्थायी->name;
          files[out].अस्थायी = अस्थायी;
          in += अंक_merged;
        }

      remainder = nfiles - in;
      cheap_slots = nmerge - out % nmerge;

      अगर (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          माप_प्रकार nshortmerge = remainder - cheap_slots + 1;
          खाता *tfp;
          काष्ठा tempnode *अस्थायी = create_अस्थायी (&tfp);
          माप_प्रकार अंक_merged = mergefiles (&files[in], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, अस्थायी->name);
          ntemps -= MIN (ntemps, अंक_merged);
          files[out].name = अस्थायी->name;
          files[out++].अस्थायी = अस्थायी;
          in += अंक_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      स_हटाओ (&files[out], &files[in], (nfiles - in) * माप *files);
      ntemps += out;
      nfiles -= in - out;
    }

  avoid_trashing_निवेश (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  जबतक (true)
    {
      /* Merge directly into the output file if possible.  */
      खाता **fps;
      माप_प्रकार nopened = open_निवेश_files (files, nfiles, &fps);

      अगर (nopened == nfiles)
        {
          खाता *ofp = stream_open (output_file, "w");
          अगर (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              अवरोध;
            }
          अगर (त्रुटि_सं != EMFILE || nopened <= 2)
            sort_die (_("open failed"), output_file);
        }
      अन्यथा अगर (nopened <= 2)
        sort_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      खाता *tfp;
      काष्ठा tempnode *अस्थायी;
      करो
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          अस्थायी = maybe_create_अस्थायी (&tfp, ! (nopened <= 2));
        }
      जबतक (!अस्थायी);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, अस्थायी->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = अस्थायी->name;
      files[0].अस्थायी = अस्थायी;

      स_हटाओ (&files[1], &files[nopened], (nfiles - nopened) * माप *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

अटल व्योम
sort (अक्षर *स्थिर *files, माप_प्रकार nfiles, अक्षर स्थिर *output_file,
      माप_प्रकार nthreads)
{
  काष्ठा बफर buf;
  काष्ठा sort_बफर_policy policy;
  माप_प्रकार ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.limit = 0;
  policy.growth_failed = false;

  जबतक (nfiles)
    {
      अक्षर स्थिर *अस्थायी_output;
      अक्षर स्थिर *file = *files;
      खाता *fp = xfopen (file, "r");
      खाता *tfp;

      माप_प्रकार bytes_per_पंक्ति;
      अगर (nthreads > 1)
        {
          /* Get log P. */
          माप_प्रकार tmp = 1;
          माप_प्रकार mult = 1;
          जबतक (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          bytes_per_पंक्ति = (mult * माप (काष्ठा पंक्ति));
        }
      अन्यथा
        bytes_per_पंक्ति = माप (काष्ठा पंक्ति) * 3 / 2;

      अगर (! buf.alloc)
        {
          sort_बफर_policy (&fp, 1, files, nfiles, bytes_per_पंक्ति,
                              &policy);
          initbuf (&buf, bytes_per_पंक्ति, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      जबतक (fillbuf (&buf, fp, file, &policy))
        {
          काष्ठा पंक्ति *पंक्ति;

          अगर (buf.eof && nfiles
              && (bytes_per_पंक्ति + 1
                  < (buf.alloc - buf.used - bytes_per_पंक्ति * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.बायाँ = buf.used;
              अवरोध;
            }

          saved_पंक्ति.text = शून्य;
          पंक्ति = बफर_linelim (&buf);
          अगर (buf.eof && !nfiles && !ntemps && !buf.बायाँ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              अस्थायी_output = output_file;
              output_file_created = true;
            }
          अन्यथा
            {
              ++ntemps;
              अस्थायी_output = create_अस्थायी (&tfp)->name;
            }
          अगर (1 < buf.nlines)
            {
              अगर (nthreads > 1)
                {
                  काष्ठा merge_गाँठ_queue queue;
                  queue_init (&queue, nthreads);
                  काष्ठा merge_गाँठ *merge_वृक्ष =
                    merge_वृक्ष_init (nthreads, buf.nlines, पंक्ति);

                  sortlines (पंक्ति, nthreads, buf.nlines, merge_वृक्ष + 1,
                             &queue, tfp, अस्थायी_output);

                  merge_वृक्ष_destroy (nthreads, merge_वृक्ष);
                  queue_destroy (&queue);
                }
              अन्यथा
                {
                  sequential_sort (पंक्ति, buf.nlines,
                                   पंक्ति - buf.nlines, false);
                  क्रम (माप_प्रकार i = 0; i < buf.nlines; i++)
                    write_unique (पंक्ति - i - 1, tfp, अस्थायी_output);
                }
            }
          अन्यथा
            write_unique (पंक्ति - 1, tfp, अस्थायी_output);

          xfclose (tfp, अस्थायी_output);

          अगर (output_file_created)
            जाओ finish;
        }
      xfclose (fp, file);
    }

 finish:
  मुक्त (buf.buf);

  अगर (! output_file_created)
    {
      काष्ठा tempnode *गाँठ = temphead;
      काष्ठा sortfile *tempfiles = xnmalloc (ntemps, माप *tempfiles);
      क्रम (माप_प्रकार i = 0; गाँठ; i++)
        {
          tempfiles[i].name = गाँठ->name;
          tempfiles[i].अस्थायी = गाँठ;
          गाँठ = गाँठ->अग्र;
        }
      merge (tempfiles, ntemps, ntemps, output_file);
      मुक्त (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

अटल व्योम
insertkey (काष्ठा keyfield *कुंजी_arg)
{
  काष्ठा keyfield **p;
  काष्ठा keyfield *कुंजी = xmemdup (कुंजी_arg, माप *कुंजी);

  क्रम (p = &keylist; *p; p = &(*p)->अग्र)
    जारी;
  *p = कुंजी;
  कुंजी->अग्र = शून्य;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

अटल व्योम
badfieldspec (अक्षर स्थिर *spec, अक्षर स्थिर *msgid)
{
  error (SORT_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

अटल व्योम
incompatible_options (अक्षर स्थिर *opts)
{
  error (SORT_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

अटल व्योम
check_ordering_compatibility (व्योम)
{
  क्रम (काष्ठा keyfield *कुंजी = keylist; कुंजी; कुंजी = कुंजी->अग्र)
    अगर (1 < (कुंजी->numeric + कुंजी->general_numeric + कुंजी->human_numeric
             + कुंजी->month + (कुंजी->version | कुंजी->random | !!कुंजी->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        अक्षर opts[माप short_options];
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

अटल अक्षर स्थिर *
parse_field_गिनती (अक्षर स्थिर *string, माप_प्रकार *val, अक्षर स्थिर *msgid)
{
  अक्षर *suffix;
  uintmax_t n;

  चयन (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    हाल LONGINT_OK:
    हाल LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      अगर (*val == n)
        अवरोध;
      FALLTHROUGH;
    हाल LONGINT_OVERFLOW:
    हाल LONGINT_OVERFLOW | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      अवरोध;

    हाल LONGINT_INVALID:
      अगर (msgid)
        error (SORT_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      वापस शून्य;
    }

  वापस suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

अटल अक्षर *
set_ordering (अक्षर स्थिर *s, काष्ठा keyfield *कुंजी, क्रमागत blanktype blanktype)
{
  जबतक (*s)
    {
      चयन (*s)
        {
        हाल 'b':
          अगर (blanktype == bl_start || blanktype == bl_both)
            कुंजी->skipsblanks = true;
          अगर (blanktype == bl_end || blanktype == bl_both)
            कुंजी->skipeblanks = true;
          अवरोध;
        हाल 'd':
          कुंजी->ignore = nondictionary;
          अवरोध;
        हाल 'f':
          कुंजी->translate = fold_toupper;
          अवरोध;
        हाल 'g':
          कुंजी->general_numeric = true;
          अवरोध;
        हाल 'h':
          कुंजी->human_numeric = true;
          अवरोध;
        हाल 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          अगर (! कुंजी->ignore)
            कुंजी->ignore = nonprinting;
          अवरोध;
        हाल 'M':
          कुंजी->month = true;
          अवरोध;
        हाल 'n':
          कुंजी->numeric = true;
          अवरोध;
        हाल 'R':
          कुंजी->random = true;
          अवरोध;
        हाल 'r':
          कुंजी->reverse = true;
          अवरोध;
        हाल 'V':
          कुंजी->version = true;
          अवरोध;
        शेष:
          वापस (अक्षर *) s;
        }
      ++s;
    }
  वापस (अक्षर *) s;
}

/* Initialize KEY.  */

अटल काष्ठा keyfield *
कुंजी_init (काष्ठा keyfield *कुंजी)
{
  स_रखो (कुंजी, 0, माप *कुंजी);
  कुंजी->eword = SIZE_MAX;
  वापस कुंजी;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  काष्ठा keyfield *कुंजी;
  काष्ठा keyfield कुंजी_buf;
  काष्ठा keyfield gkey;
  bool gkey_only = false;
  अक्षर स्थिर *s;
  पूर्णांक c = 0;
  अक्षर checkonly = 0;
  bool mergeonly = false;
  अक्षर *random_source = शून्य;
  bool need_random = false;
  माप_प्रकार nthreads = 0;
  माप_प्रकार nfiles = 0;
  bool posixly_correct = (दो_पर्या ("POSIXLY_CORRECT") != शून्य);
  पूर्णांक posix_ver = posix2_version ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  अक्षर **files;
  अक्षर *files_from = शून्य;
  काष्ठा Tokens tok;
  अक्षर स्थिर *outfile = शून्य;
  bool locale_ok;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  locale_ok = !! रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (SORT_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#अगर HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#पूर्ण_अगर

  /* Get locale's representation of the decimal point.  */
  {
    काष्ठा स्था_बदल स्थिर *locale = क्षेत्र_बदलो ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_point = locale->decimal_point[0];
    अगर (! decimal_point || locale->decimal_point[1])
      decimal_point = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    अगर (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    अगर (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    क्रमागत { nsigs = countof (term_sig) };

    काष्ठा sigaction act;

    sigemptyset (&caught_signals);
    क्रम (माप_प्रकार i = 0; i < nsigs; i++)
      {
        अगर (term_sig[i] == SIGPIPE)
          जारी;  /* Handled below.  */
        sigaction (term_sig[i], शून्य, &act);
        अगर (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    क्रम (माप_प्रकार i = 0; i < nsigs; i++)
      {
        अगर (term_sig[i] == SIGPIPE)
          जारी;  /* Handled below.  */
        अगर (sigismember (&caught_signals, term_sig[i]))
          sigaction (term_sig[i], &act, शून्य);
      }
  }
  संकेत (SIGCHLD, संक_पूर्व); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  निकास_पर (exit_cleanup);

  कुंजी_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, माप *files);

  जबतक (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      पूर्णांक oi = -1;

      अगर (c == -1
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
          अगर (argc <= optind)
            अवरोध;
          files[nfiles++] = argv[optind++];
        }
      अन्यथा चयन (c)
        {
        हाल 1:
          कुंजी = शून्य;
          अगर (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              अगर (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  कुंजी = कुंजी_init (&कुंजी_buf);
                  s = parse_field_गिनती (optarg + 1, &कुंजी->sword, शून्य);
                  अगर (s && *s == '.')
                    s = parse_field_गिनती (s + 1, &कुंजी->schar, शून्य);
                  अगर (! (कुंजी->sword || कुंजी->schar))
                    कुंजी->sword = SIZE_MAX;
                  अगर (! s || *set_ordering (s, कुंजी, bl_start))
                    कुंजी = शून्य;
                  अन्यथा
                    {
                      अगर (minus_pos_usage)
                        {
                          अक्षर स्थिर *optarg1 = argv[optind++];
                          s = parse_field_गिनती (optarg1 + 1, &कुंजी->eword,
                                             N_("invalid number after '-'"));
                          अगर (*s == '.')
                            s = parse_field_गिनती (s + 1, &कुंजी->echar,
                                               N_("invalid number after '.'"));
                          अगर (!कुंजी->echar && कुंजी->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              कुंजी->eword--;
                            }
                          अगर (*set_ordering (s, कुंजी, bl_end))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      कुंजी->traditional_used = true;
                      insertkey (कुंजी);
                    }
                }
            }
          अगर (! कुंजी)
            files[nfiles++] = optarg;
          अवरोध;

        हाल SORT_OPTION:
          c = XARGMATCH ("--sort", optarg, sort_args, sort_types);
          FALLTHROUGH;
        हाल 'b':
        हाल 'd':
        हाल 'f':
        हाल 'g':
        हाल 'h':
        हाल 'i':
        हाल 'M':
        हाल 'n':
        हाल 'r':
        हाल 'R':
        हाल 'V':
          {
            अक्षर str[2];
            str[0] = c;
            str[1] = '\0';
            set_ordering (str, &gkey, bl_both);
          }
          अवरोध;

        हाल CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_args, check_types)
               : 'c');
          FALLTHROUGH;
        हाल 'c':
        हाल 'C':
          अगर (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          अवरोध;

        हाल COMPRESS_PROGRAM_OPTION:
          अगर (compress_program && !streq (compress_program, optarg))
            error (SORT_FAILURE, 0, _("multiple compress programs specified"));
          compress_program = optarg;
          अवरोध;

        हाल DEBUG_PROGRAM_OPTION:
          debug = true;
          अवरोध;

        हाल FILES0_FROM_OPTION:
          files_from = optarg;
          अवरोध;

        हाल 'k':
          कुंजी = कुंजी_init (&कुंजी_buf);

          /* Get POS1. */
          s = parse_field_गिनती (optarg, &कुंजी->sword,
                                 N_("invalid number at field start"));
          अगर (! कुंजी->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          अगर (*s == '.')
            {
              s = parse_field_गिनती (s + 1, &कुंजी->schar,
                                     N_("invalid number after '.'"));
              अगर (! कुंजी->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          अगर (! (कुंजी->sword || कुंजी->schar))
            कुंजी->sword = SIZE_MAX;
          s = set_ordering (s, कुंजी, bl_start);
          अगर (*s != ',')
            {
              कुंजी->eword = SIZE_MAX;
              कुंजी->echar = 0;
            }
          अन्यथा
            {
              /* Get POS2. */
              s = parse_field_गिनती (s + 1, &कुंजी->eword,
                                     N_("invalid number after ','"));
              अगर (! कुंजी->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              अगर (*s == '.')
                {
                  s = parse_field_गिनती (s + 1, &कुंजी->echar,
                                         N_("invalid number after '.'"));
                }
              s = set_ordering (s, कुंजी, bl_end);
            }
          अगर (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (कुंजी);
          अवरोध;

        हाल 'm':
          mergeonly = true;
          अवरोध;

        हाल NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          अवरोध;

        हाल 'o':
          अगर (outfile && !streq (outfile, optarg))
            error (SORT_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          अवरोध;

        हाल RANDOM_SOURCE_OPTION:
          अगर (random_source && !streq (random_source, optarg))
            error (SORT_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          अवरोध;

        हाल 's':
          stable = true;
          अवरोध;

        हाल 'S':
          specify_sort_आकार (oi, c, optarg);
          अवरोध;

        हाल 't':
          {
            अक्षर newtab = optarg[0];
            अगर (! newtab)
              error (SORT_FAILURE, 0, _("empty tab"));
            अगर (optarg[1])
              {
                अगर (streq (optarg, "\\0"))
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
            अगर (tab != TAB_DEFAULT && tab != newtab)
              error (SORT_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          अवरोध;

        हाल 'T':
          add_अस्थायी_dir (optarg);
          अवरोध;

        हाल PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          अवरोध;

        हाल 'u':
          unique = true;
          अवरोध;

        हाल 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          अगर (optarg == argv[optind - 1])
            {
              अक्षर स्थिर *p;
              क्रम (p = optarg; c_isdigit (*p); p++)
                जारी;
              optind -= (*p != '\0');
            }
          अवरोध;

        हाल 'z':
          eolchar = 0;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (SORT_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (संकेत (SIGPIPE, संक_छोड़ो) == संक_पूर्व);

  अगर (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      अगर (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          ख_लिखो (मानक_त्रुटि, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (SORT_FAILURE);
        }

      खाता *stream = xfopen (files_from, "r");

      readtokens0_init (&tok);

      अगर (! readtokens0 (stream, &tok))
        error (SORT_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (stream, files_from);

      अगर (tok.n_tok)
        {
          मुक्त (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          क्रम (माप_प्रकार i = 0; i < nfiles; i++)
            {
              अगर (streq (files[i], "-"))
                error (SORT_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              अन्यथा अगर (files[i][0] == '\0')
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
  क्रम (कुंजी = keylist; कुंजी; कुंजी = कुंजी->अग्र)
    {
      अगर (default_कुंजी_compare (कुंजी) && !कुंजी->reverse)
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

  अगर (!keylist && !default_कुंजी_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  अगर (debug)
    {
      अगर (checkonly || outfile)
        {
          अटल अक्षर opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      अगर (locale_ok)
        locale_ok = !! रखो_क्षेत्र (LC_COLLATE, "");
      अगर (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      अगर (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (रखो_क्षेत्र (LC_COLLATE, शून्य)));
      अन्यथा
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      कुंजी_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  अगर (need_random)
    random_md5_state_init (random_source);

  अगर (अस्थायी_dirs.n_dirs == 0)
    {
      अक्षर स्थिर *tmp_dir = दो_पर्या ("TMPDIR");
      add_अस्थायी_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  अगर (nfiles == 0)
    {
      nfiles = 1;
      मुक्त (files);
      files = xmalloc (माप *files);
      *files = (अक्षर *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  अगर (0 < sort_आकार)
    sort_आकार = MAX (sort_आकार, MIN_SORT_आकार);

  अगर (checkonly)
    {
      अगर (nfiles > 1)
        error (SORT_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      अगर (outfile)
        {
          अटल अक्षर opts[] = {0, 'o', 0};
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

  अगर (mergeonly)
    {
      काष्ठा sortfile *sortfiles = xcalloc (nfiles, माप *sortfiles);

      क्रम (माप_प्रकार i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  अन्यथा
    {
      अगर (!nthreads)
        {
          अचिन्हित दीर्घ पूर्णांक np = अंक_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_THREADS);
        }

      /* Avoid integer overflow later.  */
      माप_प्रकार nthreads_max = SIZE_MAX / (2 * माप (काष्ठा merge_गाँठ));
      nthreads = MIN (nthreads, nthreads_max);

      sort (files, nfiles, outfile, nthreads);
    }

  अगर (have_read_stdin && ख_बंद (मानक_निवेश) == खातापूर्ण)
    sort_die (_("close failed"), "-");

  main_exit (निकास_सफल);
}
