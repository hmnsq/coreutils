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

#شامل <config.h>

#شامل <getopt.h>
#شامل <pthread.h>
#شامل <sys/resource.h>
#شامل <sys/types.h>
#شامل <sys/wait.h>
#شامل <signal.h>
#شامل <spawn.h>
#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "c-ctype.h"
#شامل "fadvise.h"
#شامل "filevercmp.h"
#شامل "findprog.h"
#شامل "flexmember.h"
#شامل "hard-locale.h"
#شامل "hash.h"
#شامل "heap.h"
#شامل "ignore-value.h"
#شامل "mbswidth.h"
#شامل "nproc.h"
#شامل "physmem.h"
#شامل "posixver.h"
#شامل "quote.h"
#شامل "randread.h"
#شامل "readtokens0.h"
#شامل "stdlib--.h"
#شامل "strnumcmp.h"
#شامل "term-sig.h"
#شامل "unistd--.h"
#شامل "xmemcoll.h"
#شامل "xnanosleep.h"
#شامل "xstrtol.h"
#شامل "xstrtol-error.h"

#اگر_غیر_تعریف_شدہ RLIMIT_ڈیٹا
ساخت rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#اگر_ختم

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "sort"

#تعریف_کرو AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#اگر HAVE_LANGINFO_CODESET
# include <langinfo.h>
#اگر_ختم

#اگر !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#اگر_ختم
#اگر !defined OPEN_MAX
# define OPEN_MAX 20
#اگر_ختم

#تعریف_کرو UCHAR_LIM (UCHAR_MAX + 1)

#اگر_غیر_تعریف_شدہ DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#اگر_ختم

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#تعریف_کرو MAX_ملاؤ(کل, level) (((کل) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
شمار { SUBTHREAD_سطریں_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_سطریں_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
شمار { DEFAULT_MAX_دھاگے = 8 };

/* Exit statuses.  */
شمار
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    ترتیب_دو_برآمد_کا_ترتیب = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    ترتیب_دو_FAILURE = 2
  };

شمار
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

شمار
  {
    /* Level of the end-of-merge node, one level above the root. */
    ملاؤ_انتہا = 0,

    /* Level of the root node in merge tree. */
    ملاؤ_جڑ = 1
  };

/* The representation of the decimal point in the current locale.  */
ساکن حرف decimal_نقطہ;

/* Thousands separator; if outside char range, there is no separator.  */
ساکن صحیح thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ساکن bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ساکن bool hard_LC_COLLATE;
#اگر HAVE_NL_LANGINFO
ساکن bool hard_LC_وقت;
#اگر_ختم

#تعریف_کرو NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
شمار blanktype { bl_آغاز, bl_انتہا, bl_both };

/* The character marking end of line. Default to \n. */
ساکن حرف eolchar = '\n';

/* Lines are held in memory as counted strings. */
ساخت سطر
{
  حرف *متن;			/* Text of the line. */
  size_t لمبائی;		/* Length including final newline. */
  حرف *keybeg;			/* Start of first key. */
  حرف *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ساخت بفر
{
  حرف *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t مستعمل;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t بایاں;			/* Number of bytes left from previous reads. */
  size_t سطر_بائٹیں;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ساخت ترتیب_دو_بفر_policy
{
  size_t initial;
  size_t حد_بندی;
  bool growth_failed;
};

/* Sort key.  */
ساخت keyfield
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
  bool الٹ;			/* Reverse the sense of comparison. */
  bool نسخہ_3;			/* sort by version number */
  bool traditional_مستعمل;	/* Traditional key option format is used. */
  ساخت keyfield *اگلا;	/* Next keyfield to try. */
};

ساخت month
{
  حرف ثابت *name;
  صحیح val;
};

/* Binary merge tree node. */
ساخت ملاؤ_گرہ
{
  ساخت سطر *lo;              /* Lines to merge from LO child node. */
  ساخت سطر *hi;              /* Lines to merge from HI child node. */
  ساخت سطر *انتہا_lo;          /* End of available lines from LO. */
  ساخت سطر *انتہا_hi;          /* End of available lines from HI. */
  ساخت سطر **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ساخت ملاؤ_گرہ *والد;    /* Parent node. */
  ساخت ملاؤ_گرہ *lo_بچہ;  /* LO child node. */
  ساخت ملاؤ_گرہ *hi_بچہ;  /* HI child node. */
  غیرعلامتی صحیح level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t تالا;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ساخت ملاؤ_گرہ_قطار
{
  ساخت ڈھیر_2 *ترجیح_قطار;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ساکن ساخت سطر saved_سطر;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ساکن bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ساکن bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ساکن bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ساکن حرف fold_toupper[UCHAR_LIM];

#تعریف_کرو MONTHS_فی_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ساکن ساخت month monthtab[] =
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
#تعریف_کرو NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#تعریف_کرو MIN_ملاؤ_بفر_سائز (2 + حجم (ساخت سطر))

/* Minimum sort size; the code might not work with smaller sizes.  */
#تعریف_کرو MIN_ترتیب_دو_سائز (nmerge * MIN_ملاؤ_بفر_سائز)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ساکن size_t ملاؤ_بفر_سائز = MAX (MIN_ملاؤ_بفر_سائز, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ساکن size_t ترتیب_دو_سائز;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#تعریف_کرو داخلہ_FILE_سائز_GUESS (128 * 1024)

ساکن ساخت
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  حرف ثابت **dirs COUNTED_BY (n_dirs);
} عارضی_dirs;



/* Flag to reverse the order of all comparisons. */
ساکن bool الٹ;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ساکن bool stable;

/* An int value outside char range.  */
شمار { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
شمار { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ساکن صحیح tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ساکن bool unique;

/* Nonzero if any of the input files are the standard input. */
ساکن bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ساکن ساخت keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ساکن حرف ثابت *compress_پروگرام;

/* Annotate the output with extra info to aid the user.  */
ساکن bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ساکن غیرعلامتی صحیح nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ساکن bool default_SIGPIPE;

/* The list of temporary files. */
ساخت tempnode
{
  ساخت tempnode *متلون اگلا;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  حرف state;
  حرف name[FLEXIBLE_ARRAY_MEMBER];
};
ساکن ساخت tempnode *متلون temphead;
ساکن ساخت tempnode *متلون *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ساکن خالی
cleanup (خالی)
{
  برائے (ساخت tempnode ثابت *گرہ = temphead; گرہ; گرہ = گرہ->اگلا)
    unlink (گرہ->name);
  temphead = تہی;
}

/* Handle interrupts and hangups. */

ساکن خالی
sighandler (صحیح sig)
{
  cleanup ();

  اشارہ (sig, SIG_DFL);
  اٹھاؤ (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ساکن خالی
ترتیب_دو_die (حرف ثابت *پیغام_2, حرف ثابت *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  اگر (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (ترتیب_دو_FAILURE, errno, "%s: %s", پیغام_2,
         quotef (file ? file : _("standard output")));
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Write sorted concatenation کا all FILE(s) مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding ترتیب_دو کلیدیں اندر each سطر\n\
"));
      oputs (_("\
  -d, --dictionary-ترتیب\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-حالت\n\
         fold زیریں حالت مقصد بالائی حالت characters\n\
"));
      oputs (_("\
  -g, --general-numeric-ترتیب_دو\n\
         compare according مقصد general numerical قدر\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-ترتیب_دو\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-ترتیب_دو\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-ترتیب_دو\n\
         compare according مقصد string numerical قدر;\n\
         see بھرا documentation برائے supported لڑیاں\n\
"));
      oputs (_("\
  -R, --random-ترتیب_دو\n\
         shuffle, but گروہ identical کلیدیں.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-ماخذ_2=FILE\n\
         get random بائٹیں from FILE\n\
"));
      oputs (_("\
  -r, --الٹ\n\
         الٹ the نتیجہ کا comparisons\n\
"));
      oputs (_("\
      --ترتیب_دو=لفظ\n\
         ترتیب_دو according مقصد لفظ:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, نسخہ_3 -V\n\
"));
      oputs (_("\
  -V, --نسخہ_3-ترتیب_دو\n\
         natural ترتیب_دو کا (نسخہ_3) numbers within متن\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-سائز=NMERGE\n\
         ملاؤ at most NMERGE inputs at once; برائے مزید use عارضی files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-پہلا\n\
         check برائے sorted داخلہ; کرو not ترتیب_دو\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but کرو not report پہلا bad سطر\n\
"));
      oputs (_("\
      --compress-پروگرام=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part کا the سطر مستعمل مقصد ترتیب_دو,\n\
         and warn about questionable usage مقصد standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         اقرأ داخلہ from the files specified by NUL-terminated نام اندر file F;\
\n\
         If F is -, اقرأ نام from standard داخلہ\n\
"));
      oputs (_("\
  -k, --کلید=KEYDEF\n\
         ترتیب_دو بذریعہ a کلید; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --ملاؤ\n\
         ملاؤ already sorted files; کرو not ترتیب_دو\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         اكتب نتیجہ مقصد FILE instead کا standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize ترتیب_دو by disabling آخری-resort comparison\n\
"));
      oputs (_("\
  -S, --بفر-سائز=سائز_2\n\
         use سائز_2 برائے مین حافظہ بفر\n\
"));
      oputs (_("\
  -t, --میدان-separator=SEP\n\
         use SEP instead کا non-blank مقصد blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR برائے temporaries, not $TMPDIR or %s;\n\
         مضرب options specify مضرب directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the نمبر کا sorts تسلسل_2 concurrently مقصد N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the پہلا کا سطریں with equal کلیدیں;\n\
         with -c, check برائے strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         سطر delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] برائے آغاز and stop position, where F is a\n\
میدان نمبر and C a character position اندر the میدان; both are origin 1, and\n\
the stop position defaults مقصد the سطر's انتہا.  If neither -t nor -b is اندر\n\
effect, characters اندر a میدان are counted from the beginning کا the preceding\n\
whitespace.  OPTS is one or مزید single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options برائے that کلید.  If no کلید is given, use\n\
the entire سطر as the کلید.  Use --debug مقصد diagnose incorrect کلید usage.\n\
\n\
سائز_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% کا حافظہ, b 1, K 1024 (طےشدہ), and so on برائے M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects ترتیب_دو ترتیب.\n\
سیٹ_3 LC_ALL=C مقصد get the traditional ترتیب_دو ترتیب that استعمال_کرتا\n\
native byte اقدار.\n\
"), stdout );
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_پروگرام_OPTION,
  DEBUG_پروگرام_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_ماخذ_OPTION,
  ترتیب_دو_OPTION,
  PARALLEL_OPTION
};

ساکن حرف ثابت short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ساکن ساخت option ثابت long_options[] =
{
  {"ignore-leading-blanks", no_دلیل, تہی, 'b'},
  {"check", optional_دلیل, تہی, CHECK_OPTION},
  {"compress-program", required_دلیل, تہی, COMPRESS_پروگرام_OPTION},
  {"debug", no_دلیل, تہی, DEBUG_پروگرام_OPTION},
  {"dictionary-order", no_دلیل, تہی, 'd'},
  {"ignore-case", no_دلیل, تہی, 'f'},
  {"files0-from", required_دلیل, تہی, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_دلیل, تہی, 'g'},
  {"ignore-nonprinting", no_دلیل, تہی, 'i'},
  {"key", required_دلیل, تہی, 'k'},
  {"merge", no_دلیل, تہی, 'm'},
  {"month-sort", no_دلیل, تہی, 'M'},
  {"numeric-sort", no_دلیل, تہی, 'n'},
  {"human-numeric-sort", no_دلیل, تہی, 'h'},
  {"version-sort", no_دلیل, تہی, 'V'},
  {"random-sort", no_دلیل, تہی, 'R'},
  {"random-source", required_دلیل, تہی, RANDOM_ماخذ_OPTION},
  {"sort", required_دلیل, تہی, ترتیب_دو_OPTION},
  {"output", required_دلیل, تہی, 'o'},
  {"reverse", no_دلیل, تہی, 'r'},
  {"stable", no_دلیل, تہی, 's'},
  {"batch-size", required_دلیل, تہی, NMERGE_OPTION},
  {"buffer-size", required_دلیل, تہی, 'S'},
  {"field-separator", required_دلیل, تہی, 't'},
  {"temporary-directory", required_دلیل, تہی, 'T'},
  {"unique", no_دلیل, تہی, 'u'},
  {"zero-terminated", no_دلیل, تہی, 'z'},
  {"parallel", required_دلیل, تہی, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0},
};

#تعریف_کرو CHECK_جدول \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ساکن حرف ثابت *ثابت check_دلائل[] =
{
#تعریف_کرو _ct_(_s, _c) _s,
  CHECK_جدول تہی
#تعریف_منسوخ  _ct_
};
ساکن حرف ثابت check_types[] =
{
#تعریف_کرو _ct_(_s, _c) _c,
  CHECK_جدول
#تعریف_منسوخ  _ct_
};

#تعریف_کرو ترتیب_دو_جدول \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ساکن حرف ثابت *ثابت ترتیب_دو_دلائل[] =
{
#تعریف_کرو _st_(_s, _c) _s,
  ترتیب_دو_جدول تہی
#تعریف_منسوخ  _st_
};
ساکن حرف ثابت ترتیب_دو_types[] =
{
#تعریف_کرو _st_(_s, _c) _c,
  ترتیب_دو_جدول
#تعریف_منسوخ  _st_
};

/* The set of signals that are caught.  */
ساکن sigset_t caught_اشارات;

/* Critical section status.  */
ساخت cs_حالت
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ساکن خالی
cs_enter (ساخت cs_حالت *حالت_2)
{
  صحیح ret = pthread_sigmask (SIG_BLOCK, &caught_اشارات, &حالت_2->sigs);
  حالت_2->valid = ret == 0;
}

/* Leave a critical section.  */
ساکن خالی
cs_leave (ساخت cs_حالت ثابت *حالت_2)
{
  اگر (حالت_2->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &حالت_2->sigs, تہی);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
شمار { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ساخت sortfile
{
  /* The file's name.  */
  حرف ثابت *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ساخت tempnode *عارضی;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ساکن Hash_جدول *proctab;

شمار { INIT_PROCTAB_سائز = 47 };

ساکن size_t
proctab_hasher (خالی ثابت *اندراج_2, size_t tabsize)
{
  ساخت tempnode ثابت *گرہ = اندراج_2;
  واپس گرہ->pid % tabsize;
}

ساکن bool
proctab_comparator (خالی ثابت *e1, خالی ثابت *e2)
{
  ساخت tempnode ثابت *n1 = e1;
  ساخت tempnode ثابت *n2 = e2;
  واپس n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ساکن pid_t nprocs;

ساکن bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ساکن pid_t
reap (pid_t pid)
{
  صحیح حالت_2;
  pid_t cpid = waitpid ((pid ? pid : -1), &حالت_2, (pid ? 0 : WNOHANG));

  اگر (cpid < 0)
    error (ترتیب_دو_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_پروگرام));
  ورنہ اگر (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      اگر (! WIFEXITED (حالت_2) || WEXITSTATUS (حالت_2))
        error (ترتیب_دو_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_پروگرام));
      --nprocs;
    }

  واپس cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ساکن خالی
رجسٹر_proc (ساخت tempnode *عارضی)
{
  اگر (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_سائز, تہی,
                                 proctab_hasher,
                                 proctab_comparator,
                                 تہی);
      اگر (! proctab)
        xalloc_die ();
    }

  عارضی->state = UNREAPED;

  اگر (! hash_داخل_کرو (proctab, عارضی))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ساکن bool
delete_proc (pid_t pid)
{
  ساخت tempnode test;

  test.pid = pid;
  ساخت tempnode *گرہ = hash_remove (proctab, &test);
  اگر (! گرہ)
    واپس false;
  گرہ->state = REAPED;
  واپس true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ساکن خالی
wait_proc (pid_t pid)
{
  اگر (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ساکن خالی
reap_exited (خالی)
{
  جب (0 < nprocs && reap (0))
    جاری;
}

/* Reap at least one exited child, waiting if necessary.  */

ساکن خالی
reap_some (خالی)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ساکن خالی
reap_all (خالی)
{
  جب (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ساکن خالی
exit_cleanup (خالی)
{
  اگر (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ساخت cs_حالت cs;
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

ساکن ساخت tempnode *
create_عارضی_file (صحیح *pfd, bool survive_fd_exhaustion)
{
  ساکن حرف ثابت slashbase[] = "/sortXXXXXX";
  ساکن idx_t عارضی_dir_اشاریہ;
  صحیح fd;
  صحیح saved_errno;
  حرف ثابت *عارضی_dir = عارضی_dirs.dirs[عارضی_dir_اشاریہ];
  size_t len = لڑی_لمبائی (عارضی_dir);
  ساخت tempnode *گرہ =
    xmalloc (FLEXSIZEOF (ساخت tempnode, name, len + حجم slashbase));
  حرف *file = گرہ->name;
  ساخت cs_حالت cs;

  حافظہ_نقل (file, عارضی_dir, len);
  حافظہ_نقل (file + len, slashbase, حجم slashbase);
  گرہ->اگلا = تہی;
  اگر (++عارضی_dir_اشاریہ == عارضی_dirs.n_dirs)
    عارضی_dir_اشاریہ = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  اگر (0 <= fd)
    {
      *temptail = گرہ;
      temptail = &گرہ->اگلا;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  اگر (fd < 0)
    {
      اگر (! (survive_fd_exhaustion && errno == EMFILE))
        error (ترتیب_دو_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (عارضی_dir));
      آزاد (گرہ);
      گرہ = تہی;
    }

  *pfd = fd;
  واپس گرہ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ساکن ساخت stat *
get_outstatus (خالی)
{
  ساکن صحیح outstat_errno;
  ساکن ساخت stat outstat;
  اگر (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  واپس outstat_errno < 0 ? &outstat : تہی;
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

ساکن FILE *
دھارا_open (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp;

  اگر (*how == 'r')
    {
      اگر (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ورنہ
        {
          صحیح fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? تہی : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ورنہ اگر (*how == 'w')
    {
      اگر (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          صحیح ftruncate_errno = errno;
          ساخت stat *outst = get_outstatus ();
          اگر (!outst || S_ISREG (outst->st_منوال) || S_TYPEISSHM (outst))
            error (ترتیب_دو_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ورنہ
    affirm (!"unexpected mode passed to stream_open");

  واپس fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ساکن FILE *
xfopen (حرف ثابت *file, حرف ثابت *how)
{
  FILE *fp = دھارا_open (file, how);
  اگر (!fp)
    ترتیب_دو_die (_("open failed"), file);
  واپس fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ساکن خالی
xfclose (FILE *fp, حرف ثابت *file)
{
  انتخاب (fileno (fp))
    {
    حالت STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      توڑو;

    حالت STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      اگر (fflush (fp) != 0)
        ترتیب_دو_die (_("fflush failed"), file);
      توڑو;

    طےشدہ:
      اگر (fclose (fp) != 0)
        ترتیب_دو_die (_("close failed"), file);
      توڑو;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ساکن خالی
move_fd (صحیح oldfd, صحیح newfd)
{
  اگر (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_قدر (dup2 (oldfd, newfd));
      ignore_قدر (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ساکن صحیح
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  صحیح oldfd, صحیح newfd)
{
  صحیح نتیجہ = 0;
  اگر (oldfd != newfd)
    {
      نتیجہ = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      اگر (نتیجہ == 0)
        نتیجہ = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  واپس نتیجہ;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ساکن حرف ثابت *
get_resolved_compress_پروگرام (خالی)
{
  /* Use a cache, to perform the search only once.  */
  ساکن حرف ثابت *resolved_compress_پروگرام_cache /* = NULL */;

  اگر (resolved_compress_پروگرام_cache == تہی)
    {
      resolved_compress_پروگرام_cache =
        ڈھونڈو_اندر_given_راستہ (compress_پروگرام, getenv ("PATH"), تہی, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  واپس resolved_compress_پروگرام_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ساکن صحیح
pipe_بچہ (pid_t *pid, صحیح pipefds[2], صحیح tempfd, bool decompress,
            size_t tries)
{
  حرف ثابت *resolved_compress_پروگرام;
  ساخت tempnode *saved_temphead;
  دوہرا wait_retry = 0.25;
  ساخت cs_حالت cs;
  صحیح نتیجہ;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_پروگرام = get_resolved_compress_پروگرام ();
  اگر (resolved_compress_پروگرام == تہی)
    واپس errno;

  اگر ((نتیجہ = posix_spawnattr_init (&attr)))
    واپس نتیجہ;
  اگر ((نتیجہ = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (نتیجہ = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      واپس نتیجہ;
    }

  اگر (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      صحیح saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      واپس saved_errno;
    }

  اگر ((نتیجہ = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (نتیجہ = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((نتیجہ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (نتیجہ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (نتیجہ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((نتیجہ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (نتیجہ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (نتیجہ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      واپس نتیجہ;
    }

  حرف ثابت *ثابت argv[] =
    {
      resolved_compress_پروگرام,
      decompress ? "-d" : تہی,
      تہی
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  اگر (nmerge + 1 < nprocs)
    reap_some ();

  جب (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = تہی;

      نتیجہ = posix_spawnp (pid, resolved_compress_پروگرام, &actions, &attr,
                             (حرف * ثابت *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      اگر (نتیجہ != EAGAIN)
        توڑو;
      ورنہ
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

  اگر (نتیجہ)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ورنہ
    ++nprocs;

  واپس نتیجہ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ساکن ساخت tempnode *
maybe_create_عارضی (FILE **pfp, bool survive_fd_exhaustion)
{
  صحیح tempfd;
  ساخت tempnode *گرہ = create_عارضی_file (&tempfd, survive_fd_exhaustion);
  اگر (! گرہ)
    واپس تہی;

  گرہ->state = UNCOMPRESSED;

  اگر (compress_پروگرام)
    {
      صحیح pipefds[2];
      ساکن صحیح آخری_نتیجہ = 0;

      صحیح نتیجہ = pipe_بچہ (&گرہ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      اگر (نتیجہ)
        {
          اگر (نتیجہ != آخری_نتیجہ)
            error (0, نتیجہ, _("could not run compress program %s"),
                   quoteaf (compress_پروگرام));
        }
      ورنہ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          رجسٹر_proc (گرہ);
        }

      آخری_نتیجہ = نتیجہ;
    }

  *pfp = fdopen (tempfd, "w");
  اگر (! *pfp)
    ترتیب_دو_die (_("couldn't create temporary file"), گرہ->name);

  واپس گرہ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ساکن ساخت tempnode *
create_عارضی (FILE **pfp)
{
  واپس maybe_create_عارضی (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ساکن FILE *
open_عارضی (ساخت tempnode *عارضی)
{
  صحیح tempfd, pipefds[2];
  FILE *fp = تہی;

  اگر (عارضی->state == UNREAPED)
    wait_proc (عارضی->pid);

  tempfd = open (عارضی->name, O_RDONLY);
  اگر (tempfd < 0)
    واپس تہی;

  pid_t بچہ;
  صحیح نتیجہ = pipe_بچہ (&بچہ, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  اگر (نتیجہ)
    {
      اگر (نتیجہ != EMFILE)
        error (ترتیب_دو_FAILURE, نتیجہ, _("could not run compress program %s -d"),
               quoteaf (compress_پروگرام));
      close (tempfd);
      errno = EMFILE;
    }
  ورنہ
    {
      عارضی->pid = بچہ;
      رجسٹر_proc (عارضی);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      اگر (! fp)
        {
          صحیح saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  واپس fp;
}

/* Append DIR to the array of temporary directory names.  */
ساکن خالی
جوڑو_عارضی_dir (حرف ثابت *dir)
{
  اگر (عارضی_dirs.n_dirs == عارضی_dirs.n_dirs_alloc)
    عارضی_dirs.dirs = xpalloc (عارضی_dirs.dirs, &عارضی_dirs.n_dirs_alloc, 1, -1,
                              حجم *عارضی_dirs.dirs);

  idx_t ثابت dir_idx = عارضی_dirs.n_dirs++;
  عارضی_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ساکن خالی
zaptemp (حرف ثابت *name)
{
  ساخت tempnode *متلون *pnode;
  ساخت tempnode *گرہ;
  ساخت tempnode *اگلا;
  صحیح unlink_حالت;
  صحیح unlink_errno = 0;
  ساخت cs_حالت cs;

  برائے (pnode = &temphead; (گرہ = *pnode)->name != name; pnode = &گرہ->اگلا)
    جاری;

  اگر (گرہ->state == UNREAPED)
    wait_proc (گرہ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  اگلا = گرہ->اگلا;
  cs_enter (&cs);
  unlink_حالت = unlink (name);
  unlink_errno = errno;
  *pnode = اگلا;
  cs_leave (&cs);

  اگر (unlink_حالت != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  اگر (! اگلا)
    temptail = pnode;
  آزاد (گرہ);
}

#اگر HAVE_NL_LANGINFO

ساکن صحیح
struct_month_cmp (خالی ثابت *m1, خالی ثابت *m2)
{
  ساخت month ثابت *month1 = m1;
  ساخت month ثابت *month2 = m2;
  واپس strcmp (month1->name, month2->name);
}

#اگر_ختم

/* Initialize the character class tables. */

ساکن خالی
inittables (خالی)
{
  برائے (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! حرف_یا_ہندسہ (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = بڑا_حرف (i);
    }

#اگر HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  اگر (hard_LC_وقت)
    {
      برائے (size_t i = 0; i < MONTHS_فی_YEAR; i++)
        {
          حرف ثابت *s;
          size_t s_len;
          size_t j, k;
          حرف *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = لڑی_لمبائی (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          برائے (j = k = 0; j < s_len; j++)
            اگر (! isblank (مقصد_uchar (s[j])))
              name[k++] = fold_toupper[مقصد_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_فی_YEAR, حجم *monthtab, struct_month_cmp);
    }
#اگر_ختم
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ساکن خالی
specify_nmerge (صحیح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  ساخت rlimit rlimit;
  شمار strtol_error e = xstrtoumax (s, تہی, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  غیرعلامتی صحیح max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  اگر (e == LONGINT_OK)
    {
      nmerge = n;
      اگر (nmerge != n)
        e = LONGINT_حد_سے_تجاوز;
      ورنہ
        {
          اگر (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (ترتیب_دو_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ورنہ اگر (max_nmerge < nmerge)
            {
              e = LONGINT_حد_سے_تجاوز;
            }
          ورنہ
            واپس;
        }
    }

  اگر (e == LONGINT_حد_سے_تجاوز)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (ترتیب_دو_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ورنہ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ساکن خالی
specify_ترتیب_دو_سائز (صحیح oi, حرف c, حرف ثابت *s)
{
  uintmax_t n;
  حرف *suffix;
  شمار strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  اگر (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_حد_سے_تجاوز;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  اگر (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    انتخاب (suffix[0])
      {
      حالت 'b':
        e = LONGINT_OK;
        توڑو;

      حالت '%':
        {
          دوہرا mem = physmem_کل () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          اگر (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ورنہ
            e = LONGINT_حد_سے_تجاوز;
        }
        توڑو;
      }

  اگر (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      اگر (n < ترتیب_دو_سائز)
        واپس;

      ترتیب_دو_سائز = n;
      اگر (ترتیب_دو_سائز == n)
        {
          ترتیب_دو_سائز = MAX (ترتیب_دو_سائز, MIN_ترتیب_دو_سائز);
          واپس;
        }

      e = LONGINT_حد_سے_تجاوز;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ساکن size_t
specify_nthreads (صحیح oi, حرف c, حرف ثابت *s)
{
  uintmax_t nthreads;
  شمار strtol_error e = xstrtoumax (s, تہی, 10, &nthreads, "");
  اگر (e == LONGINT_حد_سے_تجاوز)
    واپس SIZE_MAX;
  اگر (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  اگر (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  اگر (nthreads == 0)
    error (ترتیب_دو_FAILURE, 0, _("number in parallel must be nonzero"));
  واپس nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ساکن size_t
default_ترتیب_دو_سائز (خالی)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t سائز = SIZE_MAX;
  ساخت rlimit rlimit;
  اگر (getrlimit (RLIMIT_ڈیٹا, &rlimit) == 0 && rlimit.rlim_cur < سائز)
    سائز = rlimit.rlim_cur;
#اگر_تعریف_شدہ RLIMIT_AS
  اگر (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < سائز)
    سائز = rlimit.rlim_cur;
#اگر_ختم

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  سائز /= 2;

#اگر_تعریف_شدہ RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  اگر (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < سائز)
    سائز = rlimit.rlim_cur / 16 * 15;
#اگر_ختم

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  دوہرا avail = physmem_دستیاب ();
  دوہرا کل = physmem_کل ();
  دوہرا mem = MAX (avail, کل / 8);

  /* Leave a 1/4 margin for physical memory.  */
  اگر (کل * 0.75 < سائز)
    سائز = کل * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  اگر (mem < سائز)
    سائز = mem;
  واپس MAX (سائز, MIN_ترتیب_دو_سائز);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ساکن bool
سطر_aligned_سائز (size_t *alloc)
{
  size_t سائز = *alloc;
  size_t alignment = حجم (ساخت سطر);
  size_t padding = alignment - سائز % alignment;
  size_t aligned;

  اگر (ckd_جوڑو (&aligned, سائز, padding))
    واپس false;

  *alloc = aligned;
  واپس true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ساکن size_t
default_initial_ترتیب_دو_سائز (size_t سطر_بائٹیں)
{
  size_t سائز = سطر_بائٹیں + 2;
  size_t داخلہ_سائز;

  اگر (ckd_mul (&داخلہ_سائز, داخلہ_FILE_سائز_GUESS, سطر_بائٹیں + 1)
      || ckd_جوڑو (&سائز, سائز, داخلہ_سائز))
    واپس SIZE_MAX;

  واپس MAX (سائز, MIN_ترتیب_دو_سائز);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ساکن bool
داخلہ_سائز_بفر_بائٹیں (uintmax_t file_سائز, size_t سطر_بائٹیں,
                         size_t *worst_case)
{
  size_t worst_case_فی_داخلہ_byte = سطر_بائٹیں + 1;

  اگر (SIZE_MAX / worst_case_فی_داخلہ_byte < file_سائز)
    واپس false;

  size_t سائز = file_سائز * worst_case_فی_داخلہ_byte;
  اگر (SIZE_MAX - سائز < 1)
    واپس false;

  *worst_case = سائز + 1;
  واپس true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ساکن خالی
ترتیب_دو_بفر_policy (FILE *ثابت *fps, size_t nfps,
                    حرف *ثابت *files, size_t nfiles,
                    size_t سطر_بائٹیں,
                    ساخت ترتیب_دو_بفر_policy *policy)
{
  size_t سائز_bound = ترتیب_دو_سائز ? ترتیب_دو_سائز : default_ترتیب_دو_سائز ();
  size_t initial_bound =
    ترتیب_دو_سائز ? سائز_bound : default_initial_ترتیب_دو_سائز (سطر_بائٹیں);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_فی_داخلہ_byte = سطر_بائٹیں + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t سائز = worst_case_فی_داخلہ_byte + 1;

  initial_bound = MIN (initial_bound, سائز_bound);
  policy->حد_بندی = سائز_bound;
  policy->growth_failed = false;

  برائے (size_t i = 0; i < nfiles; i++)
    {
      ساخت stat st;
      off_t file_سائز;
      size_t worst_case;
      bool معلوم_سائز;

      اگر (سائز_bound <= سائز)
        {
          policy->initial = سائز_bound;
          واپس;
        }

      اگر ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        ترتیب_دو_die (_("stat failed"), files[i]);

      معلوم_سائز = usable_st_سائز (&st) && 0 < st.st_سائز;
      اگر (معلوم_سائز)
        file_سائز = st.st_سائز;
      ورنہ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          اگر (ترتیب_دو_سائز)
            {
              policy->initial = سائز_bound;
              واپس;
            }
          file_سائز = داخلہ_FILE_سائز_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      اگر (! داخلہ_سائز_بفر_بائٹیں (file_سائز, سطر_بائٹیں, &worst_case)
          || سائز_bound - سائز <= worst_case)
        {
          policy->initial = سائز_bound;
          واپس;
        }

      اگر (! معلوم_سائز && ! ترتیب_دو_سائز
          && (initial_bound <= سائز || initial_bound - سائز <= worst_case))
        {
          policy->initial = MAX (سائز, initial_bound);
          واپس;
        }

      سائز += worst_case;
    }

  policy->initial = MAX (سائز, MIN_ترتیب_دو_سائز);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ساکن خالی
initbuf (ساخت بفر *buf, size_t سطر_بائٹیں, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  جب (true)
    {
      اگر (! سطر_aligned_سائز (&alloc))
        buf->buf = تہی;
      ورنہ
        buf->buf = حافظہ_دو (alloc);
      اگر (buf->buf)
        توڑو;
      alloc /= 2;
      اگر (alloc <= سطر_بائٹیں + 1)
        xalloc_die ();
    }

  buf->سطر_بائٹیں = سطر_بائٹیں;
  buf->alloc = alloc;
  buf->مستعمل = buf->بایاں = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ساکن اندرونی ساخت سطر *
بفر_linelim (ساخت بفر ثابت *buf)
{
  خالی *linelim = buf->buf + buf->alloc;
  واپس linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ساکن bool
try_growbuf (ساخت بفر *buf, size_t alloc)
{
  اگر (! سطر_aligned_سائز (&alloc) || alloc <= buf->alloc)
    واپس false;

  حرف *newbuf = حافظہ_دو (alloc);
  اگر (! newbuf)
    واپس false;

  حرف *oldbuf = buf->buf;
  ساخت سطر *old_linelim = بفر_linelim (buf);
  ساخت سطر *old_سطر = old_linelim - buf->nlines;

  حافظہ_نقل (newbuf, oldbuf, buf->مستعمل);

  ساخت سطر *new_linelim = (خالی *) (newbuf + alloc);
  ساخت سطر *new_سطر = new_linelim - buf->nlines;
  حافظہ_نقل (new_سطر, old_سطر, buf->nlines * حجم *new_سطر);

  برائے (ساخت سطر *سطر = new_سطر; سطر < new_linelim; سطر++)
    {
      سطر->متن = newbuf + (سطر->متن - oldbuf);
      اگر (keylist)
        {
          سطر->keybeg = newbuf + (سطر->keybeg - oldbuf);
          سطر->keylim = newbuf + (سطر->keylim - oldbuf);
        }
    }

  آزاد (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  واپس true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ساکن bool
maybe_growbuf (ساخت بفر *buf, ساخت ترتیب_دو_بفر_policy *policy)
{
  اگر (! policy || policy->growth_failed || policy->حد_بندی <= buf->alloc)
    واپس false;

  size_t alloc;
  اگر (buf->alloc <= policy->حد_بندی / 3)
    alloc = buf->alloc * 3;
  ورنہ
    alloc = policy->حد_بندی;

  اگر (try_growbuf (buf, alloc))
    واپس true;

  policy->growth_failed = true;
  واپس false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ساکن حرف *
begfield (ساخت سطر ثابت *سطر, ساخت keyfield ثابت *کلید)
{
  حرف *ptr = سطر->متن, *lim = ptr + سطر->لمبائی - 1;
  size_t sword = کلید->sword;
  size_t schar = کلید->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  اگر (tab != TAB_DEFAULT)
    جب (ptr < lim && sword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        اگر (ptr < lim)
          ++ptr;
      }
  ورنہ
    جب (ptr < lim && sword--)
      {
        جب (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;
        جب (ptr < lim && !blanks[مقصد_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  اگر (کلید->skipsblanks)
    جب (ptr < lim && blanks[مقصد_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t بقایا_بائٹیں = lim - ptr;
  اگر (schar < بقایا_بائٹیں)
    ptr += schar;
  ورنہ
    ptr = lim;

  واپس ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_خالص
ساکن حرف *
limfield (ساخت سطر ثابت *سطر, ساخت keyfield ثابت *کلید)
{
  حرف *ptr = سطر->متن, *lim = ptr + سطر->لمبائی - 1;
  size_t eword = کلید->eword, echar = کلید->echar;

  اگر (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  اگر (tab != TAB_DEFAULT)
    جب (ptr < lim && eword--)
      {
        حرف *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        اگر (ptr < lim && (eword || echar))
          ++ptr;
      }
  ورنہ
    جب (ptr < lim && eword--)
      {
        جب (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;
        جب (ptr < lim && !blanks[مقصد_uchar (*ptr)])
          ++ptr;
      }

#اگر_تعریف_شدہ POSIX_UNSPECIFIED
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
  اگر (tab != TAB_DEFAULT)
    {
      حرف *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      اگر (newlim)
        lim = newlim;
    }
  ورنہ
    {
      حرف *newlim;
      newlim = ptr;
      جب (newlim < lim && blanks[مقصد_uchar (*newlim)])
        ++newlim;
      جب (newlim < lim && !blanks[مقصد_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#اگر_ختم

  اگر (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      اگر (کلید->skipeblanks)
        جب (ptr < lim && blanks[مقصد_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t بقایا_بائٹیں = lim - ptr;
      اگر (echar < بقایا_بائٹیں)
        ptr += echar;
      ورنہ
        ptr = lim;
    }

  واپس ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ساکن bool
fillbuf (ساخت بفر *buf, FILE *fp, حرف ثابت *file,
         ساخت ترتیب_دو_بفر_policy *policy)
{
  ساخت keyfield ثابت *کلید = keylist;
  حرف eol = eolchar;
  size_t سطر_بائٹیں = buf->سطر_بائٹیں;
  size_t mergesize = ملاؤ_بفر_سائز - MIN_ملاؤ_بفر_سائز;

  اگر (buf->eof)
    واپس false;

  اگر (buf->مستعمل != buf->بایاں)
    {
      memmove (buf->buf, buf->buf + buf->مستعمل - buf->بایاں, buf->بایاں);
      buf->مستعمل = buf->بایاں;
      buf->nlines = 0;
    }

  جب (true)
    {
      حرف *ptr = buf->buf + buf->مستعمل;
      ساخت سطر *linelim = بفر_linelim (buf);
      ساخت سطر *سطر = linelim - buf->nlines;
      size_t avail = (حرف *) linelim - buf->nlines * سطر_بائٹیں - ptr;
      حرف *سطر_آغاز = buf->nlines ? سطر->متن + سطر->لمبائی : buf->buf;

      جب (سطر_بائٹیں + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (سطر_بائٹیں + 1);
          size_t بائٹیں_read = fread (ptr, 1, readsize, fp);
          حرف *ptrlim = ptr + بائٹیں_read;
          حرف *p;
          avail -= بائٹیں_read;

          اگر (بائٹیں_read != readsize)
            {
              اگر (ferror (fp))
                ترتیب_دو_die (_("read failed"), file);
              اگر (feof (fp))
                {
                  buf->eof = true;
                  اگر (buf->buf == ptrlim)
                    واپس false;
                  اگر (سطر_آغاز != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          جب ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              سطر--;
              سطر->متن = سطر_آغاز;
              سطر->لمبائی = ptr - سطر_آغاز;
              mergesize = MAX (mergesize, سطر->لمبائی);
              avail -= سطر_بائٹیں;

              اگر (کلید)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  سطر->keylim = (کلید->eword == SIZE_MAX
                                  ? p
                                  : limfield (سطر, کلید));

                  اگر (کلید->sword != SIZE_MAX)
                    سطر->keybeg = begfield (سطر, کلید);
                  ورنہ
                    {
                      اگر (کلید->skipsblanks)
                        جب (blanks[مقصد_uchar (*سطر_آغاز)])
                          سطر_آغاز++;
                      سطر->keybeg = سطر_آغاز;
                    }
                }

              سطر_آغاز = ptr;
            }

          ptr = ptrlim;
          اگر (buf->eof)
            توڑو;
        }

      buf->مستعمل = ptr - buf->buf;
      buf->nlines = بفر_linelim (buf) - سطر;
      اگر (buf->nlines != 0)
        {
          اگر (! buf->eof && maybe_growbuf (buf, policy))
            جاری;
          buf->بایاں = ptr - سطر_آغاز;
          ملاؤ_بفر_سائز = mergesize + MIN_ملاؤ_بفر_سائز;
          واپس true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t سطر_alloc = buf->alloc / حجم (ساخت سطر);
        buf->buf = xpalloc (buf->buf, &سطر_alloc, 1, -1, حجم (ساخت سطر));
        buf->alloc = سطر_alloc * حجم (ساخت سطر);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ساکن حرف ثابت unit_ترتیب[UCHAR_LIM] =
  {
#اگر ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ورنہ
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
#اگر_ختم
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ساکن حرف
traverse_raw_نمبر (حرف ثابت **نمبر)
{
  حرف ثابت *p = *نمبر;
  حرف ch;
  حرف max_digit = '\0';
  bool سرے_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  جب (c_isdigit (ch = *p++))
    {
      اگر (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      سرے_with_thousands_sep = (*p == thousands_sep);
      اگر (سرے_with_thousands_sep)
        ++p;
    }

  اگر (سرے_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *نمبر = p - 2;
      واپس max_digit;
    }

  اگر (ch == decimal_نقطہ)
    جب (c_isdigit (ch = *p++))
      اگر (max_digit < ch)
        max_digit = ch;

  *نمبر = p - 1;
  واپس max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_خالص
ساکن صحیح
ڈھونڈو_unit_ترتیب (حرف ثابت *نمبر)
{
  bool minus_sign = (*نمبر == '-');
  حرف ثابت *p = نمبر + minus_sign;
  حرف max_digit = traverse_raw_نمبر (&p);
  اگر ('0' < max_digit)
    {
      غیرعلامتی حرف ch = *p;
      صحیح ترتیب = unit_ترتیب[ch];
      واپس (minus_sign ? -ترتیب : ترتیب);
    }
  ورنہ
    واپس 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_خالص
ساکن صحیح
human_numcompare (حرف ثابت *a, حرف ثابت *b)
{
  جب (blanks[مقصد_uchar (*a)])
    a++;
  جب (blanks[مقصد_uchar (*b)])
    b++;

  صحیح diff = ڈھونڈو_unit_ترتیب (a) - ڈھونڈو_unit_ترتیب (b);
  واپس (diff ? diff : strnumcmp (a, b, decimal_نقطہ, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_خالص
ساکن صحیح
numcompare (حرف ثابت *a, حرف ثابت *b)
{
  جب (blanks[مقصد_uchar (*a)])
    a++;
  جب (blanks[مقصد_uchar (*b)])
    b++;

  واپس strnumcmp (a, b, decimal_نقطہ, thousands_sep);
}

ساکن صحیح
nan_compare (طویل دوہرا a, طویل دوہرا b)
{
  حرف buf[2][حجم "-nan""()" + CHAR_BIT * حجم a];
  snprintf (buf[0], حجم buf[0], "%Lf", a);
  snprintf (buf[1], حجم buf[1], "%Lf", b);
  واپس strcmp (buf[0], buf[1]);
}

ساکن صحیح
general_numcompare (حرف ثابت *sa, حرف ثابت *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  حرف *ea;
  حرف *eb;
  طویل دوہرا a = strtold (sa, &ea);
  طویل دوہرا b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  اگر (sa == ea)
    واپس sb == eb ? 0 : -1;
  اگر (sb == eb)
    واپس 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  واپس (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ساکن صحیح
getmonth (حرف ثابت *month, حرف **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_فی_YEAR;

  جب (blanks[مقصد_uchar (*month)])
    month++;

  کرو
    {
      size_t ix = (lo + hi) / 2;
      حرف ثابت *m = month;
      حرف ثابت *n = monthtab[ix].name;

      برائے (;; m++, n++)
        {
          اگر (!*n)
            {
              اگر (ea)
                *ea = (حرف *) m;
              واپس monthtab[ix].val;
            }
          اگر (مقصد_uchar (fold_toupper[مقصد_uchar (*m)]) < مقصد_uchar (*n))
            {
              hi = ix;
              توڑو;
            }
          ورنہ اگر (مقصد_uchar (fold_toupper[مقصد_uchar (*m)]) > مقصد_uchar (*n))
            {
              lo = ix + 1;
              توڑو;
            }
        }
    }
  جب (lo < hi);

  واپس 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# اگر 14 <= __GNUC__
#  pragma GCC diagnostic ڈالو
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#اگر_ختم

#شامل "md5.h"

#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# اگر 14 <= __GNUC__
#  pragma GCC diagnostic نکالو
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_ترجیح_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ساکن خالی
link_failure (خالی)
{
  error (ترتیب_دو_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ساکن خالی *
علامت_پتہ (خالی *handle, حرف ثابت *علامت)
{
  خالی *پتہ = dlsym (handle, علامت);
  اگر (!پتہ)
    link_failure ();
  واپس پتہ;
}
#اگر_ختم

/* Dynamically link the crypto library, if it needs linking.  */
ساکن خالی
link_libcrypto (خالی)
{
#اگر DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  خالی *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  اگر (!handle)
    link_failure ();
  ptr_MD5_Init = علامت_پتہ (handle, "MD5_Init");
  ptr_MD5_Update = علامت_پتہ (handle, "MD5_Update");
  ptr_MD5_Final = علامت_پتہ (handle, "MD5_Final");
#اگر_ختم
}

/* A randomly chosen MD5 state, used for random comparison.  */
ساکن ساخت md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ساکن خالی
random_md5_state_init (حرف ثابت *random_ماخذ)
{
  غیرعلامتی حرف buf[MD5_DIGEST_سائز];
  ساخت randread_ماخذ *r = randread_new (random_ماخذ, حجم buf);
  اگر (! r)
    ترتیب_دو_die (_("open failed"), random_ماخذ ? random_ماخذ : "getrandom");
  randread (r, buf, حجم buf);
  اگر (randread_free (r) != 0)
    ترتیب_دو_die (_("close failed"), random_ماخذ);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_بائٹیں (buf, حجم buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ساکن size_t
xstrxfrm (حرف *محدود dest, حرف ثابت *محدود src, size_t destsize)
{
  errno = 0;
  size_t translated_سائز = strxfrm (dest, src, destsize);

  اگر (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (ترتیب_دو_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  واپس translated_سائز;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ساکن صحیح
compare_random (حرف *محدود texta, size_t lena,
                حرف *محدود textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  صحیح xfrm_diff = 0;

  حرف stackbuf[4000];
  حرف *buf = stackbuf;
  size_t bufsize = حجم stackbuf;
  خالی *allocated = تہی;
  uint32_t dig[2][MD5_DIGEST_سائز / حجم (uint32_t)];
  ساخت md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  اگر (hard_LC_COLLATE)
    {
      حرف ثابت *lima = texta + lena;
      حرف ثابت *limb = textb + lenb;

      جب (true)
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
          اگر (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              آزاد (allocated);
              buf = allocated = حافظہ_دو (bufsize);
              اگر (! buf)
                {
                  buf = stackbuf;
                  bufsize = حجم stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_سماتا = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_سماتا ? buf + sizea : تہی), textb,
                          (a_سماتا ? bufsize - sizea : 0))
                + 1)
             : 0);

          اگر (! (a_سماتا && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              اگر (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              آزاد (allocated);
              buf = allocated = xmalloc (bufsize);
              اگر (texta < lima)
                strxfrm (buf, texta, sizea);
              اگر (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          اگر (texta < lima)
            texta += لڑی_لمبائی (texta) + 1;
          اگر (textb < limb)
            textb += لڑی_لمبائی (textb) + 1;
          اگر (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              توڑو;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_بائٹیں (buf, sizea, &s[0]);
          md5_process_بائٹیں (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          اگر (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              اگر (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_بائٹیں (texta, lena, &s[0]); md5_اختتام_ctx (&s[0], dig[0]);
  md5_process_بائٹیں (textb, lenb, &s[1]); md5_اختتام_ctx (&s[1], dig[1]);
  صحیح diff = memcmp (dig[0], dig[1], حجم dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  اگر (! diff)
    {
      اگر (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          اگر (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  آزاد (allocated);

  واپس diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ساکن size_t
debug_چوڑائی (حرف ثابت *متن, حرف ثابت *lim)
{
  size_t چوڑائی = mbsnwidth (متن, lim - متن, 0);
  جب (متن < lim)
    چوڑائی += (*متن++ == '\t');
  واپس چوڑائی;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ساکن خالی
mark_کلید (size_t انحراف, size_t چوڑائی)
{
  جب (انحراف--)
    حرف_لکھو (' ');

  اگر (!چوڑائی)
    لکھو (_("^ no match for key\n"));
  ورنہ
    {
      کرو
        حرف_لکھو ('_');
      جب (--چوڑائی);

      حرف_لکھو ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ساکن اندرونی bool
کلید_numeric (ساخت keyfield ثابت *کلید)
{
  واپس کلید->numeric || کلید->general_numeric || کلید->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ساکن خالی
debug_کلید (ساخت سطر ثابت *سطر, ساخت keyfield ثابت *کلید)
{
  حرف *متن = سطر->متن;
  حرف *beg = متن;
  حرف *lim = متن + سطر->لمبائی - 1;

  اگر (کلید)
    {
      اگر (کلید->sword != SIZE_MAX)
        beg = begfield (سطر, کلید);
      اگر (کلید->eword != SIZE_MAX)
        {
          lim = limfield (سطر, کلید);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      اگر ((کلید->skipsblanks && کلید->sword == SIZE_MAX)
          || کلید->month || کلید_numeric (کلید))
        {
          حرف saved = *lim;
          *lim = '\0';

          جب (blanks[مقصد_uchar (*beg)])
            beg++;

          حرف *tighter_lim = beg;

          اگر (lim < beg)
            tighter_lim = lim;
          ورنہ اگر (کلید->month)
            getmonth (beg, &tighter_lim);
          ورنہ اگر (کلید->general_numeric)
            ignore_قدر (strtold (beg, &tighter_lim));
          ورنہ اگر (کلید->numeric || کلید->human_numeric)
            {
              حرف ثابت *p = beg + (beg < lim && *beg == '-');
              حرف max_digit = traverse_raw_نمبر (&p);
              اگر ('0' <= max_digit)
                {
                  غیرعلامتی حرف ch = *p;
                  tighter_lim = (حرف *) p
                    + (کلید->human_numeric && unit_ترتیب[ch]);
                }
            }
          ورنہ
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t انحراف = debug_چوڑائی (متن, beg);
  size_t چوڑائی = debug_چوڑائی (beg, lim);
  mark_کلید (انحراف, چوڑائی);
}

/* Debug LINE by underlining its keys.  */

ساکن خالی
debug_سطر (ساخت سطر ثابت *سطر)
{
  ساخت keyfield ثابت *کلید = keylist;

  کرو
    debug_کلید (سطر, کلید);
  جب (کلید && ((کلید = کلید->اگلا) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ساکن bool
default_کلید_compare (ساخت keyfield ثابت *کلید)
{
  واپس ! (کلید->ignore
            || کلید->translate
            || کلید->skipsblanks
            || کلید->skipeblanks
            || کلید_numeric (کلید)
            || کلید->month
            || کلید->نسخہ_3
            || کلید->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ساکن خالی
کلید_مقصد_opts (ساخت keyfield ثابت *کلید, حرف *opts)
{
  اگر (کلید->skipsblanks || کلید->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  اگر (کلید->ignore == nondictionary)
    *opts++ = 'd';
  اگر (کلید->translate)
    *opts++ = 'f';
  اگر (کلید->general_numeric)
    *opts++ = 'g';
  اگر (کلید->human_numeric)
    *opts++ = 'h';
  اگر (کلید->ignore == nonprinting)
    *opts++ = 'i';
  اگر (کلید->month)
    *opts++ = 'M';
  اگر (کلید->numeric)
    *opts++ = 'n';
  اگر (کلید->random)
    *opts++ = 'R';
  اگر (کلید->الٹ)
    *opts++ = 'r';
  اگر (کلید->نسخہ_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ساکن خالی
کلید_warnings (ساخت keyfield ثابت *gkey, bool gkey_only)
{
  ساخت keyfield ugkey = *gkey;
  غیرعلامتی طویل keynum = 1;
  bool basic_numeric_میدان = false;
  bool general_numeric_میدان = false;
  bool basic_numeric_میدان_وسعت = false;
  bool general_numeric_میدان_وسعت = false;

  برائے (ساخت keyfield *کلید = keylist; کلید; کلید = کلید->اگلا, keynum++)
    {
      اگر (کلید_numeric (کلید))
        {
          اگر (کلید->general_numeric)
            general_numeric_میدان = true;
          ورنہ
            basic_numeric_میدان = true;
        }

      اگر (کلید->traditional_مستعمل)
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

          اگر (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          اگر (کلید->eword != SIZE_MAX)
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
      bool zero_چوڑائی = کلید->sword != SIZE_MAX && کلید->eword < کلید->sword;
      اگر (zero_چوڑائی)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = کلید_numeric (کلید) || کلید->month;
      bool سطر_انحراف = کلید->eword == 0 && کلید->echar != 0; /* -k1.x,1.y  */
      اگر (!zero_چوڑائی && !gkey_only && tab == TAB_DEFAULT && !سطر_انحراف
          && ((!کلید->skipsblanks && !implicit_skip)
              || (!کلید->skipsblanks && کلید->schar)
              || (!کلید->skipeblanks && کلید->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      اگر (!gkey_only && کلید_numeric (کلید))
        {
          size_t sword = کلید->sword + 1;
          size_t eword = کلید->eword + 1;
          اگر (!sword)
            sword++;
          اگر (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              اگر (کلید->general_numeric)
                general_numeric_میدان_وسعت = true;
              ورنہ
                basic_numeric_میدان_وسعت = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      اگر (ugkey.ignore && (ugkey.ignore == کلید->ignore))
        ugkey.ignore = تہی;
      اگر (ugkey.translate && (ugkey.translate == کلید->translate))
        ugkey.translate = تہی;
      ugkey.skipsblanks &= !کلید->skipsblanks;
      ugkey.skipeblanks &= !کلید->skipeblanks;
      ugkey.month &= !کلید->month;
      ugkey.numeric &= !کلید->numeric;
      ugkey.general_numeric &= !کلید->general_numeric;
      ugkey.human_numeric &= !کلید->human_numeric;
      ugkey.random &= !کلید->random;
      ugkey.نسخہ_3 &= !کلید->نسخہ_3;
      ugkey.الٹ &= !کلید->الٹ;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool نمبر_locale_warned = false;
  اگر (basic_numeric_میدان_وسعت)
    {
      اگر (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (مقصد_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((حرف []) {thousands_sep, 0})));
          نمبر_locale_warned = true;
        }
    }
  اگر (basic_numeric_میدان_وسعت || general_numeric_میدان_وسعت)
    {
      اگر (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (مقصد_uchar (decimal_نقطہ)))
          : tab == decimal_نقطہ)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((حرف []) {decimal_نقطہ, 0})));
          نمبر_locale_warned = true;
        }
      ورنہ اگر (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((حرف []) {tab, 0})));
        }
      ورنہ اگر (general_numeric_میدان_وسعت && tab == '+')
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
  اگر ((basic_numeric_میدان || general_numeric_میدان) && ! نمبر_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((حرف []) {decimal_نقطہ, 0})));

    }

  اگر (basic_numeric_میدان && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  اگر (!default_کلید_compare (&ugkey)
      || (ugkey.الٹ && (stable || unique) && keylist))
    {
      bool ugkey_الٹ = ugkey.الٹ;
      اگر (!(stable || unique))
        ugkey.الٹ = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      حرف opts[حجم short_options];
      کلید_مقصد_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (لڑی_لمبائی (opts))), opts);
      ugkey.الٹ = ugkey_الٹ;
    }
  اگر (ugkey.الٹ && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ساکن صحیح
diff_reversed (صحیح diff, bool reversed)
{
  واپس reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ساکن صحیح
keycompare (ساخت سطر ثابت *a, ساخت سطر ثابت *b)
{
  ساخت keyfield *کلید = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  حرف *texta = a->keybeg;
  حرف *textb = b->keybeg;
  حرف *lima = a->keylim;
  حرف *limb = b->keylim;

  صحیح diff;

  جب (true)
    {
      حرف ثابت *translate = کلید->translate;
      bool ثابت *ignore = کلید->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      اگر (hard_LC_COLLATE || کلید_numeric (کلید)
          || کلید->month || کلید->random || کلید->نسخہ_3)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          حرف *ta = texta;
          حرف *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          حرف enda = ta[tlena];
          حرف endb = tb[tlenb];

          خالی *allocated = تہی;
          حرف stackbuf[4000];

          اگر (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t سائز = lena + 1 + lenb + 1;
              اگر (سائز <= حجم stackbuf)
                ta = stackbuf;
              ورنہ
                ta = allocated = xmalloc (سائز);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              برائے (tlena = i = 0; i < lena; i++)
                اگر (! (ignore && ignore[مقصد_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[مقصد_uchar (texta[i])]
                                 : texta[i]);

              برائے (tlenb = i = 0; i < lenb; i++)
                اگر (! (ignore && ignore[مقصد_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[مقصد_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          اگر (کلید->numeric)
            diff = numcompare (ta, tb);
          ورنہ اگر (کلید->general_numeric)
            diff = general_numcompare (ta, tb);
          ورنہ اگر (کلید->human_numeric)
            diff = human_numcompare (ta, tb);
          ورنہ اگر (کلید->month)
            diff = getmonth (ta, تہی) - getmonth (tb, تہی);
          ورنہ اگر (کلید->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ورنہ اگر (کلید->نسخہ_3)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ورنہ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              اگر (tlena == 0)
                diff = - NONZERO (tlenb);
              ورنہ اگر (tlenb == 0)
                diff = 1;
              ورنہ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          آزاد (allocated);
        }
      ورنہ اگر (ignore)
        {
#تعریف_کرو CMP_WITH_IGNORE(A, B)						\
  کرو									\
    {									\
          جب (true)							\
            {								\
              جب (texta < lima && ignore[مقصد_uchar (*texta)])		\
                ++texta;						\
              جب (textb < limb && ignore[مقصد_uchar (*textb)])		\
                ++textb;						\
              اگر (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  توڑو;						\
                }							\
              diff = مقصد_uchar (A) - مقصد_uchar (B);			\
              اگر (diff)							\
                توڑو;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  جب (0)

          اگر (translate)
            CMP_WITH_IGNORE (translate[مقصد_uchar (*texta)],
                             translate[مقصد_uchar (*textb)]);
          ورنہ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ورنہ
        {
          size_t lenmin = MIN (lena, lenb);
          اگر (lenmin == 0)
            diff = 0;
          ورنہ اگر (translate)
            {
              size_t i = 0;
              کرو
                {
                  diff = (مقصد_uchar (translate[مقصد_uchar (texta[i])])
                          - مقصد_uchar (translate[مقصد_uchar (textb[i])]));
                  اگر (diff)
                    توڑو;
                  i++;
                }
              جب (i < lenmin);
            }
          ورنہ
            diff = memcmp (texta, textb, lenmin);

          اگر (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      اگر (diff)
        توڑو;

      کلید = کلید->اگلا;
      اگر (! کلید)
        واپس 0;

      /* Find the beginning and limit of the next field.  */
      اگر (کلید->eword != SIZE_MAX)
        lima = limfield (a, کلید), limb = limfield (b, کلید);
      ورنہ
        lima = a->متن + a->لمبائی - 1, limb = b->متن + b->لمبائی - 1;

      اگر (کلید->sword != SIZE_MAX)
        texta = begfield (a, کلید), textb = begfield (b, کلید);
      ورنہ
        {
          texta = a->متن, textb = b->متن;
          اگر (کلید->skipsblanks)
            {
              جب (texta < lima && blanks[مقصد_uchar (*texta)])
                ++texta;
              جب (textb < limb && blanks[مقصد_uchar (*textb)])
                ++textb;
            }
        }
    }

  واپس diff_reversed (diff, کلید->الٹ);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ساکن صحیح
compare (ساخت سطر ثابت *a, ساخت سطر ثابت *b)
{
  صحیح diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  اگر (keylist)
    {
      diff = keycompare (a, b);
      اگر (diff || unique || stable)
        واپس diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->لمبائی - 1, blen = b->لمبائی - 1;

  اگر (alen == 0)
    diff = - NONZERO (blen);
  ورنہ اگر (blen == 0)
    diff = 1;
  ورنہ اگر (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->متن, alen + 1, b->متن, blen + 1);
    }
  ورنہ
    {
      diff = memcmp (a->متن, b->متن, MIN (alen, blen));
      اگر (!diff)
        diff = _GL_CMP (alen, blen);
    }

  واپس diff_reversed (diff, الٹ);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ساکن خالی
write_سطر (ساخت سطر ثابت *سطر, FILE *fp, حرف ثابت *output_file)
{
  حرف *buf = سطر->متن;
  size_t n_بائٹیں = سطر->لمبائی;
  حرف *ebuf = buf + n_بائٹیں;

  اگر (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      حرف ثابت *c = buf;

      جب (c < ebuf)
        {
          حرف wc = *c++;
          اگر (wc == '\t')
            wc = '>';
          ورنہ اگر (c == ebuf)
            wc = '\n';
          اگر (fputc (wc, fp) == EOF)
            ترتیب_دو_die (_("write failed"), output_file);
        }

      debug_سطر (سطر);
    }
  ورنہ
    {
      ebuf[-1] = eolchar;
      اگر (fwrite (buf, 1, n_بائٹیں, fp) != n_بائٹیں)
        ترتیب_دو_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ساکن bool
check (حرف ثابت *file_name, حرف checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ساخت بفر buf;		/* Input buffer. */
  ساخت سطر عارضی;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t سطر_نمبر = 0;
  ساخت keyfield ثابت *کلید = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, حجم (ساخت سطر),
           MAX (ملاؤ_بفر_سائز, ترتیب_دو_سائز));
  عارضی.متن = تہی;

  جب (fillbuf (&buf, fp, file_name, تہی))
    {
      ساخت سطر ثابت *سطر = بفر_linelim (&buf);
      ساخت سطر ثابت *linebase = سطر - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      اگر (alloc && nonunique <= compare (&عارضی, سطر - 1))
        {
        ملا_disorder:
          {
            اگر (checkonly == 'c')
              {
                ساخت سطر ثابت *disorder_سطر = سطر - 1;
                uintmax_t disorder_سطر_نمبر =
                  بفر_linelim (&buf) - disorder_سطر + سطر_نمبر;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         پروگرام_name, file_name, disorder_سطر_نمبر);
                write_سطر (disorder_سطر, stderr, _("standard error"));
              }

            ordered = false;
            توڑو;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      جب (linebase < --سطر)
        اگر (nonunique <= compare (سطر, سطر - 1))
          جاؤ ملا_disorder;

      سطر_نمبر += buf.nlines;

      /* Save the last line of the buffer.  */
      اگر (alloc < سطر->لمبائی)
        {
          کرو
            {
              alloc *= 2;
              اگر (! alloc)
                {
                  alloc = سطر->لمبائی;
                  توڑو;
                }
            }
          جب (alloc < سطر->لمبائی);

          آزاد (عارضی.متن);
          عارضی.متن = xmalloc (alloc);
        }
      حافظہ_نقل (عارضی.متن, سطر->متن, سطر->لمبائی);
      عارضی.لمبائی = سطر->لمبائی;
      اگر (کلید)
        {
          عارضی.keybeg = عارضی.متن + (سطر->keybeg - سطر->متن);
          عارضی.keylim = عارضی.متن + (سطر->keylim - سطر->متن);
        }
    }

  xfclose (fp, file_name);
  آزاد (buf.buf);
  آزاد (عارضی.متن);
  واپس ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ساکن size_t
open_داخلہ_files (ساخت sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, حجم *fps);
  صحیح i;

  /* Open as many input files as we can.  */
  برائے (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].عارضی && files[i].عارضی->state != UNCOMPRESSED
                ? open_عارضی (files[i].عارضی)
                : دھارا_open (files[i].name, "r"));
      اگر (!fps[i])
        توڑو;
    }

  واپس i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ساکن خالی
mergefps (ساخت sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, حرف ثابت *output_file, FILE **fps)
{
  ساخت بفر *بفر = xnmalloc (nfiles, حجم *بفر);
                                /* Input buffers for each file. */
  ساخت سطر saved;		/* Saved line storage for unique check. */
  ساخت سطر ثابت *savedline = تہی;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ساخت سطر ثابت **cur = xnmalloc (nfiles, حجم *cur);
                                /* Current line in each line table. */
  ساخت سطر ثابت **بنیاد_2 = xnmalloc (nfiles, حجم *بنیاد_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, حجم *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ساخت keyfield ثابت *کلید = keylist;
  saved.متن = تہی;

  /* Read initial lines from each input file. */
  برائے (size_t i = 0; i < nfiles; )
    {
      initbuf (&بفر[i], حجم (ساخت سطر),
               MAX (ملاؤ_بفر_سائز, ترتیب_دو_سائز / nfiles));
      اگر (fillbuf (&بفر[i], fps[i], files[i].name, تہی))
        {
          ساخت سطر ثابت *linelim = بفر_linelim (&بفر[i]);
          cur[i] = linelim - 1;
          بنیاد_2[i] = linelim - بفر[i].nlines;
          i++;
        }
      ورنہ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          اگر (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          آزاد (بفر[i].buf);
          --nfiles;
          برائے (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  برائے (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  برائے (size_t i = 1; i < nfiles; ++i)
    اگر (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  جب (nfiles)
    {
      ساخت سطر ثابت *سب_سے_چھوٹا = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      اگر (unique)
        {
          اگر (savedline && compare (savedline, سب_سے_چھوٹا))
            {
              savedline = تہی;
              write_سطر (&saved, ofp, output_file);
            }
          اگر (!savedline)
            {
              savedline = &saved;
              اگر (savealloc < سب_سے_چھوٹا->لمبائی)
                {
                  کرو
                    اگر (! savealloc)
                      {
                        savealloc = سب_سے_چھوٹا->لمبائی;
                        توڑو;
                      }
                  جب ((savealloc *= 2) < سب_سے_چھوٹا->لمبائی);

                  آزاد (saved.متن);
                  saved.متن = xmalloc (savealloc);
                }
              saved.لمبائی = سب_سے_چھوٹا->لمبائی;
              حافظہ_نقل (saved.متن, سب_سے_چھوٹا->متن, saved.لمبائی);
              اگر (کلید)
                {
                  saved.keybeg =
                    saved.متن + (سب_سے_چھوٹا->keybeg - سب_سے_چھوٹا->متن);
                  saved.keylim =
                    saved.متن + (سب_سے_چھوٹا->keylim - سب_سے_چھوٹا->متن);
                }
            }
        }
      ورنہ
        write_سطر (سب_سے_چھوٹا, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      اگر (بنیاد_2[ord[0]] < سب_سے_چھوٹا)
        cur[ord[0]] = سب_سے_چھوٹا - 1;
      ورنہ
        {
          اگر (fillbuf (&بفر[ord[0]], fps[ord[0]], files[ord[0]].name,
                       تہی))
            {
              ساخت سطر ثابت *linelim = بفر_linelim (&بفر[ord[0]]);
              cur[ord[0]] = linelim - 1;
              بنیاد_2[ord[0]] = linelim - بفر[ord[0]].nlines;
            }
          ورنہ
            {
              /* We reached EOF on fps[ord[0]].  */
              برائے (size_t i = 1; i < nfiles; ++i)
                اگر (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              اگر (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              آزاد (بفر[ord[0]].buf);
              برائے (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  بفر[i] = بفر[i + 1];
                  cur[i] = cur[i + 1];
                  بنیاد_2[i] = بنیاد_2[i + 1];
                }
              برائے (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              جاری;
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
        size_t گنتی_کا_smaller_سطریں;

        جب (lo < hi)
          {
            صحیح cmp = compare (cur[ord0], cur[ord[probe]]);
            اگر (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ورنہ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        گنتی_کا_smaller_سطریں = lo - 1;
        برائے (size_t j = 0; j < گنتی_کا_smaller_سطریں; j++)
          ord[j] = ord[j + 1];
        ord[گنتی_کا_smaller_سطریں] = ord0;
      }
    }

  اگر (unique && savedline)
    {
      write_سطر (&saved, ofp, output_file);
      آزاد (saved.متن);
    }

  xfclose (ofp, output_file);
  آزاد (fps);
  آزاد (بفر);
  آزاد (ord);
  آزاد (بنیاد_2);
  آزاد (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ساکن size_t
mergefiles (ساخت sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, حرف ثابت *output_file)
{
  FILE **fps;
  size_t nopened = open_داخلہ_files (files, nfiles, &fps);
  اگر (nopened < nfiles && nopened < 2)
    ترتیب_دو_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  واپس nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ساکن خالی
mergelines (ساخت سطر *محدود t, size_t nlines,
            ساخت سطر ثابت *محدود lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ساخت سطر *hi = t - nlo;

  جب (true)
    اگر (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        اگر (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            واپس;
          }
      }
    ورنہ
      {
        *--t = *--hi;
        اگر (! --nhi)
          {
            کرو
              *--t = *--lo;
            جب (--nlo);

            واپس;
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

ساکن خالی
sequential_ترتیب_دو (ساخت سطر *محدود سطریں, size_t nlines,
                 ساخت سطر *محدود عارضی, bool مقصد_عارضی)
{
  اگر (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      صحیح تبادلہ = (0 < compare (&سطریں[-1], &سطریں[-2]));
      اگر (مقصد_عارضی)
        {
          عارضی[-1] = سطریں[-1 - تبادلہ];
          عارضی[-2] = سطریں[-2 + تبادلہ];
        }
      ورنہ اگر (تبادلہ)
        {
          عارضی[-1] = سطریں[-1];
          سطریں[-1] = سطریں[-2];
          سطریں[-2] = عارضی[-1];
        }
    }
  ورنہ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ساخت سطر *lo = سطریں;
      ساخت سطر *hi = سطریں - nlo;

      sequential_ترتیب_دو (hi, nhi, عارضی - (مقصد_عارضی ? nlo : 0), مقصد_عارضی);
      اگر (1 < nlo)
        sequential_ترتیب_دو (lo, nlo, عارضی, !مقصد_عارضی);
      ورنہ اگر (!مقصد_عارضی)
        عارضی[-1] = lo[-1];

      ساخت سطر *dest;
      ساخت سطر ثابت *sorted_lo;
      اگر (مقصد_عارضی)
        {
          dest = عارضی;
          sorted_lo = سطریں;
        }
      ورنہ
        {
          dest = سطریں;
          sorted_lo = عارضی;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ساکن ساخت ملاؤ_گرہ *init_گرہ (ساخت ملاؤ_گرہ *محدود,
                                     ساخت ملاؤ_گرہ *محدود,
                                     ساخت سطر *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ساکن ساخت ملاؤ_گرہ *
ملاؤ_درخت_init (size_t nthreads, size_t nlines, ساخت سطر *dest)
{
  ساخت ملاؤ_گرہ *ملاؤ_درخت = xmalloc (2 * حجم *ملاؤ_درخت * nthreads);

  ساخت ملاؤ_گرہ *جڑ = ملاؤ_درخت;
  جڑ->lo = جڑ->hi = جڑ->انتہا_lo = جڑ->انتہا_hi = تہی;
  جڑ->dest = تہی;
  جڑ->nlo = جڑ->nhi = nlines;
  جڑ->والد = تہی;
  جڑ->level = ملاؤ_انتہا;
  جڑ->queued = false;
  pthread_mutex_init (&جڑ->تالا, تہی);

  init_گرہ (جڑ, جڑ + 1, dest, nthreads, nlines, false);
  واپس ملاؤ_درخت;
}

/* Destroy the merge tree. */
ساکن خالی
ملاؤ_درخت_destroy (size_t nthreads, ساخت ملاؤ_گرہ *ملاؤ_درخت)
{
  size_t n_nodes = nthreads * 2;
  ساخت ملاؤ_گرہ *گرہ = ملاؤ_درخت;

  جب (n_nodes--)
    {
      pthread_mutex_destroy (&گرہ->تالا);
      گرہ++;
    }

  آزاد (ملاؤ_درخت);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ساکن ساخت ملاؤ_گرہ *
init_گرہ (ساخت ملاؤ_گرہ *محدود والد,
           ساخت ملاؤ_گرہ *محدود گرہ_pool,
           ساخت سطر *dest, size_t nthreads,
           size_t کل_سطریں, bool is_lo_بچہ)
{
  size_t nlines = (is_lo_بچہ ? والد->nlo : والد->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ساخت سطر *lo = dest - کل_سطریں;
  ساخت سطر *hi = lo - nlo;
  ساخت سطر **والد_انتہا = (is_lo_بچہ ? &والد->انتہا_lo : &والد->انتہا_hi);

  ساخت ملاؤ_گرہ *گرہ = گرہ_pool++;
  گرہ->lo = گرہ->انتہا_lo = lo;
  گرہ->hi = گرہ->انتہا_hi = hi;
  گرہ->dest = والد_انتہا;
  گرہ->nlo = nlo;
  گرہ->nhi = nhi;
  گرہ->والد = والد;
  گرہ->level = والد->level + 1;
  گرہ->queued = false;
  pthread_mutex_init (&گرہ->تالا, تہی);

  اگر (nthreads > 1)
    {
      size_t lo_دھاگے = nthreads / 2;
      size_t hi_دھاگے = nthreads - lo_دھاگے;
      گرہ->lo_بچہ = گرہ_pool;
      گرہ_pool = init_گرہ (گرہ, گرہ_pool, lo, lo_دھاگے,
                             کل_سطریں, true);
      گرہ->hi_بچہ = گرہ_pool;
      گرہ_pool = init_گرہ (گرہ, گرہ_pool, hi, hi_دھاگے,
                             کل_سطریں, false);
    }
  ورنہ
    {
      گرہ->lo_بچہ = تہی;
      گرہ->hi_بچہ = تہی;
    }
  واپس گرہ_pool;
}


/* Compare two merge nodes A and B for priority.  */

ساکن صحیح
compare_nodes (خالی ثابت *a, خالی ثابت *b)
{
  ساخت ملاؤ_گرہ ثابت *nodea = a;
  ساخت ملاؤ_گرہ ثابت *nodeb = b;
  اگر (nodea->level == nodeb->level)
      واپس (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  واپس nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ساکن اندرونی خالی
تالا_گرہ (ساخت ملاؤ_گرہ *گرہ)
{
  pthread_mutex_تالا (&گرہ->تالا);
}

/* Unlock a merge tree NODE. */

ساکن اندرونی خالی
unlock_گرہ (ساخت ملاؤ_گرہ *گرہ)
{
  pthread_mutex_unlock (&گرہ->تالا);
}

/* Destroy merge QUEUE. */

ساکن خالی
قطار_destroy (ساخت ملاؤ_گرہ_قطار *قطار)
{
  ڈھیر_free (قطار->ترجیح_قطار);
  pthread_cond_destroy (&قطار->cond);
  pthread_mutex_destroy (&قطار->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ساکن خالی
قطار_init (ساخت ملاؤ_گرہ_قطار *قطار, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  قطار->ترجیح_قطار = ڈھیر_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&قطار->mutex, تہی);
  pthread_cond_init (&قطار->cond, تہی);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ساکن خالی
قطار_داخل_کرو (ساخت ملاؤ_گرہ_قطار *قطار, ساخت ملاؤ_گرہ *گرہ)
{
  pthread_mutex_تالا (&قطار->mutex);
  ڈھیر_داخل_کرو (قطار->ترجیح_قطار, گرہ);
  گرہ->queued = true;
  pthread_cond_signal (&قطار->cond);
  pthread_mutex_unlock (&قطار->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ساکن ساخت ملاؤ_گرہ *
قطار_نکالو (ساخت ملاؤ_گرہ_قطار *قطار)
{
  ساخت ملاؤ_گرہ *گرہ;
  pthread_mutex_تالا (&قطار->mutex);
  جب (! (گرہ = ڈھیر_remove_چوٹی (قطار->ترجیح_قطار)))
    pthread_cond_wait (&قطار->cond, &قطار->mutex);
  pthread_mutex_unlock (&قطار->mutex);
  تالا_گرہ (گرہ);
  گرہ->queued = false;
  واپس گرہ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ساکن خالی
write_unique (ساخت سطر ثابت *سطر, FILE *tfp, حرف ثابت *عارضی_output)
{
  اگر (unique)
    {
      اگر (saved_سطر.متن && ! compare (سطر, &saved_سطر))
        واپس;
      saved_سطر = *سطر;
    }

  write_سطر (سطر, tfp, عارضی_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ساکن خالی
mergelines_گرہ (ساخت ملاؤ_گرہ *محدود گرہ, size_t کل_سطریں,
                 FILE *tfp, حرف ثابت *عارضی_output)
{
  ساخت سطر *lo_orig = گرہ->lo;
  ساخت سطر *hi_orig = گرہ->hi;
  size_t مقصد_ملاؤ = MAX_ملاؤ (کل_سطریں, گرہ->level);
  size_t merged_lo;
  size_t merged_hi;

  اگر (گرہ->level > ملاؤ_جڑ)
    {
      /* Merge to destination buffer. */
      ساخت سطر *dest = *گرہ->dest;
      جب (گرہ->lo != گرہ->انتہا_lo && گرہ->hi != گرہ->انتہا_hi && مقصد_ملاؤ--)
        اگر (compare (گرہ->lo - 1, گرہ->hi - 1) <= 0)
          *--dest = *--گرہ->lo;
        ورنہ
          *--dest = *--گرہ->hi;

      merged_lo = lo_orig - گرہ->lo;
      merged_hi = hi_orig - گرہ->hi;

      اگر (گرہ->nhi == merged_hi)
        جب (گرہ->lo != گرہ->انتہا_lo && مقصد_ملاؤ--)
          *--dest = *--گرہ->lo;
      ورنہ اگر (گرہ->nlo == merged_lo)
        جب (گرہ->hi != گرہ->انتہا_hi && مقصد_ملاؤ--)
          *--dest = *--گرہ->hi;
      *گرہ->dest = dest;
    }
  ورنہ
    {
      /* Merge directly to output. */
      جب (گرہ->lo != گرہ->انتہا_lo && گرہ->hi != گرہ->انتہا_hi && مقصد_ملاؤ--)
        {
          اگر (compare (گرہ->lo - 1, گرہ->hi - 1) <= 0)
            write_unique (--گرہ->lo, tfp, عارضی_output);
          ورنہ
            write_unique (--گرہ->hi, tfp, عارضی_output);
        }

      merged_lo = lo_orig - گرہ->lo;
      merged_hi = hi_orig - گرہ->hi;

      اگر (گرہ->nhi == merged_hi)
        {
          جب (گرہ->lo != گرہ->انتہا_lo && مقصد_ملاؤ--)
            write_unique (--گرہ->lo, tfp, عارضی_output);
        }
      ورنہ اگر (گرہ->nlo == merged_lo)
        {
          جب (گرہ->hi != گرہ->انتہا_hi && مقصد_ملاؤ--)
            write_unique (--گرہ->hi, tfp, عارضی_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - گرہ->lo;
  merged_hi = hi_orig - گرہ->hi;
  گرہ->nlo -= merged_lo;
  گرہ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ساکن خالی
قطار_check_داخل_کرو (ساخت ملاؤ_گرہ_قطار *قطار, ساخت ملاؤ_گرہ *گرہ)
{
  اگر (! گرہ->queued)
    {
      bool lo_avail = (گرہ->lo - گرہ->انتہا_lo) != 0;
      bool hi_avail = (گرہ->hi - گرہ->انتہا_hi) != 0;
      اگر (lo_avail ? hi_avail || ! گرہ->nhi : hi_avail && ! گرہ->nlo)
        قطار_داخل_کرو (قطار, گرہ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ساکن خالی
قطار_check_داخل_کرو_والد (ساخت ملاؤ_گرہ_قطار *قطار,
                           ساخت ملاؤ_گرہ *گرہ)
{
  اگر (گرہ->level > ملاؤ_جڑ)
    {
      تالا_گرہ (گرہ->والد);
      قطار_check_داخل_کرو (قطار, گرہ->والد);
      unlock_گرہ (گرہ->والد);
    }
  ورنہ اگر (گرہ->nlo + گرہ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      قطار_داخل_کرو (قطار, گرہ->والد);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ساکن خالی
ملاؤ_loop (ساخت ملاؤ_گرہ_قطار *قطار,
            size_t کل_سطریں, FILE *tfp, حرف ثابت *عارضی_output)
{
  جب (true)
    {
      ساخت ملاؤ_گرہ *گرہ = قطار_نکالو (قطار);

      اگر (گرہ->level == ملاؤ_انتہا)
        {
          unlock_گرہ (گرہ);
          /* Reinsert so other threads can pop it. */
          قطار_داخل_کرو (قطار, گرہ);
          توڑو;
        }
      mergelines_گرہ (گرہ, کل_سطریں, tfp, عارضی_output);
      قطار_check_داخل_کرو (قطار, گرہ);
      قطار_check_داخل_کرو_والد (قطار, گرہ);

      unlock_گرہ (گرہ);
    }
}


ساکن خالی sortlines (ساخت سطر *محدود, size_t, size_t,
                       ساخت ملاؤ_گرہ *, ساخت ملاؤ_گرہ_قطار *,
                       FILE *, حرف ثابت *);

/* Thread arguments for sortlines_thread. */

ساخت thread_دلائل
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ساخت سطر *سطریں;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ثابت کل_سطریں;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ساخت ملاؤ_گرہ *ثابت گرہ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ساخت ملاؤ_گرہ_قطار *ثابت قطار;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  حرف ثابت *output_عارضی;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ساکن خالی *
sortlines_thread (خالی *ڈیٹا)
{
  ساخت thread_دلائل ثابت *دلائل = ڈیٹا;
  sortlines (دلائل->سطریں, دلائل->nthreads, دلائل->کل_سطریں,
             دلائل->گرہ, دلائل->قطار, دلائل->tfp,
             دلائل->output_عارضی);
  واپس تہی;
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

ساکن خالی
sortlines (ساخت سطر *محدود سطریں, size_t nthreads,
           size_t کل_سطریں, ساخت ملاؤ_گرہ *گرہ,
           ساخت ملاؤ_گرہ_قطار *قطار, FILE *tfp, حرف ثابت *عارضی_output)
{
  size_t nlines = گرہ->nlo + گرہ->nhi;

  /* Calculate thread arguments. */
  size_t lo_دھاگے = nthreads / 2;
  size_t hi_دھاگے = nthreads - lo_دھاگے;
  pthread_t thread;
  ساخت thread_دلائل دلائل = {سطریں, lo_دھاگے, کل_سطریں,
                             گرہ->lo_بچہ, قطار, tfp, عارضی_output};

  اگر (nthreads > 1 && SUBTHREAD_سطریں_HEURISTIC <= nlines
      && pthread_create (&thread, تہی, sortlines_thread, &دلائل) == 0)
    {
      sortlines (سطریں - گرہ->nlo, hi_دھاگے, کل_سطریں,
                 گرہ->hi_بچہ, قطار, tfp, عارضی_output);
      pthread_join (thread, تہی);
    }
  ورنہ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = گرہ->nlo;
      size_t nhi = گرہ->nhi;
      ساخت سطر *عارضی = سطریں - کل_سطریں;
      اگر (1 < nhi)
        sequential_ترتیب_دو (سطریں - nlo, nhi, عارضی - nlo / 2, false);
      اگر (1 < nlo)
        sequential_ترتیب_دو (سطریں, nlo, عارضی, false);

      /* Update merge NODE. No need to lock yet. */
      گرہ->lo = سطریں;
      گرہ->hi = سطریں - nlo;
      گرہ->انتہا_lo = سطریں - nlo;
      گرہ->انتہا_hi = سطریں - nlo - nhi;

      قطار_داخل_کرو (قطار, گرہ);
      ملاؤ_loop (قطار, کل_سطریں, tfp, عارضی_output);
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

ساکن خالی
avoid_trashing_داخلہ (ساخت sortfile *files, size_t ntemps,
                      size_t nfiles, حرف ثابت *outfile)
{
  ساخت tempnode *tempcopy = تہی;

  برائے (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ساخت stat instat;

      اگر (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ورنہ
        {
          ساخت stat *outst = get_outstatus ();
          اگر (!outst)
            توڑو;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      اگر (same)
        {
          اگر (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_عارضی (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].عارضی = tempcopy;
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

ساکن خالی
check_inputs (حرف *ثابت *files, size_t nfiles)
{
  برائے (size_t i = 0; i < nfiles; i++)
    {
      اگر (streq (files[i], "-"))
        جاری;

      اگر (euidaccess (files[i], R_OK) != 0)
        ترتیب_دو_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ساکن خالی
check_output (حرف ثابت *outfile)
{
  اگر (outfile)
    {
      صحیح oflags = O_WRONLY | O_ثنائی | O_CLOEXEC | O_CREAT;
      صحیح outfd = open (outfile, oflags, منوال_RW_UGO);
      اگر (outfd < 0)
        ترتیب_دو_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ساکن خالی
ملاؤ (ساخت sortfile *files, size_t ntemps, size_t nfiles,
       حرف ثابت *output_file)
{
  جب (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t اندر;

      /* Number of output files generated so far.  */
      size_t برآمد;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      برائے (برآمد = اندر = 0; nmerge <= nfiles - اندر; برآمد++)
        {
          FILE *tfp;
          ساخت tempnode *عارضی = create_عارضی (&tfp);
          size_t عدد_merged = mergefiles (&files[اندر], MIN (ntemps, nmerge),
                                          nmerge, tfp, عارضی->name);
          ntemps -= MIN (ntemps, عدد_merged);
          files[برآمد].name = عارضی->name;
          files[برآمد].عارضی = عارضی;
          اندر += عدد_merged;
        }

      remainder = nfiles - اندر;
      cheap_slots = nmerge - برآمد % nmerge;

      اگر (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ساخت tempnode *عارضی = create_عارضی (&tfp);
          size_t عدد_merged = mergefiles (&files[اندر], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, عارضی->name);
          ntemps -= MIN (ntemps, عدد_merged);
          files[برآمد].name = عارضی->name;
          files[برآمد++].عارضی = عارضی;
          اندر += عدد_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[برآمد], &files[اندر], (nfiles - اندر) * حجم *files);
      ntemps += برآمد;
      nfiles -= اندر - برآمد;
    }

  avoid_trashing_داخلہ (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  جب (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_داخلہ_files (files, nfiles, &fps);

      اگر (nopened == nfiles)
        {
          FILE *ofp = دھارا_open (output_file, "w");
          اگر (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              توڑو;
            }
          اگر (errno != EMFILE || nopened <= 2)
            ترتیب_دو_die (_("open failed"), output_file);
        }
      ورنہ اگر (nopened <= 2)
        ترتیب_دو_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ساخت tempnode *عارضی;
      کرو
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          عارضی = maybe_create_عارضی (&tfp, ! (nopened <= 2));
        }
      جب (!عارضی);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, عارضی->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = عارضی->name;
      files[0].عارضی = عارضی;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * حجم *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ساکن خالی
ترتیب_دو (حرف *ثابت *files, size_t nfiles, حرف ثابت *output_file,
      size_t nthreads)
{
  ساخت بفر buf;
  ساخت ترتیب_دو_بفر_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.حد_بندی = 0;
  policy.growth_failed = false;

  جب (nfiles)
    {
      حرف ثابت *عارضی_output;
      حرف ثابت *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t بائٹیں_فی_سطر;
      اگر (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          جب (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          بائٹیں_فی_سطر = (mult * حجم (ساخت سطر));
        }
      ورنہ
        بائٹیں_فی_سطر = حجم (ساخت سطر) * 3 / 2;

      اگر (! buf.alloc)
        {
          ترتیب_دو_بفر_policy (&fp, 1, files, nfiles, بائٹیں_فی_سطر,
                              &policy);
          initbuf (&buf, بائٹیں_فی_سطر, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      جب (fillbuf (&buf, fp, file, &policy))
        {
          ساخت سطر *سطر;

          اگر (buf.eof && nfiles
              && (بائٹیں_فی_سطر + 1
                  < (buf.alloc - buf.مستعمل - بائٹیں_فی_سطر * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.بایاں = buf.مستعمل;
              توڑو;
            }

          saved_سطر.متن = تہی;
          سطر = بفر_linelim (&buf);
          اگر (buf.eof && !nfiles && !ntemps && !buf.بایاں)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              عارضی_output = output_file;
              output_file_created = true;
            }
          ورنہ
            {
              ++ntemps;
              عارضی_output = create_عارضی (&tfp)->name;
            }
          اگر (1 < buf.nlines)
            {
              اگر (nthreads > 1)
                {
                  ساخت ملاؤ_گرہ_قطار قطار;
                  قطار_init (&قطار, nthreads);
                  ساخت ملاؤ_گرہ *ملاؤ_درخت =
                    ملاؤ_درخت_init (nthreads, buf.nlines, سطر);

                  sortlines (سطر, nthreads, buf.nlines, ملاؤ_درخت + 1,
                             &قطار, tfp, عارضی_output);

                  ملاؤ_درخت_destroy (nthreads, ملاؤ_درخت);
                  قطار_destroy (&قطار);
                }
              ورنہ
                {
                  sequential_ترتیب_دو (سطر, buf.nlines,
                                   سطر - buf.nlines, false);
                  برائے (size_t i = 0; i < buf.nlines; i++)
                    write_unique (سطر - i - 1, tfp, عارضی_output);
                }
            }
          ورنہ
            write_unique (سطر - 1, tfp, عارضی_output);

          xfclose (tfp, عارضی_output);

          اگر (output_file_created)
            جاؤ اختتام;
        }
      xfclose (fp, file);
    }

 اختتام:
  آزاد (buf.buf);

  اگر (! output_file_created)
    {
      ساخت tempnode *گرہ = temphead;
      ساخت sortfile *tempfiles = xnmalloc (ntemps, حجم *tempfiles);
      برائے (size_t i = 0; گرہ; i++)
        {
          tempfiles[i].name = گرہ->name;
          tempfiles[i].عارضی = گرہ;
          گرہ = گرہ->اگلا;
        }
      ملاؤ (tempfiles, ntemps, ntemps, output_file);
      آزاد (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ساکن خالی
insertkey (ساخت keyfield *کلید_arg)
{
  ساخت keyfield **p;
  ساخت keyfield *کلید = xmemdup (کلید_arg, حجم *کلید);

  برائے (p = &keylist; *p; p = &(*p)->اگلا)
    جاری;
  *p = کلید;
  کلید->اگلا = تہی;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ساکن خالی
badfieldspec (حرف ثابت *spec, حرف ثابت *msgid)
{
  error (ترتیب_دو_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ساکن خالی
incompatible_options (حرف ثابت *opts)
{
  error (ترتیب_دو_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ساکن خالی
check_ordering_compatibility (خالی)
{
  برائے (ساخت keyfield *کلید = keylist; کلید; کلید = کلید->اگلا)
    اگر (1 < (کلید->numeric + کلید->general_numeric + کلید->human_numeric
             + کلید->month + (کلید->نسخہ_3 | کلید->random | !!کلید->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        حرف opts[حجم short_options];
        /* Clear flags we're not interested in.  */
        کلید->skipsblanks = کلید->skipeblanks = کلید->الٹ = false;
        کلید_مقصد_opts (کلید, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ساکن حرف ثابت *
parse_میدان_گنتی (حرف ثابت *string, size_t *val, حرف ثابت *msgid)
{
  حرف *suffix;
  uintmax_t n;

  انتخاب (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    حالت LONGINT_OK:
    حالت LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      اگر (*val == n)
        توڑو;
      FALLTHROUGH;
    حالت LONGINT_حد_سے_تجاوز:
    حالت LONGINT_حد_سے_تجاوز | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      توڑو;

    حالت LONGINT_INVALID:
      اگر (msgid)
        error (ترتیب_دو_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      واپس تہی;
    }

  واپس suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ساکن حرف *
سیٹ_ordering (حرف ثابت *s, ساخت keyfield *کلید, شمار blanktype blanktype)
{
  جب (*s)
    {
      انتخاب (*s)
        {
        حالت 'b':
          اگر (blanktype == bl_آغاز || blanktype == bl_both)
            کلید->skipsblanks = true;
          اگر (blanktype == bl_انتہا || blanktype == bl_both)
            کلید->skipeblanks = true;
          توڑو;
        حالت 'd':
          کلید->ignore = nondictionary;
          توڑو;
        حالت 'f':
          کلید->translate = fold_toupper;
          توڑو;
        حالت 'g':
          کلید->general_numeric = true;
          توڑو;
        حالت 'h':
          کلید->human_numeric = true;
          توڑو;
        حالت 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          اگر (! کلید->ignore)
            کلید->ignore = nonprinting;
          توڑو;
        حالت 'M':
          کلید->month = true;
          توڑو;
        حالت 'n':
          کلید->numeric = true;
          توڑو;
        حالت 'R':
          کلید->random = true;
          توڑو;
        حالت 'r':
          کلید->الٹ = true;
          توڑو;
        حالت 'V':
          کلید->نسخہ_3 = true;
          توڑو;
        طےشدہ:
          واپس (حرف *) s;
        }
      ++s;
    }
  واپس (حرف *) s;
}

/* Initialize KEY.  */

ساکن ساخت keyfield *
کلید_init (ساخت keyfield *کلید)
{
  memset (کلید, 0, حجم *کلید);
  کلید->eword = SIZE_MAX;
  واپس کلید;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  ساخت keyfield *کلید;
  ساخت keyfield کلید_buf;
  ساخت keyfield gkey;
  bool gkey_only = false;
  حرف ثابت *s;
  صحیح c = 0;
  حرف checkonly = 0;
  bool mergeonly = false;
  حرف *random_ماخذ = تہی;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != تہی);
  صحیح posix_ver = posix2_نسخہ ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  حرف **files;
  حرف *files_from = تہی;
  ساخت علامات tok;
  حرف ثابت *outfile = تہی;
  bool locale_ok;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ترتیب_دو_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#اگر HAVE_NL_LANGINFO
  hard_LC_وقت = hard_locale (LC_وقت);
#اگر_ختم

  /* Get locale's representation of the decimal point.  */
  {
    ساخت lconv ثابت *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_نقطہ = locale->decimal_نقطہ[0];
    اگر (! decimal_نقطہ || locale->decimal_نقطہ[1])
      decimal_نقطہ = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    اگر (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    اگر (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    شمار { nsigs = countof (رکن_sig) };

    ساخت sigaction act;

    sigemptyset (&caught_اشارات);
    برائے (size_t i = 0; i < nsigs; i++)
      {
        اگر (رکن_sig[i] == SIGPIPE)
          جاری;  /* Handled below.  */
        sigaction (رکن_sig[i], تہی, &act);
        اگر (act.sa_handler != SIG_IGN)
          sigaddset (&caught_اشارات, رکن_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_اشارات;
    act.sa_flags = 0;

    برائے (size_t i = 0; i < nsigs; i++)
      {
        اگر (رکن_sig[i] == SIGPIPE)
          جاری;  /* Handled below.  */
        اگر (sigismember (&caught_اشارات, رکن_sig[i]))
          sigaction (رکن_sig[i], &act, تہی);
      }
  }
  اشارہ (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  کلید_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, حجم *files);

  جب (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      صحیح oi = -1;

      اگر (c == -1
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
          اگر (argc <= optind)
            توڑو;
          files[nfiles++] = argv[optind++];
        }
      ورنہ انتخاب (c)
        {
        حالت 1:
          کلید = تہی;
          اگر (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              اگر (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  کلید = کلید_init (&کلید_buf);
                  s = parse_میدان_گنتی (optarg + 1, &کلید->sword, تہی);
                  اگر (s && *s == '.')
                    s = parse_میدان_گنتی (s + 1, &کلید->schar, تہی);
                  اگر (! (کلید->sword || کلید->schar))
                    کلید->sword = SIZE_MAX;
                  اگر (! s || *سیٹ_ordering (s, کلید, bl_آغاز))
                    کلید = تہی;
                  ورنہ
                    {
                      اگر (minus_pos_usage)
                        {
                          حرف ثابت *optarg1 = argv[optind++];
                          s = parse_میدان_گنتی (optarg1 + 1, &کلید->eword,
                                             N_("invalid number after '-'"));
                          اگر (*s == '.')
                            s = parse_میدان_گنتی (s + 1, &کلید->echar,
                                               N_("invalid number after '.'"));
                          اگر (!کلید->echar && کلید->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              کلید->eword--;
                            }
                          اگر (*سیٹ_ordering (s, کلید, bl_انتہا))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      کلید->traditional_مستعمل = true;
                      insertkey (کلید);
                    }
                }
            }
          اگر (! کلید)
            files[nfiles++] = optarg;
          توڑو;

        حالت ترتیب_دو_OPTION:
          c = XARGMATCH ("--sort", optarg, ترتیب_دو_دلائل, ترتیب_دو_types);
          FALLTHROUGH;
        حالت 'b':
        حالت 'd':
        حالت 'f':
        حالت 'g':
        حالت 'h':
        حالت 'i':
        حالت 'M':
        حالت 'n':
        حالت 'r':
        حالت 'R':
        حالت 'V':
          {
            حرف str[2];
            str[0] = c;
            str[1] = '\0';
            سیٹ_ordering (str, &gkey, bl_both);
          }
          توڑو;

        حالت CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_دلائل, check_types)
               : 'c');
          FALLTHROUGH;
        حالت 'c':
        حالت 'C':
          اگر (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          توڑو;

        حالت COMPRESS_پروگرام_OPTION:
          اگر (compress_پروگرام && !streq (compress_پروگرام, optarg))
            error (ترتیب_دو_FAILURE, 0, _("multiple compress programs specified"));
          compress_پروگرام = optarg;
          توڑو;

        حالت DEBUG_پروگرام_OPTION:
          debug = true;
          توڑو;

        حالت FILES0_FROM_OPTION:
          files_from = optarg;
          توڑو;

        حالت 'k':
          کلید = کلید_init (&کلید_buf);

          /* Get POS1. */
          s = parse_میدان_گنتی (optarg, &کلید->sword,
                                 N_("invalid number at field start"));
          اگر (! کلید->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          اگر (*s == '.')
            {
              s = parse_میدان_گنتی (s + 1, &کلید->schar,
                                     N_("invalid number after '.'"));
              اگر (! کلید->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          اگر (! (کلید->sword || کلید->schar))
            کلید->sword = SIZE_MAX;
          s = سیٹ_ordering (s, کلید, bl_آغاز);
          اگر (*s != ',')
            {
              کلید->eword = SIZE_MAX;
              کلید->echar = 0;
            }
          ورنہ
            {
              /* Get POS2. */
              s = parse_میدان_گنتی (s + 1, &کلید->eword,
                                     N_("invalid number after ','"));
              اگر (! کلید->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              اگر (*s == '.')
                {
                  s = parse_میدان_گنتی (s + 1, &کلید->echar,
                                         N_("invalid number after '.'"));
                }
              s = سیٹ_ordering (s, کلید, bl_انتہا);
            }
          اگر (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (کلید);
          توڑو;

        حالت 'm':
          mergeonly = true;
          توڑو;

        حالت NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          توڑو;

        حالت 'o':
          اگر (outfile && !streq (outfile, optarg))
            error (ترتیب_دو_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          توڑو;

        حالت RANDOM_ماخذ_OPTION:
          اگر (random_ماخذ && !streq (random_ماخذ, optarg))
            error (ترتیب_دو_FAILURE, 0, _("multiple random sources specified"));
          random_ماخذ = optarg;
          توڑو;

        حالت 's':
          stable = true;
          توڑو;

        حالت 'S':
          specify_ترتیب_دو_سائز (oi, c, optarg);
          توڑو;

        حالت 't':
          {
            حرف newtab = optarg[0];
            اگر (! newtab)
              error (ترتیب_دو_FAILURE, 0, _("empty tab"));
            اگر (optarg[1])
              {
                اگر (streq (optarg, "\\0"))
                  newtab = '\0';
                ورنہ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (ترتیب_دو_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            اگر (tab != TAB_DEFAULT && tab != newtab)
              error (ترتیب_دو_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          توڑو;

        حالت 'T':
          جوڑو_عارضی_dir (optarg);
          توڑو;

        حالت PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          توڑو;

        حالت 'u':
          unique = true;
          توڑو;

        حالت 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          اگر (optarg == argv[optind - 1])
            {
              حرف ثابت *p;
              برائے (p = optarg; c_isdigit (*p); p++)
                جاری;
              optind -= (*p != '\0');
            }
          توڑو;

        حالت 'z':
          eolchar = 0;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (ترتیب_دو_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (اشارہ (SIGPIPE, SIG_IGN) == SIG_DFL);

  اگر (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      اگر (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (ترتیب_دو_FAILURE);
        }

      FILE *دھارا = xfopen (files_from, "r");

      readtokens0_init (&tok);

      اگر (! readtokens0 (دھارا, &tok))
        error (ترتیب_دو_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (دھارا, files_from);

      اگر (tok.n_tok)
        {
          آزاد (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          برائے (size_t i = 0; i < nfiles; i++)
            {
              اگر (streq (files[i], "-"))
                error (ترتیب_دو_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ورنہ اگر (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  غیرعلامتی طویل صحیح file_نمبر = i + 1;
                  error (ترتیب_دو_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_نمبر);
                }
            }
        }
      ورنہ
        error (ترتیب_دو_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  برائے (کلید = keylist; کلید; کلید = کلید->اگلا)
    {
      اگر (default_کلید_compare (کلید) && !کلید->الٹ)
        {
          کلید->ignore = gkey.ignore;
          کلید->translate = gkey.translate;
          کلید->skipsblanks = gkey.skipsblanks;
          کلید->skipeblanks = gkey.skipeblanks;
          کلید->month = gkey.month;
          کلید->numeric = gkey.numeric;
          کلید->general_numeric = gkey.general_numeric;
          کلید->human_numeric = gkey.human_numeric;
          کلید->نسخہ_3 = gkey.نسخہ_3;
          کلید->random = gkey.random;
          کلید->الٹ = gkey.الٹ;
        }

      need_random |= کلید->random;
    }

  اگر (!keylist && !default_کلید_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  اگر (debug)
    {
      اگر (checkonly || outfile)
        {
          ساکن حرف opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      اگر (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      اگر (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      اگر (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, تہی)));
      ورنہ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      کلید_warnings (&gkey, gkey_only);
    }

  الٹ = gkey.الٹ;

  اگر (need_random)
    random_md5_state_init (random_ماخذ);

  اگر (عارضی_dirs.n_dirs == 0)
    {
      حرف ثابت *tmp_dir = getenv ("TMPDIR");
      جوڑو_عارضی_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  اگر (nfiles == 0)
    {
      nfiles = 1;
      آزاد (files);
      files = xmalloc (حجم *files);
      *files = (حرف *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  اگر (0 < ترتیب_دو_سائز)
    ترتیب_دو_سائز = MAX (ترتیب_دو_سائز, MIN_ترتیب_دو_سائز);

  اگر (checkonly)
    {
      اگر (nfiles > 1)
        error (ترتیب_دو_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      اگر (outfile)
        {
          ساکن حرف opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      باہر_نکلو (check (files[0], checkonly) ? EXIT_SUCCESS : ترتیب_دو_برآمد_کا_ترتیب);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  اگر (mergeonly)
    {
      ساخت sortfile *sortfiles = xcalloc (nfiles, حجم *sortfiles);

      برائے (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      ملاؤ (sortfiles, 0, nfiles, outfile);
    }
  ورنہ
    {
      اگر (!nthreads)
        {
          غیرعلامتی طویل صحیح np = عدد_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_دھاگے);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * حجم (ساخت ملاؤ_گرہ));
      nthreads = MIN (nthreads, nthreads_max);

      ترتیب_دو (files, nfiles, outfile, nthreads);
    }

  اگر (have_read_stdin && fclose (stdin) == EOF)
    ترتیب_دو_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
