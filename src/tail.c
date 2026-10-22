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

#जर HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#endif

/* Linux can optimize the handling of local files.  */
#जर defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# जर HAVE_SYS_STATFS_H
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
प्रकारव्याख्या intmax_t मोजणी_t;
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

स्थिर अक्षर स्थिरांक *स्थिरांक follow_mode_string[] =
{
  "descriptor", "name", रिक्त
};

स्थिर गणना Follow_mode स्थिरांक follow_mode_map[] =
{
  Follow_descriptor, Follow_name,
};

संरचना File_spec
{
  /* The actual file name, or "-" for stdin.  */
  अक्षर *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  अक्षर स्थिरांक *prettyname;

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
  पूर्णांक fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  पूर्णांक errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  पूर्णांक blocking;

#जर HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  पूर्णांक wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  पूर्णांक parent_wd;

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
स्थिर अक्षर ओळ_end = '\n';

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
स्थिर पूर्णांक nbpids = 0;
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
  RETRY_OPTION = अक्षर_उच्च + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_आदान_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_PROGRAM_OPTION,
};

स्थिर संरचना option स्थिरांक long_options[] =
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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      म_लिखो (_("\
Print the last %d ओळी of each खाता to standard output.\n\
With more than one खाता, precede each with a header giving the file name.\n\
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
         with --follow=name, reopen a खाता which has not\n\
         changed size after N (पूर्वनिर्धारित %d) iterations\n\
         to see जर it has been unlinked or renamed\n\
         (this is the usual स्थिती of rotated log files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, निकास after PID no longer exists;\n\
         can be repeated to watch multiple processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output headers giving file names\n\
"));
     oputs (_("\
      --retry\n\
         keep trying to खोलो a file जर it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep साठी approximately N seconds\n\
         (पूर्वनिर्धारित 1.0) between iterations;\n\
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
     ख_माला_दो (_("\
\n\
अंक_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on साठी T, P, E, Z, Y, R, Q.\n\
द्विमान_2 prefixes can be used, too: KiB=K, MiB=M, and so on.\n\
\n\
"), मानक_निकास);
     ख_माला_दो (_("\
With --follow (-f), tail defaults to following the file descriptor, which\n\
means that even जर a tail'ed file is renamed, tail will सुरूठेवा to track\n\
its end.  This पूर्वनिर्धारित behavior is not desirable when you really want to\n\
track the actual name of the file, not the file descriptor (e.g., log\n\
rotation).  Use --follow=name in that स्थिती.  That causes tail to track the\n\
named file in a way that accommodates renaming, removal and creation.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
स्थिर शून्य
die_pipe (शून्य)
{
  उठवा (SIGPIPE);
  निकास (निकास_त्रुटि);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
स्थिर शून्य
check_output_alive (शून्य)
{
  जर (! monitor_output)
    परत;

  जर (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_UNUSED स्थिर bool
valid_file_spec (संरचना File_spec स्थिरांक *f)
{
  /* Exactly one of the following subexpressions must be true. */
  परत (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

स्थिर off_t
xlseek (पूर्णांक fd, off_t offset, पूर्णांक whence, अक्षर स्थिरांक *prettyname)
{
  off_t new_offset = lseek (fd, offset, whence);

  जर (0 <= new_offset)
    परत new_offset;

  स्थिर अक्षर स्थिरांक *whence_msgid[] = {
    [शुरू_से] = N_("%s: cannot seek to offset %jd"),
    [प्रस्तुत_से] = N_("%s: cannot seek to relative offset %jd"),
    [अंत_से] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = offset;
  error (निकास_त्रुटि, त्रुटि_सं, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

स्थिर शून्य
record_open_fd (संरचना File_spec *f, पूर्णांक fd,
                off_t read_pos, संरचना stat स्थिरांक *st,
                पूर्णांक blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->mode = st->st_mode;
  जर (S_ISREG (st->st_mode))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, प्रस्तुत_से, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

स्थिर शून्य
close_fd (पूर्णांक fd, संरचना File_spec स्थिरांक *f)
{
  जर (STDIN_FILENO < fd && बंद (fd) < 0)
    error (0, त्रुटि_सं, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

स्थिर शून्य
write_header (अक्षर स्थिरांक *prettyname)
{
  स्थिर bool first_file = true;

  म_लिखो ("%s==> %s <==\n", first_file ? "" : "\n", quotef (prettyname));
  first_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

स्थिर शून्य
xwrite_stdout (अक्षर स्थिरांक *buffer, idx_t n_bytes)
{
  जर (n_bytes > 0 && ख_डालो (buffer, 1, n_bytes, मानक_निकास) < n_bytes)
    {
      साफ_त्रुटि (मानक_निकास); /* To avoid redundant close_stdout diagnostic.  */
      error (निकास_त्रुटि, त्रुटि_सं, _("error writing %s"),
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
dump_remainder (bool want_header, अक्षर स्थिरांक *prettyname, पूर्णांक fd,
                मोजणी_t n_bytes)
{
  मोजणी_t n_read = 0;
  मोजणी_t n_remaining = n_bytes;

  करा
    {
      अक्षर buffer[बफ_मान];
      idx_t n = n_bytes < 0 || बफ_मान < n_remaining ? बफ_मान : n_remaining;
      ssize_t bytes_read = पठ (fd, buffer, n);
      जर (bytes_read < 0)
        {
          जर (त्रुटि_सं != EAGAIN)
            error (निकास_त्रुटि, त्रुटि_सं, _("error reading %s"),
                   quoteaf (prettyname));
          थांबा;
        }
      जर (bytes_read == 0)
        थांबा;
      n_read += bytes_read;

      जर (want_header)
        {
          write_header (prettyname);
          want_header = false;
        }
      xwrite_stdout (buffer, bytes_read);
      जर (n_bytes == COPY_A_BUFFER)
        थांबा;

      n_remaining -= bytes_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  पर्यंत (n_remaining != 0);

  परत n_read;
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
file_ओळी (अक्षर स्थिरांक *prettyname, पूर्णांक fd, संरचना stat स्थिरांक *sb,
            मोजणी_t n_ओळी, off_t start_pos, off_t end_pos)
{
  अक्षर *buffer;
  idx_t bufsize = बफ_मान;
  off_t pos = end_pos;

  जर (n_ओळी == 0)
    परत pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_mode));
  जर (sb->st_size % page_size == 0)
    bufsize = MAX (बफ_मान, page_size);

  buffer = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t bytes_to_read = (pos - start_pos) % bufsize;
  जर (bytes_to_read == 0)
    bytes_to_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -bytes_to_read, प्रस्तुत_से, prettyname);
  ssize_t bytes_read = पठ (fd, buffer, bytes_to_read);
  जर (bytes_read < 0)
    {
      pos = -1 - त्रुटि_सं;
      error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
      जा free_buffer;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  जर (bytes_read && buffer[bytes_read - 1] != ओळ_end)
    --n_ओळी;

  करा
    {
      pos += bytes_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = bytes_read;
      पर्यंत (n)
        {
          अक्षर स्थिरांक *nl;
          nl = memrchr (buffer, ओळ_end, n);
          जर (nl == रिक्त)
            थांबा;
          n = nl - buffer;
          जर (n_ओळी-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, bytes_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, end_pos - pos);
              जा free_buffer;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      जर (pos - bytes_read == start_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (buffer, bytes_read);
          dump_remainder (false, prettyname, fd, end_pos - pos);
          जा free_buffer;
        }

      pos = xlseek (fd, -(bufsize + bytes_read), प्रस्तुत_से, prettyname);
      bytes_read = पठ (fd, buffer, bufsize);
      जर (bytes_read < 0)
        {
          pos = -1 - त्रुटि_सं;
          error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
          जा free_buffer;
        }
    }
  पर्यंत (bytes_read > 0);

free_buffer:
  मुक्त (buffer);
  परत pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

स्थिर पूर्णांक
pipe_ओळी (अक्षर स्थिरांक *prettyname, पूर्णांक fd, मोजणी_t n_ओळी)
{
  संरचना linebuffer
  {
    अक्षर buffer[बफ_मान];
    idx_t nbytes;
    idx_t nlines;
    संरचना linebuffer *next;
  };
  प्रकारव्याख्या संरचना linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  idx_t total_ओळी = 0;	/* Total number of newlines in all buffers.  */
  पूर्णांक ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  first = last = xmalloc (आकार (LBUFFER));
  first->nbytes = first->nlines = 0;
  first->next = रिक्त;
  tmp = xmalloc (आकार (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  पर्यंत (true)
    {
      n_read = पठ (fd, tmp->buffer, बफ_मान);
      जर (n_read <= 0)
        थांबा;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->next = रिक्त;

      /* Count the number of newlines just read.  */
      {
        अक्षर स्थिरांक *buffer_end = tmp->buffer + n_read;
        अक्षर स्थिरांक *p = tmp->buffer;
        पर्यंत ((p = स_प्रथम (p, ओळ_end, buffer_end - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      total_ओळी += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      जर (tmp->nbytes + last->nbytes < बफ_मान)
        {
          स_नकल (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
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
          जर (total_ओळी - first->nlines > n_ओळी)
            {
              tmp = first;
              total_ओळी -= first->nlines;
              first = first->next;
            }
          अन्यथा
            tmp = xmalloc (आकार (LBUFFER));
        }
    }

  मुक्त (tmp);

  जर (n_read < 0 && त्रुटि_सं != EAGAIN)
    {
      ret = -1 - त्रुटि_सं;
      error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
      जा free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  जर (last->nbytes == 0)
    जा free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  जर (n_ओळी == 0)
    जा free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  जर (last->buffer[last->nbytes - 1] != ओळ_end)
    {
      ++last->nlines;
      ++total_ओळी;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  साठी (tmp = first; total_ओळी - tmp->nlines > n_ओळी; tmp = tmp->next)
    total_ओळी -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    अक्षर स्थिरांक *beg = tmp->buffer;
    अक्षर स्थिरांक *buffer_end = tmp->buffer + tmp->nbytes;
    जर (total_ओळी > n_ओळी)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        साठी (idx_t j = total_ओळी - n_ओळी; j; --j)
          {
            beg = rawmemchr (beg, ओळ_end);
            ++beg;
          }
      }

    xwrite_stdout (beg, buffer_end - beg);
  }

  साठी (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

free_lbuffers:
  पर्यंत (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }
  परत ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
pipe_bytes (अक्षर स्थिरांक *prettyname, पूर्णांक fd, मोजणी_t n_bytes,
            off_t read_pos)
{
  संरचना charbuffer
  {
    अक्षर buffer[बफ_मान];
    idx_t nbytes;
    संरचना charbuffer *next;
  };
  प्रकारव्याख्या संरचना charbuffer CBUFFER;
  CBUFFER *first, *last, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t total_bytes = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  जर (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  first = last = xmalloc (आकार (CBUFFER));
  first->nbytes = 0;
  first->next = रिक्त;
  tmp = xmalloc (आकार (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  पर्यंत (true)
    {
      n_read = पठ (fd, tmp->buffer, बफ_मान);
      जर (n_read <= 0)
        थांबा;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->next = रिक्त;

      total_bytes += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      जर (tmp->nbytes + last->nbytes < बफ_मान)
        {
          स_नकल (&last->buffer[last->nbytes], tmp->buffer, tmp->nbytes);
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
          जर (total_bytes - first->nbytes > n_bytes)
            {
              tmp = first;
              total_bytes -= first->nbytes;
              first = first->next;
            }
          अन्यथा
            {
              tmp = xmalloc (आकार (CBUFFER));
            }
        }
    }

  मुक्त (tmp);

  जर (n_read < 0 && त्रुटि_सं != EAGAIN)
    {
      read_pos = -1 - त्रुटि_सं;
      error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
      जा free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  साठी (tmp = first; total_bytes - tmp->nbytes > n_bytes; tmp = tmp->next)
    total_bytes -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  जर (total_bytes > n_bytes)
    i = total_bytes - n_bytes;
  अन्यथा
    i = 0;
  xwrite_stdout (&tmp->buffer[i], tmp->nbytes - i);

  साठी (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite_stdout (tmp->buffer, tmp->nbytes);

  जर (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  पर्यंत (first)
    {
      tmp = first->next;
      मुक्त (first);
      first = tmp;
    }
  परत read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

स्थिर पूर्णांक
start_bytes (अक्षर स्थिरांक *prettyname, पूर्णांक fd, मोजणी_t n_bytes)
{
  अक्षर buffer[बफ_मान];

  पर्यंत (0 < n_bytes)
    {
      ssize_t bytes_read = पठ (fd, buffer, बफ_मान);
      जर (bytes_read == 0)
        परत -1;
      जर (bytes_read < 0)
        {
          पूर्णांक ret = -1 - त्रुटि_सं;
          error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
          परत ret;
        }
      जर (bytes_read <= n_bytes)
        n_bytes -= bytes_read;
      अन्यथा
        {
          xwrite_stdout (&buffer[n_bytes], bytes_read - n_bytes);
          थांबा;
        }
    }

  परत 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

स्थिर पूर्णांक
start_ओळी (अक्षर स्थिरांक *prettyname, पूर्णांक fd, मोजणी_t n_ओळी)
{
  जर (n_ओळी == 0)
    परत 0;

  पर्यंत (true)
    {
      अक्षर buffer[बफ_मान];
      ssize_t bytes_read = पठ (fd, buffer, बफ_मान);
      जर (bytes_read == 0) /* EOF */
        परत -1;
      जर (bytes_read < 0) /* error */
        {
          पूर्णांक ret = -1 - त्रुटि_सं;
          error (0, त्रुटि_सं, _("error reading %s"), quoteaf (prettyname));
          परत ret;
        }

      अक्षर *buffer_end = buffer + bytes_read;
      अक्षर *p = buffer;
      पर्यंत ((p = स_प्रथम (p, ओळ_end, buffer_end - p)))
        {
          ++p;
          जर (--n_ओळी == 0)
            {
              जर (p < buffer_end)
                xwrite_stdout (p, buffer_end - p);
              परत 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
स्थिर bool
fremote (पूर्णांक fd, संरचना File_spec स्थिरांक *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#जर HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  संरचना statfs buf;
  पूर्णांक err = fstatfs (fd, &buf);
  जर (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      जर (त्रुटि_सं != ENOSYS)
        error (0, त्रुटि_सं, _("cannot determine location of %s. "
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

  परत remote;
}

/* open/fstat F->name and handle changes.  */
स्थिर शून्य
recheck (संरचना File_spec *f, bool blocking)
{
  संरचना stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  पूर्णांक prev_errnum = f->errnum;
  bool new_file;
  पूर्णांक fd = (is_stdin
            ? STDIN_FILENO
            : खोलो (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  पूर्णांक open_errno = fd < 0 ? त्रुटि_सं : 0;

  affirm (valid_file_spec (f));

  जर (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  अन्यथा जर (fd < 0 || ख_स्थिति (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : त्रुटि_सं;
      जर (fd < 0)
        {
          जर (f->tailable)
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
      अन्यथा जर (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  अन्यथा जर (!IS_TAILABLE_FILE_TYPE (new_stats.st_mode))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_mode == Follow_name);
      जर (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  अन्यथा जर ((f->remote = fremote (fd, f)) && ! disable_inotify)
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
  जर (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  अन्यथा जर (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  अन्यथा जर (f->fd < 0)
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
  अन्यथा जर (!SAME_INODE (*f, new_stats))
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
  जर (new_file)
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
any_live_files (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  जर (reopen_inaccessible_files && follow_mode == Follow_name)
    परत true;

  साठी (पूर्णांक i = 0; i < n_files; i++)
    {
      जर (0 <= f[i].fd)
        परत true;
      अन्यथा
        {
          जर (! f[i].ignore && reopen_inaccessible_files)
            परत true;
        }
    }

  परत false;
}

/* Return true if any writers exist, even as zombies.  */

स्थिर bool
some_writers_exist (शून्य)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  साठी (idx_t i = 0; i < nbpids; )
    {
      जर (समाप्त (pids[i], 0) < 0 && त्रुटि_सं == ESRCH)
        {
          nbpids--;
          स_हटाओ (&pids[i], &pids[i + 1], (nbpids - i) * आकार pids[i]);
        }
      अन्यथा
        i++;
    }


  परत 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

स्थिर शून्य
tail_forever (संरचना File_spec *f, पूर्णांक n_files, द्विगुण sleep_interval)
{
  पूर्णांक last = n_files - 1;

  स्थिर bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  जर (ख_साफ (मानक_निकास) < 0)
    write_error ();

  पर्यंत (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_mode == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].mode));

      जर (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_आदान = false;

      साठी (पूर्णांक i = 0; i < n_files; i++)
        {
          संरचना stat stats;

          जर (f[i].ignore)
            सुरूठेवा;

          पूर्णांक fd = f[i].fd;
          जर (fd < 0)
            {
              recheck (&f[i], blocking);
              सुरूठेवा;
            }

          अक्षर स्थिरांक *prettyname = f[i].prettyname;
          mode_t mode = f[i].mode;

          जर (f[i].blocking != blocking)
            {
              पूर्णांक old_flags = fcntl (fd, F_GETFL);
              पूर्णांक new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              जर (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  जर (S_ISREG (f[i].mode) && त्रुटि_सं == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  अन्यथा
                    error (निकास_त्रुटि, त्रुटि_सं,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              अन्यथा
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          जर (!f[i].blocking)
            {
              जर (ख_स्थिति (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = त्रुटि_सं;
                  error (0, त्रुटि_सं, "%s", quotef (prettyname));
                  बंद (fd); /* ignore failure */
                  सुरूठेवा;
                }

              जर (f[i].mode == stats.st_mode
                  && (! S_ISREG (stats.st_mode)
                      || f[i].read_pos == stats.st_size)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  जर ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_mode == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  जर (fd != f[i].fd || S_ISREG (stats.st_mode) || 1 < n_files)
                    सुरूठेवा;
                  अन्यथा
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].mode = stats.st_mode;

              /* reset counter */
              जर (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              जर (S_ISREG (mode) && stats.st_size < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, शुरू_से, prettyname);
                }

              जर (i != last)
                {
                  जर (print_headers)
                    write_header (prettyname);
                  last = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          मोजणी_t bytes_to_read;
          जर (f[i].blocking)
            bytes_to_read = COPY_A_BUFFER;
          अन्यथा जर (S_ISREG (mode) && f[i].remote)
            bytes_to_read = stats.st_size - f[i].read_pos;
          अन्यथा
            bytes_to_read = COPY_TO_EOF;

          मोजणी_t nr = dump_remainder (false, prettyname, fd, bytes_to_read);
          जर (0 < nr)
            {
              जर (S_ISREG (mode))
                f[i].read_pos += nr;
              जर (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_आदान = true;
            }
        }

      जर (! any_live_files (f, n_files))
        {
          error (निकास_त्रुटि, 0, _("no files remaining"));
          थांबा;
        }

      जर ((!any_आदान || blocking) && ख_साफ (मानक_निकास) < 0)
        write_error ();

      check_output_alive ();

      जर (!any_आदान)
        {
          जर (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              जर (!nbpids)
                थांबा;
              जर (!some_writers_exist ())
                सुरूठेवा;
            }

          जर (xnanosleep (sleep_interval))
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot read realtime clock"));
        }
    }
}

#जर HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

स्थिर bool
any_remote_file (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  साठी (पूर्णांक i = 0; i < n_files; i++)
    जर (0 <= f[i].fd && f[i].remote)
      परत true;
  परत false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

स्थिर bool
any_non_remote_file (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  साठी (पूर्णांक i = 0; i < n_files; i++)
    जर (0 <= f[i].fd && ! f[i].remote)
      परत true;
  परत false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

स्थिर bool
any_symlinks (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  साठी (पूर्णांक i = 0; i < n_files; i++)
    जर (issymlink (f[i].name) == 1)
      परत true;
  परत false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

स्थिर bool
any_non_regular_fifo (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  साठी (पूर्णांक i = 0; i < n_files; i++)
    जर (0 <= f[i].fd && ! S_ISREG (f[i].mode) && ! S_ISFIFO (f[i].mode))
      परत true;
  परत false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

स्थिर bool
tailable_stdin (स्थिरांक संरचना File_spec *f, पूर्णांक n_files)
{
  साठी (पूर्णांक i = 0; i < n_files; i++)
    जर (!f[i].ignore && streq (f[i].name, "-"))
      परत true;
  परत false;
}

स्थिर माप_प्रकार
wd_hasher (स्थिरांक शून्य *entry, माप_प्रकार tabsize)
{
  स्थिरांक संरचना File_spec *spec = entry;
  परत spec->wd % tabsize;
}

स्थिर bool
wd_comparator (स्थिरांक शून्य *e1, स्थिरांक शून्य *e2)
{
  स्थिरांक संरचना File_spec *spec1 = e1;
  स्थिरांक संरचना File_spec *spec2 = e2;
  परत spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
स्थिर माप_प्रकार
parent_name_hasher (स्थिरांक शून्य *entry, माप_प्रकार tabsize)
{
  स्थिरांक संरचना File_spec *spec = entry;
  अक्षर स्थिरांक *name = spec->name + spec->basename_start;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  परत (hash_string (name, tabsize) + (माप_प्रकार) spec->parent_wd) % tabsize;
}

स्थिर bool
parent_name_comparator (स्थिरांक शून्य *e1, स्थिरांक शून्य *e2)
{
  स्थिरांक संरचना File_spec *a = e1;
  स्थिरांक संरचना File_spec *b = e2;
  परत (a->parent_wd == b->parent_wd
          && streq (a->name + a->basename_start,
                    b->name + b->basename_start));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
स्थिर शून्य
check_fspec (संरचना File_spec *fspec, संरचना File_spec **prev_fspec)
{
  जर (fspec->fd < 0)
    परत;

  संरचना stat stats;
  जर (ख_स्थिति (fspec->fd, &stats) < 0)
    {
      fspec->errnum = त्रुटि_सं;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      परत;
    }

  अक्षर स्थिरांक *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  जर (S_ISREG (fspec->mode) && stats.st_size < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, शुरू_से, prettyname);
    }
  अन्यथा जर (S_ISREG (fspec->mode) && stats.st_size == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    परत;

  bool want_header = print_headers && (fspec != *prev_fspec);

  मोजणी_t nr = dump_remainder (want_header, prettyname, fspec->fd, COPY_TO_EOF);
  जर (0 < nr)
    {
      जर (S_ISREG (fspec->mode))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      जर (ख_साफ (मानक_निकास) < 0)
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
tail_forever_inotify (पूर्णांक wd, संरचना File_spec *f, पूर्णांक n_files,
                      द्विगुण sleep_interval, Hash_table **wd_to_namep,
                      Hash_table **parent_by_namep)
{
# जर TAIL_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  पूर्णांक max_realloc = 3;

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
  अक्षर *evbuf;
  idx_t evbuf_off = 0;

  *parent_by_namep = रिक्त;

  wd_to_name = hash_initialize (n_files, रिक्त, wd_hasher, wd_comparator,
                                रिक्त);
  जर (! wd_to_name)
    xalloc_die ();
  *wd_to_namep = wd_to_name;

  जर (follow_mode == Follow_name)
    {
      parent_by_name = hash_initialize (n_files, रिक्त, parent_name_hasher,
                                        parent_name_comparator, रिक्त);
      जर (! parent_by_name)
        xalloc_die ();
      *parent_by_namep = parent_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_mask = IN_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  जर (follow_mode == Follow_name)
    inotify_wd_mask |= (IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  साठी (पूर्णांक i = 0; i < n_files; i++)
    {
      जर (!f[i].ignore)
        {
          idx_t fnlen = म_माप (f[i].name);
          जर (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          जर (follow_mode == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              अक्षर prev = f[i].name[dirlen];
              f[i].basename_start = last_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].parent_wd = inotify_add_watch (wd, dirlen ? f[i].name : ".",
                                                  (IN_CREATE | IN_DELETE
                                                   | IN_MOVED_TO | IN_ATTRIB
                                                   | IN_DELETE_SELF));

              f[i].name[dirlen] = prev;

              जर (f[i].parent_wd < 0)
                {
                  जर (त्रुटि_सं != ENOSPC) /* suppress confusing error.  */
                    error (0, त्रुटि_सं, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  अन्यथा
                    error (0, 0, _("inotify resources exhausted"));
                  found_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  थांबा;
                }

              जर (hash_घाला (parent_by_name, &(f[i])) == रिक्त)
                xalloc_die ();
            }

          f[i].wd = inotify_add_watch (wd, f[i].name, inotify_wd_mask);

          जर (f[i].wd < 0)
            {
              जर (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              जर (त्रुटि_सं == ENOSPC || त्रुटि_सं == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  थांबा;
                }
              अन्यथा जर (त्रुटि_सं != f[i].errnum)
                error (0, त्रुटि_सं, _("cannot watch %s"), quoteaf (f[i].name));
              सुरूठेवा;
            }

          जर (hash_घाला (wd_to_name, &(f[i])) == रिक्त)
            xalloc_die ();

          found_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  जर (no_inotify_resources || found_unwatchable_dir
      || (follow_mode == Follow_descriptor && tailed_but_unwatchable))
    परत;
  जर (follow_mode == Follow_descriptor && !found_watchable_file)
    निकास (निकास_त्रुटि);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  साठी (पूर्णांक i = 0; i < n_files; i++)
    {
      जर (! f[i].ignore)
        {
          /* check for new files.  */
          जर (follow_mode == Follow_name)
            recheck (&(f[i]), false);
          अन्यथा जर (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              संरचना stat stats;

              जर (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, त्रुटि_सं, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  परत;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  जर (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += आकार (संरचना inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  सूचक_भेद_प्रकार len = 0;
  पर्यंत (true)
    {
      संरचना File_spec *fspec;
      संरचना inotify_event *ev;
      शून्य *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      जर (follow_mode == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_to_name) == 0)
        error (निकास_त्रुटि, 0, _("no files remaining"));

      जर (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          पूर्णांक file_change;
          संरचना pollfd pfd[2];
          करा
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              पूर्णांक delay = -1;

              जर (pids)
                {
                  जर (!nbpids)
                    निकास (निकास_सफल);

                  जर (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  अन्यथा जर (sleep_interval < पूर्णांक_उच्च / 1000 - 1)
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
          पर्यंत (file_change == 0 || (file_change < 0 && त्रुटि_सं == EINTR));

          जर (file_change < 0)
            error (निकास_त्रुटि, त्रुटि_सं,
                   _("error waiting for inotify and output events"));
          जर (pfd[1].revents)
            die_pipe ();

          len = पठ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          जर ((len == 0 || (len < 0 && त्रुटि_सं == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              सुरूठेवा;
            }

          जर (len <= 0)
            error (निकास_त्रुटि, त्रुटि_सं, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += आकार (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      जर ((ev->mask & IN_DELETE_SELF) && ! ev->len)
        {
          साठी (पूर्णांक i = 0; i < n_files; i++)
            {
              जर (ev->wd == f[i].parent_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  परत;
                }
            }
        }

      जर (ev->len) /* event on ev->name in watched directory.  */
        {
          संरचना File_spec किल्ली;
          जर (! parent_by_name)
            सुरूठेवा;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          किल्ली.parent_wd = ev->wd;
          किल्ली.name = ev->name;
          किल्ली.basename_start = 0;
          fspec = hash_lookup (parent_by_name, &किल्ली);

          /* It is not a watched file.  */
          जर (! fspec)
            सुरूठेवा;

          पूर्णांक new_wd = -1;
          bool deleting = !! (ev->mask & IN_DELETE);

          जर (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_add_watch (wd, fspec->name, inotify_wd_mask);
            }

          जर (! deleting && new_wd < 0)
            {
              जर (त्रुटि_सं == ENOSPC || त्रुटि_सं == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  परत; /* revert to polling.  */
                }
              अन्यथा
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, त्रुटि_सं, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          जर (new_watch)
            {
              जर (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_to_name, fspec);
                }

              fspec->wd = new_wd;

              जर (new_wd < 0)
                सुरूठेवा;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              संरचना File_spec *prev = hash_remove (wd_to_name, fspec);
              जर (prev && prev != fspec)
                {
                  जर (follow_mode == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              जर (hash_घाला (wd_to_name, fspec) == रिक्त)
                xalloc_die ();
            }

          जर (follow_mode == Follow_name)
            recheck (fspec, false);
        }
      अन्यथा
        {
          संरचना File_spec किल्ली;
          किल्ली.wd = ev->wd;
          fspec = hash_lookup (wd_to_name, &किल्ली);
        }

      जर (! fspec)
        सुरूठेवा;

      जर (ev->mask & (IN_ATTRIB | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          जर (ev->mask & IN_DELETE_SELF
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

          सुरूठेवा;
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
tail_bytes (अक्षर स्थिरांक *prettyname, पूर्णांक fd, संरचना stat स्थिरांक *st,
            मोजणी_t n_bytes)
{
  off_t current_pos
    = (presume_आदान_pipe
       ? -1
       : lseek (fd, from_start ? MIN (n_bytes, OFF_T_MAX) : 0, प्रस्तुत_से));

  जर (from_start)
    {
      जर (current_pos < 0)
        {
          पूर्णांक t = start_bytes (prettyname, fd, n_bytes);
          जर (t < 0)
            परत t;
        }
      n_bytes = COPY_TO_EOF;
    }
  अन्यथा
    {
      off_t initial_pos = current_pos;
      off_t end_pos = -1;

      जर (0 <= current_pos)
        {
          जर (usable_st_size (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_size = STP_BLKSIZE (st);
              जर (smallish_size < st->st_size)
                end_pos = st->st_size;
            }
          अन्यथा
            {
              off_t minus_n;
              जर (ckd_sub (&minus_n, 0, n_bytes))
                end_pos = 0;
              अन्यथा
                {
                  off_t e_n_pos = lseek (fd, minus_n, अंत_से);
                  जर (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      end_pos = e_n_pos + n_bytes;
                    }
                  अन्यथा
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, अंत_से);
                      जर (0 <= e_pos)
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
      जर (pos != current_pos)
        current_pos = xlseek (fd, pos, शुरू_से, prettyname);

      जर (end_pos < 0)
        परत pipe_bytes (prettyname, fd, n_bytes, current_pos);
    }

  मोजणी_t nr = dump_remainder (false, prettyname, fd, n_bytes);
  परत current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
tail_ओळी (अक्षर स्थिरांक *prettyname, पूर्णांक fd, संरचना stat स्थिरांक *st,
            मोजणी_t n_ओळी)
{
  जर (from_start)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      जर (OFF_T_MAX <= n_ओळी)
        {
          off_t e = lseek (fd, 0, अंत_से);
          जर (0 <= e)
            परत e;
        }

      पूर्णांक t = start_ओळी (prettyname, fd, n_ओळी);
      जर (t < 0)
        परत t;
      dump_remainder (false, prettyname, fd, COPY_TO_EOF);
      परत -1;
    }
  अन्यथा
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t start_pos = (!presume_आदान_pipe && S_ISREG (st->st_mode)
                         ? lseek (fd, 0, प्रस्तुत_से)
                         : -1);
      off_t end_pos = start_pos < 0 ? -1 : lseek (fd, 0, अंत_से);
      परत (end_pos < 0
              ? pipe_ओळी (prettyname, fd, n_ओळी)
              : start_pos < end_pos
              ? file_ओळी (prettyname, fd, st, n_ओळी, start_pos, end_pos)
              : start_pos != end_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, start_pos, शुरू_से, prettyname))
              : start_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

स्थिर off_t
tail (अक्षर स्थिरांक *filename, पूर्णांक fd, संरचना stat स्थिरांक *st, मोजणी_t n_units)
{
  परत (मोजणी_ओळी ? tail_ओळी : tail_bytes) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

स्थिर bool
tail_file (संरचना File_spec *f, मोजणी_t n_files, मोजणी_t n_units)
{
  पूर्णांक fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  जर (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_द्विमान_mode (STDIN_FILENO, O_द्विमान);
    }
  अन्यथा
    fd = खोलो (f->name, O_RDONLY | O_द्विमान | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  जर (fd < 0)
    {
      जर (forever)
        {
          f->fd = -1;
          f->errnum = त्रुटि_सं;
          f->ignore = ! reopen_inaccessible_files;
          f->st_dev = 0;
          f->st_ino = 0;
        }
      error (0, त्रुटि_सं, _("cannot open %s for reading"),
             quoteaf (f->prettyname));
      ok = false;
    }
  अन्यथा
    {
      जर (print_headers)
        write_header (f->prettyname);

      off_t read_pos;
      संरचना stat stats;
      ok = 0 <= ख_स्थिति (fd, &stats);
      जर (!ok)
        {
          f->errnum = त्रुटि_सं;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      अन्यथा
        {
          read_pos = tail (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          जर (IS_TAILABLE_FILE_TYPE (stats.st_mode))
            f->tailable = true;
          अन्यथा जर (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      जर (forever)
        {
          जर (!ok)
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
          जर (!is_stdin && बंद (fd) < 0)
            {
              error (0, त्रुटि_सं, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  परत ok;
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
parse_obsolete_option (पूर्णांक argc, अक्षर * स्थिरांक *argv, मोजणी_t *n_units)
{
  bool t_from_start;
  bool t_मोजणी_ओळी = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  जर (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    परत false;

  पूर्णांक posix_ver = posix2_version ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  अक्षर स्थिरांक *p = argv[1];

  निवड (*p++)
    {
    पूर्वनिर्धारित:
      परत false;

    स्थिती '+':
      /* Leading "+" is a file name in the standard form.  */
      जर (!traditional_usage)
        परत false;

      t_from_start = true;
      थांबा;

    स्थिती '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      जर (!obsolete_usage && !p[p[0] == 'c'])
        परत false;

      t_from_start = false;
      थांबा;
    }

  मोजणी_t n;
  जर (!c_isdigit (*p))
    n = DEFAULT_N_ओळी;
  अन्यथा
    साठी (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_add (&n, n, *p - '0') ? मोजणी_MAX : n;

  निवड (*p)
    {
    स्थिती 'b': n = ckd_mul (&n, n, 512) ? मोजणी_MAX : n; FALLTHROUGH;
    स्थिती 'c': t_मोजणी_ओळी = false; FALLTHROUGH;
    स्थिती 'l': p++; थांबा;
    }

  जर (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  जर (*p)
    परत false;

  *n_units = n;

  /* Set globals.  */
  from_start = t_from_start;
  मोजणी_ओळी = t_मोजणी_ओळी;
  forever = t_forever;

  परत true;
}

स्थिर शून्य
parse_options (पूर्णांक argc, अक्षर **argv,
               मोजणी_t *n_units, गणना header_mode *header_mode,
               द्विगुण *sleep_interval)
{
  पूर्णांक c;

  पर्यंत ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती 'F':
          forever = true;
          follow_mode = Follow_name;
          reopen_inaccessible_files = true;
          थांबा;

        स्थिती 'c':
        स्थिती 'n':
          मोजणी_ओळी = (c == 'n');
          जर (*optarg == '+')
            from_start = true;
          अन्यथा जर (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, मोजणी_MAX, "bkKmMGTPEZYRQ0",
                                 (मोजणी_ओळी
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          थांबा;

        स्थिती 'f':
        स्थिती LONG_FOLLOW_OPTION:
          forever = true;
          जर (optarg == रिक्त)
            follow_mode = DEFAULT_FOLLOW_MODE;
          अन्यथा
            follow_mode = XARGMATCH ("--follow", optarg,
                                     follow_mode_string, follow_mode_map);
          थांबा;

        स्थिती RETRY_OPTION:
          reopen_inaccessible_files = true;
          थांबा;

        स्थिती MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, मोजणी_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          थांबा;

        स्थिती DEBUG_PROGRAM_OPTION:
          debug = true;
          थांबा;

        स्थिती DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          थांबा;

        स्थिती PID_OPTION:
          जर (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (पूर्णांक_उच्च, IDX_MAX), आकार *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          थांबा;

        स्थिती PRESUME_आदान_PIPE_OPTION:
          presume_आदान_pipe = true;
          थांबा;

        स्थिती 'q':
          *header_mode = never;
          थांबा;

        स्थिती 's':
          {
            अक्षर *ep;
            त्रुटि_सं = 0;
            द्विगुण s = cl_strtod (optarg, &ep);
            जर (optarg == ep || *ep || ! (0 <= s))
              error (निकास_त्रुटि, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, त्रुटि_सं);
          }
          थांबा;

        स्थिती 'v':
          *header_mode = always;
          थांबा;

        स्थिती 'z':
          ओळ_end = '\0';
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्थिती '0': स्थिती '1': स्थिती '2': स्थिती '3': स्थिती '4':
        स्थिती '5': स्थिती '6': स्थिती '7': स्थिती '8': स्थिती '9':
          error (निकास_त्रुटि, 0, _("option used in invalid context -- %c"), c);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (reopen_inaccessible_files)
    {
      जर (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      अन्यथा जर (follow_mode == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  जर (pids && (!forever || (समाप्त (pids[0], 0) < 0 && त्रुटि_सं == ENOSYS)))
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
ignore_fifo_and_pipe (संरचना File_spec *f, पूर्णांक n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  साठी (पूर्णांक i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].mode)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      जर (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      अन्यथा
        some_viable = true;
    }

  परत some_viable;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  गणना header_mode header_mode = multiple_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  मोजणी_t n_units = DEFAULT_N_ओळी;
  पूर्णांक n_files;
  अक्षर **file;
  संरचना File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  द्विगुण sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  {
    पूर्णांक p = getpagesize ();
    जर (IDX_MAX < p)
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

  जर (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  अन्यथा
    {
      स्थिर अक्षर *dummy_stdin = (अक्षर *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool found_hyphen = false;

    साठी (पूर्णांक i = 0; i < n_files; i++)
      जर (streq (file[i], "-"))
        found_hyphen = true;

    /* When following by name, there must be a name.  */
    जर (found_hyphen && follow_mode == Follow_name)
      error (निकास_त्रुटि, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    जर (forever && found_hyphen)
      {
        संरचना stat in_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_mode == Follow_descriptor
                          && n_files == 1 && ! ख_स्थिति (STDIN_FILENO, &in_stat)
                          && ! S_ISREG (in_stat.st_mode));

        जर (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  जर (! forever && n_units == (from_start ? मोजणी_MAX : 0))
    परत निकास_सफल;

  जर (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, आकार *F);
  साठी (पूर्णांक i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  जर (header_mode == always
      || (header_mode == multiple_files && n_files > 1))
    print_headers = true;

  xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  साठी (पूर्णांक i = 0; i < n_files; i++)
    ok &= tail_file (&F[i], n_files, n_units);

  जर (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      संरचना stat out_stat;
      जर (ख_स्थिति (STDOUT_FILENO, &out_stat) < 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("standard output"));
      monitor_output = (S_ISFIFO (out_stat.st_mode)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#जर HAVE_INOTIFY
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
      जर (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_mode == Follow_descriptor)))
        disable_inotify = true;

      जर (!disable_inotify)
        {
          पूर्णांक wd = inotify_init ();
          जर (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              जर (ख_साफ (मानक_निकास) < 0)
                write_error ();

              Hash_table *ht;
              Hash_table *parent_ht = रिक्त;
              tail_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &parent_ht);
              hash_free (ht);
              जर (parent_ht)
                hash_free (parent_ht);
              बंद (wd);
              त्रुटि_सं = 0;
            }
          error (0, त्रुटि_सं, _("inotify cannot be used, reverting to polling"));
        }
#endif
      disable_inotify = true;
      tail_forever (F, n_files, sleep_interval);
    }

  जर (have_read_stdin && बंद (STDIN_FILENO) < 0)
    error (निकास_त्रुटि, त्रुटि_सं, "-");
  main_exit (ok ? निकास_सफल : निकास_त्रुटि);
}
