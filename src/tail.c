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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "cl-strtod.h"
#अन्तर्भाव "dtimespec-bound.h"
#अन्तर्भाव "fcntl--.h"
#अन्तर्भाव "iopoll.h"
#अन्तर्भाव "isapipe.h"
#अन्तर्भाव "issymlink.h"
#अन्तर्भाव "posixver.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-size.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "xbinary-io.h"
#अन्तर्भाव "xdectoint.h"
#अन्तर्भाव "xnanosleep.h"

#यदि HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#endif

/* Linux can optimize the handling of local files.  */
#यदि defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# यदि HAVE_SYS_STATFS_H
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
#define DEFAULT_N_ओळी 10

/* Special values for dump_remainder's N_BYTES parameter.  */
गणना { COPY_TO_EOF = -1, COPY_A_BUFFER = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
प्रकारनिर्देश intmax_t मोजणी_t;
#define मोजणी_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#define DEFAULT_FOLLOW_MODE Follow_descriptor

गणना Follow_mode
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

स्थिर वर्ण ध्रुव *ध्रुव follow_mode_string[] =
{
  "descriptor", "name", रिक्त
};

स्थिर गणना Follow_mode ध्रुव follow_mode_map[] =
{
  Follow_descriptor, Follow_name,
};

संरचना File_spec
{
  /* The actual file name, or "-" for stdin.  */
  वर्ण *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  वर्ण ध्रुव *prettyname;

  /* Attributes of the file the last time we checked.  */
  संरचना timespec mtime;
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
  पूर्णाङ्क fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  पूर्णाङ्क errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  पूर्णाङ्क blocking;

#यदि HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  पूर्णाङ्क wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  पूर्णाङ्क parent_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_start;
#endif

  /* See description of DEFAULT_MAX_N_... below.  */
  मोजणी_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
स्थिर bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
स्थिर bool मोजणी_ओळी = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
स्थिर गणना Follow_mode follow_mode = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
स्थिर bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
स्थिर bool monitor_output;

/* If true, count from start of file instead of end.  */
स्थिर bool from_start;

/* If true, print filename headers.  */
स्थिर bool print_headers;

/* Character to split lines by. */
स्थिर वर्ण ओळ_end = '\n';

/* When to print the filename banners.  */
गणना header_mode
{
  multiple_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
गणना { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
स्थिर मोजणी_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
स्थिर पूर्णाङ्क nbpids = 0;
स्थिर pid_t * pids = रिक्त;
स्थिर idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
स्थिर idx_t page_size;

/* True if we have ever read standard input.  */
स्थिर bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
स्थिर bool presume_आदान_pipe;

/* If nonzero then don't use inotify even if available.  */
स्थिर bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
स्थिर bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_आदान_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_PROGRAM_OPTION,
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"bytes", required_argument, रिक्त, 'c'},
  {"debug", no_argument, रिक्त, DEBUG_PROGRAM_OPTION},
  {"follow", optional_argument, रिक्त, LONG_FOLLOW_OPTION},
  {"lines", required_argument, रिक्त, 'n'},
  {"max-unchanged-stats", required_argument, रिक्त,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_argument, रिक्त,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_argument, रिक्त, PID_OPTION},
  {"-presume-input-pipe", no_argument, रिक्त,
   PRESUME_आदान_PIPE_OPTION}, /* do not document */
  {"quiet", no_argument, रिक्त, 'q'},
  {"retry", no_argument, रिक्त, RETRY_OPTION},
  {"silent", no_argument, रिक्त, 'q'},
  {"sleep-interval", required_argument, रिक्त, 's'},
  {"verbose", no_argument, रिक्त, 'v'},
  {"zero-terminated", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      printf (_("\
Print the last %d ओळी of each FILE to standard output.\n\
With more than one FILE, precede each with a header giving the file name.\n\
"), DEFAULT_N_ओळी);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --bytes=[+]अंक_2\n\
         output the last अंक_2 bytes;\n\
         or use -c +अंक_2 to output starting with byte अंक_2 of each file\n\
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
  -n, --ओळी=[+]अंक_2\n\
         output the last अंक_2 ओळी, instead of the last %d;\n\
         or use -n +अंक_2 to skip अंक_2-1 ओळी at the start\n\
"), DEFAULT_N_ओळी);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         changed size after N (स्वभाव %d) iterations\n\
         to see यदि it has been unlinked or renamed\n\
         (this is the usual पक्ष of rotated log files);\n\
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
         keep trying to open a file यदि it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep प्रति approximately N seconds\n\
         (स्वभाव 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at least once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output headers giving file names\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ओळ delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (VERSION_OPTION_DESCRIPTION);
     fputs (_("\
\n\
अंक_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on प्रति T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), tail defaults to following the file descriptor, which\n\
means that even यदि a tail'ed file is renamed, tail will अनुवर्त to track\n\
its end.  This स्वभाव behavior is not desirable when you really want to\n\
track the actual name of the file, not the file descriptor (e.g., log\n\
rotation).  Use --follow=name in that पक्ष.  That causes tail to track the\n\
named file in a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
स्थिर शून्य
die_pipe (शून्य)
{
  उत्थापय (SIGPIPE);
  exit (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
स्थिर शून्य
check_output_alive (शून्य)
{
  यदि (! monitor_output)
    प्रत्यागम;

  यदि (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_UNUSED स्थिर bool
valid_file_spec (संरचना File_spec ध्रुव *f)
{
  /* Exactly one of the following subexpressions must be true. */
  प्रत्यागम (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

स्थिर off_t
xlseek (पूर्णाङ्क fd, off_t offset, पूर्णाङ्क whence, वर्ण ध्रुव *prettyname)
{
  off_t new_offset = lseek (fd, offset, whence);

  यदि (0 <= new_offset)
    प्रत्यागम new_offset;

  स्थिर वर्ण ध्रुव *whence_msgid[] = {
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

स्थिर शून्य
record_open_fd (संरचना File_spec *f, पूर्णाङ्क fd,
                off_t read_pos, संरचना stat ध्रुव *st,
                पूर्णाङ्क blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->mode = st->st_mode;
  यदि (S_ISREG (st->st_mode))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

स्थिर शून्य
close_fd (पूर्णाङ्क fd, संरचना File_spec ध्रुव *f)
{
  यदि (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

स्थिर शून्य
write_header (वर्ण ध्रुव *prettyname)
{
  स्थिर bool first_file = true;

  printf ("%s==> %s <==\n", first_file ? "" : "\n", quotef (prettyname));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

स्थिर शून्य
xwrite_stdout (वर्ण ध्रुव *buffer, idx_t n_bytes)
{
  यदि (n_bytes > 0 && fwrite (buffer, 1, n_bytes, stdout) < n_bytes)
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

स्थिर मोजणी_t
dump_remainder (bool want_header, वर्ण ध्रुव *prettyname, पूर्णाङ्क fd,
                मोजणी_t n_bytes)
{
  मोजणी_t n_read = 0;
  मोजणी_t n_remaining = n_bytes;

  कुरु
    {
      वर्ण buffer[BUFSIZ];
      idx_t n = n_bytes < 0 || BUFSIZ < n_remaining ? BUFSIZ : n_remaining;
      ssize_t bytes_read = पठ (fd, buffer, n);
      यदि (bytes_read < 0)
        {
          यदि (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          भङ्ग;
        }
      यदि (bytes_read == 0)
        भङ्ग;
      n_read += bytes_read;

      यदि (want_header)
        {
          write_header (prettyname);
          want_header = false;
        }
      xwrite_stdout (buffer, bytes_read);
      यदि (n_bytes == COPY_A_BUFFER)
        भङ्ग;

      n_remaining -= bytes_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  यावत् (n_remaining != 0);

  प्रत्यागम n_read;
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

स्थिर off_t
file_ओळी (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, संरचना stat ध्रुव *sb,
            मोजणी_t n_ओळी, off_t start_pos, off_t end_pos)
{
  वर्ण *buffer;
  idx_t bufsize = BUFSIZ;
  off_t pos = end_pos;

  यदि (n_ओळी == 0)
    प्रत्यागम pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_mode));
  यदि (sb->st_size % page_size == 0)
    bufsize = MAX (BUFSIZ, page_size);

  buffer = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t bytes_to_read = (pos - start_pos) % bufsize;
  यदि (bytes_to_read == 0)
    bytes_to_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -bytes_to_read, SEEK_CUR, prettyname);
  ssize_t bytes_read = पठ (fd, buffer, bytes_to_read);
  यदि (bytes_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      गच्छ free_buffer;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  यदि (bytes_read && buffer[bytes_read - 1] != ओळ_end)
    --n_ओळी;

  कुरु
    {
      pos += bytes_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      यावत् (n)
        {
          वर्ण ध्रुव *nl;
          nl = memrchr (buffer, ओळ_end, n);
          यदि (nl == रिक्त)
            भङ्ग;
          n = nl - buffer;
          यदि (n_ओळी-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, bytes_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, end_pos - pos);
              गच्छ free_buffer;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      यदि (pos - bytes_read == start_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (buffer, bytes_read);
          dump_remainder (false, prettyname, fd, end_pos - pos);
          गच्छ free_buffer;
        }

      pos = xlseek (fd, -(bufsize + bytes_read), SEEK_CUR, prettyname);
      bytes_read = पठ (fd, buffer, bufsize);
      यदि (bytes_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          गच्छ free_buffer;
        }
    }
  यावत् (bytes_read > 0);

free_buffer:
  मुक्त (buffer);
  प्रत्यागम pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

स्थिर पूर्णाङ्क
pipe_ओळी (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, मोजणी_t n_ओळी)
{
  संरचना linebuffer
  {
    वर्ण buffer[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    संरचना linebuffer *next;
  };
  प्रकारनिर्देश संरचना linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  idx_t total_ओळी = 0;	/* Total number of newlines in all buffers.  */
  पूर्णाङ्क ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (परिमाण (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = रिक्त;
  tmp = xmalloc (परिमाण (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  यावत् (true)
    {
      n_read = पठ (fd, tmp->buffer, BUFSIZ);
      यदि (n_read <= 0)
        भङ्ग;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = रिक्त;

      /* Count the number of newlines just read.  */
      {
        वर्ण ध्रुव *buffer_end = tmp->buffer + n_read;
        वर्ण ध्रुव *p = tmp->buffer;
        यावत् ((p = memchr (p, ओळ_end, buffer_end - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_ओळी += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      यदि (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
          last->nlines += tmp->nlines;
        }
      अन्यथा
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          यदि (total_ओळी - first->nlines > n_ओळी)
            {
              tmp = first;
              total_ओळी -= first->nlines;
              first = first->next;
            }
          अन्यथा
            tmp = xmalloc (परिमाण (LBUFFER));
        }
    }

  मुक्त (tmp);

  यदि (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      गच्छ free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  यदि (last->nbytes == 0)
    गच्छ free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  यदि (n_ओळी == 0)
    गच्छ free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  यदि (last->buffer[last->nbytes - 1] != ओळ_end)
    {
      ++last->nlines;
      ++total_ओळी;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  प्रति (tmp = first; total_ओळी - tmp->nlines > n_ओळी; tmp = tmp->next)
    total_ओळी -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    वर्ण ध्रुव *beg = tmp->buffer;
    वर्ण ध्रुव *buffer_end = tmp->buffer + tmp->nbytes;
    यदि (total_ओळी > n_ओळी)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        प्रति (idx_t j = total_ओळी - n_ओळी; j; --j)
          {
            beg = rawmemchr (beg, ओळ_end);
            ++beg;
          }
      }

    xwrite_stdout (beg, buffer_end - beg);
  }

  प्रति (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

free_lbuffers:
  यावत् (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }
  प्रत्यागम ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
pipe_bytes (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, मोजणी_t n_bytes,
            off_t read_pos)
{
  संरचना charbuffer
  {
    वर्ण buffer[BUFSIZ];
    idx_t nbytes;
    संरचना charbuffer *next;
  };
  प्रकारनिर्देश संरचना charbuffer CBUFFER;
  CBUFFER *first, *last, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t total_bytes = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  यदि (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  first = last = xmalloc (परिमाण (CBUFFER));
  first->nbytes = 0;
  first->next = रिक्त;
  tmp = xmalloc (परिमाण (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  यावत् (true)
    {
      n_read = पठ (fd, tmp->buffer, BUFSIZ);
      यदि (n_read <= 0)
        भङ्ग;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->next = रिक्त;

      total_bytes += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      यदि (tmp->nbytes + last->nbytes < BUFSIZ)
        {
          memcpy (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
          last->nbytes += tmp->nbytes;
        }
      अन्यथा
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          last = last->next = tmp;
          यदि (total_bytes - first->nbytes > n_bytes)
            {
              tmp = first;
              total_bytes -= first->nbytes;
              first = first->next;
            }
          अन्यथा
            {
              tmp = xmalloc (परिमाण (CBUFFER));
            }
        }
    }

  मुक्त (tmp);

  यदि (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      गच्छ free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  प्रति (tmp = first; total_bytes - tmp->nbytes > n_bytes; tmp = tmp->next)
    total_bytes -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  यदि (total_bytes > n_bytes)
    i = total_bytes - n_bytes;
  अन्यथा
    i = 0;
  xwrite_stdout (&tmp->buffer[i], tmp->nbytes - i);

  प्रति (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

  यदि (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  यावत् (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }
  प्रत्यागम read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

स्थिर पूर्णाङ्क
start_bytes (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, मोजणी_t n_bytes)
{
  वर्ण buffer[BUFSIZ];

  यावत् (0 < n_bytes)
    {
      ssize_t bytes_read = पठ (fd, buffer, BUFSIZ);
      यदि (bytes_read == 0)
        प्रत्यागम -1;
      यदि (bytes_read < 0)
        {
          पूर्णाङ्क ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          प्रत्यागम ret;
        }
      यदि (bytes_read <= n_bytes)
        n_bytes -= bytes_read;
      अन्यथा
        {
          xwrite_stdout (&buffer[n_bytes], bytes_read - n_bytes);
          भङ्ग;
        }
    }

  प्रत्यागम 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

स्थिर पूर्णाङ्क
start_ओळी (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, मोजणी_t n_ओळी)
{
  यदि (n_ओळी == 0)
    प्रत्यागम 0;

  यावत् (true)
    {
      वर्ण buffer[BUFSIZ];
      ssize_t bytes_read = पठ (fd, buffer, BUFSIZ);
      यदि (bytes_read == 0) /* EOF */
        प्रत्यागम -1;
      यदि (bytes_read < 0) /* error */
        {
          पूर्णाङ्क ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          प्रत्यागम ret;
        }

      वर्ण *buffer_end = buffer + bytes_read;
      वर्ण *p = buffer;
      यावत् ((p = memchr (p, ओळ_end, buffer_end - p)))
        {
          ++p;
          यदि (--n_ओळी == 0)
            {
              यदि (p < buffer_end)
                xwrite_stdout (p, buffer_end - p);
              प्रत्यागम 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
स्थिर bool
fremote (पूर्णाङ्क fd, संरचना File_spec ध्रुव *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#यदि HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  संरचना statfs buf;
  पूर्णाङ्क err = fstatfs (fd, &buf);
  यदि (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      यदि (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  अन्यथा
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#endif

  प्रत्यागम remote;
}

/* open/fstat F->name and handle changes.  */
स्थिर शून्य
recheck (संरचना File_spec *f, bool blocking)
{
  संरचना stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  पूर्णाङ्क prev_errnum = f->errnum;
  bool new_file;
  पूर्णाङ्क fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  पूर्णाङ्क open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  यदि (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  अन्यथा यदि (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      यदि (fd < 0)
        {
          यदि (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          अन्यथा
            {
              /* say nothing... it's still not tailable */
            }
        }
      अन्यथा यदि (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  अन्यथा यदि (!IS_TAILABLE_FILE_TYPE (new_stats.st_mode))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_mode == Follow_name);
      यदि (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  अन्यथा यदि ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  अन्यथा
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  यदि (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  अन्यथा यदि (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  अन्यथा यदि (f->fd < 0)
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
  अन्यथा यदि (!SAME_INODE (*f, new_stats))
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
  अन्यथा
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
  यदि (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      record_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

स्थिर bool
any_live_files (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  यदि (reopen_inaccessible_files && follow_mode == Follow_name)
    प्रत्यागम true;

  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      यदि (0 <= f[i].fd)
        प्रत्यागम true;
      अन्यथा
        {
          यदि (! f[i].ignore && reopen_inaccessible_files)
            प्रत्यागम true;
        }
    }

  प्रत्यागम false;
}

/* Return true if any writers exist, even as zombies.  */

स्थिर bool
some_writers_exist (शून्य)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  प्रति (idx_t i = 0; i < nbpids; )
    {
      यदि (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * परिमाण pids[i]);
        }
      अन्यथा
        i++;
    }


  प्रत्यागम 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

स्थिर शून्य
tail_forever (संरचना File_spec *f, पूर्णाङ्क n_files, द्विगुण sleep_interval)
{
  पूर्णाङ्क last = n_files - 1;

  स्थिर bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  यदि (fflush (stdout) < 0)
    write_error ();

  यावत् (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_mode == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].mode));

      यदि (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_आदान = false;

      प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
        {
          संरचना stat stats;

          यदि (f[i].ignore)
            अनुवर्त;

          पूर्णाङ्क fd = f[i].fd;
          यदि (fd < 0)
            {
              recheck (&f[i], blocking);
              अनुवर्त;
            }

          वर्ण ध्रुव *prettyname = f[i].prettyname;
          mode_t mode = f[i].mode;

          यदि (f[i].blocking != blocking)
            {
              पूर्णाङ्क old_flags = fcntl (fd, F_GETFL);
              पूर्णाङ्क new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              यदि (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  यदि (S_ISREG (f[i].mode) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  अन्यथा
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              अन्यथा
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          यदि (!f[i].blocking)
            {
              यदि (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  अनुवर्त;
                }

              यदि (f[i].mode == stats.st_mode
                  && (! S_ISREG (stats.st_mode)
                      || f[i].read_pos == stats.st_size)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  यदि ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_mode == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  यदि (fd != f[i].fd || S_ISREG (stats.st_mode) || 1 < n_files)
                    अनुवर्त;
                  अन्यथा
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].mode = stats.st_mode;

              /* reset counter */
              यदि (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              यदि (S_ISREG (mode) && stats.st_size < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              यदि (i != last)
                {
                  यदि (print_headers)
                    write_header (prettyname);
                  last = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          मोजणी_t bytes_to_read;
          यदि (f[i].blocking)
            bytes_to_read = COPY_A_BUFFER;
          अन्यथा यदि (S_ISREG (mode) && f[i].remote)
            bytes_to_read = stats.st_size - f[i].read_pos;
          अन्यथा
            bytes_to_read = COPY_TO_EOF;

          मोजणी_t nr = dump_remainder (false, prettyname, fd, bytes_to_read);
          यदि (0 < nr)
            {
              यदि (S_ISREG (mode))
                f[i].read_pos += nr;
              यदि (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_आदान = true;
            }
        }

      यदि (! any_live_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          भङ्ग;
        }

      यदि ((!any_आदान || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      यदि (!any_आदान)
        {
          यदि (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              यदि (!nbpids)
                भङ्ग;
              यदि (!some_writers_exist ())
                अनुवर्त;
            }

          यदि (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#यदि HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

स्थिर bool
any_remote_file (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    यदि (0 <= f[i].fd && f[i].remote)
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

स्थिर bool
any_non_remote_file (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    यदि (0 <= f[i].fd && ! f[i].remote)
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

स्थिर bool
any_symlinks (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    यदि (issymlink (f[i].name) == 1)
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

स्थिर bool
any_non_regular_fifo (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    यदि (0 <= f[i].fd && ! S_ISREG (f[i].mode) && ! S_ISFIFO (f[i].mode))
      प्रत्यागम true;
  प्रत्यागम false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

स्थिर bool
tailable_stdin (ध्रुव संरचना File_spec *f, पूर्णाङ्क n_files)
{
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    यदि (!f[i].ignore && streq (f[i].name, "-"))
      प्रत्यागम true;
  प्रत्यागम false;
}

स्थिर size_t
wd_hasher (ध्रुव शून्य *entry, size_t tabsize)
{
  ध्रुव संरचना File_spec *spec = entry;
  प्रत्यागम spec->wd % tabsize;
}

स्थिर bool
wd_comparator (ध्रुव शून्य *e1, ध्रुव शून्य *e2)
{
  ध्रुव संरचना File_spec *spec1 = e1;
  ध्रुव संरचना File_spec *spec2 = e2;
  प्रत्यागम spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
स्थिर size_t
parent_name_hasher (ध्रुव शून्य *entry, size_t tabsize)
{
  ध्रुव संरचना File_spec *spec = entry;
  वर्ण ध्रुव *name = spec->name + spec->basename_start;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  प्रत्यागम (hash_string (name, tabsize) + (size_t) spec->parent_wd) % tabsize;
}

स्थिर bool
parent_name_comparator (ध्रुव शून्य *e1, ध्रुव शून्य *e2)
{
  ध्रुव संरचना File_spec *a = e1;
  ध्रुव संरचना File_spec *b = e2;
  प्रत्यागम (a->parent_wd == b->parent_wd
          && streq (a->name + a->basename_start,
                    b->name + b->basename_start));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
स्थिर शून्य
check_fspec (संरचना File_spec *fspec, संरचना File_spec **prev_fspec)
{
  यदि (fspec->fd < 0)
    प्रत्यागम;

  संरचना stat stats;
  यदि (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      प्रत्यागम;
    }

  वर्ण ध्रुव *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  यदि (S_ISREG (fspec->mode) && stats.st_size < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  अन्यथा यदि (S_ISREG (fspec->mode) && stats.st_size == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    प्रत्यागम;

  bool want_header = print_headers && (fspec != *prev_fspec);

  मोजणी_t nr = dump_remainder (want_header, prettyname, fspec->fd, COPY_TO_EOF);
  यदि (0 < nr)
    {
      यदि (S_ISREG (fspec->mode))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      यदि (fflush (stdout) < 0)
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
स्थिर शून्य
tail_forever_inotify (पूर्णाङ्क wd, संरचना File_spec *f, पूर्णाङ्क n_files,
                      द्विगुण sleep_interval, Hash_table **wd_to_namep,
                      Hash_table **parent_by_namep)
{
# यदि TAIL_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  पूर्णाङ्क max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_table *wd_to_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_table *parent_by_name = रिक्त;

  bool found_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool found_unwatchable_dir = false;
  bool no_inotify_resources = false;
  संरचना File_spec *prev_fspec;
  idx_t evlen = 0;
  वर्ण *evbuf;
  idx_t evbuf_off = 0;

  *parent_by_namep = रिक्त;

  wd_to_name = hash_initialize (n_files, रिक्त, wd_hasher, wd_comparator,
                                रिक्त);
  यदि (! wd_to_name)
    xalloc_die ();
  *wd_to_namep = wd_to_name;

  यदि (follow_mode == Follow_name)
    {
      parent_by_name = hash_initialize (n_files, रिक्त, parent_name_hasher,
                                        parent_name_comparator, रिक्त);
      यदि (! parent_by_name)
        xalloc_die ();
      *parent_by_namep = parent_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_mask = IN_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  यदि (follow_mode == Follow_name)
    inotify_wd_mask |= (IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      यदि (!f[i].ignore)
        {
          idx_t fnlen = strlen (f[i].name);
          यदि (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          यदि (follow_mode == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              वर्ण prev = f[i].name[dirlen];
              f[i].basename_start = last_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].parent_wd = inotify_add_watch (wd, dirlen ? f[i].name : ".",
                                                  (IN_CREATE | IN_DELETE
                                                   | IN_MOVED_TO | IN_ATTRIB
                                                   | IN_DELETE_SELF));

              f[i].name[dirlen] = prev;

              यदि (f[i].parent_wd < 0)
                {
                  यदि (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  अन्यथा
                    error (0, 0, _("inotify resources exhausted"));
                  found_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  भङ्ग;
                }

              यदि (hash_घाला (parent_by_name, &(f[i])) == रिक्त)
                xalloc_die ();
            }

          f[i].wd = inotify_add_watch (wd, f[i].name, inotify_wd_mask);

          यदि (f[i].wd < 0)
            {
              यदि (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              यदि (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  भङ्ग;
                }
              अन्यथा यदि (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              अनुवर्त;
            }

          यदि (hash_घाला (wd_to_name, &(f[i])) == रिक्त)
            xalloc_die ();

          found_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  यदि (no_inotify_resources || found_unwatchable_dir
      || (follow_mode == Follow_descriptor && tailed_but_unwatchable))
    प्रत्यागम;
  यदि (follow_mode == Follow_descriptor && !found_watchable_file)
    exit (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      यदि (! f[i].ignore)
        {
          /* check for new files.  */
          यदि (follow_mode == Follow_name)
            recheck (&(f[i]), false);
          अन्यथा यदि (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              संरचना stat stats;

              यदि (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  प्रत्यागम;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  यदि (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += परिमाण (संरचना inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  यावत् (true)
    {
      संरचना File_spec *fspec;
      संरचना inotify_event *ev;
      शून्य *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      यदि (follow_mode == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_to_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      यदि (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          पूर्णाङ्क file_change;
          संरचना pollfd pfd[2];
          कुरु
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              पूर्णाङ्क delay = -1;

              यदि (pids)
                {
                  यदि (!nbpids)
                    exit (EXIT_SUCCESS);

                  यदि (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  अन्यथा यदि (sleep_interval < INT_MAX / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      द्विगुण ddelay = sleep_interval * 1000;
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
          यावत् (file_change == 0 || (file_change < 0 && errno == EINTR));

          यदि (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          यदि (pfd[1].revents)
            die_pipe ();

          len = पठ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          यदि ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              अनुवर्त;
            }

          यदि (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += परिमाण (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      यदि ((ev->mask & IN_DELETE_SELF) && ! ev->len)
        {
          प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
            {
              यदि (ev->wd == f[i].parent_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  प्रत्यागम;
                }
            }
        }

      यदि (ev->len) /* event on ev->name in watched directory.  */
        {
          संरचना File_spec किल्ली;
          यदि (! parent_by_name)
            अनुवर्त;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          किल्ली.parent_wd = ev->wd;
          किल्ली.name = ev->name;
          किल्ली.basename_start = 0;
          fspec = hash_lookup (parent_by_name, &किल्ली);

          /* It is not a watched file.  */
          यदि (! fspec)
            अनुवर्त;

          पूर्णाङ्क new_wd = -1;
          bool deleting = !! (ev->mask & IN_DELETE);

          यदि (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_add_watch (wd, fspec->name, inotify_wd_mask);
            }

          यदि (! deleting && new_wd < 0)
            {
              यदि (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  प्रत्यागम; /* revert to polling.  */
                }
              अन्यथा
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          यदि (new_watch)
            {
              यदि (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_to_name, fspec);
                }

              fspec->wd = new_wd;

              यदि (new_wd < 0)
                अनुवर्त;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              संरचना File_spec *prev = hash_remove (wd_to_name, fspec);
              यदि (prev && prev != fspec)
                {
                  यदि (follow_mode == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              यदि (hash_घाला (wd_to_name, fspec) == रिक्त)
                xalloc_die ();
            }

          यदि (follow_mode == Follow_name)
            recheck (fspec, false);
        }
      अन्यथा
        {
          संरचना File_spec किल्ली;
          किल्ली.wd = ev->wd;
          fspec = hash_lookup (wd_to_name, &किल्ली);
        }

      यदि (! fspec)
        अनुवर्त;

      यदि (ev->mask & (IN_ATTRIB | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          यदि (ev->mask & IN_DELETE_SELF
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

          अनुवर्त;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#endif

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
tail_bytes (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, संरचना stat ध्रुव *st,
            मोजणी_t n_bytes)
{
  off_t current_pos
    = (presume_आदान_pipe
       ? -1
       : lseek (fd, from_start ? MIN (n_bytes, OFF_T_MAX) : 0, SEEK_CUR));

  यदि (from_start)
    {
      यदि (current_pos < 0)
        {
          पूर्णाङ्क t = start_bytes (prettyname, fd, n_bytes);
          यदि (t < 0)
            प्रत्यागम t;
        }
      n_bytes = COPY_TO_EOF;
    }
  अन्यथा
    {
      off_t initial_pos = current_pos;
      off_t end_pos = -1;

      यदि (0 <= current_pos)
        {
          यदि (usable_st_size (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_size = STP_BLKSIZE (st);
              यदि (smallish_size < st->st_size)
                end_pos = st->st_size;
            }
          अन्यथा
            {
              off_t minus_n;
              यदि (ckd_sub (&minus_n, 0, n_bytes))
                end_pos = 0;
              अन्यथा
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  यदि (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      end_pos = e_n_pos + n_bytes;
                    }
                  अन्यथा
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      यदि (0 <= e_pos)
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
      यदि (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      यदि (end_pos < 0)
        प्रत्यागम pipe_bytes (prettyname, fd, n_bytes, current_pos);
    }

  मोजणी_t nr = dump_remainder (false, prettyname, fd, n_bytes);
  प्रत्यागम current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
tail_ओळी (वर्ण ध्रुव *prettyname, पूर्णाङ्क fd, संरचना stat ध्रुव *st,
            मोजणी_t n_ओळी)
{
  यदि (from_start)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      यदि (OFF_T_MAX <= n_ओळी)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          यदि (0 <= e)
            प्रत्यागम e;
        }

      पूर्णाङ्क t = start_ओळी (prettyname, fd, n_ओळी);
      यदि (t < 0)
        प्रत्यागम t;
      dump_remainder (false, prettyname, fd, COPY_TO_EOF);
      प्रत्यागम -1;
    }
  अन्यथा
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t start_pos = (!presume_आदान_pipe && S_ISREG (st->st_mode)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t end_pos = start_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      प्रत्यागम (end_pos < 0
              ? pipe_ओळी (prettyname, fd, n_ओळी)
              : start_pos < end_pos
              ? file_ओळी (prettyname, fd, st, n_ओळी, start_pos, end_pos)
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

स्थिर off_t
tail (वर्ण ध्रुव *filename, पूर्णाङ्क fd, संरचना stat ध्रुव *st, मोजणी_t n_units)
{
  प्रत्यागम (मोजणी_ओळी ? tail_ओळी : tail_bytes) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

स्थिर bool
tail_file (संरचना File_spec *f, मोजणी_t n_files, मोजणी_t n_units)
{
  पूर्णाङ्क fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  यदि (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    fd = open (f->name, O_RDONLY | O_द्विमान | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  यदि (fd < 0)
    {
      यदि (forever)
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
  अन्यथा
    {
      यदि (print_headers)
        write_header (f->prettyname);

      off_t read_pos;
      संरचना stat stats;
      ok = 0 <= fstat (fd, &stats);
      यदि (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      अन्यथा
        {
          read_pos = tail (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          यदि (IS_TAILABLE_FILE_TYPE (stats.st_mode))
            f->tailable = true;
          अन्यथा यदि (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      यदि (forever)
        {
          यदि (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          अन्यथा
            {
              record_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      अन्यथा
        {
          यदि (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  प्रत्यागम ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

स्थिर bool
parse_obsolete_option (पूर्णाङ्क argc, वर्ण * ध्रुव *argv, मोजणी_t *n_units)
{
  bool t_from_start;
  bool t_मोजणी_ओळी = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  यदि (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    प्रत्यागम false;

  पूर्णाङ्क posix_ver = posix2_version ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  वर्ण ध्रुव *p = argv[1];

  विकल्प (*p++)
    {
    स्वभाव:
      प्रत्यागम false;

    पक्ष '+':
      /* Leading "+" is a file name in the standard form.  */
      यदि (!traditional_usage)
        प्रत्यागम false;

      t_from_start = true;
      भङ्ग;

    पक्ष '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      यदि (!obsolete_usage && !p[p[0] == 'c'])
        प्रत्यागम false;

      t_from_start = false;
      भङ्ग;
    }

  मोजणी_t n;
  यदि (!c_isdigit (*p))
    n = DEFAULT_N_ओळी;
  अन्यथा
    प्रति (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_add (&n, n, *p - '0') ? मोजणी_MAX : n;

  विकल्प (*p)
    {
    पक्ष 'b': n = ckd_mul (&n, n, 512) ? मोजणी_MAX : n; FALLTHROUGH;
    पक्ष 'c': t_मोजणी_ओळी = false; FALLTHROUGH;
    पक्ष 'l': p++; भङ्ग;
    }

  यदि (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  यदि (*p)
    प्रत्यागम false;

  *n_units = n;

  /* Set globals.  */
  from_start = t_from_start;
  मोजणी_ओळी = t_मोजणी_ओळी;
  forever = t_forever;

  प्रत्यागम true;
}

स्थिर शून्य
parse_options (पूर्णाङ्क argc, वर्ण **argv,
               मोजणी_t *n_units, गणना header_mode *header_mode,
               द्विगुण *sleep_interval)
{
  पूर्णाङ्क c;

  यावत् ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'F':
          forever = true;
          follow_mode = Follow_name;
          reopen_inaccessible_files = true;
          भङ्ग;

        पक्ष 'c':
        पक्ष 'n':
          मोजणी_ओळी = (c == 'n');
          यदि (*optarg == '+')
            from_start = true;
          अन्यथा यदि (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, मोजणी_MAX, "bkKmMGTPEZYRQ0",
                                 (मोजणी_ओळी
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          भङ्ग;

        पक्ष 'f':
        पक्ष LONG_FOLLOW_OPTION:
          forever = true;
          यदि (optarg == रिक्त)
            follow_mode = DEFAULT_FOLLOW_MODE;
          अन्यथा
            follow_mode = XARGMATCH ("--follow", optarg,
                                     follow_mode_string, follow_mode_map);
          भङ्ग;

        पक्ष RETRY_OPTION:
          reopen_inaccessible_files = true;
          भङ्ग;

        पक्ष MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, मोजणी_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          भङ्ग;

        पक्ष DEBUG_PROGRAM_OPTION:
          debug = true;
          भङ्ग;

        पक्ष DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          भङ्ग;

        पक्ष PID_OPTION:
          यदि (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (INT_MAX, IDX_MAX), परिमाण *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          भङ्ग;

        पक्ष PRESUME_आदान_PIPE_OPTION:
          presume_आदान_pipe = true;
          भङ्ग;

        पक्ष 'q':
          *header_mode = never;
          भङ्ग;

        पक्ष 's':
          {
            वर्ण *ep;
            errno = 0;
            द्विगुण s = cl_strtod (optarg, &ep);
            यदि (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          भङ्ग;

        पक्ष 'v':
          *header_mode = always;
          भङ्ग;

        पक्ष 'z':
          ओळ_end = '\0';
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
        पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (reopen_inaccessible_files)
    {
      यदि (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      अन्यथा यदि (follow_mode == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  यदि (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      मुक्त (pids);
      pids = रिक्त;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
स्थिर bool
ignore_fifo_and_pipe (संरचना File_spec *f, पूर्णाङ्क n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].mode)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      यदि (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      अन्यथा
        some_viable = true;
    }

  प्रत्यागम some_viable;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  गणना header_mode header_mode = multiple_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  मोजणी_t n_units = DEFAULT_N_ओळी;
  पूर्णाङ्क n_files;
  वर्ण **file;
  संरचना File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  द्विगुण sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    पूर्णाङ्क p = getpagesize ();
    यदि (IDX_MAX < p)
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
  n_units -= from_start && 0 < n_units && n_units < मोजणी_MAX;

  यदि (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  अन्यथा
    {
      स्थिर वर्ण *dummy_stdin = (वर्ण *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool found_hyphen = false;

    प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
      यदि (streq (file[i], "-"))
        found_hyphen = true;

    /* When following by name, there must be a name.  */
    यदि (found_hyphen && follow_mode == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    यदि (forever && found_hyphen)
      {
        संरचना stat in_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_mode == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &in_stat)
                          && ! S_ISREG (in_stat.st_mode));

        यदि (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  यदि (! forever && n_units == (from_start ? मोजणी_MAX : 0))
    प्रत्यागम EXIT_SUCCESS;

  यदि (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, परिमाण *F);
  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  यदि (header_mode == always
      || (header_mode == multiple_files && n_files > 1))
    print_headers = true;

  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  प्रति (पूर्णाङ्क i = 0; i < n_files; i++)
    ok &= tail_file (&F[i], n_files, n_units);

  यदि (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      संरचना stat out_stat;
      यदि (fstat (STDOUT_FILENO, &out_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (out_stat.st_mode)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#यदि HAVE_INOTIFY
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
      यदि (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_mode == Follow_descriptor)))
        disable_inotify = true;

      यदि (!disable_inotify)
        {
          पूर्णाङ्क wd = inotify_init ();
          यदि (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              यदि (fflush (stdout) < 0)
                write_error ();

              Hash_table *ht;
              Hash_table *parent_ht = रिक्त;
              tail_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &parent_ht);
              hash_free (ht);
              यदि (parent_ht)
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

  यदि (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
