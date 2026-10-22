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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "safe-read.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
ಸಂರಚನಾ control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t ಸಾಲುಗಳು_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  ಪೂರ್ಣಾಙ್ಕ argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ಸಂರಚನಾ re_pattern_buffer re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#define START_SIZE	8191

/* Number of lines kept in each node in line list. */
#define CTRL_SIZE	80

#ifdef DEBUG
/* Some small values to test the algorithms. */
# define START_SIZE	200
# define CTRL_SIZE	1
#endif

/* A string with a length count. */
ಸಂರಚನಾ cstring
{
  idx_t len;
  ವರ್ಣ *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ಸಂರಚನಾ ಸಾಲು
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t ಸೇರಿಸು_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  ಸಂರಚನಾ cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  ಸಂರಚನಾ ಸಾಲು *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ಸಂರಚನಾ buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_ಸಾಲು;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t ಅಂಕೆ_ಸಾಲುಗಳು;		/* Number of complete lines in this buffer. */
  ವರ್ಣ *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  ಸಂರಚನಾ ಸಾಲು *ಸಾಲು_start;	/* Head of list of pointers to lines. */
  ಸಂರಚನಾ ಸಾಲು *curr_ಸಾಲು;	/* The line start record currently in use. */
  ಸಂರಚನಾ buffer_record *next;
};

ಸ್ಥಿರ ಶೂನ್ಯ close_output_file (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ create_output_file (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ delete_all_files (bool);
ಸ್ಥಿರ ಶೂನ್ಯ save_ಸಾಲು_to_file (ಧ್ರುವ ಸಂರಚನಾ cstring *ಸಾಲು);

/* Start of buffer list. */
ಸ್ಥಿರ ಸಂರಚನಾ buffer_record *head = ರಿಕ್ತ;

/* Partially read line. */
ಸ್ಥಿರ ವರ್ಣ *hold_area = ರಿಕ್ತ;

/* Number of bytes in 'hold_area'. */
ಸ್ಥಿರ idx_t hold_ಎಣಿಕೆ = 0;

/* Number of the last line in the buffers. */
ಸ್ಥಿರ intmax_t last_ಸಾಲು_ಸಂಖ್ಯೆ = 0;

/* Number of the line currently being examined. */
ಸ್ಥಿರ intmax_t current_ಸಾಲು = 0;

/* If true, we have read EOF. */
ಸ್ಥಿರ bool have_read_eof = false;

/* Name of output files. */
ಸ್ಥಿರ ವರ್ಣ *ಚಞ್ಚಲ filename_space = ರಿಕ್ತ;

/* Prefix part of output file names. */
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಚಞ್ಚಲ prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
ಸ್ಥಿರ ವರ್ಣ *ಚಞ್ಚಲ suffix = ರಿಕ್ತ;

/* Number of digits to use in output file names. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಚಞ್ಚಲ digits = 2;

/* Number of files created so far. */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ಚಞ್ಚಲ files_created = 0;

/* Number of bytes written to current file. */
ಸ್ಥಿರ intmax_t bytes_written;

/* Output file pointer. */
ಸ್ಥಿರ FILE *output_stream = ರಿಕ್ತ;

/* Output file name. */
ಸ್ಥಿರ ವರ್ಣ *output_filename = ರಿಕ್ತ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ಸ್ಥಿರ ವರ್ಣ **global_argv;

/* If true, do not print the count of bytes in each output file. */
ಸ್ಥಿರ bool suppress_ಎಣಿಕೆ;

/* If true, remove output files on error. */
ಸ್ಥಿರ bool ಚಞ್ಚಲ remove_files = true;

/* If true, remove all output files which have a zero length. */
ಸ್ಥಿರ bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
ಸ್ಥಿರ bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ಸ್ಥಿರ ಸಂರಚನಾ control *controls;

/* Number of elements in 'controls'. */
ಸ್ಥಿರ idx_t control_used;

/* The set of signals that are caught.  */
ಸ್ಥಿರ sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"digits", required_argument, ರಿಕ್ತ, 'n'},
  {"quiet", no_argument, ರಿಕ್ತ, 'q'},  /* Deprecated.  */
  {"silent", no_argument, ರಿಕ್ತ, 's'},
  {"keep-files", no_argument, ರಿಕ್ತ, 'k'},
  {"elide-empty-files", no_argument, ರಿಕ್ತ, 'z'},
  {"prefix", required_argument, ರಿಕ್ತ, 'f'},
  {"suffix-format", required_argument, ರಿಕ್ತ, 'b'},
  {"suppress-matched", no_argument, ರಿಕ್ತ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ರಿಕ್ತ);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
cleanup_fatal (ಶೂನ್ಯ)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

ಬಾಹ್ಯ ಶೂನ್ಯ
xalloc_die (ಶೂನ್ಯ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
interrupt_handler (ಪೂರ್ಣಾಙ್ಕ sig)
{
  delete_all_files (true);
  ಸಙ್ಕೇತ (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ಉತ್ಥಾಪಯ (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ಸ್ಥಿರ ಶೂನ್ಯ
save_to_hold_area (ವರ್ಣ *start, idx_t ಅಂಕೆ)
{
  ಮುಕ್ತ (hold_area);
  hold_area = start;
  hold_ಎಣಿಕೆ = ಅಂಕೆ;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ಸ್ಥಿರ idx_t
read_ಒಳಹರಿವು (ವರ್ಣ *dest, idx_t max_n_bytes)
{
  ಯದಿ (max_n_bytes == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  ಯದಿ (bytes_read == 0)
    have_read_eof = true;

  ಯದಿ (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ಪ್ರತ್ಯಾಗಮ bytes_read;
}

/* Initialize existing line record P. */

ಸ್ಥಿರ ಶೂನ್ಯ
clear_ಸಾಲು_control (ಸಂರಚನಾ ಸಾಲು *p)
{
  p->used = 0;
  p->ಸೇರಿಸು_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

ಸ್ಥಿರ ಸಂರಚನಾ ಸಾಲು *
new_ಸಾಲು_control (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ ಸಾಲು *p = xmalloc (ಪರಿಮಾಣ *p);

  p->next = ರಿಕ್ತ;
  clear_ಸಾಲು_control (p);

  ಪ್ರತ್ಯಾಗಮ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ಸ್ಥಿರ ಶೂನ್ಯ
keep_new_ಸಾಲು (ಸಂರಚನಾ buffer_record *b, ವರ್ಣ *ಸಾಲು_start, idx_t ಸಾಲು_len)
{
  ಸಂರಚನಾ ಸಾಲು *l;

  /* If there is no existing area to keep line info, get some. */
  ಯದಿ (b->ಸಾಲು_start == ರಿಕ್ತ)
    b->ಸಾಲು_start = b->curr_ಸಾಲು = new_ಸಾಲು_control ();

  /* If existing area for lines is full, get more. */
  ಯದಿ (b->curr_ಸಾಲು->used == CTRL_SIZE)
    {
      b->curr_ಸಾಲು->next = new_ಸಾಲು_control ();
      b->curr_ಸಾಲು = b->curr_ಸಾಲು->next;
    }

  l = b->curr_ಸಾಲು;

  /* Record the start of the line, and update counters. */
  l->starts[l->ಸೇರಿಸು_index].str = ಸಾಲು_start;
  l->starts[l->ಸೇರಿಸು_index].len = ಸಾಲು_len;
  l->used++;
  l->ಸೇರಿಸು_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ಸ್ಥಿರ idx_t
record_ಸಾಲು_starts (ಸಂರಚನಾ buffer_record *b)
{
  ವರ್ಣ *ಸಾಲು_start;		/* Start of current line. */
  idx_t ಸಾಲುಗಳು;			/* Number of lines found. */
  idx_t ಸಾಲು_length;		/* Length of each line found. */

  ಯದಿ (b->bytes_used == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಸಾಲುಗಳು = 0;
  ಸಾಲು_start = b->buffer;
  ವರ್ಣ *buffer_end = ಸಾಲು_start + b->bytes_used;
  *buffer_end = '\n';

  ಯಾವತ್ (true)
    {
      ವರ್ಣ *ಸಾಲು_end = rawmemchr (ಸಾಲು_start, '\n');
      ಯದಿ (ಸಾಲು_end == buffer_end)
        ಭಙ್ಗ;
      ಸಾಲು_length = ಸಾಲು_end - ಸಾಲು_start + 1;
      keep_new_ಸಾಲು (b, ಸಾಲು_start, ಸಾಲು_length);
      ಸಾಲು_start = ಸಾಲು_end + 1;
      ಸಾಲುಗಳು++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_ಎಡ = buffer_end - ಸಾಲು_start;
  ಯದಿ (bytes_ಎಡ)
    {
      ಯದಿ (have_read_eof)
        {
          keep_new_ಸಾಲು (b, ಸಾಲು_start, bytes_ಎಡ);
          ಸಾಲುಗಳು++;
        }
      ಅನ್ಯಥಾ
        save_to_hold_area (ximemdup (ಸಾಲು_start, bytes_ಎಡ), bytes_ಎಡ);
    }

  b->ಅಂಕೆ_ಸಾಲುಗಳು = ಸಾಲುಗಳು;
  b->first_available = b->start_ಸಾಲು = last_ಸಾಲು_ಸಂಖ್ಯೆ + 1;
  last_ಸಾಲು_ಸಂಖ್ಯೆ += ಸಾಲುಗಳು;

  ಪ್ರತ್ಯಾಗಮ ಸಾಲುಗಳು;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ಯದಿ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ಸ್ಥಿರ ಶೂನ್ಯ
free_buffer (ಸಂರಚನಾ buffer_record *buf)
{
  ಪ್ರತಿ (ಸಂರಚನಾ ಸಾಲು *l = buf->ಸಾಲು_start; l;)
    {
      ಸಂರಚನಾ ಸಾಲು *n = l->next;
      ಮುಕ್ತ (l);
      l = n;
    }
  ಮುಕ್ತ (buf->buffer);
  ಮುಕ್ತ (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ಸ್ಥಿರ ATTRIBUTE_DEALLOC (free_buffer, 1)
ಸಂರಚನಾ buffer_record *
get_new_buffer (idx_t min_size)
{
  ಸಂರಚನಾ buffer_record *new_buffer = xmalloc (ಪರಿಮಾಣ *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (ರಿಕ್ತ, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_ಸಾಲು = new_buffer->first_available = last_ಸಾಲು_ಸಂಖ್ಯೆ + 1;
  new_buffer->ಅಂಕೆ_ಸಾಲುಗಳು = 0;
  new_buffer->ಸಾಲು_start = new_buffer->curr_ಸಾಲು = ರಿಕ್ತ;
  new_buffer->next = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ಸ್ಥಿರ ಶೂನ್ಯ
save_buffer (ಸಂರಚನಾ buffer_record *buf)
{
  buf->next = ರಿಕ್ತ;
  buf->curr_ಸಾಲು = buf->ಸಾಲು_start;

  ಯದಿ (head == ರಿಕ್ತ)
    head = buf;
  ಅನ್ಯಥಾ
    {
      ಸಂರಚನಾ buffer_record *p;
      ಪ್ರತಿ (p = head; p->next; p = p->next)
        /* Do nothing. */ ;
      p->next = buf;
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

ಸ್ಥಿರ bool
load_buffer (ಶೂನ್ಯ)
{
  ಯದಿ (have_read_eof)
    ಪ್ರತ್ಯಾಗಮ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_ಎಣಿಕೆ + 1);

  ಯಾವತ್ (true)
    {
      ಸಂರಚನಾ buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      ವರ್ಣ *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      ಯದಿ (hold_ಎಣಿಕೆ)
        {
          p = mempcpy (p, hold_area, hold_ಎಣಿಕೆ);
          b->bytes_used += hold_ಎಣಿಕೆ;
          bytes_avail -= hold_ಎಣಿಕೆ;
          hold_ಎಣಿಕೆ = 0;
        }

      b->bytes_used += read_ಒಳಹರಿವು (p, bytes_avail - 1);

      ಯದಿ (record_ಸಾಲು_starts (b) != 0)
        {
          save_buffer (b);
          ಪ್ರತ್ಯಾಗಮ true;
        }

      free_buffer (b);
      ಯದಿ (have_read_eof)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ಸ್ಥಿರ intmax_t
get_first_ಸಾಲು_in_buffer (ಶೂನ್ಯ)
{
  ಯದಿ (head == ರಿಕ್ತ && !load_buffer ())
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ಸ್ಥಿರ ಸಂರಚನಾ cstring *
remove_ಸಾಲು (ಶೂನ್ಯ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ಸ್ಥಿರ ಸಂರಚನಾ buffer_record *prev_buf = ರಿಕ್ತ;

  ಸಂರಚನಾ cstring *ಸಾಲು;		/* Return value. */
  ಸಂರಚನಾ ಸಾಲು *l;		/* For convenience. */

  ಯದಿ (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = ರಿಕ್ತ;
    }

  ಯದಿ (head == ರಿಕ್ತ && !load_buffer ())
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಯದಿ (current_ಸಾಲು < head->first_available)
    current_ಸಾಲು = head->first_available;

  ++(head->first_available);

  l = head->curr_ಸಾಲು;

  ಸಾಲು = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  ಯದಿ (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_ಸಾಲು = l->next;
      ಯದಿ (head->curr_ಸಾಲು == ರಿಕ್ತ || head->curr_ಸಾಲು->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  ಪ್ರತ್ಯಾಗಮ ಸಾಲು;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ಸ್ಥಿರ ಸಂರಚನಾ cstring *
find_ಸಾಲು (intmax_t linenum)
{
  ಯದಿ (head == ರಿಕ್ತ && !load_buffer ())
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಯದಿ (linenum < head->start_ಸಾಲು)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತಿ (ಸಂರಚನಾ buffer_record *b = head;;)
    {
      ಯದಿ (linenum < b->start_ಸಾಲು + b->ಅಂಕೆ_ಸಾಲುಗಳು)
        {
          /* The line is in this buffer. */
          ಸಂರಚನಾ ಸಾಲು *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->ಸಾಲು_start;
          offset = linenum - b->start_ಸಾಲು;
          /* Find the control record. */
          ಯಾವತ್ (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          ಪ್ರತ್ಯಾಗಮ &l->starts[offset];
        }
      ಯದಿ (b->next == ರಿಕ್ತ && !load_buffer ())
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ಸ್ಥಿರ bool
no_more_ಸಾಲುಗಳು (ಶೂನ್ಯ)
{
  ಪ್ರತ್ಯಾಗಮ find_ಸಾಲು (current_ಸಾಲು + 1) == ರಿಕ್ತ;
}

/* Open NAME as standard input.  */

ಸ್ಥಿರ ಶೂನ್ಯ
set_ಒಳಹರಿವು_file (ವರ್ಣ ಧ್ರುವ *name)
{
  ಯದಿ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ಸ್ಥಿರ ಶೂನ್ಯ
write_to_file (intmax_t last_ಸಾಲು, bool ignore, ಪೂರ್ಣಾಙ್ಕ argnum)
{
  ಸಂರಚನಾ cstring *ಸಾಲು;
  intmax_t first_ಸಾಲು;		/* First available input line. */
  intmax_t ಸಾಲುಗಳು;		/* Number of lines to output. */

  first_ಸಾಲು = get_first_ಸಾಲು_in_buffer ();

  ಯದಿ (! first_ಸಾಲು || first_ಸಾಲು > last_ಸಾಲು)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  ಸಾಲುಗಳು = last_ಸಾಲು - first_ಸಾಲು;

  ಪ್ರತಿ (intmax_t i = 0; i < ಸಾಲುಗಳು; i++)
    {
      ಸಾಲು = remove_ಸಾಲು ();
      ಯದಿ (ಸಾಲು == ರಿಕ್ತ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ಯದಿ (!ignore)
        save_ಸಾಲು_to_file (ಸಾಲು);
    }
}

/* Output any lines left after all regexps have been processed. */

ಸ್ಥಿರ ಶೂನ್ಯ
dump_rest_of_file (ಶೂನ್ಯ)
{
  ಸಂರಚನಾ cstring *ಸಾಲು;

  ಯಾವತ್ ((ಸಾಲು = remove_ಸಾಲು ()) != ರಿಕ್ತ)
    save_ಸಾಲು_to_file (ಸಾಲು);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಸಾಲು_error (ಧ್ರುವ ಸಂರಚನಾ control *p, intmax_t repetition)
{
  ವರ್ಣ buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->ಸಾಲುಗಳು_required, buf)));
  ಯದಿ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ಅನ್ಯಥಾ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಸಾಲು_ಎಣಿಕೆ (ಧ್ರುವ ಸಂರಚನಾ control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_ಸಾಲು_to_save = p->ಸಾಲುಗಳು_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ಯದಿ (no_more_ಸಾಲುಗಳು () && suppress_matched)
    handle_ಸಾಲು_error (p, repetition);

  ಯದಿ (!(linenum = get_first_ಸಾಲು_in_buffer ()))
    handle_ಸಾಲು_error (p, repetition);

  ಯಾವತ್ (linenum++ < last_ಸಾಲು_to_save)
    {
      ಸಂರಚನಾ cstring *ಸಾಲು = remove_ಸಾಲು ();
      ಯದಿ (ಸಾಲು == ರಿಕ್ತ)
        handle_ಸಾಲು_error (p, repetition);
      save_ಸಾಲು_to_file (ಸಾಲು);
    }

  close_output_file ();

  ಯದಿ (suppress_matched)
    remove_ಸಾಲು ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ಯದಿ (no_more_ಸಾಲುಗಳು () && !suppress_matched)
    handle_ಸಾಲು_error (p, repetition);
}

ಸ್ಥಿರ ಶೂನ್ಯ
regexp_error (ಸಂರಚನಾ control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  ಯದಿ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ಅನ್ಯಥಾ
    fprintf (stderr, "\n");

  ಯದಿ (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_regexp (ಸಂರಚನಾ control *p, intmax_t repetition)
{
  ಸಂರಚನಾ cstring *ಸಾಲು;		/* From input file. */
  idx_t ಸಾಲು_len;		/* To make "$" in regexps work. */
  intmax_t break_ಸಾಲು;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ಯದಿ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ಯದಿ (p->offset >= 0)
    {
      ಯಾವತ್ (true)
        {
          ಸಾಲು = find_ಸಾಲು (++current_ಸಾಲು);
          ಯದಿ (ಸಾಲು == ರಿಕ್ತ)
            {
              ಯದಿ (p->repeat_forever)
                {
                  ಯದಿ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ಅನ್ಯಥಾ
                regexp_error (p, repetition, ignore);
            }
          ಸಾಲು_len = ಸಾಲು->len;
          ಯದಿ (ಸಾಲು->str[ಸಾಲು_len - 1] == '\n')
            ಸಾಲು_len--;
          ret = re_ಹುಡುಕು (&p->re_compiled, ಸಾಲು->str, ಸಾಲು_len,
                           0, ಸಾಲು_len, ರಿಕ್ತ);
          ಯದಿ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ಯದಿ (ret == -1)
            {
              ಸಾಲು = remove_ಸಾಲು ();
              ಯದಿ (!ignore)
                save_ಸಾಲು_to_file (ಸಾಲು);
            }
          ಅನ್ಯಥಾ
            ಭಙ್ಗ;
        }
    }
  ಅನ್ಯಥಾ
    {
      /* Buffer the lines. */
      ಯಾವತ್ (true)
        {
          ಸಾಲು = find_ಸಾಲು (++current_ಸಾಲು);
          ಯದಿ (ಸಾಲು == ರಿಕ್ತ)
            {
              ಯದಿ (p->repeat_forever)
                {
                  ಯದಿ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ಅನ್ಯಥಾ
                regexp_error (p, repetition, ignore);
            }
          ಸಾಲು_len = ಸಾಲು->len;
          ಯದಿ (ಸಾಲು->str[ಸಾಲು_len - 1] == '\n')
            ಸಾಲು_len--;
          ret = re_ಹುಡುಕು (&p->re_compiled, ಸಾಲು->str, ಸಾಲು_len,
                           0, ಸಾಲು_len, ರಿಕ್ತ);
          ಯದಿ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ಯದಿ (ret != -1)
            ಭಙ್ಗ;
        }
    }

  /* Account for any offset from this regexp. */
  break_ಸಾಲು = current_ಸಾಲು + p->offset;

  write_to_file (break_ಸಾಲು, ignore, p->argnum);

  ಯದಿ (!ignore)
    close_output_file ();

  ಯದಿ (p->offset > 0)
    current_ಸಾಲು = break_ಸಾಲು;

  ಯದಿ (suppress_matched)
    remove_ಸಾಲು ();
}

/* Split the input file according to the control records we have built. */

ಸ್ಥಿರ ಶೂನ್ಯ
split_file (ಶೂನ್ಯ)
{
  ಪ್ರತಿ (idx_t i = 0; i < control_used; i++)
    {
      ಯದಿ (controls[i].regexpr)
        {
          ಪ್ರತಿ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ಅನ್ಯಥಾ
        {
          ಪ್ರತಿ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_ಸಾಲು_ಎಣಿಕೆ (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_of_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ಸ್ಥಿರ ವರ್ಣ *
make_filename (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
  strcpy (filename_space, prefix);
  ಯದಿ (suffix)
    sprintf (filename_space + strlen (prefix), suffix, ಅಂಕೆ);
  ಅನ್ಯಥಾ
    sprintf (filename_space + strlen (prefix), "%0*d", digits, ಅಂಕೆ);
  ಪ್ರತ್ಯಾಗಮ filename_space;
}

/* Create the next output file. */

ಸ್ಥಿರ ಶೂನ್ಯ
create_output_file (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಙ್ಕ nfiles = files_created;
  bool fopen_ok;
  ಪೂರ್ಣಾಙ್ಕ fopen_errno;

  output_filename = make_filename (nfiles);

  ಯದಿ (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ಅನ್ಯಥಾ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != ರಿಕ್ತ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ರಿಕ್ತ);
    }

  ಯದಿ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ಸ್ಥಿರ ಶೂನ್ಯ
delete_all_files (bool in_signal_handler)
{
  ಯದಿ (! remove_files)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = files_created; 0 <= --i; )
    {
      ವರ್ಣ ಧ್ರುವ *name = make_filename (i);
      ಯದಿ (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ಸ್ಥಿರ ಶೂನ್ಯ
close_output_file (ಶೂನ್ಯ)
{
  ಯದಿ (output_stream)
    {
      ಯದಿ (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = ರಿಕ್ತ;
          cleanup_fatal ();
        }
      ಯದಿ (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = ರಿಕ್ತ;
          cleanup_fatal ();
        }
      ಯದಿ (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          ಪೂರ್ಣಾಙ್ಕ unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ರಿಕ್ತ);

          ಯದಿ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (!suppress_ಎಣಿಕೆ)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = ರಿಕ್ತ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಸಾಲು_to_file (ಧ್ರುವ ಸಂರಚನಾ cstring *ಸಾಲು)
{
  idx_t l = fwrite (ಸಾಲು->str, ಪರಿಮಾಣ (ವರ್ಣ), ಸಾಲು->len, output_stream);
  ಯದಿ (l != ಸಾಲು->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = ರಿಕ್ತ;
      cleanup_fatal ();
    }
  bytes_written += ಸಾಲು->len;
}

/* Return a new, initialized control record. */

ಸ್ಥಿರ ಸಂರಚನಾ control *
new_control_record (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ idx_t control_allocated = 0; /* Total space allocated. */
  ಸಂರಚನಾ control *p;

  ಯದಿ (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, ಪರಿಮಾಣ *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->ಸಾಲುಗಳು_required = 0;
  p->offset = 0;
  ಪ್ರತ್ಯಾಗಮ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_for_offset (ಸಂರಚನಾ control *p, ವರ್ಣ ಧ್ರುವ *str, ವರ್ಣ ಧ್ರುವ *ಅಂಕೆ)
{
  ಯದಿ (xstrtoimax (ಅಂಕೆ, ರಿಕ್ತ, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_repeat_ಎಣಿಕೆ (ಪೂರ್ಣಾಙ್ಕ argnum, ಸಂರಚನಾ control *p, ವರ್ಣ *str)
{
  ವರ್ಣ *end;

  end = str + strlen (str) - 1;
  ಯದಿ (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  ಯದಿ (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ಅನ್ಯಥಾ
    {
      uintmax_t val;
      ಯದಿ (xstrtoumax (str + 1, ರಿಕ್ತ, 10, &val, "") != LONGINT_OK
          || ckd_add (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *end = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ಸ್ಥಿರ ಸಂರಚನಾ control *
extract_regexp (ಪೂರ್ಣಾಙ್ಕ argnum, bool ignore, ವರ್ಣ ಧ್ರುವ *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  ವರ್ಣ delim = *str;
  ವರ್ಣ ಧ್ರುವ *closing_delim;
  ಸಂರಚನಾ control *p;
  ವರ್ಣ ಧ್ರುವ *err;

  closing_delim = strrchr (str + 1, delim);
  ಯದಿ (closing_delim == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = ರಿಕ್ತ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ರಿಕ್ತ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  ಯದಿ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ಯದಿ (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  ಪ್ರತ್ಯಾಗಮ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_patterns (ಪೂರ್ಣಾಙ್ಕ argc, ಪೂರ್ಣಾಙ್ಕ start, ವರ್ಣ **argv)
{
  ಸಂರಚನಾ control *p;		/* New control record created. */
  ಸ್ಥಿರ intmax_t last_val = 0;

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = start; i < argc; i++)
    {
      ಯದಿ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      ಅನ್ಯಥಾ
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          ಯದಿ (xstrtoumax (argv[i], ರಿಕ್ತ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ಯದಿ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ಯದಿ (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          ಯದಿ (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->ಸಾಲುಗಳು_required = val;
        }

      ಯದಿ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ಎಣಿಕೆ (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ಗಣನಾ { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ಸ್ಥಿರ idx_t
get_format_flags (ವರ್ಣ ಧ್ರುವ *format, ಪೂರ್ಣಾಙ್ಕ *flags_ptr)
{
  ಪೂರ್ಣಾಙ್ಕ flags = 0;

  ಪ್ರತಿ (idx_t ಎಣಿಕೆ = 0; ; ಎಣಿಕೆ++)
    {
      ವಿಕಲ್ಪ (format[ಎಣಿಕೆ])
        {
        ಪಕ್ಷ '-':
        ಪಕ್ಷ '0':
          ಭಙ್ಗ;

        ಪಕ್ಷ '\'':
          flags |= FLAG_THOUSANDS;
          ಭಙ್ಗ;

        ಪಕ್ಷ '#':
          flags |= FLAG_ALTERNATIVE;
          ಭಙ್ಗ;

        ಸ್ವಭಾವ:
          *flags_ptr = flags;
          ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_format_conv_type (ವರ್ಣ *format, ಪೂರ್ಣಾಙ್ಕ flags)
{
  unsigned ವರ್ಣ ch = *format;
  ಪೂರ್ಣಾಙ್ಕ compatible_flags = FLAG_THOUSANDS;

  ವಿಕಲ್ಪ (ch)
    {
    ಪಕ್ಷ 'd':
    ಪಕ್ಷ 'i':
      ಭಙ್ಗ;

    ಪಕ್ಷ 'u':
      *format = 'd';
      ಭಙ್ಗ;

    ಪಕ್ಷ 'o':
    ಪಕ್ಷ 'x':
    ಪಕ್ಷ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ಭಙ್ಗ;

    ಪಕ್ಷ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    ಸ್ವಭಾವ:
      ಯದಿ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ಅನ್ಯಥಾ
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ಯದಿ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ಸ್ಥಿರ idx_t
max_out (ವರ್ಣ *format)
{
  bool percent = false;

  ಪ್ರತಿ (ವರ್ಣ *f = format; *f; f++)
    ಯದಿ (*f == '%' && *++f != '%')
      {
        ಯದಿ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        ಪೂರ್ಣಾಙ್ಕ flags;
        f += get_format_flags (f, &flags);
        ಯಾವತ್ (c_isdigit (*f))
          f++;
        ಯದಿ (*f == '.')
          ಯಾವತ್ (c_isdigit (*++f))
            ಅನುವರ್ತ;
        check_format_conv_type (f, flags);
      }

  ಯದಿ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  ಪೂರ್ಣಾಙ್ಕ maxlen = snprintf (ರಿಕ್ತ, 0, format, INT_MAX);
  ಯದಿ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  ಪ್ರತ್ಯಾಗಮ maxlen;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ರಿಕ್ತ))
         != -1)
    ವಿಕಲ್ಪ (optc)
      {
      ಪಕ್ಷ 'f':
        prefix = optarg;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'b':
        suffix = optarg;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'k':
        remove_files = false;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        ಭಙ್ಗ;

      ಪಕ್ಷ 's':
      ಪಕ್ಷ 'q':
        suppress_ಎಣಿಕೆ = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'z':
        elide_empty_files = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }

  ಯದಿ (argc - optind < 2)
    {
      ಯದಿ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (ಪೂರ್ಣಾಙ್ಕ), digits));
  idx_t filename_size;
  ಯದಿ (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_ಒಳಹರಿವು_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ಗಣನಾ { nsigs = countof (term_sig) };

    ಸಂರಚನಾ sigaction act;

    sigemptyset (&caught_signals);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], ರಿಕ್ತ, &act);
        ಯದಿ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < nsigs; i++)
      ಯದಿ (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, ರಿಕ್ತ);
  }

  split_file ();

  ಯದಿ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... FILE PATTERN...\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE separated by PATTERN(s) to files 'xx00', 'xx01', ...,\n\
and output byte counts of each piece to standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard ಒಳಹರಿವು ಯದಿ FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead of %02d\n\
"));
      oputs (_("\
  -f, --prefix=PREFIX\n\
         use PREFIX instead of 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         ಕುರು not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the ಸಾಲುಗಳು matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified ಸಂಖ್ಯೆ of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         ಕುರು not print counts of output file sizes\n\
"));
      oputs (_("\
  -z, --elide-empty-files\n\
         suppress empty output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each PATTERN may be:\n\
  INTEGER            copy up to but not including specified ಸಾಲು ಸಂಖ್ಯೆ\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching ಸಾಲು\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching ಸಾಲು\n\
  {INTEGER}          repeat the previous pattern specified ಸಂಖ್ಯೆ of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A ಸಾಲು OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
