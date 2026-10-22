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

#अन्तर्भाव <config.h>

#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव <regex.h>

#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "fd-reopen.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "safe-read.h"
#अन्तर्भाव "stdio--.h"
#अन्तर्भाव "term-sig.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
संरचना control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t ओळी_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  पूर्णांक argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  संरचना re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
संरचना cstring
{
  idx_t len;
  अक्षर *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
संरचना ओळ
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t घाला_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  संरचना cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  संरचना ओळ *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
संरचना buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_ओळ;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t अंक_ओळी;		/* Number of complete lines in this buffer. */
  अक्षर *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  संरचना ओळ *ओळ_start;	/* Head of list of pointers to lines. */
  संरचना ओळ *curr_ओळ;	/* The line start record currently in use. */
  संरचना buffer_record *next;
};

स्थिर शून्य close_output_file (शून्य);
स्थिर शून्य create_output_file (शून्य);
स्थिर शून्य delete_all_files (bool);
स्थिर शून्य save_ओळ_to_file (स्थिरांक संरचना cstring *ओळ);

/* Start of buffer list. */
स्थिर संरचना buffer_record *head = रिक्त;

/* Partially read line. */
स्थिर अक्षर *hold_area = रिक्त;

/* Number of bytes in 'hold_area'. */
स्थिर idx_t hold_मोजणी = 0;

/* Number of the last line in the buffers. */
स्थिर intmax_t last_ओळ_संख्या = 0;

/* Number of the line currently being examined. */
स्थिर intmax_t current_ओळ = 0;

/* If true, we have read EOF. */
स्थिर bool have_read_eof = false;

/* Name of output files. */
स्थिर अक्षर *अस्थिर filename_space = रिक्त;

/* Prefix part of output file names. */
स्थिर अक्षर स्थिरांक *अस्थिर prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
स्थिर अक्षर *अस्थिर suffix = रिक्त;

/* Number of digits to use in output file names. */
स्थिर पूर्णांक अस्थिर digits = 2;

/* Number of files created so far. */
स्थिर पूर्णांक अस्थिर files_created = 0;

/* Number of bytes written to current file. */
स्थिर intmax_t bytes_written;

/* Output file pointer. */
स्थिर खाता *output_stream = रिक्त;

/* Output file name. */
स्थिर अक्षर *output_filename = रिक्त;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
स्थिर अक्षर **global_argv;

/* If true, do not print the count of bytes in each output file. */
स्थिर bool suppress_मोजणी;

/* If true, remove output files on error. */
स्थिर bool अस्थिर remove_files = true;

/* If true, remove all output files which have a zero length. */
स्थिर bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
स्थिर bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
स्थिर संरचना control *controls;

/* Number of elements in 'controls'. */
स्थिर idx_t control_used;

/* The set of signals that are caught.  */
स्थिर sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  SUPPRESS_MATCHED_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"digits", required_argument, रिक्त, 'n'},
  {"quiet", no_argument, रिक्त, 'q'},  /* Deprecated.  */
  {"silent", no_argument, रिक्त, 's'},
  {"keep-files", no_argument, रिक्त, 'k'},
  {"elide-empty-files", no_argument, रिक्त, 'z'},
  {"prefix", required_argument, रिक्त, 'f'},
  {"suffix-format", required_argument, रिक्त, 'b'},
  {"suppress-matched", no_argument, रिक्त, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

स्थिर शून्य
cleanup (शून्य)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, रिक्त);
}

स्थिर _Noreturn शून्य
cleanup_fatal (शून्य)
{
  cleanup ();
  निकास (निकास_त्रुटि);
}

बाह्य शून्य
xalloc_die (शून्य)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

स्थिर शून्य
interrupt_handler (पूर्णांक sig)
{
  delete_all_files (true);
  संकेत (sig, संक_पूर्व);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  उठवा (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

स्थिर शून्य
save_to_hold_area (अक्षर *start, idx_t अंक)
{
  मुक्त (hold_area);
  hold_area = start;
  hold_मोजणी = अंक;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

स्थिर idx_t
read_आदान (अक्षर *dest, idx_t max_n_bytes)
{
  जर (max_n_bytes == 0)
    परत 0;

  सूचक_भेद_प्रकार bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  जर (bytes_read == 0)
    have_read_eof = true;

  जर (bytes_read < 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  परत bytes_read;
}

/* Initialize existing line record P. */

स्थिर शून्य
clear_ओळ_control (संरचना ओळ *p)
{
  p->used = 0;
  p->घाला_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

स्थिर संरचना ओळ *
new_ओळ_control (शून्य)
{
  संरचना ओळ *p = xmalloc (आकार *p);

  p->next = रिक्त;
  clear_ओळ_control (p);

  परत p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

स्थिर शून्य
keep_new_ओळ (संरचना buffer_record *b, अक्षर *ओळ_start, idx_t ओळ_len)
{
  संरचना ओळ *l;

  /* If there is no existing area to keep line info, get some. */
  जर (b->ओळ_start == रिक्त)
    b->ओळ_start = b->curr_ओळ = new_ओळ_control ();

  /* If existing area for lines is full, get more. */
  जर (b->curr_ओळ->used == CTRL_SIZE)
    {
      b->curr_ओळ->next = new_ओळ_control ();
      b->curr_ओळ = b->curr_ओळ->next;
    }

  l = b->curr_ओळ;

  /* Record the start of the line, and update counters. */
  l->starts[l->घाला_index].str = ओळ_start;
  l->starts[l->घाला_index].len = ओळ_len;
  l->used++;
  l->घाला_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

स्थिर idx_t
record_ओळ_starts (संरचना buffer_record *b)
{
  अक्षर *ओळ_start;		/* Start of current line. */
  idx_t ओळी;			/* Number of lines found. */
  idx_t ओळ_length;		/* Length of each line found. */

  जर (b->bytes_used == 0)
    परत 0;

  ओळी = 0;
  ओळ_start = b->buffer;
  अक्षर *buffer_end = ओळ_start + b->bytes_used;
  *buffer_end = '\n';

  पर्यंत (true)
    {
      अक्षर *ओळ_end = rawmemchr (ओळ_start, '\n');
      जर (ओळ_end == buffer_end)
        थांबा;
      ओळ_length = ओळ_end - ओळ_start + 1;
      keep_new_ओळ (b, ओळ_start, ओळ_length);
      ओळ_start = ओळ_end + 1;
      ओळी++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_डावा = buffer_end - ओळ_start;
  जर (bytes_डावा)
    {
      जर (have_read_eof)
        {
          keep_new_ओळ (b, ओळ_start, bytes_डावा);
          ओळी++;
        }
      अन्यथा
        save_to_hold_area (ximemdup (ओळ_start, bytes_डावा), bytes_डावा);
    }

  b->अंक_ओळी = ओळी;
  b->first_available = b->start_ओळ = last_ओळ_संख्या + 1;
  last_ओळ_संख्या += ओळी;

  परत ओळी;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#जर 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

स्थिर शून्य
free_buffer (संरचना buffer_record *buf)
{
  साठी (संरचना ओळ *l = buf->ओळ_start; l;)
    {
      संरचना ओळ *n = l->next;
      मुक्त (l);
      l = n;
    }
  मुक्त (buf->buffer);
  मुक्त (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

स्थिर ATTRIBUTE_DEALLOC (free_buffer, 1)
संरचना buffer_record *
get_new_buffer (idx_t min_size)
{
  संरचना buffer_record *new_buffer = xmalloc (आकार *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (रिक्त, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_ओळ = new_buffer->first_available = last_ओळ_संख्या + 1;
  new_buffer->अंक_ओळी = 0;
  new_buffer->ओळ_start = new_buffer->curr_ओळ = रिक्त;
  new_buffer->next = रिक्त;

  परत new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

स्थिर शून्य
save_buffer (संरचना buffer_record *buf)
{
  buf->next = रिक्त;
  buf->curr_ओळ = buf->ओळ_start;

  जर (head == रिक्त)
    head = buf;
  अन्यथा
    {
      संरचना buffer_record *p;
      साठी (p = head; p->next; p = p->next)
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

स्थिर bool
load_buffer (शून्य)
{
  जर (have_read_eof)
    परत false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_मोजणी + 1);

  पर्यंत (true)
    {
      संरचना buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      अक्षर *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      जर (hold_मोजणी)
        {
          p = mempcpy (p, hold_area, hold_मोजणी);
          b->bytes_used += hold_मोजणी;
          bytes_avail -= hold_मोजणी;
          hold_मोजणी = 0;
        }

      b->bytes_used += read_आदान (p, bytes_avail - 1);

      जर (record_ओळ_starts (b) != 0)
        {
          save_buffer (b);
          परत true;
        }

      free_buffer (b);
      जर (have_read_eof)
        परत false;
      जर (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

स्थिर intmax_t
get_first_ओळ_in_buffer (शून्य)
{
  जर (head == रिक्त && !load_buffer ())
    परत 0;

  परत head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

स्थिर संरचना cstring *
remove_ओळ (शून्य)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  स्थिर संरचना buffer_record *prev_buf = रिक्त;

  संरचना cstring *ओळ;		/* Return value. */
  संरचना ओळ *l;		/* For convenience. */

  जर (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = रिक्त;
    }

  जर (head == रिक्त && !load_buffer ())
    परत रिक्त;

  जर (current_ओळ < head->first_available)
    current_ओळ = head->first_available;

  ++(head->first_available);

  l = head->curr_ओळ;

  ओळ = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  जर (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_ओळ = l->next;
      जर (head->curr_ओळ == रिक्त || head->curr_ओळ->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  परत ओळ;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

स्थिर संरचना cstring *
find_ओळ (intmax_t linenum)
{
  जर (head == रिक्त && !load_buffer ())
    परत रिक्त;

  जर (linenum < head->start_ओळ)
    परत रिक्त;

  साठी (संरचना buffer_record *b = head;;)
    {
      जर (linenum < b->start_ओळ + b->अंक_ओळी)
        {
          /* The line is in this buffer. */
          संरचना ओळ *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->ओळ_start;
          offset = linenum - b->start_ओळ;
          /* Find the control record. */
          पर्यंत (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          परत &l->starts[offset];
        }
      जर (b->next == रिक्त && !load_buffer ())
        परत रिक्त;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

स्थिर bool
no_more_ओळी (शून्य)
{
  परत find_ओळ (current_ओळ + 1) == रिक्त;
}

/* Open NAME as standard input.  */

स्थिर शून्य
set_आदान_file (अक्षर स्थिरांक *name)
{
  जर (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

स्थिर शून्य
write_to_file (intmax_t last_ओळ, bool ignore, पूर्णांक argnum)
{
  संरचना cstring *ओळ;
  intmax_t first_ओळ;		/* First available input line. */
  intmax_t ओळी;		/* Number of lines to output. */

  first_ओळ = get_first_ओळ_in_buffer ();

  जर (! first_ओळ || first_ओळ > last_ओळ)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  ओळी = last_ओळ - first_ओळ;

  साठी (intmax_t i = 0; i < ओळी; i++)
    {
      ओळ = remove_ओळ ();
      जर (ओळ == रिक्त)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      जर (!ignore)
        save_ओळ_to_file (ओळ);
    }
}

/* Output any lines left after all regexps have been processed. */

स्थिर शून्य
dump_rest_of_file (शून्य)
{
  संरचना cstring *ओळ;

  पर्यंत ((ओळ = remove_ओळ ()) != रिक्त)
    save_ओळ_to_file (ओळ);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

स्थिर शून्य
handle_ओळ_error (स्थिरांक संरचना control *p, intmax_t repetition)
{
  अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];

  ख_लिखो (मानक_त्रुटि, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->ओळी_required, buf)));
  जर (repetition)
    ख_लिखो (मानक_त्रुटि, _(" on repetition %jd\n"), repetition);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

स्थिर शून्य
process_ओळ_मोजणी (स्थिरांक संरचना control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_ओळ_to_save = p->ओळी_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  जर (no_more_ओळी () && suppress_matched)
    handle_ओळ_error (p, repetition);

  जर (!(linenum = get_first_ओळ_in_buffer ()))
    handle_ओळ_error (p, repetition);

  पर्यंत (linenum++ < last_ओळ_to_save)
    {
      संरचना cstring *ओळ = remove_ओळ ();
      जर (ओळ == रिक्त)
        handle_ओळ_error (p, repetition);
      save_ओळ_to_file (ओळ);
    }

  close_output_file ();

  जर (suppress_matched)
    remove_ओळ ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  जर (no_more_ओळी () && !suppress_matched)
    handle_ओळ_error (p, repetition);
}

स्थिर शून्य
regexp_error (संरचना control *p, intmax_t repetition, bool ignore)
{
  ख_लिखो (मानक_त्रुटि, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  जर (repetition)
    ख_लिखो (मानक_त्रुटि, _(" on repetition %jd\n"), repetition);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "\n");

  जर (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

स्थिर शून्य
process_regexp (संरचना control *p, intmax_t repetition)
{
  संरचना cstring *ओळ;		/* From input file. */
  idx_t ओळ_len;		/* To make "$" in regexps work. */
  intmax_t break_ओळ;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  जर (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  जर (p->offset >= 0)
    {
      पर्यंत (true)
        {
          ओळ = find_ओळ (++current_ओळ);
          जर (ओळ == रिक्त)
            {
              जर (p->repeat_forever)
                {
                  जर (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  निकास (निकास_सफल);
                }
              अन्यथा
                regexp_error (p, repetition, ignore);
            }
          ओळ_len = ओळ->len;
          जर (ओळ->str[ओळ_len - 1] == '\n')
            ओळ_len--;
          ret = re_शोध (&p->re_compiled, ओळ->str, ओळ_len,
                           0, ओळ_len, रिक्त);
          जर (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          जर (ret == -1)
            {
              ओळ = remove_ओळ ();
              जर (!ignore)
                save_ओळ_to_file (ओळ);
            }
          अन्यथा
            थांबा;
        }
    }
  अन्यथा
    {
      /* Buffer the lines. */
      पर्यंत (true)
        {
          ओळ = find_ओळ (++current_ओळ);
          जर (ओळ == रिक्त)
            {
              जर (p->repeat_forever)
                {
                  जर (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  निकास (निकास_सफल);
                }
              अन्यथा
                regexp_error (p, repetition, ignore);
            }
          ओळ_len = ओळ->len;
          जर (ओळ->str[ओळ_len - 1] == '\n')
            ओळ_len--;
          ret = re_शोध (&p->re_compiled, ओळ->str, ओळ_len,
                           0, ओळ_len, रिक्त);
          जर (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          जर (ret != -1)
            थांबा;
        }
    }

  /* Account for any offset from this regexp. */
  break_ओळ = current_ओळ + p->offset;

  write_to_file (break_ओळ, ignore, p->argnum);

  जर (!ignore)
    close_output_file ();

  जर (p->offset > 0)
    current_ओळ = break_ओळ;

  जर (suppress_matched)
    remove_ओळ ();
}

/* Split the input file according to the control records we have built. */

स्थिर शून्य
split_file (शून्य)
{
  साठी (idx_t i = 0; i < control_used; i++)
    {
      जर (controls[i].regexpr)
        {
          साठी (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      अन्यथा
        {
          साठी (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_ओळ_मोजणी (&controls[i], j);
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

स्थिर अक्षर *
make_filename (पूर्णांक अंक)
{
  म_नकल (filename_space, prefix);
  जर (suffix)
    प्र_लिखो (filename_space + म_माप (prefix), suffix, अंक);
  अन्यथा
    प्र_लिखो (filename_space + म_माप (prefix), "%0*d", digits, अंक);
  परत filename_space;
}

/* Create the next output file. */

स्थिर शून्य
create_output_file (शून्य)
{
  पूर्णांक nfiles = files_created;
  bool fopen_ok;
  पूर्णांक fopen_errno;

  output_filename = make_filename (nfiles);

  जर (nfiles == पूर्णांक_उच्च)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  अन्यथा
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = ख_खोलो (output_filename, "w");
      fopen_ok = (output_stream != रिक्त);
      fopen_errno = त्रुटि_सं;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, रिक्त);
    }

  जर (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

स्थिर शून्य
delete_all_files (bool in_signal_handler)
{
  जर (! remove_files)
    परत;

  साठी (पूर्णांक i = files_created; 0 <= --i; )
    {
      अक्षर स्थिरांक *name = make_filename (i);
      जर (unlink (name) != 0 && त्रुटि_सं != ENOENT && !in_signal_handler)
        error (0, त्रुटि_सं, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

स्थिर शून्य
close_output_file (शून्य)
{
  जर (output_stream)
    {
      जर (ख_त्रुटि (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = रिक्त;
          cleanup_fatal ();
        }
      जर (ख_बंद (output_stream) != 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (output_filename));
          output_stream = रिक्त;
          cleanup_fatal ();
        }
      जर (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          पूर्णांक unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = त्रुटि_सं;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, रिक्त);

          जर (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      अन्यथा
        {
          जर (!suppress_मोजणी)
            ख_लिखो (मानक_निकास, "%jd\n", bytes_written);
        }
      output_stream = रिक्त;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

स्थिर शून्य
save_ओळ_to_file (स्थिरांक संरचना cstring *ओळ)
{
  idx_t l = ख_डालो (ओळ->str, आकार (अक्षर), ओळ->len, output_stream);
  जर (l != ओळ->len)
    {
      error (0, त्रुटि_सं, _("write error for %s"), quoteaf (output_filename));
      output_stream = रिक्त;
      cleanup_fatal ();
    }
  bytes_written += ओळ->len;
}

/* Return a new, initialized control record. */

स्थिर संरचना control *
new_control_record (शून्य)
{
  स्थिर idx_t control_allocated = 0; /* Total space allocated. */
  संरचना control *p;

  जर (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, आकार *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->ओळी_required = 0;
  p->offset = 0;
  परत p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

स्थिर शून्य
check_for_offset (संरचना control *p, अक्षर स्थिरांक *str, अक्षर स्थिरांक *अंक)
{
  जर (xstrtoimax (अंक, रिक्त, 10, &p->offset, "") != LONGINT_OK)
    error (निकास_त्रुटि, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

स्थिर शून्य
parse_repeat_मोजणी (पूर्णांक argnum, संरचना control *p, अक्षर *str)
{
  अक्षर *end;

  end = str + म_माप (str) - 1;
  जर (*end != '}')
    error (निकास_त्रुटि, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  जर (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  अन्यथा
    {
      uintmax_t val;
      जर (xstrtoumax (str + 1, रिक्त, 10, &val, "") != LONGINT_OK
          || ckd_add (&p->repeat, val, 0))
        {
          error (निकास_त्रुटि, 0,
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

स्थिर संरचना control *
extract_regexp (पूर्णांक argnum, bool ignore, अक्षर स्थिरांक *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  अक्षर delim = *str;
  अक्षर स्थिरांक *closing_delim;
  संरचना control *p;
  अक्षर स्थिरांक *err;

  closing_delim = म_खोजप (str + 1, delim);
  जर (closing_delim == रिक्त)
    error (निकास_त्रुटि, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = रिक्त;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (अच_अक्षर_उच्च + 1);
  p->re_compiled.translate = रिक्त;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  जर (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  जर (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  परत p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

स्थिर शून्य
parse_patterns (पूर्णांक argc, पूर्णांक start, अक्षर **argv)
{
  संरचना control *p;		/* New control record created. */
  स्थिर intmax_t last_val = 0;

  साठी (पूर्णांक i = start; i < argc; i++)
    {
      जर (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      अन्यथा
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          जर (xstrtoumax (argv[i], रिक्त, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (निकास_त्रुटि, 0, _("%s: invalid pattern"), quote (argv[i]));
          जर (val == 0)
            error (निकास_त्रुटि, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          जर (val < last_val)
            error (निकास_त्रुटि, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          जर (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->ओळी_required = val;
        }

      जर (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_मोजणी (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
गणना { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
स्थिर idx_t
get_format_flags (अक्षर स्थिरांक *format, पूर्णांक *flags_ptr)
{
  पूर्णांक flags = 0;

  साठी (idx_t मोजणी = 0; ; मोजणी++)
    {
      निवड (format[मोजणी])
        {
        स्थिती '-':
        स्थिती '0':
          थांबा;

        स्थिती '\'':
          flags |= FLAG_THOUSANDS;
          थांबा;

        स्थिती '#':
          flags |= FLAG_ALTERNATIVE;
          थांबा;

        पूर्वनिर्धारित:
          *flags_ptr = flags;
          परत मोजणी;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
स्थिर शून्य
check_format_conv_type (अक्षर *format, पूर्णांक flags)
{
  अचिन्हित अक्षर ch = *format;
  पूर्णांक compatible_flags = FLAG_THOUSANDS;

  निवड (ch)
    {
    स्थिती 'd':
    स्थिती 'i':
      थांबा;

    स्थिती 'u':
      *format = 'd';
      थांबा;

    स्थिती 'o':
    स्थिती 'x':
    स्थिती 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      थांबा;

    स्थिती 0:
      error (निकास_त्रुटि, 0, _("missing conversion specifier in suffix"));

    पूर्वनिर्धारित:
      जर (है_छाप (ch))
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      अन्यथा
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  जर (flags & ~ compatible_flags)
    error (निकास_त्रुटि, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
स्थिर idx_t
max_out (अक्षर *format)
{
  bool percent = false;

  साठी (अक्षर *f = format; *f; f++)
    जर (*f == '%' && *++f != '%')
      {
        जर (percent)
          error (निकास_त्रुटि, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        पूर्णांक flags;
        f += get_format_flags (f, &flags);
        पर्यंत (c_isdigit (*f))
          f++;
        जर (*f == '.')
          पर्यंत (c_isdigit (*++f))
            सुरूठेवा;
        check_format_conv_type (f, flags);
      }

  जर (! percent)
    error (निकास_त्रुटि, 0,
           _("missing %% conversion specification in suffix"));

  पूर्णांक maxlen = snprintf (रिक्त, 0, format, पूर्णांक_उच्च);
  जर (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  परत maxlen;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  global_argv = argv;

  पर्यंत ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, रिक्त))
         != -1)
    निवड (optc)
      {
      स्थिती 'f':
        prefix = optarg;
        थांबा;

      स्थिती 'b':
        suffix = optarg;
        थांबा;

      स्थिती 'k':
        remove_files = false;
        थांबा;

      स्थिती 'n':
        digits = xdectoimax (optarg, 0, MIN (पूर्णांक_उच्च, IDX_MAX), "",
                             _("invalid number"), 0);
        थांबा;

      स्थिती 's':
      स्थिती 'q':
        suppress_मोजणी = true;
        थांबा;

      स्थिती 'z':
        elide_empty_files = true;
        थांबा;

      स्थिती SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        थांबा;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }

  जर (argc - optind < 2)
    {
      जर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  idx_t prefix_len = म_माप (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (पूर्णांक), digits));
  idx_t filename_size;
  जर (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_आदान_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    गणना { nsigs = countof (term_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_signals);
    साठी (पूर्णांक i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], रिक्त, &act);
        जर (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    साठी (पूर्णांक i = 0; i < nsigs; i++)
      जर (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, रिक्त);
  }

  split_file ();

  जर (बंद (STDIN_FILENO) != 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  परत निकास_सफल;
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... खाता PATTERN...\n\
"),
              program_name);
      ख_माला_दो (_("\
Output pieces of खाता separated by PATTERN(s) to files 'xx00', 'xx01', ...,\n\
and output byte counts of each piece to standard output.\n\
"), मानक_निकास);
       ख_माला_दो (_("\
\n\
Read standard आदान जर खाता is -\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use प्र_लिखो FORMAT instead of %02d\n\
"));
      oputs (_("\
  -f, --prefix=PREFIX\n\
         use PREFIX instead of 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         करा not हटाओ output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the ओळी matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified संख्या of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         करा not print counts of output file sizes\n\
"));
      oputs (_("\
  -z, --elide-empty-files\n\
         suppress empty output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Each PATTERN may be:\n\
  INTEGER            copy up to but not including specified ओळ संख्या\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching ओळ\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching ओळ\n\
  {INTEGER}          repeat the previous pattern specified संख्या of बार\n\
  {*}                repeat the previous pattern as many बार as possible\n\
\n\
A ओळ OFFSET is an integer optionally preceded by '+' or '-'\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}
