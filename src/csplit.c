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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>

#அந்தர்பாவ "system.h"

#அந்தர்பாவ <regex.h>

#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "fd-reopen.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "safe-read.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "term-sig.h"
#அந்தர்பாவ "xdectoint.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
கட்டமைப்பு control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t வரிகள்_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  முழு argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  கட்டமைப்பு re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
கட்டமைப்பு cstring
{
  idx_t len;
  எழுத்து *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
கட்டமைப்பு வரி
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t செருகு_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  கட்டமைப்பு cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  கட்டமைப்பு வரி *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
கட்டமைப்பு buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_வரி;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t இலக்கம்_வரிகள்;		/* Number of complete lines in this buffer. */
  எழுத்து *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  கட்டமைப்பு வரி *வரி_start;	/* Head of list of pointers to lines. */
  கட்டமைப்பு வரி *curr_வரி;	/* The line start record currently in use. */
  கட்டமைப்பு buffer_record *next;
};

நிலையான வெறுமை close_output_file (வெறுமை);
நிலையான வெறுமை create_output_file (வெறுமை);
நிலையான வெறுமை delete_all_files (bool);
நிலையான வெறுமை save_வரி_to_file (மாறிலி கட்டமைப்பு cstring *வரி);

/* Start of buffer list. */
நிலையான கட்டமைப்பு buffer_record *head = வெற்று;

/* Partially read line. */
நிலையான எழுத்து *hold_area = வெற்று;

/* Number of bytes in 'hold_area'. */
நிலையான idx_t hold_மொத்தம் = 0;

/* Number of the last line in the buffers. */
நிலையான intmax_t last_வரி_எண் = 0;

/* Number of the line currently being examined. */
நிலையான intmax_t current_வரி = 0;

/* If true, we have read EOF. */
நிலையான bool have_read_eof = false;

/* Name of output files. */
நிலையான எழுத்து *நிலையற்ற filename_space = வெற்று;

/* Prefix part of output file names. */
நிலையான எழுத்து மாறிலி *நிலையற்ற prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
நிலையான எழுத்து *நிலையற்ற suffix = வெற்று;

/* Number of digits to use in output file names. */
நிலையான முழு நிலையற்ற digits = 2;

/* Number of files created so far. */
நிலையான முழு நிலையற்ற files_created = 0;

/* Number of bytes written to current file. */
நிலையான intmax_t bytes_written;

/* Output file pointer. */
நிலையான FILE *output_stream = வெற்று;

/* Output file name. */
நிலையான எழுத்து *output_filename = வெற்று;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
நிலையான எழுத்து **global_argv;

/* If true, do not print the count of bytes in each output file. */
நிலையான bool suppress_மொத்தம்;

/* If true, remove output files on error. */
நிலையான bool நிலையற்ற remove_files = true;

/* If true, remove all output files which have a zero length. */
நிலையான bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
நிலையான bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
நிலையான கட்டமைப்பு control *controls;

/* Number of elements in 'controls'. */
நிலையான idx_t control_used;

/* The set of signals that are caught.  */
நிலையான sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"digits", required_argument, வெற்று, 'n'},
  {"quiet", no_argument, வெற்று, 'q'},  /* Deprecated.  */
  {"silent", no_argument, வெற்று, 's'},
  {"keep-files", no_argument, வெற்று, 'k'},
  {"elide-empty-files", no_argument, வெற்று, 'z'},
  {"prefix", required_argument, வெற்று, 'f'},
  {"suffix-format", required_argument, வெற்று, 'b'},
  {"suppress-matched", no_argument, வெற்று, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

நிலையான வெறுமை
cleanup (வெறுமை)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, வெற்று);
}

நிலையான _Noreturn வெறுமை
cleanup_fatal (வெறுமை)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

வெளி வெறுமை
xalloc_die (வெறுமை)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

நிலையான வெறுமை
interrupt_handler (முழு sig)
{
  delete_all_files (true);
  சமிக்ஞை (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  எழுப்பு (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

நிலையான வெறுமை
save_to_hold_area (எழுத்து *start, idx_t இலக்கம்)
{
  விடுவி (hold_area);
  hold_area = start;
  hold_மொத்தம் = இலக்கம்;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

நிலையான idx_t
read_உள்ளீடு (எழுத்து *dest, idx_t max_n_bytes)
{
  எனில் (max_n_bytes == 0)
    திரும்பு 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  எனில் (bytes_read == 0)
    have_read_eof = true;

  எனில் (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  திரும்பு bytes_read;
}

/* Initialize existing line record P. */

நிலையான வெறுமை
clear_வரி_control (கட்டமைப்பு வரி *p)
{
  p->used = 0;
  p->செருகு_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

நிலையான கட்டமைப்பு வரி *
new_வரி_control (வெறுமை)
{
  கட்டமைப்பு வரி *p = xmalloc (அளவு *p);

  p->next = வெற்று;
  clear_வரி_control (p);

  திரும்பு p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

நிலையான வெறுமை
keep_new_வரி (கட்டமைப்பு buffer_record *b, எழுத்து *வரி_start, idx_t வரி_len)
{
  கட்டமைப்பு வரி *l;

  /* If there is no existing area to keep line info, get some. */
  எனில் (b->வரி_start == வெற்று)
    b->வரி_start = b->curr_வரி = new_வரி_control ();

  /* If existing area for lines is full, get more. */
  எனில் (b->curr_வரி->used == CTRL_SIZE)
    {
      b->curr_வரி->next = new_வரி_control ();
      b->curr_வரி = b->curr_வரி->next;
    }

  l = b->curr_வரி;

  /* Record the start of the line, and update counters. */
  l->starts[l->செருகு_index].str = வரி_start;
  l->starts[l->செருகு_index].len = வரி_len;
  l->used++;
  l->செருகு_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

நிலையான idx_t
record_வரி_starts (கட்டமைப்பு buffer_record *b)
{
  எழுத்து *வரி_start;		/* Start of current line. */
  idx_t வரிகள்;			/* Number of lines found. */
  idx_t வரி_length;		/* Length of each line found. */

  எனில் (b->bytes_used == 0)
    திரும்பு 0;

  வரிகள் = 0;
  வரி_start = b->buffer;
  எழுத்து *buffer_end = வரி_start + b->bytes_used;
  *buffer_end = '\n';

  போது (true)
    {
      எழுத்து *வரி_end = rawmemchr (வரி_start, '\n');
      எனில் (வரி_end == buffer_end)
        நிறுத்து;
      வரி_length = வரி_end - வரி_start + 1;
      keep_new_வரி (b, வரி_start, வரி_length);
      வரி_start = வரி_end + 1;
      வரிகள்++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_இடது = buffer_end - வரி_start;
  எனில் (bytes_இடது)
    {
      எனில் (have_read_eof)
        {
          keep_new_வரி (b, வரி_start, bytes_இடது);
          வரிகள்++;
        }
      இல்லையெனில்
        save_to_hold_area (ximemdup (வரி_start, bytes_இடது), bytes_இடது);
    }

  b->இலக்கம்_வரிகள் = வரிகள்;
  b->first_available = b->start_வரி = last_வரி_எண் + 1;
  last_வரி_எண் += வரிகள்;

  திரும்பு வரிகள்;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#எனில் 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

நிலையான வெறுமை
free_buffer (கட்டமைப்பு buffer_record *buf)
{
  க்கு (கட்டமைப்பு வரி *l = buf->வரி_start; l;)
    {
      கட்டமைப்பு வரி *n = l->next;
      விடுவி (l);
      l = n;
    }
  விடுவி (buf->buffer);
  விடுவி (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

நிலையான ATTRIBUTE_DEALLOC (free_buffer, 1)
கட்டமைப்பு buffer_record *
get_new_buffer (idx_t min_size)
{
  கட்டமைப்பு buffer_record *new_buffer = xmalloc (அளவு *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (வெற்று, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_வரி = new_buffer->first_available = last_வரி_எண் + 1;
  new_buffer->இலக்கம்_வரிகள் = 0;
  new_buffer->வரி_start = new_buffer->curr_வரி = வெற்று;
  new_buffer->next = வெற்று;

  திரும்பு new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

நிலையான வெறுமை
save_buffer (கட்டமைப்பு buffer_record *buf)
{
  buf->next = வெற்று;
  buf->curr_வரி = buf->வரி_start;

  எனில் (head == வெற்று)
    head = buf;
  இல்லையெனில்
    {
      கட்டமைப்பு buffer_record *p;
      க்கு (p = head; p->next; p = p->next)
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

நிலையான bool
load_buffer (வெறுமை)
{
  எனில் (have_read_eof)
    திரும்பு false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_மொத்தம் + 1);

  போது (true)
    {
      கட்டமைப்பு buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      எழுத்து *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      எனில் (hold_மொத்தம்)
        {
          p = mempcpy (p, hold_area, hold_மொத்தம்);
          b->bytes_used += hold_மொத்தம்;
          bytes_avail -= hold_மொத்தம்;
          hold_மொத்தம் = 0;
        }

      b->bytes_used += read_உள்ளீடு (p, bytes_avail - 1);

      எனில் (record_வரி_starts (b) != 0)
        {
          save_buffer (b);
          திரும்பு true;
        }

      free_buffer (b);
      எனில் (have_read_eof)
        திரும்பு false;
      எனில் (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

நிலையான intmax_t
get_first_வரி_in_buffer (வெறுமை)
{
  எனில் (head == வெற்று && !load_buffer ())
    திரும்பு 0;

  திரும்பு head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

நிலையான கட்டமைப்பு cstring *
remove_வரி (வெறுமை)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  நிலையான கட்டமைப்பு buffer_record *prev_buf = வெற்று;

  கட்டமைப்பு cstring *வரி;		/* Return value. */
  கட்டமைப்பு வரி *l;		/* For convenience. */

  எனில் (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = வெற்று;
    }

  எனில் (head == வெற்று && !load_buffer ())
    திரும்பு வெற்று;

  எனில் (current_வரி < head->first_available)
    current_வரி = head->first_available;

  ++(head->first_available);

  l = head->curr_வரி;

  வரி = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  எனில் (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_வரி = l->next;
      எனில் (head->curr_வரி == வெற்று || head->curr_வரி->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  திரும்பு வரி;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

நிலையான கட்டமைப்பு cstring *
find_வரி (intmax_t linenum)
{
  எனில் (head == வெற்று && !load_buffer ())
    திரும்பு வெற்று;

  எனில் (linenum < head->start_வரி)
    திரும்பு வெற்று;

  க்கு (கட்டமைப்பு buffer_record *b = head;;)
    {
      எனில் (linenum < b->start_வரி + b->இலக்கம்_வரிகள்)
        {
          /* The line is in this buffer. */
          கட்டமைப்பு வரி *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->வரி_start;
          offset = linenum - b->start_வரி;
          /* Find the control record. */
          போது (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          திரும்பு &l->starts[offset];
        }
      எனில் (b->next == வெற்று && !load_buffer ())
        திரும்பு வெற்று;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

நிலையான bool
no_more_வரிகள் (வெறுமை)
{
  திரும்பு find_வரி (current_வரி + 1) == வெற்று;
}

/* Open NAME as standard input.  */

நிலையான வெறுமை
set_உள்ளீடு_file (எழுத்து மாறிலி *name)
{
  எனில் (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

நிலையான வெறுமை
write_to_file (intmax_t last_வரி, bool ignore, முழு argnum)
{
  கட்டமைப்பு cstring *வரி;
  intmax_t first_வரி;		/* First available input line. */
  intmax_t வரிகள்;		/* Number of lines to output. */

  first_வரி = get_first_வரி_in_buffer ();

  எனில் (! first_வரி || first_வரி > last_வரி)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  வரிகள் = last_வரி - first_வரி;

  க்கு (intmax_t i = 0; i < வரிகள்; i++)
    {
      வரி = remove_வரி ();
      எனில் (வரி == வெற்று)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      எனில் (!ignore)
        save_வரி_to_file (வரி);
    }
}

/* Output any lines left after all regexps have been processed. */

நிலையான வெறுமை
dump_rest_of_file (வெறுமை)
{
  கட்டமைப்பு cstring *வரி;

  போது ((வரி = remove_வரி ()) != வெற்று)
    save_வரி_to_file (வரி);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

நிலையான வெறுமை
handle_வரி_error (மாறிலி கட்டமைப்பு control *p, intmax_t repetition)
{
  எழுத்து buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->வரிகள்_required, buf)));
  எனில் (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  இல்லையெனில்
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

நிலையான வெறுமை
process_வரி_மொத்தம் (மாறிலி கட்டமைப்பு control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_வரி_to_save = p->வரிகள்_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  எனில் (no_more_வரிகள் () && suppress_matched)
    handle_வரி_error (p, repetition);

  எனில் (!(linenum = get_first_வரி_in_buffer ()))
    handle_வரி_error (p, repetition);

  போது (linenum++ < last_வரி_to_save)
    {
      கட்டமைப்பு cstring *வரி = remove_வரி ();
      எனில் (வரி == வெற்று)
        handle_வரி_error (p, repetition);
      save_வரி_to_file (வரி);
    }

  close_output_file ();

  எனில் (suppress_matched)
    remove_வரி ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  எனில் (no_more_வரிகள் () && !suppress_matched)
    handle_வரி_error (p, repetition);
}

நிலையான வெறுமை
regexp_error (கட்டமைப்பு control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  எனில் (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  இல்லையெனில்
    fprintf (stderr, "\n");

  எனில் (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

நிலையான வெறுமை
process_regexp (கட்டமைப்பு control *p, intmax_t repetition)
{
  கட்டமைப்பு cstring *வரி;		/* From input file. */
  idx_t வரி_len;		/* To make "$" in regexps work. */
  intmax_t break_வரி;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  எனில் (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  எனில் (p->offset >= 0)
    {
      போது (true)
        {
          வரி = find_வரி (++current_வரி);
          எனில் (வரி == வெற்று)
            {
              எனில் (p->repeat_forever)
                {
                  எனில் (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              இல்லையெனில்
                regexp_error (p, repetition, ignore);
            }
          வரி_len = வரி->len;
          எனில் (வரி->str[வரி_len - 1] == '\n')
            வரி_len--;
          ret = re_தேடல் (&p->re_compiled, வரி->str, வரி_len,
                           0, வரி_len, வெற்று);
          எனில் (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          எனில் (ret == -1)
            {
              வரி = remove_வரி ();
              எனில் (!ignore)
                save_வரி_to_file (வரி);
            }
          இல்லையெனில்
            நிறுத்து;
        }
    }
  இல்லையெனில்
    {
      /* Buffer the lines. */
      போது (true)
        {
          வரி = find_வரி (++current_வரி);
          எனில் (வரி == வெற்று)
            {
              எனில் (p->repeat_forever)
                {
                  எனில் (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              இல்லையெனில்
                regexp_error (p, repetition, ignore);
            }
          வரி_len = வரி->len;
          எனில் (வரி->str[வரி_len - 1] == '\n')
            வரி_len--;
          ret = re_தேடல் (&p->re_compiled, வரி->str, வரி_len,
                           0, வரி_len, வெற்று);
          எனில் (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          எனில் (ret != -1)
            நிறுத்து;
        }
    }

  /* Account for any offset from this regexp. */
  break_வரி = current_வரி + p->offset;

  write_to_file (break_வரி, ignore, p->argnum);

  எனில் (!ignore)
    close_output_file ();

  எனில் (p->offset > 0)
    current_வரி = break_வரி;

  எனில் (suppress_matched)
    remove_வரி ();
}

/* Split the input file according to the control records we have built. */

நிலையான வெறுமை
split_file (வெறுமை)
{
  க்கு (idx_t i = 0; i < control_used; i++)
    {
      எனில் (controls[i].regexpr)
        {
          க்கு (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      இல்லையெனில்
        {
          க்கு (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_வரி_மொத்தம் (&controls[i], j);
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

நிலையான எழுத்து *
make_filename (முழு இலக்கம்)
{
  strcpy (filename_space, prefix);
  எனில் (suffix)
    sprintf (filename_space + strlen (prefix), suffix, இலக்கம்);
  இல்லையெனில்
    sprintf (filename_space + strlen (prefix), "%0*d", digits, இலக்கம்);
  திரும்பு filename_space;
}

/* Create the next output file. */

நிலையான வெறுமை
create_output_file (வெறுமை)
{
  முழு nfiles = files_created;
  bool fopen_ok;
  முழு fopen_errno;

  output_filename = make_filename (nfiles);

  எனில் (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  இல்லையெனில்
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != வெற்று);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, வெற்று);
    }

  எனில் (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

நிலையான வெறுமை
delete_all_files (bool in_signal_handler)
{
  எனில் (! remove_files)
    திரும்பு;

  க்கு (முழு i = files_created; 0 <= --i; )
    {
      எழுத்து மாறிலி *name = make_filename (i);
      எனில் (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

நிலையான வெறுமை
close_output_file (வெறுமை)
{
  எனில் (output_stream)
    {
      எனில் (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = வெற்று;
          cleanup_fatal ();
        }
      எனில் (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = வெற்று;
          cleanup_fatal ();
        }
      எனில் (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          முழு unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, வெற்று);

          எனில் (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      இல்லையெனில்
        {
          எனில் (!suppress_மொத்தம்)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = வெற்று;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

நிலையான வெறுமை
save_வரி_to_file (மாறிலி கட்டமைப்பு cstring *வரி)
{
  idx_t l = fwrite (வரி->str, அளவு (எழுத்து), வரி->len, output_stream);
  எனில் (l != வரி->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = வெற்று;
      cleanup_fatal ();
    }
  bytes_written += வரி->len;
}

/* Return a new, initialized control record. */

நிலையான கட்டமைப்பு control *
new_control_record (வெறுமை)
{
  நிலையான idx_t control_allocated = 0; /* Total space allocated. */
  கட்டமைப்பு control *p;

  எனில் (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, அளவு *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->வரிகள்_required = 0;
  p->offset = 0;
  திரும்பு p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

நிலையான வெறுமை
check_for_offset (கட்டமைப்பு control *p, எழுத்து மாறிலி *str, எழுத்து மாறிலி *இலக்கம்)
{
  எனில் (xstrtoimax (இலக்கம், வெற்று, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

நிலையான வெறுமை
parse_repeat_மொத்தம் (முழு argnum, கட்டமைப்பு control *p, எழுத்து *str)
{
  எழுத்து *end;

  end = str + strlen (str) - 1;
  எனில் (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  எனில் (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  இல்லையெனில்
    {
      uintmax_t val;
      எனில் (xstrtoumax (str + 1, வெற்று, 10, &val, "") != LONGINT_OK
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

நிலையான கட்டமைப்பு control *
extract_regexp (முழு argnum, bool ignore, எழுத்து மாறிலி *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  எழுத்து delim = *str;
  எழுத்து மாறிலி *closing_delim;
  கட்டமைப்பு control *p;
  எழுத்து மாறிலி *err;

  closing_delim = strrchr (str + 1, delim);
  எனில் (closing_delim == வெற்று)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = வெற்று;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = வெற்று;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  எனில் (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  எனில் (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  திரும்பு p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

நிலையான வெறுமை
parse_patterns (முழு argc, முழு start, எழுத்து **argv)
{
  கட்டமைப்பு control *p;		/* New control record created. */
  நிலையான intmax_t last_val = 0;

  க்கு (முழு i = start; i < argc; i++)
    {
      எனில் (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      இல்லையெனில்
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          எனில் (xstrtoumax (argv[i], வெற்று, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          எனில் (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          எனில் (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          எனில் (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->வரிகள்_required = val;
        }

      எனில் (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_மொத்தம் (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
எண்ணிக்கை { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
நிலையான idx_t
get_format_flags (எழுத்து மாறிலி *format, முழு *flags_ptr)
{
  முழு flags = 0;

  க்கு (idx_t மொத்தம் = 0; ; மொத்தம்++)
    {
      தேர்வு (format[மொத்தம்])
        {
        நிலை '-':
        நிலை '0':
          நிறுத்து;

        நிலை '\'':
          flags |= FLAG_THOUSANDS;
          நிறுத்து;

        நிலை '#':
          flags |= FLAG_ALTERNATIVE;
          நிறுத்து;

        இயல்பு:
          *flags_ptr = flags;
          திரும்பு மொத்தம்;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
நிலையான வெறுமை
check_format_conv_type (எழுத்து *format, முழு flags)
{
  unsigned எழுத்து ch = *format;
  முழு compatible_flags = FLAG_THOUSANDS;

  தேர்வு (ch)
    {
    நிலை 'd':
    நிலை 'i':
      நிறுத்து;

    நிலை 'u':
      *format = 'd';
      நிறுத்து;

    நிலை 'o':
    நிலை 'x':
    நிலை 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      நிறுத்து;

    நிலை 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    இயல்பு:
      எனில் (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      இல்லையெனில்
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  எனில் (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
நிலையான idx_t
max_out (எழுத்து *format)
{
  bool percent = false;

  க்கு (எழுத்து *f = format; *f; f++)
    எனில் (*f == '%' && *++f != '%')
      {
        எனில் (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        முழு flags;
        f += get_format_flags (f, &flags);
        போது (c_isdigit (*f))
          f++;
        எனில் (*f == '.')
          போது (c_isdigit (*++f))
            தொடர்;
        check_format_conv_type (f, flags);
      }

  எனில் (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  முழு maxlen = snprintf (வெற்று, 0, format, INT_MAX);
  எனில் (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  திரும்பு maxlen;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  போது ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, வெற்று))
         != -1)
    தேர்வு (optc)
      {
      நிலை 'f':
        prefix = optarg;
        நிறுத்து;

      நிலை 'b':
        suffix = optarg;
        நிறுத்து;

      நிலை 'k':
        remove_files = false;
        நிறுத்து;

      நிலை 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        நிறுத்து;

      நிலை 's':
      நிலை 'q':
        suppress_மொத்தம் = true;
        நிறுத்து;

      நிலை 'z':
        elide_empty_files = true;
        நிறுத்து;

      நிலை SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        நிறுத்து;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      இயல்பு:
        usage (EXIT_FAILURE);
      }

  எனில் (argc - optind < 2)
    {
      எனில் (argc <= optind)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (முழு), digits));
  idx_t filename_size;
  எனில் (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_உள்ளீடு_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    எண்ணிக்கை { nsigs = countof (term_sig) };

    கட்டமைப்பு sigaction act;

    sigemptyset (&caught_signals);
    க்கு (முழு i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], வெற்று, &act);
        எனில் (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    க்கு (முழு i = 0; i < nsigs; i++)
      எனில் (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, வெற்று);
  }

  split_file ();

  எனில் (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  திரும்பு EXIT_SUCCESS;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
Read standard உள்ளீடு எனில் FILE is -\n\
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
         செய் not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the வரிகள் matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified எண் of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         செய் not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified வரி எண்\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching வரி\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching வரி\n\
  {INTEGER}          repeat the previous pattern specified எண் of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A வரி OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
