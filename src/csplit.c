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
#تعریف_کرو پروگرام_NAME "csplit"

#تعریف_کرو AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#تعریف_کرو DEFAULT_سابقہ	"xx"

/* A compiled pattern arg. */
ساخت control
{
  intmax_t انحراف;		/* Offset from regexp to split at. */
  intmax_t سطریں_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحیح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ساخت re_نمونہ_بفر re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#تعریف_کرو آغاز_سائز	8191

/* Number of lines kept in each node in line list. */
#تعریف_کرو CTRL_سائز	80

#اگر_تعریف_شدہ DEBUG
/* Some small values to test the algorithms. */
# define آغاز_سائز	200
# define CTRL_سائز	1
#اگر_ختم

/* A string with a length count. */
ساخت cstring
{
  idx_t len;
  حرف *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ساخت سطر
{
  idx_t مستعمل;			/* Number of offsets used in this struct. */
  idx_t داخل_کرو_اشاریہ;		/* Next offset to use when inserting line. */
  idx_t retrieve_اشاریہ;	/* Next index to use when retrieving line. */
  ساخت cstring starts[CTRL_سائز]; /* Lines in the data area. */
  ساخت سطر *اگلا;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ساخت بفر_ریکارڈ
{
  idx_t بائٹیں_alloc;		/* Size of the buffer area. */
  idx_t بائٹیں_مستعمل;		/* Bytes used in the buffer area. */
  intmax_t آغاز_سطر;		/* First line number in this buffer. */
  intmax_t پہلا_دستیاب;	/* First line that can be retrieved. */
  idx_t عدد_سطریں;		/* Number of complete lines in this buffer. */
  حرف *بفر COUNTED_BY (بائٹیں_alloc);	/* Data area. */
  ساخت سطر *سطر_آغاز;	/* Head of list of pointers to lines. */
  ساخت سطر *curr_سطر;	/* The line start record currently in use. */
  ساخت بفر_ریکارڈ *اگلا;
};

ساکن خالی close_output_file (خالی);
ساکن خالی create_output_file (خالی);
ساکن خالی delete_all_files (bool);
ساکن خالی save_سطر_مقصد_file (ثابت ساخت cstring *سطر);

/* Start of buffer list. */
ساکن ساخت بفر_ریکارڈ *سر = تہی;

/* Partially read line. */
ساکن حرف *hold_area = تہی;

/* Number of bytes in 'hold_area'. */
ساکن idx_t hold_گنتی = 0;

/* Number of the last line in the buffers. */
ساکن intmax_t آخری_سطر_نمبر = 0;

/* Number of the line currently being examined. */
ساکن intmax_t current_سطر = 0;

/* If true, we have read EOF. */
ساکن bool have_read_eof = false;

/* Name of output files. */
ساکن حرف *متلون filename_space = تہی;

/* Prefix part of output file names. */
ساکن حرف ثابت *متلون سابقہ = DEFAULT_سابقہ;

/* Suffix part of output file names. */
ساکن حرف *متلون suffix = تہی;

/* Number of digits to use in output file names. */
ساکن صحیح متلون digits = 2;

/* Number of files created so far. */
ساکن صحیح متلون files_created = 0;

/* Number of bytes written to current file. */
ساکن intmax_t بائٹیں_written;

/* Output file pointer. */
ساکن FILE *output_دھارا = تہی;

/* Output file name. */
ساکن حرف *output_filename = تہی;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ساکن حرف **global_argv;

/* If true, do not print the count of bytes in each output file. */
ساکن bool suppress_گنتی;

/* If true, remove output files on error. */
ساکن bool متلون remove_files = true;

/* If true, remove all output files which have a zero length. */
ساکن bool elide_خالی_خانے_files;

/* If true, suppress the lines that match the PATTERN */
ساکن bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ساکن ساخت control *controls;

/* Number of elements in 'controls'. */
ساکن idx_t control_مستعمل;

/* The set of signals that are caught.  */
ساکن sigset_t caught_اشارات;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"digits", required_دلیل, تہی, 'n'},
  {"quiet", no_دلیل, تہی, 'q'},  /* Deprecated.  */
  {"silent", no_دلیل, تہی, 's'},
  {"keep-files", no_دلیل, تہی, 'k'},
  {"elide-empty-files", no_دلیل, تہی, 'z'},
  {"prefix", required_دلیل, تہی, 'f'},
  {"suffix-format", required_دلیل, تہی, 'b'},
  {"suppress-matched", no_دلیل, تہی, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ساکن خالی
cleanup (خالی)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_اشارات, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, تہی);
}

ساکن _Noreturn خالی
cleanup_fatal (خالی)
{
  cleanup ();
  باہر_نکلو (EXIT_FAILURE);
}

خارجی خالی
xalloc_die (خالی)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ساکن خالی
مداخلت_handler (صحیح sig)
{
  delete_all_files (true);
  اشارہ (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  اٹھاؤ (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ساکن خالی
save_مقصد_hold_area (حرف *آغاز, idx_t عدد)
{
  آزاد (hold_area);
  hold_area = آغاز;
  hold_گنتی = عدد;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ساکن idx_t
read_داخلہ (حرف *dest, idx_t max_n_بائٹیں)
{
  اگر (max_n_بائٹیں == 0)
    واپس 0;

  ptrdiff_t بائٹیں_read = محفوظ_read (STDIN_FILENO, dest, max_n_بائٹیں);

  اگر (بائٹیں_read == 0)
    have_read_eof = true;

  اگر (بائٹیں_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  واپس بائٹیں_read;
}

/* Initialize existing line record P. */

ساکن خالی
clear_سطر_control (ساخت سطر *p)
{
  p->مستعمل = 0;
  p->داخل_کرو_اشاریہ = 0;
  p->retrieve_اشاریہ = 0;
}

/* Return a new, initialized line record. */

ساکن ساخت سطر *
new_سطر_control (خالی)
{
  ساخت سطر *p = xmalloc (حجم *p);

  p->اگلا = تہی;
  clear_سطر_control (p);

  واپس p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ساکن خالی
keep_new_سطر (ساخت بفر_ریکارڈ *b, حرف *سطر_آغاز, idx_t سطر_len)
{
  ساخت سطر *l;

  /* If there is no existing area to keep line info, get some. */
  اگر (b->سطر_آغاز == تہی)
    b->سطر_آغاز = b->curr_سطر = new_سطر_control ();

  /* If existing area for lines is full, get more. */
  اگر (b->curr_سطر->مستعمل == CTRL_سائز)
    {
      b->curr_سطر->اگلا = new_سطر_control ();
      b->curr_سطر = b->curr_سطر->اگلا;
    }

  l = b->curr_سطر;

  /* Record the start of the line, and update counters. */
  l->starts[l->داخل_کرو_اشاریہ].str = سطر_آغاز;
  l->starts[l->داخل_کرو_اشاریہ].len = سطر_len;
  l->مستعمل++;
  l->داخل_کرو_اشاریہ++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ساکن idx_t
ریکارڈ_سطر_starts (ساخت بفر_ریکارڈ *b)
{
  حرف *سطر_آغاز;		/* Start of current line. */
  idx_t سطریں;			/* Number of lines found. */
  idx_t سطر_لمبائی;		/* Length of each line found. */

  اگر (b->بائٹیں_مستعمل == 0)
    واپس 0;

  سطریں = 0;
  سطر_آغاز = b->بفر;
  حرف *بفر_انتہا = سطر_آغاز + b->بائٹیں_مستعمل;
  *بفر_انتہا = '\n';

  جب (true)
    {
      حرف *سطر_انتہا = rawmemchr (سطر_آغاز, '\n');
      اگر (سطر_انتہا == بفر_انتہا)
        توڑو;
      سطر_لمبائی = سطر_انتہا - سطر_آغاز + 1;
      keep_new_سطر (b, سطر_آغاز, سطر_لمبائی);
      سطر_آغاز = سطر_انتہا + 1;
      سطریں++;
    }

  /* Check for an incomplete last line. */
  idx_t بائٹیں_بایاں = بفر_انتہا - سطر_آغاز;
  اگر (بائٹیں_بایاں)
    {
      اگر (have_read_eof)
        {
          keep_new_سطر (b, سطر_آغاز, بائٹیں_بایاں);
          سطریں++;
        }
      ورنہ
        save_مقصد_hold_area (ximemdup (سطر_آغاز, بائٹیں_بایاں), بائٹیں_بایاں);
    }

  b->عدد_سطریں = سطریں;
  b->پہلا_دستیاب = b->آغاز_سطر = آخری_سطر_نمبر + 1;
  آخری_سطر_نمبر += سطریں;

  واپس سطریں;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#اگر 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#اگر_ختم

ساکن خالی
free_بفر (ساخت بفر_ریکارڈ *buf)
{
  برائے (ساخت سطر *l = buf->سطر_آغاز; l;)
    {
      ساخت سطر *n = l->اگلا;
      آزاد (l);
      l = n;
    }
  آزاد (buf->بفر);
  آزاد (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ساکن ATTRIBUTE_DEALLOC (free_بفر, 1)
ساخت بفر_ریکارڈ *
get_new_بفر (idx_t min_سائز)
{
  ساخت بفر_ریکارڈ *new_بفر = xmalloc (حجم *new_بفر);
  new_بفر->بائٹیں_alloc = 0;
  new_بفر->بفر = xpalloc (تہی, &new_بفر->بائٹیں_alloc, min_سائز,
                                -1, 1);
  new_بفر->بائٹیں_مستعمل = 0;
  new_بفر->آغاز_سطر = new_بفر->پہلا_دستیاب = آخری_سطر_نمبر + 1;
  new_بفر->عدد_سطریں = 0;
  new_بفر->سطر_آغاز = new_بفر->curr_سطر = تہی;
  new_بفر->اگلا = تہی;

  واپس new_بفر;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ساکن خالی
save_بفر (ساخت بفر_ریکارڈ *buf)
{
  buf->اگلا = تہی;
  buf->curr_سطر = buf->سطر_آغاز;

  اگر (سر == تہی)
    سر = buf;
  ورنہ
    {
      ساخت بفر_ریکارڈ *p;
      برائے (p = سر; p->اگلا; p = p->اگلا)
        /* Do nothing. */ ;
      p->اگلا = buf;
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

ساکن bool
بھرو_بفر (خالی)
{
  اگر (have_read_eof)
    واپس false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t بائٹیں_wanted = MAX (آغاز_سائز, hold_گنتی + 1);

  جب (true)
    {
      ساخت بفر_ریکارڈ *b = get_new_بفر (بائٹیں_wanted);
      idx_t بائٹیں_alloc = b->بائٹیں_alloc;
      idx_t بائٹیں_avail = بائٹیں_alloc;
      حرف *p = b->بفر;

      /* First check the 'holding' area for a partial line. */
      اگر (hold_گنتی)
        {
          p = mempcpy (p, hold_area, hold_گنتی);
          b->بائٹیں_مستعمل += hold_گنتی;
          بائٹیں_avail -= hold_گنتی;
          hold_گنتی = 0;
        }

      b->بائٹیں_مستعمل += read_داخلہ (p, بائٹیں_avail - 1);

      اگر (ریکارڈ_سطر_starts (b) != 0)
        {
          save_بفر (b);
          واپس true;
        }

      free_بفر (b);
      اگر (have_read_eof)
        واپس false;
      اگر (ckd_جوڑو (&بائٹیں_wanted, بائٹیں_alloc, بائٹیں_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ساکن intmax_t
get_پہلا_سطر_اندر_بفر (خالی)
{
  اگر (سر == تہی && !بھرو_بفر ())
    واپس 0;

  واپس سر->پہلا_دستیاب;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ساکن ساخت cstring *
remove_سطر (خالی)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ساکن ساخت بفر_ریکارڈ *پچھلا_buf = تہی;

  ساخت cstring *سطر;		/* Return value. */
  ساخت سطر *l;		/* For convenience. */

  اگر (پچھلا_buf)
    {
      free_بفر (پچھلا_buf);
      پچھلا_buf = تہی;
    }

  اگر (سر == تہی && !بھرو_بفر ())
    واپس تہی;

  اگر (current_سطر < سر->پہلا_دستیاب)
    current_سطر = سر->پہلا_دستیاب;

  ++(سر->پہلا_دستیاب);

  l = سر->curr_سطر;

  سطر = &l->starts[l->retrieve_اشاریہ];

  /* Advance index to next line. */
  اگر (++l->retrieve_اشاریہ == l->مستعمل)
    {
      /* Go on to the next line record. */
      سر->curr_سطر = l->اگلا;
      اگر (سر->curr_سطر == تہی || سر->curr_سطر->مستعمل == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          پچھلا_buf = سر;
          سر = سر->اگلا;
        }
    }

  واپس سطر;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ساکن ساخت cstring *
ڈھونڈو_سطر (intmax_t linenum)
{
  اگر (سر == تہی && !بھرو_بفر ())
    واپس تہی;

  اگر (linenum < سر->آغاز_سطر)
    واپس تہی;

  برائے (ساخت بفر_ریکارڈ *b = سر;;)
    {
      اگر (linenum < b->آغاز_سطر + b->عدد_سطریں)
        {
          /* The line is in this buffer. */
          ساخت سطر *l;
          idx_t انحراف;	/* How far into the buffer the line is. */

          l = b->سطر_آغاز;
          انحراف = linenum - b->آغاز_سطر;
          /* Find the control record. */
          جب (انحراف >= CTRL_سائز)
            {
              l = l->اگلا;
              انحراف -= CTRL_سائز;
            }
          واپس &l->starts[انحراف];
        }
      اگر (b->اگلا == تہی && !بھرو_بفر ())
        واپس تہی;
      b = b->اگلا;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ساکن bool
no_مزید_سطریں (خالی)
{
  واپس ڈھونڈو_سطر (current_سطر + 1) == تہی;
}

/* Open NAME as standard input.  */

ساکن خالی
سیٹ_داخلہ_file (حرف ثابت *name)
{
  اگر (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ساکن خالی
write_مقصد_file (intmax_t آخری_سطر, bool ignore, صحیح argnum)
{
  ساخت cstring *سطر;
  intmax_t پہلا_سطر;		/* First available input line. */
  intmax_t سطریں;		/* Number of lines to output. */

  پہلا_سطر = get_پہلا_سطر_اندر_بفر ();

  اگر (! پہلا_سطر || پہلا_سطر > آخری_سطر)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  سطریں = آخری_سطر - پہلا_سطر;

  برائے (intmax_t i = 0; i < سطریں; i++)
    {
      سطر = remove_سطر ();
      اگر (سطر == تہی)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      اگر (!ignore)
        save_سطر_مقصد_file (سطر);
    }
}

/* Output any lines left after all regexps have been processed. */

ساکن خالی
dump_rest_کا_file (خالی)
{
  ساخت cstring *سطر;

  جب ((سطر = remove_سطر ()) != تہی)
    save_سطر_مقصد_file (سطر);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ساکن خالی
handle_سطر_error (ثابت ساخت control *p, intmax_t repetition)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           پروگرام_name, quote (imaxtostr (p->سطریں_required, buf)));
  اگر (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ورنہ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ساکن خالی
process_سطر_گنتی (ثابت ساخت control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t آخری_سطر_مقصد_save = p->سطریں_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  اگر (no_مزید_سطریں () && suppress_matched)
    handle_سطر_error (p, repetition);

  اگر (!(linenum = get_پہلا_سطر_اندر_بفر ()))
    handle_سطر_error (p, repetition);

  جب (linenum++ < آخری_سطر_مقصد_save)
    {
      ساخت cstring *سطر = remove_سطر ();
      اگر (سطر == تہی)
        handle_سطر_error (p, repetition);
      save_سطر_مقصد_file (سطر);
    }

  close_output_file ();

  اگر (suppress_matched)
    remove_سطر ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  اگر (no_مزید_سطریں () && !suppress_matched)
    handle_سطر_error (p, repetition);
}

ساکن خالی
regexp_error (ساخت control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           پروگرام_name, quote (global_argv[p->argnum]));

  اگر (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ورنہ
    fprintf (stderr, "\n");

  اگر (!ignore)
    {
      dump_rest_کا_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ساکن خالی
process_regexp (ساخت control *p, intmax_t repetition)
{
  ساخت cstring *سطر;		/* From input file. */
  idx_t سطر_len;		/* To make "$" in regexps work. */
  intmax_t break_سطر;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  اگر (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  اگر (p->انحراف >= 0)
    {
      جب (true)
        {
          سطر = ڈھونڈو_سطر (++current_سطر);
          اگر (سطر == تہی)
            {
              اگر (p->repeat_forever)
                {
                  اگر (!ignore)
                    {
                      dump_rest_کا_file ();
                      close_output_file ();
                    }
                  باہر_نکلو (EXIT_SUCCESS);
                }
              ورنہ
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          اگر (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_تلاش (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, تہی);
          اگر (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          اگر (ret == -1)
            {
              سطر = remove_سطر ();
              اگر (!ignore)
                save_سطر_مقصد_file (سطر);
            }
          ورنہ
            توڑو;
        }
    }
  ورنہ
    {
      /* Buffer the lines. */
      جب (true)
        {
          سطر = ڈھونڈو_سطر (++current_سطر);
          اگر (سطر == تہی)
            {
              اگر (p->repeat_forever)
                {
                  اگر (!ignore)
                    {
                      dump_rest_کا_file ();
                      close_output_file ();
                    }
                  باہر_نکلو (EXIT_SUCCESS);
                }
              ورنہ
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          اگر (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_تلاش (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, تہی);
          اگر (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          اگر (ret != -1)
            توڑو;
        }
    }

  /* Account for any offset from this regexp. */
  break_سطر = current_سطر + p->انحراف;

  write_مقصد_file (break_سطر, ignore, p->argnum);

  اگر (!ignore)
    close_output_file ();

  اگر (p->انحراف > 0)
    current_سطر = break_سطر;

  اگر (suppress_matched)
    remove_سطر ();
}

/* Split the input file according to the control records we have built. */

ساکن خالی
بانٹو_file (خالی)
{
  برائے (idx_t i = 0; i < control_مستعمل; i++)
    {
      اگر (controls[i].regexpr)
        {
          برائے (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ورنہ
        {
          برائے (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_سطر_گنتی (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_کا_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ساکن حرف *
make_filename (صحیح عدد)
{
  strcpy (filename_space, سابقہ);
  اگر (suffix)
    sprintf (filename_space + لڑی_لمبائی (سابقہ), suffix, عدد);
  ورنہ
    sprintf (filename_space + لڑی_لمبائی (سابقہ), "%0*d", digits, عدد);
  واپس filename_space;
}

/* Create the next output file. */

ساکن خالی
create_output_file (خالی)
{
  صحیح nfiles = files_created;
  bool fopen_ok;
  صحیح fopen_errno;

  output_filename = make_filename (nfiles);

  اگر (nfiles == زیادہ_سے_زیادہ_عدد)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ورنہ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_اشارات, &oldset);
      output_دھارا = fopen (output_filename, "w");
      fopen_ok = (output_دھارا != تہی);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, تہی);
    }

  اگر (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  بائٹیں_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ساکن خالی
delete_all_files (bool اندر_signal_handler)
{
  اگر (! remove_files)
    واپس;

  برائے (صحیح i = files_created; 0 <= --i; )
    {
      حرف ثابت *name = make_filename (i);
      اگر (unlink (name) != 0 && errno != ENOENT && !اندر_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ساکن خالی
close_output_file (خالی)
{
  اگر (output_دھارا)
    {
      اگر (ferror (output_دھارا))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_دھارا = تہی;
          cleanup_fatal ();
        }
      اگر (fclose (output_دھارا) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_دھارا = تہی;
          cleanup_fatal ();
        }
      اگر (بائٹیں_written == 0 && elide_خالی_خانے_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحیح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_اشارات, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, تہی);

          اگر (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ورنہ
        {
          اگر (!suppress_گنتی)
            fprintf (stdout, "%jd\n", بائٹیں_written);
        }
      output_دھارا = تہی;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ساکن خالی
save_سطر_مقصد_file (ثابت ساخت cstring *سطر)
{
  idx_t l = fwrite (سطر->str, حجم (حرف), سطر->len, output_دھارا);
  اگر (l != سطر->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_دھارا = تہی;
      cleanup_fatal ();
    }
  بائٹیں_written += سطر->len;
}

/* Return a new, initialized control record. */

ساکن ساخت control *
new_control_ریکارڈ (خالی)
{
  ساکن idx_t control_allocated = 0; /* Total space allocated. */
  ساخت control *p;

  اگر (control_مستعمل == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, حجم *controls);
  p = &controls[control_مستعمل++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->سطریں_required = 0;
  p->انحراف = 0;
  واپس p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ساکن خالی
check_for_انحراف (ساخت control *p, حرف ثابت *str, حرف ثابت *عدد)
{
  اگر (xstrtoimax (عدد, تہی, 10, &p->انحراف, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ساکن خالی
parse_repeat_گنتی (صحیح argnum, ساخت control *p, حرف *str)
{
  حرف *انتہا;

  انتہا = str + لڑی_لمبائی (str) - 1;
  اگر (*انتہا != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *انتہا = '\0';

  اگر (str + 1 == انتہا - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ورنہ
    {
      uintmax_t val;
      اگر (xstrtoumax (str + 1, تہی, 10, &val, "") != LONGINT_OK
          || ckd_جوڑو (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *انتہا = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ساکن ساخت control *
نکالو_regexp (صحیح argnum, bool ignore, حرف ثابت *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  حرف delim = *str;
  حرف ثابت *closing_delim;
  ساخت control *p;
  حرف ثابت *err;

  closing_delim = strrchr (str + 1, delim);
  اگر (closing_delim == تہی)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_ریکارڈ ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.بفر = تہی;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = تہی;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالی_خانے_RANGES;
  err = re_compile_نمونہ (str + 1, len, &p->re_compiled);
  اگر (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  اگر (closing_delim[1])
    check_for_انحراف (p, str, closing_delim + 1);

  واپس p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ساکن خالی
parse_patterns (صحیح argc, صحیح آغاز, حرف **argv)
{
  ساخت control *p;		/* New control record created. */
  ساکن intmax_t آخری_val = 0;

  برائے (صحیح i = آغاز; i < argc; i++)
    {
      اگر (*argv[i] == '/' || *argv[i] == '%')
        {
          p = نکالو_regexp (i, *argv[i] == '%', argv[i]);
        }
      ورنہ
        {
          p = new_control_ریکارڈ ();
          p->argnum = i;

          uintmax_t val;
          اگر (xstrtoumax (argv[i], تہی, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          اگر (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          اگر (val < آخری_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), آخری_val);

          اگر (val == آخری_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          آخری_val = val;

          p->سطریں_required = val;
        }

      اگر (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_گنتی (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
شمار { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ساکن idx_t
get_format_flags (حرف ثابت *format, صحیح *flags_ptr)
{
  صحیح flags = 0;

  برائے (idx_t گنتی = 0; ; گنتی++)
    {
      انتخاب (format[گنتی])
        {
        حالت '-':
        حالت '0':
          توڑو;

        حالت '\'':
          flags |= FLAG_THOUSANDS;
          توڑو;

        حالت '#':
          flags |= FLAG_ALTERNATIVE;
          توڑو;

        طےشدہ:
          *flags_ptr = flags;
          واپس گنتی;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ساکن خالی
check_format_conv_type (حرف *format, صحیح flags)
{
  غیرعلامتی حرف ch = *format;
  صحیح compatible_flags = FLAG_THOUSANDS;

  انتخاب (ch)
    {
    حالت 'd':
    حالت 'i':
      توڑو;

    حالت 'u':
      *format = 'd';
      توڑو;

    حالت 'o':
    حالت 'x':
    حالت 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      توڑو;

    حالت 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    طےشدہ:
      اگر (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ورنہ
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
ساکن idx_t
max_برآمد (حرف *format)
{
  bool percent = false;

  برائے (حرف *f = format; *f; f++)
    اگر (*f == '%' && *++f != '%')
      {
        اگر (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        صحیح flags;
        f += get_format_flags (f, &flags);
        جب (c_isdigit (*f))
          f++;
        اگر (*f == '.')
          جب (c_isdigit (*++f))
            جاری;
        check_format_conv_type (f, flags);
      }

  اگر (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  صحیح maxlen = snprintf (تہی, 0, format, زیادہ_سے_زیادہ_عدد);
  اگر (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  واپس maxlen;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  جب ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, تہی))
         != -1)
    انتخاب (optc)
      {
      حالت 'f':
        سابقہ = optarg;
        توڑو;

      حالت 'b':
        suffix = optarg;
        توڑو;

      حالت 'k':
        remove_files = false;
        توڑو;

      حالت 'n':
        digits = xdectoimax (optarg, 0, MIN (زیادہ_سے_زیادہ_عدد, IDX_MAX), "",
                             _("invalid number"), 0);
        توڑو;

      حالت 's':
      حالت 'q':
        suppress_گنتی = true;
        توڑو;

      حالت 'z':
        elide_خالی_خانے_files = true;
        توڑو;

      حالت SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        توڑو;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      طےشدہ:
        usage (EXIT_FAILURE);
      }

  اگر (argc - optind < 2)
    {
      اگر (argc <= optind)
        error (0, 0, _("missing operand"));
      ورنہ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t سابقہ_len = لڑی_لمبائی (سابقہ);
  idx_t max_digit_string_len
    = (suffix
       ? max_برآمد (suffix)
       : MAX (INT_STRLEN_BOUND (صحیح), digits));
  idx_t filename_سائز;
  اگر (ckd_جوڑو (&filename_سائز, سابقہ_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_سائز);

  سیٹ_داخلہ_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    شمار { nsigs = countof (رکن_sig) };

    ساخت sigaction act;

    sigemptyset (&caught_اشارات);
    برائے (صحیح i = 0; i < nsigs; i++)
      {
        sigaction (رکن_sig[i], تہی, &act);
        اگر (act.sa_handler != SIG_IGN)
          sigaddset (&caught_اشارات, رکن_sig[i]);
      }

    act.sa_handler = مداخلت_handler;
    act.sa_mask = caught_اشارات;
    act.sa_flags = 0;

    برائے (صحیح i = 0; i < nsigs; i++)
      اگر (sigismember (&caught_اشارات, رکن_sig[i]))
        sigaction (رکن_sig[i], &act, تہی);
  }

  بانٹو_file ();

  اگر (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  واپس EXIT_SUCCESS;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... FILE نمونہ...\n\
"),
              پروگرام_name);
      fputs (_("\
Output pieces کا FILE separated by نمونہ(s) مقصد files 'xx00', 'xx01', ...,\n\
and output byte counts کا each piece مقصد standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard داخلہ اگر FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead کا %02d\n\
"));
      oputs (_("\
  -f, --سابقہ=سابقہ_2\n\
         use سابقہ_2 instead کا 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         کرو not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the سطریں matching نمونہ\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified نمبر کا digits instead کا 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         کرو not print counts کا output file sizes\n\
"));
      oputs (_("\
  -z, --elide-خالی_خانے-files\n\
         suppress خالی_خانے output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each نمونہ may be:\n\
  INTEGER            copy اوپر مقصد but not including specified سطر نمبر\n\
  /REGEXP/[انحراف_2]   copy اوپر مقصد but not including a matching سطر\n\
  %REGEXP%[انحراف_2]   skip مقصد, but not including a matching سطر\n\
  {INTEGER}          repeat the پچھلا_2 نمونہ_2 specified نمبر کا times\n\
  {*}                repeat the پچھلا_2 نمونہ_2 as many times as possible\n\
\n\
A سطر انحراف_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}
