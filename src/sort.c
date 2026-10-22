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

#جيڪڏهن_وصف_نه_ٿيل RLIMIT_معلومات
بنية rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#جيڪڏهن_پورو

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "sort"

#وصف_ڪريو AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#جيڪڏهن_پورو

#إذا !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#جيڪڏهن_پورو
#إذا !defined OPEN_MAX
# define OPEN_MAX 20
#جيڪڏهن_پورو

#وصف_ڪريو UCHAR_LIM (UCHAR_MAX + 1)

#جيڪڏهن_وصف_نه_ٿيل DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#جيڪڏهن_پورو

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#وصف_ڪريو MAX_ملايو(ڪل, level) (((ڪل) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
تعداد { SUBTHREAD_سٽون_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_سٽون_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
تعداد { DEFAULT_MAX_ڌاڳا = 8 };

/* Exit statuses.  */
تعداد
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    ترتيب_ڏيو_ٻاهريون_جو_ترتيب = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    ترتيب_ڏيو_FAILURE = 2
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
    ملايو_پڇاڙي = 0,

    /* Level of the root node in merge tree. */
    ملايو_پاڙ = 1
  };

/* The representation of the decimal point in the current locale.  */
ساكن حرف decimal_نقطو;

/* Thousands separator; if outside char range, there is no separator.  */
ساكن صحيح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساكن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساكن bool hard_LC_COLLATE;
#إذا HAVE_NL_LANGINFO
ساكن bool hard_LC_وقت;
#جيڪڏهن_پورو

#وصف_ڪريو NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
تعداد blanktype { bl_شروعات, bl_پڇاڙي, bl_both };

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
بنية سٽ
{
  حرف *متن;			/* Text of the line. */
  size_t ڊيگهائي;		/* Length including final newline. */
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
  size_t استعمال_ٿيل;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t کاٻو;			/* Number of bytes left from previous reads. */
  size_t سٽ_بائيٽون;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
بنية ترتيب_ڏيو_بفر_policy
{
  size_t initial;
  size_t حد_بندي;
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
  bool ابتو;			/* Reverse the sense of comparison. */
  bool نسخو_3;			/* sort by version number */
  bool traditional_استعمال_ٿيل;	/* Traditional key option format is used. */
  بنية keyfield *ايندڙ;	/* Next keyfield to try. */
};

بنية month
{
  حرف ثابت *name;
  صحيح val;
};

/* Binary merge tree node. */
بنية ملايو_ڳنڍ
{
  بنية سٽ *lo;              /* Lines to merge from LO child node. */
  بنية سٽ *hi;              /* Lines to merge from HI child node. */
  بنية سٽ *پڇاڙي_lo;          /* End of available lines from LO. */
  بنية سٽ *پڇاڙي_hi;          /* End of available lines from HI. */
  بنية سٽ **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  بنية ملايو_ڳنڍ *والد;    /* Parent node. */
  بنية ملايو_ڳنڍ *lo_ٻار;  /* LO child node. */
  بنية ملايو_ڳنڍ *hi_ٻار;  /* HI child node. */
  بنانشان صحيح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t تالو;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
بنية ملايو_ڳنڍ_قطار
{
  بنية ڍير_2 *ترجيح_قطار;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساكن بنية سٽ saved_سٽ;

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

#وصف_ڪريو MONTHS_في_YEAR 12

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
#وصف_ڪريو NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#وصف_ڪريو MIN_ملايو_بفر_سائيز (2 + حجم (بنية سٽ))

/* Minimum sort size; the code might not work with smaller sizes.  */
#وصف_ڪريو MIN_ترتيب_ڏيو_سائيز (nmerge * MIN_ملايو_بفر_سائيز)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ساكن size_t ملايو_بفر_سائيز = MAX (MIN_ملايو_بفر_سائيز, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ساكن size_t ترتيب_ڏيو_سائيز;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#وصف_ڪريو داخلا_FILE_سائيز_GUESS (128 * 1024)

ساكن بنية
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  حرف ثابت **dirs COUNTED_BY (n_dirs);
} عارضي_dirs;



/* Flag to reverse the order of all comparisons. */
ساكن bool ابتو;

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
ساكن حرف ثابت *compress_پروگرام;

/* Annotate the output with extra info to aid the user.  */
ساكن bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ساكن بنانشان صحيح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ساكن bool default_SIGPIPE;

/* The list of temporary files. */
بنية tempnode
{
  بنية tempnode *متطاير ايندڙ;
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
  لكل (بنية tempnode ثابت *ڳنڍ = temphead; ڳنڍ; ڳنڍ = ڳنڍ->ايندڙ)
    unlink (ڳنڍ->name);
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
ترتيب_ڏيو_die (حرف ثابت *پيغام_2, حرف ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  إذا (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (ترتيب_ڏيو_FAILURE, errno, "%s: %s", پيغام_2,
         quotef (file ? file : _("standard output")));
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Write sorted concatenation جو all FILE(s) منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding ترتيب_ڏيو ڪنجيون اندريون each سٽ\n\
"));
      oputs (_("\
  -d, --dictionary-ترتيب\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالة\n\
         fold هيٺاهون حالة منزل مٿانهون حالة characters\n\
"));
      oputs (_("\
  -g, --general-numeric-ترتيب_ڏيو\n\
         compare according منزل general numerical قدر\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-ترتيب_ڏيو\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-ترتيب_ڏيو\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-ترتيب_ڏيو\n\
         compare according منزل string numerical قدر;\n\
         see ڀريل documentation لكل supported لڙيون\n\
"));
      oputs (_("\
  -R, --random-ترتيب_ڏيو\n\
         shuffle, but گروهه identical ڪنجيون.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-ذريعو_2=FILE\n\
         get random بائيٽون from FILE\n\
"));
      oputs (_("\
  -r, --ابتو\n\
         ابتو the نتيجو جو comparisons\n\
"));
      oputs (_("\
      --ترتيب_ڏيو=لفظ\n\
         ترتيب_ڏيو according منزل لفظ:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, نسخو_3 -V\n\
"));
      oputs (_("\
  -V, --نسخو_3-ترتيب_ڏيو\n\
         natural ترتيب_ڏيو جو (نسخو_3) numbers within متن\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-سائيز=NMERGE\n\
         ملايو at most NMERGE inputs at once; لكل وڌيڪ use عارضي files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-پهريون\n\
         check لكل sorted داخلا; افعل not ترتيب_ڏيو\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but افعل not report پهريون bad سٽ\n\
"));
      oputs (_("\
      --compress-پروگرام=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part جو the سٽ استعمال_ٿيل منزل ترتيب_ڏيو,\n\
         and warn about questionable usage منزل standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ داخلا from the files specified by NUL-terminated نالا اندريون file F;\
\n\
         If F is -, اقرأ نالا from standard داخلا\n\
"));
      oputs (_("\
  -k, --ڪنجي=KEYDEF\n\
         ترتيب_ڏيو ذريعي a ڪنجي; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --ملايو\n\
         ملايو already sorted files; افعل not ترتيب_ڏيو\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         اكتب نتيجو منزل FILE instead جو standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize ترتيب_ڏيو by disabling آخري-resort comparison\n\
"));
      oputs (_("\
  -S, --بفر-سائيز=سائيز_2\n\
         use سائيز_2 لكل مکيه يادگيري بفر\n\
"));
      oputs (_("\
  -t, --ميدان-separator=SEP\n\
         use SEP instead جو non-blank منزل blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR لكل temporaries, not $TMPDIR or %s;\n\
         گڻو options specify گڻو directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the عدد جو sorts لاڳيتو_2 concurrently منزل N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the پهريون جو سٽون with equal ڪنجيون;\n\
         with -c, check لكل strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سٽ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] لكل شروعات and stop position, where F is a\n\
ميدان عدد and C a character position اندريون the ميدان; both are origin 1, and\n\
the stop position defaults منزل the سٽ's پڇاڙي.  If neither -t nor -b is اندريون\n\
effect, characters اندريون a ميدان are counted from the beginning جو the preceding\n\
whitespace.  OPTS is one or وڌيڪ single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options لكل that ڪنجي.  If no ڪنجي is given, use\n\
the entire سٽ as the ڪنجي.  Use --debug منزل diagnose incorrect ڪنجي usage.\n\
\n\
سائيز_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% جو يادگيري, b 1, K 1024 (افتراضي), and so on لكل M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects ترتيب_ڏيو ترتيب.\n\
سيٽ_3 LC_ALL=C منزل get the traditional ترتيب_ڏيو ترتيب that استعمال_ڪري\n\
native byte قدرون.\n\
"), stdout );
      emit_ancillary_info (پروگرام_NAME);
    }

  ٻاهر_نڪرو (حالت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_پروگرام_OPTION,
  DEBUG_پروگرام_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_ذريعو_OPTION,
  ترتيب_ڏيو_OPTION,
  PARALLEL_OPTION
};

ساكن حرف ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ساكن بنية option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_دليل, عدم, 'b'},
  {"check", optional_دليل, عدم, CHECK_OPTION},
  {"compress-program", required_دليل, عدم, COMPRESS_پروگرام_OPTION},
  {"debug", no_دليل, عدم, DEBUG_پروگرام_OPTION},
  {"dictionary-order", no_دليل, عدم, 'd'},
  {"ignore-case", no_دليل, عدم, 'f'},
  {"files0-from", required_دليل, عدم, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_دليل, عدم, 'g'},
  {"ignore-nonprinting", no_دليل, عدم, 'i'},
  {"key", required_دليل, عدم, 'k'},
  {"merge", no_دليل, عدم, 'm'},
  {"month-sort", no_دليل, عدم, 'M'},
  {"numeric-sort", no_دليل, عدم, 'n'},
  {"human-numeric-sort", no_دليل, عدم, 'h'},
  {"version-sort", no_دليل, عدم, 'V'},
  {"random-sort", no_دليل, عدم, 'R'},
  {"random-source", required_دليل, عدم, RANDOM_ذريعو_OPTION},
  {"sort", required_دليل, عدم, ترتيب_ڏيو_OPTION},
  {"output", required_دليل, عدم, 'o'},
  {"reverse", no_دليل, عدم, 'r'},
  {"stable", no_دليل, عدم, 's'},
  {"batch-size", required_دليل, عدم, NMERGE_OPTION},
  {"buffer-size", required_دليل, عدم, 'S'},
  {"field-separator", required_دليل, عدم, 't'},
  {"temporary-directory", required_دليل, عدم, 'T'},
  {"unique", no_دليل, عدم, 'u'},
  {"zero-terminated", no_دليل, عدم, 'z'},
  {"parallel", required_دليل, عدم, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

#وصف_ڪريو CHECK_جدول \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ساكن حرف ثابت *ثابت check_دليلون[] =
{
#وصف_ڪريو _ct_(_s, _c) _s,
  CHECK_جدول عدم
#وصف_رد  _ct_
};
ساكن حرف ثابت check_types[] =
{
#وصف_ڪريو _ct_(_s, _c) _c,
  CHECK_جدول
#وصف_رد  _ct_
};

#وصف_ڪريو ترتيب_ڏيو_جدول \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ساكن حرف ثابت *ثابت ترتيب_ڏيو_دليلون[] =
{
#وصف_ڪريو _st_(_s, _c) _s,
  ترتيب_ڏيو_جدول عدم
#وصف_رد  _st_
};
ساكن حرف ثابت ترتيب_ڏيو_types[] =
{
#وصف_ڪريو _st_(_s, _c) _c,
  ترتيب_ڏيو_جدول
#وصف_رد  _st_
};

/* The set of signals that are caught.  */
ساكن sigset_t caught_سگنل;

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
  صحيح ret = pthread_sigmask (SIG_BLOCK, &caught_سگنل, &حالت->sigs);
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
  بنية tempnode *عارضي;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ساكن Hash_جدول *proctab;

تعداد { INIT_PROCTAB_سائيز = 47 };

ساكن size_t
proctab_hasher (فراغ ثابت *داخلا_2, size_t tabsize)
{
  بنية tempnode ثابت *ڳنڍ = داخلا_2;
  أرجع ڳنڍ->pid % tabsize;
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
    error (ترتيب_ڏيو_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_پروگرام));
  وإلا إذا (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      إذا (! WIFEXITED (حالت) || WEXITSTATUS (حالت))
        error (ترتيب_ڏيو_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_پروگرام));
      --nprocs;
    }

  أرجع cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساكن فراغ
رجسٽر_proc (بنية tempnode *عارضي)
{
  إذا (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_سائيز, عدم,
                                 proctab_hasher,
                                 proctab_comparator,
                                 عدم);
      إذا (! proctab)
        xalloc_die ();
    }

  عارضي->state = UNREAPED;

  إذا (! hash_وجهو (proctab, عارضي))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساكن bool
delete_proc (pid_t pid)
{
  بنية tempnode test;

  test.pid = pid;
  بنية tempnode *ڳنڍ = hash_remove (proctab, &test);
  إذا (! ڳنڍ)
    أرجع false;
  ڳنڍ->state = REAPED;
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
create_عارضي_file (صحيح *pfd, bool survive_fd_exhaustion)
{
  ساكن حرف ثابت slashbase[] = "/sortXXXXXX";
  ساكن idx_t عارضي_dir_اشاريو;
  صحيح fd;
  صحيح saved_errno;
  حرف ثابت *عارضي_dir = عارضي_dirs.dirs[عارضي_dir_اشاريو];
  size_t len = لڙي_ڊيگهه (عارضي_dir);
  بنية tempnode *ڳنڍ =
    xmalloc (FLEXSIZEOF (بنية tempnode, name, len + حجم slashbase));
  حرف *file = ڳنڍ->name;
  بنية cs_حالت cs;

  يادگيري_نقل (file, عارضي_dir, len);
  يادگيري_نقل (file + len, slashbase, حجم slashbase);
  ڳنڍ->ايندڙ = عدم;
  إذا (++عارضي_dir_اشاريو == عارضي_dirs.n_dirs)
    عارضي_dir_اشاريو = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  إذا (0 <= fd)
    {
      *temptail = ڳنڍ;
      temptail = &ڳنڍ->ايندڙ;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  إذا (fd < 0)
    {
      إذا (! (survive_fd_exhaustion && errno == EMFILE))
        error (ترتيب_ڏيو_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (عارضي_dir));
      حرر (ڳنڍ);
      ڳنڍ = عدم;
    }

  *pfd = fd;
  أرجع ڳنڍ;
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
وهڪرو_open (حرف ثابت *file, حرف ثابت *how)
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
          إذا (!outst || S_ISREG (outst->st_گهڻائي) || S_TYPEISSHM (outst))
            error (ترتيب_ڏيو_FAILURE, ftruncate_errno, _("%s: error truncating"),
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
  FILE *fp = وهڪرو_open (file, how);
  إذا (!fp)
    ترتيب_ڏيو_die (_("open failed"), file);
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
        ترتيب_ڏيو_die (_("fflush failed"), file);
      اكسر;

    افتراضي:
      إذا (fclose (fp) != 0)
        ترتيب_ڏيو_die (_("close failed"), file);
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
      ignore_قدر (dup2 (oldfd, newfd));
      ignore_قدر (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساكن صحيح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحيح oldfd, صحيح newfd)
{
  صحيح نتيجو = 0;
  إذا (oldfd != newfd)
    {
      نتيجو = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      إذا (نتيجو == 0)
        نتيجو = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  أرجع نتيجو;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساكن حرف ثابت *
get_resolved_compress_پروگرام (فراغ)
{
  /* Use a cache, to perform the search only once.  */
  ساكن حرف ثابت *resolved_compress_پروگرام_cache /* = NULL */;

  إذا (resolved_compress_پروگرام_cache == عدم)
    {
      resolved_compress_پروگرام_cache =
        ڳوليو_اندريون_given_رستو (compress_پروگرام, getenv ("PATH"), عدم, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  أرجع resolved_compress_پروگرام_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ساكن صحيح
pipe_ٻار (pid_t *pid, صحيح pipefds[2], صحيح tempfd, bool decompress,
            size_t tries)
{
  حرف ثابت *resolved_compress_پروگرام;
  بنية tempnode *saved_temphead;
  مزدوج wait_retry = 0.25;
  بنية cs_حالت cs;
  صحيح نتيجو;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_پروگرام = get_resolved_compress_پروگرام ();
  إذا (resolved_compress_پروگرام == عدم)
    أرجع errno;

  إذا ((نتيجو = posix_spawnattr_init (&attr)))
    أرجع نتيجو;
  إذا ((نتيجو = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (نتيجو = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      أرجع نتيجو;
    }

  إذا (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحيح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع saved_errno;
    }

  إذا ((نتيجو = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (نتيجو = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((نتيجو = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (نتيجو = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (نتيجو = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((نتيجو = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (نتيجو = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (نتيجو = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع نتيجو;
    }

  حرف ثابت *ثابت argv[] =
    {
      resolved_compress_پروگرام,
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

      نتيجو = posix_spawnp (pid, resolved_compress_پروگرام, &actions, &attr,
                             (حرف * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      إذا (نتيجو != EAGAIN)
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

  إذا (نتيجو)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  وإلا
    ++nprocs;

  أرجع نتيجو;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ساكن بنية tempnode *
maybe_create_عارضي (FILE **pfp, bool survive_fd_exhaustion)
{
  صحيح tempfd;
  بنية tempnode *ڳنڍ = create_عارضي_file (&tempfd, survive_fd_exhaustion);
  إذا (! ڳنڍ)
    أرجع عدم;

  ڳنڍ->state = UNCOMPRESSED;

  إذا (compress_پروگرام)
    {
      صحيح pipefds[2];
      ساكن صحيح آخري_نتيجو = 0;

      صحيح نتيجو = pipe_ٻار (&ڳنڍ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      إذا (نتيجو)
        {
          إذا (نتيجو != آخري_نتيجو)
            error (0, نتيجو, _("could not run compress program %s"),
                   quoteaf (compress_پروگرام));
        }
      وإلا
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          رجسٽر_proc (ڳنڍ);
        }

      آخري_نتيجو = نتيجو;
    }

  *pfp = fdopen (tempfd, "w");
  إذا (! *pfp)
    ترتيب_ڏيو_die (_("couldn't create temporary file"), ڳنڍ->name);

  أرجع ڳنڍ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساكن بنية tempnode *
create_عارضي (FILE **pfp)
{
  أرجع maybe_create_عارضي (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ساكن FILE *
open_عارضي (بنية tempnode *عارضي)
{
  صحيح tempfd, pipefds[2];
  FILE *fp = عدم;

  إذا (عارضي->state == UNREAPED)
    wait_proc (عارضي->pid);

  tempfd = open (عارضي->name, O_RDONLY);
  إذا (tempfd < 0)
    أرجع عدم;

  pid_t ٻار;
  صحيح نتيجو = pipe_ٻار (&ٻار, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  إذا (نتيجو)
    {
      إذا (نتيجو != EMFILE)
        error (ترتيب_ڏيو_FAILURE, نتيجو, _("could not run compress program %s -d"),
               quoteaf (compress_پروگرام));
      close (tempfd);
      errno = EMFILE;
    }
  وإلا
    {
      عارضي->pid = ٻار;
      رجسٽر_proc (عارضي);
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
جوڙيو_عارضي_dir (حرف ثابت *dir)
{
  إذا (عارضي_dirs.n_dirs == عارضي_dirs.n_dirs_alloc)
    عارضي_dirs.dirs = xpalloc (عارضي_dirs.dirs, &عارضي_dirs.n_dirs_alloc, 1, -1,
                              حجم *عارضي_dirs.dirs);

  idx_t ثابت dir_idx = عارضي_dirs.n_dirs++;
  عارضي_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ساكن فراغ
zaptemp (حرف ثابت *name)
{
  بنية tempnode *متطاير *pnode;
  بنية tempnode *ڳنڍ;
  بنية tempnode *ايندڙ;
  صحيح unlink_حالت;
  صحيح unlink_errno = 0;
  بنية cs_حالت cs;

  لكل (pnode = &temphead; (ڳنڍ = *pnode)->name != name; pnode = &ڳنڍ->ايندڙ)
    استمر;

  إذا (ڳنڍ->state == UNREAPED)
    wait_proc (ڳنڍ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  ايندڙ = ڳنڍ->ايندڙ;
  cs_enter (&cs);
  unlink_حالت = unlink (name);
  unlink_errno = errno;
  *pnode = ايندڙ;
  cs_leave (&cs);

  إذا (unlink_حالت != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  إذا (! ايندڙ)
    temptail = pnode;
  حرر (ڳنڍ);
}

#إذا HAVE_NL_LANGINFO

ساكن صحيح
struct_month_cmp (فراغ ثابت *m1, فراغ ثابت *m2)
{
  بنية month ثابت *month1 = m1;
  بنية month ثابت *month2 = m2;
  أرجع strcmp (month1->name, month2->name);
}

#جيڪڏهن_پورو

/* Initialize the character class tables. */

ساكن فراغ
inittables (فراغ)
{
  لكل (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! اکر_يا_انگ (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = وڏو_اکر (i);
    }

#إذا HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  إذا (hard_LC_وقت)
    {
      لكل (size_t i = 0; i < MONTHS_في_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = لڙي_ڊيگهه (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          لكل (j = k = 0; j < s_len; j++)
            إذا (! isblank (منزل_uchar (s[j])))
              name[k++] = fold_toupper[منزل_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_في_YEAR, حجم *monthtab, struct_month_cmp);
    }
#جيڪڏهن_پورو
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
  بنانشان صحيح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  إذا (e == LONGINT_OK)
    {
      nmerge = n;
      إذا (nmerge != n)
        e = LONGINT_حد_کان_مٿي;
      وإلا
        {
          إذا (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (ترتيب_ڏيو_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          وإلا إذا (max_nmerge < nmerge)
            {
              e = LONGINT_حد_کان_مٿي;
            }
          وإلا
            أرجع;
        }
    }

  إذا (e == LONGINT_حد_کان_مٿي)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (ترتيب_ڏيو_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  وإلا
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ساكن فراغ
specify_ترتيب_ڏيو_سائيز (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  حرف *suffix;
  تعداد strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  إذا (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_حد_کان_مٿي;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  إذا (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    بدل (suffix[0])
      {
      حالة 'b':
        e = LONGINT_OK;
        اكسر;

      حالة '%':
        {
          مزدوج mem = physmem_ڪل () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          إذا (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          وإلا
            e = LONGINT_حد_کان_مٿي;
        }
        اكسر;
      }

  إذا (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      إذا (n < ترتيب_ڏيو_سائيز)
        أرجع;

      ترتيب_ڏيو_سائيز = n;
      إذا (ترتيب_ڏيو_سائيز == n)
        {
          ترتيب_ڏيو_سائيز = MAX (ترتيب_ڏيو_سائيز, MIN_ترتيب_ڏيو_سائيز);
          أرجع;
        }

      e = LONGINT_حد_کان_مٿي;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ساكن size_t
specify_nthreads (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t nthreads;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &nthreads, "");
  إذا (e == LONGINT_حد_کان_مٿي)
    أرجع SIZE_MAX;
  إذا (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  إذا (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  إذا (nthreads == 0)
    error (ترتيب_ڏيو_FAILURE, 0, _("number in parallel must be nonzero"));
  أرجع nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ساكن size_t
default_ترتيب_ڏيو_سائيز (فراغ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t سائيز = SIZE_MAX;
  بنية rlimit rlimit;
  إذا (getrlimit (RLIMIT_معلومات, &rlimit) == 0 && rlimit.rlim_cur < سائيز)
    سائيز = rlimit.rlim_cur;
#جيڪڏهن_وصف_ٿيل RLIMIT_AS
  إذا (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < سائيز)
    سائيز = rlimit.rlim_cur;
#جيڪڏهن_پورو

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  سائيز /= 2;

#جيڪڏهن_وصف_ٿيل RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  إذا (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < سائيز)
    سائيز = rlimit.rlim_cur / 16 * 15;
#جيڪڏهن_پورو

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  مزدوج avail = physmem_موجود ();
  مزدوج ڪل = physmem_ڪل ();
  مزدوج mem = MAX (avail, ڪل / 8);

  /* Leave a 1/4 margin for physical memory.  */
  إذا (ڪل * 0.75 < سائيز)
    سائيز = ڪل * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  إذا (mem < سائيز)
    سائيز = mem;
  أرجع MAX (سائيز, MIN_ترتيب_ڏيو_سائيز);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساكن bool
سٽ_aligned_سائيز (size_t *alloc)
{
  size_t سائيز = *alloc;
  size_t alignment = حجم (بنية سٽ);
  size_t padding = alignment - سائيز % alignment;
  size_t aligned;

  إذا (ckd_جوڙيو (&aligned, سائيز, padding))
    أرجع false;

  *alloc = aligned;
  أرجع true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساكن size_t
default_initial_ترتيب_ڏيو_سائيز (size_t سٽ_بائيٽون)
{
  size_t سائيز = سٽ_بائيٽون + 2;
  size_t داخلا_سائيز;

  إذا (ckd_mul (&داخلا_سائيز, داخلا_FILE_سائيز_GUESS, سٽ_بائيٽون + 1)
      || ckd_جوڙيو (&سائيز, سائيز, داخلا_سائيز))
    أرجع SIZE_MAX;

  أرجع MAX (سائيز, MIN_ترتيب_ڏيو_سائيز);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساكن bool
داخلا_سائيز_بفر_بائيٽون (uintmax_t file_سائيز, size_t سٽ_بائيٽون,
                         size_t *worst_case)
{
  size_t worst_case_في_داخلا_byte = سٽ_بائيٽون + 1;

  إذا (SIZE_MAX / worst_case_في_داخلا_byte < file_سائيز)
    أرجع false;

  size_t سائيز = file_سائيز * worst_case_في_داخلا_byte;
  إذا (SIZE_MAX - سائيز < 1)
    أرجع false;

  *worst_case = سائيز + 1;
  أرجع true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ساكن فراغ
ترتيب_ڏيو_بفر_policy (FILE *ثابت *fps, size_t nfps,
                    حرف *ثابت *files, size_t nfiles,
                    size_t سٽ_بائيٽون,
                    بنية ترتيب_ڏيو_بفر_policy *policy)
{
  size_t سائيز_bound = ترتيب_ڏيو_سائيز ? ترتيب_ڏيو_سائيز : default_ترتيب_ڏيو_سائيز ();
  size_t initial_bound =
    ترتيب_ڏيو_سائيز ? سائيز_bound : default_initial_ترتيب_ڏيو_سائيز (سٽ_بائيٽون);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_في_داخلا_byte = سٽ_بائيٽون + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t سائيز = worst_case_في_داخلا_byte + 1;

  initial_bound = MIN (initial_bound, سائيز_bound);
  policy->حد_بندي = سائيز_bound;
  policy->growth_failed = false;

  لكل (size_t i = 0; i < nfiles; i++)
    {
      بنية stat st;
      off_t file_سائيز;
      size_t worst_case;
      bool معلوم_سائيز;

      إذا (سائيز_bound <= سائيز)
        {
          policy->initial = سائيز_bound;
          أرجع;
        }

      إذا ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        ترتيب_ڏيو_die (_("stat failed"), files[i]);

      معلوم_سائيز = usable_st_سائيز (&st) && 0 < st.st_سائيز;
      إذا (معلوم_سائيز)
        file_سائيز = st.st_سائيز;
      وإلا
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          إذا (ترتيب_ڏيو_سائيز)
            {
              policy->initial = سائيز_bound;
              أرجع;
            }
          file_سائيز = داخلا_FILE_سائيز_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      إذا (! داخلا_سائيز_بفر_بائيٽون (file_سائيز, سٽ_بائيٽون, &worst_case)
          || سائيز_bound - سائيز <= worst_case)
        {
          policy->initial = سائيز_bound;
          أرجع;
        }

      إذا (! معلوم_سائيز && ! ترتيب_ڏيو_سائيز
          && (initial_bound <= سائيز || initial_bound - سائيز <= worst_case))
        {
          policy->initial = MAX (سائيز, initial_bound);
          أرجع;
        }

      سائيز += worst_case;
    }

  policy->initial = MAX (سائيز, MIN_ترتيب_ڏيو_سائيز);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساكن فراغ
initbuf (بنية بفر *buf, size_t سٽ_بائيٽون, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  طالما (true)
    {
      إذا (! سٽ_aligned_سائيز (&alloc))
        buf->buf = عدم;
      وإلا
        buf->buf = يادگيري_ڏيو (alloc);
      إذا (buf->buf)
        اكسر;
      alloc /= 2;
      إذا (alloc <= سٽ_بائيٽون + 1)
        xalloc_die ();
    }

  buf->سٽ_بائيٽون = سٽ_بائيٽون;
  buf->alloc = alloc;
  buf->استعمال_ٿيل = buf->کاٻو = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساكن مضمن بنية سٽ *
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
  إذا (! سٽ_aligned_سائيز (&alloc) || alloc <= buf->alloc)
    أرجع false;

  حرف *newbuf = يادگيري_ڏيو (alloc);
  إذا (! newbuf)
    أرجع false;

  حرف *oldbuf = buf->buf;
  بنية سٽ *old_linelim = بفر_linelim (buf);
  بنية سٽ *old_سٽ = old_linelim - buf->nlines;

  يادگيري_نقل (newbuf, oldbuf, buf->استعمال_ٿيل);

  بنية سٽ *new_linelim = (فراغ *) (newbuf + alloc);
  بنية سٽ *new_سٽ = new_linelim - buf->nlines;
  يادگيري_نقل (new_سٽ, old_سٽ, buf->nlines * حجم *new_سٽ);

  لكل (بنية سٽ *سٽ = new_سٽ; سٽ < new_linelim; سٽ++)
    {
      سٽ->متن = newbuf + (سٽ->متن - oldbuf);
      إذا (keylist)
        {
          سٽ->keybeg = newbuf + (سٽ->keybeg - oldbuf);
          سٽ->keylim = newbuf + (سٽ->keylim - oldbuf);
        }
    }

  حرر (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  أرجع true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساكن bool
maybe_growbuf (بنية بفر *buf, بنية ترتيب_ڏيو_بفر_policy *policy)
{
  إذا (! policy || policy->growth_failed || policy->حد_بندي <= buf->alloc)
    أرجع false;

  size_t alloc;
  إذا (buf->alloc <= policy->حد_بندي / 3)
    alloc = buf->alloc * 3;
  وإلا
    alloc = policy->حد_بندي;

  إذا (try_growbuf (buf, alloc))
    أرجع true;

  policy->growth_failed = true;
  أرجع false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ساكن حرف *
begfield (بنية سٽ ثابت *سٽ, بنية keyfield ثابت *ڪنجي)
{
  حرف *ptr = سٽ->متن, *lim = ptr + سٽ->ڊيگهائي - 1;
  size_t sword = ڪنجي->sword;
  size_t schar = ڪنجي->schar;

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
  إذا (ڪنجي->skipsblanks)
    طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t بچيل_بائيٽون = lim - ptr;
  إذا (schar < بچيل_بائيٽون)
    ptr += schar;
  وإلا
    ptr = lim;

  أرجع ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_خالص
ساكن حرف *
limfield (بنية سٽ ثابت *سٽ, بنية keyfield ثابت *ڪنجي)
{
  حرف *ptr = سٽ->متن, *lim = ptr + سٽ->ڊيگهائي - 1;
  size_t eword = ڪنجي->eword, echar = ڪنجي->echar;

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

#جيڪڏهن_وصف_ٿيل POSIX_UNSPECIFIED
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
#جيڪڏهن_پورو

  إذا (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      إذا (ڪنجي->skipeblanks)
        طالما (ptr < lim && blanks[منزل_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t بچيل_بائيٽون = lim - ptr;
      إذا (echar < بچيل_بائيٽون)
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
         بنية ترتيب_ڏيو_بفر_policy *policy)
{
  بنية keyfield ثابت *ڪنجي = keylist;
  حرف eol = eolchar;
  size_t سٽ_بائيٽون = buf->سٽ_بائيٽون;
  size_t mergesize = ملايو_بفر_سائيز - MIN_ملايو_بفر_سائيز;

  إذا (buf->eof)
    أرجع false;

  إذا (buf->استعمال_ٿيل != buf->کاٻو)
    {
      memmove (buf->buf, buf->buf + buf->استعمال_ٿيل - buf->کاٻو, buf->کاٻو);
      buf->استعمال_ٿيل = buf->کاٻو;
      buf->nlines = 0;
    }

  طالما (true)
    {
      حرف *ptr = buf->buf + buf->استعمال_ٿيل;
      بنية سٽ *linelim = بفر_linelim (buf);
      بنية سٽ *سٽ = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * سٽ_بائيٽون - ptr;
      حرف *سٽ_شروعات = buf->nlines ? سٽ->متن + سٽ->ڊيگهائي : buf->buf;

      طالما (سٽ_بائيٽون + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (سٽ_بائيٽون + 1);
          size_t بائيٽون_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + بائيٽون_read;
          حرف *p;
          avail -= بائيٽون_read;

          إذا (بائيٽون_read != readsize)
            {
              إذا (ferror (fp))
                ترتيب_ڏيو_die (_("read failed"), file);
              إذا (feof (fp))
                {
                  buf->eof = true;
                  إذا (buf->buf == ptrlim)
                    أرجع false;
                  إذا (سٽ_شروعات != ptrlim && ptrlim[-1] != eol)
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
              سٽ--;
              سٽ->متن = سٽ_شروعات;
              سٽ->ڊيگهائي = ptr - سٽ_شروعات;
              mergesize = MAX (mergesize, سٽ->ڊيگهائي);
              avail -= سٽ_بائيٽون;

              إذا (ڪنجي)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  سٽ->keylim = (ڪنجي->eword == SIZE_MAX
                                  ? p
                                  : limfield (سٽ, ڪنجي));

                  إذا (ڪنجي->sword != SIZE_MAX)
                    سٽ->keybeg = begfield (سٽ, ڪنجي);
                  وإلا
                    {
                      إذا (ڪنجي->skipsblanks)
                        طالما (blanks[منزل_uchar (*سٽ_شروعات)])
                          سٽ_شروعات++;
                      سٽ->keybeg = سٽ_شروعات;
                    }
                }

              سٽ_شروعات = ptr;
            }

          ptr = ptrlim;
          إذا (buf->eof)
            اكسر;
        }

      buf->استعمال_ٿيل = ptr - buf->buf;
      buf->nlines = بفر_linelim (buf) - سٽ;
      إذا (buf->nlines != 0)
        {
          إذا (! buf->eof && maybe_growbuf (buf, policy))
            استمر;
          buf->کاٻو = ptr - سٽ_شروعات;
          ملايو_بفر_سائيز = mergesize + MIN_ملايو_بفر_سائيز;
          أرجع true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t سٽ_alloc = buf->alloc / حجم (بنية سٽ);
        buf->buf = xpalloc (buf->buf, &سٽ_alloc, 1, -1, حجم (بنية سٽ));
        buf->alloc = سٽ_alloc * حجم (بنية سٽ);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ساكن حرف ثابت unit_ترتيب[UCHAR_LIM] =
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
#جيڪڏهن_پورو
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ساكن حرف
traverse_raw_عدد (حرف ثابت **عدد)
{
  حرف ثابت *p = *عدد;
  حرف ch;
  حرف max_digit = '\0';
  bool پڇاڙيون_with_thousands_sep = false;

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
      پڇاڙيون_with_thousands_sep = (*p == thousands_sep);
      إذا (پڇاڙيون_with_thousands_sep)
        ++p;
    }

  إذا (پڇاڙيون_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *عدد = p - 2;
      أرجع max_digit;
    }

  إذا (ch == decimal_نقطو)
    طالما (c_isdigit (ch = *p++))
      إذا (max_digit < ch)
        max_digit = ch;

  *عدد = p - 1;
  أرجع max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_خالص
ساكن صحيح
ڳوليو_unit_ترتيب (حرف ثابت *عدد)
{
  bool minus_sign = (*عدد == '-');
  حرف ثابت *p = عدد + minus_sign;
  حرف max_digit = traverse_raw_عدد (&p);
  إذا ('0' < max_digit)
    {
      بنانشان حرف ch = *p;
      صحيح ترتيب = unit_ترتيب[ch];
      أرجع (minus_sign ? -ترتيب : ترتيب);
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

  صحيح diff = ڳوليو_unit_ترتيب (a) - ڳوليو_unit_ترتيب (b);
  أرجع (diff ? diff : strnumcmp (a, b, decimal_نقطو, thousands_sep));
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

  أرجع strnumcmp (a, b, decimal_نقطو, thousands_sep);
}

ساكن صحيح
nan_compare (ڊگهو مزدوج a, ڊگهو مزدوج b)
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
  ڊگهو مزدوج a = strtold (sa, &ea);
  ڊگهو مزدوج b = strtold (sb, &eb);

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
  size_t hi = MONTHS_في_YEAR;

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
#  pragma GCC diagnostic ڌڪو
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#جيڪڏهن_پورو

#ضمن "md5.h"

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# إذا 14 <= __GNUC__
#  pragma GCC diagnostic ڪڍو
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_ترجيح_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ساكن فراغ
link_failure (فراغ)
{
  error (ترتيب_ڏيو_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ساكن فراغ *
نشان_پتو (فراغ *handle, حرف ثابت *نشان)
{
  فراغ *پتو = dlsym (handle, نشان);
  إذا (!پتو)
    link_failure ();
  أرجع پتو;
}
#جيڪڏهن_پورو

/* Dynamically link the crypto library, if it needs linking.  */
ساكن فراغ
link_libcrypto (فراغ)
{
#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  فراغ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  إذا (!handle)
    link_failure ();
  ptr_MD5_Init = نشان_پتو (handle, "MD5_Init");
  ptr_MD5_Update = نشان_پتو (handle, "MD5_Update");
  ptr_MD5_Final = نشان_پتو (handle, "MD5_Final");
#جيڪڏهن_پورو
}

/* A randomly chosen MD5 state, used for random comparison.  */
ساكن بنية md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ساكن فراغ
random_md5_state_init (حرف ثابت *random_ذريعو)
{
  بنانشان حرف buf[MD5_DIGEST_سائيز];
  بنية randread_ذريعو *r = randread_new (random_ذريعو, حجم buf);
  إذا (! r)
    ترتيب_ڏيو_die (_("open failed"), random_ذريعو ? random_ذريعو : "getrandom");
  randread (r, buf, حجم buf);
  إذا (randread_free (r) != 0)
    ترتيب_ڏيو_die (_("close failed"), random_ذريعو);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_بائيٽون (buf, حجم buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ساكن size_t
xstrxfrm (حرف *محدود dest, حرف ثابت *محدود src, size_t destsize)
{
  errno = 0;
  size_t translated_سائيز = strxfrm (dest, src, destsize);

  إذا (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (ترتيب_ڏيو_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  أرجع translated_سائيز;
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
  uint32_t dig[2][MD5_DIGEST_سائيز / حجم (uint32_t)];
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
              buf = allocated = يادگيري_ڏيو (bufsize);
              إذا (! buf)
                {
                  buf = stackbuf;
                  bufsize = حجم stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_سمائي = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_سمائي ? buf + sizea : عدم), textb,
                          (a_سمائي ? bufsize - sizea : 0))
                + 1)
             : 0);

          إذا (! (a_سمائي && sizea + sizeb <= bufsize))
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
            texta += لڙي_ڊيگهه (texta) + 1;
          إذا (textb < limb)
            textb += لڙي_ڊيگهه (textb) + 1;
          إذا (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              اكسر;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_بائيٽون (buf, sizea, &s[0]);
          md5_process_بائيٽون (buf + sizea, sizeb, &s[1]);

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
  md5_process_بائيٽون (texta, lena, &s[0]); md5_پڄاڻي_ctx (&s[0], dig[0]);
  md5_process_بائيٽون (textb, lenb, &s[1]); md5_پڄاڻي_ctx (&s[1], dig[1]);
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
debug_ويڪر (حرف ثابت *متن, حرف ثابت *lim)
{
  size_t ويڪر = mbsnwidth (متن, lim - متن, 0);
  طالما (متن < lim)
    ويڪر += (*متن++ == '\t');
  أرجع ويڪر;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساكن فراغ
mark_ڪنجي (size_t وٿي, size_t ويڪر)
{
  طالما (وٿي--)
    اکر_لکو (' ');

  إذا (!ويڪر)
    لکو (_("^ no match for key\n"));
  وإلا
    {
      افعل
        اکر_لکو ('_');
      طالما (--ويڪر);

      اکر_لکو ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساكن مضمن bool
ڪنجي_numeric (بنية keyfield ثابت *ڪنجي)
{
  أرجع ڪنجي->numeric || ڪنجي->general_numeric || ڪنجي->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساكن فراغ
debug_ڪنجي (بنية سٽ ثابت *سٽ, بنية keyfield ثابت *ڪنجي)
{
  حرف *متن = سٽ->متن;
  حرف *beg = متن;
  حرف *lim = متن + سٽ->ڊيگهائي - 1;

  إذا (ڪنجي)
    {
      إذا (ڪنجي->sword != SIZE_MAX)
        beg = begfield (سٽ, ڪنجي);
      إذا (ڪنجي->eword != SIZE_MAX)
        {
          lim = limfield (سٽ, ڪنجي);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      إذا ((ڪنجي->skipsblanks && ڪنجي->sword == SIZE_MAX)
          || ڪنجي->month || ڪنجي_numeric (ڪنجي))
        {
          حرف saved = *lim;
          *lim = '\0';

          طالما (blanks[منزل_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          إذا (lim < beg)
            tighter_lim = lim;
          وإلا إذا (ڪنجي->month)
            getmonth (beg, &tighter_lim);
          وإلا إذا (ڪنجي->general_numeric)
            ignore_قدر (strtold (beg, &tighter_lim));
          وإلا إذا (ڪنجي->numeric || ڪنجي->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_عدد (&p);
              إذا ('0' <= max_digit)
                {
                  بنانشان حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (ڪنجي->human_numeric && unit_ترتيب[ch]);
                }
            }
          وإلا
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t وٿي = debug_ويڪر (متن, beg);
  size_t ويڪر = debug_ويڪر (beg, lim);
  mark_ڪنجي (وٿي, ويڪر);
}

/* Debug LINE by underlining its keys.  */

ساكن فراغ
debug_سٽ (بنية سٽ ثابت *سٽ)
{
  بنية keyfield ثابت *ڪنجي = keylist;

  افعل
    debug_ڪنجي (سٽ, ڪنجي);
  طالما (ڪنجي && ((ڪنجي = ڪنجي->ايندڙ) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساكن bool
default_ڪنجي_compare (بنية keyfield ثابت *ڪنجي)
{
  أرجع ! (ڪنجي->ignore
            || ڪنجي->translate
            || ڪنجي->skipsblanks
            || ڪنجي->skipeblanks
            || ڪنجي_numeric (ڪنجي)
            || ڪنجي->month
            || ڪنجي->نسخو_3
            || ڪنجي->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساكن فراغ
ڪنجي_منزل_opts (بنية keyfield ثابت *ڪنجي, حرف *opts)
{
  إذا (ڪنجي->skipsblanks || ڪنجي->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  إذا (ڪنجي->ignore == nondictionary)
    *opts++ = 'd';
  إذا (ڪنجي->translate)
    *opts++ = 'f';
  إذا (ڪنجي->general_numeric)
    *opts++ = 'g';
  إذا (ڪنجي->human_numeric)
    *opts++ = 'h';
  إذا (ڪنجي->ignore == nonprinting)
    *opts++ = 'i';
  إذا (ڪنجي->month)
    *opts++ = 'M';
  إذا (ڪنجي->numeric)
    *opts++ = 'n';
  إذا (ڪنجي->random)
    *opts++ = 'R';
  إذا (ڪنجي->ابتو)
    *opts++ = 'r';
  إذا (ڪنجي->نسخو_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساكن فراغ
ڪنجي_warnings (بنية keyfield ثابت *gkey, bool gkey_only)
{
  بنية keyfield ugkey = *gkey;
  بنانشان ڊگهو keynum = 1;
  bool basic_numeric_ميدان = false;
  bool general_numeric_ميدان = false;
  bool basic_numeric_ميدان_وسعت = false;
  bool general_numeric_ميدان_وسعت = false;

  لكل (بنية keyfield *ڪنجي = keylist; ڪنجي; ڪنجي = ڪنجي->ايندڙ, keynum++)
    {
      إذا (ڪنجي_numeric (ڪنجي))
        {
          إذا (ڪنجي->general_numeric)
            general_numeric_ميدان = true;
          وإلا
            basic_numeric_ميدان = true;
        }

      إذا (ڪنجي->traditional_استعمال_ٿيل)
        {
          size_t sword = ڪنجي->sword;
          size_t eword = ڪنجي->eword;
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
          إذا (ڪنجي->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ڪنجي->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_ويڪر = ڪنجي->sword != SIZE_MAX && ڪنجي->eword < ڪنجي->sword;
      إذا (zero_ويڪر)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ڪنجي_numeric (ڪنجي) || ڪنجي->month;
      bool سٽ_وٿي = ڪنجي->eword == 0 && ڪنجي->echar != 0; /* -k1.x,1.y  */
      إذا (!zero_ويڪر && !gkey_only && tab == TAB_DEFAULT && !سٽ_وٿي
          && ((!ڪنجي->skipsblanks && !implicit_skip)
              || (!ڪنجي->skipsblanks && ڪنجي->schar)
              || (!ڪنجي->skipeblanks && ڪنجي->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      إذا (!gkey_only && ڪنجي_numeric (ڪنجي))
        {
          size_t sword = ڪنجي->sword + 1;
          size_t eword = ڪنجي->eword + 1;
          إذا (!sword)
            sword++;
          إذا (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              إذا (ڪنجي->general_numeric)
                general_numeric_ميدان_وسعت = true;
              وإلا
                basic_numeric_ميدان_وسعت = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      إذا (ugkey.ignore && (ugkey.ignore == ڪنجي->ignore))
        ugkey.ignore = عدم;
      إذا (ugkey.translate && (ugkey.translate == ڪنجي->translate))
        ugkey.translate = عدم;
      ugkey.skipsblanks &= !ڪنجي->skipsblanks;
      ugkey.skipeblanks &= !ڪنجي->skipeblanks;
      ugkey.month &= !ڪنجي->month;
      ugkey.numeric &= !ڪنجي->numeric;
      ugkey.general_numeric &= !ڪنجي->general_numeric;
      ugkey.human_numeric &= !ڪنجي->human_numeric;
      ugkey.random &= !ڪنجي->random;
      ugkey.نسخو_3 &= !ڪنجي->نسخو_3;
      ugkey.ابتو &= !ڪنجي->ابتو;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool عدد_locale_warned = false;
  إذا (basic_numeric_ميدان_وسعت)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (منزل_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          عدد_locale_warned = true;
        }
    }
  إذا (basic_numeric_ميدان_وسعت || general_numeric_ميدان_وسعت)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (منزل_uchar (decimal_نقطو)))
          : tab == decimal_نقطو)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_نقطو, 0})));
          عدد_locale_warned = true;
        }
      وإلا إذا (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      وإلا إذا (general_numeric_ميدان_وسعت && tab == '+')
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
  إذا ((basic_numeric_ميدان || general_numeric_ميدان) && ! عدد_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_نقطو, 0})));

    }

  إذا (basic_numeric_ميدان && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  إذا (!default_ڪنجي_compare (&ugkey)
      || (ugkey.ابتو && (stable || unique) && keylist))
    {
      bool ugkey_ابتو = ugkey.ابتو;
      إذا (!(stable || unique))
        ugkey.ابتو = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      ڪنجي_منزل_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (لڙي_ڊيگهه (opts))), opts);
      ugkey.ابتو = ugkey_ابتو;
    }
  إذا (ugkey.ابتو && !(stable || unique) && keylist)
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
keycompare (بنية سٽ ثابت *a, بنية سٽ ثابت *b)
{
  بنية keyfield *ڪنجي = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحيح diff;

  طالما (true)
    {
      حرف ثابت *translate = ڪنجي->translate;
      bool ثابت *ignore = ڪنجي->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      إذا (hard_LC_COLLATE || ڪنجي_numeric (ڪنجي)
          || ڪنجي->month || ڪنجي->random || ڪنجي->نسخو_3)
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
              size_t سائيز = lena + 1 + lenb + 1;
              إذا (سائيز <= حجم stackbuf)
                ta = stackbuf;
              وإلا
                ta = allocated = xmalloc (سائيز);
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

          إذا (ڪنجي->numeric)
            diff = numcompare (ta, tb);
          وإلا إذا (ڪنجي->general_numeric)
            diff = general_numcompare (ta, tb);
          وإلا إذا (ڪنجي->human_numeric)
            diff = human_numcompare (ta, tb);
          وإلا إذا (ڪنجي->month)
            diff = getmonth (ta, عدم) - getmonth (tb, عدم);
          وإلا إذا (ڪنجي->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          وإلا إذا (ڪنجي->نسخو_3)
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
#وصف_ڪريو CMP_WITH_IGNORE(A, B)						\
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

      ڪنجي = ڪنجي->ايندڙ;
      إذا (! ڪنجي)
        أرجع 0;

      /* Find the beginning and limit of the next field.  */
      إذا (ڪنجي->eword != SIZE_MAX)
        lima = limfield (a, ڪنجي), limb = limfield (b, ڪنجي);
      وإلا
        lima = a->متن + a->ڊيگهائي - 1, limb = b->متن + b->ڊيگهائي - 1;

      إذا (ڪنجي->sword != SIZE_MAX)
        texta = begfield (a, ڪنجي), textb = begfield (b, ڪنجي);
      وإلا
        {
          texta = a->متن, textb = b->متن;
          إذا (ڪنجي->skipsblanks)
            {
              طالما (texta < lima && blanks[منزل_uchar (*texta)])
                ++texta;
              طالما (textb < limb && blanks[منزل_uchar (*textb)])
                ++textb;
            }
        }
    }

  أرجع diff_reversed (diff, ڪنجي->ابتو);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساكن صحيح
compare (بنية سٽ ثابت *a, بنية سٽ ثابت *b)
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
  alen = a->ڊيگهائي - 1, blen = b->ڊيگهائي - 1;

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

  أرجع diff_reversed (diff, ابتو);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساكن فراغ
write_سٽ (بنية سٽ ثابت *سٽ, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = سٽ->متن;
  size_t n_بائيٽون = سٽ->ڊيگهائي;
  حرف *ebuf = buf + n_بائيٽون;

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
            ترتيب_ڏيو_die (_("write failed"), output_file);
        }

      debug_سٽ (سٽ);
    }
  وإلا
    {
      ebuf[-1] = eolchar;
      إذا (fwrite (buf, 1, n_بائيٽون, fp) != n_بائيٽون)
        ترتيب_ڏيو_die (_("write failed"), output_file);
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
  بنية سٽ عارضي;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t سٽ_عدد = 0;
  بنية keyfield ثابت *ڪنجي = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (بنية سٽ),
           MAX (ملايو_بفر_سائيز, ترتيب_ڏيو_سائيز));
  عارضي.متن = عدم;

  طالما (fillbuf (&buf, fp, file_name, عدم))
    {
      بنية سٽ ثابت *سٽ = بفر_linelim (&buf);
      بنية سٽ ثابت *linebase = سٽ - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      إذا (alloc && nonunique <= compare (&عارضي, سٽ - 1))
        {
        مليو_disorder:
          {
            إذا (checkonly == 'c')
              {
                بنية سٽ ثابت *disorder_سٽ = سٽ - 1;
                uintmax_t disorder_سٽ_عدد =
                  بفر_linelim (&buf) - disorder_سٽ + سٽ_عدد;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         پروگرام_name, file_name, disorder_سٽ_عدد);
                write_سٽ (disorder_سٽ, stderr, _("standard error"));
              }

            ordered = false;
            اكسر;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      طالما (linebase < --سٽ)
        إذا (nonunique <= compare (سٽ, سٽ - 1))
          اذهب مليو_disorder;

      سٽ_عدد += buf.nlines;

      /* Save the last line of the buffer.  */
      إذا (alloc < سٽ->ڊيگهائي)
        {
          افعل
            {
              alloc *= 2;
              إذا (! alloc)
                {
                  alloc = سٽ->ڊيگهائي;
                  اكسر;
                }
            }
          طالما (alloc < سٽ->ڊيگهائي);

          حرر (عارضي.متن);
          عارضي.متن = xmalloc (alloc);
        }
      يادگيري_نقل (عارضي.متن, سٽ->متن, سٽ->ڊيگهائي);
      عارضي.ڊيگهائي = سٽ->ڊيگهائي;
      إذا (ڪنجي)
        {
          عارضي.keybeg = عارضي.متن + (سٽ->keybeg - سٽ->متن);
          عارضي.keylim = عارضي.متن + (سٽ->keylim - سٽ->متن);
        }
    }

  xfclose (fp, file_name);
  حرر (buf.buf);
  حرر (عارضي.متن);
  أرجع ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساكن size_t
open_داخلا_files (بنية sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحيح i;

  /* Open as many input files as we can.  */
  لكل (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].عارضي && files[i].عارضي->state != UNCOMPRESSED
                ? open_عارضي (files[i].عارضي)
                : وهڪرو_open (files[i].name, "r"));
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
  بنية سٽ saved;		/* Saved line storage for unique check. */
  بنية سٽ ثابت *savedline = عدم;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  بنية سٽ ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  بنية سٽ ثابت **بنياد_2 = xnmalloc (nfiles, حجم *بنياد_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  بنية keyfield ثابت *ڪنجي = keylist;
  saved.متن = عدم;

  /* Read initial lines from each input file. */
  لكل (size_t i = 0; i < nfiles; )
    {
      initbuf (&بفر[i], حجم (بنية سٽ),
               MAX (ملايو_بفر_سائيز, ترتيب_ڏيو_سائيز / nfiles));
      إذا (fillbuf (&بفر[i], fps[i], files[i].name, عدم))
        {
          بنية سٽ ثابت *linelim = بفر_linelim (&بفر[i]);
          cur[i] = linelim - 1;
          بنياد_2[i] = linelim - بفر[i].nlines;
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
      بنية سٽ ثابت *سڀ_کان_ننڍو = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      إذا (unique)
        {
          إذا (savedline && compare (savedline, سڀ_کان_ننڍو))
            {
              savedline = عدم;
              write_سٽ (&saved, ofp, output_file);
            }
          إذا (!savedline)
            {
              savedline = &saved;
              إذا (savealloc < سڀ_کان_ننڍو->ڊيگهائي)
                {
                  افعل
                    إذا (! savealloc)
                      {
                        savealloc = سڀ_کان_ننڍو->ڊيگهائي;
                        اكسر;
                      }
                  طالما ((savealloc *= 2) < سڀ_کان_ننڍو->ڊيگهائي);

                  حرر (saved.متن);
                  saved.متن = xmalloc (savealloc);
                }
              saved.ڊيگهائي = سڀ_کان_ننڍو->ڊيگهائي;
              يادگيري_نقل (saved.متن, سڀ_کان_ننڍو->متن, saved.ڊيگهائي);
              إذا (ڪنجي)
                {
                  saved.keybeg =
                    saved.متن + (سڀ_کان_ننڍو->keybeg - سڀ_کان_ننڍو->متن);
                  saved.keylim =
                    saved.متن + (سڀ_کان_ننڍو->keylim - سڀ_کان_ننڍو->متن);
                }
            }
        }
      وإلا
        write_سٽ (سڀ_کان_ننڍو, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      إذا (بنياد_2[ord[0]] < سڀ_کان_ننڍو)
        cur[ord[0]] = سڀ_کان_ننڍو - 1;
      وإلا
        {
          إذا (fillbuf (&بفر[ord[0]], fps[ord[0]], files[ord[0]].name,
                       عدم))
            {
              بنية سٽ ثابت *linelim = بفر_linelim (&بفر[ord[0]]);
              cur[ord[0]] = linelim - 1;
              بنياد_2[ord[0]] = linelim - بفر[ord[0]].nlines;
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
                  بنياد_2[i] = بنياد_2[i + 1];
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
        size_t ڳڻپ_جو_smaller_سٽون;

        طالما (lo < hi)
          {
            صحيح cmp = compare (cur[ord0], cur[ord[probe]]);
            إذا (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            وإلا
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ڳڻپ_جو_smaller_سٽون = lo - 1;
        لكل (size_t j = 0; j < ڳڻپ_جو_smaller_سٽون; j++)
          ord[j] = ord[j + 1];
        ord[ڳڻپ_جو_smaller_سٽون] = ord0;
      }
    }

  إذا (unique && savedline)
    {
      write_سٽ (&saved, ofp, output_file);
      حرر (saved.متن);
    }

  xfclose (ofp, output_file);
  حرر (fps);
  حرر (بفر);
  حرر (ord);
  حرر (بنياد_2);
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
  size_t nopened = open_داخلا_files (files, nfiles, &fps);
  إذا (nopened < nfiles && nopened < 2)
    ترتيب_ڏيو_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  أرجع nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساكن فراغ
mergelines (بنية سٽ *محدود t, size_t nlines,
            بنية سٽ ثابت *محدود lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية سٽ *hi = t - nlo;

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
sequential_ترتيب_ڏيو (بنية سٽ *محدود سٽون, size_t nlines,
                 بنية سٽ *محدود عارضي, bool منزل_عارضي)
{
  إذا (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحيح مٽا_سٽا = (0 < compare (&سٽون[-1], &سٽون[-2]));
      إذا (منزل_عارضي)
        {
          عارضي[-1] = سٽون[-1 - مٽا_سٽا];
          عارضي[-2] = سٽون[-2 + مٽا_سٽا];
        }
      وإلا إذا (مٽا_سٽا)
        {
          عارضي[-1] = سٽون[-1];
          سٽون[-1] = سٽون[-2];
          سٽون[-2] = عارضي[-1];
        }
    }
  وإلا
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      بنية سٽ *lo = سٽون;
      بنية سٽ *hi = سٽون - nlo;

      sequential_ترتيب_ڏيو (hi, nhi, عارضي - (منزل_عارضي ? nlo : 0), منزل_عارضي);
      إذا (1 < nlo)
        sequential_ترتيب_ڏيو (lo, nlo, عارضي, !منزل_عارضي);
      وإلا إذا (!منزل_عارضي)
        عارضي[-1] = lo[-1];

      بنية سٽ *dest;
      بنية سٽ ثابت *sorted_lo;
      إذا (منزل_عارضي)
        {
          dest = عارضي;
          sorted_lo = سٽون;
        }
      وإلا
        {
          dest = سٽون;
          sorted_lo = عارضي;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساكن بنية ملايو_ڳنڍ *init_ڳنڍ (بنية ملايو_ڳنڍ *محدود,
                                     بنية ملايو_ڳنڍ *محدود,
                                     بنية سٽ *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساكن بنية ملايو_ڳنڍ *
ملايو_وڻ_init (size_t nthreads, size_t nlines, بنية سٽ *dest)
{
  بنية ملايو_ڳنڍ *ملايو_وڻ = xmalloc (2 * حجم *ملايو_وڻ * nthreads);

  بنية ملايو_ڳنڍ *پاڙ = ملايو_وڻ;
  پاڙ->lo = پاڙ->hi = پاڙ->پڇاڙي_lo = پاڙ->پڇاڙي_hi = عدم;
  پاڙ->dest = عدم;
  پاڙ->nlo = پاڙ->nhi = nlines;
  پاڙ->والد = عدم;
  پاڙ->level = ملايو_پڇاڙي;
  پاڙ->queued = false;
  pthread_mutex_init (&پاڙ->تالو, عدم);

  init_ڳنڍ (پاڙ, پاڙ + 1, dest, nthreads, nlines, false);
  أرجع ملايو_وڻ;
}

/* Destroy the merge tree. */
ساكن فراغ
ملايو_وڻ_destroy (size_t nthreads, بنية ملايو_ڳنڍ *ملايو_وڻ)
{
  size_t n_nodes = nthreads * 2;
  بنية ملايو_ڳنڍ *ڳنڍ = ملايو_وڻ;

  طالما (n_nodes--)
    {
      pthread_mutex_destroy (&ڳنڍ->تالو);
      ڳنڍ++;
    }

  حرر (ملايو_وڻ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساكن بنية ملايو_ڳنڍ *
init_ڳنڍ (بنية ملايو_ڳنڍ *محدود والد,
           بنية ملايو_ڳنڍ *محدود ڳنڍ_pool,
           بنية سٽ *dest, size_t nthreads,
           size_t ڪل_سٽون, bool is_lo_ٻار)
{
  size_t nlines = (is_lo_ٻار ? والد->nlo : والد->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية سٽ *lo = dest - ڪل_سٽون;
  بنية سٽ *hi = lo - nlo;
  بنية سٽ **والد_پڇاڙي = (is_lo_ٻار ? &والد->پڇاڙي_lo : &والد->پڇاڙي_hi);

  بنية ملايو_ڳنڍ *ڳنڍ = ڳنڍ_pool++;
  ڳنڍ->lo = ڳنڍ->پڇاڙي_lo = lo;
  ڳنڍ->hi = ڳنڍ->پڇاڙي_hi = hi;
  ڳنڍ->dest = والد_پڇاڙي;
  ڳنڍ->nlo = nlo;
  ڳنڍ->nhi = nhi;
  ڳنڍ->والد = والد;
  ڳنڍ->level = والد->level + 1;
  ڳنڍ->queued = false;
  pthread_mutex_init (&ڳنڍ->تالو, عدم);

  إذا (nthreads > 1)
    {
      size_t lo_ڌاڳا = nthreads / 2;
      size_t hi_ڌاڳا = nthreads - lo_ڌاڳا;
      ڳنڍ->lo_ٻار = ڳنڍ_pool;
      ڳنڍ_pool = init_ڳنڍ (ڳنڍ, ڳنڍ_pool, lo, lo_ڌاڳا,
                             ڪل_سٽون, true);
      ڳنڍ->hi_ٻار = ڳنڍ_pool;
      ڳنڍ_pool = init_ڳنڍ (ڳنڍ, ڳنڍ_pool, hi, hi_ڌاڳا,
                             ڪل_سٽون, false);
    }
  وإلا
    {
      ڳنڍ->lo_ٻار = عدم;
      ڳنڍ->hi_ٻار = عدم;
    }
  أرجع ڳنڍ_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساكن صحيح
compare_nodes (فراغ ثابت *a, فراغ ثابت *b)
{
  بنية ملايو_ڳنڍ ثابت *nodea = a;
  بنية ملايو_ڳنڍ ثابت *nodeb = b;
  إذا (nodea->level == nodeb->level)
      أرجع (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  أرجع nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساكن مضمن فراغ
تالو_ڳنڍ (بنية ملايو_ڳنڍ *ڳنڍ)
{
  pthread_mutex_تالو (&ڳنڍ->تالو);
}

/* Unlock a merge tree NODE. */

ساكن مضمن فراغ
unlock_ڳنڍ (بنية ملايو_ڳنڍ *ڳنڍ)
{
  pthread_mutex_unlock (&ڳنڍ->تالو);
}

/* Destroy merge QUEUE. */

ساكن فراغ
قطار_destroy (بنية ملايو_ڳنڍ_قطار *قطار)
{
  ڍير_free (قطار->ترجيح_قطار);
  pthread_cond_destroy (&قطار->cond);
  pthread_mutex_destroy (&قطار->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساكن فراغ
قطار_init (بنية ملايو_ڳنڍ_قطار *قطار, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  قطار->ترجيح_قطار = ڍير_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&قطار->mutex, عدم);
  pthread_cond_init (&قطار->cond, عدم);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ساكن فراغ
قطار_وجهو (بنية ملايو_ڳنڍ_قطار *قطار, بنية ملايو_ڳنڍ *ڳنڍ)
{
  pthread_mutex_تالو (&قطار->mutex);
  ڍير_وجهو (قطار->ترجيح_قطار, ڳنڍ);
  ڳنڍ->queued = true;
  pthread_cond_signal (&قطار->cond);
  pthread_mutex_unlock (&قطار->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساكن بنية ملايو_ڳنڍ *
قطار_ڪڍو (بنية ملايو_ڳنڍ_قطار *قطار)
{
  بنية ملايو_ڳنڍ *ڳنڍ;
  pthread_mutex_تالو (&قطار->mutex);
  طالما (! (ڳنڍ = ڍير_remove_چوٽي (قطار->ترجيح_قطار)))
    pthread_cond_wait (&قطار->cond, &قطار->mutex);
  pthread_mutex_unlock (&قطار->mutex);
  تالو_ڳنڍ (ڳنڍ);
  ڳنڍ->queued = false;
  أرجع ڳنڍ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساكن فراغ
write_unique (بنية سٽ ثابت *سٽ, FILE *tfp, حرف ثابت *عارضي_output)
{
  إذا (unique)
    {
      إذا (saved_سٽ.متن && ! compare (سٽ, &saved_سٽ))
        أرجع;
      saved_سٽ = *سٽ;
    }

  write_سٽ (سٽ, tfp, عارضي_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساكن فراغ
mergelines_ڳنڍ (بنية ملايو_ڳنڍ *محدود ڳنڍ, size_t ڪل_سٽون,
                 FILE *tfp, حرف ثابت *عارضي_output)
{
  بنية سٽ *lo_orig = ڳنڍ->lo;
  بنية سٽ *hi_orig = ڳنڍ->hi;
  size_t منزل_ملايو = MAX_ملايو (ڪل_سٽون, ڳنڍ->level);
  size_t merged_lo;
  size_t merged_hi;

  إذا (ڳنڍ->level > ملايو_پاڙ)
    {
      /* Merge to destination buffer. */
      بنية سٽ *dest = *ڳنڍ->dest;
      طالما (ڳنڍ->lo != ڳنڍ->پڇاڙي_lo && ڳنڍ->hi != ڳنڍ->پڇاڙي_hi && منزل_ملايو--)
        إذا (compare (ڳنڍ->lo - 1, ڳنڍ->hi - 1) <= 0)
          *--dest = *--ڳنڍ->lo;
        وإلا
          *--dest = *--ڳنڍ->hi;

      merged_lo = lo_orig - ڳنڍ->lo;
      merged_hi = hi_orig - ڳنڍ->hi;

      إذا (ڳنڍ->nhi == merged_hi)
        طالما (ڳنڍ->lo != ڳنڍ->پڇاڙي_lo && منزل_ملايو--)
          *--dest = *--ڳنڍ->lo;
      وإلا إذا (ڳنڍ->nlo == merged_lo)
        طالما (ڳنڍ->hi != ڳنڍ->پڇاڙي_hi && منزل_ملايو--)
          *--dest = *--ڳنڍ->hi;
      *ڳنڍ->dest = dest;
    }
  وإلا
    {
      /* Merge directly to output. */
      طالما (ڳنڍ->lo != ڳنڍ->پڇاڙي_lo && ڳنڍ->hi != ڳنڍ->پڇاڙي_hi && منزل_ملايو--)
        {
          إذا (compare (ڳنڍ->lo - 1, ڳنڍ->hi - 1) <= 0)
            write_unique (--ڳنڍ->lo, tfp, عارضي_output);
          وإلا
            write_unique (--ڳنڍ->hi, tfp, عارضي_output);
        }

      merged_lo = lo_orig - ڳنڍ->lo;
      merged_hi = hi_orig - ڳنڍ->hi;

      إذا (ڳنڍ->nhi == merged_hi)
        {
          طالما (ڳنڍ->lo != ڳنڍ->پڇاڙي_lo && منزل_ملايو--)
            write_unique (--ڳنڍ->lo, tfp, عارضي_output);
        }
      وإلا إذا (ڳنڍ->nlo == merged_lo)
        {
          طالما (ڳنڍ->hi != ڳنڍ->پڇاڙي_hi && منزل_ملايو--)
            write_unique (--ڳنڍ->hi, tfp, عارضي_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ڳنڍ->lo;
  merged_hi = hi_orig - ڳنڍ->hi;
  ڳنڍ->nlo -= merged_lo;
  ڳنڍ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساكن فراغ
قطار_check_وجهو (بنية ملايو_ڳنڍ_قطار *قطار, بنية ملايو_ڳنڍ *ڳنڍ)
{
  إذا (! ڳنڍ->queued)
    {
      bool lo_avail = (ڳنڍ->lo - ڳنڍ->پڇاڙي_lo) != 0;
      bool hi_avail = (ڳنڍ->hi - ڳنڍ->پڇاڙي_hi) != 0;
      إذا (lo_avail ? hi_avail || ! ڳنڍ->nhi : hi_avail && ! ڳنڍ->nlo)
        قطار_وجهو (قطار, ڳنڍ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساكن فراغ
قطار_check_وجهو_والد (بنية ملايو_ڳنڍ_قطار *قطار,
                           بنية ملايو_ڳنڍ *ڳنڍ)
{
  إذا (ڳنڍ->level > ملايو_پاڙ)
    {
      تالو_ڳنڍ (ڳنڍ->والد);
      قطار_check_وجهو (قطار, ڳنڍ->والد);
      unlock_ڳنڍ (ڳنڍ->والد);
    }
  وإلا إذا (ڳنڍ->nlo + ڳنڍ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      قطار_وجهو (قطار, ڳنڍ->والد);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساكن فراغ
ملايو_loop (بنية ملايو_ڳنڍ_قطار *قطار,
            size_t ڪل_سٽون, FILE *tfp, حرف ثابت *عارضي_output)
{
  طالما (true)
    {
      بنية ملايو_ڳنڍ *ڳنڍ = قطار_ڪڍو (قطار);

      إذا (ڳنڍ->level == ملايو_پڇاڙي)
        {
          unlock_ڳنڍ (ڳنڍ);
          /* Reinsert so other threads can pop it. */
          قطار_وجهو (قطار, ڳنڍ);
          اكسر;
        }
      mergelines_ڳنڍ (ڳنڍ, ڪل_سٽون, tfp, عارضي_output);
      قطار_check_وجهو (قطار, ڳنڍ);
      قطار_check_وجهو_والد (قطار, ڳنڍ);

      unlock_ڳنڍ (ڳنڍ);
    }
}


ساكن فراغ sortlines (بنية سٽ *محدود, size_t, size_t,
                       بنية ملايو_ڳنڍ *, بنية ملايو_ڳنڍ_قطار *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

بنية thread_دليلون
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  بنية سٽ *سٽون;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت ڪل_سٽون;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  بنية ملايو_ڳنڍ *ثابت ڳنڍ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  بنية ملايو_ڳنڍ_قطار *ثابت قطار;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  حرف ثابت *output_عارضي;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ساكن فراغ *
sortlines_thread (فراغ *معلومات)
{
  بنية thread_دليلون ثابت *دليلون = معلومات;
  sortlines (دليلون->سٽون, دليلون->nthreads, دليلون->ڪل_سٽون,
             دليلون->ڳنڍ, دليلون->قطار, دليلون->tfp,
             دليلون->output_عارضي);
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
sortlines (بنية سٽ *محدود سٽون, size_t nthreads,
           size_t ڪل_سٽون, بنية ملايو_ڳنڍ *ڳنڍ,
           بنية ملايو_ڳنڍ_قطار *قطار, FILE *tfp, حرف ثابت *عارضي_output)
{
  size_t nlines = ڳنڍ->nlo + ڳنڍ->nhi;

  /* Calculate thread arguments. */
  size_t lo_ڌاڳا = nthreads / 2;
  size_t hi_ڌاڳا = nthreads - lo_ڌاڳا;
  pthread_t thread;
  بنية thread_دليلون دليلون = {سٽون, lo_ڌاڳا, ڪل_سٽون,
                             ڳنڍ->lo_ٻار, قطار, tfp, عارضي_output};

  إذا (nthreads > 1 && SUBTHREAD_سٽون_HEURISTIC <= nlines
      && pthread_create (&thread, عدم, sortlines_thread, &دليلون) == 0)
    {
      sortlines (سٽون - ڳنڍ->nlo, hi_ڌاڳا, ڪل_سٽون,
                 ڳنڍ->hi_ٻار, قطار, tfp, عارضي_output);
      pthread_join (thread, عدم);
    }
  وإلا
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ڳنڍ->nlo;
      size_t nhi = ڳنڍ->nhi;
      بنية سٽ *عارضي = سٽون - ڪل_سٽون;
      إذا (1 < nhi)
        sequential_ترتيب_ڏيو (سٽون - nlo, nhi, عارضي - nlo / 2, false);
      إذا (1 < nlo)
        sequential_ترتيب_ڏيو (سٽون, nlo, عارضي, false);

      /* Update merge NODE. No need to lock yet. */
      ڳنڍ->lo = سٽون;
      ڳنڍ->hi = سٽون - nlo;
      ڳنڍ->پڇاڙي_lo = سٽون - nlo;
      ڳنڍ->پڇاڙي_hi = سٽون - nlo - nhi;

      قطار_وجهو (قطار, ڳنڍ);
      ملايو_loop (قطار, ڪل_سٽون, tfp, عارضي_output);
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
avoid_trashing_داخلا (بنية sortfile *files, size_t ntemps,
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
              tempcopy = create_عارضي (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].عارضي = tempcopy;
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
        ترتيب_ڏيو_die (_("cannot read"), files[i]);
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
      صحيح oflags = O_WRONLY | O_ٻٽو | O_CLOEXEC | O_CREAT;
      صحيح outfd = open (outfile, oflags, گهڻائي_RW_UGO);
      إذا (outfd < 0)
        ترتيب_ڏيو_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ساكن فراغ
ملايو (بنية sortfile *files, size_t ntemps, size_t nfiles,
       حرف ثابت *output_file)
{
  طالما (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t اندريون;

      /* Number of output files generated so far.  */
      size_t ٻاهريون;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      لكل (ٻاهريون = اندريون = 0; nmerge <= nfiles - اندريون; ٻاهريون++)
        {
          FILE *tfp;
          بنية tempnode *عارضي = create_عارضي (&tfp);
          size_t انگ_merged = mergefiles (&files[اندريون], MIN (ntemps, nmerge),
                                          nmerge, tfp, عارضي->name);
          ntemps -= MIN (ntemps, انگ_merged);
          files[ٻاهريون].name = عارضي->name;
          files[ٻاهريون].عارضي = عارضي;
          اندريون += انگ_merged;
        }

      remainder = nfiles - اندريون;
      cheap_slots = nmerge - ٻاهريون % nmerge;

      إذا (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          بنية tempnode *عارضي = create_عارضي (&tfp);
          size_t انگ_merged = mergefiles (&files[اندريون], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, عارضي->name);
          ntemps -= MIN (ntemps, انگ_merged);
          files[ٻاهريون].name = عارضي->name;
          files[ٻاهريون++].عارضي = عارضي;
          اندريون += انگ_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[ٻاهريون], &files[اندريون], (nfiles - اندريون) * حجم *files);
      ntemps += ٻاهريون;
      nfiles -= اندريون - ٻاهريون;
    }

  avoid_trashing_داخلا (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  طالما (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_داخلا_files (files, nfiles, &fps);

      إذا (nopened == nfiles)
        {
          FILE *ofp = وهڪرو_open (output_file, "w");
          إذا (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              اكسر;
            }
          إذا (errno != EMFILE || nopened <= 2)
            ترتيب_ڏيو_die (_("open failed"), output_file);
        }
      وإلا إذا (nopened <= 2)
        ترتيب_ڏيو_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      بنية tempnode *عارضي;
      افعل
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          عارضي = maybe_create_عارضي (&tfp, ! (nopened <= 2));
        }
      طالما (!عارضي);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, عارضي->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = عارضي->name;
      files[0].عارضي = عارضي;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * حجم *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ساكن فراغ
ترتيب_ڏيو (حرف *ثابت *files, size_t nfiles, حرف ثابت *output_file,
      size_t nthreads)
{
  بنية بفر buf;
  بنية ترتيب_ڏيو_بفر_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.حد_بندي = 0;
  policy.growth_failed = false;

  طالما (nfiles)
    {
      حرف ثابت *عارضي_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t بائيٽون_في_سٽ;
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
          بائيٽون_في_سٽ = (mult * حجم (بنية سٽ));
        }
      وإلا
        بائيٽون_في_سٽ = حجم (بنية سٽ) * 3 / 2;

      إذا (! buf.alloc)
        {
          ترتيب_ڏيو_بفر_policy (&fp, 1, files, nfiles, بائيٽون_في_سٽ,
                              &policy);
          initbuf (&buf, بائيٽون_في_سٽ, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      طالما (fillbuf (&buf, fp, file, &policy))
        {
          بنية سٽ *سٽ;

          إذا (buf.eof && nfiles
              && (بائيٽون_في_سٽ + 1
                  < (buf.alloc - buf.استعمال_ٿيل - بائيٽون_في_سٽ * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.کاٻو = buf.استعمال_ٿيل;
              اكسر;
            }

          saved_سٽ.متن = عدم;
          سٽ = بفر_linelim (&buf);
          إذا (buf.eof && !nfiles && !ntemps && !buf.کاٻو)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              عارضي_output = output_file;
              output_file_created = true;
            }
          وإلا
            {
              ++ntemps;
              عارضي_output = create_عارضي (&tfp)->name;
            }
          إذا (1 < buf.nlines)
            {
              إذا (nthreads > 1)
                {
                  بنية ملايو_ڳنڍ_قطار قطار;
                  قطار_init (&قطار, nthreads);
                  بنية ملايو_ڳنڍ *ملايو_وڻ =
                    ملايو_وڻ_init (nthreads, buf.nlines, سٽ);

                  sortlines (سٽ, nthreads, buf.nlines, ملايو_وڻ + 1,
                             &قطار, tfp, عارضي_output);

                  ملايو_وڻ_destroy (nthreads, ملايو_وڻ);
                  قطار_destroy (&قطار);
                }
              وإلا
                {
                  sequential_ترتيب_ڏيو (سٽ, buf.nlines,
                                   سٽ - buf.nlines, false);
                  لكل (size_t i = 0; i < buf.nlines; i++)
                    write_unique (سٽ - i - 1, tfp, عارضي_output);
                }
            }
          وإلا
            write_unique (سٽ - 1, tfp, عارضي_output);

          xfclose (tfp, عارضي_output);

          إذا (output_file_created)
            اذهب پڄاڻي;
        }
      xfclose (fp, file);
    }

 پڄاڻي:
  حرر (buf.buf);

  إذا (! output_file_created)
    {
      بنية tempnode *ڳنڍ = temphead;
      بنية sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      لكل (size_t i = 0; ڳنڍ; i++)
        {
          tempfiles[i].name = ڳنڍ->name;
          tempfiles[i].عارضي = ڳنڍ;
          ڳنڍ = ڳنڍ->ايندڙ;
        }
      ملايو (tempfiles, ntemps, ntemps, output_file);
      حرر (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساكن فراغ
insertkey (بنية keyfield *ڪنجي_arg)
{
  بنية keyfield **p;
  بنية keyfield *ڪنجي = xmemdup (ڪنجي_arg, حجم *ڪنجي);

  لكل (p = &keylist; *p; p = &(*p)->ايندڙ)
    استمر;
  *p = ڪنجي;
  ڪنجي->ايندڙ = عدم;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ساكن فراغ
badfieldspec (حرف ثابت *spec, حرف ثابت *msgid)
{
  error (ترتيب_ڏيو_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ساكن فراغ
incompatible_options (حرف ثابت *opts)
{
  error (ترتيب_ڏيو_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ساكن فراغ
check_ordering_compatibility (فراغ)
{
  لكل (بنية keyfield *ڪنجي = keylist; ڪنجي; ڪنجي = ڪنجي->ايندڙ)
    إذا (1 < (ڪنجي->numeric + ڪنجي->general_numeric + ڪنجي->human_numeric
             + ڪنجي->month + (ڪنجي->نسخو_3 | ڪنجي->random | !!ڪنجي->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        ڪنجي->skipsblanks = ڪنجي->skipeblanks = ڪنجي->ابتو = false;
        ڪنجي_منزل_opts (ڪنجي, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساكن حرف ثابت *
parse_ميدان_ڳڻپ (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
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
    حالة LONGINT_حد_کان_مٿي:
    حالة LONGINT_حد_کان_مٿي | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      اكسر;

    حالة LONGINT_INVALID:
      إذا (msgid)
        error (ترتيب_ڏيو_FAILURE, 0, _("%s: invalid count at start of %s"),
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
سيٽ_ordering (حرف ثابت *s, بنية keyfield *ڪنجي, تعداد blanktype blanktype)
{
  طالما (*s)
    {
      بدل (*s)
        {
        حالة 'b':
          إذا (blanktype == bl_شروعات || blanktype == bl_both)
            ڪنجي->skipsblanks = true;
          إذا (blanktype == bl_پڇاڙي || blanktype == bl_both)
            ڪنجي->skipeblanks = true;
          اكسر;
        حالة 'd':
          ڪنجي->ignore = nondictionary;
          اكسر;
        حالة 'f':
          ڪنجي->translate = fold_toupper;
          اكسر;
        حالة 'g':
          ڪنجي->general_numeric = true;
          اكسر;
        حالة 'h':
          ڪنجي->human_numeric = true;
          اكسر;
        حالة 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          إذا (! ڪنجي->ignore)
            ڪنجي->ignore = nonprinting;
          اكسر;
        حالة 'M':
          ڪنجي->month = true;
          اكسر;
        حالة 'n':
          ڪنجي->numeric = true;
          اكسر;
        حالة 'R':
          ڪنجي->random = true;
          اكسر;
        حالة 'r':
          ڪنجي->ابتو = true;
          اكسر;
        حالة 'V':
          ڪنجي->نسخو_3 = true;
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
ڪنجي_init (بنية keyfield *ڪنجي)
{
  memset (ڪنجي, 0, حجم *ڪنجي);
  ڪنجي->eword = SIZE_MAX;
  أرجع ڪنجي;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  بنية keyfield *ڪنجي;
  بنية keyfield ڪنجي_buf;
  بنية keyfield gkey;
  bool gkey_only = false;
  حرف ثابت *s;
  صحيح c = 0;
  حرف checkonly = 0;
  bool mergeonly = false;
  حرف *random_ذريعو = عدم;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  صحيح posix_ver = posix2_نسخو ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  حرف **files;
  حرف *files_from = عدم;
  بنية ٽوڪن tok;
  حرف ثابت *outfile = عدم;
  bool locale_ok;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ترتيب_ڏيو_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#إذا HAVE_NL_LANGINFO
  hard_LC_وقت = hard_locale (LC_وقت);
#جيڪڏهن_پورو

  /* Get locale's representation of the decimal point.  */
  {
    بنية lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_نقطو = locale->decimal_نقطو[0];
    إذا (! decimal_نقطو || locale->decimal_نقطو[1])
      decimal_نقطو = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    إذا (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    إذا (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    تعداد { nsigs = countof (رڪن_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سگنل);
    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (رڪن_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        sigaction (رڪن_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سگنل, رڪن_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_سگنل;
    act.sa_flags = 0;

    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (رڪن_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        إذا (sigismember (&caught_سگنل, رڪن_sig[i]))
          sigaction (رڪن_sig[i], &act, عدم);
      }
  }
  إشارة (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ڪنجي_init (&gkey);
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
          ڪنجي = عدم;
          إذا (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              إذا (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ڪنجي = ڪنجي_init (&ڪنجي_buf);
                  s = parse_ميدان_ڳڻپ (optarg + 1, &ڪنجي->sword, عدم);
                  إذا (s && *s == '.')
                    s = parse_ميدان_ڳڻپ (s + 1, &ڪنجي->schar, عدم);
                  إذا (! (ڪنجي->sword || ڪنجي->schar))
                    ڪنجي->sword = SIZE_MAX;
                  إذا (! s || *سيٽ_ordering (s, ڪنجي, bl_شروعات))
                    ڪنجي = عدم;
                  وإلا
                    {
                      إذا (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_ميدان_ڳڻپ (optarg1 + 1, &ڪنجي->eword,
                                             N_("invalid number after '-'"));
                          إذا (*s == '.')
                            s = parse_ميدان_ڳڻپ (s + 1, &ڪنجي->echar,
                                               N_("invalid number after '.'"));
                          إذا (!ڪنجي->echar && ڪنجي->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ڪنجي->eword--;
                            }
                          إذا (*سيٽ_ordering (s, ڪنجي, bl_پڇاڙي))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ڪنجي->traditional_استعمال_ٿيل = true;
                      insertkey (ڪنجي);
                    }
                }
            }
          إذا (! ڪنجي)
            files[nfiles++] = optarg;
          اكسر;

        حالة ترتيب_ڏيو_OPTION:
          c = XARGMATCH ("--sort", optarg, ترتيب_ڏيو_دليلون, ترتيب_ڏيو_types);
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
            سيٽ_ordering (str, &gkey, bl_both);
          }
          اكسر;

        حالة CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_دليلون, check_types)
               : 'c');
          FALLTHROUGH;
        حالة 'c':
        حالة 'C':
          إذا (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          اكسر;

        حالة COMPRESS_پروگرام_OPTION:
          إذا (compress_پروگرام && !streq (compress_پروگرام, optarg))
            error (ترتيب_ڏيو_FAILURE, 0, _("multiple compress programs specified"));
          compress_پروگرام = optarg;
          اكسر;

        حالة DEBUG_پروگرام_OPTION:
          debug = true;
          اكسر;

        حالة FILES0_FROM_OPTION:
          files_from = optarg;
          اكسر;

        حالة 'k':
          ڪنجي = ڪنجي_init (&ڪنجي_buf);

          /* Get POS1. */
          s = parse_ميدان_ڳڻپ (optarg, &ڪنجي->sword,
                                 N_("invalid number at field start"));
          إذا (! ڪنجي->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          إذا (*s == '.')
            {
              s = parse_ميدان_ڳڻپ (s + 1, &ڪنجي->schar,
                                     N_("invalid number after '.'"));
              إذا (! ڪنجي->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          إذا (! (ڪنجي->sword || ڪنجي->schar))
            ڪنجي->sword = SIZE_MAX;
          s = سيٽ_ordering (s, ڪنجي, bl_شروعات);
          إذا (*s != ',')
            {
              ڪنجي->eword = SIZE_MAX;
              ڪنجي->echar = 0;
            }
          وإلا
            {
              /* Get POS2. */
              s = parse_ميدان_ڳڻپ (s + 1, &ڪنجي->eword,
                                     N_("invalid number after ','"));
              إذا (! ڪنجي->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              إذا (*s == '.')
                {
                  s = parse_ميدان_ڳڻپ (s + 1, &ڪنجي->echar,
                                         N_("invalid number after '.'"));
                }
              s = سيٽ_ordering (s, ڪنجي, bl_پڇاڙي);
            }
          إذا (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ڪنجي);
          اكسر;

        حالة 'm':
          mergeonly = true;
          اكسر;

        حالة NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          اكسر;

        حالة 'o':
          إذا (outfile && !streq (outfile, optarg))
            error (ترتيب_ڏيو_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          اكسر;

        حالة RANDOM_ذريعو_OPTION:
          إذا (random_ذريعو && !streq (random_ذريعو, optarg))
            error (ترتيب_ڏيو_FAILURE, 0, _("multiple random sources specified"));
          random_ذريعو = optarg;
          اكسر;

        حالة 's':
          stable = true;
          اكسر;

        حالة 'S':
          specify_ترتيب_ڏيو_سائيز (oi, c, optarg);
          اكسر;

        حالة 't':
          {
            حرف newtab = optarg[0];
            إذا (! newtab)
              error (ترتيب_ڏيو_FAILURE, 0, _("empty tab"));
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
                    error (ترتيب_ڏيو_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            إذا (tab != TAB_DEFAULT && tab != newtab)
              error (ترتيب_ڏيو_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          اكسر;

        حالة 'T':
          جوڙيو_عارضي_dir (optarg);
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

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (ترتيب_ڏيو_FAILURE);
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
          usage (ترتيب_ڏيو_FAILURE);
        }

      FILE *وهڪرو = xfopen (files_from, "r");

      readtokens0_init (&tok);

      إذا (! readtokens0 (وهڪرو, &tok))
        error (ترتيب_ڏيو_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (وهڪرو, files_from);

      إذا (tok.n_tok)
        {
          حرر (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          لكل (size_t i = 0; i < nfiles; i++)
            {
              إذا (streq (files[i], "-"))
                error (ترتيب_ڏيو_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              وإلا إذا (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  بنانشان ڊگهو صحيح file_عدد = i + 1;
                  error (ترتيب_ڏيو_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_عدد);
                }
            }
        }
      وإلا
        error (ترتيب_ڏيو_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  لكل (ڪنجي = keylist; ڪنجي; ڪنجي = ڪنجي->ايندڙ)
    {
      إذا (default_ڪنجي_compare (ڪنجي) && !ڪنجي->ابتو)
        {
          ڪنجي->ignore = gkey.ignore;
          ڪنجي->translate = gkey.translate;
          ڪنجي->skipsblanks = gkey.skipsblanks;
          ڪنجي->skipeblanks = gkey.skipeblanks;
          ڪنجي->month = gkey.month;
          ڪنجي->numeric = gkey.numeric;
          ڪنجي->general_numeric = gkey.general_numeric;
          ڪنجي->human_numeric = gkey.human_numeric;
          ڪنجي->نسخو_3 = gkey.نسخو_3;
          ڪنجي->random = gkey.random;
          ڪنجي->ابتو = gkey.ابتو;
        }

      need_random |= ڪنجي->random;
    }

  إذا (!keylist && !default_ڪنجي_compare (&gkey))
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

      ڪنجي_warnings (&gkey, gkey_only);
    }

  ابتو = gkey.ابتو;

  إذا (need_random)
    random_md5_state_init (random_ذريعو);

  إذا (عارضي_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      جوڙيو_عارضي_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
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
  إذا (0 < ترتيب_ڏيو_سائيز)
    ترتيب_ڏيو_سائيز = MAX (ترتيب_ڏيو_سائيز, MIN_ترتيب_ڏيو_سائيز);

  إذا (checkonly)
    {
      إذا (nfiles > 1)
        error (ترتيب_ڏيو_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      إذا (outfile)
        {
          ساكن حرف opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      ٻاهر_نڪرو (check (files[0], checkonly) ? EXIT_SUCCESS : ترتيب_ڏيو_ٻاهريون_جو_ترتيب);
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

      ملايو (sortfiles, 0, nfiles, outfile);
    }
  وإلا
    {
      إذا (!nthreads)
        {
          بنانشان ڊگهو صحيح np = انگ_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_ڌاڳا);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (بنية ملايو_ڳنڍ));
      nthreads = MIN (nthreads, nthreads_max);

      ترتيب_ڏيو (files, nfiles, outfile, nthreads);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    ترتيب_ڏيو_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
