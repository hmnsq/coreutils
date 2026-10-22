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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <spawn.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "filevercmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "findprog.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "flexmember.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hard-locale.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hash.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "heap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbswidth.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "nproc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "physmem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixver.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "randread.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readtokens0.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdlib--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "strnumcmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "term-sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unistd--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xmemcoll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xnanosleep.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol-error.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RLIMIT_ਅੰਕੜੇ
ਬਣਤਰ rlimit { size_t rlim_cur; };
# define getrlimit(Resource, Rlp) (-1)
#ਜੇ_ਅੰਤ

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "sort"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Mike Haertel"), \
  proper_name ("Paul Eggert")

#ਜੇ HAVE_LANGINFO_CODESET
# include <langinfo.h>
#ਜੇ_ਅੰਤ

#ਜੇ !defined OPEN_MAX && defined NR_OPEN
# define OPEN_MAX NR_OPEN
#ਜੇ_ਅੰਤ
#ਜੇ !defined OPEN_MAX
# define OPEN_MAX 20
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCHAR_LIM (UCHAR_MAX + 1)

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DEFAULT_TMPDIR
# define DEFAULT_TMPDIR "/tmp"
#ਜੇ_ਅੰਤ

/* Maximum number of lines to merge every time a NODE is taken from
   the merge queue.  Node is at LEVEL in the binary merge tree,
   and is responsible for merging TOTAL lines. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਮਿਲਾਓ(ਕੁੱਲ, level) (((ਕੁੱਲ) >> (2 * ((level) + 1))) + 1)

/* Heuristic value for the number of lines for which it is worth creating
   a subthread, during an internal merge sort.  I.e., it is a small number
   of "average" lines for which sorting via two threads is faster than
   sorting via one on an "average" system.  On a dual-core 2.0 GHz i686
   system with 3GB of RAM and 2MB of L2 cache, a file containing 128K
   lines of gensort -a output is sorted slightly faster with --parallel=2
   than with --parallel=1.  By contrast, using --parallel=1 is about 10%
   faster than using --parallel=2 with a 64K-line input.  */
ਗਿਣਤੀ { SUBTHREAD_ਸਤਰਾਂ_HEURISTIC = 128 * 1024 };
static_assert (4 <= SUBTHREAD_ਸਤਰਾਂ_HEURISTIC);

/* The number of threads after which there are
   diminishing performance gains.  */
ਗਿਣਤੀ { DEFAULT_MAX_ਧਾਗੇ = 8 };

/* Exit statuses.  */
ਗਿਣਤੀ
  {
    /* POSIX says to exit with status 1 if invoked with -c and the
       input is not properly sorted.  */
    ਛਾਂਟੋ_ਬਾਹਰਲਾ_ਦਾ_ਕ੍ਰਮ = 1,

    /* POSIX says any other irregular exit must exit with a status
       code greater than 1.  */
    ਛਾਂਟੋ_FAILURE = 2
  };

ਗਿਣਤੀ
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

ਗਿਣਤੀ
  {
    /* Level of the end-of-merge node, one level above the root. */
    ਮਿਲਾਓ_ਸਿਰਾ = 0,

    /* Level of the root node in merge tree. */
    ਮਿਲਾਓ_ਜੜ੍ਹ = 1
  };

/* The representation of the decimal point in the current locale.  */
ਸਥਿਰ ਅੱਖਰ decimal_ਬਿੰਦੂ;

/* Thousands separator; if outside char range, there is no separator.  */
ਸਥਿਰ ਪੂਰਨ thousands_sep;
/* We currently ignore multi-byte grouping chars.  */
ਸਥਿਰ bool thousands_sep_ignored;

/* Nonzero if the corresponding locales are hard.  */
ਸਥਿਰ bool hard_LC_COLLATE;
#ਜੇ HAVE_NL_LANGINFO
ਸਥਿਰ bool hard_LC_ਸਮਾਂ;
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NONZERO(x) ((x) != 0)

/* The kind of blanks for '-b' to skip in various options. */
ਗਿਣਤੀ blanktype { bl_ਸ਼ੁਰੂ, bl_ਸਿਰਾ, bl_both };

/* The character marking end of line. Default to \n. */
ਸਥਿਰ ਅੱਖਰ eolchar = '\n';

/* Lines are held in memory as counted strings. */
ਬਣਤਰ ਸਤਰ
{
  ਅੱਖਰ *ਪਾਠ;			/* Text of the line. */
  size_t ਲੰਮਾਈ;		/* Length including final newline. */
  ਅੱਖਰ *keybeg;			/* Start of first key. */
  ਅੱਖਰ *keylim;			/* Limit of first key. */
};

/* Input buffers. */
ਬਣਤਰ ਬਫਰ
{
  ਅੱਖਰ *buf;			/* Dynamically allocated buffer,
                                   partitioned into 3 regions:
                                   - input data;
                                   - unused area;
                                   - an array of lines, in reverse order.  */
  size_t ਵਰਤਿਆ;			/* Number of bytes used for input data.  */
  size_t nlines;		/* Number of lines in the line array.  */
  idx_t alloc;			/* Number of bytes allocated. */
  size_t ਖੱਬਾ;			/* Number of bytes left from previous reads. */
  size_t ਸਤਰ_ਬਾਈਟਾਂ;		/* Number of bytes to reserve for each line. */
  bool eof;			/* An EOF has been read.  */
};

/* Policy for sizing the main sort buffer.  INITIAL is the allocation
   to try first.  LIMIT is the largest allocation to try while reading
   normal input; a single input line can still force a larger buffer.  */
ਬਣਤਰ ਛਾਂਟੋ_ਬਫਰ_policy
{
  size_t initial;
  size_t ਹੱਦਬੰਦੀ;
  bool growth_failed;
};

/* Sort key.  */
ਬਣਤਰ keyfield
{
  size_t sword;			/* Zero-origin 'word' to start at. */
  size_t schar;			/* Additional characters to skip. */
  size_t eword;			/* Zero-origin last 'word' of key. */
  size_t echar;			/* Additional characters in field. */
  bool ਅਚਲ *ignore;		/* Boolean array of characters to ignore. */
  ਅੱਖਰ ਅਚਲ *translate;	/* Translation applied to characters. */
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
  bool ਉਲਟ;			/* Reverse the sense of comparison. */
  bool ਸੰਸਕਰਣ_3;			/* sort by version number */
  bool traditional_ਵਰਤਿਆ;	/* Traditional key option format is used. */
  ਬਣਤਰ keyfield *ਅਗਲਾ;	/* Next keyfield to try. */
};

ਬਣਤਰ month
{
  ਅੱਖਰ ਅਚਲ *name;
  ਪੂਰਨ val;
};

/* Binary merge tree node. */
ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ
{
  ਬਣਤਰ ਸਤਰ *lo;              /* Lines to merge from LO child node. */
  ਬਣਤਰ ਸਤਰ *hi;              /* Lines to merge from HI child node. */
  ਬਣਤਰ ਸਤਰ *ਸਿਰਾ_lo;          /* End of available lines from LO. */
  ਬਣਤਰ ਸਤਰ *ਸਿਰਾ_hi;          /* End of available lines from HI. */
  ਬਣਤਰ ਸਤਰ **dest;           /* Pointer to destination of merge. */
  size_t nlo;                   /* Total Lines remaining from LO. */
  size_t nhi;                   /* Total lines remaining from HI. */
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਮਾਪਾ;    /* Parent node. */
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *lo_ਬੱਚਾ;  /* LO child node. */
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *hi_ਬੱਚਾ;  /* HI child node. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level;           /* Level in merge tree. */
  bool queued;                  /* Node is already in heap. */
  pthread_mutex_t ਤਾਲਾ;         /* Lock for node operations. */
};

/* Priority queue of merge nodes. */
ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ
{
  ਬਣਤਰ ਢੇਰ_2 *ਤਰਜੀਹ_ਕਤਾਰ;  /* Priority queue of merge tree nodes. */
  pthread_mutex_t mutex;        /* Lock for queue operations. */
  pthread_cond_t cond;          /* Conditional wait for empty queue to populate
                                   when popping. */
};

/* Used to implement --unique (-u).  */
ਸਥਿਰ ਬਣਤਰ ਸਤਰ saved_ਸਤਰ;

/* FIXME: None of these tables work with multibyte character sets.
   Also, there are many other bugs when handling multibyte characters.
   One way to fix this is to rewrite 'sort' to use wide characters
   internally, but doing this with good performance is a bit
   tricky.  */

/* Table of blanks.  */
ਸਥਿਰ bool blanks[UCHAR_LIM];

/* Table of non-printing characters. */
ਸਥਿਰ bool nonprinting[UCHAR_LIM];

/* Table of non-dictionary characters (not letters, digits, or blanks). */
ਸਥਿਰ bool nondictionary[UCHAR_LIM];

/* Translation table folding lower case to upper.  */
ਸਥਿਰ ਅੱਖਰ fold_toupper[UCHAR_LIM];

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MONTHS_ਪ੍ਰਤੀ_YEAR 12

/* Table mapping month names to integers.
   Alphabetic order allows binary search. */
ਸਥਿਰ ਬਣਤਰ month monthtab[] =
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NMERGE_DEFAULT 16

/* Minimum size for a merge or check buffer.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ (2 + ਆਕਾਰ (ਬਣਤਰ ਸਤਰ))

/* Minimum sort size; the code might not work with smaller sizes.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_ਛਾਂਟੋ_ਮਾਪ (nmerge * MIN_ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ)

/* The number of bytes needed for a merge or check buffer, which can
   function relatively efficiently even if it holds only one line.  If
   a longer line is seen, this value is increased.  */
ਸਥਿਰ size_t ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ = MAX (MIN_ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ, 256 * 1024);

/* The approximate maximum number of bytes of main memory to use, as
   specified by the user.  Zero if the user has not specified a size.  */
ਸਥਿਰ size_t ਛਾਂਟੋ_ਮਾਪ;

/* The initial allocation factor for non-regular files.
   This is used, e.g., when reading from a pipe.
   Don't make it too big, since it is multiplied by ~130 to
   obtain the size of the actual buffer sort will allocate.
   Also, there may be 8 threads all doing this at the same time.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਨਿਵੇਸ਼_FILE_ਮਾਪ_GUESS (128 * 1024)

ਸਥਿਰ ਬਣਤਰ
{
  /* Number of temporary directory names used.  */
  idx_t n_dirs;

  /* Number of allocated slots for temporary directories.  */
  idx_t n_dirs_alloc;

  /* Array of directory names in which any temporary files are
     to be created. */
  ਅੱਖਰ ਅਚਲ **dirs COUNTED_BY (n_dirs);
} ਆਰਜ਼ੀ_dirs;



/* Flag to reverse the order of all comparisons. */
ਸਥਿਰ bool ਉਲਟ;

/* Flag for stable sort.  This turns off the last ditch bytewise
   comparison of lines, and instead leaves lines in the same order
   they were read if all keys compare equal.  */
ਸਥਿਰ bool stable;

/* An int value outside char range.  */
ਗਿਣਤੀ { NON_CHAR = CHAR_MAX + 1 };

/* If TAB has this value, blanks separate fields.  */
ਗਿਣਤੀ { TAB_DEFAULT = CHAR_MAX + 1 };

/* Tab character separating fields.  If TAB_DEFAULT, then fields are
   separated by the empty string between a non-blank character and a blank
   character. */
ਸਥਿਰ ਪੂਰਨ tab = TAB_DEFAULT;

/* Flag to remove consecutive duplicate lines from the output.
   Only the last of a sequence of equal lines will be output. */
ਸਥਿਰ bool unique;

/* Nonzero if any of the input files are the standard input. */
ਸਥਿਰ bool have_read_stdin;

/* List of key field comparisons to be tried.  */
ਸਥਿਰ ਬਣਤਰ keyfield *keylist;

/* Program used to (de)compress temp files.  Must accept -d.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *compress_ਪ੍ਰੋਗਰਾਮ;

/* Annotate the output with extra info to aid the user.  */
ਸਥਿਰ bool debug;

/* Maximum number of files to merge in one go.  If more than this
   number are present, temp files will be used. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nmerge = NMERGE_DEFAULT;

/* Whether SIGPIPE had the default disposition at startup.  */
ਸਥਿਰ bool default_SIGPIPE;

/* The list of temporary files. */
ਬਣਤਰ tempnode
{
  ਬਣਤਰ tempnode *ਅਸਥਿਰ ਅਗਲਾ;
  pid_t pid;     /* The subprocess PID; undefined if state == UNCOMPRESSED.  */
  ਅੱਖਰ state;
  ਅੱਖਰ name[FLEXIBLE_ARRAY_MEMBER];
};
ਸਥਿਰ ਬਣਤਰ tempnode *ਅਸਥਿਰ temphead;
ਸਥਿਰ ਬਣਤਰ tempnode *ਅਸਥਿਰ *temptail = &temphead;

/* Clean up any remaining temporary files.  */

ਸਥਿਰ ਖਾਲੀ
cleanup (ਖਾਲੀ)
{
  ਲਈ (ਬਣਤਰ tempnode ਅਚਲ *ਗੰਢ = temphead; ਗੰਢ; ਗੰਢ = ਗੰਢ->ਅਗਲਾ)
    unlink (ਗੰਢ->name);
  temphead = ਨਿਰਾ;
}

/* Handle interrupts and hangups. */

ਸਥਿਰ ਖਾਲੀ
sighandler (ਪੂਰਨ sig)
{
  cleanup ();

  ਸੰਕੇਤ (sig, SIG_DFL);
  ਉਠਾਓ (sig);
}

/* Report MESSAGE for FILE, then clean up and exit.
   If FILE is null, it represents standard output.  */

ਸਥਿਰ ਖਾਲੀ
ਛਾਂਟੋ_die (ਅੱਖਰ ਅਚਲ *ਸੁਨੇਹਾ_2, ਅੱਖਰ ਅਚਲ *file)
{
  /* If we got EPIPE writing to stdout (from a previous fwrite() or fclose()
     and SIGPIPE was originally SIG_DFL, mimic standard SIGPIPE behavior.  */
  ਜੇ (errno == EPIPE && !file && default_SIGPIPE)
    sighandler (SIGPIPE);

  error (ਛਾਂਟੋ_FAILURE, errno, "%s: %s", ਸੁਨੇਹਾ_2,
         quotef (file ? file : _("standard output")));
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
  or:  %s [OPTION]... --files0-from=F\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Write sorted concatenation ਦਾ all FILE(s) ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fputs (_("\
Ordering options:\n\
\n\
"), stdout);
      oputs (_("\
  -b, --ignore-leading-blanks\n\
         ignore leading blanks when finding ਛਾਂਟੋ ਕੁੰਜੀਆਂ ਵਿੱਚ each ਸਤਰ\n\
"));
      oputs (_("\
  -d, --dictionary-ਕ੍ਰਮ\n\
         consider only blanks and alphanumeric characters\n\
"));
      oputs (_("\
  -f, --ignore-ਹਾਲਤ\n\
         fold ਹੇਠਲਾ ਹਾਲਤ ਮੰਜ਼ਿਲ ਉੱਤਲਾ ਹਾਲਤ characters\n\
"));
      oputs (_("\
  -g, --general-numeric-ਛਾਂਟੋ\n\
         compare according ਮੰਜ਼ਿਲ general numerical ਮੁੱਲ\n\
"));
      oputs (_("\
  -i, --ignore-nonprinting\n\
         consider only printable characters\n\
"));
      oputs (_("\
  -M, --month-ਛਾਂਟੋ\n\
         compare (unknown) < 'JAN' < ... < 'DEC'\n\
"));
      oputs (_("\
  -h, --human-numeric-ਛਾਂਟੋ\n\
         compare human readable numbers (e.g., 2K 1G)\n\
"));
      oputs (_("\
  -n, --numeric-ਛਾਂਟੋ\n\
         compare according ਮੰਜ਼ਿਲ string numerical ਮੁੱਲ;\n\
         see ਭਰਿਆ documentation ਲਈ supported ਲੜੀਆਂ\n\
"));
      oputs (_("\
  -R, --random-ਛਾਂਟੋ\n\
         shuffle, but ਸਮੂਹ identical ਕੁੰਜੀਆਂ.  See also shuf(1)\n\
"));
      oputs (_("\
      --random-ਸਰੋਤ_2=FILE\n\
         get random ਬਾਈਟਾਂ from FILE\n\
"));
      oputs (_("\
  -r, --ਉਲਟ\n\
         ਉਲਟ the ਨਤੀਜਾ ਦਾ comparisons\n\
"));
      oputs (_("\
      --ਛਾਂਟੋ=ਸ਼ਬਦ\n\
         ਛਾਂਟੋ according ਮੰਜ਼ਿਲ ਸ਼ਬਦ:\n\
           general-numeric -g, human-numeric -h, month -M,\n\
           numeric -n, random -R, ਸੰਸਕਰਣ_3 -V\n\
"));
      oputs (_("\
  -V, --ਸੰਸਕਰਣ_3-ਛਾਂਟੋ\n\
         natural ਛਾਂਟੋ ਦਾ (ਸੰਸਕਰਣ_3) numbers within ਪਾਠ\n\
"));
      fputs (_("\
\n\
Other options:\n\
\n\
"), stdout);
      oputs (_("\
      --batch-ਮਾਪ=NMERGE\n\
         ਮਿਲਾਓ at most NMERGE inputs at once; ਲਈ ਹੋਰ use ਆਰਜ਼ੀ files\n\
"));
      oputs (_("\
  -c, --check, --check=diagnose-ਪਹਿਲਾ\n\
         check ਲਈ sorted ਨਿਵੇਸ਼; ਕਰੋ not ਛਾਂਟੋ\n\
"));
      oputs (_("\
  -C, --check=quiet, --check=silent\n\
         like -c, but ਕਰੋ not report ਪਹਿਲਾ bad ਸਤਰ\n\
"));
      oputs (_("\
      --compress-ਪ੍ਰੋਗਰਾਮ=PROG\n\
         compress temporaries with PROG; decompress them with PROG -d\n\
"));
      oputs (_("\
      --debug\n\
         annotate the part ਦਾ the ਸਤਰ ਵਰਤਿਆ ਮੰਜ਼ਿਲ ਛਾਂਟੋ,\n\
         and warn about questionable usage ਮੰਜ਼ਿਲ standard error\n\
"));
      oputs (_("\
      --files0-from=F\n\
         ਪਠ ਨਿਵੇਸ਼ from the files specified by NUL-terminated ਨਾਂ ਵਿੱਚ file F;\
\n\
         If F is -, ਪਠ ਨਾਂ from standard ਨਿਵੇਸ਼\n\
"));
      oputs (_("\
  -k, --ਕੁੰਜੀ=KEYDEF\n\
         ਛਾਂਟੋ ਰਾਹੀਂ a ਕੁੰਜੀ; KEYDEF gives location and type\n\
"));
      oputs (_("\
  -m, --ਮਿਲਾਓ\n\
         ਮਿਲਾਓ already sorted files; ਕਰੋ not ਛਾਂਟੋ\n\
"));
      oputs (_("\
  -o, --output=FILE\n\
         ਲਿਖ ਨਤੀਜਾ ਮੰਜ਼ਿਲ FILE instead ਦਾ standard output\n\
"));
      oputs (_("\
  -s, --stable\n\
         stabilize ਛਾਂਟੋ by disabling ਆਖਰੀ-resort comparison\n\
"));
      oputs (_("\
  -S, --ਬਫਰ-ਮਾਪ=ਮਾਪ_2\n\
         use ਮਾਪ_2 ਲਈ ਮੁੱਖ ਸਮ੍ਰਿਤੀ ਬਫਰ\n\
"));
      oputs (_("\
  -t, --ਖੇਤਰ-separator=SEP\n\
         use SEP instead ਦਾ non-blank ਮੰਜ਼ਿਲ blank transition\n\
"));
      oprintf (_("\
  -T, --temporary-directory=DIR\n\
         use DIR ਲਈ temporaries, not $TMPDIR or %s;\n\
         ਗੁਣਜ options specify ਗੁਣਜ directories\n\
"), DEFAULT_TMPDIR);
      oputs (_("\
      --parallel=N\n\
         change the ਸੰਖਿਆ ਦਾ sorts ਲੜੀਵਾਰ_2 concurrently ਮੰਜ਼ਿਲ N\n\
"));
      oputs (_("\
  -u, --unique\n\
         output only the ਪਹਿਲਾ ਦਾ ਸਤਰਾਂ with equal ਕੁੰਜੀਆਂ;\n\
         with -c, check ਲਈ strict ordering\n\
"));
      oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
KEYDEF is F[.C][OPTS][,F[.C][OPTS]] ਲਈ ਸ਼ੁਰੂ and stop position, where F is a\n\
ਖੇਤਰ ਸੰਖਿਆ and C a character position ਵਿੱਚ the ਖੇਤਰ; both are origin 1, and\n\
the stop position defaults ਮੰਜ਼ਿਲ the ਸਤਰ's ਸਿਰਾ.  If neither -t nor -b is ਵਿੱਚ\n\
effect, characters ਵਿੱਚ a ਖੇਤਰ are counted from the beginning ਦਾ the preceding\n\
whitespace.  OPTS is one or ਹੋਰ single-letter ordering options [bdfgiMhnRrV],\
\n\
which override global ordering options ਲਈ that ਕੁੰਜੀ.  If no ਕੁੰਜੀ is given, use\n\
the entire ਸਤਰ as the ਕੁੰਜੀ.  Use --debug ਮੰਜ਼ਿਲ diagnose incorrect ਕੁੰਜੀ usage.\n\
\n\
ਮਾਪ_2 may be followed by the following multiplicative suffixes:\n\
"), stdout);
      fputs (_("\
% 1% ਦਾ ਸਮ੍ਰਿਤੀ, b 1, K 1024 (ਮੂਲ), and so on ਲਈ M, G, T, P, E, Z, Y, R, Q.\
\n\n\
*** WARNING ***\n\
The locale specified by the environment affects ਛਾਂਟੋ ਕ੍ਰਮ.\n\
ਸੈੱਟ_3 LC_ALL=C ਮੰਜ਼ਿਲ get the traditional ਛਾਂਟੋ ਕ੍ਰਮ that ਵਰਤਦਾ\n\
native byte ਮੁੱਲਾਂ.\n\
"), stdout );
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  CHECK_OPTION = CHAR_MAX + 1,
  COMPRESS_ਪ੍ਰੋਗਰਾਮ_OPTION,
  DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION,
  FILES0_FROM_OPTION,
  NMERGE_OPTION,
  RANDOM_ਸਰੋਤ_OPTION,
  ਛਾਂਟੋ_OPTION,
  PARALLEL_OPTION
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ short_options[] = "-bcCdfghik:mMno:rRsS:t:T:uVy:z";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"ignore-leading-blanks", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"check", optional_ਦਲੀਲ, ਨਿਰਾ, CHECK_OPTION},
  {"compress-program", required_ਦਲੀਲ, ਨਿਰਾ, COMPRESS_ਪ੍ਰੋਗਰਾਮ_OPTION},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION},
  {"dictionary-order", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"ignore-case", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"files0-from", required_ਦਲੀਲ, ਨਿਰਾ, FILES0_FROM_OPTION},
  {"general-numeric-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {"ignore-nonprinting", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"key", required_ਦਲੀਲ, ਨਿਰਾ, 'k'},
  {"merge", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"month-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'M'},
  {"numeric-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"human-numeric-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {"version-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'V'},
  {"random-sort", no_ਦਲੀਲ, ਨਿਰਾ, 'R'},
  {"random-source", required_ਦਲੀਲ, ਨਿਰਾ, RANDOM_ਸਰੋਤ_OPTION},
  {"sort", required_ਦਲੀਲ, ਨਿਰਾ, ਛਾਂਟੋ_OPTION},
  {"output", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"reverse", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"stable", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"batch-size", required_ਦਲੀਲ, ਨਿਰਾ, NMERGE_OPTION},
  {"buffer-size", required_ਦਲੀਲ, ਨਿਰਾ, 'S'},
  {"field-separator", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"temporary-directory", required_ਦਲੀਲ, ਨਿਰਾ, 'T'},
  {"unique", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {"parallel", required_ਦਲੀਲ, ਨਿਰਾ, PARALLEL_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0},
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CHECK_ਸਾਰਣੀ \
  _ct_("quiet",          'C') \
  _ct_("silent",         'C') \
  _ct_("diagnose-first", 'c')

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ check_ਦਲੀਲਾਂ[] =
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _ct_(_s, _c) _s,
  CHECK_ਸਾਰਣੀ ਨਿਰਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  _ct_
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ check_types[] =
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _ct_(_s, _c) _c,
  CHECK_ਸਾਰਣੀ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  _ct_
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਛਾਂਟੋ_ਸਾਰਣੀ \
  _st_("general-numeric", 'g') \
  _st_("human-numeric",   'h') \
  _st_("month",           'M') \
  _st_("numeric",         'n') \
  _st_("random",          'R') \
  _st_("version",         'V')

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਛਾਂਟੋ_ਦਲੀਲਾਂ[] =
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _st_(_s, _c) _s,
  ਛਾਂਟੋ_ਸਾਰਣੀ ਨਿਰਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  _st_
};
ਸਥਿਰ ਅੱਖਰ ਅਚਲ ਛਾਂਟੋ_types[] =
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _st_(_s, _c) _c,
  ਛਾਂਟੋ_ਸਾਰਣੀ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ  _st_
};

/* The set of signals that are caught.  */
ਸਥਿਰ sigset_t caught_ਸੰਕੇਤ;

/* Critical section status.  */
ਬਣਤਰ cs_ਸਥਿਤੀ
{
  bool valid;
  sigset_t sigs;
};

/* Enter a critical section.  */
ਸਥਿਰ ਖਾਲੀ
cs_enter (ਬਣਤਰ cs_ਸਥਿਤੀ *ਸਥਿਤੀ)
{
  ਪੂਰਨ ret = pthread_sigmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &ਸਥਿਤੀ->sigs);
  ਸਥਿਤੀ->valid = ret == 0;
}

/* Leave a critical section.  */
ਸਥਿਰ ਖਾਲੀ
cs_leave (ਬਣਤਰ cs_ਸਥਿਤੀ ਅਚਲ *ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ->valid)
    {
      /* Ignore failure when restoring the signal mask. */
      pthread_sigmask (SIG_SETMASK, &ਸਥਿਤੀ->sigs, ਨਿਰਾ);
    }
}

/* Possible states for a temp file.  If compressed, the file's status
   is unreaped or reaped, depending on whether 'sort' has waited for
   the subprocess to finish.  */
ਗਿਣਤੀ { UNCOMPRESSED, UNREAPED, REAPED };

/* A file to be sorted.  */
ਬਣਤਰ sortfile
{
  /* The file's name.  */
  ਅੱਖਰ ਅਚਲ *name;

  /* Non-null if this is a temporary file, in which case NAME == TEMP->name.  */
  ਬਣਤਰ tempnode *ਆਰਜ਼ੀ;
};

/* Map PIDs of unreaped subprocesses to their struct tempnode objects.  */
ਸਥਿਰ Hash_ਸਾਰਣੀ *proctab;

ਗਿਣਤੀ { INIT_PROCTAB_ਮਾਪ = 47 };

ਸਥਿਰ size_t
proctab_hasher (ਖਾਲੀ ਅਚਲ *ਇੰਦਰਾਜ, size_t tabsize)
{
  ਬਣਤਰ tempnode ਅਚਲ *ਗੰਢ = ਇੰਦਰਾਜ;
  ਵਾਪਸ ਗੰਢ->pid % tabsize;
}

ਸਥਿਰ bool
proctab_comparator (ਖਾਲੀ ਅਚਲ *e1, ਖਾਲੀ ਅਚਲ *e2)
{
  ਬਣਤਰ tempnode ਅਚਲ *n1 = e1;
  ਬਣਤਰ tempnode ਅਚਲ *n2 = e2;
  ਵਾਪਸ n1->pid == n2->pid;
}

/* The number of unreaped child processes.  */
ਸਥਿਰ pid_t nprocs;

ਸਥਿਰ bool delete_proc (pid_t);

/* If PID is positive, wait for the child process with that PID to
   exit, and assume that PID has already been removed from the process
   table.  If PID is 0 or -1, clean up some child that has exited (by
   waiting for it, and removing it from the proc table) and return the
   child's process ID.  However, if PID is 0 and no children have
   exited, return 0 without waiting.  */

ਸਥਿਰ pid_t
reap (pid_t pid)
{
  ਪੂਰਨ ਸਥਿਤੀ;
  pid_t cpid = waitpid ((pid ? pid : -1), &ਸਥਿਤੀ, (pid ? 0 : WNOHANG));

  ਜੇ (cpid < 0)
    error (ਛਾਂਟੋ_FAILURE, errno, _("waiting for %s [-d]"),
           quoteaf (compress_ਪ੍ਰੋਗਰਾਮ));
  ਨਹੀਂਤਾਂ ਜੇ (0 < cpid && (0 < pid || delete_proc (cpid)))
    {
      ਜੇ (! WIFEXITED (ਸਥਿਤੀ) || WEXITSTATUS (ਸਥਿਤੀ))
        error (ਛਾਂਟੋ_FAILURE, 0, _("%s [-d] terminated abnormally"),
               quoteaf (compress_ਪ੍ਰੋਗਰਾਮ));
      --nprocs;
    }

  ਵਾਪਸ cpid;
}

/* TEMP represents a new process; add it to the process table.  Create
   the process table the first time it's called.  */

ਸਥਿਰ ਖਾਲੀ
ਰਜਿਸਟਰ_proc (ਬਣਤਰ tempnode *ਆਰਜ਼ੀ)
{
  ਜੇ (! proctab)
    {
      proctab = hash_initialize (INIT_PROCTAB_ਮਾਪ, ਨਿਰਾ,
                                 proctab_hasher,
                                 proctab_comparator,
                                 ਨਿਰਾ);
      ਜੇ (! proctab)
        xalloc_die ();
    }

  ਆਰਜ਼ੀ->state = UNREAPED;

  ਜੇ (! hash_ਪਾਓ (proctab, ਆਰਜ਼ੀ))
    xalloc_die ();
}

/* If PID is in the process table, remove it and return true.
   Otherwise, return false.  */

ਸਥਿਰ bool
delete_proc (pid_t pid)
{
  ਬਣਤਰ tempnode test;

  test.pid = pid;
  ਬਣਤਰ tempnode *ਗੰਢ = hash_remove (proctab, &test);
  ਜੇ (! ਗੰਢ)
    ਵਾਪਸ false;
  ਗੰਢ->state = REAPED;
  ਵਾਪਸ true;
}

/* Remove PID from the process table, and wait for it to exit if it
   hasn't already.  */

ਸਥਿਰ ਖਾਲੀ
wait_proc (pid_t pid)
{
  ਜੇ (delete_proc (pid))
    reap (pid);
}

/* Reap any exited children.  Do not block; reap only those that have
   already exited.  */

ਸਥਿਰ ਖਾਲੀ
reap_exited (ਖਾਲੀ)
{
  ਜਦੋਂ (0 < nprocs && reap (0))
    ਜਾਰੀ;
}

/* Reap at least one exited child, waiting if necessary.  */

ਸਥਿਰ ਖਾਲੀ
reap_some (ਖਾਲੀ)
{
  reap (-1);
  reap_exited ();
}

/* Reap all children, waiting if necessary.  */

ਸਥਿਰ ਖਾਲੀ
reap_all (ਖਾਲੀ)
{
  ਜਦੋਂ (0 < nprocs)
    reap (-1);
}

/* Cleanup actions to take when exiting.  */

ਸਥਿਰ ਖਾਲੀ
exit_cleanup (ਖਾਲੀ)
{
  ਜੇ (temphead)
    {
      /* Clean up any remaining temporary files in a critical section so
         that a signal handler does not try to clean them too.  */
      ਬਣਤਰ cs_ਸਥਿਤੀ cs;
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

ਸਥਿਰ ਬਣਤਰ tempnode *
create_ਆਰਜ਼ੀ_file (ਪੂਰਨ *pfd, bool survive_fd_exhaustion)
{
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ slashbase[] = "/sortXXXXXX";
  ਸਥਿਰ idx_t ਆਰਜ਼ੀ_dir_ਸੂਚਕਾਂਕ;
  ਪੂਰਨ fd;
  ਪੂਰਨ saved_errno;
  ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_dir = ਆਰਜ਼ੀ_dirs.dirs[ਆਰਜ਼ੀ_dir_ਸੂਚਕਾਂਕ];
  size_t len = ਲੜੀ_ਲੰਬਾਈ (ਆਰਜ਼ੀ_dir);
  ਬਣਤਰ tempnode *ਗੰਢ =
    xmalloc (FLEXSIZEOF (ਬਣਤਰ tempnode, name, len + ਆਕਾਰ slashbase));
  ਅੱਖਰ *file = ਗੰਢ->name;
  ਬਣਤਰ cs_ਸਥਿਤੀ cs;

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (file, ਆਰਜ਼ੀ_dir, len);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (file + len, slashbase, ਆਕਾਰ slashbase);
  ਗੰਢ->ਅਗਲਾ = ਨਿਰਾ;
  ਜੇ (++ਆਰਜ਼ੀ_dir_ਸੂਚਕਾਂਕ == ਆਰਜ਼ੀ_dirs.n_dirs)
    ਆਰਜ਼ੀ_dir_ਸੂਚਕਾਂਕ = 0;

  /* Create the temporary file in a critical section, to avoid races.  */
  cs_enter (&cs);
  fd = mkostemp (file, O_CLOEXEC);
  ਜੇ (0 <= fd)
    {
      *temptail = ਗੰਢ;
      temptail = &ਗੰਢ->ਅਗਲਾ;
    }
  saved_errno = errno;
  cs_leave (&cs);
  errno = saved_errno;

  ਜੇ (fd < 0)
    {
      ਜੇ (! (survive_fd_exhaustion && errno == EMFILE))
        error (ਛਾਂਟੋ_FAILURE, errno, _("cannot create temporary file in %s"),
               quoteaf (ਆਰਜ਼ੀ_dir));
      ਮੁਕਤ (ਗੰਢ);
      ਗੰਢ = ਨਿਰਾ;
    }

  *pfd = fd;
  ਵਾਪਸ ਗੰਢ;
}

/* Return a pointer to stdout status, or NULL on failure.  */

ਸਥਿਰ ਬਣਤਰ stat *
get_outstatus (ਖਾਲੀ)
{
  ਸਥਿਰ ਪੂਰਨ outstat_errno;
  ਸਥਿਰ ਬਣਤਰ stat outstat;
  ਜੇ (outstat_errno == 0)
    outstat_errno = fstat (STDOUT_FILENO, &outstat) == 0 ? -1 : errno;
  ਵਾਪਸ outstat_errno < 0 ? &outstat : ਨਿਰਾ;
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

ਸਥਿਰ FILE *
ਧਾਰਾ_open (ਅੱਖਰ ਅਚਲ *file, ਅੱਖਰ ਅਚਲ *how)
{
  FILE *fp;

  ਜੇ (*how == 'r')
    {
      ਜੇ (streq (file, "-"))
        {
          have_read_stdin = true;
          fp = stdin;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ fd = open (file, O_RDONLY | O_CLOEXEC);
          fp = fd < 0 ? ਨਿਰਾ : fdopen (fd, how);
        }
      fadvise (fp, FADVISE_SEQUENTIAL);
    }
  ਨਹੀਂਤਾਂ ਜੇ (*how == 'w')
    {
      ਜੇ (file && ftruncate (STDOUT_FILENO, 0) != 0)
        {
          ਪੂਰਨ ftruncate_errno = errno;
          ਬਣਤਰ stat *outst = get_outstatus ();
          ਜੇ (!outst || S_ISREG (outst->st_ਬਹੁਲਕ) || S_TYPEISSHM (outst))
            error (ਛਾਂਟੋ_FAILURE, ftruncate_errno, _("%s: error truncating"),
                   quotef (file));
        }
      fp = stdout;
    }
  ਨਹੀਂਤਾਂ
    affirm (!"unexpected mode passed to stream_open");

  ਵਾਪਸ fp;
}

/* Same as stream_open, except always return a non-null value; die on
   failure.  */

ਸਥਿਰ FILE *
xfopen (ਅੱਖਰ ਅਚਲ *file, ਅੱਖਰ ਅਚਲ *how)
{
  FILE *fp = ਧਾਰਾ_open (file, how);
  ਜੇ (!fp)
    ਛਾਂਟੋ_die (_("open failed"), file);
  ਵਾਪਸ fp;
}

/* Close FP, whose name is FILE, and report any errors.  */

ਸਥਿਰ ਖਾਲੀ
xfclose (FILE *fp, ਅੱਖਰ ਅਚਲ *file)
{
  ਚੋਣ (fileno (fp))
    {
    ਹਾਲਤ STDIN_FILENO:
      /* Allow reading stdin from tty more than once.  */
      clearerr (fp);
      ਰੋਕੋ;

    ਹਾਲਤ STDOUT_FILENO:
      /* Don't close stdout just yet.  close_stdout does that.  */
      ਜੇ (fflush (fp) != 0)
        ਛਾਂਟੋ_die (_("fflush failed"), file);
      ਰੋਕੋ;

    ਮੂਲ:
      ਜੇ (fclose (fp) != 0)
        ਛਾਂਟੋ_die (_("close failed"), file);
      ਰੋਕੋ;
    }
}

/* Move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not close-on-exec.  */

ਸਥਿਰ ਖਾਲੀ
move_fd (ਪੂਰਨ oldfd, ਪੂਰਨ newfd)
{
  ਜੇ (oldfd != newfd)
    {
      /* These should never fail for our usage.  */
      ignore_ਮੁੱਲ (dup2 (oldfd, newfd));
      ignore_ਮੁੱਲ (close (oldfd));
    }
}

/* Setup ACTION to move OLDFD to NEWFD.  If OLDFD != NEWFD, NEWFD is not
   close-on-exec.  Returns 0 if successful, or an error number otherwise.  */

ਸਥਿਰ ਪੂਰਨ
posix_spawn_file_actions_move_fd (posix_spawn_file_actions_t *actions,
                                  ਪੂਰਨ oldfd, ਪੂਰਨ newfd)
{
  ਪੂਰਨ ਨਤੀਜਾ = 0;
  ਜੇ (oldfd != newfd)
    {
      ਨਤੀਜਾ = posix_spawn_file_actions_adddup2 (actions, oldfd, newfd);
      ਜੇ (ਨਤੀਜਾ == 0)
        ਨਤੀਜਾ = posix_spawn_file_actions_addclose (actions, oldfd);
    }
  ਵਾਪਸ ਨਤੀਜਾ;
}

/* Look up COMPRESS_PROGRAM in $PATH, and return the resolved program name.
   Upon error, return NULL with errno set.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
get_resolved_compress_ਪ੍ਰੋਗਰਾਮ (ਖਾਲੀ)
{
  /* Use a cache, to perform the search only once.  */
  ਸਥਿਰ ਅੱਖਰ ਅਚਲ *resolved_compress_ਪ੍ਰੋਗਰਾਮ_cache /* = NULL */;

  ਜੇ (resolved_compress_ਪ੍ਰੋਗਰਾਮ_cache == ਨਿਰਾ)
    {
      resolved_compress_ਪ੍ਰੋਗਰਾਮ_cache =
        ਲੱਭੋ_ਵਿੱਚ_given_ਰਸਤਾ (compress_ਪ੍ਰੋਗਰਾਮ, getenv ("PATH"), ਨਿਰਾ, false);
      /* If resolved_compress_program_cache == NULL, errno is set here.  */
    }

  ਵਾਪਸ resolved_compress_ਪ੍ਰੋਗਰਾਮ_cache;
}

/* Execute COMPRESS_PROGRAM in a child process.  The child processes pid is
   stored in PD.  The TRIES parameter specifies how many times to try to create
   a child process before giving up.  Return 0 on success, or an error number
   otherwise.  */

ਸਥਿਰ ਪੂਰਨ
pipe_ਬੱਚਾ (pid_t *pid, ਪੂਰਨ pipefds[2], ਪੂਰਨ tempfd, bool decompress,
            size_t tries)
{
  ਅੱਖਰ ਅਚਲ *resolved_compress_ਪ੍ਰੋਗਰਾਮ;
  ਬਣਤਰ tempnode *saved_temphead;
  ਦੁੱਗਣਾ wait_retry = 0.25;
  ਬਣਤਰ cs_ਸਥਿਤੀ cs;
  ਪੂਰਨ ਨਤੀਜਾ;
  posix_spawnattr_t attr;
  posix_spawn_file_actions_t actions;

  /* Lookup the program before we spawn, so that we consistently
     handle access issues to COMPRESS_PROGRAM, because on some
     implementations/emulations of posix_spawn we get only a
     generic (fatal) error from the child in that case.  */
  resolved_compress_ਪ੍ਰੋਗਰਾਮ = get_resolved_compress_ਪ੍ਰੋਗਰਾਮ ();
  ਜੇ (resolved_compress_ਪ੍ਰੋਗਰਾਮ == ਨਿਰਾ)
    ਵਾਪਸ errno;

  ਜੇ ((ਨਤੀਜਾ = posix_spawnattr_init (&attr)))
    ਵਾਪਸ ਨਤੀਜਾ;
  ਜੇ ((ਨਤੀਜਾ = posix_spawnattr_setflags (&attr, POSIX_SPAWN_USEVFORK))
      || (ਨਤੀਜਾ = posix_spawn_file_actions_init (&actions)))
    {
      posix_spawnattr_destroy (&attr);
      ਵਾਪਸ ਨਤੀਜਾ;
    }

  ਜੇ (pipe2 (pipefds, O_CLOEXEC) < 0)
    {
      ਪੂਰਨ saved_errno = errno;
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ਵਾਪਸ saved_errno;
    }

  ਜੇ ((ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions, STDIN_FILENO))
      || (ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions, STDOUT_FILENO))
      || (decompress
          ? ((ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[0]))
             || (ਨਤੀਜਾ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDIN_FILENO))
             || (ਨਤੀਜਾ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[1],
                                                            STDOUT_FILENO)))
          : ((ਨਤੀਜਾ = posix_spawn_file_actions_addclose (&actions,
                                                          pipefds[1]))
             || (ਨਤੀਜਾ = posix_spawn_file_actions_move_fd (&actions, tempfd,
                                                            STDOUT_FILENO))
             || (ਨਤੀਜਾ = posix_spawn_file_actions_move_fd (&actions,
                                                            pipefds[0],
                                                            STDIN_FILENO)))))
    {
      close (pipefds[0]);
      close (pipefds[1]);
      posix_spawnattr_destroy (&attr);
      posix_spawn_file_actions_destroy (&actions);
      ਵਾਪਸ ਨਤੀਜਾ;
    }

  ਅੱਖਰ ਅਚਲ *ਅਚਲ argv[] =
    {
      resolved_compress_ਪ੍ਰੋਗਰਾਮ,
      decompress ? "-d" : ਨਿਰਾ,
      ਨਿਰਾ
    };

  /* At least NMERGE + 1 subprocesses are needed.  More could be created, but
     uncontrolled subprocess generation can hurt performance significantly.
     Allow at most NMERGE + 2 subprocesses, on the theory that there
     may be some useful parallelism by letting compression for the
     previous merge finish (1 subprocess) in parallel with the current
     merge (NMERGE + 1 subprocesses).  */

  ਜੇ (nmerge + 1 < nprocs)
    reap_some ();

  ਜਦੋਂ (tries--)
    {
      /* This is so the child process won't delete our temp files
         if it receives a signal before exec-ing.  */
      cs_enter (&cs);
      saved_temphead = temphead;
      temphead = ਨਿਰਾ;

      ਨਤੀਜਾ = posix_spawnp (pid, resolved_compress_ਪ੍ਰੋਗਰਾਮ, &actions, &attr,
                             (ਅੱਖਰ * ਅਚਲ *) argv, environ);

      temphead = saved_temphead;

      cs_leave (&cs);

      ਜੇ (ਨਤੀਜਾ != EAGAIN)
        ਰੋਕੋ;
      ਨਹੀਂਤਾਂ
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

  ਜੇ (ਨਤੀਜਾ)
    {
      close (pipefds[0]);
      close (pipefds[1]);
    }
  ਨਹੀਂਤਾਂ
    ++nprocs;

  ਵਾਪਸ ਨਤੀਜਾ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return
   the address of the new temp node.  If the creation
   fails, return NULL if the failure is due to file descriptor
   exhaustion and SURVIVE_FD_EXHAUSTION; otherwise, die.  */

ਸਥਿਰ ਬਣਤਰ tempnode *
maybe_create_ਆਰਜ਼ੀ (FILE **pfp, bool survive_fd_exhaustion)
{
  ਪੂਰਨ tempfd;
  ਬਣਤਰ tempnode *ਗੰਢ = create_ਆਰਜ਼ੀ_file (&tempfd, survive_fd_exhaustion);
  ਜੇ (! ਗੰਢ)
    ਵਾਪਸ ਨਿਰਾ;

  ਗੰਢ->state = UNCOMPRESSED;

  ਜੇ (compress_ਪ੍ਰੋਗਰਾਮ)
    {
      ਪੂਰਨ pipefds[2];
      ਸਥਿਰ ਪੂਰਨ ਆਖਰੀ_ਨਤੀਜਾ = 0;

      ਪੂਰਨ ਨਤੀਜਾ = pipe_ਬੱਚਾ (&ਗੰਢ->pid, pipefds, tempfd, false,
                               MAX_TRIES_COMPRESS);

      ਜੇ (ਨਤੀਜਾ)
        {
          ਜੇ (ਨਤੀਜਾ != ਆਖਰੀ_ਨਤੀਜਾ)
            error (0, ਨਤੀਜਾ, _("could not run compress program %s"),
                   quoteaf (compress_ਪ੍ਰੋਗਰਾਮ));
        }
      ਨਹੀਂਤਾਂ
        {
          close (tempfd);
          close (pipefds[0]);
          tempfd = pipefds[1];

          ਰਜਿਸਟਰ_proc (ਗੰਢ);
        }

      ਆਖਰੀ_ਨਤੀਜਾ = ਨਤੀਜਾ;
    }

  *pfp = fdopen (tempfd, "w");
  ਜੇ (! *pfp)
    ਛਾਂਟੋ_die (_("couldn't create temporary file"), ਗੰਢ->name);

  ਵਾਪਸ ਗੰਢ;
}

/* Create a temporary file and, if asked for, start a compressor
   to that file.  Set *PFP to the file handle and return the address
   of the new temp node.  Die on failure.  */

ਸਥਿਰ ਬਣਤਰ tempnode *
create_ਆਰਜ਼ੀ (FILE **pfp)
{
  ਵਾਪਸ maybe_create_ਆਰਜ਼ੀ (pfp, false);
}

/* Open a compressed temp file and start a decompression process through
   which to filter the input.  Return NULL (setting errno to
   EMFILE) if we ran out of file descriptors, and die on any other
   kind of failure.  */

ਸਥਿਰ FILE *
open_ਆਰਜ਼ੀ (ਬਣਤਰ tempnode *ਆਰਜ਼ੀ)
{
  ਪੂਰਨ tempfd, pipefds[2];
  FILE *fp = ਨਿਰਾ;

  ਜੇ (ਆਰਜ਼ੀ->state == UNREAPED)
    wait_proc (ਆਰਜ਼ੀ->pid);

  tempfd = open (ਆਰਜ਼ੀ->name, O_RDONLY);
  ਜੇ (tempfd < 0)
    ਵਾਪਸ ਨਿਰਾ;

  pid_t ਬੱਚਾ;
  ਪੂਰਨ ਨਤੀਜਾ = pipe_ਬੱਚਾ (&ਬੱਚਾ, pipefds, tempfd, true,
                           MAX_TRIES_DECOMPRESS);

  ਜੇ (ਨਤੀਜਾ)
    {
      ਜੇ (ਨਤੀਜਾ != EMFILE)
        error (ਛਾਂਟੋ_FAILURE, ਨਤੀਜਾ, _("could not run compress program %s -d"),
               quoteaf (compress_ਪ੍ਰੋਗਰਾਮ));
      close (tempfd);
      errno = EMFILE;
    }
  ਨਹੀਂਤਾਂ
    {
      ਆਰਜ਼ੀ->pid = ਬੱਚਾ;
      ਰਜਿਸਟਰ_proc (ਆਰਜ਼ੀ);
      close (tempfd);
      close (pipefds[1]);

      fp = fdopen (pipefds[0], "r");
      ਜੇ (! fp)
        {
          ਪੂਰਨ saved_errno = errno;
          close (pipefds[0]);
          errno = saved_errno;
        }
    }

  ਵਾਪਸ fp;
}

/* Append DIR to the array of temporary directory names.  */
ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_ਆਰਜ਼ੀ_dir (ਅੱਖਰ ਅਚਲ *dir)
{
  ਜੇ (ਆਰਜ਼ੀ_dirs.n_dirs == ਆਰਜ਼ੀ_dirs.n_dirs_alloc)
    ਆਰਜ਼ੀ_dirs.dirs = xpalloc (ਆਰਜ਼ੀ_dirs.dirs, &ਆਰਜ਼ੀ_dirs.n_dirs_alloc, 1, -1,
                              ਆਕਾਰ *ਆਰਜ਼ੀ_dirs.dirs);

  idx_t ਅਚਲ dir_idx = ਆਰਜ਼ੀ_dirs.n_dirs++;
  ਆਰਜ਼ੀ_dirs.dirs[dir_idx] = dir;
}

/* Remove NAME from the list of temporary files.  */

ਸਥਿਰ ਖਾਲੀ
zaptemp (ਅੱਖਰ ਅਚਲ *name)
{
  ਬਣਤਰ tempnode *ਅਸਥਿਰ *pnode;
  ਬਣਤਰ tempnode *ਗੰਢ;
  ਬਣਤਰ tempnode *ਅਗਲਾ;
  ਪੂਰਨ unlink_ਸਥਿਤੀ;
  ਪੂਰਨ unlink_errno = 0;
  ਬਣਤਰ cs_ਸਥਿਤੀ cs;

  ਲਈ (pnode = &temphead; (ਗੰਢ = *pnode)->name != name; pnode = &ਗੰਢ->ਅਗਲਾ)
    ਜਾਰੀ;

  ਜੇ (ਗੰਢ->state == UNREAPED)
    wait_proc (ਗੰਢ->pid);

  /* Unlink the temporary file in a critical section to avoid races.  */
  ਅਗਲਾ = ਗੰਢ->ਅਗਲਾ;
  cs_enter (&cs);
  unlink_ਸਥਿਤੀ = unlink (name);
  unlink_errno = errno;
  *pnode = ਅਗਲਾ;
  cs_leave (&cs);

  ਜੇ (unlink_ਸਥਿਤੀ != 0)
    error (0, unlink_errno, _("warning: cannot remove: %s"), quotef (name));
  ਜੇ (! ਅਗਲਾ)
    temptail = pnode;
  ਮੁਕਤ (ਗੰਢ);
}

#ਜੇ HAVE_NL_LANGINFO

ਸਥਿਰ ਪੂਰਨ
struct_month_cmp (ਖਾਲੀ ਅਚਲ *m1, ਖਾਲੀ ਅਚਲ *m2)
{
  ਬਣਤਰ month ਅਚਲ *month1 = m1;
  ਬਣਤਰ month ਅਚਲ *month2 = m2;
  ਵਾਪਸ strcmp (month1->name, month2->name);
}

#ਜੇ_ਅੰਤ

/* Initialize the character class tables. */

ਸਥਿਰ ਖਾਲੀ
inittables (ਖਾਲੀ)
{
  ਲਈ (size_t i = 0; i < UCHAR_LIM; ++i)
    {
      blanks[i] = i == '\n' || isblank (i);
      nondictionary[i] = ! blanks[i] && ! ਅੱਖਰ_ਜਾਂ_ਅੰਕ (i);
      nonprinting[i] = ! isprint (i);
      fold_toupper[i] = ਵੱਡਾ_ਅੱਖਰ (i);
    }

#ਜੇ HAVE_NL_LANGINFO
  /* If we're not in the "C" locale, read different names for months.  */
  ਜੇ (hard_LC_ਸਮਾਂ)
    {
      ਲਈ (size_t i = 0; i < MONTHS_ਪ੍ਰਤੀ_YEAR; i++)
        {
          ਅੱਖਰ ਅਚਲ *s;
          size_t s_len;
          size_t j, k;
          ਅੱਖਰ *name;

          s = nl_langinfo (ABMON_1 + i);
          s_len = ਲੜੀ_ਲੰਬਾਈ (s);
          monthtab[i].name = name = xmalloc (s_len + 1);
          monthtab[i].val = i + 1;

          ਲਈ (j = k = 0; j < s_len; j++)
            ਜੇ (! isblank (ਮੰਜ਼ਿਲ_uchar (s[j])))
              name[k++] = fold_toupper[ਮੰਜ਼ਿਲ_uchar (s[j])];
          name[k] = '\0';
        }
      qsort (monthtab, MONTHS_ਪ੍ਰਤੀ_YEAR, ਆਕਾਰ *monthtab, struct_month_cmp);
    }
#ਜੇ_ਅੰਤ
}

/* Specify how many inputs may be merged at once.
   This may be set on the command-line with the
   --batch-size option. */
ਸਥਿਰ ਖਾਲੀ
specify_nmerge (ਪੂਰਨ oi, ਅੱਖਰ c, ਅੱਖਰ ਅਚਲ *s)
{
  uintmax_t n;
  ਬਣਤਰ rlimit rlimit;
  ਗਿਣਤੀ strtol_error e = xstrtoumax (s, ਨਿਰਾ, 10, &n, "");

  /* Try to find out how many file descriptors we'll be able
     to open.  We need at least nmerge + 3 (STDIN_FILENO,
     STDOUT_FILENO and STDERR_FILENO). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_nmerge = ((getrlimit (RLIMIT_NOFILE, &rlimit) == 0
                              ? rlimit.rlim_cur
                              : OPEN_MAX)
                             - 3);

  ਜੇ (e == LONGINT_OK)
    {
      nmerge = n;
      ਜੇ (nmerge != n)
        e = LONGINT_ਉਛਾਲ;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (nmerge < 2)
            {
              error (0, 0, _("invalid --%s argument %s"),
                     long_options[oi].name, quote (s));
              error (ਛਾਂਟੋ_FAILURE, 0,
                     _("minimum --%s argument is %s"),
                     long_options[oi].name, quote ("2"));
            }
          ਨਹੀਂਤਾਂ ਜੇ (max_nmerge < nmerge)
            {
              e = LONGINT_ਉਛਾਲ;
            }
          ਨਹੀਂਤਾਂ
            ਵਾਪਸ;
        }
    }

  ਜੇ (e == LONGINT_ਉਛਾਲ)
    {
      error (0, 0, _("--%s argument %s too large"),
             long_options[oi].name, quote (s));
      error (ਛਾਂਟੋ_FAILURE, 0,
             _("maximum --%s argument with current rlimit is %u"),
             long_options[oi].name, max_nmerge);
    }
  ਨਹੀਂਤਾਂ
    xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the amount of main memory to use when sorting.  */
ਸਥਿਰ ਖਾਲੀ
specify_ਛਾਂਟੋ_ਮਾਪ (ਪੂਰਨ oi, ਅੱਖਰ c, ਅੱਖਰ ਅਚਲ *s)
{
  uintmax_t n;
  ਅੱਖਰ *suffix;
  ਗਿਣਤੀ strtol_error e = xstrtoumax (s, &suffix, 10, &n, "EgGkKmMPQRtTYZ");

  /* The default unit is KiB.  */
  ਜੇ (e == LONGINT_OK && c_isdigit (suffix[-1]) && ckd_mul (&n, n, 1024))
    e = LONGINT_ਉਛਾਲ;

  /* A 'b' suffix means bytes; a '%' suffix means percent of memory.  */
  ਜੇ (e == LONGINT_INVALID_SUFFIX_CHAR && c_isdigit (suffix[-1]) && ! suffix[1])
    ਚੋਣ (suffix[0])
      {
      ਹਾਲਤ 'b':
        e = LONGINT_OK;
        ਰੋਕੋ;

      ਹਾਲਤ '%':
        {
          ਦੁੱਗਣਾ mem = physmem_ਕੁੱਲ () * n / 100;

          /* Use "<", not "<=", to avoid problems with rounding.  */
          ਜੇ (mem < UINTMAX_MAX)
            {
              n = mem;
              e = LONGINT_OK;
            }
          ਨਹੀਂਤਾਂ
            e = LONGINT_ਉਛਾਲ;
        }
        ਰੋਕੋ;
      }

  ਜੇ (e == LONGINT_OK)
    {
      /* If multiple sort sizes are specified, take the maximum, so
         that option order does not matter.  */
      ਜੇ (n < ਛਾਂਟੋ_ਮਾਪ)
        ਵਾਪਸ;

      ਛਾਂਟੋ_ਮਾਪ = n;
      ਜੇ (ਛਾਂਟੋ_ਮਾਪ == n)
        {
          ਛਾਂਟੋ_ਮਾਪ = MAX (ਛਾਂਟੋ_ਮਾਪ, MIN_ਛਾਂਟੋ_ਮਾਪ);
          ਵਾਪਸ;
        }

      e = LONGINT_ਉਛਾਲ;
    }

  xstrtol_fatal (e, oi, c, long_options, s);
}

/* Specify the number of threads to spawn during internal sort.  */
ਸਥਿਰ size_t
specify_nthreads (ਪੂਰਨ oi, ਅੱਖਰ c, ਅੱਖਰ ਅਚਲ *s)
{
  uintmax_t nthreads;
  ਗਿਣਤੀ strtol_error e = xstrtoumax (s, ਨਿਰਾ, 10, &nthreads, "");
  ਜੇ (e == LONGINT_ਉਛਾਲ)
    ਵਾਪਸ SIZE_MAX;
  ਜੇ (e != LONGINT_OK)
    xstrtol_fatal (e, oi, c, long_options, s);
  ਜੇ (SIZE_MAX < nthreads)
    nthreads = SIZE_MAX;
  ਜੇ (nthreads == 0)
    error (ਛਾਂਟੋ_FAILURE, 0, _("number in parallel must be nonzero"));
  ਵਾਪਸ nthreads;
}

/* Return the default sort size.  This is a growth limit, not necessarily
   the initial allocation size.  */
ਸਥਿਰ size_t
default_ਛਾਂਟੋ_ਮਾਪ (ਖਾਲੀ)
{
  /* Let SIZE be MEM, but no more than the maximum object size,
     total memory, or system resource limits.  Don't bother to check
     for values like RLIM_INFINITY since in practice they are not much
     less than SIZE_MAX.  */
  size_t ਮਾਪ = SIZE_MAX;
  ਬਣਤਰ rlimit rlimit;
  ਜੇ (getrlimit (RLIMIT_ਅੰਕੜੇ, &rlimit) == 0 && rlimit.rlim_cur < ਮਾਪ)
    ਮਾਪ = rlimit.rlim_cur;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RLIMIT_AS
  ਜੇ (getrlimit (RLIMIT_AS, &rlimit) == 0 && rlimit.rlim_cur < ਮਾਪ)
    ਮਾਪ = rlimit.rlim_cur;
#ਜੇ_ਅੰਤ

  /* Leave a large safety margin for the above limits, as failure can
     occur when they are exceeded.  */
  ਮਾਪ /= 2;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RLIMIT_RSS
  /* Leave a 1/16 margin for RSS to leave room for code, stack, etc.
     Exceeding RSS is not fatal, but can be quite slow.  */
  ਜੇ (getrlimit (RLIMIT_RSS, &rlimit) == 0 && rlimit.rlim_cur / 16 * 15 < ਮਾਪ)
    ਮਾਪ = rlimit.rlim_cur / 16 * 15;
#ਜੇ_ਅੰਤ

  /* Let MEM be available memory or 1/8 of total memory, whichever
     is greater.  */
  ਦੁੱਗਣਾ avail = physmem_ਉਪਲਬਧ ();
  ਦੁੱਗਣਾ ਕੁੱਲ = physmem_ਕੁੱਲ ();
  ਦੁੱਗਣਾ mem = MAX (avail, ਕੁੱਲ / 8);

  /* Leave a 1/4 margin for physical memory.  */
  ਜੇ (ਕੁੱਲ * 0.75 < ਮਾਪ)
    ਮਾਪ = ਕੁੱਲ * 0.75;

  /* Return the minimum of MEM and SIZE, but no less than
     MIN_SORT_SIZE.  Avoid the MIN macro here, as it is not quite
     right when only one argument is floating point.  */
  ਜੇ (mem < ਮਾਪ)
    ਮਾਪ = mem;
  ਵਾਪਸ MAX (ਮਾਪ, MIN_ਛਾਂਟੋ_ਮਾਪ);
}

/* Return true if *ALLOC has been adjusted to a size suitable for a sort
   buffer.  */

ਸਥਿਰ bool
ਸਤਰ_aligned_ਮਾਪ (size_t *alloc)
{
  size_t ਮਾਪ = *alloc;
  size_t alignment = ਆਕਾਰ (ਬਣਤਰ ਸਤਰ);
  size_t padding = alignment - ਮਾਪ % alignment;
  size_t aligned;

  ਜੇ (ckd_ਜੋੜੋ (&aligned, ਮਾਪ, padding))
    ਵਾਪਸ false;

  *alloc = aligned;
  ਵਾਪਸ true;
}

/* Return the default initial allocation for a growable sort buffer.  */

ਸਥਿਰ size_t
default_initial_ਛਾਂਟੋ_ਮਾਪ (size_t ਸਤਰ_ਬਾਈਟਾਂ)
{
  size_t ਮਾਪ = ਸਤਰ_ਬਾਈਟਾਂ + 2;
  size_t ਨਿਵੇਸ਼_ਮਾਪ;

  ਜੇ (ckd_mul (&ਨਿਵੇਸ਼_ਮਾਪ, ਨਿਵੇਸ਼_FILE_ਮਾਪ_GUESS, ਸਤਰ_ਬਾਈਟਾਂ + 1)
      || ckd_ਜੋੜੋ (&ਮਾਪ, ਮਾਪ, ਨਿਵੇਸ਼_ਮਾਪ))
    ਵਾਪਸ SIZE_MAX;

  ਵਾਪਸ MAX (ਮਾਪ, MIN_ਛਾਂਟੋ_ਮਾਪ);
}

/* Store into *WORST_CASE the allocation needed for FILE_SIZE input bytes
   in the worst case, where each input byte is a line delimiter except
   for a final non-delimiter byte.  Return false on overflow.  */

ਸਥਿਰ bool
ਨਿਵੇਸ਼_ਮਾਪ_ਬਫਰ_ਬਾਈਟਾਂ (uintmax_t file_ਮਾਪ, size_t ਸਤਰ_ਬਾਈਟਾਂ,
                         size_t *worst_case)
{
  size_t worst_case_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_byte = ਸਤਰ_ਬਾਈਟਾਂ + 1;

  ਜੇ (SIZE_MAX / worst_case_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_byte < file_ਮਾਪ)
    ਵਾਪਸ false;

  size_t ਮਾਪ = file_ਮਾਪ * worst_case_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_byte;
  ਜੇ (SIZE_MAX - ਮਾਪ < 1)
    ਵਾਪਸ false;

  *worst_case = ਮਾਪ + 1;
  ਵਾਪਸ true;
}

/* Set *POLICY to the sort buffer policy to use with the input files identified
   by FPS and FILES, which are alternate names of the same files.
   NFILES gives the number of input files; NFPS may be less.  Assume
   that each input line requires LINE_BYTES extra bytes' worth of line
   information.  Do not exceed the size bound specified by the user
   (or a default size bound, if the user does not specify one).  */

ਸਥਿਰ ਖਾਲੀ
ਛਾਂਟੋ_ਬਫਰ_policy (FILE *ਅਚਲ *fps, size_t nfps,
                    ਅੱਖਰ *ਅਚਲ *files, size_t nfiles,
                    size_t ਸਤਰ_ਬਾਈਟਾਂ,
                    ਬਣਤਰ ਛਾਂਟੋ_ਬਫਰ_policy *policy)
{
  size_t ਮਾਪ_bound = ਛਾਂਟੋ_ਮਾਪ ? ਛਾਂਟੋ_ਮਾਪ : default_ਛਾਂਟੋ_ਮਾਪ ();
  size_t initial_bound =
    ਛਾਂਟੋ_ਮਾਪ ? ਮਾਪ_bound : default_initial_ਛਾਂਟੋ_ਮਾਪ (ਸਤਰ_ਬਾਈਟਾਂ);

  /* In the worst case, each input byte is a newline.  */
  size_t worst_case_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_byte = ਸਤਰ_ਬਾਈਟਾਂ + 1;

  /* Keep enough room for one extra input line and an extra byte.
     This extra room might be needed when preparing to read EOF.  */
  size_t ਮਾਪ = worst_case_ਪ੍ਰਤੀ_ਨਿਵੇਸ਼_byte + 1;

  initial_bound = MIN (initial_bound, ਮਾਪ_bound);
  policy->ਹੱਦਬੰਦੀ = ਮਾਪ_bound;
  policy->growth_failed = false;

  ਲਈ (size_t i = 0; i < nfiles; i++)
    {
      ਬਣਤਰ stat st;
      off_t file_ਮਾਪ;
      size_t worst_case;
      bool ਗਿਆਤ_ਮਾਪ;

      ਜੇ (ਮਾਪ_bound <= ਮਾਪ)
        {
          policy->initial = ਮਾਪ_bound;
          ਵਾਪਸ;
        }

      ਜੇ ((i < nfps ? fstat (fileno (fps[i]), &st)
           : streq (files[i], "-") ? fstat (STDIN_FILENO, &st)
           : stat (files[i], &st))
          != 0)
        ਛਾਂਟੋ_die (_("stat failed"), files[i]);

      ਗਿਆਤ_ਮਾਪ = usable_st_ਮਾਪ (&st) && 0 < st.st_ਮਾਪ;
      ਜੇ (ਗਿਆਤ_ਮਾਪ)
        file_ਮਾਪ = st.st_ਮਾਪ;
      ਨਹੀਂਤਾਂ
        {
          /* The file has unknown size.  If the user specified a sort
             buffer size, use that; otherwise, guess the size.  */
          ਜੇ (ਛਾਂਟੋ_ਮਾਪ)
            {
              policy->initial = ਮਾਪ_bound;
              ਵਾਪਸ;
            }
          file_ਮਾਪ = ਨਿਵੇਸ਼_FILE_ਮਾਪ_GUESS;
        }

      /* Add the amount of memory needed to represent the worst case
         where the input consists entirely of newlines followed by a
         single non-newline.  Check for overflow.  */
      ਜੇ (! ਨਿਵੇਸ਼_ਮਾਪ_ਬਫਰ_ਬਾਈਟਾਂ (file_ਮਾਪ, ਸਤਰ_ਬਾਈਟਾਂ, &worst_case)
          || ਮਾਪ_bound - ਮਾਪ <= worst_case)
        {
          policy->initial = ਮਾਪ_bound;
          ਵਾਪਸ;
        }

      ਜੇ (! ਗਿਆਤ_ਮਾਪ && ! ਛਾਂਟੋ_ਮਾਪ
          && (initial_bound <= ਮਾਪ || initial_bound - ਮਾਪ <= worst_case))
        {
          policy->initial = MAX (ਮਾਪ, initial_bound);
          ਵਾਪਸ;
        }

      ਮਾਪ += worst_case;
    }

  policy->initial = MAX (ਮਾਪ, MIN_ਛਾਂਟੋ_ਮਾਪ);
}

/* Initialize BUF.  Reserve LINE_BYTES bytes for each line; LINE_BYTES
   must be at least sizeof (struct line).  Allocate ALLOC bytes
   initially.  */

ਸਥਿਰ ਖਾਲੀ
initbuf (ਬਣਤਰ ਬਫਰ *buf, size_t ਸਤਰ_ਬਾਈਟਾਂ, size_t alloc)
{
  /* Ensure that the line array is properly aligned.  If the desired
     size cannot be allocated, repeatedly halve it until allocation
     succeeds.  The smaller allocation may hurt overall performance,
     but that's better than failing.  */
  ਜਦੋਂ (true)
    {
      ਜੇ (! ਸਤਰ_aligned_ਮਾਪ (&alloc))
        buf->buf = ਨਿਰਾ;
      ਨਹੀਂਤਾਂ
        buf->buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ (alloc);
      ਜੇ (buf->buf)
        ਰੋਕੋ;
      alloc /= 2;
      ਜੇ (alloc <= ਸਤਰ_ਬਾਈਟਾਂ + 1)
        xalloc_die ();
    }

  buf->ਸਤਰ_ਬਾਈਟਾਂ = ਸਤਰ_ਬਾਈਟਾਂ;
  buf->alloc = alloc;
  buf->ਵਰਤਿਆ = buf->ਖੱਬਾ = buf->nlines = 0;
  buf->eof = false;
}

/* Return one past the limit of the line array.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ ਸਤਰ *
ਬਫਰ_linelim (ਬਣਤਰ ਬਫਰ ਅਚਲ *buf)
{
  ਖਾਲੀ *linelim = buf->buf + buf->alloc;
  ਵਾਪਸ linelim;
}

/* Try to resize BUF to ALLOC bytes.  Return true if successful.  This
   preserves the input data and the line array, adjusting line pointers
   when the buffer base changes.  */

ਸਥਿਰ bool
try_growbuf (ਬਣਤਰ ਬਫਰ *buf, size_t alloc)
{
  ਜੇ (! ਸਤਰ_aligned_ਮਾਪ (&alloc) || alloc <= buf->alloc)
    ਵਾਪਸ false;

  ਅੱਖਰ *newbuf = ਸਮ੍ਰਿਤੀ_ਦਿਓ (alloc);
  ਜੇ (! newbuf)
    ਵਾਪਸ false;

  ਅੱਖਰ *oldbuf = buf->buf;
  ਬਣਤਰ ਸਤਰ *old_linelim = ਬਫਰ_linelim (buf);
  ਬਣਤਰ ਸਤਰ *old_ਸਤਰ = old_linelim - buf->nlines;

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (newbuf, oldbuf, buf->ਵਰਤਿਆ);

  ਬਣਤਰ ਸਤਰ *new_linelim = (ਖਾਲੀ *) (newbuf + alloc);
  ਬਣਤਰ ਸਤਰ *new_ਸਤਰ = new_linelim - buf->nlines;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (new_ਸਤਰ, old_ਸਤਰ, buf->nlines * ਆਕਾਰ *new_ਸਤਰ);

  ਲਈ (ਬਣਤਰ ਸਤਰ *ਸਤਰ = new_ਸਤਰ; ਸਤਰ < new_linelim; ਸਤਰ++)
    {
      ਸਤਰ->ਪਾਠ = newbuf + (ਸਤਰ->ਪਾਠ - oldbuf);
      ਜੇ (keylist)
        {
          ਸਤਰ->keybeg = newbuf + (ਸਤਰ->keybeg - oldbuf);
          ਸਤਰ->keylim = newbuf + (ਸਤਰ->keylim - oldbuf);
        }
    }

  ਮੁਕਤ (oldbuf);
  buf->buf = newbuf;
  buf->alloc = alloc;
  ਵਾਪਸ true;
}

/* Try to grow BUF according to POLICY.  Return true if the buffer grew.  */

ਸਥਿਰ bool
maybe_growbuf (ਬਣਤਰ ਬਫਰ *buf, ਬਣਤਰ ਛਾਂਟੋ_ਬਫਰ_policy *policy)
{
  ਜੇ (! policy || policy->growth_failed || policy->ਹੱਦਬੰਦੀ <= buf->alloc)
    ਵਾਪਸ false;

  size_t alloc;
  ਜੇ (buf->alloc <= policy->ਹੱਦਬੰਦੀ / 3)
    alloc = buf->alloc * 3;
  ਨਹੀਂਤਾਂ
    alloc = policy->ਹੱਦਬੰਦੀ;

  ਜੇ (try_growbuf (buf, alloc))
    ਵਾਪਸ true;

  policy->growth_failed = true;
  ਵਾਪਸ false;
}

/* Return a pointer to the first character of the field specified
   by KEY in LINE. */

ਸਥਿਰ ਅੱਖਰ *
begfield (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ)
{
  ਅੱਖਰ *ptr = ਸਤਰ->ਪਾਠ, *lim = ptr + ਸਤਰ->ਲੰਮਾਈ - 1;
  size_t sword = ਕੁੰਜੀ->sword;
  size_t schar = ਕੁੰਜੀ->schar;

  /* The leading field separator itself is included in a field when -t
     is absent.  */

  ਜੇ (tab != TAB_DEFAULT)
    ਜਦੋਂ (ptr < lim && sword--)
      {
        ਅੱਖਰ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ਜੇ (ptr < lim)
          ++ptr;
      }
  ਨਹੀਂਤਾਂ
    ਜਦੋਂ (ptr < lim && sword--)
      {
        ਜਦੋਂ (ptr < lim && blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
          ++ptr;
        ਜਦੋਂ (ptr < lim && !blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
          ++ptr;
      }

  /* If we're ignoring leading blanks when computing the Start
     of the field, skip past them here.  */
  ਜੇ (ਕੁੰਜੀ->skipsblanks)
    ਜਦੋਂ (ptr < lim && blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
      ++ptr;

  /* Advance PTR by SCHAR (if possible), but no further than LIM.  */
  size_t ਬਚਿਆ_ਬਾਈਟਾਂ = lim - ptr;
  ਜੇ (schar < ਬਚਿਆ_ਬਾਈਟਾਂ)
    ptr += schar;
  ਨਹੀਂਤਾਂ
    ptr = lim;

  ਵਾਪਸ ptr;
}

/* Return the limit of (a pointer to the first character after) the field
   in LINE specified by KEY. */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਅੱਖਰ *
limfield (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ)
{
  ਅੱਖਰ *ptr = ਸਤਰ->ਪਾਠ, *lim = ptr + ਸਤਰ->ਲੰਮਾਈ - 1;
  size_t eword = ਕੁੰਜੀ->eword, echar = ਕੁੰਜੀ->echar;

  ਜੇ (echar == 0)
    eword++; /* Skip all of end field.  */

  /* Move PTR past EWORD fields or to one past the last byte on LINE,
     whichever comes first.  If there are more than EWORD fields, leave
     PTR pointing at the beginning of the field having zero-based index,
     EWORD.  If a delimiter character was specified (via -t), then that
     'beginning' is the first character following the delimiting TAB.
     Otherwise, leave PTR pointing at the first 'blank' character after
     the preceding field.  */
  ਜੇ (tab != TAB_DEFAULT)
    ਜਦੋਂ (ptr < lim && eword--)
      {
        ਅੱਖਰ *sep = memchr (ptr, tab, lim - ptr);
        ptr = sep ? sep : lim;
        ਜੇ (ptr < lim && (eword || echar))
          ++ptr;
      }
  ਨਹੀਂਤਾਂ
    ਜਦੋਂ (ptr < lim && eword--)
      {
        ਜਦੋਂ (ptr < lim && blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
          ++ptr;
        ਜਦੋਂ (ptr < lim && !blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
          ++ptr;
      }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ POSIX_UNSPECIFIED
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
  ਜੇ (tab != TAB_DEFAULT)
    {
      ਅੱਖਰ *newlim;
      newlim = memchr (ptr, tab, lim - ptr);
      ਜੇ (newlim)
        lim = newlim;
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ *newlim;
      newlim = ptr;
      ਜਦੋਂ (newlim < lim && blanks[ਮੰਜ਼ਿਲ_uchar (*newlim)])
        ++newlim;
      ਜਦੋਂ (newlim < lim && !blanks[ਮੰਜ਼ਿਲ_uchar (*newlim)])
        ++newlim;
      lim = newlim;
    }
#ਜੇ_ਅੰਤ

  ਜੇ (echar != 0) /* We need to skip over a portion of the end field.  */
    {
      /* If we're ignoring leading blanks when computing the End
         of the field, skip past them here.  */
      ਜੇ (ਕੁੰਜੀ->skipeblanks)
        ਜਦੋਂ (ptr < lim && blanks[ਮੰਜ਼ਿਲ_uchar (*ptr)])
          ++ptr;

      /* Advance PTR by ECHAR (if possible), but no further than LIM.  */
      size_t ਬਚਿਆ_ਬਾਈਟਾਂ = lim - ptr;
      ਜੇ (echar < ਬਚਿਆ_ਬਾਈਟਾਂ)
        ptr += echar;
      ਨਹੀਂਤਾਂ
        ptr = lim;
    }

  ਵਾਪਸ ptr;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.  If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Set up BUF's line
   table too.  FILE is the name of the file corresponding to FP.
   Return true if some input was read.  */

ਸਥਿਰ bool
fillbuf (ਬਣਤਰ ਬਫਰ *buf, FILE *fp, ਅੱਖਰ ਅਚਲ *file,
         ਬਣਤਰ ਛਾਂਟੋ_ਬਫਰ_policy *policy)
{
  ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ = keylist;
  ਅੱਖਰ eol = eolchar;
  size_t ਸਤਰ_ਬਾਈਟਾਂ = buf->ਸਤਰ_ਬਾਈਟਾਂ;
  size_t mergesize = ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ - MIN_ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ;

  ਜੇ (buf->eof)
    ਵਾਪਸ false;

  ਜੇ (buf->ਵਰਤਿਆ != buf->ਖੱਬਾ)
    {
      memmove (buf->buf, buf->buf + buf->ਵਰਤਿਆ - buf->ਖੱਬਾ, buf->ਖੱਬਾ);
      buf->ਵਰਤਿਆ = buf->ਖੱਬਾ;
      buf->nlines = 0;
    }

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *ptr = buf->buf + buf->ਵਰਤਿਆ;
      ਬਣਤਰ ਸਤਰ *linelim = ਬਫਰ_linelim (buf);
      ਬਣਤਰ ਸਤਰ *ਸਤਰ = linelim - buf->nlines;
      size_t avail = (ਅੱਖਰ *) linelim - buf->nlines * ਸਤਰ_ਬਾਈਟਾਂ - ptr;
      ਅੱਖਰ *ਸਤਰ_ਸ਼ੁਰੂ = buf->nlines ? ਸਤਰ->ਪਾਠ + ਸਤਰ->ਲੰਮਾਈ : buf->buf;

      ਜਦੋਂ (ਸਤਰ_ਬਾਈਟਾਂ + 1 < avail)
        {
          /* Read as many bytes as possible, but do not read so many
             bytes that there might not be enough room for the
             corresponding line array.  The worst case is when the
             rest of the input file consists entirely of newlines,
             except that the last byte is not a newline.  */
          size_t readsize = (avail - 1) / (ਸਤਰ_ਬਾਈਟਾਂ + 1);
          size_t ਬਾਈਟਾਂ_read = fread (ptr, 1, readsize, fp);
          ਅੱਖਰ *ptrlim = ptr + ਬਾਈਟਾਂ_read;
          ਅੱਖਰ *p;
          avail -= ਬਾਈਟਾਂ_read;

          ਜੇ (ਬਾਈਟਾਂ_read != readsize)
            {
              ਜੇ (ferror (fp))
                ਛਾਂਟੋ_die (_("read failed"), file);
              ਜੇ (feof (fp))
                {
                  buf->eof = true;
                  ਜੇ (buf->buf == ptrlim)
                    ਵਾਪਸ false;
                  ਜੇ (ਸਤਰ_ਸ਼ੁਰੂ != ptrlim && ptrlim[-1] != eol)
                    *ptrlim++ = eol;
                }
            }

          /* Find and record each line in the just-read input.  */
          ਜਦੋਂ ((p = memchr (ptr, eol, ptrlim - ptr)))
            {
              /* Delimit the line with NUL. This eliminates the need to
                 temporarily replace the last byte with NUL when calling
                 xmemcoll, which increases performance.  */
              *p = '\0';
              ptr = p + 1;
              ਸਤਰ--;
              ਸਤਰ->ਪਾਠ = ਸਤਰ_ਸ਼ੁਰੂ;
              ਸਤਰ->ਲੰਮਾਈ = ptr - ਸਤਰ_ਸ਼ੁਰੂ;
              mergesize = MAX (mergesize, ਸਤਰ->ਲੰਮਾਈ);
              avail -= ਸਤਰ_ਬਾਈਟਾਂ;

              ਜੇ (ਕੁੰਜੀ)
                {
                  /* Precompute the position of the first key for
                     efficiency.  */
                  ਸਤਰ->keylim = (ਕੁੰਜੀ->eword == SIZE_MAX
                                  ? p
                                  : limfield (ਸਤਰ, ਕੁੰਜੀ));

                  ਜੇ (ਕੁੰਜੀ->sword != SIZE_MAX)
                    ਸਤਰ->keybeg = begfield (ਸਤਰ, ਕੁੰਜੀ);
                  ਨਹੀਂਤਾਂ
                    {
                      ਜੇ (ਕੁੰਜੀ->skipsblanks)
                        ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*ਸਤਰ_ਸ਼ੁਰੂ)])
                          ਸਤਰ_ਸ਼ੁਰੂ++;
                      ਸਤਰ->keybeg = ਸਤਰ_ਸ਼ੁਰੂ;
                    }
                }

              ਸਤਰ_ਸ਼ੁਰੂ = ptr;
            }

          ptr = ptrlim;
          ਜੇ (buf->eof)
            ਰੋਕੋ;
        }

      buf->ਵਰਤਿਆ = ptr - buf->buf;
      buf->nlines = ਬਫਰ_linelim (buf) - ਸਤਰ;
      ਜੇ (buf->nlines != 0)
        {
          ਜੇ (! buf->eof && maybe_growbuf (buf, policy))
            ਜਾਰੀ;
          buf->ਖੱਬਾ = ptr - ਸਤਰ_ਸ਼ੁਰੂ;
          ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ = mergesize + MIN_ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ;
          ਵਾਪਸ true;
        }

      {
        /* The current input line is too long to fit in the buffer.
           Increase the buffer size and try again, keeping it properly
           aligned.  */
        idx_t ਸਤਰ_alloc = buf->alloc / ਆਕਾਰ (ਬਣਤਰ ਸਤਰ);
        buf->buf = xpalloc (buf->buf, &ਸਤਰ_alloc, 1, -1, ਆਕਾਰ (ਬਣਤਰ ਸਤਰ));
        buf->alloc = ਸਤਰ_alloc * ਆਕਾਰ (ਬਣਤਰ ਸਤਰ);
      }
    }
}

/* Table that maps characters to order-of-magnitude values.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ unit_ਕ੍ਰਮ[UCHAR_LIM] =
  {
#ਜੇ ! ('K' == 75 && 'M' == 77 && 'G' == 71 && 'T' == 84 && 'P' == 80 \
       && 'E' == 69 && 'Z' == 90 && 'Y' == 89 && 'R' == 82 && 'Q' == 81 \
       && 'k' == 107)
    /* This initializer syntax works on all C99 hosts.  For now, use
       it only on non-ASCII hosts, to ease the pain of porting to
       pre-C99 ASCII hosts.  */
    ['K']=1, ['M']=2, ['G']=3, ['T']=4, ['P']=5, ['E']=6, ['Z']=7, ['Y']=8,
    ['R']=9, ['Q']=10,
    ['k']=1,
#ਨਹੀਂਤਾਂ
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
#ਜੇ_ਅੰਤ
  };

/* Traverse number given as *number consisting of digits, thousands_sep, and
   decimal_point chars only.  Returns the highest digit found in the number,
   or '\0' if no digit has been found.  Upon return *number points at the
   character that immediately follows after the given number.  */
ਸਥਿਰ ਅੱਖਰ
traverse_raw_ਸੰਖਿਆ (ਅੱਖਰ ਅਚਲ **ਸੰਖਿਆ)
{
  ਅੱਖਰ ਅਚਲ *p = *ਸੰਖਿਆ;
  ਅੱਖਰ ch;
  ਅੱਖਰ max_digit = '\0';
  bool ਸਿਰੇ_with_thousands_sep = false;

  /* Scan to end of number.
     Decimals or separators not followed by digits stop the scan.
     Numbers ending in decimals or separators are thus considered
     to be lacking in units.
     FIXME: add support for multibyte thousands_sep and decimal_point.  */

  ਜਦੋਂ (c_isdigit (ch = *p++))
    {
      ਜੇ (max_digit < ch)
        max_digit = ch;

      /* Allow to skip only one occurrence of thousands_sep to avoid finding
         the unit in the next column in case thousands_sep matches as blank
         and is used as column delimiter.  */
      ਸਿਰੇ_with_thousands_sep = (*p == thousands_sep);
      ਜੇ (ਸਿਰੇ_with_thousands_sep)
        ++p;
    }

  ਜੇ (ਸਿਰੇ_with_thousands_sep)
    {
      /* thousands_sep not followed by digit is not allowed.  */
      *ਸੰਖਿਆ = p - 2;
      ਵਾਪਸ max_digit;
    }

  ਜੇ (ch == decimal_ਬਿੰਦੂ)
    ਜਦੋਂ (c_isdigit (ch = *p++))
      ਜੇ (max_digit < ch)
        max_digit = ch;

  *ਸੰਖਿਆ = p - 1;
  ਵਾਪਸ max_digit;
}

/* Return an integer that represents the order of magnitude of the
   unit following the number.  The number may contain thousands
   separators and a decimal point, but it may not contain leading blanks.
   Negative numbers get negative orders; zero numbers have a zero order.  */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
ਲੱਭੋ_unit_ਕ੍ਰਮ (ਅੱਖਰ ਅਚਲ *ਸੰਖਿਆ)
{
  bool minus_sign = (*ਸੰਖਿਆ == '-');
  ਅੱਖਰ ਅਚਲ *p = ਸੰਖਿਆ + minus_sign;
  ਅੱਖਰ max_digit = traverse_raw_ਸੰਖਿਆ (&p);
  ਜੇ ('0' < max_digit)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch = *p;
      ਪੂਰਨ ਕ੍ਰਮ = unit_ਕ੍ਰਮ[ch];
      ਵਾਪਸ (minus_sign ? -ਕ੍ਰਮ : ਕ੍ਰਮ);
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
}

/* Compare numbers A and B ending in units with SI or IEC prefixes
       <none/unknown> < K/k < M < G < T < P < E < Z < Y < R < Q */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
human_numcompare (ਅੱਖਰ ਅਚਲ *a, ਅੱਖਰ ਅਚਲ *b)
{
  ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*a)])
    a++;
  ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*b)])
    b++;

  ਪੂਰਨ diff = ਲੱਭੋ_unit_ਕ੍ਰਮ (a) - ਲੱਭੋ_unit_ਕ੍ਰਮ (b);
  ਵਾਪਸ (diff ? diff : strnumcmp (a, b, decimal_ਬਿੰਦੂ, thousands_sep));
}

/* Compare strings A and B as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */

ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ ਪੂਰਨ
numcompare (ਅੱਖਰ ਅਚਲ *a, ਅੱਖਰ ਅਚਲ *b)
{
  ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*a)])
    a++;
  ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*b)])
    b++;

  ਵਾਪਸ strnumcmp (a, b, decimal_ਬਿੰਦੂ, thousands_sep);
}

ਸਥਿਰ ਪੂਰਨ
nan_compare (ਲੰਮਾ ਦੁੱਗਣਾ a, ਲੰਮਾ ਦੁੱਗਣਾ b)
{
  ਅੱਖਰ buf[2][ਆਕਾਰ "-nan""()" + CHAR_BIT * ਆਕਾਰ a];
  snprintf (buf[0], ਆਕਾਰ buf[0], "%Lf", a);
  snprintf (buf[1], ਆਕਾਰ buf[1], "%Lf", b);
  ਵਾਪਸ strcmp (buf[0], buf[1]);
}

ਸਥਿਰ ਪੂਰਨ
general_numcompare (ਅੱਖਰ ਅਚਲ *sa, ਅੱਖਰ ਅਚਲ *sb)
{
  /* FIXME: maybe add option to try expensive FP conversion
     only if A and B can't be compared more cheaply/accurately.  */

  ਅੱਖਰ *ea;
  ਅੱਖਰ *eb;
  ਲੰਮਾ ਦੁੱਗਣਾ a = strtold (sa, &ea);
  ਲੰਮਾ ਦੁੱਗਣਾ b = strtold (sb, &eb);

  /* Put conversion errors at the start of the collating sequence.  */
  ਜੇ (sa == ea)
    ਵਾਪਸ sb == eb ? 0 : -1;
  ਜੇ (sb == eb)
    ਵਾਪਸ 1;

  /* Sort numbers in the usual way, where -0 == +0.  Put NaNs after
     conversion errors but before numbers; sort them by internal
     bit-pattern, for lack of a more portable alternative.  */
  ਵਾਪਸ (a < b ? -1
          : a > b ? 1
          : a == b ? 0
          : b == b ? -1
          : a == a ? 1
          : nan_compare (a, b));
}

/* Return an integer in 1..12 of the month name MONTH.
   Return 0 if the name in S is not recognized.  */

ਸਥਿਰ ਪੂਰਨ
getmonth (ਅੱਖਰ ਅਚਲ *month, ਅੱਖਰ **ea)
{
  size_t lo = 0;
  size_t hi = MONTHS_ਪ੍ਰਤੀ_YEAR;

  ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*month)])
    month++;

  ਕਰੋ
    {
      size_t ix = (lo + hi) / 2;
      ਅੱਖਰ ਅਚਲ *m = month;
      ਅੱਖਰ ਅਚਲ *n = monthtab[ix].name;

      ਲਈ (;; m++, n++)
        {
          ਜੇ (!*n)
            {
              ਜੇ (ea)
                *ea = (ਅੱਖਰ *) m;
              ਵਾਪਸ monthtab[ix].val;
            }
          ਜੇ (ਮੰਜ਼ਿਲ_uchar (fold_toupper[ਮੰਜ਼ਿਲ_uchar (*m)]) < ਮੰਜ਼ਿਲ_uchar (*n))
            {
              hi = ix;
              ਰੋਕੋ;
            }
          ਨਹੀਂਤਾਂ ਜੇ (ਮੰਜ਼ਿਲ_uchar (fold_toupper[ਮੰਜ਼ਿਲ_uchar (*m)]) > ਮੰਜ਼ਿਲ_uchar (*n))
            {
              lo = ix + 1;
              ਰੋਕੋ;
            }
        }
    }
  ਜਦੋਂ (lo < hi);

  ਵਾਪਸ 0;
}

/* When using the OpenSSL implementation, dynamically link only if -R.
   This saves startup time in the usual (sans -R) case.  */

#ਜੇ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
/* In the typical case where md5.h does not #undef HAVE_OPENSSL_MD5,
   trick md5.h into declaring and using pointers to functions not functions.
   This causes the compiler's -lcrypto option to have no effect,
   as sort.o no longer uses any crypto symbols statically.  */

# ਜੇ 14 <= __GNUC__
#  pragma GCC diagnostic ਧੱਕੋ
#  pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
# endif
# define MD5_Init (*ptr_MD5_Init)
# define MD5_Update (*ptr_MD5_Update)
# define MD5_Final (*ptr_MD5_Final)
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "md5.h"

#ਜੇ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
# ਜੇ 14 <= __GNUC__
#  pragma GCC diagnostic ਕੱਢੋ
# endif
# include <dlfcn.h>

/* Require distributions make OpenSSL available for hardware
   optimized implementations of MD-5.  */
# include "sd-dlopen.h"
SD_ELF_NOTE_DLOPEN ("random-sort",
                    "Support for random sorting",
                    SD_ELF_NOTE_DLOPEN_ਤਰਜੀਹ_REQUIRED,
                    LIBCRYPTO_SONAME);

/* Diagnose a dynamic linking failure.  */
ਸਥਿਰ ਖਾਲੀ
link_failure (ਖਾਲੀ)
{
  error (ਛਾਂਟੋ_FAILURE, 0, "%s", dlerror ());
}

/* Return a function pointer in HANDLE for SYMBOL.  */
ਸਥਿਰ ਖਾਲੀ *
ਚਿੰਨ੍ਹ_ਪਤਾ (ਖਾਲੀ *handle, ਅੱਖਰ ਅਚਲ *ਚਿੰਨ੍ਹ)
{
  ਖਾਲੀ *ਪਤਾ = dlsym (handle, ਚਿੰਨ੍ਹ);
  ਜੇ (!ਪਤਾ)
    link_failure ();
  ਵਾਪਸ ਪਤਾ;
}
#ਜੇ_ਅੰਤ

/* Dynamically link the crypto library, if it needs linking.  */
ਸਥਿਰ ਖਾਲੀ
link_libcrypto (ਖਾਲੀ)
{
#ਜੇ DLOPEN_LIBCRYPTO && HAVE_OPENSSL_MD5
  ਖਾਲੀ *handle = dlopen (LIBCRYPTO_SONAME, RTLD_LAZY | RTLD_GLOBAL);
  ਜੇ (!handle)
    link_failure ();
  ptr_MD5_Init = ਚਿੰਨ੍ਹ_ਪਤਾ (handle, "MD5_Init");
  ptr_MD5_Update = ਚਿੰਨ੍ਹ_ਪਤਾ (handle, "MD5_Update");
  ptr_MD5_Final = ਚਿੰਨ੍ਹ_ਪਤਾ (handle, "MD5_Final");
#ਜੇ_ਅੰਤ
}

/* A randomly chosen MD5 state, used for random comparison.  */
ਸਥਿਰ ਬਣਤਰ md5_ctx random_md5_state;

/* Initialize the randomly chosen MD5 state.  */

ਸਥਿਰ ਖਾਲੀ
random_md5_state_init (ਅੱਖਰ ਅਚਲ *random_ਸਰੋਤ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ buf[MD5_DIGEST_ਮਾਪ];
  ਬਣਤਰ randread_ਸਰੋਤ *r = randread_new (random_ਸਰੋਤ, ਆਕਾਰ buf);
  ਜੇ (! r)
    ਛਾਂਟੋ_die (_("open failed"), random_ਸਰੋਤ ? random_ਸਰੋਤ : "getrandom");
  randread (r, buf, ਆਕਾਰ buf);
  ਜੇ (randread_free (r) != 0)
    ਛਾਂਟੋ_die (_("close failed"), random_ਸਰੋਤ);
  link_libcrypto ();
  md5_init_ctx (&random_md5_state);
  md5_process_ਬਾਈਟਾਂ (buf, ਆਕਾਰ buf, &random_md5_state);
}

/* This is like strxfrm, except it reports any error and exits.  */

ਸਥਿਰ size_t
xstrxfrm (ਅੱਖਰ *ਸੀਮਿਤ dest, ਅੱਖਰ ਅਚਲ *ਸੀਮਿਤ src, size_t destsize)
{
  errno = 0;
  size_t translated_ਮਾਪ = strxfrm (dest, src, destsize);

  ਜੇ (errno)
    {
      error (0, errno, _("string transformation failed"));
      error (0, 0, _("set LC_ALL='C' to work around the problem"));
      error (ਛਾਂਟੋ_FAILURE, 0,
             _("the original string was %s"),
             quotearg_n_style (0, locale_quoting_style, src));
    }

  ਵਾਪਸ translated_ਮਾਪ;
}

/* Compare the keys TEXTA (of length LENA) and TEXTB (of length LENB)
   using one or more random hash functions.  TEXTA[LENA] and
   TEXTB[LENB] must be zero.  */

ਸਥਿਰ ਪੂਰਨ
compare_random (ਅੱਖਰ *ਸੀਮਿਤ texta, size_t lena,
                ਅੱਖਰ *ਸੀਮਿਤ textb, size_t lenb)
{
  /* XFRM_DIFF records the equivalent of memcmp on the transformed
     data.  This is used to break ties if there is a checksum
     collision, and this is good enough given the astronomically low
     probability of a collision.  */
  ਪੂਰਨ xfrm_diff = 0;

  ਅੱਖਰ stackbuf[4000];
  ਅੱਖਰ *buf = stackbuf;
  size_t bufsize = ਆਕਾਰ stackbuf;
  ਖਾਲੀ *allocated = ਨਿਰਾ;
  uint32_t dig[2][MD5_DIGEST_ਮਾਪ / ਆਕਾਰ (uint32_t)];
  ਬਣਤਰ md5_ctx s[2];
  s[0] = s[1] = random_md5_state;

  ਜੇ (hard_LC_COLLATE)
    {
      ਅੱਖਰ ਅਚਲ *lima = texta + lena;
      ਅੱਖਰ ਅਚਲ *limb = textb + lenb;

      ਜਦੋਂ (true)
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
          ਜੇ (bufsize < guess_bufsize)
            {
              bufsize = MAX (guess_bufsize, bufsize * 3 / 2);
              ਮੁਕਤ (allocated);
              buf = allocated = ਸਮ੍ਰਿਤੀ_ਦਿਓ (bufsize);
              ਜੇ (! buf)
                {
                  buf = stackbuf;
                  bufsize = ਆਕਾਰ stackbuf;
                }
            }

          size_t sizea =
            (texta < lima ? xstrxfrm (buf, texta, bufsize) + 1 : 0);
          bool a_ਸਮਾਉਂਦਾ = sizea <= bufsize;
          size_t sizeb =
            (textb < limb
             ? (xstrxfrm ((a_ਸਮਾਉਂਦਾ ? buf + sizea : ਨਿਰਾ), textb,
                          (a_ਸਮਾਉਂਦਾ ? bufsize - sizea : 0))
                + 1)
             : 0);

          ਜੇ (! (a_ਸਮਾਉਂਦਾ && sizea + sizeb <= bufsize))
            {
              bufsize = sizea + sizeb;
              ਜੇ (bufsize < SIZE_MAX / 3)
                bufsize = bufsize * 3 / 2;
              ਮੁਕਤ (allocated);
              buf = allocated = xmalloc (bufsize);
              ਜੇ (texta < lima)
                strxfrm (buf, texta, sizea);
              ਜੇ (textb < limb)
                strxfrm (buf + sizea, textb, sizeb);
            }

          /* Advance past NULs to the next part of each input string,
             exiting the loop if both strings are exhausted.  When
             exiting the loop, prepare to finish off the tiebreaker
             comparison properly.  */
          ਜੇ (texta < lima)
            texta += ਲੜੀ_ਲੰਬਾਈ (texta) + 1;
          ਜੇ (textb < limb)
            textb += ਲੜੀ_ਲੰਬਾਈ (textb) + 1;
          ਜੇ (! (texta < lima || textb < limb))
            {
              lena = sizea; texta = buf;
              lenb = sizeb; textb = buf + sizea;
              ਰੋਕੋ;
            }

          /* Accumulate the transformed data in the corresponding
             checksums.  */
          md5_process_ਬਾਈਟਾਂ (buf, sizea, &s[0]);
          md5_process_ਬਾਈਟਾਂ (buf + sizea, sizeb, &s[1]);

          /* Update the tiebreaker comparison of the transformed data.  */
          ਜੇ (! xfrm_diff)
            {
              xfrm_diff = memcmp (buf, buf + sizea, MIN (sizea, sizeb));
              ਜੇ (! xfrm_diff)
                xfrm_diff = _GL_CMP (sizea, sizeb);
            }
        }
    }

  /* Compute and compare the checksums.  */
  md5_process_ਬਾਈਟਾਂ (texta, lena, &s[0]); md5_ਅੰਤ_ctx (&s[0], dig[0]);
  md5_process_ਬਾਈਟਾਂ (textb, lenb, &s[1]); md5_ਅੰਤ_ctx (&s[1], dig[1]);
  ਪੂਰਨ diff = memcmp (dig[0], dig[1], ਆਕਾਰ dig[0]);

  /* Fall back on the tiebreaker if the checksums collide.  */
  ਜੇ (! diff)
    {
      ਜੇ (! xfrm_diff)
        {
          xfrm_diff = memcmp (texta, textb, MIN (lena, lenb));
          ਜੇ (! xfrm_diff)
            xfrm_diff = _GL_CMP (lena, lenb);
        }

      diff = xfrm_diff;
    }

  ਮੁਕਤ (allocated);

  ਵਾਪਸ diff;
}

/* Return the printable width of the block of memory starting at
   TEXT and ending just before LIM, counting each tab as one byte.
   FIXME: Should we generally be counting non printable chars?  */

ਸਥਿਰ size_t
debug_ਚੌੜਾਈ (ਅੱਖਰ ਅਚਲ *ਪਾਠ, ਅੱਖਰ ਅਚਲ *lim)
{
  size_t ਚੌੜਾਈ = mbsnwidth (ਪਾਠ, lim - ਪਾਠ, 0);
  ਜਦੋਂ (ਪਾਠ < lim)
    ਚੌੜਾਈ += (*ਪਾਠ++ == '\t');
  ਵਾਪਸ ਚੌੜਾਈ;
}

/* For debug mode, "underline" a key at the
   specified offset and screen width.  */

ਸਥਿਰ ਖਾਲੀ
mark_ਕੁੰਜੀ (size_t ਵਿੱਥ, size_t ਚੌੜਾਈ)
{
  ਜਦੋਂ (ਵਿੱਥ--)
    ਅੱਖਰ_ਲਿਖੋ (' ');

  ਜੇ (!ਚੌੜਾਈ)
    ਲਿਖੋ (_("^ no match for key\n"));
  ਨਹੀਂਤਾਂ
    {
      ਕਰੋ
        ਅੱਖਰ_ਲਿਖੋ ('_');
      ਜਦੋਂ (--ਚੌੜਾਈ);

      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }
}

/* Return true if KEY is a numeric key.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਕੁੰਜੀ_numeric (ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ)
{
  ਵਾਪਸ ਕੁੰਜੀ->numeric || ਕੁੰਜੀ->general_numeric || ਕੁੰਜੀ->human_numeric;
}

/* For LINE, output a debugging line that underlines KEY in LINE.
   If KEY is null, underline the whole line.  */

ਸਥਿਰ ਖਾਲੀ
debug_ਕੁੰਜੀ (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ)
{
  ਅੱਖਰ *ਪਾਠ = ਸਤਰ->ਪਾਠ;
  ਅੱਖਰ *beg = ਪਾਠ;
  ਅੱਖਰ *lim = ਪਾਠ + ਸਤਰ->ਲੰਮਾਈ - 1;

  ਜੇ (ਕੁੰਜੀ)
    {
      ਜੇ (ਕੁੰਜੀ->sword != SIZE_MAX)
        beg = begfield (ਸਤਰ, ਕੁੰਜੀ);
      ਜੇ (ਕੁੰਜੀ->eword != SIZE_MAX)
        {
          lim = limfield (ਸਤਰ, ਕੁੰਜੀ);
          /* Treat field ends before field starts as empty fields.  */
          lim = MAX (beg, lim);
        }

      ਜੇ ((ਕੁੰਜੀ->skipsblanks && ਕੁੰਜੀ->sword == SIZE_MAX)
          || ਕੁੰਜੀ->month || ਕੁੰਜੀ_numeric (ਕੁੰਜੀ))
        {
          ਅੱਖਰ saved = *lim;
          *lim = '\0';

          ਜਦੋਂ (blanks[ਮੰਜ਼ਿਲ_uchar (*beg)])
            beg++;

          ਅੱਖਰ *tighter_lim = beg;

          ਜੇ (lim < beg)
            tighter_lim = lim;
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->month)
            getmonth (beg, &tighter_lim);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->general_numeric)
            ignore_ਮੁੱਲ (strtold (beg, &tighter_lim));
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->numeric || ਕੁੰਜੀ->human_numeric)
            {
              ਅੱਖਰ ਅਚਲ *p = beg + (beg < lim && *beg == '-');
              ਅੱਖਰ max_digit = traverse_raw_ਸੰਖਿਆ (&p);
              ਜੇ ('0' <= max_digit)
                {
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch = *p;
                  tighter_lim = (ਅੱਖਰ *) p
                    + (ਕੁੰਜੀ->human_numeric && unit_ਕ੍ਰਮ[ch]);
                }
            }
          ਨਹੀਂਤਾਂ
            tighter_lim = lim;

          *lim = saved;
          lim = tighter_lim;
        }
    }

  size_t ਵਿੱਥ = debug_ਚੌੜਾਈ (ਪਾਠ, beg);
  size_t ਚੌੜਾਈ = debug_ਚੌੜਾਈ (beg, lim);
  mark_ਕੁੰਜੀ (ਵਿੱਥ, ਚੌੜਾਈ);
}

/* Debug LINE by underlining its keys.  */

ਸਥਿਰ ਖਾਲੀ
debug_ਸਤਰ (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ)
{
  ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ = keylist;

  ਕਰੋ
    debug_ਕੁੰਜੀ (ਸਤਰ, ਕੁੰਜੀ);
  ਜਦੋਂ (ਕੁੰਜੀ && ((ਕੁੰਜੀ = ਕੁੰਜੀ->ਅਗਲਾ) || ! (unique || stable)));
}

/* Return whether sorting options specified for key.  */

ਸਥਿਰ bool
default_ਕੁੰਜੀ_compare (ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ)
{
  ਵਾਪਸ ! (ਕੁੰਜੀ->ignore
            || ਕੁੰਜੀ->translate
            || ਕੁੰਜੀ->skipsblanks
            || ਕੁੰਜੀ->skipeblanks
            || ਕੁੰਜੀ_numeric (ਕੁੰਜੀ)
            || ਕੁੰਜੀ->month
            || ਕੁੰਜੀ->ਸੰਸਕਰਣ_3
            || ਕੁੰਜੀ->random
            /* || key->reverse */
           );
}

/* Convert a key to the short options used to specify it.  */

ਸਥਿਰ ਖਾਲੀ
ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_opts (ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ, ਅੱਖਰ *opts)
{
  ਜੇ (ਕੁੰਜੀ->skipsblanks || ਕੁੰਜੀ->skipeblanks)
    *opts++ = 'b';/* either disables global -b  */
  ਜੇ (ਕੁੰਜੀ->ignore == nondictionary)
    *opts++ = 'd';
  ਜੇ (ਕੁੰਜੀ->translate)
    *opts++ = 'f';
  ਜੇ (ਕੁੰਜੀ->general_numeric)
    *opts++ = 'g';
  ਜੇ (ਕੁੰਜੀ->human_numeric)
    *opts++ = 'h';
  ਜੇ (ਕੁੰਜੀ->ignore == nonprinting)
    *opts++ = 'i';
  ਜੇ (ਕੁੰਜੀ->month)
    *opts++ = 'M';
  ਜੇ (ਕੁੰਜੀ->numeric)
    *opts++ = 'n';
  ਜੇ (ਕੁੰਜੀ->random)
    *opts++ = 'R';
  ਜੇ (ਕੁੰਜੀ->ਉਲਟ)
    *opts++ = 'r';
  ਜੇ (ਕੁੰਜੀ->ਸੰਸਕਰਣ_3)
    *opts++ = 'V';
  *opts = '\0';
}

/* Output data independent key warnings to stderr.  */

ਸਥਿਰ ਖਾਲੀ
ਕੁੰਜੀ_warnings (ਬਣਤਰ keyfield ਅਚਲ *gkey, bool gkey_only)
{
  ਬਣਤਰ keyfield ugkey = *gkey;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ keynum = 1;
  bool basic_numeric_ਖੇਤਰ = false;
  bool general_numeric_ਖੇਤਰ = false;
  bool basic_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ = false;
  bool general_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ = false;

  ਲਈ (ਬਣਤਰ keyfield *ਕੁੰਜੀ = keylist; ਕੁੰਜੀ; ਕੁੰਜੀ = ਕੁੰਜੀ->ਅਗਲਾ, keynum++)
    {
      ਜੇ (ਕੁੰਜੀ_numeric (ਕੁੰਜੀ))
        {
          ਜੇ (ਕੁੰਜੀ->general_numeric)
            general_numeric_ਖੇਤਰ = true;
          ਨਹੀਂਤਾਂ
            basic_numeric_ਖੇਤਰ = true;
        }

      ਜੇ (ਕੁੰਜੀ->traditional_ਵਰਤਿਆ)
        {
          size_t sword = ਕੁੰਜੀ->sword;
          size_t eword = ਕੁੰਜੀ->eword;
          ਅੱਖਰ tmp[INT_BUFSIZE_BOUND (uintmax_t)];
          /* obsolescent syntax +A.x -B.y is equivalent to:
               -k A+1.x+1,B.y   (when y = 0)
               -k A+1.x+1,B+1.y (when y > 0)  */
          ਅੱਖਰ obuf[INT_BUFSIZE_BOUND (sword) * 2 + 4]; /* +# -#  */
          ਅੱਖਰ nbuf[INT_BUFSIZE_BOUND (sword) * 2 + 5]; /* -k #,#  */
          ਅੱਖਰ *po = obuf;
          ਅੱਖਰ *pn = nbuf;

          ਜੇ (sword == SIZE_MAX)
            sword++;

          po = stpcpy (stpcpy (po, "+"), umaxtostr (sword, tmp));
          pn = stpcpy (stpcpy (pn, "-k "), umaxtostr (sword + 1, tmp));
          ਜੇ (ਕੁੰਜੀ->eword != SIZE_MAX)
            {
              stpcpy (stpcpy (po, " -"), umaxtostr (eword + 1, tmp));
              stpcpy (stpcpy (pn, ","),
                      umaxtostr (eword + 1
                                 + (ਕੁੰਜੀ->echar == SIZE_MAX), tmp));
            }
          error (0, 0, _("obsolescent key %s used; consider %s instead"),
                 quote_n (0, obuf), quote_n (1, nbuf));
        }

      /* Warn about field specs that will never match.  */
      bool zero_ਚੌੜਾਈ = ਕੁੰਜੀ->sword != SIZE_MAX && ਕੁੰਜੀ->eword < ਕੁੰਜੀ->sword;
      ਜੇ (zero_ਚੌੜਾਈ)
        error (0, 0, _("key %lu has zero width and will be ignored"), keynum);

      /* Warn about significant leading blanks.  */
      bool implicit_skip = ਕੁੰਜੀ_numeric (ਕੁੰਜੀ) || ਕੁੰਜੀ->month;
      bool ਸਤਰ_ਵਿੱਥ = ਕੁੰਜੀ->eword == 0 && ਕੁੰਜੀ->echar != 0; /* -k1.x,1.y  */
      ਜੇ (!zero_ਚੌੜਾਈ && !gkey_only && tab == TAB_DEFAULT && !ਸਤਰ_ਵਿੱਥ
          && ((!ਕੁੰਜੀ->skipsblanks && !implicit_skip)
              || (!ਕੁੰਜੀ->skipsblanks && ਕੁੰਜੀ->schar)
              || (!ਕੁੰਜੀ->skipeblanks && ਕੁੰਜੀ->echar)))
        error (0, 0, _("leading blanks are significant in key %lu; "
                       "consider also specifying 'b'"), keynum);

      /* Warn about numeric comparisons spanning fields,
         as field delimiters could be interpreted as part
         of the number (maybe only in other locales).  */
      ਜੇ (!gkey_only && ਕੁੰਜੀ_numeric (ਕੁੰਜੀ))
        {
          size_t sword = ਕੁੰਜੀ->sword + 1;
          size_t eword = ਕੁੰਜੀ->eword + 1;
          ਜੇ (!sword)
            sword++;
          ਜੇ (!eword || sword < eword)
            {
              error (0, 0, _("key %lu is numeric and spans multiple fields"),
                     keynum);
              ਜੇ (ਕੁੰਜੀ->general_numeric)
                general_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ = true;
              ਨਹੀਂਤਾਂ
                basic_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ = true;
            }
        }

      /* Flag global options not copied or specified in any key.  */
      ਜੇ (ugkey.ignore && (ugkey.ignore == ਕੁੰਜੀ->ignore))
        ugkey.ignore = ਨਿਰਾ;
      ਜੇ (ugkey.translate && (ugkey.translate == ਕੁੰਜੀ->translate))
        ugkey.translate = ਨਿਰਾ;
      ugkey.skipsblanks &= !ਕੁੰਜੀ->skipsblanks;
      ugkey.skipeblanks &= !ਕੁੰਜੀ->skipeblanks;
      ugkey.month &= !ਕੁੰਜੀ->month;
      ugkey.numeric &= !ਕੁੰਜੀ->numeric;
      ugkey.general_numeric &= !ਕੁੰਜੀ->general_numeric;
      ugkey.human_numeric &= !ਕੁੰਜੀ->human_numeric;
      ugkey.random &= !ਕੁੰਜੀ->random;
      ugkey.ਸੰਸਕਰਣ_3 &= !ਕੁੰਜੀ->ਸੰਸਕਰਣ_3;
      ugkey.ਉਲਟ &= !ਕੁੰਜੀ->ਉਲਟ;
    }

  /* Explicitly warn if field delimiters in this locale
     don't constrain numbers.  */
  bool ਸੰਖਿਆ_locale_warned = false;
  ਜੇ (basic_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ)
    {
      ਜੇ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (ਮੰਜ਼ਿਲ_uchar (thousands_sep)))
          : tab == thousands_sep)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "group separator in numbers"),
                 quote (((ਅੱਖਰ []) {thousands_sep, 0})));
          ਸੰਖਿਆ_locale_warned = true;
        }
    }
  ਜੇ (basic_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ || general_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ)
    {
      ਜੇ (tab == TAB_DEFAULT
          ? thousands_sep != NON_CHAR && (isblank (ਮੰਜ਼ਿਲ_uchar (decimal_ਬਿੰਦੂ)))
          : tab == decimal_ਬਿੰਦੂ)
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "decimal point in numbers"),
                 quote (((ਅੱਖਰ []) {decimal_ਬਿੰਦੂ, 0})));
          ਸੰਖਿਆ_locale_warned = true;
        }
      ਨਹੀਂਤਾਂ ਜੇ (tab == '-')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "minus sign in numbers"),
                 quote (((ਅੱਖਰ []) {tab, 0})));
        }
      ਨਹੀਂਤਾਂ ਜੇ (general_numeric_ਖੇਤਰ_ਵਿਸਤਾਰ && tab == '+')
        {
          error (0, 0,
                 _("field separator %s is treated as a "
                   "plus sign in numbers"),
                 quote (((ਅੱਖਰ []) {tab, 0})));
        }
    }

  /* Explicitly indicate the decimal point used in this locale,
     as it suggests that robust scripts need to consider
     setting the locale when comparing numbers.  */
  ਜੇ ((basic_numeric_ਖੇਤਰ || general_numeric_ਖੇਤਰ) && ! ਸੰਖਿਆ_locale_warned)
    {
      error (0, 0,
             _("numbers use %s as a decimal point in this locale"),
             quote (((ਅੱਖਰ []) {decimal_ਬਿੰਦੂ, 0})));

    }

  ਜੇ (basic_numeric_ਖੇਤਰ && thousands_sep_ignored)
    {
      error (0, 0,
             _("the multi-byte number group separator "
               "in this locale is not supported"));
    }

  /* Warn about ignored global options flagged above.
     This clears all flags if UGKEY is the only one in the list.  */
  ਜੇ (!default_ਕੁੰਜੀ_compare (&ugkey)
      || (ugkey.ਉਲਟ && (stable || unique) && keylist))
    {
      bool ugkey_ਉਲਟ = ugkey.ਉਲਟ;
      ਜੇ (!(stable || unique))
        ugkey.ਉਲਟ = false;
      /* The following is too big, but guaranteed to be "big enough".  */
      ਅੱਖਰ opts[ਆਕਾਰ short_options];
      ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_opts (&ugkey, opts);
      error (0, 0,
             ngettext ("option '-%s' is ignored",
                       "options '-%s' are ignored",
                       select_plural (ਲੜੀ_ਲੰਬਾਈ (opts))), opts);
      ugkey.ਉਲਟ = ugkey_ਉਲਟ;
    }
  ਜੇ (ugkey.ਉਲਟ && !(stable || unique) && keylist)
    error (0, 0, _("option '-r' only applies to last-resort comparison"));
}

/* Return either the sense of DIFF or its reverse, depending on REVERSED.
   If REVERSED, do not simply negate DIFF as that can mishandle INT_MIN.  */

ਸਥਿਰ ਪੂਰਨ
diff_reversed (ਪੂਰਨ diff, bool reversed)
{
  ਵਾਪਸ reversed ? _GL_CMP (0, diff) : diff;
}

/* Compare two lines A and B trying every key in sequence until there
   are no more keys or a difference is found. */

ਸਥਿਰ ਪੂਰਨ
keycompare (ਬਣਤਰ ਸਤਰ ਅਚਲ *a, ਬਣਤਰ ਸਤਰ ਅਚਲ *b)
{
  ਬਣਤਰ keyfield *ਕੁੰਜੀ = keylist;

  /* For the first iteration only, the key positions have been
     precomputed for us. */
  ਅੱਖਰ *texta = a->keybeg;
  ਅੱਖਰ *textb = b->keybeg;
  ਅੱਖਰ *lima = a->keylim;
  ਅੱਖਰ *limb = b->keylim;

  ਪੂਰਨ diff;

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ ਅਚਲ *translate = ਕੁੰਜੀ->translate;
      bool ਅਚਲ *ignore = ਕੁੰਜੀ->ignore;

      /* Treat field ends before field starts as empty fields.  */
      lima = MAX (texta, lima);
      limb = MAX (textb, limb);

      /* Find the lengths. */
      size_t lena = lima - texta;
      size_t lenb = limb - textb;

      ਜੇ (hard_LC_COLLATE || ਕੁੰਜੀ_numeric (ਕੁੰਜੀ)
          || ਕੁੰਜੀ->month || ਕੁੰਜੀ->random || ਕੁੰਜੀ->ਸੰਸਕਰਣ_3)
        {
          /* Ordinarily use the keys in-place, temporarily null-terminated.  */
          ਅੱਖਰ *ta = texta;
          ਅੱਖਰ *tb = textb;
          size_t tlena = lena;
          size_t tlenb = lenb;
          ਅੱਖਰ enda = ta[tlena];
          ਅੱਖਰ endb = tb[tlenb];

          ਖਾਲੀ *allocated = ਨਿਰਾ;
          ਅੱਖਰ stackbuf[4000];

          ਜੇ (ignore || translate)
            {
              /* Compute with copies of the keys, which are the result of
                 translating or ignoring characters, and which need their
                 own storage.  */

              size_t i;

              /* Allocate space for copies.  */
              size_t ਮਾਪ = lena + 1 + lenb + 1;
              ਜੇ (ਮਾਪ <= ਆਕਾਰ stackbuf)
                ta = stackbuf;
              ਨਹੀਂਤਾਂ
                ta = allocated = xmalloc (ਮਾਪ);
              tb = ta + lena + 1;

              /* Put into each copy a version of the key in which the
                 requested characters are ignored or translated.  */
              ਲਈ (tlena = i = 0; i < lena; i++)
                ਜੇ (! (ignore && ignore[ਮੰਜ਼ਿਲ_uchar (texta[i])]))
                  ta[tlena++] = (translate
                                 ? translate[ਮੰਜ਼ਿਲ_uchar (texta[i])]
                                 : texta[i]);

              ਲਈ (tlenb = i = 0; i < lenb; i++)
                ਜੇ (! (ignore && ignore[ਮੰਜ਼ਿਲ_uchar (textb[i])]))
                  tb[tlenb++] = (translate
                                 ? translate[ਮੰਜ਼ਿਲ_uchar (textb[i])]
                                 : textb[i]);
            }

          ta[tlena] = '\0';
          tb[tlenb] = '\0';

          ਜੇ (ਕੁੰਜੀ->numeric)
            diff = numcompare (ta, tb);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->general_numeric)
            diff = general_numcompare (ta, tb);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->human_numeric)
            diff = human_numcompare (ta, tb);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->month)
            diff = getmonth (ta, ਨਿਰਾ) - getmonth (tb, ਨਿਰਾ);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->random)
            diff = compare_random (ta, tlena, tb, tlenb);
          ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->ਸੰਸਕਰਣ_3)
            diff = filenvercmp (ta, tlena, tb, tlenb);
          ਨਹੀਂਤਾਂ
            {
              /* Locale-dependent string sorting.  This is slower than
                 C-locale sorting, which is implemented below.  */
              ਜੇ (tlena == 0)
                diff = - NONZERO (tlenb);
              ਨਹੀਂਤਾਂ ਜੇ (tlenb == 0)
                diff = 1;
              ਨਹੀਂਤਾਂ
                diff = xmemcoll0 (ta, tlena + 1, tb, tlenb + 1);
            }

          ta[tlena] = enda;
          tb[tlenb] = endb;

          ਮੁਕਤ (allocated);
        }
      ਨਹੀਂਤਾਂ ਜੇ (ignore)
        {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CMP_WITH_IGNORE(A, B)						\
  ਕਰੋ									\
    {									\
          ਜਦੋਂ (true)							\
            {								\
              ਜਦੋਂ (texta < lima && ignore[ਮੰਜ਼ਿਲ_uchar (*texta)])		\
                ++texta;						\
              ਜਦੋਂ (textb < limb && ignore[ਮੰਜ਼ਿਲ_uchar (*textb)])		\
                ++textb;						\
              ਜੇ (! (texta < lima && textb < limb))			\
                {							\
                  diff = (texta < lima) - (textb < limb);		\
                  ਰੋਕੋ;						\
                }							\
              diff = ਮੰਜ਼ਿਲ_uchar (A) - ਮੰਜ਼ਿਲ_uchar (B);			\
              ਜੇ (diff)							\
                ਰੋਕੋ;							\
              ++texta;							\
              ++textb;							\
            }								\
                                                                        \
    }									\
  ਜਦੋਂ (0)

          ਜੇ (translate)
            CMP_WITH_IGNORE (translate[ਮੰਜ਼ਿਲ_uchar (*texta)],
                             translate[ਮੰਜ਼ਿਲ_uchar (*textb)]);
          ਨਹੀਂਤਾਂ
            CMP_WITH_IGNORE (*texta, *textb);
        }
      ਨਹੀਂਤਾਂ
        {
          size_t lenmin = MIN (lena, lenb);
          ਜੇ (lenmin == 0)
            diff = 0;
          ਨਹੀਂਤਾਂ ਜੇ (translate)
            {
              size_t i = 0;
              ਕਰੋ
                {
                  diff = (ਮੰਜ਼ਿਲ_uchar (translate[ਮੰਜ਼ਿਲ_uchar (texta[i])])
                          - ਮੰਜ਼ਿਲ_uchar (translate[ਮੰਜ਼ਿਲ_uchar (textb[i])]));
                  ਜੇ (diff)
                    ਰੋਕੋ;
                  i++;
                }
              ਜਦੋਂ (i < lenmin);
            }
          ਨਹੀਂਤਾਂ
            diff = memcmp (texta, textb, lenmin);

          ਜੇ (! diff)
            diff = _GL_CMP (lena, lenb);
        }

      ਜੇ (diff)
        ਰੋਕੋ;

      ਕੁੰਜੀ = ਕੁੰਜੀ->ਅਗਲਾ;
      ਜੇ (! ਕੁੰਜੀ)
        ਵਾਪਸ 0;

      /* Find the beginning and limit of the next field.  */
      ਜੇ (ਕੁੰਜੀ->eword != SIZE_MAX)
        lima = limfield (a, ਕੁੰਜੀ), limb = limfield (b, ਕੁੰਜੀ);
      ਨਹੀਂਤਾਂ
        lima = a->ਪਾਠ + a->ਲੰਮਾਈ - 1, limb = b->ਪਾਠ + b->ਲੰਮਾਈ - 1;

      ਜੇ (ਕੁੰਜੀ->sword != SIZE_MAX)
        texta = begfield (a, ਕੁੰਜੀ), textb = begfield (b, ਕੁੰਜੀ);
      ਨਹੀਂਤਾਂ
        {
          texta = a->ਪਾਠ, textb = b->ਪਾਠ;
          ਜੇ (ਕੁੰਜੀ->skipsblanks)
            {
              ਜਦੋਂ (texta < lima && blanks[ਮੰਜ਼ਿਲ_uchar (*texta)])
                ++texta;
              ਜਦੋਂ (textb < limb && blanks[ਮੰਜ਼ਿਲ_uchar (*textb)])
                ++textb;
            }
        }
    }

  ਵਾਪਸ diff_reversed (diff, ਕੁੰਜੀ->ਉਲਟ);
}

/* Compare two lines A and B, returning negative, zero, or positive
   depending on whether A compares less than, equal to, or greater than B. */

ਸਥਿਰ ਪੂਰਨ
compare (ਬਣਤਰ ਸਤਰ ਅਚਲ *a, ਬਣਤਰ ਸਤਰ ਅਚਲ *b)
{
  ਪੂਰਨ diff;
  size_t alen, blen;

  /* First try to compare on the specified keys (if any).
     The only two cases with no key at all are unadorned sort,
     and unadorned sort -r. */
  ਜੇ (keylist)
    {
      diff = keycompare (a, b);
      ਜੇ (diff || unique || stable)
        ਵਾਪਸ diff;
    }

  /* If the keys all compare equal (or no keys were specified)
     fall through to the default comparison.  */
  alen = a->ਲੰਮਾਈ - 1, blen = b->ਲੰਮਾਈ - 1;

  ਜੇ (alen == 0)
    diff = - NONZERO (blen);
  ਨਹੀਂਤਾਂ ਜੇ (blen == 0)
    diff = 1;
  ਨਹੀਂਤਾਂ ਜੇ (hard_LC_COLLATE)
    {
      /* xmemcoll0 is a performance enhancement as
         it will not unconditionally write '\0' after the
         passed in buffers, which was seen to give around
         a 3% increase in performance for short lines.  */
      diff = xmemcoll0 (a->ਪਾਠ, alen + 1, b->ਪਾਠ, blen + 1);
    }
  ਨਹੀਂਤਾਂ
    {
      diff = memcmp (a->ਪਾਠ, b->ਪਾਠ, MIN (alen, blen));
      ਜੇ (!diff)
        diff = _GL_CMP (alen, blen);
    }

  ਵਾਪਸ diff_reversed (diff, ਉਲਟ);
}

/* Write LINE to output stream FP; the output file's name is
   OUTPUT_FILE if OUTPUT_FILE is non-null, and is the standard output
   otherwise.  If debugging is enabled and FP is standard output,
   append some debugging information.  */

ਸਥਿਰ ਖਾਲੀ
write_ਸਤਰ (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, FILE *fp, ਅੱਖਰ ਅਚਲ *output_file)
{
  ਅੱਖਰ *buf = ਸਤਰ->ਪਾਠ;
  size_t n_ਬਾਈਟਾਂ = ਸਤਰ->ਲੰਮਾਈ;
  ਅੱਖਰ *ebuf = buf + n_ਬਾਈਟਾਂ;

  ਜੇ (!output_file && debug)
    {
      /* Convert TAB to '>' and EOL to \n, and then output debugging info.  */
      ਅੱਖਰ ਅਚਲ *c = buf;

      ਜਦੋਂ (c < ebuf)
        {
          ਅੱਖਰ wc = *c++;
          ਜੇ (wc == '\t')
            wc = '>';
          ਨਹੀਂਤਾਂ ਜੇ (c == ebuf)
            wc = '\n';
          ਜੇ (fputc (wc, fp) == EOF)
            ਛਾਂਟੋ_die (_("write failed"), output_file);
        }

      debug_ਸਤਰ (ਸਤਰ);
    }
  ਨਹੀਂਤਾਂ
    {
      ebuf[-1] = eolchar;
      ਜੇ (fwrite (buf, 1, n_ਬਾਈਟਾਂ, fp) != n_ਬਾਈਟਾਂ)
        ਛਾਂਟੋ_die (_("write failed"), output_file);
      ebuf[-1] = '\0';
    }
}

/* Check that the lines read from FILE_NAME come in order.  Return
   true if they are in order.  If CHECKONLY == 'c', also print a
   diagnostic (FILE_NAME, line number, contents of line) to stderr if
   they are not in order.  */

ਸਥਿਰ bool
check (ਅੱਖਰ ਅਚਲ *file_name, ਅੱਖਰ checkonly)
{
  FILE *fp = xfopen (file_name, "r");
  ਬਣਤਰ ਬਫਰ buf;		/* Input buffer. */
  ਬਣਤਰ ਸਤਰ ਆਰਜ਼ੀ;		/* Copy of previous line. */
  size_t alloc = 0;
  uintmax_t ਸਤਰ_ਸੰਖਿਆ = 0;
  ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ = keylist;
  bool nonunique = ! unique;
  bool ordered = true;

  initbuf (&buf, ਆਕਾਰ (ਬਣਤਰ ਸਤਰ),
           MAX (ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ, ਛਾਂਟੋ_ਮਾਪ));
  ਆਰਜ਼ੀ.ਪਾਠ = ਨਿਰਾ;

  ਜਦੋਂ (fillbuf (&buf, fp, file_name, ਨਿਰਾ))
    {
      ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ = ਬਫਰ_linelim (&buf);
      ਬਣਤਰ ਸਤਰ ਅਚਲ *linebase = ਸਤਰ - buf.nlines;

      /* Make sure the line saved from the old buffer contents is
         less than or equal to the first line of the new buffer. */
      ਜੇ (alloc && nonunique <= compare (&ਆਰਜ਼ੀ, ਸਤਰ - 1))
        {
        ਮਿਲਿਆ_disorder:
          {
            ਜੇ (checkonly == 'c')
              {
                ਬਣਤਰ ਸਤਰ ਅਚਲ *disorder_ਸਤਰ = ਸਤਰ - 1;
                uintmax_t disorder_ਸਤਰ_ਸੰਖਿਆ =
                  ਬਫਰ_linelim (&buf) - disorder_ਸਤਰ + ਸਤਰ_ਸੰਖਿਆ;
                fprintf (stderr, _("%s: %s:%ju: disorder: "),
                         ਪ੍ਰੋਗਰਾਮ_name, file_name, disorder_ਸਤਰ_ਸੰਖਿਆ);
                write_ਸਤਰ (disorder_ਸਤਰ, stderr, _("standard error"));
              }

            ordered = false;
            ਰੋਕੋ;
          }
        }

      /* Compare each line in the buffer with its successor.  */
      ਜਦੋਂ (linebase < --ਸਤਰ)
        ਜੇ (nonunique <= compare (ਸਤਰ, ਸਤਰ - 1))
          ਜਾਓ ਮਿਲਿਆ_disorder;

      ਸਤਰ_ਸੰਖਿਆ += buf.nlines;

      /* Save the last line of the buffer.  */
      ਜੇ (alloc < ਸਤਰ->ਲੰਮਾਈ)
        {
          ਕਰੋ
            {
              alloc *= 2;
              ਜੇ (! alloc)
                {
                  alloc = ਸਤਰ->ਲੰਮਾਈ;
                  ਰੋਕੋ;
                }
            }
          ਜਦੋਂ (alloc < ਸਤਰ->ਲੰਮਾਈ);

          ਮੁਕਤ (ਆਰਜ਼ੀ.ਪਾਠ);
          ਆਰਜ਼ੀ.ਪਾਠ = xmalloc (alloc);
        }
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਆਰਜ਼ੀ.ਪਾਠ, ਸਤਰ->ਪਾਠ, ਸਤਰ->ਲੰਮਾਈ);
      ਆਰਜ਼ੀ.ਲੰਮਾਈ = ਸਤਰ->ਲੰਮਾਈ;
      ਜੇ (ਕੁੰਜੀ)
        {
          ਆਰਜ਼ੀ.keybeg = ਆਰਜ਼ੀ.ਪਾਠ + (ਸਤਰ->keybeg - ਸਤਰ->ਪਾਠ);
          ਆਰਜ਼ੀ.keylim = ਆਰਜ਼ੀ.ਪਾਠ + (ਸਤਰ->keylim - ਸਤਰ->ਪਾਠ);
        }
    }

  xfclose (fp, file_name);
  ਮੁਕਤ (buf.buf);
  ਮੁਕਤ (ਆਰਜ਼ੀ.ਪਾਠ);
  ਵਾਪਸ ordered;
}

/* Open FILES (there are NFILES of them) and store the resulting array
   of stream pointers into (*PFPS).  Allocate the array.  Return the
   number of successfully opened files, setting errno if this value is
   less than NFILES.  */

ਸਥਿਰ size_t
open_ਨਿਵੇਸ਼_files (ਬਣਤਰ sortfile *files, size_t nfiles, FILE ***pfps)
{
  FILE **fps = *pfps = xnmalloc (nfiles, ਆਕਾਰ *fps);
  ਪੂਰਨ i;

  /* Open as many input files as we can.  */
  ਲਈ (i = 0; i < nfiles; i++)
    {
      fps[i] = (files[i].ਆਰਜ਼ੀ && files[i].ਆਰਜ਼ੀ->state != UNCOMPRESSED
                ? open_ਆਰਜ਼ੀ (files[i].ਆਰਜ਼ੀ)
                : ਧਾਰਾ_open (files[i].name, "r"));
      ਜੇ (!fps[i])
        ਰੋਕੋ;
    }

  ਵਾਪਸ i;
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   FPS is the vector of open stream corresponding to the files.
   Close input and output streams before returning.
   OUTPUT_FILE gives the name of the output file.  If it is null,
   the output file is standard output.  */

ਸਥਿਰ ਖਾਲੀ
mergefps (ਬਣਤਰ sortfile *files, size_t ntemps, size_t nfiles,
          FILE *ofp, ਅੱਖਰ ਅਚਲ *output_file, FILE **fps)
{
  ਬਣਤਰ ਬਫਰ *ਬਫਰ = xnmalloc (nfiles, ਆਕਾਰ *ਬਫਰ);
                                /* Input buffers for each file. */
  ਬਣਤਰ ਸਤਰ saved;		/* Saved line storage for unique check. */
  ਬਣਤਰ ਸਤਰ ਅਚਲ *savedline = ਨਿਰਾ;
                                /* &saved if there is a saved line. */
  size_t savealloc = 0;		/* Size allocated for the saved line. */
  ਬਣਤਰ ਸਤਰ ਅਚਲ **cur = xnmalloc (nfiles, ਆਕਾਰ *cur);
                                /* Current line in each line table. */
  ਬਣਤਰ ਸਤਰ ਅਚਲ **ਅਧਾਰ_2 = xnmalloc (nfiles, ਆਕਾਰ *ਅਧਾਰ_2);
                                /* Base of each line table.  */
  size_t *ord = xnmalloc (nfiles, ਆਕਾਰ *ord);
                                /* Table representing a permutation of fps,
                                   such that cur[ord[0]] is the smallest line
                                   and will be next output. */
  size_t t;
  ਬਣਤਰ keyfield ਅਚਲ *ਕੁੰਜੀ = keylist;
  saved.ਪਾਠ = ਨਿਰਾ;

  /* Read initial lines from each input file. */
  ਲਈ (size_t i = 0; i < nfiles; )
    {
      initbuf (&ਬਫਰ[i], ਆਕਾਰ (ਬਣਤਰ ਸਤਰ),
               MAX (ਮਿਲਾਓ_ਬਫਰ_ਮਾਪ, ਛਾਂਟੋ_ਮਾਪ / nfiles));
      ਜੇ (fillbuf (&ਬਫਰ[i], fps[i], files[i].name, ਨਿਰਾ))
        {
          ਬਣਤਰ ਸਤਰ ਅਚਲ *linelim = ਬਫਰ_linelim (&ਬਫਰ[i]);
          cur[i] = linelim - 1;
          ਅਧਾਰ_2[i] = linelim - ਬਫਰ[i].nlines;
          i++;
        }
      ਨਹੀਂਤਾਂ
        {
          /* fps[i] is empty; eliminate it from future consideration.  */
          xfclose (fps[i], files[i].name);
          ਜੇ (i < ntemps)
            {
              ntemps--;
              zaptemp (files[i].name);
            }
          ਮੁਕਤ (ਬਫਰ[i].buf);
          --nfiles;
          ਲਈ (size_t j = i; j < nfiles; ++j)
            {
              files[j] = files[j + 1];
              fps[j] = fps[j + 1];
            }
        }
    }

  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  ਲਈ (size_t i = 0; i < nfiles; ++i)
    ord[i] = i;
  ਲਈ (size_t i = 1; i < nfiles; ++i)
    ਜੇ (0 < compare (cur[ord[i - 1]], cur[ord[i]]))
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;

  /* Repeatedly output the smallest line until no input remains. */
  ਜਦੋਂ (nfiles)
    {
      ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਭ_ਤੋਂ_ਛੋਟਾ = cur[ord[0]];

      /* If uniquified output is turned on, output only the first of
         an identical series of lines. */
      ਜੇ (unique)
        {
          ਜੇ (savedline && compare (savedline, ਸਭ_ਤੋਂ_ਛੋਟਾ))
            {
              savedline = ਨਿਰਾ;
              write_ਸਤਰ (&saved, ofp, output_file);
            }
          ਜੇ (!savedline)
            {
              savedline = &saved;
              ਜੇ (savealloc < ਸਭ_ਤੋਂ_ਛੋਟਾ->ਲੰਮਾਈ)
                {
                  ਕਰੋ
                    ਜੇ (! savealloc)
                      {
                        savealloc = ਸਭ_ਤੋਂ_ਛੋਟਾ->ਲੰਮਾਈ;
                        ਰੋਕੋ;
                      }
                  ਜਦੋਂ ((savealloc *= 2) < ਸਭ_ਤੋਂ_ਛੋਟਾ->ਲੰਮਾਈ);

                  ਮੁਕਤ (saved.ਪਾਠ);
                  saved.ਪਾਠ = xmalloc (savealloc);
                }
              saved.ਲੰਮਾਈ = ਸਭ_ਤੋਂ_ਛੋਟਾ->ਲੰਮਾਈ;
              ਸਮ੍ਰਿਤੀ_ਨਕਲ (saved.ਪਾਠ, ਸਭ_ਤੋਂ_ਛੋਟਾ->ਪਾਠ, saved.ਲੰਮਾਈ);
              ਜੇ (ਕੁੰਜੀ)
                {
                  saved.keybeg =
                    saved.ਪਾਠ + (ਸਭ_ਤੋਂ_ਛੋਟਾ->keybeg - ਸਭ_ਤੋਂ_ਛੋਟਾ->ਪਾਠ);
                  saved.keylim =
                    saved.ਪਾਠ + (ਸਭ_ਤੋਂ_ਛੋਟਾ->keylim - ਸਭ_ਤੋਂ_ਛੋਟਾ->ਪਾਠ);
                }
            }
        }
      ਨਹੀਂਤਾਂ
        write_ਸਤਰ (ਸਭ_ਤੋਂ_ਛੋਟਾ, ofp, output_file);

      /* Check if we need to read more lines into memory. */
      ਜੇ (ਅਧਾਰ_2[ord[0]] < ਸਭ_ਤੋਂ_ਛੋਟਾ)
        cur[ord[0]] = ਸਭ_ਤੋਂ_ਛੋਟਾ - 1;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (fillbuf (&ਬਫਰ[ord[0]], fps[ord[0]], files[ord[0]].name,
                       ਨਿਰਾ))
            {
              ਬਣਤਰ ਸਤਰ ਅਚਲ *linelim = ਬਫਰ_linelim (&ਬਫਰ[ord[0]]);
              cur[ord[0]] = linelim - 1;
              ਅਧਾਰ_2[ord[0]] = linelim - ਬਫਰ[ord[0]].nlines;
            }
          ਨਹੀਂਤਾਂ
            {
              /* We reached EOF on fps[ord[0]].  */
              ਲਈ (size_t i = 1; i < nfiles; ++i)
                ਜੇ (ord[i] > ord[0])
                  --ord[i];
              --nfiles;
              xfclose (fps[ord[0]], files[ord[0]].name);
              ਜੇ (ord[0] < ntemps)
                {
                  ntemps--;
                  zaptemp (files[ord[0]].name);
                }
              ਮੁਕਤ (ਬਫਰ[ord[0]].buf);
              ਲਈ (size_t i = ord[0]; i < nfiles; ++i)
                {
                  fps[i] = fps[i + 1];
                  files[i] = files[i + 1];
                  ਬਫਰ[i] = ਬਫਰ[i + 1];
                  cur[i] = cur[i + 1];
                  ਅਧਾਰ_2[i] = ਅਧਾਰ_2[i + 1];
                }
              ਲਈ (size_t i = 0; i < nfiles; ++i)
                ord[i] = ord[i + 1];
              ਜਾਰੀ;
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
        size_t ਗਿਣਤੀ_ਦਾ_smaller_ਸਤਰਾਂ;

        ਜਦੋਂ (lo < hi)
          {
            ਪੂਰਨ cmp = compare (cur[ord0], cur[ord[probe]]);
            ਜੇ (cmp < 0 || (cmp == 0 && ord0 < ord[probe]))
              hi = probe;
            ਨਹੀਂਤਾਂ
              lo = probe + 1;
            probe = (lo + hi) / 2;
          }

        ਗਿਣਤੀ_ਦਾ_smaller_ਸਤਰਾਂ = lo - 1;
        ਲਈ (size_t j = 0; j < ਗਿਣਤੀ_ਦਾ_smaller_ਸਤਰਾਂ; j++)
          ord[j] = ord[j + 1];
        ord[ਗਿਣਤੀ_ਦਾ_smaller_ਸਤਰਾਂ] = ord0;
      }
    }

  ਜੇ (unique && savedline)
    {
      write_ਸਤਰ (&saved, ofp, output_file);
      ਮੁਕਤ (saved.ਪਾਠ);
    }

  xfclose (ofp, output_file);
  ਮੁਕਤ (fps);
  ਮੁਕਤ (ਬਫਰ);
  ਮੁਕਤ (ord);
  ਮੁਕਤ (ਅਧਾਰ_2);
  ਮੁਕਤ (cur);
}

/* Merge lines from FILES onto OFP.  NTEMPS is the number of temporary
   files (all of which are at the start of the FILES array), and
   NFILES is the number of files; 0 <= NTEMPS <= NFILES <= NMERGE.
   Close input and output files before returning.
   OUTPUT_FILE gives the name of the output file.

   Return the number of files successfully merged.  This number can be
   less than NFILES if we ran low on file descriptors, but in this
   case it is never less than 2.  */

ਸਥਿਰ size_t
mergefiles (ਬਣਤਰ sortfile *files, size_t ntemps, size_t nfiles,
            FILE *ofp, ਅੱਖਰ ਅਚਲ *output_file)
{
  FILE **fps;
  size_t nopened = open_ਨਿਵੇਸ਼_files (files, nfiles, &fps);
  ਜੇ (nopened < nfiles && nopened < 2)
    ਛਾਂਟੋ_die (_("open failed"), files[nopened].name);
  mergefps (files, ntemps, nopened, ofp, output_file, fps);
  ਵਾਪਸ nopened;
}

/* Merge into T (of size NLINES) the two sorted arrays of lines
   LO (with NLINES / 2 members), and
   T - (NLINES / 2) (with NLINES - NLINES / 2 members).
   T and LO point just past their respective arrays, and the arrays
   are in reverse order.  NLINES must be at least 2.  */

ਸਥਿਰ ਖਾਲੀ
mergelines (ਬਣਤਰ ਸਤਰ *ਸੀਮਿਤ t, size_t nlines,
            ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸੀਮਿਤ lo)
{
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ਬਣਤਰ ਸਤਰ *hi = t - nlo;

  ਜਦੋਂ (true)
    ਜੇ (compare (lo - 1, hi - 1) <= 0)
      {
        *--t = *--lo;
        ਜੇ (! --nlo)
          {
            /* HI must equal T now, and there is no need to copy from
               HI to T. */
            ਵਾਪਸ;
          }
      }
    ਨਹੀਂਤਾਂ
      {
        *--t = *--hi;
        ਜੇ (! --nhi)
          {
            ਕਰੋ
              *--t = *--lo;
            ਜਦੋਂ (--nlo);

            ਵਾਪਸ;
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

ਸਥਿਰ ਖਾਲੀ
sequential_ਛਾਂਟੋ (ਬਣਤਰ ਸਤਰ *ਸੀਮਿਤ ਸਤਰਾਂ, size_t nlines,
                 ਬਣਤਰ ਸਤਰ *ਸੀਮਿਤ ਆਰਜ਼ੀ, bool ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ)
{
  ਜੇ (nlines == 2)
    {
      /* Declare 'swap' as int, not bool, to work around a bug
        <https://lists.gnu.org/r/bug-coreutils/2005-10/msg00086.html>
         in the IBM xlc 6.0.0.0 compiler in 64-bit mode.  */
      ਪੂਰਨ ਅਦਲਾ_ਬਦਲੀ = (0 < compare (&ਸਤਰਾਂ[-1], &ਸਤਰਾਂ[-2]));
      ਜੇ (ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ)
        {
          ਆਰਜ਼ੀ[-1] = ਸਤਰਾਂ[-1 - ਅਦਲਾ_ਬਦਲੀ];
          ਆਰਜ਼ੀ[-2] = ਸਤਰਾਂ[-2 + ਅਦਲਾ_ਬਦਲੀ];
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਅਦਲਾ_ਬਦਲੀ)
        {
          ਆਰਜ਼ੀ[-1] = ਸਤਰਾਂ[-1];
          ਸਤਰਾਂ[-1] = ਸਤਰਾਂ[-2];
          ਸਤਰਾਂ[-2] = ਆਰਜ਼ੀ[-1];
        }
    }
  ਨਹੀਂਤਾਂ
    {
      size_t nlo = nlines / 2;
      size_t nhi = nlines - nlo;
      ਬਣਤਰ ਸਤਰ *lo = ਸਤਰਾਂ;
      ਬਣਤਰ ਸਤਰ *hi = ਸਤਰਾਂ - nlo;

      sequential_ਛਾਂਟੋ (hi, nhi, ਆਰਜ਼ੀ - (ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ ? nlo : 0), ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ);
      ਜੇ (1 < nlo)
        sequential_ਛਾਂਟੋ (lo, nlo, ਆਰਜ਼ੀ, !ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ);
      ਨਹੀਂਤਾਂ ਜੇ (!ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ)
        ਆਰਜ਼ੀ[-1] = lo[-1];

      ਬਣਤਰ ਸਤਰ *dest;
      ਬਣਤਰ ਸਤਰ ਅਚਲ *sorted_lo;
      ਜੇ (ਮੰਜ਼ਿਲ_ਆਰਜ਼ੀ)
        {
          dest = ਆਰਜ਼ੀ;
          sorted_lo = ਸਤਰਾਂ;
        }
      ਨਹੀਂਤਾਂ
        {
          dest = ਸਤਰਾਂ;
          sorted_lo = ਆਰਜ਼ੀ;
        }
      mergelines (dest, nlines, sorted_lo);
    }
}

ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *init_ਗੰਢ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਸੀਮਿਤ,
                                     ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਸੀਮਿਤ,
                                     ਬਣਤਰ ਸਤਰ *, size_t, size_t, bool);


/* Create and return a merge tree for NTHREADS threads, sorting NLINES
   lines, with destination DEST.  */
ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *
ਮਿਲਾਓ_ਰੁੱਖ_init (size_t nthreads, size_t nlines, ਬਣਤਰ ਸਤਰ *dest)
{
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਮਿਲਾਓ_ਰੁੱਖ = xmalloc (2 * ਆਕਾਰ *ਮਿਲਾਓ_ਰੁੱਖ * nthreads);

  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਜੜ੍ਹ = ਮਿਲਾਓ_ਰੁੱਖ;
  ਜੜ੍ਹ->lo = ਜੜ੍ਹ->hi = ਜੜ੍ਹ->ਸਿਰਾ_lo = ਜੜ੍ਹ->ਸਿਰਾ_hi = ਨਿਰਾ;
  ਜੜ੍ਹ->dest = ਨਿਰਾ;
  ਜੜ੍ਹ->nlo = ਜੜ੍ਹ->nhi = nlines;
  ਜੜ੍ਹ->ਮਾਪਾ = ਨਿਰਾ;
  ਜੜ੍ਹ->level = ਮਿਲਾਓ_ਸਿਰਾ;
  ਜੜ੍ਹ->queued = false;
  pthread_mutex_init (&ਜੜ੍ਹ->ਤਾਲਾ, ਨਿਰਾ);

  init_ਗੰਢ (ਜੜ੍ਹ, ਜੜ੍ਹ + 1, dest, nthreads, nlines, false);
  ਵਾਪਸ ਮਿਲਾਓ_ਰੁੱਖ;
}

/* Destroy the merge tree. */
ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_ਰੁੱਖ_destroy (size_t nthreads, ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਮਿਲਾਓ_ਰੁੱਖ)
{
  size_t n_nodes = nthreads * 2;
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ = ਮਿਲਾਓ_ਰੁੱਖ;

  ਜਦੋਂ (n_nodes--)
    {
      pthread_mutex_destroy (&ਗੰਢ->ਤਾਲਾ);
      ਗੰਢ++;
    }

  ਮੁਕਤ (ਮਿਲਾਓ_ਰੁੱਖ);
}

/* Initialize a merge tree node and its descendants.  The node's
   parent is PARENT.  The node and its descendants are taken from the
   array of nodes NODE_POOL.  Their destination starts at DEST; they
   will consume NTHREADS threads.  The total number of sort lines is
   TOTAL_LINES.  IS_LO_CHILD is true if the node is the low child of
   its parent.  */

ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *
init_ਗੰਢ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਸੀਮਿਤ ਮਾਪਾ,
           ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਸੀਮਿਤ ਗੰਢ_pool,
           ਬਣਤਰ ਸਤਰ *dest, size_t nthreads,
           size_t ਕੁੱਲ_ਸਤਰਾਂ, bool is_lo_ਬੱਚਾ)
{
  size_t nlines = (is_lo_ਬੱਚਾ ? ਮਾਪਾ->nlo : ਮਾਪਾ->nhi);
  size_t nlo = nlines / 2;
  size_t nhi = nlines - nlo;
  ਬਣਤਰ ਸਤਰ *lo = dest - ਕੁੱਲ_ਸਤਰਾਂ;
  ਬਣਤਰ ਸਤਰ *hi = lo - nlo;
  ਬਣਤਰ ਸਤਰ **ਮਾਪਾ_ਸਿਰਾ = (is_lo_ਬੱਚਾ ? &ਮਾਪਾ->ਸਿਰਾ_lo : &ਮਾਪਾ->ਸਿਰਾ_hi);

  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ = ਗੰਢ_pool++;
  ਗੰਢ->lo = ਗੰਢ->ਸਿਰਾ_lo = lo;
  ਗੰਢ->hi = ਗੰਢ->ਸਿਰਾ_hi = hi;
  ਗੰਢ->dest = ਮਾਪਾ_ਸਿਰਾ;
  ਗੰਢ->nlo = nlo;
  ਗੰਢ->nhi = nhi;
  ਗੰਢ->ਮਾਪਾ = ਮਾਪਾ;
  ਗੰਢ->level = ਮਾਪਾ->level + 1;
  ਗੰਢ->queued = false;
  pthread_mutex_init (&ਗੰਢ->ਤਾਲਾ, ਨਿਰਾ);

  ਜੇ (nthreads > 1)
    {
      size_t lo_ਧਾਗੇ = nthreads / 2;
      size_t hi_ਧਾਗੇ = nthreads - lo_ਧਾਗੇ;
      ਗੰਢ->lo_ਬੱਚਾ = ਗੰਢ_pool;
      ਗੰਢ_pool = init_ਗੰਢ (ਗੰਢ, ਗੰਢ_pool, lo, lo_ਧਾਗੇ,
                             ਕੁੱਲ_ਸਤਰਾਂ, true);
      ਗੰਢ->hi_ਬੱਚਾ = ਗੰਢ_pool;
      ਗੰਢ_pool = init_ਗੰਢ (ਗੰਢ, ਗੰਢ_pool, hi, hi_ਧਾਗੇ,
                             ਕੁੱਲ_ਸਤਰਾਂ, false);
    }
  ਨਹੀਂਤਾਂ
    {
      ਗੰਢ->lo_ਬੱਚਾ = ਨਿਰਾ;
      ਗੰਢ->hi_ਬੱਚਾ = ਨਿਰਾ;
    }
  ਵਾਪਸ ਗੰਢ_pool;
}


/* Compare two merge nodes A and B for priority.  */

ਸਥਿਰ ਪੂਰਨ
compare_nodes (ਖਾਲੀ ਅਚਲ *a, ਖਾਲੀ ਅਚਲ *b)
{
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ ਅਚਲ *nodea = a;
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ ਅਚਲ *nodeb = b;
  ਜੇ (nodea->level == nodeb->level)
      ਵਾਪਸ (nodea->nlo + nodea->nhi) < (nodeb->nlo + nodeb->nhi);
  ਵਾਪਸ nodea->level < nodeb->level;
}

/* Lock a merge tree NODE.  */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਤਾਲਾ_ਗੰਢ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ)
{
  pthread_mutex_ਤਾਲਾ (&ਗੰਢ->ਤਾਲਾ);
}

/* Unlock a merge tree NODE. */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
unlock_ਗੰਢ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ)
{
  pthread_mutex_unlock (&ਗੰਢ->ਤਾਲਾ);
}

/* Destroy merge QUEUE. */

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_destroy (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ)
{
  ਢੇਰ_free (ਕਤਾਰ->ਤਰਜੀਹ_ਕਤਾਰ);
  pthread_cond_destroy (&ਕਤਾਰ->cond);
  pthread_mutex_destroy (&ਕਤਾਰ->mutex);
}

/* Initialize merge QUEUE, allocating space suitable for a maximum of
   NTHREADS threads.  */

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_init (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ, size_t nthreads)
{
  /* Though it's highly unlikely all nodes are in the heap at the same
     time, the heap should accommodate all of them.  Counting a null
     dummy head for the heap, reserve 2 * NTHREADS nodes.  */
  ਕਤਾਰ->ਤਰਜੀਹ_ਕਤਾਰ = ਢੇਰ_alloc (compare_nodes, 2 * nthreads);
  pthread_mutex_init (&ਕਤਾਰ->mutex, ਨਿਰਾ);
  pthread_cond_init (&ਕਤਾਰ->cond, ਨਿਰਾ);
}

/* Insert NODE into QUEUE.  The caller either holds a lock on NODE, or
   does not need to lock NODE.  */

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਪਾਓ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ, ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ)
{
  pthread_mutex_ਤਾਲਾ (&ਕਤਾਰ->mutex);
  ਢੇਰ_ਪਾਓ (ਕਤਾਰ->ਤਰਜੀਹ_ਕਤਾਰ, ਗੰਢ);
  ਗੰਢ->queued = true;
  pthread_cond_signal (&ਕਤਾਰ->cond);
  pthread_mutex_unlock (&ਕਤਾਰ->mutex);
}

/* Pop the top node off the priority QUEUE, lock the node, return it.  */

ਸਥਿਰ ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *
ਕਤਾਰ_ਕੱਢੋ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ)
{
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ;
  pthread_mutex_ਤਾਲਾ (&ਕਤਾਰ->mutex);
  ਜਦੋਂ (! (ਗੰਢ = ਢੇਰ_remove_ਸਿਖਰ (ਕਤਾਰ->ਤਰਜੀਹ_ਕਤਾਰ)))
    pthread_cond_wait (&ਕਤਾਰ->cond, &ਕਤਾਰ->mutex);
  pthread_mutex_unlock (&ਕਤਾਰ->mutex);
  ਤਾਲਾ_ਗੰਢ (ਗੰਢ);
  ਗੰਢ->queued = false;
  ਵਾਪਸ ਗੰਢ;
}

/* Output LINE to TFP, unless -u is specified and the line compares
   equal to the previous line.  TEMP_OUTPUT is the name of TFP, or
   is null if TFP is standard output.

   This function does not save the line for comparison later, so it is
   appropriate only for internal sort.  */

ਸਥਿਰ ਖਾਲੀ
write_unique (ਬਣਤਰ ਸਤਰ ਅਚਲ *ਸਤਰ, FILE *tfp, ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_output)
{
  ਜੇ (unique)
    {
      ਜੇ (saved_ਸਤਰ.ਪਾਠ && ! compare (ਸਤਰ, &saved_ਸਤਰ))
        ਵਾਪਸ;
      saved_ਸਤਰ = *ਸਤਰ;
    }

  write_ਸਤਰ (ਸਤਰ, tfp, ਆਰਜ਼ੀ_output);
}

/* Merge the lines currently available to a NODE in the binary
   merge tree.  Merge a number of lines appropriate for this merge
   level, assuming TOTAL_LINES is the total number of lines.

   If merging at the top level, send output to TFP.  TEMP_OUTPUT is
   the name of TFP, or is null if TFP is standard output.  */

ਸਥਿਰ ਖਾਲੀ
mergelines_ਗੰਢ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਸੀਮਿਤ ਗੰਢ, size_t ਕੁੱਲ_ਸਤਰਾਂ,
                 FILE *tfp, ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_output)
{
  ਬਣਤਰ ਸਤਰ *lo_orig = ਗੰਢ->lo;
  ਬਣਤਰ ਸਤਰ *hi_orig = ਗੰਢ->hi;
  size_t ਮੰਜ਼ਿਲ_ਮਿਲਾਓ = MAX_ਮਿਲਾਓ (ਕੁੱਲ_ਸਤਰਾਂ, ਗੰਢ->level);
  size_t merged_lo;
  size_t merged_hi;

  ਜੇ (ਗੰਢ->level > ਮਿਲਾਓ_ਜੜ੍ਹ)
    {
      /* Merge to destination buffer. */
      ਬਣਤਰ ਸਤਰ *dest = *ਗੰਢ->dest;
      ਜਦੋਂ (ਗੰਢ->lo != ਗੰਢ->ਸਿਰਾ_lo && ਗੰਢ->hi != ਗੰਢ->ਸਿਰਾ_hi && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
        ਜੇ (compare (ਗੰਢ->lo - 1, ਗੰਢ->hi - 1) <= 0)
          *--dest = *--ਗੰਢ->lo;
        ਨਹੀਂਤਾਂ
          *--dest = *--ਗੰਢ->hi;

      merged_lo = lo_orig - ਗੰਢ->lo;
      merged_hi = hi_orig - ਗੰਢ->hi;

      ਜੇ (ਗੰਢ->nhi == merged_hi)
        ਜਦੋਂ (ਗੰਢ->lo != ਗੰਢ->ਸਿਰਾ_lo && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
          *--dest = *--ਗੰਢ->lo;
      ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->nlo == merged_lo)
        ਜਦੋਂ (ਗੰਢ->hi != ਗੰਢ->ਸਿਰਾ_hi && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
          *--dest = *--ਗੰਢ->hi;
      *ਗੰਢ->dest = dest;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Merge directly to output. */
      ਜਦੋਂ (ਗੰਢ->lo != ਗੰਢ->ਸਿਰਾ_lo && ਗੰਢ->hi != ਗੰਢ->ਸਿਰਾ_hi && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
        {
          ਜੇ (compare (ਗੰਢ->lo - 1, ਗੰਢ->hi - 1) <= 0)
            write_unique (--ਗੰਢ->lo, tfp, ਆਰਜ਼ੀ_output);
          ਨਹੀਂਤਾਂ
            write_unique (--ਗੰਢ->hi, tfp, ਆਰਜ਼ੀ_output);
        }

      merged_lo = lo_orig - ਗੰਢ->lo;
      merged_hi = hi_orig - ਗੰਢ->hi;

      ਜੇ (ਗੰਢ->nhi == merged_hi)
        {
          ਜਦੋਂ (ਗੰਢ->lo != ਗੰਢ->ਸਿਰਾ_lo && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
            write_unique (--ਗੰਢ->lo, tfp, ਆਰਜ਼ੀ_output);
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->nlo == merged_lo)
        {
          ਜਦੋਂ (ਗੰਢ->hi != ਗੰਢ->ਸਿਰਾ_hi && ਮੰਜ਼ਿਲ_ਮਿਲਾਓ--)
            write_unique (--ਗੰਢ->hi, tfp, ਆਰਜ਼ੀ_output);
        }
    }

  /* Update NODE. */
  merged_lo = lo_orig - ਗੰਢ->lo;
  merged_hi = hi_orig - ਗੰਢ->hi;
  ਗੰਢ->nlo -= merged_lo;
  ਗੰਢ->nhi -= merged_hi;
}

/* Into QUEUE, insert NODE if it is not already queued, and if one of
   NODE's children has available lines and the other either has
   available lines or has exhausted its lines.  */

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_check_ਪਾਓ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ, ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ)
{
  ਜੇ (! ਗੰਢ->queued)
    {
      bool lo_avail = (ਗੰਢ->lo - ਗੰਢ->ਸਿਰਾ_lo) != 0;
      bool hi_avail = (ਗੰਢ->hi - ਗੰਢ->ਸਿਰਾ_hi) != 0;
      ਜੇ (lo_avail ? hi_avail || ! ਗੰਢ->nhi : hi_avail && ! ਗੰਢ->nlo)
        ਕਤਾਰ_ਪਾਓ (ਕਤਾਰ, ਗੰਢ);
    }
}

/* Into QUEUE, insert NODE's parent if the parent can now be worked on.  */

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_check_ਪਾਓ_ਮਾਪਾ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ,
                           ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ)
{
  ਜੇ (ਗੰਢ->level > ਮਿਲਾਓ_ਜੜ੍ਹ)
    {
      ਤਾਲਾ_ਗੰਢ (ਗੰਢ->ਮਾਪਾ);
      ਕਤਾਰ_check_ਪਾਓ (ਕਤਾਰ, ਗੰਢ->ਮਾਪਾ);
      unlock_ਗੰਢ (ਗੰਢ->ਮਾਪਾ);
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->nlo + ਗੰਢ->nhi == 0)
    {
      /* If the MERGE_ROOT NODE has finished merging, insert the
         MERGE_END node.  */
      ਕਤਾਰ_ਪਾਓ (ਕਤਾਰ, ਗੰਢ->ਮਾਪਾ);
    }
}

/* Repeatedly pop QUEUE for a node with lines to merge, and merge at least
   some of those lines, until the MERGE_END node is popped.
   TOTAL_LINES is the total number of lines.  If merging at the top
   level, send output to TFP.  TEMP_OUTPUT is the name of TFP, or is
   null if TFP is standard output.  */

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_loop (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ,
            size_t ਕੁੱਲ_ਸਤਰਾਂ, FILE *tfp, ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_output)
{
  ਜਦੋਂ (true)
    {
      ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ = ਕਤਾਰ_ਕੱਢੋ (ਕਤਾਰ);

      ਜੇ (ਗੰਢ->level == ਮਿਲਾਓ_ਸਿਰਾ)
        {
          unlock_ਗੰਢ (ਗੰਢ);
          /* Reinsert so other threads can pop it. */
          ਕਤਾਰ_ਪਾਓ (ਕਤਾਰ, ਗੰਢ);
          ਰੋਕੋ;
        }
      mergelines_ਗੰਢ (ਗੰਢ, ਕੁੱਲ_ਸਤਰਾਂ, tfp, ਆਰਜ਼ੀ_output);
      ਕਤਾਰ_check_ਪਾਓ (ਕਤਾਰ, ਗੰਢ);
      ਕਤਾਰ_check_ਪਾਓ_ਮਾਪਾ (ਕਤਾਰ, ਗੰਢ);

      unlock_ਗੰਢ (ਗੰਢ);
    }
}


ਸਥਿਰ ਖਾਲੀ sortlines (ਬਣਤਰ ਸਤਰ *ਸੀਮਿਤ, size_t, size_t,
                       ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *, ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *,
                       FILE *, ਅੱਖਰ ਅਚਲ *);

/* Thread arguments for sortlines_thread. */

ਬਣਤਰ thread_ਦਲੀਲਾਂ
{
  /* Source, i.e., the array of lines to sort.  This points just past
     the end of the array.  */
  ਬਣਤਰ ਸਤਰ *ਸਤਰਾਂ;

  /* Number of threads to use.  If 0 or 1, sort single-threaded.  */
  size_t nthreads;

  /* Number of lines in LINES and DEST.  */
  size_t ਅਚਲ ਕੁੱਲ_ਸਤਰਾਂ;

  /* Merge node. Lines from this node and this node's sibling will merged
     to this node's parent. */
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਅਚਲ ਗੰਢ;

  /* The priority queue controlling available work for the entire
     internal sort.  */
  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਅਚਲ ਕਤਾਰ;

  /* If at the top level, the file to output to, and the file's name.
     If the file is standard output, the file's name is null.  */
  FILE *tfp;
  ਅੱਖਰ ਅਚਲ *output_ਆਰਜ਼ੀ;
};

/* Like sortlines, except with a signature acceptable to pthread_create.  */

ਸਥਿਰ ਖਾਲੀ *
sortlines_thread (ਖਾਲੀ *ਅੰਕੜੇ)
{
  ਬਣਤਰ thread_ਦਲੀਲਾਂ ਅਚਲ *ਦਲੀਲਾਂ = ਅੰਕੜੇ;
  sortlines (ਦਲੀਲਾਂ->ਸਤਰਾਂ, ਦਲੀਲਾਂ->nthreads, ਦਲੀਲਾਂ->ਕੁੱਲ_ਸਤਰਾਂ,
             ਦਲੀਲਾਂ->ਗੰਢ, ਦਲੀਲਾਂ->ਕਤਾਰ, ਦਲੀਲਾਂ->tfp,
             ਦਲੀਲਾਂ->output_ਆਰਜ਼ੀ);
  ਵਾਪਸ ਨਿਰਾ;
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

ਸਥਿਰ ਖਾਲੀ
sortlines (ਬਣਤਰ ਸਤਰ *ਸੀਮਿਤ ਸਤਰਾਂ, size_t nthreads,
           size_t ਕੁੱਲ_ਸਤਰਾਂ, ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਗੰਢ,
           ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ *ਕਤਾਰ, FILE *tfp, ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_output)
{
  size_t nlines = ਗੰਢ->nlo + ਗੰਢ->nhi;

  /* Calculate thread arguments. */
  size_t lo_ਧਾਗੇ = nthreads / 2;
  size_t hi_ਧਾਗੇ = nthreads - lo_ਧਾਗੇ;
  pthread_t thread;
  ਬਣਤਰ thread_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {ਸਤਰਾਂ, lo_ਧਾਗੇ, ਕੁੱਲ_ਸਤਰਾਂ,
                             ਗੰਢ->lo_ਬੱਚਾ, ਕਤਾਰ, tfp, ਆਰਜ਼ੀ_output};

  ਜੇ (nthreads > 1 && SUBTHREAD_ਸਤਰਾਂ_HEURISTIC <= nlines
      && pthread_create (&thread, ਨਿਰਾ, sortlines_thread, &ਦਲੀਲਾਂ) == 0)
    {
      sortlines (ਸਤਰਾਂ - ਗੰਢ->nlo, hi_ਧਾਗੇ, ਕੁੱਲ_ਸਤਰਾਂ,
                 ਗੰਢ->hi_ਬੱਚਾ, ਕਤਾਰ, tfp, ਆਰਜ਼ੀ_output);
      pthread_join (thread, ਨਿਰਾ);
    }
  ਨਹੀਂਤਾਂ
    {
      /* Nthreads = 1, this is a leaf NODE, or pthread_create failed.
         Sort with 1 thread. */
      size_t nlo = ਗੰਢ->nlo;
      size_t nhi = ਗੰਢ->nhi;
      ਬਣਤਰ ਸਤਰ *ਆਰਜ਼ੀ = ਸਤਰਾਂ - ਕੁੱਲ_ਸਤਰਾਂ;
      ਜੇ (1 < nhi)
        sequential_ਛਾਂਟੋ (ਸਤਰਾਂ - nlo, nhi, ਆਰਜ਼ੀ - nlo / 2, false);
      ਜੇ (1 < nlo)
        sequential_ਛਾਂਟੋ (ਸਤਰਾਂ, nlo, ਆਰਜ਼ੀ, false);

      /* Update merge NODE. No need to lock yet. */
      ਗੰਢ->lo = ਸਤਰਾਂ;
      ਗੰਢ->hi = ਸਤਰਾਂ - nlo;
      ਗੰਢ->ਸਿਰਾ_lo = ਸਤਰਾਂ - nlo;
      ਗੰਢ->ਸਿਰਾ_hi = ਸਤਰਾਂ - nlo - nhi;

      ਕਤਾਰ_ਪਾਓ (ਕਤਾਰ, ਗੰਢ);
      ਮਿਲਾਓ_loop (ਕਤਾਰ, ਕੁੱਲ_ਸਤਰਾਂ, tfp, ਆਰਜ਼ੀ_output);
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

ਸਥਿਰ ਖਾਲੀ
avoid_trashing_ਨਿਵੇਸ਼ (ਬਣਤਰ sortfile *files, size_t ntemps,
                      size_t nfiles, ਅੱਖਰ ਅਚਲ *outfile)
{
  ਬਣਤਰ tempnode *tempcopy = ਨਿਰਾ;

  ਲਈ (size_t i = ntemps; i < nfiles; i++)
    {
      bool is_stdin = streq (files[i].name, "-");
      bool same;
      ਬਣਤਰ stat instat;

      ਜੇ (outfile && streq (outfile, files[i].name) && !is_stdin)
        same = true;
      ਨਹੀਂਤਾਂ
        {
          ਬਣਤਰ stat *outst = get_outstatus ();
          ਜੇ (!outst)
            ਰੋਕੋ;

          same = (((is_stdin
                    ? fstat (STDIN_FILENO, &instat)
                    : stat (files[i].name, &instat))
                   == 0)
                  && psame_inode (&instat, outst));
        }

      ਜੇ (same)
        {
          ਜੇ (! tempcopy)
            {
              FILE *tftp;
              tempcopy = create_ਆਰਜ਼ੀ (&tftp);
              mergefiles (&files[i], 0, 1, tftp, tempcopy->name);
            }

          files[i].name = tempcopy->name;
          files[i].ਆਰਜ਼ੀ = tempcopy;
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

ਸਥਿਰ ਖਾਲੀ
check_inputs (ਅੱਖਰ *ਅਚਲ *files, size_t nfiles)
{
  ਲਈ (size_t i = 0; i < nfiles; i++)
    {
      ਜੇ (streq (files[i], "-"))
        ਜਾਰੀ;

      ਜੇ (euidaccess (files[i], R_OK) != 0)
        ਛਾਂਟੋ_die (_("cannot read"), files[i]);
    }
}

/* Ensure a specified output file can be created or written to,
   and point stdout to it.  Do not truncate the file.
   Exit with a diagnostic on failure.  */

ਸਥਿਰ ਖਾਲੀ
check_output (ਅੱਖਰ ਅਚਲ *outfile)
{
  ਜੇ (outfile)
    {
      ਪੂਰਨ oflags = O_WRONLY | O_ਦੁਆਧਾਰੀ | O_CLOEXEC | O_CREAT;
      ਪੂਰਨ outfd = open (outfile, oflags, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (outfd < 0)
        ਛਾਂਟੋ_die (_("open failed"), outfile);
      move_fd (outfd, STDOUT_FILENO);
    }
}

/* Merge the input FILES.  NTEMPS is the number of files at the
   start of FILES that are temporary; it is zero at the top level.
   NFILES is the total number of files.  Put the output in
   OUTPUT_FILE; a null OUTPUT_FILE stands for standard output.  */

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ (ਬਣਤਰ sortfile *files, size_t ntemps, size_t nfiles,
       ਅੱਖਰ ਅਚਲ *output_file)
{
  ਜਦੋਂ (nmerge < nfiles)
    {
      /* Number of input files processed so far.  */
      size_t ਵਿੱਚ;

      /* Number of output files generated so far.  */
      size_t ਬਾਹਰਲਾ;

      /* nfiles % NMERGE; this counts input files that are left over
         after all full-sized merges have been done.  */
      size_t remainder;

      /* Number of easily-available slots at the next loop iteration.  */
      size_t cheap_slots;

      /* Do as many NMERGE-size merges as possible. In the case that
         nmerge is bogus, increment by the maximum number of file
         descriptors allowed.  */
      ਲਈ (ਬਾਹਰਲਾ = ਵਿੱਚ = 0; nmerge <= nfiles - ਵਿੱਚ; ਬਾਹਰਲਾ++)
        {
          FILE *tfp;
          ਬਣਤਰ tempnode *ਆਰਜ਼ੀ = create_ਆਰਜ਼ੀ (&tfp);
          size_t ਅੰਕ_merged = mergefiles (&files[ਵਿੱਚ], MIN (ntemps, nmerge),
                                          nmerge, tfp, ਆਰਜ਼ੀ->name);
          ntemps -= MIN (ntemps, ਅੰਕ_merged);
          files[ਬਾਹਰਲਾ].name = ਆਰਜ਼ੀ->name;
          files[ਬਾਹਰਲਾ].ਆਰਜ਼ੀ = ਆਰਜ਼ੀ;
          ਵਿੱਚ += ਅੰਕ_merged;
        }

      remainder = nfiles - ਵਿੱਚ;
      cheap_slots = nmerge - ਬਾਹਰਲਾ % nmerge;

      ਜੇ (cheap_slots < remainder)
        {
          /* So many files remain that they can't all be put into the last
             NMERGE-sized output window.  Do one more merge.  Merge as few
             files as possible, to avoid needless I/O.  */
          size_t nshortmerge = remainder - cheap_slots + 1;
          FILE *tfp;
          ਬਣਤਰ tempnode *ਆਰਜ਼ੀ = create_ਆਰਜ਼ੀ (&tfp);
          size_t ਅੰਕ_merged = mergefiles (&files[ਵਿੱਚ], MIN (ntemps, nshortmerge),
                                          nshortmerge, tfp, ਆਰਜ਼ੀ->name);
          ntemps -= MIN (ntemps, ਅੰਕ_merged);
          files[ਬਾਹਰਲਾ].name = ਆਰਜ਼ੀ->name;
          files[ਬਾਹਰਲਾ++].ਆਰਜ਼ੀ = ਆਰਜ਼ੀ;
          ਵਿੱਚ += ਅੰਕ_merged;
        }

      /* Put the remaining input files into the last NMERGE-sized output
         window, so they will be merged in the next pass.  */
      memmove (&files[ਬਾਹਰਲਾ], &files[ਵਿੱਚ], (nfiles - ਵਿੱਚ) * ਆਕਾਰ *files);
      ntemps += ਬਾਹਰਲਾ;
      nfiles -= ਵਿੱਚ - ਬਾਹਰਲਾ;
    }

  avoid_trashing_ਨਿਵੇਸ਼ (files, ntemps, nfiles, output_file);

  /* We aren't guaranteed that this final mergefiles will work, therefore we
     try to merge into the output, and then merge as much as we can into a
     temp file if we can't. Repeat.  */

  ਜਦੋਂ (true)
    {
      /* Merge directly into the output file if possible.  */
      FILE **fps;
      size_t nopened = open_ਨਿਵੇਸ਼_files (files, nfiles, &fps);

      ਜੇ (nopened == nfiles)
        {
          FILE *ofp = ਧਾਰਾ_open (output_file, "w");
          ਜੇ (ofp)
            {
              mergefps (files, ntemps, nfiles, ofp, output_file, fps);
              ਰੋਕੋ;
            }
          ਜੇ (errno != EMFILE || nopened <= 2)
            ਛਾਂਟੋ_die (_("open failed"), output_file);
        }
      ਨਹੀਂਤਾਂ ਜੇ (nopened <= 2)
        ਛਾਂਟੋ_die (_("open failed"), files[nopened].name);

      /* We ran out of file descriptors.  Close one of the input
         files, to gain a file descriptor.  Then create a temporary
         file with our spare file descriptor.  Retry if that failed
         (e.g., some other process could open a file between the time
         we closed and tried to create).  */
      FILE *tfp;
      ਬਣਤਰ tempnode *ਆਰਜ਼ੀ;
      ਕਰੋ
        {
          nopened--;
          xfclose (fps[nopened], files[nopened].name);
          ਆਰਜ਼ੀ = maybe_create_ਆਰਜ਼ੀ (&tfp, ! (nopened <= 2));
        }
      ਜਦੋਂ (!ਆਰਜ਼ੀ);

      /* Merge into the newly allocated temporary.  */
      mergefps (&files[0], MIN (ntemps, nopened), nopened, tfp, ਆਰਜ਼ੀ->name,
                fps);
      ntemps -= MIN (ntemps, nopened);
      files[0].name = ਆਰਜ਼ੀ->name;
      files[0].ਆਰਜ਼ੀ = ਆਰਜ਼ੀ;

      memmove (&files[1], &files[nopened], (nfiles - nopened) * ਆਕਾਰ *files);
      ntemps++;
      nfiles -= nopened - 1;
    }
}

/* Sort NFILES FILES onto OUTPUT_FILE.  Use at most NTHREADS threads.  */

ਸਥਿਰ ਖਾਲੀ
ਛਾਂਟੋ (ਅੱਖਰ *ਅਚਲ *files, size_t nfiles, ਅੱਖਰ ਅਚਲ *output_file,
      size_t nthreads)
{
  ਬਣਤਰ ਬਫਰ buf;
  ਬਣਤਰ ਛਾਂਟੋ_ਬਫਰ_policy policy;
  size_t ntemps = 0;
  bool output_file_created = false;

  buf.alloc = 0;
  policy.initial = policy.ਹੱਦਬੰਦੀ = 0;
  policy.growth_failed = false;

  ਜਦੋਂ (nfiles)
    {
      ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ_output;
      ਅੱਖਰ ਅਚਲ *file = *files;
      FILE *fp = xfopen (file, "r");
      FILE *tfp;

      size_t ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ;
      ਜੇ (nthreads > 1)
        {
          /* Get log P. */
          size_t tmp = 1;
          size_t mult = 1;
          ਜਦੋਂ (tmp < nthreads)
            {
              tmp *= 2;
              mult++;
            }
          ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ = (mult * ਆਕਾਰ (ਬਣਤਰ ਸਤਰ));
        }
      ਨਹੀਂਤਾਂ
        ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ = ਆਕਾਰ (ਬਣਤਰ ਸਤਰ) * 3 / 2;

      ਜੇ (! buf.alloc)
        {
          ਛਾਂਟੋ_ਬਫਰ_policy (&fp, 1, files, nfiles, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ,
                              &policy);
          initbuf (&buf, ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ, policy.initial);
        }
      buf.eof = false;
      files++;
      nfiles--;

      ਜਦੋਂ (fillbuf (&buf, fp, file, &policy))
        {
          ਬਣਤਰ ਸਤਰ *ਸਤਰ;

          ਜੇ (buf.eof && nfiles
              && (ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ + 1
                  < (buf.alloc - buf.ਵਰਤਿਆ - ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_ਸਤਰ * buf.nlines)))
            {
              /* End of file, but there is more input and buffer room.
                 Concatenate the next input file; this is faster in
                 the usual case.  */
              buf.ਖੱਬਾ = buf.ਵਰਤਿਆ;
              ਰੋਕੋ;
            }

          saved_ਸਤਰ.ਪਾਠ = ਨਿਰਾ;
          ਸਤਰ = ਬਫਰ_linelim (&buf);
          ਜੇ (buf.eof && !nfiles && !ntemps && !buf.ਖੱਬਾ)
            {
              xfclose (fp, file);
              tfp = xfopen (output_file, "w");
              ਆਰਜ਼ੀ_output = output_file;
              output_file_created = true;
            }
          ਨਹੀਂਤਾਂ
            {
              ++ntemps;
              ਆਰਜ਼ੀ_output = create_ਆਰਜ਼ੀ (&tfp)->name;
            }
          ਜੇ (1 < buf.nlines)
            {
              ਜੇ (nthreads > 1)
                {
                  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ_ਕਤਾਰ ਕਤਾਰ;
                  ਕਤਾਰ_init (&ਕਤਾਰ, nthreads);
                  ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ *ਮਿਲਾਓ_ਰੁੱਖ =
                    ਮਿਲਾਓ_ਰੁੱਖ_init (nthreads, buf.nlines, ਸਤਰ);

                  sortlines (ਸਤਰ, nthreads, buf.nlines, ਮਿਲਾਓ_ਰੁੱਖ + 1,
                             &ਕਤਾਰ, tfp, ਆਰਜ਼ੀ_output);

                  ਮਿਲਾਓ_ਰੁੱਖ_destroy (nthreads, ਮਿਲਾਓ_ਰੁੱਖ);
                  ਕਤਾਰ_destroy (&ਕਤਾਰ);
                }
              ਨਹੀਂਤਾਂ
                {
                  sequential_ਛਾਂਟੋ (ਸਤਰ, buf.nlines,
                                   ਸਤਰ - buf.nlines, false);
                  ਲਈ (size_t i = 0; i < buf.nlines; i++)
                    write_unique (ਸਤਰ - i - 1, tfp, ਆਰਜ਼ੀ_output);
                }
            }
          ਨਹੀਂਤਾਂ
            write_unique (ਸਤਰ - 1, tfp, ਆਰਜ਼ੀ_output);

          xfclose (tfp, ਆਰਜ਼ੀ_output);

          ਜੇ (output_file_created)
            ਜਾਓ ਅੰਤ;
        }
      xfclose (fp, file);
    }

 ਅੰਤ:
  ਮੁਕਤ (buf.buf);

  ਜੇ (! output_file_created)
    {
      ਬਣਤਰ tempnode *ਗੰਢ = temphead;
      ਬਣਤਰ sortfile *tempfiles = xnmalloc (ntemps, ਆਕਾਰ *tempfiles);
      ਲਈ (size_t i = 0; ਗੰਢ; i++)
        {
          tempfiles[i].name = ਗੰਢ->name;
          tempfiles[i].ਆਰਜ਼ੀ = ਗੰਢ;
          ਗੰਢ = ਗੰਢ->ਅਗਲਾ;
        }
      ਮਿਲਾਓ (tempfiles, ntemps, ntemps, output_file);
      ਮੁਕਤ (tempfiles);
    }

  reap_all ();
}

/* Insert a malloc'd copy of key KEY_ARG at the end of the key list.  */

ਸਥਿਰ ਖਾਲੀ
insertkey (ਬਣਤਰ keyfield *ਕੁੰਜੀ_arg)
{
  ਬਣਤਰ keyfield **p;
  ਬਣਤਰ keyfield *ਕੁੰਜੀ = xmemdup (ਕੁੰਜੀ_arg, ਆਕਾਰ *ਕੁੰਜੀ);

  ਲਈ (p = &keylist; *p; p = &(*p)->ਅਗਲਾ)
    ਜਾਰੀ;
  *p = ਕੁੰਜੀ;
  ਕੁੰਜੀ->ਅਗਲਾ = ਨਿਰਾ;
}

/* Report a bad field specification SPEC, with extra info MSGID.  */

ਸਥਿਰ ਖਾਲੀ
badfieldspec (ਅੱਖਰ ਅਚਲ *spec, ਅੱਖਰ ਅਚਲ *msgid)
{
  error (ਛਾਂਟੋ_FAILURE, 0, _("%s: invalid field specification %s"),
         _(msgid), quote (spec));
}

/* Report incompatible options.  */

ਸਥਿਰ ਖਾਲੀ
incompatible_options (ਅੱਖਰ ਅਚਲ *opts)
{
  error (ਛਾਂਟੋ_FAILURE, 0, _("options '-%s' are incompatible"), (opts));
}

/* Check compatibility of ordering options.  */

ਸਥਿਰ ਖਾਲੀ
check_ordering_compatibility (ਖਾਲੀ)
{
  ਲਈ (ਬਣਤਰ keyfield *ਕੁੰਜੀ = keylist; ਕੁੰਜੀ; ਕੁੰਜੀ = ਕੁੰਜੀ->ਅਗਲਾ)
    ਜੇ (1 < (ਕੁੰਜੀ->numeric + ਕੁੰਜੀ->general_numeric + ਕੁੰਜੀ->human_numeric
             + ਕੁੰਜੀ->month + (ਕੁੰਜੀ->ਸੰਸਕਰਣ_3 | ਕੁੰਜੀ->random | !!ਕੁੰਜੀ->ignore)))
      {
        /* The following is too big, but guaranteed to be "big enough".  */
        ਅੱਖਰ opts[ਆਕਾਰ short_options];
        /* Clear flags we're not interested in.  */
        ਕੁੰਜੀ->skipsblanks = ਕੁੰਜੀ->skipeblanks = ਕੁੰਜੀ->ਉਲਟ = false;
        ਕੁੰਜੀ_ਮੰਜ਼ਿਲ_opts (ਕੁੰਜੀ, opts);
        incompatible_options (opts);
      }
}

/* Parse the leading integer in STRING and store the resulting value
   (which must fit into size_t) into *VAL.  Return the address of the
   suffix after the integer.  If the value is too large, silently
   substitute SIZE_MAX.  If MSGID is null, return NULL after
   failure; otherwise, report MSGID and exit on failure.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
parse_ਖੇਤਰ_ਗਿਣਤੀ (ਅੱਖਰ ਅਚਲ *string, size_t *val, ਅੱਖਰ ਅਚਲ *msgid)
{
  ਅੱਖਰ *suffix;
  uintmax_t n;

  ਚੋਣ (xstrtoumax (string, &suffix, 10, &n, ""))
    {
    ਹਾਲਤ LONGINT_OK:
    ਹਾਲਤ LONGINT_INVALID_SUFFIX_CHAR:
      *val = n;
      ਜੇ (*val == n)
        ਰੋਕੋ;
      FALLTHROUGH;
    ਹਾਲਤ LONGINT_ਉਛਾਲ:
    ਹਾਲਤ LONGINT_ਉਛਾਲ | LONGINT_INVALID_SUFFIX_CHAR:
      *val = SIZE_MAX;
      ਰੋਕੋ;

    ਹਾਲਤ LONGINT_INVALID:
      ਜੇ (msgid)
        error (ਛਾਂਟੋ_FAILURE, 0, _("%s: invalid count at start of %s"),
               _(msgid), quote (string));
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਵਾਪਸ suffix;
}

/* Set the ordering options for KEY specified in S.
   Return the address of the first character in S that
   is not a valid ordering option.
   BLANKTYPE is the kind of blanks that 'b' should skip. */

ਸਥਿਰ ਅੱਖਰ *
ਸੈੱਟ_ordering (ਅੱਖਰ ਅਚਲ *s, ਬਣਤਰ keyfield *ਕੁੰਜੀ, ਗਿਣਤੀ blanktype blanktype)
{
  ਜਦੋਂ (*s)
    {
      ਚੋਣ (*s)
        {
        ਹਾਲਤ 'b':
          ਜੇ (blanktype == bl_ਸ਼ੁਰੂ || blanktype == bl_both)
            ਕੁੰਜੀ->skipsblanks = true;
          ਜੇ (blanktype == bl_ਸਿਰਾ || blanktype == bl_both)
            ਕੁੰਜੀ->skipeblanks = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'd':
          ਕੁੰਜੀ->ignore = nondictionary;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          ਕੁੰਜੀ->translate = fold_toupper;
          ਰੋਕੋ;
        ਹਾਲਤ 'g':
          ਕੁੰਜੀ->general_numeric = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'h':
          ਕੁੰਜੀ->human_numeric = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'i':
          /* Option order should not matter, so don't let -i override
             -d.  -d implies -i, but -i does not imply -d.  */
          ਜੇ (! ਕੁੰਜੀ->ignore)
            ਕੁੰਜੀ->ignore = nonprinting;
          ਰੋਕੋ;
        ਹਾਲਤ 'M':
          ਕੁੰਜੀ->month = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          ਕੁੰਜੀ->numeric = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'R':
          ਕੁੰਜੀ->random = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'r':
          ਕੁੰਜੀ->ਉਲਟ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'V':
          ਕੁੰਜੀ->ਸੰਸਕਰਣ_3 = true;
          ਰੋਕੋ;
        ਮੂਲ:
          ਵਾਪਸ (ਅੱਖਰ *) s;
        }
      ++s;
    }
  ਵਾਪਸ (ਅੱਖਰ *) s;
}

/* Initialize KEY.  */

ਸਥਿਰ ਬਣਤਰ keyfield *
ਕੁੰਜੀ_init (ਬਣਤਰ keyfield *ਕੁੰਜੀ)
{
  memset (ਕੁੰਜੀ, 0, ਆਕਾਰ *ਕੁੰਜੀ);
  ਕੁੰਜੀ->eword = SIZE_MAX;
  ਵਾਪਸ ਕੁੰਜੀ;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਣਤਰ keyfield *ਕੁੰਜੀ;
  ਬਣਤਰ keyfield ਕੁੰਜੀ_buf;
  ਬਣਤਰ keyfield gkey;
  bool gkey_only = false;
  ਅੱਖਰ ਅਚਲ *s;
  ਪੂਰਨ c = 0;
  ਅੱਖਰ checkonly = 0;
  bool mergeonly = false;
  ਅੱਖਰ *random_ਸਰੋਤ = ਨਿਰਾ;
  bool need_random = false;
  size_t nthreads = 0;
  size_t nfiles = 0;
  bool posixly_correct = (getenv ("POSIXLY_CORRECT") != ਨਿਰਾ);
  ਪੂਰਨ posix_ver = posix2_ਸੰਸਕਰਣ ();
  bool traditional_usage = ! (200112 <= posix_ver && posix_ver < 200809);
  ਅੱਖਰ **files;
  ਅੱਖਰ *files_from = ਨਿਰਾ;
  ਬਣਤਰ ਟੋਕਨ tok;
  ਅੱਖਰ ਅਚਲ *outfile = ਨਿਰਾ;
  bool locale_ok;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  locale_ok = !! setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (ਛਾਂਟੋ_FAILURE);

  hard_LC_COLLATE = hard_locale (LC_COLLATE);
#ਜੇ HAVE_NL_LANGINFO
  hard_LC_ਸਮਾਂ = hard_locale (LC_ਸਮਾਂ);
#ਜੇ_ਅੰਤ

  /* Get locale's representation of the decimal point.  */
  {
    ਬਣਤਰ lconv ਅਚਲ *locale = localeconv ();

    /* If the locale doesn't define a decimal point, or if the decimal
       point is multibyte, use the C locale's decimal point.  FIXME:
       add support for multibyte decimal points.  */
    decimal_ਬਿੰਦੂ = locale->decimal_ਬਿੰਦੂ[0];
    ਜੇ (! decimal_ਬਿੰਦੂ || locale->decimal_ਬਿੰਦੂ[1])
      decimal_ਬਿੰਦੂ = '.';

    /* FIXME: add support for multibyte thousands separators.  */
    thousands_sep = locale->thousands_sep[0];
    ਜੇ (thousands_sep && locale->thousands_sep[1])
      thousands_sep_ignored = true;
    ਜੇ (! thousands_sep || locale->thousands_sep[1])
      thousands_sep = NON_CHAR;
  }

  inittables ();

  {
    ਗਿਣਤੀ { nsigs = countof (ਪਦ_sig) };

    ਬਣਤਰ sigaction act;

    sigemptyset (&caught_ਸੰਕੇਤ);
    ਲਈ (size_t i = 0; i < nsigs; i++)
      {
        ਜੇ (ਪਦ_sig[i] == SIGPIPE)
          ਜਾਰੀ;  /* Handled below.  */
        sigaction (ਪਦ_sig[i], ਨਿਰਾ, &act);
        ਜੇ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_ਸੰਕੇਤ, ਪਦ_sig[i]);
      }

    act.sa_handler = sighandler;
    act.sa_mask = caught_ਸੰਕੇਤ;
    act.sa_flags = 0;

    ਲਈ (size_t i = 0; i < nsigs; i++)
      {
        ਜੇ (ਪਦ_sig[i] == SIGPIPE)
          ਜਾਰੀ;  /* Handled below.  */
        ਜੇ (sigismember (&caught_ਸੰਕੇਤ, ਪਦ_sig[i]))
          sigaction (ਪਦ_sig[i], &act, ਨਿਰਾ);
      }
  }
  ਸੰਕੇਤ (SIGCHLD, SIG_DFL); /* Don't inherit CHLD handling from parent.  */

  /* The signal mask is known, so it is safe to invoke exit_cleanup.  */
  atexit (exit_cleanup);

  ਕੁੰਜੀ_init (&gkey);
  gkey.sword = SIZE_MAX;

  files = xnmalloc (argc, ਆਕਾਰ *files);

  ਜਦੋਂ (true)
    {
      /* Parse an operand as a file after "--" was seen; or if
         pedantic and a file was seen, unless the POSIX version
         is not 1003.1-2001 and -c was not seen and the operand is
         "-o FILE" or "-oFILE".  */
      ਪੂਰਨ oi = -1;

      ਜੇ (c == -1
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
          ਜੇ (argc <= optind)
            ਰੋਕੋ;
          files[nfiles++] = argv[optind++];
        }
      ਨਹੀਂਤਾਂ ਚੋਣ (c)
        {
        ਹਾਲਤ 1:
          ਕੁੰਜੀ = ਨਿਰਾ;
          ਜੇ (optarg[0] == '+')
            {
              bool minus_pos_usage = (optind != argc && argv[optind][0] == '-'
                                      && c_isdigit (argv[optind][1]));
              traditional_usage |= minus_pos_usage && !posixly_correct;
              ਜੇ (traditional_usage)
                {
                  /* Treat +POS1 [-POS2] as a key if possible; but silently
                     treat an operand as a file if it is not a valid +POS1.  */
                  ਕੁੰਜੀ = ਕੁੰਜੀ_init (&ਕੁੰਜੀ_buf);
                  s = parse_ਖੇਤਰ_ਗਿਣਤੀ (optarg + 1, &ਕੁੰਜੀ->sword, ਨਿਰਾ);
                  ਜੇ (s && *s == '.')
                    s = parse_ਖੇਤਰ_ਗਿਣਤੀ (s + 1, &ਕੁੰਜੀ->schar, ਨਿਰਾ);
                  ਜੇ (! (ਕੁੰਜੀ->sword || ਕੁੰਜੀ->schar))
                    ਕੁੰਜੀ->sword = SIZE_MAX;
                  ਜੇ (! s || *ਸੈੱਟ_ordering (s, ਕੁੰਜੀ, bl_ਸ਼ੁਰੂ))
                    ਕੁੰਜੀ = ਨਿਰਾ;
                  ਨਹੀਂਤਾਂ
                    {
                      ਜੇ (minus_pos_usage)
                        {
                          ਅੱਖਰ ਅਚਲ *optarg1 = argv[optind++];
                          s = parse_ਖੇਤਰ_ਗਿਣਤੀ (optarg1 + 1, &ਕੁੰਜੀ->eword,
                                             N_("invalid number after '-'"));
                          ਜੇ (*s == '.')
                            s = parse_ਖੇਤਰ_ਗਿਣਤੀ (s + 1, &ਕੁੰਜੀ->echar,
                                               N_("invalid number after '.'"));
                          ਜੇ (!ਕੁੰਜੀ->echar && ਕੁੰਜੀ->eword)
                            {
                              /* obsolescent syntax +A.x -B.y is equivalent to:
                                   -k A+1.x+1,B.y   (when y = 0)
                                   -k A+1.x+1,B+1.y (when y > 0)
                                 So eword is decremented as in the -k case
                                 only when the end field (B) is specified and
                                 echar (y) is 0.  */
                              ਕੁੰਜੀ->eword--;
                            }
                          ਜੇ (*ਸੈੱਟ_ordering (s, ਕੁੰਜੀ, bl_ਸਿਰਾ))
                            badfieldspec (optarg1,
                                      N_("stray character in field spec"));
                        }
                      ਕੁੰਜੀ->traditional_ਵਰਤਿਆ = true;
                      insertkey (ਕੁੰਜੀ);
                    }
                }
            }
          ਜੇ (! ਕੁੰਜੀ)
            files[nfiles++] = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ ਛਾਂਟੋ_OPTION:
          c = XARGMATCH ("--sort", optarg, ਛਾਂਟੋ_ਦਲੀਲਾਂ, ਛਾਂਟੋ_types);
          FALLTHROUGH;
        ਹਾਲਤ 'b':
        ਹਾਲਤ 'd':
        ਹਾਲਤ 'f':
        ਹਾਲਤ 'g':
        ਹਾਲਤ 'h':
        ਹਾਲਤ 'i':
        ਹਾਲਤ 'M':
        ਹਾਲਤ 'n':
        ਹਾਲਤ 'r':
        ਹਾਲਤ 'R':
        ਹਾਲਤ 'V':
          {
            ਅੱਖਰ str[2];
            str[0] = c;
            str[1] = '\0';
            ਸੈੱਟ_ordering (str, &gkey, bl_both);
          }
          ਰੋਕੋ;

        ਹਾਲਤ CHECK_OPTION:
          c = (optarg
               ? XARGMATCH ("--check", optarg, check_ਦਲੀਲਾਂ, check_types)
               : 'c');
          FALLTHROUGH;
        ਹਾਲਤ 'c':
        ਹਾਲਤ 'C':
          ਜੇ (checkonly && checkonly != c)
            incompatible_options ("cC");
          checkonly = c;
          ਰੋਕੋ;

        ਹਾਲਤ COMPRESS_ਪ੍ਰੋਗਰਾਮ_OPTION:
          ਜੇ (compress_ਪ੍ਰੋਗਰਾਮ && !streq (compress_ਪ੍ਰੋਗਰਾਮ, optarg))
            error (ਛਾਂਟੋ_FAILURE, 0, _("multiple compress programs specified"));
          compress_ਪ੍ਰੋਗਰਾਮ = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION:
          debug = true;
          ਰੋਕੋ;

        ਹਾਲਤ FILES0_FROM_OPTION:
          files_from = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'k':
          ਕੁੰਜੀ = ਕੁੰਜੀ_init (&ਕੁੰਜੀ_buf);

          /* Get POS1. */
          s = parse_ਖੇਤਰ_ਗਿਣਤੀ (optarg, &ਕੁੰਜੀ->sword,
                                 N_("invalid number at field start"));
          ਜੇ (! ਕੁੰਜੀ->sword--)
            {
              /* Provoke with 'sort -k0' */
              badfieldspec (optarg, N_("field number is zero"));
            }
          ਜੇ (*s == '.')
            {
              s = parse_ਖੇਤਰ_ਗਿਣਤੀ (s + 1, &ਕੁੰਜੀ->schar,
                                     N_("invalid number after '.'"));
              ਜੇ (! ਕੁੰਜੀ->schar--)
                {
                  /* Provoke with 'sort -k1.0' */
                  badfieldspec (optarg, N_("character offset is zero"));
                }
            }
          ਜੇ (! (ਕੁੰਜੀ->sword || ਕੁੰਜੀ->schar))
            ਕੁੰਜੀ->sword = SIZE_MAX;
          s = ਸੈੱਟ_ordering (s, ਕੁੰਜੀ, bl_ਸ਼ੁਰੂ);
          ਜੇ (*s != ',')
            {
              ਕੁੰਜੀ->eword = SIZE_MAX;
              ਕੁੰਜੀ->echar = 0;
            }
          ਨਹੀਂਤਾਂ
            {
              /* Get POS2. */
              s = parse_ਖੇਤਰ_ਗਿਣਤੀ (s + 1, &ਕੁੰਜੀ->eword,
                                     N_("invalid number after ','"));
              ਜੇ (! ਕੁੰਜੀ->eword--)
                {
                  /* Provoke with 'sort -k1,0' */
                  badfieldspec (optarg, N_("field number is zero"));
                }
              ਜੇ (*s == '.')
                {
                  s = parse_ਖੇਤਰ_ਗਿਣਤੀ (s + 1, &ਕੁੰਜੀ->echar,
                                         N_("invalid number after '.'"));
                }
              s = ਸੈੱਟ_ordering (s, ਕੁੰਜੀ, bl_ਸਿਰਾ);
            }
          ਜੇ (*s)
            badfieldspec (optarg, N_("stray character in field spec"));
          insertkey (ਕੁੰਜੀ);
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          mergeonly = true;
          ਰੋਕੋ;

        ਹਾਲਤ NMERGE_OPTION:
          specify_nmerge (oi, c, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'o':
          ਜੇ (outfile && !streq (outfile, optarg))
            error (ਛਾਂਟੋ_FAILURE, 0, _("multiple output files specified"));
          outfile = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ RANDOM_ਸਰੋਤ_OPTION:
          ਜੇ (random_ਸਰੋਤ && !streq (random_ਸਰੋਤ, optarg))
            error (ਛਾਂਟੋ_FAILURE, 0, _("multiple random sources specified"));
          random_ਸਰੋਤ = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          stable = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'S':
          specify_ਛਾਂਟੋ_ਮਾਪ (oi, c, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          {
            ਅੱਖਰ newtab = optarg[0];
            ਜੇ (! newtab)
              error (ਛਾਂਟੋ_FAILURE, 0, _("empty tab"));
            ਜੇ (optarg[1])
              {
                ਜੇ (streq (optarg, "\\0"))
                  newtab = '\0';
                ਨਹੀਂਤਾਂ
                  {
                    /* Provoke with 'sort -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    error (ਛਾਂਟੋ_FAILURE, 0, _("multi-character tab %s"),
                           quote (optarg));
                  }
              }
            ਜੇ (tab != TAB_DEFAULT && tab != newtab)
              error (ਛਾਂਟੋ_FAILURE, 0, _("incompatible tabs"));
            tab = newtab;
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'T':
          ਜੋੜੋ_ਆਰਜ਼ੀ_dir (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ PARALLEL_OPTION:
          nthreads = specify_nthreads (oi, c, optarg);
          ਰੋਕੋ;

        ਹਾਲਤ 'u':
          unique = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'y':
          /* Accept and ignore e.g. -y0 for compatibility with Solaris 2.x
             through Solaris 7.  It is also accepted by many non-Solaris
             "sort" implementations, e.g., AIX 5.2, HP-UX 11i v2, IRIX 6.5.
             -y is marked as obsolete starting with Solaris 8 (1999), but is
             still accepted as of Solaris 10 prerelease (2004).

             Solaris 2.5.1 "sort -y 100" reads the input file "100", but
             emulate Solaris 8 and 9 "sort -y 100" which ignores the "100",
             and which in general ignores the argument after "-y" if it
             consists entirely of digits (it can even be empty).  */
          ਜੇ (optarg == argv[optind - 1])
            {
              ਅੱਖਰ ਅਚਲ *p;
              ਲਈ (p = optarg; c_isdigit (*p); p++)
                ਜਾਰੀ;
              optind -= (*p != '\0');
            }
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          eolchar = 0;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (ਛਾਂਟੋ_FAILURE);
        }
    }

  /* Ignore SIGPIPE so write failures are reported via EPIPE errno.
     For stdout, sort_die() will reraise SIGPIPE if it was originally SIG_DFL.
     For compression pipes, sort_die() will exit with SORT_FAILURE.  */
  default_SIGPIPE = (ਸੰਕੇਤ (SIGPIPE, SIG_IGN) == SIG_DFL);

  ਜੇ (files_from)
    {
      /* When using --files0-from=F, you may not specify any files
         on the command-line.  */
      ਜੇ (nfiles)
        {
          error (0, 0, _("extra operand %s"), quoteaf (files[0]));
          fprintf (stderr, "%s\n",
                   _("file operands cannot be combined with --files0-from"));
          usage (ਛਾਂਟੋ_FAILURE);
        }

      FILE *ਧਾਰਾ = xfopen (files_from, "r");

      readtokens0_init (&tok);

      ਜੇ (! readtokens0 (ਧਾਰਾ, &tok))
        error (ਛਾਂਟੋ_FAILURE, 0, _("cannot read file names from %s"),
               quoteaf (files_from));
      xfclose (ਧਾਰਾ, files_from);

      ਜੇ (tok.n_tok)
        {
          ਮੁਕਤ (files);
          files = tok.tok;
          nfiles = tok.n_tok;
          ਲਈ (size_t i = 0; i < nfiles; i++)
            {
              ਜੇ (streq (files[i], "-"))
                error (ਛਾਂਟੋ_FAILURE, 0, _("when reading file names from "
                                          "standard input, "
                                          "no file name of %s allowed"),
                       quoteaf (files[i]));
              ਨਹੀਂਤਾਂ ਜੇ (files[i][0] == '\0')
                {
                  /* Using the standard 'filename:line-number:' prefix here is
                     not totally appropriate, since NUL is the separator,
                     not NL, but it might be better than nothing.  */
                  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ file_ਸੰਖਿਆ = i + 1;
                  error (ਛਾਂਟੋ_FAILURE, 0,
                         _("%s:%lu: invalid zero-length file name"),
                         quotef (files_from), file_ਸੰਖਿਆ);
                }
            }
        }
      ਨਹੀਂਤਾਂ
        error (ਛਾਂਟੋ_FAILURE, 0, _("no input from %s"),
               quoteaf (files_from));
    }

  /* Inheritance of global options to individual keys. */
  ਲਈ (ਕੁੰਜੀ = keylist; ਕੁੰਜੀ; ਕੁੰਜੀ = ਕੁੰਜੀ->ਅਗਲਾ)
    {
      ਜੇ (default_ਕੁੰਜੀ_compare (ਕੁੰਜੀ) && !ਕੁੰਜੀ->ਉਲਟ)
        {
          ਕੁੰਜੀ->ignore = gkey.ignore;
          ਕੁੰਜੀ->translate = gkey.translate;
          ਕੁੰਜੀ->skipsblanks = gkey.skipsblanks;
          ਕੁੰਜੀ->skipeblanks = gkey.skipeblanks;
          ਕੁੰਜੀ->month = gkey.month;
          ਕੁੰਜੀ->numeric = gkey.numeric;
          ਕੁੰਜੀ->general_numeric = gkey.general_numeric;
          ਕੁੰਜੀ->human_numeric = gkey.human_numeric;
          ਕੁੰਜੀ->ਸੰਸਕਰਣ_3 = gkey.ਸੰਸਕਰਣ_3;
          ਕੁੰਜੀ->random = gkey.random;
          ਕੁੰਜੀ->ਉਲਟ = gkey.ਉਲਟ;
        }

      need_random |= ਕੁੰਜੀ->random;
    }

  ਜੇ (!keylist && !default_ਕੁੰਜੀ_compare (&gkey))
    {
      gkey_only = true;
      insertkey (&gkey);
      need_random |= gkey.random;
    }

  check_ordering_compatibility ();

  ਜੇ (debug)
    {
      ਜੇ (checkonly || outfile)
        {
          ਸਥਿਰ ਅੱਖਰ opts[] = "X --debug";
          opts[0] = (checkonly ? checkonly : 'o');
          incompatible_options (opts);
        }

      /* Always output the locale in debug mode, since this
         is such a common source of confusion.  */

      /* OpenBSD can only set some categories with LC_ALL above,
         so set LC_COLLATE explicitly to flag errors.  */
      ਜੇ (locale_ok)
        locale_ok = !! setlocale (LC_COLLATE, "");
      ਜੇ (! locale_ok)
          error (0, 0, "%s", _("failed to set locale"));
      ਜੇ (hard_LC_COLLATE)
        error (0, 0, _("text ordering performed using %s sorting rules"),
               quote (setlocale (LC_COLLATE, ਨਿਰਾ)));
      ਨਹੀਂਤਾਂ
        error (0, 0, "%s",
               _("text ordering performed using simple byte comparison"));

      ਕੁੰਜੀ_warnings (&gkey, gkey_only);
    }

  ਉਲਟ = gkey.ਉਲਟ;

  ਜੇ (need_random)
    random_md5_state_init (random_ਸਰੋਤ);

  ਜੇ (ਆਰਜ਼ੀ_dirs.n_dirs == 0)
    {
      ਅੱਖਰ ਅਚਲ *tmp_dir = getenv ("TMPDIR");
      ਜੋੜੋ_ਆਰਜ਼ੀ_dir (tmp_dir ? tmp_dir : DEFAULT_TMPDIR);
    }

  ਜੇ (nfiles == 0)
    {
      nfiles = 1;
      ਮੁਕਤ (files);
      files = xmalloc (ਆਕਾਰ *files);
      *files = (ਅੱਖਰ *) "-";
    }

  /* Need to re-check that we meet the minimum requirement for memory
     usage with the final value for NMERGE. */
  ਜੇ (0 < ਛਾਂਟੋ_ਮਾਪ)
    ਛਾਂਟੋ_ਮਾਪ = MAX (ਛਾਂਟੋ_ਮਾਪ, MIN_ਛਾਂਟੋ_ਮਾਪ);

  ਜੇ (checkonly)
    {
      ਜੇ (nfiles > 1)
        error (ਛਾਂਟੋ_FAILURE, 0, _("extra operand %s not allowed with -%c"),
               quoteaf (files[1]), checkonly);

      ਜੇ (outfile)
        {
          ਸਥਿਰ ਅੱਖਰ opts[] = {0, 'o', 0};
          opts[0] = checkonly;
          incompatible_options (opts);
        }

      /* POSIX requires that sort return 1 IFF invoked with -c or -C and the
         input is not properly sorted.  */
      ਬਾਹਰ_ਨਿਕਲੋ (check (files[0], checkonly) ? EXIT_SUCCESS : ਛਾਂਟੋ_ਬਾਹਰਲਾ_ਦਾ_ਕ੍ਰਮ);
    }

  /* Check all inputs are accessible, or exit immediately.  */
  check_inputs (files, nfiles);

  /* Check output is writable, or exit immediately.  */
  check_output (outfile);

  ਜੇ (mergeonly)
    {
      ਬਣਤਰ sortfile *sortfiles = xcalloc (nfiles, ਆਕਾਰ *sortfiles);

      ਲਈ (size_t i = 0; i < nfiles; ++i)
        sortfiles[i].name = files[i];

      ਮਿਲਾਓ (sortfiles, 0, nfiles, outfile);
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (!nthreads)
        {
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ np = ਅੰਕ_processors (NPROC_CURRENT_OVERRIDABLE);
          nthreads = MIN (np, DEFAULT_MAX_ਧਾਗੇ);
        }

      /* Avoid integer overflow later.  */
      size_t nthreads_max = SIZE_MAX / (2 * ਆਕਾਰ (ਬਣਤਰ ਮਿਲਾਓ_ਗੰਢ));
      nthreads = MIN (nthreads, nthreads_max);

      ਛਾਂਟੋ (files, nfiles, outfile, nthreads);
    }

  ਜੇ (have_read_stdin && fclose (stdin) == EOF)
    ਛਾਂਟੋ_die (_("close failed"), "-");

  main_exit (EXIT_SUCCESS);
}
