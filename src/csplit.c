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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>

#অন্তর্ভাব "system.h"

#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "fd-reopen.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "safe-read.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "term-sig.h"
#অন্তর্ভাব "xdectoint.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
সংরচনা control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t সারিগুলি_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  পূর্ণাঙ্ক argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  সংরচনা re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
সংরচনা cstring
{
  idx_t len;
  বর্ণ *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
সংরচনা সারি
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t ঢোকাও_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  সংরচনা cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  সংরচনা সারি *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
সংরচনা buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_সারি;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t অঙ্ক_সারিগুলি;		/* Number of complete lines in this buffer. */
  বর্ণ *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  সংরচনা সারি *সারি_start;	/* Head of list of pointers to lines. */
  সংরচনা সারি *curr_সারি;	/* The line start record currently in use. */
  সংরচনা buffer_record *next;
};

স্থির শূন্য close_output_file (শূন্য);
স্থির শূন্য create_output_file (শূন্য);
স্থির শূন্য delete_all_files (bool);
স্থির শূন্য save_সারি_to_file (ধ্রুব সংরচনা cstring *সারি);

/* Start of buffer list. */
স্থির সংরচনা buffer_record *head = রিক্ত;

/* Partially read line. */
স্থির বর্ণ *hold_area = রিক্ত;

/* Number of bytes in 'hold_area'. */
স্থির idx_t hold_গুনতি = 0;

/* Number of the last line in the buffers. */
স্থির intmax_t last_সারি_সংখ্যা = 0;

/* Number of the line currently being examined. */
স্থির intmax_t current_সারি = 0;

/* If true, we have read EOF. */
স্থির bool have_read_eof = false;

/* Name of output files. */
স্থির বর্ণ *চঞ্চল filename_space = রিক্ত;

/* Prefix part of output file names. */
স্থির বর্ণ ধ্রুব *চঞ্চল prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
স্থির বর্ণ *চঞ্চল suffix = রিক্ত;

/* Number of digits to use in output file names. */
স্থির পূর্ণাঙ্ক চঞ্চল digits = 2;

/* Number of files created so far. */
স্থির পূর্ণাঙ্ক চঞ্চল files_created = 0;

/* Number of bytes written to current file. */
স্থির intmax_t bytes_written;

/* Output file pointer. */
স্থির FILE *output_stream = রিক্ত;

/* Output file name. */
স্থির বর্ণ *output_filename = রিক্ত;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
স্থির বর্ণ **global_argv;

/* If true, do not print the count of bytes in each output file. */
স্থির bool suppress_গুনতি;

/* If true, remove output files on error. */
স্থির bool চঞ্চল remove_files = true;

/* If true, remove all output files which have a zero length. */
স্থির bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
স্থির bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
স্থির সংরচনা control *controls;

/* Number of elements in 'controls'. */
স্থির idx_t control_used;

/* The set of signals that are caught.  */
স্থির sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"digits", required_argument, রিক্ত, 'n'},
  {"quiet", no_argument, রিক্ত, 'q'},  /* Deprecated.  */
  {"silent", no_argument, রিক্ত, 's'},
  {"keep-files", no_argument, রিক্ত, 'k'},
  {"elide-empty-files", no_argument, রিক্ত, 'z'},
  {"prefix", required_argument, রিক্ত, 'f'},
  {"suffix-format", required_argument, রিক্ত, 'b'},
  {"suppress-matched", no_argument, রিক্ত, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

স্থির শূন্য
cleanup (শূন্য)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, রিক্ত);
}

স্থির _Noreturn শূন্য
cleanup_fatal (শূন্য)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

বাহ্য শূন্য
xalloc_die (শূন্য)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

স্থির শূন্য
interrupt_handler (পূর্ণাঙ্ক sig)
{
  delete_all_files (true);
  সঙ্কেত (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  উত্থাপয (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

স্থির শূন্য
save_to_hold_area (বর্ণ *start, idx_t অঙ্ক)
{
  মুক্ত (hold_area);
  hold_area = start;
  hold_গুনতি = অঙ্ক;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

স্থির idx_t
read_প্রবেশ (বর্ণ *dest, idx_t max_n_bytes)
{
  যদি (max_n_bytes == 0)
    প্রত্যাগম 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  যদি (bytes_read == 0)
    have_read_eof = true;

  যদি (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  প্রত্যাগম bytes_read;
}

/* Initialize existing line record P. */

স্থির শূন্য
clear_সারি_control (সংরচনা সারি *p)
{
  p->used = 0;
  p->ঢোকাও_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

স্থির সংরচনা সারি *
new_সারি_control (শূন্য)
{
  সংরচনা সারি *p = xmalloc (পরিমাণ *p);

  p->next = রিক্ত;
  clear_সারি_control (p);

  প্রত্যাগম p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

স্থির শূন্য
keep_new_সারি (সংরচনা buffer_record *b, বর্ণ *সারি_start, idx_t সারি_len)
{
  সংরচনা সারি *l;

  /* If there is no existing area to keep line info, get some. */
  যদি (b->সারি_start == রিক্ত)
    b->সারি_start = b->curr_সারি = new_সারি_control ();

  /* If existing area for lines is full, get more. */
  যদি (b->curr_সারি->used == CTRL_SIZE)
    {
      b->curr_সারি->next = new_সারি_control ();
      b->curr_সারি = b->curr_সারি->next;
    }

  l = b->curr_সারি;

  /* Record the start of the line, and update counters. */
  l->starts[l->ঢোকাও_index].str = সারি_start;
  l->starts[l->ঢোকাও_index].len = সারি_len;
  l->used++;
  l->ঢোকাও_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

স্থির idx_t
record_সারি_starts (সংরচনা buffer_record *b)
{
  বর্ণ *সারি_start;		/* Start of current line. */
  idx_t সারিগুলি;			/* Number of lines found. */
  idx_t সারি_length;		/* Length of each line found. */

  যদি (b->bytes_used == 0)
    প্রত্যাগম 0;

  সারিগুলি = 0;
  সারি_start = b->buffer;
  বর্ণ *buffer_end = সারি_start + b->bytes_used;
  *buffer_end = '\n';

  যাবত্ (true)
    {
      বর্ণ *সারি_end = rawmemchr (সারি_start, '\n');
      যদি (সারি_end == buffer_end)
        ভঙ্গ;
      সারি_length = সারি_end - সারি_start + 1;
      keep_new_সারি (b, সারি_start, সারি_length);
      সারি_start = সারি_end + 1;
      সারিগুলি++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_বাম = buffer_end - সারি_start;
  যদি (bytes_বাম)
    {
      যদি (have_read_eof)
        {
          keep_new_সারি (b, সারি_start, bytes_বাম);
          সারিগুলি++;
        }
      অন্যথা
        save_to_hold_area (ximemdup (সারি_start, bytes_বাম), bytes_বাম);
    }

  b->অঙ্ক_সারিগুলি = সারিগুলি;
  b->first_available = b->start_সারি = last_সারি_সংখ্যা + 1;
  last_সারি_সংখ্যা += সারিগুলি;

  প্রত্যাগম সারিগুলি;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#যদি 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

স্থির শূন্য
free_buffer (সংরচনা buffer_record *buf)
{
  প্রতি (সংরচনা সারি *l = buf->সারি_start; l;)
    {
      সংরচনা সারি *n = l->next;
      মুক্ত (l);
      l = n;
    }
  মুক্ত (buf->buffer);
  মুক্ত (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

স্থির ATTRIBUTE_DEALLOC (free_buffer, 1)
সংরচনা buffer_record *
get_new_buffer (idx_t min_size)
{
  সংরচনা buffer_record *new_buffer = xmalloc (পরিমাণ *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (রিক্ত, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_সারি = new_buffer->first_available = last_সারি_সংখ্যা + 1;
  new_buffer->অঙ্ক_সারিগুলি = 0;
  new_buffer->সারি_start = new_buffer->curr_সারি = রিক্ত;
  new_buffer->next = রিক্ত;

  প্রত্যাগম new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

স্থির শূন্য
save_buffer (সংরচনা buffer_record *buf)
{
  buf->next = রিক্ত;
  buf->curr_সারি = buf->সারি_start;

  যদি (head == রিক্ত)
    head = buf;
  অন্যথা
    {
      সংরচনা buffer_record *p;
      প্রতি (p = head; p->next; p = p->next)
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

স্থির bool
load_buffer (শূন্য)
{
  যদি (have_read_eof)
    প্রত্যাগম false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_গুনতি + 1);

  যাবত্ (true)
    {
      সংরচনা buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      বর্ণ *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      যদি (hold_গুনতি)
        {
          p = mempcpy (p, hold_area, hold_গুনতি);
          b->bytes_used += hold_গুনতি;
          bytes_avail -= hold_গুনতি;
          hold_গুনতি = 0;
        }

      b->bytes_used += read_প্রবেশ (p, bytes_avail - 1);

      যদি (record_সারি_starts (b) != 0)
        {
          save_buffer (b);
          প্রত্যাগম true;
        }

      free_buffer (b);
      যদি (have_read_eof)
        প্রত্যাগম false;
      যদি (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

স্থির intmax_t
get_first_সারি_in_buffer (শূন্য)
{
  যদি (head == রিক্ত && !load_buffer ())
    প্রত্যাগম 0;

  প্রত্যাগম head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

স্থির সংরচনা cstring *
remove_সারি (শূন্য)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  স্থির সংরচনা buffer_record *prev_buf = রিক্ত;

  সংরচনা cstring *সারি;		/* Return value. */
  সংরচনা সারি *l;		/* For convenience. */

  যদি (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = রিক্ত;
    }

  যদি (head == রিক্ত && !load_buffer ())
    প্রত্যাগম রিক্ত;

  যদি (current_সারি < head->first_available)
    current_সারি = head->first_available;

  ++(head->first_available);

  l = head->curr_সারি;

  সারি = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  যদি (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_সারি = l->next;
      যদি (head->curr_সারি == রিক্ত || head->curr_সারি->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  প্রত্যাগম সারি;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

স্থির সংরচনা cstring *
find_সারি (intmax_t linenum)
{
  যদি (head == রিক্ত && !load_buffer ())
    প্রত্যাগম রিক্ত;

  যদি (linenum < head->start_সারি)
    প্রত্যাগম রিক্ত;

  প্রতি (সংরচনা buffer_record *b = head;;)
    {
      যদি (linenum < b->start_সারি + b->অঙ্ক_সারিগুলি)
        {
          /* The line is in this buffer. */
          সংরচনা সারি *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->সারি_start;
          offset = linenum - b->start_সারি;
          /* Find the control record. */
          যাবত্ (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          প্রত্যাগম &l->starts[offset];
        }
      যদি (b->next == রিক্ত && !load_buffer ())
        প্রত্যাগম রিক্ত;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

স্থির bool
no_more_সারিগুলি (শূন্য)
{
  প্রত্যাগম find_সারি (current_সারি + 1) == রিক্ত;
}

/* Open NAME as standard input.  */

স্থির শূন্য
set_প্রবেশ_file (বর্ণ ধ্রুব *name)
{
  যদি (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

স্থির শূন্য
write_to_file (intmax_t last_সারি, bool ignore, পূর্ণাঙ্ক argnum)
{
  সংরচনা cstring *সারি;
  intmax_t first_সারি;		/* First available input line. */
  intmax_t সারিগুলি;		/* Number of lines to output. */

  first_সারি = get_first_সারি_in_buffer ();

  যদি (! first_সারি || first_সারি > last_সারি)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  সারিগুলি = last_সারি - first_সারি;

  প্রতি (intmax_t i = 0; i < সারিগুলি; i++)
    {
      সারি = remove_সারি ();
      যদি (সারি == রিক্ত)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      যদি (!ignore)
        save_সারি_to_file (সারি);
    }
}

/* Output any lines left after all regexps have been processed. */

স্থির শূন্য
dump_rest_of_file (শূন্য)
{
  সংরচনা cstring *সারি;

  যাবত্ ((সারি = remove_সারি ()) != রিক্ত)
    save_সারি_to_file (সারি);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

স্থির শূন্য
handle_সারি_error (ধ্রুব সংরচনা control *p, intmax_t repetition)
{
  বর্ণ buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->সারিগুলি_required, buf)));
  যদি (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  অন্যথা
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

স্থির শূন্য
process_সারি_গুনতি (ধ্রুব সংরচনা control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_সারি_to_save = p->সারিগুলি_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  যদি (no_more_সারিগুলি () && suppress_matched)
    handle_সারি_error (p, repetition);

  যদি (!(linenum = get_first_সারি_in_buffer ()))
    handle_সারি_error (p, repetition);

  যাবত্ (linenum++ < last_সারি_to_save)
    {
      সংরচনা cstring *সারি = remove_সারি ();
      যদি (সারি == রিক্ত)
        handle_সারি_error (p, repetition);
      save_সারি_to_file (সারি);
    }

  close_output_file ();

  যদি (suppress_matched)
    remove_সারি ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  যদি (no_more_সারিগুলি () && !suppress_matched)
    handle_সারি_error (p, repetition);
}

স্থির শূন্য
regexp_error (সংরচনা control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  যদি (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  অন্যথা
    fprintf (stderr, "\n");

  যদি (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

স্থির শূন্য
process_regexp (সংরচনা control *p, intmax_t repetition)
{
  সংরচনা cstring *সারি;		/* From input file. */
  idx_t সারি_len;		/* To make "$" in regexps work. */
  intmax_t break_সারি;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  যদি (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  যদি (p->offset >= 0)
    {
      যাবত্ (true)
        {
          সারি = find_সারি (++current_সারি);
          যদি (সারি == রিক্ত)
            {
              যদি (p->repeat_forever)
                {
                  যদি (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              অন্যথা
                regexp_error (p, repetition, ignore);
            }
          সারি_len = সারি->len;
          যদি (সারি->str[সারি_len - 1] == '\n')
            সারি_len--;
          ret = re_অনুসন্ধান (&p->re_compiled, সারি->str, সারি_len,
                           0, সারি_len, রিক্ত);
          যদি (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          যদি (ret == -1)
            {
              সারি = remove_সারি ();
              যদি (!ignore)
                save_সারি_to_file (সারি);
            }
          অন্যথা
            ভঙ্গ;
        }
    }
  অন্যথা
    {
      /* Buffer the lines. */
      যাবত্ (true)
        {
          সারি = find_সারি (++current_সারি);
          যদি (সারি == রিক্ত)
            {
              যদি (p->repeat_forever)
                {
                  যদি (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              অন্যথা
                regexp_error (p, repetition, ignore);
            }
          সারি_len = সারি->len;
          যদি (সারি->str[সারি_len - 1] == '\n')
            সারি_len--;
          ret = re_অনুসন্ধান (&p->re_compiled, সারি->str, সারি_len,
                           0, সারি_len, রিক্ত);
          যদি (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          যদি (ret != -1)
            ভঙ্গ;
        }
    }

  /* Account for any offset from this regexp. */
  break_সারি = current_সারি + p->offset;

  write_to_file (break_সারি, ignore, p->argnum);

  যদি (!ignore)
    close_output_file ();

  যদি (p->offset > 0)
    current_সারি = break_সারি;

  যদি (suppress_matched)
    remove_সারি ();
}

/* Split the input file according to the control records we have built. */

স্থির শূন্য
split_file (শূন্য)
{
  প্রতি (idx_t i = 0; i < control_used; i++)
    {
      যদি (controls[i].regexpr)
        {
          প্রতি (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      অন্যথা
        {
          প্রতি (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_সারি_গুনতি (&controls[i], j);
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

স্থির বর্ণ *
make_filename (পূর্ণাঙ্ক অঙ্ক)
{
  strcpy (filename_space, prefix);
  যদি (suffix)
    sprintf (filename_space + strlen (prefix), suffix, অঙ্ক);
  অন্যথা
    sprintf (filename_space + strlen (prefix), "%0*d", digits, অঙ্ক);
  প্রত্যাগম filename_space;
}

/* Create the next output file. */

স্থির শূন্য
create_output_file (শূন্য)
{
  পূর্ণাঙ্ক nfiles = files_created;
  bool fopen_ok;
  পূর্ণাঙ্ক fopen_errno;

  output_filename = make_filename (nfiles);

  যদি (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  অন্যথা
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != রিক্ত);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, রিক্ত);
    }

  যদি (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

স্থির শূন্য
delete_all_files (bool in_signal_handler)
{
  যদি (! remove_files)
    প্রত্যাগম;

  প্রতি (পূর্ণাঙ্ক i = files_created; 0 <= --i; )
    {
      বর্ণ ধ্রুব *name = make_filename (i);
      যদি (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

স্থির শূন্য
close_output_file (শূন্য)
{
  যদি (output_stream)
    {
      যদি (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = রিক্ত;
          cleanup_fatal ();
        }
      যদি (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = রিক্ত;
          cleanup_fatal ();
        }
      যদি (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          পূর্ণাঙ্ক unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, রিক্ত);

          যদি (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      অন্যথা
        {
          যদি (!suppress_গুনতি)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = রিক্ত;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

স্থির শূন্য
save_সারি_to_file (ধ্রুব সংরচনা cstring *সারি)
{
  idx_t l = fwrite (সারি->str, পরিমাণ (বর্ণ), সারি->len, output_stream);
  যদি (l != সারি->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = রিক্ত;
      cleanup_fatal ();
    }
  bytes_written += সারি->len;
}

/* Return a new, initialized control record. */

স্থির সংরচনা control *
new_control_record (শূন্য)
{
  স্থির idx_t control_allocated = 0; /* Total space allocated. */
  সংরচনা control *p;

  যদি (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, পরিমাণ *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->সারিগুলি_required = 0;
  p->offset = 0;
  প্রত্যাগম p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

স্থির শূন্য
check_for_offset (সংরচনা control *p, বর্ণ ধ্রুব *str, বর্ণ ধ্রুব *অঙ্ক)
{
  যদি (xstrtoimax (অঙ্ক, রিক্ত, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

স্থির শূন্য
parse_repeat_গুনতি (পূর্ণাঙ্ক argnum, সংরচনা control *p, বর্ণ *str)
{
  বর্ণ *end;

  end = str + strlen (str) - 1;
  যদি (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  যদি (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  অন্যথা
    {
      uintmax_t val;
      যদি (xstrtoumax (str + 1, রিক্ত, 10, &val, "") != LONGINT_OK
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

স্থির সংরচনা control *
extract_regexp (পূর্ণাঙ্ক argnum, bool ignore, বর্ণ ধ্রুব *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  বর্ণ delim = *str;
  বর্ণ ধ্রুব *closing_delim;
  সংরচনা control *p;
  বর্ণ ধ্রুব *err;

  closing_delim = strrchr (str + 1, delim);
  যদি (closing_delim == রিক্ত)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = রিক্ত;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = রিক্ত;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  যদি (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  যদি (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  প্রত্যাগম p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

স্থির শূন্য
parse_patterns (পূর্ণাঙ্ক argc, পূর্ণাঙ্ক start, বর্ণ **argv)
{
  সংরচনা control *p;		/* New control record created. */
  স্থির intmax_t last_val = 0;

  প্রতি (পূর্ণাঙ্ক i = start; i < argc; i++)
    {
      যদি (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      অন্যথা
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          যদি (xstrtoumax (argv[i], রিক্ত, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          যদি (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          যদি (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          যদি (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->সারিগুলি_required = val;
        }

      যদি (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_গুনতি (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
গণনা { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
স্থির idx_t
get_format_flags (বর্ণ ধ্রুব *format, পূর্ণাঙ্ক *flags_ptr)
{
  পূর্ণাঙ্ক flags = 0;

  প্রতি (idx_t গুনতি = 0; ; গুনতি++)
    {
      বিকল্প (format[গুনতি])
        {
        পক্ষ '-':
        পক্ষ '0':
          ভঙ্গ;

        পক্ষ '\'':
          flags |= FLAG_THOUSANDS;
          ভঙ্গ;

        পক্ষ '#':
          flags |= FLAG_ALTERNATIVE;
          ভঙ্গ;

        স্বভাব:
          *flags_ptr = flags;
          প্রত্যাগম গুনতি;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
স্থির শূন্য
check_format_conv_type (বর্ণ *format, পূর্ণাঙ্ক flags)
{
  unsigned বর্ণ ch = *format;
  পূর্ণাঙ্ক compatible_flags = FLAG_THOUSANDS;

  বিকল্প (ch)
    {
    পক্ষ 'd':
    পক্ষ 'i':
      ভঙ্গ;

    পক্ষ 'u':
      *format = 'd';
      ভঙ্গ;

    পক্ষ 'o':
    পক্ষ 'x':
    পক্ষ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ভঙ্গ;

    পক্ষ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    স্বভাব:
      যদি (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      অন্যথা
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  যদি (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
স্থির idx_t
max_out (বর্ণ *format)
{
  bool percent = false;

  প্রতি (বর্ণ *f = format; *f; f++)
    যদি (*f == '%' && *++f != '%')
      {
        যদি (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        পূর্ণাঙ্ক flags;
        f += get_format_flags (f, &flags);
        যাবত্ (c_isdigit (*f))
          f++;
        যদি (*f == '.')
          যাবত্ (c_isdigit (*++f))
            অনুবর্ত;
        check_format_conv_type (f, flags);
      }

  যদি (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  পূর্ণাঙ্ক maxlen = snprintf (রিক্ত, 0, format, INT_MAX);
  যদি (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  প্রত্যাগম maxlen;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  পূর্ণাঙ্ক optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  যাবত্ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, রিক্ত))
         != -1)
    বিকল্প (optc)
      {
      পক্ষ 'f':
        prefix = optarg;
        ভঙ্গ;

      পক্ষ 'b':
        suffix = optarg;
        ভঙ্গ;

      পক্ষ 'k':
        remove_files = false;
        ভঙ্গ;

      পক্ষ 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        ভঙ্গ;

      পক্ষ 's':
      পক্ষ 'q':
        suppress_গুনতি = true;
        ভঙ্গ;

      পক্ষ 'z':
        elide_empty_files = true;
        ভঙ্গ;

      পক্ষ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ভঙ্গ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      স্বভাব:
        usage (EXIT_FAILURE);
      }

  যদি (argc - optind < 2)
    {
      যদি (argc <= optind)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (পূর্ণাঙ্ক), digits));
  idx_t filename_size;
  যদি (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_প্রবেশ_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    গণনা { nsigs = countof (term_sig) };

    সংরচনা sigaction act;

    sigemptyset (&caught_signals);
    প্রতি (পূর্ণাঙ্ক i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], রিক্ত, &act);
        যদি (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    প্রতি (পূর্ণাঙ্ক i = 0; i < nsigs; i++)
      যদি (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, রিক্ত);
  }

  split_file ();

  যদি (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  প্রত্যাগম EXIT_SUCCESS;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
Read standard প্রবেশ যদি FILE is -\n\
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
         কুরু not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the সারিগুলি matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified সংখ্যা of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         কুরু not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified সারি সংখ্যা\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching সারি\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching সারি\n\
  {INTEGER}          repeat the previous pattern specified সংখ্যা of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A সারি OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
