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

#यदि_अपरिभाषितम् RLIMIT_दत्त
संरचना rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#यदि_अन्तः

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "sort"

#परिभाषय AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#यदि HAVE_LANGINFO_CODESET
# include <langinfo.h>
#यदि_अन्तः

#यदि !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#यदि_अन्तः
#यदि !defined OPEN_MAX
# define OPEN_MAX 20
#यदि_अन्तः

#परिभाषय UCHAR_LIM (UCHAR_MAX + 1)

#यदि_अपरिभाषितम् DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#यदि_अन्तः

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#परिभाषय MAX_MERGE(योग_2, level) (((योग_2) >> (2 * ((level) + 1))) + 1)

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
गणना { DEFAULT_MAX_तन्तवः = 8 };

/* Exit statuses.  */
गणना
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    क्रमय_बहिर्गमनम्_सम्बन्धि_क्रमः = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    क्रमय_FAILURE = 2
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
    MERGE_अन्तः = 0,

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
#यदि HAVE_NL_LANGINFO
स्थिर bool hard_LC_TIME;
#यदि_अन्तः

#परिभाषय NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
गणना blanktype { bl_start, bl_अन्तः, bl_both };

/* The character marking end of line. Default to \n. */
स्थिर वर्ण eolchar = '\n';

/* Lines are held in memory as counted strings. */
संरचना पङ्क्तिः
{
  वर्ण *text;			/* Text of the line. */
  size_t दीर्घता;		/* Length including final newline. */
  वर्ण *keybeg;			/* Start of first key. */
  वर्ण *keylim;			/* Limit of first key. */
};

/* Input buffers. */
संरचना बफर
{
  वर्ण *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t प्रयुक्तम्;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t वामः;			/* Number of bytes left from previous reads. */
  size_t पङ्क्तिः_अष्टकानि;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
संरचना क्रमय_बफर_policy
{
  size_t initial;
  size_t सीमा;
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
  bool संस्करणम्_3;			/* sort by version number */
  bool traditional_प्रयुक्तम्;	/* Traditional key option format is used. */
  संरचना keyfield *अग्र;	/* Next keyfield to try. */
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
  संरचना पङ्क्तिः *अन्तः_lo;          /* End of available lines from LO. */
  संरचना पङ्क्तिः *अन्तः_hi;          /* End of available lines from HI. */
  संरचना पङ्क्तिः **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  संरचना merge_ग्रन्थिः *parent;    /* Parent node. */
  संरचना merge_ग्रन्थिः *lo_child;  /* LO child node. */
  संरचना merge_ग्रन्थिः *hi_child;  /* HI child node. */
  अचिह्नित पूर्णाङ्क level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t तालकम्;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
संरचना merge_ग्रन्थिः_queue
{
  संरचना heap *प्राथमिकता_queue;  /* Priority queue of merge tree nodes. */
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

#परिभाषय MONTHS_प्रत्येकम्_YEAR 12

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
#परिभाषय NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#परिभाषय MIN_MERGE_बफर_आकार (2 + परिमाण (संरचना पङ्क्तिः))

/* Minimum sort size; the code might not work with smaller sizes.  */
#परिभाषय MIN_क्रमय_आकार (nmerge * MIN_MERGE_बफर_आकार)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
स्थिर size_t merge_बफर_आकार = MAX (MIN_MERGE_बफर_आकार, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
स्थिर size_t क्रमय_आकार;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#परिभाषय निवेशः_FILE_आकार_GUESS (128 * 1024)

स्थिर संरचना
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  वर्ण ध्रुव **dirs COUNTED_BY (n_dirs);
} अस्थायी_dirs;



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
स्थिर वर्ण ध्रुव *compress_क्रमादेशः;

/* Annotate the output with extra info to aid the user.  */
स्थिर bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
स्थिर अचिह्नित पूर्णाङ्क nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
स्थिर bool default_SIGPIPE;

/* The list of temporary files. */
संरचना tempnode
{
  संरचना tempnode *चञ्चल अग्र;
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
  प्रति (संरचना tempnode ध्रुव *ग्रन्थिः = temphead; ग्रन्थिः; ग्रन्थिः = ग्रन्थिः->अग्र)
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
क्रमय_die (वर्ण ध्रुव *सन्देशः_2, वर्ण ध्रुव *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  यदि (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (क्रमय_FAILURE, errno, "%s: %s", सन्देशः_2,
         quotef (file ? file : _("standard output")));
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Write sorted concatenation सम्बन्धि all FILE(s) गन्तव्यम् standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding क्रमय कुञ्चिकाः अन्तर् each पङ्क्तिः\n\
"));
      oputs (_("\
  -d, --dictionary-क्रमः\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-पक्ष\n\
         fold अधस्तनम् पक्ष गन्तव्यम् ऊर्ध्वस्थम् पक्ष characters\n\
"));
      oputs (_("\
  -g, --general-numeric-क्रमय\n\
         compare according गन्तव्यम् general numerical मानम्\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-क्रमय\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-क्रमय\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-क्रमय\n\
         compare according गन्तव्यम् string numerical मानम्;\n\
         see पूर्णम् documentation प्रति supported वाक्यानि\n\
"));
      oputs (_("\
  -R, --random-क्रमय\n\
         shuffle, but गणः identical कुञ्चिकाः.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-source=FILE\n\
         get random अष्टकानि from FILE\n\
"));
      oputs (_("\
  -r, --reverse\n\
         reverse the परिणाम सम्बन्धि comparisons\n\
"));
      oputs (_("\
      --क्रमय=WORD\n\
         क्रमय according गन्तव्यम् WORD:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, संस्करणम्_3 -V\n\
"));
      oputs (_("\
  -V, --संस्करणम्_3-क्रमय\n\
         natural क्रमय सम्बन्धि (संस्करणम्_3) numbers within text\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-आकार=NMERGE\n\
         merge at most NMERGE inputs at once; प्रति अधिकम् use अस्थायी files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-first\n\
         check प्रति sorted निवेशः; कुरु not क्रमय\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but कुरु not report first bad पङ्क्तिः\n\
"));
      oputs (_("\
      --compress-क्रमादेशः=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part सम्बन्धि the पङ्क्तिः प्रयुक्तम् गन्तव्यम् क्रमय,\n\
         and warn about questionable usage गन्तव्यम् standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         पठ निवेशः from the files specified by NUL-terminated नामानि अन्तर् file F;\
\n\
         If F is -, पठ नामानि from standard निवेशः\n\
"));
      oputs (_("\
  -k, --कुञ्चिका=KEYDEF\n\
         क्रमय द्वारा a कुञ्चिका; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --merge\n\
         merge already sorted files; कुरु not क्रमय\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         लिख परिणाम गन्तव्यम् FILE instead सम्बन्धि standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize क्रमय by disabling चरमः-resort comparison\n\
"));
      oputs (_("\
  -S, --बफर-आकार=आकार_2\n\
         use आकार_2 प्रति मुख्यम् स्मृतिः बफर\n\
"));
      oputs (_("\
  -t, --क्षेत्रम्-separator=SEP\n\
         use SEP instead सम्बन्धि non-blank गन्तव्यम् blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR प्रति temporaries, not $TMPDIR or %s;\n\
         गुणजः options specify गुणजः directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the सङ्ख्या सम्बन्धि sorts सततम्_2 concurrently गन्तव्यम् N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the first सम्बन्धि पङ्क्तयः with equal कुञ्चिकाः;\n\
         with -c, check प्रति strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         पङ्क्तिः delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] प्रति start and stop position, where F is a\n\
क्षेत्रम् सङ्ख्या and C a character position अन्तर् the क्षेत्रम्; both are origin 1, and\n\
the stop position defaults गन्तव्यम् the पङ्क्तिः's अन्तः.  If neither -t nor -b is अन्तर्\n\
effect, characters अन्तर् a क्षेत्रम् are counted from the beginning सम्बन्धि the preceding\n\
whitespace.  OPTS is one or अधिकम् single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options प्रति that कुञ्चिका.  If no कुञ्चिका is given, use\n\
the entire पङ्क्तिः as the कुञ्चिका.  Use --debug गन्तव्यम् diagnose incorrect कुञ्चिका usage.\n\
\n\
आकार_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% सम्बन्धि स्मृतिः, b 1, K 1024 (स्वभाव), and so on प्रति M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects क्रमय क्रमः.\n\
समुच्चयः_3 LC_ALL=C गन्तव्यम् get the traditional क्रमय क्रमः that उपयुङ्क्ते\n\
native byte मानानि.\n\
"), stdout );
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_क्रमादेशः_OPTION,
  DEBUG_क्रमादेशः_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_SOURCE_OPTION,
  क्रमय_OPTION,
  PARALLEL_OPTION
};

स्थिर वर्ण ध्रुव short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"ignore-leading-blanks", no_तर्कः, रिक्त, 'b'},
  {"check", optional_तर्कः, रिक्त, CHECK_OPTION},
  {"compress-program", required_तर्कः, रिक्त, COMPRESS_क्रमादेशः_OPTION},
  {"debug", no_तर्कः, रिक्त, DEBUG_क्रमादेशः_OPTION},
  {"dictionary-order", no_तर्कः, रिक्त, 'd'},
  {"ignore-case", no_तर्कः, रिक्त, 'f'},
  {"files0-from", required_तर्कः, रिक्त, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_तर्कः, रिक्त, 'g'},
  {"ignore-nonprinting", no_तर्कः, रिक्त, 'i'},
  {"key", required_तर्कः, रिक्त, 'k'},
  {"merge", no_तर्कः, रिक्त, 'm'},
  {"month-sort", no_तर्कः, रिक्त, 'M'},
  {"numeric-sort", no_तर्कः, रिक्त, 'n'},
  {"human-numeric-sort", no_तर्कः, रिक्त, 'h'},
  {"version-sort", no_तर्कः, रिक्त, 'V'},
  {"random-sort", no_तर्कः, रिक्त, 'R'},
  {"random-source", required_तर्कः, रिक्त, RANDOM_SOURCE_OPTION},
  {"sort", required_तर्कः, रिक्त, क्रमय_OPTION},
  {"output", required_तर्कः, रिक्त, 'o'},
  {"reverse", no_तर्कः, रिक्त, 'r'},
  {"stable", no_तर्कः, रिक्त, 's'},
  {"batch-size", required_तर्कः, रिक्त, NMERGE_OPTION},
  {"buffer-size", required_तर्कः, रिक्त, 'S'},
  {"field-separator", required_तर्कः, रिक्त, 't'},
  {"temporary-directory", required_तर्कः, रिक्त, 'T'},
  {"unique", no_तर्कः, रिक्त, 'u'},
  {"zero-terminated", no_तर्कः, रिक्त, 'z'},
  {"parallel", required_तर्कः, रिक्त, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0},
};

#परिभाषय CHECK_सारणी \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

स्थिर वर्ण ध्रुव *ध्रुव check_तर्काः[] =
{
#परिभाषय _ct_(_s, _c) _s,
  CHECK_सारणी रिक्त
#अपरिभाषय  _ct_
};
स्थिर वर्ण ध्रुव check_types[] =
{
#परिभाषय _ct_(_s, _c) _c,
  CHECK_सारणी
#अपरिभाषय  _ct_
};

#परिभाषय क्रमय_सारणी \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

स्थिर वर्ण ध्रुव *ध्रुव क्रमय_तर्काः[] =
{
#परिभाषय _st_(_s, _c) _s,
  क्रमय_सारणी रिक्त
#अपरिभाषय  _st_
};
स्थिर वर्ण ध्रुव क्रमय_types[] =
{
#परिभाषय _st_(_s, _c) _c,
  क्रमय_सारणी
#अपरिभाषय  _st_
};

/* The set of signals that are caught.  */
स्थिर sigset_t caught_सङ्केताः;

/* Critical section status.  */
संरचना cs_स्थितिः
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
स्थिर शून्य
cs_enter (संरचना cs_स्थितिः *स्थितिः)
{
  पूर्णाङ्क ret = pthread_sigmask (SIG_BLOCK, &caught_सङ्केताः, &स्थितिः->sigs);
  स्थितिः->valid = ret == 0;
}

/* Leave a critical section.  */
स्थिर शून्य
cs_leave (संरचना cs_स्थितिः ध्रुव *स्थितिः)
{
  यदि (स्थितिः->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &स्थितिः->sigs, रिक्त);
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
  संरचना tempnode *अस्थायी;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
स्थिर Hash_सारणी *proctab;

गणना { INIT_PROCTAB_आकार = 47 };

स्थिर size_t
proctab_hasher (शून्य ध्रुव *प्रविष्टिः, size_t tabsize)
{
  संरचना tempnode ध्रुव *ग्रन्थिः = प्रविष्टिः;
  प्रत्यागम ग्रन्थिः->pid % tabsize;
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
  पूर्णाङ्क स्थितिः;
  pid_t cpid = waitpid ((pid ? pid : -1), &स्थितिः, (pid ? 0 : WNOHANG));

  यदि (cpid < 0)
    error (क्रमय_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_क्रमादेशः));
  अन्यथा यदि (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      यदि (! WIFEXITED (स्थितिः) || WEXITSTATUS (स्थितिः))
        error (क्रमय_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_क्रमादेशः));
      --nprocs;
    }

  प्रत्यागम cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

स्थिर शून्य
पञ्जिकाकोशः_proc (संरचना tempnode *अस्थायी)
{
  यदि (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_आकार, रिक्त,
                                 proctab_hasher,
                                 proctab_comparator,
                                 रिक्त);
      यदि (! proctab)
        xalloc_die ();
    }

  अस्थायी->state = UNREAPED;

  यदि (! hash_निवेशय (proctab, अस्थायी))
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
  यदि (! ग्रन्थिः)
    प्रत्यागम false;
  ग्रन्थिः->state = REAPED;
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
      संरचना cs_स्थितिः cs;
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
create_अस्थायी_file (पूर्णाङ्क *pfd, bool survive_fd_exhaustion)
{
  स्थिर वर्ण ध्रुव slashbase[] = "/sortXXXXXX";
  स्थिर idx_t अस्थायी_dir_सूचकांक;
  पूर्णाङ्क fd;
  पूर्णाङ्क saved_errno;
  वर्ण ध्रुव *अस्थायी_dir = अस्थायी_dirs.dirs[अस्थायी_dir_सूचकांक];
  size_t len = वाक्यदैर्घ्यम् (अस्थायी_dir);
  संरचना tempnode *ग्रन्थिः =
    xmalloc (FLEXSIZEOF (संरचना tempnode, name, len + परिमाण slashbase));
  वर्ण *file = ग्रन्थिः->name;
  संरचना cs_स्थितिः cs;

  स्मृतिं_प्रतिलिख (file, अस्थायी_dir, len);
  स्मृतिं_प्रतिलिख (file + len, slashbase, परिमाण slashbase);
  ग्रन्थिः->अग्र = रिक्त;
  यदि (++अस्थायी_dir_सूचकांक == अस्थायी_dirs.n_dirs)
    अस्थायी_dir_सूचकांक = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  यदि (0 <= fd)
    {
      *temptail = ग्रन्थिः;
      temptail = &ग्रन्थिः->अग्र;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  यदि (fd < 0)
    {
      यदि (! (survive_fd_exhaustion && errno == EMFILE))
        error (क्रमय_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (अस्थायी_dir));
      मुक्त (ग्रन्थिः);
      ग्रन्थिः = रिक्त;
    }

  *pfd = fd;
  प्रत्यागम ग्रन्थिः;
}

/* Return a pointer to stdout status, or NULL on failure.  */

स्थिर संरचना stat *
get_outstatus (शून्य)
{
  स्थिर पूर्णाङ्क outstat_errno;
  स्थिर संरचना stat outstat;
  यदि (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
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

स्थिर FILE *
प्रवाहः_open (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  FILE *fp;

  यदि (*how == 'r')
    {
      यदि (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      अन्यथा
        {
          पूर्णाङ्क fd = उद्घाटय (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? रिक्त : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  अन्यथा यदि (*how == 'w')
    {
      यदि (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          पूर्णाङ्क ftruncate_errno = errno;
          संरचना stat *outst = get_outstatus ();
          यदि (!outst || S_ISREG (outst->st_बहुलकम्) || S_TYPEISSHM (outst))
            error (क्रमय_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  अन्यथा
    affirm (!"unexpected mode passed to stream_open");

  प्रत्यागम fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

स्थिर FILE *
xfopen (वर्ण ध्रुव *file, वर्ण ध्रुव *how)
{
  FILE *fp = प्रवाहः_open (file, how);
  यदि (!fp)
    क्रमय_die (_("open failed"), file);
  प्रत्यागम fp;
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
      यदि (fflush (fp) != 0)
        क्रमय_die (_("fflush failed"), file);
      भङ्ग;

    स्वभाव:
      यदि (fclose (fp) != 0)
        क्रमय_die (_("close failed"), file);
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
  पूर्णाङ्क परिणाम = 0;
  यदि (oldfd != newfd)
    {
      परिणाम = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      यदि (परिणाम == 0)
        परिणाम = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  प्रत्यागम परिणाम;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

स्थिर वर्ण ध्रुव *
get_resolved_compress_क्रमादेशः (शून्य)
{
  /* Use a cache, to perform the search only once.  */
  स्थिर वर्ण ध्रुव *resolved_compress_क्रमादेशः_cache /* = NULL */;

  यदि (resolved_compress_क्रमादेशः_cache == रिक्त)
    {
      resolved_compress_क्रमादेशः_cache =
        find_अन्तर्_given_मार्गः (compress_क्रमादेशः, getenv ("PATH"), रिक्त, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  प्रत्यागम resolved_compress_क्रमादेशः_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

स्थिर पूर्णाङ्क
pipe_child (pid_t *pid, पूर्णाङ्क pipefds[2], पूर्णाङ्क tempfd, bool decompress,
            size_t tries)
{
  वर्ण ध्रुव *resolved_compress_क्रमादेशः;
  संरचना tempnode *saved_temphead;
  द्विगुण wait_retry = 0.25;
  संरचना cs_स्थितिः cs;
  पूर्णाङ्क परिणाम;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_क्रमादेशः = get_resolved_compress_क्रमादेशः ();
  यदि (resolved_compress_क्रमादेशः == रिक्त)
    प्रत्यागम errno;

  यदि ((परिणाम = posix_spawnattr_init (&attr)))
    प्रत्यागम परिणाम;
  यदि ((परिणाम = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (परिणाम = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      प्रत्यागम परिणाम;
    }

  यदि (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      पूर्णाङ्क saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      प्रत्यागम saved_errno;
    }

  यदि ((परिणाम = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
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
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      प्रत्यागम परिणाम;
    }

  वर्ण ध्रुव *ध्रुव argv[] =
    {
      resolved_compress_क्रमादेशः,
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

      परिणाम = posix_spawnp (pid, resolved_compress_क्रमादेशः, &actions, &attr,
                             (वर्ण * ध्रुव *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      यदि (परिणाम != EAGAIN)
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

  यदि (परिणाम)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  अन्यथा
    ++nprocs;

  प्रत्यागम परिणाम;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

स्थिर संरचना tempnode *
maybe_create_अस्थायी (FILE **pfp, bool survive_fd_exhaustion)
{
  पूर्णाङ्क tempfd;
  संरचना tempnode *ग्रन्थिः = create_अस्थायी_file (&tempfd, survive_fd_exhaustion);
  यदि (! ग्रन्थिः)
    प्रत्यागम रिक्त;

  ग्रन्थिः->state = UNCOMPRESSED;

  यदि (compress_क्रमादेशः)
    {
      पूर्णाङ्क pipefds[2];
      स्थिर पूर्णाङ्क चरमः_परिणाम = 0;

      पूर्णाङ्क परिणाम = pipe_child (&ग्रन्थिः->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      यदि (परिणाम)
        {
          यदि (परिणाम != चरमः_परिणाम)
            error (0, परिणाम, _("could not run compress program %s"),
                   quoteaf (compress_क्रमादेशः));
        }
      अन्यथा
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          पञ्जिकाकोशः_proc (ग्रन्थिः);
        }

      चरमः_परिणाम = परिणाम;
    }

  *pfp = fdopen (tempfd, "w");
  यदि (! *pfp)
    क्रमय_die (_("couldn't create temporary file"), ग्रन्थिः->name);

  प्रत्यागम ग्रन्थिः;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

स्थिर संरचना tempnode *
create_अस्थायी (FILE **pfp)
{
  प्रत्यागम maybe_create_अस्थायी (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

स्थिर FILE *
open_अस्थायी (संरचना tempnode *अस्थायी)
{
  पूर्णाङ्क tempfd, pipefds[2];
  FILE *fp = रिक्त;

  यदि (अस्थायी->state == UNREAPED)
    wait_proc (अस्थायी->pid);

  tempfd = उद्घाटय (अस्थायी->name, O_RDONLY);
  यदि (tempfd < 0)
    प्रत्यागम रिक्त;

  pid_t child;
  पूर्णाङ्क परिणाम = pipe_child (&child, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  यदि (परिणाम)
    {
      यदि (परिणाम != EMFILE)
        error (क्रमय_FAILURE, परिणाम, _("could not run compress program %s -d"),
               quoteaf (compress_क्रमादेशः));
      close (tempfd);
      errno = EMFILE;
    }
  अन्यथा
    {
      अस्थायी->pid = child;
      पञ्जिकाकोशः_proc (अस्थायी);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      यदि (! fp)
        {
          पूर्णाङ्क saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  प्रत्यागम fp;
}

/* Append DIR to the array of temporary directory names.  */
स्थिर शून्य
सङ्कलय_अस्थायी_dir (वर्ण ध्रुव *dir)
{
  यदि (अस्थायी_dirs.n_dirs == अस्थायी_dirs.n_dirs_alloc)
    अस्थायी_dirs.dirs = xpalloc (अस्थायी_dirs.dirs, &अस्थायी_dirs.n_dirs_alloc, 1, -1,
                              परिमाण *अस्थायी_dirs.dirs);

  idx_t ध्रुव dir_idx = अस्थायी_dirs.n_dirs++;
  अस्थायी_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

स्थिर शून्य
zaptemp (वर्ण ध्रुव *name)
{
  संरचना tempnode *चञ्चल *pnode;
  संरचना tempnode *ग्रन्थिः;
  संरचना tempnode *अग्र;
  पूर्णाङ्क unlink_स्थितिः;
  पूर्णाङ्क unlink_errno = 0;
  संरचना cs_स्थितिः cs;

  प्रति (pnode = &temphead; (ग्रन्थिः = *pnode)->name != name; pnode = &ग्रन्थिः->अग्र)
    अनुवर्त;

  यदि (ग्रन्थिः->state == UNREAPED)
    wait_proc (ग्रन्थिः->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  अग्र = ग्रन्थिः->अग्र;
  cs_enter (&cs);
  unlink_स्थितिः = unlink (name);
  unlink_errno = errno;
  *pnode = अग्र;
  cs_leave (&cs);

  यदि (unlink_स्थितिः != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  यदि (! अग्र)
    temptail = pnode;
  मुक्त (ग्रन्थिः);
}

#यदि HAVE_NL_LANGINFO

स्थिर पूर्णाङ्क
struct_month_cmp (शून्य ध्रुव *m1, शून्य ध्रुव *m2)
{
  संरचना month ध्रुव *month1 = m1;
  संरचना month ध्रुव *month2 = m2;
  प्रत्यागम वाक्यं_तुलय (month1->name, month2->name);
}

#यदि_अन्तः

/* Initialize the character class tables. */

स्थिर शून्य
inittables (शून्य)
{
  प्रति (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! अक्षरम्_अङ्कः_वा (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = महाक्षरम् (i);
    }

#यदि HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  यदि (hard_LC_TIME)
    {
      प्रति (size_t i = 0; i < MONTHS_प्रत्येकम्_YEAR; i++)
        {
          वर्ण ध्रुव *s;
          size_t s_len;
          size_t j, k;
          वर्ण *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = वाक्यदैर्घ्यम् (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          प्रति (j = k = 0; j < s_len; j++)
            यदि (! isblank (गन्तव्यम्_uchar (s[j])))
              name[k++] = fold_toupper[गन्तव्यम्_uchar (s[j])];
          name[k] = '\0';
        }
      शीघ्रं_क्रमय (monthtab, MONTHS_प्रत्येकम्_YEAR, परिमाण *monthtab, struct_month_cmp);
    }
#यदि_अन्तः
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
  अचिह्नित पूर्णाङ्क max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  यदि (e == LONGINT_OK)
    {
      nmerge = n;
      यदि (nmerge != n)
        e = LONGINT_अतिप्रवाहः;
      अन्यथा
        {
          यदि (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (क्रमय_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          अन्यथा यदि (max_nmerge < nmerge)
            {
              e = LONGINT_अतिप्रवाहः;
            }
          अन्यथा
            प्रत्यागम;
        }
    }

  यदि (e == LONGINT_अतिप्रवाहः)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (क्रमय_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  अन्यथा
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
स्थिर शून्य
specify_क्रमय_आकार (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t n;
  वर्ण *suffix;
  गणना strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  यदि (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_अतिप्रवाहः;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  यदि (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    विकल्प (suffix[0])
      {
      पक्ष 'b':
        e = LONGINT_OK;
        भङ्ग;

      पक्ष '%':
        {
          द्विगुण mem = physmem_योग () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          यदि (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          अन्यथा
            e = LONGINT_अतिप्रवाहः;
        }
        भङ्ग;
      }

  यदि (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      यदि (n < क्रमय_आकार)
        प्रत्यागम;

      क्रमय_आकार = n;
      यदि (क्रमय_आकार == n)
        {
          क्रमय_आकार = MAX (क्रमय_आकार, MIN_क्रमय_आकार);
          प्रत्यागम;
        }

      e = LONGINT_अतिप्रवाहः;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
स्थिर size_t
specify_nthreads (पूर्णाङ्क oi, वर्ण c, वर्ण ध्रुव *s)
{
  uintmax_t nthreads;
  गणना strtol_error e = xstrtoumax (s, रिक्त, 10, &nthreads, "");
  यदि (e == LONGINT_अतिप्रवाहः)
    प्रत्यागम SIZE_MAX;
  यदि (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  यदि (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  यदि (nthreads == 0)
    error (क्रमय_FAILURE, 0, _("number in parallel must be nonzero"));
  प्रत्यागम nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
स्थिर size_t
default_क्रमय_आकार (शून्य)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t आकार = SIZE_MAX;
  संरचना rlimit rlimit;
  यदि (getrlimit (RLIMIT_दत्त, &rlimit) == 0 && rlimit.rlim_cur < आकार)
    आकार = rlimit.rlim_cur;
#यदि_परिभाषितम् RLIMIT_AS
  यदि (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < आकार)
    आकार = rlimit.rlim_cur;
#यदि_अन्तः

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  आकार /= 2;

#यदि_परिभाषितम् RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  यदि (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < आकार)
    आकार = rlimit.rlim_cur / 16 * 15;
#यदि_अन्तः

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  द्विगुण avail = physmem_उपलब्धम् ();
  द्विगुण योग_2 = physmem_योग ();
  द्विगुण mem = MAX (avail, योग_2 / 8);

  /* Leave a 1/4 margin for physical memory.  */
  यदि (योग_2 * 0.75 < आकार)
    आकार = योग_2 * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  यदि (mem < आकार)
    आकार = mem;
  प्रत्यागम MAX (आकार, MIN_क्रमय_आकार);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

स्थिर bool
पङ्क्तिः_aligned_आकार (size_t *alloc)
{
  size_t आकार = *alloc;
  size_t alignment = परिमाण (संरचना पङ्क्तिः);
  size_t padding = alignment - आकार % alignment;
  size_t aligned;

  यदि (ckd_सङ्कलय (&aligned, आकार, padding))
    प्रत्यागम false;

  *alloc = aligned;
  प्रत्यागम true;
}

/* Return the default initial allocation for a growable sort buffer.  */

स्थिर size_t
default_initial_क्रमय_आकार (size_t पङ्क्तिः_अष्टकानि)
{
  size_t आकार = पङ्क्तिः_अष्टकानि + 2;
  size_t निवेशः_आकार;

  यदि (ckd_mul (&निवेशः_आकार, निवेशः_FILE_आकार_GUESS, पङ्क्तिः_अष्टकानि + 1)
      || ckd_सङ्कलय (&आकार, आकार, निवेशः_आकार))
    प्रत्यागम SIZE_MAX;

  प्रत्यागम MAX (आकार, MIN_क्रमय_आकार);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

स्थिर bool
निवेशः_आकार_बफर_अष्टकानि (uintmax_t file_आकार, size_t पङ्क्तिः_अष्टकानि,
                         size_t *worst_case)
{
  size_t worst_case_प्रत्येकम्_निवेशः_byte = पङ्क्तिः_अष्टकानि + 1;

  यदि (SIZE_MAX / worst_case_प्रत्येकम्_निवेशः_byte < file_आकार)
    प्रत्यागम false;

  size_t आकार = file_आकार * worst_case_प्रत्येकम्_निवेशः_byte;
  यदि (SIZE_MAX - आकार < 1)
    प्रत्यागम false;

  *worst_case = आकार + 1;
  प्रत्यागम true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

स्थिर शून्य
क्रमय_बफर_policy (FILE *ध्रुव *fps, size_t nfps,
                    वर्ण *ध्रुव *files, size_t nfiles,
                    size_t पङ्क्तिः_अष्टकानि,
                    संरचना क्रमय_बफर_policy *policy)
{
  size_t आकार_bound = क्रमय_आकार ? क्रमय_आकार : default_क्रमय_आकार ();
  size_t initial_bound =
    क्रमय_आकार ? आकार_bound : default_initial_क्रमय_आकार (पङ्क्तिः_अष्टकानि);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_प्रत्येकम्_निवेशः_byte = पङ्क्तिः_अष्टकानि + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t आकार = worst_case_प्रत्येकम्_निवेशः_byte + 1;

  initial_bound = MIN (initial_bound, आकार_bound);
  policy->सीमा = आकार_bound;
  policy->growth_failed = false;

  प्रति (size_t i = 0; i < nfiles; i++)
    {
      संरचना stat st;
      off_t file_आकार;
      size_t worst_case;
      bool ज्ञातम्_आकार;

      यदि (आकार_bound <= आकार)
        {
          policy->initial = आकार_bound;
          प्रत्यागम;
        }

      यदि ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        क्रमय_die (_("stat failed"), files[i]);

      ज्ञातम्_आकार = usable_st_आकार (&st) && 0 < st.st_आकार;
      यदि (ज्ञातम्_आकार)
        file_आकार = st.st_आकार;
      अन्यथा
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          यदि (क्रमय_आकार)
            {
              policy->initial = आकार_bound;
              प्रत्यागम;
            }
          file_आकार = निवेशः_FILE_आकार_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      यदि (! निवेशः_आकार_बफर_अष्टकानि (file_आकार, पङ्क्तिः_अष्टकानि, &worst_case)
          || आकार_bound - आकार <= worst_case)
        {
          policy->initial = आकार_bound;
          प्रत्यागम;
        }

      यदि (! ज्ञातम्_आकार && ! क्रमय_आकार
          && (initial_bound <= आकार || initial_bound - आकार <= worst_case))
        {
          policy->initial = MAX (आकार, initial_bound);
          प्रत्यागम;
        }

      आकार += worst_case;
    }

  policy->initial = MAX (आकार, MIN_क्रमय_आकार);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

स्थिर शून्य
initbuf (संरचना बफर *buf, size_t पङ्क्तिः_अष्टकानि, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  यावत् (true)
    {
      यदि (! पङ्क्तिः_aligned_आकार (&alloc))
        buf->buf = रिक्त;
      अन्यथा
        buf->buf = स्मृतिं_देहि (alloc);
      यदि (buf->buf)
        भङ्ग;
      alloc /= 2;
      यदि (alloc <= पङ्क्तिः_अष्टकानि + 1)
        xalloc_die ();
    }

  buf->पङ्क्तिः_अष्टकानि = पङ्क्तिः_अष्टकानि;
  buf->alloc = alloc;
  buf->प्रयुक्तम् = buf->वामः = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

स्थिर अन्तःस्थ संरचना पङ्क्तिः *
बफर_linelim (संरचना बफर ध्रुव *buf)
{
  शून्य *linelim = buf->buf + buf->alloc;
  प्रत्यागम linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

स्थिर bool
try_growbuf (संरचना बफर *buf, size_t alloc)
{
  यदि (! पङ्क्तिः_aligned_आकार (&alloc) || alloc <= buf->alloc)
    प्रत्यागम false;

  वर्ण *newbuf = स्मृतिं_देहि (alloc);
  यदि (! newbuf)
    प्रत्यागम false;

  वर्ण *oldbuf = buf->buf;
  संरचना पङ्क्तिः *old_linelim = बफर_linelim (buf);
  संरचना पङ्क्तिः *old_पङ्क्तिः = old_linelim - buf->nlines;

  स्मृतिं_प्रतिलिख (newbuf, oldbuf, buf->प्रयुक्तम्);

  संरचना पङ्क्तिः *new_linelim = (शून्य *) (newbuf + alloc);
  संरचना पङ्क्तिः *new_पङ्क्तिः = new_linelim - buf->nlines;
  स्मृतिं_प्रतिलिख (new_पङ्क्तिः, old_पङ्क्तिः, buf->nlines * परिमाण *new_पङ्क्तिः);

  प्रति (संरचना पङ्क्तिः *पङ्क्तिः = new_पङ्क्तिः; पङ्क्तिः < new_linelim; पङ्क्तिः++)
    {
      पङ्क्तिः->text = newbuf + (पङ्क्तिः->text - oldbuf);
      यदि (keylist)
        {
          पङ्क्तिः->keybeg = newbuf + (पङ्क्तिः->keybeg - oldbuf);
          पङ्क्तिः->keylim = newbuf + (पङ्क्तिः->keylim - oldbuf);
        }
    }

  मुक्त (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  प्रत्यागम true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

स्थिर bool
maybe_growbuf (संरचना बफर *buf, संरचना क्रमय_बफर_policy *policy)
{
  यदि (! policy || policy->growth_failed || policy->सीमा <= buf->alloc)
    प्रत्यागम false;

  size_t alloc;
  यदि (buf->alloc <= policy->सीमा / 3)
    alloc = buf->alloc * 3;
  अन्यथा
    alloc = policy->सीमा;

  यदि (try_growbuf (buf, alloc))
    प्रत्यागम true;

  policy->growth_failed = true;
  प्रत्यागम false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

स्थिर वर्ण *
begfield (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *ptr = पङ्क्तिः->text, *lim = ptr + पङ्क्तिः->दीर्घता - 1;
  size_t sword = कुञ्चिका->sword;
  size_t schar = कुञ्चिका->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  यदि (tab != TAB_DEFAULT)
    यावत् (ptr < lim && sword--)
      {
        वर्ण *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        यदि (ptr < lim)
          ++ptr;
      }
  अन्यथा
    यावत् (ptr < lim && sword--)
      {
        यावत् (ptr < lim && blanks[गन्तव्यम्_uchar (*ptr)])
          ++ptr;
        यावत् (ptr < lim && !blanks[गन्तव्यम्_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  यदि (कुञ्चिका->skipsblanks)
    यावत् (ptr < lim && blanks[गन्तव्यम्_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t remaining_अष्टकानि = lim - ptr;
  यदि (schar < remaining_अष्टकानि)
    ptr += schar;
  अन्यथा
    ptr = lim;

  प्रत्यागम ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_विशुद्धम्
स्थिर वर्ण *
limfield (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *ptr = पङ्क्तिः->text, *lim = ptr + पङ्क्तिः->दीर्घता - 1;
  size_t eword = कुञ्चिका->eword, echar = कुञ्चिका->echar;

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
        वर्ण *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        यदि (ptr < lim && (eword || echar))
          ++ptr;
      }
  अन्यथा
    यावत् (ptr < lim && eword--)
      {
        यावत् (ptr < lim && blanks[गन्तव्यम्_uchar (*ptr)])
          ++ptr;
        यावत् (ptr < lim && !blanks[गन्तव्यम्_uchar (*ptr)])
          ++ptr;
      }

#यदि_परिभाषितम् POSIX_UNSPECIFIED
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
      newlim = memchr (ptr, tab, lim - ptr);
      यदि (newlim)
        lim = newlim;
    }
  अन्यथा
    {
      वर्ण *newlim;
      newlim = ptr;
      यावत् (newlim < lim && blanks[गन्तव्यम्_uchar (*newlim)])
        ++newlim;
      यावत् (newlim < lim && !blanks[गन्तव्यम्_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#यदि_अन्तः

  यदि (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      यदि (कुञ्चिका->skipeblanks)
        यावत् (ptr < lim && blanks[गन्तव्यम्_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t remaining_अष्टकानि = lim - ptr;
      यदि (echar < remaining_अष्टकानि)
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
fillbuf (संरचना बफर *buf, FILE *fp, वर्ण ध्रुव *file,
         संरचना क्रमय_बफर_policy *policy)
{
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  वर्ण eol = eolchar;
  size_t पङ्क्तिः_अष्टकानि = buf->पङ्क्तिः_अष्टकानि;
  size_t mergesize = merge_बफर_आकार - MIN_MERGE_बफर_आकार;

  यदि (buf->eof)
    प्रत्यागम false;

  यदि (buf->प्रयुक्तम् != buf->वामः)
    {
      memmove (buf->buf, buf->buf + buf->प्रयुक्तम् - buf->वामः, buf->वामः);
      buf->प्रयुक्तम् = buf->वामः;
      buf->nlines = 0;
    }

  यावत् (true)
    {
      वर्ण *ptr = buf->buf + buf->प्रयुक्तम्;
      संरचना पङ्क्तिः *linelim = बफर_linelim (buf);
      संरचना पङ्क्तिः *पङ्क्तिः = linelim - buf->nlines;
      size_t avail = (वर्ण *) linelim - buf->nlines * पङ्क्तिः_अष्टकानि - ptr;
      वर्ण *पङ्क्तिः_start = buf->nlines ? पङ्क्तिः->text + पङ्क्तिः->दीर्घता : buf->buf;

      यावत् (पङ्क्तिः_अष्टकानि + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (पङ्क्तिः_अष्टकानि + 1);
          size_t अष्टकानि_read = fread (ptr, 1, readsize, fp);
          वर्ण *ptrlim = ptr + अष्टकानि_read;
          वर्ण *p;
          avail -= अष्टकानि_read;

          यदि (अष्टकानि_read != readsize)
            {
              यदि (ferror (fp))
                क्रमय_die (_("read failed"), file);
              यदि (feof (fp))
                {
                  buf->eof = true;
                  यदि (buf->buf == ptrlim)
                    प्रत्यागम false;
                  यदि (पङ्क्तिः_start != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          यावत् ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              पङ्क्तिः--;
              पङ्क्तिः->text = पङ्क्तिः_start;
              पङ्क्तिः->दीर्घता = ptr - पङ्क्तिः_start;
              mergesize = MAX (mergesize, पङ्क्तिः->दीर्घता);
              avail -= पङ्क्तिः_अष्टकानि;

              यदि (कुञ्चिका)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  पङ्क्तिः->keylim = (कुञ्चिका->eword == SIZE_MAX
                                  ? p
                                  : limfield (पङ्क्तिः, कुञ्चिका));

                  यदि (कुञ्चिका->sword != SIZE_MAX)
                    पङ्क्तिः->keybeg = begfield (पङ्क्तिः, कुञ्चिका);
                  अन्यथा
                    {
                      यदि (कुञ्चिका->skipsblanks)
                        यावत् (blanks[गन्तव्यम्_uchar (*पङ्क्तिः_start)])
                          पङ्क्तिः_start++;
                      पङ्क्तिः->keybeg = पङ्क्तिः_start;
                    }
                }

              पङ्क्तिः_start = ptr;
            }

          ptr = ptrlim;
          यदि (buf->eof)
            भङ्ग;
        }

      buf->प्रयुक्तम् = ptr - buf->buf;
      buf->nlines = बफर_linelim (buf) - पङ्क्तिः;
      यदि (buf->nlines != 0)
        {
          यदि (! buf->eof && maybe_growbuf (buf, policy))
            अनुवर्त;
          buf->वामः = ptr - पङ्क्तिः_start;
          merge_बफर_आकार = mergesize + MIN_MERGE_बफर_आकार;
          प्रत्यागम true;
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
स्थिर वर्ण ध्रुव unit_क्रमः[UCHAR_LIM] =
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
#यदि_अन्तः
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
  bool अन्ताः_with_thousands_sep = false;

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
      अन्ताः_with_thousands_sep = (*p == thousands_sep);
      यदि (अन्ताः_with_thousands_sep)
        ++p;
    }

  यदि (अन्ताः_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *सङ्ख्या = p - 2;
      प्रत्यागम max_digit;
    }

  यदि (ch == decimal_point)
    यावत् (c_isdigit (ch = *p++))
      यदि (max_digit < ch)
        max_digit = ch;

  *सङ्ख्या = p - 1;
  प्रत्यागम max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
find_unit_क्रमः (वर्ण ध्रुव *सङ्ख्या)
{
  bool minus_sign = (*सङ्ख्या == '-');
  वर्ण ध्रुव *p = सङ्ख्या + minus_sign;
  वर्ण max_digit = traverse_raw_सङ्ख्या (&p);
  यदि ('0' < max_digit)
    {
      अचिह्नित वर्ण ch = *p;
      पूर्णाङ्क क्रमः = unit_क्रमः[ch];
      प्रत्यागम (minus_sign ? -क्रमः : क्रमः);
    }
  अन्यथा
    प्रत्यागम 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
human_numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  यावत् (blanks[गन्तव्यम्_uchar (*a)])
    a++;
  यावत् (blanks[गन्तव्यम्_uchar (*b)])
    b++;

  पूर्णाङ्क diff = find_unit_क्रमः (a) - find_unit_क्रमः (b);
  प्रत्यागम (diff ? diff : strnumcmp (a, b, decimal_point, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_विशुद्धम्
स्थिर पूर्णाङ्क
numcompare (वर्ण ध्रुव *a, वर्ण ध्रुव *b)
{
  यावत् (blanks[गन्तव्यम्_uchar (*a)])
    a++;
  यावत् (blanks[गन्तव्यम्_uchar (*b)])
    b++;

  प्रत्यागम strnumcmp (a, b, decimal_point, thousands_sep);
}

स्थिर पूर्णाङ्क
nan_compare (दीर्घ द्विगुण a, दीर्घ द्विगुण b)
{
  वर्ण buf[2][परिमाण "-nan""()" + CHAR_BIT * परिमाण a];
  snprintf (buf[0], परिमाण buf[0], "%Lf", a);
  snprintf (buf[1], परिमाण buf[1], "%Lf", b);
  प्रत्यागम वाक्यं_तुलय (buf[0], buf[1]);
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
  size_t lo = 0;
  size_t hi = MONTHS_प्रत्येकम्_YEAR;

  यावत् (blanks[गन्तव्यम्_uchar (*month)])
    month++;

  कुरु
    {
      size_t ix = (lo + hi) / 2;
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
          यदि (गन्तव्यम्_uchar (fold_toupper[गन्तव्यम्_uchar (*m)]) < गन्तव्यम्_uchar (*n))
            {
              hi = ix;
              भङ्ग;
            }
          अन्यथा यदि (गन्तव्यम्_uchar (fold_toupper[गन्तव्यम्_uchar (*m)]) > गन्तव्यम्_uchar (*n))
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
#  pragma GCC diagnostic प्रक्षिप
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#यदि_अन्तः

#अन्तर्भाव "md5.h"

#यदि DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# यदि 14 <= __GNUC__
#  pragma GCC diagnostic निष्कासय
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_प्राथमिकता_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
स्थिर शून्य
link_failure (शून्य)
{
  error (क्रमय_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
स्थिर शून्य *
चिह्नम्_स्थानाङ्कः (शून्य *handle, वर्ण ध्रुव *चिह्नम्)
{
  शून्य *स्थानाङ्कः = dlsym (handle, चिह्नम्);
  यदि (!स्थानाङ्कः)
    link_failure ();
  प्रत्यागम स्थानाङ्कः;
}
#यदि_अन्तः

/* Dynamically link the crypto library, if it needs linking.  */
स्थिर शून्य
link_libcrypto (शून्य)
{
#यदि DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  शून्य *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  यदि (!handle)
    link_failure ();
  ptr_MD5_Init = चिह्नम्_स्थानाङ्कः (handle, "MD5_Init");
  ptr_MD5_Update = चिह्नम्_स्थानाङ्कः (handle, "MD5_Update");
  ptr_MD5_Final = चिह्नम्_स्थानाङ्कः (handle, "MD5_Final");
#यदि_अन्तः
}

/* A randomly chosen MD5 state, used for random comparison.  */
स्थिर संरचना md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

स्थिर शून्य
random_md5_state_init (वर्ण ध्रुव *random_source)
{
  अचिह्नित वर्ण buf[MD5_DIGEST_आकार];
  संरचना randread_source *r = randread_new (random_source, परिमाण buf);
  यदि (! r)
    क्रमय_die (_("open failed"), random_source ? random_source : "getrandom");
  randread (r, buf, परिमाण buf);
  यदि (randread_free (r) != 0)
    क्रमय_die (_("close failed"), random_source);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_अष्टकानि (buf, परिमाण buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

स्थिर size_t
xstrxfrm (वर्ण *सीमित dest, वर्ण ध्रुव *सीमित src, size_t destsize)
{
  errno = 0;
  size_t translated_आकार = strxfrm (dest, src, destsize);

  यदि (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (क्रमय_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  प्रत्यागम translated_आकार;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

स्थिर पूर्णाङ्क
compare_random (वर्ण *सीमित texta, size_t lena,
                वर्ण *सीमित textb, size_t lenb)
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
  uint32_t dig[2][MD5_DIGEST_आकार / परिमाण (uint32_t)];
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
          size_t guess_bufsize = 3 * (lena + lenb) + 2;
          यदि (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              मुक्त (allocated);
              buf = allocated = स्मृतिं_देहि (bufsize);
              यदि (! buf)
                {
                  buf = stackbuf;
                  bufsize = परिमाण stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_माति = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_माति ? buf + sizea : रिक्त), textb,
                          (a_माति ? bufsize - sizea : 0))
                + 1)
             : 0);

          यदि (! (a_माति && sizea + sizeb <= bufsize))
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
            texta += वाक्यदैर्घ्यम् (texta) + 1;
          यदि (textb < limb)
            textb += वाक्यदैर्घ्यम् (textb) + 1;
          यदि (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              भङ्ग;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_अष्टकानि (buf, sizea, &s[0]);
          md5_process_अष्टकानि (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          यदि (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              यदि (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_अष्टकानि (texta, lena, &s[0]); md5_finish_ctx (&s[0], dig[0]);
  md5_process_अष्टकानि (textb, lenb, &s[1]); md5_finish_ctx (&s[1], dig[1]);
  पूर्णाङ्क diff = memcmp (dig[0], dig[1], परिमाण dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  यदि (! diff)
    {
      यदि (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
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

स्थिर size_t
debug_विस्तारः (वर्ण ध्रुव *text, वर्ण ध्रुव *lim)
{
  size_t विस्तारः = mbsnwidth (text, lim - text, 0);
  यावत् (text < lim)
    विस्तारः += (*text++ == '\t');
  प्रत्यागम विस्तारः;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

स्थिर शून्य
mark_कुञ्चिका (size_t विस्थापनम्, size_t विस्तारः)
{
  यावत् (विस्थापनम्--)
    अक्षरं_लिख (' ');

  यदि (!विस्तारः)
    प्रारूप_लिख (_("^ no match for key\n"));
  अन्यथा
    {
      कुरु
        अक्षरं_लिख ('_');
      यावत् (--विस्तारः);

      अक्षरं_लिख ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

स्थिर अन्तःस्थ bool
कुञ्चिका_numeric (संरचना keyfield ध्रुव *कुञ्चिका)
{
  प्रत्यागम कुञ्चिका->numeric || कुञ्चिका->general_numeric || कुञ्चिका->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

स्थिर शून्य
debug_कुञ्चिका (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, संरचना keyfield ध्रुव *कुञ्चिका)
{
  वर्ण *text = पङ्क्तिः->text;
  वर्ण *beg = text;
  वर्ण *lim = text + पङ्क्तिः->दीर्घता - 1;

  यदि (कुञ्चिका)
    {
      यदि (कुञ्चिका->sword != SIZE_MAX)
        beg = begfield (पङ्क्तिः, कुञ्चिका);
      यदि (कुञ्चिका->eword != SIZE_MAX)
        {
          lim = limfield (पङ्क्तिः, कुञ्चिका);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      यदि ((कुञ्चिका->skipsblanks && कुञ्चिका->sword == SIZE_MAX)
          || कुञ्चिका->month || कुञ्चिका_numeric (कुञ्चिका))
        {
          वर्ण saved = *lim;
          *lim = '\0';

          यावत् (blanks[गन्तव्यम्_uchar (*beg)])
            beg++;

          वर्ण *tighter_lim = beg;

          यदि (lim < beg)
            tighter_lim = lim;
          अन्यथा यदि (कुञ्चिका->month)
            getmonth (beg, &tighter_lim);
          अन्यथा यदि (कुञ्चिका->general_numeric)
            ignore_मानम् (strtold (beg, &tighter_lim));
          अन्यथा यदि (कुञ्चिका->numeric || कुञ्चिका->human_numeric)
            {
              वर्ण ध्रुव *p = beg + (beg < lim && *beg == '-');
              वर्ण max_digit = traverse_raw_सङ्ख्या (&p);
              यदि ('0' <= max_digit)
                {
                  अचिह्नित वर्ण ch = *p;
                  tighter_lim = (वर्ण *) p
                    + (कुञ्चिका->human_numeric && unit_क्रमः[ch]);
                }
            }
          अन्यथा
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t विस्थापनम् = debug_विस्तारः (text, beg);
  size_t विस्तारः = debug_विस्तारः (beg, lim);
  mark_कुञ्चिका (विस्थापनम्, विस्तारः);
}

/* Debug LINE by underlining its keys.  */

स्थिर शून्य
debug_पङ्क्तिः (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः)
{
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;

  कुरु
    debug_कुञ्चिका (पङ्क्तिः, कुञ्चिका);
  यावत् (कुञ्चिका && ((कुञ्चिका = कुञ्चिका->अग्र) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

स्थिर bool
default_कुञ्चिका_compare (संरचना keyfield ध्रुव *कुञ्चिका)
{
  प्रत्यागम ! (कुञ्चिका->ignore
            || कुञ्चिका->translate
            || कुञ्चिका->skipsblanks
            || कुञ्चिका->skipeblanks
            || कुञ्चिका_numeric (कुञ्चिका)
            || कुञ्चिका->month
            || कुञ्चिका->संस्करणम्_3
            || कुञ्चिका->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

स्थिर शून्य
कुञ्चिका_गन्तव्यम्_opts (संरचना keyfield ध्रुव *कुञ्चिका, वर्ण *opts)
{
  यदि (कुञ्चिका->skipsblanks || कुञ्चिका->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  यदि (कुञ्चिका->ignore == nondictionary)
    *opts++ = 'd';
  यदि (कुञ्चिका->translate)
    *opts++ = 'f';
  यदि (कुञ्चिका->general_numeric)
    *opts++ = 'g';
  यदि (कुञ्चिका->human_numeric)
    *opts++ = 'h';
  यदि (कुञ्चिका->ignore == nonprinting)
    *opts++ = 'i';
  यदि (कुञ्चिका->month)
    *opts++ = 'M';
  यदि (कुञ्चिका->numeric)
    *opts++ = 'n';
  यदि (कुञ्चिका->random)
    *opts++ = 'R';
  यदि (कुञ्चिका->reverse)
    *opts++ = 'r';
  यदि (कुञ्चिका->संस्करणम्_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

स्थिर शून्य
कुञ्चिका_warnings (संरचना keyfield ध्रुव *gkey, bool gkey_only)
{
  संरचना keyfield ugkey = *gkey;
  अचिह्नित दीर्घ keynum = 1;
  bool basic_numeric_क्षेत्रम् = false;
  bool general_numeric_क्षेत्रम् = false;
  bool basic_numeric_क्षेत्रम्_विस्तृतिः = false;
  bool general_numeric_क्षेत्रम्_विस्तृतिः = false;

  प्रति (संरचना keyfield *कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->अग्र, keynum++)
    {
      यदि (कुञ्चिका_numeric (कुञ्चिका))
        {
          यदि (कुञ्चिका->general_numeric)
            general_numeric_क्षेत्रम् = true;
          अन्यथा
            basic_numeric_क्षेत्रम् = true;
        }

      यदि (कुञ्चिका->traditional_प्रयुक्तम्)
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

          यदि (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          यदि (कुञ्चिका->eword != SIZE_MAX)
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
      bool zero_विस्तारः = कुञ्चिका->sword != SIZE_MAX && कुञ्चिका->eword < कुञ्चिका->sword;
      यदि (zero_विस्तारः)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = कुञ्चिका_numeric (कुञ्चिका) || कुञ्चिका->month;
      bool पङ्क्तिः_विस्थापनम् = कुञ्चिका->eword == 0 && कुञ्चिका->echar != 0; /* -k1.x,1.y  */
      यदि (!zero_विस्तारः && !gkey_only && tab == TAB_DEFAULT && !पङ्क्तिः_विस्थापनम्
          && ((!कुञ्चिका->skipsblanks && !implicit_skip)
              || (!कुञ्चिका->skipsblanks && कुञ्चिका->schar)
              || (!कुञ्चिका->skipeblanks && कुञ्चिका->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      यदि (!gkey_only && कुञ्चिका_numeric (कुञ्चिका))
        {
          size_t sword = कुञ्चिका->sword + 1;
          size_t eword = कुञ्चिका->eword + 1;
          यदि (!sword)
            sword++;
          यदि (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              यदि (कुञ्चिका->general_numeric)
                general_numeric_क्षेत्रम्_विस्तृतिः = true;
              अन्यथा
                basic_numeric_क्षेत्रम्_विस्तृतिः = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      यदि (ugkey.ignore && (ugkey.ignore == कुञ्चिका->ignore))
        ugkey.ignore = रिक्त;
      यदि (ugkey.translate && (ugkey.translate == कुञ्चिका->translate))
        ugkey.translate = रिक्त;
      ugkey.skipsblanks &= !कुञ्चिका->skipsblanks;
      ugkey.skipeblanks &= !कुञ्चिका->skipeblanks;
      ugkey.month &= !कुञ्चिका->month;
      ugkey.numeric &= !कुञ्चिका->numeric;
      ugkey.general_numeric &= !कुञ्चिका->general_numeric;
      ugkey.human_numeric &= !कुञ्चिका->human_numeric;
      ugkey.random &= !कुञ्चिका->random;
      ugkey.संस्करणम्_3 &= !कुञ्चिका->संस्करणम्_3;
      ugkey.reverse &= !कुञ्चिका->reverse;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool सङ्ख्या_locale_warned = false;
  यदि (basic_numeric_क्षेत्रम्_विस्तृतिः)
    {
      यदि (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (गन्तव्यम्_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((वर्ण []) {thousands_sep, 0})));
          सङ्ख्या_locale_warned = true;
        }
    }
  यदि (basic_numeric_क्षेत्रम्_विस्तृतिः || general_numeric_क्षेत्रम्_विस्तृतिः)
    {
      यदि (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (गन्तव्यम्_uchar (decimal_point)))
          : tab == decimal_point)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((वर्ण []) {decimal_point, 0})));
          सङ्ख्या_locale_warned = true;
        }
      अन्यथा यदि (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((वर्ण []) {tab, 0})));
        }
      अन्यथा यदि (general_numeric_क्षेत्रम्_विस्तृतिः && tab == '+')
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
  यदि ((basic_numeric_क्षेत्रम् || general_numeric_क्षेत्रम्) && ! सङ्ख्या_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((वर्ण []) {decimal_point, 0})));

    }

  यदि (basic_numeric_क्षेत्रम् && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  यदि (!default_कुञ्चिका_compare (&ugkey)
      || (ugkey.reverse && (stable || unique) && keylist))
    {
      bool ugkey_reverse = ugkey.reverse;
      यदि (!(stable || unique))
        ugkey.reverse = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      वर्ण opts[परिमाण short_options];
      कुञ्चिका_गन्तव्यम्_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (वाक्यदैर्घ्यम् (opts))), opts);
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

  यावत् (true)
    {
      वर्ण ध्रुव *translate = कुञ्चिका->translate;
      bool ध्रुव *ignore = कुञ्चिका->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      यदि (hard_LC_COLLATE || कुञ्चिका_numeric (कुञ्चिका)
          || कुञ्चिका->month || कुञ्चिका->random || कुञ्चिका->संस्करणम्_3)
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

          यदि (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t आकार = lena + 1 + lenb + 1;
              यदि (आकार <= परिमाण stackbuf)
                ta = stackbuf;
              अन्यथा
                ta = allocated = xmalloc (आकार);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              प्रति (tlena = i = 0; i < lena; i++)
                यदि (! (ignore && ignore[गन्तव्यम्_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[गन्तव्यम्_uchar (texta[i])]
                                 : texta[i]);

              प्रति (tlenb = i = 0; i < lenb; i++)
                यदि (! (ignore && ignore[गन्तव्यम्_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[गन्तव्यम्_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          यदि (कुञ्चिका->numeric)
            diff = numcompare (ta, tb);
          अन्यथा यदि (कुञ्चिका->general_numeric)
            diff = general_numcompare (ta, tb);
          अन्यथा यदि (कुञ्चिका->human_numeric)
            diff = human_numcompare (ta, tb);
          अन्यथा यदि (कुञ्चिका->month)
            diff = getmonth (ta, रिक्त) - getmonth (tb, रिक्त);
          अन्यथा यदि (कुञ्चिका->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          अन्यथा यदि (कुञ्चिका->संस्करणम्_3)
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
#परिभाषय CMP_WITH_IGNORE(A, B)						\
  कुरु									\
    {									\
          यावत् (true)							\
            {								\
              यावत् (texta < lima && ignore[गन्तव्यम्_uchar (*texta)])		\
                ++texta;						\
              यावत् (textb < limb && ignore[गन्तव्यम्_uchar (*textb)])		\
                ++textb;						\
              यदि (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  भङ्ग;						\
                }							\
              diff = गन्तव्यम्_uchar (A) - गन्तव्यम्_uchar (B);			\
              यदि (diff)							\
                भङ्ग;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  यावत् (0)

          यदि (translate)
            CMP_WITH_IGNORE (translate[गन्तव्यम्_uchar (*texta)],
                             translate[गन्तव्यम्_uchar (*textb)]);
          अन्यथा
            CMP_WITH_IGNORE (*texta, *textb);
        }
      अन्यथा
        {
          size_t lenmin = MIN (lena, lenb);
          यदि (lenmin == 0)
            diff = 0;
          अन्यथा यदि (translate)
            {
              size_t i = 0;
              कुरु
                {
                  diff = (गन्तव्यम्_uchar (translate[गन्तव्यम्_uchar (texta[i])])
                          - गन्तव्यम्_uchar (translate[गन्तव्यम्_uchar (textb[i])]));
                  यदि (diff)
                    भङ्ग;
                  i++;
                }
              यावत् (i < lenmin);
            }
          अन्यथा
            diff = memcmp (texta, textb, lenmin);

          यदि (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      यदि (diff)
        भङ्ग;

      कुञ्चिका = कुञ्चिका->अग्र;
      यदि (! कुञ्चिका)
        प्रत्यागम 0;

      /* Find the beginning and limit of the next field.  */
      यदि (कुञ्चिका->eword != SIZE_MAX)
        lima = limfield (a, कुञ्चिका), limb = limfield (b, कुञ्चिका);
      अन्यथा
        lima = a->text + a->दीर्घता - 1, limb = b->text + b->दीर्घता - 1;

      यदि (कुञ्चिका->sword != SIZE_MAX)
        texta = begfield (a, कुञ्चिका), textb = begfield (b, कुञ्चिका);
      अन्यथा
        {
          texta = a->text, textb = b->text;
          यदि (कुञ्चिका->skipsblanks)
            {
              यावत् (texta < lima && blanks[गन्तव्यम्_uchar (*texta)])
                ++texta;
              यावत् (textb < limb && blanks[गन्तव्यम्_uchar (*textb)])
                ++textb;
            }
        }
    }

  प्रत्यागम diff_reversed (diff, कुञ्चिका->reverse);
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
  यदि (keylist)
    {
      diff = keycompare (a, b);
      यदि (diff || unique || stable)
        प्रत्यागम diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->दीर्घता - 1, blen = b->दीर्घता - 1;

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
      diff = memcmp (a->text, b->text, MIN (alen, blen));
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
write_पङ्क्तिः (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, FILE *fp, वर्ण ध्रुव *output_file)
{
  वर्ण *buf = पङ्क्तिः->text;
  size_t n_अष्टकानि = पङ्क्तिः->दीर्घता;
  वर्ण *ebuf = buf + n_अष्टकानि;

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
          यदि (fputc (wc, fp) == EOF)
            क्रमय_die (_("write failed"), output_file);
        }

      debug_पङ्क्तिः (पङ्क्तिः);
    }
  अन्यथा
    {
      ebuf[-1] = eolchar;
      यदि (fwrite (buf, 1, n_अष्टकानि, fp) != n_अष्टकानि)
        क्रमय_die (_("write failed"), output_file);
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
  संरचना बफर buf;		/* Input buffer. */
  संरचना पङ्क्तिः अस्थायी;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t पङ्क्तिः_सङ्ख्या = 0;
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, परिमाण (संरचना पङ्क्तिः),
           MAX (merge_बफर_आकार, क्रमय_आकार));
  अस्थायी.text = रिक्त;

  यावत् (fillbuf (&buf, fp, file_name, रिक्त))
    {
      संरचना पङ्क्तिः ध्रुव *पङ्क्तिः = बफर_linelim (&buf);
      संरचना पङ्क्तिः ध्रुव *linebase = पङ्क्तिः - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      यदि (alloc && nonunique <= compare (&अस्थायी, पङ्क्तिः - 1))
        {
        प्राप्तम्_disorder:
          {
            यदि (checkonly == 'c')
              {
                संरचना पङ्क्तिः ध्रुव *disorder_पङ्क्तिः = पङ्क्तिः - 1;
                uintmax_t disorder_पङ्क्तिः_सङ्ख्या =
                  बफर_linelim (&buf) - disorder_पङ्क्तिः + पङ्क्तिः_सङ्ख्या;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         क्रमादेशः_name, file_name, disorder_पङ्क्तिः_सङ्ख्या);
                write_पङ्क्तिः (disorder_पङ्क्तिः, stderr, _("standard error"));
              }

            ordered = false;
            भङ्ग;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      यावत् (linebase < --पङ्क्तिः)
        यदि (nonunique <= compare (पङ्क्तिः, पङ्क्तिः - 1))
          गच्छ प्राप्तम्_disorder;

      पङ्क्तिः_सङ्ख्या += buf.nlines;

      /* Save the last line of the buffer.  */
      यदि (alloc < पङ्क्तिः->दीर्घता)
        {
          कुरु
            {
              alloc *= 2;
              यदि (! alloc)
                {
                  alloc = पङ्क्तिः->दीर्घता;
                  भङ्ग;
                }
            }
          यावत् (alloc < पङ्क्तिः->दीर्घता);

          मुक्त (अस्थायी.text);
          अस्थायी.text = xmalloc (alloc);
        }
      स्मृतिं_प्रतिलिख (अस्थायी.text, पङ्क्तिः->text, पङ्क्तिः->दीर्घता);
      अस्थायी.दीर्घता = पङ्क्तिः->दीर्घता;
      यदि (कुञ्चिका)
        {
          अस्थायी.keybeg = अस्थायी.text + (पङ्क्तिः->keybeg - पङ्क्तिः->text);
          अस्थायी.keylim = अस्थायी.text + (पङ्क्तिः->keylim - पङ्क्तिः->text);
        }
    }

  xfclose (fp, file_name);
  मुक्त (buf.buf);
  मुक्त (अस्थायी.text);
  प्रत्यागम ordered;
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
  प्रति (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].अस्थायी && files[i].अस्थायी->state != UNCOMPRESSED
                ? open_अस्थायी (files[i].अस्थायी)
                : प्रवाहः_open (files[i].name, "r"));
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
mergefps (संरचना sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, वर्ण ध्रुव *output_file, FILE **fps)
{
  संरचना बफर *बफर = xnmalloc (nfiles, परिमाण *बफर);
                                /* Input buffers for each file. */
  संरचना पङ्क्तिः saved;		/* Saved line storage for unique check. */
  संरचना पङ्क्तिः ध्रुव *savedline = रिक्त;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  संरचना पङ्क्तिः ध्रुव **cur = xnmalloc (nfiles, परिमाण *cur);
                                /* Current line in each line table. */
  संरचना पङ्क्तिः ध्रुव **आधारः_2 = xnmalloc (nfiles, परिमाण *आधारः_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, परिमाण *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  संरचना keyfield ध्रुव *कुञ्चिका = keylist;
  saved.text = रिक्त;

  /* Read initial lines from each input file. */
  प्रति (size_t i = 0; i < nfiles; )
    {
      initbuf (&बफर[i], परिमाण (संरचना पङ्क्तिः),
               MAX (merge_बफर_आकार, क्रमय_आकार / nfiles));
      यदि (fillbuf (&बफर[i], fps[i], files[i].name, रिक्त))
        {
          संरचना पङ्क्तिः ध्रुव *linelim = बफर_linelim (&बफर[i]);
          cur[i] = linelim - 1;
          आधारः_2[i] = linelim - बफर[i].nlines;
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
          मुक्त (बफर[i].buf);
          --nfiles;
          प्रति (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  प्रति (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  प्रति (size_t i = 1; i < nfiles; ++i)
    यदि (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  यावत् (nfiles)
    {
      संरचना पङ्क्तिः ध्रुव *smallest = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      यदि (unique)
        {
          यदि (savedline && compare (savedline, smallest))
            {
              savedline = रिक्त;
              write_पङ्क्तिः (&saved, ofp, output_file);
            }
          यदि (!savedline)
            {
              savedline = &saved;
              यदि (savealloc < smallest->दीर्घता)
                {
                  कुरु
                    यदि (! savealloc)
                      {
                        savealloc = smallest->दीर्घता;
                        भङ्ग;
                      }
                  यावत् ((savealloc *= 2) < smallest->दीर्घता);

                  मुक्त (saved.text);
                  saved.text = xmalloc (savealloc);
                }
              saved.दीर्घता = smallest->दीर्घता;
              स्मृतिं_प्रतिलिख (saved.text, smallest->text, saved.दीर्घता);
              यदि (कुञ्चिका)
                {
                  saved.keybeg =
                    saved.text + (smallest->keybeg - smallest->text);
                  saved.keylim =
                    saved.text + (smallest->keylim - smallest->text);
                }
            }
        }
      अन्यथा
        write_पङ्क्तिः (smallest, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      यदि (आधारः_2[ord[0]] < smallest)
        cur[ord[0]] = smallest - 1;
      अन्यथा
        {
          यदि (fillbuf (&बफर[ord[0]], fps[ord[0]], files[ord[0]].name,
                       रिक्त))
            {
              संरचना पङ्क्तिः ध्रुव *linelim = बफर_linelim (&बफर[ord[0]]);
              cur[ord[0]] = linelim - 1;
              आधारः_2[ord[0]] = linelim - बफर[ord[0]].nlines;
            }
          अन्यथा
            {
              /* We reached EOF on fps[ord[0]].  */
              प्रति (size_t i = 1; i < nfiles; ++i)
                यदि (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              यदि (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              मुक्त (बफर[ord[0]].buf);
              प्रति (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  बफर[i] = बफर[i + 1];
                  cur[i] = cur[i + 1];
                  आधारः_2[i] = आधारः_2[i + 1];
                }
              प्रति (size_t i = 0; i < nfiles; ++i)
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
        size_t गणनम्_सम्बन्धि_smaller_पङ्क्तयः;

        यावत् (lo < hi)
          {
            पूर्णाङ्क cmp = compare (cur[ord0], cur[ord[probe]]);
            यदि (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            अन्यथा
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        गणनम्_सम्बन्धि_smaller_पङ्क्तयः = lo - 1;
        प्रति (size_t j = 0; j < गणनम्_सम्बन्धि_smaller_पङ्क्तयः; j++)
          ord[j] = ord[j + 1];
        ord[गणनम्_सम्बन्धि_smaller_पङ्क्तयः] = ord0;
      }
    }

  यदि (unique && savedline)
    {
      write_पङ्क्तिः (&saved, ofp, output_file);
      मुक्त (saved.text);
    }

  xfclose (ofp, output_file);
  मुक्त (fps);
  मुक्त (बफर);
  मुक्त (ord);
  मुक्त (आधारः_2);
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
  यदि (nopened < nfiles && nopened < 2)
    क्रमय_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  प्रत्यागम nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

स्थिर शून्य
mergelines (संरचना पङ्क्तिः *सीमित t, size_t nlines,
            संरचना पङ्क्तिः ध्रुव *सीमित lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  संरचना पङ्क्तिः *hi = t - nlo;

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
sequential_क्रमय (संरचना पङ्क्तिः *सीमित पङ्क्तयः, size_t nlines,
                 संरचना पङ्क्तिः *सीमित अस्थायी, bool गन्तव्यम्_अस्थायी)
{
  यदि (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      पूर्णाङ्क swap = (0 < compare (&पङ्क्तयः[-1], &पङ्क्तयः[-2]));
      यदि (गन्तव्यम्_अस्थायी)
        {
          अस्थायी[-1] = पङ्क्तयः[-1 - swap];
          अस्थायी[-2] = पङ्क्तयः[-2 + swap];
        }
      अन्यथा यदि (swap)
        {
          अस्थायी[-1] = पङ्क्तयः[-1];
          पङ्क्तयः[-1] = पङ्क्तयः[-2];
          पङ्क्तयः[-2] = अस्थायी[-1];
        }
    }
  अन्यथा
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      संरचना पङ्क्तिः *lo = पङ्क्तयः;
      संरचना पङ्क्तिः *hi = पङ्क्तयः - nlo;

      sequential_क्रमय (hi, nhi, अस्थायी - (गन्तव्यम्_अस्थायी ? nlo : 0), गन्तव्यम्_अस्थायी);
      यदि (1 < nlo)
        sequential_क्रमय (lo, nlo, अस्थायी, !गन्तव्यम्_अस्थायी);
      अन्यथा यदि (!गन्तव्यम्_अस्थायी)
        अस्थायी[-1] = lo[-1];

      संरचना पङ्क्तिः *dest;
      संरचना पङ्क्तिः ध्रुव *sorted_lo;
      यदि (गन्तव्यम्_अस्थायी)
        {
          dest = अस्थायी;
          sorted_lo = पङ्क्तयः;
        }
      अन्यथा
        {
          dest = पङ्क्तयः;
          sorted_lo = अस्थायी;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

स्थिर संरचना merge_ग्रन्थिः *init_ग्रन्थिः (संरचना merge_ग्रन्थिः *सीमित,
                                     संरचना merge_ग्रन्थिः *सीमित,
                                     संरचना पङ्क्तिः *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
स्थिर संरचना merge_ग्रन्थिः *
merge_वृक्षः_init (size_t nthreads, size_t nlines, संरचना पङ्क्तिः *dest)
{
  संरचना merge_ग्रन्थिः *merge_वृक्षः = xmalloc (2 * परिमाण *merge_वृक्षः * nthreads);

  संरचना merge_ग्रन्थिः *मूलम् = merge_वृक्षः;
  मूलम्->lo = मूलम्->hi = मूलम्->अन्तः_lo = मूलम्->अन्तः_hi = रिक्त;
  मूलम्->dest = रिक्त;
  मूलम्->nlo = मूलम्->nhi = nlines;
  मूलम्->parent = रिक्त;
  मूलम्->level = MERGE_अन्तः;
  मूलम्->queued = false;
  pthread_mutex_init (&मूलम्->तालकम्, रिक्त);

  init_ग्रन्थिः (मूलम्, मूलम् + 1, dest, nthreads, nlines, false);
  प्रत्यागम merge_वृक्षः;
}

/* Destroy the merge tree. */
स्थिर शून्य
merge_वृक्षः_destroy (size_t nthreads, संरचना merge_ग्रन्थिः *merge_वृक्षः)
{
  size_t n_nodes = nthreads * 2;
  संरचना merge_ग्रन्थिः *ग्रन्थिः = merge_वृक्षः;

  यावत् (n_nodes--)
    {
      pthread_mutex_destroy (&ग्रन्थिः->तालकम्);
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
init_ग्रन्थिः (संरचना merge_ग्रन्थिः *सीमित parent,
           संरचना merge_ग्रन्थिः *सीमित ग्रन्थिः_pool,
           संरचना पङ्क्तिः *dest, size_t nthreads,
           size_t योग_पङ्क्तयः, bool is_lo_child)
{
  size_t nlines = (is_lo_child ? parent->nlo : parent->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  संरचना पङ्क्तिः *lo = dest - योग_पङ्क्तयः;
  संरचना पङ्क्तिः *hi = lo - nlo;
  संरचना पङ्क्तिः **parent_अन्तः = (is_lo_child ? &parent->अन्तः_lo : &parent->अन्तः_hi);

  संरचना merge_ग्रन्थिः *ग्रन्थिः = ग्रन्थिः_pool++;
  ग्रन्थिः->lo = ग्रन्थिः->अन्तः_lo = lo;
  ग्रन्थिः->hi = ग्रन्थिः->अन्तः_hi = hi;
  ग्रन्थिः->dest = parent_अन्तः;
  ग्रन्थिः->nlo = nlo;
  ग्रन्थिः->nhi = nhi;
  ग्रन्थिः->parent = parent;
  ग्रन्थिः->level = parent->level + 1;
  ग्रन्थिः->queued = false;
  pthread_mutex_init (&ग्रन्थिः->तालकम्, रिक्त);

  यदि (nthreads > 1)
    {
      size_t lo_तन्तवः = nthreads / 2;
      size_t hi_तन्तवः = nthreads - lo_तन्तवः;
      ग्रन्थिः->lo_child = ग्रन्थिः_pool;
      ग्रन्थिः_pool = init_ग्रन्थिः (ग्रन्थिः, ग्रन्थिः_pool, lo, lo_तन्तवः,
                             योग_पङ्क्तयः, true);
      ग्रन्थिः->hi_child = ग्रन्थिः_pool;
      ग्रन्थिः_pool = init_ग्रन्थिः (ग्रन्थिः, ग्रन्थिः_pool, hi, hi_तन्तवः,
                             योग_पङ्क्तयः, false);
    }
  अन्यथा
    {
      ग्रन्थिः->lo_child = रिक्त;
      ग्रन्थिः->hi_child = रिक्त;
    }
  प्रत्यागम ग्रन्थिः_pool;
}


/* Compare two merge nodes A and B for priority.  */

स्थिर पूर्णाङ्क
compare_nodes (शून्य ध्रुव *a, शून्य ध्रुव *b)
{
  संरचना merge_ग्रन्थिः ध्रुव *nodea = a;
  संरचना merge_ग्रन्थिः ध्रुव *nodeb = b;
  यदि (nodea->level == nodeb->level)
      प्रत्यागम (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  प्रत्यागम nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

स्थिर अन्तःस्थ शून्य
तालकम्_ग्रन्थिः (संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_तालकम् (&ग्रन्थिः->तालकम्);
}

/* Unlock a merge tree NODE. */

स्थिर अन्तःस्थ शून्य
unlock_ग्रन्थिः (संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_unlock (&ग्रन्थिः->तालकम्);
}

/* Destroy merge QUEUE. */

स्थिर शून्य
queue_destroy (संरचना merge_ग्रन्थिः_queue *queue)
{
  heap_free (queue->प्राथमिकता_queue);
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
  queue->प्राथमिकता_queue = heap_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&queue->mutex, रिक्त);
  pthread_cond_init (&queue->cond, रिक्त);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

स्थिर शून्य
queue_निवेशय (संरचना merge_ग्रन्थिः_queue *queue, संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  pthread_mutex_तालकम् (&queue->mutex);
  heap_निवेशय (queue->प्राथमिकता_queue, ग्रन्थिः);
  ग्रन्थिः->queued = true;
  pthread_cond_signal (&queue->cond);
  pthread_mutex_unlock (&queue->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

स्थिर संरचना merge_ग्रन्थिः *
queue_निष्कासय (संरचना merge_ग्रन्थिः_queue *queue)
{
  संरचना merge_ग्रन्थिः *ग्रन्थिः;
  pthread_mutex_तालकम् (&queue->mutex);
  यावत् (! (ग्रन्थिः = heap_remove_शिखरम् (queue->प्राथमिकता_queue)))
    pthread_cond_wait (&queue->cond, &queue->mutex);
  pthread_mutex_unlock (&queue->mutex);
  तालकम्_ग्रन्थिः (ग्रन्थिः);
  ग्रन्थिः->queued = false;
  प्रत्यागम ग्रन्थिः;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

स्थिर शून्य
write_unique (संरचना पङ्क्तिः ध्रुव *पङ्क्तिः, FILE *tfp, वर्ण ध्रुव *अस्थायी_output)
{
  यदि (unique)
    {
      यदि (saved_पङ्क्तिः.text && ! compare (पङ्क्तिः, &saved_पङ्क्तिः))
        प्रत्यागम;
      saved_पङ्क्तिः = *पङ्क्तिः;
    }

  write_पङ्क्तिः (पङ्क्तिः, tfp, अस्थायी_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

स्थिर शून्य
mergelines_ग्रन्थिः (संरचना merge_ग्रन्थिः *सीमित ग्रन्थिः, size_t योग_पङ्क्तयः,
                 FILE *tfp, वर्ण ध्रुव *अस्थायी_output)
{
  संरचना पङ्क्तिः *lo_orig = ग्रन्थिः->lo;
  संरचना पङ्क्तिः *hi_orig = ग्रन्थिः->hi;
  size_t गन्तव्यम्_merge = MAX_MERGE (योग_पङ्क्तयः, ग्रन्थिः->level);
  size_t merged_lo;
  size_t merged_hi;

  यदि (ग्रन्थिः->level > MERGE_मूलम्)
    {
      /* Merge to destination buffer. */
      संरचना पङ्क्तिः *dest = *ग्रन्थिः->dest;
      यावत् (ग्रन्थिः->lo != ग्रन्थिः->अन्तः_lo && ग्रन्थिः->hi != ग्रन्थिः->अन्तः_hi && गन्तव्यम्_merge--)
        यदि (compare (ग्रन्थिः->lo - 1, ग्रन्थिः->hi - 1) <= 0)
          *--dest = *--ग्रन्थिः->lo;
        अन्यथा
          *--dest = *--ग्रन्थिः->hi;

      merged_lo = lo_orig - ग्रन्थिः->lo;
      merged_hi = hi_orig - ग्रन्थिः->hi;

      यदि (ग्रन्थिः->nhi == merged_hi)
        यावत् (ग्रन्थिः->lo != ग्रन्थिः->अन्तः_lo && गन्तव्यम्_merge--)
          *--dest = *--ग्रन्थिः->lo;
      अन्यथा यदि (ग्रन्थिः->nlo == merged_lo)
        यावत् (ग्रन्थिः->hi != ग्रन्थिः->अन्तः_hi && गन्तव्यम्_merge--)
          *--dest = *--ग्रन्थिः->hi;
      *ग्रन्थिः->dest = dest;
    }
  अन्यथा
    {
      /* Merge directly to output. */
      यावत् (ग्रन्थिः->lo != ग्रन्थिः->अन्तः_lo && ग्रन्थिः->hi != ग्रन्थिः->अन्तः_hi && गन्तव्यम्_merge--)
        {
          यदि (compare (ग्रन्थिः->lo - 1, ग्रन्थिः->hi - 1) <= 0)
            write_unique (--ग्रन्थिः->lo, tfp, अस्थायी_output);
          अन्यथा
            write_unique (--ग्रन्थिः->hi, tfp, अस्थायी_output);
        }

      merged_lo = lo_orig - ग्रन्थिः->lo;
      merged_hi = hi_orig - ग्रन्थिः->hi;

      यदि (ग्रन्थिः->nhi == merged_hi)
        {
          यावत् (ग्रन्थिः->lo != ग्रन्थिः->अन्तः_lo && गन्तव्यम्_merge--)
            write_unique (--ग्रन्थिः->lo, tfp, अस्थायी_output);
        }
      अन्यथा यदि (ग्रन्थिः->nlo == merged_lo)
        {
          यावत् (ग्रन्थिः->hi != ग्रन्थिः->अन्तः_hi && गन्तव्यम्_merge--)
            write_unique (--ग्रन्थिः->hi, tfp, अस्थायी_output);
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
  यदि (! ग्रन्थिः->queued)
    {
      bool lo_avail = (ग्रन्थिः->lo - ग्रन्थिः->अन्तः_lo) != 0;
      bool hi_avail = (ग्रन्थिः->hi - ग्रन्थिः->अन्तः_hi) != 0;
      यदि (lo_avail ? hi_avail || ! ग्रन्थिः->nhi : hi_avail && ! ग्रन्थिः->nlo)
        queue_निवेशय (queue, ग्रन्थिः);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

स्थिर शून्य
queue_check_निवेशय_parent (संरचना merge_ग्रन्थिः_queue *queue,
                           संरचना merge_ग्रन्थिः *ग्रन्थिः)
{
  यदि (ग्रन्थिः->level > MERGE_मूलम्)
    {
      तालकम्_ग्रन्थिः (ग्रन्थिः->parent);
      queue_check_निवेशय (queue, ग्रन्थिः->parent);
      unlock_ग्रन्थिः (ग्रन्थिः->parent);
    }
  अन्यथा यदि (ग्रन्थिः->nlo + ग्रन्थिः->nhi == 0)
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
            size_t योग_पङ्क्तयः, FILE *tfp, वर्ण ध्रुव *अस्थायी_output)
{
  यावत् (true)
    {
      संरचना merge_ग्रन्थिः *ग्रन्थिः = queue_निष्कासय (queue);

      यदि (ग्रन्थिः->level == MERGE_अन्तः)
        {
          unlock_ग्रन्थिः (ग्रन्थिः);
          /* Reinsert so other threads can pop it. */
          queue_निवेशय (queue, ग्रन्थिः);
          भङ्ग;
        }
      mergelines_ग्रन्थिः (ग्रन्थिः, योग_पङ्क्तयः, tfp, अस्थायी_output);
      queue_check_निवेशय (queue, ग्रन्थिः);
      queue_check_निवेशय_parent (queue, ग्रन्थिः);

      unlock_ग्रन्थिः (ग्रन्थिः);
    }
}


स्थिर शून्य sortlines (संरचना पङ्क्तिः *सीमित, size_t, size_t,
                       संरचना merge_ग्रन्थिः *, संरचना merge_ग्रन्थिः_queue *,
                       FILE *, वर्ण ध्रुव *);

/* Thread arguments for sortlines_thread. */

संरचना thread_तर्काः
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  संरचना पङ्क्तिः *पङ्क्तयः;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ध्रुव योग_पङ्क्तयः;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  संरचना merge_ग्रन्थिः *ध्रुव ग्रन्थिः;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  संरचना merge_ग्रन्थिः_queue *ध्रुव queue;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  वर्ण ध्रुव *output_अस्थायी;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

स्थिर शून्य *
sortlines_thread (शून्य *दत्त)
{
  संरचना thread_तर्काः ध्रुव *तर्काः = दत्त;
  sortlines (तर्काः->पङ्क्तयः, तर्काः->nthreads, तर्काः->योग_पङ्क्तयः,
             तर्काः->ग्रन्थिः, तर्काः->queue, तर्काः->tfp,
             तर्काः->output_अस्थायी);
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
sortlines (संरचना पङ्क्तिः *सीमित पङ्क्तयः, size_t nthreads,
           size_t योग_पङ्क्तयः, संरचना merge_ग्रन्थिः *ग्रन्थिः,
           संरचना merge_ग्रन्थिः_queue *queue, FILE *tfp, वर्ण ध्रुव *अस्थायी_output)
{
  size_t nlines = ग्रन्थिः->nlo + ग्रन्थिः->nhi;

  /* Calculate thread arguments. */
  size_t lo_तन्तवः = nthreads / 2;
  size_t hi_तन्तवः = nthreads - lo_तन्तवः;
  pthread_t thread;
  संरचना thread_तर्काः तर्काः = {पङ्क्तयः, lo_तन्तवः, योग_पङ्क्तयः,
                             ग्रन्थिः->lo_child, queue, tfp, अस्थायी_output};

  यदि (nthreads > 1 && SUBTHREAD_पङ्क्तयः_HEURISTIC <= nlines
      && pthread_create (&thread, रिक्त, sortlines_thread, &तर्काः) == 0)
    {
      sortlines (पङ्क्तयः - ग्रन्थिः->nlo, hi_तन्तवः, योग_पङ्क्तयः,
                 ग्रन्थिः->hi_child, queue, tfp, अस्थायी_output);
      pthread_join (thread, रिक्त);
    }
  अन्यथा
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ग्रन्थिः->nlo;
      size_t nhi = ग्रन्थिः->nhi;
      संरचना पङ्क्तिः *अस्थायी = पङ्क्तयः - योग_पङ्क्तयः;
      यदि (1 < nhi)
        sequential_क्रमय (पङ्क्तयः - nlo, nhi, अस्थायी - nlo / 2, false);
      यदि (1 < nlo)
        sequential_क्रमय (पङ्क्तयः, nlo, अस्थायी, false);

      /* Update merge NODE. No need to lock yet. */
      ग्रन्थिः->lo = पङ्क्तयः;
      ग्रन्थिः->hi = पङ्क्तयः - nlo;
      ग्रन्थिः->अन्तः_lo = पङ्क्तयः - nlo;
      ग्रन्थिः->अन्तः_hi = पङ्क्तयः - nlo - nhi;

      queue_निवेशय (queue, ग्रन्थिः);
      merge_loop (queue, योग_पङ्क्तयः, tfp, अस्थायी_output);
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

  प्रति (size_t i = ntemps; i < nfiles; i++)
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
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      यदि (same)
        {
          यदि (! tempcopy)
            {
              FILE *tftp;
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

स्थिर शून्य
check_inputs (वर्ण *ध्रुव *files, size_t nfiles)
{
  प्रति (size_t i = 0; i < nfiles; i++)
    {
      यदि (streq (files[i], "-"))
        अनुवर्त;

      यदि (euidaccess (files[i], R_OK) != 0)
        क्रमय_die (_("cannot read"), files[i]);
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
      पूर्णाङ्क oflags = O_WRONLY | O_द्विमान | O_CLOEXEC | O_CREAT;
      पूर्णाङ्क outfd = उद्घाटय (outfile, oflags, बहुलकम्_RW_UGO);
      यदि (outfd < 0)
        क्रमय_die (_("open failed"), outfile);
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
  यावत् (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t अन्तर्;

      /* Number of output files generated so far.  */
      size_t बहिर्गमनम्;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      प्रति (बहिर्गमनम् = अन्तर् = 0; nmerge <= nfiles - अन्तर्; बहिर्गमनम्++)
        {
          FILE *tfp;
          संरचना tempnode *अस्थायी = create_अस्थायी (&tfp);
          size_t अङ्कः_merged = mergefiles (&files[अन्तर्], MIN (ntemps, nmerge),
                                          nmerge, tfp, अस्थायी->name);
          ntemps -= MIN (ntemps, अङ्कः_merged);
          files[बहिर्गमनम्].name = अस्थायी->name;
          files[बहिर्गमनम्].अस्थायी = अस्थायी;
          अन्तर् += अङ्कः_merged;
        }

      remainder = nfiles - अन्तर्;
      cheap_slots = nmerge - बहिर्गमनम् % nmerge;

      यदि (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          संरचना tempnode *अस्थायी = create_अस्थायी (&tfp);
          size_t अङ्कः_merged = mergefiles (&files[अन्तर्], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, अस्थायी->name);
          ntemps -= MIN (ntemps, अङ्कः_merged);
          files[बहिर्गमनम्].name = अस्थायी->name;
          files[बहिर्गमनम्++].अस्थायी = अस्थायी;
          अन्तर् += अङ्कः_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[बहिर्गमनम्], &files[अन्तर्], (nfiles - अन्तर्) * परिमाण *files);
      ntemps += बहिर्गमनम्;
      nfiles -= अन्तर् - बहिर्गमनम्;
    }

  avoid_trashing_निवेशः (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  यावत् (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_निवेशः_files (files, nfiles, &fps);

      यदि (nopened == nfiles)
        {
          FILE *ofp = प्रवाहः_open (output_file, "w");
          यदि (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              भङ्ग;
            }
          यदि (errno != EMFILE || nopened <= 2)
            क्रमय_die (_("open failed"), output_file);
        }
      अन्यथा यदि (nopened <= 2)
        क्रमय_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      संरचना tempnode *अस्थायी;
      कुरु
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          अस्थायी = maybe_create_अस्थायी (&tfp, ! (nopened <= 2));
        }
      यावत् (!अस्थायी);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, अस्थायी->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = अस्थायी->name;
      files[0].अस्थायी = अस्थायी;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * परिमाण *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

स्थिर शून्य
क्रमय (वर्ण *ध्रुव *files, size_t nfiles, वर्ण ध्रुव *output_file,
      size_t nthreads)
{
  संरचना बफर buf;
  संरचना क्रमय_बफर_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.सीमा = 0;
  policy.growth_failed = false;

  यावत् (nfiles)
    {
      वर्ण ध्रुव *अस्थायी_output;
      वर्ण ध्रुव *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t अष्टकानि_प्रत्येकम्_पङ्क्तिः;
      यदि (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          यावत् (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          अष्टकानि_प्रत्येकम्_पङ्क्तिः = (mult * परिमाण (संरचना पङ्क्तिः));
        }
      अन्यथा
        अष्टकानि_प्रत्येकम्_पङ्क्तिः = परिमाण (संरचना पङ्क्तिः) * 3 / 2;

      यदि (! buf.alloc)
        {
          क्रमय_बफर_policy (&fp, 1, files, nfiles, अष्टकानि_प्रत्येकम्_पङ्क्तिः,
                              &policy);
          initbuf (&buf, अष्टकानि_प्रत्येकम्_पङ्क्तिः, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      यावत् (fillbuf (&buf, fp, file, &policy))
        {
          संरचना पङ्क्तिः *पङ्क्तिः;

          यदि (buf.eof && nfiles
              && (अष्टकानि_प्रत्येकम्_पङ्क्तिः + 1
                  < (buf.alloc - buf.प्रयुक्तम् - अष्टकानि_प्रत्येकम्_पङ्क्तिः * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.वामः = buf.प्रयुक्तम्;
              भङ्ग;
            }

          saved_पङ्क्तिः.text = रिक्त;
          पङ्क्तिः = बफर_linelim (&buf);
          यदि (buf.eof && !nfiles && !ntemps && !buf.वामः)
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
          यदि (1 < buf.nlines)
            {
              यदि (nthreads > 1)
                {
                  संरचना merge_ग्रन्थिः_queue queue;
                  queue_init (&queue, nthreads);
                  संरचना merge_ग्रन्थिः *merge_वृक्षः =
                    merge_वृक्षः_init (nthreads, buf.nlines, पङ्क्तिः);

                  sortlines (पङ्क्तिः, nthreads, buf.nlines, merge_वृक्षः + 1,
                             &queue, tfp, अस्थायी_output);

                  merge_वृक्षः_destroy (nthreads, merge_वृक्षः);
                  queue_destroy (&queue);
                }
              अन्यथा
                {
                  sequential_क्रमय (पङ्क्तिः, buf.nlines,
                                   पङ्क्तिः - buf.nlines, false);
                  प्रति (size_t i = 0; i < buf.nlines; i++)
                    write_unique (पङ्क्तिः - i - 1, tfp, अस्थायी_output);
                }
            }
          अन्यथा
            write_unique (पङ्क्तिः - 1, tfp, अस्थायी_output);

          xfclose (tfp, अस्थायी_output);

          यदि (output_file_created)
            गच्छ finish;
        }
      xfclose (fp, file);
    }

 finish:
  मुक्त (buf.buf);

  यदि (! output_file_created)
    {
      संरचना tempnode *ग्रन्थिः = temphead;
      संरचना sortfile *tempfiles = xnmalloc (ntemps, परिमाण *tempfiles);
      प्रति (size_t i = 0; ग्रन्थिः; i++)
        {
          tempfiles[i].name = ग्रन्थिः->name;
          tempfiles[i].अस्थायी = ग्रन्थिः;
          ग्रन्थिः = ग्रन्थिः->अग्र;
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

  प्रति (p = &keylist; *p; p = &(*p)->अग्र)
    अनुवर्त;
  *p = कुञ्चिका;
  कुञ्चिका->अग्र = रिक्त;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

स्थिर शून्य
badfieldspec (वर्ण ध्रुव *spec, वर्ण ध्रुव *msgid)
{
  error (क्रमय_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

स्थिर शून्य
incompatible_options (वर्ण ध्रुव *opts)
{
  error (क्रमय_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

स्थिर शून्य
check_ordering_compatibility (शून्य)
{
  प्रति (संरचना keyfield *कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->अग्र)
    यदि (1 < (कुञ्चिका->numeric + कुञ्चिका->general_numeric + कुञ्चिका->human_numeric
             + कुञ्चिका->month + (कुञ्चिका->संस्करणम्_3 | कुञ्चिका->random | !!कुञ्चिका->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        वर्ण opts[परिमाण short_options];
        /* Clear flags we're not interested in.  */
        कुञ्चिका->skipsblanks = कुञ्चिका->skipeblanks = कुञ्चिका->reverse = false;
        कुञ्चिका_गन्तव्यम्_opts (कुञ्चिका, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

स्थिर वर्ण ध्रुव *
parse_क्षेत्रम्_गणनम् (वर्ण ध्रुव *string, size_t *val, वर्ण ध्रुव *msgid)
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
    पक्ष LONGINT_अतिप्रवाहः:
    पक्ष LONGINT_अतिप्रवाहः | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      भङ्ग;

    पक्ष LONGINT_INVALID:
      यदि (msgid)
        error (क्रमय_FAILURE, 0, _("%s: invalid count at start of %s"),
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
समुच्चयः_ordering (वर्ण ध्रुव *s, संरचना keyfield *कुञ्चिका, गणना blanktype blanktype)
{
  यावत् (*s)
    {
      विकल्प (*s)
        {
        पक्ष 'b':
          यदि (blanktype == bl_start || blanktype == bl_both)
            कुञ्चिका->skipsblanks = true;
          यदि (blanktype == bl_अन्तः || blanktype == bl_both)
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
          यदि (! कुञ्चिका->ignore)
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
          कुञ्चिका->संस्करणम्_3 = true;
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
कुञ्चिका_init (संरचना keyfield *कुञ्चिका)
{
  स्मृतिं_पूरय (कुञ्चिका, 0, परिमाण *कुञ्चिका);
  कुञ्चिका->eword = SIZE_MAX;
  प्रत्यागम कुञ्चिका;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
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
  पूर्णाङ्क posix_ver = posix2_संस्करणम् ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  वर्ण **files;
  वर्ण *files_from = रिक्त;
  संरचना Tokens tok;
  वर्ण ध्रुव *outfile = रिक्त;
  bool locale_ok;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (क्रमय_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#यदि HAVE_NL_LANGINFO
  hard_LC_TIME = hard_locale (LC_TIME);
#यदि_अन्तः

  /* Get locale's representation of the decimal point.  */
  {
    संरचना lconv ध्रुव *locale = localeconv ();

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
    गणना { nsigs = countof (पदांशः_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_सङ्केताः);
    प्रति (size_t i = 0; i < nsigs; i++)
      {
        यदि (पदांशः_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        sigaction (पदांशः_sig[i], रिक्त, &act);
        यदि (act.sa_handler != SIG_IGN)
          sigaddset (&caught_सङ्केताः, पदांशः_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_सङ्केताः;
    act.sa_flags = 0;

    प्रति (size_t i = 0; i < nsigs; i++)
      {
        यदि (पदांशः_sig[i] == SIGPIPE)
          अनुवर्त;  /* Handled below.  */
        यदि (sigismember (&caught_सङ्केताः, पदांशः_sig[i]))
          sigaction (पदांशः_sig[i], &act, रिक्त);
      }
  }
  सङ्केत (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  कुञ्चिका_init (&gkey);
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
          कुञ्चिका = रिक्त;
          यदि (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              यदि (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  कुञ्चिका = कुञ्चिका_init (&कुञ्चिका_buf);
                  s = parse_क्षेत्रम्_गणनम् (optarg + 1, &कुञ्चिका->sword, रिक्त);
                  यदि (s && *s == '.')
                    s = parse_क्षेत्रम्_गणनम् (s + 1, &कुञ्चिका->schar, रिक्त);
                  यदि (! (कुञ्चिका->sword || कुञ्चिका->schar))
                    कुञ्चिका->sword = SIZE_MAX;
                  यदि (! s || *समुच्चयः_ordering (s, कुञ्चिका, bl_start))
                    कुञ्चिका = रिक्त;
                  अन्यथा
                    {
                      यदि (minus_pos_usage)
                        {
                          वर्ण ध्रुव *optarg1 = argv[optind++];
                          s = parse_क्षेत्रम्_गणनम् (optarg1 + 1, &कुञ्चिका->eword,
                                             N_("invalid number after '-'"));
                          यदि (*s == '.')
                            s = parse_क्षेत्रम्_गणनम् (s + 1, &कुञ्चिका->echar,
                                               N_("invalid number after '.'"));
                          यदि (!कुञ्चिका->echar && कुञ्चिका->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              कुञ्चिका->eword--;
                            }
                          यदि (*समुच्चयः_ordering (s, कुञ्चिका, bl_अन्तः))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      कुञ्चिका->traditional_प्रयुक्तम् = true;
                      insertkey (कुञ्चिका);
                    }
                }
            }
          यदि (! कुञ्चिका)
            files[nfiles++] = optarg;
          भङ्ग;

        पक्ष क्रमय_OPTION:
          c = XARGMATCH ("--sort", optarg, क्रमय_तर्काः, क्रमय_types);
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
            समुच्चयः_ordering (str, &gkey, bl_both);
          }
          भङ्ग;

        पक्ष CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_तर्काः, check_types)
               : 'c');
          FALLTHROUGH;
        पक्ष 'c':
        पक्ष 'C':
          यदि (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          भङ्ग;

        पक्ष COMPRESS_क्रमादेशः_OPTION:
          यदि (compress_क्रमादेशः && !streq (compress_क्रमादेशः, optarg))
            error (क्रमय_FAILURE, 0, _("multiple compress programs specified"));
          compress_क्रमादेशः = optarg;
          भङ्ग;

        पक्ष DEBUG_क्रमादेशः_OPTION:
          debug = true;
          भङ्ग;

        पक्ष FILES0_FROM_OPTION:
          files_from = optarg;
          भङ्ग;

        पक्ष 'k':
          कुञ्चिका = कुञ्चिका_init (&कुञ्चिका_buf);

          /* Get POS1. */
          s = parse_क्षेत्रम्_गणनम् (optarg, &कुञ्चिका->sword,
                                 N_("invalid number at field start"));
          यदि (! कुञ्चिका->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          यदि (*s == '.')
            {
              s = parse_क्षेत्रम्_गणनम् (s + 1, &कुञ्चिका->schar,
                                     N_("invalid number after '.'"));
              यदि (! कुञ्चिका->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          यदि (! (कुञ्चिका->sword || कुञ्चिका->schar))
            कुञ्चिका->sword = SIZE_MAX;
          s = समुच्चयः_ordering (s, कुञ्चिका, bl_start);
          यदि (*s != ',')
            {
              कुञ्चिका->eword = SIZE_MAX;
              कुञ्चिका->echar = 0;
            }
          अन्यथा
            {
              /* Get POS2. */
              s = parse_क्षेत्रम्_गणनम् (s + 1, &कुञ्चिका->eword,
                                     N_("invalid number after ','"));
              यदि (! कुञ्चिका->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              यदि (*s == '.')
                {
                  s = parse_क्षेत्रम्_गणनम् (s + 1, &कुञ्चिका->echar,
                                         N_("invalid number after '.'"));
                }
              s = समुच्चयः_ordering (s, कुञ्चिका, bl_अन्तः);
            }
          यदि (*s)
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
          यदि (outfile && !streq (outfile, optarg))
            error (क्रमय_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          भङ्ग;

        पक्ष RANDOM_SOURCE_OPTION:
          यदि (random_source && !streq (random_source, optarg))
            error (क्रमय_FAILURE, 0, _("multiple random sources specified"));
          random_source = optarg;
          भङ्ग;

        पक्ष 's':
          stable = true;
          भङ्ग;

        पक्ष 'S':
          specify_क्रमय_आकार (oi, c, optarg);
          भङ्ग;

        पक्ष 't':
          {
            वर्ण newtab = optarg[0];
            यदि (! newtab)
              error (क्रमय_FAILURE, 0, _("empty tab"));
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
                    error (क्रमय_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            यदि (tab != TAB_DEFAULT && tab != newtab)
              error (क्रमय_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          भङ्ग;

        पक्ष 'T':
          सङ्कलय_अस्थायी_dir (optarg);
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

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (क्रमय_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (सङ्केत (SIGPIPE, SIG_IGN) == SIG_DFL);

  यदि (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      यदि (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (क्रमय_FAILURE);
        }

      FILE *प्रवाहः = xfopen (files_from, "r");

      readtokens0_init (&tok);

      यदि (! readtokens0 (प्रवाहः, &tok))
        error (क्रमय_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (प्रवाहः, files_from);

      यदि (tok.n_tok)
        {
          मुक्त (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          प्रति (size_t i = 0; i < nfiles; i++)
            {
              यदि (streq (files[i], "-"))
                error (क्रमय_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              अन्यथा यदि (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  अचिह्नित दीर्घ पूर्णाङ्क file_सङ्ख्या = i + 1;
                  error (क्रमय_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_सङ्ख्या);
                }
            }
        }
      अन्यथा
        error (क्रमय_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  प्रति (कुञ्चिका = keylist; कुञ्चिका; कुञ्चिका = कुञ्चिका->अग्र)
    {
      यदि (default_कुञ्चिका_compare (कुञ्चिका) && !कुञ्चिका->reverse)
        {
          कुञ्चिका->ignore = gkey.ignore;
          कुञ्चिका->translate = gkey.translate;
          कुञ्चिका->skipsblanks = gkey.skipsblanks;
          कुञ्चिका->skipeblanks = gkey.skipeblanks;
          कुञ्चिका->month = gkey.month;
          कुञ्चिका->numeric = gkey.numeric;
          कुञ्चिका->general_numeric = gkey.general_numeric;
          कुञ्चिका->human_numeric = gkey.human_numeric;
          कुञ्चिका->संस्करणम्_3 = gkey.संस्करणम्_3;
          कुञ्चिका->random = gkey.random;
          कुञ्चिका->reverse = gkey.reverse;
        }

      need_random |= कुञ्चिका->random;
    }

  यदि (!keylist && !default_कुञ्चिका_compare (&gkey))
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
        locale_ok = !! setlocale (LC_COLLATE, "");
      यदि (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      यदि (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, रिक्त)));
      अन्यथा
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      कुञ्चिका_warnings (&gkey, gkey_only);
    }

  reverse = gkey.reverse;

  यदि (need_random)
    random_md5_state_init (random_source);

  यदि (अस्थायी_dirs.n_dirs == 0)
    {
      वर्ण ध्रुव *tmp_dir = getenv ("TMPDIR");
      सङ्कलय_अस्थायी_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
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
  यदि (0 < क्रमय_आकार)
    क्रमय_आकार = MAX (क्रमय_आकार, MIN_क्रमय_आकार);

  यदि (checkonly)
    {
      यदि (nfiles > 1)
        error (क्रमय_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      यदि (outfile)
        {
          स्थिर वर्ण opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      निर्गच्छ (check (files[0], checkonly) ? EXIT_SUCCESS : क्रमय_बहिर्गमनम्_सम्बन्धि_क्रमः);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  यदि (mergeonly)
    {
      संरचना sortfile *sortfiles = xcalloc (nfiles, परिमाण *sortfiles);

      प्रति (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      merge (sortfiles, 0, nfiles, outfile);
    }
  अन्यथा
    {
      यदि (!nthreads)
        {
          अचिह्नित दीर्घ पूर्णाङ्क np = अङ्कः_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_तन्तवः);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * परिमाण (संरचना merge_ग्रन्थिः));
      nthreads = MIN (nthreads, nthreads_max);

      क्रमय (files, nfiles, outfile, nthreads);
    }

  यदि (have_read_stdin && fclose (stdin) == EOF)
    क्रमय_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
