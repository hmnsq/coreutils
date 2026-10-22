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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ <regex.h>

#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "fd-reopen.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "safe-read.h"
#അന്തര്ഭാവ "stdio--.h"
#അന്തര്ഭാവ "term-sig.h"
#അന്തര്ഭാവ "xdectoint.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "csplit"

#define AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#define DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
ഘടന control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t വരികൾ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  പൂർണ്ണസംഖ്യ argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ഘടന re_pattern_buffer re_compiled;	/* Compiled regular expression. */
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
ഘടന cstring
{
  idx_t len;
  അക്ഷരം *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ഘടന വരി
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t തിരുകുക_index;		/* Next offset to use when inserting line. */
  idx_t retrieve_index;	/* Next index to use when retrieving line. */
  ഘടന cstring starts[CTRL_SIZE]; /* Lines in the data area. */
  ഘടന വരി *next;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ഘടന buffer_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_വരി;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t അക്കം_വരികൾ;		/* Number of complete lines in this buffer. */
  അക്ഷരം *buffer COUNTED_BY (bytes_alloc);	/* Data area. */
  ഘടന വരി *വരി_start;	/* Head of list of pointers to lines. */
  ഘടന വരി *curr_വരി;	/* The line start record currently in use. */
  ഘടന buffer_record *next;
};

സ്ഥിതം ശൂന്യം close_output_file (ശൂന്യം);
സ്ഥിതം ശൂന്യം create_output_file (ശൂന്യം);
സ്ഥിതം ശൂന്യം delete_all_files (bool);
സ്ഥിതം ശൂന്യം save_വരി_to_file (സ്ഥിരം ഘടന cstring *വരി);

/* Start of buffer list. */
സ്ഥിതം ഘടന buffer_record *head = ശൂന്യ;

/* Partially read line. */
സ്ഥിതം അക്ഷരം *hold_area = ശൂന്യ;

/* Number of bytes in 'hold_area'. */
സ്ഥിതം idx_t hold_എണ്ണം = 0;

/* Number of the last line in the buffers. */
സ്ഥിതം intmax_t last_വരി_സംഖ്യ = 0;

/* Number of the line currently being examined. */
സ്ഥിതം intmax_t current_വരി = 0;

/* If true, we have read EOF. */
സ്ഥിതം bool have_read_eof = false;

/* Name of output files. */
സ്ഥിതം അക്ഷരം *അസ്ഥിര filename_space = ശൂന്യ;

/* Prefix part of output file names. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *അസ്ഥിര prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
സ്ഥിതം അക്ഷരം *അസ്ഥിര suffix = ശൂന്യ;

/* Number of digits to use in output file names. */
സ്ഥിതം പൂർണ്ണസംഖ്യ അസ്ഥിര digits = 2;

/* Number of files created so far. */
സ്ഥിതം പൂർണ്ണസംഖ്യ അസ്ഥിര files_created = 0;

/* Number of bytes written to current file. */
സ്ഥിതം intmax_t bytes_written;

/* Output file pointer. */
സ്ഥിതം FILE *output_stream = ശൂന്യ;

/* Output file name. */
സ്ഥിതം അക്ഷരം *output_filename = ശൂന്യ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
സ്ഥിതം അക്ഷരം **global_argv;

/* If true, do not print the count of bytes in each output file. */
സ്ഥിതം bool suppress_എണ്ണം;

/* If true, remove output files on error. */
സ്ഥിതം bool അസ്ഥിര remove_files = true;

/* If true, remove all output files which have a zero length. */
സ്ഥിതം bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
സ്ഥിതം bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
സ്ഥിതം ഘടന control *controls;

/* Number of elements in 'controls'. */
സ്ഥിതം idx_t control_used;

/* The set of signals that are caught.  */
സ്ഥിതം sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"digits", required_argument, ശൂന്യ, 'n'},
  {"quiet", no_argument, ശൂന്യ, 'q'},  /* Deprecated.  */
  {"silent", no_argument, ശൂന്യ, 's'},
  {"keep-files", no_argument, ശൂന്യ, 'k'},
  {"elide-empty-files", no_argument, ശൂന്യ, 'z'},
  {"prefix", required_argument, ശൂന്യ, 'f'},
  {"suffix-format", required_argument, ശൂന്യ, 'b'},
  {"suppress-matched", no_argument, ശൂന്യ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

സ്ഥിതം ശൂന്യം
cleanup (ശൂന്യം)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ശൂന്യ);
}

സ്ഥിതം _Noreturn ശൂന്യം
cleanup_fatal (ശൂന്യം)
{
  cleanup ();
  exit (EXIT_FAILURE);
}

ബാഹ്യ ശൂന്യം
xalloc_die (ശൂന്യം)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

സ്ഥിതം ശൂന്യം
interrupt_handler (പൂർണ്ണസംഖ്യ sig)
{
  delete_all_files (true);
  സൂചന (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ഉയർത്തുക (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

സ്ഥിതം ശൂന്യം
save_to_hold_area (അക്ഷരം *start, idx_t അക്കം)
{
  സ്വതന്ത്രം (hold_area);
  hold_area = start;
  hold_എണ്ണം = അക്കം;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

സ്ഥിതം idx_t
read_നിവേശം (അക്ഷരം *dest, idx_t max_n_bytes)
{
  എങ്കിൽ (max_n_bytes == 0)
    തിരികെ 0;

  ptrdiff_t bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  എങ്കിൽ (bytes_read == 0)
    have_read_eof = true;

  എങ്കിൽ (bytes_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  തിരികെ bytes_read;
}

/* Initialize existing line record P. */

സ്ഥിതം ശൂന്യം
clear_വരി_control (ഘടന വരി *p)
{
  p->used = 0;
  p->തിരുകുക_index = 0;
  p->retrieve_index = 0;
}

/* Return a new, initialized line record. */

സ്ഥിതം ഘടന വരി *
new_വരി_control (ശൂന്യം)
{
  ഘടന വരി *p = xmalloc (വലുപ്പം *p);

  p->next = ശൂന്യ;
  clear_വരി_control (p);

  തിരികെ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

സ്ഥിതം ശൂന്യം
keep_new_വരി (ഘടന buffer_record *b, അക്ഷരം *വരി_start, idx_t വരി_len)
{
  ഘടന വരി *l;

  /* If there is no existing area to keep line info, get some. */
  എങ്കിൽ (b->വരി_start == ശൂന്യ)
    b->വരി_start = b->curr_വരി = new_വരി_control ();

  /* If existing area for lines is full, get more. */
  എങ്കിൽ (b->curr_വരി->used == CTRL_SIZE)
    {
      b->curr_വരി->next = new_വരി_control ();
      b->curr_വരി = b->curr_വരി->next;
    }

  l = b->curr_വരി;

  /* Record the start of the line, and update counters. */
  l->starts[l->തിരുകുക_index].str = വരി_start;
  l->starts[l->തിരുകുക_index].len = വരി_len;
  l->used++;
  l->തിരുകുക_index++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

സ്ഥിതം idx_t
record_വരി_starts (ഘടന buffer_record *b)
{
  അക്ഷരം *വരി_start;		/* Start of current line. */
  idx_t വരികൾ;			/* Number of lines found. */
  idx_t വരി_length;		/* Length of each line found. */

  എങ്കിൽ (b->bytes_used == 0)
    തിരികെ 0;

  വരികൾ = 0;
  വരി_start = b->buffer;
  അക്ഷരം *buffer_end = വരി_start + b->bytes_used;
  *buffer_end = '\n';

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *വരി_end = rawmemchr (വരി_start, '\n');
      എങ്കിൽ (വരി_end == buffer_end)
        നിർത്തുക;
      വരി_length = വരി_end - വരി_start + 1;
      keep_new_വരി (b, വരി_start, വരി_length);
      വരി_start = വരി_end + 1;
      വരികൾ++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_ഇടത് = buffer_end - വരി_start;
  എങ്കിൽ (bytes_ഇടത്)
    {
      എങ്കിൽ (have_read_eof)
        {
          keep_new_വരി (b, വരി_start, bytes_ഇടത്);
          വരികൾ++;
        }
      അല്ലെങ്കിൽ
        save_to_hold_area (ximemdup (വരി_start, bytes_ഇടത്), bytes_ഇടത്);
    }

  b->അക്കം_വരികൾ = വരികൾ;
  b->first_available = b->start_വരി = last_വരി_സംഖ്യ + 1;
  last_വരി_സംഖ്യ += വരികൾ;

  തിരികെ വരികൾ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#എങ്കിൽ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

സ്ഥിതം ശൂന്യം
free_buffer (ഘടന buffer_record *buf)
{
  വേണ്ടി (ഘടന വരി *l = buf->വരി_start; l;)
    {
      ഘടന വരി *n = l->next;
      സ്വതന്ത്രം (l);
      l = n;
    }
  സ്വതന്ത്രം (buf->buffer);
  സ്വതന്ത്രം (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

സ്ഥിതം ATTRIBUTE_DEALLOC (free_buffer, 1)
ഘടന buffer_record *
get_new_buffer (idx_t min_size)
{
  ഘടന buffer_record *new_buffer = xmalloc (വലുപ്പം *new_buffer);
  new_buffer->bytes_alloc = 0;
  new_buffer->buffer = xpalloc (ശൂന്യ, &new_buffer->bytes_alloc, min_size,
                                -1, 1);
  new_buffer->bytes_used = 0;
  new_buffer->start_വരി = new_buffer->first_available = last_വരി_സംഖ്യ + 1;
  new_buffer->അക്കം_വരികൾ = 0;
  new_buffer->വരി_start = new_buffer->curr_വരി = ശൂന്യ;
  new_buffer->next = ശൂന്യ;

  തിരികെ new_buffer;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

സ്ഥിതം ശൂന്യം
save_buffer (ഘടന buffer_record *buf)
{
  buf->next = ശൂന്യ;
  buf->curr_വരി = buf->വരി_start;

  എങ്കിൽ (head == ശൂന്യ)
    head = buf;
  അല്ലെങ്കിൽ
    {
      ഘടന buffer_record *p;
      വേണ്ടി (p = head; p->next; p = p->next)
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

സ്ഥിതം bool
load_buffer (ശൂന്യം)
{
  എങ്കിൽ (have_read_eof)
    തിരികെ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_SIZE, hold_എണ്ണം + 1);

  ആകുമ്പോൾ (true)
    {
      ഘടന buffer_record *b = get_new_buffer (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      അക്ഷരം *p = b->buffer;

      /* First check the 'holding' area for a partial line. */
      എങ്കിൽ (hold_എണ്ണം)
        {
          p = mempcpy (p, hold_area, hold_എണ്ണം);
          b->bytes_used += hold_എണ്ണം;
          bytes_avail -= hold_എണ്ണം;
          hold_എണ്ണം = 0;
        }

      b->bytes_used += read_നിവേശം (p, bytes_avail - 1);

      എങ്കിൽ (record_വരി_starts (b) != 0)
        {
          save_buffer (b);
          തിരികെ true;
        }

      free_buffer (b);
      എങ്കിൽ (have_read_eof)
        തിരികെ false;
      എങ്കിൽ (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

സ്ഥിതം intmax_t
get_first_വരി_in_buffer (ശൂന്യം)
{
  എങ്കിൽ (head == ശൂന്യ && !load_buffer ())
    തിരികെ 0;

  തിരികെ head->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

സ്ഥിതം ഘടന cstring *
remove_വരി (ശൂന്യം)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  സ്ഥിതം ഘടന buffer_record *prev_buf = ശൂന്യ;

  ഘടന cstring *വരി;		/* Return value. */
  ഘടന വരി *l;		/* For convenience. */

  എങ്കിൽ (prev_buf)
    {
      free_buffer (prev_buf);
      prev_buf = ശൂന്യ;
    }

  എങ്കിൽ (head == ശൂന്യ && !load_buffer ())
    തിരികെ ശൂന്യ;

  എങ്കിൽ (current_വരി < head->first_available)
    current_വരി = head->first_available;

  ++(head->first_available);

  l = head->curr_വരി;

  വരി = &l->starts[l->retrieve_index];

  /* Advance index to next line. */
  എങ്കിൽ (++l->retrieve_index == l->used)
    {
      /* Go on to the next line record. */
      head->curr_വരി = l->next;
      എങ്കിൽ (head->curr_വരി == ശൂന്യ || head->curr_വരി->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = head;
          head = head->next;
        }
    }

  തിരികെ വരി;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

സ്ഥിതം ഘടന cstring *
find_വരി (intmax_t linenum)
{
  എങ്കിൽ (head == ശൂന്യ && !load_buffer ())
    തിരികെ ശൂന്യ;

  എങ്കിൽ (linenum < head->start_വരി)
    തിരികെ ശൂന്യ;

  വേണ്ടി (ഘടന buffer_record *b = head;;)
    {
      എങ്കിൽ (linenum < b->start_വരി + b->അക്കം_വരികൾ)
        {
          /* The line is in this buffer. */
          ഘടന വരി *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->വരി_start;
          offset = linenum - b->start_വരി;
          /* Find the control record. */
          ആകുമ്പോൾ (offset >= CTRL_SIZE)
            {
              l = l->next;
              offset -= CTRL_SIZE;
            }
          തിരികെ &l->starts[offset];
        }
      എങ്കിൽ (b->next == ശൂന്യ && !load_buffer ())
        തിരികെ ശൂന്യ;
      b = b->next;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

സ്ഥിതം bool
no_more_വരികൾ (ശൂന്യം)
{
  തിരികെ find_വരി (current_വരി + 1) == ശൂന്യ;
}

/* Open NAME as standard input.  */

സ്ഥിതം ശൂന്യം
set_നിവേശം_file (അക്ഷരം സ്ഥിരം *name)
{
  എങ്കിൽ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

സ്ഥിതം ശൂന്യം
write_to_file (intmax_t last_വരി, bool ignore, പൂർണ്ണസംഖ്യ argnum)
{
  ഘടന cstring *വരി;
  intmax_t first_വരി;		/* First available input line. */
  intmax_t വരികൾ;		/* Number of lines to output. */

  first_വരി = get_first_വരി_in_buffer ();

  എങ്കിൽ (! first_വരി || first_വരി > last_വരി)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  വരികൾ = last_വരി - first_വരി;

  വേണ്ടി (intmax_t i = 0; i < വരികൾ; i++)
    {
      വരി = remove_വരി ();
      എങ്കിൽ (വരി == ശൂന്യ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      എങ്കിൽ (!ignore)
        save_വരി_to_file (വരി);
    }
}

/* Output any lines left after all regexps have been processed. */

സ്ഥിതം ശൂന്യം
dump_rest_of_file (ശൂന്യം)
{
  ഘടന cstring *വരി;

  ആകുമ്പോൾ ((വരി = remove_വരി ()) != ശൂന്യ)
    save_വരി_to_file (വരി);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

സ്ഥിതം ശൂന്യം
handle_വരി_error (സ്ഥിരം ഘടന control *p, intmax_t repetition)
{
  അക്ഷരം buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->വരികൾ_required, buf)));
  എങ്കിൽ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  അല്ലെങ്കിൽ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

സ്ഥിതം ശൂന്യം
process_വരി_എണ്ണം (സ്ഥിരം ഘടന control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_വരി_to_save = p->വരികൾ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  എങ്കിൽ (no_more_വരികൾ () && suppress_matched)
    handle_വരി_error (p, repetition);

  എങ്കിൽ (!(linenum = get_first_വരി_in_buffer ()))
    handle_വരി_error (p, repetition);

  ആകുമ്പോൾ (linenum++ < last_വരി_to_save)
    {
      ഘടന cstring *വരി = remove_വരി ();
      എങ്കിൽ (വരി == ശൂന്യ)
        handle_വരി_error (p, repetition);
      save_വരി_to_file (വരി);
    }

  close_output_file ();

  എങ്കിൽ (suppress_matched)
    remove_വരി ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  എങ്കിൽ (no_more_വരികൾ () && !suppress_matched)
    handle_വരി_error (p, repetition);
}

സ്ഥിതം ശൂന്യം
regexp_error (ഘടന control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  എങ്കിൽ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  അല്ലെങ്കിൽ
    fprintf (stderr, "\n");

  എങ്കിൽ (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

സ്ഥിതം ശൂന്യം
process_regexp (ഘടന control *p, intmax_t repetition)
{
  ഘടന cstring *വരി;		/* From input file. */
  idx_t വരി_len;		/* To make "$" in regexps work. */
  intmax_t break_വരി;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  എങ്കിൽ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  എങ്കിൽ (p->offset >= 0)
    {
      ആകുമ്പോൾ (true)
        {
          വരി = find_വരി (++current_വരി);
          എങ്കിൽ (വരി == ശൂന്യ)
            {
              എങ്കിൽ (p->repeat_forever)
                {
                  എങ്കിൽ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              അല്ലെങ്കിൽ
                regexp_error (p, repetition, ignore);
            }
          വരി_len = വരി->len;
          എങ്കിൽ (വരി->str[വരി_len - 1] == '\n')
            വരി_len--;
          ret = re_തിരയൽ (&p->re_compiled, വരി->str, വരി_len,
                           0, വരി_len, ശൂന്യ);
          എങ്കിൽ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          എങ്കിൽ (ret == -1)
            {
              വരി = remove_വരി ();
              എങ്കിൽ (!ignore)
                save_വരി_to_file (വരി);
            }
          അല്ലെങ്കിൽ
            നിർത്തുക;
        }
    }
  അല്ലെങ്കിൽ
    {
      /* Buffer the lines. */
      ആകുമ്പോൾ (true)
        {
          വരി = find_വരി (++current_വരി);
          എങ്കിൽ (വരി == ശൂന്യ)
            {
              എങ്കിൽ (p->repeat_forever)
                {
                  എങ്കിൽ (!ignore)
                    {
                      dump_rest_of_file ();
                      close_output_file ();
                    }
                  exit (EXIT_SUCCESS);
                }
              അല്ലെങ്കിൽ
                regexp_error (p, repetition, ignore);
            }
          വരി_len = വരി->len;
          എങ്കിൽ (വരി->str[വരി_len - 1] == '\n')
            വരി_len--;
          ret = re_തിരയൽ (&p->re_compiled, വരി->str, വരി_len,
                           0, വരി_len, ശൂന്യ);
          എങ്കിൽ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          എങ്കിൽ (ret != -1)
            നിർത്തുക;
        }
    }

  /* Account for any offset from this regexp. */
  break_വരി = current_വരി + p->offset;

  write_to_file (break_വരി, ignore, p->argnum);

  എങ്കിൽ (!ignore)
    close_output_file ();

  എങ്കിൽ (p->offset > 0)
    current_വരി = break_വരി;

  എങ്കിൽ (suppress_matched)
    remove_വരി ();
}

/* Split the input file according to the control records we have built. */

സ്ഥിതം ശൂന്യം
split_file (ശൂന്യം)
{
  വേണ്ടി (idx_t i = 0; i < control_used; i++)
    {
      എങ്കിൽ (controls[i].regexpr)
        {
          വേണ്ടി (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      അല്ലെങ്കിൽ
        {
          വേണ്ടി (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_വരി_എണ്ണം (&controls[i], j);
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

സ്ഥിതം അക്ഷരം *
make_filename (പൂർണ്ണസംഖ്യ അക്കം)
{
  strcpy (filename_space, prefix);
  എങ്കിൽ (suffix)
    sprintf (filename_space + strlen (prefix), suffix, അക്കം);
  അല്ലെങ്കിൽ
    sprintf (filename_space + strlen (prefix), "%0*d", digits, അക്കം);
  തിരികെ filename_space;
}

/* Create the next output file. */

സ്ഥിതം ശൂന്യം
create_output_file (ശൂന്യം)
{
  പൂർണ്ണസംഖ്യ nfiles = files_created;
  bool fopen_ok;
  പൂർണ്ണസംഖ്യ fopen_errno;

  output_filename = make_filename (nfiles);

  എങ്കിൽ (nfiles == INT_MAX)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  അല്ലെങ്കിൽ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
      output_stream = fopen (output_filename, "w");
      fopen_ok = (output_stream != ശൂന്യ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ശൂന്യ);
    }

  എങ്കിൽ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

സ്ഥിതം ശൂന്യം
delete_all_files (bool in_signal_handler)
{
  എങ്കിൽ (! remove_files)
    തിരികെ;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = files_created; 0 <= --i; )
    {
      അക്ഷരം സ്ഥിരം *name = make_filename (i);
      എങ്കിൽ (unlink (name) != 0 && errno != ENOENT && !in_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

സ്ഥിതം ശൂന്യം
close_output_file (ശൂന്യം)
{
  എങ്കിൽ (output_stream)
    {
      എങ്കിൽ (ferror (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = ശൂന്യ;
          cleanup_fatal ();
        }
      എങ്കിൽ (fclose (output_stream) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_stream = ശൂന്യ;
          cleanup_fatal ();
        }
      എങ്കിൽ (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          പൂർണ്ണസംഖ്യ unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ശൂന്യ);

          എങ്കിൽ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (!suppress_എണ്ണം)
            fprintf (stdout, "%jd\n", bytes_written);
        }
      output_stream = ശൂന്യ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

സ്ഥിതം ശൂന്യം
save_വരി_to_file (സ്ഥിരം ഘടന cstring *വരി)
{
  idx_t l = fwrite (വരി->str, വലുപ്പം (അക്ഷരം), വരി->len, output_stream);
  എങ്കിൽ (l != വരി->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_stream = ശൂന്യ;
      cleanup_fatal ();
    }
  bytes_written += വരി->len;
}

/* Return a new, initialized control record. */

സ്ഥിതം ഘടന control *
new_control_record (ശൂന്യം)
{
  സ്ഥിതം idx_t control_allocated = 0; /* Total space allocated. */
  ഘടന control *p;

  എങ്കിൽ (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, വലുപ്പം *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->വരികൾ_required = 0;
  p->offset = 0;
  തിരികെ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

സ്ഥിതം ശൂന്യം
check_for_offset (ഘടന control *p, അക്ഷരം സ്ഥിരം *str, അക്ഷരം സ്ഥിരം *അക്കം)
{
  എങ്കിൽ (xstrtoimax (അക്കം, ശൂന്യ, 10, &p->offset, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

സ്ഥിതം ശൂന്യം
parse_repeat_എണ്ണം (പൂർണ്ണസംഖ്യ argnum, ഘടന control *p, അക്ഷരം *str)
{
  അക്ഷരം *end;

  end = str + strlen (str) - 1;
  എങ്കിൽ (*end != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  എങ്കിൽ (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  അല്ലെങ്കിൽ
    {
      uintmax_t val;
      എങ്കിൽ (xstrtoumax (str + 1, ശൂന്യ, 10, &val, "") != LONGINT_OK
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

സ്ഥിതം ഘടന control *
extract_regexp (പൂർണ്ണസംഖ്യ argnum, bool ignore, അക്ഷരം സ്ഥിരം *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  അക്ഷരം delim = *str;
  അക്ഷരം സ്ഥിരം *closing_delim;
  ഘടന control *p;
  അക്ഷരം സ്ഥിരം *err;

  closing_delim = strrchr (str + 1, delim);
  എങ്കിൽ (closing_delim == ശൂന്യ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.buffer = ശൂന്യ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ശൂന്യ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  എങ്കിൽ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  എങ്കിൽ (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  തിരികെ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

സ്ഥിതം ശൂന്യം
parse_patterns (പൂർണ്ണസംഖ്യ argc, പൂർണ്ണസംഖ്യ start, അക്ഷരം **argv)
{
  ഘടന control *p;		/* New control record created. */
  സ്ഥിതം intmax_t last_val = 0;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = start; i < argc; i++)
    {
      എങ്കിൽ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      അല്ലെങ്കിൽ
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          എങ്കിൽ (xstrtoumax (argv[i], ശൂന്യ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          എങ്കിൽ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          എങ്കിൽ (val < last_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          എങ്കിൽ (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->വരികൾ_required = val;
        }

      എങ്കിൽ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_എണ്ണം (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
എണ്ണൽ { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
സ്ഥിതം idx_t
get_format_flags (അക്ഷരം സ്ഥിരം *format, പൂർണ്ണസംഖ്യ *flags_ptr)
{
  പൂർണ്ണസംഖ്യ flags = 0;

  വേണ്ടി (idx_t എണ്ണം = 0; ; എണ്ണം++)
    {
      തിരഞ്ഞെടുപ്പ് (format[എണ്ണം])
        {
        അവസ്ഥ '-':
        അവസ്ഥ '0':
          നിർത്തുക;

        അവസ്ഥ '\'':
          flags |= FLAG_THOUSANDS;
          നിർത്തുക;

        അവസ്ഥ '#':
          flags |= FLAG_ALTERNATIVE;
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          *flags_ptr = flags;
          തിരികെ എണ്ണം;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
സ്ഥിതം ശൂന്യം
check_format_conv_type (അക്ഷരം *format, പൂർണ്ണസംഖ്യ flags)
{
  unsigned അക്ഷരം ch = *format;
  പൂർണ്ണസംഖ്യ compatible_flags = FLAG_THOUSANDS;

  തിരഞ്ഞെടുപ്പ് (ch)
    {
    അവസ്ഥ 'd':
    അവസ്ഥ 'i':
      നിർത്തുക;

    അവസ്ഥ 'u':
      *format = 'd';
      നിർത്തുക;

    അവസ്ഥ 'o':
    അവസ്ഥ 'x':
    അവസ്ഥ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      നിർത്തുക;

    അവസ്ഥ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    സ്ഥിരസ്ഥിതി:
      എങ്കിൽ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      അല്ലെങ്കിൽ
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  എങ്കിൽ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
സ്ഥിതം idx_t
max_out (അക്ഷരം *format)
{
  bool percent = false;

  വേണ്ടി (അക്ഷരം *f = format; *f; f++)
    എങ്കിൽ (*f == '%' && *++f != '%')
      {
        എങ്കിൽ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        പൂർണ്ണസംഖ്യ flags;
        f += get_format_flags (f, &flags);
        ആകുമ്പോൾ (c_isdigit (*f))
          f++;
        എങ്കിൽ (*f == '.')
          ആകുമ്പോൾ (c_isdigit (*++f))
            തുടരുക;
        check_format_conv_type (f, flags);
      }

  എങ്കിൽ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  പൂർണ്ണസംഖ്യ maxlen = snprintf (ശൂന്യ, 0, format, INT_MAX);
  എങ്കിൽ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  തിരികെ maxlen;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ശൂന്യ))
         != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      അവസ്ഥ 'f':
        prefix = optarg;
        നിർത്തുക;

      അവസ്ഥ 'b':
        suffix = optarg;
        നിർത്തുക;

      അവസ്ഥ 'k':
        remove_files = false;
        നിർത്തുക;

      അവസ്ഥ 'n':
        digits = xdectoimax (optarg, 0, MIN (INT_MAX, IDX_MAX), "",
                             _("invalid number"), 0);
        നിർത്തുക;

      അവസ്ഥ 's':
      അവസ്ഥ 'q':
        suppress_എണ്ണം = true;
        നിർത്തുക;

      അവസ്ഥ 'z':
        elide_empty_files = true;
        നിർത്തുക;

      അവസ്ഥ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }

  എങ്കിൽ (argc - optind < 2)
    {
      എങ്കിൽ (argc <= optind)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t prefix_len = strlen (prefix);
  idx_t max_digit_string_len
    = (suffix
       ? max_out (suffix)
       : MAX (INT_STRLEN_BOUND (പൂർണ്ണസംഖ്യ), digits));
  idx_t filename_size;
  എങ്കിൽ (ckd_add (&filename_size, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_size);

  set_നിവേശം_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    എണ്ണൽ { nsigs = countof (term_sig) };

    ഘടന sigaction act;

    sigemptyset (&caught_signals);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], ശൂന്യ, &act);
        എങ്കിൽ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < nsigs; i++)
      എങ്കിൽ (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, ശൂന്യ);
  }

  split_file ();

  എങ്കിൽ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  തിരികെ EXIT_SUCCESS;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
Read standard നിവേശം എങ്കിൽ FILE is -\n\
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
         ചെയ്യുക not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the വരികൾ matching PATTERN\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified സംഖ്യ of digits instead of 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         ചെയ്യുക not print counts of output file sizes\n\
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
  INTEGER            copy up to but not including specified വരി സംഖ്യ\n\
  /REGEXP/[OFFSET]   copy up to but not including a matching വരി\n\
  %REGEXP%[OFFSET]   skip to, but not including a matching വരി\n\
  {INTEGER}          repeat the previous pattern specified സംഖ്യ of times\n\
  {*}                repeat the previous pattern as many times as possible\n\
\n\
A വരി OFFSET is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}
