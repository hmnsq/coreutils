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
#وصف_ڪريو پروگرام_NAME "csplit"

#وصف_ڪريو AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#وصف_ڪريو DEFAULT_اڳياڙي	"xx"

/* A compiled pattern arg. */
بنية control
{
  intmax_t وٿي;		/* Offset from regexp to split at. */
  intmax_t سٽون_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  صحيح argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  بنية re_نمونو_بفر re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#وصف_ڪريو شروعات_سائيز	8191

/* Number of lines kept in each node in line list. */
#وصف_ڪريو CTRL_سائيز	80

#جيڪڏهن_وصف_ٿيل DEBUG
/* Some small values to test the algorithms. */
# define شروعات_سائيز	200
# define CTRL_سائيز	1
#جيڪڏهن_پورو

/* A string with a length count. */
بنية cstring
{
  idx_t len;
  حرف *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
بنية سٽ
{
  idx_t استعمال_ٿيل;			/* Number of offsets used in this struct. */
  idx_t وجهو_اشاريو;		/* Next offset to use when inserting line. */
  idx_t retrieve_اشاريو;	/* Next index to use when retrieving line. */
  بنية cstring starts[CTRL_سائيز]; /* Lines in the data area. */
  بنية سٽ *ايندڙ;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
بنية بفر_رڪارڊ
{
  idx_t بائيٽون_alloc;		/* Size of the buffer area. */
  idx_t بائيٽون_استعمال_ٿيل;		/* Bytes used in the buffer area. */
  intmax_t شروعات_سٽ;		/* First line number in this buffer. */
  intmax_t پهريون_موجود;	/* First line that can be retrieved. */
  idx_t انگ_سٽون;		/* Number of complete lines in this buffer. */
  حرف *بفر COUNTED_BY (بائيٽون_alloc);	/* Data area. */
  بنية سٽ *سٽ_شروعات;	/* Head of list of pointers to lines. */
  بنية سٽ *curr_سٽ;	/* The line start record currently in use. */
  بنية بفر_رڪارڊ *ايندڙ;
};

ساكن فراغ close_output_file (فراغ);
ساكن فراغ create_output_file (فراغ);
ساكن فراغ delete_all_files (bool);
ساكن فراغ save_سٽ_منزل_file (ثابت بنية cstring *سٽ);

/* Start of buffer list. */
ساكن بنية بفر_رڪارڊ *سر = عدم;

/* Partially read line. */
ساكن حرف *hold_area = عدم;

/* Number of bytes in 'hold_area'. */
ساكن idx_t hold_ڳڻپ = 0;

/* Number of the last line in the buffers. */
ساكن intmax_t آخري_سٽ_عدد = 0;

/* Number of the line currently being examined. */
ساكن intmax_t current_سٽ = 0;

/* If true, we have read EOF. */
ساكن bool have_read_eof = false;

/* Name of output files. */
ساكن حرف *متطاير filename_space = عدم;

/* Prefix part of output file names. */
ساكن حرف ثابت *متطاير اڳياڙي = DEFAULT_اڳياڙي;

/* Suffix part of output file names. */
ساكن حرف *متطاير suffix = عدم;

/* Number of digits to use in output file names. */
ساكن صحيح متطاير digits = 2;

/* Number of files created so far. */
ساكن صحيح متطاير files_created = 0;

/* Number of bytes written to current file. */
ساكن intmax_t بائيٽون_written;

/* Output file pointer. */
ساكن FILE *output_وهڪرو = عدم;

/* Output file name. */
ساكن حرف *output_filename = عدم;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ساكن حرف **global_argv;

/* If true, do not print the count of bytes in each output file. */
ساكن bool suppress_ڳڻپ;

/* If true, remove output files on error. */
ساكن bool متطاير remove_files = true;

/* If true, remove all output files which have a zero length. */
ساكن bool elide_خالي_جايون_files;

/* If true, suppress the lines that match the PATTERN */
ساكن bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ساكن بنية control *controls;

/* Number of elements in 'controls'. */
ساكن idx_t control_استعمال_ٿيل;

/* The set of signals that are caught.  */
ساكن sigset_t caught_سگنل;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"digits", required_دليل, عدم, 'n'},
  {"quiet", no_دليل, عدم, 'q'},  /* Deprecated.  */
  {"silent", no_دليل, عدم, 's'},
  {"keep-files", no_دليل, عدم, 'k'},
  {"elide-empty-files", no_دليل, عدم, 'z'},
  {"prefix", required_دليل, عدم, 'f'},
  {"suffix-format", required_دليل, عدم, 'b'},
  {"suppress-matched", no_دليل, عدم, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ساكن فراغ
cleanup (فراغ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_سگنل, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, عدم);
}

ساكن _Noreturn فراغ
cleanup_fatal (فراغ)
{
  cleanup ();
  ٻاهر_نڪرو (EXIT_FAILURE);
}

خارجي فراغ
xalloc_die (فراغ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ساكن فراغ
مداخلت_handler (صحيح sig)
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
save_منزل_hold_area (حرف *شروعات, idx_t انگ)
{
  حرر (hold_area);
  hold_area = شروعات;
  hold_ڳڻپ = انگ;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ساكن idx_t
read_داخلا (حرف *dest, idx_t max_n_بائيٽون)
{
  إذا (max_n_بائيٽون == 0)
    أرجع 0;

  ptrdiff_t بائيٽون_read = محفوظ_read (STDIN_FILENO, dest, max_n_بائيٽون);

  إذا (بائيٽون_read == 0)
    have_read_eof = true;

  إذا (بائيٽون_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  أرجع بائيٽون_read;
}

/* Initialize existing line record P. */

ساكن فراغ
clear_سٽ_control (بنية سٽ *p)
{
  p->استعمال_ٿيل = 0;
  p->وجهو_اشاريو = 0;
  p->retrieve_اشاريو = 0;
}

/* Return a new, initialized line record. */

ساكن بنية سٽ *
new_سٽ_control (فراغ)
{
  بنية سٽ *p = xmalloc (حجم *p);

  p->ايندڙ = عدم;
  clear_سٽ_control (p);

  أرجع p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ساكن فراغ
keep_new_سٽ (بنية بفر_رڪارڊ *b, حرف *سٽ_شروعات, idx_t سٽ_len)
{
  بنية سٽ *l;

  /* If there is no existing area to keep line info, get some. */
  إذا (b->سٽ_شروعات == عدم)
    b->سٽ_شروعات = b->curr_سٽ = new_سٽ_control ();

  /* If existing area for lines is full, get more. */
  إذا (b->curr_سٽ->استعمال_ٿيل == CTRL_سائيز)
    {
      b->curr_سٽ->ايندڙ = new_سٽ_control ();
      b->curr_سٽ = b->curr_سٽ->ايندڙ;
    }

  l = b->curr_سٽ;

  /* Record the start of the line, and update counters. */
  l->starts[l->وجهو_اشاريو].str = سٽ_شروعات;
  l->starts[l->وجهو_اشاريو].len = سٽ_len;
  l->استعمال_ٿيل++;
  l->وجهو_اشاريو++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ساكن idx_t
رڪارڊ_سٽ_starts (بنية بفر_رڪارڊ *b)
{
  حرف *سٽ_شروعات;		/* Start of current line. */
  idx_t سٽون;			/* Number of lines found. */
  idx_t سٽ_ڊيگهائي;		/* Length of each line found. */

  إذا (b->بائيٽون_استعمال_ٿيل == 0)
    أرجع 0;

  سٽون = 0;
  سٽ_شروعات = b->بفر;
  حرف *بفر_پڇاڙي = سٽ_شروعات + b->بائيٽون_استعمال_ٿيل;
  *بفر_پڇاڙي = '\n';

  طالما (true)
    {
      حرف *سٽ_پڇاڙي = rawmemchr (سٽ_شروعات, '\n');
      إذا (سٽ_پڇاڙي == بفر_پڇاڙي)
        اكسر;
      سٽ_ڊيگهائي = سٽ_پڇاڙي - سٽ_شروعات + 1;
      keep_new_سٽ (b, سٽ_شروعات, سٽ_ڊيگهائي);
      سٽ_شروعات = سٽ_پڇاڙي + 1;
      سٽون++;
    }

  /* Check for an incomplete last line. */
  idx_t بائيٽون_کاٻو = بفر_پڇاڙي - سٽ_شروعات;
  إذا (بائيٽون_کاٻو)
    {
      إذا (have_read_eof)
        {
          keep_new_سٽ (b, سٽ_شروعات, بائيٽون_کاٻو);
          سٽون++;
        }
      وإلا
        save_منزل_hold_area (ximemdup (سٽ_شروعات, بائيٽون_کاٻو), بائيٽون_کاٻو);
    }

  b->انگ_سٽون = سٽون;
  b->پهريون_موجود = b->شروعات_سٽ = آخري_سٽ_عدد + 1;
  آخري_سٽ_عدد += سٽون;

  أرجع سٽون;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#جيڪڏهن_پورو

ساكن فراغ
free_بفر (بنية بفر_رڪارڊ *buf)
{
  لكل (بنية سٽ *l = buf->سٽ_شروعات; l;)
    {
      بنية سٽ *n = l->ايندڙ;
      حرر (l);
      l = n;
    }
  حرر (buf->بفر);
  حرر (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ساكن ATTRIBUTE_DEALLOC (free_بفر, 1)
بنية بفر_رڪارڊ *
get_new_بفر (idx_t min_سائيز)
{
  بنية بفر_رڪارڊ *new_بفر = xmalloc (حجم *new_بفر);
  new_بفر->بائيٽون_alloc = 0;
  new_بفر->بفر = xpalloc (عدم, &new_بفر->بائيٽون_alloc, min_سائيز,
                                -1, 1);
  new_بفر->بائيٽون_استعمال_ٿيل = 0;
  new_بفر->شروعات_سٽ = new_بفر->پهريون_موجود = آخري_سٽ_عدد + 1;
  new_بفر->انگ_سٽون = 0;
  new_بفر->سٽ_شروعات = new_بفر->curr_سٽ = عدم;
  new_بفر->ايندڙ = عدم;

  أرجع new_بفر;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ساكن فراغ
save_بفر (بنية بفر_رڪارڊ *buf)
{
  buf->ايندڙ = عدم;
  buf->curr_سٽ = buf->سٽ_شروعات;

  إذا (سر == عدم)
    سر = buf;
  وإلا
    {
      بنية بفر_رڪارڊ *p;
      لكل (p = سر; p->ايندڙ; p = p->ايندڙ)
        /* Do nothing. */ ;
      p->ايندڙ = buf;
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
ڀريو_بفر (فراغ)
{
  إذا (have_read_eof)
    أرجع false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t بائيٽون_wanted = MAX (شروعات_سائيز, hold_ڳڻپ + 1);

  طالما (true)
    {
      بنية بفر_رڪارڊ *b = get_new_بفر (بائيٽون_wanted);
      idx_t بائيٽون_alloc = b->بائيٽون_alloc;
      idx_t بائيٽون_avail = بائيٽون_alloc;
      حرف *p = b->بفر;

      /* First check the 'holding' area for a partial line. */
      إذا (hold_ڳڻپ)
        {
          p = mempcpy (p, hold_area, hold_ڳڻپ);
          b->بائيٽون_استعمال_ٿيل += hold_ڳڻپ;
          بائيٽون_avail -= hold_ڳڻپ;
          hold_ڳڻپ = 0;
        }

      b->بائيٽون_استعمال_ٿيل += read_داخلا (p, بائيٽون_avail - 1);

      إذا (رڪارڊ_سٽ_starts (b) != 0)
        {
          save_بفر (b);
          أرجع true;
        }

      free_بفر (b);
      إذا (have_read_eof)
        أرجع false;
      إذا (ckd_جوڙيو (&بائيٽون_wanted, بائيٽون_alloc, بائيٽون_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ساكن intmax_t
get_پهريون_سٽ_اندريون_بفر (فراغ)
{
  إذا (سر == عدم && !ڀريو_بفر ())
    أرجع 0;

  أرجع سر->پهريون_موجود;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ساكن بنية cstring *
remove_سٽ (فراغ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ساكن بنية بفر_رڪارڊ *prev_buf = عدم;

  بنية cstring *سٽ;		/* Return value. */
  بنية سٽ *l;		/* For convenience. */

  إذا (prev_buf)
    {
      free_بفر (prev_buf);
      prev_buf = عدم;
    }

  إذا (سر == عدم && !ڀريو_بفر ())
    أرجع عدم;

  إذا (current_سٽ < سر->پهريون_موجود)
    current_سٽ = سر->پهريون_موجود;

  ++(سر->پهريون_موجود);

  l = سر->curr_سٽ;

  سٽ = &l->starts[l->retrieve_اشاريو];

  /* Advance index to next line. */
  إذا (++l->retrieve_اشاريو == l->استعمال_ٿيل)
    {
      /* Go on to the next line record. */
      سر->curr_سٽ = l->ايندڙ;
      إذا (سر->curr_سٽ == عدم || سر->curr_سٽ->استعمال_ٿيل == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = سر;
          سر = سر->ايندڙ;
        }
    }

  أرجع سٽ;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ساكن بنية cstring *
ڳوليو_سٽ (intmax_t linenum)
{
  إذا (سر == عدم && !ڀريو_بفر ())
    أرجع عدم;

  إذا (linenum < سر->شروعات_سٽ)
    أرجع عدم;

  لكل (بنية بفر_رڪارڊ *b = سر;;)
    {
      إذا (linenum < b->شروعات_سٽ + b->انگ_سٽون)
        {
          /* The line is in this buffer. */
          بنية سٽ *l;
          idx_t وٿي;	/* How far into the buffer the line is. */

          l = b->سٽ_شروعات;
          وٿي = linenum - b->شروعات_سٽ;
          /* Find the control record. */
          طالما (وٿي >= CTRL_سائيز)
            {
              l = l->ايندڙ;
              وٿي -= CTRL_سائيز;
            }
          أرجع &l->starts[وٿي];
        }
      إذا (b->ايندڙ == عدم && !ڀريو_بفر ())
        أرجع عدم;
      b = b->ايندڙ;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ساكن bool
no_وڌيڪ_سٽون (فراغ)
{
  أرجع ڳوليو_سٽ (current_سٽ + 1) == عدم;
}

/* Open NAME as standard input.  */

ساكن فراغ
سيٽ_داخلا_file (حرف ثابت *name)
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
write_منزل_file (intmax_t آخري_سٽ, bool ignore, صحيح argnum)
{
  بنية cstring *سٽ;
  intmax_t پهريون_سٽ;		/* First available input line. */
  intmax_t سٽون;		/* Number of lines to output. */

  پهريون_سٽ = get_پهريون_سٽ_اندريون_بفر ();

  إذا (! پهريون_سٽ || پهريون_سٽ > آخري_سٽ)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  سٽون = آخري_سٽ - پهريون_سٽ;

  لكل (intmax_t i = 0; i < سٽون; i++)
    {
      سٽ = remove_سٽ ();
      إذا (سٽ == عدم)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      إذا (!ignore)
        save_سٽ_منزل_file (سٽ);
    }
}

/* Output any lines left after all regexps have been processed. */

ساكن فراغ
dump_rest_جو_file (فراغ)
{
  بنية cstring *سٽ;

  طالما ((سٽ = remove_سٽ ()) != عدم)
    save_سٽ_منزل_file (سٽ);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ساكن فراغ
handle_سٽ_error (ثابت بنية control *p, intmax_t repetition)
{
  حرف buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           پروگرام_name, quote (imaxtostr (p->سٽون_required, buf)));
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
process_سٽ_ڳڻپ (ثابت بنية control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t آخري_سٽ_منزل_save = p->سٽون_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  إذا (no_وڌيڪ_سٽون () && suppress_matched)
    handle_سٽ_error (p, repetition);

  إذا (!(linenum = get_پهريون_سٽ_اندريون_بفر ()))
    handle_سٽ_error (p, repetition);

  طالما (linenum++ < آخري_سٽ_منزل_save)
    {
      بنية cstring *سٽ = remove_سٽ ();
      إذا (سٽ == عدم)
        handle_سٽ_error (p, repetition);
      save_سٽ_منزل_file (سٽ);
    }

  close_output_file ();

  إذا (suppress_matched)
    remove_سٽ ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  إذا (no_وڌيڪ_سٽون () && !suppress_matched)
    handle_سٽ_error (p, repetition);
}

ساكن فراغ
regexp_error (بنية control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           پروگرام_name, quote (global_argv[p->argnum]));

  إذا (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  وإلا
    fprintf (stderr, "\n");

  إذا (!ignore)
    {
      dump_rest_جو_file ();
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
  بنية cstring *سٽ;		/* From input file. */
  idx_t سٽ_len;		/* To make "$" in regexps work. */
  intmax_t break_سٽ;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  إذا (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  إذا (p->وٿي >= 0)
    {
      طالما (true)
        {
          سٽ = ڳوليو_سٽ (++current_سٽ);
          إذا (سٽ == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_جو_file ();
                      close_output_file ();
                    }
                  ٻاهر_نڪرو (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          سٽ_len = سٽ->len;
          إذا (سٽ->str[سٽ_len - 1] == '\n')
            سٽ_len--;
          ret = re_ڳولا (&p->re_compiled, سٽ->str, سٽ_len,
                           0, سٽ_len, عدم);
          إذا (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          إذا (ret == -1)
            {
              سٽ = remove_سٽ ();
              إذا (!ignore)
                save_سٽ_منزل_file (سٽ);
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
          سٽ = ڳوليو_سٽ (++current_سٽ);
          إذا (سٽ == عدم)
            {
              إذا (p->repeat_forever)
                {
                  إذا (!ignore)
                    {
                      dump_rest_جو_file ();
                      close_output_file ();
                    }
                  ٻاهر_نڪرو (EXIT_SUCCESS);
                }
              وإلا
                regexp_error (p, repetition, ignore);
            }
          سٽ_len = سٽ->len;
          إذا (سٽ->str[سٽ_len - 1] == '\n')
            سٽ_len--;
          ret = re_ڳولا (&p->re_compiled, سٽ->str, سٽ_len,
                           0, سٽ_len, عدم);
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
  break_سٽ = current_سٽ + p->وٿي;

  write_منزل_file (break_سٽ, ignore, p->argnum);

  إذا (!ignore)
    close_output_file ();

  إذا (p->وٿي > 0)
    current_سٽ = break_سٽ;

  إذا (suppress_matched)
    remove_سٽ ();
}

/* Split the input file according to the control records we have built. */

ساكن فراغ
ورهايو_file (فراغ)
{
  لكل (idx_t i = 0; i < control_استعمال_ٿيل; i++)
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
            process_سٽ_ڳڻپ (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_جو_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ساكن حرف *
make_filename (صحيح انگ)
{
  strcpy (filename_space, اڳياڙي);
  إذا (suffix)
    sprintf (filename_space + لڙي_ڊيگهه (اڳياڙي), suffix, انگ);
  وإلا
    sprintf (filename_space + لڙي_ڊيگهه (اڳياڙي), "%0*d", digits, انگ);
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

  إذا (nfiles == وڌ_کان_وڌ_عدد)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  وإلا
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_سگنل, &oldset);
      output_وهڪرو = fopen (output_filename, "w");
      fopen_ok = (output_وهڪرو != عدم);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, عدم);
    }

  إذا (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  بائيٽون_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ساكن فراغ
delete_all_files (bool اندريون_signal_handler)
{
  إذا (! remove_files)
    أرجع;

  لكل (صحيح i = files_created; 0 <= --i; )
    {
      حرف ثابت *name = make_filename (i);
      إذا (unlink (name) != 0 && errno != ENOENT && !اندريون_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ساكن فراغ
close_output_file (فراغ)
{
  إذا (output_وهڪرو)
    {
      إذا (ferror (output_وهڪرو))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_وهڪرو = عدم;
          cleanup_fatal ();
        }
      إذا (fclose (output_وهڪرو) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_وهڪرو = عدم;
          cleanup_fatal ();
        }
      إذا (بائيٽون_written == 0 && elide_خالي_جايون_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          صحيح unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_سگنل, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, عدم);

          إذا (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      وإلا
        {
          إذا (!suppress_ڳڻپ)
            fprintf (stdout, "%jd\n", بائيٽون_written);
        }
      output_وهڪرو = عدم;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ساكن فراغ
save_سٽ_منزل_file (ثابت بنية cstring *سٽ)
{
  idx_t l = fwrite (سٽ->str, حجم (حرف), سٽ->len, output_وهڪرو);
  إذا (l != سٽ->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_وهڪرو = عدم;
      cleanup_fatal ();
    }
  بائيٽون_written += سٽ->len;
}

/* Return a new, initialized control record. */

ساكن بنية control *
new_control_رڪارڊ (فراغ)
{
  ساكن idx_t control_allocated = 0; /* Total space allocated. */
  بنية control *p;

  إذا (control_استعمال_ٿيل == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, حجم *controls);
  p = &controls[control_استعمال_ٿيل++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->سٽون_required = 0;
  p->وٿي = 0;
  أرجع p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ساكن فراغ
check_for_وٿي (بنية control *p, حرف ثابت *str, حرف ثابت *انگ)
{
  إذا (xstrtoimax (انگ, عدم, 10, &p->وٿي, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ساكن فراغ
parse_repeat_ڳڻپ (صحيح argnum, بنية control *p, حرف *str)
{
  حرف *پڇاڙي;

  پڇاڙي = str + لڙي_ڊيگهه (str) - 1;
  إذا (*پڇاڙي != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *پڇاڙي = '\0';

  إذا (str + 1 == پڇاڙي - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  وإلا
    {
      uintmax_t val;
      إذا (xstrtoumax (str + 1, عدم, 10, &val, "") != LONGINT_OK
          || ckd_جوڙيو (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *پڇاڙي = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ساكن بنية control *
ڪڍو_regexp (صحيح argnum, bool ignore, حرف ثابت *str)
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
  p = new_control_رڪارڊ ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.بفر = عدم;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = عدم;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_خالي_جايون_RANGES;
  err = re_compile_نمونو (str + 1, len, &p->re_compiled);
  إذا (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  إذا (closing_delim[1])
    check_for_وٿي (p, str, closing_delim + 1);

  أرجع p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ساكن فراغ
parse_patterns (صحيح argc, صحيح شروعات, حرف **argv)
{
  بنية control *p;		/* New control record created. */
  ساكن intmax_t آخري_val = 0;

  لكل (صحيح i = شروعات; i < argc; i++)
    {
      إذا (*argv[i] == '/' || *argv[i] == '%')
        {
          p = ڪڍو_regexp (i, *argv[i] == '%', argv[i]);
        }
      وإلا
        {
          p = new_control_رڪارڊ ();
          p->argnum = i;

          uintmax_t val;
          إذا (xstrtoumax (argv[i], عدم, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          إذا (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          إذا (val < آخري_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), آخري_val);

          إذا (val == آخري_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          آخري_val = val;

          p->سٽون_required = val;
        }

      إذا (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ڳڻپ (i, p, argv[i]);
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

  لكل (idx_t ڳڻپ = 0; ; ڳڻپ++)
    {
      بدل (format[ڳڻپ])
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
          أرجع ڳڻپ;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ساكن فراغ
check_format_conv_type (حرف *format, صحيح flags)
{
  بنانشان حرف ch = *format;
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
max_ٻاهريون (حرف *format)
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

  صحيح maxlen = snprintf (عدم, 0, format, وڌ_کان_وڌ_عدد);
  إذا (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  أرجع maxlen;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح optc;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
        اڳياڙي = optarg;
        اكسر;

      حالة 'b':
        suffix = optarg;
        اكسر;

      حالة 'k':
        remove_files = false;
        اكسر;

      حالة 'n':
        digits = xdectoimax (optarg, 0, MIN (وڌ_کان_وڌ_عدد, IDX_MAX), "",
                             _("invalid number"), 0);
        اكسر;

      حالة 's':
      حالة 'q':
        suppress_ڳڻپ = true;
        اكسر;

      حالة 'z':
        elide_خالي_جايون_files = true;
        اكسر;

      حالة SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

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

  idx_t اڳياڙي_len = لڙي_ڊيگهه (اڳياڙي);
  idx_t max_digit_string_len
    = (suffix
       ? max_ٻاهريون (suffix)
       : MAX (INT_STRLEN_BOUND (صحيح), digits));
  idx_t filename_سائيز;
  إذا (ckd_جوڙيو (&filename_سائيز, اڳياڙي_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_سائيز);

  سيٽ_داخلا_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    تعداد { nsigs = countof (رڪن_sig) };

    بنية sigaction act;

    sigemptyset (&caught_سگنل);
    لكل (صحيح i = 0; i < nsigs; i++)
      {
        sigaction (رڪن_sig[i], عدم, &act);
        إذا (act.sa_handler != SIG_IGN)
          sigaddset (&caught_سگنل, رڪن_sig[i]);
      }

    act.sa_handler = مداخلت_handler;
    act.sa_mask = caught_سگنل;
    act.sa_flags = 0;

    لكل (صحيح i = 0; i < nsigs; i++)
      إذا (sigismember (&caught_سگنل, رڪن_sig[i]))
        sigaction (رڪن_sig[i], &act, عدم);
  }

  ورهايو_file ();

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
      لکو (_("\
Usage: %s [OPTION]... FILE نمونو...\n\
"),
              پروگرام_name);
      fputs (_("\
Output pieces جو FILE separated by نمونو(s) منزل files 'xx00', 'xx01', ...,\n\
and output byte counts جو each piece منزل standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard داخلا إذا FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead جو %02d\n\
"));
      oputs (_("\
  -f, --اڳياڙي=اڳياڙي_2\n\
         use اڳياڙي_2 instead جو 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         افعل not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the سٽون matching نمونو\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified عدد جو digits instead جو 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         افعل not print counts جو output file sizes\n\
"));
      oputs (_("\
  -z, --elide-خالي_جايون-files\n\
         suppress خالي_جايون output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each نمونو may be:\n\
  INTEGER            copy مٿي منزل but not including specified سٽ عدد\n\
  /REGEXP/[وٿي_2]   copy مٿي منزل but not including a matching سٽ\n\
  %REGEXP%[وٿي_2]   skip منزل, but not including a matching سٽ\n\
  {INTEGER}          repeat the اڳيون نمونو_2 specified عدد جو times\n\
  {*}                repeat the اڳيون نمونو_2 as many times as possible\n\
\n\
A سٽ وٿي_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}
