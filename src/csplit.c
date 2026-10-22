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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ <regex.h>

#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "fd-reopen.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "safe-read.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "term-sig.h"
#અન્તર્ભાવ "xdectoint.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
સંરચના control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t લીટીઓ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  પૂર્ણાંક argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  સંરચના re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
સંરચના cstring
{
  idx_t len;
  અક્ષર *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
સંરચના લીટી
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t ઉમેરો_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  સંરચના cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  સંરચના લીટી *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
સંરચના buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_લીટી;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t અંક_લીટીઓ;		/* Number of complete lines in this buffer. */
  અક્ષર *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  સંરચના લીટી *લીટી_start;	/* Head of list of pointers to lines. */
  સંરચના લીટી *curr_લીટી;	/* The line start record currently in use. */
  સંરચના buffer_record *next;
};

સ્થિર શૂન્ય close_output_file (શૂન્ય);
સ્થિર શૂન્ય create_output_file (શૂન્ય);
સ્થિર શૂન્ય delete_all_files (bool);
સ્થિર શૂન્ય save_લીટી_to_file (અચલ સંરચના cstring *લીટી);

/* Start of buffer list. */
સ્થિર સંરચના buffer_record *head = ખાલી;

/* Partially read line. */
સ્થિર અક્ષર *hold_area = ખાલી;

/* Number of bytes in 'hold_area'. */
સ્થિર idx_t hold_ગણતરી = 0;

/* Number of the last line in the buffers. */
સ્થિર intmax_t last_લીટી_સંખ્યા = 0;

/* Number of the line currently being examined. */
સ્થિર intmax_t current_લીટી = 0;

/* If true, we have read EOF. */
સ્થિર bool have_read_eof = false;

/* Name of output files. */
સ્થિર અક્ષર *અસ્થિર filename_space = ખાલી;

/* Prefix part of output file names. */
સ્થિર અક્ષર અચલ *અસ્થિર prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
સ્થિર અક્ષર *અસ્થિર suffix = ખાલી;

/* Number of digits to use in output file names. */
સ્થિર પૂર્ણાંક અસ્થિર digits = 2;

/* Number of files created so far. */
સ્થિર પૂર્ણાંક અસ્થિર files_created = 0;

/* Number of bytes written to current file. */
સ્થિર intmax_t bytes_written;

/* Output file pointer. */
સ્થિર FILE *output_stream = ખાલી;

/* Output file name. */
સ્થિર અક્ષર *output_filename = ખાલી;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
સ્થિર અક્ષર **global_argv;

/* If true, do not print the count of bytes in each output file. */
સ્થિર bool suppress_ગણતરી;

/* If true, remove output files on error. */
સ્થિર bool અસ્થિર remove_files = true;

/* If true, remove all output files which have a zero length. */
સ્થિર bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
સ્થિર bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
સ્થિર સંરચના control *controls;

/* Number of elements in 'controls'. */
સ્થિર idx_t control_used;

/* The set of signals that are caught.  */
સ્થિર sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"digits", required_argument, ખાલી, 'n'},
  {"quiet", no_argument, ખાલી, 'q'},  /* Deprecated.  */
  {"silent", no_argument, ખાલી, 's'},
  {"keep-files", no_argument, ખાલી, 'k'},
  {"elide-empty-files", no_argument, ખાલી, 'z'},
  {"prefix", required_argument, ખાલી, 'f'},
  {"suffix-format", required_argument, ખાલી, 'b'},
  {"suppress-matched", no_argument, ખાલી, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

સ્થિર શૂન્ય
cleanup (શૂન્ય)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ખાલી);
}

સ્થિર _Noreturn શૂન્ય
cleanup_fatal (શૂન્ય)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

બાહ્ય શૂન્ય
xalloc_die (શૂન્ય)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

સ્થિર શૂન્ય
interrupt_handler (પૂર્ણાંક sig)
{
  delete_all_files (true);
  સંકેત (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ઉઠાવો (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

સ્થિર શૂન્ય
save_to_hold_area (અક્ષર *start, idx_t અંક)
{
  મુક્ત (hold_area);
  hold_area = start;
  hold_ગણતરી = અંક;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

સ્થિર idx_t
read_આગત (અક્ષર *dest, idx_t max_n_bytes)
{
  જો (max_n_bytes == 0)
    પાછા 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  જો (bytes_read == 0)
    have_read_eof = true;

  જો (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  પાછા bytes_read;
}

/* Initialize existing line record P. */

સ્થિર શૂન્ય
clear_લીટી_control (સંરચના લીટી *p)
{
  p->used = 0;
  p->ઉમેરો_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

સ્થિર સંરચના લીટી *
new_લીટી_control (શૂન્ય)
{
  સંરચના લીટી *p = xmalloc (કદ *p);

  p->next = ખાલી;
  clear_લીટી_control (p);

  પાછા p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

સ્થિર શૂન્ય
keep_new_લીટી (સંરચના buffer_record *b, અક્ષર *લીટી_start, idx_t લીટી_len)
{
  સંરચના લીટી *l;

  /* If there is no existing area to keep line info, get some. */
  જો (b->લીટી_start == ખાલી)
    b->લીટી_start = b->curr_લીટી = new_લીટી_control ();

  /* If existing area for lines is full, get more. */
  જો (b->curr_લીટી->used == CTRL_SIZE)
    {
      b->curr_લીટી->next = new_લીટી_control ();
      b->curr_લીટી = b->curr_લીટી->next;
    }

  l = b->curr_લીટી;

  /* Record the start of the line, and update counters. */
  l->starts[l->ઉમેરો_index].str = લીટી_start;
  l->starts[l->ઉમેરો_index].len = લીટી_len;
  l->used++;
  l->ઉમેરો_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

સ્થિર idx_t
record_લીટી_starts (સંરચના buffer_record *b)
{
  અક્ષર *લીટી_start;		/* Start of current line. */
  idx_t લીટીઓ;			/* Number of lines found. */
  idx_t લીટી_length;		/* Length of each line found. */

  જો (b->bytes_used == 0)
    પાછા 0;

  લીટીઓ = 0;
  લીટી_start = b->buffer;
  અક્ષર *buffer_end = લીટી_start + b->bytes_used;
  *buffer_end = '\n';

  જ્યારે (true)
    {
      અક્ષર *લીટી_end = rawmemchr (લીટી_start, '\n');
      જો (લીટી_end == buffer_end)
        તોડો;
      લીટી_length = લીટી_end - લીટી_start + 1;
      keep_new_લીટી (b, લીટી_start, લીટી_length);
      લીટી_start = લીટી_end + 1;
      લીટીઓ++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_ડાબું = buffer_end - લીટી_start;
  જો (bytes_ડાબું)
    {
      જો (have_read_eof)
        {
          keep_new_લીટી (b, લીટી_start, bytes_ડાબું);
          લીટીઓ++;
        }
      નહીંતર
        save_to_hold_area (ximemdup (લીટી_start, bytes_ડાબું), bytes_ડાબું);
    }

  b->અંક_લીટીઓ = લીટીઓ;
  b->first_available = b->start_લીટી = last_લીટી_સંખ્યા + 1;
  last_લીટી_સંખ્યા += લીટીઓ;

  પાછા લીટીઓ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#જો 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

સ્થિર શૂન્ય
free_buffer (સંરચના buffer_record *buf)
{
  માટે (સંરચના લીટી *l = buf->લીટી_start; l;)
    {
      સંરચના લીટી *n = l->next;
      મુક્ત (l);
      l = n;
    }
  મુક્ત (buf->buffer);
  મુક્ત (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

સ્થિર ATTRIBUTE_DEALLOC (free_buffer, 1)
સંરચના buffer_record *
get_new_buffer (idx_t min_size)
{
  સંરચના buffer_record *new_buffer = xmalloc (કદ *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (ખાલી, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_લીટી = new_buffer->first_available = last_લીટી_સંખ્યા + 1;
  new_buffer->અંક_લીટીઓ = 0;
  new_buffer->લીટી_start = new_buffer->curr_લીટી = ખાલી;
  new_buffer->next = ખાલી;

  પાછા new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

સ્થિર શૂન્ય
save_buffer (સંરચના buffer_record *buf)
{
  buf->next = ખાલી;
  buf->curr_લીટી = buf->લીટી_start;

  જો (head == ખાલી)
    head = buf;
  નહીંતર
    {
      સંરચના buffer_record *p;
      માટે (p = head; p->next; p = p->next)
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

સ્થિર bool
load_buffer (શૂન્ય)
{
  જો (have_read_eof)
    પાછા false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_ગણતરી + 1);

  જ્યારે (true)
    {
      સંરચના buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      અક્ષર *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      જો (hold_ગણતરી)
        {
          p = mempcpy (p, hold_area, hold_ગણતરી);
          b->bytes_used += hold_ગણતરી;
          bytes_avail -= hold_ગણતરી;
          hold_ગણતરી = 0;
        }

      b->bytes_used += read_આગત (p, bytes_avail - 1);

      જો (record_લીટી_starts (b) != 0)
        {
          save_buffer (b);
          પાછા true;
        }

      free_buffer (b);
      જો (have_read_eof)
        પાછા false;
      જો (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

સ્થિર intmax_t
get_first_લીટી_in_buffer (શૂન્ય)
{
  જો (head == ખાલી && !load_buffer ())
    પાછા 0;

  પાછા head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

સ્થિર સંરચના cstring *
remove_લીટી (શૂન્ય)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  સ્થિર સંરચના buffer_record *prev_buf = ખાલી;

  સંરચના cstring *લીટી;		/* Return value. */
  સંરચના લીટી *l;		/* For convenience. */

  જો (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = ખાલી;
    }

  જો (head == ખાલી && !load_buffer ())
    પાછા ખાલી;

  જો (current_લીટી < head->first_available)
    current_લીટી = head->first_available;

  ++(head->first_available);

  l = head->curr_લીટી;

  લીટી = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  જો (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_લીટી = l->next;
      જો (head->curr_લીટી == ખાલી || head->curr_લીટી->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  પાછા લીટી;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

સ્થિર સંરચના cstring *
find_લીટી (intmax_t linenum)
{
  જો (head == ખાલી && !load_buffer ())
    પાછા ખાલી;

  જો (linenum < head->start_લીટી)
    પાછા ખાલી;

  માટે (સંરચના buffer_record *b = head;;)
    {
      જો (linenum < b->start_લીટી + b->અંક_લીટીઓ)
        {
          /* The line is in this buffer. */
          સંરચના લીટી *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->લીટી_start;
          offset = linenum - b->start_લીટી;
          /* Find the control record. */
          જ્યારે (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          પાછા &l->starts[offset];
        }
      જો (b->next == ખાલી && !load_buffer ())
        પાછા ખાલી;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

સ્થિર bool
no_more_લીટીઓ (શૂન્ય)
{
  પાછા find_લીટી (current_લીટી + 1) == ખાલી;
}

/* Open NAME as standard input.  */

સ્થિર શૂન્ય
set_આગત_file (અક્ષર અચલ *name)
{
  જો (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

સ્થિર શૂન્ય
write_to_file (intmax_t last_લીટી, bool ignore, પૂર્ણાંક argnum)
{
  સંરચના cstring *લીટી;
  intmax_t first_લીટી;		/* First available input line. */
  intmax_t લીટીઓ;		/* Number of lines to output. */

  first_લીટી = get_first_લીટી_in_buffer ();

  જો (! first_લીટી || first_લીટી > last_લીટી)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  લીટીઓ = last_લીટી - first_લીટી;

  માટે (intmax_t i = 0; i < લીટીઓ; i++)
    {
      લીટી = remove_લીટી ();
      જો (લીટી == ખાલી)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      જો (!ignore)
        save_લીટી_to_file (લીટી);
    }
}

/* Output any lines left after all regexps have been processed. */

સ્થિર શૂન્ય
dump_rest_of_file (શૂન્ય)
{
  સંરચના cstring *લીટી;

  જ્યારે ((લીટી = remove_લીટી ()) != ખાલી)
    save_લીટી_to_file (લીટી);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

સ્થિર શૂન્ય
handle_લીટી_error (અચલ સંરચના control *p, intmax_t repetition)
{
  અક્ષર buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->લીટીઓ_required, buf)));
  જો (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  નહીંતર
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

સ્થિર શૂન્ય
process_લીટી_ગણતરી (અચલ સંરચના control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_લીટી_to_save = p->લીટીઓ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  જો (no_more_લીટીઓ () && suppress_matched)
    handle_લીટી_error (p, repetition);

  જો (!(linenum = get_first_લીટી_in_buffer ()))
    handle_લીટી_error (p, repetition);

  જ્યારે (linenum++ < last_લીટી_to_save)
    {
      સંરચના cstring *લીટી = remove_લીટી ();
      જો (લીટી == ખાલી)
        handle_લીટી_error (p, repetition);
      save_લીટી_to_file (લીટી);
    }

  close_output_file ();

  જો (suppress_matched)
    remove_લીટી ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  જો (no_more_લીટીઓ () && !suppress_matched)
    handle_લીટી_error (p, repetition);
}

સ્થિર શૂન્ય
regexp_error (સંરચના control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  જો (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  નહીંતર
    fprintf (stderr, "\n");

  જો (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

સ્થિર શૂન્ય
process_regexp (સંરચના control *p, intmax_t repetition)
{
  સંરચના cstring *લીટી;		/* From input file. */
  idx_t લીટી_len;		/* To make "$" in regexps work. */
  intmax_t break_લીટી;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  જો (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  જો (p->offset >= 0)
    {
      જ્યારે (true)
        {
          લીટી = find_લીટી (++current_લીટી);
          જો (લીટી == ખાલી)
            {
              જો (p->repeat_forever)
                {
                  જો (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              નહીંતર
                regexp_error (p, repetition, ignore);
            }
          લીટી_len = લીટી->len;
          જો (લીટી->str[લીટી_len - 1] == '\n')
            લીટી_len--;
          ret = re_શોધ (&p->re_compiled, લીટી->str, લીટી_len,
                           0, લીટી_len, ખાલી);
          જો (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          જો (ret == -1)
            {
              લીટી = remove_લીટી ();
              જો (!ignore)
                save_લીટી_to_file (લીટી);
            }
          નહીંતર
            તોડો;
        }
    }
  નહીંતર
    {
      /* Buffer the lines. */
      જ્યારે (true)
        {
          લીટી = find_લીટી (++current_લીટી);
          જો (લીટી == ખાલી)
            {
              જો (p->repeat_forever)
                {
                  જો (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              નહીંતર
                regexp_error (p, repetition, ignore);
            }
          લીટી_len = લીટી->len;
          જો (લીટી->str[લીટી_len - 1] == '\n')
            લીટી_len--;
          ret = re_શોધ (&p->re_compiled, લીટી->str, લીટી_len,
                           0, લીટી_len, ખાલી);
          જો (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          જો (ret != -1)
            તોડો;
        }
    }

  /* Account for any offset from this regexp. */
  break_લીટી = current_લીટી + p->offset;

  write_to_file (break_લીટી, ignore, p->argnum);

  જો (!ignore)
    close_output_file ();

  જો (p->offset > 0)
    current_લીટી = break_લીટી;

  જો (suppress_matched)
    remove_લીટી ();
}

/* Split the input file according to the control records we have built. */

સ્થિર શૂન્ય
split_file (શૂન્ય)
{
  માટે (idx_t i = 0; i < control_used; i++)
    {
      જો (controls[i].regexpr)
        {
          માટે (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      નહીંતર
        {
          માટે (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_લીટી_ગણતરી (&controls[i], j);
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

સ્થિર અક્ષર *
make_filename (પૂર્ણાંક અંક)
{
  strcpy (filename_space, prefix);
  જો (suffix)
    sprintf (filename_space + strlen (prefix), suffix, અંક);
  નહીંતર
    sprintf (filename_space + strlen (prefix), "%0*d", digits, અંક);
  પાછા filename_space;
}

/* Create the next output file. */

સ્થિર શૂન્ય
create_output_file (શૂન્ય)
{
  પૂર્ણાંક nfiles = files_created;
  bool fopen_ok;
  પૂર્ણાંક fopen_errno;

  output_filename = make_filename (nfiles);

  જો (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  નહીંતર
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != ખાલી);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ખાલી);
    }

  જો (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

સ્થિર શૂન્ય
delete_all_files (bool in_signal_handler)
{
  જો (! remove_files)
    પાછા;

  માટે (પૂર્ણાંક i = files_created; 0 <= --i; )
    {
      અક્ષર અચલ *name = make_filename (i);
      જો (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

સ્થિર શૂન્ય
close_output_file (શૂન્ય)
{
  જો (output_stream)
    {
      જો (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = ખાલી;
          cleanup_fatal ();
        }
      જો (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = ખાલી;
          cleanup_fatal ();
        }
      જો (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          પૂર્ણાંક unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ખાલી);

          જો (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      નહીંતર
        {
          જો (!suppress_ગણતરી)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = ખાલી;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

સ્થિર શૂન્ય
save_લીટી_to_file (અચલ સંરચના cstring *લીટી)
{
  idx_t l = fwrite (લીટી->str, કદ (અક્ષર), લીટી->len, output_stream);
  જો (l != લીટી->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = ખાલી;
      cleanup_fatal ();
    }
  bytes_written += લીટી->len;
}

/* Return a new, initialized control record. */

સ્થિર સંરચના control *
new_control_record (શૂન્ય)
{
  સ્થિર idx_t control_allocated = 0; /* Total space allocated. */
  સંરચના control *p;

  જો (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, કદ *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->લીટીઓ_required = 0;
  p->offset = 0;
  પાછા p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

સ્થિર શૂન્ય
check_for_offset (સંરચના control *p, અક્ષર અચલ *str, અક્ષર અચલ *અંક)
{
  જો (xstrtoimax (અંક, ખાલી, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

સ્થિર શૂન્ય
parse_repeat_ગણતરી (પૂર્ણાંક argnum, સંરચના control *p, અક્ષર *str)
{
  અક્ષર *end;

  end = str + strlen (str) - 1;
  જો (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  જો (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  નહીંતર
    {
      uintmax_t val;
      જો (xstrtoumax (str + 1, ખાલી, 10, &val, "") != LONGINT_OK
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

સ્થિર સંરચના control *
extract_regexp (પૂર્ણાંક argnum, bool ignore, અક્ષર અચલ *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  અક્ષર delim = *str;
  અક્ષર અચલ *closing_delim;
  સંરચના control *p;
  અક્ષર અચલ *err;

  closing_delim = strrchr (str + 1, delim);
  જો (closing_delim == ખાલી)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = ખાલી;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ખાલી;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  જો (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  જો (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  પાછા p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

સ્થિર શૂન્ય
parse_patterns (પૂર્ણાંક argc, પૂર્ણાંક start, અક્ષર **argv)
{
  સંરચના control *p;		/* New control record created. */
  સ્થિર intmax_t last_val = 0;

  માટે (પૂર્ણાંક i = start; i < argc; i++)
    {
      જો (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      નહીંતર
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          જો (xstrtoumax (argv[i], ખાલી, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          જો (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          જો (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          જો (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->લીટીઓ_required = val;
        }

      જો (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ગણતરી (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ગણના { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
સ્થિર idx_t
get_format_flags (અક્ષર અચલ *format, પૂર્ણાંક *flags_ptr)
{
  પૂર્ણાંક flags = 0;

  માટે (idx_t ગણતરી = 0; ; ગણતરી++)
    {
      પસંદગી (format[ગણતરી])
        {
        સ્થિતિ '-':
        સ્થિતિ '0':
          તોડો;

        સ્થિતિ '\'':
          flags |= FLAG_THOUSANDS;
          તોડો;

        સ્થિતિ '#':
          flags |= FLAG_ALTERNATIVE;
          તોડો;

        મૂળભૂત:
          *flags_ptr = flags;
          પાછા ગણતરી;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
સ્થિર શૂન્ય
check_format_conv_type (અક્ષર *format, પૂર્ણાંક flags)
{
  unsigned અક્ષર ch = *format;
  પૂર્ણાંક compatible_flags = FLAG_THOUSANDS;

  પસંદગી (ch)
    {
    સ્થિતિ 'd':
    સ્થિતિ 'i':
      તોડો;

    સ્થિતિ 'u':
      *format = 'd';
      તોડો;

    સ્થિતિ 'o':
    સ્થિતિ 'x':
    સ્થિતિ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      તોડો;

    સ્થિતિ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    મૂળભૂત:
      જો (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      નહીંતર
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  જો (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
સ્થિર idx_t
max_out (અક્ષર *format)
{
  bool percent = false;

  માટે (અક્ષર *f = format; *f; f++)
    જો (*f == '%' && *++f != '%')
      {
        જો (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        પૂર્ણાંક flags;
        f += get_format_flags (f, &flags);
        જ્યારે (c_isdigit (*f))
          f++;
        જો (*f == '.')
          જ્યારે (c_isdigit (*++f))
            ચાલુ;
        check_format_conv_type (f, flags);
      }

  જો (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  પૂર્ણાંક maxlen = snprintf (ખાલી, 0, format, INT_MAX);
  જો (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  પાછા maxlen;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  જ્યારે ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ખાલી))
         != -1)
    પસંદગી (optc)
      {
      સ્થિતિ 'f':
        prefix = optarg;
        તોડો;

      સ્થિતિ 'b':
        suffix = optarg;
        તોડો;

      સ્થિતિ 'k':
        remove_files = false;
        તોડો;

      સ્થિતિ 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        તોડો;

      સ્થિતિ 's':
      સ્થિતિ 'q':
        suppress_ગણતરી = true;
        તોડો;

      સ્થિતિ 'z':
        elide_empty_files = true;
        તોડો;

      સ્થિતિ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        તોડો;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  જો (argc - optind < 2)
    {
      જો (argc <= optind)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (પૂર્ણાંક), digits));
  idx_t filename_size;
  જો (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_આગત_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ગણના { nsigs = countof (term_sig) };

    સંરચના sigaction act;

    sigemptyset (&caught_signals);
    માટે (પૂર્ણાંક i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], ખાલી, &act);
        જો (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    માટે (પૂર્ણાંક i = 0; i < nsigs; i++)
      જો (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, ખાલી);
  }

  split_file ();

  જો (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  પાછા EXIT_SUCCESS;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
Read standard આગત જો FILE is -\n\
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
         કરો not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the લીટીઓ matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified સંખ્યા of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         કરો not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified લીટી સંખ્યા\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching લીટી\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching લીટી\n\
  {INTEGER}          repeat the previous pattern specified સંખ્યા of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A લીટી OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
