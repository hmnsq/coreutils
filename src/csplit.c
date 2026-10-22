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

#شامل <config.h>

#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>

#شامل "system.h"

#شامل <regex.h>

#شامل "c-ctype.h"
#شامل "fd-reopen.h"
#شامل "quote.h"
#شامل "safe-read.h"
#شامل "stdio--.h"
#شامل "term-sig.h"
#شامل "xdectoint.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
ساختار control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t سطرها_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحیح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ساختار re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
ساختار cstring
{
  idx_t len;
  نویسه *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ساختار سطر
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t درج_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  ساختار cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  ساختار سطر *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ساختار buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_سطر;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t رقم_سطرها;		/* Number of complete lines in this buffer. */
  نویسه *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  ساختار سطر *سطر_start;	/* Head of list of pointers to lines. */
  ساختار سطر *curr_سطر;	/* The line start record currently in use. */
  ساختار buffer_record *next;
};

ایستا تهی close_output_file (تهی);
ایستا تهی create_output_file (تهی);
ایستا تهی delete_all_files (bool);
ایستا تهی save_سطر_to_file (ثابت ساختار cstring *سطر);

/* Start of buffer list. */
ایستا ساختار buffer_record *head = پوچ;

/* Partially read line. */
ایستا نویسه *hold_area = پوچ;

/* Number of bytes in 'hold_area'. */
ایستا idx_t hold_شمار = 0;

/* Number of the last line in the buffers. */
ایستا intmax_t last_سطر_عدد = 0;

/* Number of the line currently being examined. */
ایستا intmax_t current_سطر = 0;

/* If true, we have read EOF. */
ایستا bool have_read_eof = false;

/* Name of output files. */
ایستا نویسه *فرار filename_space = پوچ;

/* Prefix part of output file names. */
ایستا نویسه ثابت *فرار prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
ایستا نویسه *فرار suffix = پوچ;

/* Number of digits to use in output file names. */
ایستا صحیح فرار digits = 2;

/* Number of files created so far. */
ایستا صحیح فرار files_created = 0;

/* Number of bytes written to current file. */
ایستا intmax_t bytes_written;

/* Output file pointer. */
ایستا FILE *output_stream = پوچ;

/* Output file name. */
ایستا نویسه *output_filename = پوچ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ایستا نویسه **global_argv;

/* If true, do not print the count of bytes in each output file. */
ایستا bool suppress_شمار;

/* If true, remove output files on error. */
ایستا bool فرار remove_files = true;

/* If true, remove all output files which have a zero length. */
ایستا bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
ایستا bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ایستا ساختار control *controls;

/* Number of elements in 'controls'. */
ایستا idx_t control_used;

/* The set of signals that are caught.  */
ایستا sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"digits", required_argument, پوچ, 'n'},
  {"quiet", no_argument, پوچ, 'q'},  /* Deprecated.  */
  {"silent", no_argument, پوچ, 's'},
  {"keep-files", no_argument, پوچ, 'k'},
  {"elide-empty-files", no_argument, پوچ, 'z'},
  {"prefix", required_argument, پوچ, 'f'},
  {"suffix-format", required_argument, پوچ, 'b'},
  {"suppress-matched", no_argument, پوچ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ایستا تهی
cleanup (تهی)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, پوچ);
}

ایستا _Noreturn تهی
cleanup_fatal (تهی)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

بیرونی تهی
xalloc_die (تهی)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ایستا تهی
interrupt_handler (صحیح sig)
{
  delete_all_files (true);
  سیگنال (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  بالاببر (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ایستا تهی
save_to_hold_area (نویسه *start, idx_t رقم)
{
  آزاد (hold_area);
  hold_area = start;
  hold_شمار = رقم;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ایستا idx_t
read_درون_داد (نویسه *dest, idx_t max_n_bytes)
{
  اگر (max_n_bytes == 0)
    بازگشت 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  اگر (bytes_read == 0)
    have_read_eof = true;

  اگر (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  بازگشت bytes_read;
}

/* Initialize existing line record P. */

ایستا تهی
clear_سطر_control (ساختار سطر *p)
{
  p->used = 0;
  p->درج_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

ایستا ساختار سطر *
new_سطر_control (تهی)
{
  ساختار سطر *p = xmalloc (اندازه *p);

  p->next = پوچ;
  clear_سطر_control (p);

  بازگشت p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ایستا تهی
keep_new_سطر (ساختار buffer_record *b, نویسه *سطر_start, idx_t سطر_len)
{
  ساختار سطر *l;

  /* If there is no existing area to keep line info, get some. */
  اگر (b->سطر_start == پوچ)
    b->سطر_start = b->curr_سطر = new_سطر_control ();

  /* If existing area for lines is full, get more. */
  اگر (b->curr_سطر->used == CTRL_SIZE)
    {
      b->curr_سطر->next = new_سطر_control ();
      b->curr_سطر = b->curr_سطر->next;
    }

  l = b->curr_سطر;

  /* Record the start of the line, and update counters. */
  l->starts[l->درج_index].str = سطر_start;
  l->starts[l->درج_index].len = سطر_len;
  l->used++;
  l->درج_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ایستا idx_t
record_سطر_starts (ساختار buffer_record *b)
{
  نویسه *سطر_start;		/* Start of current line. */
  idx_t سطرها;			/* Number of lines found. */
  idx_t سطر_length;		/* Length of each line found. */

  اگر (b->bytes_used == 0)
    بازگشت 0;

  سطرها = 0;
  سطر_start = b->buffer;
  نویسه *buffer_end = سطر_start + b->bytes_used;
  *buffer_end = '\n';

  وقتی (true)
    {
      نویسه *سطر_end = rawmemchr (سطر_start, '\n');
      اگر (سطر_end == buffer_end)
        بشکن;
      سطر_length = سطر_end - سطر_start + 1;
      keep_new_سطر (b, سطر_start, سطر_length);
      سطر_start = سطر_end + 1;
      سطرها++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_چپ = buffer_end - سطر_start;
  اگر (bytes_چپ)
    {
      اگر (have_read_eof)
        {
          keep_new_سطر (b, سطر_start, bytes_چپ);
          سطرها++;
        }
      وگرنه
        save_to_hold_area (ximemdup (سطر_start, bytes_چپ), bytes_چپ);
    }

  b->رقم_سطرها = سطرها;
  b->first_available = b->start_سطر = last_سطر_عدد + 1;
  last_سطر_عدد += سطرها;

  بازگشت سطرها;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#اگر 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ایستا تهی
free_buffer (ساختار buffer_record *buf)
{
  برای (ساختار سطر *l = buf->سطر_start; l;)
    {
      ساختار سطر *n = l->next;
      آزاد (l);
      l = n;
    }
  آزاد (buf->buffer);
  آزاد (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ایستا ATTRIBUTE_DEALLOC (free_buffer, 1)
ساختار buffer_record *
get_new_buffer (idx_t min_size)
{
  ساختار buffer_record *new_buffer = xmalloc (اندازه *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (پوچ, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_سطر = new_buffer->first_available = last_سطر_عدد + 1;
  new_buffer->رقم_سطرها = 0;
  new_buffer->سطر_start = new_buffer->curr_سطر = پوچ;
  new_buffer->next = پوچ;

  بازگشت new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ایستا تهی
save_buffer (ساختار buffer_record *buf)
{
  buf->next = پوچ;
  buf->curr_سطر = buf->سطر_start;

  اگر (head == پوچ)
    head = buf;
  وگرنه
    {
      ساختار buffer_record *p;
      برای (p = head; p->next; p = p->next)
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

ایستا bool
load_buffer (تهی)
{
  اگر (have_read_eof)
    بازگشت false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_شمار + 1);

  وقتی (true)
    {
      ساختار buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      نویسه *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      اگر (hold_شمار)
        {
          p = mempcpy (p, hold_area, hold_شمار);
          b->bytes_used += hold_شمار;
          bytes_avail -= hold_شمار;
          hold_شمار = 0;
        }

      b->bytes_used += read_درون_داد (p, bytes_avail - 1);

      اگر (record_سطر_starts (b) != 0)
        {
          save_buffer (b);
          بازگشت true;
        }

      free_buffer (b);
      اگر (have_read_eof)
        بازگشت false;
      اگر (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ایستا intmax_t
get_first_سطر_in_buffer (تهی)
{
  اگر (head == پوچ && !load_buffer ())
    بازگشت 0;

  بازگشت head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ایستا ساختار cstring *
remove_سطر (تهی)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ایستا ساختار buffer_record *prev_buf = پوچ;

  ساختار cstring *سطر;		/* Return value. */
  ساختار سطر *l;		/* For convenience. */

  اگر (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = پوچ;
    }

  اگر (head == پوچ && !load_buffer ())
    بازگشت پوچ;

  اگر (current_سطر < head->first_available)
    current_سطر = head->first_available;

  ++(head->first_available);

  l = head->curr_سطر;

  سطر = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  اگر (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_سطر = l->next;
      اگر (head->curr_سطر == پوچ || head->curr_سطر->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  بازگشت سطر;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ایستا ساختار cstring *
find_سطر (intmax_t linenum)
{
  اگر (head == پوچ && !load_buffer ())
    بازگشت پوچ;

  اگر (linenum < head->start_سطر)
    بازگشت پوچ;

  برای (ساختار buffer_record *b = head;;)
    {
      اگر (linenum < b->start_سطر + b->رقم_سطرها)
        {
          /* The line is in this buffer. */
          ساختار سطر *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->سطر_start;
          offset = linenum - b->start_سطر;
          /* Find the control record. */
          وقتی (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          بازگشت &l->starts[offset];
        }
      اگر (b->next == پوچ && !load_buffer ())
        بازگشت پوچ;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ایستا bool
no_more_سطرها (تهی)
{
  بازگشت find_سطر (current_سطر + 1) == پوچ;
}

/* Open NAME as standard input.  */

ایستا تهی
set_درون_داد_file (نویسه ثابت *name)
{
  اگر (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ایستا تهی
write_to_file (intmax_t last_سطر, bool ignore, صحیح argnum)
{
  ساختار cstring *سطر;
  intmax_t first_سطر;		/* First available input line. */
  intmax_t سطرها;		/* Number of lines to output. */

  first_سطر = get_first_سطر_in_buffer ();

  اگر (! first_سطر || first_سطر > last_سطر)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  سطرها = last_سطر - first_سطر;

  برای (intmax_t i = 0; i < سطرها; i++)
    {
      سطر = remove_سطر ();
      اگر (سطر == پوچ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      اگر (!ignore)
        save_سطر_to_file (سطر);
    }
}

/* Output any lines left after all regexps have been processed. */

ایستا تهی
dump_rest_of_file (تهی)
{
  ساختار cstring *سطر;

  وقتی ((سطر = remove_سطر ()) != پوچ)
    save_سطر_to_file (سطر);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ایستا تهی
handle_سطر_error (ثابت ساختار control *p, intmax_t repetition)
{
  نویسه buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->سطرها_required, buf)));
  اگر (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وگرنه
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ایستا تهی
process_سطر_شمار (ثابت ساختار control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_سطر_to_save = p->سطرها_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  اگر (no_more_سطرها () && suppress_matched)
    handle_سطر_error (p, repetition);

  اگر (!(linenum = get_first_سطر_in_buffer ()))
    handle_سطر_error (p, repetition);

  وقتی (linenum++ < last_سطر_to_save)
    {
      ساختار cstring *سطر = remove_سطر ();
      اگر (سطر == پوچ)
        handle_سطر_error (p, repetition);
      save_سطر_to_file (سطر);
    }

  close_output_file ();

  اگر (suppress_matched)
    remove_سطر ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  اگر (no_more_سطرها () && !suppress_matched)
    handle_سطر_error (p, repetition);
}

ایستا تهی
regexp_error (ساختار control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  اگر (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وگرنه
    fprintf (stderr, "\n");

  اگر (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ایستا تهی
process_regexp (ساختار control *p, intmax_t repetition)
{
  ساختار cstring *سطر;		/* From input file. */
  idx_t سطر_len;		/* To make "$" in regexps work. */
  intmax_t break_سطر;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  اگر (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  اگر (p->offset >= 0)
    {
      وقتی (true)
        {
          سطر = find_سطر (++current_سطر);
          اگر (سطر == پوچ)
            {
              اگر (p->repeat_forever)
                {
                  اگر (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              وگرنه
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          اگر (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_جستجو (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, پوچ);
          اگر (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          اگر (ret == -1)
            {
              سطر = remove_سطر ();
              اگر (!ignore)
                save_سطر_to_file (سطر);
            }
          وگرنه
            بشکن;
        }
    }
  وگرنه
    {
      /* Buffer the lines. */
      وقتی (true)
        {
          سطر = find_سطر (++current_سطر);
          اگر (سطر == پوچ)
            {
              اگر (p->repeat_forever)
                {
                  اگر (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              وگرنه
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          اگر (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_جستجو (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, پوچ);
          اگر (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          اگر (ret != -1)
            بشکن;
        }
    }

  /* Account for any offset from this regexp. */
  break_سطر = current_سطر + p->offset;

  write_to_file (break_سطر, ignore, p->argnum);

  اگر (!ignore)
    close_output_file ();

  اگر (p->offset > 0)
    current_سطر = break_سطر;

  اگر (suppress_matched)
    remove_سطر ();
}

/* Split the input file according to the control records we have built. */

ایستا تهی
split_file (تهی)
{
  برای (idx_t i = 0; i < control_used; i++)
    {
      اگر (controls[i].regexpr)
        {
          برای (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      وگرنه
        {
          برای (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_سطر_شمار (&controls[i], j);
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

ایستا نویسه *
make_filename (صحیح رقم)
{
  strcpy (filename_space, prefix);
  اگر (suffix)
    sprintf (filename_space + strlen (prefix), suffix, رقم);
  وگرنه
    sprintf (filename_space + strlen (prefix), "%0*d", digits, رقم);
  بازگشت filename_space;
}

/* Create the next output file. */

ایستا تهی
create_output_file (تهی)
{
  صحیح nfiles = files_created;
  bool fopen_ok;
  صحیح fopen_errno;

  output_filename = make_filename (nfiles);

  اگر (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  وگرنه
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != پوچ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, پوچ);
    }

  اگر (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ایستا تهی
delete_all_files (bool in_signal_handler)
{
  اگر (! remove_files)
    بازگشت;

  برای (صحیح i = files_created; 0 <= --i; )
    {
      نویسه ثابت *name = make_filename (i);
      اگر (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ایستا تهی
close_output_file (تهی)
{
  اگر (output_stream)
    {
      اگر (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = پوچ;
          cleanup_fatal ();
        }
      اگر (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = پوچ;
          cleanup_fatal ();
        }
      اگر (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحیح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, پوچ);

          اگر (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      وگرنه
        {
          اگر (!suppress_شمار)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = پوچ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ایستا تهی
save_سطر_to_file (ثابت ساختار cstring *سطر)
{
  idx_t l = fwrite (سطر->str, اندازه (نویسه), سطر->len, output_stream);
  اگر (l != سطر->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = پوچ;
      cleanup_fatal ();
    }
  bytes_written += سطر->len;
}

/* Return a new, initialized control record. */

ایستا ساختار control *
new_control_record (تهی)
{
  ایستا idx_t control_allocated = 0; /* Total space allocated. */
  ساختار control *p;

  اگر (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, اندازه *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->سطرها_required = 0;
  p->offset = 0;
  بازگشت p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ایستا تهی
check_for_offset (ساختار control *p, نویسه ثابت *str, نویسه ثابت *رقم)
{
  اگر (xstrtoimax (رقم, پوچ, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ایستا تهی
parse_repeat_شمار (صحیح argnum, ساختار control *p, نویسه *str)
{
  نویسه *end;

  end = str + strlen (str) - 1;
  اگر (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  اگر (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  وگرنه
    {
      uintmax_t val;
      اگر (xstrtoumax (str + 1, پوچ, 10, &val, "") != LONGINT_OK
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

ایستا ساختار control *
extract_regexp (صحیح argnum, bool ignore, نویسه ثابت *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  نویسه delim = *str;
  نویسه ثابت *closing_delim;
  ساختار control *p;
  نویسه ثابت *err;

  closing_delim = strrchr (str + 1, delim);
  اگر (closing_delim == پوچ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = پوچ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = پوچ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  اگر (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  اگر (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  بازگشت p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ایستا تهی
parse_patterns (صحیح argc, صحیح start, نویسه **argv)
{
  ساختار control *p;		/* New control record created. */
  ایستا intmax_t last_val = 0;

  برای (صحیح i = start; i < argc; i++)
    {
      اگر (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      وگرنه
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          اگر (xstrtoumax (argv[i], پوچ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          اگر (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          اگر (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          اگر (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->سطرها_required = val;
        }

      اگر (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_شمار (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
شمارش { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ایستا idx_t
get_format_flags (نویسه ثابت *format, صحیح *flags_ptr)
{
  صحیح flags = 0;

  برای (idx_t شمار = 0; ; شمار++)
    {
      انتخاب (format[شمار])
        {
        حالت '-':
        حالت '0':
          بشکن;

        حالت '\'':
          flags |= FLAG_THOUSANDS;
          بشکن;

        حالت '#':
          flags |= FLAG_ALTERNATIVE;
          بشکن;

        پیشفرض:
          *flags_ptr = flags;
          بازگشت شمار;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ایستا تهی
check_format_conv_type (نویسه *format, صحیح flags)
{
  unsigned نویسه ch = *format;
  صحیح compatible_flags = FLAG_THOUSANDS;

  انتخاب (ch)
    {
    حالت 'd':
    حالت 'i':
      بشکن;

    حالت 'u':
      *format = 'd';
      بشکن;

    حالت 'o':
    حالت 'x':
    حالت 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      بشکن;

    حالت 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    پیشفرض:
      اگر (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      وگرنه
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  اگر (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ایستا idx_t
max_out (نویسه *format)
{
  bool percent = false;

  برای (نویسه *f = format; *f; f++)
    اگر (*f == '%' && *++f != '%')
      {
        اگر (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        صحیح flags;
        f += get_format_flags (f, &flags);
        وقتی (c_isdigit (*f))
          f++;
        اگر (*f == '.')
          وقتی (c_isdigit (*++f))
            ادامه;
        check_format_conv_type (f, flags);
      }

  اگر (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  صحیح maxlen = snprintf (پوچ, 0, format, INT_MAX);
  اگر (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  بازگشت maxlen;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  وقتی ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, پوچ))
         != -1)
    انتخاب (optc)
      {
      حالت 'f':
        prefix = optarg;
        بشکن;

      حالت 'b':
        suffix = optarg;
        بشکن;

      حالت 'k':
        remove_files = false;
        بشکن;

      حالت 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        بشکن;

      حالت 's':
      حالت 'q':
        suppress_شمار = true;
        بشکن;

      حالت 'z':
        elide_empty_files = true;
        بشکن;

      حالت SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        بشکن;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      پیشفرض:
        usage (EXIT_FAILURE);
      }

  اگر (argc - optind < 2)
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      وگرنه
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (صحیح), digits));
  idx_t filename_size;
  اگر (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_درون_داد_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    شمارش { nsigs = countof (term_sig) };

    ساختار sigaction act;

    sigemptyset (&caught_signals);
    برای (صحیح i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], پوچ, &act);
        اگر (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    برای (صحیح i = 0; i < nsigs; i++)
      اگر (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, پوچ);
  }

  split_file ();

  اگر (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  بازگشت EXIT_SUCCESS;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
Read standard درون_داد اگر FILE is -\n\
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
         انجامبده not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the سطرها matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified عدد of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         انجامبده not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified سطر عدد\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching سطر\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching سطر\n\
  {INTEGER}          repeat the previous pattern specified عدد of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A سطر OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
