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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <pthread.h>
#అన్తర్భావ <sys/resource.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <spawn.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "filevercmp.h"
#అన్తర్భావ "findprog.h"
#అన్తర్భావ "flexmember.h"
#అన్తర్భావ "hard-locale.h"
#అన్తర్భావ "hash.h"
#అన్తర్భావ "heap.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "mbswidth.h"
#అన్తర్భావ "nproc.h"
#అన్తర్భావ "physmem.h"
#అన్తర్భావ "posixver.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "randread.h"
#అన్తర్భావ "readtokens0.h"
#అన్తర్భావ "stdlib--.h"
#అన్తర్భావ "strnumcmp.h"
#అన్తర్భావ "term-sig.h"
#అన్తర్భావ "unistd--.h"
#అన్తర్భావ "xmemcoll.h"
#అన్తర్భావ "xnanosleep.h"
#అన్తర్భావ "xstrtol.h"
#అన్తర్భావ "xstrtol-error.h"

#నిర్వచితం_కాకపోతే RLIMIT_దత్తాంశం
నిర్మాణం rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#అయితే_ముగింపు

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "sort"

#నిర్వచించు AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ఒకవేళ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#అయితే_ముగింపు

#ఒకవేళ !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#అయితే_ముగింపు
#ఒకవేళ !defined OPEN_MAX
# define OPEN_MAX 20
#అయితే_ముగింపు

#నిర్వచించు UCHAR_LIM (UCHAR_MAX + 1)

#నిర్వచితం_కాకపోతే DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#అయితే_ముగింపు

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#నిర్వచించు MAX_విలీనం(మొత్తం, level) (((మొత్తం) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
గణన { SUBTHREAD_పంక్తులు_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_పంక్తులు_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
గణన { DEFAULT_MAX_దారాలు = 8 };

/* Exit statuses.  */
గణన
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    క్రమీకరించు_బహిర్గతం_యొక్క_క్రమం = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    క్రమీకరించు_FAILURE = 2
  };

గణన
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

గణన
  {
    /* Level of the end-of-merge node, one level above the root. */
    విలీనం_చివర = 0,

    /* Level of the root node in merge tree. */
    విలీనం_మూలం = 1
  };

/* The representation of the decimal point in the current locale.  */
స్థిర అక్షరం decimal_బిందువు;

/* Thousands separator; if outside char range, there is no separator.  */
స్థిర పూర్ణాంకం thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
స్థిర bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
స్థిర bool hard_LC_COLLATE;
#ఒకవేళ HAVE_NL_LANGINFO
స్థిర bool hard_LC_సమయం;
#అయితే_ముగింపు

#నిర్వచించు NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
గణన blanktype { bl_ప్రారంభం, bl_చివర, bl_both };

/* The character marking end of line. Default to \n. */
స్థిర అక్షరం eolchar = '\n';

/* Lines are held in memory as counted strings. */
నిర్మాణం పంక్తి
{
  అక్షరం *పాఠ్యం;			/* Text of the line. */
  size_t నిడివి;		/* Length including final newline. */
  అక్షరం *keybeg;			/* Start of first key. */
  అక్షరం *keylim;			/* Limit of first key. */
};

/* Input buffers. */
నిర్మాణం బఫరు
{
  అక్షరం *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t వాడిన;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ఎడమ;			/* Number of bytes left from previous reads. */
  size_t పంక్తి_బైట్లు;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
నిర్మాణం క్రమీకరించు_బఫరు_policy
{
  size_t initial;
  size_t హద్దు;
  bool growth_failed;
};

/* Sort key.  */
నిర్మాణం keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool స్థిరరాశి *ignore;		/* Boolean array of characters to ignore. */
  అక్షరం స్థిరరాశి *translate;	/* Translation applied to characters. */
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
  bool తిరగేయి;			/* Reverse the sense of comparison. */
  bool సంస్కరణ_3;			/* sort by version number */
  bool traditional_వాడిన;	/* Traditional key option format is used. */
  నిర్మాణం keyfield *తదుపరి;	/* Next keyfield to try. */
};

నిర్మాణం month
{
  అక్షరం స్థిరరాశి *name;
  పూర్ణాంకం val;
};

/* Binary merge tree node. */
నిర్మాణం విలీనం_కణుపు
{
  నిర్మాణం పంక్తి *lo;              /* Lines to merge from LO child node. */
  నిర్మాణం పంక్తి *hi;              /* Lines to merge from HI child node. */
  నిర్మాణం పంక్తి *చివర_lo;          /* End of available lines from LO. */
  నిర్మాణం పంక్తి *చివర_hi;          /* End of available lines from HI. */
  నిర్మాణం పంక్తి **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  నిర్మాణం విలీనం_కణుపు *మాతృ;    /* Parent node. */
  నిర్మాణం విలీనం_కణుపు *lo_బిడ్డ;  /* LO child node. */
  నిర్మాణం విలీనం_కణుపు *hi_బిడ్డ;  /* HI child node. */
  గుర్తులేని పూర్ణాంకం level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t తాళం;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
నిర్మాణం విలీనం_కణుపు_వరుస
{
  నిర్మాణం కుప్ప *ప్రాధాన్యత_వరుస;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
స్థిర నిర్మాణం పంక్తి saved_పంక్తి;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
స్థిర bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
స్థిర bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
స్థిర bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
స్థిర అక్షరం fold_toupper[UCHAR_LIM];

#నిర్వచించు MONTHS_చొప్పున_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
స్థిర నిర్మాణం month monthtab[] =
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
#నిర్వచించు NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#నిర్వచించు MIN_విలీనం_బఫరు_కొలత (2 + పరిమాణం (నిర్మాణం పంక్తి))

/* Minimum sort size; the code might not work with smaller sizes.  */
#నిర్వచించు MIN_క్రమీకరించు_కొలత (nmerge * MIN_విలీనం_బఫరు_కొలత)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
స్థిర size_t విలీనం_బఫరు_కొలత = MAX (MIN_విలీనం_బఫరు_కొలత, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
స్థిర size_t క్రమీకరించు_కొలత;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#నిర్వచించు ఆదానం_FILE_కొలత_GUESS (128 * 1024)

స్థిర నిర్మాణం
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  అక్షరం స్థిరరాశి **dirs COUNTED_BY (n_dirs);
} తాత్కాలిక_dirs;



/* Flag to reverse the order of all comparisons. */
స్థిర bool తిరగేయి;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
స్థిర bool stable;

/* An int value outside char range.  */
గణన { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
గణన { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
స్థిర పూర్ణాంకం tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
స్థిర bool unique;

/* Nonzero if any of the input files are the standard input. */
స్థిర bool have_read_stdin;

/* List of key field comparisons to be tried.  */
స్థిర నిర్మాణం keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
స్థిర అక్షరం స్థిరరాశి *compress_కార్యక్రమం;

/* Annotate the output with extra info to aid the user.  */
స్థిర bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
స్థిర గుర్తులేని పూర్ణాంకం nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
స్థిర bool default_SIGPIPE;

/* The list of temporary files. */
నిర్మాణం tempnode
{
  నిర్మాణం tempnode *అస్థిర తదుపరి;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  అక్షరం state;
  అక్షరం name[FLEXIBLE_ARRAY_MEMBER];
};
స్థిర నిర్మాణం tempnode *అస్థిర temphead;
స్థిర నిర్మాణం tempnode *అస్థిర *temptail = &temphead;

/* Clean up any remaining temporary files.  */

స్థిర శూన్య
cleanup (శూన్య)
{
  కొరకు (నిర్మాణం tempnode స్థిరరాశి *కణుపు = temphead; కణుపు; కణుపు = కణుపు->తదుపరి)
    unlink (కణుపు->name);
  temphead = ఖాళీ;
}

/* Handle interrupts and hangups. */

స్థిర శూన్య
sighandler (పూర్ణాంకం sig)
{
  cleanup ();

  సంకేతం (sig, SIG_DFL);
  లేపు (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

స్థిర శూన్య
క్రమీకరించు_die (అక్షరం స్థిరరాశి *సందేశం_2, అక్షరం స్థిరరాశి *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ఒకవేళ (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (క్రమీకరించు_FAILURE, errno, "%s: %s", సందేశం_2,
         quotef (file ? file : _("standard output")));
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Write sorted concatenation యొక్క all FILE(s) గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding క్రమీకరించు కీలకాలు లోపల each పంక్తి\n\
"));
      oputs (_("\
  -d, --dictionary-క్రమం\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-స్థితి\n\
         fold క్రింది స్థితి గమ్యం పై స్థితి characters\n\
"));
      oputs (_("\
  -g, --general-numeric-క్రమీకరించు\n\
         compare according గమ్యం general numerical విలువ\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-క్రమీకరించు\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-క్రమీకరించు\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-క్రమీకరించు\n\
         compare according గమ్యం string numerical విలువ;\n\
         see నిండిన documentation కొరకు supported అక్షరశ్రేణులు\n\
"));
      oputs (_("\
  -R, --random-క్రమీకరించు\n\
         shuffle, but సమూహం identical కీలకాలు.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-ఆధారం_2=FILE\n\
         get random బైట్లు from FILE\n\
"));
      oputs (_("\
  -r, --తిరగేయి\n\
         తిరగేయి the ఫలితం యొక్క comparisons\n\
"));
      oputs (_("\
      --క్రమీకరించు=మాట\n\
         క్రమీకరించు according గమ్యం మాట:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, సంస్కరణ_3 -V\n\
"));
      oputs (_("\
  -V, --సంస్కరణ_3-క్రమీకరించు\n\
         natural క్రమీకరించు యొక్క (సంస్కరణ_3) numbers within పాఠ్యం\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-కొలత=NMERGE\n\
         విలీనం at most NMERGE inputs at once; కొరకు మరిన్ని use తాత్కాలిక files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-మొదటి\n\
         check కొరకు sorted ఆదానం; చేయి not క్రమీకరించు\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but చేయి not report మొదటి bad పంక్తి\n\
"));
      oputs (_("\
      --compress-కార్యక్రమం=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part యొక్క the పంక్తి వాడిన గమ్యం క్రమీకరించు,\n\
         and warn about questionable usage గమ్యం standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         పఠ ఆదానం from the files specified by NUL-terminated పేర్లు లోపల file F;\
\n\
         If F is -, పఠ పేర్లు from standard ఆదానం\n\
"));
      oputs (_("\
  -k, --కీలకం=KEYDEF\n\
         క్రమీకరించు ద్వారా a కీలకం; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --విలీనం\n\
         విలీనం already sorted files; చేయి not క్రమీకరించు\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         లిఖ ఫలితం గమ్యం FILE instead యొక్క standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize క్రమీకరించు by disabling చివరి-resort comparison\n\
"));
      oputs (_("\
  -S, --బఫరు-కొలత=కొలత_2\n\
         use కొలత_2 కొరకు ప్రధాన జ్ఞాపకం బఫరు\n\
"));
      oputs (_("\
  -t, --క్షేత్రం-separator=SEP\n\
         use SEP instead యొక్క non-blank గమ్యం blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR కొరకు temporaries, not $TMPDIR or %s;\n\
         గుణిజం options specify గుణిజం directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the సంఖ్య యొక్క sorts నిరంతరం_2 concurrently గమ్యం N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the మొదటి యొక్క పంక్తులు with equal కీలకాలు;\n\
         with -c, check కొరకు strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] కొరకు ప్రారంభం and stop position, where F is a\n\
క్షేత్రం సంఖ్య and C a character position లోపల the క్షేత్రం; both are origin 1, and\n\
the stop position defaults గమ్యం the పంక్తి's చివర.  If neither -t nor -b is లోపల\n\
effect, characters లోపల a క్షేత్రం are counted from the beginning యొక్క the preceding\n\
whitespace.  OPTS is one or మరిన్ని single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options కొరకు that కీలకం.  If no కీలకం is given, use\n\
the entire పంక్తి as the కీలకం.  Use --debug గమ్యం diagnose incorrect కీలకం usage.\n\
\n\
కొలత_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% యొక్క జ్ఞాపకం, b 1, K 1024 (అప్రమేయ), and so on కొరకు M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects క్రమీకరించు క్రమం.\n\
సమితి_3 LC_ALL=C గమ్యం get the traditional క్రమీకరించు క్రమం that ఉపయోగిస్తుంది\n\
native byte విలువలు.\n\
"), stdout );
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_కార్యక్రమం_OPTION,
  DEBUG_కార్యక్రమం_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_ఆధారం_OPTION,
  క్రమీకరించు_OPTION,
  PARALLEL_OPTION
};

స్థిర అక్షరం స్థిరరాశి short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"ignore-leading-blanks", no_వాదం, ఖాళీ, 'b'},
  {"check", optional_వాదం, ఖాళీ, CHECK_OPTION},
  {"compress-program", required_వాదం, ఖాళీ, COMPRESS_కార్యక్రమం_OPTION},
  {"debug", no_వాదం, ఖాళీ, DEBUG_కార్యక్రమం_OPTION},
  {"dictionary-order", no_వాదం, ఖాళీ, 'd'},
  {"ignore-case", no_వాదం, ఖాళీ, 'f'},
  {"files0-from", required_వాదం, ఖాళీ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_వాదం, ఖాళీ, 'g'},
  {"ignore-nonprinting", no_వాదం, ఖాళీ, 'i'},
  {"key", required_వాదం, ఖాళీ, 'k'},
  {"merge", no_వాదం, ఖాళీ, 'm'},
  {"month-sort", no_వాదం, ఖాళీ, 'M'},
  {"numeric-sort", no_వాదం, ఖాళీ, 'n'},
  {"human-numeric-sort", no_వాదం, ఖాళీ, 'h'},
  {"version-sort", no_వాదం, ఖాళీ, 'V'},
  {"random-sort", no_వాదం, ఖాళీ, 'R'},
  {"random-source", required_వాదం, ఖాళీ, RANDOM_ఆధారం_OPTION},
  {"sort", required_వాదం, ఖాళీ, క్రమీకరించు_OPTION},
  {"output", required_వాదం, ఖాళీ, 'o'},
  {"reverse", no_వాదం, ఖాళీ, 'r'},
  {"stable", no_వాదం, ఖాళీ, 's'},
  {"batch-size", required_వాదం, ఖాళీ, NMERGE_OPTION},
  {"buffer-size", required_వాదం, ఖాళీ, 'S'},
  {"field-separator", required_వాదం, ఖాళీ, 't'},
  {"temporary-directory", required_వాదం, ఖాళీ, 'T'},
  {"unique", no_వాదం, ఖాళీ, 'u'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {"parallel", required_వాదం, ఖాళీ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0},
};

#నిర్వచించు CHECK_పట్టిక \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి check_వాదనలు[] =
{
#నిర్వచించు _ct_(_s, _c) _s,
  CHECK_పట్టిక ఖాళీ
#నిర్వచనం_రద్దు  _ct_
};
స్థిర అక్షరం స్థిరరాశి check_types[] =
{
#నిర్వచించు _ct_(_s, _c) _c,
  CHECK_పట్టిక
#నిర్వచనం_రద్దు  _ct_
};

#నిర్వచించు క్రమీకరించు_పట్టిక \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి క్రమీకరించు_వాదనలు[] =
{
#నిర్వచించు _st_(_s, _c) _s,
  క్రమీకరించు_పట్టిక ఖాళీ
#నిర్వచనం_రద్దు  _st_
};
స్థిర అక్షరం స్థిరరాశి క్రమీకరించు_types[] =
{
#నిర్వచించు _st_(_s, _c) _c,
  క్రమీకరించు_పట్టిక
#నిర్వచనం_రద్దు  _st_
};

/* The set of signals that are caught.  */
స్థిర sigset_t caught_సంకేతాలు;

/* Critical section status.  */
నిర్మాణం cs_స్థితి
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
స్థిర శూన్య
cs_enter (నిర్మాణం cs_స్థితి *స్థితి_2)
{
  పూర్ణాంకం ret = pthread_sigmask (SIG_BLOCK, &caught_సంకేతాలు, &స్థితి_2->sigs);
  స్థితి_2->valid = ret == 0;
}

/* Leave a critical section.  */
స్థిర శూన్య
cs_leave (నిర్మాణం cs_స్థితి స్థిరరాశి *స్థితి_2)
{
  ఒకవేళ (స్థితి_2->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &స్థితి_2->sigs, ఖాళీ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
గణన { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
నిర్మాణం sortfile
{
  /* The file's name.  */
  అక్షరం స్థిరరాశి *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  నిర్మాణం tempnode *తాత్కాలిక;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
స్థిర Hash_పట్టిక *proctab;

గణన { INIT_PROCTAB_కొలత = 47 };

స్థిర size_t
proctab_hasher (శూన్య స్థిరరాశి *నమోదు_2, size_t tabsize)
{
  నిర్మాణం tempnode స్థిరరాశి *కణుపు = నమోదు_2;
  తిరిగి కణుపు->pid % tabsize;
}

స్థిర bool
proctab_comparator (శూన్య స్థిరరాశి *e1, శూన్య స్థిరరాశి *e2)
{
  నిర్మాణం tempnode స్థిరరాశి *n1 = e1;
  నిర్మాణం tempnode స్థిరరాశి *n2 = e2;
  తిరిగి n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
స్థిర pid_t nprocs;

స్థిర bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

స్థిర pid_t
reap (pid_t pid)
{
  పూర్ణాంకం స్థితి_2;
  pid_t cpid = waitpid ((pid ? pid : -1), &స్థితి_2, (pid ? 0 : WNOHANG));

  ఒకవేళ (cpid < 0)
    error (క్రమీకరించు_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_కార్యక్రమం));
  లేకపోతే ఒకవేళ (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ఒకవేళ (! WIFEXITED (స్థితి_2) || WEXITSTATUS (స్థితి_2))
        error (క్రమీకరించు_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_కార్యక్రమం));
      --nprocs;
    }

  తిరిగి cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

స్థిర శూన్య
రిజిస్టరు_proc (నిర్మాణం tempnode *తాత్కాలిక)
{
  ఒకవేళ (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_కొలత, ఖాళీ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ఖాళీ);
      ఒకవేళ (! proctab)
        xalloc_die ();
    }

  తాత్కాలిక->state = UNREAPED;

  ఒకవేళ (! hash_చొప్పించు (proctab, తాత్కాలిక))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

స్థిర bool
delete_proc (pid_t pid)
{
  నిర్మాణం tempnode test;

  test.pid = pid;
  నిర్మాణం tempnode *కణుపు = hash_remove (proctab, &test);
  ఒకవేళ (! కణుపు)
    తిరిగి false;
  కణుపు->state = REAPED;
  తిరిగి true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

స్థిర శూన్య
wait_proc (pid_t pid)
{
  ఒకవేళ (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

స్థిర శూన్య
reap_exited (శూన్య)
{
  అయితే (0 < nprocs && reap (0))
    కొనసాగు;
}

/* Reap at least one exited child, waiting if necessary.  */

స్థిర శూన్య
reap_some (శూన్య)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

స్థిర శూన్య
reap_all (శూన్య)
{
  అయితే (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

స్థిర శూన్య
exit_cleanup (శూన్య)
{
  ఒకవేళ (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      నిర్మాణం cs_స్థితి cs;
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

స్థిర నిర్మాణం tempnode *
create_తాత్కాలిక_file (పూర్ణాంకం *pfd, bool survive_fd_exhaustion)
{
  స్థిర అక్షరం స్థిరరాశి slashbase[] = "/sortXXXXXX";
  స్థిర idx_t తాత్కాలిక_dir_సూచీ;
  పూర్ణాంకం fd;
  పూర్ణాంకం saved_errno;
  అక్షరం స్థిరరాశి *తాత్కాలిక_dir = తాత్కాలిక_dirs.dirs[తాత్కాలిక_dir_సూచీ];
  size_t len = అక్షరశ్రేణి_పొడవు (తాత్కాలిక_dir);
  నిర్మాణం tempnode *కణుపు =
    xmalloc (FLEXSIZEOF (నిర్మాణం tempnode, name, len + పరిమాణం slashbase));
  అక్షరం *file = కణుపు->name;
  నిర్మాణం cs_స్థితి cs;

  జ్ఞాపకం_నకలు (file, తాత్కాలిక_dir, len);
  జ్ఞాపకం_నకలు (file + len, slashbase, పరిమాణం slashbase);
  కణుపు->తదుపరి = ఖాళీ;
  ఒకవేళ (++తాత్కాలిక_dir_సూచీ == తాత్కాలిక_dirs.n_dirs)
    తాత్కాలిక_dir_సూచీ = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ఒకవేళ (0 <= fd)
    {
      *temptail = కణుపు;
      temptail = &కణుపు->తదుపరి;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ఒకవేళ (fd < 0)
    {
      ఒకవేళ (! (survive_fd_exhaustion && errno == EMFILE))
        error (క్రమీకరించు_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (తాత్కాలిక_dir));
      విడుదల (కణుపు);
      కణుపు = ఖాళీ;
    }

  *pfd = fd;
  తిరిగి కణుపు;
}

/* Return a pointer to stdout status, or NULL on failure.  */

స్థిర నిర్మాణం stat *
get_outstatus (శూన్య)
{
  స్థిర పూర్ణాంకం outstat_errno;
  స్థిర నిర్మాణం stat outstat;
  ఒకవేళ (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  తిరిగి outstat_errno < 0 ? &outstat : ఖాళీ;
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

స్థిర FILE *
ప్రవాహం_open (అక్షరం స్థిరరాశి *file, అక్షరం స్థిరరాశి *how)
{
  FILE *fp;

  ఒకవేళ (*how == 'r')
    {
      ఒకవేళ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      లేకపోతే
        {
          పూర్ణాంకం fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ఖాళీ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  లేకపోతే ఒకవేళ (*how == 'w')
    {
      ఒకవేళ (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          పూర్ణాంకం ftruncate_errno = errno;
          నిర్మాణం stat *outst = get_outstatus ();
          ఒకవేళ (!outst || S_ISREG (outst->st_బహుళకం) || S_TYPEISSHM (outst))
            error (క్రమీకరించు_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  లేకపోతే
    affirm (!"unexpected mode passed to stream_open");

  తిరిగి fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

స్థిర FILE *
xfopen (అక్షరం స్థిరరాశి *file, అక్షరం స్థిరరాశి *how)
{
  FILE *fp = ప్రవాహం_open (file, how);
  ఒకవేళ (!fp)
    క్రమీకరించు_die (_("open failed"), file);
  తిరిగి fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

స్థిర శూన్య
xfclose (FILE *fp, అక్షరం స్థిరరాశి *file)
{
  ఎంపిక (fileno (fp))
    {
    స్థితి STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ఆపు;

    స్థితి STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ఒకవేళ (fflush (fp) != 0)
        క్రమీకరించు_die (_("fflush failed"), file);
      ఆపు;

    అప్రమేయ:
      ఒకవేళ (fclose (fp) != 0)
        క్రమీకరించు_die (_("close failed"), file);
      ఆపు;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

స్థిర శూన్య
move_fd (పూర్ణాంకం oldfd, పూర్ణాంకం newfd)
{
  ఒకవేళ (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_విలువ (dup2 (oldfd, newfd));
      ignore_విలువ (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

స్థిర పూర్ణాంకం
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  పూర్ణాంకం oldfd, పూర్ణాంకం newfd)
{
  పూర్ణాంకం ఫలితం = 0;
  ఒకవేళ (oldfd != newfd)
    {
      ఫలితం = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ఒకవేళ (ఫలితం == 0)
        ఫలితం = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  తిరిగి ఫలితం;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

స్థిర అక్షరం స్థిరరాశి *
get_resolved_compress_కార్యక్రమం (శూన్య)
{
  /* Use a cache, to perform the search only once.  */
  స్థిర అక్షరం స్థిరరాశి *resolved_compress_కార్యక్రమం_cache /* = NULL */;

  ఒకవేళ (resolved_compress_కార్యక్రమం_cache == ఖాళీ)
    {
      resolved_compress_కార్యక్రమం_cache =
        కనుగొను_లోపల_given_మార్గం (compress_కార్యక్రమం, getenv ("PATH"), ఖాళీ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  తిరిగి resolved_compress_కార్యక్రమం_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

స్థిర పూర్ణాంకం
pipe_బిడ్డ (pid_t *pid, పూర్ణాంకం pipefds[2], పూర్ణాంకం tempfd, bool decompress,
            size_t tries)
{
  అక్షరం స్థిరరాశి *resolved_compress_కార్యక్రమం;
  నిర్మాణం tempnode *saved_temphead;
  ద్విగుణ wait_retry = 0.25;
  నిర్మాణం cs_స్థితి cs;
  పూర్ణాంకం ఫలితం;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_కార్యక్రమం = get_resolved_compress_కార్యక్రమం ();
  ఒకవేళ (resolved_compress_కార్యక్రమం == ఖాళీ)
    తిరిగి errno;

  ఒకవేళ ((ఫలితం = posix_spawnattr_init (&attr)))
    తిరిగి ఫలితం;
  ఒకవేళ ((ఫలితం = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (ఫలితం = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      తిరిగి ఫలితం;
    }

  ఒకవేళ (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      పూర్ణాంకం saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      తిరిగి saved_errno;
    }

  ఒకవేళ ((ఫలితం = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (ఫలితం = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((ఫలితం = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (ఫలితం = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (ఫలితం = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((ఫలితం = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (ఫలితం = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (ఫలితం = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      తిరిగి ఫలితం;
    }

  అక్షరం స్థిరరాశి *స్థిరరాశి argv[] =
    {
      resolved_compress_కార్యక్రమం,
      decompress ? "-d" : ఖాళీ,
      ఖాళీ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ఒకవేళ (nmerge + 1 < nprocs)
    reap_some ();

  అయితే (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ఖాళీ;

      ఫలితం = posix_spawnp (pid, resolved_compress_కార్యక్రమం, &actions, &attr,
                             (అక్షరం * స్థిరరాశి *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ఒకవేళ (ఫలితం != EAGAIN)
        ఆపు;
      లేకపోతే
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

  ఒకవేళ (ఫలితం)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  లేకపోతే
    ++nprocs;

  తిరిగి ఫలితం;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

స్థిర నిర్మాణం tempnode *
maybe_create_తాత్కాలిక (FILE **pfp, bool survive_fd_exhaustion)
{
  పూర్ణాంకం tempfd;
  నిర్మాణం tempnode *కణుపు = create_తాత్కాలిక_file (&tempfd, survive_fd_exhaustion);
  ఒకవేళ (! కణుపు)
    తిరిగి ఖాళీ;

  కణుపు->state = UNCOMPRESSED;

  ఒకవేళ (compress_కార్యక్రమం)
    {
      పూర్ణాంకం pipefds[2];
      స్థిర పూర్ణాంకం చివరి_ఫలితం = 0;

      పూర్ణాంకం ఫలితం = pipe_బిడ్డ (&కణుపు->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ఒకవేళ (ఫలితం)
        {
          ఒకవేళ (ఫలితం != చివరి_ఫలితం)
            error (0, ఫలితం, _("could not run compress program %s"),
                   quoteaf (compress_కార్యక్రమం));
        }
      లేకపోతే
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          రిజిస్టరు_proc (కణుపు);
        }

      చివరి_ఫలితం = ఫలితం;
    }

  *pfp = fdopen (tempfd, "w");
  ఒకవేళ (! *pfp)
    క్రమీకరించు_die (_("couldn't create temporary file"), కణుపు->name);

  తిరిగి కణుపు;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

స్థిర నిర్మాణం tempnode *
create_తాత్కాలిక (FILE **pfp)
{
  తిరిగి maybe_create_తాత్కాలిక (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

స్థిర FILE *
open_తాత్కాలిక (నిర్మాణం tempnode *తాత్కాలిక)
{
  పూర్ణాంకం tempfd, pipefds[2];
  FILE *fp = ఖాళీ;

  ఒకవేళ (తాత్కాలిక->state == UNREAPED)
    wait_proc (తాత్కాలిక->pid);

  tempfd = open (తాత్కాలిక->name, O_RDONLY);
  ఒకవేళ (tempfd < 0)
    తిరిగి ఖాళీ;

  pid_t బిడ్డ;
  పూర్ణాంకం ఫలితం = pipe_బిడ్డ (&బిడ్డ, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ఒకవేళ (ఫలితం)
    {
      ఒకవేళ (ఫలితం != EMFILE)
        error (క్రమీకరించు_FAILURE, ఫలితం, _("could not run compress program %s -d"),
               quoteaf (compress_కార్యక్రమం));
      close (tempfd);
      errno = EMFILE;
    }
  లేకపోతే
    {
      తాత్కాలిక->pid = బిడ్డ;
      రిజిస్టరు_proc (తాత్కాలిక);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ఒకవేళ (! fp)
        {
          పూర్ణాంకం saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  తిరిగి fp;
}

/* Append DIR to the array of temporary directory names.  */
స్థిర శూన్య
కూడు_తాత్కాలిక_dir (అక్షరం స్థిరరాశి *dir)
{
  ఒకవేళ (తాత్కాలిక_dirs.n_dirs == తాత్కాలిక_dirs.n_dirs_alloc)
    తాత్కాలిక_dirs.dirs = xpalloc (తాత్కాలిక_dirs.dirs, &తాత్కాలిక_dirs.n_dirs_alloc, 1, -1,
                              పరిమాణం *తాత్కాలిక_dirs.dirs);

  idx_t స్థిరరాశి dir_idx = తాత్కాలిక_dirs.n_dirs++;
  తాత్కాలిక_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

స్థిర శూన్య
zaptemp (అక్షరం స్థిరరాశి *name)
{
  నిర్మాణం tempnode *అస్థిర *pnode;
  నిర్మాణం tempnode *కణుపు;
  నిర్మాణం tempnode *తదుపరి;
  పూర్ణాంకం unlink_స్థితి;
  పూర్ణాంకం unlink_errno = 0;
  నిర్మాణం cs_స్థితి cs;

  కొరకు (pnode = &temphead; (కణుపు = *pnode)->name != name; pnode = &కణుపు->తదుపరి)
    కొనసాగు;

  ఒకవేళ (కణుపు->state == UNREAPED)
    wait_proc (కణుపు->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  తదుపరి = కణుపు->తదుపరి;
  cs_enter (&cs);
  unlink_స్థితి = unlink (name);
  unlink_errno = errno;
  *pnode = తదుపరి;
  cs_leave (&cs);

  ఒకవేళ (unlink_స్థితి != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ఒకవేళ (! తదుపరి)
    temptail = pnode;
  విడుదల (కణుపు);
}

#ఒకవేళ HAVE_NL_LANGINFO

స్థిర పూర్ణాంకం
struct_month_cmp (శూన్య స్థిరరాశి *m1, శూన్య స్థిరరాశి *m2)
{
  నిర్మాణం month స్థిరరాశి *month1 = m1;
  నిర్మాణం month స్థిరరాశి *month2 = m2;
  తిరిగి strcmp (month1->name, month2->name);
}

#అయితే_ముగింపు

/* Initialize the character class tables. */

స్థిర శూన్య
inittables (శూన్య)
{
  కొరకు (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! అక్షరమా_అంకెనా (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = పెద్ద_అక్షరం (i);
    }

#ఒకవేళ HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ఒకవేళ (hard_LC_సమయం)
    {
      కొరకు (size_t i = 0; i < MONTHS_చొప్పున_YEAR; i++)
        {
          అక్షరం స్థిరరాశి *s;
          size_t s_len;
          size_t j, k;
          అక్షరం *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = అక్షరశ్రేణి_పొడవు (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          కొరకు (j = k = 0; j < s_len; j++)
            ఒకవేళ (! isblank (గమ్యం_uchar (s[j])))
              name[k++] = fold_toupper[గమ్యం_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_చొప్పున_YEAR, పరిమాణం *monthtab, struct_month_cmp);
    }
#అయితే_ముగింపు
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
స్థిర శూన్య
specify_nmerge (పూర్ణాంకం oi, అక్షరం c, అక్షరం స్థిరరాశి *s)
{
  uintmax_t n;
  నిర్మాణం rlimit rlimit;
  గణన strtol_error e = xstrtoumax (s, ఖాళీ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  గుర్తులేని పూర్ణాంకం max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ఒకవేళ (e == LONGINT_OK)
    {
      nmerge = n;
      ఒకవేళ (nmerge != n)
        e = LONGINT_అధికప్రవాహం;
      లేకపోతే
        {
          ఒకవేళ (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (క్రమీకరించు_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          లేకపోతే ఒకవేళ (max_nmerge < nmerge)
            {
              e = LONGINT_అధికప్రవాహం;
            }
          లేకపోతే
            తిరిగి;
        }
    }

  ఒకవేళ (e == LONGINT_అధికప్రవాహం)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (క్రమీకరించు_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  లేకపోతే
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
స్థిర శూన్య
specify_క్రమీకరించు_కొలత (పూర్ణాంకం oi, అక్షరం c, అక్షరం స్థిరరాశి *s)
{
  uintmax_t n;
  అక్షరం *suffix;
  గణన strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ఒకవేళ (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_అధికప్రవాహం;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ఒకవేళ (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    ఎంపిక (suffix[0])
      {
      స్థితి 'b':
        e = LONGINT_OK;
        ఆపు;

      స్థితి '%':
        {
          ద్విగుణ mem = physmem_మొత్తం () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ఒకవేళ (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          లేకపోతే
            e = LONGINT_అధికప్రవాహం;
        }
        ఆపు;
      }

  ఒకవేళ (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ఒకవేళ (n < క్రమీకరించు_కొలత)
        తిరిగి;

      క్రమీకరించు_కొలత = n;
      ఒకవేళ (క్రమీకరించు_కొలత == n)
        {
          క్రమీకరించు_కొలత = MAX (క్రమీకరించు_కొలత, MIN_క్రమీకరించు_కొలత);
          తిరిగి;
        }

      e = LONGINT_అధికప్రవాహం;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
స్థిర size_t
specify_nthreads (పూర్ణాంకం oi, అక్షరం c, అక్షరం స్థిరరాశి *s)
{
  uintmax_t nthreads;
  గణన strtol_error e = xstrtoumax (s, ఖాళీ, 10, &nthreads, "");
  ఒకవేళ (e == LONGINT_అధికప్రవాహం)
    తిరిగి SIZE_MAX;
  ఒకవేళ (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ఒకవేళ (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ఒకవేళ (nthreads == 0)
    error (క్రమీకరించు_FAILURE, 0, _("number in parallel must be nonzero"));
  తిరిగి nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
స్థిర size_t
default_క్రమీకరించు_కొలత (శూన్య)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t కొలత = SIZE_MAX;
  నిర్మాణం rlimit rlimit;
  ఒకవేళ (getrlimit (RLIMIT_దత్తాంశం, &rlimit) == 0 && rlimit.rlim_cur < కొలత)
    కొలత = rlimit.rlim_cur;
#నిర్వచితమైతే RLIMIT_AS
  ఒకవేళ (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < కొలత)
    కొలత = rlimit.rlim_cur;
#అయితే_ముగింపు

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  కొలత /= 2;

#నిర్వచితమైతే RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ఒకవేళ (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < కొలత)
    కొలత = rlimit.rlim_cur / 16 * 15;
#అయితే_ముగింపు

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ద్విగుణ avail = physmem_అందుబాటు ();
  ద్విగుణ మొత్తం = physmem_మొత్తం ();
  ద్విగుణ mem = MAX (avail, మొత్తం / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ఒకవేళ (మొత్తం * 0.75 < కొలత)
    కొలత = మొత్తం * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ఒకవేళ (mem < కొలత)
    కొలత = mem;
  తిరిగి MAX (కొలత, MIN_క్రమీకరించు_కొలత);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

స్థిర bool
పంక్తి_aligned_కొలత (size_t *alloc)
{
  size_t కొలత = *alloc;
  size_t alignment = పరిమాణం (నిర్మాణం పంక్తి);
  size_t padding = alignment - కొలత % alignment;
  size_t aligned;

  ఒకవేళ (ckd_కూడు (&aligned, కొలత, padding))
    తిరిగి false;

  *alloc = aligned;
  తిరిగి true;
}

/* Return the default initial allocation for a growable sort buffer.  */

స్థిర size_t
default_initial_క్రమీకరించు_కొలత (size_t పంక్తి_బైట్లు)
{
  size_t కొలత = పంక్తి_బైట్లు + 2;
  size_t ఆదానం_కొలత;

  ఒకవేళ (ckd_mul (&ఆదానం_కొలత, ఆదానం_FILE_కొలత_GUESS, పంక్తి_బైట్లు + 1)
      || ckd_కూడు (&కొలత, కొలత, ఆదానం_కొలత))
    తిరిగి SIZE_MAX;

  తిరిగి MAX (కొలత, MIN_క్రమీకరించు_కొలత);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

స్థిర bool
ఆదానం_కొలత_బఫరు_బైట్లు (uintmax_t file_కొలత, size_t పంక్తి_బైట్లు,
                         size_t *worst_case)
{
  size_t worst_case_చొప్పున_ఆదానం_byte = పంక్తి_బైట్లు + 1;

  ఒకవేళ (SIZE_MAX / worst_case_చొప్పున_ఆదానం_byte < file_కొలత)
    తిరిగి false;

  size_t కొలత = file_కొలత * worst_case_చొప్పున_ఆదానం_byte;
  ఒకవేళ (SIZE_MAX - కొలత < 1)
    తిరిగి false;

  *worst_case = కొలత + 1;
  తిరిగి true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

స్థిర శూన్య
క్రమీకరించు_బఫరు_policy (FILE *స్థిరరాశి *fps, size_t nfps,
                    అక్షరం *స్థిరరాశి *files, size_t nfiles,
                    size_t పంక్తి_బైట్లు,
                    నిర్మాణం క్రమీకరించు_బఫరు_policy *policy)
{
  size_t కొలత_bound = క్రమీకరించు_కొలత ? క్రమీకరించు_కొలత : default_క్రమీకరించు_కొలత ();
  size_t initial_bound =
    క్రమీకరించు_కొలత ? కొలత_bound : default_initial_క్రమీకరించు_కొలత (పంక్తి_బైట్లు);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_చొప్పున_ఆదానం_byte = పంక్తి_బైట్లు + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t కొలత = worst_case_చొప్పున_ఆదానం_byte + 1;

  initial_bound = MIN (initial_bound, కొలత_bound);
  policy->హద్దు = కొలత_bound;
  policy->growth_failed = false;

  కొరకు (size_t i = 0; i < nfiles; i++)
    {
      నిర్మాణం stat st;
      off_t file_కొలత;
      size_t worst_case;
      bool తెలిసిన_కొలత;

      ఒకవేళ (కొలత_bound <= కొలత)
        {
          policy->initial = కొలత_bound;
          తిరిగి;
        }

      ఒకవేళ ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        క్రమీకరించు_die (_("stat failed"), files[i]);

      తెలిసిన_కొలత = usable_st_కొలత (&st) && 0 < st.st_కొలత;
      ఒకవేళ (తెలిసిన_కొలత)
        file_కొలత = st.st_కొలత;
      లేకపోతే
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ఒకవేళ (క్రమీకరించు_కొలత)
            {
              policy->initial = కొలత_bound;
              తిరిగి;
            }
          file_కొలత = ఆదానం_FILE_కొలత_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ఒకవేళ (! ఆదానం_కొలత_బఫరు_బైట్లు (file_కొలత, పంక్తి_బైట్లు, &worst_case)
          || కొలత_bound - కొలత <= worst_case)
        {
          policy->initial = కొలత_bound;
          తిరిగి;
        }

      ఒకవేళ (! తెలిసిన_కొలత && ! క్రమీకరించు_కొలత
          && (initial_bound <= కొలత || initial_bound - కొలత <= worst_case))
        {
          policy->initial = MAX (కొలత, initial_bound);
          తిరిగి;
        }

      కొలత += worst_case;
    }

  policy->initial = MAX (కొలత, MIN_క్రమీకరించు_కొలత);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

స్థిర శూన్య
initbuf (నిర్మాణం బఫరు *buf, size_t పంక్తి_బైట్లు, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  అయితే (true)
    {
      ఒకవేళ (! పంక్తి_aligned_కొలత (&alloc))
        buf->buf = ఖాళీ;
      లేకపోతే
        buf->buf = జ్ఞాపకం_ఇవ్వు (alloc);
      ఒకవేళ (buf->buf)
        ఆపు;
      alloc /= 2;
      ఒకవేళ (alloc <= పంక్తి_బైట్లు + 1)
        xalloc_die ();
    }

  buf->పంక్తి_బైట్లు = పంక్తి_బైట్లు;
  buf->alloc = alloc;
  buf->వాడిన = buf->ఎడమ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

స్థిర అంతఃవరుస నిర్మాణం పంక్తి *
బఫరు_linelim (నిర్మాణం బఫరు స్థిరరాశి *buf)
{
  శూన్య *linelim = buf->buf + buf->alloc;
  తిరిగి linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

స్థిర bool
try_growbuf (నిర్మాణం బఫరు *buf, size_t alloc)
{
  ఒకవేళ (! పంక్తి_aligned_కొలత (&alloc) || alloc <= buf->alloc)
    తిరిగి false;

  అక్షరం *newbuf = జ్ఞాపకం_ఇవ్వు (alloc);
  ఒకవేళ (! newbuf)
    తిరిగి false;

  అక్షరం *oldbuf = buf->buf;
  నిర్మాణం పంక్తి *old_linelim = బఫరు_linelim (buf);
  నిర్మాణం పంక్తి *old_పంక్తి = old_linelim - buf->nlines;

  జ్ఞాపకం_నకలు (newbuf, oldbuf, buf->వాడిన);

  నిర్మాణం పంక్తి *new_linelim = (శూన్య *) (newbuf + alloc);
  నిర్మాణం పంక్తి *new_పంక్తి = new_linelim - buf->nlines;
  జ్ఞాపకం_నకలు (new_పంక్తి, old_పంక్తి, buf->nlines * పరిమాణం *new_పంక్తి);

  కొరకు (నిర్మాణం పంక్తి *పంక్తి = new_పంక్తి; పంక్తి < new_linelim; పంక్తి++)
    {
      పంక్తి->పాఠ్యం = newbuf + (పంక్తి->పాఠ్యం - oldbuf);
      ఒకవేళ (keylist)
        {
          పంక్తి->keybeg = newbuf + (పంక్తి->keybeg - oldbuf);
          పంక్తి->keylim = newbuf + (పంక్తి->keylim - oldbuf);
        }
    }

  విడుదల (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  తిరిగి true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

స్థిర bool
maybe_growbuf (నిర్మాణం బఫరు *buf, నిర్మాణం క్రమీకరించు_బఫరు_policy *policy)
{
  ఒకవేళ (! policy || policy->growth_failed || policy->హద్దు <= buf->alloc)
    తిరిగి false;

  size_t alloc;
  ఒకవేళ (buf->alloc <= policy->హద్దు / 3)
    alloc = buf->alloc * 3;
  లేకపోతే
    alloc = policy->హద్దు;

  ఒకవేళ (try_growbuf (buf, alloc))
    తిరిగి true;

  policy->growth_failed = true;
  తిరిగి false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

స్థిర అక్షరం *
begfield (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, నిర్మాణం keyfield స్థిరరాశి *కీలకం)
{
  అక్షరం *ptr = పంక్తి->పాఠ్యం, *lim = ptr + పంక్తి->నిడివి - 1;
  size_t sword = కీలకం->sword;
  size_t schar = కీలకం->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ఒకవేళ (tab != TAB_DEFAULT)
    అయితే (ptr < lim && sword--)
      {
        అక్షరం *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ఒకవేళ (ptr < lim)
          ++ptr;
      }
  లేకపోతే
    అయితే (ptr < lim && sword--)
      {
        అయితే (ptr < lim && blanks[గమ్యం_uchar (*ptr)])
          ++ptr;
        అయితే (ptr < lim && !blanks[గమ్యం_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ఒకవేళ (కీలకం->skipsblanks)
    అయితే (ptr < lim && blanks[గమ్యం_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t మిగిలిన_బైట్లు = lim - ptr;
  ఒకవేళ (schar < మిగిలిన_బైట్లు)
    ptr += schar;
  లేకపోతే
    ptr = lim;

  తిరిగి ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_శుద్ధ
స్థిర అక్షరం *
limfield (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, నిర్మాణం keyfield స్థిరరాశి *కీలకం)
{
  అక్షరం *ptr = పంక్తి->పాఠ్యం, *lim = ptr + పంక్తి->నిడివి - 1;
  size_t eword = కీలకం->eword, echar = కీలకం->echar;

  ఒకవేళ (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ఒకవేళ (tab != TAB_DEFAULT)
    అయితే (ptr < lim && eword--)
      {
        అక్షరం *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ఒకవేళ (ptr < lim && (eword || echar))
          ++ptr;
      }
  లేకపోతే
    అయితే (ptr < lim && eword--)
      {
        అయితే (ptr < lim && blanks[గమ్యం_uchar (*ptr)])
          ++ptr;
        అయితే (ptr < lim && !blanks[గమ్యం_uchar (*ptr)])
          ++ptr;
      }

#నిర్వచితమైతే POSIX_UNSPECIFIED
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
  ఒకవేళ (tab != TAB_DEFAULT)
    {
      అక్షరం *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ఒకవేళ (newlim)
        lim = newlim;
    }
  లేకపోతే
    {
      అక్షరం *newlim;
      newlim = ptr;
      అయితే (newlim < lim && blanks[గమ్యం_uchar (*newlim)])
        ++newlim;
      అయితే (newlim < lim && !blanks[గమ్యం_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#అయితే_ముగింపు

  ఒకవేళ (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ఒకవేళ (కీలకం->skipeblanks)
        అయితే (ptr < lim && blanks[గమ్యం_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t మిగిలిన_బైట్లు = lim - ptr;
      ఒకవేళ (echar < మిగిలిన_బైట్లు)
        ptr += echar;
      లేకపోతే
        ptr = lim;
    }

  తిరిగి ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

స్థిర bool
fillbuf (నిర్మాణం బఫరు *buf, FILE *fp, అక్షరం స్థిరరాశి *file,
         నిర్మాణం క్రమీకరించు_బఫరు_policy *policy)
{
  నిర్మాణం keyfield స్థిరరాశి *కీలకం = keylist;
  అక్షరం eol = eolchar;
  size_t పంక్తి_బైట్లు = buf->పంక్తి_బైట్లు;
  size_t mergesize = విలీనం_బఫరు_కొలత - MIN_విలీనం_బఫరు_కొలత;

  ఒకవేళ (buf->eof)
    తిరిగి false;

  ఒకవేళ (buf->వాడిన != buf->ఎడమ)
    {
      memmove (buf->buf, buf->buf + buf->వాడిన - buf->ఎడమ, buf->ఎడమ);
      buf->వాడిన = buf->ఎడమ;
      buf->nlines = 0;
    }

  అయితే (true)
    {
      అక్షరం *ptr = buf->buf + buf->వాడిన;
      నిర్మాణం పంక్తి *linelim = బఫరు_linelim (buf);
      నిర్మాణం పంక్తి *పంక్తి = linelim - buf->nlines;
      size_t avail = (అక్షరం *) linelim - buf->nlines * పంక్తి_బైట్లు - ptr;
      అక్షరం *పంక్తి_ప్రారంభం = buf->nlines ? పంక్తి->పాఠ్యం + పంక్తి->నిడివి : buf->buf;

      అయితే (పంక్తి_బైట్లు + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (పంక్తి_బైట్లు + 1);
          size_t బైట్లు_read = fread (ptr, 1, readsize, fp);
          అక్షరం *ptrlim = ptr + బైట్లు_read;
          అక్షరం *p;
          avail -= బైట్లు_read;

          ఒకవేళ (బైట్లు_read != readsize)
            {
              ఒకవేళ (ferror (fp))
                క్రమీకరించు_die (_("read failed"), file);
              ఒకవేళ (feof (fp))
                {
                  buf->eof = true;
                  ఒకవేళ (buf->buf == ptrlim)
                    తిరిగి false;
                  ఒకవేళ (పంక్తి_ప్రారంభం != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          అయితే ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              పంక్తి--;
              పంక్తి->పాఠ్యం = పంక్తి_ప్రారంభం;
              పంక్తి->నిడివి = ptr - పంక్తి_ప్రారంభం;
              mergesize = MAX (mergesize, పంక్తి->నిడివి);
              avail -= పంక్తి_బైట్లు;

              ఒకవేళ (కీలకం)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  పంక్తి->keylim = (కీలకం->eword == SIZE_MAX
                                  ? p
                                  : limfield (పంక్తి, కీలకం));

                  ఒకవేళ (కీలకం->sword != SIZE_MAX)
                    పంక్తి->keybeg = begfield (పంక్తి, కీలకం);
                  లేకపోతే
                    {
                      ఒకవేళ (కీలకం->skipsblanks)
                        అయితే (blanks[గమ్యం_uchar (*పంక్తి_ప్రారంభం)])
                          పంక్తి_ప్రారంభం++;
                      పంక్తి->keybeg = పంక్తి_ప్రారంభం;
                    }
                }

              పంక్తి_ప్రారంభం = ptr;
            }

          ptr = ptrlim;
          ఒకవేళ (buf->eof)
            ఆపు;
        }

      buf->వాడిన = ptr - buf->buf;
      buf->nlines = బఫరు_linelim (buf) - పంక్తి;
      ఒకవేళ (buf->nlines != 0)
        {
          ఒకవేళ (! buf->eof && maybe_growbuf (buf, policy))
            కొనసాగు;
          buf->ఎడమ = ptr - పంక్తి_ప్రారంభం;
          విలీనం_బఫరు_కొలత = mergesize + MIN_విలీనం_బఫరు_కొలత;
          తిరిగి true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t పంక్తి_alloc = buf->alloc / పరిమాణం (నిర్మాణం పంక్తి);
        buf->buf = xpalloc (buf->buf, &పంక్తి_alloc, 1, -1, పరిమాణం (నిర్మాణం పంక్తి));
        buf->alloc = పంక్తి_alloc * పరిమాణం (నిర్మాణం పంక్తి);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
స్థిర అక్షరం స్థిరరాశి unit_క్రమం[UCHAR_LIM] =
  {
#ఒకవేళ ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#లేకపోతే
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
#అయితే_ముగింపు
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
స్థిర అక్షరం
traverse_raw_సంఖ్య (అక్షరం స్థిరరాశి **సంఖ్య)
{
  అక్షరం స్థిరరాశి *p = *సంఖ్య;
  అక్షరం ch;
  అక్షరం max_digit = '\0';
  bool చివర్లు_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  అయితే (c_isdigit (ch = *p++))
    {
      ఒకవేళ (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      చివర్లు_with_thousands_sep = (*p == thousands_sep);
      ఒకవేళ (చివర్లు_with_thousands_sep)
        ++p;
    }

  ఒకవేళ (చివర్లు_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *సంఖ్య = p - 2;
      తిరిగి max_digit;
    }

  ఒకవేళ (ch == decimal_బిందువు)
    అయితే (c_isdigit (ch = *p++))
      ఒకవేళ (max_digit < ch)
        max_digit = ch;

  *సంఖ్య = p - 1;
  తిరిగి max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
కనుగొను_unit_క్రమం (అక్షరం స్థిరరాశి *సంఖ్య)
{
  bool minus_sign = (*సంఖ్య == '-');
  అక్షరం స్థిరరాశి *p = సంఖ్య + minus_sign;
  అక్షరం max_digit = traverse_raw_సంఖ్య (&p);
  ఒకవేళ ('0' < max_digit)
    {
      గుర్తులేని అక్షరం ch = *p;
      పూర్ణాంకం క్రమం = unit_క్రమం[ch];
      తిరిగి (minus_sign ? -క్రమం : క్రమం);
    }
  లేకపోతే
    తిరిగి 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
human_numcompare (అక్షరం స్థిరరాశి *a, అక్షరం స్థిరరాశి *b)
{
  అయితే (blanks[గమ్యం_uchar (*a)])
    a++;
  అయితే (blanks[గమ్యం_uchar (*b)])
    b++;

  పూర్ణాంకం diff = కనుగొను_unit_క్రమం (a) - కనుగొను_unit_క్రమం (b);
  తిరిగి (diff ? diff : strnumcmp (a, b, decimal_బిందువు, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_శుద్ధ
స్థిర పూర్ణాంకం
numcompare (అక్షరం స్థిరరాశి *a, అక్షరం స్థిరరాశి *b)
{
  అయితే (blanks[గమ్యం_uchar (*a)])
    a++;
  అయితే (blanks[గమ్యం_uchar (*b)])
    b++;

  తిరిగి strnumcmp (a, b, decimal_బిందువు, thousands_sep);
}

స్థిర పూర్ణాంకం
nan_compare (దీర్ఘ ద్విగుణ a, దీర్ఘ ద్విగుణ b)
{
  అక్షరం buf[2][పరిమాణం "-nan""()" + CHAR_BIT * పరిమాణం a];
  snprintf (buf[0], పరిమాణం buf[0], "%Lf", a);
  snprintf (buf[1], పరిమాణం buf[1], "%Lf", b);
  తిరిగి strcmp (buf[0], buf[1]);
}

స్థిర పూర్ణాంకం
general_numcompare (అక్షరం స్థిరరాశి *sa, అక్షరం స్థిరరాశి *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  అక్షరం *ea;
  అక్షరం *eb;
  దీర్ఘ ద్విగుణ a = strtold (sa, &ea);
  దీర్ఘ ద్విగుణ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ఒకవేళ (sa == ea)
    తిరిగి sb == eb ? 0 : -1;
  ఒకవేళ (sb == eb)
    తిరిగి 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  తిరిగి (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

స్థిర పూర్ణాంకం
getmonth (అక్షరం స్థిరరాశి *month, అక్షరం **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_చొప్పున_YEAR;

  అయితే (blanks[గమ్యం_uchar (*month)])
    month++;

  చేయి
    {
      size_t ix = (lo + hi) / 2;
      అక్షరం స్థిరరాశి *m = month;
      అక్షరం స్థిరరాశి *n = monthtab[ix].name;

      కొరకు (;; m++, n++)
        {
          ఒకవేళ (!*n)
            {
              ఒకవేళ (ea)
                *ea = (అక్షరం *) m;
              తిరిగి monthtab[ix].val;
            }
          ఒకవేళ (గమ్యం_uchar (fold_toupper[గమ్యం_uchar (*m)]) < గమ్యం_uchar (*n))
            {
              hi = ix;
              ఆపు;
            }
          లేకపోతే ఒకవేళ (గమ్యం_uchar (fold_toupper[గమ్యం_uchar (*m)]) > గమ్యం_uchar (*n))
            {
              lo = ix + 1;
              ఆపు;
            }
        }
    }
  అయితే (lo < hi);

  తిరిగి 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ఒకవేళ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ఒకవేళ 14 <= __GNUC__
#  pragma GCC diagnostic నెట్టు
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#అయితే_ముగింపు

#అన్తర్భావ "md5.h"

#ఒకవేళ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ఒకవేళ 14 <= __GNUC__
#  pragma GCC diagnostic తీయి
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_ప్రాధాన్యత_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
స్థిర శూన్య
link_failure (శూన్య)
{
  error (క్రమీకరించు_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
స్థిర శూన్య *
చిహ్నం_చిరునామా (శూన్య *handle, అక్షరం స్థిరరాశి *చిహ్నం)
{
  శూన్య *చిరునామా = dlsym (handle, చిహ్నం);
  ఒకవేళ (!చిరునామా)
    link_failure ();
  తిరిగి చిరునామా;
}
#అయితే_ముగింపు

/* Dynamically link the crypto library, if it needs linking.  */
స్థిర శూన్య
link_libcrypto (శూన్య)
{
#ఒకవేళ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  శూన్య *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ఒకవేళ (!handle)
    link_failure ();
  ptr_MD5_Init = చిహ్నం_చిరునామా (handle, "MD5_Init");
  ptr_MD5_Update = చిహ్నం_చిరునామా (handle, "MD5_Update");
  ptr_MD5_Final = చిహ్నం_చిరునామా (handle, "MD5_Final");
#అయితే_ముగింపు
}

/* A randomly chosen MD5 state, used for random comparison.  */
స్థిర నిర్మాణం md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

స్థిర శూన్య
random_md5_state_init (అక్షరం స్థిరరాశి *random_ఆధారం)
{
  గుర్తులేని అక్షరం buf[MD5_DIGEST_కొలత];
  నిర్మాణం randread_ఆధారం *r = randread_new (random_ఆధారం, పరిమాణం buf);
  ఒకవేళ (! r)
    క్రమీకరించు_die (_("open failed"), random_ఆధారం ? random_ఆధారం : "getrandom");
  randread (r, buf, పరిమాణం buf);
  ఒకవేళ (randread_free (r) != 0)
    క్రమీకరించు_die (_("close failed"), random_ఆధారం);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_బైట్లు (buf, పరిమాణం buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

స్థిర size_t
xstrxfrm (అక్షరం *పరిమిత dest, అక్షరం స్థిరరాశి *పరిమిత src, size_t destsize)
{
  errno = 0;
  size_t translated_కొలత = strxfrm (dest, src, destsize);

  ఒకవేళ (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (క్రమీకరించు_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  తిరిగి translated_కొలత;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

స్థిర పూర్ణాంకం
compare_random (అక్షరం *పరిమిత texta, size_t lena,
                అక్షరం *పరిమిత textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  పూర్ణాంకం xfrm_diff = 0;

  అక్షరం stackbuf[4000];
  అక్షరం *buf = stackbuf;
  size_t bufsize = పరిమాణం stackbuf;
  శూన్య *allocated = ఖాళీ;
  uint32_t dig[2][MD5_DIGEST_కొలత / పరిమాణం (uint32_t)];
  నిర్మాణం md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ఒకవేళ (hard_LC_COLLATE)
    {
      అక్షరం స్థిరరాశి *lima = texta + lena;
      అక్షరం స్థిరరాశి *limb = textb + lenb;

      అయితే (true)
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
          ఒకవేళ (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              విడుదల (allocated);
              buf = allocated = జ్ఞాపకం_ఇవ్వు (bufsize);
              ఒకవేళ (! buf)
                {
                  buf = stackbuf;
                  bufsize = పరిమాణం stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_సరిపోతుంది = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_సరిపోతుంది ? buf + sizea : ఖాళీ), textb,
                          (a_సరిపోతుంది ? bufsize - sizea : 0))
                + 1)
             : 0);

          ఒకవేళ (! (a_సరిపోతుంది && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ఒకవేళ (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              విడుదల (allocated);
              buf = allocated = xmalloc (bufsize);
              ఒకవేళ (texta < lima)
                strxfrm (buf, texta, sizea);
              ఒకవేళ (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ఒకవేళ (texta < lima)
            texta += అక్షరశ్రేణి_పొడవు (texta) + 1;
          ఒకవేళ (textb < limb)
            textb += అక్షరశ్రేణి_పొడవు (textb) + 1;
          ఒకవేళ (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ఆపు;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_బైట్లు (buf, sizea, &s[0]);
          md5_process_బైట్లు (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ఒకవేళ (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ఒకవేళ (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_బైట్లు (texta, lena, &s[0]); md5_ముగింపు_ctx (&s[0], dig[0]);
  md5_process_బైట్లు (textb, lenb, &s[1]); md5_ముగింపు_ctx (&s[1], dig[1]);
  పూర్ణాంకం diff = memcmp (dig[0], dig[1], పరిమాణం dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ఒకవేళ (! diff)
    {
      ఒకవేళ (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ఒకవేళ (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  విడుదల (allocated);

  తిరిగి diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

స్థిర size_t
debug_వెడల్పు (అక్షరం స్థిరరాశి *పాఠ్యం, అక్షరం స్థిరరాశి *lim)
{
  size_t వెడల్పు = mbsnwidth (పాఠ్యం, lim - పాఠ్యం, 0);
  అయితే (పాఠ్యం < lim)
    వెడల్పు += (*పాఠ్యం++ == '\t');
  తిరిగి వెడల్పు;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

స్థిర శూన్య
mark_కీలకం (size_t స్థానభ్రంశం, size_t వెడల్పు)
{
  అయితే (స్థానభ్రంశం--)
    అక్షరం_రాయి (' ');

  ఒకవేళ (!వెడల్పు)
    రాయి (_("^ no match for key\n"));
  లేకపోతే
    {
      చేయి
        అక్షరం_రాయి ('_');
      అయితే (--వెడల్పు);

      అక్షరం_రాయి ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

స్థిర అంతఃవరుస bool
కీలకం_numeric (నిర్మాణం keyfield స్థిరరాశి *కీలకం)
{
  తిరిగి కీలకం->numeric || కీలకం->general_numeric || కీలకం->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

స్థిర శూన్య
debug_కీలకం (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, నిర్మాణం keyfield స్థిరరాశి *కీలకం)
{
  అక్షరం *పాఠ్యం = పంక్తి->పాఠ్యం;
  అక్షరం *beg = పాఠ్యం;
  అక్షరం *lim = పాఠ్యం + పంక్తి->నిడివి - 1;

  ఒకవేళ (కీలకం)
    {
      ఒకవేళ (కీలకం->sword != SIZE_MAX)
        beg = begfield (పంక్తి, కీలకం);
      ఒకవేళ (కీలకం->eword != SIZE_MAX)
        {
          lim = limfield (పంక్తి, కీలకం);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ఒకవేళ ((కీలకం->skipsblanks && కీలకం->sword == SIZE_MAX)
          || కీలకం->month || కీలకం_numeric (కీలకం))
        {
          అక్షరం saved = *lim;
          *lim = '\0';

          అయితే (blanks[గమ్యం_uchar (*beg)])
            beg++;

          అక్షరం *tighter_lim = beg;

          ఒకవేళ (lim < beg)
            tighter_lim = lim;
          లేకపోతే ఒకవేళ (కీలకం->month)
            getmonth (beg, &tighter_lim);
          లేకపోతే ఒకవేళ (కీలకం->general_numeric)
            ignore_విలువ (strtold (beg, &tighter_lim));
          లేకపోతే ఒకవేళ (కీలకం->numeric || కీలకం->human_numeric)
            {
              అక్షరం స్థిరరాశి *p = beg + (beg < lim && *beg == '-');
              అక్షరం max_digit = traverse_raw_సంఖ్య (&p);
              ఒకవేళ ('0' <= max_digit)
                {
                  గుర్తులేని అక్షరం ch = *p;
                  tighter_lim = (అక్షరం *) p
                    + (కీలకం->human_numeric && unit_క్రమం[ch]);
                }
            }
          లేకపోతే
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t స్థానభ్రంశం = debug_వెడల్పు (పాఠ్యం, beg);
  size_t వెడల్పు = debug_వెడల్పు (beg, lim);
  mark_కీలకం (స్థానభ్రంశం, వెడల్పు);
}

/* Debug LINE by underlining its keys.  */

స్థిర శూన్య
debug_పంక్తి (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి)
{
  నిర్మాణం keyfield స్థిరరాశి *కీలకం = keylist;

  చేయి
    debug_కీలకం (పంక్తి, కీలకం);
  అయితే (కీలకం && ((కీలకం = కీలకం->తదుపరి) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

స్థిర bool
default_కీలకం_compare (నిర్మాణం keyfield స్థిరరాశి *కీలకం)
{
  తిరిగి ! (కీలకం->ignore
            || కీలకం->translate
            || కీలకం->skipsblanks
            || కీలకం->skipeblanks
            || కీలకం_numeric (కీలకం)
            || కీలకం->month
            || కీలకం->సంస్కరణ_3
            || కీలకం->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

స్థిర శూన్య
కీలకం_గమ్యం_opts (నిర్మాణం keyfield స్థిరరాశి *కీలకం, అక్షరం *opts)
{
  ఒకవేళ (కీలకం->skipsblanks || కీలకం->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ఒకవేళ (కీలకం->ignore == nondictionary)
    *opts++ = 'd';
  ఒకవేళ (కీలకం->translate)
    *opts++ = 'f';
  ఒకవేళ (కీలకం->general_numeric)
    *opts++ = 'g';
  ఒకవేళ (కీలకం->human_numeric)
    *opts++ = 'h';
  ఒకవేళ (కీలకం->ignore == nonprinting)
    *opts++ = 'i';
  ఒకవేళ (కీలకం->month)
    *opts++ = 'M';
  ఒకవేళ (కీలకం->numeric)
    *opts++ = 'n';
  ఒకవేళ (కీలకం->random)
    *opts++ = 'R';
  ఒకవేళ (కీలకం->తిరగేయి)
    *opts++ = 'r';
  ఒకవేళ (కీలకం->సంస్కరణ_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

స్థిర శూన్య
కీలకం_warnings (నిర్మాణం keyfield స్థిరరాశి *gkey, bool gkey_only)
{
  నిర్మాణం keyfield ugkey = *gkey;
  గుర్తులేని దీర్ఘ keynum = 1;
  bool basic_numeric_క్షేత్రం = false;
  bool general_numeric_క్షేత్రం = false;
  bool basic_numeric_క్షేత్రం_పరిధి = false;
  bool general_numeric_క్షేత్రం_పరిధి = false;

  కొరకు (నిర్మాణం keyfield *కీలకం = keylist; కీలకం; కీలకం = కీలకం->తదుపరి, keynum++)
    {
      ఒకవేళ (కీలకం_numeric (కీలకం))
        {
          ఒకవేళ (కీలకం->general_numeric)
            general_numeric_క్షేత్రం = true;
          లేకపోతే
            basic_numeric_క్షేత్రం = true;
        }

      ఒకవేళ (కీలకం->traditional_వాడిన)
        {
          size_t sword = కీలకం->sword;
          size_t eword = కీలకం->eword;
          అక్షరం tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          అక్షరం obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          అక్షరం nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          అక్షరం *po = obuf;
          అక్షరం *pn = nbuf;

          ఒకవేళ (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ఒకవేళ (కీలకం->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (కీలకం->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_వెడల్పు = కీలకం->sword != SIZE_MAX && కీలకం->eword < కీలకం->sword;
      ఒకవేళ (zero_వెడల్పు)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = కీలకం_numeric (కీలకం) || కీలకం->month;
      bool పంక్తి_స్థానభ్రంశం = కీలకం->eword == 0 && కీలకం->echar != 0; /* -k1.x,1.y  */
      ఒకవేళ (!zero_వెడల్పు && !gkey_only && tab == TAB_DEFAULT && !పంక్తి_స్థానభ్రంశం
          && ((!కీలకం->skipsblanks && !implicit_skip)
              || (!కీలకం->skipsblanks && కీలకం->schar)
              || (!కీలకం->skipeblanks && కీలకం->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ఒకవేళ (!gkey_only && కీలకం_numeric (కీలకం))
        {
          size_t sword = కీలకం->sword + 1;
          size_t eword = కీలకం->eword + 1;
          ఒకవేళ (!sword)
            sword++;
          ఒకవేళ (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ఒకవేళ (కీలకం->general_numeric)
                general_numeric_క్షేత్రం_పరిధి = true;
              లేకపోతే
                basic_numeric_క్షేత్రం_పరిధి = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ఒకవేళ (ugkey.ignore && (ugkey.ignore == కీలకం->ignore))
        ugkey.ignore = ఖాళీ;
      ఒకవేళ (ugkey.translate && (ugkey.translate == కీలకం->translate))
        ugkey.translate = ఖాళీ;
      ugkey.skipsblanks &= !కీలకం->skipsblanks;
      ugkey.skipeblanks &= !కీలకం->skipeblanks;
      ugkey.month &= !కీలకం->month;
      ugkey.numeric &= !కీలకం->numeric;
      ugkey.general_numeric &= !కీలకం->general_numeric;
      ugkey.human_numeric &= !కీలకం->human_numeric;
      ugkey.random &= !కీలకం->random;
      ugkey.సంస్కరణ_3 &= !కీలకం->సంస్కరణ_3;
      ugkey.తిరగేయి &= !కీలకం->తిరగేయి;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool సంఖ్య_locale_warned = false;
  ఒకవేళ (basic_numeric_క్షేత్రం_పరిధి)
    {
      ఒకవేళ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (గమ్యం_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((అక్షరం []) {thousands_sep, 0})));
          సంఖ్య_locale_warned = true;
        }
    }
  ఒకవేళ (basic_numeric_క్షేత్రం_పరిధి || general_numeric_క్షేత్రం_పరిధి)
    {
      ఒకవేళ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (గమ్యం_uchar (decimal_బిందువు)))
          : tab == decimal_బిందువు)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((అక్షరం []) {decimal_బిందువు, 0})));
          సంఖ్య_locale_warned = true;
        }
      లేకపోతే ఒకవేళ (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((అక్షరం []) {tab, 0})));
        }
      లేకపోతే ఒకవేళ (general_numeric_క్షేత్రం_పరిధి && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((అక్షరం []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ఒకవేళ ((basic_numeric_క్షేత్రం || general_numeric_క్షేత్రం) && ! సంఖ్య_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((అక్షరం []) {decimal_బిందువు, 0})));

    }

  ఒకవేళ (basic_numeric_క్షేత్రం && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ఒకవేళ (!default_కీలకం_compare (&ugkey)
      || (ugkey.తిరగేయి && (stable || unique) && keylist))
    {
      bool ugkey_తిరగేయి = ugkey.తిరగేయి;
      ఒకవేళ (!(stable || unique))
        ugkey.తిరగేయి = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      అక్షరం opts[పరిమాణం short_options];
      కీలకం_గమ్యం_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (అక్షరశ్రేణి_పొడవు (opts))), opts);
      ugkey.తిరగేయి = ugkey_తిరగేయి;
    }
  ఒకవేళ (ugkey.తిరగేయి && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

స్థిర పూర్ణాంకం
diff_reversed (పూర్ణాంకం diff, bool reversed)
{
  తిరిగి reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

స్థిర పూర్ణాంకం
keycompare (నిర్మాణం పంక్తి స్థిరరాశి *a, నిర్మాణం పంక్తి స్థిరరాశి *b)
{
  నిర్మాణం keyfield *కీలకం = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  అక్షరం *texta = a->keybeg;
  అక్షరం *textb = b->keybeg;
  అక్షరం *lima = a->keylim;
  అక్షరం *limb = b->keylim;

  పూర్ణాంకం diff;

  అయితే (true)
    {
      అక్షరం స్థిరరాశి *translate = కీలకం->translate;
      bool స్థిరరాశి *ignore = కీలకం->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ఒకవేళ (hard_LC_COLLATE || కీలకం_numeric (కీలకం)
          || కీలకం->month || కీలకం->random || కీలకం->సంస్కరణ_3)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          అక్షరం *ta = texta;
          అక్షరం *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          అక్షరం enda = ta[tlena];
          అక్షరం endb = tb[tlenb];

          శూన్య *allocated = ఖాళీ;
          అక్షరం stackbuf[4000];

          ఒకవేళ (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t కొలత = lena + 1 + lenb + 1;
              ఒకవేళ (కొలత <= పరిమాణం stackbuf)
                ta = stackbuf;
              లేకపోతే
                ta = allocated = xmalloc (కొలత);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              కొరకు (tlena = i = 0; i < lena; i++)
                ఒకవేళ (! (ignore && ignore[గమ్యం_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[గమ్యం_uchar (texta[i])]
                                 : texta[i]);

              కొరకు (tlenb = i = 0; i < lenb; i++)
                ఒకవేళ (! (ignore && ignore[గమ్యం_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[గమ్యం_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ఒకవేళ (కీలకం->numeric)
            diff = numcompare (ta, tb);
          లేకపోతే ఒకవేళ (కీలకం->general_numeric)
            diff = general_numcompare (ta, tb);
          లేకపోతే ఒకవేళ (కీలకం->human_numeric)
            diff = human_numcompare (ta, tb);
          లేకపోతే ఒకవేళ (కీలకం->month)
            diff = getmonth (ta, ఖాళీ) - getmonth (tb, ఖాళీ);
          లేకపోతే ఒకవేళ (కీలకం->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          లేకపోతే ఒకవేళ (కీలకం->సంస్కరణ_3)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          లేకపోతే
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ఒకవేళ (tlena == 0)
                diff = - NONZERO (tlenb);
              లేకపోతే ఒకవేళ (tlenb == 0)
                diff = 1;
              లేకపోతే
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          విడుదల (allocated);
        }
      లేకపోతే ఒకవేళ (ignore)
        {
#నిర్వచించు CMP_WITH_IGNORE(A, B)						\
  చేయి									\
    {									\
          అయితే (true)							\
            {								\
              అయితే (texta < lima && ignore[గమ్యం_uchar (*texta)])		\
                ++texta;						\
              అయితే (textb < limb && ignore[గమ్యం_uchar (*textb)])		\
                ++textb;						\
              ఒకవేళ (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ఆపు;						\
                }							\
              diff = గమ్యం_uchar (A) - గమ్యం_uchar (B);			\
              ఒకవేళ (diff)							\
                ఆపు;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  అయితే (0)

          ఒకవేళ (translate)
            CMP_WITH_IGNORE (translate[గమ్యం_uchar (*texta)],
                             translate[గమ్యం_uchar (*textb)]);
          లేకపోతే
            CMP_WITH_IGNORE (*texta, *textb);
        }
      లేకపోతే
        {
          size_t lenmin = MIN (lena, lenb);
          ఒకవేళ (lenmin == 0)
            diff = 0;
          లేకపోతే ఒకవేళ (translate)
            {
              size_t i = 0;
              చేయి
                {
                  diff = (గమ్యం_uchar (translate[గమ్యం_uchar (texta[i])])
                          - గమ్యం_uchar (translate[గమ్యం_uchar (textb[i])]));
                  ఒకవేళ (diff)
                    ఆపు;
                  i++;
                }
              అయితే (i < lenmin);
            }
          లేకపోతే
            diff = memcmp (texta, textb, lenmin);

          ఒకవేళ (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ఒకవేళ (diff)
        ఆపు;

      కీలకం = కీలకం->తదుపరి;
      ఒకవేళ (! కీలకం)
        తిరిగి 0;

      /* Find the beginning and limit of the next field.  */
      ఒకవేళ (కీలకం->eword != SIZE_MAX)
        lima = limfield (a, కీలకం), limb = limfield (b, కీలకం);
      లేకపోతే
        lima = a->పాఠ్యం + a->నిడివి - 1, limb = b->పాఠ్యం + b->నిడివి - 1;

      ఒకవేళ (కీలకం->sword != SIZE_MAX)
        texta = begfield (a, కీలకం), textb = begfield (b, కీలకం);
      లేకపోతే
        {
          texta = a->పాఠ్యం, textb = b->పాఠ్యం;
          ఒకవేళ (కీలకం->skipsblanks)
            {
              అయితే (texta < lima && blanks[గమ్యం_uchar (*texta)])
                ++texta;
              అయితే (textb < limb && blanks[గమ్యం_uchar (*textb)])
                ++textb;
            }
        }
    }

  తిరిగి diff_reversed (diff, కీలకం->తిరగేయి);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

స్థిర పూర్ణాంకం
compare (నిర్మాణం పంక్తి స్థిరరాశి *a, నిర్మాణం పంక్తి స్థిరరాశి *b)
{
  పూర్ణాంకం diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ఒకవేళ (keylist)
    {
      diff = keycompare (a, b);
      ఒకవేళ (diff || unique || stable)
        తిరిగి diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->నిడివి - 1, blen = b->నిడివి - 1;

  ఒకవేళ (alen == 0)
    diff = - NONZERO (blen);
  లేకపోతే ఒకవేళ (blen == 0)
    diff = 1;
  లేకపోతే ఒకవేళ (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->పాఠ్యం, alen + 1, b->పాఠ్యం, blen + 1);
    }
  లేకపోతే
    {
      diff = memcmp (a->పాఠ్యం, b->పాఠ్యం, MIN (alen, blen));
      ఒకవేళ (!diff)
        diff = _GL_CMP (alen, blen);
    }

  తిరిగి diff_reversed (diff, తిరగేయి);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

స్థిర శూన్య
write_పంక్తి (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, FILE *fp, అక్షరం స్థిరరాశి *output_file)
{
  అక్షరం *buf = పంక్తి->పాఠ్యం;
  size_t n_బైట్లు = పంక్తి->నిడివి;
  అక్షరం *ebuf = buf + n_బైట్లు;

  ఒకవేళ (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      అక్షరం స్థిరరాశి *c = buf;

      అయితే (c < ebuf)
        {
          అక్షరం wc = *c++;
          ఒకవేళ (wc == '\t')
            wc = '>';
          లేకపోతే ఒకవేళ (c == ebuf)
            wc = '\n';
          ఒకవేళ (fputc (wc, fp) == EOF)
            క్రమీకరించు_die (_("write failed"), output_file);
        }

      debug_పంక్తి (పంక్తి);
    }
  లేకపోతే
    {
      ebuf[-1] = eolchar;
      ఒకవేళ (fwrite (buf, 1, n_బైట్లు, fp) != n_బైట్లు)
        క్రమీకరించు_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

స్థిర bool
check (అక్షరం స్థిరరాశి *file_name, అక్షరం checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  నిర్మాణం బఫరు buf;		/* Input buffer. */
  నిర్మాణం పంక్తి తాత్కాలిక;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t పంక్తి_సంఖ్య = 0;
  నిర్మాణం keyfield స్థిరరాశి *కీలకం = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, పరిమాణం (నిర్మాణం పంక్తి),
           MAX (విలీనం_బఫరు_కొలత, క్రమీకరించు_కొలత));
  తాత్కాలిక.పాఠ్యం = ఖాళీ;

  అయితే (fillbuf (&buf, fp, file_name, ఖాళీ))
    {
      నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి = బఫరు_linelim (&buf);
      నిర్మాణం పంక్తి స్థిరరాశి *linebase = పంక్తి - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ఒకవేళ (alloc && nonunique <= compare (&తాత్కాలిక, పంక్తి - 1))
        {
        దొరికింది_disorder:
          {
            ఒకవేళ (checkonly == 'c')
              {
                నిర్మాణం పంక్తి స్థిరరాశి *disorder_పంక్తి = పంక్తి - 1;
                uintmax_t disorder_పంక్తి_సంఖ్య =
                  బఫరు_linelim (&buf) - disorder_పంక్తి + పంక్తి_సంఖ్య;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         కార్యక్రమం_name, file_name, disorder_పంక్తి_సంఖ్య);
                write_పంక్తి (disorder_పంక్తి, stderr, _("standard error"));
              }

            ordered = false;
            ఆపు;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      అయితే (linebase < --పంక్తి)
        ఒకవేళ (nonunique <= compare (పంక్తి, పంక్తి - 1))
          వెళ్ళు దొరికింది_disorder;

      పంక్తి_సంఖ్య += buf.nlines;

      /* Save the last line of the buffer.  */
      ఒకవేళ (alloc < పంక్తి->నిడివి)
        {
          చేయి
            {
              alloc *= 2;
              ఒకవేళ (! alloc)
                {
                  alloc = పంక్తి->నిడివి;
                  ఆపు;
                }
            }
          అయితే (alloc < పంక్తి->నిడివి);

          విడుదల (తాత్కాలిక.పాఠ్యం);
          తాత్కాలిక.పాఠ్యం = xmalloc (alloc);
        }
      జ్ఞాపకం_నకలు (తాత్కాలిక.పాఠ్యం, పంక్తి->పాఠ్యం, పంక్తి->నిడివి);
      తాత్కాలిక.నిడివి = పంక్తి->నిడివి;
      ఒకవేళ (కీలకం)
        {
          తాత్కాలిక.keybeg = తాత్కాలిక.పాఠ్యం + (పంక్తి->keybeg - పంక్తి->పాఠ్యం);
          తాత్కాలిక.keylim = తాత్కాలిక.పాఠ్యం + (పంక్తి->keylim - పంక్తి->పాఠ్యం);
        }
    }

  xfclose (fp, file_name);
  విడుదల (buf.buf);
  విడుదల (తాత్కాలిక.పాఠ్యం);
  తిరిగి ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

స్థిర size_t
open_ఆదానం_files (నిర్మాణం sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, పరిమాణం *fps);
  పూర్ణాంకం i;

  /* Open as many input files as we can.  */
  కొరకు (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].తాత్కాలిక && files[i].తాత్కాలిక->state != UNCOMPRESSED
                ? open_తాత్కాలిక (files[i].తాత్కాలిక)
                : ప్రవాహం_open (files[i].name, "r"));
      ఒకవేళ (!fps[i])
        ఆపు;
    }

  తిరిగి i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

స్థిర శూన్య
mergefps (నిర్మాణం sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, అక్షరం స్థిరరాశి *output_file, FILE **fps)
{
  నిర్మాణం బఫరు *బఫరు = xnmalloc (nfiles, పరిమాణం *బఫరు);
                                /* Input buffers for each file. */
  నిర్మాణం పంక్తి saved;		/* Saved line storage for unique check. */
  నిర్మాణం పంక్తి స్థిరరాశి *savedline = ఖాళీ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  నిర్మాణం పంక్తి స్థిరరాశి **cur = xnmalloc (nfiles, పరిమాణం *cur);
                                /* Current line in each line table. */
  నిర్మాణం పంక్తి స్థిరరాశి **పునాది_2 = xnmalloc (nfiles, పరిమాణం *పునాది_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, పరిమాణం *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  నిర్మాణం keyfield స్థిరరాశి *కీలకం = keylist;
  saved.పాఠ్యం = ఖాళీ;

  /* Read initial lines from each input file. */
  కొరకు (size_t i = 0; i < nfiles; )
    {
      initbuf (&బఫరు[i], పరిమాణం (నిర్మాణం పంక్తి),
               MAX (విలీనం_బఫరు_కొలత, క్రమీకరించు_కొలత / nfiles));
      ఒకవేళ (fillbuf (&బఫరు[i], fps[i], files[i].name, ఖాళీ))
        {
          నిర్మాణం పంక్తి స్థిరరాశి *linelim = బఫరు_linelim (&బఫరు[i]);
          cur[i] = linelim - 1;
          పునాది_2[i] = linelim - బఫరు[i].nlines;
          i++;
        }
      లేకపోతే
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ఒకవేళ (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          విడుదల (బఫరు[i].buf);
          --nfiles;
          కొరకు (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  కొరకు (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  కొరకు (size_t i = 1; i < nfiles; ++i)
    ఒకవేళ (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  అయితే (nfiles)
    {
      నిర్మాణం పంక్తి స్థిరరాశి *అతిచిన్న = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ఒకవేళ (unique)
        {
          ఒకవేళ (savedline && compare (savedline, అతిచిన్న))
            {
              savedline = ఖాళీ;
              write_పంక్తి (&saved, ofp, output_file);
            }
          ఒకవేళ (!savedline)
            {
              savedline = &saved;
              ఒకవేళ (savealloc < అతిచిన్న->నిడివి)
                {
                  చేయి
                    ఒకవేళ (! savealloc)
                      {
                        savealloc = అతిచిన్న->నిడివి;
                        ఆపు;
                      }
                  అయితే ((savealloc *= 2) < అతిచిన్న->నిడివి);

                  విడుదల (saved.పాఠ్యం);
                  saved.పాఠ్యం = xmalloc (savealloc);
                }
              saved.నిడివి = అతిచిన్న->నిడివి;
              జ్ఞాపకం_నకలు (saved.పాఠ్యం, అతిచిన్న->పాఠ్యం, saved.నిడివి);
              ఒకవేళ (కీలకం)
                {
                  saved.keybeg =
                    saved.పాఠ్యం + (అతిచిన్న->keybeg - అతిచిన్న->పాఠ్యం);
                  saved.keylim =
                    saved.పాఠ్యం + (అతిచిన్న->keylim - అతిచిన్న->పాఠ్యం);
                }
            }
        }
      లేకపోతే
        write_పంక్తి (అతిచిన్న, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ఒకవేళ (పునాది_2[ord[0]] < అతిచిన్న)
        cur[ord[0]] = అతిచిన్న - 1;
      లేకపోతే
        {
          ఒకవేళ (fillbuf (&బఫరు[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ఖాళీ))
            {
              నిర్మాణం పంక్తి స్థిరరాశి *linelim = బఫరు_linelim (&బఫరు[ord[0]]);
              cur[ord[0]] = linelim - 1;
              పునాది_2[ord[0]] = linelim - బఫరు[ord[0]].nlines;
            }
          లేకపోతే
            {
              /* We reached EOF on fps[ord[0]].  */
              కొరకు (size_t i = 1; i < nfiles; ++i)
                ఒకవేళ (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ఒకవేళ (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              విడుదల (బఫరు[ord[0]].buf);
              కొరకు (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  బఫరు[i] = బఫరు[i + 1];
                  cur[i] = cur[i + 1];
                  పునాది_2[i] = పునాది_2[i + 1];
                }
              కొరకు (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              కొనసాగు;
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
        size_t లెక్క_యొక్క_smaller_పంక్తులు;

        అయితే (lo < hi)
          {
            పూర్ణాంకం cmp = compare (cur[ord0], cur[ord[probe]]);
            ఒకవేళ (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            లేకపోతే
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        లెక్క_యొక్క_smaller_పంక్తులు = lo - 1;
        కొరకు (size_t j = 0; j < లెక్క_యొక్క_smaller_పంక్తులు; j++)
          ord[j] = ord[j + 1];
        ord[లెక్క_యొక్క_smaller_పంక్తులు] = ord0;
      }
    }

  ఒకవేళ (unique && savedline)
    {
      write_పంక్తి (&saved, ofp, output_file);
      విడుదల (saved.పాఠ్యం);
    }

  xfclose (ofp, output_file);
  విడుదల (fps);
  విడుదల (బఫరు);
  విడుదల (ord);
  విడుదల (పునాది_2);
  విడుదల (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

స్థిర size_t
mergefiles (నిర్మాణం sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, అక్షరం స్థిరరాశి *output_file)
{
  FILE **fps;
  size_t nopened = open_ఆదానం_files (files, nfiles, &fps);
  ఒకవేళ (nopened < nfiles && nopened < 2)
    క్రమీకరించు_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  తిరిగి nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

స్థిర శూన్య
mergelines (నిర్మాణం పంక్తి *పరిమిత t, size_t nlines,
            నిర్మాణం పంక్తి స్థిరరాశి *పరిమిత lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  నిర్మాణం పంక్తి *hi = t - nlo;

  అయితే (true)
    ఒకవేళ (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ఒకవేళ (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            తిరిగి;
          }
      }
    లేకపోతే
      {
        *--t = *--hi;
        ఒకవేళ (! --nhi)
          {
            చేయి
              *--t = *--lo;
            అయితే (--nlo);

            తిరిగి;
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

స్థిర శూన్య
sequential_క్రమీకరించు (నిర్మాణం పంక్తి *పరిమిత పంక్తులు, size_t nlines,
                 నిర్మాణం పంక్తి *పరిమిత తాత్కాలిక, bool గమ్యం_తాత్కాలిక)
{
  ఒకవేళ (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      పూర్ణాంకం మార్పిడి = (0 < compare (&పంక్తులు[-1], &పంక్తులు[-2]));
      ఒకవేళ (గమ్యం_తాత్కాలిక)
        {
          తాత్కాలిక[-1] = పంక్తులు[-1 - మార్పిడి];
          తాత్కాలిక[-2] = పంక్తులు[-2 + మార్పిడి];
        }
      లేకపోతే ఒకవేళ (మార్పిడి)
        {
          తాత్కాలిక[-1] = పంక్తులు[-1];
          పంక్తులు[-1] = పంక్తులు[-2];
          పంక్తులు[-2] = తాత్కాలిక[-1];
        }
    }
  లేకపోతే
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      నిర్మాణం పంక్తి *lo = పంక్తులు;
      నిర్మాణం పంక్తి *hi = పంక్తులు - nlo;

      sequential_క్రమీకరించు (hi, nhi, తాత్కాలిక - (గమ్యం_తాత్కాలిక ? nlo : 0), గమ్యం_తాత్కాలిక);
      ఒకవేళ (1 < nlo)
        sequential_క్రమీకరించు (lo, nlo, తాత్కాలిక, !గమ్యం_తాత్కాలిక);
      లేకపోతే ఒకవేళ (!గమ్యం_తాత్కాలిక)
        తాత్కాలిక[-1] = lo[-1];

      నిర్మాణం పంక్తి *dest;
      నిర్మాణం పంక్తి స్థిరరాశి *sorted_lo;
      ఒకవేళ (గమ్యం_తాత్కాలిక)
        {
          dest = తాత్కాలిక;
          sorted_lo = పంక్తులు;
        }
      లేకపోతే
        {
          dest = పంక్తులు;
          sorted_lo = తాత్కాలిక;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

స్థిర నిర్మాణం విలీనం_కణుపు *init_కణుపు (నిర్మాణం విలీనం_కణుపు *పరిమిత,
                                     నిర్మాణం విలీనం_కణుపు *పరిమిత,
                                     నిర్మాణం పంక్తి *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
స్థిర నిర్మాణం విలీనం_కణుపు *
విలీనం_వృక్షం_init (size_t nthreads, size_t nlines, నిర్మాణం పంక్తి *dest)
{
  నిర్మాణం విలీనం_కణుపు *విలీనం_వృక్షం = xmalloc (2 * పరిమాణం *విలీనం_వృక్షం * nthreads);

  నిర్మాణం విలీనం_కణుపు *మూలం = విలీనం_వృక్షం;
  మూలం->lo = మూలం->hi = మూలం->చివర_lo = మూలం->చివర_hi = ఖాళీ;
  మూలం->dest = ఖాళీ;
  మూలం->nlo = మూలం->nhi = nlines;
  మూలం->మాతృ = ఖాళీ;
  మూలం->level = విలీనం_చివర;
  మూలం->queued = false;
  pthread_mutex_init (&మూలం->తాళం, ఖాళీ);

  init_కణుపు (మూలం, మూలం + 1, dest, nthreads, nlines, false);
  తిరిగి విలీనం_వృక్షం;
}

/* Destroy the merge tree. */
స్థిర శూన్య
విలీనం_వృక్షం_destroy (size_t nthreads, నిర్మాణం విలీనం_కణుపు *విలీనం_వృక్షం)
{
  size_t n_nodes = nthreads * 2;
  నిర్మాణం విలీనం_కణుపు *కణుపు = విలీనం_వృక్షం;

  అయితే (n_nodes--)
    {
      pthread_mutex_destroy (&కణుపు->తాళం);
      కణుపు++;
    }

  విడుదల (విలీనం_వృక్షం);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

స్థిర నిర్మాణం విలీనం_కణుపు *
init_కణుపు (నిర్మాణం విలీనం_కణుపు *పరిమిత మాతృ,
           నిర్మాణం విలీనం_కణుపు *పరిమిత కణుపు_pool,
           నిర్మాణం పంక్తి *dest, size_t nthreads,
           size_t మొత్తం_పంక్తులు, bool is_lo_బిడ్డ)
{
  size_t nlines = (is_lo_బిడ్డ ? మాతృ->nlo : మాతృ->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  నిర్మాణం పంక్తి *lo = dest - మొత్తం_పంక్తులు;
  నిర్మాణం పంక్తి *hi = lo - nlo;
  నిర్మాణం పంక్తి **మాతృ_చివర = (is_lo_బిడ్డ ? &మాతృ->చివర_lo : &మాతృ->చివర_hi);

  నిర్మాణం విలీనం_కణుపు *కణుపు = కణుపు_pool++;
  కణుపు->lo = కణుపు->చివర_lo = lo;
  కణుపు->hi = కణుపు->చివర_hi = hi;
  కణుపు->dest = మాతృ_చివర;
  కణుపు->nlo = nlo;
  కణుపు->nhi = nhi;
  కణుపు->మాతృ = మాతృ;
  కణుపు->level = మాతృ->level + 1;
  కణుపు->queued = false;
  pthread_mutex_init (&కణుపు->తాళం, ఖాళీ);

  ఒకవేళ (nthreads > 1)
    {
      size_t lo_దారాలు = nthreads / 2;
      size_t hi_దారాలు = nthreads - lo_దారాలు;
      కణుపు->lo_బిడ్డ = కణుపు_pool;
      కణుపు_pool = init_కణుపు (కణుపు, కణుపు_pool, lo, lo_దారాలు,
                             మొత్తం_పంక్తులు, true);
      కణుపు->hi_బిడ్డ = కణుపు_pool;
      కణుపు_pool = init_కణుపు (కణుపు, కణుపు_pool, hi, hi_దారాలు,
                             మొత్తం_పంక్తులు, false);
    }
  లేకపోతే
    {
      కణుపు->lo_బిడ్డ = ఖాళీ;
      కణుపు->hi_బిడ్డ = ఖాళీ;
    }
  తిరిగి కణుపు_pool;
}


/* Compare two merge nodes A and B for priority.  */

స్థిర పూర్ణాంకం
compare_nodes (శూన్య స్థిరరాశి *a, శూన్య స్థిరరాశి *b)
{
  నిర్మాణం విలీనం_కణుపు స్థిరరాశి *nodea = a;
  నిర్మాణం విలీనం_కణుపు స్థిరరాశి *nodeb = b;
  ఒకవేళ (nodea->level == nodeb->level)
      తిరిగి (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  తిరిగి nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

స్థిర అంతఃవరుస శూన్య
తాళం_కణుపు (నిర్మాణం విలీనం_కణుపు *కణుపు)
{
  pthread_mutex_తాళం (&కణుపు->తాళం);
}

/* Unlock a merge tree NODE. */

స్థిర అంతఃవరుస శూన్య
unlock_కణుపు (నిర్మాణం విలీనం_కణుపు *కణుపు)
{
  pthread_mutex_unlock (&కణుపు->తాళం);
}

/* Destroy merge QUEUE. */

స్థిర శూన్య
వరుస_destroy (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస)
{
  కుప్ప_free (వరుస->ప్రాధాన్యత_వరుస);
  pthread_cond_destroy (&వరుస->cond);
  pthread_mutex_destroy (&వరుస->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

స్థిర శూన్య
వరుస_init (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  వరుస->ప్రాధాన్యత_వరుస = కుప్ప_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&వరుస->mutex, ఖాళీ);
  pthread_cond_init (&వరుస->cond, ఖాళీ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

స్థిర శూన్య
వరుస_చొప్పించు (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస, నిర్మాణం విలీనం_కణుపు *కణుపు)
{
  pthread_mutex_తాళం (&వరుస->mutex);
  కుప్ప_చొప్పించు (వరుస->ప్రాధాన్యత_వరుస, కణుపు);
  కణుపు->queued = true;
  pthread_cond_signal (&వరుస->cond);
  pthread_mutex_unlock (&వరుస->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

స్థిర నిర్మాణం విలీనం_కణుపు *
వరుస_తీయి (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస)
{
  నిర్మాణం విలీనం_కణుపు *కణుపు;
  pthread_mutex_తాళం (&వరుస->mutex);
  అయితే (! (కణుపు = కుప్ప_remove_శిఖరం (వరుస->ప్రాధాన్యత_వరుస)))
    pthread_cond_wait (&వరుస->cond, &వరుస->mutex);
  pthread_mutex_unlock (&వరుస->mutex);
  తాళం_కణుపు (కణుపు);
  కణుపు->queued = false;
  తిరిగి కణుపు;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

స్థిర శూన్య
write_unique (నిర్మాణం పంక్తి స్థిరరాశి *పంక్తి, FILE *tfp, అక్షరం స్థిరరాశి *తాత్కాలిక_output)
{
  ఒకవేళ (unique)
    {
      ఒకవేళ (saved_పంక్తి.పాఠ్యం && ! compare (పంక్తి, &saved_పంక్తి))
        తిరిగి;
      saved_పంక్తి = *పంక్తి;
    }

  write_పంక్తి (పంక్తి, tfp, తాత్కాలిక_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

స్థిర శూన్య
mergelines_కణుపు (నిర్మాణం విలీనం_కణుపు *పరిమిత కణుపు, size_t మొత్తం_పంక్తులు,
                 FILE *tfp, అక్షరం స్థిరరాశి *తాత్కాలిక_output)
{
  నిర్మాణం పంక్తి *lo_orig = కణుపు->lo;
  నిర్మాణం పంక్తి *hi_orig = కణుపు->hi;
  size_t గమ్యం_విలీనం = MAX_విలీనం (మొత్తం_పంక్తులు, కణుపు->level);
  size_t merged_lo;
  size_t merged_hi;

  ఒకవేళ (కణుపు->level > విలీనం_మూలం)
    {
      /* Merge to destination buffer. */
      నిర్మాణం పంక్తి *dest = *కణుపు->dest;
      అయితే (కణుపు->lo != కణుపు->చివర_lo && కణుపు->hi != కణుపు->చివర_hi && గమ్యం_విలీనం--)
        ఒకవేళ (compare (కణుపు->lo - 1, కణుపు->hi - 1) <= 0)
          *--dest = *--కణుపు->lo;
        లేకపోతే
          *--dest = *--కణుపు->hi;

      merged_lo = lo_orig - కణుపు->lo;
      merged_hi = hi_orig - కణుపు->hi;

      ఒకవేళ (కణుపు->nhi == merged_hi)
        అయితే (కణుపు->lo != కణుపు->చివర_lo && గమ్యం_విలీనం--)
          *--dest = *--కణుపు->lo;
      లేకపోతే ఒకవేళ (కణుపు->nlo == merged_lo)
        అయితే (కణుపు->hi != కణుపు->చివర_hi && గమ్యం_విలీనం--)
          *--dest = *--కణుపు->hi;
      *కణుపు->dest = dest;
    }
  లేకపోతే
    {
      /* Merge directly to output. */
      అయితే (కణుపు->lo != కణుపు->చివర_lo && కణుపు->hi != కణుపు->చివర_hi && గమ్యం_విలీనం--)
        {
          ఒకవేళ (compare (కణుపు->lo - 1, కణుపు->hi - 1) <= 0)
            write_unique (--కణుపు->lo, tfp, తాత్కాలిక_output);
          లేకపోతే
            write_unique (--కణుపు->hi, tfp, తాత్కాలిక_output);
        }

      merged_lo = lo_orig - కణుపు->lo;
      merged_hi = hi_orig - కణుపు->hi;

      ఒకవేళ (కణుపు->nhi == merged_hi)
        {
          అయితే (కణుపు->lo != కణుపు->చివర_lo && గమ్యం_విలీనం--)
            write_unique (--కణుపు->lo, tfp, తాత్కాలిక_output);
        }
      లేకపోతే ఒకవేళ (కణుపు->nlo == merged_lo)
        {
          అయితే (కణుపు->hi != కణుపు->చివర_hi && గమ్యం_విలీనం--)
            write_unique (--కణుపు->hi, tfp, తాత్కాలిక_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - కణుపు->lo;
  merged_hi = hi_orig - కణుపు->hi;
  కణుపు->nlo -= merged_lo;
  కణుపు->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

స్థిర శూన్య
వరుస_check_చొప్పించు (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస, నిర్మాణం విలీనం_కణుపు *కణుపు)
{
  ఒకవేళ (! కణుపు->queued)
    {
      bool lo_avail = (కణుపు->lo - కణుపు->చివర_lo) != 0;
      bool hi_avail = (కణుపు->hi - కణుపు->చివర_hi) != 0;
      ఒకవేళ (lo_avail ? hi_avail || ! కణుపు->nhi : hi_avail && ! కణుపు->nlo)
        వరుస_చొప్పించు (వరుస, కణుపు);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

స్థిర శూన్య
వరుస_check_చొప్పించు_మాతృ (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస,
                           నిర్మాణం విలీనం_కణుపు *కణుపు)
{
  ఒకవేళ (కణుపు->level > విలీనం_మూలం)
    {
      తాళం_కణుపు (కణుపు->మాతృ);
      వరుస_check_చొప్పించు (వరుస, కణుపు->మాతృ);
      unlock_కణుపు (కణుపు->మాతృ);
    }
  లేకపోతే ఒకవేళ (కణుపు->nlo + కణుపు->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      వరుస_చొప్పించు (వరుస, కణుపు->మాతృ);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

స్థిర శూన్య
విలీనం_loop (నిర్మాణం విలీనం_కణుపు_వరుస *వరుస,
            size_t మొత్తం_పంక్తులు, FILE *tfp, అక్షరం స్థిరరాశి *తాత్కాలిక_output)
{
  అయితే (true)
    {
      నిర్మాణం విలీనం_కణుపు *కణుపు = వరుస_తీయి (వరుస);

      ఒకవేళ (కణుపు->level == విలీనం_చివర)
        {
          unlock_కణుపు (కణుపు);
          /* Reinsert so other threads can pop it. */
          వరుస_చొప్పించు (వరుస, కణుపు);
          ఆపు;
        }
      mergelines_కణుపు (కణుపు, మొత్తం_పంక్తులు, tfp, తాత్కాలిక_output);
      వరుస_check_చొప్పించు (వరుస, కణుపు);
      వరుస_check_చొప్పించు_మాతృ (వరుస, కణుపు);

      unlock_కణుపు (కణుపు);
    }
}


స్థిర శూన్య sortlines (నిర్మాణం పంక్తి *పరిమిత, size_t, size_t,
                       నిర్మాణం విలీనం_కణుపు *, నిర్మాణం విలీనం_కణుపు_వరుస *,
                       FILE *, అక్షరం స్థిరరాశి *);

/* Thread arguments for sortlines_thread. */

నిర్మాణం thread_వాదనలు
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  నిర్మాణం పంక్తి *పంక్తులు;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t స్థిరరాశి మొత్తం_పంక్తులు;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  నిర్మాణం విలీనం_కణుపు *స్థిరరాశి కణుపు;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  నిర్మాణం విలీనం_కణుపు_వరుస *స్థిరరాశి వరుస;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  అక్షరం స్థిరరాశి *output_తాత్కాలిక;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

స్థిర శూన్య *
sortlines_thread (శూన్య *దత్తాంశం)
{
  నిర్మాణం thread_వాదనలు స్థిరరాశి *వాదనలు = దత్తాంశం;
  sortlines (వాదనలు->పంక్తులు, వాదనలు->nthreads, వాదనలు->మొత్తం_పంక్తులు,
             వాదనలు->కణుపు, వాదనలు->వరుస, వాదనలు->tfp,
             వాదనలు->output_తాత్కాలిక);
  తిరిగి ఖాళీ;
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

స్థిర శూన్య
sortlines (నిర్మాణం పంక్తి *పరిమిత పంక్తులు, size_t nthreads,
           size_t మొత్తం_పంక్తులు, నిర్మాణం విలీనం_కణుపు *కణుపు,
           నిర్మాణం విలీనం_కణుపు_వరుస *వరుస, FILE *tfp, అక్షరం స్థిరరాశి *తాత్కాలిక_output)
{
  size_t nlines = కణుపు->nlo + కణుపు->nhi;

  /* Calculate thread arguments. */
  size_t lo_దారాలు = nthreads / 2;
  size_t hi_దారాలు = nthreads - lo_దారాలు;
  pthread_t thread;
  నిర్మాణం thread_వాదనలు వాదనలు = {పంక్తులు, lo_దారాలు, మొత్తం_పంక్తులు,
                             కణుపు->lo_బిడ్డ, వరుస, tfp, తాత్కాలిక_output};

  ఒకవేళ (nthreads > 1 && SUBTHREAD_పంక్తులు_HEURISTIC <= nlines
      && pthread_create (&thread, ఖాళీ, sortlines_thread, &వాదనలు) == 0)
    {
      sortlines (పంక్తులు - కణుపు->nlo, hi_దారాలు, మొత్తం_పంక్తులు,
                 కణుపు->hi_బిడ్డ, వరుస, tfp, తాత్కాలిక_output);
      pthread_join (thread, ఖాళీ);
    }
  లేకపోతే
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = కణుపు->nlo;
      size_t nhi = కణుపు->nhi;
      నిర్మాణం పంక్తి *తాత్కాలిక = పంక్తులు - మొత్తం_పంక్తులు;
      ఒకవేళ (1 < nhi)
        sequential_క్రమీకరించు (పంక్తులు - nlo, nhi, తాత్కాలిక - nlo / 2, false);
      ఒకవేళ (1 < nlo)
        sequential_క్రమీకరించు (పంక్తులు, nlo, తాత్కాలిక, false);

      /* Update merge NODE. No need to lock yet. */
      కణుపు->lo = పంక్తులు;
      కణుపు->hi = పంక్తులు - nlo;
      కణుపు->చివర_lo = పంక్తులు - nlo;
      కణుపు->చివర_hi = పంక్తులు - nlo - nhi;

      వరుస_చొప్పించు (వరుస, కణుపు);
      విలీనం_loop (వరుస, మొత్తం_పంక్తులు, tfp, తాత్కాలిక_output);
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

స్థిర శూన్య
avoid_trashing_ఆదానం (నిర్మాణం sortfile *files, size_t ntemps,
                      size_t nfiles, అక్షరం స్థిరరాశి *outfile)
{
  నిర్మాణం tempnode *tempcopy = ఖాళీ;

  కొరకు (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      నిర్మాణం stat instat;

      ఒకవేళ (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      లేకపోతే
        {
          నిర్మాణం stat *outst = get_outstatus ();
          ఒకవేళ (!outst)
            ఆపు;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ఒకవేళ (same)
        {
          ఒకవేళ (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_తాత్కాలిక (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].తాత్కాలిక = tempcopy;
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

స్థిర శూన్య
check_inputs (అక్షరం *స్థిరరాశి *files, size_t nfiles)
{
  కొరకు (size_t i = 0; i < nfiles; i++)
    {
      ఒకవేళ (streq (files[i], "-"))
        కొనసాగు;

      ఒకవేళ (euidaccess (files[i], R_OK) != 0)
        క్రమీకరించు_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

స్థిర శూన్య
check_output (అక్షరం స్థిరరాశి *outfile)
{
  ఒకవేళ (outfile)
    {
      పూర్ణాంకం oflags = O_WRONLY | O_ద్విమాన | O_CLOEXEC | O_CREAT;
      పూర్ణాంకం outfd = open (outfile, oflags, బహుళకం_RW_UGO);
      ఒకవేళ (outfd < 0)
        క్రమీకరించు_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

స్థిర శూన్య
విలీనం (నిర్మాణం sortfile *files, size_t ntemps, size_t nfiles,
       అక్షరం స్థిరరాశి *output_file)
{
  అయితే (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t లోపల;

      /* Number of output files generated so far.  */
      size_t బహిర్గతం;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      కొరకు (బహిర్గతం = లోపల = 0; nmerge <= nfiles - లోపల; బహిర్గతం++)
        {
          FILE *tfp;
          నిర్మాణం tempnode *తాత్కాలిక = create_తాత్కాలిక (&tfp);
          size_t అంకె_merged = mergefiles (&files[లోపల], MIN (ntemps, nmerge),
                                          nmerge, tfp, తాత్కాలిక->name);
          ntemps -= MIN (ntemps, అంకె_merged);
          files[బహిర్గతం].name = తాత్కాలిక->name;
          files[బహిర్గతం].తాత్కాలిక = తాత్కాలిక;
          లోపల += అంకె_merged;
        }

      remainder = nfiles - లోపల;
      cheap_slots = nmerge - బహిర్గతం % nmerge;

      ఒకవేళ (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          నిర్మాణం tempnode *తాత్కాలిక = create_తాత్కాలిక (&tfp);
          size_t అంకె_merged = mergefiles (&files[లోపల], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, తాత్కాలిక->name);
          ntemps -= MIN (ntemps, అంకె_merged);
          files[బహిర్గతం].name = తాత్కాలిక->name;
          files[బహిర్గతం++].తాత్కాలిక = తాత్కాలిక;
          లోపల += అంకె_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[బహిర్గతం], &files[లోపల], (nfiles - లోపల) * పరిమాణం *files);
      ntemps += బహిర్గతం;
      nfiles -= లోపల - బహిర్గతం;
    }

  avoid_trashing_ఆదానం (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  అయితే (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ఆదానం_files (files, nfiles, &fps);

      ఒకవేళ (nopened == nfiles)
        {
          FILE *ofp = ప్రవాహం_open (output_file, "w");
          ఒకవేళ (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ఆపు;
            }
          ఒకవేళ (errno != EMFILE || nopened <= 2)
            క్రమీకరించు_die (_("open failed"), output_file);
        }
      లేకపోతే ఒకవేళ (nopened <= 2)
        క్రమీకరించు_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      నిర్మాణం tempnode *తాత్కాలిక;
      చేయి
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          తాత్కాలిక = maybe_create_తాత్కాలిక (&tfp, ! (nopened <= 2));
        }
      అయితే (!తాత్కాలిక);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, తాత్కాలిక->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = తాత్కాలిక->name;
      files[0].తాత్కాలిక = తాత్కాలిక;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * పరిమాణం *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

స్థిర శూన్య
క్రమీకరించు (అక్షరం *స్థిరరాశి *files, size_t nfiles, అక్షరం స్థిరరాశి *output_file,
      size_t nthreads)
{
  నిర్మాణం బఫరు buf;
  నిర్మాణం క్రమీకరించు_బఫరు_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.హద్దు = 0;
  policy.growth_failed = false;

  అయితే (nfiles)
    {
      అక్షరం స్థిరరాశి *తాత్కాలిక_output;
      అక్షరం స్థిరరాశి *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t బైట్లు_చొప్పున_పంక్తి;
      ఒకవేళ (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          అయితే (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          బైట్లు_చొప్పున_పంక్తి = (mult * పరిమాణం (నిర్మాణం పంక్తి));
        }
      లేకపోతే
        బైట్లు_చొప్పున_పంక్తి = పరిమాణం (నిర్మాణం పంక్తి) * 3 / 2;

      ఒకవేళ (! buf.alloc)
        {
          క్రమీకరించు_బఫరు_policy (&fp, 1, files, nfiles, బైట్లు_చొప్పున_పంక్తి,
                              &policy);
          initbuf (&buf, బైట్లు_చొప్పున_పంక్తి, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      అయితే (fillbuf (&buf, fp, file, &policy))
        {
          నిర్మాణం పంక్తి *పంక్తి;

          ఒకవేళ (buf.eof && nfiles
              && (బైట్లు_చొప్పున_పంక్తి + 1
                  < (buf.alloc - buf.వాడిన - బైట్లు_చొప్పున_పంక్తి * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ఎడమ = buf.వాడిన;
              ఆపు;
            }

          saved_పంక్తి.పాఠ్యం = ఖాళీ;
          పంక్తి = బఫరు_linelim (&buf);
          ఒకవేళ (buf.eof && !nfiles && !ntemps && !buf.ఎడమ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              తాత్కాలిక_output = output_file;
              output_file_created = true;
            }
          లేకపోతే
            {
              ++ntemps;
              తాత్కాలిక_output = create_తాత్కాలిక (&tfp)->name;
            }
          ఒకవేళ (1 < buf.nlines)
            {
              ఒకవేళ (nthreads > 1)
                {
                  నిర్మాణం విలీనం_కణుపు_వరుస వరుస;
                  వరుస_init (&వరుస, nthreads);
                  నిర్మాణం విలీనం_కణుపు *విలీనం_వృక్షం =
                    విలీనం_వృక్షం_init (nthreads, buf.nlines, పంక్తి);

                  sortlines (పంక్తి, nthreads, buf.nlines, విలీనం_వృక్షం + 1,
                             &వరుస, tfp, తాత్కాలిక_output);

                  విలీనం_వృక్షం_destroy (nthreads, విలీనం_వృక్షం);
                  వరుస_destroy (&వరుస);
                }
              లేకపోతే
                {
                  sequential_క్రమీకరించు (పంక్తి, buf.nlines,
                                   పంక్తి - buf.nlines, false);
                  కొరకు (size_t i = 0; i < buf.nlines; i++)
                    write_unique (పంక్తి - i - 1, tfp, తాత్కాలిక_output);
                }
            }
          లేకపోతే
            write_unique (పంక్తి - 1, tfp, తాత్కాలిక_output);

          xfclose (tfp, తాత్కాలిక_output);

          ఒకవేళ (output_file_created)
            వెళ్ళు ముగింపు;
        }
      xfclose (fp, file);
    }

 ముగింపు:
  విడుదల (buf.buf);

  ఒకవేళ (! output_file_created)
    {
      నిర్మాణం tempnode *కణుపు = temphead;
      నిర్మాణం sortfile *tempfiles = xnmalloc (ntemps, పరిమాణం *tempfiles);
      కొరకు (size_t i = 0; కణుపు; i++)
        {
          tempfiles[i].name = కణుపు->name;
          tempfiles[i].తాత్కాలిక = కణుపు;
          కణుపు = కణుపు->తదుపరి;
        }
      విలీనం (tempfiles, ntemps, ntemps, output_file);
      విడుదల (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

స్థిర శూన్య
insertkey (నిర్మాణం keyfield *కీలకం_arg)
{
  నిర్మాణం keyfield **p;
  నిర్మాణం keyfield *కీలకం = xmemdup (కీలకం_arg, పరిమాణం *కీలకం);

  కొరకు (p = &keylist; *p; p = &(*p)->తదుపరి)
    కొనసాగు;
  *p = కీలకం;
  కీలకం->తదుపరి = ఖాళీ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

స్థిర శూన్య
badfieldspec (అక్షరం స్థిరరాశి *spec, అక్షరం స్థిరరాశి *msgid)
{
  error (క్రమీకరించు_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

స్థిర శూన్య
incompatible_options (అక్షరం స్థిరరాశి *opts)
{
  error (క్రమీకరించు_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

స్థిర శూన్య
check_ordering_compatibility (శూన్య)
{
  కొరకు (నిర్మాణం keyfield *కీలకం = keylist; కీలకం; కీలకం = కీలకం->తదుపరి)
    ఒకవేళ (1 < (కీలకం->numeric + కీలకం->general_numeric + కీలకం->human_numeric
             + కీలకం->month + (కీలకం->సంస్కరణ_3 | కీలకం->random | !!కీలకం->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        అక్షరం opts[పరిమాణం short_options];
        /* Clear flags we're not interested in.  */
        కీలకం->skipsblanks = కీలకం->skipeblanks = కీలకం->తిరగేయి = false;
        కీలకం_గమ్యం_opts (కీలకం, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

స్థిర అక్షరం స్థిరరాశి *
parse_క్షేత్రం_లెక్క (అక్షరం స్థిరరాశి *string, size_t *val, అక్షరం స్థిరరాశి *msgid)
{
  అక్షరం *suffix;
  uintmax_t n;

  ఎంపిక (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    స్థితి LONGINT_OK:
    స్థితి LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ఒకవేళ (*val == n)
        ఆపు;
      FALLTHROUGH;
    స్థితి LONGINT_అధికప్రవాహం:
    స్థితి LONGINT_అధికప్రవాహం | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ఆపు;

    స్థితి LONGINT_INVALID:
      ఒకవేళ (msgid)
        error (క్రమీకరించు_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      తిరిగి ఖాళీ;
    }

  తిరిగి suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

స్థిర అక్షరం *
సమితి_ordering (అక్షరం స్థిరరాశి *s, నిర్మాణం keyfield *కీలకం, గణన blanktype blanktype)
{
  అయితే (*s)
    {
      ఎంపిక (*s)
        {
        స్థితి 'b':
          ఒకవేళ (blanktype == bl_ప్రారంభం || blanktype == bl_both)
            కీలకం->skipsblanks = true;
          ఒకవేళ (blanktype == bl_చివర || blanktype == bl_both)
            కీలకం->skipeblanks = true;
          ఆపు;
        స్థితి 'd':
          కీలకం->ignore = nondictionary;
          ఆపు;
        స్థితి 'f':
          కీలకం->translate = fold_toupper;
          ఆపు;
        స్థితి 'g':
          కీలకం->general_numeric = true;
          ఆపు;
        స్థితి 'h':
          కీలకం->human_numeric = true;
          ఆపు;
        స్థితి 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ఒకవేళ (! కీలకం->ignore)
            కీలకం->ignore = nonprinting;
          ఆపు;
        స్థితి 'M':
          కీలకం->month = true;
          ఆపు;
        స్థితి 'n':
          కీలకం->numeric = true;
          ఆపు;
        స్థితి 'R':
          కీలకం->random = true;
          ఆపు;
        స్థితి 'r':
          కీలకం->తిరగేయి = true;
          ఆపు;
        స్థితి 'V':
          కీలకం->సంస్కరణ_3 = true;
          ఆపు;
        అప్రమేయ:
          తిరిగి (అక్షరం *) s;
        }
      ++s;
    }
  తిరిగి (అక్షరం *) s;
}

/* Initialize KEY.  */

స్థిర నిర్మాణం keyfield *
కీలకం_init (నిర్మాణం keyfield *కీలకం)
{
  memset (కీలకం, 0, పరిమాణం *కీలకం);
  కీలకం->eword = SIZE_MAX;
  తిరిగి కీలకం;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  నిర్మాణం keyfield *కీలకం;
  నిర్మాణం keyfield కీలకం_buf;
  నిర్మాణం keyfield gkey;
  bool gkey_only = false;
  అక్షరం స్థిరరాశి *s;
  పూర్ణాంకం c = 0;
  అక్షరం checkonly = 0;
  bool mergeonly = false;
  అక్షరం *random_ఆధారం = ఖాళీ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ఖాళీ);
  పూర్ణాంకం posix_ver = posix2_సంస్కరణ ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  అక్షరం **files;
  అక్షరం *files_from = ఖాళీ;
  నిర్మాణం టోకెన్లు tok;
  అక్షరం స్థిరరాశి *outfile = ఖాళీ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (క్రమీకరించు_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ఒకవేళ HAVE_NL_LANGINFO
  hard_LC_సమయం = hard_locale (LC_సమయం);
#అయితే_ముగింపు

  /* Get locale's representation of the decimal point.  */
  {
    నిర్మాణం lconv స్థిరరాశి *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_బిందువు = locale->decimal_బిందువు[0];
    ఒకవేళ (! decimal_బిందువు || locale->decimal_బిందువు[1])
      decimal_బిందువు = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ఒకవేళ (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ఒకవేళ (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    గణన { nsigs = countof (పదం_sig) };

    నిర్మాణం sigaction act;

    sigemptyset (&caught_సంకేతాలు);
    కొరకు (size_t i = 0; i < nsigs; i++)
      {
        ఒకవేళ (పదం_sig[i] == SIGPIPE)
          కొనసాగు;  /* Handled below.  */
        sigaction (పదం_sig[i], ఖాళీ, &act);
        ఒకవేళ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_సంకేతాలు, పదం_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_సంకేతాలు;
    act.sa_flags = 0;

    కొరకు (size_t i = 0; i < nsigs; i++)
      {
        ఒకవేళ (పదం_sig[i] == SIGPIPE)
          కొనసాగు;  /* Handled below.  */
        ఒకవేళ (sigismember (&caught_సంకేతాలు, పదం_sig[i]))
          sigaction (పదం_sig[i], &act, ఖాళీ);
      }
  }
  సంకేతం (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  కీలకం_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, పరిమాణం *files);

  అయితే (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      పూర్ణాంకం oi = -1;

      ఒకవేళ (c == -1
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
          ఒకవేళ (argc <= optind)
            ఆపు;
          files[nfiles++] = argv[optind++];
        }
      లేకపోతే ఎంపిక (c)
        {
        స్థితి 1:
          కీలకం = ఖాళీ;
          ఒకవేళ (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ఒకవేళ (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  కీలకం = కీలకం_init (&కీలకం_buf);
                  s = parse_క్షేత్రం_లెక్క (optarg + 1, &కీలకం->sword, ఖాళీ);
                  ఒకవేళ (s && *s == '.')
                    s = parse_క్షేత్రం_లెక్క (s + 1, &కీలకం->schar, ఖాళీ);
                  ఒకవేళ (! (కీలకం->sword || కీలకం->schar))
                    కీలకం->sword = SIZE_MAX;
                  ఒకవేళ (! s || *సమితి_ordering (s, కీలకం, bl_ప్రారంభం))
                    కీలకం = ఖాళీ;
                  లేకపోతే
                    {
                      ఒకవేళ (minus_pos_usage)
                        {
                          అక్షరం స్థిరరాశి *optarg1 = argv[optind++];
                          s = parse_క్షేత్రం_లెక్క (optarg1 + 1, &కీలకం->eword,
                                             N_("invalid number after '-'"));
                          ఒకవేళ (*s == '.')
                            s = parse_క్షేత్రం_లెక్క (s + 1, &కీలకం->echar,
                                               N_("invalid number after '.'"));
                          ఒకవేళ (!కీలకం->echar && కీలకం->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              కీలకం->eword--;
                            }
                          ఒకవేళ (*సమితి_ordering (s, కీలకం, bl_చివర))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      కీలకం->traditional_వాడిన = true;
                      insertkey (కీలకం);
                    }
                }
            }
          ఒకవేళ (! కీలకం)
            files[nfiles++] = optarg;
          ఆపు;

        స్థితి క్రమీకరించు_OPTION:
          c = XARGMATCH ("--sort", optarg, క్రమీకరించు_వాదనలు, క్రమీకరించు_types);
          FALLTHROUGH;
        స్థితి 'b':
        స్థితి 'd':
        స్థితి 'f':
        స్థితి 'g':
        స్థితి 'h':
        స్థితి 'i':
        స్థితి 'M':
        స్థితి 'n':
        స్థితి 'r':
        స్థితి 'R':
        స్థితి 'V':
          {
            అక్షరం str[2];
            str[0] = c;
            str[1] = '\0';
            సమితి_ordering (str, &gkey, bl_both);
          }
          ఆపు;

        స్థితి CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_వాదనలు, check_types)
               : 'c');
          FALLTHROUGH;
        స్థితి 'c':
        స్థితి 'C':
          ఒకవేళ (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ఆపు;

        స్థితి COMPRESS_కార్యక్రమం_OPTION:
          ఒకవేళ (compress_కార్యక్రమం && !streq (compress_కార్యక్రమం, optarg))
            error (క్రమీకరించు_FAILURE, 0, _("multiple compress programs specified"));
          compress_కార్యక్రమం = optarg;
          ఆపు;

        స్థితి DEBUG_కార్యక్రమం_OPTION:
          debug = true;
          ఆపు;

        స్థితి FILES0_FROM_OPTION:
          files_from = optarg;
          ఆపు;

        స్థితి 'k':
          కీలకం = కీలకం_init (&కీలకం_buf);

          /* Get POS1. */
          s = parse_క్షేత్రం_లెక్క (optarg, &కీలకం->sword,
                                 N_("invalid number at field start"));
          ఒకవేళ (! కీలకం->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ఒకవేళ (*s == '.')
            {
              s = parse_క్షేత్రం_లెక్క (s + 1, &కీలకం->schar,
                                     N_("invalid number after '.'"));
              ఒకవేళ (! కీలకం->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ఒకవేళ (! (కీలకం->sword || కీలకం->schar))
            కీలకం->sword = SIZE_MAX;
          s = సమితి_ordering (s, కీలకం, bl_ప్రారంభం);
          ఒకవేళ (*s != ',')
            {
              కీలకం->eword = SIZE_MAX;
              కీలకం->echar = 0;
            }
          లేకపోతే
            {
              /* Get POS2. */
              s = parse_క్షేత్రం_లెక్క (s + 1, &కీలకం->eword,
                                     N_("invalid number after ','"));
              ఒకవేళ (! కీలకం->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ఒకవేళ (*s == '.')
                {
                  s = parse_క్షేత్రం_లెక్క (s + 1, &కీలకం->echar,
                                         N_("invalid number after '.'"));
                }
              s = సమితి_ordering (s, కీలకం, bl_చివర);
            }
          ఒకవేళ (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (కీలకం);
          ఆపు;

        స్థితి 'm':
          mergeonly = true;
          ఆపు;

        స్థితి NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ఆపు;

        స్థితి 'o':
          ఒకవేళ (outfile && !streq (outfile, optarg))
            error (క్రమీకరించు_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ఆపు;

        స్థితి RANDOM_ఆధారం_OPTION:
          ఒకవేళ (random_ఆధారం && !streq (random_ఆధారం, optarg))
            error (క్రమీకరించు_FAILURE, 0, _("multiple random sources specified"));
          random_ఆధారం = optarg;
          ఆపు;

        స్థితి 's':
          stable = true;
          ఆపు;

        స్థితి 'S':
          specify_క్రమీకరించు_కొలత (oi, c, optarg);
          ఆపు;

        స్థితి 't':
          {
            అక్షరం newtab = optarg[0];
            ఒకవేళ (! newtab)
              error (క్రమీకరించు_FAILURE, 0, _("empty tab"));
            ఒకవేళ (optarg[1])
              {
                ఒకవేళ (streq (optarg, "\\0"))
                  newtab = '\0';
                లేకపోతే
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (క్రమీకరించు_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ఒకవేళ (tab != TAB_DEFAULT && tab != newtab)
              error (క్రమీకరించు_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ఆపు;

        స్థితి 'T':
          కూడు_తాత్కాలిక_dir (optarg);
          ఆపు;

        స్థితి PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ఆపు;

        స్థితి 'u':
          unique = true;
          ఆపు;

        స్థితి 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ఒకవేళ (optarg == argv[optind - 1])
            {
              అక్షరం స్థిరరాశి *p;
              కొరకు (p = optarg; c_isdigit (*p); p++)
                కొనసాగు;
              optind -= (*p != '\0');
            }
          ఆపు;

        స్థితి 'z':
          eolchar = 0;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (క్రమీకరించు_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (సంకేతం (SIGPIPE, SIG_IGN) == SIG_DFL);

  ఒకవేళ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ఒకవేళ (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (క్రమీకరించు_FAILURE);
        }

      FILE *ప్రవాహం = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ఒకవేళ (! readtokens0 (ప్రవాహం, &tok))
        error (క్రమీకరించు_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (ప్రవాహం, files_from);

      ఒకవేళ (tok.n_tok)
        {
          విడుదల (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          కొరకు (size_t i = 0; i < nfiles; i++)
            {
              ఒకవేళ (streq (files[i], "-"))
                error (క్రమీకరించు_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              లేకపోతే ఒకవేళ (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  గుర్తులేని దీర్ఘ పూర్ణాంకం file_సంఖ్య = i + 1;
                  error (క్రమీకరించు_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_సంఖ్య);
                }
            }
        }
      లేకపోతే
        error (క్రమీకరించు_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  కొరకు (కీలకం = keylist; కీలకం; కీలకం = కీలకం->తదుపరి)
    {
      ఒకవేళ (default_కీలకం_compare (కీలకం) && !కీలకం->తిరగేయి)
        {
          కీలకం->ignore = gkey.ignore;
          కీలకం->translate = gkey.translate;
          కీలకం->skipsblanks = gkey.skipsblanks;
          కీలకం->skipeblanks = gkey.skipeblanks;
          కీలకం->month = gkey.month;
          కీలకం->numeric = gkey.numeric;
          కీలకం->general_numeric = gkey.general_numeric;
          కీలకం->human_numeric = gkey.human_numeric;
          కీలకం->సంస్కరణ_3 = gkey.సంస్కరణ_3;
          కీలకం->random = gkey.random;
          కీలకం->తిరగేయి = gkey.తిరగేయి;
        }

      need_random |= కీలకం->random;
    }

  ఒకవేళ (!keylist && !default_కీలకం_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ఒకవేళ (debug)
    {
      ఒకవేళ (checkonly || outfile)
        {
          స్థిర అక్షరం opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ఒకవేళ (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ఒకవేళ (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ఒకవేళ (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ఖాళీ)));
      లేకపోతే
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      కీలకం_warnings (&gkey, gkey_only);
    }

  తిరగేయి = gkey.తిరగేయి;

  ఒకవేళ (need_random)
    random_md5_state_init (random_ఆధారం);

  ఒకవేళ (తాత్కాలిక_dirs.n_dirs == 0)
    {
      అక్షరం స్థిరరాశి *tmp_dir = getenv ("TMPDIR");
      కూడు_తాత్కాలిక_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ఒకవేళ (nfiles == 0)
    {
      nfiles = 1;
      విడుదల (files);
      files = xmalloc (పరిమాణం *files);
      *files = (అక్షరం *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ఒకవేళ (0 < క్రమీకరించు_కొలత)
    క్రమీకరించు_కొలత = MAX (క్రమీకరించు_కొలత, MIN_క్రమీకరించు_కొలత);

  ఒకవేళ (checkonly)
    {
      ఒకవేళ (nfiles > 1)
        error (క్రమీకరించు_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ఒకవేళ (outfile)
        {
          స్థిర అక్షరం opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      నిష్క్రమించు (check (files[0], checkonly) ? EXIT_SUCCESS : క్రమీకరించు_బహిర్గతం_యొక్క_క్రమం);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  ఒకవేళ (mergeonly)
    {
      నిర్మాణం sortfile *sortfiles = xcalloc (nfiles, పరిమాణం *sortfiles);

      కొరకు (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      విలీనం (sortfiles, 0, nfiles, outfile);
    }
  లేకపోతే
    {
      ఒకవేళ (!nthreads)
        {
          గుర్తులేని దీర్ఘ పూర్ణాంకం np = అంకె_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_దారాలు);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * పరిమాణం (నిర్మాణం విలీనం_కణుపు));
      nthreads = MIN (nthreads, nthreads_max);

      క్రమీకరించు (files, nfiles, outfile, nthreads);
    }

  ఒకవేళ (have_read_stdin && fclose (stdin) == EOF)
    క్రమీకరించు_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
