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

#ضمن <config.h>

#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <signal.h>

#ضمن "system.h"

#ضمن <regex.h>

#ضمن "c-ctype.h"
#ضمن "fd-reopen.h"
#ضمن "quote.h"
#ضمن "safe-read.h"
#ضمن "stdio--.h"
#ضمن "term-sig.h"
#ضمن "xdectoint.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
بنية control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t lines_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحيح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  بنية re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
بنية cstring
{
  idx_t len;
  حرف *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
بنية line
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t insert_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  بنية cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  بنية line *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
بنية buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_line;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t num_lines;		/* Number of complete lines in this buffer. */
  حرف *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  بنية line *line_start;	/* Head of list of pointers to lines. */
  بنية line *curr_line;	/* The line start record currently in use. */
  بنية buffer_record *next;
};

ساكن فراغ close_output_file (فراغ);
ساكن فراغ create_output_file (فراغ);
ساكن فراغ delete_all_files (bool);
ساكن فراغ save_line_to_file (ثابت بنية cstring *line);

/* Start of buffer list. */
ساكن بنية buffer_record *head = عدم;

/* Partially read line. */
ساكن حرف *hold_area = عدم;

/* Number of bytes in 'hold_area'. */
ساكن idx_t hold_count = 0;

/* Number of the last line in the buffers. */
ساكن intmax_t last_line_number = 0;

/* Number of the line currently being examined. */
ساكن intmax_t current_line = 0;

/* If true, we have read EOF. */
ساكن bool have_read_eof = false;

/* Name of output files. */
ساكن حرف *متطاير filename_space = عدم;

/* Prefix part of output file names. */
ساكن حرف ثابت *متطاير prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
ساكن حرف *متطاير suffix = عدم;

/* Number of digits to use in output file names. */
ساكن صحيح متطاير digits = 2;

/* Number of files created so far. */
ساكن صحيح متطاير files_created = 0;

/* Number of bytes written to current file. */
ساكن intmax_t bytes_written;

/* Output file pointer. */
ساكن FILE *output_stream = عدم;

/* Output file name. */
ساكن حرف *output_filename = عدم;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ساكن حرف **global_argv;

/* If true, do not print the count of bytes in each output file. */
ساكن bool suppress_count;

/* If true, remove output files on error. */
ساكن bool متطاير remove_files = true;

/* If true, remove all output files which have a zero length. */
ساكن bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
ساكن bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ساكن بنية control *controls;

/* Number of elements in 'controls'. */
ساكن idx_t control_used;

/* The set of signals that are caught.  */
ساكن sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"digits", required_argument, عدم, 'n'},
  {"quiet", no_argument, عدم, 'q'},  /* Deprecated.  */
  {"silent", no_argument, عدم, 's'},
  {"keep-files", no_argument, عدم, 'k'},
  {"elide-empty-files", no_argument, عدم, 'z'},
  {"prefix", required_argument, عدم, 'f'},
  {"suffix-format", required_argument, عدم, 'b'},
  {"suppress-matched", no_argument, عدم, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ساكن فراغ
cleanup (فراغ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, عدم);
}

ساكن _Noreturn فراغ
cleanup_fatal (فراغ)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

خارجي فراغ
xalloc_die (فراغ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ساكن فراغ
interrupt_handler (صحيح sig)
{
  delete_all_files (true);
  إشارة (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ارفع (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ساكن فراغ
save_to_hold_area (حرف *start, idx_t num)
{
  حرر (hold_area);
  hold_area = start;
  hold_count = num;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ساكن idx_t
read_input (حرف *dest, idx_t max_n_bytes)
{
  𐤀𐤌 (max_n_bytes == 0)
    أرجع 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  𐤀𐤌 (bytes_read == 0)
    have_read_eof = true;

  𐤀𐤌 (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع bytes_read;
}

/* Initialize existing line record P. */

ساكن فراغ
clear_line_control (بنية line *p)
{
  p->used = 0;
  p->insert_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

ساكن بنية line *
new_line_control (فراغ)
{
  بنية line *p = xmalloc (حجم *p);

  p->next = عدم;
  clear_line_control (p);

  أرجع p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ساكن فراغ
keep_new_line (بنية buffer_record *b, حرف *line_start, idx_t line_len)
{
  بنية line *l;

  /* If there is no existing area to keep line info, get some. */
  𐤀𐤌 (b->line_start == عدم)
    b->line_start = b->curr_line = new_line_control ();

  /* If existing area for lines is full, get more. */
  𐤀𐤌 (b->curr_line->used == CTRL_SIZE)
    {
      b->curr_line->next = new_line_control ();
      b->curr_line = b->curr_line->next;
    }

  l = b->curr_line;

  /* Record the start of the line, and update counters. */
  l->starts[l->insert_index].str = line_start;
  l->starts[l->insert_index].len = line_len;
  l->used++;
  l->insert_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ساكن idx_t
record_line_starts (بنية buffer_record *b)
{
  حرف *line_start;		/* Start of current line. */
  idx_t lines;			/* Number of lines found. */
  idx_t line_length;		/* Length of each line found. */

  𐤀𐤌 (b->bytes_used == 0)
    أرجع 0;

  lines = 0;
  line_start = b->buffer;
  حرف *buffer_end = line_start + b->bytes_used;
  *buffer_end = '\n';

  طالما (true)
    {
      حرف *line_end = rawmemchr (line_start, '\n');
      𐤀𐤌 (line_end == buffer_end)
        اكسر;
      line_length = line_end - line_start + 1;
      keep_new_line (b, line_start, line_length);
      line_start = line_end + 1;
      lines++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_left = buffer_end - line_start;
  𐤀𐤌 (bytes_left)
    {
      𐤀𐤌 (have_read_eof)
        {
          keep_new_line (b, line_start, bytes_left);
          lines++;
        }
      وإلا
        save_to_hold_area (ximemdup (line_start, bytes_left), bytes_left);
    }

  b->num_lines = lines;
  b->first_available = b->start_line = last_line_number + 1;
  last_line_number += lines;

  أرجع lines;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#𐤀𐤌 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ساكن فراغ
free_buffer (بنية buffer_record *buf)
{
  لكل (بنية line *l = buf->line_start; l;)
    {
      بنية line *n = l->next;
      حرر (l);
      l = n;
    }
  حرر (buf->buffer);
  حرر (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ساكن ATTRIBUTE_DEALLOC (free_buffer, 1)
بنية buffer_record *
get_new_buffer (idx_t min_size)
{
  بنية buffer_record *new_buffer = xmalloc (حجم *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (عدم, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_line = new_buffer->first_available = last_line_number + 1;
  new_buffer->num_lines = 0;
  new_buffer->line_start = new_buffer->curr_line = عدم;
  new_buffer->next = عدم;

  أرجع new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ساكن فراغ
save_buffer (بنية buffer_record *buf)
{
  buf->next = عدم;
  buf->curr_line = buf->line_start;

  𐤀𐤌 (head == عدم)
    head = buf;
  وإلا
    {
      بنية buffer_record *p;
      لكل (p = head; p->next; p = p->next)
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

ساكن bool
load_buffer (فراغ)
{
  𐤀𐤌 (have_read_eof)
    أرجع false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_count + 1);

  طالما (true)
    {
      بنية buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      حرف *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      𐤀𐤌 (hold_count)
        {
          p = mempcpy (p, hold_area, hold_count);
          b->bytes_used += hold_count;
          bytes_avail -= hold_count;
          hold_count = 0;
        }

      b->bytes_used += read_input (p, bytes_avail - 1);

      𐤀𐤌 (record_line_starts (b) != 0)
        {
          save_buffer (b);
          أرجع true;
        }

      free_buffer (b);
      𐤀𐤌 (have_read_eof)
        أرجع false;
      𐤀𐤌 (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ساكن intmax_t
get_first_line_in_buffer (فراغ)
{
  𐤀𐤌 (head == عدم && !load_buffer ())
    أرجع 0;

  أرجع head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ساكن بنية cstring *
remove_line (فراغ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ساكن بنية buffer_record *prev_buf = عدم;

  بنية cstring *line;		/* Return value. */
  بنية line *l;		/* For convenience. */

  𐤀𐤌 (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = عدم;
    }

  𐤀𐤌 (head == عدم && !load_buffer ())
    أرجع عدم;

  𐤀𐤌 (current_line < head->first_available)
    current_line = head->first_available;

  ++(head->first_available);

  l = head->curr_line;

  line = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  𐤀𐤌 (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_line = l->next;
      𐤀𐤌 (head->curr_line == عدم || head->curr_line->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  أرجع line;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ساكن بنية cstring *
find_line (intmax_t linenum)
{
  𐤀𐤌 (head == عدم && !load_buffer ())
    أرجع عدم;

  𐤀𐤌 (linenum < head->start_line)
    أرجع عدم;

  لكل (بنية buffer_record *b = head;;)
    {
      𐤀𐤌 (linenum < b->start_line + b->num_lines)
        {
          /* The line is in this buffer. */
          بنية line *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->line_start;
          offset = linenum - b->start_line;
          /* Find the control record. */
          طالما (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          أرجع &l->starts[offset];
        }
      𐤀𐤌 (b->next == عدم && !load_buffer ())
        أرجع عدم;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ساكن bool
no_more_lines (فراغ)
{
  أرجع find_line (current_line + 1) == عدم;
}

/* Open NAME as standard input.  */

ساكن فراغ
set_input_file (حرف ثابت *name)
{
  𐤀𐤌 (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ساكن فراغ
write_to_file (intmax_t last_line, bool ignore, صحيح argnum)
{
  بنية cstring *line;
  intmax_t first_line;		/* First available input line. */
  intmax_t lines;		/* Number of lines to output. */

  first_line = get_first_line_in_buffer ();

  𐤀𐤌 (! first_line || first_line > last_line)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  lines = last_line - first_line;

  لكل (intmax_t i = 0; i < lines; i++)
    {
      line = remove_line ();
      𐤀𐤌 (line == عدم)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      𐤀𐤌 (!ignore)
        save_line_to_file (line);
    }
}

/* Output any lines left after all regexps have been processed. */

ساكن فراغ
dump_rest_of_file (فراغ)
{
  بنية cstring *line;

  طالما ((line = remove_line ()) != عدم)
    save_line_to_file (line);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ساكن فراغ
handle_line_error (ثابت بنية control *p, intmax_t repetition)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->lines_required, buf)));
  𐤀𐤌 (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وإلا
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ساكن فراغ
process_line_count (ثابت بنية control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_line_to_save = p->lines_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  𐤀𐤌 (no_more_lines () && suppress_matched)
    handle_line_error (p, repetition);

  𐤀𐤌 (!(linenum = get_first_line_in_buffer ()))
    handle_line_error (p, repetition);

  طالما (linenum++ < last_line_to_save)
    {
      بنية cstring *line = remove_line ();
      𐤀𐤌 (line == عدم)
        handle_line_error (p, repetition);
      save_line_to_file (line);
    }

  close_output_file ();

  𐤀𐤌 (suppress_matched)
    remove_line ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  𐤀𐤌 (no_more_lines () && !suppress_matched)
    handle_line_error (p, repetition);
}

ساكن فراغ
regexp_error (بنية control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  𐤀𐤌 (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وإلا
    fprintf (stderr, "\n");

  𐤀𐤌 (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ساكن فراغ
process_regexp (بنية control *p, intmax_t repetition)
{
  بنية cstring *line;		/* From input file. */
  idx_t line_len;		/* To make "$" in regexps work. */
  intmax_t break_line;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  𐤀𐤌 (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  𐤀𐤌 (p->offset >= 0)
    {
      طالما (true)
        {
          line = find_line (++current_line);
          𐤀𐤌 (line == عدم)
            {
              𐤀𐤌 (p->repeat_forever)
                {
                  𐤀𐤌 (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          line_len = line->len;
          𐤀𐤌 (line->str[line_len - 1] == '\n')
            line_len--;
          ret = re_search (&p->re_compiled, line->str, line_len,
                           0, line_len, عدم);
          𐤀𐤌 (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          𐤀𐤌 (ret == -1)
            {
              line = remove_line ();
              𐤀𐤌 (!ignore)
                save_line_to_file (line);
            }
          وإلا
            اكسر;
        }
    }
  وإلا
    {
      /* Buffer the lines. */
      طالما (true)
        {
          line = find_line (++current_line);
          𐤀𐤌 (line == عدم)
            {
              𐤀𐤌 (p->repeat_forever)
                {
                  𐤀𐤌 (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          line_len = line->len;
          𐤀𐤌 (line->str[line_len - 1] == '\n')
            line_len--;
          ret = re_search (&p->re_compiled, line->str, line_len,
                           0, line_len, عدم);
          𐤀𐤌 (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          𐤀𐤌 (ret != -1)
            اكسر;
        }
    }

  /* Account for any offset from this regexp. */
  break_line = current_line + p->offset;

  write_to_file (break_line, ignore, p->argnum);

  𐤀𐤌 (!ignore)
    close_output_file ();

  𐤀𐤌 (p->offset > 0)
    current_line = break_line;

  𐤀𐤌 (suppress_matched)
    remove_line ();
}

/* Split the input file according to the control records we have built. */

ساكن فراغ
split_file (فراغ)
{
  لكل (idx_t i = 0; i < control_used; i++)
    {
      𐤀𐤌 (controls[i].regexpr)
        {
          لكل (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      وإلا
        {
          لكل (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_line_count (&controls[i], j);
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

ساكن حرف *
make_filename (صحيح num)
{
  strcpy (filename_space, prefix);
  𐤀𐤌 (suffix)
    sprintf (filename_space + strlen (prefix), suffix, num);
  وإلا
    sprintf (filename_space + strlen (prefix), "%0*d", digits, num);
  أرجع filename_space;
}

/* Create the next output file. */

ساكن فراغ
create_output_file (فراغ)
{
  صحيح nfiles = files_created;
  bool fopen_ok;
  صحيح fopen_errno;

  output_filename = make_filename (nfiles);

  𐤀𐤌 (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  وإلا
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != عدم);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, عدم);
    }

  𐤀𐤌 (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ساكن فراغ
delete_all_files (bool in_signal_handler)
{
  𐤀𐤌 (! remove_files)
    أرجع;

  لكل (صحيح i = files_created; 0 <= --i; )
    {
      حرف ثابت *name = make_filename (i);
      𐤀𐤌 (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ساكن فراغ
close_output_file (فراغ)
{
  𐤀𐤌 (output_stream)
    {
      𐤀𐤌 (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = عدم;
          cleanup_fatal ();
        }
      𐤀𐤌 (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = عدم;
          cleanup_fatal ();
        }
      𐤀𐤌 (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحيح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, عدم);

          𐤀𐤌 (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      وإلا
        {
          𐤀𐤌 (!suppress_count)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = عدم;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ساكن فراغ
save_line_to_file (ثابت بنية cstring *line)
{
  idx_t l = fwrite (line->str, حجم (حرف), line->len, output_stream);
  𐤀𐤌 (l != line->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = عدم;
      cleanup_fatal ();
    }
  bytes_written += line->len;
}

/* Return a new, initialized control record. */

ساكن بنية control *
new_control_record (فراغ)
{
  ساكن idx_t control_allocated = 0; /* Total space allocated. */
  بنية control *p;

  𐤀𐤌 (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, حجم *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->lines_required = 0;
  p->offset = 0;
  أرجع p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ساكن فراغ
check_for_offset (بنية control *p, حرف ثابت *str, حرف ثابت *num)
{
  𐤀𐤌 (xstrtoimax (num, عدم, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ساكن فراغ
parse_repeat_count (صحيح argnum, بنية control *p, حرف *str)
{
  حرف *end;

  end = str + strlen (str) - 1;
  𐤀𐤌 (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  𐤀𐤌 (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  وإلا
    {
      uintmax_t val;
      𐤀𐤌 (xstrtoumax (str + 1, عدم, 10, &val, "") != LONGINT_OK
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

ساكن بنية control *
extract_regexp (صحيح argnum, bool ignore, حرف ثابت *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  حرف delim = *str;
  حرف ثابت *closing_delim;
  بنية control *p;
  حرف ثابت *err;

  closing_delim = strrchr (str + 1, delim);
  𐤀𐤌 (closing_delim == عدم)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = عدم;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = عدم;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  𐤀𐤌 (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  𐤀𐤌 (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  أرجع p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ساكن فراغ
parse_patterns (صحيح argc, صحيح start, حرف **argv)
{
  بنية control *p;		/* New control record created. */
  ساكن intmax_t last_val = 0;

  لكل (صحيح i = start; i < argc; i++)
    {
      𐤀𐤌 (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      وإلا
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          𐤀𐤌 (xstrtoumax (argv[i], عدم, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          𐤀𐤌 (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          𐤀𐤌 (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          𐤀𐤌 (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->lines_required = val;
        }

      𐤀𐤌 (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_count (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
تعداد { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ساكن idx_t
get_format_flags (حرف ثابت *format, صحيح *flags_ptr)
{
  صحيح flags = 0;

  لكل (idx_t count = 0; ; count++)
    {
      بدل (format[count])
        {
        حالة '-':
        حالة '0':
          اكسر;

        حالة '\'':
          flags |= FLAG_THOUSANDS;
          اكسر;

        حالة '#':
          flags |= FLAG_ALTERNATIVE;
          اكسر;

        افتراضي:
          *flags_ptr = flags;
          أرجع count;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ساكن فراغ
check_format_conv_type (حرف *format, صحيح flags)
{
  unsigned حرف ch = *format;
  صحيح compatible_flags = FLAG_THOUSANDS;

  بدل (ch)
    {
    حالة 'd':
    حالة 'i':
      اكسر;

    حالة 'u':
      *format = 'd';
      اكسر;

    حالة 'o':
    حالة 'x':
    حالة 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      اكسر;

    حالة 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    افتراضي:
      𐤀𐤌 (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      وإلا
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  𐤀𐤌 (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ساكن idx_t
max_out (حرف *format)
{
  bool percent = false;

  لكل (حرف *f = format; *f; f++)
    𐤀𐤌 (*f == '%' && *++f != '%')
      {
        𐤀𐤌 (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        صحيح flags;
        f += get_format_flags (f, &flags);
        طالما (c_isdigit (*f))
          f++;
        𐤀𐤌 (*f == '.')
          طالما (c_isdigit (*++f))
            استمر;
        check_format_conv_type (f, flags);
      }

  𐤀𐤌 (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  صحيح maxlen = snprintf (عدم, 0, format, INT_MAX);
  𐤀𐤌 (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  أرجع maxlen;
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  طالما ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, عدم))
         != -1)
    بدل (optc)
      {
      حالة 'f':
        prefix = optarg;
        اكسر;

      حالة 'b':
        suffix = optarg;
        اكسر;

      حالة 'k':
        remove_files = false;
        اكسر;

      حالة 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        اكسر;

      حالة 's':
      حالة 'q':
        suppress_count = true;
        اكسر;

      حالة 'z':
        elide_empty_files = true;
        اكسر;

      حالة SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  𐤀𐤌 (argc - optind < 2)
    {
      𐤀𐤌 (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (صحيح), digits));
  idx_t filename_size;
  𐤀𐤌 (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_input_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    تعداد { nsigs = countof (term_sig) };

    بنية sigaction act;

    sigemptyset (&caught_signals);
    لكل (صحيح i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], عدم, &act);
        𐤀𐤌 (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    لكل (صحيح i = 0; i < nsigs; i++)
      𐤀𐤌 (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, عدم);
  }

  split_file ();

  𐤀𐤌 (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع EXIT_SUCCESS;
}

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
Read standard input 𐤀𐤌 FILE is -\n\
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
         𐤐𐤏𐤋 not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the lines matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified number of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         𐤐𐤏𐤋 not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified line number\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching line\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching line\n\
  {INTEGER}          repeat the previous pattern specified number of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A line OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
