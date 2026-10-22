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

#అన్తర్భావ <config.h>

#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ <regex.h>

#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "safe-read.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "term-sig.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "csplit"

#నిర్వచించు AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#నిర్వచించు DEFAULT_ఉపసర్గ	"xx"

/* A compiled pattern arg. */
నిర్మాణం control
{
  intmax_t స్థానభ్రంశం;		/* Offset from regexp to split at. */
  intmax_t పంక్తులు_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  పూర్ణాంకం argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  నిర్మాణం re_నమూనా_బఫరు re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#నిర్వచించు ప్రారంభం_కొలత	8191

/* Number of lines kept in each node in line list. */
#నిర్వచించు CTRL_కొలత	80

#నిర్వచితమైతే DEBUG
/* Some small values to test the algorithms. */
# define ప్రారంభం_కొలత	200
# define CTRL_కొలత	1
#అయితే_ముగింపు

/* A string with a length count. */
నిర్మాణం cstring
{
  idx_t len;
  అక్షరం *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
నిర్మాణం పంక్తి
{
  idx_t వాడిన;			/* Number of offsets used in this struct. */
  idx_t చొప్పించు_సూచీ;		/* Next offset to use when inserting line. */
  idx_t retrieve_సూచీ;	/* Next index to use when retrieving line. */
  నిర్మాణం cstring starts[CTRL_కొలత]; /* Lines in the data area. */
  నిర్మాణం పంక్తి *తదుపరి;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
నిర్మాణం బఫరు_రికార్డు
{
  idx_t బైట్లు_alloc;		/* Size of the buffer area. */
  idx_t బైట్లు_వాడిన;		/* Bytes used in the buffer area. */
  intmax_t ప్రారంభం_పంక్తి;		/* First line number in this buffer. */
  intmax_t మొదటి_అందుబాటు;	/* First line that can be retrieved. */
  idx_t అంకె_పంక్తులు;		/* Number of complete lines in this buffer. */
  అక్షరం *బఫరు COUNTED_BY (బైట్లు_alloc);	/* Data area. */
  నిర్మాణం పంక్తి *పంక్తి_ప్రారంభం;	/* Head of list of pointers to lines. */
  నిర్మాణం పంక్తి *curr_పంక్తి;	/* The line start record currently in use. */
  నిర్మాణం బఫరు_రికార్డు *తదుపరి;
};

స్థిర శూన్య close_output_file (శూన్య);
స్థిర శూన్య create_output_file (శూన్య);
స్థిర శూన్య delete_all_files (bool);
స్థిర శూన్య save_పంక్తి_గమ్యం_file (స్థిరరాశి నిర్మాణం cstring *పంక్తి);

/* Start of buffer list. */
స్థిర నిర్మాణం బఫరు_రికార్డు *తల = ఖాళీ;

/* Partially read line. */
స్థిర అక్షరం *hold_area = ఖాళీ;

/* Number of bytes in 'hold_area'. */
స్థిర idx_t hold_లెక్క = 0;

/* Number of the last line in the buffers. */
స్థిర intmax_t చివరి_పంక్తి_సంఖ్య = 0;

/* Number of the line currently being examined. */
స్థిర intmax_t current_పంక్తి = 0;

/* If true, we have read EOF. */
స్థిర bool have_read_eof = false;

/* Name of output files. */
స్థిర అక్షరం *అస్థిర filename_space = ఖాళీ;

/* Prefix part of output file names. */
స్థిర అక్షరం స్థిరరాశి *అస్థిర ఉపసర్గ = DEFAULT_ఉపసర్గ;

/* Suffix part of output file names. */
స్థిర అక్షరం *అస్థిర suffix = ఖాళీ;

/* Number of digits to use in output file names. */
స్థిర పూర్ణాంకం అస్థిర digits = 2;

/* Number of files created so far. */
స్థిర పూర్ణాంకం అస్థిర files_created = 0;

/* Number of bytes written to current file. */
స్థిర intmax_t బైట్లు_written;

/* Output file pointer. */
స్థిర FILE *output_ప్రవాహం = ఖాళీ;

/* Output file name. */
స్థిర అక్షరం *output_filename = ఖాళీ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
స్థిర అక్షరం **global_argv;

/* If true, do not print the count of bytes in each output file. */
స్థిర bool suppress_లెక్క;

/* If true, remove output files on error. */
స్థిర bool అస్థిర remove_files = true;

/* If true, remove all output files which have a zero length. */
స్థిర bool elide_ఖాళీలు_files;

/* If true, suppress the lines that match the PATTERN */
స్థిర bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
స్థిర నిర్మాణం control *controls;

/* Number of elements in 'controls'. */
స్థిర idx_t control_వాడిన;

/* The set of signals that are caught.  */
స్థిర sigset_t caught_సంకేతాలు;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"digits", required_వాదం, ఖాళీ, 'n'},
  {"quiet", no_వాదం, ఖాళీ, 'q'},  /* Deprecated.  */
  {"silent", no_వాదం, ఖాళీ, 's'},
  {"keep-files", no_వాదం, ఖాళీ, 'k'},
  {"elide-empty-files", no_వాదం, ఖాళీ, 'z'},
  {"prefix", required_వాదం, ఖాళీ, 'f'},
  {"suffix-format", required_వాదం, ఖాళీ, 'b'},
  {"suppress-matched", no_వాదం, ఖాళీ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

స్థిర శూన్య
cleanup (శూన్య)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_సంకేతాలు, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ఖాళీ);
}

స్థిర _Noreturn శూన్య
cleanup_fatal (శూన్య)
{
  cleanup ();
  నిష్క్రమించు (EXIT_FAILURE);
}

బాహ్య శూన్య
xalloc_die (శూన్య)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

స్థిర శూన్య
అంతరాయం_handler (పూర్ణాంకం sig)
{
  delete_all_files (true);
  సంకేతం (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  లేపు (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

స్థిర శూన్య
save_గమ్యం_hold_area (అక్షరం *ప్రారంభం, idx_t అంకె)
{
  విడుదల (hold_area);
  hold_area = ప్రారంభం;
  hold_లెక్క = అంకె;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

స్థిర idx_t
read_ఆదానం (అక్షరం *dest, idx_t max_n_బైట్లు)
{
  ఒకవేళ (max_n_బైట్లు == 0)
    తిరిగి 0;

  ptrdiff_t బైట్లు_read = సురక్షిత_read (STDIN_FILENO, dest, max_n_బైట్లు);

  ఒకవేళ (బైట్లు_read == 0)
    have_read_eof = true;

  ఒకవేళ (బైట్లు_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  తిరిగి బైట్లు_read;
}

/* Initialize existing line record P. */

స్థిర శూన్య
clear_పంక్తి_control (నిర్మాణం పంక్తి *p)
{
  p->వాడిన = 0;
  p->చొప్పించు_సూచీ = 0;
  p->retrieve_సూచీ = 0;
}

/* Return a new, initialized line record. */

స్థిర నిర్మాణం పంక్తి *
new_పంక్తి_control (శూన్య)
{
  నిర్మాణం పంక్తి *p = xmalloc (పరిమాణం *p);

  p->తదుపరి = ఖాళీ;
  clear_పంక్తి_control (p);

  తిరిగి p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

స్థిర శూన్య
keep_new_పంక్తి (నిర్మాణం బఫరు_రికార్డు *b, అక్షరం *పంక్తి_ప్రారంభం, idx_t పంక్తి_len)
{
  నిర్మాణం పంక్తి *l;

  /* If there is no existing area to keep line info, get some. */
  ఒకవేళ (b->పంక్తి_ప్రారంభం == ఖాళీ)
    b->పంక్తి_ప్రారంభం = b->curr_పంక్తి = new_పంక్తి_control ();

  /* If existing area for lines is full, get more. */
  ఒకవేళ (b->curr_పంక్తి->వాడిన == CTRL_కొలత)
    {
      b->curr_పంక్తి->తదుపరి = new_పంక్తి_control ();
      b->curr_పంక్తి = b->curr_పంక్తి->తదుపరి;
    }

  l = b->curr_పంక్తి;

  /* Record the start of the line, and update counters. */
  l->starts[l->చొప్పించు_సూచీ].str = పంక్తి_ప్రారంభం;
  l->starts[l->చొప్పించు_సూచీ].len = పంక్తి_len;
  l->వాడిన++;
  l->చొప్పించు_సూచీ++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

స్థిర idx_t
రికార్డు_పంక్తి_starts (నిర్మాణం బఫరు_రికార్డు *b)
{
  అక్షరం *పంక్తి_ప్రారంభం;		/* Start of current line. */
  idx_t పంక్తులు;			/* Number of lines found. */
  idx_t పంక్తి_నిడివి;		/* Length of each line found. */

  ఒకవేళ (b->బైట్లు_వాడిన == 0)
    తిరిగి 0;

  పంక్తులు = 0;
  పంక్తి_ప్రారంభం = b->బఫరు;
  అక్షరం *బఫరు_చివర = పంక్తి_ప్రారంభం + b->బైట్లు_వాడిన;
  *బఫరు_చివర = '\n';

  అయితే (true)
    {
      అక్షరం *పంక్తి_చివర = rawmemchr (పంక్తి_ప్రారంభం, '\n');
      ఒకవేళ (పంక్తి_చివర == బఫరు_చివర)
        ఆపు;
      పంక్తి_నిడివి = పంక్తి_చివర - పంక్తి_ప్రారంభం + 1;
      keep_new_పంక్తి (b, పంక్తి_ప్రారంభం, పంక్తి_నిడివి);
      పంక్తి_ప్రారంభం = పంక్తి_చివర + 1;
      పంక్తులు++;
    }

  /* Check for an incomplete last line. */
  idx_t బైట్లు_ఎడమ = బఫరు_చివర - పంక్తి_ప్రారంభం;
  ఒకవేళ (బైట్లు_ఎడమ)
    {
      ఒకవేళ (have_read_eof)
        {
          keep_new_పంక్తి (b, పంక్తి_ప్రారంభం, బైట్లు_ఎడమ);
          పంక్తులు++;
        }
      లేకపోతే
        save_గమ్యం_hold_area (ximemdup (పంక్తి_ప్రారంభం, బైట్లు_ఎడమ), బైట్లు_ఎడమ);
    }

  b->అంకె_పంక్తులు = పంక్తులు;
  b->మొదటి_అందుబాటు = b->ప్రారంభం_పంక్తి = చివరి_పంక్తి_సంఖ్య + 1;
  చివరి_పంక్తి_సంఖ్య += పంక్తులు;

  తిరిగి పంక్తులు;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ఒకవేళ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#అయితే_ముగింపు

స్థిర శూన్య
free_బఫరు (నిర్మాణం బఫరు_రికార్డు *buf)
{
  కొరకు (నిర్మాణం పంక్తి *l = buf->పంక్తి_ప్రారంభం; l;)
    {
      నిర్మాణం పంక్తి *n = l->తదుపరి;
      విడుదల (l);
      l = n;
    }
  విడుదల (buf->బఫరు);
  విడుదల (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

స్థిర ATTRIBUTE_DEALLOC (free_బఫరు, 1)
నిర్మాణం బఫరు_రికార్డు *
get_new_బఫరు (idx_t min_కొలత)
{
  నిర్మాణం బఫరు_రికార్డు *new_బఫరు = xmalloc (పరిమాణం *new_బఫరు);
  new_బఫరు->బైట్లు_alloc = 0;
  new_బఫరు->బఫరు = xpalloc (ఖాళీ, &new_బఫరు->బైట్లు_alloc, min_కొలత,
                                -1, 1);
  new_బఫరు->బైట్లు_వాడిన = 0;
  new_బఫరు->ప్రారంభం_పంక్తి = new_బఫరు->మొదటి_అందుబాటు = చివరి_పంక్తి_సంఖ్య + 1;
  new_బఫరు->అంకె_పంక్తులు = 0;
  new_బఫరు->పంక్తి_ప్రారంభం = new_బఫరు->curr_పంక్తి = ఖాళీ;
  new_బఫరు->తదుపరి = ఖాళీ;

  తిరిగి new_బఫరు;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

స్థిర శూన్య
save_బఫరు (నిర్మాణం బఫరు_రికార్డు *buf)
{
  buf->తదుపరి = ఖాళీ;
  buf->curr_పంక్తి = buf->పంక్తి_ప్రారంభం;

  ఒకవేళ (తల == ఖాళీ)
    తల = buf;
  లేకపోతే
    {
      నిర్మాణం బఫరు_రికార్డు *p;
      కొరకు (p = తల; p->తదుపరి; p = p->తదుపరి)
        /* Do nothing. */ ;
      p->తదుపరి = buf;
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

స్థిర bool
ఎక్కించు_బఫరు (శూన్య)
{
  ఒకవేళ (have_read_eof)
    తిరిగి false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t బైట్లు_wanted = MAX (ప్రారంభం_కొలత, hold_లెక్క + 1);

  అయితే (true)
    {
      నిర్మాణం బఫరు_రికార్డు *b = get_new_బఫరు (బైట్లు_wanted);
      idx_t బైట్లు_alloc = b->బైట్లు_alloc;
      idx_t బైట్లు_avail = బైట్లు_alloc;
      అక్షరం *p = b->బఫరు;

      /* First check the 'holding' area for a partial line. */
      ఒకవేళ (hold_లెక్క)
        {
          p = mempcpy (p, hold_area, hold_లెక్క);
          b->బైట్లు_వాడిన += hold_లెక్క;
          బైట్లు_avail -= hold_లెక్క;
          hold_లెక్క = 0;
        }

      b->బైట్లు_వాడిన += read_ఆదానం (p, బైట్లు_avail - 1);

      ఒకవేళ (రికార్డు_పంక్తి_starts (b) != 0)
        {
          save_బఫరు (b);
          తిరిగి true;
        }

      free_బఫరు (b);
      ఒకవేళ (have_read_eof)
        తిరిగి false;
      ఒకవేళ (ckd_కూడు (&బైట్లు_wanted, బైట్లు_alloc, బైట్లు_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

స్థిర intmax_t
get_మొదటి_పంక్తి_లోపల_బఫరు (శూన్య)
{
  ఒకవేళ (తల == ఖాళీ && !ఎక్కించు_బఫరు ())
    తిరిగి 0;

  తిరిగి తల->మొదటి_అందుబాటు;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

స్థిర నిర్మాణం cstring *
remove_పంక్తి (శూన్య)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  స్థిర నిర్మాణం బఫరు_రికార్డు *prev_buf = ఖాళీ;

  నిర్మాణం cstring *పంక్తి;		/* Return value. */
  నిర్మాణం పంక్తి *l;		/* For convenience. */

  ఒకవేళ (prev_buf)
    {
      free_బఫరు (prev_buf);
      prev_buf = ఖాళీ;
    }

  ఒకవేళ (తల == ఖాళీ && !ఎక్కించు_బఫరు ())
    తిరిగి ఖాళీ;

  ఒకవేళ (current_పంక్తి < తల->మొదటి_అందుబాటు)
    current_పంక్తి = తల->మొదటి_అందుబాటు;

  ++(తల->మొదటి_అందుబాటు);

  l = తల->curr_పంక్తి;

  పంక్తి = &l->starts[l->retrieve_సూచీ];

  /* Advance index to next line. */
  ఒకవేళ (++l->retrieve_సూచీ == l->వాడిన)
    {
      /* Go on to the next line record. */
      తల->curr_పంక్తి = l->తదుపరి;
      ఒకవేళ (తల->curr_పంక్తి == ఖాళీ || తల->curr_పంక్తి->వాడిన == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = తల;
          తల = తల->తదుపరి;
        }
    }

  తిరిగి పంక్తి;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

స్థిర నిర్మాణం cstring *
కనుగొను_పంక్తి (intmax_t linenum)
{
  ఒకవేళ (తల == ఖాళీ && !ఎక్కించు_బఫరు ())
    తిరిగి ఖాళీ;

  ఒకవేళ (linenum < తల->ప్రారంభం_పంక్తి)
    తిరిగి ఖాళీ;

  కొరకు (నిర్మాణం బఫరు_రికార్డు *b = తల;;)
    {
      ఒకవేళ (linenum < b->ప్రారంభం_పంక్తి + b->అంకె_పంక్తులు)
        {
          /* The line is in this buffer. */
          నిర్మాణం పంక్తి *l;
          idx_t స్థానభ్రంశం;	/* How far into the buffer the line is. */

          l = b->పంక్తి_ప్రారంభం;
          స్థానభ్రంశం = linenum - b->ప్రారంభం_పంక్తి;
          /* Find the control record. */
          అయితే (స్థానభ్రంశం >= CTRL_కొలత)
            {
              l = l->తదుపరి;
              స్థానభ్రంశం -= CTRL_కొలత;
            }
          తిరిగి &l->starts[స్థానభ్రంశం];
        }
      ఒకవేళ (b->తదుపరి == ఖాళీ && !ఎక్కించు_బఫరు ())
        తిరిగి ఖాళీ;
      b = b->తదుపరి;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

స్థిర bool
no_మరిన్ని_పంక్తులు (శూన్య)
{
  తిరిగి కనుగొను_పంక్తి (current_పంక్తి + 1) == ఖాళీ;
}

/* Open NAME as standard input.  */

స్థిర శూన్య
సమితి_ఆదానం_file (అక్షరం స్థిరరాశి *name)
{
  ఒకవేళ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

స్థిర శూన్య
write_గమ్యం_file (intmax_t చివరి_పంక్తి, bool ignore, పూర్ణాంకం argnum)
{
  నిర్మాణం cstring *పంక్తి;
  intmax_t మొదటి_పంక్తి;		/* First available input line. */
  intmax_t పంక్తులు;		/* Number of lines to output. */

  మొదటి_పంక్తి = get_మొదటి_పంక్తి_లోపల_బఫరు ();

  ఒకవేళ (! మొదటి_పంక్తి || మొదటి_పంక్తి > చివరి_పంక్తి)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  పంక్తులు = చివరి_పంక్తి - మొదటి_పంక్తి;

  కొరకు (intmax_t i = 0; i < పంక్తులు; i++)
    {
      పంక్తి = remove_పంక్తి ();
      ఒకవేళ (పంక్తి == ఖాళీ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ఒకవేళ (!ignore)
        save_పంక్తి_గమ్యం_file (పంక్తి);
    }
}

/* Output any lines left after all regexps have been processed. */

స్థిర శూన్య
dump_rest_యొక్క_file (శూన్య)
{
  నిర్మాణం cstring *పంక్తి;

  అయితే ((పంక్తి = remove_పంక్తి ()) != ఖాళీ)
    save_పంక్తి_గమ్యం_file (పంక్తి);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

స్థిర శూన్య
handle_పంక్తి_error (స్థిరరాశి నిర్మాణం control *p, intmax_t repetition)
{
  అక్షరం buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           కార్యక్రమం_name, quote (imaxtostr (p->పంక్తులు_required, buf)));
  ఒకవేళ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  లేకపోతే
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

స్థిర శూన్య
process_పంక్తి_లెక్క (స్థిరరాశి నిర్మాణం control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t చివరి_పంక్తి_గమ్యం_save = p->పంక్తులు_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ఒకవేళ (no_మరిన్ని_పంక్తులు () && suppress_matched)
    handle_పంక్తి_error (p, repetition);

  ఒకవేళ (!(linenum = get_మొదటి_పంక్తి_లోపల_బఫరు ()))
    handle_పంక్తి_error (p, repetition);

  అయితే (linenum++ < చివరి_పంక్తి_గమ్యం_save)
    {
      నిర్మాణం cstring *పంక్తి = remove_పంక్తి ();
      ఒకవేళ (పంక్తి == ఖాళీ)
        handle_పంక్తి_error (p, repetition);
      save_పంక్తి_గమ్యం_file (పంక్తి);
    }

  close_output_file ();

  ఒకవేళ (suppress_matched)
    remove_పంక్తి ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ఒకవేళ (no_మరిన్ని_పంక్తులు () && !suppress_matched)
    handle_పంక్తి_error (p, repetition);
}

స్థిర శూన్య
regexp_error (నిర్మాణం control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           కార్యక్రమం_name, quote (global_argv[p->argnum]));

  ఒకవేళ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  లేకపోతే
    fprintf (stderr, "\n");

  ఒకవేళ (!ignore)
    {
      dump_rest_యొక్క_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

స్థిర శూన్య
process_regexp (నిర్మాణం control *p, intmax_t repetition)
{
  నిర్మాణం cstring *పంక్తి;		/* From input file. */
  idx_t పంక్తి_len;		/* To make "$" in regexps work. */
  intmax_t break_పంక్తి;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ఒకవేళ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ఒకవేళ (p->స్థానభ్రంశం >= 0)
    {
      అయితే (true)
        {
          పంక్తి = కనుగొను_పంక్తి (++current_పంక్తి);
          ఒకవేళ (పంక్తి == ఖాళీ)
            {
              ఒకవేళ (p->repeat_forever)
                {
                  ఒకవేళ (!ignore)
                    {
                      dump_rest_యొక్క_file ();
                      close_output_file ();
                    }
                  నిష్క్రమించు (EXIT_SUCCESS);
                }
              లేకపోతే
                regexp_error (p, repetition, ignore);
            }
          పంక్తి_len = పంక్తి->len;
          ఒకవేళ (పంక్తి->str[పంక్తి_len - 1] == '\n')
            పంక్తి_len--;
          ret = re_శోధన (&p->re_compiled, పంక్తి->str, పంక్తి_len,
                           0, పంక్తి_len, ఖాళీ);
          ఒకవేళ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ఒకవేళ (ret == -1)
            {
              పంక్తి = remove_పంక్తి ();
              ఒకవేళ (!ignore)
                save_పంక్తి_గమ్యం_file (పంక్తి);
            }
          లేకపోతే
            ఆపు;
        }
    }
  లేకపోతే
    {
      /* Buffer the lines. */
      అయితే (true)
        {
          పంక్తి = కనుగొను_పంక్తి (++current_పంక్తి);
          ఒకవేళ (పంక్తి == ఖాళీ)
            {
              ఒకవేళ (p->repeat_forever)
                {
                  ఒకవేళ (!ignore)
                    {
                      dump_rest_యొక్క_file ();
                      close_output_file ();
                    }
                  నిష్క్రమించు (EXIT_SUCCESS);
                }
              లేకపోతే
                regexp_error (p, repetition, ignore);
            }
          పంక్తి_len = పంక్తి->len;
          ఒకవేళ (పంక్తి->str[పంక్తి_len - 1] == '\n')
            పంక్తి_len--;
          ret = re_శోధన (&p->re_compiled, పంక్తి->str, పంక్తి_len,
                           0, పంక్తి_len, ఖాళీ);
          ఒకవేళ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ఒకవేళ (ret != -1)
            ఆపు;
        }
    }

  /* Account for any offset from this regexp. */
  break_పంక్తి = current_పంక్తి + p->స్థానభ్రంశం;

  write_గమ్యం_file (break_పంక్తి, ignore, p->argnum);

  ఒకవేళ (!ignore)
    close_output_file ();

  ఒకవేళ (p->స్థానభ్రంశం > 0)
    current_పంక్తి = break_పంక్తి;

  ఒకవేళ (suppress_matched)
    remove_పంక్తి ();
}

/* Split the input file according to the control records we have built. */

స్థిర శూన్య
విభజించు_file (శూన్య)
{
  కొరకు (idx_t i = 0; i < control_వాడిన; i++)
    {
      ఒకవేళ (controls[i].regexpr)
        {
          కొరకు (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      లేకపోతే
        {
          కొరకు (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_పంక్తి_లెక్క (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_యొక్క_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

స్థిర అక్షరం *
make_filename (పూర్ణాంకం అంకె)
{
  strcpy (filename_space, ఉపసర్గ);
  ఒకవేళ (suffix)
    sprintf (filename_space + అక్షరశ్రేణి_పొడవు (ఉపసర్గ), suffix, అంకె);
  లేకపోతే
    sprintf (filename_space + అక్షరశ్రేణి_పొడవు (ఉపసర్గ), "%0*d", digits, అంకె);
  తిరిగి filename_space;
}

/* Create the next output file. */

స్థిర శూన్య
create_output_file (శూన్య)
{
  పూర్ణాంకం nfiles = files_created;
  bool fopen_ok;
  పూర్ణాంకం fopen_errno;

  output_filename = make_filename (nfiles);

  ఒకవేళ (nfiles == గరిష్ఠ_పూర్ణాంకం)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  లేకపోతే
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_సంకేతాలు, &oldset);
      output_ప్రవాహం = fopen (output_filename, "w");
      fopen_ok = (output_ప్రవాహం != ఖాళీ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ఖాళీ);
    }

  ఒకవేళ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  బైట్లు_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

స్థిర శూన్య
delete_all_files (bool లోపల_signal_handler)
{
  ఒకవేళ (! remove_files)
    తిరిగి;

  కొరకు (పూర్ణాంకం i = files_created; 0 <= --i; )
    {
      అక్షరం స్థిరరాశి *name = make_filename (i);
      ఒకవేళ (unlink (name) != 0 && errno != ENOENT && !లోపల_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

స్థిర శూన్య
close_output_file (శూన్య)
{
  ఒకవేళ (output_ప్రవాహం)
    {
      ఒకవేళ (ferror (output_ప్రవాహం))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_ప్రవాహం = ఖాళీ;
          cleanup_fatal ();
        }
      ఒకవేళ (fclose (output_ప్రవాహం) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_ప్రవాహం = ఖాళీ;
          cleanup_fatal ();
        }
      ఒకవేళ (బైట్లు_written == 0 && elide_ఖాళీలు_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          పూర్ణాంకం unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_సంకేతాలు, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ఖాళీ);

          ఒకవేళ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      లేకపోతే
        {
          ఒకవేళ (!suppress_లెక్క)
            fprintf (stdout, "%jd\n", బైట్లు_written);
        }
      output_ప్రవాహం = ఖాళీ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

స్థిర శూన్య
save_పంక్తి_గమ్యం_file (స్థిరరాశి నిర్మాణం cstring *పంక్తి)
{
  idx_t l = fwrite (పంక్తి->str, పరిమాణం (అక్షరం), పంక్తి->len, output_ప్రవాహం);
  ఒకవేళ (l != పంక్తి->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_ప్రవాహం = ఖాళీ;
      cleanup_fatal ();
    }
  బైట్లు_written += పంక్తి->len;
}

/* Return a new, initialized control record. */

స్థిర నిర్మాణం control *
new_control_రికార్డు (శూన్య)
{
  స్థిర idx_t control_allocated = 0; /* Total space allocated. */
  నిర్మాణం control *p;

  ఒకవేళ (control_వాడిన == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, పరిమాణం *controls);
  p = &controls[control_వాడిన++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->పంక్తులు_required = 0;
  p->స్థానభ్రంశం = 0;
  తిరిగి p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

స్థిర శూన్య
check_for_స్థానభ్రంశం (నిర్మాణం control *p, అక్షరం స్థిరరాశి *str, అక్షరం స్థిరరాశి *అంకె)
{
  ఒకవేళ (xstrtoimax (అంకె, ఖాళీ, 10, &p->స్థానభ్రంశం, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

స్థిర శూన్య
parse_repeat_లెక్క (పూర్ణాంకం argnum, నిర్మాణం control *p, అక్షరం *str)
{
  అక్షరం *చివర;

  చివర = str + అక్షరశ్రేణి_పొడవు (str) - 1;
  ఒకవేళ (*చివర != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *చివర = '\0';

  ఒకవేళ (str + 1 == చివర - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  లేకపోతే
    {
      uintmax_t val;
      ఒకవేళ (xstrtoumax (str + 1, ఖాళీ, 10, &val, "") != LONGINT_OK
          || ckd_కూడు (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *చివర = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

స్థిర నిర్మాణం control *
వెలికితీయి_regexp (పూర్ణాంకం argnum, bool ignore, అక్షరం స్థిరరాశి *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  అక్షరం delim = *str;
  అక్షరం స్థిరరాశి *closing_delim;
  నిర్మాణం control *p;
  అక్షరం స్థిరరాశి *err;

  closing_delim = strrchr (str + 1, delim);
  ఒకవేళ (closing_delim == ఖాళీ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_రికార్డు ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.బఫరు = ఖాళీ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ఖాళీ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ఖాళీలు_RANGES;
  err = re_compile_నమూనా (str + 1, len, &p->re_compiled);
  ఒకవేళ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ఒకవేళ (closing_delim[1])
    check_for_స్థానభ్రంశం (p, str, closing_delim + 1);

  తిరిగి p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

స్థిర శూన్య
parse_patterns (పూర్ణాంకం argc, పూర్ణాంకం ప్రారంభం, అక్షరం **argv)
{
  నిర్మాణం control *p;		/* New control record created. */
  స్థిర intmax_t చివరి_val = 0;

  కొరకు (పూర్ణాంకం i = ప్రారంభం; i < argc; i++)
    {
      ఒకవేళ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = వెలికితీయి_regexp (i, *argv[i] == '%', argv[i]);
        }
      లేకపోతే
        {
          p = new_control_రికార్డు ();
          p->argnum = i;

          uintmax_t val;
          ఒకవేళ (xstrtoumax (argv[i], ఖాళీ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ఒకవేళ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ఒకవేళ (val < చివరి_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), చివరి_val);

          ఒకవేళ (val == చివరి_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          చివరి_val = val;

          p->పంక్తులు_required = val;
        }

      ఒకవేళ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_లెక్క (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
గణన { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
స్థిర idx_t
get_format_flags (అక్షరం స్థిరరాశి *format, పూర్ణాంకం *flags_ptr)
{
  పూర్ణాంకం flags = 0;

  కొరకు (idx_t లెక్క = 0; ; లెక్క++)
    {
      ఎంపిక (format[లెక్క])
        {
        స్థితి '-':
        స్థితి '0':
          ఆపు;

        స్థితి '\'':
          flags |= FLAG_THOUSANDS;
          ఆపు;

        స్థితి '#':
          flags |= FLAG_ALTERNATIVE;
          ఆపు;

        అప్రమేయ:
          *flags_ptr = flags;
          తిరిగి లెక్క;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
స్థిర శూన్య
check_format_conv_type (అక్షరం *format, పూర్ణాంకం flags)
{
  గుర్తులేని అక్షరం ch = *format;
  పూర్ణాంకం compatible_flags = FLAG_THOUSANDS;

  ఎంపిక (ch)
    {
    స్థితి 'd':
    స్థితి 'i':
      ఆపు;

    స్థితి 'u':
      *format = 'd';
      ఆపు;

    స్థితి 'o':
    స్థితి 'x':
    స్థితి 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ఆపు;

    స్థితి 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    అప్రమేయ:
      ఒకవేళ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      లేకపోతే
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ఒకవేళ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
స్థిర idx_t
max_బహిర్గతం (అక్షరం *format)
{
  bool percent = false;

  కొరకు (అక్షరం *f = format; *f; f++)
    ఒకవేళ (*f == '%' && *++f != '%')
      {
        ఒకవేళ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        పూర్ణాంకం flags;
        f += get_format_flags (f, &flags);
        అయితే (c_isdigit (*f))
          f++;
        ఒకవేళ (*f == '.')
          అయితే (c_isdigit (*++f))
            కొనసాగు;
        check_format_conv_type (f, flags);
      }

  ఒకవేళ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  పూర్ణాంకం maxlen = snprintf (ఖాళీ, 0, format, గరిష్ఠ_పూర్ణాంకం);
  ఒకవేళ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  తిరిగి maxlen;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  అయితే ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ఖాళీ))
         != -1)
    ఎంపిక (optc)
      {
      స్థితి 'f':
        ఉపసర్గ = optarg;
        ఆపు;

      స్థితి 'b':
        suffix = optarg;
        ఆపు;

      స్థితి 'k':
        remove_files = false;
        ఆపు;

      స్థితి 'n':
        digits = xdectoimax (optarg, 0, MIN (గరిష్ఠ_పూర్ణాంకం, IDX_MAX), "",
                             _("invalid number"), 0);
        ఆపు;

      స్థితి 's':
      స్థితి 'q':
        suppress_లెక్క = true;
        ఆపు;

      స్థితి 'z':
        elide_ఖాళీలు_files = true;
        ఆపు;

      స్థితి SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ఆపు;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  ఒకవేళ (argc - optind < 2)
    {
      ఒకవేళ (argc <= optind)
        error (0, 0, _("missing operand"));
      లేకపోతే
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
  idx_t max_digit_string_len
    = (suffix
       ? max_బహిర్గతం (suffix)
       : MAX (INT_STRLEN_BOUND (పూర్ణాంకం), digits));
  idx_t filename_కొలత;
  ఒకవేళ (ckd_కూడు (&filename_కొలత, ఉపసర్గ_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_కొలత);

  సమితి_ఆదానం_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    గణన { nsigs = countof (పదం_sig) };

    నిర్మాణం sigaction act;

    sigemptyset (&caught_సంకేతాలు);
    కొరకు (పూర్ణాంకం i = 0; i < nsigs; i++)
      {
        sigaction (పదం_sig[i], ఖాళీ, &act);
        ఒకవేళ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_సంకేతాలు, పదం_sig[i]);
      }

    act.sa_handler = అంతరాయం_handler;
    act.sa_mask = caught_సంకేతాలు;
    act.sa_flags = 0;

    కొరకు (పూర్ణాంకం i = 0; i < nsigs; i++)
      ఒకవేళ (sigismember (&caught_సంకేతాలు, పదం_sig[i]))
        sigaction (పదం_sig[i], &act, ఖాళీ);
  }

  విభజించు_file ();

  ఒకవేళ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  తిరిగి EXIT_SUCCESS;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... FILE నమూనా...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Output pieces యొక్క FILE separated by నమూనా(s) గమ్యం files 'xx00', 'xx01', ...,\n\
and output byte counts యొక్క each piece గమ్యం standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard ఆదానం ఒకవేళ FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead యొక్క %02d\n\
"));
      oputs (_("\
  -f, --ఉపసర్గ=ఉపసర్గ_2\n\
         use ఉపసర్గ_2 instead యొక్క 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         చేయి not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the పంక్తులు matching నమూనా\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified సంఖ్య యొక్క digits instead యొక్క 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         చేయి not print counts యొక్క output file sizes\n\
"));
      oputs (_("\
  -z, --elide-ఖాళీలు-files\n\
         suppress ఖాళీలు output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each నమూనా may be:\n\
  INTEGER            copy పైకి గమ్యం but not including specified పంక్తి సంఖ్య\n\
  /REGEXP/[స్థానభ్రంశం_2]   copy పైకి గమ్యం but not including a matching పంక్తి\n\
  %REGEXP%[స్థానభ్రంశం_2]   skip గమ్యం, but not including a matching పంక్తి\n\
  {INTEGER}          repeat the మునుపటి నమూనా_2 specified సంఖ్య యొక్క times\n\
  {*}                repeat the మునుపటి నమూనా_2 as many times as possible\n\
\n\
A పంక్తి స్థానభ్రంశం_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}
