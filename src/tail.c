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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "argmatch.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "cl-strtod.h"
#ଅନ୍ତର୍ଭାଵ "dtimespec-bound.h"
#ଅନ୍ତର୍ଭାଵ "fcntl--.h"
#ଅନ୍ତର୍ଭାଵ "iopoll.h"
#ଅନ୍ତର୍ଭାଵ "isapipe.h"
#ଅନ୍ତର୍ଭାଵ "issymlink.h"
#ଅନ୍ତର୍ଭାଵ "posixver.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stat-size.h"
#ଅନ୍ତର୍ଭାଵ "stat-time.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"
#ଅନ୍ତର୍ଭାଵ "xdectoint.h"
#ଅନ୍ତର୍ଭାଵ "xnanosleep.h"

#ଯଦି HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#endif

/* Linux can optimize the handling of local files.  */
#ଯଦି defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# ଯଦି HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tail"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#define DEFAULT_N_ଧାଡ଼ିଗୁଡ଼ିକ 10

/* Special values for dump_remainder's N_BYTES parameter.  */
ଗଣନା { COPY_TO_EOF = -1, COPY_A_BUFFER = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
ପ୍ରକାରବ୍ୟାଖ୍ୟା intmax_t ଗଣନା_t;
#define ଗଣନା_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#define DEFAULT_FOLLOW_MODE Follow_descriptor

ଗଣନା Follow_mode
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
#define IS_TAILABLE_FILE_TYPE(Mode) \
  (S_ISREG (Mode) || S_ISFIFO (Mode) || S_ISSOCK (Mode) || S_ISCHR (Mode))

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ follow_mode_string[] =
{
  "descriptor", "name", ଖାଲି
};

ସ୍ଥିର ଗଣନା Follow_mode ଅଚଳ follow_mode_map[] =
{
  Follow_descriptor, Follow_name,
};

ସଂରଚନା File_spec
{
  /* The actual file name, or "-" for stdin.  */
  ଅକ୍ଷର *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  ଅକ୍ଷର ଅଚଳ *prettyname;

  /* Attributes of the file the last time we checked.  */
  ସଂରଚନା timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t mode;

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
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocking;

#ଯଦି HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_start;
#endif

  /* See description of DEFAULT_MAX_N_... below.  */
  ଗଣନା_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ସ୍ଥିର bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ସ୍ଥିର bool ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ସ୍ଥିର ଗଣନା Follow_mode follow_mode = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ସ୍ଥିର bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ସ୍ଥିର bool monitor_output;

/* If true, count from start of file instead of end.  */
ସ୍ଥିର bool from_start;

/* If true, print filename headers.  */
ସ୍ଥିର bool print_headers;

/* Character to split lines by. */
ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_end = '\n';

/* When to print the filename banners.  */
ଗଣନା header_mode
{
  multiple_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
ଗଣନା { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ସ୍ଥିର ଗଣନା_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbpids = 0;
ସ୍ଥିର pid_t * pids = ଖାଲି;
ସ୍ଥିର idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ସ୍ଥିର idx_t page_size;

/* True if we have ever read standard input.  */
ସ୍ଥିର bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ସ୍ଥିର bool presume_ନିବେଶ_pipe;

/* If nonzero then don't use inotify even if available.  */
ସ୍ଥିର bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
ସ୍ଥିର bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_ନିବେଶ_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_PROGRAM_OPTION,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"bytes", required_argument, ଖାଲି, 'c'},
  {"debug", no_argument, ଖାଲି, DEBUG_PROGRAM_OPTION},
  {"follow", optional_argument, ଖାଲି, LONG_FOLLOW_OPTION},
  {"lines", required_argument, ଖାଲି, 'n'},
  {"max-unchanged-stats", required_argument, ଖାଲି,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_argument, ଖାଲି,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_argument, ଖାଲି, PID_OPTION},
  {"-presume-input-pipe", no_argument, ଖାଲି,
   PRESUME_ନିବେଶ_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ଖାଲି, 'q'},
  {"retry", no_argument, ଖାଲି, RETRY_OPTION},
  {"silent", no_argument, ଖାଲି, 'q'},
  {"sleep-interval", required_argument, ଖାଲି, 's'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the last %d ଧାଡ଼ିଗୁଡ଼ିକ of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_N_ଧାଡ଼ିଗୁଡ଼ିକ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --bytes=[+]ଅଙ୍କ_2\n\
         output the last ଅଙ୍କ_2 bytes;\n\
         or use -c +ଅଙ୍କ_2 to output starting with byte ଅଙ୍କ_2 of each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is used\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended data as the file grows;\n\
         an absent option argument means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --ଧାଡ଼ିଗୁଡ଼ିକ=[+]ଅଙ୍କ_2\n\
         output the last ଅଙ୍କ_2 ଧାଡ଼ିଗୁଡ଼ିକ, instead of the last %d;\n\
         or use -n +ଅଙ୍କ_2 to skip ଅଙ୍କ_2-1 ଧାଡ଼ିଗୁଡ଼ିକ at the start\n\
"), DEFAULT_N_ଧାଡ଼ିଗୁଡ଼ିକ);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         changed size after N (ପୂର୍ବନିର୍ଦ୍ଧାରିତ %d) iterations\n\
         to see ଯଦି it has been unlinked or renamed\n\
         (this is the usual ସ୍ଥିତି of rotated log files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, exit after PID no longer exists;\n\
         can be repeated to watch multiple processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output headers giving file names\n\
"));
     oputs (_("\
      --retry\n\
         keep trying to open a file ଯଦି it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep ପାଇଁ approximately N seconds\n\
         (ପୂର୍ବନିର୍ଦ୍ଧାରିତ 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at least once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output headers giving file names\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (VERSION_OPTION_DESCRIPTION);
     fputs (_("\
\n\
ଅଙ୍କ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ପାଇଁ T, P, E, Z, Y, R, Q.\n\
ଦ୍ୱିମିକ_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), tail defaults to following the file descriptor, which\n\
means that even ଯଦି a tail'ed file is renamed, tail will ଜାରି to track\n\
its end.  This ପୂର୍ବନିର୍ଦ୍ଧାରିତ behavior is not desirable when you really want to\n\
track the actual name of the file, not the file descriptor (e.g., log\n\
rotation).  Use --follow=name in that ସ୍ଥିତି.  That causes tail to track the\n\
named file in a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ସ୍ଥିର ଶୂନ୍ୟ
die_pipe (ଶୂନ୍ୟ)
{
  ଉଠାଅ (SIGPIPE);
  exit (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
ସ୍ଥିର ଶୂନ୍ୟ
check_output_alive (ଶୂନ୍ୟ)
{
  ଯଦି (! monitor_output)
    ଫେରନ୍ତୁ;

  ଯଦି (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_UNUSED ସ୍ଥିର bool
valid_file_spec (ସଂରଚନା File_spec ଅଚଳ *f)
{
  /* Exactly one of the following subexpressions must be true. */
  ଫେରନ୍ତୁ (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

ସ୍ଥିର off_t
xlseek (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, off_t offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା whence, ଅକ୍ଷର ଅଚଳ *prettyname)
{
  off_t new_offset = lseek (fd, offset, whence);

  ଯଦି (0 <= new_offset)
    ଫେରନ୍ତୁ new_offset;

  ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = offset;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ସ୍ଥିର ଶୂନ୍ୟ
record_open_fd (ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
                off_t read_pos, ସଂରଚନା stat ଅଚଳ *st,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->mode = st->st_mode;
  ଯଦି (S_ISREG (st->st_mode))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

ସ୍ଥିର ଶୂନ୍ୟ
close_fd (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା File_spec ଅଚଳ *f)
{
  ଯଦି (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

ସ୍ଥିର ଶୂନ୍ୟ
write_header (ଅକ୍ଷର ଅଚଳ *prettyname)
{
  ସ୍ଥିର bool first_file = true;

  printf ("%s==> %s <==\n", first_file ? "" : "\n", quotef (prettyname));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ସ୍ଥିର ଶୂନ୍ୟ
xwrite_stdout (ଅକ୍ଷର ଅଚଳ *buffer, idx_t n_bytes)
{
  ଯଦି (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
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

ସ୍ଥିର ଗଣନା_t
dump_remainder (bool want_header, ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
                ଗଣନା_t n_bytes)
{
  ଗଣନା_t n_read = 0;
  ଗଣନା_t n_remaining = n_bytes;

  କର
    {
      ଅକ୍ଷର buffer[BUFSIZ];
      idx_t n = n_bytes < 0 || BUFSIZ < n_remaining ? BUFSIZ : n_remaining;
      ssize_t bytes_read = ପଠ (fd, buffer, n);
      ଯଦି (bytes_read < 0)
        {
          ଯଦି (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          ଭାଙ୍ଗ;
        }
      ଯଦି (bytes_read == 0)
        ଭାଙ୍ଗ;
      n_read += bytes_read;

      ଯଦି (want_header)
        {
          write_header (prettyname);
          want_header = false;
        }
      xwrite_stdout (buffer, bytes_read);
      ଯଦି (n_bytes == COPY_A_BUFFER)
        ଭାଙ୍ଗ;

      n_remaining -= bytes_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  ଯେତେବେଳେ (n_remaining != 0);

  ଫେରନ୍ତୁ n_read;
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

ସ୍ଥିର off_t
file_ଧାଡ଼ିଗୁଡ଼ିକ (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *sb,
            ଗଣନା_t n_ଧାଡ଼ିଗୁଡ଼ିକ, off_t start_pos, off_t end_pos)
{
  ଅକ୍ଷର *buffer;
  idx_t bufsize = BUFSIZ;
  off_t pos = end_pos;

  ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
    ଫେରନ୍ତୁ pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_mode));
  ଯଦି (sb->st_size % page_size == 0)
    bufsize = MAX (BUFSIZ, page_size);

  buffer = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t bytes_to_read = (pos - start_pos) % bufsize;
  ଯଦି (bytes_to_read == 0)
    bytes_to_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -bytes_to_read, SEEK_CUR, prettyname);
  ssize_t bytes_read = ପଠ (fd, buffer, bytes_to_read);
  ଯଦି (bytes_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ଯାଅ free_buffer;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  ଯଦି (bytes_read && buffer[bytes_read - 1] != ଧାଡ଼ି_end)
    --n_ଧାଡ଼ିଗୁଡ଼ିକ;

  କର
    {
      pos += bytes_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      ଯେତେବେଳେ (n)
        {
          ଅକ୍ଷର ଅଚଳ *nl;
          nl = memrchr (buffer, ଧାଡ଼ି_end, n);
          ଯଦି (nl == ଖାଲି)
            ଭାଙ୍ଗ;
          n = nl - buffer;
          ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, bytes_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, end_pos - pos);
              ଯାଅ free_buffer;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ଯଦି (pos - bytes_read == start_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (buffer, bytes_read);
          dump_remainder (false, prettyname, fd, end_pos - pos);
          ଯାଅ free_buffer;
        }

      pos = xlseek (fd, -(bufsize + bytes_read), SEEK_CUR, prettyname);
      bytes_read = ପଠ (fd, buffer, bufsize);
      ଯଦି (bytes_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ଯାଅ free_buffer;
        }
    }
  ଯେତେବେଳେ (bytes_read > 0);

free_buffer:
  ମୁକ୍ତ (buffer);
  ଫେରନ୍ତୁ pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
pipe_ଧାଡ଼ିଗୁଡ଼ିକ (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଗଣନା_t n_ଧାଡ଼ିଗୁଡ଼ିକ)
{
  ସଂରଚନା linebuffer
  {
    ଅକ୍ଷର buffer[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    ସଂରଚନା linebuffer *next;
  };
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  idx_t total_ଧାଡ଼ିଗୁଡ଼ିକ = 0;	/* Total number of newlines in all buffers.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (ଆକାର (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = ଖାଲି;
  tmp = xmalloc (ଆକାର (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  ଯେତେବେଳେ (true)
    {
      n_read = ପଠ (fd, tmp->buffer, BUFSIZ);
      ଯଦି (n_read <= 0)
        ଭାଙ୍ଗ;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = ଖାଲି;

      /* Count the number of newlines just read.  */
      {
        ଅକ୍ଷର ଅଚଳ *buffer_end = tmp->buffer + n_read;
        ଅକ୍ଷର ଅଚଳ *p = tmp->buffer;
        ଯେତେବେଳେ ((p = memchr (p, ଧାଡ଼ି_end, buffer_end - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_ଧାଡ଼ିଗୁଡ଼ିକ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ଯଦି (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      ଅନ୍ୟଥା
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          ଯଦି (total_ଧାଡ଼ିଗୁଡ଼ିକ - first->nlines > n_ଧାଡ଼ିଗୁଡ଼ିକ)
            {
              tmp = first;
              total_ଧାଡ଼ିଗୁଡ଼ିକ -= first->nlines;
              first = first->next;
            }
          ଅନ୍ୟଥା
            tmp = xmalloc (ଆକାର (LBUFFER));
        }
    }

  ମୁକ୍ତ (tmp);

  ଯଦି (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ଯାଅ free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  ଯଦି (last->nbytes == 0)
    ଯାଅ free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
    ଯାଅ free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  ଯଦି (last->buffer[last->nbytes - 1] != ଧାଡ଼ି_end)
    {
      ++last->nlines;
      ++total_ଧାଡ଼ିଗୁଡ଼ିକ;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  ପାଇଁ (tmp = first; total_ଧାଡ଼ିଗୁଡ଼ିକ - tmp->nlines > n_ଧାଡ଼ିଗୁଡ଼ିକ; tmp = tmp->next)
    total_ଧାଡ଼ିଗୁଡ଼ିକ -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    ଅକ୍ଷର ଅଚଳ *beg = tmp->buffer;
    ଅକ୍ଷର ଅଚଳ *buffer_end = tmp->buffer + tmp->nbytes;
    ଯଦି (total_ଧାଡ଼ିଗୁଡ଼ିକ > n_ଧାଡ଼ିଗୁଡ଼ିକ)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        ପାଇଁ (idx_t j = total_ଧାଡ଼ିଗୁଡ଼ିକ - n_ଧାଡ଼ିଗୁଡ଼ିକ; j; --j)
          {
            beg = rawmemchr (beg, ଧାଡ଼ି_end);
            ++beg;
          }
      }

    xwrite_stdout (beg, buffer_end - beg);
  }

  ପାଇଁ (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

free_lbuffers:
  ଯେତେବେଳେ (first)
    {
      tmp = first->next;
      ମୁକ୍ତ (first);
      first = tmp;
    }
  ଫେରନ୍ତୁ ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ସ୍ଥିର off_t
pipe_bytes (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଗଣନା_t n_bytes,
            off_t read_pos)
{
  ସଂରଚନା charbuffer
  {
    ଅକ୍ଷର buffer[BUFSIZ];
    idx_t nbytes;
    ସଂରଚନା charbuffer *next;
  };
  ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା charbuffer CBUFFER;
  CBUFFER *first, *last, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t total_bytes = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  ଯଦି (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  first = last = xmalloc (ଆକାର (CBUFFER));
  first->nbytes = 0;
  first->next = ଖାଲି;
  tmp = xmalloc (ଆକାର (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  ଯେତେବେଳେ (true)
    {
      n_read = ପଠ (fd, tmp->buffer, BUFSIZ);
      ଯଦି (n_read <= 0)
        ଭାଙ୍ଗ;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->next = ଖାଲି;

      total_bytes += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      ଯଦି (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
        }
      ଅନ୍ୟଥା
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          ଯଦି (total_bytes - first->nbytes > n_bytes)
            {
              tmp = first;
              total_bytes -= first->nbytes;
              first = first->next;
            }
          ଅନ୍ୟଥା
            {
              tmp = xmalloc (ଆକାର (CBUFFER));
            }
        }
    }

  ମୁକ୍ତ (tmp);

  ଯଦି (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ଯାଅ free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  ପାଇଁ (tmp = first; total_bytes - tmp->nbytes > n_bytes; tmp = tmp->next)
    total_bytes -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  ଯଦି (total_bytes > n_bytes)
    i = total_bytes - n_bytes;
  ଅନ୍ୟଥା
    i = 0;
  xwrite_stdout (&tmp->buffer[i], tmp->nbytes - i);

  ପାଇଁ (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

  ଯଦି (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  ଯେତେବେଳେ (first)
    {
      tmp = first->next;
      ମୁକ୍ତ (first);
      first = tmp;
    }
  ଫେରନ୍ତୁ read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
start_bytes (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଗଣନା_t n_bytes)
{
  ଅକ୍ଷର buffer[BUFSIZ];

  ଯେତେବେଳେ (0 < n_bytes)
    {
      ssize_t bytes_read = ପଠ (fd, buffer, BUFSIZ);
      ଯଦି (bytes_read == 0)
        ଫେରନ୍ତୁ -1;
      ଯଦି (bytes_read < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ଫେରନ୍ତୁ ret;
        }
      ଯଦି (bytes_read <= n_bytes)
        n_bytes -= bytes_read;
      ଅନ୍ୟଥା
        {
          xwrite_stdout (&buffer[n_bytes], bytes_read - n_bytes);
          ଭାଙ୍ଗ;
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
start_ଧାଡ଼ିଗୁଡ଼ିକ (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଗଣନା_t n_ଧାଡ଼ିଗୁଡ଼ିକ)
{
  ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
    ଫେରନ୍ତୁ 0;

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର buffer[BUFSIZ];
      ssize_t bytes_read = ପଠ (fd, buffer, BUFSIZ);
      ଯଦି (bytes_read == 0) /* EOF */
        ଫେରନ୍ତୁ -1;
      ଯଦି (bytes_read < 0) /* error */
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ଫେରନ୍ତୁ ret;
        }

      ଅକ୍ଷର *buffer_end = buffer + bytes_read;
      ଅକ୍ଷର *p = buffer;
      ଯେତେବେଳେ ((p = memchr (p, ଧାଡ଼ି_end, buffer_end - p)))
        {
          ++p;
          ଯଦି (--n_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
            {
              ଯଦି (p < buffer_end)
                xwrite_stdout (p, buffer_end - p);
              ଫେରନ୍ତୁ 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
ସ୍ଥିର bool
fremote (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା File_spec ଅଚଳ *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#ଯଦି HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  ସଂରଚନା statfs buf;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = fstatfs (fd, &buf);
  ଯଦି (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      ଯଦି (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  ଅନ୍ୟଥା
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#endif

  ଫେରନ୍ତୁ remote;
}

/* open/fstat F->name and handle changes.  */
ସ୍ଥିର ଶୂନ୍ୟ
recheck (ସଂରଚନା File_spec *f, bool blocking)
{
  ସଂରଚନା stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_errnum = f->errnum;
  bool new_file;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  ଯଦି (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  ଅନ୍ୟଥା ଯଦି (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      ଯଦି (fd < 0)
        {
          ଯଦି (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          ଅନ୍ୟଥା
            {
              /* say nothing... it's still not tailable */
            }
        }
      ଅନ୍ୟଥା ଯଦି (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  ଅନ୍ୟଥା ଯଦି (!IS_TAILABLE_FILE_TYPE (new_stats.st_mode))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_mode == Follow_name);
      ଯଦି (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  ଅନ୍ୟଥା ଯଦି ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  ଅନ୍ୟଥା
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  ଯଦି (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  ଅନ୍ୟଥା ଯଦି (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  ଅନ୍ୟଥା ଯଦି (f->fd < 0)
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
  ଅନ୍ୟଥା ଯଦି (!SAME_INODE (*f, new_stats))
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
  ଅନ୍ୟଥା
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
  ଯଦି (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      record_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ସ୍ଥିର bool
any_live_files (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  ଯଦି (reopen_inaccessible_files && follow_mode == Follow_name)
    ଫେରନ୍ତୁ true;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    {
      ଯଦି (0 <= f[i].fd)
        ଫେରନ୍ତୁ true;
      ଅନ୍ୟଥା
        {
          ଯଦି (! f[i].ignore && reopen_inaccessible_files)
            ଫେରନ୍ତୁ true;
        }
    }

  ଫେରନ୍ତୁ false;
}

/* Return true if any writers exist, even as zombies.  */

ସ୍ଥିର bool
some_writers_exist (ଶୂନ୍ୟ)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  ପାଇଁ (idx_t i = 0; i < nbpids; )
    {
      ଯଦି (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * ଆକାର pids[i]);
        }
      ଅନ୍ୟଥା
        i++;
    }


  ଫେରନ୍ତୁ 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

ସ୍ଥିର ଶୂନ୍ୟ
tail_forever (ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files, ଦ୍ୱିଗୁଣ sleep_interval)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା last = n_files - 1;

  ସ୍ଥିର bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  ଯଦି (fflush (stdout) < 0)
    write_error ();

  ଯେତେବେଳେ (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_mode == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].mode));

      ଯଦି (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_ନିବେଶ = false;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
        {
          ସଂରଚନା stat stats;

          ଯଦି (f[i].ignore)
            ଜାରି;

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = f[i].fd;
          ଯଦି (fd < 0)
            {
              recheck (&f[i], blocking);
              ଜାରି;
            }

          ଅକ୍ଷର ଅଚଳ *prettyname = f[i].prettyname;
          mode_t mode = f[i].mode;

          ଯଦି (f[i].blocking != blocking)
            {
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_flags = fcntl (fd, F_GETFL);
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              ଯଦି (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  ଯଦି (S_ISREG (f[i].mode) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  ଅନ୍ୟଥା
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              ଅନ୍ୟଥା
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          ଯଦି (!f[i].blocking)
            {
              ଯଦି (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  ଜାରି;
                }

              ଯଦି (f[i].mode == stats.st_mode
                  && (! S_ISREG (stats.st_mode)
                      || f[i].read_pos == stats.st_size)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  ଯଦି ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_mode == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  ଯଦି (fd != f[i].fd || S_ISREG (stats.st_mode) || 1 < n_files)
                    ଜାରି;
                  ଅନ୍ୟଥା
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].mode = stats.st_mode;

              /* reset counter */
              ଯଦି (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              ଯଦି (S_ISREG (mode) && stats.st_size < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              ଯଦି (i != last)
                {
                  ଯଦି (print_headers)
                    write_header (prettyname);
                  last = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          ଗଣନା_t bytes_to_read;
          ଯଦି (f[i].blocking)
            bytes_to_read = COPY_A_BUFFER;
          ଅନ୍ୟଥା ଯଦି (S_ISREG (mode) && f[i].remote)
            bytes_to_read = stats.st_size - f[i].read_pos;
          ଅନ୍ୟଥା
            bytes_to_read = COPY_TO_EOF;

          ଗଣନା_t nr = dump_remainder (false, prettyname, fd, bytes_to_read);
          ଯଦି (0 < nr)
            {
              ଯଦି (S_ISREG (mode))
                f[i].read_pos += nr;
              ଯଦି (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_ନିବେଶ = true;
            }
        }

      ଯଦି (! any_live_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          ଭାଙ୍ଗ;
        }

      ଯଦି ((!any_ନିବେଶ || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      ଯଦି (!any_ନିବେଶ)
        {
          ଯଦି (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              ଯଦି (!nbpids)
                ଭାଙ୍ଗ;
              ଯଦି (!some_writers_exist ())
                ଜାରି;
            }

          ଯଦି (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#ଯଦି HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

ସ୍ଥିର bool
any_remote_file (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ଯଦି (0 <= f[i].fd && f[i].remote)
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

ସ୍ଥିର bool
any_non_remote_file (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ଯଦି (0 <= f[i].fd && ! f[i].remote)
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

ସ୍ଥିର bool
any_symlinks (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ଯଦି (issymlink (f[i].name) == 1)
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

ସ୍ଥିର bool
any_non_regular_fifo (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ଯଦି (0 <= f[i].fd && ! S_ISREG (f[i].mode) && ! S_ISFIFO (f[i].mode))
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

ସ୍ଥିର bool
tailable_stdin (ଅଚଳ ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ଯଦି (!f[i].ignore && streq (f[i].name, "-"))
      ଫେରନ୍ତୁ true;
  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର size_t
wd_hasher (ଅଚଳ ଶୂନ୍ୟ *entry, size_t tabsize)
{
  ଅଚଳ ସଂରଚନା File_spec *spec = entry;
  ଫେରନ୍ତୁ spec->wd % tabsize;
}

ସ୍ଥିର bool
wd_comparator (ଅଚଳ ଶୂନ୍ୟ *e1, ଅଚଳ ଶୂନ୍ୟ *e2)
{
  ଅଚଳ ସଂରଚନା File_spec *spec1 = e1;
  ଅଚଳ ସଂରଚନା File_spec *spec2 = e2;
  ଫେରନ୍ତୁ spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
ସ୍ଥିର size_t
parent_name_hasher (ଅଚଳ ଶୂନ୍ୟ *entry, size_t tabsize)
{
  ଅଚଳ ସଂରଚନା File_spec *spec = entry;
  ଅକ୍ଷର ଅଚଳ *name = spec->name + spec->basename_start;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  ଫେରନ୍ତୁ (hash_string (name, tabsize) + (size_t) spec->parent_wd) % tabsize;
}

ସ୍ଥିର bool
parent_name_comparator (ଅଚଳ ଶୂନ୍ୟ *e1, ଅଚଳ ଶୂନ୍ୟ *e2)
{
  ଅଚଳ ସଂରଚନା File_spec *a = e1;
  ଅଚଳ ସଂରଚନା File_spec *b = e2;
  ଫେରନ୍ତୁ (a->parent_wd == b->parent_wd
          && streq (a->name + a->basename_start,
                    b->name + b->basename_start));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
ସ୍ଥିର ଶୂନ୍ୟ
check_fspec (ସଂରଚନା File_spec *fspec, ସଂରଚନା File_spec **prev_fspec)
{
  ଯଦି (fspec->fd < 0)
    ଫେରନ୍ତୁ;

  ସଂରଚନା stat stats;
  ଯଦି (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      ଫେରନ୍ତୁ;
    }

  ଅକ୍ଷର ଅଚଳ *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  ଯଦି (S_ISREG (fspec->mode) && stats.st_size < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  ଅନ୍ୟଥା ଯଦି (S_ISREG (fspec->mode) && stats.st_size == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    ଫେରନ୍ତୁ;

  bool want_header = print_headers && (fspec != *prev_fspec);

  ଗଣନା_t nr = dump_remainder (want_header, prettyname, fspec->fd, COPY_TO_EOF);
  ଯଦି (0 < nr)
    {
      ଯଦି (S_ISREG (fspec->mode))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      ଯଦି (fflush (stdout) < 0)
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
ସ୍ଥିର ଶୂନ୍ୟ
tail_forever_inotify (ପୂର୍ଣ୍ଣସଂଖ୍ୟା wd, ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files,
                      ଦ୍ୱିଗୁଣ sleep_interval, Hash_table **wd_to_namep,
                      Hash_table **parent_by_namep)
{
# ଯଦି TAIL_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_table *wd_to_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_table *parent_by_name = ଖାଲି;

  bool found_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool found_unwatchable_dir = false;
  bool no_inotify_resources = false;
  ସଂରଚନା File_spec *prev_fspec;
  idx_t evlen = 0;
  ଅକ୍ଷର *evbuf;
  idx_t evbuf_off = 0;

  *parent_by_namep = ଖାଲି;

  wd_to_name = hash_initialize (n_files, ଖାଲି, wd_hasher, wd_comparator,
                                ଖାଲି);
  ଯଦି (! wd_to_name)
    xalloc_die ();
  *wd_to_namep = wd_to_name;

  ଯଦି (follow_mode == Follow_name)
    {
      parent_by_name = hash_initialize (n_files, ଖାଲି, parent_name_hasher,
                                        parent_name_comparator, ଖାଲି);
      ଯଦି (! parent_by_name)
        xalloc_die ();
      *parent_by_namep = parent_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_mask = IN_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  ଯଦି (follow_mode == Follow_name)
    inotify_wd_mask |= (IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    {
      ଯଦି (!f[i].ignore)
        {
          idx_t fnlen = strlen (f[i].name);
          ଯଦି (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          ଯଦି (follow_mode == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              ଅକ୍ଷର prev = f[i].name[dirlen];
              f[i].basename_start = last_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].parent_wd = inotify_add_watch (wd, dirlen ? f[i].name : ".",
                                                  (IN_CREATE | IN_DELETE
                                                   | IN_MOVED_TO | IN_ATTRIB
                                                   | IN_DELETE_SELF));

              f[i].name[dirlen] = prev;

              ଯଦି (f[i].parent_wd < 0)
                {
                  ଯଦି (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  ଅନ୍ୟଥା
                    error (0, 0, _("inotify resources exhausted"));
                  found_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  ଭାଙ୍ଗ;
                }

              ଯଦି (hash_ଭର୍ତ୍ତି (parent_by_name, &(f[i])) == ଖାଲି)
                xalloc_die ();
            }

          f[i].wd = inotify_add_watch (wd, f[i].name, inotify_wd_mask);

          ଯଦି (f[i].wd < 0)
            {
              ଯଦି (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              ଯଦି (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  ଭାଙ୍ଗ;
                }
              ଅନ୍ୟଥା ଯଦି (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              ଜାରି;
            }

          ଯଦି (hash_ଭର୍ତ୍ତି (wd_to_name, &(f[i])) == ଖାଲି)
            xalloc_die ();

          found_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  ଯଦି (no_inotify_resources || found_unwatchable_dir
      || (follow_mode == Follow_descriptor && tailed_but_unwatchable))
    ଫେରନ୍ତୁ;
  ଯଦି (follow_mode == Follow_descriptor && !found_watchable_file)
    exit (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    {
      ଯଦି (! f[i].ignore)
        {
          /* check for new files.  */
          ଯଦି (follow_mode == Follow_name)
            recheck (&(f[i]), false);
          ଅନ୍ୟଥା ଯଦି (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              ସଂରଚନା stat stats;

              ଯଦି (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  ଫେରନ୍ତୁ;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  ଯଦି (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += ଆକାର (ସଂରଚନା inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା File_spec *fspec;
      ସଂରଚନା inotify_event *ev;
      ଶୂନ୍ୟ *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      ଯଦି (follow_mode == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_to_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      ଯଦି (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା file_change;
          ସଂରଚନା pollfd pfd[2];
          କର
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା delay = -1;

              ଯଦି (pids)
                {
                  ଯଦି (!nbpids)
                    exit (EXIT_SUCCESS);

                  ଯଦି (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  ଅନ୍ୟଥା ଯଦି (sleep_interval < INT_MAX / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      ଦ୍ୱିଗୁଣ ddelay = sleep_interval * 1000;
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
          ଯେତେବେଳେ (file_change == 0 || (file_change < 0 && errno == EINTR));

          ଯଦି (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          ଯଦି (pfd[1].revents)
            die_pipe ();

          len = ପଠ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          ଯଦି ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              ଜାରି;
            }

          ଯଦି (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += ଆକାର (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      ଯଦି ((ev->mask & IN_DELETE_SELF) && ! ev->len)
        {
          ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
            {
              ଯଦି (ev->wd == f[i].parent_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  ଫେରନ୍ତୁ;
                }
            }
        }

      ଯଦି (ev->len) /* event on ev->name in watched directory.  */
        {
          ସଂରଚନା File_spec ଚାବି;
          ଯଦି (! parent_by_name)
            ଜାରି;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          ଚାବି.parent_wd = ev->wd;
          ଚାବି.name = ev->name;
          ଚାବି.basename_start = 0;
          fspec = hash_lookup (parent_by_name, &ଚାବି);

          /* It is not a watched file.  */
          ଯଦି (! fspec)
            ଜାରି;

          ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_wd = -1;
          bool deleting = !! (ev->mask & IN_DELETE);

          ଯଦି (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_add_watch (wd, fspec->name, inotify_wd_mask);
            }

          ଯଦି (! deleting && new_wd < 0)
            {
              ଯଦି (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  ଫେରନ୍ତୁ; /* revert to polling.  */
                }
              ଅନ୍ୟଥା
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          ଯଦି (new_watch)
            {
              ଯଦି (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_to_name, fspec);
                }

              fspec->wd = new_wd;

              ଯଦି (new_wd < 0)
                ଜାରି;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              ସଂରଚନା File_spec *prev = hash_remove (wd_to_name, fspec);
              ଯଦି (prev && prev != fspec)
                {
                  ଯଦି (follow_mode == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              ଯଦି (hash_ଭର୍ତ୍ତି (wd_to_name, fspec) == ଖାଲି)
                xalloc_die ();
            }

          ଯଦି (follow_mode == Follow_name)
            recheck (fspec, false);
        }
      ଅନ୍ୟଥା
        {
          ସଂରଚନା File_spec ଚାବି;
          ଚାବି.wd = ev->wd;
          fspec = hash_lookup (wd_to_name, &ଚାବି);
        }

      ଯଦି (! fspec)
        ଜାରି;

      ଯଦି (ev->mask & (IN_ATTRIB | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          ଯଦି (ev->mask & IN_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->mask & IN_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_to_name, fspec);
            }

          /* Note we get IN_ATTRIB for unlink() as st_nlink decrements.
             The usual path is a close() done in recheck() triggers
             an IN_DELETE_SELF event as the inode is removed.
             However sometimes open() will succeed as even though
             st_nlink is decremented, the dentry (cache) is not updated.
             Thus we depend on the IN_DELETE event on the directory
             to trigger processing for the removed file.  */

          recheck (fspec, false);

          ଜାରି;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#endif

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ସ୍ଥିର off_t
tail_bytes (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *st,
            ଗଣନା_t n_bytes)
{
  off_t current_pos
    = (presume_ନିବେଶ_pipe
       ? -1
       : lseek (fd, from_start ? MIN (n_bytes, OFF_T_MAX) : 0, SEEK_CUR));

  ଯଦି (from_start)
    {
      ଯଦି (current_pos < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = start_bytes (prettyname, fd, n_bytes);
          ଯଦି (t < 0)
            ଫେରନ୍ତୁ t;
        }
      n_bytes = COPY_TO_EOF;
    }
  ଅନ୍ୟଥା
    {
      off_t initial_pos = current_pos;
      off_t end_pos = -1;

      ଯଦି (0 <= current_pos)
        {
          ଯଦି (usable_st_size (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_size = STP_BLKSIZE (st);
              ଯଦି (smallish_size < st->st_size)
                end_pos = st->st_size;
            }
          ଅନ୍ୟଥା
            {
              off_t minus_n;
              ଯଦି (ckd_sub (&minus_n, 0, n_bytes))
                end_pos = 0;
              ଅନ୍ୟଥା
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  ଯଦି (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      end_pos = e_n_pos + n_bytes;
                    }
                  ଅନ୍ୟଥା
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      ଯଦି (0 <= e_pos)
                        current_pos = end_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < end_pos && n_bytes < end_pos - initial_pos
                   ? end_pos - n_bytes
                   : initial_pos);
      ଯଦି (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      ଯଦି (end_pos < 0)
        ଫେରନ୍ତୁ pipe_bytes (prettyname, fd, n_bytes, current_pos);
    }

  ଗଣନା_t nr = dump_remainder (false, prettyname, fd, n_bytes);
  ଫେରନ୍ତୁ current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ସ୍ଥିର off_t
tail_ଧାଡ଼ିଗୁଡ଼ିକ (ଅକ୍ଷର ଅଚଳ *prettyname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *st,
            ଗଣନା_t n_ଧାଡ଼ିଗୁଡ଼ିକ)
{
  ଯଦି (from_start)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      ଯଦି (OFF_T_MAX <= n_ଧାଡ଼ିଗୁଡ଼ିକ)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          ଯଦି (0 <= e)
            ଫେରନ୍ତୁ e;
        }

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = start_ଧାଡ଼ିଗୁଡ଼ିକ (prettyname, fd, n_ଧାଡ଼ିଗୁଡ଼ିକ);
      ଯଦି (t < 0)
        ଫେରନ୍ତୁ t;
      dump_remainder (false, prettyname, fd, COPY_TO_EOF);
      ଫେରନ୍ତୁ -1;
    }
  ଅନ୍ୟଥା
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t start_pos = (!presume_ନିବେଶ_pipe && S_ISREG (st->st_mode)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t end_pos = start_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      ଫେରନ୍ତୁ (end_pos < 0
              ? pipe_ଧାଡ଼ିଗୁଡ଼ିକ (prettyname, fd, n_ଧାଡ଼ିଗୁଡ଼ିକ)
              : start_pos < end_pos
              ? file_ଧାଡ଼ିଗୁଡ଼ିକ (prettyname, fd, st, n_ଧାଡ଼ିଗୁଡ଼ିକ, start_pos, end_pos)
              : start_pos != end_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, start_pos, SEEK_SET, prettyname))
              : start_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ସ୍ଥିର off_t
tail (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ସଂରଚନା stat ଅଚଳ *st, ଗଣନା_t n_units)
{
  ଫେରନ୍ତୁ (ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ ? tail_ଧାଡ଼ିଗୁଡ଼ିକ : tail_bytes) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ସ୍ଥିର bool
tail_file (ସଂରଚନା File_spec *f, ଗଣନା_t n_files, ଗଣନା_t n_units)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  ଯଦି (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ଦ୍ୱିମିକ_mode (STDIN_FILENO, O_ଦ୍ୱିମିକ);
    }
  ଅନ୍ୟଥା
    fd = open (f->name, O_RDONLY | O_ଦ୍ୱିମିକ | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  ଯଦି (fd < 0)
    {
      ଯଦି (forever)
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
  ଅନ୍ୟଥା
    {
      ଯଦି (print_headers)
        write_header (f->prettyname);

      off_t read_pos;
      ସଂରଚନା stat stats;
      ok = 0 <= fstat (fd, &stats);
      ଯଦି (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      ଅନ୍ୟଥା
        {
          read_pos = tail (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          ଯଦି (IS_TAILABLE_FILE_TYPE (stats.st_mode))
            f->tailable = true;
          ଅନ୍ୟଥା ଯଦି (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      ଯଦି (forever)
        {
          ଯଦି (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          ଅନ୍ୟଥା
            {
              record_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  ଫେରନ୍ତୁ ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

ସ୍ଥିର bool
parse_obsolete_option (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର * ଅଚଳ *argv, ଗଣନା_t *n_units)
{
  bool t_from_start;
  bool t_ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  ଯଦି (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    ଫେରନ୍ତୁ false;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା posix_ver = posix2_version ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  ଅକ୍ଷର ଅଚଳ *p = argv[1];

  ବାଛ (*p++)
    {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ false;

    ସ୍ଥିତି '+':
      /* Leading "+" is a file name in the standard form.  */
      ଯଦି (!traditional_usage)
        ଫେରନ୍ତୁ false;

      t_from_start = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      ଯଦି (!obsolete_usage && !p[p[0] == 'c'])
        ଫେରନ୍ତୁ false;

      t_from_start = false;
      ଭାଙ୍ଗ;
    }

  ଗଣନା_t n;
  ଯଦି (!c_isdigit (*p))
    n = DEFAULT_N_ଧାଡ଼ିଗୁଡ଼ିକ;
  ଅନ୍ୟଥା
    ପାଇଁ (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_add (&n, n, *p - '0') ? ଗଣନା_MAX : n;

  ବାଛ (*p)
    {
    ସ୍ଥିତି 'b': n = ckd_mul (&n, n, 512) ? ଗଣନା_MAX : n; FALLTHROUGH;
    ସ୍ଥିତି 'c': t_ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ = false; FALLTHROUGH;
    ସ୍ଥିତି 'l': p++; ଭାଙ୍ଗ;
    }

  ଯଦି (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  ଯଦି (*p)
    ଫେରନ୍ତୁ false;

  *n_units = n;

  /* Set globals.  */
  from_start = t_from_start;
  ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ = t_ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ;
  forever = t_forever;

  ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_options (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv,
               ଗଣନା_t *n_units, ଗଣନା header_mode *header_mode,
               ଦ୍ୱିଗୁଣ *sleep_interval)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'F':
          forever = true;
          follow_mode = Follow_name;
          reopen_inaccessible_files = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'c':
        ସ୍ଥିତି 'n':
          ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ = (c == 'n');
          ଯଦି (*optarg == '+')
            from_start = true;
          ଅନ୍ୟଥା ଯଦି (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, ଗଣନା_MAX, "bkKmMGTPEZYRQ0",
                                 (ଗଣନା_ଧାଡ଼ିଗୁଡ଼ିକ
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
        ସ୍ଥିତି LONG_FOLLOW_OPTION:
          forever = true;
          ଯଦି (optarg == ଖାଲି)
            follow_mode = DEFAULT_FOLLOW_MODE;
          ଅନ୍ୟଥା
            follow_mode = XARGMATCH ("--follow", optarg,
                                     follow_mode_string, follow_mode_map);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି RETRY_OPTION:
          reopen_inaccessible_files = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, ଗଣନା_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DEBUG_PROGRAM_OPTION:
          debug = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PID_OPTION:
          ଯଦି (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (INT_MAX, IDX_MAX), ଆକାର *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି PRESUME_ନିବେଶ_PIPE_OPTION:
          presume_ନିବେଶ_pipe = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'q':
          *header_mode = never;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 's':
          {
            ଅକ୍ଷର *ep;
            errno = 0;
            ଦ୍ୱିଗୁଣ s = cl_strtod (optarg, &ep);
            ଯଦି (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'v':
          *header_mode = always;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          ଧାଡ଼ି_end = '\0';
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3': ସ୍ଥିତି '4':
        ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7': ସ୍ଥିତି '8': ସ୍ଥିତି '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (reopen_inaccessible_files)
    {
      ଯଦି (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      ଅନ୍ୟଥା ଯଦି (follow_mode == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  ଯଦି (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      ମୁକ୍ତ (pids);
      pids = ଖାଲି;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
ସ୍ଥିର bool
ignore_fifo_and_pipe (ସଂରଚନା File_spec *f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].mode)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      ଯଦି (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      ଅନ୍ୟଥା
        some_viable = true;
    }

  ଫେରନ୍ତୁ some_viable;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଗଣନା header_mode header_mode = multiple_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  ଗଣନା_t n_units = DEFAULT_N_ଧାଡ଼ିଗୁଡ଼ିକ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files;
  ଅକ୍ଷର **file;
  ସଂରଚନା File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  ଦ୍ୱିଗୁଣ sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା p = getpagesize ();
    ଯଦି (IDX_MAX < p)
      xalloc_die ();
    page_size = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &header_mode, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_start && 0 < n_units && n_units < ଗଣନା_MAX;

  ଯଦି (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  ଅନ୍ୟଥା
    {
      ସ୍ଥିର ଅକ୍ଷର *dummy_stdin = (ଅକ୍ଷର *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool found_hyphen = false;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
      ଯଦି (streq (file[i], "-"))
        found_hyphen = true;

    /* When following by name, there must be a name.  */
    ଯଦି (found_hyphen && follow_mode == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    ଯଦି (forever && found_hyphen)
      {
        ସଂରଚନା stat in_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_mode == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &in_stat)
                          && ! S_ISREG (in_stat.st_mode));

        ଯଦି (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  ଯଦି (! forever && n_units == (from_start ? ଗଣନା_MAX : 0))
    ଫେରନ୍ତୁ EXIT_SUCCESS;

  ଯଦି (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, ଆକାର *F);
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  ଯଦି (header_mode == always
      || (header_mode == multiple_files && n_files > 1))
    print_headers = true;

  xset_ଦ୍ୱିମିକ_mode (STDOUT_FILENO, O_ଦ୍ୱିମିକ);

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; i++)
    ok &= tail_file (&F[i], n_files, n_units);

  ଯଦି (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      ସଂରଚନା stat out_stat;
      ଯଦି (fstat (STDOUT_FILENO, &out_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (out_stat.st_mode)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#ଯଦି HAVE_INOTIFY
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
      ଯଦି (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_mode == Follow_descriptor)))
        disable_inotify = true;

      ଯଦି (!disable_inotify)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା wd = inotify_init ();
          ଯଦି (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              ଯଦି (fflush (stdout) < 0)
                write_error ();

              Hash_table *ht;
              Hash_table *parent_ht = ଖାଲି;
              tail_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &parent_ht);
              hash_free (ht);
              ଯଦି (parent_ht)
                hash_free (parent_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#endif
      disable_inotify = true;
      tail_forever (F, n_files, sleep_interval);
    }

  ଯଦି (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
