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
#تعریف_کړه پروګرام_NAME "csplit"

#تعریف_کړه AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#تعریف_کړه DEFAULT_مختاړی	"xx"

/* A compiled pattern arg. */
بنية control
{
  intmax_t دننه_واټن;		/* Offset from regexp to split at. */
  intmax_t کرښې_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحيح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  بنية re_نمونه_بفر re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#تعریف_کړه پیل_اندازه	8191

/* Number of lines kept in each node in line list. */
#تعریف_کړه CTRL_اندازه	80

#که_تعریف_شوی DEBUG
/* Some small values to test the algorithms. */
# define پیل_اندازه	200
# define CTRL_اندازه	1
#د_که_پای

/* A string with a length count. */
بنية cstring
{
  idx_t len;
  حرف *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
بنية کرښه
{
  idx_t کارول_شوی;			/* Number of offsets used in this struct. */
  idx_t ورننباسه_شاخص;		/* Next offset to use when inserting line. */
  idx_t retrieve_شاخص;	/* Next index to use when retrieving line. */
  بنية cstring starts[CTRL_اندازه]; /* Lines in the data area. */
  بنية کرښه *بل;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
بنية بفر_ریکارډ
{
  idx_t بایټونه_alloc;		/* Size of the buffer area. */
  idx_t بایټونه_کارول_شوی;		/* Bytes used in the buffer area. */
  intmax_t پیل_کرښه;		/* First line number in this buffer. */
  intmax_t لومړی_شته;	/* First line that can be retrieved. */
  idx_t عدد_کرښې;		/* Number of complete lines in this buffer. */
  حرف *بفر COUNTED_BY (بایټونه_alloc);	/* Data area. */
  بنية کرښه *کرښه_پیل;	/* Head of list of pointers to lines. */
  بنية کرښه *curr_کرښه;	/* The line start record currently in use. */
  بنية بفر_ریکارډ *بل;
};

ساكن فراغ close_output_file (فراغ);
ساكن فراغ create_output_file (فراغ);
ساكن فراغ delete_all_files (bool);
ساكن فراغ save_کرښه_منزل_file (ثابت بنية cstring *کرښه);

/* Start of buffer list. */
ساكن بنية بفر_ریکارډ *سر = عدم;

/* Partially read line. */
ساكن حرف *hold_area = عدم;

/* Number of bytes in 'hold_area'. */
ساكن idx_t hold_شمېر = 0;

/* Number of the last line in the buffers. */
ساكن intmax_t وروستی_کرښه_شمېره = 0;

/* Number of the line currently being examined. */
ساكن intmax_t current_کرښه = 0;

/* If true, we have read EOF. */
ساكن bool have_read_eof = false;

/* Name of output files. */
ساكن حرف *متطاير filename_space = عدم;

/* Prefix part of output file names. */
ساكن حرف ثابت *متطاير مختاړی = DEFAULT_مختاړی;

/* Suffix part of output file names. */
ساكن حرف *متطاير suffix = عدم;

/* Number of digits to use in output file names. */
ساكن صحيح متطاير digits = 2;

/* Number of files created so far. */
ساكن صحيح متطاير files_created = 0;

/* Number of bytes written to current file. */
ساكن intmax_t بایټونه_written;

/* Output file pointer. */
ساكن FILE *output_بهیر = عدم;

/* Output file name. */
ساكن حرف *output_filename = عدم;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ساكن حرف **global_argv;

/* If true, do not print the count of bytes in each output file. */
ساكن bool suppress_شمېر;

/* If true, remove output files on error. */
ساكن bool متطاير remove_files = true;

/* If true, remove all output files which have a zero length. */
ساكن bool elide_تشه_files;

/* If true, suppress the lines that match the PATTERN */
ساكن bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ساكن بنية control *controls;

/* Number of elements in 'controls'. */
ساكن idx_t control_کارول_شوی;

/* The set of signals that are caught.  */
ساكن sigset_t caught_سیګنالونه;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"digits", required_دلیل, عدم, 'n'},
  {"quiet", no_دلیل, عدم, 'q'},  /* Deprecated.  */
  {"silent", no_دلیل, عدم, 's'},
  {"keep-files", no_دلیل, عدم, 'k'},
  {"elide-empty-files", no_دلیل, عدم, 'z'},
  {"prefix", required_دلیل, عدم, 'f'},
  {"suffix-format", required_دلیل, عدم, 'b'},
  {"suppress-matched", no_دلیل, عدم, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ساكن فراغ
cleanup (فراغ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_سیګنالونه, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, عدم);
}

ساكن _Noreturn فراغ
cleanup_fatal (فراغ)
{
  cleanup ();
  وتل (EXIT_FAILURE);
}

خارجي فراغ
xalloc_die (فراغ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ساكن فراغ
مداخله_handler (صحيح sig)
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
save_منزل_hold_area (حرف *پیل, idx_t عدد)
{
  حرر (hold_area);
  hold_area = پیل;
  hold_شمېر = عدد;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ساكن idx_t
read_ننوت (حرف *dest, idx_t max_n_بایټونه)
{
  إذا (max_n_بایټونه == 0)
    أرجع 0;

  ptrdiff_t بایټونه_read = خوندي_read (STDIN_FILENO, dest, max_n_بایټونه);

  إذا (بایټونه_read == 0)
    have_read_eof = true;

  إذا (بایټونه_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع بایټونه_read;
}

/* Initialize existing line record P. */

ساكن فراغ
clear_کرښه_control (بنية کرښه *p)
{
  p->کارول_شوی = 0;
  p->ورننباسه_شاخص = 0;
  p->retrieve_شاخص = 0;
}

/* Return a new, initialized line record. */

ساكن بنية کرښه *
new_کرښه_control (فراغ)
{
  بنية کرښه *p = xmalloc (حجم *p);

  p->بل = عدم;
  clear_کرښه_control (p);

  أرجع p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ساكن فراغ
keep_new_کرښه (بنية بفر_ریکارډ *b, حرف *کرښه_پیل, idx_t کرښه_len)
{
  بنية کرښه *l;

  /* If there is no existing area to keep line info, get some. */
  إذا (b->کرښه_پیل == عدم)
    b->کرښه_پیل = b->curr_کرښه = new_کرښه_control ();

  /* If existing area for lines is full, get more. */
  إذا (b->curr_کرښه->کارول_شوی == CTRL_اندازه)
    {
      b->curr_کرښه->بل = new_کرښه_control ();
      b->curr_کرښه = b->curr_کرښه->بل;
    }

  l = b->curr_کرښه;

  /* Record the start of the line, and update counters. */
  l->starts[l->ورننباسه_شاخص].str = کرښه_پیل;
  l->starts[l->ورننباسه_شاخص].len = کرښه_len;
  l->کارول_شوی++;
  l->ورننباسه_شاخص++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ساكن idx_t
ریکارډ_کرښه_starts (بنية بفر_ریکارډ *b)
{
  حرف *کرښه_پیل;		/* Start of current line. */
  idx_t کرښې;			/* Number of lines found. */
  idx_t کرښه_اوږدتیا;		/* Length of each line found. */

  إذا (b->بایټونه_کارول_شوی == 0)
    أرجع 0;

  کرښې = 0;
  کرښه_پیل = b->بفر;
  حرف *بفر_پای_ټکی = کرښه_پیل + b->بایټونه_کارول_شوی;
  *بفر_پای_ټکی = '\n';

  طالما (true)
    {
      حرف *کرښه_پای_ټکی = rawmemchr (کرښه_پیل, '\n');
      إذا (کرښه_پای_ټکی == بفر_پای_ټکی)
        اكسر;
      کرښه_اوږدتیا = کرښه_پای_ټکی - کرښه_پیل + 1;
      keep_new_کرښه (b, کرښه_پیل, کرښه_اوږدتیا);
      کرښه_پیل = کرښه_پای_ټکی + 1;
      کرښې++;
    }

  /* Check for an incomplete last line. */
  idx_t بایټونه_کیڼ = بفر_پای_ټکی - کرښه_پیل;
  إذا (بایټونه_کیڼ)
    {
      إذا (have_read_eof)
        {
          keep_new_کرښه (b, کرښه_پیل, بایټونه_کیڼ);
          کرښې++;
        }
      وإلا
        save_منزل_hold_area (ximemdup (کرښه_پیل, بایټونه_کیڼ), بایټونه_کیڼ);
    }

  b->عدد_کرښې = کرښې;
  b->لومړی_شته = b->پیل_کرښه = وروستی_کرښه_شمېره + 1;
  وروستی_کرښه_شمېره += کرښې;

  أرجع کرښې;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#د_که_پای

ساكن فراغ
free_بفر (بنية بفر_ریکارډ *buf)
{
  لكل (بنية کرښه *l = buf->کرښه_پیل; l;)
    {
      بنية کرښه *n = l->بل;
      حرر (l);
      l = n;
    }
  حرر (buf->بفر);
  حرر (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ساكن ATTRIBUTE_DEALLOC (free_بفر, 1)
بنية بفر_ریکارډ *
get_new_بفر (idx_t min_اندازه)
{
  بنية بفر_ریکارډ *new_بفر = xmalloc (حجم *new_بفر);
  new_بفر->بایټونه_alloc = 0;
  new_بفر->بفر = xpalloc (عدم, &new_بفر->بایټونه_alloc, min_اندازه,
                                -1, 1);
  new_بفر->بایټونه_کارول_شوی = 0;
  new_بفر->پیل_کرښه = new_بفر->لومړی_شته = وروستی_کرښه_شمېره + 1;
  new_بفر->عدد_کرښې = 0;
  new_بفر->کرښه_پیل = new_بفر->curr_کرښه = عدم;
  new_بفر->بل = عدم;

  أرجع new_بفر;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ساكن فراغ
save_بفر (بنية بفر_ریکارډ *buf)
{
  buf->بل = عدم;
  buf->curr_کرښه = buf->کرښه_پیل;

  إذا (سر == عدم)
    سر = buf;
  وإلا
    {
      بنية بفر_ریکارډ *p;
      لكل (p = سر; p->بل; p = p->بل)
        /* Do nothing. */ ;
      p->بل = buf;
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
بارول_بفر (فراغ)
{
  إذا (have_read_eof)
    أرجع false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t بایټونه_wanted = MAX (پیل_اندازه, hold_شمېر + 1);

  طالما (true)
    {
      بنية بفر_ریکارډ *b = get_new_بفر (بایټونه_wanted);
      idx_t بایټونه_alloc = b->بایټونه_alloc;
      idx_t بایټونه_avail = بایټونه_alloc;
      حرف *p = b->بفر;

      /* First check the 'holding' area for a partial line. */
      إذا (hold_شمېر)
        {
          p = mempcpy (p, hold_area, hold_شمېر);
          b->بایټونه_کارول_شوی += hold_شمېر;
          بایټونه_avail -= hold_شمېر;
          hold_شمېر = 0;
        }

      b->بایټونه_کارول_شوی += read_ننوت (p, بایټونه_avail - 1);

      إذا (ریکارډ_کرښه_starts (b) != 0)
        {
          save_بفر (b);
          أرجع true;
        }

      free_بفر (b);
      إذا (have_read_eof)
        أرجع false;
      إذا (ckd_جمع_کړه (&بایټونه_wanted, بایټونه_alloc, بایټونه_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ساكن intmax_t
get_لومړی_کرښه_دننه_بفر (فراغ)
{
  إذا (سر == عدم && !بارول_بفر ())
    أرجع 0;

  أرجع سر->لومړی_شته;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ساكن بنية cstring *
remove_کرښه (فراغ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ساكن بنية بفر_ریکارډ *prev_buf = عدم;

  بنية cstring *کرښه;		/* Return value. */
  بنية کرښه *l;		/* For convenience. */

  إذا (prev_buf)
    {
      free_بفر (prev_buf);
      prev_buf = عدم;
    }

  إذا (سر == عدم && !بارول_بفر ())
    أرجع عدم;

  إذا (current_کرښه < سر->لومړی_شته)
    current_کرښه = سر->لومړی_شته;

  ++(سر->لومړی_شته);

  l = سر->curr_کرښه;

  کرښه = &l->starts[l->retrieve_شاخص];

  /* Advance index to next line. */
  إذا (++l->retrieve_شاخص == l->کارول_شوی)
    {
      /* Go on to the next line record. */
      سر->curr_کرښه = l->بل;
      إذا (سر->curr_کرښه == عدم || سر->curr_کرښه->کارول_شوی == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = سر;
          سر = سر->بل;
        }
    }

  أرجع کرښه;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ساكن بنية cstring *
پیدا_کړه_کرښه (intmax_t linenum)
{
  إذا (سر == عدم && !بارول_بفر ())
    أرجع عدم;

  إذا (linenum < سر->پیل_کرښه)
    أرجع عدم;

  لكل (بنية بفر_ریکارډ *b = سر;;)
    {
      إذا (linenum < b->پیل_کرښه + b->عدد_کرښې)
        {
          /* The line is in this buffer. */
          بنية کرښه *l;
          idx_t دننه_واټن;	/* How far into the buffer the line is. */

          l = b->کرښه_پیل;
          دننه_واټن = linenum - b->پیل_کرښه;
          /* Find the control record. */
          طالما (دننه_واټن >= CTRL_اندازه)
            {
              l = l->بل;
              دننه_واټن -= CTRL_اندازه;
            }
          أرجع &l->starts[دننه_واټن];
        }
      إذا (b->بل == عدم && !بارول_بفر ())
        أرجع عدم;
      b = b->بل;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ساكن bool
no_نور_کرښې (فراغ)
{
  أرجع پیدا_کړه_کرښه (current_کرښه + 1) == عدم;
}

/* Open NAME as standard input.  */

ساكن فراغ
سېټ_ننوت_file (حرف ثابت *name)
{
  إذا (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ساكن فراغ
write_منزل_file (intmax_t وروستی_کرښه, bool ignore, صحيح argnum)
{
  بنية cstring *کرښه;
  intmax_t لومړی_کرښه;		/* First available input line. */
  intmax_t کرښې;		/* Number of lines to output. */

  لومړی_کرښه = get_لومړی_کرښه_دننه_بفر ();

  إذا (! لومړی_کرښه || لومړی_کرښه > وروستی_کرښه)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  کرښې = وروستی_کرښه - لومړی_کرښه;

  لكل (intmax_t i = 0; i < کرښې; i++)
    {
      کرښه = remove_کرښه ();
      إذا (کرښه == عدم)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      إذا (!ignore)
        save_کرښه_منزل_file (کرښه);
    }
}

/* Output any lines left after all regexps have been processed. */

ساكن فراغ
dump_rest_د_file (فراغ)
{
  بنية cstring *کرښه;

  طالما ((کرښه = remove_کرښه ()) != عدم)
    save_کرښه_منزل_file (کرښه);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ساكن فراغ
handle_کرښه_error (ثابت بنية control *p, intmax_t repetition)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           پروګرام_name, quote (imaxtostr (p->کرښې_required, buf)));
  إذا (repetition)
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
process_کرښه_شمېر (ثابت بنية control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t وروستی_کرښه_منزل_save = p->کرښې_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  إذا (no_نور_کرښې () && suppress_matched)
    handle_کرښه_error (p, repetition);

  إذا (!(linenum = get_لومړی_کرښه_دننه_بفر ()))
    handle_کرښه_error (p, repetition);

  طالما (linenum++ < وروستی_کرښه_منزل_save)
    {
      بنية cstring *کرښه = remove_کرښه ();
      إذا (کرښه == عدم)
        handle_کرښه_error (p, repetition);
      save_کرښه_منزل_file (کرښه);
    }

  close_output_file ();

  إذا (suppress_matched)
    remove_کرښه ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  إذا (no_نور_کرښې () && !suppress_matched)
    handle_کرښه_error (p, repetition);
}

ساكن فراغ
regexp_error (بنية control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           پروګرام_name, quote (global_argv[p->argnum]));

  إذا (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وإلا
    fprintf (stderr, "\n");

  إذا (!ignore)
    {
      dump_rest_د_file ();
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
  بنية cstring *کرښه;		/* From input file. */
  idx_t کرښه_len;		/* To make "$" in regexps work. */
  intmax_t break_کرښه;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  إذا (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  إذا (p->دننه_واټن >= 0)
    {
      طالما (true)
        {
          کرښه = پیدا_کړه_کرښه (++current_کرښه);
          إذا (کرښه == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_د_file ();
                      close_output_file ();
                    }
                  وتل (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          کرښه_len = کرښه->len;
          إذا (کرښه->str[کرښه_len - 1] == '\n')
            کرښه_len--;
          ret = re_پلټنه (&p->re_compiled, کرښه->str, کرښه_len,
                           0, کرښه_len, عدم);
          إذا (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          إذا (ret == -1)
            {
              کرښه = remove_کرښه ();
              إذا (!ignore)
                save_کرښه_منزل_file (کرښه);
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
          کرښه = پیدا_کړه_کرښه (++current_کرښه);
          إذا (کرښه == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_د_file ();
                      close_output_file ();
                    }
                  وتل (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          کرښه_len = کرښه->len;
          إذا (کرښه->str[کرښه_len - 1] == '\n')
            کرښه_len--;
          ret = re_پلټنه (&p->re_compiled, کرښه->str, کرښه_len,
                           0, کرښه_len, عدم);
          إذا (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          إذا (ret != -1)
            اكسر;
        }
    }

  /* Account for any offset from this regexp. */
  break_کرښه = current_کرښه + p->دننه_واټن;

  write_منزل_file (break_کرښه, ignore, p->argnum);

  إذا (!ignore)
    close_output_file ();

  إذا (p->دننه_واټن > 0)
    current_کرښه = break_کرښه;

  إذا (suppress_matched)
    remove_کرښه ();
}

/* Split the input file according to the control records we have built. */

ساكن فراغ
وېشل_file (فراغ)
{
  لكل (idx_t i = 0; i < control_کارول_شوی; i++)
    {
      إذا (controls[i].regexpr)
        {
          لكل (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      وإلا
        {
          لكل (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_کرښه_شمېر (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_د_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ساكن حرف *
make_filename (صحيح عدد)
{
  strcpy (filename_space, مختاړی);
  إذا (suffix)
    sprintf (filename_space + مزي_اوږدوالی (مختاړی), suffix, عدد);
  وإلا
    sprintf (filename_space + مزي_اوږدوالی (مختاړی), "%0*d", digits, عدد);
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

  إذا (nfiles == لوی_عدد)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  وإلا
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_سیګنالونه, &oldset);
      output_بهیر = fopen (output_filename, "w");
      fopen_ok = (output_بهیر != عدم);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, عدم);
    }

  إذا (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  بایټونه_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ساكن فراغ
delete_all_files (bool دننه_signal_handler)
{
  إذا (! remove_files)
    أرجع;

  لكل (صحيح i = files_created; 0 <= --i; )
    {
      حرف ثابت *name = make_filename (i);
      إذا (unlink (name) != 0 && errno != ENOENT && !دننه_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ساكن فراغ
close_output_file (فراغ)
{
  إذا (output_بهیر)
    {
      إذا (ferror (output_بهیر))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_بهیر = عدم;
          cleanup_fatal ();
        }
      إذا (fclose (output_بهیر) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_بهیر = عدم;
          cleanup_fatal ();
        }
      إذا (بایټونه_written == 0 && elide_تشه_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحيح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_سیګنالونه, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, عدم);

          إذا (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      وإلا
        {
          إذا (!suppress_شمېر)
            fprintf (stdout, "%jd\n", بایټونه_written);
        }
      output_بهیر = عدم;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ساكن فراغ
save_کرښه_منزل_file (ثابت بنية cstring *کرښه)
{
  idx_t l = fwrite (کرښه->str, حجم (حرف), کرښه->len, output_بهیر);
  إذا (l != کرښه->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_بهیر = عدم;
      cleanup_fatal ();
    }
  بایټونه_written += کرښه->len;
}

/* Return a new, initialized control record. */

ساكن بنية control *
new_control_ریکارډ (فراغ)
{
  ساكن idx_t control_allocated = 0; /* Total space allocated. */
  بنية control *p;

  إذا (control_کارول_شوی == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, حجم *controls);
  p = &controls[control_کارول_شوی++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->کرښې_required = 0;
  p->دننه_واټن = 0;
  أرجع p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ساكن فراغ
check_for_دننه_واټن (بنية control *p, حرف ثابت *str, حرف ثابت *عدد)
{
  إذا (xstrtoimax (عدد, عدم, 10, &p->دننه_واټن, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ساكن فراغ
parse_repeat_شمېر (صحيح argnum, بنية control *p, حرف *str)
{
  حرف *پای_ټکی;

  پای_ټکی = str + مزي_اوږدوالی (str) - 1;
  إذا (*پای_ټکی != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *پای_ټکی = '\0';

  إذا (str + 1 == پای_ټکی - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  وإلا
    {
      uintmax_t val;
      إذا (xstrtoumax (str + 1, عدم, 10, &val, "") != LONGINT_OK
          || ckd_جمع_کړه (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *پای_ټکی = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ساكن بنية control *
ایستل_regexp (صحيح argnum, bool ignore, حرف ثابت *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  حرف delim = *str;
  حرف ثابت *closing_delim;
  بنية control *p;
  حرف ثابت *err;

  closing_delim = strrchr (str + 1, delim);
  إذا (closing_delim == عدم)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_ریکارډ ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.بفر = عدم;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = عدم;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_تشه_RANGES;
  err = re_compile_نمونه (str + 1, len, &p->re_compiled);
  إذا (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  إذا (closing_delim[1])
    check_for_دننه_واټن (p, str, closing_delim + 1);

  أرجع p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ساكن فراغ
parse_patterns (صحيح argc, صحيح پیل, حرف **argv)
{
  بنية control *p;		/* New control record created. */
  ساكن intmax_t وروستی_val = 0;

  لكل (صحيح i = پیل; i < argc; i++)
    {
      إذا (*argv[i] == '/' || *argv[i] == '%')
        {
          p = ایستل_regexp (i, *argv[i] == '%', argv[i]);
        }
      وإلا
        {
          p = new_control_ریکارډ ();
          p->argnum = i;

          uintmax_t val;
          إذا (xstrtoumax (argv[i], عدم, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          إذا (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          إذا (val < وروستی_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), وروستی_val);

          إذا (val == وروستی_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          وروستی_val = val;

          p->کرښې_required = val;
        }

      إذا (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_شمېر (i, p, argv[i]);
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

  لكل (idx_t شمېر = 0; ; شمېر++)
    {
      بدل (format[شمېر])
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
          أرجع شمېر;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ساكن فراغ
check_format_conv_type (حرف *format, صحيح flags)
{
  بینښه حرف ch = *format;
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
      إذا (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      وإلا
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  إذا (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ساكن idx_t
max_بهر (حرف *format)
{
  bool percent = false;

  لكل (حرف *f = format; *f; f++)
    إذا (*f == '%' && *++f != '%')
      {
        إذا (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        صحيح flags;
        f += get_format_flags (f, &flags);
        طالما (c_isdigit (*f))
          f++;
        إذا (*f == '.')
          طالما (c_isdigit (*++f))
            استمر;
        check_format_conv_type (f, flags);
      }

  إذا (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  صحيح maxlen = snprintf (عدم, 0, format, لوی_عدد);
  إذا (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  أرجع maxlen;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح optc;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
        مختاړی = optarg;
        اكسر;

      حالة 'b':
        suffix = optarg;
        اكسر;

      حالة 'k':
        remove_files = false;
        اكسر;

      حالة 'n':
        digits = xdectoimax (optarg, 0, MIN (لوی_عدد, IDX_MAX), "",
                             _("invalid number"), 0);
        اكسر;

      حالة 's':
      حالة 'q':
        suppress_شمېر = true;
        اكسر;

      حالة 'z':
        elide_تشه_files = true;
        اكسر;

      حالة SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  إذا (argc - optind < 2)
    {
      إذا (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t مختاړی_len = مزي_اوږدوالی (مختاړی);
  idx_t max_digit_string_len
    = (suffix
       ? max_بهر (suffix)
       : MAX (INT_STRLEN_BOUND (صحيح), digits));
  idx_t filename_اندازه;
  إذا (ckd_جمع_کړه (&filename_اندازه, مختاړی_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_اندازه);

  سېټ_ننوت_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    تعداد { nsigs = countof (غړی_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سیګنالونه);
    لكل (صحيح i = 0; i < nsigs; i++)
      {
        sigaction (غړی_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سیګنالونه, غړی_sig[i]);
      }

    act.sa_handler = مداخله_handler;
    act.sa_mask = caught_سیګنالونه;
    act.sa_flags = 0;

    لكل (صحيح i = 0; i < nsigs; i++)
      إذا (sigismember (&caught_سیګنالونه, غړی_sig[i]))
        sigaction (غړی_sig[i], &act, عدم);
  }

  وېشل_file ();

  إذا (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع EXIT_SUCCESS;
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... FILE نمونه...\n\
"),
              پروګرام_name);
      fputs (_("\
Output pieces د FILE separated by نمونه(s) منزل files 'xx00', 'xx01', ...,\n\
and output byte counts د each piece منزل standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard ننوت إذا FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead د %02d\n\
"));
      oputs (_("\
  -f, --مختاړی=مختاړی_2\n\
         use مختاړی_2 instead د 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         افعل not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the کرښې matching نمونه\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified شمېره د digits instead د 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         افعل not print counts د output file sizes\n\
"));
      oputs (_("\
  -z, --elide-تشه-files\n\
         suppress تشه output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each نمونه may be:\n\
  INTEGER            copy پورته منزل but not including specified کرښه شمېره\n\
  /REGEXP/[دننه_واټن_2]   copy پورته منزل but not including a matching کرښه\n\
  %REGEXP%[دننه_واټن_2]   skip منزل, but not including a matching کرښه\n\
  {INTEGER}          repeat the پخوانی نمونه_2 specified شمېره د times\n\
  {*}                repeat the پخوانی نمونه_2 as many times as possible\n\
\n\
A کرښه دننه_واټن_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}
