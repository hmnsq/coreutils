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
#تعریف_کن برنامه_NAME "csplit"

#تعریف_کن AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#تعریف_کن DEFAULT_پیشوند	"xx"

/* A compiled pattern arg. */
بنية control
{
  intmax_t جابه_جایی;		/* Offset from regexp to split at. */
  intmax_t سطرها_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحيح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  بنية re_الگو_میانگیر re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#تعریف_کن شروع_اندازه	8191

/* Number of lines kept in each node in line list. */
#تعریف_کن CTRL_اندازه	80

#اگر_تعریف_شده DEBUG
/* Some small values to test the algorithms. */
# define شروع_اندازه	200
# define CTRL_اندازه	1
#پایان_اگر

/* A string with a length count. */
بنية cstring
{
  idx_t len;
  حرف *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
بنية سطر
{
  idx_t استفاده_شده;			/* Number of offsets used in this struct. */
  idx_t درج_شاخص;		/* Next offset to use when inserting line. */
  idx_t retrieve_شاخص;	/* Next index to use when retrieving line. */
  بنية cstring starts[CTRL_اندازه]; /* Lines in the data area. */
  بنية سطر *بعدی;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
بنية میانگیر_رکورد
{
  idx_t بایتها_alloc;		/* Size of the buffer area. */
  idx_t بایتها_استفاده_شده;		/* Bytes used in the buffer area. */
  intmax_t شروع_سطر;		/* First line number in this buffer. */
  intmax_t اولی_موجود;	/* First line that can be retrieved. */
  idx_t رقم_سطرها;		/* Number of complete lines in this buffer. */
  حرف *میانگیر COUNTED_BY (بایتها_alloc);	/* Data area. */
  بنية سطر *سطر_شروع;	/* Head of list of pointers to lines. */
  بنية سطر *curr_سطر;	/* The line start record currently in use. */
  بنية میانگیر_رکورد *بعدی;
};

ساكن فراغ close_output_file (فراغ);
ساكن فراغ create_output_file (فراغ);
ساكن فراغ delete_all_files (bool);
ساكن فراغ save_سطر_مقصد_file (ثابت بنية cstring *سطر);

/* Start of buffer list. */
ساكن بنية میانگیر_رکورد *سر = عدم;

/* Partially read line. */
ساكن حرف *hold_area = عدم;

/* Number of bytes in 'hold_area'. */
ساكن idx_t hold_شمار = 0;

/* Number of the last line in the buffers. */
ساكن intmax_t آخرین_سطر_عدد = 0;

/* Number of the line currently being examined. */
ساكن intmax_t current_سطر = 0;

/* If true, we have read EOF. */
ساكن bool have_read_eof = false;

/* Name of output files. */
ساكن حرف *متطاير filename_space = عدم;

/* Prefix part of output file names. */
ساكن حرف ثابت *متطاير پیشوند = DEFAULT_پیشوند;

/* Suffix part of output file names. */
ساكن حرف *متطاير suffix = عدم;

/* Number of digits to use in output file names. */
ساكن صحيح متطاير digits = 2;

/* Number of files created so far. */
ساكن صحيح متطاير files_created = 0;

/* Number of bytes written to current file. */
ساكن intmax_t بایتها_written;

/* Output file pointer. */
ساكن FILE *output_جریان = عدم;

/* Output file name. */
ساكن حرف *output_filename = عدم;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ساكن حرف **global_argv;

/* If true, do not print the count of bytes in each output file. */
ساكن bool suppress_شمار;

/* If true, remove output files on error. */
ساكن bool متطاير remove_files = true;

/* If true, remove all output files which have a zero length. */
ساكن bool elide_خالی_files;

/* If true, suppress the lines that match the PATTERN */
ساكن bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ساكن بنية control *controls;

/* Number of elements in 'controls'. */
ساكن idx_t control_استفاده_شده;

/* The set of signals that are caught.  */
ساكن sigset_t caught_سیگنالها;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"digits", required_آرگومان, عدم, 'n'},
  {"quiet", no_آرگومان, عدم, 'q'},  /* Deprecated.  */
  {"silent", no_آرگومان, عدم, 's'},
  {"keep-files", no_آرگومان, عدم, 'k'},
  {"elide-empty-files", no_آرگومان, عدم, 'z'},
  {"prefix", required_آرگومان, عدم, 'f'},
  {"suffix-format", required_آرگومان, عدم, 'b'},
  {"suppress-matched", no_آرگومان, عدم, SUPPRESS_MATCHED_OPTION},
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

  sigprocmask (SIG_BLOCK, &caught_سیگنالها, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, عدم);
}

ساكن _Noreturn فراغ
cleanup_fatal (فراغ)
{
  cleanup ();
  خروج (EXIT_FAILURE);
}

خارجي فراغ
xalloc_die (فراغ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ساكن فراغ
وقفه_handler (صحيح sig)
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
save_مقصد_hold_area (حرف *شروع, idx_t رقم)
{
  حرر (hold_area);
  hold_area = شروع;
  hold_شمار = رقم;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ساكن idx_t
read_درون_داد (حرف *dest, idx_t max_n_بایتها)
{
  إذا (max_n_بایتها == 0)
    أرجع 0;

  ptrdiff_t بایتها_read = امن_read (STDIN_FILENO, dest, max_n_بایتها);

  إذا (بایتها_read == 0)
    have_read_eof = true;

  إذا (بایتها_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع بایتها_read;
}

/* Initialize existing line record P. */

ساكن فراغ
clear_سطر_control (بنية سطر *p)
{
  p->استفاده_شده = 0;
  p->درج_شاخص = 0;
  p->retrieve_شاخص = 0;
}

/* Return a new, initialized line record. */

ساكن بنية سطر *
new_سطر_control (فراغ)
{
  بنية سطر *p = xmalloc (حجم *p);

  p->بعدی = عدم;
  clear_سطر_control (p);

  أرجع p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ساكن فراغ
keep_new_سطر (بنية میانگیر_رکورد *b, حرف *سطر_شروع, idx_t سطر_len)
{
  بنية سطر *l;

  /* If there is no existing area to keep line info, get some. */
  إذا (b->سطر_شروع == عدم)
    b->سطر_شروع = b->curr_سطر = new_سطر_control ();

  /* If existing area for lines is full, get more. */
  إذا (b->curr_سطر->استفاده_شده == CTRL_اندازه)
    {
      b->curr_سطر->بعدی = new_سطر_control ();
      b->curr_سطر = b->curr_سطر->بعدی;
    }

  l = b->curr_سطر;

  /* Record the start of the line, and update counters. */
  l->starts[l->درج_شاخص].str = سطر_شروع;
  l->starts[l->درج_شاخص].len = سطر_len;
  l->استفاده_شده++;
  l->درج_شاخص++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ساكن idx_t
رکورد_سطر_starts (بنية میانگیر_رکورد *b)
{
  حرف *سطر_شروع;		/* Start of current line. */
  idx_t سطرها;			/* Number of lines found. */
  idx_t سطر_درازی;		/* Length of each line found. */

  إذا (b->بایتها_استفاده_شده == 0)
    أرجع 0;

  سطرها = 0;
  سطر_شروع = b->میانگیر;
  حرف *میانگیر_انتها = سطر_شروع + b->بایتها_استفاده_شده;
  *میانگیر_انتها = '\n';

  طالما (true)
    {
      حرف *سطر_انتها = rawmemchr (سطر_شروع, '\n');
      إذا (سطر_انتها == میانگیر_انتها)
        اكسر;
      سطر_درازی = سطر_انتها - سطر_شروع + 1;
      keep_new_سطر (b, سطر_شروع, سطر_درازی);
      سطر_شروع = سطر_انتها + 1;
      سطرها++;
    }

  /* Check for an incomplete last line. */
  idx_t بایتها_چپ = میانگیر_انتها - سطر_شروع;
  إذا (بایتها_چپ)
    {
      إذا (have_read_eof)
        {
          keep_new_سطر (b, سطر_شروع, بایتها_چپ);
          سطرها++;
        }
      وإلا
        save_مقصد_hold_area (ximemdup (سطر_شروع, بایتها_چپ), بایتها_چپ);
    }

  b->رقم_سطرها = سطرها;
  b->اولی_موجود = b->شروع_سطر = آخرین_سطر_عدد + 1;
  آخرین_سطر_عدد += سطرها;

  أرجع سطرها;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#پایان_اگر

ساكن فراغ
free_میانگیر (بنية میانگیر_رکورد *buf)
{
  لكل (بنية سطر *l = buf->سطر_شروع; l;)
    {
      بنية سطر *n = l->بعدی;
      حرر (l);
      l = n;
    }
  حرر (buf->میانگیر);
  حرر (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ساكن ATTRIBUTE_DEALLOC (free_میانگیر, 1)
بنية میانگیر_رکورد *
get_new_میانگیر (idx_t min_اندازه)
{
  بنية میانگیر_رکورد *new_میانگیر = xmalloc (حجم *new_میانگیر);
  new_میانگیر->بایتها_alloc = 0;
  new_میانگیر->میانگیر = xpalloc (عدم, &new_میانگیر->بایتها_alloc, min_اندازه,
                                -1, 1);
  new_میانگیر->بایتها_استفاده_شده = 0;
  new_میانگیر->شروع_سطر = new_میانگیر->اولی_موجود = آخرین_سطر_عدد + 1;
  new_میانگیر->رقم_سطرها = 0;
  new_میانگیر->سطر_شروع = new_میانگیر->curr_سطر = عدم;
  new_میانگیر->بعدی = عدم;

  أرجع new_میانگیر;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ساكن فراغ
save_میانگیر (بنية میانگیر_رکورد *buf)
{
  buf->بعدی = عدم;
  buf->curr_سطر = buf->سطر_شروع;

  إذا (سر == عدم)
    سر = buf;
  وإلا
    {
      بنية میانگیر_رکورد *p;
      لكل (p = سر; p->بعدی; p = p->بعدی)
        /* Do nothing. */ ;
      p->بعدی = buf;
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
بارگیری_میانگیر (فراغ)
{
  إذا (have_read_eof)
    أرجع false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t بایتها_wanted = MAX (شروع_اندازه, hold_شمار + 1);

  طالما (true)
    {
      بنية میانگیر_رکورد *b = get_new_میانگیر (بایتها_wanted);
      idx_t بایتها_alloc = b->بایتها_alloc;
      idx_t بایتها_avail = بایتها_alloc;
      حرف *p = b->میانگیر;

      /* First check the 'holding' area for a partial line. */
      إذا (hold_شمار)
        {
          p = mempcpy (p, hold_area, hold_شمار);
          b->بایتها_استفاده_شده += hold_شمار;
          بایتها_avail -= hold_شمار;
          hold_شمار = 0;
        }

      b->بایتها_استفاده_شده += read_درون_داد (p, بایتها_avail - 1);

      إذا (رکورد_سطر_starts (b) != 0)
        {
          save_میانگیر (b);
          أرجع true;
        }

      free_میانگیر (b);
      إذا (have_read_eof)
        أرجع false;
      إذا (ckd_جمع_کن (&بایتها_wanted, بایتها_alloc, بایتها_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ساكن intmax_t
get_اولی_سطر_درون_میانگیر (فراغ)
{
  إذا (سر == عدم && !بارگیری_میانگیر ())
    أرجع 0;

  أرجع سر->اولی_موجود;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ساكن بنية cstring *
remove_سطر (فراغ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ساكن بنية میانگیر_رکورد *prev_buf = عدم;

  بنية cstring *سطر;		/* Return value. */
  بنية سطر *l;		/* For convenience. */

  إذا (prev_buf)
    {
      free_میانگیر (prev_buf);
      prev_buf = عدم;
    }

  إذا (سر == عدم && !بارگیری_میانگیر ())
    أرجع عدم;

  إذا (current_سطر < سر->اولی_موجود)
    current_سطر = سر->اولی_موجود;

  ++(سر->اولی_موجود);

  l = سر->curr_سطر;

  سطر = &l->starts[l->retrieve_شاخص];

  /* Advance index to next line. */
  إذا (++l->retrieve_شاخص == l->استفاده_شده)
    {
      /* Go on to the next line record. */
      سر->curr_سطر = l->بعدی;
      إذا (سر->curr_سطر == عدم || سر->curr_سطر->استفاده_شده == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = سر;
          سر = سر->بعدی;
        }
    }

  أرجع سطر;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ساكن بنية cstring *
پیدا_کن_سطر (intmax_t linenum)
{
  إذا (سر == عدم && !بارگیری_میانگیر ())
    أرجع عدم;

  إذا (linenum < سر->شروع_سطر)
    أرجع عدم;

  لكل (بنية میانگیر_رکورد *b = سر;;)
    {
      إذا (linenum < b->شروع_سطر + b->رقم_سطرها)
        {
          /* The line is in this buffer. */
          بنية سطر *l;
          idx_t جابه_جایی;	/* How far into the buffer the line is. */

          l = b->سطر_شروع;
          جابه_جایی = linenum - b->شروع_سطر;
          /* Find the control record. */
          طالما (جابه_جایی >= CTRL_اندازه)
            {
              l = l->بعدی;
              جابه_جایی -= CTRL_اندازه;
            }
          أرجع &l->starts[جابه_جایی];
        }
      إذا (b->بعدی == عدم && !بارگیری_میانگیر ())
        أرجع عدم;
      b = b->بعدی;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ساكن bool
no_بیشتر_سطرها (فراغ)
{
  أرجع پیدا_کن_سطر (current_سطر + 1) == عدم;
}

/* Open NAME as standard input.  */

ساكن فراغ
مجموعه_درون_داد_file (حرف ثابت *name)
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
write_مقصد_file (intmax_t آخرین_سطر, bool ignore, صحيح argnum)
{
  بنية cstring *سطر;
  intmax_t اولی_سطر;		/* First available input line. */
  intmax_t سطرها;		/* Number of lines to output. */

  اولی_سطر = get_اولی_سطر_درون_میانگیر ();

  إذا (! اولی_سطر || اولی_سطر > آخرین_سطر)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  سطرها = آخرین_سطر - اولی_سطر;

  لكل (intmax_t i = 0; i < سطرها; i++)
    {
      سطر = remove_سطر ();
      إذا (سطر == عدم)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      إذا (!ignore)
        save_سطر_مقصد_file (سطر);
    }
}

/* Output any lines left after all regexps have been processed. */

ساكن فراغ
dump_rest_از_file (فراغ)
{
  بنية cstring *سطر;

  طالما ((سطر = remove_سطر ()) != عدم)
    save_سطر_مقصد_file (سطر);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ساكن فراغ
handle_سطر_error (ثابت بنية control *p, intmax_t repetition)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           برنامه_name, quote (imaxtostr (p->سطرها_required, buf)));
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
process_سطر_شمار (ثابت بنية control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t آخرین_سطر_مقصد_save = p->سطرها_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  إذا (no_بیشتر_سطرها () && suppress_matched)
    handle_سطر_error (p, repetition);

  إذا (!(linenum = get_اولی_سطر_درون_میانگیر ()))
    handle_سطر_error (p, repetition);

  طالما (linenum++ < آخرین_سطر_مقصد_save)
    {
      بنية cstring *سطر = remove_سطر ();
      إذا (سطر == عدم)
        handle_سطر_error (p, repetition);
      save_سطر_مقصد_file (سطر);
    }

  close_output_file ();

  إذا (suppress_matched)
    remove_سطر ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  إذا (no_بیشتر_سطرها () && !suppress_matched)
    handle_سطر_error (p, repetition);
}

ساكن فراغ
regexp_error (بنية control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           برنامه_name, quote (global_argv[p->argnum]));

  إذا (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وإلا
    fprintf (stderr, "\n");

  إذا (!ignore)
    {
      dump_rest_از_file ();
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
  بنية cstring *سطر;		/* From input file. */
  idx_t سطر_len;		/* To make "$" in regexps work. */
  intmax_t break_سطر;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  إذا (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  إذا (p->جابه_جایی >= 0)
    {
      طالما (true)
        {
          سطر = پیدا_کن_سطر (++current_سطر);
          إذا (سطر == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_از_file ();
                      close_output_file ();
                    }
                  خروج (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          إذا (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_جستجو (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, عدم);
          إذا (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          إذا (ret == -1)
            {
              سطر = remove_سطر ();
              إذا (!ignore)
                save_سطر_مقصد_file (سطر);
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
          سطر = پیدا_کن_سطر (++current_سطر);
          إذا (سطر == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_از_file ();
                      close_output_file ();
                    }
                  خروج (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          سطر_len = سطر->len;
          إذا (سطر->str[سطر_len - 1] == '\n')
            سطر_len--;
          ret = re_جستجو (&p->re_compiled, سطر->str, سطر_len,
                           0, سطر_len, عدم);
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
  break_سطر = current_سطر + p->جابه_جایی;

  write_مقصد_file (break_سطر, ignore, p->argnum);

  إذا (!ignore)
    close_output_file ();

  إذا (p->جابه_جایی > 0)
    current_سطر = break_سطر;

  إذا (suppress_matched)
    remove_سطر ();
}

/* Split the input file according to the control records we have built. */

ساكن فراغ
تقسیم_file (فراغ)
{
  لكل (idx_t i = 0; i < control_استفاده_شده; i++)
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
            process_سطر_شمار (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_از_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ساكن حرف *
make_filename (صحيح رقم)
{
  strcpy (filename_space, پیشوند);
  إذا (suffix)
    sprintf (filename_space + طول_رشته (پیشوند), suffix, رقم);
  وإلا
    sprintf (filename_space + طول_رشته (پیشوند), "%0*d", digits, رقم);
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

  إذا (nfiles == بیشترین_صحیح)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  وإلا
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_سیگنالها, &oldset);
      output_جریان = fopen (output_filename, "w");
      fopen_ok = (output_جریان != عدم);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, عدم);
    }

  إذا (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  بایتها_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ساكن فراغ
delete_all_files (bool درون_signal_handler)
{
  إذا (! remove_files)
    أرجع;

  لكل (صحيح i = files_created; 0 <= --i; )
    {
      حرف ثابت *name = make_filename (i);
      إذا (unlink (name) != 0 && errno != ENOENT && !درون_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ساكن فراغ
close_output_file (فراغ)
{
  إذا (output_جریان)
    {
      إذا (ferror (output_جریان))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_جریان = عدم;
          cleanup_fatal ();
        }
      إذا (fclose (output_جریان) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_جریان = عدم;
          cleanup_fatal ();
        }
      إذا (بایتها_written == 0 && elide_خالی_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحيح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_سیگنالها, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, عدم);

          إذا (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      وإلا
        {
          إذا (!suppress_شمار)
            fprintf (stdout, "%jd\n", بایتها_written);
        }
      output_جریان = عدم;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ساكن فراغ
save_سطر_مقصد_file (ثابت بنية cstring *سطر)
{
  idx_t l = fwrite (سطر->str, حجم (حرف), سطر->len, output_جریان);
  إذا (l != سطر->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_جریان = عدم;
      cleanup_fatal ();
    }
  بایتها_written += سطر->len;
}

/* Return a new, initialized control record. */

ساكن بنية control *
new_control_رکورد (فراغ)
{
  ساكن idx_t control_allocated = 0; /* Total space allocated. */
  بنية control *p;

  إذا (control_استفاده_شده == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, حجم *controls);
  p = &controls[control_استفاده_شده++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->سطرها_required = 0;
  p->جابه_جایی = 0;
  أرجع p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ساكن فراغ
check_for_جابه_جایی (بنية control *p, حرف ثابت *str, حرف ثابت *رقم)
{
  إذا (xstrtoimax (رقم, عدم, 10, &p->جابه_جایی, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ساكن فراغ
parse_repeat_شمار (صحيح argnum, بنية control *p, حرف *str)
{
  حرف *انتها;

  انتها = str + طول_رشته (str) - 1;
  إذا (*انتها != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *انتها = '\0';

  إذا (str + 1 == انتها - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  وإلا
    {
      uintmax_t val;
      إذا (xstrtoumax (str + 1, عدم, 10, &val, "") != LONGINT_OK
          || ckd_جمع_کن (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *انتها = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ساكن بنية control *
استخراج_regexp (صحيح argnum, bool ignore, حرف ثابت *str)
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
  p = new_control_رکورد ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.میانگیر = عدم;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = عدم;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالی_RANGES;
  err = re_compile_الگو (str + 1, len, &p->re_compiled);
  إذا (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  إذا (closing_delim[1])
    check_for_جابه_جایی (p, str, closing_delim + 1);

  أرجع p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ساكن فراغ
parse_patterns (صحيح argc, صحيح شروع, حرف **argv)
{
  بنية control *p;		/* New control record created. */
  ساكن intmax_t آخرین_val = 0;

  لكل (صحيح i = شروع; i < argc; i++)
    {
      إذا (*argv[i] == '/' || *argv[i] == '%')
        {
          p = استخراج_regexp (i, *argv[i] == '%', argv[i]);
        }
      وإلا
        {
          p = new_control_رکورد ();
          p->argnum = i;

          uintmax_t val;
          إذا (xstrtoumax (argv[i], عدم, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          إذا (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          إذا (val < آخرین_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), آخرین_val);

          إذا (val == آخرین_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          آخرین_val = val;

          p->سطرها_required = val;
        }

      إذا (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_شمار (i, p, argv[i]);
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

  لكل (idx_t شمار = 0; ; شمار++)
    {
      بدل (format[شمار])
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
          أرجع شمار;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ساكن فراغ
check_format_conv_type (حرف *format, صحيح flags)
{
  بیعلامت حرف ch = *format;
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
max_بیرون (حرف *format)
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

  صحيح maxlen = snprintf (عدم, 0, format, بیشترین_صحیح);
  إذا (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  أرجع maxlen;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح optc;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
        پیشوند = optarg;
        اكسر;

      حالة 'b':
        suffix = optarg;
        اكسر;

      حالة 'k':
        remove_files = false;
        اكسر;

      حالة 'n':
        digits = xdectoimax (optarg, 0, MIN (بیشترین_صحیح, IDX_MAX), "",
                             _("invalid number"), 0);
        اكسر;

      حالة 's':
      حالة 'q':
        suppress_شمار = true;
        اكسر;

      حالة 'z':
        elide_خالی_files = true;
        اكسر;

      حالة SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

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

  idx_t پیشوند_len = طول_رشته (پیشوند);
  idx_t max_digit_string_len
    = (suffix
       ? max_بیرون (suffix)
       : MAX (INT_STRLEN_BOUND (صحيح), digits));
  idx_t filename_اندازه;
  إذا (ckd_جمع_کن (&filename_اندازه, پیشوند_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_اندازه);

  مجموعه_درون_داد_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    تعداد { nsigs = countof (جمله_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سیگنالها);
    لكل (صحيح i = 0; i < nsigs; i++)
      {
        sigaction (جمله_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سیگنالها, جمله_sig[i]);
      }

    act.sa_handler = وقفه_handler;
    act.sa_mask = caught_سیگنالها;
    act.sa_flags = 0;

    لكل (صحيح i = 0; i < nsigs; i++)
      إذا (sigismember (&caught_سیگنالها, جمله_sig[i]))
        sigaction (جمله_sig[i], &act, عدم);
  }

  تقسیم_file ();

  إذا (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع EXIT_SUCCESS;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... FILE الگو...\n\
"),
              برنامه_name);
      fputs (_("\
Output pieces از FILE separated by الگو(s) مقصد files 'xx00', 'xx01', ...,\n\
and output byte counts از each piece مقصد standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard درون_داد إذا FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead از %02d\n\
"));
      oputs (_("\
  -f, --پیشوند=پیشوند_2\n\
         use پیشوند_2 instead از 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         افعل not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the سطرها matching الگو\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified عدد از digits instead از 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         افعل not print counts از output file sizes\n\
"));
      oputs (_("\
  -z, --elide-خالی-files\n\
         suppress خالی output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each الگو may be:\n\
  INTEGER            copy به_بالا مقصد but not including specified سطر عدد\n\
  /REGEXP/[جابه_جایی_2]   copy به_بالا مقصد but not including a matching سطر\n\
  %REGEXP%[جابه_جایی_2]   skip مقصد, but not including a matching سطر\n\
  {INTEGER}          repeat the قبلی الگو_2 specified عدد از times\n\
  {*}                repeat the قبلی الگو_2 as many times as possible\n\
\n\
A سطر جابه_جایی_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}
