/* csplit - split a file into sections determined by context lines
   Copyright (C) 1991-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Written by Stuart Kemp, cpsrk@groper.jcu.edu.au.
   Modified by David MacKenzie, djm@gnu.ai.mit.edu. */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "safe-read.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "term-sig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "csplit"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_ਅਗੇਤਰ	"xx"

/* A compiled pattern arg. */
ਬਣਤਰ control
{
  intmax_t ਵਿੱਥ;		/* Offset from regexp to split at. */
  intmax_t ਸਤਰਾਂ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  ਪੂਰਨ argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ਬਣਤਰ re_ਨਮੂਨਾ_ਬਫਰ re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸ਼ੁਰੂ_ਮਾਪ	8191

/* Number of lines kept in each node in line list. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CTRL_ਮਾਪ	80

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
/* Some small values to test the algorithms. */
# define ਸ਼ੁਰੂ_ਮਾਪ	200
# define CTRL_ਮਾਪ	1
#ਜੇ_ਅੰਤ

/* A string with a length count. */
ਬਣਤਰ cstring
{
  idx_t len;
  ਅੱਖਰ *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ਬਣਤਰ ਸਤਰ
{
  idx_t ਵਰਤਿਆ;			/* Number of offsets used in this struct. */
  idx_t ਪਾਓ_ਸੂਚਕਾਂਕ;		/* Next offset to use when inserting line. */
  idx_t retrieve_ਸੂਚਕਾਂਕ;	/* Next index to use when retrieving line. */
  ਬਣਤਰ cstring starts[CTRL_ਮਾਪ]; /* Lines in the data area. */
  ਬਣਤਰ ਸਤਰ *ਅਗਲਾ;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ
{
  idx_t ਬਾਈਟਾਂ_alloc;		/* Size of the buffer area. */
  idx_t ਬਾਈਟਾਂ_ਵਰਤਿਆ;		/* Bytes used in the buffer area. */
  intmax_t ਸ਼ੁਰੂ_ਸਤਰ;		/* First line number in this buffer. */
  intmax_t ਪਹਿਲਾ_ਉਪਲਬਧ;	/* First line that can be retrieved. */
  idx_t ਅੰਕ_ਸਤਰਾਂ;		/* Number of complete lines in this buffer. */
  ਅੱਖਰ *ਬਫਰ COUNTED_BY (ਬਾਈਟਾਂ_alloc);	/* Data area. */
  ਬਣਤਰ ਸਤਰ *ਸਤਰ_ਸ਼ੁਰੂ;	/* Head of list of pointers to lines. */
  ਬਣਤਰ ਸਤਰ *curr_ਸਤਰ;	/* The line start record currently in use. */
  ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *ਅਗਲਾ;
};

ਸਥਿਰ ਖਾਲੀ close_output_file (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ create_output_file (ਖਾਲੀ);
ਸਥਿਰ ਖਾਲੀ delete_all_files (bool);
ਸਥਿਰ ਖਾਲੀ save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਅਚਲ ਬਣਤਰ cstring *ਸਤਰ);

/* Start of buffer list. */
ਸਥਿਰ ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *ਸਿਰ = ਨਿਰਾ;

/* Partially read line. */
ਸਥਿਰ ਅੱਖਰ *hold_area = ਨਿਰਾ;

/* Number of bytes in 'hold_area'. */
ਸਥਿਰ idx_t hold_ਗਿਣਤੀ = 0;

/* Number of the last line in the buffers. */
ਸਥਿਰ intmax_t ਆਖਰੀ_ਸਤਰ_ਸੰਖਿਆ = 0;

/* Number of the line currently being examined. */
ਸਥਿਰ intmax_t current_ਸਤਰ = 0;

/* If true, we have read EOF. */
ਸਥਿਰ bool have_read_eof = false;

/* Name of output files. */
ਸਥਿਰ ਅੱਖਰ *ਅਸਥਿਰ filename_space = ਨਿਰਾ;

/* Prefix part of output file names. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਸਥਿਰ ਅਗੇਤਰ = DEFAULT_ਅਗੇਤਰ;

/* Suffix part of output file names. */
ਸਥਿਰ ਅੱਖਰ *ਅਸਥਿਰ suffix = ਨਿਰਾ;

/* Number of digits to use in output file names. */
ਸਥਿਰ ਪੂਰਨ ਅਸਥਿਰ digits = 2;

/* Number of files created so far. */
ਸਥਿਰ ਪੂਰਨ ਅਸਥਿਰ files_created = 0;

/* Number of bytes written to current file. */
ਸਥਿਰ intmax_t ਬਾਈਟਾਂ_written;

/* Output file pointer. */
ਸਥਿਰ FILE *output_ਧਾਰਾ = ਨਿਰਾ;

/* Output file name. */
ਸਥਿਰ ਅੱਖਰ *output_filename = ਨਿਰਾ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ਸਥਿਰ ਅੱਖਰ **global_argv;

/* If true, do not print the count of bytes in each output file. */
ਸਥਿਰ bool suppress_ਗਿਣਤੀ;

/* If true, remove output files on error. */
ਸਥਿਰ bool ਅਸਥਿਰ remove_files = true;

/* If true, remove all output files which have a zero length. */
ਸਥਿਰ bool elide_ਖਾਲੀ_ਥਾਵਾਂ_files;

/* If true, suppress the lines that match the PATTERN */
ਸਥਿਰ bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ਸਥਿਰ ਬਣਤਰ control *controls;

/* Number of elements in 'controls'. */
ਸਥਿਰ idx_t control_ਵਰਤਿਆ;

/* The set of signals that are caught.  */
ਸਥਿਰ sigset_t caught_ਸੰਕੇਤ;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"digits", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},  /* Deprecated.  */
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"keep-files", no_ਦਲੀਲ, ਨਿਰਾ, 'k'},
  {"elide-empty-files", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {"prefix", required_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"suffix-format", required_ਦਲੀਲ, ਨਿਰਾ, 'b'},
  {"suppress-matched", no_ਦਲੀਲ, ਨਿਰਾ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ਸਥਿਰ ਖਾਲੀ
cleanup (ਖਾਲੀ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ਨਿਰਾ);
}

ਸਥਿਰ _Noreturn ਖਾਲੀ
cleanup_fatal (ਖਾਲੀ)
{
  cleanup ();
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

ਬਾਹਰੀ ਖਾਲੀ
xalloc_die (ਖਾਲੀ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ਸਥਿਰ ਖਾਲੀ
ਰੁਕਾਵਟ_handler (ਪੂਰਨ sig)
{
  delete_all_files (true);
  ਸੰਕੇਤ (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ਉਠਾਓ (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ਸਥਿਰ ਖਾਲੀ
save_ਮੰਜ਼ਿਲ_hold_area (ਅੱਖਰ *ਸ਼ੁਰੂ, idx_t ਅੰਕ)
{
  ਮੁਕਤ (hold_area);
  hold_area = ਸ਼ੁਰੂ;
  hold_ਗਿਣਤੀ = ਅੰਕ;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ਸਥਿਰ idx_t
read_ਨਿਵੇਸ਼ (ਅੱਖਰ *dest, idx_t max_n_ਬਾਈਟਾਂ)
{
  ਜੇ (max_n_ਬਾਈਟਾਂ == 0)
    ਵਾਪਸ 0;

  ptrdiff_t ਬਾਈਟਾਂ_read = ਸੁਰੱਖਿਅਤ_read (STDIN_FILENO, dest, max_n_ਬਾਈਟਾਂ);

  ਜੇ (ਬਾਈਟਾਂ_read == 0)
    have_read_eof = true;

  ਜੇ (ਬਾਈਟਾਂ_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ਵਾਪਸ ਬਾਈਟਾਂ_read;
}

/* Initialize existing line record P. */

ਸਥਿਰ ਖਾਲੀ
clear_ਸਤਰ_control (ਬਣਤਰ ਸਤਰ *p)
{
  p->ਵਰਤਿਆ = 0;
  p->ਪਾਓ_ਸੂਚਕਾਂਕ = 0;
  p->retrieve_ਸੂਚਕਾਂਕ = 0;
}

/* Return a new, initialized line record. */

ਸਥਿਰ ਬਣਤਰ ਸਤਰ *
new_ਸਤਰ_control (ਖਾਲੀ)
{
  ਬਣਤਰ ਸਤਰ *p = xmalloc (ਆਕਾਰ *p);

  p->ਅਗਲਾ = ਨਿਰਾ;
  clear_ਸਤਰ_control (p);

  ਵਾਪਸ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ਸਥਿਰ ਖਾਲੀ
keep_new_ਸਤਰ (ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *b, ਅੱਖਰ *ਸਤਰ_ਸ਼ੁਰੂ, idx_t ਸਤਰ_len)
{
  ਬਣਤਰ ਸਤਰ *l;

  /* If there is no existing area to keep line info, get some. */
  ਜੇ (b->ਸਤਰ_ਸ਼ੁਰੂ == ਨਿਰਾ)
    b->ਸਤਰ_ਸ਼ੁਰੂ = b->curr_ਸਤਰ = new_ਸਤਰ_control ();

  /* If existing area for lines is full, get more. */
  ਜੇ (b->curr_ਸਤਰ->ਵਰਤਿਆ == CTRL_ਮਾਪ)
    {
      b->curr_ਸਤਰ->ਅਗਲਾ = new_ਸਤਰ_control ();
      b->curr_ਸਤਰ = b->curr_ਸਤਰ->ਅਗਲਾ;
    }

  l = b->curr_ਸਤਰ;

  /* Record the start of the line, and update counters. */
  l->starts[l->ਪਾਓ_ਸੂਚਕਾਂਕ].str = ਸਤਰ_ਸ਼ੁਰੂ;
  l->starts[l->ਪਾਓ_ਸੂਚਕਾਂਕ].len = ਸਤਰ_len;
  l->ਵਰਤਿਆ++;
  l->ਪਾਓ_ਸੂਚਕਾਂਕ++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ਸਥਿਰ idx_t
ਰਿਕਾਰਡ_ਸਤਰ_starts (ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *b)
{
  ਅੱਖਰ *ਸਤਰ_ਸ਼ੁਰੂ;		/* Start of current line. */
  idx_t ਸਤਰਾਂ;			/* Number of lines found. */
  idx_t ਸਤਰ_ਲੰਮਾਈ;		/* Length of each line found. */

  ਜੇ (b->ਬਾਈਟਾਂ_ਵਰਤਿਆ == 0)
    ਵਾਪਸ 0;

  ਸਤਰਾਂ = 0;
  ਸਤਰ_ਸ਼ੁਰੂ = b->ਬਫਰ;
  ਅੱਖਰ *ਬਫਰ_ਸਿਰਾ = ਸਤਰ_ਸ਼ੁਰੂ + b->ਬਾਈਟਾਂ_ਵਰਤਿਆ;
  *ਬਫਰ_ਸਿਰਾ = '\n';

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *ਸਤਰ_ਸਿਰਾ = rawmemchr (ਸਤਰ_ਸ਼ੁਰੂ, '\n');
      ਜੇ (ਸਤਰ_ਸਿਰਾ == ਬਫਰ_ਸਿਰਾ)
        ਰੋਕੋ;
      ਸਤਰ_ਲੰਮਾਈ = ਸਤਰ_ਸਿਰਾ - ਸਤਰ_ਸ਼ੁਰੂ + 1;
      keep_new_ਸਤਰ (b, ਸਤਰ_ਸ਼ੁਰੂ, ਸਤਰ_ਲੰਮਾਈ);
      ਸਤਰ_ਸ਼ੁਰੂ = ਸਤਰ_ਸਿਰਾ + 1;
      ਸਤਰਾਂ++;
    }

  /* Check for an incomplete last line. */
  idx_t ਬਾਈਟਾਂ_ਖੱਬਾ = ਬਫਰ_ਸਿਰਾ - ਸਤਰ_ਸ਼ੁਰੂ;
  ਜੇ (ਬਾਈਟਾਂ_ਖੱਬਾ)
    {
      ਜੇ (have_read_eof)
        {
          keep_new_ਸਤਰ (b, ਸਤਰ_ਸ਼ੁਰੂ, ਬਾਈਟਾਂ_ਖੱਬਾ);
          ਸਤਰਾਂ++;
        }
      ਨਹੀਂਤਾਂ
        save_ਮੰਜ਼ਿਲ_hold_area (ximemdup (ਸਤਰ_ਸ਼ੁਰੂ, ਬਾਈਟਾਂ_ਖੱਬਾ), ਬਾਈਟਾਂ_ਖੱਬਾ);
    }

  b->ਅੰਕ_ਸਤਰਾਂ = ਸਤਰਾਂ;
  b->ਪਹਿਲਾ_ਉਪਲਬਧ = b->ਸ਼ੁਰੂ_ਸਤਰ = ਆਖਰੀ_ਸਤਰ_ਸੰਖਿਆ + 1;
  ਆਖਰੀ_ਸਤਰ_ਸੰਖਿਆ += ਸਤਰਾਂ;

  ਵਾਪਸ ਸਤਰਾਂ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ਜੇ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
free_ਬਫਰ (ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *buf)
{
  ਲਈ (ਬਣਤਰ ਸਤਰ *l = buf->ਸਤਰ_ਸ਼ੁਰੂ; l;)
    {
      ਬਣਤਰ ਸਤਰ *n = l->ਅਗਲਾ;
      ਮੁਕਤ (l);
      l = n;
    }
  ਮੁਕਤ (buf->ਬਫਰ);
  ਮੁਕਤ (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ਸਥਿਰ ATTRIBUTE_DEALLOC (free_ਬਫਰ, 1)
ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *
get_new_ਬਫਰ (idx_t min_ਮਾਪ)
{
  ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *new_ਬਫਰ = xmalloc (ਆਕਾਰ *new_ਬਫਰ);
  new_ਬਫਰ->ਬਾਈਟਾਂ_alloc = 0;
  new_ਬਫਰ->ਬਫਰ = xpalloc (ਨਿਰਾ, &new_ਬਫਰ->ਬਾਈਟਾਂ_alloc, min_ਮਾਪ,
                                -1, 1);
  new_ਬਫਰ->ਬਾਈਟਾਂ_ਵਰਤਿਆ = 0;
  new_ਬਫਰ->ਸ਼ੁਰੂ_ਸਤਰ = new_ਬਫਰ->ਪਹਿਲਾ_ਉਪਲਬਧ = ਆਖਰੀ_ਸਤਰ_ਸੰਖਿਆ + 1;
  new_ਬਫਰ->ਅੰਕ_ਸਤਰਾਂ = 0;
  new_ਬਫਰ->ਸਤਰ_ਸ਼ੁਰੂ = new_ਬਫਰ->curr_ਸਤਰ = ਨਿਰਾ;
  new_ਬਫਰ->ਅਗਲਾ = ਨਿਰਾ;

  ਵਾਪਸ new_ਬਫਰ;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ਸਥਿਰ ਖਾਲੀ
save_ਬਫਰ (ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *buf)
{
  buf->ਅਗਲਾ = ਨਿਰਾ;
  buf->curr_ਸਤਰ = buf->ਸਤਰ_ਸ਼ੁਰੂ;

  ਜੇ (ਸਿਰ == ਨਿਰਾ)
    ਸਿਰ = buf;
  ਨਹੀਂਤਾਂ
    {
      ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *p;
      ਲਈ (p = ਸਿਰ; p->ਅਗਲਾ; p = p->ਅਗਲਾ)
        /* Do nothing. */ ;
      p->ਅਗਲਾ = buf;
    }
}

/* Fill a buffer of input.

   Set the initial size of the buffer to a default.
   Fill the buffer (from the hold area and input stream)
   and find the individual lines.
   If no lines are found (the buffer is too small to hold the next line),
   release the current buffer (whose contents would have been put in the
   hold area) and repeat the process with another large buffer until at least
   one entire line has been read.

   Return true if a new buffer was obtained, otherwise false
   (in which case end-of-file must have been encountered). */

ਸਥਿਰ bool
ਲੱਦੋ_ਬਫਰ (ਖਾਲੀ)
{
  ਜੇ (have_read_eof)
    ਵਾਪਸ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t ਬਾਈਟਾਂ_wanted = MAX (ਸ਼ੁਰੂ_ਮਾਪ, hold_ਗਿਣਤੀ + 1);

  ਜਦੋਂ (true)
    {
      ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *b = get_new_ਬਫਰ (ਬਾਈਟਾਂ_wanted);
      idx_t ਬਾਈਟਾਂ_alloc = b->ਬਾਈਟਾਂ_alloc;
      idx_t ਬਾਈਟਾਂ_avail = ਬਾਈਟਾਂ_alloc;
      ਅੱਖਰ *p = b->ਬਫਰ;

      /* First check the 'holding' area for a partial line. */
      ਜੇ (hold_ਗਿਣਤੀ)
        {
          p = mempcpy (p, hold_area, hold_ਗਿਣਤੀ);
          b->ਬਾਈਟਾਂ_ਵਰਤਿਆ += hold_ਗਿਣਤੀ;
          ਬਾਈਟਾਂ_avail -= hold_ਗਿਣਤੀ;
          hold_ਗਿਣਤੀ = 0;
        }

      b->ਬਾਈਟਾਂ_ਵਰਤਿਆ += read_ਨਿਵੇਸ਼ (p, ਬਾਈਟਾਂ_avail - 1);

      ਜੇ (ਰਿਕਾਰਡ_ਸਤਰ_starts (b) != 0)
        {
          save_ਬਫਰ (b);
          ਵਾਪਸ true;
        }

      free_ਬਫਰ (b);
      ਜੇ (have_read_eof)
        ਵਾਪਸ false;
      ਜੇ (ckd_ਜੋੜੋ (&ਬਾਈਟਾਂ_wanted, ਬਾਈਟਾਂ_alloc, ਬਾਈਟਾਂ_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ਸਥਿਰ intmax_t
get_ਪਹਿਲਾ_ਸਤਰ_ਵਿੱਚ_ਬਫਰ (ਖਾਲੀ)
{
  ਜੇ (ਸਿਰ == ਨਿਰਾ && !ਲੱਦੋ_ਬਫਰ ())
    ਵਾਪਸ 0;

  ਵਾਪਸ ਸਿਰ->ਪਹਿਲਾ_ਉਪਲਬਧ;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ਸਥਿਰ ਬਣਤਰ cstring *
remove_ਸਤਰ (ਖਾਲੀ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ਸਥਿਰ ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *prev_buf = ਨਿਰਾ;

  ਬਣਤਰ cstring *ਸਤਰ;		/* Return value. */
  ਬਣਤਰ ਸਤਰ *l;		/* For convenience. */

  ਜੇ (prev_buf)
    {
      free_ਬਫਰ (prev_buf);
      prev_buf = ਨਿਰਾ;
    }

  ਜੇ (ਸਿਰ == ਨਿਰਾ && !ਲੱਦੋ_ਬਫਰ ())
    ਵਾਪਸ ਨਿਰਾ;

  ਜੇ (current_ਸਤਰ < ਸਿਰ->ਪਹਿਲਾ_ਉਪਲਬਧ)
    current_ਸਤਰ = ਸਿਰ->ਪਹਿਲਾ_ਉਪਲਬਧ;

  ++(ਸਿਰ->ਪਹਿਲਾ_ਉਪਲਬਧ);

  l = ਸਿਰ->curr_ਸਤਰ;

  ਸਤਰ = &l->starts[l->retrieve_ਸੂਚਕਾਂਕ];

  /* Advance index to next line. */
  ਜੇ (++l->retrieve_ਸੂਚਕਾਂਕ == l->ਵਰਤਿਆ)
    {
      /* Go on to the next line record. */
      ਸਿਰ->curr_ਸਤਰ = l->ਅਗਲਾ;
      ਜੇ (ਸਿਰ->curr_ਸਤਰ == ਨਿਰਾ || ਸਿਰ->curr_ਸਤਰ->ਵਰਤਿਆ == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = ਸਿਰ;
          ਸਿਰ = ਸਿਰ->ਅਗਲਾ;
        }
    }

  ਵਾਪਸ ਸਤਰ;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ਸਥਿਰ ਬਣਤਰ cstring *
ਲੱਭੋ_ਸਤਰ (intmax_t linenum)
{
  ਜੇ (ਸਿਰ == ਨਿਰਾ && !ਲੱਦੋ_ਬਫਰ ())
    ਵਾਪਸ ਨਿਰਾ;

  ਜੇ (linenum < ਸਿਰ->ਸ਼ੁਰੂ_ਸਤਰ)
    ਵਾਪਸ ਨਿਰਾ;

  ਲਈ (ਬਣਤਰ ਬਫਰ_ਰਿਕਾਰਡ *b = ਸਿਰ;;)
    {
      ਜੇ (linenum < b->ਸ਼ੁਰੂ_ਸਤਰ + b->ਅੰਕ_ਸਤਰਾਂ)
        {
          /* The line is in this buffer. */
          ਬਣਤਰ ਸਤਰ *l;
          idx_t ਵਿੱਥ;	/* How far into the buffer the line is. */

          l = b->ਸਤਰ_ਸ਼ੁਰੂ;
          ਵਿੱਥ = linenum - b->ਸ਼ੁਰੂ_ਸਤਰ;
          /* Find the control record. */
          ਜਦੋਂ (ਵਿੱਥ >= CTRL_ਮਾਪ)
            {
              l = l->ਅਗਲਾ;
              ਵਿੱਥ -= CTRL_ਮਾਪ;
            }
          ਵਾਪਸ &l->starts[ਵਿੱਥ];
        }
      ਜੇ (b->ਅਗਲਾ == ਨਿਰਾ && !ਲੱਦੋ_ਬਫਰ ())
        ਵਾਪਸ ਨਿਰਾ;
      b = b->ਅਗਲਾ;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ਸਥਿਰ bool
no_ਹੋਰ_ਸਤਰਾਂ (ਖਾਲੀ)
{
  ਵਾਪਸ ਲੱਭੋ_ਸਤਰ (current_ਸਤਰ + 1) == ਨਿਰਾ;
}

/* Open NAME as standard input.  */

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਨਿਵੇਸ਼_file (ਅੱਖਰ ਅਚਲ *name)
{
  ਜੇ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ਸਥਿਰ ਖਾਲੀ
write_ਮੰਜ਼ਿਲ_file (intmax_t ਆਖਰੀ_ਸਤਰ, bool ignore, ਪੂਰਨ argnum)
{
  ਬਣਤਰ cstring *ਸਤਰ;
  intmax_t ਪਹਿਲਾ_ਸਤਰ;		/* First available input line. */
  intmax_t ਸਤਰਾਂ;		/* Number of lines to output. */

  ਪਹਿਲਾ_ਸਤਰ = get_ਪਹਿਲਾ_ਸਤਰ_ਵਿੱਚ_ਬਫਰ ();

  ਜੇ (! ਪਹਿਲਾ_ਸਤਰ || ਪਹਿਲਾ_ਸਤਰ > ਆਖਰੀ_ਸਤਰ)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  ਸਤਰਾਂ = ਆਖਰੀ_ਸਤਰ - ਪਹਿਲਾ_ਸਤਰ;

  ਲਈ (intmax_t i = 0; i < ਸਤਰਾਂ; i++)
    {
      ਸਤਰ = remove_ਸਤਰ ();
      ਜੇ (ਸਤਰ == ਨਿਰਾ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ਜੇ (!ignore)
        save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਸਤਰ);
    }
}

/* Output any lines left after all regexps have been processed. */

ਸਥਿਰ ਖਾਲੀ
dump_rest_ਦਾ_file (ਖਾਲੀ)
{
  ਬਣਤਰ cstring *ਸਤਰ;

  ਜਦੋਂ ((ਸਤਰ = remove_ਸਤਰ ()) != ਨਿਰਾ)
    save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਸਤਰ);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ਸਥਿਰ ਖਾਲੀ
handle_ਸਤਰ_error (ਅਚਲ ਬਣਤਰ control *p, intmax_t repetition)
{
  ਅੱਖਰ buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           ਪ੍ਰੋਗਰਾਮ_name, quote (imaxtostr (p->ਸਤਰਾਂ_required, buf)));
  ਜੇ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ਨਹੀਂਤਾਂ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ਸਥਿਰ ਖਾਲੀ
process_ਸਤਰ_ਗਿਣਤੀ (ਅਚਲ ਬਣਤਰ control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t ਆਖਰੀ_ਸਤਰ_ਮੰਜ਼ਿਲ_save = p->ਸਤਰਾਂ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ਜੇ (no_ਹੋਰ_ਸਤਰਾਂ () && suppress_matched)
    handle_ਸਤਰ_error (p, repetition);

  ਜੇ (!(linenum = get_ਪਹਿਲਾ_ਸਤਰ_ਵਿੱਚ_ਬਫਰ ()))
    handle_ਸਤਰ_error (p, repetition);

  ਜਦੋਂ (linenum++ < ਆਖਰੀ_ਸਤਰ_ਮੰਜ਼ਿਲ_save)
    {
      ਬਣਤਰ cstring *ਸਤਰ = remove_ਸਤਰ ();
      ਜੇ (ਸਤਰ == ਨਿਰਾ)
        handle_ਸਤਰ_error (p, repetition);
      save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਸਤਰ);
    }

  close_output_file ();

  ਜੇ (suppress_matched)
    remove_ਸਤਰ ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ਜੇ (no_ਹੋਰ_ਸਤਰਾਂ () && !suppress_matched)
    handle_ਸਤਰ_error (p, repetition);
}

ਸਥਿਰ ਖਾਲੀ
regexp_error (ਬਣਤਰ control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           ਪ੍ਰੋਗਰਾਮ_name, quote (global_argv[p->argnum]));

  ਜੇ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ਨਹੀਂਤਾਂ
    fprintf (stderr, "\n");

  ਜੇ (!ignore)
    {
      dump_rest_ਦਾ_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ਸਥਿਰ ਖਾਲੀ
process_regexp (ਬਣਤਰ control *p, intmax_t repetition)
{
  ਬਣਤਰ cstring *ਸਤਰ;		/* From input file. */
  idx_t ਸਤਰ_len;		/* To make "$" in regexps work. */
  intmax_t break_ਸਤਰ;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ਜੇ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ਜੇ (p->ਵਿੱਥ >= 0)
    {
      ਜਦੋਂ (true)
        {
          ਸਤਰ = ਲੱਭੋ_ਸਤਰ (++current_ਸਤਰ);
          ਜੇ (ਸਤਰ == ਨਿਰਾ)
            {
              ਜੇ (p->repeat_forever)
                {
                  ਜੇ (!ignore)
                    {
                      dump_rest_ਦਾ_file ();
                      close_output_file ();
                    }
                  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
                }
              ਨਹੀਂਤਾਂ
                regexp_error (p, repetition, ignore);
            }
          ਸਤਰ_len = ਸਤਰ->len;
          ਜੇ (ਸਤਰ->str[ਸਤਰ_len - 1] == '\n')
            ਸਤਰ_len--;
          ret = re_ਖੋਜ (&p->re_compiled, ਸਤਰ->str, ਸਤਰ_len,
                           0, ਸਤਰ_len, ਨਿਰਾ);
          ਜੇ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ਜੇ (ret == -1)
            {
              ਸਤਰ = remove_ਸਤਰ ();
              ਜੇ (!ignore)
                save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਸਤਰ);
            }
          ਨਹੀਂਤਾਂ
            ਰੋਕੋ;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* Buffer the lines. */
      ਜਦੋਂ (true)
        {
          ਸਤਰ = ਲੱਭੋ_ਸਤਰ (++current_ਸਤਰ);
          ਜੇ (ਸਤਰ == ਨਿਰਾ)
            {
              ਜੇ (p->repeat_forever)
                {
                  ਜੇ (!ignore)
                    {
                      dump_rest_ਦਾ_file ();
                      close_output_file ();
                    }
                  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
                }
              ਨਹੀਂਤਾਂ
                regexp_error (p, repetition, ignore);
            }
          ਸਤਰ_len = ਸਤਰ->len;
          ਜੇ (ਸਤਰ->str[ਸਤਰ_len - 1] == '\n')
            ਸਤਰ_len--;
          ret = re_ਖੋਜ (&p->re_compiled, ਸਤਰ->str, ਸਤਰ_len,
                           0, ਸਤਰ_len, ਨਿਰਾ);
          ਜੇ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ਜੇ (ret != -1)
            ਰੋਕੋ;
        }
    }

  /* Account for any offset from this regexp. */
  break_ਸਤਰ = current_ਸਤਰ + p->ਵਿੱਥ;

  write_ਮੰਜ਼ਿਲ_file (break_ਸਤਰ, ignore, p->argnum);

  ਜੇ (!ignore)
    close_output_file ();

  ਜੇ (p->ਵਿੱਥ > 0)
    current_ਸਤਰ = break_ਸਤਰ;

  ਜੇ (suppress_matched)
    remove_ਸਤਰ ();
}

/* Split the input file according to the control records we have built. */

ਸਥਿਰ ਖਾਲੀ
ਵੰਡੋ_file (ਖਾਲੀ)
{
  ਲਈ (idx_t i = 0; i < control_ਵਰਤਿਆ; i++)
    {
      ਜੇ (controls[i].regexpr)
        {
          ਲਈ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ਨਹੀਂਤਾਂ
        {
          ਲਈ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_ਸਤਰ_ਗਿਣਤੀ (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_ਦਾ_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ਸਥਿਰ ਅੱਖਰ *
make_filename (ਪੂਰਨ ਅੰਕ)
{
  strcpy (filename_space, ਅਗੇਤਰ);
  ਜੇ (suffix)
    sprintf (filename_space + ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ), suffix, ਅੰਕ);
  ਨਹੀਂਤਾਂ
    sprintf (filename_space + ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ), "%0*d", digits, ਅੰਕ);
  ਵਾਪਸ filename_space;
}

/* Create the next output file. */

ਸਥਿਰ ਖਾਲੀ
create_output_file (ਖਾਲੀ)
{
  ਪੂਰਨ nfiles = files_created;
  bool fopen_ok;
  ਪੂਰਨ fopen_errno;

  output_filename = make_filename (nfiles);

  ਜੇ (nfiles == ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &oldset);
      output_ਧਾਰਾ = fopen (output_filename, "w");
      fopen_ok = (output_ਧਾਰਾ != ਨਿਰਾ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ਨਿਰਾ);
    }

  ਜੇ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  ਬਾਈਟਾਂ_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ਸਥਿਰ ਖਾਲੀ
delete_all_files (bool ਵਿੱਚ_signal_handler)
{
  ਜੇ (! remove_files)
    ਵਾਪਸ;

  ਲਈ (ਪੂਰਨ i = files_created; 0 <= --i; )
    {
      ਅੱਖਰ ਅਚਲ *name = make_filename (i);
      ਜੇ (unlink (name) != 0 && errno != ENOENT && !ਵਿੱਚ_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ਸਥਿਰ ਖਾਲੀ
close_output_file (ਖਾਲੀ)
{
  ਜੇ (output_ਧਾਰਾ)
    {
      ਜੇ (ferror (output_ਧਾਰਾ))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_ਧਾਰਾ = ਨਿਰਾ;
          cleanup_fatal ();
        }
      ਜੇ (fclose (output_ਧਾਰਾ) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_ਧਾਰਾ = ਨਿਰਾ;
          cleanup_fatal ();
        }
      ਜੇ (ਬਾਈਟਾਂ_written == 0 && elide_ਖਾਲੀ_ਥਾਵਾਂ_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          ਪੂਰਨ unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_ਸੰਕੇਤ, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ਨਿਰਾ);

          ਜੇ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (!suppress_ਗਿਣਤੀ)
            fprintf (stdout, "%jd\n", ਬਾਈਟਾਂ_written);
        }
      output_ਧਾਰਾ = ਨਿਰਾ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ਸਥਿਰ ਖਾਲੀ
save_ਸਤਰ_ਮੰਜ਼ਿਲ_file (ਅਚਲ ਬਣਤਰ cstring *ਸਤਰ)
{
  idx_t l = fwrite (ਸਤਰ->str, ਆਕਾਰ (ਅੱਖਰ), ਸਤਰ->len, output_ਧਾਰਾ);
  ਜੇ (l != ਸਤਰ->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_ਧਾਰਾ = ਨਿਰਾ;
      cleanup_fatal ();
    }
  ਬਾਈਟਾਂ_written += ਸਤਰ->len;
}

/* Return a new, initialized control record. */

ਸਥਿਰ ਬਣਤਰ control *
new_control_ਰਿਕਾਰਡ (ਖਾਲੀ)
{
  ਸਥਿਰ idx_t control_allocated = 0; /* Total space allocated. */
  ਬਣਤਰ control *p;

  ਜੇ (control_ਵਰਤਿਆ == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, ਆਕਾਰ *controls);
  p = &controls[control_ਵਰਤਿਆ++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->ਸਤਰਾਂ_required = 0;
  p->ਵਿੱਥ = 0;
  ਵਾਪਸ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ਸਥਿਰ ਖਾਲੀ
check_for_ਵਿੱਥ (ਬਣਤਰ control *p, ਅੱਖਰ ਅਚਲ *str, ਅੱਖਰ ਅਚਲ *ਅੰਕ)
{
  ਜੇ (xstrtoimax (ਅੰਕ, ਨਿਰਾ, 10, &p->ਵਿੱਥ, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ਸਥਿਰ ਖਾਲੀ
parse_repeat_ਗਿਣਤੀ (ਪੂਰਨ argnum, ਬਣਤਰ control *p, ਅੱਖਰ *str)
{
  ਅੱਖਰ *ਸਿਰਾ;

  ਸਿਰਾ = str + ਲੜੀ_ਲੰਬਾਈ (str) - 1;
  ਜੇ (*ਸਿਰਾ != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *ਸਿਰਾ = '\0';

  ਜੇ (str + 1 == ਸਿਰਾ - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ਨਹੀਂਤਾਂ
    {
      uintmax_t val;
      ਜੇ (xstrtoumax (str + 1, ਨਿਰਾ, 10, &val, "") != LONGINT_OK
          || ckd_ਜੋੜੋ (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *ਸਿਰਾ = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ਸਥਿਰ ਬਣਤਰ control *
ਕੱਢੋ_regexp (ਪੂਰਨ argnum, bool ignore, ਅੱਖਰ ਅਚਲ *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  ਅੱਖਰ delim = *str;
  ਅੱਖਰ ਅਚਲ *closing_delim;
  ਬਣਤਰ control *p;
  ਅੱਖਰ ਅਚਲ *err;

  closing_delim = strrchr (str + 1, delim);
  ਜੇ (closing_delim == ਨਿਰਾ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_ਰਿਕਾਰਡ ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.ਬਫਰ = ਨਿਰਾ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ਨਿਰਾ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ਖਾਲੀ_ਥਾਵਾਂ_RANGES;
  err = re_compile_ਨਮੂਨਾ (str + 1, len, &p->re_compiled);
  ਜੇ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ਜੇ (closing_delim[1])
    check_for_ਵਿੱਥ (p, str, closing_delim + 1);

  ਵਾਪਸ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ਸਥਿਰ ਖਾਲੀ
parse_patterns (ਪੂਰਨ argc, ਪੂਰਨ ਸ਼ੁਰੂ, ਅੱਖਰ **argv)
{
  ਬਣਤਰ control *p;		/* New control record created. */
  ਸਥਿਰ intmax_t ਆਖਰੀ_val = 0;

  ਲਈ (ਪੂਰਨ i = ਸ਼ੁਰੂ; i < argc; i++)
    {
      ਜੇ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = ਕੱਢੋ_regexp (i, *argv[i] == '%', argv[i]);
        }
      ਨਹੀਂਤਾਂ
        {
          p = new_control_ਰਿਕਾਰਡ ();
          p->argnum = i;

          uintmax_t val;
          ਜੇ (xstrtoumax (argv[i], ਨਿਰਾ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ਜੇ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ਜੇ (val < ਆਖਰੀ_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), ਆਖਰੀ_val);

          ਜੇ (val == ਆਖਰੀ_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          ਆਖਰੀ_val = val;

          p->ਸਤਰਾਂ_required = val;
        }

      ਜੇ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ਗਿਣਤੀ (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ਗਿਣਤੀ { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ਸਥਿਰ idx_t
get_format_flags (ਅੱਖਰ ਅਚਲ *format, ਪੂਰਨ *flags_ptr)
{
  ਪੂਰਨ flags = 0;

  ਲਈ (idx_t ਗਿਣਤੀ_2 = 0; ; ਗਿਣਤੀ_2++)
    {
      ਚੋਣ (format[ਗਿਣਤੀ_2])
        {
        ਹਾਲਤ '-':
        ਹਾਲਤ '0':
          ਰੋਕੋ;

        ਹਾਲਤ '\'':
          flags |= FLAG_THOUSANDS;
          ਰੋਕੋ;

        ਹਾਲਤ '#':
          flags |= FLAG_ALTERNATIVE;
          ਰੋਕੋ;

        ਮੂਲ:
          *flags_ptr = flags;
          ਵਾਪਸ ਗਿਣਤੀ_2;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ਸਥਿਰ ਖਾਲੀ
check_format_conv_type (ਅੱਖਰ *format, ਪੂਰਨ flags)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch = *format;
  ਪੂਰਨ compatible_flags = FLAG_THOUSANDS;

  ਚੋਣ (ch)
    {
    ਹਾਲਤ 'd':
    ਹਾਲਤ 'i':
      ਰੋਕੋ;

    ਹਾਲਤ 'u':
      *format = 'd';
      ਰੋਕੋ;

    ਹਾਲਤ 'o':
    ਹਾਲਤ 'x':
    ਹਾਲਤ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ਰੋਕੋ;

    ਹਾਲਤ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    ਮੂਲ:
      ਜੇ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ਨਹੀਂਤਾਂ
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ਜੇ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ਸਥਿਰ idx_t
max_ਬਾਹਰਲਾ (ਅੱਖਰ *format)
{
  bool percent = false;

  ਲਈ (ਅੱਖਰ *f = format; *f; f++)
    ਜੇ (*f == '%' && *++f != '%')
      {
        ਜੇ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        ਪੂਰਨ flags;
        f += get_format_flags (f, &flags);
        ਜਦੋਂ (c_isdigit (*f))
          f++;
        ਜੇ (*f == '.')
          ਜਦੋਂ (c_isdigit (*++f))
            ਜਾਰੀ;
        check_format_conv_type (f, flags);
      }

  ਜੇ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  ਪੂਰਨ maxlen = snprintf (ਨਿਰਾ, 0, format, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
  ਜੇ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  ਵਾਪਸ maxlen;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ਨਿਰਾ))
         != -1)
    ਚੋਣ (optc)
      {
      ਹਾਲਤ 'f':
        ਅਗੇਤਰ = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ 'b':
        suffix = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ 'k':
        remove_files = false;
        ਰੋਕੋ;

      ਹਾਲਤ 'n':
        digits = xdectoimax (optarg, 0, MIN (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, IDX_MAX), "",
                             _("invalid number"), 0);
        ਰੋਕੋ;

      ਹਾਲਤ 's':
      ਹਾਲਤ 'q':
        suppress_ਗਿਣਤੀ = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'z':
        elide_ਖਾਲੀ_ਥਾਵਾਂ_files = true;
        ਰੋਕੋ;

      ਹਾਲਤ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  ਜੇ (argc - optind < 2)
    {
      ਜੇ (argc <= optind)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t ਅਗੇਤਰ_len = ਲੜੀ_ਲੰਬਾਈ (ਅਗੇਤਰ);
  idx_t max_digit_string_len
    = (suffix
       ? max_ਬਾਹਰਲਾ (suffix)
       : MAX (INT_STRLEN_BOUND (ਪੂਰਨ), digits));
  idx_t filename_ਮਾਪ;
  ਜੇ (ckd_ਜੋੜੋ (&filename_ਮਾਪ, ਅਗੇਤਰ_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_ਮਾਪ);

  ਸੈੱਟ_ਨਿਵੇਸ਼_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ਗਿਣਤੀ { nsigs = countof (ਪਦ_sig) };

    ਬਣਤਰ sigaction act;

    sigemptyset (&caught_ਸੰਕੇਤ);
    ਲਈ (ਪੂਰਨ i = 0; i < nsigs; i++)
      {
        sigaction (ਪਦ_sig[i], ਨਿਰਾ, &act);
        ਜੇ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_ਸੰਕੇਤ, ਪਦ_sig[i]);
      }

    act.sa_handler = ਰੁਕਾਵਟ_handler;
    act.sa_mask = caught_ਸੰਕੇਤ;
    act.sa_flags = 0;

    ਲਈ (ਪੂਰਨ i = 0; i < nsigs; i++)
      ਜੇ (sigismember (&caught_ਸੰਕੇਤ, ਪਦ_sig[i]))
        sigaction (ਪਦ_sig[i], &act, ਨਿਰਾ);
  }

  ਵੰਡੋ_file ();

  ਜੇ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ਵਾਪਸ EXIT_SUCCESS;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... FILE ਨਮੂਨਾ...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Output pieces ਦਾ FILE separated by ਨਮੂਨਾ(s) ਮੰਜ਼ਿਲ files 'xx00', 'xx01', ...,\n\
and output byte counts ਦਾ each piece ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard ਨਿਵੇਸ਼ ਜੇ FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead ਦਾ %02d\n\
"));
      oputs (_("\
  -f, --ਅਗੇਤਰ=ਅਗੇਤਰ_2\n\
         use ਅਗੇਤਰ_2 instead ਦਾ 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         ਕਰੋ not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the ਸਤਰਾਂ matching ਨਮੂਨਾ\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified ਸੰਖਿਆ ਦਾ digits instead ਦਾ 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         ਕਰੋ not print counts ਦਾ output file sizes\n\
"));
      oputs (_("\
  -z, --elide-ਖਾਲੀ_ਥਾਵਾਂ-files\n\
         suppress ਖਾਲੀ_ਥਾਵਾਂ output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each ਨਮੂਨਾ may be:\n\
  INTEGER            copy ਉੱਪਰ ਮੰਜ਼ਿਲ but not including specified ਸਤਰ ਸੰਖਿਆ\n\
  /REGEXP/[ਵਿੱਥ_2]   copy ਉੱਪਰ ਮੰਜ਼ਿਲ but not including a matching ਸਤਰ\n\
  %REGEXP%[ਵਿੱਥ_2]   skip ਮੰਜ਼ਿਲ, but not including a matching ਸਤਰ\n\
  {INTEGER}          repeat the ਪਿਛਲਾ ਨਮੂਨਾ_2 specified ਸੰਖਿਆ ਦਾ times\n\
  {*}                repeat the ਪਿਛਲਾ ਨਮੂਨਾ_2 as many times as possible\n\
\n\
A ਸਤਰ ਵਿੱਥ_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}
