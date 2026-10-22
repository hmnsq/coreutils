/* tail -- output the last part of file(s)
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Can display any amount of data, unlike the Unix version, which uses
   a fixed size buffer and therefore can only deliver a limited number
   of lines.

   Original version by Paul Rubin <phr@ocf.berkeley.edu>.
   Extensions by David MacKenzie <djm@gnu.ai.mit.edu>.
   tail -f for multiple files by Ian Lance Taylor <ian@airs.com>.
   inotify back-end by Giuseppe Scrivano <gscrivano@gnu.org>.  */

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "cl-strtod.h"
#అన్తర్భావ "dtimespec-bound.h"
#అన్తర్భావ "fcntl--.h"
#అన్తర్భావ "iopoll.h"
#అన్తర్భావ "isapipe.h"
#అన్తర్భావ "issymlink.h"
#అన్తర్భావ "posixver.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-size.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "xbinary-io.h"
#అన్తర్భావ "xdectoint.h"
#అన్తర్భావ "xnanosleep.h"

#ఒకవేళ HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#అయితే_ముగింపు

/* Linux can optimize the handling of local files.  */
#ఒకవేళ defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# ఒకవేళ HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#అయితే_ముగింపు

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tail"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#నిర్వచించు DEFAULT_N_పంక్తులు 10

/* Special values for dump_remainder's N_BYTES parameter.  */
గణన { COPY_గమ్యం_EOF = -1, COPY_A_బఫరు = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
రకనిర్వచనం intmax_t లెక్క_t;
#నిర్వచించు లెక్క_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#నిర్వచించు DEFAULT_FOLLOW_బహుళకం Follow_descriptor

గణన Follow_బహుళకం
{
  /* Follow the name of each file: if the file is renamed, try to reopen
     that name and track the end of the new file if/when it's recreated.
     This is useful for tracking logs that are occasionally rotated.  */
  Follow_name = 1,

  /* Follow each descriptor obtained upon opening a file.
     That means we'll continue to follow the end of a file even after
     it has been renamed or unlinked.  */
  Follow_descriptor = 2
};

/* The types of files for which tail works.  */
#నిర్వచించు IS_TAILABLE_FILE_TYPE(బహుళకం_3) \
  (S_ISREG (బహుళకం_3) || S_ISFIFO (బహుళకం_3) || S_ISSOCK (బహుళకం_3) || S_ISCHR (బహుళకం_3))

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి follow_బహుళకం_string[] =
{
  "descriptor", "name", ఖాళీ
};

స్థిర గణన Follow_బహుళకం స్థిరరాశి follow_బహుళకం_map[] =
{
  Follow_descriptor, Follow_name,
};

నిర్మాణం File_spec
{
  /* The actual file name, or "-" for stdin.  */
  అక్షరం *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  అక్షరం స్థిరరాశి *prettyname;

  /* Attributes of the file the last time we checked.  */
  నిర్మాణం timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t బహుళకం_2;

  /* If a regular file, the file's read position the last time we
     checked.  For non-regular files, the value is unspecified.  */
  off_t read_pos;

  /* The specified name initially referred to a directory or some other
     type for which tail isn't meaningful.  Unlike for a permission problem
     (tailable, below) once this is set, the name is not checked ever again.  */
  bool ignore;

  /* See the description of fremote.  */
  bool remote;

  /* A file is tailable if it exists, is readable, and is of type
     IS_TAILABLE_FILE_TYPE.  */
  bool tailable;

  /* File descriptor on which the file is open; -1 if it's not open.  */
  పూర్ణాంకం fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  పూర్ణాంకం errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  పూర్ణాంకం blocking;

#ఒకవేళ HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  పూర్ణాంకం wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  పూర్ణాంకం మాతృ_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_ప్రారంభం;
#అయితే_ముగింపు

  /* See description of DEFAULT_MAX_N_... below.  */
  లెక్క_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
స్థిర bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
స్థిర bool లెక్క_పంక్తులు = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
స్థిర గణన Follow_బహుళకం follow_బహుళకం = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
స్థిర bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
స్థిర bool monitor_output;

/* If true, count from start of file instead of end.  */
స్థిర bool from_ప్రారంభం;

/* If true, print filename headers.  */
స్థిర bool print_శీర్షికలు;

/* Character to split lines by. */
స్థిర అక్షరం పంక్తి_చివర = '\n';

/* When to print the filename banners.  */
గణన శీర్షిక_బహుళకం
{
  గుణిజం_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
గణన { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
స్థిర లెక్క_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
స్థిర పూర్ణాంకం nbpids = 0;
స్థిర pid_t * pids = ఖాళీ;
స్థిర idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
స్థిర idx_t పుట_కొలత;

/* True if we have ever read standard input.  */
స్థిర bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
స్థిర bool presume_ఆదానం_pipe;

/* If nonzero then don't use inotify even if available.  */
స్థిర bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
స్థిర bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_ఆదానం_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_కార్యక్రమం_OPTION,
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"bytes", required_వాదం, ఖాళీ, 'c'},
  {"debug", no_వాదం, ఖాళీ, DEBUG_కార్యక్రమం_OPTION},
  {"follow", optional_వాదం, ఖాళీ, LONG_FOLLOW_OPTION},
  {"lines", required_వాదం, ఖాళీ, 'n'},
  {"max-unchanged-stats", required_వాదం, ఖాళీ,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_వాదం, ఖాళీ,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_వాదం, ఖాళీ, PID_OPTION},
  {"-presume-input-pipe", no_వాదం, ఖాళీ,
   PRESUME_ఆదానం_PIPE_OPTION}, /* do not document */
  {"quiet", no_వాదం, ఖాళీ, 'q'},
  {"retry", no_వాదం, ఖాళీ, RETRY_OPTION},
  {"silent", no_వాదం, ఖాళీ, 'q'},
  {"sleep-interval", required_వాదం, ఖాళీ, 's'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      రాయి (_("\
Print the చివరి %d పంక్తులు యొక్క each FILE గమ్యం standard output.\n\
With మరిన్ని than one FILE, precede each with a శీర్షిక giving the file name.\n\
"), DEFAULT_N_పంక్తులు);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --బైట్లు=[+]అంకె_2\n\
         output the చివరి అంకె_2 బైట్లు;\n\
         or use -c +అంకె_2 గమ్యం output starting with byte అంకె_2 యొక్క each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is వాడిన\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended దత్తాంశం as the file grows;\n\
         an absent option వాదం means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --పంక్తులు=[+]అంకె_2\n\
         output the చివరి అంకె_2 పంక్తులు, instead యొక్క the చివరి %d;\n\
         or use -n +అంకె_2 గమ్యం skip అంకె_2-1 పంక్తులు at the ప్రారంభం\n\
"), DEFAULT_N_పంక్తులు);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         మారింది కొలత after N (అప్రమేయ %d) iterations\n\
         గమ్యం see ఒకవేళ it has been unlinked or renamed\n\
         (this is the usual స్థితి యొక్క rotated సంవర్గమానం files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, నిష్క్రమించు after PID no longer exists;\n\
         can be repeated గమ్యం watch గుణిజం processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output శీర్షికలు giving file పేర్లు\n\
"));
     oputs (_("\
      --retry\n\
         keep trying గమ్యం open a file ఒకవేళ it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep కొరకు approximately N seconds\n\
         (అప్రమేయ 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at కనిష్ఠ once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output శీర్షికలు giving file పేర్లు\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         పంక్తి delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (సంస్కరణ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
అంకె_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on కొరకు T, P, E, Z, Y, R, Q.\n\
ద్విమాన_2 prefixes can be వాడిన, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), తోక defaults గమ్యం following the file descriptor, which\n\
means that even ఒకవేళ a తోక'ed file is renamed, తోక will కొనసాగు గమ్యం track\n\
its చివర.  This అప్రమేయ behavior is not desirable when you really want గమ్యం\n\
track the actual name యొక్క the file, not the file descriptor (e.g., సంవర్గమానం\n\
rotation).  Use --follow=name లోపల that స్థితి.  That causes తోక గమ్యం track the\n\
named file లోపల a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
స్థిర శూన్య
die_pipe (శూన్య)
{
  లేపు (SIGPIPE);
  నిష్క్రమించు (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
స్థిర శూన్య
check_output_alive (శూన్య)
{
  ఒకవేళ (! monitor_output)
    తిరిగి;

  ఒకవేళ (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_వాడని స్థిర bool
valid_file_spec (నిర్మాణం File_spec స్థిరరాశి *f)
{
  /* Exactly one of the following subexpressions must be true. */
  తిరిగి (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

స్థిర off_t
xlseek (పూర్ణాంకం fd, off_t స్థానభ్రంశం, పూర్ణాంకం whence, అక్షరం స్థిరరాశి *prettyname)
{
  off_t new_స్థానభ్రంశం = lseek (fd, స్థానభ్రంశం, whence);

  ఒకవేళ (0 <= new_స్థానభ్రంశం)
    తిరిగి new_స్థానభ్రంశం;

  స్థిర అక్షరం స్థిరరాశి *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = స్థానభ్రంశం;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

స్థిర శూన్య
రికార్డు_open_fd (నిర్మాణం File_spec *f, పూర్ణాంకం fd,
                off_t read_pos, నిర్మాణం stat స్థిరరాశి *st,
                పూర్ణాంకం blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->బహుళకం_2 = st->st_బహుళకం;
  ఒకవేళ (S_ISREG (st->st_బహుళకం))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

స్థిర శూన్య
close_fd (పూర్ణాంకం fd, నిర్మాణం File_spec స్థిరరాశి *f)
{
  ఒకవేళ (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

స్థిర శూన్య
write_శీర్షిక (అక్షరం స్థిరరాశి *prettyname)
{
  స్థిర bool మొదటి_file = true;

  రాయి ("%s==> %s <==\n", మొదటి_file ? "" : "\n", quotef (prettyname));
  మొదటి_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

స్థిర శూన్య
xwrite_stdout (అక్షరం స్థిరరాశి *బఫరు, idx_t n_బైట్లు)
{
  ఒకవేళ (n_బైట్లు > 0 && fwrite (బఫరు, 1, n_బైట్లు, stdout) < n_బైట్లు)
    {
      clearerr (stdout); /* To avoid redundant close_stdout diagnostic.  */
      error (EXIT_FAILURE, errno, _("error writing %s"),
             quoteaf ("standard output"));
    }
}

/* Read and output bytes from a file, starting at its current position.
   If WANT_HEADER, if there is output then output a header before it.
   The file's name is PRETTYNAME and its file descriptor is FD.
   If N_BYTES is nonnegative, read and output at most N_BYTES.
   If N_BYTES is COPY_TO_EOF, copy until end of file.
   If N_BYTES is COPY_A_BUFFER, copy at most one buffer's worth.
   Return the number of bytes read from the file.  */

స్థిర లెక్క_t
dump_remainder (bool want_శీర్షిక, అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd,
                లెక్క_t n_బైట్లు)
{
  లెక్క_t n_read = 0;
  లెక్క_t n_మిగిలిన = n_బైట్లు;

  చేయి
    {
      అక్షరం బఫరు[BUFSIZ];
      idx_t n = n_బైట్లు < 0 || BUFSIZ < n_మిగిలిన ? BUFSIZ : n_మిగిలిన;
      ssize_t బైట్లు_read = పఠ (fd, బఫరు, n);
      ఒకవేళ (బైట్లు_read < 0)
        {
          ఒకవేళ (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          ఆపు;
        }
      ఒకవేళ (బైట్లు_read == 0)
        ఆపు;
      n_read += బైట్లు_read;

      ఒకవేళ (want_శీర్షిక)
        {
          write_శీర్షిక (prettyname);
          want_శీర్షిక = false;
        }
      xwrite_stdout (బఫరు, బైట్లు_read);
      ఒకవేళ (n_బైట్లు == COPY_A_బఫరు)
        ఆపు;

      n_మిగిలిన -= బైట్లు_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  అయితే (n_మిగిలిన != 0);

  తిరిగి n_read;
}

/* Print the last N_LINES lines from the end of file FD.
   Go backward through the file, reading 'BUFSIZ' bytes at a time (except
   probably the first), until we hit the start of the file or have
   read NUMBER newlines.
   START_POS is the starting position of the read pointer for the file
   associated with FD (may be nonzero).
   END_POS is the file offset of EOF (one larger than offset of last byte).
   The file is a regular file positioned at END_POS.
   Return (-1 - errno) on failure, otherwise the resulting file offset,
   which can be less than READ_POS if the file shrank.  */

స్థిర off_t
file_పంక్తులు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *sb,
            లెక్క_t n_పంక్తులు, off_t ప్రారంభం_pos, off_t చివర_pos)
{
  అక్షరం *బఫరు;
  idx_t bufsize = BUFSIZ;
  off_t pos = చివర_pos;

  ఒకవేళ (n_పంక్తులు == 0)
    తిరిగి pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_బహుళకం));
  ఒకవేళ (sb->st_కొలత % పుట_కొలత == 0)
    bufsize = MAX (BUFSIZ, పుట_కొలత);

  బఫరు = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t బైట్లు_గమ్యం_read = (pos - ప్రారంభం_pos) % bufsize;
  ఒకవేళ (బైట్లు_గమ్యం_read == 0)
    బైట్లు_గమ్యం_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -బైట్లు_గమ్యం_read, SEEK_CUR, prettyname);
  ssize_t బైట్లు_read = పఠ (fd, బఫరు, బైట్లు_గమ్యం_read);
  ఒకవేళ (బైట్లు_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      వెళ్ళు free_బఫరు;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  ఒకవేళ (బైట్లు_read && బఫరు[బైట్లు_read - 1] != పంక్తి_చివర)
    --n_పంక్తులు;

  చేయి
    {
      pos += బైట్లు_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = బైట్లు_read;
      అయితే (n)
        {
          అక్షరం స్థిరరాశి *nl;
          nl = memrchr (బఫరు, పంక్తి_చివర, n);
          ఒకవేళ (nl == ఖాళీ)
            ఆపు;
          n = nl - బఫరు;
          ఒకవేళ (n_పంక్తులు-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, బైట్లు_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, చివర_pos - pos);
              వెళ్ళు free_బఫరు;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ఒకవేళ (pos - బైట్లు_read == ప్రారంభం_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (బఫరు, బైట్లు_read);
          dump_remainder (false, prettyname, fd, చివర_pos - pos);
          వెళ్ళు free_బఫరు;
        }

      pos = xlseek (fd, -(bufsize + బైట్లు_read), SEEK_CUR, prettyname);
      బైట్లు_read = పఠ (fd, బఫరు, bufsize);
      ఒకవేళ (బైట్లు_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          వెళ్ళు free_బఫరు;
        }
    }
  అయితే (బైట్లు_read > 0);

free_బఫరు:
  విడుదల (బఫరు);
  తిరిగి pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

స్థిర పూర్ణాంకం
pipe_పంక్తులు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, లెక్క_t n_పంక్తులు)
{
  నిర్మాణం linebuffer
  {
    అక్షరం బఫరు[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    నిర్మాణం linebuffer *తదుపరి;
  };
  రకనిర్వచనం నిర్మాణం linebuffer LBUFFER;
  LBUFFER *మొదటి, *చివరి, *tmp;
  idx_t మొత్తం_పంక్తులు = 0;	/* Total number of newlines in all buffers.  */
  పూర్ణాంకం ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  మొదటి = చివరి = xmalloc (పరిమాణం (LBUFFER));
  మొదటి->nbytes = మొదటి->nlines = 0;
  మొదటి->తదుపరి = ఖాళీ;
  tmp = xmalloc (పరిమాణం (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  అయితే (true)
    {
      n_read = పఠ (fd, tmp->బఫరు, BUFSIZ);
      ఒకవేళ (n_read <= 0)
        ఆపు;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->తదుపరి = ఖాళీ;

      /* Count the number of newlines just read.  */
      {
        అక్షరం స్థిరరాశి *బఫరు_చివర = tmp->బఫరు + n_read;
        అక్షరం స్థిరరాశి *p = tmp->బఫరు;
        అయితే ((p = memchr (p, పంక్తి_చివర, బఫరు_చివర - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      మొత్తం_పంక్తులు += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ఒకవేళ (tmp->nbytes + చివరి->nbytes < BUFSIZ)
        {
          జ్ఞాపకం_నకలు (&చివరి->బఫరు[చివరి->nbytes], tmp->బఫరు, tmp->nbytes);
          చివరి->nbytes += tmp->nbytes;
          చివరి->nlines += tmp->nlines;
        }
      లేకపోతే
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          చివరి = చివరి->తదుపరి = tmp;
          ఒకవేళ (మొత్తం_పంక్తులు - మొదటి->nlines > n_పంక్తులు)
            {
              tmp = మొదటి;
              మొత్తం_పంక్తులు -= మొదటి->nlines;
              మొదటి = మొదటి->తదుపరి;
            }
          లేకపోతే
            tmp = xmalloc (పరిమాణం (LBUFFER));
        }
    }

  విడుదల (tmp);

  ఒకవేళ (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      వెళ్ళు free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  ఒకవేళ (చివరి->nbytes == 0)
    వెళ్ళు free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  ఒకవేళ (n_పంక్తులు == 0)
    వెళ్ళు free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  ఒకవేళ (చివరి->బఫరు[చివరి->nbytes - 1] != పంక్తి_చివర)
    {
      ++చివరి->nlines;
      ++మొత్తం_పంక్తులు;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  కొరకు (tmp = మొదటి; మొత్తం_పంక్తులు - tmp->nlines > n_పంక్తులు; tmp = tmp->తదుపరి)
    మొత్తం_పంక్తులు -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    అక్షరం స్థిరరాశి *beg = tmp->బఫరు;
    అక్షరం స్థిరరాశి *బఫరు_చివర = tmp->బఫరు + tmp->nbytes;
    ఒకవేళ (మొత్తం_పంక్తులు > n_పంక్తులు)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        కొరకు (idx_t j = మొత్తం_పంక్తులు - n_పంక్తులు; j; --j)
          {
            beg = rawmemchr (beg, పంక్తి_చివర);
            ++beg;
          }
      }

    xwrite_stdout (beg, బఫరు_చివర - beg);
  }

  కొరకు (tmp = tmp->తదుపరి; tmp; tmp = tmp->తదుపరి)
    xwrite_stdout (tmp->బఫరు, tmp->nbytes);

free_lbuffers:
  అయితే (మొదటి)
    {
      tmp = మొదటి->తదుపరి;
      విడుదల (మొదటి);
      మొదటి = tmp;
    }
  తిరిగి ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

స్థిర off_t
pipe_బైట్లు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, లెక్క_t n_బైట్లు,
            off_t read_pos)
{
  నిర్మాణం charbuffer
  {
    అక్షరం బఫరు[BUFSIZ];
    idx_t nbytes;
    నిర్మాణం charbuffer *తదుపరి;
  };
  రకనిర్వచనం నిర్మాణం charbuffer CBUFFER;
  CBUFFER *మొదటి, *చివరి, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t మొత్తం_బైట్లు = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  ఒకవేళ (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  మొదటి = చివరి = xmalloc (పరిమాణం (CBUFFER));
  మొదటి->nbytes = 0;
  మొదటి->తదుపరి = ఖాళీ;
  tmp = xmalloc (పరిమాణం (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  అయితే (true)
    {
      n_read = పఠ (fd, tmp->బఫరు, BUFSIZ);
      ఒకవేళ (n_read <= 0)
        ఆపు;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->తదుపరి = ఖాళీ;

      మొత్తం_బైట్లు += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      ఒకవేళ (tmp->nbytes + చివరి->nbytes < BUFSIZ)
        {
          జ్ఞాపకం_నకలు (&చివరి->బఫరు[చివరి->nbytes], tmp->బఫరు, tmp->nbytes);
          చివరి->nbytes += tmp->nbytes;
        }
      లేకపోతే
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          చివరి = చివరి->తదుపరి = tmp;
          ఒకవేళ (మొత్తం_బైట్లు - మొదటి->nbytes > n_బైట్లు)
            {
              tmp = మొదటి;
              మొత్తం_బైట్లు -= మొదటి->nbytes;
              మొదటి = మొదటి->తదుపరి;
            }
          లేకపోతే
            {
              tmp = xmalloc (పరిమాణం (CBUFFER));
            }
        }
    }

  విడుదల (tmp);

  ఒకవేళ (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      వెళ్ళు free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  కొరకు (tmp = మొదటి; మొత్తం_బైట్లు - tmp->nbytes > n_బైట్లు; tmp = tmp->తదుపరి)
    మొత్తం_బైట్లు -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  ఒకవేళ (మొత్తం_బైట్లు > n_బైట్లు)
    i = మొత్తం_బైట్లు - n_బైట్లు;
  లేకపోతే
    i = 0;
  xwrite_stdout (&tmp->బఫరు[i], tmp->nbytes - i);

  కొరకు (tmp = tmp->తదుపరి; tmp; tmp = tmp->తదుపరి)
    xwrite_stdout (tmp->బఫరు, tmp->nbytes);

  ఒకవేళ (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  అయితే (మొదటి)
    {
      tmp = మొదటి->తదుపరి;
      విడుదల (మొదటి);
      మొదటి = tmp;
    }
  తిరిగి read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

స్థిర పూర్ణాంకం
ప్రారంభం_బైట్లు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, లెక్క_t n_బైట్లు)
{
  అక్షరం బఫరు[BUFSIZ];

  అయితే (0 < n_బైట్లు)
    {
      ssize_t బైట్లు_read = పఠ (fd, బఫరు, BUFSIZ);
      ఒకవేళ (బైట్లు_read == 0)
        తిరిగి -1;
      ఒకవేళ (బైట్లు_read < 0)
        {
          పూర్ణాంకం ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          తిరిగి ret;
        }
      ఒకవేళ (బైట్లు_read <= n_బైట్లు)
        n_బైట్లు -= బైట్లు_read;
      లేకపోతే
        {
          xwrite_stdout (&బఫరు[n_బైట్లు], బైట్లు_read - n_బైట్లు);
          ఆపు;
        }
    }

  తిరిగి 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

స్థిర పూర్ణాంకం
ప్రారంభం_పంక్తులు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, లెక్క_t n_పంక్తులు)
{
  ఒకవేళ (n_పంక్తులు == 0)
    తిరిగి 0;

  అయితే (true)
    {
      అక్షరం బఫరు[BUFSIZ];
      ssize_t బైట్లు_read = పఠ (fd, బఫరు, BUFSIZ);
      ఒకవేళ (బైట్లు_read == 0) /* EOF */
        తిరిగి -1;
      ఒకవేళ (బైట్లు_read < 0) /* error */
        {
          పూర్ణాంకం ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          తిరిగి ret;
        }

      అక్షరం *బఫరు_చివర = బఫరు + బైట్లు_read;
      అక్షరం *p = బఫరు;
      అయితే ((p = memchr (p, పంక్తి_చివర, బఫరు_చివర - p)))
        {
          ++p;
          ఒకవేళ (--n_పంక్తులు == 0)
            {
              ఒకవేళ (p < బఫరు_చివర)
                xwrite_stdout (p, బఫరు_చివర - p);
              తిరిగి 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
స్థిర bool
fremote (పూర్ణాంకం fd, నిర్మాణం File_spec స్థిరరాశి *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#ఒకవేళ HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  నిర్మాణం statfs buf;
  పూర్ణాంకం err = fstatfs (fd, &buf);
  ఒకవేళ (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      ఒకవేళ (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  లేకపోతే
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#అయితే_ముగింపు

  తిరిగి remote;
}

/* open/fstat F->name and handle changes.  */
స్థిర శూన్య
recheck (నిర్మాణం File_spec *f, bool blocking)
{
  నిర్మాణం stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  పూర్ణాంకం prev_errnum = f->errnum;
  bool new_file;
  పూర్ణాంకం fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  పూర్ణాంకం open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  ఒకవేళ (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  లేకపోతే ఒకవేళ (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      ఒకవేళ (fd < 0)
        {
          ఒకవేళ (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          లేకపోతే
            {
              /* say nothing... it's still not tailable */
            }
        }
      లేకపోతే ఒకవేళ (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  లేకపోతే ఒకవేళ (!IS_TAILABLE_FILE_TYPE (new_stats.st_బహుళకం))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_బహుళకం == Follow_name);
      ఒకవేళ (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  లేకపోతే ఒకవేళ ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  లేకపోతే
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  ఒకవేళ (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  లేకపోతే ఒకవేళ (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  లేకపోతే ఒకవేళ (f->fd < 0)
    {
      /* A new file even when inodes haven't changed as <dev,inode>
         pairs can be reused, and we know the file was missing
         on the previous iteration.  Note this also means the file
         is redisplayed in --follow=name mode if renamed away from
         and back to a monitored name.  */
      new_file = true;

      error (0, 0,
             _("%s has appeared;  following new file"),
             quoteaf (f->prettyname));
    }
  లేకపోతే ఒకవేళ (!SAME_INODE (*f, new_stats))
    {
      /* File has been replaced (e.g., via log rotation) --
        tail the new one.  */
      new_file = true;

      error (0, 0,
             _("%s has been replaced;  following new file"),
             quoteaf (f->prettyname));

      /* Close the old one.  */
      close_fd (f->fd, f);
    }
  లేకపోతే
    {
      /* No changes detected, so close new fd.  */
      close_fd (fd, f);
    }

  /* FIXME: When a log is rotated, daemons tend to log to the
     old file descriptor until the new file is present and
     the daemon is sent a signal.  Therefore tail may miss entries
     being written to the old file.  Perhaps we should keep
     the older file open and continue to monitor it until
     data is written to a new file.  */
  ఒకవేళ (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      రికార్డు_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

స్థిర bool
any_సజీవ_files (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  ఒకవేళ (reopen_inaccessible_files && follow_బహుళకం == Follow_name)
    తిరిగి true;

  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      ఒకవేళ (0 <= f[i].fd)
        తిరిగి true;
      లేకపోతే
        {
          ఒకవేళ (! f[i].ignore && reopen_inaccessible_files)
            తిరిగి true;
        }
    }

  తిరిగి false;
}

/* Return true if any writers exist, even as zombies.  */

స్థిర bool
some_writers_exist (శూన్య)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  కొరకు (idx_t i = 0; i < nbpids; )
    {
      ఒకవేళ (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * పరిమాణం pids[i]);
        }
      లేకపోతే
        i++;
    }


  తిరిగి 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

స్థిర శూన్య
తోక_forever (నిర్మాణం File_spec *f, పూర్ణాంకం n_files, ద్విగుణ sleep_interval)
{
  పూర్ణాంకం చివరి = n_files - 1;

  స్థిర bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  ఒకవేళ (fflush (stdout) < 0)
    write_error ();

  అయితే (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_బహుళకం == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].బహుళకం_2));

      ఒకవేళ (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_ఆదానం = false;

      కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
        {
          నిర్మాణం stat stats;

          ఒకవేళ (f[i].ignore)
            కొనసాగు;

          పూర్ణాంకం fd = f[i].fd;
          ఒకవేళ (fd < 0)
            {
              recheck (&f[i], blocking);
              కొనసాగు;
            }

          అక్షరం స్థిరరాశి *prettyname = f[i].prettyname;
          mode_t బహుళకం_2 = f[i].బహుళకం_2;

          ఒకవేళ (f[i].blocking != blocking)
            {
              పూర్ణాంకం old_flags = fcntl (fd, F_GETFL);
              పూర్ణాంకం new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              ఒకవేళ (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  ఒకవేళ (S_ISREG (f[i].బహుళకం_2) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  లేకపోతే
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              లేకపోతే
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          ఒకవేళ (!f[i].blocking)
            {
              ఒకవేళ (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  కొనసాగు;
                }

              ఒకవేళ (f[i].బహుళకం_2 == stats.st_బహుళకం
                  && (! S_ISREG (stats.st_బహుళకం)
                      || f[i].read_pos == stats.st_కొలత)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  ఒకవేళ ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_బహుళకం == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  ఒకవేళ (fd != f[i].fd || S_ISREG (stats.st_బహుళకం) || 1 < n_files)
                    కొనసాగు;
                  లేకపోతే
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].బహుళకం_2 = stats.st_బహుళకం;

              /* reset counter */
              ఒకవేళ (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              ఒకవేళ (S_ISREG (బహుళకం_2) && stats.st_కొలత < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              ఒకవేళ (i != చివరి)
                {
                  ఒకవేళ (print_శీర్షికలు)
                    write_శీర్షిక (prettyname);
                  చివరి = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          లెక్క_t బైట్లు_గమ్యం_read;
          ఒకవేళ (f[i].blocking)
            బైట్లు_గమ్యం_read = COPY_A_బఫరు;
          లేకపోతే ఒకవేళ (S_ISREG (బహుళకం_2) && f[i].remote)
            బైట్లు_గమ్యం_read = stats.st_కొలత - f[i].read_pos;
          లేకపోతే
            బైట్లు_గమ్యం_read = COPY_గమ్యం_EOF;

          లెక్క_t nr = dump_remainder (false, prettyname, fd, బైట్లు_గమ్యం_read);
          ఒకవేళ (0 < nr)
            {
              ఒకవేళ (S_ISREG (బహుళకం_2))
                f[i].read_pos += nr;
              ఒకవేళ (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_ఆదానం = true;
            }
        }

      ఒకవేళ (! any_సజీవ_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          ఆపు;
        }

      ఒకవేళ ((!any_ఆదానం || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      ఒకవేళ (!any_ఆదానం)
        {
          ఒకవేళ (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              ఒకవేళ (!nbpids)
                ఆపు;
              ఒకవేళ (!some_writers_exist ())
                కొనసాగు;
            }

          ఒకవేళ (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#ఒకవేళ HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

స్థిర bool
any_remote_file (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ఒకవేళ (0 <= f[i].fd && f[i].remote)
      తిరిగి true;
  తిరిగి false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

స్థిర bool
any_non_remote_file (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ఒకవేళ (0 <= f[i].fd && ! f[i].remote)
      తిరిగి true;
  తిరిగి false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

స్థిర bool
any_symlinks (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ఒకవేళ (issymlink (f[i].name) == 1)
      తిరిగి true;
  తిరిగి false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

స్థిర bool
any_non_regular_fifo (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ఒకవేళ (0 <= f[i].fd && ! S_ISREG (f[i].బహుళకం_2) && ! S_ISFIFO (f[i].బహుళకం_2))
      తిరిగి true;
  తిరిగి false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

స్థిర bool
tailable_stdin (స్థిరరాశి నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ఒకవేళ (!f[i].ignore && streq (f[i].name, "-"))
      తిరిగి true;
  తిరిగి false;
}

స్థిర size_t
wd_hasher (స్థిరరాశి శూన్య *నమోదు_2, size_t tabsize)
{
  స్థిరరాశి నిర్మాణం File_spec *spec = నమోదు_2;
  తిరిగి spec->wd % tabsize;
}

స్థిర bool
wd_comparator (స్థిరరాశి శూన్య *e1, స్థిరరాశి శూన్య *e2)
{
  స్థిరరాశి నిర్మాణం File_spec *spec1 = e1;
  స్థిరరాశి నిర్మాణం File_spec *spec2 = e2;
  తిరిగి spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
స్థిర size_t
మాతృ_name_hasher (స్థిరరాశి శూన్య *నమోదు_2, size_t tabsize)
{
  స్థిరరాశి నిర్మాణం File_spec *spec = నమోదు_2;
  అక్షరం స్థిరరాశి *name = spec->name + spec->basename_ప్రారంభం;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  తిరిగి (hash_string (name, tabsize) + (size_t) spec->మాతృ_wd) % tabsize;
}

స్థిర bool
మాతృ_name_comparator (స్థిరరాశి శూన్య *e1, స్థిరరాశి శూన్య *e2)
{
  స్థిరరాశి నిర్మాణం File_spec *a = e1;
  స్థిరరాశి నిర్మాణం File_spec *b = e2;
  తిరిగి (a->మాతృ_wd == b->మాతృ_wd
          && streq (a->name + a->basename_ప్రారంభం,
                    b->name + b->basename_ప్రారంభం));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
స్థిర శూన్య
check_fspec (నిర్మాణం File_spec *fspec, నిర్మాణం File_spec **prev_fspec)
{
  ఒకవేళ (fspec->fd < 0)
    తిరిగి;

  నిర్మాణం stat stats;
  ఒకవేళ (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      తిరిగి;
    }

  అక్షరం స్థిరరాశి *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  ఒకవేళ (S_ISREG (fspec->బహుళకం_2) && stats.st_కొలత < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  లేకపోతే ఒకవేళ (S_ISREG (fspec->బహుళకం_2) && stats.st_కొలత == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    తిరిగి;

  bool want_శీర్షిక = print_శీర్షికలు && (fspec != *prev_fspec);

  లెక్క_t nr = dump_remainder (want_శీర్షిక, prettyname, fspec->fd, COPY_గమ్యం_EOF);
  ఒకవేళ (0 < nr)
    {
      ఒకవేళ (S_ISREG (fspec->బహుళకం_2))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      ఒకవేళ (fflush (stdout) < 0)
        write_error ();
    }
}

/* Attempt to tail N_FILES files forever, or until killed.
   Check modifications using the inotify events system.
   Exit if finished or on fatal error; return to revert to polling.

   On return (to revert to polling), *WD_TO_NAMEP and *PARENT_BY_NAMEP
   hold hash tables the caller must free.  *PARENT_BY_NAMEP is non-NULL
   only when follow_mode == Follow_name; otherwise it is set to NULL.
   Like WD_TO_NAME, PARENT_BY_NAME is owned by the caller so return
   paths need not free it (and must not call hash_free (NULL)).  */
స్థిర శూన్య
తోక_forever_inotify (పూర్ణాంకం wd, నిర్మాణం File_spec *f, పూర్ణాంకం n_files,
                      ద్విగుణ sleep_interval, Hash_పట్టిక **wd_గమ్యం_namep,
                      Hash_పట్టిక **మాతృ_by_namep)
{
# ఒకవేళ తోక_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  పూర్ణాంకం max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_పట్టిక *wd_గమ్యం_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_పట్టిక *మాతృ_by_name = ఖాళీ;

  bool దొరికింది_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool దొరికింది_unwatchable_dir = false;
  bool no_inotify_resources = false;
  నిర్మాణం File_spec *prev_fspec;
  idx_t evlen = 0;
  అక్షరం *evbuf;
  idx_t evbuf_off = 0;

  *మాతృ_by_namep = ఖాళీ;

  wd_గమ్యం_name = hash_initialize (n_files, ఖాళీ, wd_hasher, wd_comparator,
                                ఖాళీ);
  ఒకవేళ (! wd_గమ్యం_name)
    xalloc_die ();
  *wd_గమ్యం_namep = wd_గమ్యం_name;

  ఒకవేళ (follow_బహుళకం == Follow_name)
    {
      మాతృ_by_name = hash_initialize (n_files, ఖాళీ, మాతృ_name_hasher,
                                        మాతృ_name_comparator, ఖాళీ);
      ఒకవేళ (! మాతృ_by_name)
        xalloc_die ();
      *మాతృ_by_namep = మాతృ_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_ముసుగు = లోపల_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  ఒకవేళ (follow_బహుళకం == Follow_name)
    inotify_wd_ముసుగు |= (లోపల_ATTRIB | లోపల_DELETE_SELF | లోపల_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      ఒకవేళ (!f[i].ignore)
        {
          idx_t fnlen = అక్షరశ్రేణి_పొడవు (f[i].name);
          ఒకవేళ (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          ఒకవేళ (follow_బహుళకం == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              అక్షరం prev = f[i].name[dirlen];
              f[i].basename_ప్రారంభం = చివరి_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].మాతృ_wd = inotify_కూడు_watch (wd, dirlen ? f[i].name : ".",
                                                  (లోపల_CREATE | లోపల_DELETE
                                                   | లోపల_MOVED_గమ్యం | లోపల_ATTRIB
                                                   | లోపల_DELETE_SELF));

              f[i].name[dirlen] = prev;

              ఒకవేళ (f[i].మాతృ_wd < 0)
                {
                  ఒకవేళ (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  లేకపోతే
                    error (0, 0, _("inotify resources exhausted"));
                  దొరికింది_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  ఆపు;
                }

              ఒకవేళ (hash_చొప్పించు (మాతృ_by_name, &(f[i])) == ఖాళీ)
                xalloc_die ();
            }

          f[i].wd = inotify_కూడు_watch (wd, f[i].name, inotify_wd_ముసుగు);

          ఒకవేళ (f[i].wd < 0)
            {
              ఒకవేళ (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              ఒకవేళ (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  ఆపు;
                }
              లేకపోతే ఒకవేళ (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              కొనసాగు;
            }

          ఒకవేళ (hash_చొప్పించు (wd_గమ్యం_name, &(f[i])) == ఖాళీ)
            xalloc_die ();

          దొరికింది_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  ఒకవేళ (no_inotify_resources || దొరికింది_unwatchable_dir
      || (follow_బహుళకం == Follow_descriptor && tailed_but_unwatchable))
    తిరిగి;
  ఒకవేళ (follow_బహుళకం == Follow_descriptor && !దొరికింది_watchable_file)
    నిష్క్రమించు (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      ఒకవేళ (! f[i].ignore)
        {
          /* check for new files.  */
          ఒకవేళ (follow_బహుళకం == Follow_name)
            recheck (&(f[i]), false);
          లేకపోతే ఒకవేళ (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              నిర్మాణం stat stats;

              ఒకవేళ (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  తిరిగి;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  ఒకవేళ (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += పరిమాణం (నిర్మాణం inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  అయితే (true)
    {
      నిర్మాణం File_spec *fspec;
      నిర్మాణం inotify_event *ev;
      శూన్య *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      ఒకవేళ (follow_బహుళకం == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_గమ్యం_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      ఒకవేళ (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          పూర్ణాంకం file_change;
          నిర్మాణం pollfd pfd[2];
          చేయి
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              పూర్ణాంకం delay = -1;

              ఒకవేళ (pids)
                {
                  ఒకవేళ (!nbpids)
                    నిష్క్రమించు (EXIT_SUCCESS);

                  ఒకవేళ (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  లేకపోతే ఒకవేళ (sleep_interval < గరిష్ఠ_పూర్ణాంకం / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      ద్విగుణ ddelay = sleep_interval * 1000;
                      delay = ddelay;
                      delay += delay < ddelay;
                    }
                }

              pfd[0].fd = wd;
              pfd[0].events = POLLIN;
              pfd[1].fd = STDOUT_FILENO;
              pfd[1].events = pfd[1].revents = 0;
              file_change = poll (pfd, monitor_output + 1, delay);
            }
          అయితే (file_change == 0 || (file_change < 0 && errno == EINTR));

          ఒకవేళ (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          ఒకవేళ (pfd[1].revents)
            die_pipe ();

          len = పఠ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          ఒకవేళ ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              కొనసాగు;
            }

          ఒకవేళ (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += పరిమాణం (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      ఒకవేళ ((ev->ముసుగు & లోపల_DELETE_SELF) && ! ev->len)
        {
          కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
            {
              ఒకవేళ (ev->wd == f[i].మాతృ_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  తిరిగి;
                }
            }
        }

      ఒకవేళ (ev->len) /* event on ev->name in watched directory.  */
        {
          నిర్మాణం File_spec కీలకం;
          ఒకవేళ (! మాతృ_by_name)
            కొనసాగు;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          కీలకం.మాతృ_wd = ev->wd;
          కీలకం.name = ev->name;
          కీలకం.basename_ప్రారంభం = 0;
          fspec = hash_lookup (మాతృ_by_name, &కీలకం);

          /* It is not a watched file.  */
          ఒకవేళ (! fspec)
            కొనసాగు;

          పూర్ణాంకం new_wd = -1;
          bool deleting = !! (ev->ముసుగు & లోపల_DELETE);

          ఒకవేళ (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_కూడు_watch (wd, fspec->name, inotify_wd_ముసుగు);
            }

          ఒకవేళ (! deleting && new_wd < 0)
            {
              ఒకవేళ (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  తిరిగి; /* revert to polling.  */
                }
              లేకపోతే
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          ఒకవేళ (new_watch)
            {
              ఒకవేళ (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_గమ్యం_name, fspec);
                }

              fspec->wd = new_wd;

              ఒకవేళ (new_wd < 0)
                కొనసాగు;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              నిర్మాణం File_spec *prev = hash_remove (wd_గమ్యం_name, fspec);
              ఒకవేళ (prev && prev != fspec)
                {
                  ఒకవేళ (follow_బహుళకం == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              ఒకవేళ (hash_చొప్పించు (wd_గమ్యం_name, fspec) == ఖాళీ)
                xalloc_die ();
            }

          ఒకవేళ (follow_బహుళకం == Follow_name)
            recheck (fspec, false);
        }
      లేకపోతే
        {
          నిర్మాణం File_spec కీలకం;
          కీలకం.wd = ev->wd;
          fspec = hash_lookup (wd_గమ్యం_name, &కీలకం);
        }

      ఒకవేళ (! fspec)
        కొనసాగు;

      ఒకవేళ (ev->ముసుగు & (లోపల_ATTRIB | లోపల_DELETE | లోపల_DELETE_SELF | లోపల_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          ఒకవేళ (ev->ముసుగు & లోపల_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->ముసుగు & లోపల_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_గమ్యం_name, fspec);
            }

          /* Note we get IN_ATTRIB for unlink() as st_nlink decrements.
             The usual path is a close() done in recheck() triggers
             an IN_DELETE_SELF event as the inode is removed.
             However sometimes open() will succeed as even though
             st_nlink is decremented, the dentry (cache) is not updated.
             Thus we depend on the IN_DELETE event on the directory
             to trigger processing for the removed file.  */

          recheck (fspec, false);

          కొనసాగు;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#అయితే_ముగింపు

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

స్థిర off_t
తోక_బైట్లు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st,
            లెక్క_t n_బైట్లు)
{
  off_t current_pos
    = (presume_ఆదానం_pipe
       ? -1
       : lseek (fd, from_ప్రారంభం ? MIN (n_బైట్లు, OFF_T_MAX) : 0, SEEK_CUR));

  ఒకవేళ (from_ప్రారంభం)
    {
      ఒకవేళ (current_pos < 0)
        {
          పూర్ణాంకం t = ప్రారంభం_బైట్లు (prettyname, fd, n_బైట్లు);
          ఒకవేళ (t < 0)
            తిరిగి t;
        }
      n_బైట్లు = COPY_గమ్యం_EOF;
    }
  లేకపోతే
    {
      off_t initial_pos = current_pos;
      off_t చివర_pos = -1;

      ఒకవేళ (0 <= current_pos)
        {
          ఒకవేళ (usable_st_కొలత (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_కొలత = STP_BLKSIZE (st);
              ఒకవేళ (smallish_కొలత < st->st_కొలత)
                చివర_pos = st->st_కొలత;
            }
          లేకపోతే
            {
              off_t minus_n;
              ఒకవేళ (ckd_sub (&minus_n, 0, n_బైట్లు))
                చివర_pos = 0;
              లేకపోతే
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  ఒకవేళ (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      చివర_pos = e_n_pos + n_బైట్లు;
                    }
                  లేకపోతే
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      ఒకవేళ (0 <= e_pos)
                        current_pos = చివర_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < చివర_pos && n_బైట్లు < చివర_pos - initial_pos
                   ? చివర_pos - n_బైట్లు
                   : initial_pos);
      ఒకవేళ (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      ఒకవేళ (చివర_pos < 0)
        తిరిగి pipe_బైట్లు (prettyname, fd, n_బైట్లు, current_pos);
    }

  లెక్క_t nr = dump_remainder (false, prettyname, fd, n_బైట్లు);
  తిరిగి current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

స్థిర off_t
తోక_పంక్తులు (అక్షరం స్థిరరాశి *prettyname, పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st,
            లెక్క_t n_పంక్తులు)
{
  ఒకవేళ (from_ప్రారంభం)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      ఒకవేళ (OFF_T_MAX <= n_పంక్తులు)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          ఒకవేళ (0 <= e)
            తిరిగి e;
        }

      పూర్ణాంకం t = ప్రారంభం_పంక్తులు (prettyname, fd, n_పంక్తులు);
      ఒకవేళ (t < 0)
        తిరిగి t;
      dump_remainder (false, prettyname, fd, COPY_గమ్యం_EOF);
      తిరిగి -1;
    }
  లేకపోతే
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t ప్రారంభం_pos = (!presume_ఆదానం_pipe && S_ISREG (st->st_బహుళకం)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t చివర_pos = ప్రారంభం_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      తిరిగి (చివర_pos < 0
              ? pipe_పంక్తులు (prettyname, fd, n_పంక్తులు)
              : ప్రారంభం_pos < చివర_pos
              ? file_పంక్తులు (prettyname, fd, st, n_పంక్తులు, ప్రారంభం_pos, చివర_pos)
              : ప్రారంభం_pos != చివర_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, ప్రారంభం_pos, SEEK_SET, prettyname))
              : ప్రారంభం_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

స్థిర off_t
తోక (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం fd, నిర్మాణం stat స్థిరరాశి *st, లెక్క_t n_units)
{
  తిరిగి (లెక్క_పంక్తులు ? తోక_పంక్తులు : తోక_బైట్లు) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

స్థిర bool
తోక_file (నిర్మాణం File_spec *f, లెక్క_t n_files, లెక్క_t n_units)
{
  పూర్ణాంకం fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  ఒకవేళ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ద్విమాన_బహుళకం (STDIN_FILENO, O_ద్విమాన);
    }
  లేకపోతే
    fd = open (f->name, O_RDONLY | O_ద్విమాన | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  ఒకవేళ (fd < 0)
    {
      ఒకవేళ (forever)
        {
          f->fd = -1;
          f->errnum = errno;
          f->ignore = ! reopen_inaccessible_files;
          f->st_dev = 0;
          f->st_ino = 0;
        }
      error (0, errno, _("cannot open %s for reading"),
             quoteaf (f->prettyname));
      ok = false;
    }
  లేకపోతే
    {
      ఒకవేళ (print_శీర్షికలు)
        write_శీర్షిక (f->prettyname);

      off_t read_pos;
      నిర్మాణం stat stats;
      ok = 0 <= fstat (fd, &stats);
      ఒకవేళ (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      లేకపోతే
        {
          read_pos = తోక (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          ఒకవేళ (IS_TAILABLE_FILE_TYPE (stats.st_బహుళకం))
            f->tailable = true;
          లేకపోతే ఒకవేళ (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      ఒకవేళ (forever)
        {
          ఒకవేళ (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          లేకపోతే
            {
              రికార్డు_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      లేకపోతే
        {
          ఒకవేళ (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  తిరిగి ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

స్థిర bool
parse_obsolete_option (పూర్ణాంకం argc, అక్షరం * స్థిరరాశి *argv, లెక్క_t *n_units)
{
  bool t_from_ప్రారంభం;
  bool t_లెక్క_పంక్తులు = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  ఒకవేళ (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    తిరిగి false;

  పూర్ణాంకం posix_ver = posix2_సంస్కరణ ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  అక్షరం స్థిరరాశి *p = argv[1];

  ఎంపిక (*p++)
    {
    అప్రమేయ:
      తిరిగి false;

    స్థితి '+':
      /* Leading "+" is a file name in the standard form.  */
      ఒకవేళ (!traditional_usage)
        తిరిగి false;

      t_from_ప్రారంభం = true;
      ఆపు;

    స్థితి '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      ఒకవేళ (!obsolete_usage && !p[p[0] == 'c'])
        తిరిగి false;

      t_from_ప్రారంభం = false;
      ఆపు;
    }

  లెక్క_t n;
  ఒకవేళ (!c_isdigit (*p))
    n = DEFAULT_N_పంక్తులు;
  లేకపోతే
    కొరకు (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_కూడు (&n, n, *p - '0') ? లెక్క_MAX : n;

  ఎంపిక (*p)
    {
    స్థితి 'b': n = ckd_mul (&n, n, 512) ? లెక్క_MAX : n; FALLTHROUGH;
    స్థితి 'c': t_లెక్క_పంక్తులు = false; FALLTHROUGH;
    స్థితి 'l': p++; ఆపు;
    }

  ఒకవేళ (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  ఒకవేళ (*p)
    తిరిగి false;

  *n_units = n;

  /* Set globals.  */
  from_ప్రారంభం = t_from_ప్రారంభం;
  లెక్క_పంక్తులు = t_లెక్క_పంక్తులు;
  forever = t_forever;

  తిరిగి true;
}

స్థిర శూన్య
parse_options (పూర్ణాంకం argc, అక్షరం **argv,
               లెక్క_t *n_units, గణన శీర్షిక_బహుళకం *శీర్షిక_బహుళకం,
               ద్విగుణ *sleep_interval)
{
  పూర్ణాంకం c;

  అయితే ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'F':
          forever = true;
          follow_బహుళకం = Follow_name;
          reopen_inaccessible_files = true;
          ఆపు;

        స్థితి 'c':
        స్థితి 'n':
          లెక్క_పంక్తులు = (c == 'n');
          ఒకవేళ (*optarg == '+')
            from_ప్రారంభం = true;
          లేకపోతే ఒకవేళ (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, లెక్క_MAX, "bkKmMGTPEZYRQ0",
                                 (లెక్క_పంక్తులు
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          ఆపు;

        స్థితి 'f':
        స్థితి LONG_FOLLOW_OPTION:
          forever = true;
          ఒకవేళ (optarg == ఖాళీ)
            follow_బహుళకం = DEFAULT_FOLLOW_బహుళకం;
          లేకపోతే
            follow_బహుళకం = XARGMATCH ("--follow", optarg,
                                     follow_బహుళకం_string, follow_బహుళకం_map);
          ఆపు;

        స్థితి RETRY_OPTION:
          reopen_inaccessible_files = true;
          ఆపు;

        స్థితి MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, లెక్క_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          ఆపు;

        స్థితి DEBUG_కార్యక్రమం_OPTION:
          debug = true;
          ఆపు;

        స్థితి DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          ఆపు;

        స్థితి PID_OPTION:
          ఒకవేళ (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (గరిష్ఠ_పూర్ణాంకం, IDX_MAX), పరిమాణం *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          ఆపు;

        స్థితి PRESUME_ఆదానం_PIPE_OPTION:
          presume_ఆదానం_pipe = true;
          ఆపు;

        స్థితి 'q':
          *శీర్షిక_బహుళకం = never;
          ఆపు;

        స్థితి 's':
          {
            అక్షరం *ep;
            errno = 0;
            ద్విగుణ s = cl_strtod (optarg, &ep);
            ఒకవేళ (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          ఆపు;

        స్థితి 'v':
          *శీర్షిక_బహుళకం = always;
          ఆపు;

        స్థితి 'z':
          పంక్తి_చివర = '\0';
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3': స్థితి '4':
        స్థితి '5': స్థితి '6': స్థితి '7': స్థితి '8': స్థితి '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (reopen_inaccessible_files)
    {
      ఒకవేళ (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      లేకపోతే ఒకవేళ (follow_బహుళకం == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  ఒకవేళ (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      విడుదల (pids);
      pids = ఖాళీ;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
స్థిర bool
ignore_fifo_and_pipe (నిర్మాణం File_spec *f, పూర్ణాంకం n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].బహుళకం_2)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      ఒకవేళ (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      లేకపోతే
        some_viable = true;
    }

  తిరిగి some_viable;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  గణన శీర్షిక_బహుళకం శీర్షిక_బహుళకం = గుణిజం_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  లెక్క_t n_units = DEFAULT_N_పంక్తులు;
  పూర్ణాంకం n_files;
  అక్షరం **file;
  నిర్మాణం File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  ద్విగుణ sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    పూర్ణాంకం p = getpagesize ();
    ఒకవేళ (IDX_MAX < p)
      xalloc_die ();
    పుట_కొలత = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &శీర్షిక_బహుళకం, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_ప్రారంభం && 0 < n_units && n_units < లెక్క_MAX;

  ఒకవేళ (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  లేకపోతే
    {
      స్థిర అక్షరం *dummy_stdin = (అక్షరం *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool దొరికింది_hyphen = false;

    కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
      ఒకవేళ (streq (file[i], "-"))
        దొరికింది_hyphen = true;

    /* When following by name, there must be a name.  */
    ఒకవేళ (దొరికింది_hyphen && follow_బహుళకం == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    ఒకవేళ (forever && దొరికింది_hyphen)
      {
        నిర్మాణం stat లోపల_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_బహుళకం == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &లోపల_stat)
                          && ! S_ISREG (లోపల_stat.st_బహుళకం));

        ఒకవేళ (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  ఒకవేళ (! forever && n_units == (from_ప్రారంభం ? లెక్క_MAX : 0))
    తిరిగి EXIT_SUCCESS;

  ఒకవేళ (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, పరిమాణం *F);
  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  ఒకవేళ (శీర్షిక_బహుళకం == always
      || (శీర్షిక_బహుళకం == గుణిజం_files && n_files > 1))
    print_శీర్షికలు = true;

  xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);

  కొరకు (పూర్ణాంకం i = 0; i < n_files; i++)
    ok &= తోక_file (&F[i], n_files, n_units);

  ఒకవేళ (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      నిర్మాణం stat బహిర్గతం_stat;
      ఒకవేళ (fstat (STDOUT_FILENO, &బహిర్గతం_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (బహిర్గతం_stat.st_బహుళకం)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#ఒకవేళ HAVE_INOTIFY
      /* tailable_stdin() checks if the user specifies stdin via  "-",
         or implicitly by providing no arguments. If so, we won't use inotify.
         Technically, on systems with a working /dev/stdin, we *could*,
         but would it be worth it?  Verifying that it's a real device
         and hooked up to stdin is not trivial, while reverting to
         non-inotify-based tail_forever is easy and portable.

         any_remote_file() checks if the user has specified any
         files that reside on remote file systems.  inotify is not used
         in this case because it would miss any updates to the file
         that were not initiated from the local system.

         any_non_remote_file() checks if the user has specified any
         files that don't reside on remote file systems.  inotify is not used
         if there are no open files, as we can't determine if those file
         will be on a remote file system.

         any_symlinks() checks if the user has specified any symbolic links.
         inotify is not used in this case because it returns updated _targets_
         which would not match the specified names.  If we tried to always
         use the target names, then we would miss changes to the symlink itself.

         ok is false when one of the files specified could not be opened for
         reading.  In this case and when following by descriptor,
         tail_forever_inotify() cannot be used (in its current implementation).

         FIXME: inotify doesn't give any notification when a new
         (remote) file or directory is mounted on top a watched file.
         When follow_mode == Follow_name we would ideally like to detect that.
         Note if there is a change to the original file then we'll
         recheck it and follow the new file, or ignore it if the
         file has changed to being remote.

         FIXME-maybe: inotify has a watch descriptor per inode, and hence with
         our current hash implementation will only --follow data for one
         of the names when multiple hardlinked files are specified, or
         for one name when a name is specified multiple times.  */
      ఒకవేళ (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_బహుళకం == Follow_descriptor)))
        disable_inotify = true;

      ఒకవేళ (!disable_inotify)
        {
          పూర్ణాంకం wd = inotify_init ();
          ఒకవేళ (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              ఒకవేళ (fflush (stdout) < 0)
                write_error ();

              Hash_పట్టిక *ht;
              Hash_పట్టిక *మాతృ_ht = ఖాళీ;
              తోక_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &మాతృ_ht);
              hash_free (ht);
              ఒకవేళ (మాతృ_ht)
                hash_free (మాతృ_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#అయితే_ముగింపు
      disable_inotify = true;
      తోక_forever (F, n_files, sleep_interval);
    }

  ఒకవేళ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
