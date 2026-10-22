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

#include <config.h>

#include <getopt.h>
#include <sys/types.h>
#include <signal.h>

#include "system.h"

#include <regex.h>

#include "c-ctype.h"
#include "fd-reopen.h"
#include "quote.h"
#include "safe-read.h"
#include "stdio--.h"
#include "term-sig.h"
#include "xdectoint.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
sa_nracanaa control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t lines_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  puur_naa_nka argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  sa_nracanaa re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
sa_nracanaa cstring
{
  idx_t len;
  var_na *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
sa_nracanaa line
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t insert_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  sa_nracanaa cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  sa_nracanaa line *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
sa_nracanaa buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_line;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t num_lines;		/* Number of complete lines in this buffer. */
  var_na *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  sa_nracanaa line *line_start;	/* Head of list of pointers to lines. */
  sa_nracanaa line *curr_line;	/* The line start record currently in use. */
  sa_nracanaa buffer_record *next;
};

static void close_output_file (void);
static void create_output_file (void);
static void delete_all_files (bool);
static void save_line_to_file (const sa_nracanaa cstring *line);

/* Start of buffer list. */
static sa_nracanaa buffer_record *head = NULL;

/* Partially read line. */
static var_na *hold_area = NULL;

/* Number of bytes in 'hold_area'. */
static idx_t hold_count = 0;

/* Number of the last line in the buffers. */
static intmax_t last_line_number = 0;

/* Number of the line currently being examined. */
static intmax_t current_line = 0;

/* If true, we have read EOF. */
static bool have_read_eof = false;

/* Name of output files. */
static var_na *ca_ncala filename_space = NULL;

/* Prefix part of output file names. */
static var_na const *ca_ncala prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
static var_na *ca_ncala suffix = NULL;

/* Number of digits to use in output file names. */
static puur_naa_nka ca_ncala digits = 2;

/* Number of files created so far. */
static puur_naa_nka ca_ncala files_created = 0;

/* Number of bytes written to current file. */
static intmax_t bytes_written;

/* Output file pointer. */
static FILE *output_stream = NULL;

/* Output file name. */
static var_na *output_filename = NULL;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
static var_na **global_argv;

/* If true, do not print the count of bytes in each output file. */
static bool suppress_count;

/* If true, remove output files on error. */
static bool ca_ncala remove_files = true;

/* If true, remove all output files which have a zero length. */
static bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
static bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
static sa_nracanaa control *controls;

/* Number of elements in 'controls'. */
static idx_t control_used;

/* The set of signals that are caught.  */
static sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  {"digits", required_argument, NULL, 'n'},
  {"quiet", no_argument, NULL, 'q'},  /* Deprecated.  */
  {"silent", no_argument, NULL, 's'},
  {"keep-files", no_argument, NULL, 'k'},
  {"elide-empty-files", no_argument, NULL, 'z'},
  {"prefix", required_argument, NULL, 'f'},
  {"suffix-format", required_argument, NULL, 'b'},
  {"suppress-matched", no_argument, NULL, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

static void
cleanup (void)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, NULL);
}

static _Noreturn void
cleanup_fatal (void)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

extern void
xalloc_die (void)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

static void
interrupt_handler (puur_naa_nka sig)
{
  delete_all_files (true);
  sa_nkeeta (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  raise (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

static void
save_to_hold_area (var_na *start, idx_t num)
{
  free (hold_area);
  hold_area = start;
  hold_count = num;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

static idx_t
read_input (var_na *dest, idx_t max_n_bytes)
{
  ᯀᯮᯝ (max_n_bytes == 0)
    ᯀᯮᯞ 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  ᯀᯮᯝ (bytes_read == 0)
    have_read_eof = true;

  ᯀᯮᯝ (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ᯀᯮᯞ bytes_read;
}

/* Initialize existing line record P. */

static void
clear_line_control (sa_nracanaa line *p)
{
  p->used = 0;
  p->insert_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

static sa_nracanaa line *
new_line_control (void)
{
  sa_nracanaa line *p = xmalloc (parimaa_na *p);

  p->next = NULL;
  clear_line_control (p);

  ᯀᯮᯞ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

static void
keep_new_line (sa_nracanaa buffer_record *b, var_na *line_start, idx_t line_len)
{
  sa_nracanaa line *l;

  /* If there is no existing area to keep line info, get some. */
  ᯀᯮᯝ (b->line_start == NULL)
    b->line_start = b->curr_line = new_line_control ();

  /* If existing area for lines is full, get more. */
  ᯀᯮᯝ (b->curr_line->used == CTRL_SIZE)
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

static idx_t
record_line_starts (sa_nracanaa buffer_record *b)
{
  var_na *line_start;		/* Start of current line. */
  idx_t lines;			/* Number of lines found. */
  idx_t line_length;		/* Length of each line found. */

  ᯀᯮᯝ (b->bytes_used == 0)
    ᯀᯮᯞ 0;

  lines = 0;
  line_start = b->buffer;
  var_na *buffer_end = line_start + b->bytes_used;
  *buffer_end = '\n';

  ᯀᯬᯝ (true)
    {
      var_na *line_end = rawmemchr (line_start, '\n');
      ᯀᯮᯝ (line_end == buffer_end)
        bha_nga;
      line_length = line_end - line_start + 1;
      keep_new_line (b, line_start, line_length);
      line_start = line_end + 1;
      lines++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_left = buffer_end - line_start;
  ᯀᯮᯝ (bytes_left)
    {
      ᯀᯮᯝ (have_read_eof)
        {
          keep_new_line (b, line_start, bytes_left);
          lines++;
        }
      ᯀᯮᯝᯂᯪ
        save_to_hold_area (ximemdup (line_start, bytes_left), bytes_left);
    }

  b->num_lines = lines;
  b->first_available = b->start_line = last_line_number + 1;
  last_line_number += lines;

  ᯀᯮᯞ lines;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ᯀᯮᯝ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

static void
free_buffer (sa_nracanaa buffer_record *buf)
{
  for (sa_nracanaa line *l = buf->line_start; l;)
    {
      sa_nracanaa line *n = l->next;
      free (l);
      l = n;
    }
  free (buf->buffer);
  free (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

static ATTRIBUTE_DEALLOC (free_buffer, 1)
sa_nracanaa buffer_record *
get_new_buffer (idx_t min_size)
{
  sa_nracanaa buffer_record *new_buffer = xmalloc (parimaa_na *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (NULL, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_line = new_buffer->first_available = last_line_number + 1;
  new_buffer->num_lines = 0;
  new_buffer->line_start = new_buffer->curr_line = NULL;
  new_buffer->next = NULL;

  ᯀᯮᯞ new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

static void
save_buffer (sa_nracanaa buffer_record *buf)
{
  buf->next = NULL;
  buf->curr_line = buf->line_start;

  ᯀᯮᯝ (head == NULL)
    head = buf;
  ᯀᯮᯝᯂᯪ
    {
      sa_nracanaa buffer_record *p;
      for (p = head; p->next; p = p->next)
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

static bool
load_buffer (void)
{
  ᯀᯮᯝ (have_read_eof)
    ᯀᯮᯞ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_count + 1);

  ᯀᯬᯝ (true)
    {
      sa_nracanaa buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      var_na *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      ᯀᯮᯝ (hold_count)
        {
          p = mempcpy (p, hold_area, hold_count);
          b->bytes_used += hold_count;
          bytes_avail -= hold_count;
          hold_count = 0;
        }

      b->bytes_used += read_input (p, bytes_avail - 1);

      ᯀᯮᯝ (record_line_starts (b) != 0)
        {
          save_buffer (b);
          ᯀᯮᯞ true;
        }

      free_buffer (b);
      ᯀᯮᯝ (have_read_eof)
        ᯀᯮᯞ false;
      ᯀᯮᯝ (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

static intmax_t
get_first_line_in_buffer (void)
{
  ᯀᯮᯝ (head == NULL && !load_buffer ())
    ᯀᯮᯞ 0;

  ᯀᯮᯞ head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

static sa_nracanaa cstring *
remove_line (void)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  static sa_nracanaa buffer_record *prev_buf = NULL;

  sa_nracanaa cstring *line;		/* Return value. */
  sa_nracanaa line *l;		/* For convenience. */

  ᯀᯮᯝ (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = NULL;
    }

  ᯀᯮᯝ (head == NULL && !load_buffer ())
    ᯀᯮᯞ NULL;

  ᯀᯮᯝ (current_line < head->first_available)
    current_line = head->first_available;

  ++(head->first_available);

  l = head->curr_line;

  line = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  ᯀᯮᯝ (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_line = l->next;
      ᯀᯮᯝ (head->curr_line == NULL || head->curr_line->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  ᯀᯮᯞ line;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

static sa_nracanaa cstring *
find_line (intmax_t linenum)
{
  ᯀᯮᯝ (head == NULL && !load_buffer ())
    ᯀᯮᯞ NULL;

  ᯀᯮᯝ (linenum < head->start_line)
    ᯀᯮᯞ NULL;

  for (sa_nracanaa buffer_record *b = head;;)
    {
      ᯀᯮᯝ (linenum < b->start_line + b->num_lines)
        {
          /* The line is in this buffer. */
          sa_nracanaa line *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->line_start;
          offset = linenum - b->start_line;
          /* Find the control record. */
          ᯀᯬᯝ (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          ᯀᯮᯞ &l->starts[offset];
        }
      ᯀᯮᯝ (b->next == NULL && !load_buffer ())
        ᯀᯮᯞ NULL;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

static bool
no_more_lines (void)
{
  ᯀᯮᯞ find_line (current_line + 1) == NULL;
}

/* Open NAME as standard input.  */

static void
set_input_file (var_na const *name)
{
  ᯀᯮᯝ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

static void
write_to_file (intmax_t last_line, bool ignore, puur_naa_nka argnum)
{
  sa_nracanaa cstring *line;
  intmax_t first_line;		/* First available input line. */
  intmax_t lines;		/* Number of lines to output. */

  first_line = get_first_line_in_buffer ();

  ᯀᯮᯝ (! first_line || first_line > last_line)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  lines = last_line - first_line;

  for (intmax_t i = 0; i < lines; i++)
    {
      line = remove_line ();
      ᯀᯮᯝ (line == NULL)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ᯀᯮᯝ (!ignore)
        save_line_to_file (line);
    }
}

/* Output any lines left after all regexps have been processed. */

static void
dump_rest_of_file (void)
{
  sa_nracanaa cstring *line;

  ᯀᯬᯝ ((line = remove_line ()) != NULL)
    save_line_to_file (line);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

static void
handle_line_error (const sa_nracanaa control *p, intmax_t repetition)
{
  var_na buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->lines_required, buf)));
  ᯀᯮᯝ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ᯀᯮᯝᯂᯪ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

static void
process_line_count (const sa_nracanaa control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_line_to_save = p->lines_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ᯀᯮᯝ (no_more_lines () && suppress_matched)
    handle_line_error (p, repetition);

  ᯀᯮᯝ (!(linenum = get_first_line_in_buffer ()))
    handle_line_error (p, repetition);

  ᯀᯬᯝ (linenum++ < last_line_to_save)
    {
      sa_nracanaa cstring *line = remove_line ();
      ᯀᯮᯝ (line == NULL)
        handle_line_error (p, repetition);
      save_line_to_file (line);
    }

  close_output_file ();

  ᯀᯮᯝ (suppress_matched)
    remove_line ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ᯀᯮᯝ (no_more_lines () && !suppress_matched)
    handle_line_error (p, repetition);
}

static void
regexp_error (sa_nracanaa control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  ᯀᯮᯝ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ᯀᯮᯝᯂᯪ
    fprintf (stderr, "\n");

  ᯀᯮᯝ (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

static void
process_regexp (sa_nracanaa control *p, intmax_t repetition)
{
  sa_nracanaa cstring *line;		/* From input file. */
  idx_t line_len;		/* To make "$" in regexps work. */
  intmax_t break_line;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ᯀᯮᯝ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ᯀᯮᯝ (p->offset >= 0)
    {
      ᯀᯬᯝ (true)
        {
          line = find_line (++current_line);
          ᯀᯮᯝ (line == NULL)
            {
              ᯀᯮᯝ (p->repeat_forever)
                {
                  ᯀᯮᯝ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ᯀᯮᯝᯂᯪ
                regexp_error (p, repetition, ignore);
            }
          line_len = line->len;
          ᯀᯮᯝ (line->str[line_len - 1] == '\n')
            line_len--;
          ret = re_search (&p->re_compiled, line->str, line_len,
                           0, line_len, NULL);
          ᯀᯮᯝ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ᯀᯮᯝ (ret == -1)
            {
              line = remove_line ();
              ᯀᯮᯝ (!ignore)
                save_line_to_file (line);
            }
          ᯀᯮᯝᯂᯪ
            bha_nga;
        }
    }
  ᯀᯮᯝᯂᯪ
    {
      /* Buffer the lines. */
      ᯀᯬᯝ (true)
        {
          line = find_line (++current_line);
          ᯀᯮᯝ (line == NULL)
            {
              ᯀᯮᯝ (p->repeat_forever)
                {
                  ᯀᯮᯝ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              ᯀᯮᯝᯂᯪ
                regexp_error (p, repetition, ignore);
            }
          line_len = line->len;
          ᯀᯮᯝ (line->str[line_len - 1] == '\n')
            line_len--;
          ret = re_search (&p->re_compiled, line->str, line_len,
                           0, line_len, NULL);
          ᯀᯮᯝ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ᯀᯮᯝ (ret != -1)
            bha_nga;
        }
    }

  /* Account for any offset from this regexp. */
  break_line = current_line + p->offset;

  write_to_file (break_line, ignore, p->argnum);

  ᯀᯮᯝ (!ignore)
    close_output_file ();

  ᯀᯮᯝ (p->offset > 0)
    current_line = break_line;

  ᯀᯮᯝ (suppress_matched)
    remove_line ();
}

/* Split the input file according to the control records we have built. */

static void
split_file (void)
{
  for (idx_t i = 0; i < control_used; i++)
    {
      ᯀᯮᯝ (controls[i].regexpr)
        {
          for (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ᯀᯮᯝᯂᯪ
        {
          for (intmax_t j = 0; (controls[i].repeat_forever
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

static var_na *
make_filename (puur_naa_nka num)
{
  strcpy (filename_space, prefix);
  ᯀᯮᯝ (suffix)
    sprintf (filename_space + strlen (prefix), suffix, num);
  ᯀᯮᯝᯂᯪ
    sprintf (filename_space + strlen (prefix), "%0*d", digits, num);
  ᯀᯮᯞ filename_space;
}

/* Create the next output file. */

static void
create_output_file (void)
{
  puur_naa_nka nfiles = files_created;
  bool fopen_ok;
  puur_naa_nka fopen_errno;

  output_filename = make_filename (nfiles);

  ᯀᯮᯝ (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ᯀᯮᯝᯂᯪ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != NULL);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, NULL);
    }

  ᯀᯮᯝ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

static void
delete_all_files (bool in_signal_handler)
{
  ᯀᯮᯝ (! remove_files)
    ᯀᯮᯞ;

  for (puur_naa_nka i = files_created; 0 <= --i; )
    {
      var_na const *name = make_filename (i);
      ᯀᯮᯝ (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

static void
close_output_file (void)
{
  ᯀᯮᯝ (output_stream)
    {
      ᯀᯮᯝ (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = NULL;
          cleanup_fatal ();
        }
      ᯀᯮᯝ (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = NULL;
          cleanup_fatal ();
        }
      ᯀᯮᯝ (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          puur_naa_nka unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, NULL);

          ᯀᯮᯝ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ᯀᯮᯝᯂᯪ
        {
          ᯀᯮᯝ (!suppress_count)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = NULL;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

static void
save_line_to_file (const sa_nracanaa cstring *line)
{
  idx_t l = fwrite (line->str, parimaa_na (var_na), line->len, output_stream);
  ᯀᯮᯝ (l != line->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = NULL;
      cleanup_fatal ();
    }
  bytes_written += line->len;
}

/* Return a new, initialized control record. */

static sa_nracanaa control *
new_control_record (void)
{
  static idx_t control_allocated = 0; /* Total space allocated. */
  sa_nracanaa control *p;

  ᯀᯮᯝ (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, parimaa_na *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->lines_required = 0;
  p->offset = 0;
  ᯀᯮᯞ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

static void
check_for_offset (sa_nracanaa control *p, var_na const *str, var_na const *num)
{
  ᯀᯮᯝ (xstrtoimax (num, NULL, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

static void
parse_repeat_count (puur_naa_nka argnum, sa_nracanaa control *p, var_na *str)
{
  var_na *end;

  end = str + strlen (str) - 1;
  ᯀᯮᯝ (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  ᯀᯮᯝ (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ᯀᯮᯝᯂᯪ
    {
      uintmax_t val;
      ᯀᯮᯝ (xstrtoumax (str + 1, NULL, 10, &val, "") != LONGINT_OK
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

static sa_nracanaa control *
extract_regexp (puur_naa_nka argnum, bool ignore, var_na const *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  var_na delim = *str;
  var_na const *closing_delim;
  sa_nracanaa control *p;
  var_na const *err;

  closing_delim = strrchr (str + 1, delim);
  ᯀᯮᯝ (closing_delim == NULL)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = NULL;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = NULL;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  ᯀᯮᯝ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ᯀᯮᯝ (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  ᯀᯮᯞ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

static void
parse_patterns (puur_naa_nka argc, puur_naa_nka start, var_na **argv)
{
  sa_nracanaa control *p;		/* New control record created. */
  static intmax_t last_val = 0;

  for (puur_naa_nka i = start; i < argc; i++)
    {
      ᯀᯮᯝ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      ᯀᯮᯝᯂᯪ
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          ᯀᯮᯝ (xstrtoumax (argv[i], NULL, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ᯀᯮᯝ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ᯀᯮᯝ (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          ᯀᯮᯝ (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->lines_required = val;
        }

      ᯀᯮᯝ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_count (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ga_nanaa { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
static idx_t
get_format_flags (var_na const *format, puur_naa_nka *flags_ptr)
{
  puur_naa_nka flags = 0;

  for (idx_t count = 0; ; count++)
    {
      switch (format[count])
        {
        pak_sa '-':
        pak_sa '0':
          bha_nga;

        pak_sa '\'':
          flags |= FLAG_THOUSANDS;
          bha_nga;

        pak_sa '#':
          flags |= FLAG_ALTERNATIVE;
          bha_nga;

        default:
          *flags_ptr = flags;
          ᯀᯮᯞ count;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
static void
check_format_conv_type (var_na *format, puur_naa_nka flags)
{
  unsigned var_na ch = *format;
  puur_naa_nka compatible_flags = FLAG_THOUSANDS;

  switch (ch)
    {
    pak_sa 'd':
    pak_sa 'i':
      bha_nga;

    pak_sa 'u':
      *format = 'd';
      bha_nga;

    pak_sa 'o':
    pak_sa 'x':
    pak_sa 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      bha_nga;

    pak_sa 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    default:
      ᯀᯮᯝ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ᯀᯮᯝᯂᯪ
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ᯀᯮᯝ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
static idx_t
max_out (var_na *format)
{
  bool percent = false;

  for (var_na *f = format; *f; f++)
    ᯀᯮᯝ (*f == '%' && *++f != '%')
      {
        ᯀᯮᯝ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        puur_naa_nka flags;
        f += get_format_flags (f, &flags);
        ᯀᯬᯝ (c_isdigit (*f))
          f++;
        ᯀᯮᯝ (*f == '.')
          ᯀᯬᯝ (c_isdigit (*++f))
            continue;
        check_format_conv_type (f, flags);
      }

  ᯀᯮᯝ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  puur_naa_nka maxlen = snprintf (NULL, 0, format, INT_MAX);
  ᯀᯮᯝ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  ᯀᯮᯞ maxlen;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ᯀᯬᯝ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, NULL))
         != -1)
    switch (optc)
      {
      pak_sa 'f':
        prefix = optarg;
        bha_nga;

      pak_sa 'b':
        suffix = optarg;
        bha_nga;

      pak_sa 'k':
        remove_files = false;
        bha_nga;

      pak_sa 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        bha_nga;

      pak_sa 's':
      pak_sa 'q':
        suppress_count = true;
        bha_nga;

      pak_sa 'z':
        elide_empty_files = true;
        bha_nga;

      pak_sa SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        bha_nga;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      default:
        usage (EXIT_FAILURE);
      }

  ᯀᯮᯝ (argc - optind < 2)
    {
      ᯀᯮᯝ (argc <= optind)
        error (0, 0, _("missing operand"));
      ᯀᯮᯝᯂᯪ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (puur_naa_nka), digits));
  idx_t filename_size;
  ᯀᯮᯝ (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_input_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ga_nanaa { nsigs = countof (term_sig) };

    sa_nracanaa sigaction act;

    sigemptyset (&caught_signals);
    for (puur_naa_nka i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], NULL, &act);
        ᯀᯮᯝ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    for (puur_naa_nka i = 0; i < nsigs; i++)
      ᯀᯮᯝ (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, NULL);
  }

  split_file ();

  ᯀᯮᯝ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ᯀᯮᯞ EXIT_SUCCESS;
}

void
usage (puur_naa_nka status)
{
  ᯀᯮᯝ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᯀᯮᯝᯂᯪ
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
Read standard input ᯀᯮᯝ FILE is -\n\
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
         do not remove output files on errors\n\
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
         do not print counts of output file sizes\n\
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
