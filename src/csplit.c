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
  intmax_t पंक्तियाँ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  पूर्णाङ्क argnum;			/* ARGV index. */
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
  वर्ण *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
संरचना पंक्ति
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t जोड़ो_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  संरचना cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  संरचना पंक्ति *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
संरचना buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_पंक्ति;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t अंक_पंक्तियाँ;		/* Number of complete lines in this buffer. */
  वर्ण *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  संरचना पंक्ति *पंक्ति_start;	/* Head of list of pointers to lines. */
  संरचना पंक्ति *curr_पंक्ति;	/* The line start record currently in use. */
  संरचना buffer_record *next;
};

स्थिर शून्य close_output_file (शून्य);
स्थिर शून्य create_output_file (शून्य);
स्थिर शून्य delete_all_files (bool);
स्थिर शून्य save_पंक्ति_to_file (ध्रुव संरचना cstring *पंक्ति);

/* Start of buffer list. */
स्थिर संरचना buffer_record *head = रिक्त;

/* Partially read line. */
स्थिर वर्ण *hold_area = रिक्त;

/* Number of bytes in 'hold_area'. */
स्थिर idx_t hold_गिनती = 0;

/* Number of the last line in the buffers. */
स्थिर intmax_t last_पंक्ति_संख्या = 0;

/* Number of the line currently being examined. */
स्थिर intmax_t current_पंक्ति = 0;

/* If true, we have read EOF. */
स्थिर bool have_read_eof = false;

/* Name of output files. */
स्थिर वर्ण *चञ्चल filename_space = रिक्त;

/* Prefix part of output file names. */
स्थिर वर्ण ध्रुव *चञ्चल prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
स्थिर वर्ण *चञ्चल suffix = रिक्त;

/* Number of digits to use in output file names. */
स्थिर पूर्णाङ्क चञ्चल digits = 2;

/* Number of files created so far. */
स्थिर पूर्णाङ्क चञ्चल files_created = 0;

/* Number of bytes written to current file. */
स्थिर intmax_t bytes_written;

/* Output file pointer. */
स्थिर खाता *output_stream = रिक्त;

/* Output file name. */
स्थिर वर्ण *output_filename = रिक्त;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
स्थिर वर्ण **global_argv;

/* If true, do not print the count of bytes in each output file. */
स्थिर bool suppress_गिनती;

/* If true, remove output files on error. */
स्थिर bool चञ्चल remove_files = true;

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

स्थिर संरचना option ध्रुव longopts[] =
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
interrupt_handler (पूर्णाङ्क sig)
{
  delete_all_files (true);
  सङ्केत (sig, संक_पूर्व);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  उत्थापय (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

स्थिर शून्य
save_to_hold_area (वर्ण *start, idx_t अंक)
{
  मुक्त (hold_area);
  hold_area = start;
  hold_गिनती = अंक;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

स्थिर idx_t
read_निवेश (वर्ण *dest, idx_t max_n_bytes)
{
  यदि (max_n_bytes == 0)
    प्रत्यागम 0;

  सूचक_भेद_प्रकार bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  यदि (bytes_read == 0)
    have_read_eof = true;

  यदि (bytes_read < 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  प्रत्यागम bytes_read;
}

/* Initialize existing line record P. */

स्थिर शून्य
clear_पंक्ति_control (संरचना पंक्ति *p)
{
  p->used = 0;
  p->जोड़ो_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

स्थिर संरचना पंक्ति *
new_पंक्ति_control (शून्य)
{
  संरचना पंक्ति *p = xmalloc (परिमाण *p);

  p->next = रिक्त;
  clear_पंक्ति_control (p);

  प्रत्यागम p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

स्थिर शून्य
keep_new_पंक्ति (संरचना buffer_record *b, वर्ण *पंक्ति_start, idx_t पंक्ति_len)
{
  संरचना पंक्ति *l;

  /* If there is no existing area to keep line info, get some. */
  यदि (b->पंक्ति_start == रिक्त)
    b->पंक्ति_start = b->curr_पंक्ति = new_पंक्ति_control ();

  /* If existing area for lines is full, get more. */
  यदि (b->curr_पंक्ति->used == CTRL_SIZE)
    {
      b->curr_पंक्ति->next = new_पंक्ति_control ();
      b->curr_पंक्ति = b->curr_पंक्ति->next;
    }

  l = b->curr_पंक्ति;

  /* Record the start of the line, and update counters. */
  l->starts[l->जोड़ो_index].str = पंक्ति_start;
  l->starts[l->जोड़ो_index].len = पंक्ति_len;
  l->used++;
  l->जोड़ो_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

स्थिर idx_t
record_पंक्ति_starts (संरचना buffer_record *b)
{
  वर्ण *पंक्ति_start;		/* Start of current line. */
  idx_t पंक्तियाँ;			/* Number of lines found. */
  idx_t पंक्ति_length;		/* Length of each line found. */

  यदि (b->bytes_used == 0)
    प्रत्यागम 0;

  पंक्तियाँ = 0;
  पंक्ति_start = b->buffer;
  वर्ण *buffer_end = पंक्ति_start + b->bytes_used;
  *buffer_end = '\n';

  यावत् (true)
    {
      वर्ण *पंक्ति_end = rawmemchr (पंक्ति_start, '\n');
      यदि (पंक्ति_end == buffer_end)
        भङ्ग;
      पंक्ति_length = पंक्ति_end - पंक्ति_start + 1;
      keep_new_पंक्ति (b, पंक्ति_start, पंक्ति_length);
      पंक्ति_start = पंक्ति_end + 1;
      पंक्तियाँ++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_बायाँ = buffer_end - पंक्ति_start;
  यदि (bytes_बायाँ)
    {
      यदि (have_read_eof)
        {
          keep_new_पंक्ति (b, पंक्ति_start, bytes_बायाँ);
          पंक्तियाँ++;
        }
      अन्यथा
        save_to_hold_area (ximemdup (पंक्ति_start, bytes_बायाँ), bytes_बायाँ);
    }

  b->अंक_पंक्तियाँ = पंक्तियाँ;
  b->first_available = b->start_पंक्ति = last_पंक्ति_संख्या + 1;
  last_पंक्ति_संख्या += पंक्तियाँ;

  प्रत्यागम पंक्तियाँ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#यदि 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

स्थिर शून्य
free_buffer (संरचना buffer_record *buf)
{
  प्रति (संरचना पंक्ति *l = buf->पंक्ति_start; l;)
    {
      संरचना पंक्ति *n = l->next;
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
  संरचना buffer_record *new_buffer = xmalloc (परिमाण *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (रिक्त, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_पंक्ति = new_buffer->first_available = last_पंक्ति_संख्या + 1;
  new_buffer->अंक_पंक्तियाँ = 0;
  new_buffer->पंक्ति_start = new_buffer->curr_पंक्ति = रिक्त;
  new_buffer->next = रिक्त;

  प्रत्यागम new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

स्थिर शून्य
save_buffer (संरचना buffer_record *buf)
{
  buf->next = रिक्त;
  buf->curr_पंक्ति = buf->पंक्ति_start;

  यदि (head == रिक्त)
    head = buf;
  अन्यथा
    {
      संरचना buffer_record *p;
      प्रति (p = head; p->next; p = p->next)
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
  यदि (have_read_eof)
    प्रत्यागम false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_गिनती + 1);

  यावत् (true)
    {
      संरचना buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      वर्ण *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      यदि (hold_गिनती)
        {
          p = mempcpy (p, hold_area, hold_गिनती);
          b->bytes_used += hold_गिनती;
          bytes_avail -= hold_गिनती;
          hold_गिनती = 0;
        }

      b->bytes_used += read_निवेश (p, bytes_avail - 1);

      यदि (record_पंक्ति_starts (b) != 0)
        {
          save_buffer (b);
          प्रत्यागम true;
        }

      free_buffer (b);
      यदि (have_read_eof)
        प्रत्यागम false;
      यदि (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

स्थिर intmax_t
get_first_पंक्ति_in_buffer (शून्य)
{
  यदि (head == रिक्त && !load_buffer ())
    प्रत्यागम 0;

  प्रत्यागम head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

स्थिर संरचना cstring *
remove_पंक्ति (शून्य)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  स्थिर संरचना buffer_record *prev_buf = रिक्त;

  संरचना cstring *पंक्ति;		/* Return value. */
  संरचना पंक्ति *l;		/* For convenience. */

  यदि (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = रिक्त;
    }

  यदि (head == रिक्त && !load_buffer ())
    प्रत्यागम रिक्त;

  यदि (current_पंक्ति < head->first_available)
    current_पंक्ति = head->first_available;

  ++(head->first_available);

  l = head->curr_पंक्ति;

  पंक्ति = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  यदि (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_पंक्ति = l->next;
      यदि (head->curr_पंक्ति == रिक्त || head->curr_पंक्ति->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  प्रत्यागम पंक्ति;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

स्थिर संरचना cstring *
find_पंक्ति (intmax_t linenum)
{
  यदि (head == रिक्त && !load_buffer ())
    प्रत्यागम रिक्त;

  यदि (linenum < head->start_पंक्ति)
    प्रत्यागम रिक्त;

  प्रति (संरचना buffer_record *b = head;;)
    {
      यदि (linenum < b->start_पंक्ति + b->अंक_पंक्तियाँ)
        {
          /* The line is in this buffer. */
          संरचना पंक्ति *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->पंक्ति_start;
          offset = linenum - b->start_पंक्ति;
          /* Find the control record. */
          यावत् (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          प्रत्यागम &l->starts[offset];
        }
      यदि (b->next == रिक्त && !load_buffer ())
        प्रत्यागम रिक्त;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

स्थिर bool
no_more_पंक्तियाँ (शून्य)
{
  प्रत्यागम find_पंक्ति (current_पंक्ति + 1) == रिक्त;
}

/* Open NAME as standard input.  */

स्थिर शून्य
set_निवेश_file (वर्ण ध्रुव *name)
{
  यदि (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

स्थिर शून्य
write_to_file (intmax_t last_पंक्ति, bool ignore, पूर्णाङ्क argnum)
{
  संरचना cstring *पंक्ति;
  intmax_t first_पंक्ति;		/* First available input line. */
  intmax_t पंक्तियाँ;		/* Number of lines to output. */

  first_पंक्ति = get_first_पंक्ति_in_buffer ();

  यदि (! first_पंक्ति || first_पंक्ति > last_पंक्ति)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  पंक्तियाँ = last_पंक्ति - first_पंक्ति;

  प्रति (intmax_t i = 0; i < पंक्तियाँ; i++)
    {
      पंक्ति = remove_पंक्ति ();
      यदि (पंक्ति == रिक्त)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      यदि (!ignore)
        save_पंक्ति_to_file (पंक्ति);
    }
}

/* Output any lines left after all regexps have been processed. */

स्थिर शून्य
dump_rest_of_file (शून्य)
{
  संरचना cstring *पंक्ति;

  यावत् ((पंक्ति = remove_पंक्ति ()) != रिक्त)
    save_पंक्ति_to_file (पंक्ति);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

स्थिर शून्य
handle_पंक्ति_error (ध्रुव संरचना control *p, intmax_t repetition)
{
  वर्ण buf[INT_BUFSIZE_BOUND (intmax_t)];

  ख_लिखो (मानक_त्रुटि, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->पंक्तियाँ_required, buf)));
  यदि (repetition)
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
process_पंक्ति_गिनती (ध्रुव संरचना control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_पंक्ति_to_save = p->पंक्तियाँ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  यदि (no_more_पंक्तियाँ () && suppress_matched)
    handle_पंक्ति_error (p, repetition);

  यदि (!(linenum = get_first_पंक्ति_in_buffer ()))
    handle_पंक्ति_error (p, repetition);

  यावत् (linenum++ < last_पंक्ति_to_save)
    {
      संरचना cstring *पंक्ति = remove_पंक्ति ();
      यदि (पंक्ति == रिक्त)
        handle_पंक्ति_error (p, repetition);
      save_पंक्ति_to_file (पंक्ति);
    }

  close_output_file ();

  यदि (suppress_matched)
    remove_पंक्ति ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  यदि (no_more_पंक्तियाँ () && !suppress_matched)
    handle_पंक्ति_error (p, repetition);
}

स्थिर शून्य
regexp_error (संरचना control *p, intmax_t repetition, bool ignore)
{
  ख_लिखो (मानक_त्रुटि, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  यदि (repetition)
    ख_लिखो (मानक_त्रुटि, _(" on repetition %jd\n"), repetition);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "\n");

  यदि (!ignore)
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
  संरचना cstring *पंक्ति;		/* From input file. */
  idx_t पंक्ति_len;		/* To make "$" in regexps work. */
  intmax_t break_पंक्ति;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  यदि (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  यदि (p->offset >= 0)
    {
      यावत् (true)
        {
          पंक्ति = find_पंक्ति (++current_पंक्ति);
          यदि (पंक्ति == रिक्त)
            {
              यदि (p->repeat_forever)
                {
                  यदि (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  निकास (निकास_सफल);
                }
              अन्यथा
                regexp_error (p, repetition, ignore);
            }
          पंक्ति_len = पंक्ति->len;
          यदि (पंक्ति->str[पंक्ति_len - 1] == '\n')
            पंक्ति_len--;
          ret = re_खोज (&p->re_compiled, पंक्ति->str, पंक्ति_len,
                           0, पंक्ति_len, रिक्त);
          यदि (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          यदि (ret == -1)
            {
              पंक्ति = remove_पंक्ति ();
              यदि (!ignore)
                save_पंक्ति_to_file (पंक्ति);
            }
          अन्यथा
            भङ्ग;
        }
    }
  अन्यथा
    {
      /* Buffer the lines. */
      यावत् (true)
        {
          पंक्ति = find_पंक्ति (++current_पंक्ति);
          यदि (पंक्ति == रिक्त)
            {
              यदि (p->repeat_forever)
                {
                  यदि (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  निकास (निकास_सफल);
                }
              अन्यथा
                regexp_error (p, repetition, ignore);
            }
          पंक्ति_len = पंक्ति->len;
          यदि (पंक्ति->str[पंक्ति_len - 1] == '\n')
            पंक्ति_len--;
          ret = re_खोज (&p->re_compiled, पंक्ति->str, पंक्ति_len,
                           0, पंक्ति_len, रिक्त);
          यदि (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          यदि (ret != -1)
            भङ्ग;
        }
    }

  /* Account for any offset from this regexp. */
  break_पंक्ति = current_पंक्ति + p->offset;

  write_to_file (break_पंक्ति, ignore, p->argnum);

  यदि (!ignore)
    close_output_file ();

  यदि (p->offset > 0)
    current_पंक्ति = break_पंक्ति;

  यदि (suppress_matched)
    remove_पंक्ति ();
}

/* Split the input file according to the control records we have built. */

स्थिर शून्य
split_file (शून्य)
{
  प्रति (idx_t i = 0; i < control_used; i++)
    {
      यदि (controls[i].regexpr)
        {
          प्रति (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      अन्यथा
        {
          प्रति (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_पंक्ति_गिनती (&controls[i], j);
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

स्थिर वर्ण *
make_filename (पूर्णाङ्क अंक)
{
  म_नकल (filename_space, prefix);
  यदि (suffix)
    प्र_लिखो (filename_space + म_माप (prefix), suffix, अंक);
  अन्यथा
    प्र_लिखो (filename_space + म_माप (prefix), "%0*d", digits, अंक);
  प्रत्यागम filename_space;
}

/* Create the next output file. */

स्थिर शून्य
create_output_file (शून्य)
{
  पूर्णाङ्क nfiles = files_created;
  bool fopen_ok;
  पूर्णाङ्क fopen_errno;

  output_filename = make_filename (nfiles);

  यदि (nfiles == पूर्णांक_उच्च)
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

  यदि (! fopen_ok)
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
  यदि (! remove_files)
    प्रत्यागम;

  प्रति (पूर्णाङ्क i = files_created; 0 <= --i; )
    {
      वर्ण ध्रुव *name = make_filename (i);
      यदि (unlink (name) != 0 && त्रुटि_सं != ENOENT && !in_signal_handler)
        error (0, त्रुटि_सं, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

स्थिर शून्य
close_output_file (शून्य)
{
  यदि (output_stream)
    {
      यदि (ख_त्रुटि (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = रिक्त;
          cleanup_fatal ();
        }
      यदि (ख_बंद (output_stream) != 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (output_filename));
          output_stream = रिक्त;
          cleanup_fatal ();
        }
      यदि (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          पूर्णाङ्क unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = त्रुटि_सं;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, रिक्त);

          यदि (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      अन्यथा
        {
          यदि (!suppress_गिनती)
            ख_लिखो (मानक_निकास, "%jd\n", bytes_written);
        }
      output_stream = रिक्त;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

स्थिर शून्य
save_पंक्ति_to_file (ध्रुव संरचना cstring *पंक्ति)
{
  idx_t l = ख_डालो (पंक्ति->str, परिमाण (वर्ण), पंक्ति->len, output_stream);
  यदि (l != पंक्ति->len)
    {
      error (0, त्रुटि_सं, _("write error for %s"), quoteaf (output_filename));
      output_stream = रिक्त;
      cleanup_fatal ();
    }
  bytes_written += पंक्ति->len;
}

/* Return a new, initialized control record. */

स्थिर संरचना control *
new_control_record (शून्य)
{
  स्थिर idx_t control_allocated = 0; /* Total space allocated. */
  संरचना control *p;

  यदि (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, परिमाण *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->पंक्तियाँ_required = 0;
  p->offset = 0;
  प्रत्यागम p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

स्थिर शून्य
check_for_offset (संरचना control *p, वर्ण ध्रुव *str, वर्ण ध्रुव *अंक)
{
  यदि (xstrtoimax (अंक, रिक्त, 10, &p->offset, "") != LONGINT_OK)
    error (निकास_त्रुटि, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

स्थिर शून्य
parse_repeat_गिनती (पूर्णाङ्क argnum, संरचना control *p, वर्ण *str)
{
  वर्ण *end;

  end = str + म_माप (str) - 1;
  यदि (*end != '}')
    error (निकास_त्रुटि, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  यदि (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  अन्यथा
    {
      uintmax_t val;
      यदि (xstrtoumax (str + 1, रिक्त, 10, &val, "") != LONGINT_OK
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
extract_regexp (पूर्णाङ्क argnum, bool ignore, वर्ण ध्रुव *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  वर्ण delim = *str;
  वर्ण ध्रुव *closing_delim;
  संरचना control *p;
  वर्ण ध्रुव *err;

  closing_delim = म_खोजप (str + 1, delim);
  यदि (closing_delim == रिक्त)
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
  यदि (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  यदि (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  प्रत्यागम p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

स्थिर शून्य
parse_patterns (पूर्णाङ्क argc, पूर्णाङ्क start, वर्ण **argv)
{
  संरचना control *p;		/* New control record created. */
  स्थिर intmax_t last_val = 0;

  प्रति (पूर्णाङ्क i = start; i < argc; i++)
    {
      यदि (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      अन्यथा
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          यदि (xstrtoumax (argv[i], रिक्त, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (निकास_त्रुटि, 0, _("%s: invalid pattern"), quote (argv[i]));
          यदि (val == 0)
            error (निकास_त्रुटि, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          यदि (val < last_val)
            error (निकास_त्रुटि, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          यदि (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->पंक्तियाँ_required = val;
        }

      यदि (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_गिनती (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
गणना { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
स्थिर idx_t
get_format_flags (वर्ण ध्रुव *format, पूर्णाङ्क *flags_ptr)
{
  पूर्णाङ्क flags = 0;

  प्रति (idx_t गिनती = 0; ; गिनती++)
    {
      विकल्प (format[गिनती])
        {
        पक्ष '-':
        पक्ष '0':
          भङ्ग;

        पक्ष '\'':
          flags |= FLAG_THOUSANDS;
          भङ्ग;

        पक्ष '#':
          flags |= FLAG_ALTERNATIVE;
          भङ्ग;

        स्वभाव:
          *flags_ptr = flags;
          प्रत्यागम गिनती;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
स्थिर शून्य
check_format_conv_type (वर्ण *format, पूर्णाङ्क flags)
{
  अचिन्हित वर्ण ch = *format;
  पूर्णाङ्क compatible_flags = FLAG_THOUSANDS;

  विकल्प (ch)
    {
    पक्ष 'd':
    पक्ष 'i':
      भङ्ग;

    पक्ष 'u':
      *format = 'd';
      भङ्ग;

    पक्ष 'o':
    पक्ष 'x':
    पक्ष 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      भङ्ग;

    पक्ष 0:
      error (निकास_त्रुटि, 0, _("missing conversion specifier in suffix"));

    स्वभाव:
      यदि (है_छाप (ch))
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      अन्यथा
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  यदि (flags & ~ compatible_flags)
    error (निकास_त्रुटि, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
स्थिर idx_t
max_out (वर्ण *format)
{
  bool percent = false;

  प्रति (वर्ण *f = format; *f; f++)
    यदि (*f == '%' && *++f != '%')
      {
        यदि (percent)
          error (निकास_त्रुटि, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        पूर्णाङ्क flags;
        f += get_format_flags (f, &flags);
        यावत् (c_isdigit (*f))
          f++;
        यदि (*f == '.')
          यावत् (c_isdigit (*++f))
            अनुवर्त;
        check_format_conv_type (f, flags);
      }

  यदि (! percent)
    error (निकास_त्रुटि, 0,
           _("missing %% conversion specification in suffix"));

  पूर्णाङ्क maxlen = snprintf (रिक्त, 0, format, पूर्णांक_उच्च);
  यदि (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  प्रत्यागम maxlen;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  global_argv = argv;

  यावत् ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, रिक्त))
         != -1)
    विकल्प (optc)
      {
      पक्ष 'f':
        prefix = optarg;
        भङ्ग;

      पक्ष 'b':
        suffix = optarg;
        भङ्ग;

      पक्ष 'k':
        remove_files = false;
        भङ्ग;

      पक्ष 'n':
        digits = xdectoimax (optarg, 0, MIN (पूर्णांक_उच्च, IDX_MAX), "",
                             _("invalid number"), 0);
        भङ्ग;

      पक्ष 's':
      पक्ष 'q':
        suppress_गिनती = true;
        भङ्ग;

      पक्ष 'z':
        elide_empty_files = true;
        भङ्ग;

      पक्ष SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        भङ्ग;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      स्वभाव:
        usage (निकास_त्रुटि);
      }

  यदि (argc - optind < 2)
    {
      यदि (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (निकास_त्रुटि);
    }

  idx_t prefix_len = म_माप (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (पूर्णाङ्क), digits));
  idx_t filename_size;
  यदि (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_निवेश_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    गणना { nsigs = countof (term_sig) };

    संरचना sigaction act;

    sigemptyset (&caught_signals);
    प्रति (पूर्णाङ्क i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], रिक्त, &act);
        यदि (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    प्रति (पूर्णाङ्क i = 0; i < nsigs; i++)
      यदि (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, रिक्त);
  }

  split_file ();

  यदि (बंद (STDIN_FILENO) != 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  प्रत्यागम निकास_सफल;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
Read standard निवेश यदि खाता is -\n\
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
         कुरु not हटाओ output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the पंक्तियाँ matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified संख्या of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         कुरु not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified पंक्ति संख्या\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching पंक्ति\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching पंक्ति\n\
  {INTEGER}          repeat the previous pattern specified संख्या of बार\n\
  {*}                repeat the previous pattern as many बार as possible\n\
\n\
A पंक्ति OFFSET is an integer optionally preceded by '+' or '-'\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}
