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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ <regex.h>

#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "fd-reopen.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "safe-read.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "term-sig.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
ସଂରଚନା control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t ଧାଡ଼ିଗୁଡ଼ିକ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ସଂରଚନା re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
ସଂରଚନା cstring
{
  idx_t len;
  ଅକ୍ଷର *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ସଂରଚନା ଧାଡ଼ି
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t ଭର୍ତ୍ତି_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  ସଂରଚନା cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  ସଂରଚନା ଧାଡ଼ି *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ସଂରଚନା buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_ଧାଡ଼ି;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ;		/* Number of complete lines in this buffer. */
  ଅକ୍ଷର *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  ସଂରଚନା ଧାଡ଼ି *ଧାଡ଼ି_start;	/* Head of list of pointers to lines. */
  ସଂରଚନା ଧାଡ଼ି *curr_ଧାଡ଼ି;	/* The line start record currently in use. */
  ସଂରଚନା buffer_record *next;
};

ସ୍ଥିର ଶୂନ୍ୟ close_output_file (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ create_output_file (ଶୂନ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ delete_all_files (bool);
ସ୍ଥିର ଶୂନ୍ୟ save_ଧାଡ଼ି_to_file (ଅଚଳ ସଂରଚନା cstring *ଧାଡ଼ି);

/* Start of buffer list. */
ସ୍ଥିର ସଂରଚନା buffer_record *head = ଖାଲି;

/* Partially read line. */
ସ୍ଥିର ଅକ୍ଷର *hold_area = ଖାଲି;

/* Number of bytes in 'hold_area'. */
ସ୍ଥିର idx_t hold_ଗଣନା = 0;

/* Number of the last line in the buffers. */
ସ୍ଥିର intmax_t last_ଧାଡ଼ି_ସଂଖ୍ୟା = 0;

/* Number of the line currently being examined. */
ସ୍ଥିର intmax_t current_ଧାଡ଼ି = 0;

/* If true, we have read EOF. */
ସ୍ଥିର bool have_read_eof = false;

/* Name of output files. */
ସ୍ଥିର ଅକ୍ଷର *ଅସ୍ଥିର filename_space = ଖାଲି;

/* Prefix part of output file names. */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅସ୍ଥିର prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
ସ୍ଥିର ଅକ୍ଷର *ଅସ୍ଥିର suffix = ଖାଲି;

/* Number of digits to use in output file names. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅସ୍ଥିର digits = 2;

/* Number of files created so far. */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅସ୍ଥିର files_created = 0;

/* Number of bytes written to current file. */
ସ୍ଥିର intmax_t bytes_written;

/* Output file pointer. */
ସ୍ଥିର FILE *output_stream = ଖାଲି;

/* Output file name. */
ସ୍ଥିର ଅକ୍ଷର *output_filename = ଖାଲି;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ସ୍ଥିର ଅକ୍ଷର **global_argv;

/* If true, do not print the count of bytes in each output file. */
ସ୍ଥିର bool suppress_ଗଣନା;

/* If true, remove output files on error. */
ସ୍ଥିର bool ଅସ୍ଥିର remove_files = true;

/* If true, remove all output files which have a zero length. */
ସ୍ଥିର bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
ସ୍ଥିର bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ସ୍ଥିର ସଂରଚନା control *controls;

/* Number of elements in 'controls'. */
ସ୍ଥିର idx_t control_used;

/* The set of signals that are caught.  */
ସ୍ଥିର sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"digits", required_argument, ଖାଲି, 'n'},
  {"quiet", no_argument, ଖାଲି, 'q'},  /* Deprecated.  */
  {"silent", no_argument, ଖାଲି, 's'},
  {"keep-files", no_argument, ଖାଲି, 'k'},
  {"elide-empty-files", no_argument, ଖାଲି, 'z'},
  {"prefix", required_argument, ଖାଲି, 'f'},
  {"suffix-format", required_argument, ଖାଲି, 'b'},
  {"suppress-matched", no_argument, ଖାଲି, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ସ୍ଥିର ଶୂନ୍ୟ
cleanup (ଶୂନ୍ୟ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ଖାଲି);
}

ସ୍ଥିର _Noreturn ଶୂନ୍ୟ
cleanup_fatal (ଶୂନ୍ୟ)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

ବାହ୍ୟ ଶୂନ୍ୟ
xalloc_die (ଶୂନ୍ୟ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ସ୍ଥିର ଶୂନ୍ୟ
interrupt_handler (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig)
{
  delete_all_files (true);
  ସଙ୍କେତ (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ଉଠାଅ (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ସ୍ଥିର ଶୂନ୍ୟ
save_to_hold_area (ଅକ୍ଷର *start, idx_t ଅଙ୍କ)
{
  ମୁକ୍ତ (hold_area);
  hold_area = start;
  hold_ଗଣନା = ଅଙ୍କ;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ସ୍ଥିର idx_t
read_ନିବେଶ (ଅକ୍ଷର *dest, idx_t max_n_bytes)
{
  ଯଦି (max_n_bytes == 0)
    ଫେରନ୍ତୁ 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  ଯଦି (bytes_read == 0)
    have_read_eof = true;

  ଯଦି (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ଫେରନ୍ତୁ bytes_read;
}

/* Initialize existing line record P. */

ସ୍ଥିର ଶୂନ୍ୟ
clear_ଧାଡ଼ି_control (ସଂରଚନା ଧାଡ଼ି *p)
{
  p->used = 0;
  p->ଭର୍ତ୍ତି_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

ସ୍ଥିର ସଂରଚନା ଧାଡ଼ି *
new_ଧାଡ଼ି_control (ଶୂନ୍ୟ)
{
  ସଂରଚନା ଧାଡ଼ି *p = xmalloc (ଆକାର *p);

  p->next = ଖାଲି;
  clear_ଧାଡ଼ି_control (p);

  ଫେରନ୍ତୁ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ସ୍ଥିର ଶୂନ୍ୟ
keep_new_ଧାଡ଼ି (ସଂରଚନା buffer_record *b, ଅକ୍ଷର *ଧାଡ଼ି_start, idx_t ଧାଡ଼ି_len)
{
  ସଂରଚନା ଧାଡ଼ି *l;

  /* If there is no existing area to keep line info, get some. */
  ଯଦି (b->ଧାଡ଼ି_start == ଖାଲି)
    b->ଧାଡ଼ି_start = b->curr_ଧାଡ଼ି = new_ଧାଡ଼ି_control ();

  /* If existing area for lines is full, get more. */
  ଯଦି (b->curr_ଧାଡ଼ି->used == CTRL_SIZE)
    {
      b->curr_ଧାଡ଼ି->next = new_ଧାଡ଼ି_control ();
      b->curr_ଧାଡ଼ି = b->curr_ଧାଡ଼ି->next;
    }

  l = b->curr_ଧାଡ଼ି;

  /* Record the start of the line, and update counters. */
  l->starts[l->ଭର୍ତ୍ତି_index].str = ଧାଡ଼ି_start;
  l->starts[l->ଭର୍ତ୍ତି_index].len = ଧାଡ଼ି_len;
  l->used++;
  l->ଭର୍ତ୍ତି_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ସ୍ଥିର idx_t
record_ଧାଡ଼ି_starts (ସଂରଚନା buffer_record *b)
{
  ଅକ୍ଷର *ଧାଡ଼ି_start;		/* Start of current line. */
  idx_t ଧାଡ଼ିଗୁଡ଼ିକ;			/* Number of lines found. */
  idx_t ଧାଡ଼ି_length;		/* Length of each line found. */

  ଯଦି (b->bytes_used == 0)
    ଫେରନ୍ତୁ 0;

  ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  ଧାଡ଼ି_start = b->buffer;
  ଅକ୍ଷର *buffer_end = ଧାଡ଼ି_start + b->bytes_used;
  *buffer_end = '\n';

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *ଧାଡ଼ି_end = rawmemchr (ଧାଡ଼ି_start, '\n');
      ଯଦି (ଧାଡ଼ି_end == buffer_end)
        ଭାଙ୍ଗ;
      ଧାଡ଼ି_length = ଧାଡ଼ି_end - ଧାଡ଼ି_start + 1;
      keep_new_ଧାଡ଼ି (b, ଧାଡ଼ି_start, ଧାଡ଼ି_length);
      ଧାଡ଼ି_start = ଧାଡ଼ି_end + 1;
      ଧାଡ଼ିଗୁଡ଼ିକ++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_ବାମ = buffer_end - ଧାଡ଼ି_start;
  ଯଦି (bytes_ବାମ)
    {
      ଯଦି (have_read_eof)
        {
          keep_new_ଧାଡ଼ି (b, ଧାଡ଼ି_start, bytes_ବାମ);
          ଧାଡ଼ିଗୁଡ଼ିକ++;
        }
      ଅନ୍ୟଥା
        save_to_hold_area (ximemdup (ଧାଡ଼ି_start, bytes_ବାମ), bytes_ବାମ);
    }

  b->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ = ଧାଡ଼ିଗୁଡ଼ିକ;
  b->first_available = b->start_ଧାଡ଼ି = last_ଧାଡ଼ି_ସଂଖ୍ୟା + 1;
  last_ଧାଡ଼ି_ସଂଖ୍ୟା += ଧାଡ଼ିଗୁଡ଼ିକ;

  ଫେରନ୍ତୁ ଧାଡ଼ିଗୁଡ଼ିକ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ଯଦି 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ସ୍ଥିର ଶୂନ୍ୟ
free_buffer (ସଂରଚନା buffer_record *buf)
{
  ପାଇଁ (ସଂରଚନା ଧାଡ଼ି *l = buf->ଧାଡ଼ି_start; l;)
    {
      ସଂରଚନା ଧାଡ଼ି *n = l->next;
      ମୁକ୍ତ (l);
      l = n;
    }
  ମୁକ୍ତ (buf->buffer);
  ମୁକ୍ତ (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ସ୍ଥିର ATTRIBUTE_DEALLOC (free_buffer, 1)
ସଂରଚନା buffer_record *
get_new_buffer (idx_t min_size)
{
  ସଂରଚନା buffer_record *new_buffer = xmalloc (ଆକାର *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (ଖାଲି, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_ଧାଡ଼ି = new_buffer->first_available = last_ଧାଡ଼ି_ସଂଖ୍ୟା + 1;
  new_buffer->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  new_buffer->ଧାଡ଼ି_start = new_buffer->curr_ଧାଡ଼ି = ଖାଲି;
  new_buffer->next = ଖାଲି;

  ଫେରନ୍ତୁ new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ସ୍ଥିର ଶୂନ୍ୟ
save_buffer (ସଂରଚନା buffer_record *buf)
{
  buf->next = ଖାଲି;
  buf->curr_ଧାଡ଼ି = buf->ଧାଡ଼ି_start;

  ଯଦି (head == ଖାଲି)
    head = buf;
  ଅନ୍ୟଥା
    {
      ସଂରଚନା buffer_record *p;
      ପାଇଁ (p = head; p->next; p = p->next)
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

ସ୍ଥିର bool
load_buffer (ଶୂନ୍ୟ)
{
  ଯଦି (have_read_eof)
    ଫେରନ୍ତୁ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_ଗଣନା + 1);

  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      ଅକ୍ଷର *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      ଯଦି (hold_ଗଣନା)
        {
          p = mempcpy (p, hold_area, hold_ଗଣନା);
          b->bytes_used += hold_ଗଣନା;
          bytes_avail -= hold_ଗଣନା;
          hold_ଗଣନା = 0;
        }

      b->bytes_used += read_ନିବେଶ (p, bytes_avail - 1);

      ଯଦି (record_ଧାଡ଼ି_starts (b) != 0)
        {
          save_buffer (b);
          ଫେରନ୍ତୁ true;
        }

      free_buffer (b);
      ଯଦି (have_read_eof)
        ଫେରନ୍ତୁ false;
      ଯଦି (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ସ୍ଥିର intmax_t
get_first_ଧାଡ଼ି_in_buffer (ଶୂନ୍ୟ)
{
  ଯଦି (head == ଖାଲି && !load_buffer ())
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ସ୍ଥିର ସଂରଚନା cstring *
remove_ଧାଡ଼ି (ଶୂନ୍ୟ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ସ୍ଥିର ସଂରଚନା buffer_record *prev_buf = ଖାଲି;

  ସଂରଚନା cstring *ଧାଡ଼ି;		/* Return value. */
  ସଂରଚନା ଧାଡ଼ି *l;		/* For convenience. */

  ଯଦି (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = ଖାଲି;
    }

  ଯଦି (head == ଖାଲି && !load_buffer ())
    ଫେରନ୍ତୁ ଖାଲି;

  ଯଦି (current_ଧାଡ଼ି < head->first_available)
    current_ଧାଡ଼ି = head->first_available;

  ++(head->first_available);

  l = head->curr_ଧାଡ଼ି;

  ଧାଡ଼ି = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  ଯଦି (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_ଧାଡ଼ି = l->next;
      ଯଦି (head->curr_ଧାଡ଼ି == ଖାଲି || head->curr_ଧାଡ଼ି->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  ଫେରନ୍ତୁ ଧାଡ଼ି;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ସ୍ଥିର ସଂରଚନା cstring *
find_ଧାଡ଼ି (intmax_t linenum)
{
  ଯଦି (head == ଖାଲି && !load_buffer ())
    ଫେରନ୍ତୁ ଖାଲି;

  ଯଦି (linenum < head->start_ଧାଡ଼ି)
    ଫେରନ୍ତୁ ଖାଲି;

  ପାଇଁ (ସଂରଚନା buffer_record *b = head;;)
    {
      ଯଦି (linenum < b->start_ଧାଡ଼ି + b->ଅଙ୍କ_ଧାଡ଼ିଗୁଡ଼ିକ)
        {
          /* The line is in this buffer. */
          ସଂରଚନା ଧାଡ଼ି *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->ଧାଡ଼ି_start;
          offset = linenum - b->start_ଧାଡ଼ି;
          /* Find the control record. */
          ଯେତେବେଳେ (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          ଫେରନ୍ତୁ &l->starts[offset];
        }
      ଯଦି (b->next == ଖାଲି && !load_buffer ())
        ଫେରନ୍ତୁ ଖାଲି;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ସ୍ଥିର bool
no_more_ଧାଡ଼ିଗୁଡ଼ିକ (ଶୂନ୍ୟ)
{
  ଫେରନ୍ତୁ find_ଧାଡ଼ି (current_ଧାଡ଼ି + 1) == ଖାଲି;
}

/* Open NAME as standard input.  */

ସ୍ଥିର ଶୂନ୍ୟ
set_ନିବେଶ_file (ଅକ୍ଷର ଅଚଳ *name)
{
  ଯଦି (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ସ୍ଥିର ଶୂନ୍ୟ
write_to_file (intmax_t last_ଧାଡ଼ି, bool ignore, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argnum)
{
  ସଂରଚନା cstring *ଧାଡ଼ି;
  intmax_t first_ଧାଡ଼ି;		/* First available input line. */
  intmax_t ଧାଡ଼ିଗୁଡ଼ିକ;		/* Number of lines to output. */

  first_ଧାଡ଼ି = get_first_ଧାଡ଼ି_in_buffer ();

  ଯଦି (! first_ଧାଡ଼ି || first_ଧାଡ଼ି > last_ଧାଡ଼ି)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  ଧାଡ଼ିଗୁଡ଼ିକ = last_ଧାଡ଼ି - first_ଧାଡ଼ି;

  ପାଇଁ (intmax_t i = 0; i < ଧାଡ଼ିଗୁଡ଼ିକ; i++)
    {
      ଧାଡ଼ି = remove_ଧାଡ଼ି ();
      ଯଦି (ଧାଡ଼ି == ଖାଲି)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ଯଦି (!ignore)
        save_ଧାଡ଼ି_to_file (ଧାଡ଼ି);
    }
}

/* Output any lines left after all regexps have been processed. */

ସ୍ଥିର ଶୂନ୍ୟ
dump_rest_of_file (ଶୂନ୍ୟ)
{
  ସଂରଚନା cstring *ଧାଡ଼ି;

  ଯେତେବେଳେ ((ଧାଡ଼ି = remove_ଧାଡ଼ି ()) != ଖାଲି)
    save_ଧାଡ଼ି_to_file (ଧାଡ଼ି);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ସ୍ଥିର ଶୂନ୍ୟ
handle_ଧାଡ଼ି_error (ଅଚଳ ସଂରଚନା control *p, intmax_t repetition)
{
  ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->ଧାଡ଼ିଗୁଡ଼ିକ_required, buf)));
  ଯଦି (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ଅନ୍ୟଥା
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ସ୍ଥିର ଶୂନ୍ୟ
process_ଧାଡ଼ି_ଗଣନା (ଅଚଳ ସଂରଚନା control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_ଧାଡ଼ି_to_save = p->ଧାଡ଼ିଗୁଡ଼ିକ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ଯଦି (no_more_ଧାଡ଼ିଗୁଡ଼ିକ () && suppress_matched)
    handle_ଧାଡ଼ି_error (p, repetition);

  ଯଦି (!(linenum = get_first_ଧାଡ଼ି_in_buffer ()))
    handle_ଧାଡ଼ି_error (p, repetition);

  ଯେତେବେଳେ (linenum++ < last_ଧାଡ଼ି_to_save)
    {
      ସଂରଚନା cstring *ଧାଡ଼ି = remove_ଧାଡ଼ି ();
      ଯଦି (ଧାଡ଼ି == ଖାଲି)
        handle_ଧାଡ଼ି_error (p, repetition);
      save_ଧାଡ଼ି_to_file (ଧାଡ଼ି);
    }

  close_output_file ();

  ଯଦି (suppress_matched)
    remove_ଧାଡ଼ି ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ଯଦି (no_more_ଧାଡ଼ିଗୁଡ଼ିକ () && !suppress_matched)
    handle_ଧାଡ଼ି_error (p, repetition);
}

ସ୍ଥିର ଶୂନ୍ୟ
regexp_error (ସଂରଚନା control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  ଯଦି (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ଅନ୍ୟଥା
    fprintf (stderr, "\n");

  ଯଦି (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ସ୍ଥିର ଶୂନ୍ୟ
process_regexp (ସଂରଚନା control *p, intmax_t repetition)
{
  ସଂରଚନା cstring *ଧାଡ଼ି;		/* From input file. */
  idx_t ଧାଡ଼ି_len;		/* To make "$" in regexps work. */
  intmax_t break_ଧାଡ଼ି;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ଯଦି (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ଯଦି (p->offset >= 0)
    {
      ଯେତେବେଳେ (true)
        {
          ଧାଡ଼ି = find_ଧାଡ଼ି (++current_ଧାଡ଼ି);
          ଯଦି (ଧାଡ଼ି == ଖାଲି)
            {
              ଯଦି (p->repeat_forever)
                {
                  ଯଦି (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ଅନ୍ୟଥା
                regexp_error (p, repetition, ignore);
            }
          ଧାଡ଼ି_len = ଧାଡ଼ି->len;
          ଯଦି (ଧାଡ଼ି->str[ଧାଡ଼ି_len - 1] == '\n')
            ଧାଡ଼ି_len--;
          ret = re_ସନ୍ଧାନ (&p->re_compiled, ଧାଡ଼ି->str, ଧାଡ଼ି_len,
                           0, ଧାଡ଼ି_len, ଖାଲି);
          ଯଦି (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ଯଦି (ret == -1)
            {
              ଧାଡ଼ି = remove_ଧାଡ଼ି ();
              ଯଦି (!ignore)
                save_ଧାଡ଼ି_to_file (ଧାଡ଼ି);
            }
          ଅନ୍ୟଥା
            ଭାଙ୍ଗ;
        }
    }
  ଅନ୍ୟଥା
    {
      /* Buffer the lines. */
      ଯେତେବେଳେ (true)
        {
          ଧାଡ଼ି = find_ଧାଡ଼ି (++current_ଧାଡ଼ି);
          ଯଦି (ଧାଡ଼ି == ଖାଲି)
            {
              ଯଦି (p->repeat_forever)
                {
                  ଯଦି (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ଅନ୍ୟଥା
                regexp_error (p, repetition, ignore);
            }
          ଧାଡ଼ି_len = ଧାଡ଼ି->len;
          ଯଦି (ଧାଡ଼ି->str[ଧାଡ଼ି_len - 1] == '\n')
            ଧାଡ଼ି_len--;
          ret = re_ସନ୍ଧାନ (&p->re_compiled, ଧାଡ଼ି->str, ଧାଡ଼ି_len,
                           0, ଧାଡ଼ି_len, ଖାଲି);
          ଯଦି (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ଯଦି (ret != -1)
            ଭାଙ୍ଗ;
        }
    }

  /* Account for any offset from this regexp. */
  break_ଧାଡ଼ି = current_ଧାଡ଼ି + p->offset;

  write_to_file (break_ଧାଡ଼ି, ignore, p->argnum);

  ଯଦି (!ignore)
    close_output_file ();

  ଯଦି (p->offset > 0)
    current_ଧାଡ଼ି = break_ଧାଡ଼ି;

  ଯଦି (suppress_matched)
    remove_ଧାଡ଼ି ();
}

/* Split the input file according to the control records we have built. */

ସ୍ଥିର ଶୂନ୍ୟ
split_file (ଶୂନ୍ୟ)
{
  ପାଇଁ (idx_t i = 0; i < control_used; i++)
    {
      ଯଦି (controls[i].regexpr)
        {
          ପାଇଁ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ଅନ୍ୟଥା
        {
          ପାଇଁ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_ଧାଡ଼ି_ଗଣନା (&controls[i], j);
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

ସ୍ଥିର ଅକ୍ଷର *
make_filename (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
  strcpy (filename_space, prefix);
  ଯଦି (suffix)
    sprintf (filename_space + strlen (prefix), suffix, ଅଙ୍କ);
  ଅନ୍ୟଥା
    sprintf (filename_space + strlen (prefix), "%0*d", digits, ଅଙ୍କ);
  ଫେରନ୍ତୁ filename_space;
}

/* Create the next output file. */

ସ୍ଥିର ଶୂନ୍ୟ
create_output_file (ଶୂନ୍ୟ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା nfiles = files_created;
  bool fopen_ok;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fopen_errno;

  output_filename = make_filename (nfiles);

  ଯଦି (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ଅନ୍ୟଥା
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != ଖାଲି);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ଖାଲି);
    }

  ଯଦି (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ସ୍ଥିର ଶୂନ୍ୟ
delete_all_files (bool in_signal_handler)
{
  ଯଦି (! remove_files)
    ଫେରନ୍ତୁ;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = files_created; 0 <= --i; )
    {
      ଅକ୍ଷର ଅଚଳ *name = make_filename (i);
      ଯଦି (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ସ୍ଥିର ଶୂନ୍ୟ
close_output_file (ଶୂନ୍ୟ)
{
  ଯଦି (output_stream)
    {
      ଯଦି (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = ଖାଲି;
          cleanup_fatal ();
        }
      ଯଦି (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = ଖାଲି;
          cleanup_fatal ();
        }
      ଯଦି (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ଖାଲି);

          ଯଦି (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (!suppress_ଗଣନା)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = ଖାଲି;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ସ୍ଥିର ଶୂନ୍ୟ
save_ଧାଡ଼ି_to_file (ଅଚଳ ସଂରଚନା cstring *ଧାଡ଼ି)
{
  idx_t l = fwrite (ଧାଡ଼ି->str, ଆକାର (ଅକ୍ଷର), ଧାଡ଼ି->len, output_stream);
  ଯଦି (l != ଧାଡ଼ି->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = ଖାଲି;
      cleanup_fatal ();
    }
  bytes_written += ଧାଡ଼ି->len;
}

/* Return a new, initialized control record. */

ସ୍ଥିର ସଂରଚନା control *
new_control_record (ଶୂନ୍ୟ)
{
  ସ୍ଥିର idx_t control_allocated = 0; /* Total space allocated. */
  ସଂରଚନା control *p;

  ଯଦି (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, ଆକାର *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->ଧାଡ଼ିଗୁଡ଼ିକ_required = 0;
  p->offset = 0;
  ଫେରନ୍ତୁ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ସ୍ଥିର ଶୂନ୍ୟ
check_for_offset (ସଂରଚନା control *p, ଅକ୍ଷର ଅଚଳ *str, ଅକ୍ଷର ଅଚଳ *ଅଙ୍କ)
{
  ଯଦି (xstrtoimax (ଅଙ୍କ, ଖାଲି, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ସ୍ଥିର ଶୂନ୍ୟ
parse_repeat_ଗଣନା (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argnum, ସଂରଚନା control *p, ଅକ୍ଷର *str)
{
  ଅକ୍ଷର *end;

  end = str + strlen (str) - 1;
  ଯଦି (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  ଯଦି (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ଅନ୍ୟଥା
    {
      uintmax_t val;
      ଯଦି (xstrtoumax (str + 1, ଖାଲି, 10, &val, "") != LONGINT_OK
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

ସ୍ଥିର ସଂରଚନା control *
extract_regexp (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argnum, bool ignore, ଅକ୍ଷର ଅଚଳ *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  ଅକ୍ଷର delim = *str;
  ଅକ୍ଷର ଅଚଳ *closing_delim;
  ସଂରଚନା control *p;
  ଅକ୍ଷର ଅଚଳ *err;

  closing_delim = strrchr (str + 1, delim);
  ଯଦି (closing_delim == ଖାଲି)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = ଖାଲି;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ଖାଲି;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  ଯଦି (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ଯଦି (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  ଫେରନ୍ତୁ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ସ୍ଥିର ଶୂନ୍ୟ
parse_patterns (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ଅକ୍ଷର **argv)
{
  ସଂରଚନା control *p;		/* New control record created. */
  ସ୍ଥିର intmax_t last_val = 0;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start; i < argc; i++)
    {
      ଯଦି (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      ଅନ୍ୟଥା
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          ଯଦି (xstrtoumax (argv[i], ଖାଲି, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ଯଦି (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ଯଦି (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          ଯଦି (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->ଧାଡ଼ିଗୁଡ଼ିକ_required = val;
        }

      ଯଦି (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ଗଣନା (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ଗଣନା { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ସ୍ଥିର idx_t
get_format_flags (ଅକ୍ଷର ଅଚଳ *format, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *flags_ptr)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = 0;

  ପାଇଁ (idx_t ଗଣନା_2 = 0; ; ଗଣନା_2++)
    {
      ବାଛ (format[ଗଣନା_2])
        {
        ସ୍ଥିତି '-':
        ସ୍ଥିତି '0':
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '\'':
          flags |= FLAG_THOUSANDS;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '#':
          flags |= FLAG_ALTERNATIVE;
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          *flags_ptr = flags;
          ଫେରନ୍ତୁ ଗଣନା_2;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ସ୍ଥିର ଶୂନ୍ୟ
check_format_conv_type (ଅକ୍ଷର *format, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
  unsigned ଅକ୍ଷର ch = *format;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା compatible_flags = FLAG_THOUSANDS;

  ବାଛ (ch)
    {
    ସ୍ଥିତି 'd':
    ସ୍ଥିତି 'i':
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'u':
      *format = 'd';
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'o':
    ସ୍ଥିତି 'x':
    ସ୍ଥିତି 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଯଦି (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ଅନ୍ୟଥା
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ଯଦି (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ସ୍ଥିର idx_t
max_out (ଅକ୍ଷର *format)
{
  bool percent = false;

  ପାଇଁ (ଅକ୍ଷର *f = format; *f; f++)
    ଯଦି (*f == '%' && *++f != '%')
      {
        ଯଦି (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
        f += get_format_flags (f, &flags);
        ଯେତେବେଳେ (c_isdigit (*f))
          f++;
        ଯଦି (*f == '.')
          ଯେତେବେଳେ (c_isdigit (*++f))
            ଜାରି;
        check_format_conv_type (f, flags);
      }

  ଯଦି (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxlen = snprintf (ଖାଲି, 0, format, INT_MAX);
  ଯଦି (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  ଫେରନ୍ତୁ maxlen;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ଖାଲି))
         != -1)
    ବାଛ (optc)
      {
      ସ୍ଥିତି 'f':
        prefix = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'b':
        suffix = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'k':
        remove_files = false;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 's':
      ସ୍ଥିତି 'q':
        suppress_ଗଣନା = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'z':
        elide_empty_files = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  ଯଦି (argc - optind < 2)
    {
      ଯଦି (argc <= optind)
        error (0, 0, _("missing operand"));
      ଅନ୍ୟଥା
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (ପୂର୍ଣ୍ଣସଂଖ୍ୟା), digits));
  idx_t filename_size;
  ଯଦି (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_ନିବେଶ_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ଗଣନା { nsigs = countof (term_sig) };

    ସଂରଚନା sigaction act;

    sigemptyset (&caught_signals);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], ଖାଲି, &act);
        ଯଦି (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < nsigs; i++)
      ଯଦି (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, ଖାଲି);
  }

  split_file ();

  ଯଦି (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
Read standard ନିବେଶ ଯଦି FILE is -\n\
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
         କର not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the ଧାଡ଼ିଗୁଡ଼ିକ matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified ସଂଖ୍ୟା of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         କର not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified ଧାଡ଼ି ସଂଖ୍ୟା\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching ଧାଡ଼ି\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching ଧାଡ଼ି\n\
  {INTEGER}          repeat the previous pattern specified ସଂଖ୍ୟା of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A ଧାଡ଼ି OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
