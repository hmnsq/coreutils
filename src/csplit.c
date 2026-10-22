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

#समावेश <config.h>

#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश <signal.h>

#समावेश "system.h"

#समावेश <regex.h>

#समावेश "c-ctype.h"
#समावेश "fd-reopen.h"
#समावेश "quote.h"
#समावेश "safe-read.h"
#समावेश "stdio--.h"
#समावेश "term-sig.h"
#समावेश "xdectoint.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "csplit"

#घोषणा AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#घोषणा DEFAULT_PREFIX	"xx"

/* A compiled pattern arg. */
काष्ठा control
{
  intmax_t offset;		/* Offset from regexp to split at. */
  intmax_t पंक्तियाँ_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  पूर्णांक argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  काष्ठा re_pattern_बफर re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#घोषणा START_आकार	8191

/* Number of lines kept in each node in line list. */
#घोषणा CTRL_आकार	80

#अगर_घोषित DEBUG
/* Some small values to test the algorithms. */
# define START_आकार	200
# define CTRL_आकार	1
#पूर्ण_अगर

/* A string with a length count. */
काष्ठा cstring
{
  idx_t len;
  अक्षर *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
काष्ठा पंक्ति
{
  idx_t used;			/* Number of offsets used in this struct. */
  idx_t जोड़ो_सूचकांक;		/* Next offset to use when inserting line. */
  idx_t retrieve_सूचकांक;	/* Next index to use when retrieving line. */
  काष्ठा cstring starts[CTRL_आकार]; /* Lines in the data area. */
  काष्ठा पंक्ति *अग्र;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
काष्ठा बफर_record
{
  idx_t bytes_alloc;		/* Size of the buffer area. */
  idx_t bytes_used;		/* Bytes used in the buffer area. */
  intmax_t start_पंक्ति;		/* First line number in this buffer. */
  intmax_t first_available;	/* First line that can be retrieved. */
  idx_t अंक_पंक्तियाँ;		/* Number of complete lines in this buffer. */
  अक्षर *बफर COUNTED_BY (bytes_alloc);	/* Data area. */
  काष्ठा पंक्ति *पंक्ति_start;	/* Head of list of pointers to lines. */
  काष्ठा पंक्ति *curr_पंक्ति;	/* The line start record currently in use. */
  काष्ठा बफर_record *अग्र;
};

अटल व्योम close_output_file (व्योम);
अटल व्योम create_output_file (व्योम);
अटल व्योम delete_all_files (bool);
अटल व्योम save_पंक्ति_to_file (स्थिर काष्ठा cstring *पंक्ति);

/* Start of buffer list. */
अटल काष्ठा बफर_record *शीर्ष = शून्य;

/* Partially read line. */
अटल अक्षर *hold_area = शून्य;

/* Number of bytes in 'hold_area'. */
अटल idx_t hold_गिनती = 0;

/* Number of the last line in the buffers. */
अटल intmax_t last_पंक्ति_संख्या = 0;

/* Number of the line currently being examined. */
अटल intmax_t current_पंक्ति = 0;

/* If true, we have read EOF. */
अटल bool have_read_eof = false;

/* Name of output files. */
अटल अक्षर *अस्थिर filename_space = शून्य;

/* Prefix part of output file names. */
अटल अक्षर स्थिर *अस्थिर prefix = DEFAULT_PREFIX;

/* Suffix part of output file names. */
अटल अक्षर *अस्थिर suffix = शून्य;

/* Number of digits to use in output file names. */
अटल पूर्णांक अस्थिर digits = 2;

/* Number of files created so far. */
अटल पूर्णांक अस्थिर files_created = 0;

/* Number of bytes written to current file. */
अटल intmax_t bytes_written;

/* Output file pointer. */
अटल खाता *output_stream = शून्य;

/* Output file name. */
अटल अक्षर *output_filename = शून्य;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
अटल अक्षर **global_argv;

/* If true, do not print the count of bytes in each output file. */
अटल bool suppress_गिनती;

/* If true, remove output files on error. */
अटल bool अस्थिर remove_files = true;

/* If true, remove all output files which have a zero length. */
अटल bool elide_empty_files;

/* If true, suppress the lines that match the PATTERN */
अटल bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
अटल काष्ठा control *controls;

/* Number of elements in 'controls'. */
अटल idx_t control_used;

/* The set of signals that are caught.  */
अटल sigset_t caught_signals;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  SUPPRESS_MATCHED_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"digits", required_argument, शून्य, 'n'},
  {"quiet", no_argument, शून्य, 'q'},  /* Deprecated.  */
  {"silent", no_argument, शून्य, 's'},
  {"keep-files", no_argument, शून्य, 'k'},
  {"elide-empty-files", no_argument, शून्य, 'z'},
  {"prefix", required_argument, शून्य, 'f'},
  {"suffix-format", required_argument, शून्य, 'b'},
  {"suppress-matched", no_argument, शून्य, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

अटल व्योम
cleanup (व्योम)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, शून्य);
}

अटल _Noreturn व्योम
cleanup_fatal (व्योम)
{
  cleanup ();
  निकास (निकास_त्रुटि);
}

बाह्य व्योम
xalloc_die (व्योम)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

अटल व्योम
interrupt_handler (पूर्णांक sig)
{
  delete_all_files (true);
  संकेत (sig, संक_पूर्व);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  उठाओ (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

अटल व्योम
save_to_hold_area (अक्षर *start, idx_t अंक)
{
  मुक्त (hold_area);
  hold_area = start;
  hold_गिनती = अंक;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

अटल idx_t
read_निवेश (अक्षर *dest, idx_t max_n_bytes)
{
  अगर (max_n_bytes == 0)
    वापस 0;

  सूचक_भेद_प्रकार bytes_read = safe_read (STDIN_FILENO, dest, max_n_bytes);

  अगर (bytes_read == 0)
    have_read_eof = true;

  अगर (bytes_read < 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  वापस bytes_read;
}

/* Initialize existing line record P. */

अटल व्योम
clear_पंक्ति_control (काष्ठा पंक्ति *p)
{
  p->used = 0;
  p->जोड़ो_सूचकांक = 0;
  p->retrieve_सूचकांक = 0;
}

/* Return a new, initialized line record. */

अटल काष्ठा पंक्ति *
new_पंक्ति_control (व्योम)
{
  काष्ठा पंक्ति *p = xmalloc (माप *p);

  p->अग्र = शून्य;
  clear_पंक्ति_control (p);

  वापस p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

अटल व्योम
keep_new_पंक्ति (काष्ठा बफर_record *b, अक्षर *पंक्ति_start, idx_t पंक्ति_len)
{
  काष्ठा पंक्ति *l;

  /* If there is no existing area to keep line info, get some. */
  अगर (b->पंक्ति_start == शून्य)
    b->पंक्ति_start = b->curr_पंक्ति = new_पंक्ति_control ();

  /* If existing area for lines is full, get more. */
  अगर (b->curr_पंक्ति->used == CTRL_आकार)
    {
      b->curr_पंक्ति->अग्र = new_पंक्ति_control ();
      b->curr_पंक्ति = b->curr_पंक्ति->अग्र;
    }

  l = b->curr_पंक्ति;

  /* Record the start of the line, and update counters. */
  l->starts[l->जोड़ो_सूचकांक].str = पंक्ति_start;
  l->starts[l->जोड़ो_सूचकांक].len = पंक्ति_len;
  l->used++;
  l->जोड़ो_सूचकांक++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

अटल idx_t
record_पंक्ति_starts (काष्ठा बफर_record *b)
{
  अक्षर *पंक्ति_start;		/* Start of current line. */
  idx_t पंक्तियाँ;			/* Number of lines found. */
  idx_t पंक्ति_लंबाई;		/* Length of each line found. */

  अगर (b->bytes_used == 0)
    वापस 0;

  पंक्तियाँ = 0;
  पंक्ति_start = b->बफर;
  अक्षर *बफर_end = पंक्ति_start + b->bytes_used;
  *बफर_end = '\n';

  जबतक (true)
    {
      अक्षर *पंक्ति_end = rawmemchr (पंक्ति_start, '\n');
      अगर (पंक्ति_end == बफर_end)
        अवरोध;
      पंक्ति_लंबाई = पंक्ति_end - पंक्ति_start + 1;
      keep_new_पंक्ति (b, पंक्ति_start, पंक्ति_लंबाई);
      पंक्ति_start = पंक्ति_end + 1;
      पंक्तियाँ++;
    }

  /* Check for an incomplete last line. */
  idx_t bytes_बायाँ = बफर_end - पंक्ति_start;
  अगर (bytes_बायाँ)
    {
      अगर (have_read_eof)
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

  वापस पंक्तियाँ;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#अगर 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#पूर्ण_अगर

अटल व्योम
free_बफर (काष्ठा बफर_record *buf)
{
  क्रम (काष्ठा पंक्ति *l = buf->पंक्ति_start; l;)
    {
      काष्ठा पंक्ति *n = l->अग्र;
      मुक्त (l);
      l = n;
    }
  मुक्त (buf->बफर);
  मुक्त (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

अटल ATTRIBUTE_DEALLOC (free_बफर, 1)
काष्ठा बफर_record *
get_new_बफर (idx_t min_आकार)
{
  काष्ठा बफर_record *new_बफर = xmalloc (माप *new_बफर);
  new_बफर->bytes_alloc = 0;
  new_बफर->बफर = xpalloc (शून्य, &new_बफर->bytes_alloc, min_आकार,
                                -1, 1);
  new_बफर->bytes_used = 0;
  new_बफर->start_पंक्ति = new_बफर->first_available = last_पंक्ति_संख्या + 1;
  new_बफर->अंक_पंक्तियाँ = 0;
  new_बफर->पंक्ति_start = new_बफर->curr_पंक्ति = शून्य;
  new_बफर->अग्र = शून्य;

  वापस new_बफर;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

अटल व्योम
save_बफर (काष्ठा बफर_record *buf)
{
  buf->अग्र = शून्य;
  buf->curr_पंक्ति = buf->पंक्ति_start;

  अगर (शीर्ष == शून्य)
    शीर्ष = buf;
  अन्यथा
    {
      काष्ठा बफर_record *p;
      क्रम (p = शीर्ष; p->अग्र; p = p->अग्र)
        /* Do nothing. */ ;
      p->अग्र = buf;
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

अटल bool
load_बफर (व्योम)
{
  अगर (have_read_eof)
    वापस false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t bytes_wanted = MAX (START_आकार, hold_गिनती + 1);

  जबतक (true)
    {
      काष्ठा बफर_record *b = get_new_बफर (bytes_wanted);
      idx_t bytes_alloc = b->bytes_alloc;
      idx_t bytes_avail = bytes_alloc;
      अक्षर *p = b->बफर;

      /* First check the 'holding' area for a partial line. */
      अगर (hold_गिनती)
        {
          p = mempcpy (p, hold_area, hold_गिनती);
          b->bytes_used += hold_गिनती;
          bytes_avail -= hold_गिनती;
          hold_गिनती = 0;
        }

      b->bytes_used += read_निवेश (p, bytes_avail - 1);

      अगर (record_पंक्ति_starts (b) != 0)
        {
          save_बफर (b);
          वापस true;
        }

      free_बफर (b);
      अगर (have_read_eof)
        वापस false;
      अगर (ckd_add (&bytes_wanted, bytes_alloc, bytes_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

अटल intmax_t
get_first_पंक्ति_in_बफर (व्योम)
{
  अगर (शीर्ष == शून्य && !load_बफर ())
    वापस 0;

  वापस शीर्ष->first_available;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

अटल काष्ठा cstring *
remove_पंक्ति (व्योम)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  अटल काष्ठा बफर_record *पूर्व_buf = शून्य;

  काष्ठा cstring *पंक्ति;		/* Return value. */
  काष्ठा पंक्ति *l;		/* For convenience. */

  अगर (पूर्व_buf)
    {
      free_बफर (पूर्व_buf);
      पूर्व_buf = शून्य;
    }

  अगर (शीर्ष == शून्य && !load_बफर ())
    वापस शून्य;

  अगर (current_पंक्ति < शीर्ष->first_available)
    current_पंक्ति = शीर्ष->first_available;

  ++(शीर्ष->first_available);

  l = शीर्ष->curr_पंक्ति;

  पंक्ति = &l->starts[l->retrieve_सूचकांक];

  /* Advance index to next line. */
  अगर (++l->retrieve_सूचकांक == l->used)
    {
      /* Go on to the next line record. */
      शीर्ष->curr_पंक्ति = l->अग्र;
      अगर (शीर्ष->curr_पंक्ति == शून्य || शीर्ष->curr_पंक्ति->used == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          पूर्व_buf = शीर्ष;
          शीर्ष = शीर्ष->अग्र;
        }
    }

  वापस पंक्ति;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

अटल काष्ठा cstring *
find_पंक्ति (intmax_t linenum)
{
  अगर (शीर्ष == शून्य && !load_बफर ())
    वापस शून्य;

  अगर (linenum < शीर्ष->start_पंक्ति)
    वापस शून्य;

  क्रम (काष्ठा बफर_record *b = शीर्ष;;)
    {
      अगर (linenum < b->start_पंक्ति + b->अंक_पंक्तियाँ)
        {
          /* The line is in this buffer. */
          काष्ठा पंक्ति *l;
          idx_t offset;	/* How far into the buffer the line is. */

          l = b->पंक्ति_start;
          offset = linenum - b->start_पंक्ति;
          /* Find the control record. */
          जबतक (offset >= CTRL_आकार)
            {
              l = l->अग्र;
              offset -= CTRL_आकार;
            }
          वापस &l->starts[offset];
        }
      अगर (b->अग्र == शून्य && !load_बफर ())
        वापस शून्य;
      b = b->अग्र;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

अटल bool
no_more_पंक्तियाँ (व्योम)
{
  वापस find_पंक्ति (current_पंक्ति + 1) == शून्य;
}

/* Open NAME as standard input.  */

अटल व्योम
set_निवेश_file (अक्षर स्थिर *name)
{
  अगर (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

अटल व्योम
write_to_file (intmax_t last_पंक्ति, bool ignore, पूर्णांक argnum)
{
  काष्ठा cstring *पंक्ति;
  intmax_t first_पंक्ति;		/* First available input line. */
  intmax_t पंक्तियाँ;		/* Number of lines to output. */

  first_पंक्ति = get_first_पंक्ति_in_बफर ();

  अगर (! first_पंक्ति || first_पंक्ति > last_पंक्ति)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  पंक्तियाँ = last_पंक्ति - first_पंक्ति;

  क्रम (intmax_t i = 0; i < पंक्तियाँ; i++)
    {
      पंक्ति = remove_पंक्ति ();
      अगर (पंक्ति == शून्य)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      अगर (!ignore)
        save_पंक्ति_to_file (पंक्ति);
    }
}

/* Output any lines left after all regexps have been processed. */

अटल व्योम
dump_rest_of_file (व्योम)
{
  काष्ठा cstring *पंक्ति;

  जबतक ((पंक्ति = remove_पंक्ति ()) != शून्य)
    save_पंक्ति_to_file (पंक्ति);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

अटल व्योम
handle_पंक्ति_error (स्थिर काष्ठा control *p, intmax_t repetition)
{
  अक्षर buf[INT_BUFSIZE_BOUND (intmax_t)];

  ख_लिखो (मानक_त्रुटि, _("%s: %s: line number out of range"),
           program_name, quote (imaxtostr (p->पंक्तियाँ_required, buf)));
  अगर (repetition)
    ख_लिखो (मानक_त्रुटि, _(" on repetition %jd\n"), repetition);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

अटल व्योम
process_पंक्ति_गिनती (स्थिर काष्ठा control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t last_पंक्ति_to_save = p->पंक्तियाँ_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  अगर (no_more_पंक्तियाँ () && suppress_matched)
    handle_पंक्ति_error (p, repetition);

  अगर (!(linenum = get_first_पंक्ति_in_बफर ()))
    handle_पंक्ति_error (p, repetition);

  जबतक (linenum++ < last_पंक्ति_to_save)
    {
      काष्ठा cstring *पंक्ति = remove_पंक्ति ();
      अगर (पंक्ति == शून्य)
        handle_पंक्ति_error (p, repetition);
      save_पंक्ति_to_file (पंक्ति);
    }

  close_output_file ();

  अगर (suppress_matched)
    remove_पंक्ति ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  अगर (no_more_पंक्तियाँ () && !suppress_matched)
    handle_पंक्ति_error (p, repetition);
}

अटल व्योम
regexp_error (काष्ठा control *p, intmax_t repetition, bool ignore)
{
  ख_लिखो (मानक_त्रुटि, _("%s: %s: match not found"),
           program_name, quote (global_argv[p->argnum]));

  अगर (repetition)
    ख_लिखो (मानक_त्रुटि, _(" on repetition %jd\n"), repetition);
  अन्यथा
    ख_लिखो (मानक_त्रुटि, "\n");

  अगर (!ignore)
    {
      dump_rest_of_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

अटल व्योम
process_regexp (काष्ठा control *p, intmax_t repetition)
{
  काष्ठा cstring *पंक्ति;		/* From input file. */
  idx_t पंक्ति_len;		/* To make "$" in regexps work. */
  intmax_t break_पंक्ति;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  अगर (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  अगर (p->offset >= 0)
    {
      जबतक (true)
        {
          पंक्ति = find_पंक्ति (++current_पंक्ति);
          अगर (पंक्ति == शून्य)
            {
              अगर (p->repeat_forever)
                {
                  अगर (!ignore)
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
          अगर (पंक्ति->str[पंक्ति_len - 1] == '\n')
            पंक्ति_len--;
          ret = re_खोज (&p->re_compiled, पंक्ति->str, पंक्ति_len,
                           0, पंक्ति_len, शून्य);
          अगर (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          अगर (ret == -1)
            {
              पंक्ति = remove_पंक्ति ();
              अगर (!ignore)
                save_पंक्ति_to_file (पंक्ति);
            }
          अन्यथा
            अवरोध;
        }
    }
  अन्यथा
    {
      /* Buffer the lines. */
      जबतक (true)
        {
          पंक्ति = find_पंक्ति (++current_पंक्ति);
          अगर (पंक्ति == शून्य)
            {
              अगर (p->repeat_forever)
                {
                  अगर (!ignore)
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
          अगर (पंक्ति->str[पंक्ति_len - 1] == '\n')
            पंक्ति_len--;
          ret = re_खोज (&p->re_compiled, पंक्ति->str, पंक्ति_len,
                           0, पंक्ति_len, शून्य);
          अगर (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          अगर (ret != -1)
            अवरोध;
        }
    }

  /* Account for any offset from this regexp. */
  break_पंक्ति = current_पंक्ति + p->offset;

  write_to_file (break_पंक्ति, ignore, p->argnum);

  अगर (!ignore)
    close_output_file ();

  अगर (p->offset > 0)
    current_पंक्ति = break_पंक्ति;

  अगर (suppress_matched)
    remove_पंक्ति ();
}

/* Split the input file according to the control records we have built. */

अटल व्योम
split_file (व्योम)
{
  क्रम (idx_t i = 0; i < control_used; i++)
    {
      अगर (controls[i].regexpr)
        {
          क्रम (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      अन्यथा
        {
          क्रम (intmax_t j = 0; (controls[i].repeat_forever
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

अटल अक्षर *
make_filename (पूर्णांक अंक)
{
  म_नकल (filename_space, prefix);
  अगर (suffix)
    प्र_लिखो (filename_space + म_माप (prefix), suffix, अंक);
  अन्यथा
    प्र_लिखो (filename_space + म_माप (prefix), "%0*d", digits, अंक);
  वापस filename_space;
}

/* Create the next output file. */

अटल व्योम
create_output_file (व्योम)
{
  पूर्णांक nfiles = files_created;
  bool fopen_ok;
  पूर्णांक fopen_errno;

  output_filename = make_filename (nfiles);

  अगर (nfiles == पूर्णांक_उच्च)
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
      fopen_ok = (output_stream != शून्य);
      fopen_errno = त्रुटि_सं;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, शून्य);
    }

  अगर (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  bytes_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

अटल व्योम
delete_all_files (bool in_signal_handler)
{
  अगर (! remove_files)
    वापस;

  क्रम (पूर्णांक i = files_created; 0 <= --i; )
    {
      अक्षर स्थिर *name = make_filename (i);
      अगर (unlink (name) != 0 && त्रुटि_सं != ENOENT && !in_signal_handler)
        error (0, त्रुटि_सं, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

अटल व्योम
close_output_file (व्योम)
{
  अगर (output_stream)
    {
      अगर (ख_त्रुटि (output_stream))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_stream = शून्य;
          cleanup_fatal ();
        }
      अगर (ख_बंद (output_stream) != 0)
        {
          error (0, त्रुटि_सं, "%s", quotef (output_filename));
          output_stream = शून्य;
          cleanup_fatal ();
        }
      अगर (bytes_written == 0 && elide_empty_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          पूर्णांक unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_signals, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = त्रुटि_सं;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, शून्य);

          अगर (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      अन्यथा
        {
          अगर (!suppress_गिनती)
            ख_लिखो (मानक_निकास, "%jd\n", bytes_written);
        }
      output_stream = शून्य;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

अटल व्योम
save_पंक्ति_to_file (स्थिर काष्ठा cstring *पंक्ति)
{
  idx_t l = ख_डालो (पंक्ति->str, माप (अक्षर), पंक्ति->len, output_stream);
  अगर (l != पंक्ति->len)
    {
      error (0, त्रुटि_सं, _("write error for %s"), quoteaf (output_filename));
      output_stream = शून्य;
      cleanup_fatal ();
    }
  bytes_written += पंक्ति->len;
}

/* Return a new, initialized control record. */

अटल काष्ठा control *
new_control_record (व्योम)
{
  अटल idx_t control_allocated = 0; /* Total space allocated. */
  काष्ठा control *p;

  अगर (control_used == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, माप *controls);
  p = &controls[control_used++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->पंक्तियाँ_required = 0;
  p->offset = 0;
  वापस p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

अटल व्योम
check_for_offset (काष्ठा control *p, अक्षर स्थिर *str, अक्षर स्थिर *अंक)
{
  अगर (xstrtoimax (अंक, शून्य, 10, &p->offset, "") != LONGINT_OK)
    error (निकास_त्रुटि, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

अटल व्योम
parse_repeat_गिनती (पूर्णांक argnum, काष्ठा control *p, अक्षर *str)
{
  अक्षर *end;

  end = str + म_माप (str) - 1;
  अगर (*end != '}')
    error (निकास_त्रुटि, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *end = '\0';

  अगर (str + 1 == end - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  अन्यथा
    {
      uintmax_t val;
      अगर (xstrtoumax (str + 1, शून्य, 10, &val, "") != LONGINT_OK
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

अटल काष्ठा control *
extract_regexp (पूर्णांक argnum, bool ignore, अक्षर स्थिर *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  अक्षर delim = *str;
  अक्षर स्थिर *closing_delim;
  काष्ठा control *p;
  अक्षर स्थिर *err;

  closing_delim = म_खोजप (str + 1, delim);
  अगर (closing_delim == शून्य)
    error (निकास_त्रुटि, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_record ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.बफर = शून्य;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (अच_अक्षर_उच्च + 1);
  p->re_compiled.translate = शून्य;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_EMPTY_RANGES;
  err = re_compile_pattern (str + 1, len, &p->re_compiled);
  अगर (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  अगर (closing_delim[1])
    check_for_offset (p, str, closing_delim + 1);

  वापस p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

अटल व्योम
parse_patterns (पूर्णांक argc, पूर्णांक start, अक्षर **argv)
{
  काष्ठा control *p;		/* New control record created. */
  अटल intmax_t last_val = 0;

  क्रम (पूर्णांक i = start; i < argc; i++)
    {
      अगर (*argv[i] == '/' || *argv[i] == '%')
        {
          p = extract_regexp (i, *argv[i] == '%', argv[i]);
        }
      अन्यथा
        {
          p = new_control_record ();
          p->argnum = i;

          uintmax_t val;
          अगर (xstrtoumax (argv[i], शून्य, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (निकास_त्रुटि, 0, _("%s: invalid pattern"), quote (argv[i]));
          अगर (val == 0)
            error (निकास_त्रुटि, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          अगर (val < last_val)
            error (निकास_त्रुटि, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), last_val);

          अगर (val == last_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          last_val = val;

          p->पंक्तियाँ_required = val;
        }

      अगर (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_गिनती (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
क्रमागत { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
अटल idx_t
get_format_flags (अक्षर स्थिर *format, पूर्णांक *flags_ptr)
{
  पूर्णांक flags = 0;

  क्रम (idx_t गिनती = 0; ; गिनती++)
    {
      चयन (format[गिनती])
        {
        हाल '-':
        हाल '0':
          अवरोध;

        हाल '\'':
          flags |= FLAG_THOUSANDS;
          अवरोध;

        हाल '#':
          flags |= FLAG_ALTERNATIVE;
          अवरोध;

        शेष:
          *flags_ptr = flags;
          वापस गिनती;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
अटल व्योम
check_format_conv_type (अक्षर *format, पूर्णांक flags)
{
  अचिन्हित अक्षर ch = *format;
  पूर्णांक compatible_flags = FLAG_THOUSANDS;

  चयन (ch)
    {
    हाल 'd':
    हाल 'i':
      अवरोध;

    हाल 'u':
      *format = 'd';
      अवरोध;

    हाल 'o':
    हाल 'x':
    हाल 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      अवरोध;

    हाल 0:
      error (निकास_त्रुटि, 0, _("missing conversion specifier in suffix"));

    शेष:
      अगर (है_छाप (ch))
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      अन्यथा
        error (निकास_त्रुटि, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  अगर (flags & ~ compatible_flags)
    error (निकास_त्रुटि, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
अटल idx_t
max_out (अक्षर *format)
{
  bool percent = false;

  क्रम (अक्षर *f = format; *f; f++)
    अगर (*f == '%' && *++f != '%')
      {
        अगर (percent)
          error (निकास_त्रुटि, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        पूर्णांक flags;
        f += get_format_flags (f, &flags);
        जबतक (c_isdigit (*f))
          f++;
        अगर (*f == '.')
          जबतक (c_isdigit (*++f))
            जारी;
        check_format_conv_type (f, flags);
      }

  अगर (! percent)
    error (निकास_त्रुटि, 0,
           _("missing %% conversion specification in suffix"));

  पूर्णांक maxlen = snprintf (शून्य, 0, format, पूर्णांक_उच्च);
  अगर (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  वापस maxlen;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  global_argv = argv;

  जबतक ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, शून्य))
         != -1)
    चयन (optc)
      {
      हाल 'f':
        prefix = optarg;
        अवरोध;

      हाल 'b':
        suffix = optarg;
        अवरोध;

      हाल 'k':
        remove_files = false;
        अवरोध;

      हाल 'n':
        digits = xdectoimax (optarg, 0, MIN (पूर्णांक_उच्च, IDX_MAX), "",
                             _("invalid number"), 0);
        अवरोध;

      हाल 's':
      हाल 'q':
        suppress_गिनती = true;
        अवरोध;

      हाल 'z':
        elide_empty_files = true;
        अवरोध;

      हाल SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        अवरोध;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      शेष:
        usage (निकास_त्रुटि);
      }

  अगर (argc - optind < 2)
    {
      अगर (argc <= optind)
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
  idx_t filename_आकार;
  अगर (ckd_add (&filename_आकार, prefix_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_आकार);

  set_निवेश_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    क्रमागत { nsigs = countof (term_sig) };

    काष्ठा sigaction act;

    sigemptyset (&caught_signals);
    क्रम (पूर्णांक i = 0; i < nsigs; i++)
      {
        sigaction (term_sig[i], शून्य, &act);
        अगर (act.sa_handler != संक_छोड़ो)
          sigaddset (&caught_signals, term_sig[i]);
      }

    act.sa_handler = interrupt_handler;
    act.sa_mask = caught_signals;
    act.sa_flags = 0;

    क्रम (पूर्णांक i = 0; i < nsigs; i++)
      अगर (sigismember (&caught_signals, term_sig[i]))
        sigaction (term_sig[i], &act, शून्य);
  }

  split_file ();

  अगर (बंद (STDIN_FILENO) != 0)
    {
      error (0, त्रुटि_सं, _("read error"));
      cleanup_fatal ();
    }

  वापस निकास_सफल;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
Read standard निवेश अगर खाता is -\n\
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
         करो not हटाओ output files on errors\n\
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
         करो not print counts of output file sizes\n\
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
