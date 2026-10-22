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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "safe-read.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "term-sig.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "csplit"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Stuart Kemp"), \
  proper_name ("David MacKenzie")

/* The default prefix for output file names. */
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_ಪೂರ್ವಪ್ರತ್ಯಯ	"xx"

/* A compiled pattern arg. */
ರಚನೆ control
{
  intmax_t ಸ್ಥಾನಾಂತರ;		/* Offset from regexp to split at. */
  intmax_t ಸಾಲುಗಳು_required;	/* Number of lines required. */
  intmax_t repeat;		/* Repeat count. */
  ಪೂರ್ಣಾಂಕ argnum;			/* ARGV index. */
  bool repeat_forever;		/* True if '*' used as a repeat count. */
  bool ignore;			/* If true, produce no output (for regexp). */
  bool regexpr;			/* True if regular expression was used. */
  ರಚನೆ re_ನಮೂನೆ_ಬಫರ್ re_compiled;	/* Compiled regular expression. */
};

/* Initial size of data area in buffers. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಆರಂಭ_ಗಾತ್ರ	8191

/* Number of lines kept in each node in line list. */
#ವ್ಯಾಖ್ಯಾನಿಸು CTRL_ಗಾತ್ರ	80

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
/* Some small values to test the algorithms. */
# define ಆರಂಭ_ಗಾತ್ರ	200
# define CTRL_ಗಾತ್ರ	1
#ಆದರೆ_ಅಂತ್ಯ

/* A string with a length count. */
ರಚನೆ cstring
{
  idx_t len;
  ಅಕ್ಷರ *str;
};

/* Pointers to the beginnings of lines in the buffer area.
   These structures are linked together if needed. */
ರಚನೆ ಸಾಲು
{
  idx_t ಬಳಸಿದ;			/* Number of offsets used in this struct. */
  idx_t ಸೇರಿಸು_ಸೂಚ್ಯಂಕ;		/* Next offset to use when inserting line. */
  idx_t retrieve_ಸೂಚ್ಯಂಕ;	/* Next index to use when retrieving line. */
  ರಚನೆ cstring starts[CTRL_ಗಾತ್ರ]; /* Lines in the data area. */
  ರಚನೆ ಸಾಲು *ಮುಂದಿನ;		/* Next in linked list. */
};

/* The structure to hold the input lines.
   Contains a pointer to the data area and a list containing
   pointers to the individual lines. */
ರಚನೆ ಬಫರ್_ದಾಖಲೆ
{
  idx_t ಬೈಟುಗಳು_alloc;		/* Size of the buffer area. */
  idx_t ಬೈಟುಗಳು_ಬಳಸಿದ;		/* Bytes used in the buffer area. */
  intmax_t ಆರಂಭ_ಸಾಲು;		/* First line number in this buffer. */
  intmax_t ಮೊದಲ_ಲಭ್ಯ;	/* First line that can be retrieved. */
  idx_t ಅಂಕೆ_ಸಾಲುಗಳು;		/* Number of complete lines in this buffer. */
  ಅಕ್ಷರ *ಬಫರ್ COUNTED_BY (ಬೈಟುಗಳು_alloc);	/* Data area. */
  ರಚನೆ ಸಾಲು *ಸಾಲು_ಆರಂಭ;	/* Head of list of pointers to lines. */
  ರಚನೆ ಸಾಲು *curr_ಸಾಲು;	/* The line start record currently in use. */
  ರಚನೆ ಬಫರ್_ದಾಖಲೆ *ಮುಂದಿನ;
};

ಸ್ಥಿರ ಶೂನ್ಯ close_output_file (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ create_output_file (ಶೂನ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ delete_all_files (bool);
ಸ್ಥಿರ ಶೂನ್ಯ save_ಸಾಲು_ಗಮ್ಯ_file (ಸ್ಥಿರಾಂಕ ರಚನೆ cstring *ಸಾಲು);

/* Start of buffer list. */
ಸ್ಥಿರ ರಚನೆ ಬಫರ್_ದಾಖಲೆ *ತಲೆ = ಖಾಲಿ;

/* Partially read line. */
ಸ್ಥಿರ ಅಕ್ಷರ *hold_area = ಖಾಲಿ;

/* Number of bytes in 'hold_area'. */
ಸ್ಥಿರ idx_t hold_ಎಣಿಕೆ = 0;

/* Number of the last line in the buffers. */
ಸ್ಥಿರ intmax_t ಕೊನೆಯ_ಸಾಲು_ಸಂಖ್ಯೆ = 0;

/* Number of the line currently being examined. */
ಸ್ಥಿರ intmax_t current_ಸಾಲು = 0;

/* If true, we have read EOF. */
ಸ್ಥಿರ bool have_read_eof = false;

/* Name of output files. */
ಸ್ಥಿರ ಅಕ್ಷರ *ಅಸ್ಥಿರ filename_space = ಖಾಲಿ;

/* Prefix part of output file names. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಸ್ಥಿರ ಪೂರ್ವಪ್ರತ್ಯಯ = DEFAULT_ಪೂರ್ವಪ್ರತ್ಯಯ;

/* Suffix part of output file names. */
ಸ್ಥಿರ ಅಕ್ಷರ *ಅಸ್ಥಿರ suffix = ಖಾಲಿ;

/* Number of digits to use in output file names. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಸ್ಥಿರ digits = 2;

/* Number of files created so far. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಅಸ್ಥಿರ files_created = 0;

/* Number of bytes written to current file. */
ಸ್ಥಿರ intmax_t ಬೈಟುಗಳು_written;

/* Output file pointer. */
ಸ್ಥಿರ FILE *output_ಹರಿವು = ಖಾಲಿ;

/* Output file name. */
ಸ್ಥಿರ ಅಕ್ಷರ *output_filename = ಖಾಲಿ;

/* Perhaps it would be cleaner to pass arg values instead of indexes. */
ಸ್ಥಿರ ಅಕ್ಷರ **global_argv;

/* If true, do not print the count of bytes in each output file. */
ಸ್ಥಿರ bool suppress_ಎಣಿಕೆ;

/* If true, remove output files on error. */
ಸ್ಥಿರ bool ಅಸ್ಥಿರ remove_files = true;

/* If true, remove all output files which have a zero length. */
ಸ್ಥಿರ bool elide_ಖಾಲಿಗಳು_files;

/* If true, suppress the lines that match the PATTERN */
ಸ್ಥಿರ bool suppress_matched;

/* The compiled pattern arguments, which determine how to split
   the input file. */
ಸ್ಥಿರ ರಚನೆ control *controls;

/* Number of elements in 'controls'. */
ಸ್ಥಿರ idx_t control_ಬಳಸಿದ;

/* The set of signals that are caught.  */
ಸ್ಥಿರ sigset_t caught_ಸಂಕೇತಗಳು;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  SUPPRESS_MATCHED_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"digits", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},  /* Deprecated.  */
  {"silent", no_ವಾದ, ಖಾಲಿ, 's'},
  {"keep-files", no_ವಾದ, ಖಾಲಿ, 'k'},
  {"elide-empty-files", no_ವಾದ, ಖಾಲಿ, 'z'},
  {"prefix", required_ವಾದ, ಖಾಲಿ, 'f'},
  {"suffix-format", required_ವಾದ, ಖಾಲಿ, 'b'},
  {"suppress-matched", no_ವಾದ, ಖಾಲಿ, SUPPRESS_MATCHED_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Optionally remove files created so far; then exit.
   Called when an error detected. */

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup (ಶೂನ್ಯ)
{
  sigset_t oldset;

  close_output_file ();

  sigprocmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &oldset);
  delete_all_files (false);
  sigprocmask (SIG_SETMASK, &oldset, ಖಾಲಿ);
}

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
cleanup_fatal (ಶೂನ್ಯ)
{
  cleanup ();
  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಬಾಹ್ಯ ಶೂನ್ಯ
xalloc_die (ಶೂನ್ಯ)
{
  error (0, 0, "%s", _("memory exhausted"));
  cleanup_fatal ();
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಅಡಚಣೆ_handler (ಪೂರ್ಣಾಂಕ sig)
{
  delete_all_files (true);
  ಸಂಕೇತ (sig, SIG_DFL);
  /* The signal has been reset to SIG_DFL, but blocked during this
     handler.  Force the default action of this signal once the
     handler returns and the block is removed.  */
  ಎತ್ತು (sig);
}

/* Keep track of NUM bytes of a partial line in buffer START.
   These bytes will be retrieved later when another large buffer is read.  */

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಗಮ್ಯ_hold_area (ಅಕ್ಷರ *ಆರಂಭ, idx_t ಅಂಕೆ)
{
  ಬಿಡುಗಡೆ (hold_area);
  hold_area = ಆರಂಭ;
  hold_ಎಣಿಕೆ = ಅಂಕೆ;
}

/* Read up to MAX_N_BYTES bytes from the input stream into DEST.
   Return the number of bytes read. */

ಸ್ಥಿರ idx_t
read_ಒಳಹರಿವು (ಅಕ್ಷರ *dest, idx_t max_n_ಬೈಟುಗಳು)
{
  ಆದರೆ (max_n_ಬೈಟುಗಳು == 0)
    ಹಿಂತಿರುಗಿ 0;

  ptrdiff_t ಬೈಟುಗಳು_read = ಸುರಕ್ಷಿತ_read (STDIN_FILENO, dest, max_n_ಬೈಟುಗಳು);

  ಆದರೆ (ಬೈಟುಗಳು_read == 0)
    have_read_eof = true;

  ಆದರೆ (ಬೈಟುಗಳು_read < 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ಹಿಂತಿರುಗಿ ಬೈಟುಗಳು_read;
}

/* Initialize existing line record P. */

ಸ್ಥಿರ ಶೂನ್ಯ
clear_ಸಾಲು_control (ರಚನೆ ಸಾಲು *p)
{
  p->ಬಳಸಿದ = 0;
  p->ಸೇರಿಸು_ಸೂಚ್ಯಂಕ = 0;
  p->retrieve_ಸೂಚ್ಯಂಕ = 0;
}

/* Return a new, initialized line record. */

ಸ್ಥಿರ ರಚನೆ ಸಾಲು *
new_ಸಾಲು_control (ಶೂನ್ಯ)
{
  ರಚನೆ ಸಾಲು *p = xmalloc (ಗಾತ್ರ *p);

  p->ಮುಂದಿನ = ಖಾಲಿ;
  clear_ಸಾಲು_control (p);

  ಹಿಂತಿರುಗಿ p;
}

/* Record LINE_START, which is the address of the start of a line
   of length LINE_LEN in the large buffer, in the lines buffer of B. */

ಸ್ಥಿರ ಶೂನ್ಯ
keep_new_ಸಾಲು (ರಚನೆ ಬಫರ್_ದಾಖಲೆ *b, ಅಕ್ಷರ *ಸಾಲು_ಆರಂಭ, idx_t ಸಾಲು_len)
{
  ರಚನೆ ಸಾಲು *l;

  /* If there is no existing area to keep line info, get some. */
  ಆದರೆ (b->ಸಾಲು_ಆರಂಭ == ಖಾಲಿ)
    b->ಸಾಲು_ಆರಂಭ = b->curr_ಸಾಲು = new_ಸಾಲು_control ();

  /* If existing area for lines is full, get more. */
  ಆದರೆ (b->curr_ಸಾಲು->ಬಳಸಿದ == CTRL_ಗಾತ್ರ)
    {
      b->curr_ಸಾಲು->ಮುಂದಿನ = new_ಸಾಲು_control ();
      b->curr_ಸಾಲು = b->curr_ಸಾಲು->ಮುಂದಿನ;
    }

  l = b->curr_ಸಾಲು;

  /* Record the start of the line, and update counters. */
  l->starts[l->ಸೇರಿಸು_ಸೂಚ್ಯಂಕ].str = ಸಾಲು_ಆರಂಭ;
  l->starts[l->ಸೇರಿಸು_ಸೂಚ್ಯಂಕ].len = ಸಾಲು_len;
  l->ಬಳಸಿದ++;
  l->ಸೇರಿಸು_ಸೂಚ್ಯಂಕ++;
}

/* Scan the buffer in B for newline characters
   and record the line start locations and lengths in B.
   Return the number of lines found in this buffer.

   There may be an incomplete line at the end of the buffer;
   a pointer is kept to this area, which will be used when
   the next buffer is filled. */

ಸ್ಥಿರ idx_t
ದಾಖಲೆ_ಸಾಲು_starts (ರಚನೆ ಬಫರ್_ದಾಖಲೆ *b)
{
  ಅಕ್ಷರ *ಸಾಲು_ಆರಂಭ;		/* Start of current line. */
  idx_t ಸಾಲುಗಳು;			/* Number of lines found. */
  idx_t ಸಾಲು_ಉದ್ದಳತೆ;		/* Length of each line found. */

  ಆದರೆ (b->ಬೈಟುಗಳು_ಬಳಸಿದ == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಸಾಲುಗಳು = 0;
  ಸಾಲು_ಆರಂಭ = b->ಬಫರ್;
  ಅಕ್ಷರ *ಬಫರ್_ಕೊನೆ = ಸಾಲು_ಆರಂಭ + b->ಬೈಟುಗಳು_ಬಳಸಿದ;
  *ಬಫರ್_ಕೊನೆ = '\n';

  ಆಗ (true)
    {
      ಅಕ್ಷರ *ಸಾಲು_ಕೊನೆ = rawmemchr (ಸಾಲು_ಆರಂಭ, '\n');
      ಆದರೆ (ಸಾಲು_ಕೊನೆ == ಬಫರ್_ಕೊನೆ)
        ನಿಲ್ಲಿಸು;
      ಸಾಲು_ಉದ್ದಳತೆ = ಸಾಲು_ಕೊನೆ - ಸಾಲು_ಆರಂಭ + 1;
      keep_new_ಸಾಲು (b, ಸಾಲು_ಆರಂಭ, ಸಾಲು_ಉದ್ದಳತೆ);
      ಸಾಲು_ಆರಂಭ = ಸಾಲು_ಕೊನೆ + 1;
      ಸಾಲುಗಳು++;
    }

  /* Check for an incomplete last line. */
  idx_t ಬೈಟುಗಳು_ಎಡ = ಬಫರ್_ಕೊನೆ - ಸಾಲು_ಆರಂಭ;
  ಆದರೆ (ಬೈಟುಗಳು_ಎಡ)
    {
      ಆದರೆ (have_read_eof)
        {
          keep_new_ಸಾಲು (b, ಸಾಲು_ಆರಂಭ, ಬೈಟುಗಳು_ಎಡ);
          ಸಾಲುಗಳು++;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        save_ಗಮ್ಯ_hold_area (ximemdup (ಸಾಲು_ಆರಂಭ, ಬೈಟುಗಳು_ಎಡ), ಬೈಟುಗಳು_ಎಡ);
    }

  b->ಅಂಕೆ_ಸಾಲುಗಳು = ಸಾಲುಗಳು;
  b->ಮೊದಲ_ಲಭ್ಯ = b->ಆರಂಭ_ಸಾಲು = ಕೊನೆಯ_ಸಾಲು_ಸಂಖ್ಯೆ + 1;
  ಕೊನೆಯ_ಸಾಲು_ಸಂಖ್ಯೆ += ಸಾಲುಗಳು;

  ಹಿಂತಿರುಗಿ ಸಾಲುಗಳು;
}

/* Work around <https://gcc.gnu.org/PR109614>.  */
#ಆದರೆ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-mismatching-deallocation"
# pragma GCC diagnostic ignored "-Wanalyzer-use-after-free"
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ
free_ಬಫರ್ (ರಚನೆ ಬಫರ್_ದಾಖಲೆ *buf)
{
  ಗಾಗಿ (ರಚನೆ ಸಾಲು *l = buf->ಸಾಲು_ಆರಂಭ; l;)
    {
      ರಚನೆ ಸಾಲು *n = l->ಮುಂದಿನ;
      ಬಿಡುಗಡೆ (l);
      l = n;
    }
  ಬಿಡುಗಡೆ (buf->ಬಫರ್);
  ಬಿಡುಗಡೆ (buf);
}

/* Return a new buffer of at least MINSIZE bytes.  */

ಸ್ಥಿರ ATTRIBUTE_DEALLOC (free_ಬಫರ್, 1)
ರಚನೆ ಬಫರ್_ದಾಖಲೆ *
get_new_ಬಫರ್ (idx_t min_ಗಾತ್ರ)
{
  ರಚನೆ ಬಫರ್_ದಾಖಲೆ *new_ಬಫರ್ = xmalloc (ಗಾತ್ರ *new_ಬಫರ್);
  new_ಬಫರ್->ಬೈಟುಗಳು_alloc = 0;
  new_ಬಫರ್->ಬಫರ್ = xpalloc (ಖಾಲಿ, &new_ಬಫರ್->ಬೈಟುಗಳು_alloc, min_ಗಾತ್ರ,
                                -1, 1);
  new_ಬಫರ್->ಬೈಟುಗಳು_ಬಳಸಿದ = 0;
  new_ಬಫರ್->ಆರಂಭ_ಸಾಲು = new_ಬಫರ್->ಮೊದಲ_ಲಭ್ಯ = ಕೊನೆಯ_ಸಾಲು_ಸಂಖ್ಯೆ + 1;
  new_ಬಫರ್->ಅಂಕೆ_ಸಾಲುಗಳು = 0;
  new_ಬಫರ್->ಸಾಲು_ಆರಂಭ = new_ಬಫರ್->curr_ಸಾಲು = ಖಾಲಿ;
  new_ಬಫರ್->ಮುಂದಿನ = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ new_ಬಫರ್;
}

/* Append buffer BUF to the linked list of buffers that contain
   some data yet to be processed. */

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಬಫರ್ (ರಚನೆ ಬಫರ್_ದಾಖಲೆ *buf)
{
  buf->ಮುಂದಿನ = ಖಾಲಿ;
  buf->curr_ಸಾಲು = buf->ಸಾಲು_ಆರಂಭ;

  ಆದರೆ (ತಲೆ == ಖಾಲಿ)
    ತಲೆ = buf;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ರಚನೆ ಬಫರ್_ದಾಖಲೆ *p;
      ಗಾಗಿ (p = ತಲೆ; p->ಮುಂದಿನ; p = p->ಮುಂದಿನ)
        /* Do nothing. */ ;
      p->ಮುಂದಿನ = buf;
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

ಸ್ಥಿರ bool
ತುಂಬು_ಬಫರ್ (ಶೂನ್ಯ)
{
  ಆದರೆ (have_read_eof)
    ಹಿಂತಿರುಗಿ false;

  /* We must make the buffer at least as large as the amount of data
     in the partial line left over from the last call,
     plus room for a sentinel '\n'. */
  idx_t ಬೈಟುಗಳು_wanted = MAX (ಆರಂಭ_ಗಾತ್ರ, hold_ಎಣಿಕೆ + 1);

  ಆಗ (true)
    {
      ರಚನೆ ಬಫರ್_ದಾಖಲೆ *b = get_new_ಬಫರ್ (ಬೈಟುಗಳು_wanted);
      idx_t ಬೈಟುಗಳು_alloc = b->ಬೈಟುಗಳು_alloc;
      idx_t ಬೈಟುಗಳು_avail = ಬೈಟುಗಳು_alloc;
      ಅಕ್ಷರ *p = b->ಬಫರ್;

      /* First check the 'holding' area for a partial line. */
      ಆದರೆ (hold_ಎಣಿಕೆ)
        {
          p = mempcpy (p, hold_area, hold_ಎಣಿಕೆ);
          b->ಬೈಟುಗಳು_ಬಳಸಿದ += hold_ಎಣಿಕೆ;
          ಬೈಟುಗಳು_avail -= hold_ಎಣಿಕೆ;
          hold_ಎಣಿಕೆ = 0;
        }

      b->ಬೈಟುಗಳು_ಬಳಸಿದ += read_ಒಳಹರಿವು (p, ಬೈಟುಗಳು_avail - 1);

      ಆದರೆ (ದಾಖಲೆ_ಸಾಲು_starts (b) != 0)
        {
          save_ಬಫರ್ (b);
          ಹಿಂತಿರುಗಿ true;
        }

      free_ಬಫರ್ (b);
      ಆದರೆ (have_read_eof)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (ckd_ಕೂಡಿಸು (&ಬೈಟುಗಳು_wanted, ಬೈಟುಗಳು_alloc, ಬೈಟುಗಳು_alloc >> 1))
        xalloc_die ();
    }
}

/* Return the line number of the first line that has not yet been retrieved.
   Return 0 if no lines available.  */

ಸ್ಥಿರ intmax_t
get_ಮೊದಲ_ಸಾಲು_ಒಳ_ಬಫರ್ (ಶೂನ್ಯ)
{
  ಆದರೆ (ತಲೆ == ಖಾಲಿ && !ತುಂಬು_ಬಫರ್ ())
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ ತಲೆ->ಮೊದಲ_ಲಭ್ಯ;
}

/* Return a pointer to the logical first line in the buffer and make the
   next line the logical first line.
   Return NULL if there is no more input. */

ಸ್ಥಿರ ರಚನೆ cstring *
remove_ಸಾಲು (ಶೂನ್ಯ)
{
  /* If non-null, this is the buffer for which the previous call
     returned the final line.  So now, presuming that line has been
     processed, we can free the buffer and reset this pointer.  */
  ಸ್ಥಿರ ರಚನೆ ಬಫರ್_ದಾಖಲೆ *prev_buf = ಖಾಲಿ;

  ರಚನೆ cstring *ಸಾಲು;		/* Return value. */
  ರಚನೆ ಸಾಲು *l;		/* For convenience. */

  ಆದರೆ (prev_buf)
    {
      free_ಬಫರ್ (prev_buf);
      prev_buf = ಖಾಲಿ;
    }

  ಆದರೆ (ತಲೆ == ಖಾಲಿ && !ತುಂಬು_ಬಫರ್ ())
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಆದರೆ (current_ಸಾಲು < ತಲೆ->ಮೊದಲ_ಲಭ್ಯ)
    current_ಸಾಲು = ತಲೆ->ಮೊದಲ_ಲಭ್ಯ;

  ++(ತಲೆ->ಮೊದಲ_ಲಭ್ಯ);

  l = ತಲೆ->curr_ಸಾಲು;

  ಸಾಲು = &l->starts[l->retrieve_ಸೂಚ್ಯಂಕ];

  /* Advance index to next line. */
  ಆದರೆ (++l->retrieve_ಸೂಚ್ಯಂಕ == l->ಬಳಸಿದ)
    {
      /* Go on to the next line record. */
      ತಲೆ->curr_ಸಾಲು = l->ಮುಂದಿನ;
      ಆದರೆ (ತಲೆ->curr_ಸಾಲು == ಖಾಲಿ || ತಲೆ->curr_ಸಾಲು->ಬಳಸಿದ == 0)
        {
          /* Go on to the next data block.
             but first record the current one so we can free it
             once the line we're returning has been processed.  */
          prev_buf = ತಲೆ;
          ತಲೆ = ತಲೆ->ಮುಂದಿನ;
        }
    }

  ಹಿಂತಿರುಗಿ ಸಾಲು;
}

/* Search the buffers for line LINENUM, reading more input if necessary.
   Return a pointer to the line, or NULL if it is not found in the file. */

ಸ್ಥಿರ ರಚನೆ cstring *
ಕಂಡುಹಿಡಿ_ಸಾಲು (intmax_t linenum)
{
  ಆದರೆ (ತಲೆ == ಖಾಲಿ && !ತುಂಬು_ಬಫರ್ ())
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಆದರೆ (linenum < ತಲೆ->ಆರಂಭ_ಸಾಲು)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಗಾಗಿ (ರಚನೆ ಬಫರ್_ದಾಖಲೆ *b = ತಲೆ;;)
    {
      ಆದರೆ (linenum < b->ಆರಂಭ_ಸಾಲು + b->ಅಂಕೆ_ಸಾಲುಗಳು)
        {
          /* The line is in this buffer. */
          ರಚನೆ ಸಾಲು *l;
          idx_t ಸ್ಥಾನಾಂತರ;	/* How far into the buffer the line is. */

          l = b->ಸಾಲು_ಆರಂಭ;
          ಸ್ಥಾನಾಂತರ = linenum - b->ಆರಂಭ_ಸಾಲು;
          /* Find the control record. */
          ಆಗ (ಸ್ಥಾನಾಂತರ >= CTRL_ಗಾತ್ರ)
            {
              l = l->ಮುಂದಿನ;
              ಸ್ಥಾನಾಂತರ -= CTRL_ಗಾತ್ರ;
            }
          ಹಿಂತಿರುಗಿ &l->starts[ಸ್ಥಾನಾಂತರ];
        }
      ಆದರೆ (b->ಮುಂದಿನ == ಖಾಲಿ && !ತುಂಬು_ಬಫರ್ ())
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
      b = b->ಮುಂದಿನ;		/* Try the next data block. */
    }
}

/* Return true if at least one more line is available for input. */

ಸ್ಥಿರ bool
no_ಇನ್ನಷ್ಟು_ಸಾಲುಗಳು (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_ಸಾಲು (current_ಸಾಲು + 1) == ಖಾಲಿ;
}

/* Open NAME as standard input.  */

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಒಳಹರಿವು_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name)
{
  ಆದರೆ (! streq (name, "-") && fd_reopen (STDIN_FILENO, name, O_RDONLY, 0) < 0)
    error (EXIT_FAILURE, errno, _("cannot open %s for reading"),
           quoteaf (name));
}

/* Write all lines from the beginning of the buffer up to, but
   not including, line LAST_LINE, to the current output file.
   If IGNORE is true, do not output lines selected here.
   ARGNUM is the index in ARGV of the current pattern. */

ಸ್ಥಿರ ಶೂನ್ಯ
write_ಗಮ್ಯ_file (intmax_t ಕೊನೆಯ_ಸಾಲು, bool ignore, ಪೂರ್ಣಾಂಕ argnum)
{
  ರಚನೆ cstring *ಸಾಲು;
  intmax_t ಮೊದಲ_ಸಾಲು;		/* First available input line. */
  intmax_t ಸಾಲುಗಳು;		/* Number of lines to output. */

  ಮೊದಲ_ಸಾಲು = get_ಮೊದಲ_ಸಾಲು_ಒಳ_ಬಫರ್ ();

  ಆದರೆ (! ಮೊದಲ_ಸಾಲು || ಮೊದಲ_ಸಾಲು > ಕೊನೆಯ_ಸಾಲು)
    {
      error (0, 0, _("%s: line number out of range"),
             quote (global_argv[argnum]));
      cleanup_fatal ();
    }

  ಸಾಲುಗಳು = ಕೊನೆಯ_ಸಾಲು - ಮೊದಲ_ಸಾಲು;

  ಗಾಗಿ (intmax_t i = 0; i < ಸಾಲುಗಳು; i++)
    {
      ಸಾಲು = remove_ಸಾಲು ();
      ಆದರೆ (ಸಾಲು == ಖಾಲಿ)
        {
          error (0, 0, _("%s: line number out of range"),
                 quote (global_argv[argnum]));
          cleanup_fatal ();
        }
      ಆದರೆ (!ignore)
        save_ಸಾಲು_ಗಮ್ಯ_file (ಸಾಲು);
    }
}

/* Output any lines left after all regexps have been processed. */

ಸ್ಥಿರ ಶೂನ್ಯ
dump_rest_ದ_file (ಶೂನ್ಯ)
{
  ರಚನೆ cstring *ಸಾಲು;

  ಆಗ ((ಸಾಲು = remove_ಸಾಲು ()) != ಖಾಲಿ)
    save_ಸಾಲು_ಗಮ್ಯ_file (ಸಾಲು);
}

/* Handle an attempt to read beyond EOF under the control of record P,
   on iteration REPETITION if nonzero. */

ಸ್ಥಿರ ಶೂನ್ಯ
handle_ಸಾಲು_error (ಸ್ಥಿರಾಂಕ ರಚನೆ control *p, intmax_t repetition)
{
  ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (intmax_t)];

  fprintf (stderr, _("%s: %s: line number out of range"),
           ಕಾರ್ಯಕ್ರಮ_name, quote (imaxtostr (p->ಸಾಲುಗಳು_required, buf)));
  ಆದರೆ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (stderr, "\n");

  cleanup_fatal ();
}

/* Determine the line number that marks the end of this file,
   then get those lines and save them to the output file.
   P is the control record.
   REPETITION is the repetition number. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಸಾಲು_ಎಣಿಕೆ (ಸ್ಥಿರಾಂಕ ರಚನೆ control *p, intmax_t repetition)
{
  intmax_t linenum;
  intmax_t ಕೊನೆಯ_ಸಾಲು_ಗಮ್ಯ_save = p->ಸಾಲುಗಳು_required * (repetition + 1);

  create_output_file ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file.
     When suppressing matched lines, check before the loop. */
  ಆದರೆ (no_ಇನ್ನಷ್ಟು_ಸಾಲುಗಳು () && suppress_matched)
    handle_ಸಾಲು_error (p, repetition);

  ಆದರೆ (!(linenum = get_ಮೊದಲ_ಸಾಲು_ಒಳ_ಬಫರ್ ()))
    handle_ಸಾಲು_error (p, repetition);

  ಆಗ (linenum++ < ಕೊನೆಯ_ಸಾಲು_ಗಮ್ಯ_save)
    {
      ರಚನೆ cstring *ಸಾಲು = remove_ಸಾಲು ();
      ಆದರೆ (ಸಾಲು == ಖಾಲಿ)
        handle_ಸಾಲು_error (p, repetition);
      save_ಸಾಲು_ಗಮ್ಯ_file (ಸಾಲು);
    }

  close_output_file ();

  ಆದರೆ (suppress_matched)
    remove_ಸಾಲು ();

  /* Ensure that the line number specified is not 1 greater than
     the number of lines in the file. */
  ಆದರೆ (no_ಇನ್ನಷ್ಟು_ಸಾಲುಗಳು () && !suppress_matched)
    handle_ಸಾಲು_error (p, repetition);
}

ಸ್ಥಿರ ಶೂನ್ಯ
regexp_error (ರಚನೆ control *p, intmax_t repetition, bool ignore)
{
  fprintf (stderr, _("%s: %s: match not found"),
           ಕಾರ್ಯಕ್ರಮ_name, quote (global_argv[p->argnum]));

  ಆದರೆ (repetition)
    fprintf (stderr, _(" on repetition %jd\n"), repetition);
  ಇಲ್ಲದಿದ್ದರೆ
    fprintf (stderr, "\n");

  ಆದರೆ (!ignore)
    {
      dump_rest_ದ_file ();
      close_output_file ();
    }
  cleanup_fatal ();
}

/* Read the input until a line matches the regexp in P, outputting
   it unless P->IGNORE is true.
   REPETITION is this repeat-count; 0 means the first time. */

ಸ್ಥಿರ ಶೂನ್ಯ
process_regexp (ರಚನೆ control *p, intmax_t repetition)
{
  ರಚನೆ cstring *ಸಾಲು;		/* From input file. */
  idx_t ಸಾಲು_len;		/* To make "$" in regexps work. */
  intmax_t break_ಸಾಲು;		/* First line number of next file. */
  bool ignore = p->ignore;	/* If true, skip this section. */
  regoff_t ret;

  ಆದರೆ (!ignore)
    create_output_file ();

  /* If there is no offset for the regular expression, or
     it is positive, then it is not necessary to buffer the lines. */

  ಆದರೆ (p->ಸ್ಥಾನಾಂತರ >= 0)
    {
      ಆಗ (true)
        {
          ಸಾಲು = ಕಂಡುಹಿಡಿ_ಸಾಲು (++current_ಸಾಲು);
          ಆದರೆ (ಸಾಲು == ಖಾಲಿ)
            {
              ಆದರೆ (p->repeat_forever)
                {
                  ಆದರೆ (!ignore)
                    {
                      dump_rest_ದ_file ();
                      close_output_file ();
                    }
                  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                regexp_error (p, repetition, ignore);
            }
          ಸಾಲು_len = ಸಾಲು->len;
          ಆದರೆ (ಸಾಲು->str[ಸಾಲು_len - 1] == '\n')
            ಸಾಲು_len--;
          ret = re_ಹುಡುಕು (&p->re_compiled, ಸಾಲು->str, ಸಾಲು_len,
                           0, ಸಾಲು_len, ಖಾಲಿ);
          ಆದರೆ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ಆದರೆ (ret == -1)
            {
              ಸಾಲು = remove_ಸಾಲು ();
              ಆದರೆ (!ignore)
                save_ಸಾಲು_ಗಮ್ಯ_file (ಸಾಲು);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ನಿಲ್ಲಿಸು;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Buffer the lines. */
      ಆಗ (true)
        {
          ಸಾಲು = ಕಂಡುಹಿಡಿ_ಸಾಲು (++current_ಸಾಲು);
          ಆದರೆ (ಸಾಲು == ಖಾಲಿ)
            {
              ಆದರೆ (p->repeat_forever)
                {
                  ಆದರೆ (!ignore)
                    {
                      dump_rest_ದ_file ();
                      close_output_file ();
                    }
                  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
                }
              ಇಲ್ಲದಿದ್ದರೆ
                regexp_error (p, repetition, ignore);
            }
          ಸಾಲು_len = ಸಾಲು->len;
          ಆದರೆ (ಸಾಲು->str[ಸಾಲು_len - 1] == '\n')
            ಸಾಲು_len--;
          ret = re_ಹುಡುಕು (&p->re_compiled, ಸಾಲು->str, ಸಾಲು_len,
                           0, ಸಾಲು_len, ಖಾಲಿ);
          ಆದರೆ (ret == -2)
            {
              error (0, 0, _("error in regular expression search"));
              cleanup_fatal ();
            }
          ಆದರೆ (ret != -1)
            ನಿಲ್ಲಿಸು;
        }
    }

  /* Account for any offset from this regexp. */
  break_ಸಾಲು = current_ಸಾಲು + p->ಸ್ಥಾನಾಂತರ;

  write_ಗಮ್ಯ_file (break_ಸಾಲು, ignore, p->argnum);

  ಆದರೆ (!ignore)
    close_output_file ();

  ಆದರೆ (p->ಸ್ಥಾನಾಂತರ > 0)
    current_ಸಾಲು = break_ಸಾಲು;

  ಆದರೆ (suppress_matched)
    remove_ಸಾಲು ();
}

/* Split the input file according to the control records we have built. */

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಭಜಿಸು_file (ಶೂನ್ಯ)
{
  ಗಾಗಿ (idx_t i = 0; i < control_ಬಳಸಿದ; i++)
    {
      ಆದರೆ (controls[i].regexpr)
        {
          ಗಾಗಿ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_regexp (&controls[i], j);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಗಾಗಿ (intmax_t j = 0; (controls[i].repeat_forever
                       || j <= controls[i].repeat); j++)
            process_ಸಾಲು_ಎಣಿಕೆ (&controls[i], j);
        }
    }

  create_output_file ();
  dump_rest_ದ_file ();
  close_output_file ();
}

/* Return the name of output file number NUM.

   This function is called from a signal handler, so it should invoke
   only reentrant functions that are async-signal-safe.  POSIX does
   not guarantee this for the functions called below, but we don't
   know of any hosts where this implementation isn't safe.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
make_filename (ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
  strcpy (filename_space, ಪೂರ್ವಪ್ರತ್ಯಯ);
  ಆದರೆ (suffix)
    sprintf (filename_space + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ), suffix, ಅಂಕೆ);
  ಇಲ್ಲದಿದ್ದರೆ
    sprintf (filename_space + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ), "%0*d", digits, ಅಂಕೆ);
  ಹಿಂತಿರುಗಿ filename_space;
}

/* Create the next output file. */

ಸ್ಥಿರ ಶೂನ್ಯ
create_output_file (ಶೂನ್ಯ)
{
  ಪೂರ್ಣಾಂಕ nfiles = files_created;
  bool fopen_ok;
  ಪೂರ್ಣಾಂಕ fopen_errno;

  output_filename = make_filename (nfiles);

  ಆದರೆ (nfiles == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
    {
      fopen_ok = false;
      fopen_errno = EOVERFLOW;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Create the output file in a critical section, to avoid races.  */
      sigset_t oldset;
      sigprocmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &oldset);
      output_ಹರಿವು = fopen (output_filename, "w");
      fopen_ok = (output_ಹರಿವು != ಖಾಲಿ);
      fopen_errno = errno;
      files_created = nfiles + fopen_ok;
      sigprocmask (SIG_SETMASK, &oldset, ಖಾಲಿ);
    }

  ಆದರೆ (! fopen_ok)
    {
      error (0, fopen_errno, "%s", quotef (output_filename));
      cleanup_fatal ();
    }
  ಬೈಟುಗಳು_written = 0;
}

/* If requested, delete all the files we have created.  This function
   must be called only from critical sections.  */

ಸ್ಥಿರ ಶೂನ್ಯ
delete_all_files (bool ಒಳ_signal_handler)
{
  ಆದರೆ (! remove_files)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = files_created; 0 <= --i; )
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *name = make_filename (i);
      ಆದರೆ (unlink (name) != 0 && errno != ENOENT && !ಒಳ_signal_handler)
        error (0, errno, "%s", quotef (name));
    }

  files_created = 0;
}

/* Close the current output file and print the count
   of characters in this file. */

ಸ್ಥಿರ ಶೂನ್ಯ
close_output_file (ಶೂನ್ಯ)
{
  ಆದರೆ (output_ಹರಿವು)
    {
      ಆದರೆ (ferror (output_ಹರಿವು))
        {
          error (0, 0, _("write error for %s"), quoteaf (output_filename));
          output_ಹರಿವು = ಖಾಲಿ;
          cleanup_fatal ();
        }
      ಆದರೆ (fclose (output_ಹರಿವು) != 0)
        {
          error (0, errno, "%s", quotef (output_filename));
          output_ಹರಿವು = ಖಾಲಿ;
          cleanup_fatal ();
        }
      ಆದರೆ (ಬೈಟುಗಳು_written == 0 && elide_ಖಾಲಿಗಳು_files)
        {
          sigset_t oldset;
          bool unlink_ok;
          ಪೂರ್ಣಾಂಕ unlink_errno;

          /* Remove the output file in a critical section, to avoid races.  */
          sigprocmask (SIG_BLOCK, &caught_ಸಂಕೇತಗಳು, &oldset);
          unlink_ok = (unlink (output_filename) == 0);
          unlink_errno = errno;
          files_created--;
          sigprocmask (SIG_SETMASK, &oldset, ಖಾಲಿ);

          ಆದರೆ (! unlink_ok && unlink_errno != ENOENT)
            error (0, unlink_errno, "%s", quotef (output_filename));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (!suppress_ಎಣಿಕೆ)
            fprintf (stdout, "%jd\n", ಬೈಟುಗಳು_written);
        }
      output_ಹರಿವು = ಖಾಲಿ;
    }
}

/* Save line LINE to the output file and
   increment the character count for the current file. */

ಸ್ಥಿರ ಶೂನ್ಯ
save_ಸಾಲು_ಗಮ್ಯ_file (ಸ್ಥಿರಾಂಕ ರಚನೆ cstring *ಸಾಲು)
{
  idx_t l = fwrite (ಸಾಲು->str, ಗಾತ್ರ (ಅಕ್ಷರ), ಸಾಲು->len, output_ಹರಿವು);
  ಆದರೆ (l != ಸಾಲು->len)
    {
      error (0, errno, _("write error for %s"), quoteaf (output_filename));
      output_ಹರಿವು = ಖಾಲಿ;
      cleanup_fatal ();
    }
  ಬೈಟುಗಳು_written += ಸಾಲು->len;
}

/* Return a new, initialized control record. */

ಸ್ಥಿರ ರಚನೆ control *
new_control_ದಾಖಲೆ (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ idx_t control_allocated = 0; /* Total space allocated. */
  ರಚನೆ control *p;

  ಆದರೆ (control_ಬಳಸಿದ == control_allocated)
    controls = xpalloc (controls, &control_allocated, 1, -1, ಗಾತ್ರ *controls);
  p = &controls[control_ಬಳಸಿದ++];
  p->regexpr = false;
  p->repeat = 0;
  p->repeat_forever = false;
  p->ಸಾಲುಗಳು_required = 0;
  p->ಸ್ಥಾನಾಂತರ = 0;
  ಹಿಂತಿರುಗಿ p;
}

/* Check if there is a numeric offset after a regular expression.
   STR is the entire command line argument.
   P is the control record for this regular expression.
   NUM is the numeric part of STR. */

ಸ್ಥಿರ ಶೂನ್ಯ
check_for_ಸ್ಥಾನಾಂತರ (ರಚನೆ control *p, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಂಕೆ)
{
  ಆದರೆ (xstrtoimax (ಅಂಕೆ, ಖಾಲಿ, 10, &p->ಸ್ಥಾನಾಂತರ, "") != LONGINT_OK)
    error (EXIT_FAILURE, 0, _("%s: integer expected after delimiter"),
           quote (str));
}

/* Given that the first character of command line arg STR is '{',
   make sure that the rest of the string is a valid repeat count
   and store its value in P.
   ARGNUM is the ARGV index of STR. */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_repeat_ಎಣಿಕೆ (ಪೂರ್ಣಾಂಕ argnum, ರಚನೆ control *p, ಅಕ್ಷರ *str)
{
  ಅಕ್ಷರ *ಕೊನೆ;

  ಕೊನೆ = str + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (str) - 1;
  ಆದರೆ (*ಕೊನೆ != '}')
    error (EXIT_FAILURE, 0, _("%s: '}' is required in repeat count"),
           quote (str));
  *ಕೊನೆ = '\0';

  ಆದರೆ (str + 1 == ಕೊನೆ - 1 && *(str + 1) == '*')
    p->repeat_forever = true;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      uintmax_t val;
      ಆದರೆ (xstrtoumax (str + 1, ಖಾಲಿ, 10, &val, "") != LONGINT_OK
          || ckd_ಕೂಡಿಸು (&p->repeat, val, 0))
        {
          error (EXIT_FAILURE, 0,
                 _("%s}: integer required between '{' and '}'"),
                 quote (global_argv[argnum]));
        }
    }

  *ಕೊನೆ = '}';
}

/* Extract the regular expression from STR and check for a numeric offset.
   STR should start with the regexp delimiter character.
   Return a new control record for the regular expression.
   ARGNUM is the ARGV index of STR.
   Unless IGNORE is true, mark these lines for output. */

ಸ್ಥಿರ ರಚನೆ control *
ಹೊರತೆಗೆ_regexp (ಪೂರ್ಣಾಂಕ argnum, bool ignore, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  idx_t len;			/* Number of bytes in this regexp. */
  ಅಕ್ಷರ delim = *str;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *closing_delim;
  ರಚನೆ control *p;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *err;

  closing_delim = strrchr (str + 1, delim);
  ಆದರೆ (closing_delim == ಖಾಲಿ)
    error (EXIT_FAILURE, 0,
           _("%s: closing delimiter '%c' missing"), str, delim);

  len = closing_delim - str - 1;
  p = new_control_ದಾಖಲೆ ();
  p->argnum = argnum;
  p->ignore = ignore;

  p->regexpr = true;
  p->re_compiled.ಬಫರ್ = ಖಾಲಿ;
  p->re_compiled.allocated = 0;
  p->re_compiled.fastmap = xmalloc (UCHAR_MAX + 1);
  p->re_compiled.translate = ಖಾಲಿ;
  re_syntax_options =
    RE_SYNTAX_POSIX_BASIC & ~RE_CONTEXT_INVALID_DUP & ~RE_NO_ಖಾಲಿಗಳು_RANGES;
  err = re_compile_ನಮೂನೆ (str + 1, len, &p->re_compiled);
  ಆದರೆ (err)
    {
      error (0, 0, _("%s: invalid regular expression: %s"), quote (str), err);
      cleanup_fatal ();
    }

  ಆದರೆ (closing_delim[1])
    check_for_ಸ್ಥಾನಾಂತರ (p, str, closing_delim + 1);

  ಹಿಂತಿರುಗಿ p;
}

/* Extract the break patterns from args START through ARGC - 1 of ARGV.
   After each pattern, check if the next argument is a repeat count. */

ಸ್ಥಿರ ಶೂನ್ಯ
parse_patterns (ಪೂರ್ಣಾಂಕ argc, ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಅಕ್ಷರ **argv)
{
  ರಚನೆ control *p;		/* New control record created. */
  ಸ್ಥಿರ intmax_t ಕೊನೆಯ_val = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = ಆರಂಭ; i < argc; i++)
    {
      ಆದರೆ (*argv[i] == '/' || *argv[i] == '%')
        {
          p = ಹೊರತೆಗೆ_regexp (i, *argv[i] == '%', argv[i]);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          p = new_control_ದಾಖಲೆ ();
          p->argnum = i;

          uintmax_t val;
          ಆದರೆ (xstrtoumax (argv[i], ಖಾಲಿ, 10, &val, "") != LONGINT_OK
              || INTMAX_MAX < val)
            error (EXIT_FAILURE, 0, _("%s: invalid pattern"), quote (argv[i]));
          ಆದರೆ (val == 0)
            error (EXIT_FAILURE, 0,
                   _("%s: line number must be greater than zero"), argv[i]);
          ಆದರೆ (val < ಕೊನೆಯ_val)
            error (EXIT_FAILURE, 0,
                   _("line number %s is smaller than preceding line number,"
                     " %jd"), quote (argv[i]), ಕೊನೆಯ_val);

          ಆದರೆ (val == ಕೊನೆಯ_val)
            error (0, 0,
           _("warning: line number %s is the same as preceding line number"),
                   quote (argv[i]));

          ಕೊನೆಯ_val = val;

          p->ಸಾಲುಗಳು_required = val;
        }

      ಆದರೆ (i + 1 < argc && *argv[i + 1] == '{')
        {
          /* We have a repeat count. */
          i++;
          parse_repeat_ಎಣಿಕೆ (i, p, argv[i]);
        }
    }
}



/* Names for the printf format flags ' and #.  These can be ORed together.  */
ಎಣಿಕೆ { FLAG_THOUSANDS = 1, FLAG_ALTERNATIVE = 2 };

/* Scan the printf format flags in FORMAT, storing info about the
   flags into *FLAGS_PTR.  Return the number of flags found.  */
ಸ್ಥಿರ idx_t
get_format_flags (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *format, ಪೂರ್ಣಾಂಕ *flags_ptr)
{
  ಪೂರ್ಣಾಂಕ flags = 0;

  ಗಾಗಿ (idx_t ಎಣಿಕೆ_2 = 0; ; ಎಣಿಕೆ_2++)
    {
      ಆಯ್ಕೆ (format[ಎಣಿಕೆ_2])
        {
        ಸ್ಥಿತಿ '-':
        ಸ್ಥಿತಿ '0':
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '\'':
          flags |= FLAG_THOUSANDS;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '#':
          flags |= FLAG_ALTERNATIVE;
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          *flags_ptr = flags;
          ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
        }
    }
}

/* Check that the printf format conversion specifier *FORMAT is valid
   and compatible with FLAGS.  Change it to 'd' if it is 'u',
   since the format will be used with a signed value.  */
ಸ್ಥಿರ ಶೂನ್ಯ
check_format_conv_type (ಅಕ್ಷರ *format, ಪೂರ್ಣಾಂಕ flags)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch = *format;
  ಪೂರ್ಣಾಂಕ compatible_flags = FLAG_THOUSANDS;

  ಆಯ್ಕೆ (ch)
    {
    ಸ್ಥಿತಿ 'd':
    ಸ್ಥಿತಿ 'i':
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'u':
      *format = 'd';
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'o':
    ಸ್ಥಿತಿ 'x':
    ಸ್ಥಿತಿ 'X':
      compatible_flags = FLAG_ALTERNATIVE;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 0:
      error (EXIT_FAILURE, 0, _("missing conversion specifier in suffix"));

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಆದರೆ (isprint (ch))
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: %c"), ch);
      ಇಲ್ಲದಿದ್ದರೆ
        error (EXIT_FAILURE, 0,
               _("invalid conversion specifier in suffix: \\%.3o"), ch);
    }

  ಆದರೆ (flags & ~ compatible_flags)
    error (EXIT_FAILURE, 0,
           _("invalid flags in conversion specification: %%%c%c"),
           (flags & ~ compatible_flags & FLAG_ALTERNATIVE ? '#' : '\''), ch);
}

/* Return the maximum number of bytes that can be generated by
   applying FORMAT to an int value.  If the format is
   invalid, diagnose the problem and exit.  */
ಸ್ಥಿರ idx_t
max_ಹೊರಹೋಗು (ಅಕ್ಷರ *format)
{
  bool percent = false;

  ಗಾಗಿ (ಅಕ್ಷರ *f = format; *f; f++)
    ಆದರೆ (*f == '%' && *++f != '%')
      {
        ಆದರೆ (percent)
          error (EXIT_FAILURE, 0,
                 _("too many %% conversion specifications in suffix"));
        percent = true;
        ಪೂರ್ಣಾಂಕ flags;
        f += get_format_flags (f, &flags);
        ಆಗ (c_isdigit (*f))
          f++;
        ಆದರೆ (*f == '.')
          ಆಗ (c_isdigit (*++f))
            ಮುಂದುವರಿ;
        check_format_conv_type (f, flags);
      }

  ಆದರೆ (! percent)
    error (EXIT_FAILURE, 0,
           _("missing %% conversion specification in suffix"));

  ಪೂರ್ಣಾಂಕ maxlen = snprintf (ಖಾಲಿ, 0, format, ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
  ಆದರೆ (! (0 <= maxlen && maxlen <= IDX_MAX))
    xalloc_die ();
  ಹಿಂತಿರುಗಿ maxlen;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  global_argv = argv;

  ಆಗ ((optc = getopt_long (argc, argv, "f:b:kn:sqz", longopts, ಖಾಲಿ))
         != -1)
    ಆಯ್ಕೆ (optc)
      {
      ಸ್ಥಿತಿ 'f':
        ಪೂರ್ವಪ್ರತ್ಯಯ = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'b':
        suffix = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'k':
        remove_files = false;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'n':
        digits = xdectoimax (optarg, 0, MIN (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ, IDX_MAX), "",
                             _("invalid number"), 0);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 's':
      ಸ್ಥಿತಿ 'q':
        suppress_ಎಣಿಕೆ = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'z':
        elide_ಖಾಲಿಗಳು_files = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ SUPPRESS_MATCHED_OPTION:
        suppress_matched = true;
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  ಆದರೆ (argc - optind < 2)
    {
      ಆದರೆ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      usage (EXIT_FAILURE);
    }

  idx_t ಪೂರ್ವಪ್ರತ್ಯಯ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಪೂರ್ವಪ್ರತ್ಯಯ);
  idx_t max_digit_string_len
    = (suffix
       ? max_ಹೊರಹೋಗು (suffix)
       : MAX (INT_STRLEN_BOUND (ಪೂರ್ಣಾಂಕ), digits));
  idx_t filename_ಗಾತ್ರ;
  ಆದರೆ (ckd_ಕೂಡಿಸು (&filename_ಗಾತ್ರ, ಪೂರ್ವಪ್ರತ್ಯಯ_len, max_digit_string_len + 1))
    xalloc_die ();
  filename_space = ximalloc (filename_ಗಾತ್ರ);

  ಗಣ_ಒಳಹರಿವು_file (argv[optind++]);

  parse_patterns (argc, optind, argv);

  {
    ಎಣಿಕೆ { nsigs = countof (ಪದ_sig) };

    ರಚನೆ sigaction act;

    sigemptyset (&caught_ಸಂಕೇತಗಳು);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsigs; i++)
      {
        sigaction (ಪದ_sig[i], ಖಾಲಿ, &act);
        ಆದರೆ (act.sa_handler != SIG_IGN)
          sigaddset (&caught_ಸಂಕೇತಗಳು, ಪದ_sig[i]);
      }

    act.sa_handler = ಅಡಚಣೆ_handler;
    act.sa_mask = caught_ಸಂಕೇತಗಳು;
    act.sa_flags = 0;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < nsigs; i++)
      ಆದರೆ (sigismember (&caught_ಸಂಕೇತಗಳು, ಪದ_sig[i]))
        sigaction (ಪದ_sig[i], &act, ಖಾಲಿ);
  }

  ವಿಭಜಿಸು_file ();

  ಆದರೆ (close (STDIN_FILENO) != 0)
    {
      error (0, errno, _("read error"));
      cleanup_fatal ();
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... FILE ನಮೂನೆ...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Output pieces ದ FILE separated by ನಮೂನೆ(s) ಗಮ್ಯ files 'xx00', 'xx01', ...,\n\
and output byte counts ದ each piece ಗಮ್ಯ standard output.\n\
"), stdout);
       fputs (_("\
\n\
Read standard ಒಳಹರಿವು ಆದರೆ FILE is -\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -b, --suffix-format=FORMAT\n\
         use sprintf FORMAT instead ದ %02d\n\
"));
      oputs (_("\
  -f, --ಪೂರ್ವಪ್ರತ್ಯಯ=ಪೂರ್ವಪ್ರತ್ಯಯ_2\n\
         use ಪೂರ್ವಪ್ರತ್ಯಯ_2 instead ದ 'xx'\n\
"));
      oputs (_("\
  -k, --keep-files\n\
         ಮಾಡು not remove output files on errors\n\
"));
      oputs (_("\
      --suppress-matched\n\
         suppress the ಸಾಲುಗಳು matching ನಮೂನೆ\n\
"));
      oputs (_("\
  -n, --digits=DIGITS\n\
         use specified ಸಂಖ್ಯೆ ದ digits instead ದ 2\n\
"));
      oputs (_("\
  -s, --quiet, --silent\n\
         ಮಾಡು not print counts ದ output file sizes\n\
"));
      oputs (_("\
  -z, --elide-ಖಾಲಿಗಳು-files\n\
         suppress ಖಾಲಿಗಳು output files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Each ನಮೂನೆ may be:\n\
  INTEGER            copy ಮೇಲೆ ಗಮ್ಯ but not including specified ಸಾಲು ಸಂಖ್ಯೆ\n\
  /REGEXP/[ಸ್ಥಾನಾಂತರ_2]   copy ಮೇಲೆ ಗಮ್ಯ but not including a matching ಸಾಲು\n\
  %REGEXP%[ಸ್ಥಾನಾಂತರ_2]   skip ಗಮ್ಯ, but not including a matching ಸಾಲು\n\
  {INTEGER}          repeat the ಹಿಂದಿನ ನಮೂನೆ_2 specified ಸಂಖ್ಯೆ ದ times\n\
  {*}                repeat the ಹಿಂದಿನ ನಮೂನೆ_2 as many times as possible\n\
\n\
A ಸಾಲು ಸ್ಥಾನಾಂತರ_2 is an integer optionally preceded by '+' or '-'\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}
