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

#اگر_تعریف_نشده RLIMIT_داده
بنية rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#پایان_اگر

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "sort"

#تعریف_کن AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#إذا HAVE_LANGINFO_CODESET
# include <langinfo.h>
#پایان_اگر

#إذا !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#پایان_اگر
#إذا !defined OPEN_MAX
# define OPEN_MAX 20
#پایان_اگر

#تعریف_کن UCHAR_LIM (UCHAR_MAX + 1)

#اگر_تعریف_نشده DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#پایان_اگر

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#تعریف_کن MAX_ادغام(مجموع, level) (((مجموع) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
تعداد { SUBTHREAD_سطرها_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_سطرها_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
تعداد { DEFAULT_MAX_نخها = 8 };

/* Exit statuses.  */
تعداد
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    مرتب_سازی_بیرون_از_ترتیب = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    مرتب_سازی_FAILURE = 2
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
    ادغام_انتها = 0,

    /* Level of the root node in merge tree. */
    ادغام_ریشه = 1
  };

/* The representation of the decimal point in the current locale.  */
ساكن حرف decimal_نقطه;

/* Thousands separator; if outside char range, there is no separator.  */
ساكن صحيح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساكن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساكن bool hard_LC_COLLATE;
#إذا HAVE_NL_LANGINFO
ساكن bool hard_LC_زمان;
#پایان_اگر

#تعریف_کن NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
تعداد blanktype { bl_شروع, bl_انتها, bl_both };

/* The character marking end of line. Default to \n. */
ساكن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
بنية سطر
{
  حرف *متن;			/* Text of the line. */
  size_t درازی;		/* Length including final newline. */
  حرف *keybeg;			/* Start of first key. */
  حرف *keylim;			/* Limit of first key. */
};

/* Input buffers. */
بنية میانگیر
{
  حرف *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t استفاده_شده;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t چپ;			/* Number of bytes left from previous reads. */
  size_t سطر_بایتها;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
بنية مرتب_سازی_میانگیر_policy
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
  bool معکوس;			/* Reverse the sense of comparison. */
  bool نسخه_3;			/* sort by version number */
  bool traditional_استفاده_شده;	/* Traditional key option format is used. */
  بنية keyfield *بعدی;	/* Next keyfield to try. */
};

بنية month
{
  حرف ثابت *name;
  صحيح val;
};

/* Binary merge tree node. */
بنية ادغام_گره
{
  بنية سطر *lo;              /* Lines to merge from LO child node. */
  بنية سطر *hi;              /* Lines to merge from HI child node. */
  بنية سطر *انتها_lo;          /* End of available lines from LO. */
  بنية سطر *انتها_hi;          /* End of available lines from HI. */
  بنية سطر **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  بنية ادغام_گره *والد;    /* Parent node. */
  بنية ادغام_گره *lo_فرزند;  /* LO child node. */
  بنية ادغام_گره *hi_فرزند;  /* HI child node. */
  بیعلامت صحيح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t قفل;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
بنية ادغام_گره_صف
{
  بنية انباشته *اولویت_صف;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساكن بنية سطر saved_سطر;

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

#تعریف_کن MONTHS_در_هر_YEAR 12

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
#تعریف_کن NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#تعریف_کن MIN_ادغام_میانگیر_اندازه (2 + حجم (بنية سطر))

/* Minimum sort size; the code might not work with smaller sizes.  */
#تعریف_کن MIN_مرتب_سازی_اندازه (nmerge * MIN_ادغام_میانگیر_اندازه)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ساكن size_t ادغام_میانگیر_اندازه = MAX (MIN_ادغام_میانگیر_اندازه, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ساكن size_t مرتب_سازی_اندازه;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#تعریف_کن درون_داد_FILE_اندازه_GUESS (128 * 1024)

ساكن بنية
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  حرف ثابت **dirs COUNTED_BY (n_dirs);
} موقت_dirs;



/* Flag to reverse the order of all comparisons. */
ساكن bool معکوس;

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
ساكن حرف ثابت *compress_برنامه;

/* Annotate the output with extra info to aid the user.  */
ساكن bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ساكن بیعلامت صحيح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ساكن bool default_SIGPIPE;

/* The list of temporary files. */
بنية tempnode
{
  بنية tempnode *متطاير بعدی;
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
  لكل (بنية tempnode ثابت *گره = temphead; گره; گره = گره->بعدی)
    unlink (گره->name);
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
مرتب_سازی_die (حرف ثابت *پیام_2, حرف ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  إذا (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (مرتب_سازی_FAILURE, errno, "%s: %s", پیام_2,
         quotef (file ? file : _("standard output")));
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              برنامه_name, برنامه_name);
      fputs (_("\
Write sorted concatenation از all FILE(s) مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding مرتب_سازی کلیدها درون each سطر\n\
"));
      oputs (_("\
  -d, --dictionary-ترتیب\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالة\n\
         fold پایینی حالة مقصد فوقانی حالة characters\n\
"));
      oputs (_("\
  -g, --general-numeric-مرتب_سازی\n\
         compare according مقصد general numerical مقدار\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-مرتب_سازی\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-مرتب_سازی\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-مرتب_سازی\n\
         compare according مقصد string numerical مقدار;\n\
         see پر documentation لكل supported رشتهها\n\
"));
      oputs (_("\
  -R, --random-مرتب_سازی\n\
         shuffle, but گروه identical کلیدها.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-منبع_2=FILE\n\
         get random بایتها from FILE\n\
"));
      oputs (_("\
  -r, --معکوس\n\
         معکوس the نتیجه از comparisons\n\
"));
      oputs (_("\
      --مرتب_سازی=کلمه\n\
         مرتب_سازی according مقصد کلمه:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, نسخه_3 -V\n\
"));
      oputs (_("\
  -V, --نسخه_3-مرتب_سازی\n\
         natural مرتب_سازی از (نسخه_3) numbers within متن\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-اندازه=NMERGE\n\
         ادغام at most NMERGE inputs at once; لكل بیشتر use موقت files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-اولی\n\
         check لكل sorted درون_داد; افعل not مرتب_سازی\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but افعل not report اولی bad سطر\n\
"));
      oputs (_("\
      --compress-برنامه=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part از the سطر استفاده_شده مقصد مرتب_سازی,\n\
         and warn about questionable usage مقصد standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ درون_داد from the files specified by NUL-terminated نامها درون file F;\
\n\
         If F is -, اقرأ نامها from standard درون_داد\n\
"));
      oputs (_("\
  -k, --کلید=KEYDEF\n\
         مرتب_سازی از_طریق a کلید; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --ادغام\n\
         ادغام already sorted files; افعل not مرتب_سازی\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         اكتب نتیجه مقصد FILE instead از standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize مرتب_سازی by disabling آخرین-resort comparison\n\
"));
      oputs (_("\
  -S, --میانگیر-اندازه=اندازه_2\n\
         use اندازه_2 لكل اصلی حافظه میانگیر\n\
"));
      oputs (_("\
  -t, --فیلد-separator=SEP\n\
         use SEP instead از non-blank مقصد blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR لكل temporaries, not $TMPDIR or %s;\n\
         مضرب options specify مضرب directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the عدد از sorts پیاپی_2 concurrently مقصد N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the اولی از سطرها with equal کلیدها;\n\
         with -c, check لكل strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] لكل شروع and stop position, where F is a\n\
فیلد عدد and C a character position درون the فیلد; both are origin 1, and\n\
the stop position defaults مقصد the سطر's انتها.  If neither -t nor -b is درون\n\
effect, characters درون a فیلد are counted from the beginning از the preceding\n\
whitespace.  OPTS is one or بیشتر single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options لكل that کلید.  If no کلید is given, use\n\
the entire سطر as the کلید.  Use --debug مقصد diagnose incorrect کلید usage.\n\
\n\
اندازه_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% از حافظه, b 1, K 1024 (افتراضي), and so on لكل M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects مرتب_سازی ترتیب.\n\
مجموعه_3 LC_ALL=C مقصد get the traditional مرتب_سازی ترتیب that استفاده_میکند\n\
native byte مقادیر.\n\
"), stdout );
      emit_ancillary_info (برنامه_NAME);
    }

  خروج (وضعیت);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_برنامه_OPTION,
  DEBUG_برنامه_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_منبع_OPTION,
  مرتب_سازی_OPTION,
  PARALLEL_OPTION
};

ساكن حرف ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ساكن بنية option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_آرگومان, عدم, 'b'},
  {"check", optional_آرگومان, عدم, CHECK_OPTION},
  {"compress-program", required_آرگومان, عدم, COMPRESS_برنامه_OPTION},
  {"debug", no_آرگومان, عدم, DEBUG_برنامه_OPTION},
  {"dictionary-order", no_آرگومان, عدم, 'd'},
  {"ignore-case", no_آرگومان, عدم, 'f'},
  {"files0-from", required_آرگومان, عدم, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_آرگومان, عدم, 'g'},
  {"ignore-nonprinting", no_آرگومان, عدم, 'i'},
  {"key", required_آرگومان, عدم, 'k'},
  {"merge", no_آرگومان, عدم, 'm'},
  {"month-sort", no_آرگومان, عدم, 'M'},
  {"numeric-sort", no_آرگومان, عدم, 'n'},
  {"human-numeric-sort", no_آرگومان, عدم, 'h'},
  {"version-sort", no_آرگومان, عدم, 'V'},
  {"random-sort", no_آرگومان, عدم, 'R'},
  {"random-source", required_آرگومان, عدم, RANDOM_منبع_OPTION},
  {"sort", required_آرگومان, عدم, مرتب_سازی_OPTION},
  {"output", required_آرگومان, عدم, 'o'},
  {"reverse", no_آرگومان, عدم, 'r'},
  {"stable", no_آرگومان, عدم, 's'},
  {"batch-size", required_آرگومان, عدم, NMERGE_OPTION},
  {"buffer-size", required_آرگومان, عدم, 'S'},
  {"field-separator", required_آرگومان, عدم, 't'},
  {"temporary-directory", required_آرگومان, عدم, 'T'},
  {"unique", no_آرگومان, عدم, 'u'},
  {"zero-terminated", no_آرگومان, عدم, 'z'},
  {"parallel", required_آرگومان, عدم, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0},
};

#تعریف_کن CHECK_جدول \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ساكن حرف ثابت *ثابت check_آرگومانها[] =
{
#تعریف_کن _ct_(_s, _c) _s,
  CHECK_جدول عدم
#لغو_تعریف  _ct_
};
ساكن حرف ثابت check_types[] =
{
#تعریف_کن _ct_(_s, _c) _c,
  CHECK_جدول
#لغو_تعریف  _ct_
};

#تعریف_کن مرتب_سازی_جدول \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ساكن حرف ثابت *ثابت مرتب_سازی_آرگومانها[] =
{
#تعریف_کن _st_(_s, _c) _s,
  مرتب_سازی_جدول عدم
#لغو_تعریف  _st_
};
ساكن حرف ثابت مرتب_سازی_types[] =
{
#تعریف_کن _st_(_s, _c) _c,
  مرتب_سازی_جدول
#لغو_تعریف  _st_
};

/* The set of signals that are caught.  */
ساكن sigset_t caught_سیگنالها;

/* Critical section status.  */
بنية cs_وضعیت
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ساكن فراغ
cs_enter (بنية cs_وضعیت *وضعیت)
{
  صحيح ret = pthread_sigmask (SIG_BLOCK, &caught_سیگنالها, &وضعیت->sigs);
  وضعیت->valid = ret == 0;
}

/* Leave a critical section.  */
ساكن فراغ
cs_leave (بنية cs_وضعیت ثابت *وضعیت)
{
  إذا (وضعیت->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &وضعیت->sigs, عدم);
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
  بنية tempnode *موقت;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ساكن Hash_جدول *proctab;

تعداد { INIT_PROCTAB_اندازه = 47 };

ساكن size_t
proctab_hasher (فراغ ثابت *مدخل, size_t tabsize)
{
  بنية tempnode ثابت *گره = مدخل;
  أرجع گره->pid % tabsize;
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
  صحيح وضعیت;
  pid_t cpid = waitpid ((pid ? pid : -1), &وضعیت, (pid ? 0 : WNOHANG));

  إذا (cpid < 0)
    error (مرتب_سازی_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_برنامه));
  وإلا إذا (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      إذا (! WIFEXITED (وضعیت) || WEXITSTATUS (وضعیت))
        error (مرتب_سازی_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_برنامه));
      --nprocs;
    }

  أرجع cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساكن فراغ
ثبات_proc (بنية tempnode *موقت)
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

  موقت->state = UNREAPED;

  إذا (! hash_درج (proctab, موقت))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساكن bool
delete_proc (pid_t pid)
{
  بنية tempnode test;

  test.pid = pid;
  بنية tempnode *گره = hash_remove (proctab, &test);
  إذا (! گره)
    أرجع false;
  گره->state = REAPED;
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
      بنية cs_وضعیت cs;
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
create_موقت_file (صحيح *pfd, bool survive_fd_exhaustion)
{
  ساكن حرف ثابت slashbase[] = "/sortXXXXXX";
  ساكن idx_t موقت_dir_شاخص;
  صحيح fd;
  صحيح saved_errno;
  حرف ثابت *موقت_dir = موقت_dirs.dirs[موقت_dir_شاخص];
  size_t len = طول_رشته (موقت_dir);
  بنية tempnode *گره =
    xmalloc (FLEXSIZEOF (بنية tempnode, name, len + حجم slashbase));
  حرف *file = گره->name;
  بنية cs_وضعیت cs;

  کپی_حافظه (file, موقت_dir, len);
  کپی_حافظه (file + len, slashbase, حجم slashbase);
  گره->بعدی = عدم;
  إذا (++موقت_dir_شاخص == موقت_dirs.n_dirs)
    موقت_dir_شاخص = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  إذا (0 <= fd)
    {
      *temptail = گره;
      temptail = &گره->بعدی;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  إذا (fd < 0)
    {
      إذا (! (survive_fd_exhaustion && errno == EMFILE))
        error (مرتب_سازی_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (موقت_dir));
      حرر (گره);
      گره = عدم;
    }

  *pfd = fd;
  أرجع گره;
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
جریان_open (حرف ثابت *file, حرف ثابت *how)
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
          إذا (!outst || S_ISREG (outst->st_نما) || S_TYPEISSHM (outst))
            error (مرتب_سازی_FAILURE, ftruncate_errno, _("%s: error truncating"),
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
  FILE *fp = جریان_open (file, how);
  إذا (!fp)
    مرتب_سازی_die (_("open failed"), file);
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
        مرتب_سازی_die (_("fflush failed"), file);
      اكسر;

    افتراضي:
      إذا (fclose (fp) != 0)
        مرتب_سازی_die (_("close failed"), file);
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
      ignore_مقدار (dup2 (oldfd, newfd));
      ignore_مقدار (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساكن صحيح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحيح oldfd, صحيح newfd)
{
  صحيح نتیجه = 0;
  إذا (oldfd != newfd)
    {
      نتیجه = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      إذا (نتیجه == 0)
        نتیجه = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  أرجع نتیجه;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساكن حرف ثابت *
get_resolved_compress_برنامه (فراغ)
{
  /* Use a cache, to perform the search only once.  */
  ساكن حرف ثابت *resolved_compress_برنامه_cache /* = NULL */;

  إذا (resolved_compress_برنامه_cache == عدم)
    {
      resolved_compress_برنامه_cache =
        پیدا_کن_درون_given_مسیر (compress_برنامه, getenv ("PATH"), عدم, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  أرجع resolved_compress_برنامه_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ساكن صحيح
pipe_فرزند (pid_t *pid, صحيح pipefds[2], صحيح tempfd, bool decompress,
            size_t tries)
{
  حرف ثابت *resolved_compress_برنامه;
  بنية tempnode *saved_temphead;
  مزدوج wait_retry = 0.25;
  بنية cs_وضعیت cs;
  صحيح نتیجه;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_برنامه = get_resolved_compress_برنامه ();
  إذا (resolved_compress_برنامه == عدم)
    أرجع errno;

  إذا ((نتیجه = posix_spawnattr_init (&attr)))
    أرجع نتیجه;
  إذا ((نتیجه = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (نتیجه = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      أرجع نتیجه;
    }

  إذا (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحيح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع saved_errno;
    }

  إذا ((نتیجه = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (نتیجه = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((نتیجه = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (نتیجه = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (نتیجه = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((نتیجه = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (نتیجه = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (نتیجه = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      أرجع نتیجه;
    }

  حرف ثابت *ثابت argv[] =
    {
      resolved_compress_برنامه,
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

      نتیجه = posix_spawnp (pid, resolved_compress_برنامه, &actions, &attr,
                             (حرف * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      إذا (نتیجه != EAGAIN)
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

  إذا (نتیجه)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  وإلا
    ++nprocs;

  أرجع نتیجه;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ساكن بنية tempnode *
maybe_create_موقت (FILE **pfp, bool survive_fd_exhaustion)
{
  صحيح tempfd;
  بنية tempnode *گره = create_موقت_file (&tempfd, survive_fd_exhaustion);
  إذا (! گره)
    أرجع عدم;

  گره->state = UNCOMPRESSED;

  إذا (compress_برنامه)
    {
      صحيح pipefds[2];
      ساكن صحيح آخرین_نتیجه = 0;

      صحيح نتیجه = pipe_فرزند (&گره->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      إذا (نتیجه)
        {
          إذا (نتیجه != آخرین_نتیجه)
            error (0, نتیجه, _("could not run compress program %s"),
                   quoteaf (compress_برنامه));
        }
      وإلا
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          ثبات_proc (گره);
        }

      آخرین_نتیجه = نتیجه;
    }

  *pfp = fdopen (tempfd, "w");
  إذا (! *pfp)
    مرتب_سازی_die (_("couldn't create temporary file"), گره->name);

  أرجع گره;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساكن بنية tempnode *
create_موقت (FILE **pfp)
{
  أرجع maybe_create_موقت (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ساكن FILE *
open_موقت (بنية tempnode *موقت)
{
  صحيح tempfd, pipefds[2];
  FILE *fp = عدم;

  إذا (موقت->state == UNREAPED)
    wait_proc (موقت->pid);

  tempfd = open (موقت->name, O_RDONLY);
  إذا (tempfd < 0)
    أرجع عدم;

  pid_t فرزند;
  صحيح نتیجه = pipe_فرزند (&فرزند, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  إذا (نتیجه)
    {
      إذا (نتیجه != EMFILE)
        error (مرتب_سازی_FAILURE, نتیجه, _("could not run compress program %s -d"),
               quoteaf (compress_برنامه));
      close (tempfd);
      errno = EMFILE;
    }
  وإلا
    {
      موقت->pid = فرزند;
      ثبات_proc (موقت);
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
جمع_کن_موقت_dir (حرف ثابت *dir)
{
  إذا (موقت_dirs.n_dirs == موقت_dirs.n_dirs_alloc)
    موقت_dirs.dirs = xpalloc (موقت_dirs.dirs, &موقت_dirs.n_dirs_alloc, 1, -1,
                              حجم *موقت_dirs.dirs);

  idx_t ثابت dir_idx = موقت_dirs.n_dirs++;
  موقت_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ساكن فراغ
zaptemp (حرف ثابت *name)
{
  بنية tempnode *متطاير *pnode;
  بنية tempnode *گره;
  بنية tempnode *بعدی;
  صحيح unlink_وضعیت;
  صحيح unlink_errno = 0;
  بنية cs_وضعیت cs;

  لكل (pnode = &temphead; (گره = *pnode)->name != name; pnode = &گره->بعدی)
    استمر;

  إذا (گره->state == UNREAPED)
    wait_proc (گره->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  بعدی = گره->بعدی;
  cs_enter (&cs);
  unlink_وضعیت = unlink (name);
  unlink_errno = errno;
  *pnode = بعدی;
  cs_leave (&cs);

  إذا (unlink_وضعیت != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  إذا (! بعدی)
    temptail = pnode;
  حرر (گره);
}

#إذا HAVE_NL_LANGINFO

ساكن صحيح
struct_month_cmp (فراغ ثابت *m1, فراغ ثابت *m2)
{
  بنية month ثابت *month1 = m1;
  بنية month ثابت *month2 = m2;
  أرجع strcmp (month1->name, month2->name);
}

#پایان_اگر

/* Initialize the character class tables. */

ساكن فراغ
inittables (فراغ)
{
  لكل (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! آیا_حرف_یا_رقم (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = به_بزرگ (i);
    }

#إذا HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  إذا (hard_LC_زمان)
    {
      لكل (size_t i = 0; i < MONTHS_در_هر_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = طول_رشته (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          لكل (j = k = 0; j < s_len; j++)
            إذا (! isblank (مقصد_uchar (s[j])))
              name[k++] = fold_toupper[مقصد_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_در_هر_YEAR, حجم *monthtab, struct_month_cmp);
    }
#پایان_اگر
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
  بیعلامت صحيح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  إذا (e == LONGINT_OK)
    {
      nmerge = n;
      إذا (nmerge != n)
        e = LONGINT_سرریز;
      وإلا
        {
          إذا (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (مرتب_سازی_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          وإلا إذا (max_nmerge < nmerge)
            {
              e = LONGINT_سرریز;
            }
          وإلا
            أرجع;
        }
    }

  إذا (e == LONGINT_سرریز)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (مرتب_سازی_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  وإلا
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ساكن فراغ
specify_مرتب_سازی_اندازه (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  حرف *suffix;
  تعداد strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  إذا (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_سرریز;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  إذا (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    بدل (suffix[0])
      {
      حالة 'b':
        e = LONGINT_OK;
        اكسر;

      حالة '%':
        {
          مزدوج mem = physmem_مجموع () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          إذا (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          وإلا
            e = LONGINT_سرریز;
        }
        اكسر;
      }

  إذا (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      إذا (n < مرتب_سازی_اندازه)
        أرجع;

      مرتب_سازی_اندازه = n;
      إذا (مرتب_سازی_اندازه == n)
        {
          مرتب_سازی_اندازه = MAX (مرتب_سازی_اندازه, MIN_مرتب_سازی_اندازه);
          أرجع;
        }

      e = LONGINT_سرریز;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ساكن size_t
specify_nthreads (صحيح oi, حرف c, حرف ثابت *s)
{
  uintmax_t nthreads;
  تعداد strtol_error e = xstrtoumax (s, عدم, 10, &nthreads, "");
  إذا (e == LONGINT_سرریز)
    أرجع SIZE_MAX;
  إذا (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  إذا (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  إذا (nthreads == 0)
    error (مرتب_سازی_FAILURE, 0, _("number in parallel must be nonzero"));
  أرجع nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ساكن size_t
default_مرتب_سازی_اندازه (فراغ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t اندازه = SIZE_MAX;
  بنية rlimit rlimit;
  إذا (getrlimit (RLIMIT_داده, &rlimit) == 0 && rlimit.rlim_cur < اندازه)
    اندازه = rlimit.rlim_cur;
#اگر_تعریف_شده RLIMIT_AS
  إذا (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < اندازه)
    اندازه = rlimit.rlim_cur;
#پایان_اگر

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  اندازه /= 2;

#اگر_تعریف_شده RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  إذا (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < اندازه)
    اندازه = rlimit.rlim_cur / 16 * 15;
#پایان_اگر

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  مزدوج avail = physmem_موجود ();
  مزدوج مجموع = physmem_مجموع ();
  مزدوج mem = MAX (avail, مجموع / 8);

  /* Leave a 1/4 margin for physical memory.  */
  إذا (مجموع * 0.75 < اندازه)
    اندازه = مجموع * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  إذا (mem < اندازه)
    اندازه = mem;
  أرجع MAX (اندازه, MIN_مرتب_سازی_اندازه);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساكن bool
سطر_aligned_اندازه (size_t *alloc)
{
  size_t اندازه = *alloc;
  size_t alignment = حجم (بنية سطر);
  size_t padding = alignment - اندازه % alignment;
  size_t aligned;

  إذا (ckd_جمع_کن (&aligned, اندازه, padding))
    أرجع false;

  *alloc = aligned;
  أرجع true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساكن size_t
default_initial_مرتب_سازی_اندازه (size_t سطر_بایتها)
{
  size_t اندازه = سطر_بایتها + 2;
  size_t درون_داد_اندازه;

  إذا (ckd_mul (&درون_داد_اندازه, درون_داد_FILE_اندازه_GUESS, سطر_بایتها + 1)
      || ckd_جمع_کن (&اندازه, اندازه, درون_داد_اندازه))
    أرجع SIZE_MAX;

  أرجع MAX (اندازه, MIN_مرتب_سازی_اندازه);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساكن bool
درون_داد_اندازه_میانگیر_بایتها (uintmax_t file_اندازه, size_t سطر_بایتها,
                         size_t *worst_case)
{
  size_t worst_case_در_هر_درون_داد_byte = سطر_بایتها + 1;

  إذا (SIZE_MAX / worst_case_در_هر_درون_داد_byte < file_اندازه)
    أرجع false;

  size_t اندازه = file_اندازه * worst_case_در_هر_درون_داد_byte;
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
مرتب_سازی_میانگیر_policy (FILE *ثابت *fps, size_t nfps,
                    حرف *ثابت *files, size_t nfiles,
                    size_t سطر_بایتها,
                    بنية مرتب_سازی_میانگیر_policy *policy)
{
  size_t اندازه_bound = مرتب_سازی_اندازه ? مرتب_سازی_اندازه : default_مرتب_سازی_اندازه ();
  size_t initial_bound =
    مرتب_سازی_اندازه ? اندازه_bound : default_initial_مرتب_سازی_اندازه (سطر_بایتها);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_در_هر_درون_داد_byte = سطر_بایتها + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t اندازه = worst_case_در_هر_درون_داد_byte + 1;

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
        مرتب_سازی_die (_("stat failed"), files[i]);

      معلوم_اندازه = usable_st_اندازه (&st) && 0 < st.st_اندازه;
      إذا (معلوم_اندازه)
        file_اندازه = st.st_اندازه;
      وإلا
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          إذا (مرتب_سازی_اندازه)
            {
              policy->initial = اندازه_bound;
              أرجع;
            }
          file_اندازه = درون_داد_FILE_اندازه_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      إذا (! درون_داد_اندازه_میانگیر_بایتها (file_اندازه, سطر_بایتها, &worst_case)
          || اندازه_bound - اندازه <= worst_case)
        {
          policy->initial = اندازه_bound;
          أرجع;
        }

      إذا (! معلوم_اندازه && ! مرتب_سازی_اندازه
          && (initial_bound <= اندازه || initial_bound - اندازه <= worst_case))
        {
          policy->initial = MAX (اندازه, initial_bound);
          أرجع;
        }

      اندازه += worst_case;
    }

  policy->initial = MAX (اندازه, MIN_مرتب_سازی_اندازه);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساكن فراغ
initbuf (بنية میانگیر *buf, size_t سطر_بایتها, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  طالما (true)
    {
      إذا (! سطر_aligned_اندازه (&alloc))
        buf->buf = عدم;
      وإلا
        buf->buf = حافظه_بده (alloc);
      إذا (buf->buf)
        اكسر;
      alloc /= 2;
      إذا (alloc <= سطر_بایتها + 1)
        xalloc_die ();
    }

  buf->سطر_بایتها = سطر_بایتها;
  buf->alloc = alloc;
  buf->استفاده_شده = buf->چپ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساكن مضمن بنية سطر *
میانگیر_linelim (بنية میانگیر ثابت *buf)
{
  فراغ *linelim = buf->buf + buf->alloc;
  أرجع linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ساكن bool
try_growbuf (بنية میانگیر *buf, size_t alloc)
{
  إذا (! سطر_aligned_اندازه (&alloc) || alloc <= buf->alloc)
    أرجع false;

  حرف *newbuf = حافظه_بده (alloc);
  إذا (! newbuf)
    أرجع false;

  حرف *oldbuf = buf->buf;
  بنية سطر *old_linelim = میانگیر_linelim (buf);
  بنية سطر *old_سطر = old_linelim - buf->nlines;

  کپی_حافظه (newbuf, oldbuf, buf->استفاده_شده);

  بنية سطر *new_linelim = (فراغ *) (newbuf + alloc);
  بنية سطر *new_سطر = new_linelim - buf->nlines;
  کپی_حافظه (new_سطر, old_سطر, buf->nlines * حجم *new_سطر);

  لكل (بنية سطر *سطر = new_سطر; سطر < new_linelim; سطر++)
    {
      سطر->متن = newbuf + (سطر->متن - oldbuf);
      إذا (keylist)
        {
          سطر->keybeg = newbuf + (سطر->keybeg - oldbuf);
          سطر->keylim = newbuf + (سطر->keylim - oldbuf);
        }
    }

  حرر (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  أرجع true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساكن bool
maybe_growbuf (بنية میانگیر *buf, بنية مرتب_سازی_میانگیر_policy *policy)
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
begfield (بنية سطر ثابت *سطر, بنية keyfield ثابت *کلید)
{
  حرف *ptr = سطر->متن, *lim = ptr + سطر->درازی - 1;
  size_t sword = کلید->sword;
  size_t schar = کلید->schar;

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
        طالما (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[مقصد_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  إذا (کلید->skipsblanks)
    طالما (ptr < lim && blanks[مقصد_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t مانده_بایتها = lim - ptr;
  إذا (schar < مانده_بایتها)
    ptr += schar;
  وإلا
    ptr = lim;

  أرجع ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_خالص
ساكن حرف *
limfield (بنية سطر ثابت *سطر, بنية keyfield ثابت *کلید)
{
  حرف *ptr = سطر->متن, *lim = ptr + سطر->درازی - 1;
  size_t eword = کلید->eword, echar = کلید->echar;

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
        طالما (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;
        طالما (ptr < lim && !blanks[مقصد_uchar (*ptr)])
          ++ptr;
      }

#اگر_تعریف_شده POSIX_UNSPECIFIED
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
      طالما (newlim < lim && blanks[مقصد_uchar (*newlim)])
        ++newlim;
      طالما (newlim < lim && !blanks[مقصد_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#پایان_اگر

  إذا (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      إذا (کلید->skipeblanks)
        طالما (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t مانده_بایتها = lim - ptr;
      إذا (echar < مانده_بایتها)
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
fillbuf (بنية میانگیر *buf, FILE *fp, حرف ثابت *file,
         بنية مرتب_سازی_میانگیر_policy *policy)
{
  بنية keyfield ثابت *کلید = keylist;
  حرف eol = eolchar;
  size_t سطر_بایتها = buf->سطر_بایتها;
  size_t mergesize = ادغام_میانگیر_اندازه - MIN_ادغام_میانگیر_اندازه;

  إذا (buf->eof)
    أرجع false;

  إذا (buf->استفاده_شده != buf->چپ)
    {
      memmove (buf->buf, buf->buf + buf->استفاده_شده - buf->چپ, buf->چپ);
      buf->استفاده_شده = buf->چپ;
      buf->nlines = 0;
    }

  طالما (true)
    {
      حرف *ptr = buf->buf + buf->استفاده_شده;
      بنية سطر *linelim = میانگیر_linelim (buf);
      بنية سطر *سطر = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * سطر_بایتها - ptr;
      حرف *سطر_شروع = buf->nlines ? سطر->متن + سطر->درازی : buf->buf;

      طالما (سطر_بایتها + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (سطر_بایتها + 1);
          size_t بایتها_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + بایتها_read;
          حرف *p;
          avail -= بایتها_read;

          إذا (بایتها_read != readsize)
            {
              إذا (ferror (fp))
                مرتب_سازی_die (_("read failed"), file);
              إذا (feof (fp))
                {
                  buf->eof = true;
                  إذا (buf->buf == ptrlim)
                    أرجع false;
                  إذا (سطر_شروع != ptrlim && ptrlim[-1] != eol)
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
              سطر--;
              سطر->متن = سطر_شروع;
              سطر->درازی = ptr - سطر_شروع;
              mergesize = MAX (mergesize, سطر->درازی);
              avail -= سطر_بایتها;

              إذا (کلید)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  سطر->keylim = (کلید->eword == SIZE_MAX
                                  ? p
                                  : limfield (سطر, کلید));

                  إذا (کلید->sword != SIZE_MAX)
                    سطر->keybeg = begfield (سطر, کلید);
                  وإلا
                    {
                      إذا (کلید->skipsblanks)
                        طالما (blanks[مقصد_uchar (*سطر_شروع)])
                          سطر_شروع++;
                      سطر->keybeg = سطر_شروع;
                    }
                }

              سطر_شروع = ptr;
            }

          ptr = ptrlim;
          إذا (buf->eof)
            اكسر;
        }

      buf->استفاده_شده = ptr - buf->buf;
      buf->nlines = میانگیر_linelim (buf) - سطر;
      إذا (buf->nlines != 0)
        {
          إذا (! buf->eof && maybe_growbuf (buf, policy))
            استمر;
          buf->چپ = ptr - سطر_شروع;
          ادغام_میانگیر_اندازه = mergesize + MIN_ادغام_میانگیر_اندازه;
          أرجع true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t سطر_alloc = buf->alloc / حجم (بنية سطر);
        buf->buf = xpalloc (buf->buf, &سطر_alloc, 1, -1, حجم (بنية سطر));
        buf->alloc = سطر_alloc * حجم (بنية سطر);
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
#پایان_اگر
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
  bool دوسر_with_thousands_sep = false;

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
      دوسر_with_thousands_sep = (*p == thousands_sep);
      إذا (دوسر_with_thousands_sep)
        ++p;
    }

  إذا (دوسر_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *عدد = p - 2;
      أرجع max_digit;
    }

  إذا (ch == decimal_نقطه)
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
پیدا_کن_unit_ترتیب (حرف ثابت *عدد)
{
  bool minus_sign = (*عدد == '-');
  حرف ثابت *p = عدد + minus_sign;
  حرف max_digit = traverse_raw_عدد (&p);
  إذا ('0' < max_digit)
    {
      بیعلامت حرف ch = *p;
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
  طالما (blanks[مقصد_uchar (*a)])
    a++;
  طالما (blanks[مقصد_uchar (*b)])
    b++;

  صحيح diff = پیدا_کن_unit_ترتیب (a) - پیدا_کن_unit_ترتیب (b);
  أرجع (diff ? diff : strnumcmp (a, b, decimal_نقطه, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_خالص
ساكن صحيح
numcompare (حرف ثابت *a, حرف ثابت *b)
{
  طالما (blanks[مقصد_uchar (*a)])
    a++;
  طالما (blanks[مقصد_uchar (*b)])
    b++;

  أرجع strnumcmp (a, b, decimal_نقطه, thousands_sep);
}

ساكن صحيح
nan_compare (دراز مزدوج a, دراز مزدوج b)
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
  دراز مزدوج a = strtold (sa, &ea);
  دراز مزدوج b = strtold (sb, &eb);

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
  size_t hi = MONTHS_در_هر_YEAR;

  طالما (blanks[مقصد_uchar (*month)])
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
          إذا (مقصد_uchar (fold_toupper[مقصد_uchar (*m)]) < مقصد_uchar (*n))
            {
              hi = ix;
              اكسر;
            }
          وإلا إذا (مقصد_uchar (fold_toupper[مقصد_uchar (*m)]) > مقصد_uchar (*n))
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
#  pragma GCC diagnostic افزودن
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#پایان_اگر

#ضمن "md5.h"

#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# إذا 14 <= __GNUC__
#  pragma GCC diagnostic برداشت
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_اولویت_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ساكن فراغ
link_failure (فراغ)
{
  error (مرتب_سازی_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ساكن فراغ *
نماد_آدرس (فراغ *handle, حرف ثابت *نماد)
{
  فراغ *آدرس = dlsym (handle, نماد);
  إذا (!آدرس)
    link_failure ();
  أرجع آدرس;
}
#پایان_اگر

/* Dynamically link the crypto library, if it needs linking.  */
ساكن فراغ
link_libcrypto (فراغ)
{
#إذا DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  فراغ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  إذا (!handle)
    link_failure ();
  ptr_MD5_Init = نماد_آدرس (handle, "MD5_Init");
  ptr_MD5_Update = نماد_آدرس (handle, "MD5_Update");
  ptr_MD5_Final = نماد_آدرس (handle, "MD5_Final");
#پایان_اگر
}

/* A randomly chosen MD5 state, used for random comparison.  */
ساكن بنية md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ساكن فراغ
random_md5_state_init (حرف ثابت *random_منبع)
{
  بیعلامت حرف buf[MD5_DIGEST_اندازه];
  بنية randread_منبع *r = randread_new (random_منبع, حجم buf);
  إذا (! r)
    مرتب_سازی_die (_("open failed"), random_منبع ? random_منبع : "getrandom");
  randread (r, buf, حجم buf);
  إذا (randread_free (r) != 0)
    مرتب_سازی_die (_("close failed"), random_منبع);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_بایتها (buf, حجم buf, &random_md5_state);
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
      error (مرتب_سازی_FAILURE, 0,
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
              buf = allocated = حافظه_بده (bufsize);
              إذا (! buf)
                {
                  buf = stackbuf;
                  bufsize = حجم stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_میگنجد = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_میگنجد ? buf + sizea : عدم), textb,
                          (a_میگنجد ? bufsize - sizea : 0))
                + 1)
             : 0);

          إذا (! (a_میگنجد && sizea + sizeb <= bufsize))
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
            texta += طول_رشته (texta) + 1;
          إذا (textb < limb)
            textb += طول_رشته (textb) + 1;
          إذا (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              اكسر;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_بایتها (buf, sizea, &s[0]);
          md5_process_بایتها (buf + sizea, sizeb, &s[1]);

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
  md5_process_بایتها (texta, lena, &s[0]); md5_پایان_ctx (&s[0], dig[0]);
  md5_process_بایتها (textb, lenb, &s[1]); md5_پایان_ctx (&s[1], dig[1]);
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
debug_عرض (حرف ثابت *متن, حرف ثابت *lim)
{
  size_t عرض = mbsnwidth (متن, lim - متن, 0);
  طالما (متن < lim)
    عرض += (*متن++ == '\t');
  أرجع عرض;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساكن فراغ
mark_کلید (size_t جابه_جایی, size_t عرض)
{
  طالما (جابه_جایی--)
    حرف_چاپ_کن (' ');

  إذا (!عرض)
    چاپ_کن (_("^ no match for key\n"));
  وإلا
    {
      افعل
        حرف_چاپ_کن ('_');
      طالما (--عرض);

      حرف_چاپ_کن ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساكن مضمن bool
کلید_numeric (بنية keyfield ثابت *کلید)
{
  أرجع کلید->numeric || کلید->general_numeric || کلید->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساكن فراغ
debug_کلید (بنية سطر ثابت *سطر, بنية keyfield ثابت *کلید)
{
  حرف *متن = سطر->متن;
  حرف *beg = متن;
  حرف *lim = متن + سطر->درازی - 1;

  إذا (کلید)
    {
      إذا (کلید->sword != SIZE_MAX)
        beg = begfield (سطر, کلید);
      إذا (کلید->eword != SIZE_MAX)
        {
          lim = limfield (سطر, کلید);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      إذا ((کلید->skipsblanks && کلید->sword == SIZE_MAX)
          || کلید->month || کلید_numeric (کلید))
        {
          حرف saved = *lim;
          *lim = '\0';

          طالما (blanks[مقصد_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          إذا (lim < beg)
            tighter_lim = lim;
          وإلا إذا (کلید->month)
            getmonth (beg, &tighter_lim);
          وإلا إذا (کلید->general_numeric)
            ignore_مقدار (strtold (beg, &tighter_lim));
          وإلا إذا (کلید->numeric || کلید->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_عدد (&p);
              إذا ('0' <= max_digit)
                {
                  بیعلامت حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (کلید->human_numeric && unit_ترتیب[ch]);
                }
            }
          وإلا
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t جابه_جایی = debug_عرض (متن, beg);
  size_t عرض = debug_عرض (beg, lim);
  mark_کلید (جابه_جایی, عرض);
}

/* Debug LINE by underlining its keys.  */

ساكن فراغ
debug_سطر (بنية سطر ثابت *سطر)
{
  بنية keyfield ثابت *کلید = keylist;

  افعل
    debug_کلید (سطر, کلید);
  طالما (کلید && ((کلید = کلید->بعدی) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساكن bool
default_کلید_compare (بنية keyfield ثابت *کلید)
{
  أرجع ! (کلید->ignore
            || کلید->translate
            || کلید->skipsblanks
            || کلید->skipeblanks
            || کلید_numeric (کلید)
            || کلید->month
            || کلید->نسخه_3
            || کلید->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساكن فراغ
کلید_مقصد_opts (بنية keyfield ثابت *کلید, حرف *opts)
{
  إذا (کلید->skipsblanks || کلید->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  إذا (کلید->ignore == nondictionary)
    *opts++ = 'd';
  إذا (کلید->translate)
    *opts++ = 'f';
  إذا (کلید->general_numeric)
    *opts++ = 'g';
  إذا (کلید->human_numeric)
    *opts++ = 'h';
  إذا (کلید->ignore == nonprinting)
    *opts++ = 'i';
  إذا (کلید->month)
    *opts++ = 'M';
  إذا (کلید->numeric)
    *opts++ = 'n';
  إذا (کلید->random)
    *opts++ = 'R';
  إذا (کلید->معکوس)
    *opts++ = 'r';
  إذا (کلید->نسخه_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساكن فراغ
کلید_warnings (بنية keyfield ثابت *gkey, bool gkey_only)
{
  بنية keyfield ugkey = *gkey;
  بیعلامت دراز keynum = 1;
  bool basic_numeric_فیلد = false;
  bool general_numeric_فیلد = false;
  bool basic_numeric_فیلد_بازه = false;
  bool general_numeric_فیلد_بازه = false;

  لكل (بنية keyfield *کلید = keylist; کلید; کلید = کلید->بعدی, keynum++)
    {
      إذا (کلید_numeric (کلید))
        {
          إذا (کلید->general_numeric)
            general_numeric_فیلد = true;
          وإلا
            basic_numeric_فیلد = true;
        }

      إذا (کلید->traditional_استفاده_شده)
        {
          size_t sword = کلید->sword;
          size_t eword = کلید->eword;
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
          إذا (کلید->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (کلید->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_عرض = کلید->sword != SIZE_MAX && کلید->eword < کلید->sword;
      إذا (zero_عرض)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = کلید_numeric (کلید) || کلید->month;
      bool سطر_جابه_جایی = کلید->eword == 0 && کلید->echar != 0; /* -k1.x,1.y  */
      إذا (!zero_عرض && !gkey_only && tab == TAB_DEFAULT && !سطر_جابه_جایی
          && ((!کلید->skipsblanks && !implicit_skip)
              || (!کلید->skipsblanks && کلید->schar)
              || (!کلید->skipeblanks && کلید->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      إذا (!gkey_only && کلید_numeric (کلید))
        {
          size_t sword = کلید->sword + 1;
          size_t eword = کلید->eword + 1;
          إذا (!sword)
            sword++;
          إذا (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              إذا (کلید->general_numeric)
                general_numeric_فیلد_بازه = true;
              وإلا
                basic_numeric_فیلد_بازه = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      إذا (ugkey.ignore && (ugkey.ignore == کلید->ignore))
        ugkey.ignore = عدم;
      إذا (ugkey.translate && (ugkey.translate == کلید->translate))
        ugkey.translate = عدم;
      ugkey.skipsblanks &= !کلید->skipsblanks;
      ugkey.skipeblanks &= !کلید->skipeblanks;
      ugkey.month &= !کلید->month;
      ugkey.numeric &= !کلید->numeric;
      ugkey.general_numeric &= !کلید->general_numeric;
      ugkey.human_numeric &= !کلید->human_numeric;
      ugkey.random &= !کلید->random;
      ugkey.نسخه_3 &= !کلید->نسخه_3;
      ugkey.معکوس &= !کلید->معکوس;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool عدد_locale_warned = false;
  إذا (basic_numeric_فیلد_بازه)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (مقصد_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          عدد_locale_warned = true;
        }
    }
  إذا (basic_numeric_فیلد_بازه || general_numeric_فیلد_بازه)
    {
      إذا (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (مقصد_uchar (decimal_نقطه)))
          : tab == decimal_نقطه)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_نقطه, 0})));
          عدد_locale_warned = true;
        }
      وإلا إذا (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      وإلا إذا (general_numeric_فیلد_بازه && tab == '+')
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
  إذا ((basic_numeric_فیلد || general_numeric_فیلد) && ! عدد_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_نقطه, 0})));

    }

  إذا (basic_numeric_فیلد && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  إذا (!default_کلید_compare (&ugkey)
      || (ugkey.معکوس && (stable || unique) && keylist))
    {
      bool ugkey_معکوس = ugkey.معکوس;
      إذا (!(stable || unique))
        ugkey.معکوس = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      کلید_مقصد_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (طول_رشته (opts))), opts);
      ugkey.معکوس = ugkey_معکوس;
    }
  إذا (ugkey.معکوس && !(stable || unique) && keylist)
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
keycompare (بنية سطر ثابت *a, بنية سطر ثابت *b)
{
  بنية keyfield *کلید = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحيح diff;

  طالما (true)
    {
      حرف ثابت *translate = کلید->translate;
      bool ثابت *ignore = کلید->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      إذا (hard_LC_COLLATE || کلید_numeric (کلید)
          || کلید->month || کلید->random || کلید->نسخه_3)
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
                إذا (! (ignore && ignore[مقصد_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[مقصد_uchar (texta[i])]
                                 : texta[i]);

              لكل (tlenb = i = 0; i < lenb; i++)
                إذا (! (ignore && ignore[مقصد_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[مقصد_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          إذا (کلید->numeric)
            diff = numcompare (ta, tb);
          وإلا إذا (کلید->general_numeric)
            diff = general_numcompare (ta, tb);
          وإلا إذا (کلید->human_numeric)
            diff = human_numcompare (ta, tb);
          وإلا إذا (کلید->month)
            diff = getmonth (ta, عدم) - getmonth (tb, عدم);
          وإلا إذا (کلید->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          وإلا إذا (کلید->نسخه_3)
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
#تعریف_کن CMP_WITH_IGNORE(A, B)						\
  افعل									\
    {									\
          طالما (true)							\
            {								\
              طالما (texta < lima && ignore[مقصد_uchar (*texta)])		\
                ++texta;						\
              طالما (textb < limb && ignore[مقصد_uchar (*textb)])		\
                ++textb;						\
              إذا (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  اكسر;						\
                }							\
              diff = مقصد_uchar (A) - مقصد_uchar (B);			\
              إذا (diff)							\
                اكسر;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  طالما (0)

          إذا (translate)
            CMP_WITH_IGNORE (translate[مقصد_uchar (*texta)],
                             translate[مقصد_uchar (*textb)]);
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
                  diff = (مقصد_uchar (translate[مقصد_uchar (texta[i])])
                          - مقصد_uchar (translate[مقصد_uchar (textb[i])]));
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

      کلید = کلید->بعدی;
      إذا (! کلید)
        أرجع 0;

      /* Find the beginning and limit of the next field.  */
      إذا (کلید->eword != SIZE_MAX)
        lima = limfield (a, کلید), limb = limfield (b, کلید);
      وإلا
        lima = a->متن + a->درازی - 1, limb = b->متن + b->درازی - 1;

      إذا (کلید->sword != SIZE_MAX)
        texta = begfield (a, کلید), textb = begfield (b, کلید);
      وإلا
        {
          texta = a->متن, textb = b->متن;
          إذا (کلید->skipsblanks)
            {
              طالما (texta < lima && blanks[مقصد_uchar (*texta)])
                ++texta;
              طالما (textb < limb && blanks[مقصد_uchar (*textb)])
                ++textb;
            }
        }
    }

  أرجع diff_reversed (diff, کلید->معکوس);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساكن صحيح
compare (بنية سطر ثابت *a, بنية سطر ثابت *b)
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
  alen = a->درازی - 1, blen = b->درازی - 1;

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

  أرجع diff_reversed (diff, معکوس);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساكن فراغ
write_سطر (بنية سطر ثابت *سطر, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = سطر->متن;
  size_t n_بایتها = سطر->درازی;
  حرف *ebuf = buf + n_بایتها;

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
            مرتب_سازی_die (_("write failed"), output_file);
        }

      debug_سطر (سطر);
    }
  وإلا
    {
      ebuf[-1] = eolchar;
      إذا (fwrite (buf, 1, n_بایتها, fp) != n_بایتها)
        مرتب_سازی_die (_("write failed"), output_file);
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
  بنية میانگیر buf;		/* Input buffer. */
  بنية سطر موقت;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t سطر_عدد = 0;
  بنية keyfield ثابت *کلید = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (بنية سطر),
           MAX (ادغام_میانگیر_اندازه, مرتب_سازی_اندازه));
  موقت.متن = عدم;

  طالما (fillbuf (&buf, fp, file_name, عدم))
    {
      بنية سطر ثابت *سطر = میانگیر_linelim (&buf);
      بنية سطر ثابت *linebase = سطر - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      إذا (alloc && nonunique <= compare (&موقت, سطر - 1))
        {
        یافت_disorder:
          {
            إذا (checkonly == 'c')
              {
                بنية سطر ثابت *disorder_سطر = سطر - 1;
                uintmax_t disorder_سطر_عدد =
                  میانگیر_linelim (&buf) - disorder_سطر + سطر_عدد;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         برنامه_name, file_name, disorder_سطر_عدد);
                write_سطر (disorder_سطر, stderr, _("standard error"));
              }

            ordered = false;
            اكسر;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      طالما (linebase < --سطر)
        إذا (nonunique <= compare (سطر, سطر - 1))
          اذهب یافت_disorder;

      سطر_عدد += buf.nlines;

      /* Save the last line of the buffer.  */
      إذا (alloc < سطر->درازی)
        {
          افعل
            {
              alloc *= 2;
              إذا (! alloc)
                {
                  alloc = سطر->درازی;
                  اكسر;
                }
            }
          طالما (alloc < سطر->درازی);

          حرر (موقت.متن);
          موقت.متن = xmalloc (alloc);
        }
      کپی_حافظه (موقت.متن, سطر->متن, سطر->درازی);
      موقت.درازی = سطر->درازی;
      إذا (کلید)
        {
          موقت.keybeg = موقت.متن + (سطر->keybeg - سطر->متن);
          موقت.keylim = موقت.متن + (سطر->keylim - سطر->متن);
        }
    }

  xfclose (fp, file_name);
  حرر (buf.buf);
  حرر (موقت.متن);
  أرجع ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساكن size_t
open_درون_داد_files (بنية sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحيح i;

  /* Open as many input files as we can.  */
  لكل (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].موقت && files[i].موقت->state != UNCOMPRESSED
                ? open_موقت (files[i].موقت)
                : جریان_open (files[i].name, "r"));
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
  بنية میانگیر *میانگیر = xnmalloc (nfiles, حجم *میانگیر);
                                /* Input buffers for each file. */
  بنية سطر saved;		/* Saved line storage for unique check. */
  بنية سطر ثابت *savedline = عدم;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  بنية سطر ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  بنية سطر ثابت **پایه_2 = xnmalloc (nfiles, حجم *پایه_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  بنية keyfield ثابت *کلید = keylist;
  saved.متن = عدم;

  /* Read initial lines from each input file. */
  لكل (size_t i = 0; i < nfiles; )
    {
      initbuf (&میانگیر[i], حجم (بنية سطر),
               MAX (ادغام_میانگیر_اندازه, مرتب_سازی_اندازه / nfiles));
      إذا (fillbuf (&میانگیر[i], fps[i], files[i].name, عدم))
        {
          بنية سطر ثابت *linelim = میانگیر_linelim (&میانگیر[i]);
          cur[i] = linelim - 1;
          پایه_2[i] = linelim - میانگیر[i].nlines;
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
          حرر (میانگیر[i].buf);
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
      بنية سطر ثابت *کوچکترین = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      إذا (unique)
        {
          إذا (savedline && compare (savedline, کوچکترین))
            {
              savedline = عدم;
              write_سطر (&saved, ofp, output_file);
            }
          إذا (!savedline)
            {
              savedline = &saved;
              إذا (savealloc < کوچکترین->درازی)
                {
                  افعل
                    إذا (! savealloc)
                      {
                        savealloc = کوچکترین->درازی;
                        اكسر;
                      }
                  طالما ((savealloc *= 2) < کوچکترین->درازی);

                  حرر (saved.متن);
                  saved.متن = xmalloc (savealloc);
                }
              saved.درازی = کوچکترین->درازی;
              کپی_حافظه (saved.متن, کوچکترین->متن, saved.درازی);
              إذا (کلید)
                {
                  saved.keybeg =
                    saved.متن + (کوچکترین->keybeg - کوچکترین->متن);
                  saved.keylim =
                    saved.متن + (کوچکترین->keylim - کوچکترین->متن);
                }
            }
        }
      وإلا
        write_سطر (کوچکترین, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      إذا (پایه_2[ord[0]] < کوچکترین)
        cur[ord[0]] = کوچکترین - 1;
      وإلا
        {
          إذا (fillbuf (&میانگیر[ord[0]], fps[ord[0]], files[ord[0]].name,
                       عدم))
            {
              بنية سطر ثابت *linelim = میانگیر_linelim (&میانگیر[ord[0]]);
              cur[ord[0]] = linelim - 1;
              پایه_2[ord[0]] = linelim - میانگیر[ord[0]].nlines;
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
              حرر (میانگیر[ord[0]].buf);
              لكل (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  میانگیر[i] = میانگیر[i + 1];
                  cur[i] = cur[i + 1];
                  پایه_2[i] = پایه_2[i + 1];
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
        size_t شمار_از_smaller_سطرها;

        طالما (lo < hi)
          {
            صحيح cmp = compare (cur[ord0], cur[ord[probe]]);
            إذا (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            وإلا
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        شمار_از_smaller_سطرها = lo - 1;
        لكل (size_t j = 0; j < شمار_از_smaller_سطرها; j++)
          ord[j] = ord[j + 1];
        ord[شمار_از_smaller_سطرها] = ord0;
      }
    }

  إذا (unique && savedline)
    {
      write_سطر (&saved, ofp, output_file);
      حرر (saved.متن);
    }

  xfclose (ofp, output_file);
  حرر (fps);
  حرر (میانگیر);
  حرر (ord);
  حرر (پایه_2);
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
  size_t nopened = open_درون_داد_files (files, nfiles, &fps);
  إذا (nopened < nfiles && nopened < 2)
    مرتب_سازی_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  أرجع nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساكن فراغ
mergelines (بنية سطر *محدود t, size_t nlines,
            بنية سطر ثابت *محدود lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية سطر *hi = t - nlo;

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
sequential_مرتب_سازی (بنية سطر *محدود سطرها, size_t nlines,
                 بنية سطر *محدود موقت, bool مقصد_موقت)
{
  إذا (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحيح جابجا = (0 < compare (&سطرها[-1], &سطرها[-2]));
      إذا (مقصد_موقت)
        {
          موقت[-1] = سطرها[-1 - جابجا];
          موقت[-2] = سطرها[-2 + جابجا];
        }
      وإلا إذا (جابجا)
        {
          موقت[-1] = سطرها[-1];
          سطرها[-1] = سطرها[-2];
          سطرها[-2] = موقت[-1];
        }
    }
  وإلا
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      بنية سطر *lo = سطرها;
      بنية سطر *hi = سطرها - nlo;

      sequential_مرتب_سازی (hi, nhi, موقت - (مقصد_موقت ? nlo : 0), مقصد_موقت);
      إذا (1 < nlo)
        sequential_مرتب_سازی (lo, nlo, موقت, !مقصد_موقت);
      وإلا إذا (!مقصد_موقت)
        موقت[-1] = lo[-1];

      بنية سطر *dest;
      بنية سطر ثابت *sorted_lo;
      إذا (مقصد_موقت)
        {
          dest = موقت;
          sorted_lo = سطرها;
        }
      وإلا
        {
          dest = سطرها;
          sorted_lo = موقت;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساكن بنية ادغام_گره *init_گره (بنية ادغام_گره *محدود,
                                     بنية ادغام_گره *محدود,
                                     بنية سطر *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساكن بنية ادغام_گره *
ادغام_درخت_init (size_t nthreads, size_t nlines, بنية سطر *dest)
{
  بنية ادغام_گره *ادغام_درخت = xmalloc (2 * حجم *ادغام_درخت * nthreads);

  بنية ادغام_گره *ریشه = ادغام_درخت;
  ریشه->lo = ریشه->hi = ریشه->انتها_lo = ریشه->انتها_hi = عدم;
  ریشه->dest = عدم;
  ریشه->nlo = ریشه->nhi = nlines;
  ریشه->والد = عدم;
  ریشه->level = ادغام_انتها;
  ریشه->queued = false;
  pthread_mutex_init (&ریشه->قفل, عدم);

  init_گره (ریشه, ریشه + 1, dest, nthreads, nlines, false);
  أرجع ادغام_درخت;
}

/* Destroy the merge tree. */
ساكن فراغ
ادغام_درخت_destroy (size_t nthreads, بنية ادغام_گره *ادغام_درخت)
{
  size_t n_nodes = nthreads * 2;
  بنية ادغام_گره *گره = ادغام_درخت;

  طالما (n_nodes--)
    {
      pthread_mutex_destroy (&گره->قفل);
      گره++;
    }

  حرر (ادغام_درخت);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساكن بنية ادغام_گره *
init_گره (بنية ادغام_گره *محدود والد,
           بنية ادغام_گره *محدود گره_pool,
           بنية سطر *dest, size_t nthreads,
           size_t مجموع_سطرها, bool is_lo_فرزند)
{
  size_t nlines = (is_lo_فرزند ? والد->nlo : والد->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  بنية سطر *lo = dest - مجموع_سطرها;
  بنية سطر *hi = lo - nlo;
  بنية سطر **والد_انتها = (is_lo_فرزند ? &والد->انتها_lo : &والد->انتها_hi);

  بنية ادغام_گره *گره = گره_pool++;
  گره->lo = گره->انتها_lo = lo;
  گره->hi = گره->انتها_hi = hi;
  گره->dest = والد_انتها;
  گره->nlo = nlo;
  گره->nhi = nhi;
  گره->والد = والد;
  گره->level = والد->level + 1;
  گره->queued = false;
  pthread_mutex_init (&گره->قفل, عدم);

  إذا (nthreads > 1)
    {
      size_t lo_نخها = nthreads / 2;
      size_t hi_نخها = nthreads - lo_نخها;
      گره->lo_فرزند = گره_pool;
      گره_pool = init_گره (گره, گره_pool, lo, lo_نخها,
                             مجموع_سطرها, true);
      گره->hi_فرزند = گره_pool;
      گره_pool = init_گره (گره, گره_pool, hi, hi_نخها,
                             مجموع_سطرها, false);
    }
  وإلا
    {
      گره->lo_فرزند = عدم;
      گره->hi_فرزند = عدم;
    }
  أرجع گره_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساكن صحيح
compare_nodes (فراغ ثابت *a, فراغ ثابت *b)
{
  بنية ادغام_گره ثابت *nodea = a;
  بنية ادغام_گره ثابت *nodeb = b;
  إذا (nodea->level == nodeb->level)
      أرجع (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  أرجع nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساكن مضمن فراغ
قفل_گره (بنية ادغام_گره *گره)
{
  pthread_mutex_قفل (&گره->قفل);
}

/* Unlock a merge tree NODE. */

ساكن مضمن فراغ
unlock_گره (بنية ادغام_گره *گره)
{
  pthread_mutex_unlock (&گره->قفل);
}

/* Destroy merge QUEUE. */

ساكن فراغ
صف_destroy (بنية ادغام_گره_صف *صف)
{
  انباشته_free (صف->اولویت_صف);
  pthread_cond_destroy (&صف->cond);
  pthread_mutex_destroy (&صف->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساكن فراغ
صف_init (بنية ادغام_گره_صف *صف, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  صف->اولویت_صف = انباشته_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&صف->mutex, عدم);
  pthread_cond_init (&صف->cond, عدم);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ساكن فراغ
صف_درج (بنية ادغام_گره_صف *صف, بنية ادغام_گره *گره)
{
  pthread_mutex_قفل (&صف->mutex);
  انباشته_درج (صف->اولویت_صف, گره);
  گره->queued = true;
  pthread_cond_signal (&صف->cond);
  pthread_mutex_unlock (&صف->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساكن بنية ادغام_گره *
صف_برداشت (بنية ادغام_گره_صف *صف)
{
  بنية ادغام_گره *گره;
  pthread_mutex_قفل (&صف->mutex);
  طالما (! (گره = انباشته_remove_بالاترین (صف->اولویت_صف)))
    pthread_cond_wait (&صف->cond, &صف->mutex);
  pthread_mutex_unlock (&صف->mutex);
  قفل_گره (گره);
  گره->queued = false;
  أرجع گره;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساكن فراغ
write_unique (بنية سطر ثابت *سطر, FILE *tfp, حرف ثابت *موقت_output)
{
  إذا (unique)
    {
      إذا (saved_سطر.متن && ! compare (سطر, &saved_سطر))
        أرجع;
      saved_سطر = *سطر;
    }

  write_سطر (سطر, tfp, موقت_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساكن فراغ
mergelines_گره (بنية ادغام_گره *محدود گره, size_t مجموع_سطرها,
                 FILE *tfp, حرف ثابت *موقت_output)
{
  بنية سطر *lo_orig = گره->lo;
  بنية سطر *hi_orig = گره->hi;
  size_t مقصد_ادغام = MAX_ادغام (مجموع_سطرها, گره->level);
  size_t merged_lo;
  size_t merged_hi;

  إذا (گره->level > ادغام_ریشه)
    {
      /* Merge to destination buffer. */
      بنية سطر *dest = *گره->dest;
      طالما (گره->lo != گره->انتها_lo && گره->hi != گره->انتها_hi && مقصد_ادغام--)
        إذا (compare (گره->lo - 1, گره->hi - 1) <= 0)
          *--dest = *--گره->lo;
        وإلا
          *--dest = *--گره->hi;

      merged_lo = lo_orig - گره->lo;
      merged_hi = hi_orig - گره->hi;

      إذا (گره->nhi == merged_hi)
        طالما (گره->lo != گره->انتها_lo && مقصد_ادغام--)
          *--dest = *--گره->lo;
      وإلا إذا (گره->nlo == merged_lo)
        طالما (گره->hi != گره->انتها_hi && مقصد_ادغام--)
          *--dest = *--گره->hi;
      *گره->dest = dest;
    }
  وإلا
    {
      /* Merge directly to output. */
      طالما (گره->lo != گره->انتها_lo && گره->hi != گره->انتها_hi && مقصد_ادغام--)
        {
          إذا (compare (گره->lo - 1, گره->hi - 1) <= 0)
            write_unique (--گره->lo, tfp, موقت_output);
          وإلا
            write_unique (--گره->hi, tfp, موقت_output);
        }

      merged_lo = lo_orig - گره->lo;
      merged_hi = hi_orig - گره->hi;

      إذا (گره->nhi == merged_hi)
        {
          طالما (گره->lo != گره->انتها_lo && مقصد_ادغام--)
            write_unique (--گره->lo, tfp, موقت_output);
        }
      وإلا إذا (گره->nlo == merged_lo)
        {
          طالما (گره->hi != گره->انتها_hi && مقصد_ادغام--)
            write_unique (--گره->hi, tfp, موقت_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - گره->lo;
  merged_hi = hi_orig - گره->hi;
  گره->nlo -= merged_lo;
  گره->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساكن فراغ
صف_check_درج (بنية ادغام_گره_صف *صف, بنية ادغام_گره *گره)
{
  إذا (! گره->queued)
    {
      bool lo_avail = (گره->lo - گره->انتها_lo) != 0;
      bool hi_avail = (گره->hi - گره->انتها_hi) != 0;
      إذا (lo_avail ? hi_avail || ! گره->nhi : hi_avail && ! گره->nlo)
        صف_درج (صف, گره);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساكن فراغ
صف_check_درج_والد (بنية ادغام_گره_صف *صف,
                           بنية ادغام_گره *گره)
{
  إذا (گره->level > ادغام_ریشه)
    {
      قفل_گره (گره->والد);
      صف_check_درج (صف, گره->والد);
      unlock_گره (گره->والد);
    }
  وإلا إذا (گره->nlo + گره->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      صف_درج (صف, گره->والد);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساكن فراغ
ادغام_loop (بنية ادغام_گره_صف *صف,
            size_t مجموع_سطرها, FILE *tfp, حرف ثابت *موقت_output)
{
  طالما (true)
    {
      بنية ادغام_گره *گره = صف_برداشت (صف);

      إذا (گره->level == ادغام_انتها)
        {
          unlock_گره (گره);
          /* Reinsert so other threads can pop it. */
          صف_درج (صف, گره);
          اكسر;
        }
      mergelines_گره (گره, مجموع_سطرها, tfp, موقت_output);
      صف_check_درج (صف, گره);
      صف_check_درج_والد (صف, گره);

      unlock_گره (گره);
    }
}


ساكن فراغ sortlines (بنية سطر *محدود, size_t, size_t,
                       بنية ادغام_گره *, بنية ادغام_گره_صف *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

بنية thread_آرگومانها
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  بنية سطر *سطرها;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت مجموع_سطرها;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  بنية ادغام_گره *ثابت گره;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  بنية ادغام_گره_صف *ثابت صف;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  حرف ثابت *output_موقت;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ساكن فراغ *
sortlines_thread (فراغ *داده)
{
  بنية thread_آرگومانها ثابت *آرگومانها = داده;
  sortlines (آرگومانها->سطرها, آرگومانها->nthreads, آرگومانها->مجموع_سطرها,
             آرگومانها->گره, آرگومانها->صف, آرگومانها->tfp,
             آرگومانها->output_موقت);
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
sortlines (بنية سطر *محدود سطرها, size_t nthreads,
           size_t مجموع_سطرها, بنية ادغام_گره *گره,
           بنية ادغام_گره_صف *صف, FILE *tfp, حرف ثابت *موقت_output)
{
  size_t nlines = گره->nlo + گره->nhi;

  /* Calculate thread arguments. */
  size_t lo_نخها = nthreads / 2;
  size_t hi_نخها = nthreads - lo_نخها;
  pthread_t thread;
  بنية thread_آرگومانها آرگومانها = {سطرها, lo_نخها, مجموع_سطرها,
                             گره->lo_فرزند, صف, tfp, موقت_output};

  إذا (nthreads > 1 && SUBTHREAD_سطرها_HEURISTIC <= nlines
      && pthread_create (&thread, عدم, sortlines_thread, &آرگومانها) == 0)
    {
      sortlines (سطرها - گره->nlo, hi_نخها, مجموع_سطرها,
                 گره->hi_فرزند, صف, tfp, موقت_output);
      pthread_join (thread, عدم);
    }
  وإلا
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = گره->nlo;
      size_t nhi = گره->nhi;
      بنية سطر *موقت = سطرها - مجموع_سطرها;
      إذا (1 < nhi)
        sequential_مرتب_سازی (سطرها - nlo, nhi, موقت - nlo / 2, false);
      إذا (1 < nlo)
        sequential_مرتب_سازی (سطرها, nlo, موقت, false);

      /* Update merge NODE. No need to lock yet. */
      گره->lo = سطرها;
      گره->hi = سطرها - nlo;
      گره->انتها_lo = سطرها - nlo;
      گره->انتها_hi = سطرها - nlo - nhi;

      صف_درج (صف, گره);
      ادغام_loop (صف, مجموع_سطرها, tfp, موقت_output);
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
avoid_trashing_درون_داد (بنية sortfile *files, size_t ntemps,
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
              tempcopy = create_موقت (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].موقت = tempcopy;
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
        مرتب_سازی_die (_("cannot read"), files[i]);
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
      صحيح oflags = O_WRONLY | O_دودویی | O_CLOEXEC | O_CREAT;
      صحيح outfd = open (outfile, oflags, نما_RW_UGO);
      إذا (outfd < 0)
        مرتب_سازی_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ساكن فراغ
ادغام (بنية sortfile *files, size_t ntemps, size_t nfiles,
       حرف ثابت *output_file)
{
  طالما (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t درون;

      /* Number of output files generated so far.  */
      size_t بیرون;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      لكل (بیرون = درون = 0; nmerge <= nfiles - درون; بیرون++)
        {
          FILE *tfp;
          بنية tempnode *موقت = create_موقت (&tfp);
          size_t رقم_merged = mergefiles (&files[درون], MIN (ntemps, nmerge),
                                          nmerge, tfp, موقت->name);
          ntemps -= MIN (ntemps, رقم_merged);
          files[بیرون].name = موقت->name;
          files[بیرون].موقت = موقت;
          درون += رقم_merged;
        }

      remainder = nfiles - درون;
      cheap_slots = nmerge - بیرون % nmerge;

      إذا (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          بنية tempnode *موقت = create_موقت (&tfp);
          size_t رقم_merged = mergefiles (&files[درون], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, موقت->name);
          ntemps -= MIN (ntemps, رقم_merged);
          files[بیرون].name = موقت->name;
          files[بیرون++].موقت = موقت;
          درون += رقم_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[بیرون], &files[درون], (nfiles - درون) * حجم *files);
      ntemps += بیرون;
      nfiles -= درون - بیرون;
    }

  avoid_trashing_درون_داد (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  طالما (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_درون_داد_files (files, nfiles, &fps);

      إذا (nopened == nfiles)
        {
          FILE *ofp = جریان_open (output_file, "w");
          إذا (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              اكسر;
            }
          إذا (errno != EMFILE || nopened <= 2)
            مرتب_سازی_die (_("open failed"), output_file);
        }
      وإلا إذا (nopened <= 2)
        مرتب_سازی_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      بنية tempnode *موقت;
      افعل
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          موقت = maybe_create_موقت (&tfp, ! (nopened <= 2));
        }
      طالما (!موقت);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, موقت->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = موقت->name;
      files[0].موقت = موقت;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * حجم *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ساكن فراغ
مرتب_سازی (حرف *ثابت *files, size_t nfiles, حرف ثابت *output_file,
      size_t nthreads)
{
  بنية میانگیر buf;
  بنية مرتب_سازی_میانگیر_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.حد = 0;
  policy.growth_failed = false;

  طالما (nfiles)
    {
      حرف ثابت *موقت_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t بایتها_در_هر_سطر;
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
          بایتها_در_هر_سطر = (mult * حجم (بنية سطر));
        }
      وإلا
        بایتها_در_هر_سطر = حجم (بنية سطر) * 3 / 2;

      إذا (! buf.alloc)
        {
          مرتب_سازی_میانگیر_policy (&fp, 1, files, nfiles, بایتها_در_هر_سطر,
                              &policy);
          initbuf (&buf, بایتها_در_هر_سطر, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      طالما (fillbuf (&buf, fp, file, &policy))
        {
          بنية سطر *سطر;

          إذا (buf.eof && nfiles
              && (بایتها_در_هر_سطر + 1
                  < (buf.alloc - buf.استفاده_شده - بایتها_در_هر_سطر * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.چپ = buf.استفاده_شده;
              اكسر;
            }

          saved_سطر.متن = عدم;
          سطر = میانگیر_linelim (&buf);
          إذا (buf.eof && !nfiles && !ntemps && !buf.چپ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              موقت_output = output_file;
              output_file_created = true;
            }
          وإلا
            {
              ++ntemps;
              موقت_output = create_موقت (&tfp)->name;
            }
          إذا (1 < buf.nlines)
            {
              إذا (nthreads > 1)
                {
                  بنية ادغام_گره_صف صف;
                  صف_init (&صف, nthreads);
                  بنية ادغام_گره *ادغام_درخت =
                    ادغام_درخت_init (nthreads, buf.nlines, سطر);

                  sortlines (سطر, nthreads, buf.nlines, ادغام_درخت + 1,
                             &صف, tfp, موقت_output);

                  ادغام_درخت_destroy (nthreads, ادغام_درخت);
                  صف_destroy (&صف);
                }
              وإلا
                {
                  sequential_مرتب_سازی (سطر, buf.nlines,
                                   سطر - buf.nlines, false);
                  لكل (size_t i = 0; i < buf.nlines; i++)
                    write_unique (سطر - i - 1, tfp, موقت_output);
                }
            }
          وإلا
            write_unique (سطر - 1, tfp, موقت_output);

          xfclose (tfp, موقت_output);

          إذا (output_file_created)
            اذهب پایان;
        }
      xfclose (fp, file);
    }

 پایان:
  حرر (buf.buf);

  إذا (! output_file_created)
    {
      بنية tempnode *گره = temphead;
      بنية sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      لكل (size_t i = 0; گره; i++)
        {
          tempfiles[i].name = گره->name;
          tempfiles[i].موقت = گره;
          گره = گره->بعدی;
        }
      ادغام (tempfiles, ntemps, ntemps, output_file);
      حرر (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساكن فراغ
insertkey (بنية keyfield *کلید_arg)
{
  بنية keyfield **p;
  بنية keyfield *کلید = xmemdup (کلید_arg, حجم *کلید);

  لكل (p = &keylist; *p; p = &(*p)->بعدی)
    استمر;
  *p = کلید;
  کلید->بعدی = عدم;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ساكن فراغ
badfieldspec (حرف ثابت *spec, حرف ثابت *msgid)
{
  error (مرتب_سازی_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ساكن فراغ
incompatible_options (حرف ثابت *opts)
{
  error (مرتب_سازی_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ساكن فراغ
check_ordering_compatibility (فراغ)
{
  لكل (بنية keyfield *کلید = keylist; کلید; کلید = کلید->بعدی)
    إذا (1 < (کلید->numeric + کلید->general_numeric + کلید->human_numeric
             + کلید->month + (کلید->نسخه_3 | کلید->random | !!کلید->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        کلید->skipsblanks = کلید->skipeblanks = کلید->معکوس = false;
        کلید_مقصد_opts (کلید, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساكن حرف ثابت *
parse_فیلد_شمار (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
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
    حالة LONGINT_سرریز:
    حالة LONGINT_سرریز | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      اكسر;

    حالة LONGINT_INVALID:
      إذا (msgid)
        error (مرتب_سازی_FAILURE, 0, _("%s: invalid count at start of %s"),
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
مجموعه_ordering (حرف ثابت *s, بنية keyfield *کلید, تعداد blanktype blanktype)
{
  طالما (*s)
    {
      بدل (*s)
        {
        حالة 'b':
          إذا (blanktype == bl_شروع || blanktype == bl_both)
            کلید->skipsblanks = true;
          إذا (blanktype == bl_انتها || blanktype == bl_both)
            کلید->skipeblanks = true;
          اكسر;
        حالة 'd':
          کلید->ignore = nondictionary;
          اكسر;
        حالة 'f':
          کلید->translate = fold_toupper;
          اكسر;
        حالة 'g':
          کلید->general_numeric = true;
          اكسر;
        حالة 'h':
          کلید->human_numeric = true;
          اكسر;
        حالة 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          إذا (! کلید->ignore)
            کلید->ignore = nonprinting;
          اكسر;
        حالة 'M':
          کلید->month = true;
          اكسر;
        حالة 'n':
          کلید->numeric = true;
          اكسر;
        حالة 'R':
          کلید->random = true;
          اكسر;
        حالة 'r':
          کلید->معکوس = true;
          اكسر;
        حالة 'V':
          کلید->نسخه_3 = true;
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
کلید_init (بنية keyfield *کلید)
{
  memset (کلید, 0, حجم *کلید);
  کلید->eword = SIZE_MAX;
  أرجع کلید;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  بنية keyfield *کلید;
  بنية keyfield کلید_buf;
  بنية keyfield gkey;
  bool gkey_only = false;
  حرف ثابت *s;
  صحيح c = 0;
  حرف checkonly = 0;
  bool mergeonly = false;
  حرف *random_منبع = عدم;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != عدم);
  صحيح posix_ver = posix2_نسخه ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  حرف **files;
  حرف *files_from = عدم;
  بنية نشانهها tok;
  حرف ثابت *outfile = عدم;
  bool locale_ok;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (مرتب_سازی_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#إذا HAVE_NL_LANGINFO
  hard_LC_زمان = hard_locale (LC_زمان);
#پایان_اگر

  /* Get locale's representation of the decimal point.  */
  {
    بنية lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_نقطه = locale->decimal_نقطه[0];
    إذا (! decimal_نقطه || locale->decimal_نقطه[1])
      decimal_نقطه = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    إذا (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    إذا (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    تعداد { nsigs = countof (جمله_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سیگنالها);
    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (جمله_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        sigaction (جمله_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سیگنالها, جمله_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_سیگنالها;
    act.sa_flags = 0;

    لكل (size_t i = 0; i < nsigs; i++)
      {
        إذا (جمله_sig[i] == SIGPIPE)
          استمر;  /* Handled below.  */
        إذا (sigismember (&caught_سیگنالها, جمله_sig[i]))
          sigaction (جمله_sig[i], &act, عدم);
      }
  }
  إشارة (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  کلید_init (&gkey);
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
          کلید = عدم;
          إذا (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              إذا (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  کلید = کلید_init (&کلید_buf);
                  s = parse_فیلد_شمار (optarg + 1, &کلید->sword, عدم);
                  إذا (s && *s == '.')
                    s = parse_فیلد_شمار (s + 1, &کلید->schar, عدم);
                  إذا (! (کلید->sword || کلید->schar))
                    کلید->sword = SIZE_MAX;
                  إذا (! s || *مجموعه_ordering (s, کلید, bl_شروع))
                    کلید = عدم;
                  وإلا
                    {
                      إذا (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_فیلد_شمار (optarg1 + 1, &کلید->eword,
                                             N_("invalid number after '-'"));
                          إذا (*s == '.')
                            s = parse_فیلد_شمار (s + 1, &کلید->echar,
                                               N_("invalid number after '.'"));
                          إذا (!کلید->echar && کلید->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              کلید->eword--;
                            }
                          إذا (*مجموعه_ordering (s, کلید, bl_انتها))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      کلید->traditional_استفاده_شده = true;
                      insertkey (کلید);
                    }
                }
            }
          إذا (! کلید)
            files[nfiles++] = optarg;
          اكسر;

        حالة مرتب_سازی_OPTION:
          c = XARGMATCH ("--sort", optarg, مرتب_سازی_آرگومانها, مرتب_سازی_types);
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
            مجموعه_ordering (str, &gkey, bl_both);
          }
          اكسر;

        حالة CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_آرگومانها, check_types)
               : 'c');
          FALLTHROUGH;
        حالة 'c':
        حالة 'C':
          إذا (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          اكسر;

        حالة COMPRESS_برنامه_OPTION:
          إذا (compress_برنامه && !streq (compress_برنامه, optarg))
            error (مرتب_سازی_FAILURE, 0, _("multiple compress programs specified"));
          compress_برنامه = optarg;
          اكسر;

        حالة DEBUG_برنامه_OPTION:
          debug = true;
          اكسر;

        حالة FILES0_FROM_OPTION:
          files_from = optarg;
          اكسر;

        حالة 'k':
          کلید = کلید_init (&کلید_buf);

          /* Get POS1. */
          s = parse_فیلد_شمار (optarg, &کلید->sword,
                                 N_("invalid number at field start"));
          إذا (! کلید->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          إذا (*s == '.')
            {
              s = parse_فیلد_شمار (s + 1, &کلید->schar,
                                     N_("invalid number after '.'"));
              إذا (! کلید->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          إذا (! (کلید->sword || کلید->schar))
            کلید->sword = SIZE_MAX;
          s = مجموعه_ordering (s, کلید, bl_شروع);
          إذا (*s != ',')
            {
              کلید->eword = SIZE_MAX;
              کلید->echar = 0;
            }
          وإلا
            {
              /* Get POS2. */
              s = parse_فیلد_شمار (s + 1, &کلید->eword,
                                     N_("invalid number after ','"));
              إذا (! کلید->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              إذا (*s == '.')
                {
                  s = parse_فیلد_شمار (s + 1, &کلید->echar,
                                         N_("invalid number after '.'"));
                }
              s = مجموعه_ordering (s, کلید, bl_انتها);
            }
          إذا (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (کلید);
          اكسر;

        حالة 'm':
          mergeonly = true;
          اكسر;

        حالة NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          اكسر;

        حالة 'o':
          إذا (outfile && !streq (outfile, optarg))
            error (مرتب_سازی_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          اكسر;

        حالة RANDOM_منبع_OPTION:
          إذا (random_منبع && !streq (random_منبع, optarg))
            error (مرتب_سازی_FAILURE, 0, _("multiple random sources specified"));
          random_منبع = optarg;
          اكسر;

        حالة 's':
          stable = true;
          اكسر;

        حالة 'S':
          specify_مرتب_سازی_اندازه (oi, c, optarg);
          اكسر;

        حالة 't':
          {
            حرف newtab = optarg[0];
            إذا (! newtab)
              error (مرتب_سازی_FAILURE, 0, _("empty tab"));
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
                    error (مرتب_سازی_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            إذا (tab != TAB_DEFAULT && tab != newtab)
              error (مرتب_سازی_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          اكسر;

        حالة 'T':
          جمع_کن_موقت_dir (optarg);
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

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (مرتب_سازی_FAILURE);
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
          usage (مرتب_سازی_FAILURE);
        }

      FILE *جریان = xfopen (files_from, "r");

      readtokens0_init (&tok);

      إذا (! readtokens0 (جریان, &tok))
        error (مرتب_سازی_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (جریان, files_from);

      إذا (tok.n_tok)
        {
          حرر (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          لكل (size_t i = 0; i < nfiles; i++)
            {
              إذا (streq (files[i], "-"))
                error (مرتب_سازی_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              وإلا إذا (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  بیعلامت دراز صحيح file_عدد = i + 1;
                  error (مرتب_سازی_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_عدد);
                }
            }
        }
      وإلا
        error (مرتب_سازی_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  لكل (کلید = keylist; کلید; کلید = کلید->بعدی)
    {
      إذا (default_کلید_compare (کلید) && !کلید->معکوس)
        {
          کلید->ignore = gkey.ignore;
          کلید->translate = gkey.translate;
          کلید->skipsblanks = gkey.skipsblanks;
          کلید->skipeblanks = gkey.skipeblanks;
          کلید->month = gkey.month;
          کلید->numeric = gkey.numeric;
          کلید->general_numeric = gkey.general_numeric;
          کلید->human_numeric = gkey.human_numeric;
          کلید->نسخه_3 = gkey.نسخه_3;
          کلید->random = gkey.random;
          کلید->معکوس = gkey.معکوس;
        }

      need_random |= کلید->random;
    }

  إذا (!keylist && !default_کلید_compare (&gkey))
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

      کلید_warnings (&gkey, gkey_only);
    }

  معکوس = gkey.معکوس;

  إذا (need_random)
    random_md5_state_init (random_منبع);

  إذا (موقت_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      جمع_کن_موقت_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
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
  إذا (0 < مرتب_سازی_اندازه)
    مرتب_سازی_اندازه = MAX (مرتب_سازی_اندازه, MIN_مرتب_سازی_اندازه);

  إذا (checkonly)
    {
      إذا (nfiles > 1)
        error (مرتب_سازی_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      إذا (outfile)
        {
          ساكن حرف opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      خروج (check (files[0], checkonly) ? EXIT_SUCCESS : مرتب_سازی_بیرون_از_ترتیب);
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

      ادغام (sortfiles, 0, nfiles, outfile);
    }
  وإلا
    {
      إذا (!nthreads)
        {
          بیعلامت دراز صحيح np = رقم_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_نخها);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (بنية ادغام_گره));
      nthreads = MIN (nthreads, nthreads_max);

      مرتب_سازی (files, nfiles, outfile, nthreads);
    }

  إذا (have_read_stdin && fclose (stdin) == EOF)
    مرتب_سازی_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
