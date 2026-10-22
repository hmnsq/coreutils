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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "cl-strtod.h"
#അന്തര്ഭാവ "dtimespec-bound.h"
#അന്തര്ഭാവ "fcntl--.h"
#അന്തര്ഭാവ "iopoll.h"
#അന്തര്ഭാവ "isapipe.h"
#അന്തര്ഭാവ "issymlink.h"
#അന്തര്ഭാവ "posixver.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-size.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "xbinary-io.h"
#അന്തര്ഭാവ "xdectoint.h"
#അന്തര്ഭാവ "xnanosleep.h"

#എങ്കിൽ HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#endif

/* Linux can optimize the handling of local files.  */
#എങ്കിൽ defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# എങ്കിൽ HAVE_SYS_STATFS_H
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
#define DEFAULT_N_വരികൾ 10

/* Special values for dump_remainder's N_BYTES parameter.  */
എണ്ണൽ { COPY_TO_EOF = -1, COPY_A_BUFFER = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
തരനിർവചനം intmax_t എണ്ണം_t;
#define എണ്ണം_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#define DEFAULT_FOLLOW_MODE Follow_descriptor

എണ്ണൽ Follow_mode
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

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം follow_mode_string[] =
{
  "descriptor", "name", ശൂന്യ
};

സ്ഥിതം എണ്ണൽ Follow_mode സ്ഥിരം follow_mode_map[] =
{
  Follow_descriptor, Follow_name,
};

ഘടന File_spec
{
  /* The actual file name, or "-" for stdin.  */
  അക്ഷരം *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  അക്ഷരം സ്ഥിരം *prettyname;

  /* Attributes of the file the last time we checked.  */
  ഘടന timespec mtime;
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
  പൂർണ്ണസംഖ്യ fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  പൂർണ്ണസംഖ്യ errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  പൂർണ്ണസംഖ്യ blocking;

#എങ്കിൽ HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  പൂർണ്ണസംഖ്യ wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  പൂർണ്ണസംഖ്യ parent_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_start;
#endif

  /* See description of DEFAULT_MAX_N_... below.  */
  എണ്ണം_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
സ്ഥിതം bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
സ്ഥിതം bool എണ്ണം_വരികൾ = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
സ്ഥിതം എണ്ണൽ Follow_mode follow_mode = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
സ്ഥിതം bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
സ്ഥിതം bool monitor_output;

/* If true, count from start of file instead of end.  */
സ്ഥിതം bool from_start;

/* If true, print filename headers.  */
സ്ഥിതം bool print_headers;

/* Character to split lines by. */
സ്ഥിതം അക്ഷരം വരി_end = '\n';

/* When to print the filename banners.  */
എണ്ണൽ header_mode
{
  multiple_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
എണ്ണൽ { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
സ്ഥിതം എണ്ണം_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ nbpids = 0;
സ്ഥിതം pid_t * pids = ശൂന്യ;
സ്ഥിതം idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
സ്ഥിതം idx_t page_size;

/* True if we have ever read standard input.  */
സ്ഥിതം bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
സ്ഥിതം bool presume_നിവേശം_pipe;

/* If nonzero then don't use inotify even if available.  */
സ്ഥിതം bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
സ്ഥിതം bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_നിവേശം_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_PROGRAM_OPTION,
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"bytes", required_argument, ശൂന്യ, 'c'},
  {"debug", no_argument, ശൂന്യ, DEBUG_PROGRAM_OPTION},
  {"follow", optional_argument, ശൂന്യ, LONG_FOLLOW_OPTION},
  {"lines", required_argument, ശൂന്യ, 'n'},
  {"max-unchanged-stats", required_argument, ശൂന്യ,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_argument, ശൂന്യ,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_argument, ശൂന്യ, PID_OPTION},
  {"-presume-input-pipe", no_argument, ശൂന്യ,
   PRESUME_നിവേശം_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, ശൂന്യ, 'q'},
  {"retry", no_argument, ശൂന്യ, RETRY_OPTION},
  {"silent", no_argument, ശൂന്യ, 'q'},
  {"sleep-interval", required_argument, ശൂന്യ, 's'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {"zero-terminated", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the last %d വരികൾ of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_N_വരികൾ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --bytes=[+]അക്കം_2\n\
         output the last അക്കം_2 bytes;\n\
         or use -c +അക്കം_2 to output starting with byte അക്കം_2 of each file\n\
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
  -n, --വരികൾ=[+]അക്കം_2\n\
         output the last അക്കം_2 വരികൾ, instead of the last %d;\n\
         or use -n +അക്കം_2 to skip അക്കം_2-1 വരികൾ at the start\n\
"), DEFAULT_N_വരികൾ);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         changed size after N (സ്ഥിരസ്ഥിതി %d) iterations\n\
         to see എങ്കിൽ it has been unlinked or renamed\n\
         (this is the usual അവസ്ഥ of rotated log files);\n\
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
         keep trying to open a file എങ്കിൽ it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep വേണ്ടി approximately N seconds\n\
         (സ്ഥിരസ്ഥിതി 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at least once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output headers giving file names\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         വരി delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (VERSION_OPTION_DESCRIPTION);
     fputs (_("\
\n\
അക്കം_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on വേണ്ടി T, P, E, Z, Y, R, Q.\n\
ദ്വിമാന_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), tail defaults to following the file descriptor, which\n\
means that even എങ്കിൽ a tail'ed file is renamed, tail will തുടരുക to track\n\
its end.  This സ്ഥിരസ്ഥിതി behavior is not desirable when you really want to\n\
track the actual name of the file, not the file descriptor (e.g., log\n\
rotation).  Use --follow=name in that അവസ്ഥ.  That causes tail to track the\n\
named file in a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
സ്ഥിതം ശൂന്യം
die_pipe (ശൂന്യം)
{
  ഉയർത്തുക (SIGPIPE);
  exit (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
സ്ഥിതം ശൂന്യം
check_output_alive (ശൂന്യം)
{
  എങ്കിൽ (! monitor_output)
    തിരികെ;

  എങ്കിൽ (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_UNUSED സ്ഥിതം bool
valid_file_spec (ഘടന File_spec സ്ഥിരം *f)
{
  /* Exactly one of the following subexpressions must be true. */
  തിരികെ (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

സ്ഥിതം off_t
xlseek (പൂർണ്ണസംഖ്യ fd, off_t offset, പൂർണ്ണസംഖ്യ whence, അക്ഷരം സ്ഥിരം *prettyname)
{
  off_t new_offset = lseek (fd, offset, whence);

  എങ്കിൽ (0 <= new_offset)
    തിരികെ new_offset;

  സ്ഥിതം അക്ഷരം സ്ഥിരം *whence_msgid[] = {
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

സ്ഥിതം ശൂന്യം
record_open_fd (ഘടന File_spec *f, പൂർണ്ണസംഖ്യ fd,
                off_t read_pos, ഘടന stat സ്ഥിരം *st,
                പൂർണ്ണസംഖ്യ blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->mode = st->st_mode;
  എങ്കിൽ (S_ISREG (st->st_mode))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

സ്ഥിതം ശൂന്യം
close_fd (പൂർണ്ണസംഖ്യ fd, ഘടന File_spec സ്ഥിരം *f)
{
  എങ്കിൽ (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

സ്ഥിതം ശൂന്യം
write_header (അക്ഷരം സ്ഥിരം *prettyname)
{
  സ്ഥിതം bool first_file = true;

  printf ("%s==> %s <==\n", first_file ? "" : "\n", quotef (prettyname));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

സ്ഥിതം ശൂന്യം
xwrite_stdout (അക്ഷരം സ്ഥിരം *buffer, idx_t n_bytes)
{
  എങ്കിൽ (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
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

സ്ഥിതം എണ്ണം_t
dump_remainder (bool want_header, അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd,
                എണ്ണം_t n_bytes)
{
  എണ്ണം_t n_read = 0;
  എണ്ണം_t n_remaining = n_bytes;

  ചെയ്യുക
    {
      അക്ഷരം buffer[BUFSIZ];
      idx_t n = n_bytes < 0 || BUFSIZ < n_remaining ? BUFSIZ : n_remaining;
      ssize_t bytes_read = പഠ (fd, buffer, n);
      എങ്കിൽ (bytes_read < 0)
        {
          എങ്കിൽ (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          നിർത്തുക;
        }
      എങ്കിൽ (bytes_read == 0)
        നിർത്തുക;
      n_read += bytes_read;

      എങ്കിൽ (want_header)
        {
          write_header (prettyname);
          want_header = false;
        }
      xwrite_stdout (buffer, bytes_read);
      എങ്കിൽ (n_bytes == COPY_A_BUFFER)
        നിർത്തുക;

      n_remaining -= bytes_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  ആകുമ്പോൾ (n_remaining != 0);

  തിരികെ n_read;
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

സ്ഥിതം off_t
file_വരികൾ (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, ഘടന stat സ്ഥിരം *sb,
            എണ്ണം_t n_വരികൾ, off_t start_pos, off_t end_pos)
{
  അക്ഷരം *buffer;
  idx_t bufsize = BUFSIZ;
  off_t pos = end_pos;

  എങ്കിൽ (n_വരികൾ == 0)
    തിരികെ pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_mode));
  എങ്കിൽ (sb->st_size % page_size == 0)
    bufsize = MAX (BUFSIZ, page_size);

  buffer = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t bytes_to_read = (pos - start_pos) % bufsize;
  എങ്കിൽ (bytes_to_read == 0)
    bytes_to_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -bytes_to_read, SEEK_CUR, prettyname);
  ssize_t bytes_read = പഠ (fd, buffer, bytes_to_read);
  എങ്കിൽ (bytes_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      പോകുക free_buffer;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  എങ്കിൽ (bytes_read && buffer[bytes_read - 1] != വരി_end)
    --n_വരികൾ;

  ചെയ്യുക
    {
      pos += bytes_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      ആകുമ്പോൾ (n)
        {
          അക്ഷരം സ്ഥിരം *nl;
          nl = memrchr (buffer, വരി_end, n);
          എങ്കിൽ (nl == ശൂന്യ)
            നിർത്തുക;
          n = nl - buffer;
          എങ്കിൽ (n_വരികൾ-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, bytes_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, end_pos - pos);
              പോകുക free_buffer;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      എങ്കിൽ (pos - bytes_read == start_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (buffer, bytes_read);
          dump_remainder (false, prettyname, fd, end_pos - pos);
          പോകുക free_buffer;
        }

      pos = xlseek (fd, -(bufsize + bytes_read), SEEK_CUR, prettyname);
      bytes_read = പഠ (fd, buffer, bufsize);
      എങ്കിൽ (bytes_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          പോകുക free_buffer;
        }
    }
  ആകുമ്പോൾ (bytes_read > 0);

free_buffer:
  സ്വതന്ത്രം (buffer);
  തിരികെ pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
pipe_വരികൾ (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, എണ്ണം_t n_വരികൾ)
{
  ഘടന linebuffer
  {
    അക്ഷരം buffer[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    ഘടന linebuffer *next;
  };
  തരനിർവചനം ഘടന linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  idx_t total_വരികൾ = 0;	/* Total number of newlines in all buffers.  */
  പൂർണ്ണസംഖ്യ ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (വലുപ്പം (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = ശൂന്യ;
  tmp = xmalloc (വലുപ്പം (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  ആകുമ്പോൾ (true)
    {
      n_read = പഠ (fd, tmp->buffer, BUFSIZ);
      എങ്കിൽ (n_read <= 0)
        നിർത്തുക;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = ശൂന്യ;

      /* Count the number of newlines just read.  */
      {
        അക്ഷരം സ്ഥിരം *buffer_end = tmp->buffer + n_read;
        അക്ഷരം സ്ഥിരം *p = tmp->buffer;
        ആകുമ്പോൾ ((p = memchr (p, വരി_end, buffer_end - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_വരികൾ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      എങ്കിൽ (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      അല്ലെങ്കിൽ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          എങ്കിൽ (total_വരികൾ - first->nlines > n_വരികൾ)
            {
              tmp = first;
              total_വരികൾ -= first->nlines;
              first = first->next;
            }
          അല്ലെങ്കിൽ
            tmp = xmalloc (വലുപ്പം (LBUFFER));
        }
    }

  സ്വതന്ത്രം (tmp);

  എങ്കിൽ (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      പോകുക free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  എങ്കിൽ (last->nbytes == 0)
    പോകുക free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  എങ്കിൽ (n_വരികൾ == 0)
    പോകുക free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  എങ്കിൽ (last->buffer[last->nbytes - 1] != വരി_end)
    {
      ++last->nlines;
      ++total_വരികൾ;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  വേണ്ടി (tmp = first; total_വരികൾ - tmp->nlines > n_വരികൾ; tmp = tmp->next)
    total_വരികൾ -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    അക്ഷരം സ്ഥിരം *beg = tmp->buffer;
    അക്ഷരം സ്ഥിരം *buffer_end = tmp->buffer + tmp->nbytes;
    എങ്കിൽ (total_വരികൾ > n_വരികൾ)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        വേണ്ടി (idx_t j = total_വരികൾ - n_വരികൾ; j; --j)
          {
            beg = rawmemchr (beg, വരി_end);
            ++beg;
          }
      }

    xwrite_stdout (beg, buffer_end - beg);
  }

  വേണ്ടി (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

free_lbuffers:
  ആകുമ്പോൾ (first)
    {
      tmp = first->next;
      സ്വതന്ത്രം (first);
      first = tmp;
    }
  തിരികെ ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

സ്ഥിതം off_t
pipe_bytes (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, എണ്ണം_t n_bytes,
            off_t read_pos)
{
  ഘടന charbuffer
  {
    അക്ഷരം buffer[BUFSIZ];
    idx_t nbytes;
    ഘടന charbuffer *next;
  };
  തരനിർവചനം ഘടന charbuffer CBUFFER;
  CBUFFER *first, *last, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t total_bytes = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  എങ്കിൽ (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  first = last = xmalloc (വലുപ്പം (CBUFFER));
  first->nbytes = 0;
  first->next = ശൂന്യ;
  tmp = xmalloc (വലുപ്പം (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  ആകുമ്പോൾ (true)
    {
      n_read = പഠ (fd, tmp->buffer, BUFSIZ);
      എങ്കിൽ (n_read <= 0)
        നിർത്തുക;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->next = ശൂന്യ;

      total_bytes += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      എങ്കിൽ (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
        }
      അല്ലെങ്കിൽ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          എങ്കിൽ (total_bytes - first->nbytes > n_bytes)
            {
              tmp = first;
              total_bytes -= first->nbytes;
              first = first->next;
            }
          അല്ലെങ്കിൽ
            {
              tmp = xmalloc (വലുപ്പം (CBUFFER));
            }
        }
    }

  സ്വതന്ത്രം (tmp);

  എങ്കിൽ (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      പോകുക free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  വേണ്ടി (tmp = first; total_bytes - tmp->nbytes > n_bytes; tmp = tmp->next)
    total_bytes -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  എങ്കിൽ (total_bytes > n_bytes)
    i = total_bytes - n_bytes;
  അല്ലെങ്കിൽ
    i = 0;
  xwrite_stdout (&tmp->buffer[i], tmp->nbytes - i);

  വേണ്ടി (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

  എങ്കിൽ (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  ആകുമ്പോൾ (first)
    {
      tmp = first->next;
      സ്വതന്ത്രം (first);
      first = tmp;
    }
  തിരികെ read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
start_bytes (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, എണ്ണം_t n_bytes)
{
  അക്ഷരം buffer[BUFSIZ];

  ആകുമ്പോൾ (0 < n_bytes)
    {
      ssize_t bytes_read = പഠ (fd, buffer, BUFSIZ);
      എങ്കിൽ (bytes_read == 0)
        തിരികെ -1;
      എങ്കിൽ (bytes_read < 0)
        {
          പൂർണ്ണസംഖ്യ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          തിരികെ ret;
        }
      എങ്കിൽ (bytes_read <= n_bytes)
        n_bytes -= bytes_read;
      അല്ലെങ്കിൽ
        {
          xwrite_stdout (&buffer[n_bytes], bytes_read - n_bytes);
          നിർത്തുക;
        }
    }

  തിരികെ 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
start_വരികൾ (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, എണ്ണം_t n_വരികൾ)
{
  എങ്കിൽ (n_വരികൾ == 0)
    തിരികെ 0;

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം buffer[BUFSIZ];
      ssize_t bytes_read = പഠ (fd, buffer, BUFSIZ);
      എങ്കിൽ (bytes_read == 0) /* EOF */
        തിരികെ -1;
      എങ്കിൽ (bytes_read < 0) /* error */
        {
          പൂർണ്ണസംഖ്യ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          തിരികെ ret;
        }

      അക്ഷരം *buffer_end = buffer + bytes_read;
      അക്ഷരം *p = buffer;
      ആകുമ്പോൾ ((p = memchr (p, വരി_end, buffer_end - p)))
        {
          ++p;
          എങ്കിൽ (--n_വരികൾ == 0)
            {
              എങ്കിൽ (p < buffer_end)
                xwrite_stdout (p, buffer_end - p);
              തിരികെ 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
സ്ഥിതം bool
fremote (പൂർണ്ണസംഖ്യ fd, ഘടന File_spec സ്ഥിരം *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#എങ്കിൽ HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  ഘടന statfs buf;
  പൂർണ്ണസംഖ്യ err = fstatfs (fd, &buf);
  എങ്കിൽ (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      എങ്കിൽ (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  അല്ലെങ്കിൽ
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#endif

  തിരികെ remote;
}

/* open/fstat F->name and handle changes.  */
സ്ഥിതം ശൂന്യം
recheck (ഘടന File_spec *f, bool blocking)
{
  ഘടന stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  പൂർണ്ണസംഖ്യ prev_errnum = f->errnum;
  bool new_file;
  പൂർണ്ണസംഖ്യ fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  പൂർണ്ണസംഖ്യ open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  എങ്കിൽ (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      എങ്കിൽ (fd < 0)
        {
          എങ്കിൽ (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          അല്ലെങ്കിൽ
            {
              /* say nothing... it's still not tailable */
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (!IS_TAILABLE_FILE_TYPE (new_stats.st_mode))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_mode == Follow_name);
      എങ്കിൽ (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  അല്ലെങ്കിൽ എങ്കിൽ ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  അല്ലെങ്കിൽ
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  എങ്കിൽ (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  അല്ലെങ്കിൽ എങ്കിൽ (f->fd < 0)
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
  അല്ലെങ്കിൽ എങ്കിൽ (!SAME_INODE (*f, new_stats))
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
  അല്ലെങ്കിൽ
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
  എങ്കിൽ (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      record_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

സ്ഥിതം bool
any_live_files (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  എങ്കിൽ (reopen_inaccessible_files && follow_mode == Follow_name)
    തിരികെ true;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    {
      എങ്കിൽ (0 <= f[i].fd)
        തിരികെ true;
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (! f[i].ignore && reopen_inaccessible_files)
            തിരികെ true;
        }
    }

  തിരികെ false;
}

/* Return true if any writers exist, even as zombies.  */

സ്ഥിതം bool
some_writers_exist (ശൂന്യം)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  വേണ്ടി (idx_t i = 0; i < nbpids; )
    {
      എങ്കിൽ (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * വലുപ്പം pids[i]);
        }
      അല്ലെങ്കിൽ
        i++;
    }


  തിരികെ 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

സ്ഥിതം ശൂന്യം
tail_forever (ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files, ഇരട്ട sleep_interval)
{
  പൂർണ്ണസംഖ്യ last = n_files - 1;

  സ്ഥിതം bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  എങ്കിൽ (fflush (stdout) < 0)
    write_error ();

  ആകുമ്പോൾ (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_mode == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].mode));

      എങ്കിൽ (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_നിവേശം = false;

      വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
        {
          ഘടന stat stats;

          എങ്കിൽ (f[i].ignore)
            തുടരുക;

          പൂർണ്ണസംഖ്യ fd = f[i].fd;
          എങ്കിൽ (fd < 0)
            {
              recheck (&f[i], blocking);
              തുടരുക;
            }

          അക്ഷരം സ്ഥിരം *prettyname = f[i].prettyname;
          mode_t mode = f[i].mode;

          എങ്കിൽ (f[i].blocking != blocking)
            {
              പൂർണ്ണസംഖ്യ old_flags = fcntl (fd, F_GETFL);
              പൂർണ്ണസംഖ്യ new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              എങ്കിൽ (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  എങ്കിൽ (S_ISREG (f[i].mode) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  അല്ലെങ്കിൽ
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              അല്ലെങ്കിൽ
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          എങ്കിൽ (!f[i].blocking)
            {
              എങ്കിൽ (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  തുടരുക;
                }

              എങ്കിൽ (f[i].mode == stats.st_mode
                  && (! S_ISREG (stats.st_mode)
                      || f[i].read_pos == stats.st_size)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  എങ്കിൽ ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_mode == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  എങ്കിൽ (fd != f[i].fd || S_ISREG (stats.st_mode) || 1 < n_files)
                    തുടരുക;
                  അല്ലെങ്കിൽ
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].mode = stats.st_mode;

              /* reset counter */
              എങ്കിൽ (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              എങ്കിൽ (S_ISREG (mode) && stats.st_size < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              എങ്കിൽ (i != last)
                {
                  എങ്കിൽ (print_headers)
                    write_header (prettyname);
                  last = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          എണ്ണം_t bytes_to_read;
          എങ്കിൽ (f[i].blocking)
            bytes_to_read = COPY_A_BUFFER;
          അല്ലെങ്കിൽ എങ്കിൽ (S_ISREG (mode) && f[i].remote)
            bytes_to_read = stats.st_size - f[i].read_pos;
          അല്ലെങ്കിൽ
            bytes_to_read = COPY_TO_EOF;

          എണ്ണം_t nr = dump_remainder (false, prettyname, fd, bytes_to_read);
          എങ്കിൽ (0 < nr)
            {
              എങ്കിൽ (S_ISREG (mode))
                f[i].read_pos += nr;
              എങ്കിൽ (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_നിവേശം = true;
            }
        }

      എങ്കിൽ (! any_live_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          നിർത്തുക;
        }

      എങ്കിൽ ((!any_നിവേശം || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      എങ്കിൽ (!any_നിവേശം)
        {
          എങ്കിൽ (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              എങ്കിൽ (!nbpids)
                നിർത്തുക;
              എങ്കിൽ (!some_writers_exist ())
                തുടരുക;
            }

          എങ്കിൽ (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#എങ്കിൽ HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

സ്ഥിതം bool
any_remote_file (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    എങ്കിൽ (0 <= f[i].fd && f[i].remote)
      തിരികെ true;
  തിരികെ false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

സ്ഥിതം bool
any_non_remote_file (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    എങ്കിൽ (0 <= f[i].fd && ! f[i].remote)
      തിരികെ true;
  തിരികെ false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

സ്ഥിതം bool
any_symlinks (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    എങ്കിൽ (issymlink (f[i].name) == 1)
      തിരികെ true;
  തിരികെ false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

സ്ഥിതം bool
any_non_regular_fifo (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    എങ്കിൽ (0 <= f[i].fd && ! S_ISREG (f[i].mode) && ! S_ISFIFO (f[i].mode))
      തിരികെ true;
  തിരികെ false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

സ്ഥിതം bool
tailable_stdin (സ്ഥിരം ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    എങ്കിൽ (!f[i].ignore && streq (f[i].name, "-"))
      തിരികെ true;
  തിരികെ false;
}

സ്ഥിതം size_t
wd_hasher (സ്ഥിരം ശൂന്യം *entry, size_t tabsize)
{
  സ്ഥിരം ഘടന File_spec *spec = entry;
  തിരികെ spec->wd % tabsize;
}

സ്ഥിതം bool
wd_comparator (സ്ഥിരം ശൂന്യം *e1, സ്ഥിരം ശൂന്യം *e2)
{
  സ്ഥിരം ഘടന File_spec *spec1 = e1;
  സ്ഥിരം ഘടന File_spec *spec2 = e2;
  തിരികെ spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
സ്ഥിതം size_t
parent_name_hasher (സ്ഥിരം ശൂന്യം *entry, size_t tabsize)
{
  സ്ഥിരം ഘടന File_spec *spec = entry;
  അക്ഷരം സ്ഥിരം *name = spec->name + spec->basename_start;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  തിരികെ (hash_string (name, tabsize) + (size_t) spec->parent_wd) % tabsize;
}

സ്ഥിതം bool
parent_name_comparator (സ്ഥിരം ശൂന്യം *e1, സ്ഥിരം ശൂന്യം *e2)
{
  സ്ഥിരം ഘടന File_spec *a = e1;
  സ്ഥിരം ഘടന File_spec *b = e2;
  തിരികെ (a->parent_wd == b->parent_wd
          && streq (a->name + a->basename_start,
                    b->name + b->basename_start));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
സ്ഥിതം ശൂന്യം
check_fspec (ഘടന File_spec *fspec, ഘടന File_spec **prev_fspec)
{
  എങ്കിൽ (fspec->fd < 0)
    തിരികെ;

  ഘടന stat stats;
  എങ്കിൽ (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      തിരികെ;
    }

  അക്ഷരം സ്ഥിരം *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  എങ്കിൽ (S_ISREG (fspec->mode) && stats.st_size < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (S_ISREG (fspec->mode) && stats.st_size == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    തിരികെ;

  bool want_header = print_headers && (fspec != *prev_fspec);

  എണ്ണം_t nr = dump_remainder (want_header, prettyname, fspec->fd, COPY_TO_EOF);
  എങ്കിൽ (0 < nr)
    {
      എങ്കിൽ (S_ISREG (fspec->mode))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      എങ്കിൽ (fflush (stdout) < 0)
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
സ്ഥിതം ശൂന്യം
tail_forever_inotify (പൂർണ്ണസംഖ്യ wd, ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files,
                      ഇരട്ട sleep_interval, Hash_table **wd_to_namep,
                      Hash_table **parent_by_namep)
{
# എങ്കിൽ TAIL_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  പൂർണ്ണസംഖ്യ max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_table *wd_to_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_table *parent_by_name = ശൂന്യ;

  bool found_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool found_unwatchable_dir = false;
  bool no_inotify_resources = false;
  ഘടന File_spec *prev_fspec;
  idx_t evlen = 0;
  അക്ഷരം *evbuf;
  idx_t evbuf_off = 0;

  *parent_by_namep = ശൂന്യ;

  wd_to_name = hash_initialize (n_files, ശൂന്യ, wd_hasher, wd_comparator,
                                ശൂന്യ);
  എങ്കിൽ (! wd_to_name)
    xalloc_die ();
  *wd_to_namep = wd_to_name;

  എങ്കിൽ (follow_mode == Follow_name)
    {
      parent_by_name = hash_initialize (n_files, ശൂന്യ, parent_name_hasher,
                                        parent_name_comparator, ശൂന്യ);
      എങ്കിൽ (! parent_by_name)
        xalloc_die ();
      *parent_by_namep = parent_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_mask = IN_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  എങ്കിൽ (follow_mode == Follow_name)
    inotify_wd_mask |= (IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    {
      എങ്കിൽ (!f[i].ignore)
        {
          idx_t fnlen = strlen (f[i].name);
          എങ്കിൽ (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          എങ്കിൽ (follow_mode == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              അക്ഷരം prev = f[i].name[dirlen];
              f[i].basename_start = last_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].parent_wd = inotify_add_watch (wd, dirlen ? f[i].name : ".",
                                                  (IN_CREATE | IN_DELETE
                                                   | IN_MOVED_TO | IN_ATTRIB
                                                   | IN_DELETE_SELF));

              f[i].name[dirlen] = prev;

              എങ്കിൽ (f[i].parent_wd < 0)
                {
                  എങ്കിൽ (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  അല്ലെങ്കിൽ
                    error (0, 0, _("inotify resources exhausted"));
                  found_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  നിർത്തുക;
                }

              എങ്കിൽ (hash_തിരുകുക (parent_by_name, &(f[i])) == ശൂന്യ)
                xalloc_die ();
            }

          f[i].wd = inotify_add_watch (wd, f[i].name, inotify_wd_mask);

          എങ്കിൽ (f[i].wd < 0)
            {
              എങ്കിൽ (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              എങ്കിൽ (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  നിർത്തുക;
                }
              അല്ലെങ്കിൽ എങ്കിൽ (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              തുടരുക;
            }

          എങ്കിൽ (hash_തിരുകുക (wd_to_name, &(f[i])) == ശൂന്യ)
            xalloc_die ();

          found_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  എങ്കിൽ (no_inotify_resources || found_unwatchable_dir
      || (follow_mode == Follow_descriptor && tailed_but_unwatchable))
    തിരികെ;
  എങ്കിൽ (follow_mode == Follow_descriptor && !found_watchable_file)
    exit (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    {
      എങ്കിൽ (! f[i].ignore)
        {
          /* check for new files.  */
          എങ്കിൽ (follow_mode == Follow_name)
            recheck (&(f[i]), false);
          അല്ലെങ്കിൽ എങ്കിൽ (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              ഘടന stat stats;

              എങ്കിൽ (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  തിരികെ;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  എങ്കിൽ (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += വലുപ്പം (ഘടന inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  ആകുമ്പോൾ (true)
    {
      ഘടന File_spec *fspec;
      ഘടന inotify_event *ev;
      ശൂന്യം *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      എങ്കിൽ (follow_mode == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_to_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      എങ്കിൽ (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          പൂർണ്ണസംഖ്യ file_change;
          ഘടന pollfd pfd[2];
          ചെയ്യുക
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              പൂർണ്ണസംഖ്യ delay = -1;

              എങ്കിൽ (pids)
                {
                  എങ്കിൽ (!nbpids)
                    exit (EXIT_SUCCESS);

                  എങ്കിൽ (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  അല്ലെങ്കിൽ എങ്കിൽ (sleep_interval < INT_MAX / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      ഇരട്ട ddelay = sleep_interval * 1000;
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
          ആകുമ്പോൾ (file_change == 0 || (file_change < 0 && errno == EINTR));

          എങ്കിൽ (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          എങ്കിൽ (pfd[1].revents)
            die_pipe ();

          len = പഠ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          എങ്കിൽ ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              തുടരുക;
            }

          എങ്കിൽ (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += വലുപ്പം (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      എങ്കിൽ ((ev->mask & IN_DELETE_SELF) && ! ev->len)
        {
          വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
            {
              എങ്കിൽ (ev->wd == f[i].parent_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  തിരികെ;
                }
            }
        }

      എങ്കിൽ (ev->len) /* event on ev->name in watched directory.  */
        {
          ഘടന File_spec താക്കോൽ;
          എങ്കിൽ (! parent_by_name)
            തുടരുക;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          താക്കോൽ.parent_wd = ev->wd;
          താക്കോൽ.name = ev->name;
          താക്കോൽ.basename_start = 0;
          fspec = hash_lookup (parent_by_name, &താക്കോൽ);

          /* It is not a watched file.  */
          എങ്കിൽ (! fspec)
            തുടരുക;

          പൂർണ്ണസംഖ്യ new_wd = -1;
          bool deleting = !! (ev->mask & IN_DELETE);

          എങ്കിൽ (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_add_watch (wd, fspec->name, inotify_wd_mask);
            }

          എങ്കിൽ (! deleting && new_wd < 0)
            {
              എങ്കിൽ (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  തിരികെ; /* revert to polling.  */
                }
              അല്ലെങ്കിൽ
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          എങ്കിൽ (new_watch)
            {
              എങ്കിൽ (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_to_name, fspec);
                }

              fspec->wd = new_wd;

              എങ്കിൽ (new_wd < 0)
                തുടരുക;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              ഘടന File_spec *prev = hash_remove (wd_to_name, fspec);
              എങ്കിൽ (prev && prev != fspec)
                {
                  എങ്കിൽ (follow_mode == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              എങ്കിൽ (hash_തിരുകുക (wd_to_name, fspec) == ശൂന്യ)
                xalloc_die ();
            }

          എങ്കിൽ (follow_mode == Follow_name)
            recheck (fspec, false);
        }
      അല്ലെങ്കിൽ
        {
          ഘടന File_spec താക്കോൽ;
          താക്കോൽ.wd = ev->wd;
          fspec = hash_lookup (wd_to_name, &താക്കോൽ);
        }

      എങ്കിൽ (! fspec)
        തുടരുക;

      എങ്കിൽ (ev->mask & (IN_ATTRIB | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          എങ്കിൽ (ev->mask & IN_DELETE_SELF
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

          തുടരുക;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#endif

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

സ്ഥിതം off_t
tail_bytes (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, ഘടന stat സ്ഥിരം *st,
            എണ്ണം_t n_bytes)
{
  off_t current_pos
    = (presume_നിവേശം_pipe
       ? -1
       : lseek (fd, from_start ? MIN (n_bytes, OFF_T_MAX) : 0, SEEK_CUR));

  എങ്കിൽ (from_start)
    {
      എങ്കിൽ (current_pos < 0)
        {
          പൂർണ്ണസംഖ്യ t = start_bytes (prettyname, fd, n_bytes);
          എങ്കിൽ (t < 0)
            തിരികെ t;
        }
      n_bytes = COPY_TO_EOF;
    }
  അല്ലെങ്കിൽ
    {
      off_t initial_pos = current_pos;
      off_t end_pos = -1;

      എങ്കിൽ (0 <= current_pos)
        {
          എങ്കിൽ (usable_st_size (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_size = STP_BLKSIZE (st);
              എങ്കിൽ (smallish_size < st->st_size)
                end_pos = st->st_size;
            }
          അല്ലെങ്കിൽ
            {
              off_t minus_n;
              എങ്കിൽ (ckd_sub (&minus_n, 0, n_bytes))
                end_pos = 0;
              അല്ലെങ്കിൽ
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  എങ്കിൽ (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      end_pos = e_n_pos + n_bytes;
                    }
                  അല്ലെങ്കിൽ
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      എങ്കിൽ (0 <= e_pos)
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
      എങ്കിൽ (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      എങ്കിൽ (end_pos < 0)
        തിരികെ pipe_bytes (prettyname, fd, n_bytes, current_pos);
    }

  എണ്ണം_t nr = dump_remainder (false, prettyname, fd, n_bytes);
  തിരികെ current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

സ്ഥിതം off_t
tail_വരികൾ (അക്ഷരം സ്ഥിരം *prettyname, പൂർണ്ണസംഖ്യ fd, ഘടന stat സ്ഥിരം *st,
            എണ്ണം_t n_വരികൾ)
{
  എങ്കിൽ (from_start)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      എങ്കിൽ (OFF_T_MAX <= n_വരികൾ)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          എങ്കിൽ (0 <= e)
            തിരികെ e;
        }

      പൂർണ്ണസംഖ്യ t = start_വരികൾ (prettyname, fd, n_വരികൾ);
      എങ്കിൽ (t < 0)
        തിരികെ t;
      dump_remainder (false, prettyname, fd, COPY_TO_EOF);
      തിരികെ -1;
    }
  അല്ലെങ്കിൽ
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t start_pos = (!presume_നിവേശം_pipe && S_ISREG (st->st_mode)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t end_pos = start_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      തിരികെ (end_pos < 0
              ? pipe_വരികൾ (prettyname, fd, n_വരികൾ)
              : start_pos < end_pos
              ? file_വരികൾ (prettyname, fd, st, n_വരികൾ, start_pos, end_pos)
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

സ്ഥിതം off_t
tail (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ fd, ഘടന stat സ്ഥിരം *st, എണ്ണം_t n_units)
{
  തിരികെ (എണ്ണം_വരികൾ ? tail_വരികൾ : tail_bytes) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

സ്ഥിതം bool
tail_file (ഘടന File_spec *f, എണ്ണം_t n_files, എണ്ണം_t n_units)
{
  പൂർണ്ണസംഖ്യ fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  എങ്കിൽ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ദ്വിമാന_mode (STDIN_FILENO, O_ദ്വിമാന);
    }
  അല്ലെങ്കിൽ
    fd = open (f->name, O_RDONLY | O_ദ്വിമാന | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  എങ്കിൽ (fd < 0)
    {
      എങ്കിൽ (forever)
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
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (print_headers)
        write_header (f->prettyname);

      off_t read_pos;
      ഘടന stat stats;
      ok = 0 <= fstat (fd, &stats);
      എങ്കിൽ (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      അല്ലെങ്കിൽ
        {
          read_pos = tail (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          എങ്കിൽ (IS_TAILABLE_FILE_TYPE (stats.st_mode))
            f->tailable = true;
          അല്ലെങ്കിൽ എങ്കിൽ (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      എങ്കിൽ (forever)
        {
          എങ്കിൽ (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          അല്ലെങ്കിൽ
            {
              record_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  തിരികെ ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

സ്ഥിതം bool
parse_obsolete_option (പൂർണ്ണസംഖ്യ argc, അക്ഷരം * സ്ഥിരം *argv, എണ്ണം_t *n_units)
{
  bool t_from_start;
  bool t_എണ്ണം_വരികൾ = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  എങ്കിൽ (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    തിരികെ false;

  പൂർണ്ണസംഖ്യ posix_ver = posix2_version ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  അക്ഷരം സ്ഥിരം *p = argv[1];

  തിരഞ്ഞെടുപ്പ് (*p++)
    {
    സ്ഥിരസ്ഥിതി:
      തിരികെ false;

    അവസ്ഥ '+':
      /* Leading "+" is a file name in the standard form.  */
      എങ്കിൽ (!traditional_usage)
        തിരികെ false;

      t_from_start = true;
      നിർത്തുക;

    അവസ്ഥ '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      എങ്കിൽ (!obsolete_usage && !p[p[0] == 'c'])
        തിരികെ false;

      t_from_start = false;
      നിർത്തുക;
    }

  എണ്ണം_t n;
  എങ്കിൽ (!c_isdigit (*p))
    n = DEFAULT_N_വരികൾ;
  അല്ലെങ്കിൽ
    വേണ്ടി (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_add (&n, n, *p - '0') ? എണ്ണം_MAX : n;

  തിരഞ്ഞെടുപ്പ് (*p)
    {
    അവസ്ഥ 'b': n = ckd_mul (&n, n, 512) ? എണ്ണം_MAX : n; FALLTHROUGH;
    അവസ്ഥ 'c': t_എണ്ണം_വരികൾ = false; FALLTHROUGH;
    അവസ്ഥ 'l': p++; നിർത്തുക;
    }

  എങ്കിൽ (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  എങ്കിൽ (*p)
    തിരികെ false;

  *n_units = n;

  /* Set globals.  */
  from_start = t_from_start;
  എണ്ണം_വരികൾ = t_എണ്ണം_വരികൾ;
  forever = t_forever;

  തിരികെ true;
}

സ്ഥിതം ശൂന്യം
parse_options (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv,
               എണ്ണം_t *n_units, എണ്ണൽ header_mode *header_mode,
               ഇരട്ട *sleep_interval)
{
  പൂർണ്ണസംഖ്യ c;

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'F':
          forever = true;
          follow_mode = Follow_name;
          reopen_inaccessible_files = true;
          നിർത്തുക;

        അവസ്ഥ 'c':
        അവസ്ഥ 'n':
          എണ്ണം_വരികൾ = (c == 'n');
          എങ്കിൽ (*optarg == '+')
            from_start = true;
          അല്ലെങ്കിൽ എങ്കിൽ (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, എണ്ണം_MAX, "bkKmMGTPEZYRQ0",
                                 (എണ്ണം_വരികൾ
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          നിർത്തുക;

        അവസ്ഥ 'f':
        അവസ്ഥ LONG_FOLLOW_OPTION:
          forever = true;
          എങ്കിൽ (optarg == ശൂന്യ)
            follow_mode = DEFAULT_FOLLOW_MODE;
          അല്ലെങ്കിൽ
            follow_mode = XARGMATCH ("--follow", optarg,
                                     follow_mode_string, follow_mode_map);
          നിർത്തുക;

        അവസ്ഥ RETRY_OPTION:
          reopen_inaccessible_files = true;
          നിർത്തുക;

        അവസ്ഥ MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, എണ്ണം_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          നിർത്തുക;

        അവസ്ഥ DEBUG_PROGRAM_OPTION:
          debug = true;
          നിർത്തുക;

        അവസ്ഥ DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          നിർത്തുക;

        അവസ്ഥ PID_OPTION:
          എങ്കിൽ (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (INT_MAX, IDX_MAX), വലുപ്പം *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          നിർത്തുക;

        അവസ്ഥ PRESUME_നിവേശം_PIPE_OPTION:
          presume_നിവേശം_pipe = true;
          നിർത്തുക;

        അവസ്ഥ 'q':
          *header_mode = never;
          നിർത്തുക;

        അവസ്ഥ 's':
          {
            അക്ഷരം *ep;
            errno = 0;
            ഇരട്ട s = cl_strtod (optarg, &ep);
            എങ്കിൽ (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          നിർത്തുക;

        അവസ്ഥ 'v':
          *header_mode = always;
          നിർത്തുക;

        അവസ്ഥ 'z':
          വരി_end = '\0';
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
        അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (reopen_inaccessible_files)
    {
      എങ്കിൽ (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      അല്ലെങ്കിൽ എങ്കിൽ (follow_mode == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  എങ്കിൽ (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      സ്വതന്ത്രം (pids);
      pids = ശൂന്യ;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
സ്ഥിതം bool
ignore_fifo_and_pipe (ഘടന File_spec *f, പൂർണ്ണസംഖ്യ n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].mode)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      എങ്കിൽ (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      അല്ലെങ്കിൽ
        some_viable = true;
    }

  തിരികെ some_viable;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  എണ്ണൽ header_mode header_mode = multiple_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  എണ്ണം_t n_units = DEFAULT_N_വരികൾ;
  പൂർണ്ണസംഖ്യ n_files;
  അക്ഷരം **file;
  ഘടന File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  ഇരട്ട sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    പൂർണ്ണസംഖ്യ p = getpagesize ();
    എങ്കിൽ (IDX_MAX < p)
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
  n_units -= from_start && 0 < n_units && n_units < എണ്ണം_MAX;

  എങ്കിൽ (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  അല്ലെങ്കിൽ
    {
      സ്ഥിതം അക്ഷരം *dummy_stdin = (അക്ഷരം *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool found_hyphen = false;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
      എങ്കിൽ (streq (file[i], "-"))
        found_hyphen = true;

    /* When following by name, there must be a name.  */
    എങ്കിൽ (found_hyphen && follow_mode == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    എങ്കിൽ (forever && found_hyphen)
      {
        ഘടന stat in_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_mode == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &in_stat)
                          && ! S_ISREG (in_stat.st_mode));

        എങ്കിൽ (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  എങ്കിൽ (! forever && n_units == (from_start ? എണ്ണം_MAX : 0))
    തിരികെ EXIT_SUCCESS;

  എങ്കിൽ (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, വലുപ്പം *F);
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  എങ്കിൽ (header_mode == always
      || (header_mode == multiple_files && n_files > 1))
    print_headers = true;

  xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_files; i++)
    ok &= tail_file (&F[i], n_files, n_units);

  എങ്കിൽ (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      ഘടന stat out_stat;
      എങ്കിൽ (fstat (STDOUT_FILENO, &out_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (out_stat.st_mode)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#എങ്കിൽ HAVE_INOTIFY
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
      എങ്കിൽ (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_mode == Follow_descriptor)))
        disable_inotify = true;

      എങ്കിൽ (!disable_inotify)
        {
          പൂർണ്ണസംഖ്യ wd = inotify_init ();
          എങ്കിൽ (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              എങ്കിൽ (fflush (stdout) < 0)
                write_error ();

              Hash_table *ht;
              Hash_table *parent_ht = ശൂന്യ;
              tail_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &parent_ht);
              hash_free (ht);
              എങ്കിൽ (parent_ht)
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

  എങ്കിൽ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
