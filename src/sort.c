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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <pthread.h>
#ضمن <sys/resource.h>
#ضمن <sys/types.h>
#ضمن <sys/wait.h>
#ضمن <signal.h>
#ضمن <spawn.h>
#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"
#ضمن "fadvise.h"
#ضمن "filevercmp.h"
#ضمن "findprog.h"
#ضمن "flexmember.h"
#ضمن "hard-locale.h"
#ضمن "hash.h"
#ضمن "heap.h"
#ضمن "ignore-value.h"
#ضمن "mbswidth.h"
#ضمن "nproc.h"
#ضمن "physmem.h"
#ضمن "posixver.h"
#ضمن "quote.h"
#ضمن "randread.h"
#ضمن "readtokens0.h"
#ضمن "stdlib--.h"
#ضمن "strnumcmp.h"
#ضمن "term-sig.h"
#ضمن "unistd--.h"
#ضمن "xmemcoll.h"
#ضمن "xnanosleep.h"
#ضمن "xstrtol.h"
#ضمن "xstrtol-error.h"

#که_نه_تعریف_شوی RLIMIT_معلومات
بنية rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#د_که_پای

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "sort"

#تعریف_کړه AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#د_که_پای

#إذا !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#د_که_پای
#إذا !defined OPEN_MAX
# define OPEN_MAX 20
#د_که_پای

#تعریف_کړه UCHAR_LIM (UCHAR_MAX + 1)

#که_نه_تعریف_شوی DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#د_که_پای

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#تعریف_کړه MAX_یوځای(ټول, level) (((ټول) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
تعداد { SUBTHREAD_کرښې_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_کرښې_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
تعداد { DEFAULT_MAX_تارونه = 8 };

/* Exit statuses.  */
تعداد
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    ترتیبول_بهر_د_ترتیب = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    ترتیبول_FAILURE = 2
  };

تعداد
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

تعداد
  {
    /* Level of the end-of-merge node, one level above the root. */
    یوځای_پای_ټکی = 0,

    /* Level of the root node in merge tree. */
    یوځای_ریښه = 1
  };

/* The representation of the decimal point in the current locale.  */
ساكن حرف decimal_ټکی;

/* Thousands separator; if outside char range, there is no separator.  */
ساكن صحيح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساكن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساكن bool hard_LC_COLLATE;
#إذا HAVE_NL_LANGINFO
ساكن bool hard_LC_وخت;
#د_که_پای

#تعریف_کړه NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
تعداد blanktype { bl_پیل, bl_پای_ټکی, bl_both };

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
بنية کرښه
{
  حرف *متن;			/* Text of the line. */
  size_t اوږدتیا;		/* Length including final newline. */
  حرف *keybeg;			/* Start of first key. */
  حرف *keylim;			/* Limit of first key. */
};

/* Input buffers. */
بنية بفر
{
  حرف *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t کارول_شوی;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t کیڼ;			/* Number of bytes left from previous reads. */
  size_t کرښه_بایټونه;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
بنية ترتیبول_بفر_policy
{
  size_t initial;
  size_t حد;
  bool growth_failed;
};

/* Sort key.  */
بنية keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ثابت *ignore;		/* Boolean array of characters to ignore. */
  حرف ثابت *translate;	/* Translation applied to characters. */
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
  bool سرچپه;			/* Reverse the sense of comparison. */
  bool نسخه_3;			/* sort by version number */
  bool traditional_کارول_شوی;	/* Traditional key option format is used. */
  بنية keyfield *بل;	/* Next keyfield to try. */
};

بنية month
{
  حرف ثابت *name;
  صحيح val;
};

/* Binary merge tree node. */
بنية یوځای_غوټه
{
  بنية کرښه *lo;              /* Lines to merge from LO child node. */
  بنية کرښه *hi;              /* Lines to merge from HI child node. */
  بنية کرښه *پای_ټکی_lo;          /* End of available lines from LO. */
  بنية کرښه *پای_ټکی_hi;          /* End of available lines from HI. */
  بنية کرښه **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  بنية یوځای_غوټه *پلار;    /* Parent node. */
  بنية یوځای_غوټه *lo_ماشوم;  /* LO child node. */
  بنية یوځای_غوټه *hi_ماشوم;  /* HI child node. */
  بینښه صحيح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t قلف;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
بنية یوځای_غوټه_کتار
{
  بنية ډېرۍ *لومړیتوب_کتار;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساكن بنية کرښه saved_کرښه;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ساكن bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ساكن bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ساكن bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ساكن حرف fold_toupper[UCHAR_LIM];

#تعریف_کړه MONTHS_فی_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ساكن بنية month monthtab[] =
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
#تعریف_کړه NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#تعریف_کړه MIN_یوځای_بفر_اندازه (2 + حجم (بنية کرښه))

/* Minimum sort size; the code might not work with smaller sizes.  */
#تعریف_کړه MIN_ترتیبول_اندازه (nmerge * MIN_یوځای_بفر_اندازه)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ساكن size_t یوځای_بفر_اندازه = MAX (MIN_یوځای_بفر_اندازه, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ساكن size_t ترتیبول_اندازه;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#تعریف_کړه ننوت_FILE_اندازه_GUESS (128 * 1024)

ساكن بنية
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  حرف ثابت **dirs COUNTED_BY (n_dirs);
} لنډمهاله_dirs;



/* Flag to reverse the order of all comparisons. */
ساكن bool سرچپه;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ساكن bool stable;

/* An int value outside char range.  */
تعداد { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
تعداد { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ساكن صحيح tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ساكن bool unique;

/* Nonzero if any of the input files are the standard input. */
ساكن bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ساكن بنية keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ساكن حرف ثابت *compress_پروګرام;

/* Annotate the output with extra info to aid the user.  */
ساكن bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ساكن بینښه صحيح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ساكن bool default_SIGPIPE;

/* The list of temporary files. */
بنية tempnode
{
  بنية tempnode *متطاير بل;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  حرف state;
  حرف name[FLEXIBLE_ARRAY_MEMBER];
};
ساكن بنية tempnode *متطاير temphead;
ساكن بنية tempnode *متطاير *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ساكن فراغ
cleanup (فراغ)
{
  لكل (بنية tempnode ثابت *غوټه = temphead; غوټه; غوټه = غوټه->بل)
    unlink (غوټه->name);
  temphead = عدم;
}

/* Handle interrupts and hangups. */

ساكن فراغ
sighandler (صحيح sig)
{
  cleanup ();

  إشارة (sig, SIG_DFL);
  ارفع (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ساكن فراغ
ترتیبول_die (حرف ثابت *پیغام_2, حرف ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  إذا (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (ترتیبول_FAILURE, errno, "%s: %s", پیغام_2,
         quotef (file ? file : _("standard output")));
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروګرام_name, پروګرام_name);
      fputs (_("\
Write sorted concatenation د all FILE(s) منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding ترتیبول کلیګانې دننه each کرښه\n\
"));
      oputs (_("\
  -d, --dictionary-ترتیب\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالة\n\
         fold ښکتنی حالة منزل پاسنی حالة characters\n\
"));
      oputs (_("\
  -g, --general-numeric-ترتیبول\n\
         compare according منزل general numerical ارزښت\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-ترتیبول\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-ترتیبول\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-ترتیبول\n\
         compare according منزل string numerical ارزښت;\n\
         see ډک documentation لكل supported مزي_ګانې\n\
"));
      oputs (_("\
  -R, --random-ترتیبول\n\
         shuffle, but ډله identical کلیګانې.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-سرچینه_2=FILE\n\
         get random بایټونه from FILE\n\
"));
      oputs (_("\
  -r, --سرچپه\n\
         سرچپه the پایله د comparisons\n\
"));
      oputs (_("\
      --ترتیبول=کلمه\n\
         ترتیبول according منزل کلمه:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, نسخه_3 -V\n\
"));
      oputs (_("\
  -V, --نسخه_3-ترتیبول\n\
         natural ترتیبول د (نسخه_3) numbers within متن\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-اندازه=NMERGE\n\
         یوځای at most NMERGE inputs at once; لكل نور use لنډمهاله files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-لومړی\n\
         check لكل sorted ننوت; افعل not ترتیبول\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but افعل not report لومړی bad کرښه\n\
"));
      oputs (_("\
      --compress-پروګرام=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part د the کرښه کارول_شوی منزل ترتیبول,\n\
         and warn about questionable usage منزل standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ ننوت from the files specified by NUL-terminated نومونه دننه file F;\
\n\
         If F is -, اقرأ نومونه from standard ننوت\n\
"));
      oputs (_("\
  -k, --کلی=KEYDEF\n\
         ترتیبول له_لارې a کلی; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --یوځای\n\
         یوځای already sorted files; افعل not ترتیبول\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         اكتب پایله منزل FILE instead د standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize ترتیبول by disabling وروستی-resort comparison\n\
"));
      oputs (_("\
  -S, --بفر-اندازه=اندازه_2\n\
         use اندازه_2 لكل اصلي حافظه بفر\n\
"));
      oputs (_("\
  -t, --ډګر-separator=SEP\n\
         use SEP instead د non-blank منزل blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR لكل temporaries, not $TMPDIR or %s;\n\
         مضرب options specify مضرب directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the شمېره د sorts پرلپسې_2 concurrently منزل N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the لومړی د کرښې with equal کلیګانې;\n\
         with -c, check لكل strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         کرښه delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] لكل پیل and stop position, where F is a\n\
ډګر شمېره and C a character position دننه the ډګر; both are origin 1, and\n\
the stop position defaults منزل the کرښه's پای_ټکی.  If neither -t nor -b is دننه\n\
effect, characters دننه a ډګر are counted from the beginning د the preceding\n\
whitespace.  OPTS is one or نور single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options لكل that کلی.  If no کلی is given, use\n\
the entire کرښه as the کلی.  Use --debug منزل diagnose incorrect کلی usage.\n\
\n\
اندازه_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% د حافظه, b 1, K 1024 (افتراضي), and so on لكل M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects ترتیبول ترتیب.\n\
سېټ_3 LC_ALL=C منزل get the traditional ترتیبول ترتیب that کاروي\n\
native byte ارزښتونه.\n\
"), stdout );
      emit_ancillary_info (پروګرام_NAME);
    }

  وتل (حالت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_پروګرام_OPTION,
  DEBUG_پروګرام_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_سرچینه_OPTION,
  ترتیبول_OPTION,
  PARALLEL_OPTION
};

ساكن حرف ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ساكن بنية option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_دلیل, عدم, 'b'},
  {"check", optional_دلیل, عدم, CHECK_OPTION},
  {"compress-program", required_دلیل, عدم, COMPRESS_پروګرام_OPTION},
  {"debug", no_دلیل, عدم, DEBUG_پروګرام_OPTION},
  {"dictionary-order", no_دلیل, عدم, 'd'},
  {"ignore-case", no_دلیل, عدم, 'f'},
  {"files0-from", required_دلیل, عدم, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_دلیل, عدم, 'g'},
  {"ignore-nonprinting", no_دلیل, عدم, 'i'},
  {"key", required_دلیل, عدم, 'k'},
  {"merge", no_دلیل, عدم, 'm'},
  {"month-sort", no_دلیل, عدم, 'M'},
  {"numeric-sort", no_دلیل, عدم, 'n'},
  {"human-numeric-sort", no_دلیل, عدم, 'h'},
  {"version-sort", no_دلیل, عدم, 'V'},
  {"random-sort", no_دلیل, عدم, 'R'},
  {"random-source", required_دلیل, عدم, RANDOM_سرچینه_OPTION},
  {"sort", required_دلیل, عدم, ترتیبول_OPTION},
  {"output", required_دلیل, عدم, 'o'},
  {"reverse", no_دلیل, عدم, 'r'},
  {"stable", no_دلیل, عدم, 's'},
  {"batch-size", required_دلیل, عدم, NMERGE_OPTION},
  {"buffer-size", required_دلیل, عدم, 'S'},
  {"field-separator", required_دلیل, عدم, 't'},
  {"temporary-directory", required_دلیل, عدم, 'T'},
  {"unique", no_دلیل, عدم, 'u'},
  {"zero-terminated", no_دلیل, عدم, 'z'},
  {"parallel", required_دلیل, عدم, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

#تعریف_کړه CHECK_جدول \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ساكن حرف ثابت *ثابت check_دلیلونه[] =
{
#تعریف_کړه _ct_(_s, _c) _s,
  CHECK_جدول عدم
#تعریف_لغوه  _ct_
};
ساكن حرف ثابت check_types[] =
{
#تعریف_کړه _ct_(_s, _c) _c,
  CHECK_جدول
#تعریف_لغوه  _ct_
};

#تعریف_کړه ترتیبول_جدول \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ساكن حرف ثابت *ثابت ترتیبول_دلیلونه[] =
{
#تعریف_کړه _st_(_s, _c) _s,
  ترتیبول_جدول عدم
#تعریف_لغوه  _st_
};
ساكن حرف ثابت ترتیبول_types[] =
{
#تعریف_کړه _st_(_s, _c) _c,
  ترتیبول_جدول
#تعریف_لغوه  _st_
};

/* The set of signals that are caught.  */
ساكن sigset_t caught_سیګنالونه;

/* Critical section status.  */
بنية cs_حالت
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ساكن فراغ
cs_enter (بنية cs_حالت *حالت)
{
  صحيح ret = pthread_sigmask (SIG_BLOCK, &caught_سیګنالونه, &حالت->sigs);
  حالت->valid = ret == 0;
}

/* Leave a critical section.  */
ساكن فراغ
cs_leave (بنية cs_حالت ثابت *حالت)
{
  إذا (حالت->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &حالت->sigs, عدم);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
تعداد { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
بنية sortfile
{
  /* The file's name.  */
  حرف ثابت *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  بنية tempnode *لنډمهاله;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ساكن Hash_جدول *proctab;

تعداد { INIT_PROCTAB_اندازه = 47 };

ساكن size_t
proctab_hasher (فراغ ثابت *ثبت, size_t tabsize)
{
  بنية tempnode ثابت *غوټه = ثبت;
  أرجع غوټه->pid % tabsize;
}

ساكن bool
proctab_comparator (فراغ ثابت *e1, فراغ ثابت *e2)
{
  بنية tempnode ثابت *n1 = e1;
  بنية tempnode ثابت *n2 = e2;
  أرجع n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ساكن pid_t nprocs;

ساكن bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ساكن pid_t
reap (pid_t pid)
{
  صحيح حالت;
  pid_t cpid = waitpid ((pid ? pid : -1), &حالت, (pid ? 0 : WNOHANG));

  إذا (cpid < 0)
    error (ترتیبول_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_پروګرام));
  وإلا إذا (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      إذا (! WIFEXITED (حالت) || WEXITSTATUS (حالت))
        error (ترتیبول_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_پروګرام));
      --nprocs;
    }

  أرجع cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساكن فراغ
ثبتوونکی_proc (بنية tempnode *لنډمهاله)
{
  إذا (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_اندازه, عدم,
                                 proctab_hasher,
                                 proctab_comparator,
                                 عدم);
      إذا (! proctab)
        xalloc_die ();
    }

  لنډمهاله->state = UNREAPED;

  إذا (! hash_ورننباسه (proctab, لنډمهاله))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساكن bool
delete_proc (pid_t pid)
{
  بنية tempnode test;

  test.pid = pid;
  بنية tempnode *غوټه = hash_remove (proctab, &test);
  إذا (! غوټه)
    أرجع false;
  غوټه->state = REAPED;
  أرجع true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ساكن فراغ
wait_proc (pid_t pid)
{
  إذا (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ساكن فراغ
reap_exited (فراغ)
{
  طالما (0 < nprocs && reap (0))
    استمر;
}

/* Reap at least one exited child, waiting if necessary.  */

ساكن فراغ
reap_some (فراغ)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ساكن فراغ
reap_all (فراغ)
{
  طالما (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ساكن فراغ
exit_cleanup (فراغ)
{
  إذا (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      بنية cs_حالت cs;
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

ساكن بنية tempnode *
create_لنډمهاله_file (صحيح *pfd, bool survive_fd_exhaustion)
{
  ساكن حرف ثابت slashbase[] = "/sortXXXXXX";
  ساكن idx_t لنډمهاله_dir_شاخص;
  صحيح fd;
  صحيح saved_errno;
  حرف ثابت *لنډمهاله_dir = لنډمهاله_dirs.dirs[لنډمهاله_dir_شاخص];
  size_t len = مزي_اوږدوالی (لنډمهاله_dir);
  بنية tempnode *غوټه =
    xmalloc (FLEXSIZEOF (بنية tempnode, name, len + حجم slashbase));
  حرف *file = غوټه->name;
  بنية cs_حالت cs;

  حافظه_نقل (file, لنډمهاله_dir, len);
  حافظه_نقل (file + len, slashbase, حجم slashbase);
  غوټه->بل = عدم;
  إذا (++لنډمهاله_dir_شاخص == لنډمهاله_dirs.n_dirs)
    لنډمهاله_dir_شاخص = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  إذا (0 <= fd)
    {
      *temptail = غوټه;
      temptail = &غوټه->بل;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  إذا (fd < 0)
    {
      إذا (! (survive_fd_exhaustion && errno == EMFILE))
        error (ترتیبول_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (لنډمهاله_dir));
      حرر (غوټه);
      غوټه = عدم;
    }

  *pfd = fd;
  أرجع غوټه;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ساكن بنية stat *
get_outstatus (فراغ)
{
  ساكن صحيح outstat_errno;
  ساكن بنية stat outstat;
  إذا (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  أرجع outstat_errno < 0 ? &outstat : عدم;
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

ساكن FILE *
بهیر_open (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp;

  إذا (*how == 'r')
    {
      إذا (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      وإلا
        {
          صحيح fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? عدم : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  وإلا إذا (*how == 'w')
    {
      إذا (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          صحيح ftruncate_errno = errno;
          بنية stat *outst = get_outstatus ();
          إذا (!outst || S_ISREG (outst->st_ډېرپېښ) || S_TYPEISSHM (outst))
            error (ترتیبول_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  وإلا
    affirm (!"unexpected mode passed to stream_open");

  أرجع fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ساكن FILE *
xfopen (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp = بهیر_open (file, how);
  إذا (!fp)
    ترتیبول_die (_("open failed"), file);
  أرجع fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ساكن فراغ
xfclose (FILE *fp, حرف ثابت *file)
{
  بدل (fileno (fp))
    {
    حالة STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      اكسر;

    حالة STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      إذا (fflush (fp) != 0)
        ترتیبول_die (_("fflush failed"), file);
      اكسر;

    افتراضي:
      إذا (fclose (fp) != 0)
        ترتیبول_die (_("close failed"), file);
      اكسر;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ساكن فراغ
move_fd (صحيح oldfd, صحيح newfd)
{
  إذا (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_ارزښت (dup2 (oldfd, newfd));
      ignore_ارزښت (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساكن صحيح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحيح oldfd, صحيح newfd)
{
  صحيح پایله = 0;
  إذا (oldfd != newfd)
    {
      پایله = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      إذا (پایله == 0)
        پایله = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  أرجع پایله;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساكن حرف ثابت *
get_resolved_compress_پروګرام (فراغ)
{
  /* Use a cache, to perform the search only once.  */
  ساكن حرف ثابت *resolved_compress_پروګرام_cache /* = NULL */;

  إذا (resolved_compress_پروګرام_cache == عدم)
    {
      resolved_compress_پروګرام_cache =
        پیدا_کړه_دننه_given_لار (compress_پروګرام, getenv ("PATH"), عدم, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  أرجع resolved_compress_پروګرام_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ساكن صحيح
pipe_ماشوم (pid_t *pid, صحيح pipefds[2], صحيح tempfd, bool decompress,
            size_t tries)
{
  حرف ثابت *resolved_compress_پروګرام;
  بنية tempnode *saved_temphead;
  مزدوج wait_retry = 0.25;
  بنية cs_حالت cs;
  صحيح پایله;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_پروګرام = get_resolved_compress_پروګرام ();
  إذا (resolved_compress_پروګرام == عدم)
    أرجع errno;

  إذا ((پایله = posix_spawnattr_init (&attr)))
    أرجع پایله;
  إذا ((پایله = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (پایله = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      أرجع پایله;
    }

  إذا (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحيح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع saved_errno;
    }

  إذا ((پایله = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (پایله = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((پایله = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (پایله = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (پایله = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((پایله = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (پایله = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (پایله = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع پایله;
    }

  حرف ثابت *ثابت argv[] =
    {
      resolved_compress_پروګرام,
      decompress ? "-d" : عدم,
      عدم
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  إذا (nmerge + 1 < nprocs)
    reap_some ();

  طالما (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = عدم;

      پایله = posix_spawnp (pid, resolved_compress_پروګرام, &actions, &attr,
                             (حرف * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      إذا (پایله != EAGAIN)
        اكسر;
      وإلا
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

  إذا (پایله)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  وإلا
    ++nprocs;

  أرجع پایله;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ساكن بنية tempnode *
maybe_create_لنډمهاله (FILE **pfp, bool survive_fd_exhaustion)
{
  صحيح tempfd;
  بنية tempnode *غوټه = create_لنډمهاله_file (&tempfd, survive_fd_exhaustion);
  إذا (! غوټه)
    أرجع عدم;

  غوټه->state = UNCOMPRESSED;

  إذا (compress_پروګرام)
    {
      صحيح pipefds[2];
      ساكن صحيح وروستی_پایله = 0;

      صحيح پایله = pipe_ماشوم (&غوټه->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      إذا (پایله)
        {
          إذا (پایله != وروستی_پایله)
            error (0, پایله, _("could not run compress program %s"),
                   quoteaf (compress_پروګرام));
        }
      وإلا
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          ثبتوونکی_proc (غوټه);
        }

      وروستی_پایله = پایله;
    }

  *pfp = fdopen (tempfd, "w");
  إذا (! *pfp)
    ترتیبول_die (_("couldn't create temporary file"), غوټه->name);

  أرجع غوټه;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساكن بنية tempnode *
create_لنډمهاله (FILE **pfp)
{
  أرجع maybe_create_لنډمهاله (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ساكن FILE *
open_لنډمهاله (بنية tempnode *لنډمهاله)
{
  صحيح tempfd, pipefds[2];
  FILE *fp = عدم;

  إذا (لنډمهاله->state == UNREAPED)
    wait_proc (لنډمهاله->pid);

  tempfd = open (لنډمهاله->name, O_RDONLY);
  إذا (tempfd < 0)
    أرجع عدم;

  pid_t ماشوم;
  صحيح پایله = pipe_ماشوم (&ماشوم, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  إذا (پایله)
    {
      إذا (پایله != EMFILE)
        error (ترتیبول_FAILURE, پایله, _("could not run compress program %s -d"),
               quoteaf (compress_پروګرام));
      close (tempfd);
      errno = EMFILE;
    }
  وإلا
    {
      لنډمهاله->pid = ماشوم;
      ثبتوونکی_proc (لنډمهاله);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      إذا (! fp)
        {
          صحيح saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  أرجع fp;
}

/* Append DIR to the array of temporary directory names.  */
ساكن فراغ
جمع_کړه_لنډمهاله_dir (حرف ثابت *dir)
{
  إذا (لنډمهاله_dirs.n_dirs == لنډمهاله_dirs.n_dirs_alloc)
    لنډمهاله_dirs.dirs = xpalloc (لنډمهاله_dirs.dirs, &لنډمهاله_dirs.n_dirs_alloc, 1, -1,
                              حجم *لنډمهاله_dirs.dirs);

  idx_t ثابت dir_idx = لنډمهاله_dirs.n_dirs++;
  لنډمهاله_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ساكن فراغ
zaptemp (حرف ثابت *name)
{
  بنية tempnode *متطاير *pnode;
  بنية tempnode *غوټه;
  بنية tempnode *بل;
  صحيح unlink_حالت;
  صحيح unlink_errno = 0;
  بنية cs_حالت cs;

  لكل (pnode = &temphead; (غوټه = *pnode)->name != name; pnode = &غوټه->بل)
    استمر;

  إذا (غوټه->state == UNREAPED)
    wait_proc (غوټه->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  بل = غوټه->بل;
  cs_enter (&cs);
  unlink_حالت = unlink (name);
  unlink_errno = errno;
  *pnode = بل;
  cs_leave (&cs);

  إذا (unlink_حالت != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  إذا (! بل)
    temptail = pnode;
  حرر (غوټه);
}

#إذا HAVE_NL_LANGINFO

ساكن صحيح
struct_month_cmp (فراغ ثابت *m1, فراغ ثابت *m2)
{
  بنية month ثابت *month1 = m1;
  بنية month ثابت *month2 = m2;
  أرجع strcmp (month1->name, month2->name);
}

#د_که_پای

/* Initialize the character class tables. */

ساكن فراغ
inittables (فراغ)
{
  لكل (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! که_توری_یا_عدد (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = لوی_توری (i);
    }

#إذا HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  إذا (hard_LC_وخت)
    {
      لكل (size_t i = 0; i < MONTHS_فی_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = مزي_اوږدوالی (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          لكل (j = k = 0; j < s_len; j++)
            إذا (! isblank (منزل_uchar (s[j])))
              name[k++] = fold_toupper[منزل_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_فی_YEAR, حجم *monthtab, struct_month_cmp);
    }
#د_که_پای
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ساكن فراغ
specify_nmerge (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  بنية rlimit rlimit;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  بینښه صحيح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  إذا (e == LONGINT_OK)
    {
      nmerge = n;
      إذا (nmerge != n)
        e = LONGINT_ډکېدنه;
      وإلا
        {
          إذا (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (ترتیبول_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          وإلا إذا (max_nmerge < nmerge)
            {
              e = LONGINT_ډکېدنه;
            }
          وإلا
            أرجع;
        }
    }

  إذا (e == LONGINT_ډکېدنه)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (ترتیبول_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  وإلا
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ساكن فراغ
specify_ترتیبول_اندازه (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  حرف *suffix;
  تعداد strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  إذا (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_ډکېدنه;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  إذا (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    بدل (suffix[0])
      {
      حالة 'b':
        e = LONGINT_OK;
        اكسر;

      حالة '%':
        {
          مزدوج mem = physmem_ټول () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          إذا (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          وإلا
            e = LONGINT_ډکېدنه;
        }
        اكسر;
      }

  إذا (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      إذا (n < ترتیبول_اندازه)
        أرجع;

      ترتیبول_اندازه = n;
      إذا (ترتیبول_اندازه == n)
        {
          ترتیبول_اندازه = MAX (ترتیبول_اندازه, MIN_ترتیبول_اندازه);
          أرجع;
        }

      e = LONGINT_ډکېدنه;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ساكن size_t
specify_nthreads (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t nthreads;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &nthreads, "");
  إذا (e == LONGINT_ډکېدنه)
    أرجع SIZE_MAX;
  إذا (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  إذا (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  إذا (nthreads == 0)
    error (ترتیبول_FAILURE, 0, _("number in parallel must be nonzero"));
  أرجع nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ساكن size_t
default_ترتیبول_اندازه (فراغ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t اندازه = SIZE_MAX;
  بنية rlimit rlimit;
  إذا (getrlimit (RLIMIT_معلومات, &rlimit) == 0 && rlimit.rlim_cur < اندازه)
    اندازه = rlimit.rlim_cur;
#که_تعریف_شوی RLIMIT_AS
  إذا (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < اندازه)
    اندازه = rlimit.rlim_cur;
#د_که_پای

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  اندازه /= 2;

#که_تعریف_شوی RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  إذا (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < اندازه)
    اندازه = rlimit.rlim_cur / 16 * 15;
#د_که_پای

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  مزدوج avail = physmem_شته ();
  مزدوج ټول = physmem_ټول ();
  مزدوج mem = MAX (avail, ټول / 8);

  /* Leave a 1/4 margin for physical memory.  */
  إذا (ټول * 0.75 < اندازه)
    اندازه = ټول * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  إذا (mem < اندازه)
    اندازه = mem;
  أرجع MAX (اندازه, MIN_ترتیبول_اندازه);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساكن bool
کرښه_aligned_اندازه (size_t *alloc)
{
  size_t اندازه = *alloc;
  size_t alignment = حجم (بنية کرښه);
  size_t padding = alignment - اندازه % alignment;
  size_t aligned;

  إذا (ckd_جمع_کړه (&aligned, اندازه, padding))
    أرجع false;

  *alloc = aligned;
  أرجع true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساكن size_t
default_initial_ترتیبول_اندازه (size_t کرښه_بایټونه)
{
  size_t اندازه = کرښه_بایټونه + 2;
  size_t ننوت_اندازه;

  إذا (ckd_mul (&ننوت_اندازه, ننوت_FILE_اندازه_GUESS, کرښه_بایټونه + 1)
      || ckd_جمع_کړه (&اندازه, اندازه, ننوت_اندازه))
    أرجع SIZE_MAX;

  أرجع MAX (اندازه, MIN_ترتیبول_اندازه);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساكن bool
ننوت_اندازه_بفر_بایټونه (uintmax_t file_اندازه, size_t کرښه_بایټونه,
                         size_t *worst_case)
{
  size_t worst_case_فی_ننوت_byte = کرښه_بایټونه + 1;

  إذا (SIZE_MAX / worst_case_فی_ننوت_byte < file_اندازه)
    أرجع false;

  size_t اندازه = file_اندازه * worst_case_فی_ننوت_byte;
  إذا (SIZE_MAX - اندازه < 1)
    أرجع false;

  *worst_case = اندازه + 1;
  أرجع true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ساكن فراغ
ترتیبول_بفر_policy (FILE *ثابت *fps, size_t nfps,
                    حرف *ثابت *files, size_t nfiles,
                    size_t کرښه_بایټونه,
                    بنية ترتیبول_بفر_policy *policy)
{
  size_t اندازه_bound = ترتیبول_اندازه ? ترتیبول_اندازه : default_ترتیبول_اندازه ();
  size_t initial_bound =
    ترتیبول_اندازه ? اندازه_bound : default_initial_ترتیبول_اندازه (کرښه_بایټونه);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_فی_ننوت_byte = کرښه_بایټونه + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t اندازه = worst_case_فی_ننوت_byte + 1;

  initial_bound = MIN (initial_bound, اندازه_bound);
  policy->حد = اندازه_bound;
  policy->growth_failed = false;

  لكل (size_t i = 0; i < nfiles; i++)
    {
      بنية stat st;
      off_t file_اندازه;
      size_t worst_case;
      bool معلوم_اندازه;

      إذا (اندازه_bound <= اندازه)
        {
          policy->initial = اندازه_bound;
          أرجع;
        }

      إذا ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        ترتیبول_die (_("stat failed"), files[i]);

      معلوم_اندازه = usable_st_اندازه (&st) && 0 < st.st_اندازه;
      إذا (معلوم_اندازه)
        file_اندازه = st.st_اندازه;
      وإلا
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          إذا (ترتیبول_اندازه)
            {
              policy->initial = اندازه_bound;
              أرجع;
            }
          file_اندازه = ننوت_FILE_اندازه_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      إذا (! ننوت_اندازه_بفر_بایټونه (file_اندازه, کرښه_بایټونه, &worst_case)
          || اندازه_bound - اندازه <= worst_case)
        {
          policy->initial = اندازه_bound;
          أرجع;
        }

      إذا (! معلوم_اندازه && ! ترتیبول_اندازه
          && (initial_bound <= اندازه || initial_bound - اندازه <= worst_case))
        {
          policy->initial = MAX (اندازه, initial_bound);
          أرجع;
        }

      اندازه += worst_case;
    }

  policy->initial = MAX (اندازه, MIN_ترتیبول_اندازه);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساكن فراغ
initbuf (بنية بفر *buf, size_t کرښه_بایټونه, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  طالما (true)
    {
      إذا (! کرښه_aligned_اندازه (&alloc))
        buf->buf = عدم;
      وإلا
        buf->buf = حافظه_ورکړه (alloc);
      إذا (buf->buf)
        اكسر;
      alloc /= 2;
      إذا (alloc <= کرښه_بایټونه + 1)
        xalloc_die ();
    }

  buf->کرښه_بایټونه = کرښه_بایټونه;
  buf->alloc = alloc;
  buf->کارول_شوی = buf->کیڼ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساكن مضمن بنية کرښه *
بفر_linelim (بنية بفر ثابت *buf)
{
  فراغ *linelim = buf->buf + buf->alloc;
  أرجع linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ساكن bool
try_growbuf (بنية بفر *buf, size_t alloc)
{
  إذا (! کرښه_aligned_اندازه (&alloc) || alloc <= buf->alloc)
    أرجع false;

  حرف *newbuf = حافظه_ورکړه (alloc);
  إذا (! newbuf)
    أرجع false;

  حرف *oldbuf = buf->buf;
  بنية کرښه *old_linelim = بفر_linelim (buf);
  بنية کرښه *old_کرښه = old_linelim - buf->nlines;

  حافظه_نقل (newbuf, oldbuf, buf->کارول_شوی);

  بنية کرښه *new_linelim = (فراغ *) (newbuf + alloc);
  بنية کرښه *new_کرښه = new_linelim - buf->nlines;
  حافظه_نقل (new_کرښه, old_کرښه, buf->nlines * حجم *new_کرښه);

  لكل (بنية کرښه *کرښه = new_کرښه; کرښه < new_linelim; کرښه++)
    {
      کرښه->متن = newbuf + (کرښه->متن - oldbuf);
      إذا (keylist)
        {
          کرښه->keybeg = newbuf + (کرښه->keybeg - oldbuf);
          کرښه->keylim = newbuf + (کرښه->keylim - oldbuf);
        }
    }

  حرر (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  أرجع true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساكن bool
maybe_growbuf (بنية بفر *buf, بنية ترتیبول_بفر_policy *policy)
{
  إذا (! policy || policy->growth_failed || policy->حد <= buf->alloc)
    أرجع false;

  size_t alloc;
  إذا (buf->alloc <= policy->حد / 3)
    alloc = buf->alloc * 3;
  وإلا
    alloc = policy->حد;

  إذا (try_growbuf (buf, alloc))
    أرجع true;

  policy->growth_failed = true;
  أرجع false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ساكن حرف *
begfield (بنية کرښه ثابت *کرښه, بنية keyfield ثابت *کلی)
{
  حرف *ptr = کرښه->متن, *lim = ptr + کرښه->اوږدتیا - 1;
  size_t sword = کلی->sword;
  size_t schar = کلی->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  إذا (tab != TAB_DEFAULT)
    طالما (ptr < lim && sword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        إذا (ptr < lim)
          ++ptr;
      }
  وإلا
    طالما (ptr < lim && sword--)
      {
        طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[منزل_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  إذا (کلی->skipsblanks)
    طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t پاتې_شوی_بایټونه = lim - ptr;
  إذا (schar < پاتې_شوی_بایټونه)
    ptr += schar;
  وإلا
    ptr = lim;

  أرجع ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_خالص
ساكن حرف *
limfield (بنية کرښه ثابت *کرښه, بنية keyfield ثابت *کلی)
{
  حرف *ptr = کرښه->متن, *lim = ptr + کرښه->اوږدتیا - 1;
  size_t eword = کلی->eword, echar = کلی->echar;

  إذا (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  إذا (tab != TAB_DEFAULT)
    طالما (ptr < lim && eword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        إذا (ptr < lim && (eword || echar))
          ++ptr;
      }
  وإلا
    طالما (ptr < lim && eword--)
      {
        طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[منزل_uchar (*ptr)])
          ++ptr;
      }

#که_تعریف_شوی POSIX_UNSPECIFIED
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
  إذا (tab != TAB_DEFAULT)
    {
      حرف *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      إذا (newlim)
        lim = newlim;
    }
  وإلا
    {
      حرف *newlim;
      newlim = ptr;
      طالما (newlim < lim && blanks[منزل_uchar (*newlim)])
        ++newlim;
      طالما (newlim < lim && !blanks[منزل_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#د_که_پای

  إذا (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      إذا (کلی->skipeblanks)
        طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t پاتې_شوی_بایټونه = lim - ptr;
      إذا (echar < پاتې_شوی_بایټونه)
        ptr += echar;
      وإلا
        ptr = lim;
    }

  أرجع ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ساكن bool
fillbuf (بنية بفر *buf, FILE *fp, حرف ثابت *file,
         بنية ترتیبول_بفر_policy *policy)
{
  بنية keyfield ثابت *کلی = keylist;
  حرف eol = eolchar;
  size_t کرښه_بایټونه = buf->کرښه_بایټونه;
  size_t mergesize = یوځای_بفر_اندازه - MIN_یوځای_بفر_اندازه;

  إذا (buf->eof)
    أرجع false;

  إذا (buf->کارول_شوی != buf->کیڼ)
    {
      memmove (buf->buf, buf->buf + buf->کارول_شوی - buf->کیڼ, buf->کیڼ);
      buf->کارول_شوی = buf->کیڼ;
      buf->nlines = 0;
    }

  طالما (true)
    {
      حرف *ptr = buf->buf + buf->کارول_شوی;
      بنية کرښه *linelim = بفر_linelim (buf);
      بنية کرښه *کرښه = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * کرښه_بایټونه - ptr;
      حرف *کرښه_پیل = buf->nlines ? کرښه->متن + کرښه->اوږدتیا : buf->buf;

      طالما (کرښه_بایټونه + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (کرښه_بایټونه + 1);
          size_t بایټونه_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + بایټونه_read;
          حرف *p;
          avail -= بایټونه_read;

          إذا (بایټونه_read != readsize)
            {
              إذا (ferror (fp))
                ترتیبول_die (_("read failed"), file);
              إذا (feof (fp))
                {
                  buf->eof = true;
                  إذا (buf->buf == ptrlim)
                    أرجع false;
                  إذا (کرښه_پیل != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          طالما ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              کرښه--;
              کرښه->متن = کرښه_پیل;
              کرښه->اوږدتیا = ptr - کرښه_پیل;
              mergesize = MAX (mergesize, کرښه->اوږدتیا);
              avail -= کرښه_بایټونه;

              إذا (کلی)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  کرښه->keylim = (کلی->eword == SIZE_MAX
                                  ? p
                                  : limfield (کرښه, کلی));

                  إذا (کلی->sword != SIZE_MAX)
                    کرښه->keybeg = begfield (کرښه, کلی);
                  وإلا
                    {
                      إذا (کلی->skipsblanks)
                        طالما (blanks[منزل_uchar (*کرښه_پیل)])
                          کرښه_پیل++;
                      کرښه->keybeg = کرښه_پیل;
                    }
                }

              کرښه_پیل = ptr;
            }

          ptr = ptrlim;
          إذا (buf->eof)
            اكسر;
        }

      buf->کارول_شوی = ptr - buf->buf;
      buf->nlines = بفر_linelim (buf) - کرښه;
      إذا (buf->nlines != 0)
        {
          إذا (! buf->eof && maybe_growbuf (buf, policy))
            استمر;
          buf->کیڼ = ptr - کرښه_پیل;
          یوځای_بفر_اندازه = mergesize + MIN_یوځای_بفر_اندازه;
          أرجع true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t کرښه_alloc = buf->alloc / حجم (بنية کرښه);
        buf->buf = xpalloc (buf->buf, &کرښه_alloc, 1, -1, حجم (بنية کرښه));
        buf->alloc = کرښه_alloc * حجم (بنية کرښه);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ساكن حرف ثابت unit_ترتیب[UCHAR_LIM] =
  {
#إذا ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#وإلا
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
#د_که_پای
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ساكن حرف
traverse_raw_شمېره (حرف ثابت **شمېره)
{
  حرف ثابت *p = *شمېره;
  حرف ch;
  حرف max_digit = '\0';
  bool پایونه_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  طالما (c_isdigit (ch = *p++))
    {
      إذا (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      پایونه_with_thousands_sep = (*p == thousands_sep);
      إذا (پایونه_with_thousands_sep)
        ++p;
    }

  إذا (پایونه_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *شمېره = p - 2;
      أرجع max_digit;
    }

  إذا (ch == decimal_ټکی)
    طالما (c_isdigit (ch = *p++))
      إذا (max_digit < ch)
        max_digit = ch;

  *شمېره = p - 1;
  أرجع max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_خالص
ساكن صحيح
پیدا_کړه_unit_ترتیب (حرف ثابت *شمېره)
{
  bool minus_sign = (*شمېره == '-');
  حرف ثابت *p = شمېره + minus_sign;
  حرف max_digit = traverse_raw_شمېره (&p);
  إذا ('0' < max_digit)
    {
      بینښه حرف ch = *p;
      صحيح ترتیب = unit_ترتیب[ch];
      أرجع (minus_sign ? -ترتیب : ترتیب);
    }
  وإلا
    أرجع 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_خالص
ساكن صحيح
human_numcompare (حرف ثابت *a, حرف ثابت *b)
{
  طالما (blanks[منزل_uchar (*a)])
    a++;
  طالما (blanks[منزل_uchar (*b)])
    b++;

  صحيح diff = پیدا_کړه_unit_ترتیب (a) - پیدا_کړه_unit_ترتیب (b);
  أرجع (diff ? diff : strnumcmp (a, b, decimal_ټکی, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_خالص
ساكن صحيح
numcompare (حرف ثابت *a, حرف ثابت *b)
{
  طالما (blanks[منزل_uchar (*a)])
    a++;
  طالما (blanks[منزل_uchar (*b)])
    b++;

  أرجع strnumcmp (a, b, decimal_ټکی, thousands_sep);
}

ساكن صحيح
nan_compare (اوږد مزدوج a, اوږد مزدوج b)
{
  حرف buf[2][حجم "-nan""()" + CHAR_BIT * حجم a];
  snprintf (buf[0], حجم buf[0], "%Lf", a);
  snprintf (buf[1], حجم buf[1], "%Lf", b);
  أرجع strcmp (buf[0], buf[1]);
}

ساكن صحيح
general_numcompare (حرف ثابت *sa, حرف ثابت *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  حرف *ea;
  حرف *eb;
  اوږد مزدوج a = strtold (sa, &ea);
  اوږد مزدوج b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  إذا (sa == ea)
    أرجع sb == eb ? 0 : -1;
  إذا (sb == eb)
    أرجع 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  أرجع (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ساكن صحيح
getmonth (حرف ثابت *month, حرف **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_فی_YEAR;

  طالما (blanks[منزل_uchar (*month)])
    month++;

  افعل
    {
      size_t ix = (lo + hi) / 2;
      حرف ثابت *m = month;
      حرف ثابت *n = monthtab[ix].name;

      لكل (;; m++, n++)
        {
          إذا (!*n)
            {
              إذا (ea)
                *ea = (حرف *) m;
              أرجع monthtab[ix].val;
            }
          إذا (منزل_uchar (fold_toupper[منزل_uchar (*m)]) < منزل_uchar (*n))
            {
              hi = ix;
              اكسر;
            }
          وإلا إذا (منزل_uchar (fold_toupper[منزل_uchar (*m)]) > منزل_uchar (*n))
            {
              lo = ix + 1;
              اكسر;
            }
        }
    }
  طالما (lo < hi);

  أرجع 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# إذا 14 <= __GNUC__
#  pragma GCC diagnostic ټېل
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#د_که_پای

#ضمن "md5.h"

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# إذا 14 <= __GNUC__
#  pragma GCC diagnostic راوباسه
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_لومړیتوب_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ساكن فراغ
link_failure (فراغ)
{
  error (ترتیبول_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ساكن فراغ *
نښه_پته (فراغ *handle, حرف ثابت *نښه)
{
  فراغ *پته = dlsym (handle, نښه);
  إذا (!پته)
    link_failure ();
  أرجع پته;
}
#د_که_پای

/* Dynamically link the crypto library, if it needs linking.  */
ساكن فراغ
link_libcrypto (فراغ)
{
#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  فراغ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  إذا (!handle)
    link_failure ();
  ptr_MD5_Init = نښه_پته (handle, "MD5_Init");
  ptr_MD5_Update = نښه_پته (handle, "MD5_Update");
  ptr_MD5_Final = نښه_پته (handle, "MD5_Final");
#د_که_پای
}

/* A randomly chosen MD5 state, used for random comparison.  */
ساكن بنية md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ساكن فراغ
random_md5_state_init (حرف ثابت *random_سرچینه)
{
  بینښه حرف buf[MD5_DIGEST_اندازه];
  بنية randread_سرچینه *r = randread_new (random_سرچینه, حجم buf);
  إذا (! r)
    ترتیبول_die (_("open failed"), random_سرچینه ? random_سرچینه : "getrandom");
  randread (r, buf, حجم buf);
  إذا (randread_free (r) != 0)
    ترتیبول_die (_("close failed"), random_سرچینه);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_بایټونه (buf, حجم buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ساكن size_t
xstrxfrm (حرف *محدود dest, حرف ثابت *محدود src, size_t destsize)
{
  errno = 0;
  size_t translated_اندازه = strxfrm (dest, src, destsize);

  إذا (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (ترتیبول_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  أرجع translated_اندازه;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ساكن صحيح
compare_random (حرف *محدود texta, size_t lena,
                حرف *محدود textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  صحيح xfrm_diff = 0;

  حرف stackbuf[4000];
  حرف *buf = stackbuf;
  size_t bufsize = حجم stackbuf;
  فراغ *allocated = عدم;
  uint32_t dig[2][MD5_DIGEST_اندازه / حجم (uint32_t)];
  بنية md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  إذا (hard_LC_COLLATE)
    {
      حرف ثابت *lima = texta + lena;
      حرف ثابت *limb = textb + lenb;

      طالما (true)
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
          إذا (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              حرر (allocated);
              buf = allocated = حافظه_ورکړه (bufsize);
              إذا (! buf)
                {
                  buf = stackbuf;
                  bufsize = حجم stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_ځاییږي = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_ځاییږي ? buf + sizea : عدم), textb,
                          (a_ځاییږي ? bufsize - sizea : 0))
                + 1)
             : 0);

          إذا (! (a_ځاییږي && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              إذا (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              حرر (allocated);
              buf = allocated = xmalloc (bufsize);
              إذا (texta < lima)
                strxfrm (buf, texta, sizea);
              إذا (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          إذا (texta < lima)
            texta += مزي_اوږدوالی (texta) + 1;
          إذا (textb < limb)
            textb += مزي_اوږدوالی (textb) + 1;
          إذا (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              اكسر;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_بایټونه (buf, sizea, &s[0]);
          md5_process_بایټونه (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          إذا (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              إذا (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_بایټونه (texta, lena, &s[0]); md5_پای_ctx (&s[0], dig[0]);
  md5_process_بایټونه (textb, lenb, &s[1]); md5_پای_ctx (&s[1], dig[1]);
  صحيح diff = memcmp (dig[0], dig[1], حجم dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  إذا (! diff)
    {
      إذا (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          إذا (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  حرر (allocated);

  أرجع diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ساكن size_t
debug_پلنوالی (حرف ثابت *متن, حرف ثابت *lim)
{
  size_t پلنوالی = mbsnwidth (متن, lim - متن, 0);
  طالما (متن < lim)
    پلنوالی += (*متن++ == '\t');
  أرجع پلنوالی;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساكن فراغ
mark_کلی (size_t دننه_واټن, size_t پلنوالی)
{
  طالما (دننه_واټن--)
    توری_چاپ_کړه (' ');

  إذا (!پلنوالی)
    چاپ_کړه (_("^ no match for key\n"));
  وإلا
    {
      افعل
        توری_چاپ_کړه ('_');
      طالما (--پلنوالی);

      توری_چاپ_کړه ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساكن مضمن bool
کلی_numeric (بنية keyfield ثابت *کلی)
{
  أرجع کلی->numeric || کلی->general_numeric || کلی->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساكن فراغ
debug_کلی (بنية کرښه ثابت *کرښه, بنية keyfield ثابت *کلی)
{
  حرف *متن = کرښه->متن;
  حرف *beg = متن;
  حرف *lim = متن + کرښه->اوږدتیا - 1;

  إذا (کلی)
    {
      إذا (کلی->sword != SIZE_MAX)
        beg = begfield (کرښه, کلی);
      إذا (کلی->eword != SIZE_MAX)
        {
          lim = limfield (کرښه, کلی);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      إذا ((کلی->skipsblanks && کلی->sword == SIZE_MAX)
          || کلی->month || کلی_numeric (کلی))
        {
          حرف saved = *lim;
          *lim = '\0';

          طالما (blanks[منزل_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          إذا (lim < beg)
            tighter_lim = lim;
          وإلا إذا (کلی->month)
            getmonth (beg, &tighter_lim);
          وإلا إذا (کلی->general_numeric)
            ignore_ارزښت (strtold (beg, &tighter_lim));
          وإلا إذا (کلی->numeric || کلی->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_شمېره (&p);
              إذا ('0' <= max_digit)
                {
                  بینښه حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (کلی->human_numeric && unit_ترتیب[ch]);
                }
            }
          وإلا
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t دننه_واټن = debug_پلنوالی (متن, beg);
  size_t پلنوالی = debug_پلنوالی (beg, lim);
  mark_کلی (دننه_واټن, پلنوالی);
}

/* Debug LINE by underlining its keys.  */

ساكن فراغ
debug_کرښه (بنية کرښه ثابت *کرښه)
{
  بنية keyfield ثابت *کلی = keylist;

  افعل
    debug_کلی (کرښه, کلی);
  طالما (کلی && ((کلی = کلی->بل) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساكن bool
default_کلی_compare (بنية keyfield ثابت *کلی)
{
  أرجع ! (کلی->ignore
            || کلی->translate
            || کلی->skipsblanks
            || کلی->skipeblanks
            || کلی_numeric (کلی)
            || کلی->month
            || کلی->نسخه_3
            || کلی->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساكن فراغ
کلی_منزل_opts (بنية keyfield ثابت *کلی, حرف *opts)
{
  إذا (کلی->skipsblanks || کلی->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  إذا (کلی->ignore == nondictionary)
    *opts++ = 'd';
  إذا (کلی->translate)
    *opts++ = 'f';
  إذا (کلی->general_numeric)
    *opts++ = 'g';
  إذا (کلی->human_numeric)
    *opts++ = 'h';
  إذا (کلی->ignore == nonprinting)
    *opts++ = 'i';
  إذا (کلی->month)
    *opts++ = 'M';
  إذا (کلی->numeric)
    *opts++ = 'n';
  إذا (کلی->random)
    *opts++ = 'R';
  إذا (کلی->سرچپه)
    *opts++ = 'r';
  إذا (کلی->نسخه_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساكن فراغ
کلی_warnings (بنية keyfield ثابت *gkey, bool gkey_only)
{
  بنية keyfield ugkey = *gkey;
  بینښه اوږد keynum = 1;
  bool basic_numeric_ډګر = false;
  bool general_numeric_ډګر = false;
  bool basic_numeric_ډګر_پراختیا = false;
  bool general_numeric_ډګر_پراختیا = false;

  لكل (بنية keyfield *کلی = keylist; کلی; کلی = کلی->بل, keynum++)
    {
      إذا (کلی_numeric (کلی))
        {
          إذا (کلی->general_numeric)
            general_numeric_ډګر = true;
          وإلا
            basic_numeric_ډګر = true;
        }

      إذا (کلی->traditional_کارول_شوی)
        {
          size_t sword = کلی->sword;
          size_t eword = کلی->eword;
          حرف tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          حرف obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          حرف nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          حرف *po = obuf;
          حرف *pn = nbuf;

          إذا (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          إذا (کلی->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (کلی->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_پلنوالی = کلی->sword != SIZE_MAX && کلی->eword < کلی->sword;
      إذا (zero_پلنوالی)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = کلی_numeric (کلی) || کلی->month;
      bool کرښه_دننه_واټن = کلی->eword == 0 && کلی->echar != 0; /* -k1.x,1.y  */
      إذا (!zero_پلنوالی && !gkey_only && tab == TAB_DEFAULT && !کرښه_دننه_واټن
          && ((!کلی->skipsblanks && !implicit_skip)
              || (!کلی->skipsblanks && کلی->schar)
              || (!کلی->skipeblanks && کلی->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      إذا (!gkey_only && کلی_numeric (کلی))
        {
          size_t sword = کلی->sword + 1;
          size_t eword = کلی->eword + 1;
          إذا (!sword)
            sword++;
          إذا (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              إذا (کلی->general_numeric)
                general_numeric_ډګر_پراختیا = true;
              وإلا
                basic_numeric_ډګر_پراختیا = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      إذا (ugkey.ignore && (ugkey.ignore == کلی->ignore))
        ugkey.ignore = عدم;
      إذا (ugkey.translate && (ugkey.translate == کلی->translate))
        ugkey.translate = عدم;
      ugkey.skipsblanks &= !کلی->skipsblanks;
      ugkey.skipeblanks &= !کلی->skipeblanks;
      ugkey.month &= !کلی->month;
      ugkey.numeric &= !کلی->numeric;
      ugkey.general_numeric &= !کلی->general_numeric;
      ugkey.human_numeric &= !کلی->human_numeric;
      ugkey.random &= !کلی->random;
      ugkey.نسخه_3 &= !کلی->نسخه_3;
      ugkey.سرچپه &= !کلی->سرچپه;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool شمېره_locale_warned = false;
  إذا (basic_numeric_ډګر_پراختیا)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (منزل_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          شمېره_locale_warned = true;
        }
    }
  إذا (basic_numeric_ډګر_پراختیا || general_numeric_ډګر_پراختیا)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (منزل_uchar (decimal_ټکی)))
          : tab == decimal_ټکی)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_ټکی, 0})));
          شمېره_locale_warned = true;
        }
      وإلا إذا (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      وإلا إذا (general_numeric_ډګر_پراختیا && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  إذا ((basic_numeric_ډګر || general_numeric_ډګر) && ! شمېره_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_ټکی, 0})));

    }

  إذا (basic_numeric_ډګر && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  إذا (!default_کلی_compare (&ugkey)
      || (ugkey.سرچپه && (stable || unique) && keylist))
    {
      bool ugkey_سرچپه = ugkey.سرچپه;
      إذا (!(stable || unique))
        ugkey.سرچپه = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      کلی_منزل_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (مزي_اوږدوالی (opts))), opts);
      ugkey.سرچپه = ugkey_سرچپه;
    }
  إذا (ugkey.سرچپه && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ساكن صحيح
diff_reversed (صحيح diff, bool reversed)
{
  أرجع reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ساكن صحيح
keycompare (بنية کرښه ثابت *a, بنية کرښه ثابت *b)
{
  بنية keyfield *کلی = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحيح diff;

  طالما (true)
    {
      حرف ثابت *translate = کلی->translate;
      bool ثابت *ignore = کلی->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      إذا (hard_LC_COLLATE || کلی_numeric (کلی)
          || کلی->month || کلی->random || کلی->نسخه_3)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          حرف *ta = texta;
          حرف *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          حرف enda = ta[tlena];
          حرف endb = tb[tlenb];

          فراغ *allocated = عدم;
          حرف stackbuf[4000];

          إذا (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t اندازه = lena + 1 + lenb + 1;
              إذا (اندازه <= حجم stackbuf)
                ta = stackbuf;
              وإلا
                ta = allocated = xmalloc (اندازه);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              لكل (tlena = i = 0; i < lena; i++)
                إذا (! (ignore && ignore[منزل_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[منزل_uchar (texta[i])]
                                 : texta[i]);

              لكل (tlenb = i = 0; i < lenb; i++)
                إذا (! (ignore && ignore[منزل_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[منزل_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          إذا (کلی->numeric)
            diff = numcompare (ta, tb);
          وإلا إذا (کلی->general_numeric)
            diff = general_numcompare (ta, tb);
          وإلا إذا (کلی->human_numeric)
            diff = human_numcompare (ta, tb);
          وإلا إذا (کلی->month)
            diff = getmonth (ta, عدم) - getmonth (tb, عدم);
          وإلا إذا (کلی->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          وإلا إذا (کلی->نسخه_3)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          وإلا
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              إذا (tlena == 0)
                diff = - NONZERO (tlenb);
              وإلا إذا (tlenb == 0)
                diff = 1;
              وإلا
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          حرر (allocated);
        }
      وإلا إذا (ignore)
        {
#تعریف_کړه CMP_WITH_IGNORE(A, B)						\
  افعل									\
    {									\
          طالما (true)							\
            {								\
              طالما (texta < lima && ignore[منزل_uchar (*texta)])		\
                ++texta;						\
              طالما (textb < limb && ignore[منزل_uchar (*textb)])		\
                ++textb;						\
              إذا (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  اكسر;						\
                }							\
              diff = منزل_uchar (A) - منزل_uchar (B);			\
              إذا (diff)							\
                اكسر;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  طالما (0)

          إذا (translate)
            CMP_WITH_IGNORE (translate[منزل_uchar (*texta)],
                             translate[منزل_uchar (*textb)]);
          وإلا
            CMP_WITH_IGNORE (*texta, *textb);
        }
      وإلا
        {
          size_t lenmin = MIN (lena, lenb);
          إذا (lenmin == 0)
            diff = 0;
          وإلا إذا (translate)
            {
              size_t i = 0;
              افعل
                {
                  diff = (منزل_uchar (translate[منزل_uchar (texta[i])])
                          - منزل_uchar (translate[منزل_uchar (textb[i])]));
                  إذا (diff)
                    اكسر;
                  i++;
                }
              طالما (i < lenmin);
            }
          وإلا
            diff = memcmp (texta, textb, lenmin);

          إذا (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      إذا (diff)
        اكسر;

      کلی = کلی->بل;
      إذا (! کلی)
        أرجع 0;

      /* Find the beginning and limit of the next field.  */
      إذا (کلی->eword != SIZE_MAX)
        lima = limfield (a, کلی), limb = limfield (b, کلی);
      وإلا
        lima = a->متن + a->اوږدتیا - 1, limb = b->متن + b->اوږدتیا - 1;

      إذا (کلی->sword != SIZE_MAX)
        texta = begfield (a, کلی), textb = begfield (b, کلی);
      وإلا
        {
          texta = a->متن, textb = b->متن;
          إذا (کلی->skipsblanks)
            {
              طالما (texta < lima && blanks[منزل_uchar (*texta)])
                ++texta;
              طالما (textb < limb && blanks[منزل_uchar (*textb)])
                ++textb;
            }
        }
    }

  أرجع diff_reversed (diff, کلی->سرچپه);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساكن صحيح
compare (بنية کرښه ثابت *a, بنية کرښه ثابت *b)
{
  صحيح diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  إذا (keylist)
    {
      diff = keycompare (a, b);
      إذا (diff || unique || stable)
        أرجع diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->اوږدتیا - 1, blen = b->اوږدتیا - 1;

  إذا (alen == 0)
    diff = - NONZERO (blen);
  وإلا إذا (blen == 0)
    diff = 1;
  وإلا إذا (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->متن, alen + 1, b->متن, blen + 1);
    }
  وإلا
    {
      diff = memcmp (a->متن, b->متن, MIN (alen, blen));
      إذا (!diff)
        diff = _GL_CMP (alen, blen);
    }

  أرجع diff_reversed (diff, سرچپه);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساكن فراغ
write_کرښه (بنية کرښه ثابت *کرښه, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = کرښه->متن;
  size_t n_بایټونه = کرښه->اوږدتیا;
  حرف *ebuf = buf + n_بایټونه;

  إذا (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      حرف ثابت *c = buf;

      طالما (c < ebuf)
        {
          حرف wc = *c++;
          إذا (wc == '\t')
            wc = '>';
          وإلا إذا (c == ebuf)
            wc = '\n';
          إذا (fputc (wc, fp) == EOF)
            ترتیبول_die (_("write failed"), output_file);
        }

      debug_کرښه (کرښه);
    }
  وإلا
    {
      ebuf[-1] = eolchar;
      إذا (fwrite (buf, 1, n_بایټونه, fp) != n_بایټونه)
        ترتیبول_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ساكن bool
check (حرف ثابت *file_name, حرف checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  بنية بفر buf;		/* Input buffer. */
  بنية کرښه لنډمهاله;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t کرښه_شمېره = 0;
  بنية keyfield ثابت *کلی = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (بنية کرښه),
           MAX (یوځای_بفر_اندازه, ترتیبول_اندازه));
  لنډمهاله.متن = عدم;

  طالما (fillbuf (&buf, fp, file_name, عدم))
    {
      بنية کرښه ثابت *کرښه = بفر_linelim (&buf);
      بنية کرښه ثابت *linebase = کرښه - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      إذا (alloc && nonunique <= compare (&لنډمهاله, کرښه - 1))
        {
        وموندل_disorder:
          {
            إذا (checkonly == 'c')
              {
                بنية کرښه ثابت *disorder_کرښه = کرښه - 1;
                uintmax_t disorder_کرښه_شمېره =
                  بفر_linelim (&buf) - disorder_کرښه + کرښه_شمېره;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         پروګرام_name, file_name, disorder_کرښه_شمېره);
                write_کرښه (disorder_کرښه, stderr, _("standard error"));
              }

            ordered = false;
            اكسر;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      طالما (linebase < --کرښه)
        إذا (nonunique <= compare (کرښه, کرښه - 1))
          اذهب وموندل_disorder;

      کرښه_شمېره += buf.nlines;

      /* Save the last line of the buffer.  */
      إذا (alloc < کرښه->اوږدتیا)
        {
          افعل
            {
              alloc *= 2;
              إذا (! alloc)
                {
                  alloc = کرښه->اوږدتیا;
                  اكسر;
                }
            }
          طالما (alloc < کرښه->اوږدتیا);

          حرر (لنډمهاله.متن);
          لنډمهاله.متن = xmalloc (alloc);
        }
      حافظه_نقل (لنډمهاله.متن, کرښه->متن, کرښه->اوږدتیا);
      لنډمهاله.اوږدتیا = کرښه->اوږدتیا;
      إذا (کلی)
        {
          لنډمهاله.keybeg = لنډمهاله.متن + (کرښه->keybeg - کرښه->متن);
          لنډمهاله.keylim = لنډمهاله.متن + (کرښه->keylim - کرښه->متن);
        }
    }

  xfclose (fp, file_name);
  حرر (buf.buf);
  حرر (لنډمهاله.متن);
  أرجع ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساكن size_t
open_ننوت_files (بنية sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحيح i;

  /* Open as many input files as we can.  */
  لكل (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].لنډمهاله && files[i].لنډمهاله->state != UNCOMPRESSED
                ? open_لنډمهاله (files[i].لنډمهاله)
                : بهیر_open (files[i].name, "r"));
      إذا (!fps[i])
        اكسر;
    }

  أرجع i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ساكن فراغ
mergefps (بنية sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, حرف ثابت *output_file, FILE **fps)
{
  بنية بفر *بفر = xnmalloc (nfiles, حجم *بفر);
                                /* Input buffers for each file. */
  بنية کرښه saved;		/* Saved line storage for unique check. */
  بنية کرښه ثابت *savedline = عدم;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  بنية کرښه ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  بنية کرښه ثابت **بنسټ_2 = xnmalloc (nfiles, حجم *بنسټ_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  بنية keyfield ثابت *کلی = keylist;
  saved.متن = عدم;

  /* Read initial lines from each input file. */
  لكل (size_t i = 0; i < nfiles; )
    {
      initbuf (&بفر[i], حجم (بنية کرښه),
               MAX (یوځای_بفر_اندازه, ترتیبول_اندازه / nfiles));
      إذا (fillbuf (&بفر[i], fps[i], files[i].name, عدم))
        {
          بنية کرښه ثابت *linelim = بفر_linelim (&بفر[i]);
          cur[i] = linelim - 1;
          بنسټ_2[i] = linelim - بفر[i].nlines;
          i++;
        }
      وإلا
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          إذا (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          حرر (بفر[i].buf);
          --nfiles;
          لكل (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  لكل (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  لكل (size_t i = 1; i < nfiles; ++i)
    إذا (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  طالما (nfiles)
    {
      بنية کرښه ثابت *تر_ټولو_کوچنی = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      إذا (unique)
        {
          إذا (savedline && compare (savedline, تر_ټولو_کوچنی))
            {
              savedline = عدم;
              write_کرښه (&saved, ofp, output_file);
            }
          إذا (!savedline)
            {
              savedline = &saved;
              إذا (savealloc < تر_ټولو_کوچنی->اوږدتیا)
                {
                  افعل
                    إذا (! savealloc)
                      {
                        savealloc = تر_ټولو_کوچنی->اوږدتیا;
                        اكسر;
                      }
                  طالما ((savealloc *= 2) < تر_ټولو_کوچنی->اوږدتیا);

                  حرر (saved.متن);
                  saved.متن = xmalloc (savealloc);
                }
              saved.اوږدتیا = تر_ټولو_کوچنی->اوږدتیا;
              حافظه_نقل (saved.متن, تر_ټولو_کوچنی->متن, saved.اوږدتیا);
              إذا (کلی)
                {
                  saved.keybeg =
                    saved.متن + (تر_ټولو_کوچنی->keybeg - تر_ټولو_کوچنی->متن);
                  saved.keylim =
                    saved.متن + (تر_ټولو_کوچنی->keylim - تر_ټولو_کوچنی->متن);
                }
            }
        }
      وإلا
        write_کرښه (تر_ټولو_کوچنی, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      إذا (بنسټ_2[ord[0]] < تر_ټولو_کوچنی)
        cur[ord[0]] = تر_ټولو_کوچنی - 1;
      وإلا
        {
          إذا (fillbuf (&بفر[ord[0]], fps[ord[0]], files[ord[0]].name,
                       عدم))
            {
              بنية کرښه ثابت *linelim = بفر_linelim (&بفر[ord[0]]);
              cur[ord[0]] = linelim - 1;
              بنسټ_2[ord[0]] = linelim - بفر[ord[0]].nlines;
            }
          وإلا
            {
              /* We reached EOF on fps[ord[0]].  */
              لكل (size_t i = 1; i < nfiles; ++i)
                إذا (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              إذا (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              حرر (بفر[ord[0]].buf);
              لكل (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  بفر[i] = بفر[i + 1];
                  cur[i] = cur[i + 1];
                  بنسټ_2[i] = بنسټ_2[i + 1];
                }
              لكل (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              استمر;
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
        size_t شمېر_د_smaller_کرښې;

        طالما (lo < hi)
          {
            صحيح cmp = compare (cur[ord0], cur[ord[probe]]);
            إذا (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            وإلا
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        شمېر_د_smaller_کرښې = lo - 1;
        لكل (size_t j = 0; j < شمېر_د_smaller_کرښې; j++)
          ord[j] = ord[j + 1];
        ord[شمېر_د_smaller_کرښې] = ord0;
      }
    }

  إذا (unique && savedline)
    {
      write_کرښه (&saved, ofp, output_file);
      حرر (saved.متن);
    }

  xfclose (ofp, output_file);
  حرر (fps);
  حرر (بفر);
  حرر (ord);
  حرر (بنسټ_2);
  حرر (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ساكن size_t
mergefiles (بنية sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, حرف ثابت *output_file)
{
  FILE **fps;
  size_t nopened = open_ننوت_files (files, nfiles, &fps);
  إذا (nopened < nfiles && nopened < 2)
    ترتیبول_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  أرجع nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساكن فراغ
mergelines (بنية کرښه *محدود t, size_t nlines,
            بنية کرښه ثابت *محدود lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية کرښه *hi = t - nlo;

  طالما (true)
    إذا (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        إذا (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            أرجع;
          }
      }
    وإلا
      {
        *--t = *--hi;
        إذا (! --nhi)
          {
            افعل
              *--t = *--lo;
            طالما (--nlo);

            أرجع;
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

ساكن فراغ
sequential_ترتیبول (بنية کرښه *محدود کرښې, size_t nlines,
                 بنية کرښه *محدود لنډمهاله, bool منزل_لنډمهاله)
{
  إذا (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحيح بدلون = (0 < compare (&کرښې[-1], &کرښې[-2]));
      إذا (منزل_لنډمهاله)
        {
          لنډمهاله[-1] = کرښې[-1 - بدلون];
          لنډمهاله[-2] = کرښې[-2 + بدلون];
        }
      وإلا إذا (بدلون)
        {
          لنډمهاله[-1] = کرښې[-1];
          کرښې[-1] = کرښې[-2];
          کرښې[-2] = لنډمهاله[-1];
        }
    }
  وإلا
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      بنية کرښه *lo = کرښې;
      بنية کرښه *hi = کرښې - nlo;

      sequential_ترتیبول (hi, nhi, لنډمهاله - (منزل_لنډمهاله ? nlo : 0), منزل_لنډمهاله);
      إذا (1 < nlo)
        sequential_ترتیبول (lo, nlo, لنډمهاله, !منزل_لنډمهاله);
      وإلا إذا (!منزل_لنډمهاله)
        لنډمهاله[-1] = lo[-1];

      بنية کرښه *dest;
      بنية کرښه ثابت *sorted_lo;
      إذا (منزل_لنډمهاله)
        {
          dest = لنډمهاله;
          sorted_lo = کرښې;
        }
      وإلا
        {
          dest = کرښې;
          sorted_lo = لنډمهاله;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساكن بنية یوځای_غوټه *init_غوټه (بنية یوځای_غوټه *محدود,
                                     بنية یوځای_غوټه *محدود,
                                     بنية کرښه *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساكن بنية یوځای_غوټه *
یوځای_ونه_init (size_t nthreads, size_t nlines, بنية کرښه *dest)
{
  بنية یوځای_غوټه *یوځای_ونه = xmalloc (2 * حجم *یوځای_ونه * nthreads);

  بنية یوځای_غوټه *ریښه = یوځای_ونه;
  ریښه->lo = ریښه->hi = ریښه->پای_ټکی_lo = ریښه->پای_ټکی_hi = عدم;
  ریښه->dest = عدم;
  ریښه->nlo = ریښه->nhi = nlines;
  ریښه->پلار = عدم;
  ریښه->level = یوځای_پای_ټکی;
  ریښه->queued = false;
  pthread_mutex_init (&ریښه->قلف, عدم);

  init_غوټه (ریښه, ریښه + 1, dest, nthreads, nlines, false);
  أرجع یوځای_ونه;
}

/* Destroy the merge tree. */
ساكن فراغ
یوځای_ونه_destroy (size_t nthreads, بنية یوځای_غوټه *یوځای_ونه)
{
  size_t n_nodes = nthreads * 2;
  بنية یوځای_غوټه *غوټه = یوځای_ونه;

  طالما (n_nodes--)
    {
      pthread_mutex_destroy (&غوټه->قلف);
      غوټه++;
    }

  حرر (یوځای_ونه);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساكن بنية یوځای_غوټه *
init_غوټه (بنية یوځای_غوټه *محدود پلار,
           بنية یوځای_غوټه *محدود غوټه_pool,
           بنية کرښه *dest, size_t nthreads,
           size_t ټول_کرښې, bool is_lo_ماشوم)
{
  size_t nlines = (is_lo_ماشوم ? پلار->nlo : پلار->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية کرښه *lo = dest - ټول_کرښې;
  بنية کرښه *hi = lo - nlo;
  بنية کرښه **پلار_پای_ټکی = (is_lo_ماشوم ? &پلار->پای_ټکی_lo : &پلار->پای_ټکی_hi);

  بنية یوځای_غوټه *غوټه = غوټه_pool++;
  غوټه->lo = غوټه->پای_ټکی_lo = lo;
  غوټه->hi = غوټه->پای_ټکی_hi = hi;
  غوټه->dest = پلار_پای_ټکی;
  غوټه->nlo = nlo;
  غوټه->nhi = nhi;
  غوټه->پلار = پلار;
  غوټه->level = پلار->level + 1;
  غوټه->queued = false;
  pthread_mutex_init (&غوټه->قلف, عدم);

  إذا (nthreads > 1)
    {
      size_t lo_تارونه = nthreads / 2;
      size_t hi_تارونه = nthreads - lo_تارونه;
      غوټه->lo_ماشوم = غوټه_pool;
      غوټه_pool = init_غوټه (غوټه, غوټه_pool, lo, lo_تارونه,
                             ټول_کرښې, true);
      غوټه->hi_ماشوم = غوټه_pool;
      غوټه_pool = init_غوټه (غوټه, غوټه_pool, hi, hi_تارونه,
                             ټول_کرښې, false);
    }
  وإلا
    {
      غوټه->lo_ماشوم = عدم;
      غوټه->hi_ماشوم = عدم;
    }
  أرجع غوټه_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساكن صحيح
compare_nodes (فراغ ثابت *a, فراغ ثابت *b)
{
  بنية یوځای_غوټه ثابت *nodea = a;
  بنية یوځای_غوټه ثابت *nodeb = b;
  إذا (nodea->level == nodeb->level)
      أرجع (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  أرجع nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساكن مضمن فراغ
قلف_غوټه (بنية یوځای_غوټه *غوټه)
{
  pthread_mutex_قلف (&غوټه->قلف);
}

/* Unlock a merge tree NODE. */

ساكن مضمن فراغ
unlock_غوټه (بنية یوځای_غوټه *غوټه)
{
  pthread_mutex_unlock (&غوټه->قلف);
}

/* Destroy merge QUEUE. */

ساكن فراغ
کتار_destroy (بنية یوځای_غوټه_کتار *کتار)
{
  ډېرۍ_free (کتار->لومړیتوب_کتار);
  pthread_cond_destroy (&کتار->cond);
  pthread_mutex_destroy (&کتار->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساكن فراغ
کتار_init (بنية یوځای_غوټه_کتار *کتار, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  کتار->لومړیتوب_کتار = ډېرۍ_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&کتار->mutex, عدم);
  pthread_cond_init (&کتار->cond, عدم);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ساكن فراغ
کتار_ورننباسه (بنية یوځای_غوټه_کتار *کتار, بنية یوځای_غوټه *غوټه)
{
  pthread_mutex_قلف (&کتار->mutex);
  ډېرۍ_ورننباسه (کتار->لومړیتوب_کتار, غوټه);
  غوټه->queued = true;
  pthread_cond_signal (&کتار->cond);
  pthread_mutex_unlock (&کتار->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساكن بنية یوځای_غوټه *
کتار_راوباسه (بنية یوځای_غوټه_کتار *کتار)
{
  بنية یوځای_غوټه *غوټه;
  pthread_mutex_قلف (&کتار->mutex);
  طالما (! (غوټه = ډېرۍ_remove_سر (کتار->لومړیتوب_کتار)))
    pthread_cond_wait (&کتار->cond, &کتار->mutex);
  pthread_mutex_unlock (&کتار->mutex);
  قلف_غوټه (غوټه);
  غوټه->queued = false;
  أرجع غوټه;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساكن فراغ
write_unique (بنية کرښه ثابت *کرښه, FILE *tfp, حرف ثابت *لنډمهاله_output)
{
  إذا (unique)
    {
      إذا (saved_کرښه.متن && ! compare (کرښه, &saved_کرښه))
        أرجع;
      saved_کرښه = *کرښه;
    }

  write_کرښه (کرښه, tfp, لنډمهاله_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساكن فراغ
mergelines_غوټه (بنية یوځای_غوټه *محدود غوټه, size_t ټول_کرښې,
                 FILE *tfp, حرف ثابت *لنډمهاله_output)
{
  بنية کرښه *lo_orig = غوټه->lo;
  بنية کرښه *hi_orig = غوټه->hi;
  size_t منزل_یوځای = MAX_یوځای (ټول_کرښې, غوټه->level);
  size_t merged_lo;
  size_t merged_hi;

  إذا (غوټه->level > یوځای_ریښه)
    {
      /* Merge to destination buffer. */
      بنية کرښه *dest = *غوټه->dest;
      طالما (غوټه->lo != غوټه->پای_ټکی_lo && غوټه->hi != غوټه->پای_ټکی_hi && منزل_یوځای--)
        إذا (compare (غوټه->lo - 1, غوټه->hi - 1) <= 0)
          *--dest = *--غوټه->lo;
        وإلا
          *--dest = *--غوټه->hi;

      merged_lo = lo_orig - غوټه->lo;
      merged_hi = hi_orig - غوټه->hi;

      إذا (غوټه->nhi == merged_hi)
        طالما (غوټه->lo != غوټه->پای_ټکی_lo && منزل_یوځای--)
          *--dest = *--غوټه->lo;
      وإلا إذا (غوټه->nlo == merged_lo)
        طالما (غوټه->hi != غوټه->پای_ټکی_hi && منزل_یوځای--)
          *--dest = *--غوټه->hi;
      *غوټه->dest = dest;
    }
  وإلا
    {
      /* Merge directly to output. */
      طالما (غوټه->lo != غوټه->پای_ټکی_lo && غوټه->hi != غوټه->پای_ټکی_hi && منزل_یوځای--)
        {
          إذا (compare (غوټه->lo - 1, غوټه->hi - 1) <= 0)
            write_unique (--غوټه->lo, tfp, لنډمهاله_output);
          وإلا
            write_unique (--غوټه->hi, tfp, لنډمهاله_output);
        }

      merged_lo = lo_orig - غوټه->lo;
      merged_hi = hi_orig - غوټه->hi;

      إذا (غوټه->nhi == merged_hi)
        {
          طالما (غوټه->lo != غوټه->پای_ټکی_lo && منزل_یوځای--)
            write_unique (--غوټه->lo, tfp, لنډمهاله_output);
        }
      وإلا إذا (غوټه->nlo == merged_lo)
        {
          طالما (غوټه->hi != غوټه->پای_ټکی_hi && منزل_یوځای--)
            write_unique (--غوټه->hi, tfp, لنډمهاله_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - غوټه->lo;
  merged_hi = hi_orig - غوټه->hi;
  غوټه->nlo -= merged_lo;
  غوټه->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساكن فراغ
کتار_check_ورننباسه (بنية یوځای_غوټه_کتار *کتار, بنية یوځای_غوټه *غوټه)
{
  إذا (! غوټه->queued)
    {
      bool lo_avail = (غوټه->lo - غوټه->پای_ټکی_lo) != 0;
      bool hi_avail = (غوټه->hi - غوټه->پای_ټکی_hi) != 0;
      إذا (lo_avail ? hi_avail || ! غوټه->nhi : hi_avail && ! غوټه->nlo)
        کتار_ورننباسه (کتار, غوټه);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساكن فراغ
کتار_check_ورننباسه_پلار (بنية یوځای_غوټه_کتار *کتار,
                           بنية یوځای_غوټه *غوټه)
{
  إذا (غوټه->level > یوځای_ریښه)
    {
      قلف_غوټه (غوټه->پلار);
      کتار_check_ورننباسه (کتار, غوټه->پلار);
      unlock_غوټه (غوټه->پلار);
    }
  وإلا إذا (غوټه->nlo + غوټه->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      کتار_ورننباسه (کتار, غوټه->پلار);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساكن فراغ
یوځای_loop (بنية یوځای_غوټه_کتار *کتار,
            size_t ټول_کرښې, FILE *tfp, حرف ثابت *لنډمهاله_output)
{
  طالما (true)
    {
      بنية یوځای_غوټه *غوټه = کتار_راوباسه (کتار);

      إذا (غوټه->level == یوځای_پای_ټکی)
        {
          unlock_غوټه (غوټه);
          /* Reinsert so other threads can pop it. */
          کتار_ورننباسه (کتار, غوټه);
          اكسر;
        }
      mergelines_غوټه (غوټه, ټول_کرښې, tfp, لنډمهاله_output);
      کتار_check_ورننباسه (کتار, غوټه);
      کتار_check_ورننباسه_پلار (کتار, غوټه);

      unlock_غوټه (غوټه);
    }
}


ساكن فراغ sortlines (بنية کرښه *محدود, size_t, size_t,
                       بنية یوځای_غوټه *, بنية یوځای_غوټه_کتار *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

بنية thread_دلیلونه
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  بنية کرښه *کرښې;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت ټول_کرښې;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  بنية یوځای_غوټه *ثابت غوټه;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  بنية یوځای_غوټه_کتار *ثابت کتار;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  حرف ثابت *output_لنډمهاله;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ساكن فراغ *
sortlines_thread (فراغ *معلومات)
{
  بنية thread_دلیلونه ثابت *دلیلونه = معلومات;
  sortlines (دلیلونه->کرښې, دلیلونه->nthreads, دلیلونه->ټول_کرښې,
             دلیلونه->غوټه, دلیلونه->کتار, دلیلونه->tfp,
             دلیلونه->output_لنډمهاله);
  أرجع عدم;
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

ساكن فراغ
sortlines (بنية کرښه *محدود کرښې, size_t nthreads,
           size_t ټول_کرښې, بنية یوځای_غوټه *غوټه,
           بنية یوځای_غوټه_کتار *کتار, FILE *tfp, حرف ثابت *لنډمهاله_output)
{
  size_t nlines = غوټه->nlo + غوټه->nhi;

  /* Calculate thread arguments. */
  size_t lo_تارونه = nthreads / 2;
  size_t hi_تارونه = nthreads - lo_تارونه;
  pthread_t thread;
  بنية thread_دلیلونه دلیلونه = {کرښې, lo_تارونه, ټول_کرښې,
                             غوټه->lo_ماشوم, کتار, tfp, لنډمهاله_output};

  إذا (nthreads > 1 && SUBTHREAD_کرښې_HEURISTIC <= nlines
      && pthread_create (&thread, عدم, sortlines_thread, &دلیلونه) == 0)
    {
      sortlines (کرښې - غوټه->nlo, hi_تارونه, ټول_کرښې,
                 غوټه->hi_ماشوم, کتار, tfp, لنډمهاله_output);
      pthread_join (thread, عدم);
    }
  وإلا
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = غوټه->nlo;
      size_t nhi = غوټه->nhi;
      بنية کرښه *لنډمهاله = کرښې - ټول_کرښې;
      إذا (1 < nhi)
        sequential_ترتیبول (کرښې - nlo, nhi, لنډمهاله - nlo / 2, false);
      إذا (1 < nlo)
        sequential_ترتیبول (کرښې, nlo, لنډمهاله, false);

      /* Update merge NODE. No need to lock yet. */
      غوټه->lo = کرښې;
      غوټه->hi = کرښې - nlo;
      غوټه->پای_ټکی_lo = کرښې - nlo;
      غوټه->پای_ټکی_hi = کرښې - nlo - nhi;

      کتار_ورننباسه (کتار, غوټه);
      یوځای_loop (کتار, ټول_کرښې, tfp, لنډمهاله_output);
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

ساكن فراغ
avoid_trashing_ننوت (بنية sortfile *files, size_t ntemps,
                      size_t nfiles, حرف ثابت *outfile)
{
  بنية tempnode *tempcopy = عدم;

  لكل (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      بنية stat instat;

      إذا (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      وإلا
        {
          بنية stat *outst = get_outstatus ();
          إذا (!outst)
            اكسر;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      إذا (same)
        {
          إذا (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_لنډمهاله (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].لنډمهاله = tempcopy;
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

ساكن فراغ
check_inputs (حرف *ثابت *files, size_t nfiles)
{
  لكل (size_t i = 0; i < nfiles; i++)
    {
      إذا (streq (files[i], "-"))
        استمر;

      إذا (euidaccess (files[i], R_OK) != 0)
        ترتیبول_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ساكن فراغ
check_output (حرف ثابت *outfile)
{
  إذا (outfile)
    {
      صحيح oflags = O_WRONLY | O_دوګونی | O_CLOEXEC | O_CREAT;
      صحيح outfd = open (outfile, oflags, ډېرپېښ_RW_UGO);
      إذا (outfd < 0)
        ترتیبول_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ساكن فراغ
یوځای (بنية sortfile *files, size_t ntemps, size_t nfiles,
       حرف ثابت *output_file)
{
  طالما (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t دننه;

      /* Number of output files generated so far.  */
      size_t بهر;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      لكل (بهر = دننه = 0; nmerge <= nfiles - دننه; بهر++)
        {
          FILE *tfp;
          بنية tempnode *لنډمهاله = create_لنډمهاله (&tfp);
          size_t عدد_merged = mergefiles (&files[دننه], MIN (ntemps, nmerge),
                                          nmerge, tfp, لنډمهاله->name);
          ntemps -= MIN (ntemps, عدد_merged);
          files[بهر].name = لنډمهاله->name;
          files[بهر].لنډمهاله = لنډمهاله;
          دننه += عدد_merged;
        }

      remainder = nfiles - دننه;
      cheap_slots = nmerge - بهر % nmerge;

      إذا (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          بنية tempnode *لنډمهاله = create_لنډمهاله (&tfp);
          size_t عدد_merged = mergefiles (&files[دننه], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, لنډمهاله->name);
          ntemps -= MIN (ntemps, عدد_merged);
          files[بهر].name = لنډمهاله->name;
          files[بهر++].لنډمهاله = لنډمهاله;
          دننه += عدد_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[بهر], &files[دننه], (nfiles - دننه) * حجم *files);
      ntemps += بهر;
      nfiles -= دننه - بهر;
    }

  avoid_trashing_ننوت (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  طالما (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ننوت_files (files, nfiles, &fps);

      إذا (nopened == nfiles)
        {
          FILE *ofp = بهیر_open (output_file, "w");
          إذا (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              اكسر;
            }
          إذا (errno != EMFILE || nopened <= 2)
            ترتیبول_die (_("open failed"), output_file);
        }
      وإلا إذا (nopened <= 2)
        ترتیبول_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      بنية tempnode *لنډمهاله;
      افعل
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          لنډمهاله = maybe_create_لنډمهاله (&tfp, ! (nopened <= 2));
        }
      طالما (!لنډمهاله);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, لنډمهاله->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = لنډمهاله->name;
      files[0].لنډمهاله = لنډمهاله;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * حجم *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ساكن فراغ
ترتیبول (حرف *ثابت *files, size_t nfiles, حرف ثابت *output_file,
      size_t nthreads)
{
  بنية بفر buf;
  بنية ترتیبول_بفر_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.حد = 0;
  policy.growth_failed = false;

  طالما (nfiles)
    {
      حرف ثابت *لنډمهاله_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t بایټونه_فی_کرښه;
      إذا (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          طالما (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          بایټونه_فی_کرښه = (mult * حجم (بنية کرښه));
        }
      وإلا
        بایټونه_فی_کرښه = حجم (بنية کرښه) * 3 / 2;

      إذا (! buf.alloc)
        {
          ترتیبول_بفر_policy (&fp, 1, files, nfiles, بایټونه_فی_کرښه,
                              &policy);
          initbuf (&buf, بایټونه_فی_کرښه, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      طالما (fillbuf (&buf, fp, file, &policy))
        {
          بنية کرښه *کرښه;

          إذا (buf.eof && nfiles
              && (بایټونه_فی_کرښه + 1
                  < (buf.alloc - buf.کارول_شوی - بایټونه_فی_کرښه * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.کیڼ = buf.کارول_شوی;
              اكسر;
            }

          saved_کرښه.متن = عدم;
          کرښه = بفر_linelim (&buf);
          إذا (buf.eof && !nfiles && !ntemps && !buf.کیڼ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              لنډمهاله_output = output_file;
              output_file_created = true;
            }
          وإلا
            {
              ++ntemps;
              لنډمهاله_output = create_لنډمهاله (&tfp)->name;
            }
          إذا (1 < buf.nlines)
            {
              إذا (nthreads > 1)
                {
                  بنية یوځای_غوټه_کتار کتار;
                  کتار_init (&کتار, nthreads);
                  بنية یوځای_غوټه *یوځای_ونه =
                    یوځای_ونه_init (nthreads, buf.nlines, کرښه);

                  sortlines (کرښه, nthreads, buf.nlines, یوځای_ونه + 1,
                             &کتار, tfp, لنډمهاله_output);

                  یوځای_ونه_destroy (nthreads, یوځای_ونه);
                  کتار_destroy (&کتار);
                }
              وإلا
                {
                  sequential_ترتیبول (کرښه, buf.nlines,
                                   کرښه - buf.nlines, false);
                  لكل (size_t i = 0; i < buf.nlines; i++)
                    write_unique (کرښه - i - 1, tfp, لنډمهاله_output);
                }
            }
          وإلا
            write_unique (کرښه - 1, tfp, لنډمهاله_output);

          xfclose (tfp, لنډمهاله_output);

          إذا (output_file_created)
            اذهب پای;
        }
      xfclose (fp, file);
    }

 پای:
  حرر (buf.buf);

  إذا (! output_file_created)
    {
      بنية tempnode *غوټه = temphead;
      بنية sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      لكل (size_t i = 0; غوټه; i++)
        {
          tempfiles[i].name = غوټه->name;
          tempfiles[i].لنډمهاله = غوټه;
          غوټه = غوټه->بل;
        }
      یوځای (tempfiles, ntemps, ntemps, output_file);
      حرر (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساكن فراغ
insertkey (بنية keyfield *کلی_arg)
{
  بنية keyfield **p;
  بنية keyfield *کلی = xmemdup (کلی_arg, حجم *کلی);

  لكل (p = &keylist; *p; p = &(*p)->بل)
    استمر;
  *p = کلی;
  کلی->بل = عدم;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ساكن فراغ
badfieldspec (حرف ثابت *spec, حرف ثابت *msgid)
{
  error (ترتیبول_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ساكن فراغ
incompatible_options (حرف ثابت *opts)
{
  error (ترتیبول_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ساكن فراغ
check_ordering_compatibility (فراغ)
{
  لكل (بنية keyfield *کلی = keylist; کلی; کلی = کلی->بل)
    إذا (1 < (کلی->numeric + کلی->general_numeric + کلی->human_numeric
             + کلی->month + (کلی->نسخه_3 | کلی->random | !!کلی->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        کلی->skipsblanks = کلی->skipeblanks = کلی->سرچپه = false;
        کلی_منزل_opts (کلی, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساكن حرف ثابت *
parse_ډګر_شمېر (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
{
  حرف *suffix;
  uintmax_t n;

  بدل (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    حالة LONGINT_OK:
    حالة LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      إذا (*val == n)
        اكسر;
      FALLTHROUGH;
    حالة LONGINT_ډکېدنه:
    حالة LONGINT_ډکېدنه | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      اكسر;

    حالة LONGINT_INVALID:
      إذا (msgid)
        error (ترتیبول_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      أرجع عدم;
    }

  أرجع suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ساكن حرف *
سېټ_ordering (حرف ثابت *s, بنية keyfield *کلی, تعداد blanktype blanktype)
{
  طالما (*s)
    {
      بدل (*s)
        {
        حالة 'b':
          إذا (blanktype == bl_پیل || blanktype == bl_both)
            کلی->skipsblanks = true;
          إذا (blanktype == bl_پای_ټکی || blanktype == bl_both)
            کلی->skipeblanks = true;
          اكسر;
        حالة 'd':
          کلی->ignore = nondictionary;
          اكسر;
        حالة 'f':
          کلی->translate = fold_toupper;
          اكسر;
        حالة 'g':
          کلی->general_numeric = true;
          اكسر;
        حالة 'h':
          کلی->human_numeric = true;
          اكسر;
        حالة 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          إذا (! کلی->ignore)
            کلی->ignore = nonprinting;
          اكسر;
        حالة 'M':
          کلی->month = true;
          اكسر;
        حالة 'n':
          کلی->numeric = true;
          اكسر;
        حالة 'R':
          کلی->random = true;
          اكسر;
        حالة 'r':
          کلی->سرچپه = true;
          اكسر;
        حالة 'V':
          کلی->نسخه_3 = true;
          اكسر;
        افتراضي:
          أرجع (حرف *) s;
        }
      ++s;
    }
  أرجع (حرف *) s;
}

/* Initialize KEY.  */

ساكن بنية keyfield *
کلی_init (بنية keyfield *کلی)
{
  memset (کلی, 0, حجم *کلی);
  کلی->eword = SIZE_MAX;
  أرجع کلی;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  بنية keyfield *کلی;
  بنية keyfield کلی_buf;
  بنية keyfield gkey;
  bool gkey_only = false;
  حرف ثابت *s;
  صحيح c = 0;
  حرف checkonly = 0;
  bool mergeonly = false;
  حرف *random_سرچینه = عدم;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  صحيح posix_ver = posix2_نسخه ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  حرف **files;
  حرف *files_from = عدم;
  بنية نښې tok;
  حرف ثابت *outfile = عدم;
  bool locale_ok;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ترتیبول_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#إذا HAVE_NL_LANGINFO
  hard_LC_وخت = hard_locale (LC_وخت);
#د_که_پای

  /* Get locale's representation of the decimal point.  */
  {
    بنية lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_ټکی = locale->decimal_ټکی[0];
    إذا (! decimal_ټکی || locale->decimal_ټکی[1])
      decimal_ټکی = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    إذا (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    إذا (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    تعداد { nsigs = countof (غړی_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سیګنالونه);
    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (غړی_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        sigaction (غړی_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سیګنالونه, غړی_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_سیګنالونه;
    act.sa_flags = 0;

    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (غړی_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        إذا (sigismember (&caught_سیګنالونه, غړی_sig[i]))
          sigaction (غړی_sig[i], &act, عدم);
      }
  }
  إشارة (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  کلی_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, حجم *files);

  طالما (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      صحيح oi = -1;

      إذا (c == -1
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
          إذا (argc <= optind)
            اكسر;
          files[nfiles++] = argv[optind++];
        }
      وإلا بدل (c)
        {
        حالة 1:
          کلی = عدم;
          إذا (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              إذا (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  کلی = کلی_init (&کلی_buf);
                  s = parse_ډګر_شمېر (optarg + 1, &کلی->sword, عدم);
                  إذا (s && *s == '.')
                    s = parse_ډګر_شمېر (s + 1, &کلی->schar, عدم);
                  إذا (! (کلی->sword || کلی->schar))
                    کلی->sword = SIZE_MAX;
                  إذا (! s || *سېټ_ordering (s, کلی, bl_پیل))
                    کلی = عدم;
                  وإلا
                    {
                      إذا (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_ډګر_شمېر (optarg1 + 1, &کلی->eword,
                                             N_("invalid number after '-'"));
                          إذا (*s == '.')
                            s = parse_ډګر_شمېر (s + 1, &کلی->echar,
                                               N_("invalid number after '.'"));
                          إذا (!کلی->echar && کلی->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              کلی->eword--;
                            }
                          إذا (*سېټ_ordering (s, کلی, bl_پای_ټکی))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      کلی->traditional_کارول_شوی = true;
                      insertkey (کلی);
                    }
                }
            }
          إذا (! کلی)
            files[nfiles++] = optarg;
          اكسر;

        حالة ترتیبول_OPTION:
          c = XARGMATCH ("--sort", optarg, ترتیبول_دلیلونه, ترتیبول_types);
          FALLTHROUGH;
        حالة 'b':
        حالة 'd':
        حالة 'f':
        حالة 'g':
        حالة 'h':
        حالة 'i':
        حالة 'M':
        حالة 'n':
        حالة 'r':
        حالة 'R':
        حالة 'V':
          {
            حرف str[2];
            str[0] = c;
            str[1] = '\0';
            سېټ_ordering (str, &gkey, bl_both);
          }
          اكسر;

        حالة CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_دلیلونه, check_types)
               : 'c');
          FALLTHROUGH;
        حالة 'c':
        حالة 'C':
          إذا (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          اكسر;

        حالة COMPRESS_پروګرام_OPTION:
          إذا (compress_پروګرام && !streq (compress_پروګرام, optarg))
            error (ترتیبول_FAILURE, 0, _("multiple compress programs specified"));
          compress_پروګرام = optarg;
          اكسر;

        حالة DEBUG_پروګرام_OPTION:
          debug = true;
          اكسر;

        حالة FILES0_FROM_OPTION:
          files_from = optarg;
          اكسر;

        حالة 'k':
          کلی = کلی_init (&کلی_buf);

          /* Get POS1. */
          s = parse_ډګر_شمېر (optarg, &کلی->sword,
                                 N_("invalid number at field start"));
          إذا (! کلی->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          إذا (*s == '.')
            {
              s = parse_ډګر_شمېر (s + 1, &کلی->schar,
                                     N_("invalid number after '.'"));
              إذا (! کلی->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          إذا (! (کلی->sword || کلی->schar))
            کلی->sword = SIZE_MAX;
          s = سېټ_ordering (s, کلی, bl_پیل);
          إذا (*s != ',')
            {
              کلی->eword = SIZE_MAX;
              کلی->echar = 0;
            }
          وإلا
            {
              /* Get POS2. */
              s = parse_ډګر_شمېر (s + 1, &کلی->eword,
                                     N_("invalid number after ','"));
              إذا (! کلی->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              إذا (*s == '.')
                {
                  s = parse_ډګر_شمېر (s + 1, &کلی->echar,
                                         N_("invalid number after '.'"));
                }
              s = سېټ_ordering (s, کلی, bl_پای_ټکی);
            }
          إذا (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (کلی);
          اكسر;

        حالة 'm':
          mergeonly = true;
          اكسر;

        حالة NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          اكسر;

        حالة 'o':
          إذا (outfile && !streq (outfile, optarg))
            error (ترتیبول_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          اكسر;

        حالة RANDOM_سرچینه_OPTION:
          إذا (random_سرچینه && !streq (random_سرچینه, optarg))
            error (ترتیبول_FAILURE, 0, _("multiple random sources specified"));
          random_سرچینه = optarg;
          اكسر;

        حالة 's':
          stable = true;
          اكسر;

        حالة 'S':
          specify_ترتیبول_اندازه (oi, c, optarg);
          اكسر;

        حالة 't':
          {
            حرف newtab = optarg[0];
            إذا (! newtab)
              error (ترتیبول_FAILURE, 0, _("empty tab"));
            إذا (optarg[1])
              {
                إذا (streq (optarg, "\\0"))
                  newtab = '\0';
                وإلا
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (ترتیبول_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            إذا (tab != TAB_DEFAULT && tab != newtab)
              error (ترتیبول_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          اكسر;

        حالة 'T':
          جمع_کړه_لنډمهاله_dir (optarg);
          اكسر;

        حالة PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          اكسر;

        حالة 'u':
          unique = true;
          اكسر;

        حالة 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          إذا (optarg == argv[optind - 1])
            {
              حرف ثابت *p;
              لكل (p = optarg; c_isdigit (*p); p++)
                استمر;
              optind -= (*p != '\0');
            }
          اكسر;

        حالة 'z':
          eolchar = 0;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

        افتراضي:
          usage (ترتیبول_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (إشارة (SIGPIPE, SIG_IGN) == SIG_DFL);

  إذا (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      إذا (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (ترتیبول_FAILURE);
        }

      FILE *بهیر = xfopen (files_from, "r");

      readtokens0_init (&tok);

      إذا (! readtokens0 (بهیر, &tok))
        error (ترتیبول_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (بهیر, files_from);

      إذا (tok.n_tok)
        {
          حرر (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          لكل (size_t i = 0; i < nfiles; i++)
            {
              إذا (streq (files[i], "-"))
                error (ترتیبول_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              وإلا إذا (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  بینښه اوږد صحيح file_شمېره = i + 1;
                  error (ترتیبول_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_شمېره);
                }
            }
        }
      وإلا
        error (ترتیبول_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  لكل (کلی = keylist; کلی; کلی = کلی->بل)
    {
      إذا (default_کلی_compare (کلی) && !کلی->سرچپه)
        {
          کلی->ignore = gkey.ignore;
          کلی->translate = gkey.translate;
          کلی->skipsblanks = gkey.skipsblanks;
          کلی->skipeblanks = gkey.skipeblanks;
          کلی->month = gkey.month;
          کلی->numeric = gkey.numeric;
          کلی->general_numeric = gkey.general_numeric;
          کلی->human_numeric = gkey.human_numeric;
          کلی->نسخه_3 = gkey.نسخه_3;
          کلی->random = gkey.random;
          کلی->سرچپه = gkey.سرچپه;
        }

      need_random |= کلی->random;
    }

  إذا (!keylist && !default_کلی_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  إذا (debug)
    {
      إذا (checkonly || outfile)
        {
          ساكن حرف opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      إذا (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      إذا (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      إذا (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, عدم)));
      وإلا
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      کلی_warnings (&gkey, gkey_only);
    }

  سرچپه = gkey.سرچپه;

  إذا (need_random)
    random_md5_state_init (random_سرچینه);

  إذا (لنډمهاله_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      جمع_کړه_لنډمهاله_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  إذا (nfiles == 0)
    {
      nfiles = 1;
      حرر (files);
      files = xmalloc (حجم *files);
      *files = (حرف *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  إذا (0 < ترتیبول_اندازه)
    ترتیبول_اندازه = MAX (ترتیبول_اندازه, MIN_ترتیبول_اندازه);

  إذا (checkonly)
    {
      إذا (nfiles > 1)
        error (ترتیبول_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      إذا (outfile)
        {
          ساكن حرف opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      وتل (check (files[0], checkonly) ? EXIT_SUCCESS : ترتیبول_بهر_د_ترتیب);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  إذا (mergeonly)
    {
      بنية sortfile *sortfiles = xcalloc (nfiles, حجم *sortfiles);

      لكل (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      یوځای (sortfiles, 0, nfiles, outfile);
    }
  وإلا
    {
      إذا (!nthreads)
        {
          بینښه اوږد صحيح np = عدد_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_تارونه);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (بنية یوځای_غوټه));
      nthreads = MIN (nthreads, nthreads_max);

      ترتیبول (files, nfiles, outfile, nthreads);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    ترتیبول_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
