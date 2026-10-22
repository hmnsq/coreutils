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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cl-strtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dtimespec-bound.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fcntl--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "iopoll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "isapipe.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "issymlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixver.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-size.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xnanosleep.h"

#ਜੇ HAVE_INOTIFY
# include "hash.h"
# include <poll.h>
# include <sys/inotify.h>
#ਜੇ_ਅੰਤ

/* Linux can optimize the handling of local files.  */
#ਜੇ defined __linux__ || defined __ANDROID__
# include "fs.h"
# include "fs-is-local.h"
# ਜੇ HAVE_SYS_STATFS_H
#  include <sys/statfs.h>
# elif HAVE_SYS_VFS_H
#  include <sys/vfs.h>
# endif
#ਜੇ_ਅੰਤ

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "tail"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Ian Lance Taylor"), \
  proper_name ("Jim Meyering")

/* Number of items to tail.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_N_ਸਤਰਾਂ 10

/* Special values for dump_remainder's N_BYTES parameter.  */
ਗਿਣਤੀ { COPY_ਮੰਜ਼ਿਲ_EOF = -1, COPY_A_ਬਫਰ = -2 };

/* The type of a large counter.  Although it is always nonnegative,
   it is signed to help signed overflow checking catch any bugs.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ intmax_t ਗਿਣਤੀ_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_MAX INTMAX_MAX

/* FIXME: make Follow_name the default?  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_FOLLOW_ਬਹੁਲਕ Follow_descriptor

ਗਿਣਤੀ Follow_ਬਹੁਲਕ
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IS_TAILABLE_FILE_TYPE(ਬਹੁਲਕ_3) \
  (S_ISREG (ਬਹੁਲਕ_3) || S_ISFIFO (ਬਹੁਲਕ_3) || S_ISSOCK (ਬਹੁਲਕ_3) || S_ISCHR (ਬਹੁਲਕ_3))

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ follow_ਬਹੁਲਕ_string[] =
{
  "descriptor", "name", ਨਿਰਾ
};

ਸਥਿਰ ਗਿਣਤੀ Follow_ਬਹੁਲਕ ਅਚਲ follow_ਬਹੁਲਕ_map[] =
{
  Follow_descriptor, Follow_name,
};

ਬਣਤਰ File_spec
{
  /* The actual file name, or "-" for stdin.  */
  ਅੱਖਰ *name;

  /* The prettified name; the same as NAME unless NAME is "-".  */
  ਅੱਖਰ ਅਚਲ *prettyname;

  /* Attributes of the file the last time we checked.  */
  ਬਣਤਰ timespec mtime;
  dev_t st_dev;
  ino_t st_ino;
  mode_t ਬਹੁਲਕ_2;

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
  ਪੂਰਨ fd;

  /* 0 if the file's last op succeeded, otherwise the op's errno if
     applicable, otherwise -1.  */
  ਪੂਰਨ errnum;

  /* 1 if O_NONBLOCK is clear, 0 if set, -1 if not known.  */
  ਪੂਰਨ blocking;

#ਜੇ HAVE_INOTIFY
  /* The watch descriptor used by inotify.  */
  ਪੂਰਨ wd;

  /* The parent directory watch descriptor.  It is used only
   * when Follow_name is used.  */
  ਪੂਰਨ ਮਾਪਾ_wd;

  /* Offset in NAME of the basename part.  */
  idx_t basename_ਸ਼ੁਰੂ;
#ਜੇ_ਅੰਤ

  /* See description of DEFAULT_MAX_N_... below.  */
  ਗਿਣਤੀ_t n_unchanged_stats;
};

/* Keep trying to open a file even if it is inaccessible when tail starts
   or if it becomes inaccessible later -- useful only with -f.  */
ਸਥਿਰ bool reopen_inaccessible_files;

/* If true, interpret the numeric argument as the number of lines.
   Otherwise, interpret it as the number of bytes.  */
ਸਥਿਰ bool ਗਿਣਤੀ_ਸਤਰਾਂ = true;

/* Whether we follow the name of each file or the file descriptor
   that is initially associated with each name.  */
ਸਥਿਰ ਗਿਣਤੀ Follow_ਬਹੁਲਕ follow_ਬਹੁਲਕ = Follow_descriptor;

/* If true, read from the ends of all specified files until killed.  */
ਸਥਿਰ bool forever;

/* If true, monitor output so we exit if pipe reader terminates.  */
ਸਥਿਰ bool monitor_output;

/* If true, count from start of file instead of end.  */
ਸਥਿਰ bool from_ਸ਼ੁਰੂ;

/* If true, print filename headers.  */
ਸਥਿਰ bool print_ਸਿਰਲੇਖਾਂ;

/* Character to split lines by. */
ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਸਿਰਾ = '\n';

/* When to print the filename banners.  */
ਗਿਣਤੀ ਸਿਰਲੇਖ_ਬਹੁਲਕ
{
  ਗੁਣਜ_files, always, never
};

/* When tailing a file by name, if there have been this many consecutive
   iterations for which the file has not changed, then open/fstat
   the file to determine if that file name is still associated with the
   same device/inode-number pair as before.  This option is meaningful only
   when following by name.  --max-unchanged-stats=N  */
ਗਿਣਤੀ { DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS = 5 };
ਸਥਿਰ ਗਿਣਤੀ_t max_n_unchanged_stats_between_opens =
  DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS;

/* The process IDs of the processes to watch (those writing the followed
   files, or perhaps other processes the user cares about).  */
ਸਥਿਰ ਪੂਰਨ nbpids = 0;
ਸਥਿਰ pid_t * pids = ਨਿਰਾ;
ਸਥਿਰ idx_t pids_alloc;

/* Used to determine the buffer size when scanning backwards in a file.  */
ਸਥਿਰ idx_t ਪੰਨਾ_ਮਾਪ;

/* True if we have ever read standard input.  */
ਸਥਿਰ bool have_read_stdin;

/* If nonzero, skip the is-regular-file test used to determine whether
   to use the lseek optimization.  Instead, use the more general (and
   more expensive) code unconditionally. Intended solely for testing.  */
ਸਥਿਰ bool presume_ਨਿਵੇਸ਼_pipe;

/* If nonzero then don't use inotify even if available.  */
ਸਥਿਰ bool disable_inotify;

/* Annotate the output with extra info to aid the user.  */
ਸਥਿਰ bool debug;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  RETRY_OPTION = CHAR_MAX + 1,
  MAX_UNCHANGED_STATS_OPTION,
  PID_OPTION,
  PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION,
  LONG_FOLLOW_OPTION,
  DISABLE_INOTIFY_OPTION,
  DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"bytes", required_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"debug", no_ਦਲੀਲ, ਨਿਰਾ, DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION},
  {"follow", optional_ਦਲੀਲ, ਨਿਰਾ, LONG_FOLLOW_OPTION},
  {"lines", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"max-unchanged-stats", required_ਦਲੀਲ, ਨਿਰਾ,
   MAX_UNCHANGED_STATS_OPTION},
  {"-disable-inotify", no_ਦਲੀਲ, ਨਿਰਾ,
   DISABLE_INOTIFY_OPTION}, /* do not document */
  {"pid", required_ਦਲੀਲ, ਨਿਰਾ, PID_OPTION},
  {"-presume-input-pipe", no_ਦਲੀਲ, ਨਿਰਾ,
   PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION}, /* do not document */
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"retry", no_ਦਲੀਲ, ਨਿਰਾ, RETRY_OPTION},
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"sleep-interval", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (_("\
Print the ਆਖਰੀ %d ਸਤਰਾਂ ਦਾ each FILE ਮੰਜ਼ਿਲ standard output.\n\
With ਹੋਰ than one FILE, precede each with a ਸਿਰਲੇਖ giving the file name.\n\
"), DEFAULT_N_ਸਤਰਾਂ);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

     oputs (_("\
  -c, --ਬਾਈਟਾਂ=[+]ਅੰਕ_2\n\
         output the ਆਖਰੀ ਅੰਕ_2 ਬਾਈਟਾਂ;\n\
         or use -c +ਅੰਕ_2 ਮੰਜ਼ਿਲ output starting with byte ਅੰਕ_2 ਦਾ each file\n\
"));
     oputs (_("\
      --debug\n\
         indicate which --follow implementation is ਵਰਤਿਆ\n\
"));
     oputs (_("\
  -f, --follow[={name|descriptor}]\n\
         output appended ਅੰਕੜੇ as the file grows;\n\
         an absent option ਦਲੀਲ means 'descriptor'\n\
"));
     oputs (_("\
  -F\n\
         same as --follow=name --retry\n\
"));
     oprintf (_("\
  -n, --ਸਤਰਾਂ=[+]ਅੰਕ_2\n\
         output the ਆਖਰੀ ਅੰਕ_2 ਸਤਰਾਂ, instead ਦਾ the ਆਖਰੀ %d;\n\
         or use -n +ਅੰਕ_2 ਮੰਜ਼ਿਲ skip ਅੰਕ_2-1 ਸਤਰਾਂ at the ਸ਼ੁਰੂ\n\
"), DEFAULT_N_ਸਤਰਾਂ);
     oprintf (_("\
      --max-unchanged-stats=N\n\
         with --follow=name, reopen a FILE which has not\n\
         ਬਦਲਿਆ ਮਾਪ after N (ਮੂਲ %d) iterations\n\
         ਮੰਜ਼ਿਲ see ਜੇ it has been unlinked or renamed\n\
         (this is the usual ਹਾਲਤ ਦਾ rotated ਲਘੂਗਣਕ files);\n\
         with inotify, this option is rarely useful\n\
"), DEFAULT_MAX_N_UNCHANGED_STATS_BETWEEN_OPENS);
     oputs (_("\
      --pid=PID\n\
         with -f, ਬਾਹਰ_ਨਿਕਲੋ after PID no longer exists;\n\
         can be repeated ਮੰਜ਼ਿਲ watch ਗੁਣਜ processes\n\
"));
     oputs (_("\
  -q, --quiet, --silent\n\
         never output ਸਿਰਲੇਖਾਂ giving file ਨਾਂ\n\
"));
     oputs (_("\
      --retry\n\
         keep trying ਮੰਜ਼ਿਲ open a file ਜੇ it is inaccessible\n\
"));
     oputs (_("\
  -s, --sleep-interval=N\n\
         with -f, sleep ਲਈ approximately N seconds\n\
         (ਮੂਲ 1.0) between iterations;\n\
         with inotify and --pid=P,\n\
         check process P at ਘੱਟ_ਤੋਂ_ਘੱਟ once every N seconds\n\
"));
     oputs (_("\
  -v, --verbose\n\
         always output ਸਿਰਲੇਖਾਂ giving file ਨਾਂ\n\
"));
     oputs (_("\
  -z, --zero-terminated\n\
         ਸਤਰ delimiter is NUL, not newline\n\
"));
     oputs (HELP_OPTION_DESCRIPTION);
     oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
     fputs (_("\
\n\
ਅੰਕ_2 may have a multiplier suffix:\n\
b 512, kB 1000, K 1024, MB 1000*1000, M 1024*1024,\n\
GB 1000*1000*1000, G 1024*1024*1024, and so on ਲਈ T, P, E, Z, Y, R, Q.\n\
ਦੁਆਧਾਰੀ_2 prefixes can be ਵਰਤਿਆ, too: KiB=K, MiB=M, and so on.\n\
\n\
"), stdout);
     fputs (_("\
With --follow (-f), ਪੂਛ defaults ਮੰਜ਼ਿਲ following the file descriptor, which\n\
means that even ਜੇ a ਪੂਛ'ed file is renamed, ਪੂਛ will ਜਾਰੀ ਮੰਜ਼ਿਲ track\n\
its ਸਿਰਾ.  This ਮੂਲ behavior is not desirable when you really want ਮੰਜ਼ਿਲ\n\
track the actual name ਦਾ the file, not the file descriptor (e.g., ਲਘੂਗਣਕ\n\
rotation).  Use --follow=name ਵਿੱਚ that ਹਾਲਤ.  That causes ਪੂਛ ਮੰਜ਼ਿਲ track the\n\
named file ਵਿੱਚ a way that accommodates renaming, removal and creation.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Ensure exit, either with SIGPIPE or EXIT_FAILURE status.  */
ਸਥਿਰ ਖਾਲੀ
die_pipe (ਖਾਲੀ)
{
  ਉਠਾਓ (SIGPIPE);
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

/* If the output has gone away, then terminate
   as we would if we had written to this output.  */
ਸਥਿਰ ਖਾਲੀ
check_output_alive (ਖਾਲੀ)
{
  ਜੇ (! monitor_output)
    ਵਾਪਸ;

  ਜੇ (iopoll (-1, STDOUT_FILENO, false) == IOPOLL_BROKEN_OUTPUT)
    die_pipe ();
}

MAYBE_ਅਣਵਰਤਿਆ ਸਥਿਰ bool
valid_file_spec (ਬਣਤਰ File_spec ਅਚਲ *f)
{
  /* Exactly one of the following subexpressions must be true. */
  ਵਾਪਸ (f->fd < 0) ^ (f->errnum == 0);
}

/* Call lseek with the specified arguments FD, OFFSET, WHENCE.
   FD corresponds to PRETTYNAME.
   Give a diagnostic and exit nonzero if lseek fails.
   Otherwise, return the resulting offset.  */

ਸਥਿਰ off_t
xlseek (ਪੂਰਨ fd, off_t ਵਿੱਥ, ਪੂਰਨ whence, ਅੱਖਰ ਅਚਲ *prettyname)
{
  off_t new_ਵਿੱਥ = lseek (fd, ਵਿੱਥ, whence);

  ਜੇ (0 <= new_ਵਿੱਥ)
    ਵਾਪਸ new_ਵਿੱਥ;

  ਸਥਿਰ ਅੱਖਰ ਅਚਲ *whence_msgid[] = {
    [SEEK_SET] = N_("%s: cannot seek to offset %jd"),
    [SEEK_CUR] = N_("%s: cannot seek to relative offset %jd"),
    [SEEK_END] = N_("%s: cannot seek to end-relative offset %jd")
  };
  intmax_t joffset = ਵਿੱਥ;
  error (EXIT_FAILURE, errno, gettext (whence_msgid[whence]),
         quotef (prettyname), joffset);
}

/* Record a file F with descriptor FD, read position READ_POS, status ST,
   and blocking status BLOCKING.  READ_POS matters only for regular files,
   and READ_POS < 0 means the position is unknown.  */

ਸਥਿਰ ਖਾਲੀ
ਰਿਕਾਰਡ_open_fd (ਬਣਤਰ File_spec *f, ਪੂਰਨ fd,
                off_t read_pos, ਬਣਤਰ stat ਅਚਲ *st,
                ਪੂਰਨ blocking)
{
  f->fd = fd;
  f->mtime = get_stat_mtime (st);
  f->st_dev = st->st_dev;
  f->st_ino = st->st_ino;
  f->ਬਹੁਲਕ_2 = st->st_ਬਹੁਲਕ;
  ਜੇ (S_ISREG (st->st_ਬਹੁਲਕ))
    f->read_pos = (read_pos < 0
                   ? xlseek (fd, 0, SEEK_CUR, f->prettyname)
                   : read_pos);
  f->blocking = blocking;
  f->n_unchanged_stats = 0;
  f->ignore = false;
}

/* Close the file with descriptor FD and spec F.  */

ਸਥਿਰ ਖਾਲੀ
close_fd (ਪੂਰਨ fd, ਬਣਤਰ File_spec ਅਚਲ *f)
{
  ਜੇ (STDIN_FILENO < fd && close (fd) < 0)
    error (0, errno, _("closing %s (fd=%d)"), quoteaf (f->prettyname), fd);
}

ਸਥਿਰ ਖਾਲੀ
write_ਸਿਰਲੇਖ (ਅੱਖਰ ਅਚਲ *prettyname)
{
  ਸਥਿਰ bool ਪਹਿਲਾ_file = true;

  ਲਿਖੋ ("%s==> %s <==\n", ਪਹਿਲਾ_file ? "" : "\n", quotef (prettyname));
  ਪਹਿਲਾ_file = false;
}

/* Write N_BYTES from BUFFER to stdout.
   Exit immediately on error with a single diagnostic.  */

ਸਥਿਰ ਖਾਲੀ
xwrite_stdout (ਅੱਖਰ ਅਚਲ *ਬਫਰ, idx_t n_ਬਾਈਟਾਂ)
{
  ਜੇ (n_ਬਾਈਟਾਂ > 0 && fwrite (ਬਫਰ, 1, n_ਬਾਈਟਾਂ, stdout) < n_ਬਾਈਟਾਂ)
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

ਸਥਿਰ ਗਿਣਤੀ_t
dump_remainder (bool want_ਸਿਰਲੇਖ, ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd,
                ਗਿਣਤੀ_t n_ਬਾਈਟਾਂ)
{
  ਗਿਣਤੀ_t n_read = 0;
  ਗਿਣਤੀ_t n_ਬਚਿਆ = n_ਬਾਈਟਾਂ;

  ਕਰੋ
    {
      ਅੱਖਰ ਬਫਰ[BUFSIZ];
      idx_t n = n_ਬਾਈਟਾਂ < 0 || BUFSIZ < n_ਬਚਿਆ ? BUFSIZ : n_ਬਚਿਆ;
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, n);
      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          ਜੇ (errno != EAGAIN)
            error (EXIT_FAILURE, errno, _("error reading %s"),
                   quoteaf (prettyname));
          ਰੋਕੋ;
        }
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        ਰੋਕੋ;
      n_read += ਬਾਈਟਾਂ_read;

      ਜੇ (want_ਸਿਰਲੇਖ)
        {
          write_ਸਿਰਲੇਖ (prettyname);
          want_ਸਿਰਲੇਖ = false;
        }
      xwrite_stdout (ਬਫਰ, ਬਾਈਟਾਂ_read);
      ਜੇ (n_ਬਾਈਟਾਂ == COPY_A_ਬਫਰ)
        ਰੋਕੋ;

      n_ਬਚਿਆ -= ਬਾਈਟਾਂ_read;
      /* n_remaining < 0 if n_bytes == COPY_TO_EOF.  */
    }
  ਜਦੋਂ (n_ਬਚਿਆ != 0);

  ਵਾਪਸ n_read;
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

ਸਥਿਰ off_t
file_ਸਤਰਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *sb,
            ਗਿਣਤੀ_t n_ਸਤਰਾਂ, off_t ਸ਼ੁਰੂ_pos, off_t ਸਿਰਾ_pos)
{
  ਅੱਖਰ *ਬਫਰ;
  idx_t bufsize = BUFSIZ;
  off_t pos = ਸਿਰਾ_pos;

  ਜੇ (n_ਸਤਰਾਂ == 0)
    ਵਾਪਸ pos;

  /* Be careful with files with sizes that are a multiple of the page size,
     as on /proc or /sys file systems these files accept seeking to within
     the file, but then return no data when read.  So use a buffer that's
     at least PAGE_SIZE to avoid seeking within such files.

     We could also indirectly use a large enough buffer through io_blksize()
     however this would be less efficient in the common case, as it would
     generally pick a larger buffer size, resulting in reading more data
     from the end of the file.  */
  affirm (S_ISREG (sb->st_ਬਹੁਲਕ));
  ਜੇ (sb->st_ਮਾਪ % ਪੰਨਾ_ਮਾਪ == 0)
    bufsize = MAX (BUFSIZ, ਪੰਨਾ_ਮਾਪ);

  ਬਫਰ = ximalloc (bufsize);

  /* Set 'bytes_read' to the size of the last, probably partial, buffer;
     0 < 'bytes_read' <= 'bufsize'.  */
  idx_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = (pos - ਸ਼ੁਰੂ_pos) % bufsize;
  ਜੇ (ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read == 0)
    ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = bufsize;
  /* Make 'pos' a multiple of 'bufsize' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency.  */
  pos = xlseek (fd, -ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read, SEEK_CUR, prettyname);
  ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read);
  ਜੇ (ਬਾਈਟਾਂ_read < 0)
    {
      pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ਜਾਓ free_ਬਫਰ;
    }

  /* Count the incomplete line on files that don't end with a newline.  */
  ਜੇ (ਬਾਈਟਾਂ_read && ਬਫਰ[ਬਾਈਟਾਂ_read - 1] != ਸਤਰ_ਸਿਰਾ)
    --n_ਸਤਰਾਂ;

  ਕਰੋ
    {
      pos += ਬਾਈਟਾਂ_read;

      /* Scan backward, counting the newlines in this bufferfull.  */

      idx_t n = ਬਾਈਟਾਂ_read;
      ਜਦੋਂ (n)
        {
          ਅੱਖਰ ਅਚਲ *nl;
          nl = memrchr (ਬਫਰ, ਸਤਰ_ਸਿਰਾ, n);
          ਜੇ (nl == ਨਿਰਾ)
            ਰੋਕੋ;
          n = nl - ਬਫਰ;
          ਜੇ (n_ਸਤਰਾਂ-- == 0)
            {
              /* If this newline isn't the last character in the buffer,
                 output the part that is after it.  */
              xwrite_stdout (nl + 1, ਬਾਈਟਾਂ_read - (n + 1));
              pos += dump_remainder (false, prettyname, fd, ਸਿਰਾ_pos - pos);
              ਜਾਓ free_ਬਫਰ;
            }
        }

      /* Not enough newlines in that bufferfull.  */
      ਜੇ (pos - ਬਾਈਟਾਂ_read == ਸ਼ੁਰੂ_pos)
        {
          /* Not enough lines in the file; print everything from
             start_pos to the end.  */
          xwrite_stdout (ਬਫਰ, ਬਾਈਟਾਂ_read);
          dump_remainder (false, prettyname, fd, ਸਿਰਾ_pos - pos);
          ਜਾਓ free_ਬਫਰ;
        }

      pos = xlseek (fd, -(bufsize + ਬਾਈਟਾਂ_read), SEEK_CUR, prettyname);
      ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, bufsize);
      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          pos = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ਜਾਓ free_ਬਫਰ;
        }
    }
  ਜਦੋਂ (ਬਾਈਟਾਂ_read > 0);

free_ਬਫਰ:
  ਮੁਕਤ (ਬਫਰ);
  ਵਾਪਸ pos;
}

/* Print the last N_LINES lines from the end of the standard input,
   open for reading as pipe FD.
   Buffer the text as a linked list of LBUFFERs, adding them as needed.
   Return -1 if successful, (-1 - errno) otherwise.  */

ਸਥਿਰ ਪੂਰਨ
pipe_ਸਤਰਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਗਿਣਤੀ_t n_ਸਤਰਾਂ)
{
  ਬਣਤਰ linebuffer
  {
    ਅੱਖਰ ਬਫਰ[BUFSIZ];
    idx_t nbytes;
    idx_t nlines;
    ਬਣਤਰ linebuffer *ਅਗਲਾ;
  };
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ linebuffer LBUFFER;
  LBUFFER *ਪਹਿਲਾ, *ਆਖਰੀ, *tmp;
  idx_t ਕੁੱਲ_ਸਤਰਾਂ = 0;	/* Total number of newlines in all buffers.  */
  ਪੂਰਨ ret = -1;
  ssize_t n_read;		/* Size in bytes of most recent read */

  ਪਹਿਲਾ = ਆਖਰੀ = xmalloc (ਆਕਾਰ (LBUFFER));
  ਪਹਿਲਾ->nbytes = ਪਹਿਲਾ->nlines = 0;
  ਪਹਿਲਾ->ਅਗਲਾ = ਨਿਰਾ;
  tmp = xmalloc (ਆਕਾਰ (LBUFFER));

  /* Input is always read into a fresh buffer.  */
  ਜਦੋਂ (true)
    {
      n_read = ਪਠ (fd, tmp->ਬਫਰ, BUFSIZ);
      ਜੇ (n_read <= 0)
        ਰੋਕੋ;
      tmp->nbytes = n_read;
      tmp->nlines = 0;
      tmp->ਅਗਲਾ = ਨਿਰਾ;

      /* Count the number of newlines just read.  */
      {
        ਅੱਖਰ ਅਚਲ *ਬਫਰ_ਸਿਰਾ = tmp->ਬਫਰ + n_read;
        ਅੱਖਰ ਅਚਲ *p = tmp->ਬਫਰ;
        ਜਦੋਂ ((p = memchr (p, ਸਤਰ_ਸਿਰਾ, ਬਫਰ_ਸਿਰਾ - p)))
          {
            ++p;
            ++tmp->nlines;
          }
      }
      ਕੁੱਲ_ਸਤਰਾਂ += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'n_read' can
         often be very small.  */
      ਜੇ (tmp->nbytes + ਆਖਰੀ->nbytes < BUFSIZ)
        {
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (&ਆਖਰੀ->ਬਫਰ[ਆਖਰੀ->nbytes], tmp->ਬਫਰ, tmp->nbytes);
          ਆਖਰੀ->nbytes += tmp->nbytes;
          ਆਖਰੀ->nlines += tmp->nlines;
        }
      ਨਹੀਂਤਾਂ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough lines, or else malloc a new one.
             Some compaction mechanism is possible but probably not
             worthwhile.  */
          ਆਖਰੀ = ਆਖਰੀ->ਅਗਲਾ = tmp;
          ਜੇ (ਕੁੱਲ_ਸਤਰਾਂ - ਪਹਿਲਾ->nlines > n_ਸਤਰਾਂ)
            {
              tmp = ਪਹਿਲਾ;
              ਕੁੱਲ_ਸਤਰਾਂ -= ਪਹਿਲਾ->nlines;
              ਪਹਿਲਾ = ਪਹਿਲਾ->ਅਗਲਾ;
            }
          ਨਹੀਂਤਾਂ
            tmp = xmalloc (ਆਕਾਰ (LBUFFER));
        }
    }

  ਮੁਕਤ (tmp);

  ਜੇ (n_read < 0 && errno != EAGAIN)
    {
      ret = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ਜਾਓ free_lbuffers;
    }

  /* If the file is empty, then bail out.  */
  ਜੇ (ਆਖਰੀ->nbytes == 0)
    ਜਾਓ free_lbuffers;

  /* This prevents a core dump when the pipe contains no newlines.  */
  ਜੇ (n_ਸਤਰਾਂ == 0)
    ਜਾਓ free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline.  */
  ਜੇ (ਆਖਰੀ->ਬਫਰ[ਆਖਰੀ->nbytes - 1] != ਸਤਰ_ਸਿਰਾ)
    {
      ++ਆਖਰੀ->nlines;
      ++ਕੁੱਲ_ਸਤਰਾਂ;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers.  */
  ਲਈ (tmp = ਪਹਿਲਾ; ਕੁੱਲ_ਸਤਰਾਂ - tmp->nlines > n_ਸਤਰਾਂ; tmp = tmp->ਅਗਲਾ)
    ਕੁੱਲ_ਸਤਰਾਂ -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file.  */
  {
    ਅੱਖਰ ਅਚਲ *beg = tmp->ਬਫਰ;
    ਅੱਖਰ ਅਚਲ *ਬਫਰ_ਸਿਰਾ = tmp->ਬਫਰ + tmp->nbytes;
    ਜੇ (ਕੁੱਲ_ਸਤਰਾਂ > n_ਸਤਰਾਂ)
      {
        /* Skip 'total_lines' - 'n_lines' newlines.  We made sure that
           'total_lines' - 'n_lines' <= 'tmp->nlines'.  */
        ਲਈ (idx_t j = ਕੁੱਲ_ਸਤਰਾਂ - n_ਸਤਰਾਂ; j; --j)
          {
            beg = rawmemchr (beg, ਸਤਰ_ਸਿਰਾ);
            ++beg;
          }
      }

    xwrite_stdout (beg, ਬਫਰ_ਸਿਰਾ - beg);
  }

  ਲਈ (tmp = tmp->ਅਗਲਾ; tmp; tmp = tmp->ਅਗਲਾ)
    xwrite_stdout (tmp->ਬਫਰ, tmp->nbytes);

free_lbuffers:
  ਜਦੋਂ (ਪਹਿਲਾ)
    {
      tmp = ਪਹਿਲਾ->ਅਗਲਾ;
      ਮੁਕਤ (ਪਹਿਲਾ);
      ਪਹਿਲਾ = tmp;
    }
  ਵਾਪਸ ret;
}

/* Print the last N_BYTES characters from the end of FD.
   Work even if the input is a pipe.
   This is a stripped down version of pipe_lines.
   The initial file offset is READ_POS if nonnegative, otherwise unknown.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ਸਥਿਰ off_t
pipe_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਗਿਣਤੀ_t n_ਬਾਈਟਾਂ,
            off_t read_pos)
{
  ਬਣਤਰ charbuffer
  {
    ਅੱਖਰ ਬਫਰ[BUFSIZ];
    idx_t nbytes;
    ਬਣਤਰ charbuffer *ਅਗਲਾ;
  };
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ charbuffer CBUFFER;
  CBUFFER *ਪਹਿਲਾ, *ਆਖਰੀ, *tmp;
  idx_t i;			/* Index into buffers.  */
  intmax_t ਕੁੱਲ_ਬਾਈਟਾਂ = 0;	/* Total characters in all buffers.  */
  ssize_t n_read;

  ਜੇ (read_pos < 0)
    read_pos = TYPE_MINIMUM (off_t);

  ਪਹਿਲਾ = ਆਖਰੀ = xmalloc (ਆਕਾਰ (CBUFFER));
  ਪਹਿਲਾ->nbytes = 0;
  ਪਹਿਲਾ->ਅਗਲਾ = ਨਿਰਾ;
  tmp = xmalloc (ਆਕਾਰ (CBUFFER));

  /* Input is always read into a fresh buffer.  */
  ਜਦੋਂ (true)
    {
      n_read = ਪਠ (fd, tmp->ਬਫਰ, BUFSIZ);
      ਜੇ (n_read <= 0)
        ਰੋਕੋ;
      read_pos += n_read;
      tmp->nbytes = n_read;
      tmp->ਅਗਲਾ = ਨਿਰਾ;

      ਕੁੱਲ_ਬਾਈਟਾਂ += tmp->nbytes;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, 'nbytes' can
         often be very small.  */
      ਜੇ (tmp->nbytes + ਆਖਰੀ->nbytes < BUFSIZ)
        {
          ਸਮ੍ਰਿਤੀ_ਨਕਲ (&ਆਖਰੀ->ਬਫਰ[ਆਖਰੀ->nbytes], tmp->ਬਫਰ, tmp->nbytes);
          ਆਖਰੀ->nbytes += tmp->nbytes;
        }
      ਨਹੀਂਤਾਂ
        {
          /* If there's not enough room, link the new buffer onto the end of
             the list, then either free up the oldest buffer for the next
             read if that would leave enough characters, or else malloc a new
             one.  Some compaction mechanism is possible but probably not
             worthwhile.  */
          ਆਖਰੀ = ਆਖਰੀ->ਅਗਲਾ = tmp;
          ਜੇ (ਕੁੱਲ_ਬਾਈਟਾਂ - ਪਹਿਲਾ->nbytes > n_ਬਾਈਟਾਂ)
            {
              tmp = ਪਹਿਲਾ;
              ਕੁੱਲ_ਬਾਈਟਾਂ -= ਪਹਿਲਾ->nbytes;
              ਪਹਿਲਾ = ਪਹਿਲਾ->ਅਗਲਾ;
            }
          ਨਹੀਂਤਾਂ
            {
              tmp = xmalloc (ਆਕਾਰ (CBUFFER));
            }
        }
    }

  ਮੁਕਤ (tmp);

  ਜੇ (n_read < 0 && errno != EAGAIN)
    {
      read_pos = -1 - errno;
      error (0, errno, _("error reading %s"), quoteaf (prettyname));
      ਜਾਓ free_cbuffers;
    }

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers.  */
  ਲਈ (tmp = ਪਹਿਲਾ; ਕੁੱਲ_ਬਾਈਟਾਂ - tmp->nbytes > n_ਬਾਈਟਾਂ; tmp = tmp->ਅਗਲਾ)
    ਕੁੱਲ_ਬਾਈਟਾਂ -= tmp->nbytes;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that 'total_bytes' - 'n_bytes' <= 'tmp->nbytes'.  */
  ਜੇ (ਕੁੱਲ_ਬਾਈਟਾਂ > n_ਬਾਈਟਾਂ)
    i = ਕੁੱਲ_ਬਾਈਟਾਂ - n_ਬਾਈਟਾਂ;
  ਨਹੀਂਤਾਂ
    i = 0;
  xwrite_stdout (&tmp->ਬਫਰ[i], tmp->nbytes - i);

  ਲਈ (tmp = tmp->ਅਗਲਾ; tmp; tmp = tmp->ਅਗਲਾ)
    xwrite_stdout (tmp->ਬਫਰ, tmp->nbytes);

  ਜੇ (read_pos < 0)
    read_pos = -1;

free_cbuffers:
  ਜਦੋਂ (ਪਹਿਲਾ)
    {
      tmp = ਪਹਿਲਾ->ਅਗਲਾ;
      ਮੁਕਤ (ਪਹਿਲਾ);
      ਪਹਿਲਾ = tmp;
    }
  ਵਾਪਸ read_pos;
}

/* Skip N_BYTES characters from the start of pipe FD, and print
   any extra characters that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ਸਥਿਰ ਪੂਰਨ
ਸ਼ੁਰੂ_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਗਿਣਤੀ_t n_ਬਾਈਟਾਂ)
{
  ਅੱਖਰ ਬਫਰ[BUFSIZ];

  ਜਦੋਂ (0 < n_ਬਾਈਟਾਂ)
    {
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, BUFSIZ);
      ਜੇ (ਬਾਈਟਾਂ_read == 0)
        ਵਾਪਸ -1;
      ਜੇ (ਬਾਈਟਾਂ_read < 0)
        {
          ਪੂਰਨ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ਵਾਪਸ ret;
        }
      ਜੇ (ਬਾਈਟਾਂ_read <= n_ਬਾਈਟਾਂ)
        n_ਬਾਈਟਾਂ -= ਬਾਈਟਾਂ_read;
      ਨਹੀਂਤਾਂ
        {
          xwrite_stdout (&ਬਫਰ[n_ਬਾਈਟਾਂ], ਬਾਈਟਾਂ_read - n_ਬਾਈਟਾਂ);
          ਰੋਕੋ;
        }
    }

  ਵਾਪਸ 0;
}

/* Skip N_LINES lines at the start of file or pipe FD, and print
   any extra bytes that were read beyond that.
   Return (-1 - errno) on failure, -1 if EOF, 0 otherwise.  */

ਸਥਿਰ ਪੂਰਨ
ਸ਼ੁਰੂ_ਸਤਰਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਗਿਣਤੀ_t n_ਸਤਰਾਂ)
{
  ਜੇ (n_ਸਤਰਾਂ == 0)
    ਵਾਪਸ 0;

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ ਬਫਰ[BUFSIZ];
      ssize_t ਬਾਈਟਾਂ_read = ਪਠ (fd, ਬਫਰ, BUFSIZ);
      ਜੇ (ਬਾਈਟਾਂ_read == 0) /* EOF */
        ਵਾਪਸ -1;
      ਜੇ (ਬਾਈਟਾਂ_read < 0) /* error */
        {
          ਪੂਰਨ ret = -1 - errno;
          error (0, errno, _("error reading %s"), quoteaf (prettyname));
          ਵਾਪਸ ret;
        }

      ਅੱਖਰ *ਬਫਰ_ਸਿਰਾ = ਬਫਰ + ਬਾਈਟਾਂ_read;
      ਅੱਖਰ *p = ਬਫਰ;
      ਜਦੋਂ ((p = memchr (p, ਸਤਰ_ਸਿਰਾ, ਬਫਰ_ਸਿਰਾ - p)))
        {
          ++p;
          ਜੇ (--n_ਸਤਰਾਂ == 0)
            {
              ਜੇ (p < ਬਫਰ_ਸਿਰਾ)
                xwrite_stdout (p, ਬਫਰ_ਸਿਰਾ - p);
              ਵਾਪਸ 0;
            }
        }
    }
}

/* Return true if FD might be open on a file residing on a remote file system,
   false if FD is known to be local.  F is the file's spec.
   If fstatfs fails, give a diagnostic and return true.
   If fstatfs cannot be called, return true.  */
ਸਥਿਰ bool
fremote (ਪੂਰਨ fd, ਬਣਤਰ File_spec ਅਚਲ *f)
{
  bool remote = true;           /* be conservative (poll by default).  */

#ਜੇ HAVE_FSTATFS && HAVE_STRUCT_STATFS_F_TYPE \
 && (defined __linux__ || defined __ANDROID__)
  ਬਣਤਰ statfs buf;
  ਪੂਰਨ err = fstatfs (fd, &buf);
  ਜੇ (err != 0)
    {
      /* On at least linux-2.6.38, fstatfs fails with ENOSYS when FD
         is open on a pipe.  Treat that like a remote file.  */
      ਜੇ (errno != ENOSYS)
        error (0, errno, _("cannot determine location of %s. "
                           "reverting to polling"), quoteaf (f->prettyname));
    }
  ਨਹੀਂਤਾਂ
    {
      /* Treat unrecognized file systems as "remote", so caller polls.
         Note README-release has instructions for syncing the internal
         list with the latest Linux kernel file system constants.  */
      remote = is_local_fs_type (buf.f_type) <= 0;
    }
#ਜੇ_ਅੰਤ

  ਵਾਪਸ remote;
}

/* open/fstat F->name and handle changes.  */
ਸਥਿਰ ਖਾਲੀ
recheck (ਬਣਤਰ File_spec *f, bool blocking)
{
  ਬਣਤਰ stat new_stats;
  bool ok = false;
  bool is_stdin = (streq (f->name, "-"));
  ਪੂਰਨ prev_errnum = f->errnum;
  bool new_file;
  ਪੂਰਨ fd = (is_stdin
            ? STDIN_FILENO
            : open (f->name, O_RDONLY | (blocking ? 0 : O_NONBLOCK)));
  ਪੂਰਨ open_errno = fd < 0 ? errno : 0;

  affirm (valid_file_spec (f));

  ਜੇ (! disable_inotify && issymlink (f->name) == 1)
    {
      /* Diagnose the edge case where a regular file is changed
         to a symlink.  We avoid inotify with symlinks since
         it's awkward to match between symlink name and target.  */
      f->errnum = -1;
      f->ignore = true;

      error (0, 0, _("%s has been replaced with an untailable symbolic link"),
             quoteaf (f->prettyname));
    }
  ਨਹੀਂਤਾਂ ਜੇ (fd < 0 || fstat (fd, &new_stats) < 0)
    {
      f->errnum = fd < 0 ? open_errno : errno;
      ਜੇ (fd < 0)
        {
          ਜੇ (f->tailable)
            {
              /* FIXME-maybe: detect the case in which the file first becomes
                 unreadable (perms), and later becomes readable again and can
                 be seen to be the same file (dev/ino).  Otherwise, tail prints
                 the entire contents of the file when it becomes readable.  */
              error (0, f->errnum, _("%s has become inaccessible"),
                     quoteaf (f->prettyname));
            }
          ਨਹੀਂਤਾਂ
            {
              /* say nothing... it's still not tailable */
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (prev_errnum != f->errnum)
        error (0, f->errnum, "%s", quotef (f->prettyname));
    }
  ਨਹੀਂਤਾਂ ਜੇ (!IS_TAILABLE_FILE_TYPE (new_stats.st_ਬਹੁਲਕ))
    {
      f->errnum = -1;
      f->ignore = ! (reopen_inaccessible_files && follow_ਬਹੁਲਕ == Follow_name);
      ਜੇ (f->tailable || ! (prev_errnum < 0 || prev_errnum == EISDIR))
        error (0, 0, _("%s has been replaced with an untailable file%s"),
               quoteaf (f->prettyname),
               f->ignore ? _("; giving up on this name") : "");
    }
  ਨਹੀਂਤਾਂ ਜੇ ((f->remote = fremote (fd, f)) && ! disable_inotify)
    {
      f->errnum = -1;
      error (0, 0, _("%s has been replaced with an untailable remote file"),
             quoteaf (f->prettyname));
      f->ignore = true;
    }
  ਨਹੀਂਤਾਂ
    {
      ok = true;
      f->errnum = 0;
    }

  f->tailable = ok;

  new_file = false;
  ਜੇ (!ok)
    {
      close_fd (fd, f);
      close_fd (f->fd, f);
      f->fd = -1;
    }
  ਨਹੀਂਤਾਂ ਜੇ (prev_errnum && prev_errnum != ENOENT)
    {
      new_file = true;
      affirm (f->fd < 0);
      error (0, 0, _("%s has become accessible"), quoteaf (f->prettyname));
    }
  ਨਹੀਂਤਾਂ ਜੇ (f->fd < 0)
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
  ਨਹੀਂਤਾਂ ਜੇ (!SAME_INODE (*f, new_stats))
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
  ਨਹੀਂਤਾਂ
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
  ਜੇ (new_file)
    {
      /* Start at the file's current position, normally the beginning.  */
      ਰਿਕਾਰਡ_open_fd (f, fd, -1, &new_stats, is_stdin ? -1 : blocking);
    }
}

/* Return true if any of the N_FILES files in F are live, i.e., have
   open file descriptors, or should be checked again (see --retry).
   When following descriptors, checking should only continue when any
   of the files is not yet ignored.  */

ਸਥਿਰ bool
any_ਜੀਵਤ_files (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  /* In inotify mode, ignore may be set for files
     which may later be replaced with new files.
     So always consider files live in -F mode.  */
  ਜੇ (reopen_inaccessible_files && follow_ਬਹੁਲਕ == Follow_name)
    ਵਾਪਸ true;

  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      ਜੇ (0 <= f[i].fd)
        ਵਾਪਸ true;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (! f[i].ignore && reopen_inaccessible_files)
            ਵਾਪਸ true;
        }
    }

  ਵਾਪਸ false;
}

/* Return true if any writers exist, even as zombies.  */

ਸਥਿਰ bool
some_writers_exist (ਖਾਲੀ)
{
  /* To avoid some (though not all) races if the kernel reuses PIDs,
     check all PIDs instead of returning merely because one PID exists.  */
  ਲਈ (idx_t i = 0; i < nbpids; )
    {
      ਜੇ (kill (pids[i], 0) < 0 && errno == ESRCH)
        {
          nbpids--;
          memmove (&pids[i], &pids[i + 1], (nbpids - i) * ਆਕਾਰ pids[i]);
        }
      ਨਹੀਂਤਾਂ
        i++;
    }


  ਵਾਪਸ 0 < nbpids;
}

/* Tail N_FILES files forever, or until killed.
   The pertinent information for each file is stored in an entry of F.
   Loop over each of them, doing an fstat to see if they have changed size,
   and an occasional open/fstat to see if any dev/ino pair has changed.
   If none of them have changed size in one iteration, sleep for a
   while and try again.  Continue until the user interrupts us.  */

ਸਥਿਰ ਖਾਲੀ
ਪੂਛ_forever (ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files, ਦੁੱਗਣਾ sleep_interval)
{
  ਪੂਰਨ ਆਖਰੀ = n_files - 1;

  ਸਥਿਰ bool debugged;

  /* Flush initial output, in case reading blocks below.  */
  ਜੇ (fflush (stdout) < 0)
    write_error ();

  ਜਦੋਂ (true)
    {
      /* Use blocking I/O as an optimization, when it's easy.  */
      bool blocking = (!nbpids && follow_ਬਹੁਲਕ == Follow_descriptor
                       && n_files == 1 && 0 <= f[0].fd && !S_ISREG (f[0].ਬਹੁਲਕ_2));

      ਜੇ (debug && !debugged)
        {
          debugged = true;
          error (0, 0, "%s", blocking
                             ? _("using blocking mode")
                             : _("using polling mode"));
        }

      bool any_ਨਿਵੇਸ਼ = false;

      ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
        {
          ਬਣਤਰ stat stats;

          ਜੇ (f[i].ignore)
            ਜਾਰੀ;

          ਪੂਰਨ fd = f[i].fd;
          ਜੇ (fd < 0)
            {
              recheck (&f[i], blocking);
              ਜਾਰੀ;
            }

          ਅੱਖਰ ਅਚਲ *prettyname = f[i].prettyname;
          mode_t ਬਹੁਲਕ_2 = f[i].ਬਹੁਲਕ_2;

          ਜੇ (f[i].blocking != blocking)
            {
              ਪੂਰਨ old_flags = fcntl (fd, F_GETFL);
              ਪੂਰਨ new_flags = old_flags | (blocking ? 0 : O_NONBLOCK);
              ਜੇ (old_flags < 0
                  || (new_flags != old_flags
                      && fcntl (fd, F_SETFL, new_flags) < 0))
                {
                  /* Don't update f[i].blocking if fcntl fails.  */
                  ਜੇ (S_ISREG (f[i].ਬਹੁਲਕ_2) && errno == EPERM)
                    {
                      /* This happens when using tail -f on a file with
                         the append-only attribute.  */
                    }
                  ਨਹੀਂਤਾਂ
                    error (EXIT_FAILURE, errno,
                           _("%s: cannot change nonblocking mode"),
                           quotef (prettyname));
                }
              ਨਹੀਂਤਾਂ
                f[i].blocking = blocking;
            }

          bool read_unchanged = false;
          ਜੇ (!f[i].blocking)
            {
              ਜੇ (fstat (fd, &stats) < 0)
                {
                  f[i].fd = -1;
                  f[i].errnum = errno;
                  error (0, errno, "%s", quotef (prettyname));
                  close (fd); /* ignore failure */
                  ਜਾਰੀ;
                }

              ਜੇ (f[i].ਬਹੁਲਕ_2 == stats.st_ਬਹੁਲਕ
                  && (! S_ISREG (stats.st_ਬਹੁਲਕ)
                      || f[i].read_pos == stats.st_ਮਾਪ)
                  && timespec_cmp (f[i].mtime, get_stat_mtime (&stats)) == 0)
                {
                  ਜੇ ((max_n_unchanged_stats_between_opens
                       <= f[i].n_unchanged_stats++)
                      && follow_ਬਹੁਲਕ == Follow_name)
                    {
                      recheck (&f[i], f[i].blocking);
                      f[i].n_unchanged_stats = 0;
                    }
                  ਜੇ (fd != f[i].fd || S_ISREG (stats.st_ਬਹੁਲਕ) || 1 < n_files)
                    ਜਾਰੀ;
                  ਨਹੀਂਤਾਂ
                    read_unchanged = true;
                }

              affirm (fd == f[i].fd);

              /* This file has changed.  Print out what we can, and
                 then keep looping.  */

              f[i].mtime = get_stat_mtime (&stats);
              f[i].ਬਹੁਲਕ_2 = stats.st_ਬਹੁਲਕ;

              /* reset counter */
              ਜੇ (! read_unchanged)
                f[i].n_unchanged_stats = 0;

              /* XXX: This is only a heuristic, as the file may have also
                 been truncated and written to if st_size >= size
                 (in which case we ignore new data <= size).  */
              ਜੇ (S_ISREG (ਬਹੁਲਕ_2) && stats.st_ਮਾਪ < f[i].read_pos)
                {
                  error (0, 0, _("%s: file truncated"),
                         quotef (prettyname));
                  /* Assume the file was truncated to 0,
                     and therefore output all "new" data.  */
                  f[i].read_pos = xlseek (fd, 0, SEEK_SET, prettyname);
                }

              ਜੇ (i != ਆਖਰੀ)
                {
                  ਜੇ (print_ਸਿਰਲੇਖਾਂ)
                    write_ਸਿਰਲੇਖ (prettyname);
                  ਆਖਰੀ = i;
                }
            }

          /* Don't read more than st_size on networked file systems
             because it was seen on glusterfs at least, that st_size
             may be smaller than the data read on a _subsequent_ stat call.  */
          ਗਿਣਤੀ_t ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read;
          ਜੇ (f[i].blocking)
            ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = COPY_A_ਬਫਰ;
          ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (ਬਹੁਲਕ_2) && f[i].remote)
            ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = stats.st_ਮਾਪ - f[i].read_pos;
          ਨਹੀਂਤਾਂ
            ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read = COPY_ਮੰਜ਼ਿਲ_EOF;

          ਗਿਣਤੀ_t nr = dump_remainder (false, prettyname, fd, ਬਾਈਟਾਂ_ਮੰਜ਼ਿਲ_read);
          ਜੇ (0 < nr)
            {
              ਜੇ (S_ISREG (ਬਹੁਲਕ_2))
                f[i].read_pos += nr;
              ਜੇ (read_unchanged)
                f[i].n_unchanged_stats = 0;
              any_ਨਿਵੇਸ਼ = true;
            }
        }

      ਜੇ (! any_ਜੀਵਤ_files (f, n_files))
        {
          error (EXIT_FAILURE, 0, _("no files remaining"));
          ਰੋਕੋ;
        }

      ਜੇ ((!any_ਨਿਵੇਸ਼ || blocking) && fflush (stdout) < 0)
        write_error ();

      check_output_alive ();

      ਜੇ (!any_ਨਿਵੇਸ਼)
        {
          ਜੇ (pids)
            {
              /* Exit loop if it was already known that no writers exist.
                 Otherwise, to avoid a race, loop one more time
                 without sleeping if no writers exist now.  */
              ਜੇ (!nbpids)
                ਰੋਕੋ;
              ਜੇ (!some_writers_exist ())
                ਜਾਰੀ;
            }

          ਜੇ (xnanosleep (sleep_interval))
            error (EXIT_FAILURE, errno, _("cannot read realtime clock"));
        }
    }
}

#ਜੇ HAVE_INOTIFY

/* Return true if any of the N_FILES files in F is remote, i.e., has
   an open file descriptor and is on a network file system.  */

ਸਥਿਰ bool
any_remote_file (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ਜੇ (0 <= f[i].fd && f[i].remote)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Return true if any of the N_FILES files in F is non remote, i.e., has
   an open file descriptor and is not on a network file system.  */

ਸਥਿਰ bool
any_non_remote_file (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ਜੇ (0 <= f[i].fd && ! f[i].remote)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Return true if any of the N_FILES files in F is a symlink.
   Note we don't worry about the edge case where "-" exists,
   since that will have the same consequences for inotify,
   which is the only context this function is currently used.  */

ਸਥਿਰ bool
any_symlinks (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ਜੇ (issymlink (f[i].name) == 1)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Return true if any of the N_FILES files in F is not
   a regular file or fifo.  This is used to avoid adding inotify
   watches on a device file for example, which inotify
   will accept, but not give any events for.  */

ਸਥਿਰ bool
any_non_regular_fifo (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ਜੇ (0 <= f[i].fd && ! S_ISREG (f[i].ਬਹੁਲਕ_2) && ! S_ISFIFO (f[i].ਬਹੁਲਕ_2))
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* Return true if any of the N_FILES files in F represents
   stdin and is not ignored.  */

ਸਥਿਰ bool
tailable_stdin (ਅਚਲ ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ਜੇ (!f[i].ignore && streq (f[i].name, "-"))
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

ਸਥਿਰ size_t
wd_hasher (ਅਚਲ ਖਾਲੀ *ਇੰਦਰਾਜ, size_t tabsize)
{
  ਅਚਲ ਬਣਤਰ File_spec *spec = ਇੰਦਰਾਜ;
  ਵਾਪਸ spec->wd % tabsize;
}

ਸਥਿਰ bool
wd_comparator (ਅਚਲ ਖਾਲੀ *e1, ਅਚਲ ਖਾਲੀ *e2)
{
  ਅਚਲ ਬਣਤਰ File_spec *spec1 = e1;
  ਅਚਲ ਬਣਤਰ File_spec *spec2 = e2;
  ਵਾਪਸ spec1->wd == spec2->wd;
}

/* Hash Follow_name watches by parent directory wd + basename so directory
   inotify events can be matched in expected O(1) instead of scanning all
   files.  */
ਸਥਿਰ size_t
ਮਾਪਾ_name_hasher (ਅਚਲ ਖਾਲੀ *ਇੰਦਰਾਜ, size_t tabsize)
{
  ਅਚਲ ਬਣਤਰ File_spec *spec = ਇੰਦਰਾਜ;
  ਅੱਖਰ ਅਚਲ *name = spec->name + spec->basename_ਸ਼ੁਰੂ;
  /* hash_string already reduces modulo TABSIZE; mix in parent_wd so the
     same basename under different directories does not collide by design.  */
  ਵਾਪਸ (hash_string (name, tabsize) + (size_t) spec->ਮਾਪਾ_wd) % tabsize;
}

ਸਥਿਰ bool
ਮਾਪਾ_name_comparator (ਅਚਲ ਖਾਲੀ *e1, ਅਚਲ ਖਾਲੀ *e2)
{
  ਅਚਲ ਬਣਤਰ File_spec *a = e1;
  ਅਚਲ ਬਣਤਰ File_spec *b = e2;
  ਵਾਪਸ (a->ਮਾਪਾ_wd == b->ਮਾਪਾ_wd
          && streq (a->name + a->basename_ਸ਼ੁਰੂ,
                    b->name + b->basename_ਸ਼ੁਰੂ));
}

/* Output (new) data for FSPEC->fd.
   PREV_FSPEC records the last File_spec for which we output.  */
ਸਥਿਰ ਖਾਲੀ
check_fspec (ਬਣਤਰ File_spec *fspec, ਬਣਤਰ File_spec **prev_fspec)
{
  ਜੇ (fspec->fd < 0)
    ਵਾਪਸ;

  ਬਣਤਰ stat stats;
  ਜੇ (fstat (fspec->fd, &stats) < 0)
    {
      fspec->errnum = errno;
      close_fd (fspec->fd, fspec);
      fspec->fd = -1;
      ਵਾਪਸ;
    }

  ਅੱਖਰ ਅਚਲ *prettyname = fspec->prettyname;

  /* XXX: This is only a heuristic, as the file may have also
     been truncated and written to if st_size >= size
     (in which case we ignore new data <= size).
     Though in the inotify case it's more likely we'll get
     separate events for truncate() and write().  */
  ਜੇ (S_ISREG (fspec->ਬਹੁਲਕ_2) && stats.st_ਮਾਪ < fspec->read_pos)
    {
      error (0, 0, _("%s: file truncated"), quotef (prettyname));
      fspec->read_pos = xlseek (fspec->fd, 0, SEEK_SET, prettyname);
    }
  ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (fspec->ਬਹੁਲਕ_2) && stats.st_ਮਾਪ == fspec->read_pos
           && timespec_cmp (fspec->mtime, get_stat_mtime (&stats)) == 0)
    ਵਾਪਸ;

  bool want_ਸਿਰਲੇਖ = print_ਸਿਰਲੇਖਾਂ && (fspec != *prev_fspec);

  ਗਿਣਤੀ_t nr = dump_remainder (want_ਸਿਰਲੇਖ, prettyname, fspec->fd, COPY_ਮੰਜ਼ਿਲ_EOF);
  ਜੇ (0 < nr)
    {
      ਜੇ (S_ISREG (fspec->ਬਹੁਲਕ_2))
        fspec->read_pos += nr;
      *prev_fspec = fspec;
      ਜੇ (fflush (stdout) < 0)
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
ਸਥਿਰ ਖਾਲੀ
ਪੂਛ_forever_inotify (ਪੂਰਨ wd, ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files,
                      ਦੁੱਗਣਾ sleep_interval, Hash_ਸਾਰਣੀ **wd_ਮੰਜ਼ਿਲ_namep,
                      Hash_ਸਾਰਣੀ **ਮਾਪਾ_by_namep)
{
# ਜੇ ਪੂਛ_TEST_SLEEP
  /* Delay between open() and inotify_add_watch()
     to help trigger different cases.  */
  xnanosleep (1000000);
# endif
  ਪੂਰਨ max_realloc = 3;

  /* Map an inotify watch descriptor to the name of the file it's watching.  */
  Hash_ਸਾਰਣੀ *wd_ਮੰਜ਼ਿਲ_name;

  /* Map (parent_wd, basename) to File_spec for Follow_name
     directory events.  */
  Hash_ਸਾਰਣੀ *ਮਾਪਾ_by_name = ਨਿਰਾ;

  bool ਮਿਲਿਆ_watchable_file = false;
  bool tailed_but_unwatchable = false;
  bool ਮਿਲਿਆ_unwatchable_dir = false;
  bool no_inotify_resources = false;
  ਬਣਤਰ File_spec *prev_fspec;
  idx_t evlen = 0;
  ਅੱਖਰ *evbuf;
  idx_t evbuf_off = 0;

  *ਮਾਪਾ_by_namep = ਨਿਰਾ;

  wd_ਮੰਜ਼ਿਲ_name = hash_initialize (n_files, ਨਿਰਾ, wd_hasher, wd_comparator,
                                ਨਿਰਾ);
  ਜੇ (! wd_ਮੰਜ਼ਿਲ_name)
    xalloc_die ();
  *wd_ਮੰਜ਼ਿਲ_namep = wd_ਮੰਜ਼ਿਲ_name;

  ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
    {
      ਮਾਪਾ_by_name = hash_initialize (n_files, ਨਿਰਾ, ਮਾਪਾ_name_hasher,
                                        ਮਾਪਾ_name_comparator, ਨਿਰਾ);
      ਜੇ (! ਮਾਪਾ_by_name)
        xalloc_die ();
      *ਮਾਪਾ_by_namep = ਮਾਪਾ_by_name;
    }

  /* The events mask used with inotify on files (not directories).  */
  uint32_t inotify_wd_ਨਕਾਬ = ਵਿੱਚ_MODIFY;
  /* TODO: Perhaps monitor these events in Follow_descriptor mode also,
     to tag reported file names with "deleted", "moved" etc.  */
  ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
    inotify_wd_ਨਕਾਬ |= (ਵਿੱਚ_ATTRIB | ਵਿੱਚ_DELETE_SELF | ਵਿੱਚ_MOVE_SELF);

  /* Add an inotify watch for each watched file.  If -F is specified then watch
     its parent directory too, in this way when they re-appear we can add them
     again to the watch list.  */
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      ਜੇ (!f[i].ignore)
        {
          idx_t fnlen = ਲੜੀ_ਲੰਬਾਈ (f[i].name);
          ਜੇ (evlen < fnlen)
            evlen = fnlen;

          f[i].wd = -1;

          ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
            {
              idx_t dirlen = dir_len (f[i].name);
              ਅੱਖਰ prev = f[i].name[dirlen];
              f[i].basename_ਸ਼ੁਰੂ = ਆਖਰੀ_component (f[i].name) - f[i].name;

              f[i].name[dirlen] = '\0';

               /* It's fine to add the same directory more than once.
                  In that case the same watch descriptor is returned.  */
              f[i].ਮਾਪਾ_wd = inotify_ਜੋੜੋ_watch (wd, dirlen ? f[i].name : ".",
                                                  (ਵਿੱਚ_CREATE | ਵਿੱਚ_DELETE
                                                   | ਵਿੱਚ_MOVED_ਮੰਜ਼ਿਲ | ਵਿੱਚ_ATTRIB
                                                   | ਵਿੱਚ_DELETE_SELF));

              f[i].name[dirlen] = prev;

              ਜੇ (f[i].ਮਾਪਾ_wd < 0)
                {
                  ਜੇ (errno != ENOSPC) /* suppress confusing error.  */
                    error (0, errno, _("cannot watch parent directory of %s"),
                           quoteaf (f[i].name));
                  ਨਹੀਂਤਾਂ
                    error (0, 0, _("inotify resources exhausted"));
                  ਮਿਲਿਆ_unwatchable_dir = true;
                  /* We revert to polling below.  Note invalid uses
                     of the inotify API will still be diagnosed.  */
                  ਰੋਕੋ;
                }

              ਜੇ (hash_ਪਾਓ (ਮਾਪਾ_by_name, &(f[i])) == ਨਿਰਾ)
                xalloc_die ();
            }

          f[i].wd = inotify_ਜੋੜੋ_watch (wd, f[i].name, inotify_wd_ਨਕਾਬ);

          ਜੇ (f[i].wd < 0)
            {
              ਜੇ (0 <= f[i].fd)  /* already tailed.  */
                tailed_but_unwatchable = true;
              ਜੇ (errno == ENOSPC || errno == ENOMEM)
                {
                  no_inotify_resources = true;
                  error (0, 0, _("inotify resources exhausted"));
                  ਰੋਕੋ;
                }
              ਨਹੀਂਤਾਂ ਜੇ (errno != f[i].errnum)
                error (0, errno, _("cannot watch %s"), quoteaf (f[i].name));
              ਜਾਰੀ;
            }

          ਜੇ (hash_ਪਾਓ (wd_ਮੰਜ਼ਿਲ_name, &(f[i])) == ਨਿਰਾ)
            xalloc_die ();

          ਮਿਲਿਆ_watchable_file = true;
        }
    }

  /* Linux kernel 2.6.24 at least has a bug where eventually, ENOSPC is always
     returned by inotify_add_watch.  In any case we should revert to polling
     when there are no inotify resources.  Also a specified directory may not
     be currently present or accessible, so revert to polling.  Also an already
     tailed but unwatchable due rename/unlink race, should also revert.  */
  ਜੇ (no_inotify_resources || ਮਿਲਿਆ_unwatchable_dir
      || (follow_ਬਹੁਲਕ == Follow_descriptor && tailed_but_unwatchable))
    ਵਾਪਸ;
  ਜੇ (follow_ਬਹੁਲਕ == Follow_descriptor && !ਮਿਲਿਆ_watchable_file)
    ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);

  prev_fspec = &(f[n_files - 1]);

  /* Check files again.  New files or data can be available since last time we
     checked and before they are watched by inotify.  */
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      ਜੇ (! f[i].ignore)
        {
          /* check for new files.  */
          ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
            recheck (&(f[i]), false);
          ਨਹੀਂਤਾਂ ਜੇ (0 <= f[i].fd)
            {
              /* If the file was replaced in the small window since we tailed,
                 then assume the watch is on the wrong item (different to
                 that we've already produced output for), and so revert to
                 polling the original descriptor.  */
              ਬਣਤਰ stat stats;

              ਜੇ (! (stat (f[i].name, &stats) < 0
                     || SAME_INODE (f[i], stats)))
                {
                  error (0, errno, _("%s was replaced"),
                         quoteaf (f[i].prettyname));
                  ਵਾਪਸ;
                }
            }

          /* check for new data.  */
          check_fspec (&f[i], &prev_fspec);
        }
    }

  ਜੇ (debug)
    error (0, 0, "%s", _("using notification mode"));

  evlen += ਆਕਾਰ (ਬਣਤਰ inotify_event) + 1;
  evbuf = ximalloc (evlen);

  /* Wait for inotify events and handle them.  Events on directories
     ensure that watched files can be re-added when following by name.
     This loop blocks on the 'read' call until a new event is notified.
     But when --pid=P is specified, tail usually waits via poll.  */
  ptrdiff_t len = 0;
  ਜਦੋਂ (true)
    {
      ਬਣਤਰ File_spec *fspec;
      ਬਣਤਰ inotify_event *ev;
      ਖਾਲੀ *void_ev;

      /* When following by name without --retry, and the last file has
         been unlinked or renamed-away, diagnose it and return.  */
      ਜੇ (follow_ਬਹੁਲਕ == Follow_name
          && ! reopen_inaccessible_files
          && hash_get_n_entries (wd_ਮੰਜ਼ਿਲ_name) == 0)
        error (EXIT_FAILURE, 0, _("no files remaining"));

      ਜੇ (len <= evbuf_off)
        {
          /* Poll for inotify events.  When watching a PID, ensure
             that a read from WD will not block indefinitely.
             If MONITOR_OUTPUT, also poll for a broken output pipe.  */

          ਪੂਰਨ file_change;
          ਬਣਤਰ pollfd pfd[2];
          ਕਰੋ
            {
              /* How many ms to wait for changes.  -1 means wait forever.  */
              ਪੂਰਨ delay = -1;

              ਜੇ (pids)
                {
                  ਜੇ (!nbpids)
                    ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);

                  ਜੇ (!some_writers_exist () || sleep_interval <= 0)
                    delay = 0;
                  ਨਹੀਂਤਾਂ ਜੇ (sleep_interval < ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ / 1000 - 1)
                    {
                      /* delay = ceil (sleep_interval * 1000), sans libm.  */
                      ਦੁੱਗਣਾ ddelay = sleep_interval * 1000;
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
          ਜਦੋਂ (file_change == 0 || (file_change < 0 && errno == EINTR));

          ਜੇ (file_change < 0)
            error (EXIT_FAILURE, errno,
                   _("error waiting for inotify and output events"));
          ਜੇ (pfd[1].revents)
            die_pipe ();

          len = ਪਠ (wd, evbuf, evlen);
          evbuf_off = 0;

          /* For kernels prior to 2.6.21, read returns 0 when the buffer
             is too small.  */
          ਜੇ ((len == 0 || (len < 0 && errno == EINVAL))
              && max_realloc--)
            {
              len = 0;
              evlen *= 2;
              evbuf = xirealloc (evbuf, evlen);
              ਜਾਰੀ;
            }

          ਜੇ (len <= 0)
            error (EXIT_FAILURE, errno, _("error reading inotify event"));
        }

      void_ev = evbuf + evbuf_off;
      ev = void_ev;
      evbuf_off += ਆਕਾਰ (*ev) + ev->len;

      /* If a directory is deleted, IN_DELETE_SELF is emitted
         with ev->name of length 0.
         We need to catch it, otherwise it would wait forever,
         as wd for directory becomes inactive. Revert to polling now.   */
      ਜੇ ((ev->ਨਕਾਬ & ਵਿੱਚ_DELETE_SELF) && ! ev->len)
        {
          ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
            {
              ਜੇ (ev->wd == f[i].ਮਾਪਾ_wd)
                {
                  error (0, 0,
                      _("directory containing watched file was removed"));
                  ਵਾਪਸ;
                }
            }
        }

      ਜੇ (ev->len) /* event on ev->name in watched directory.  */
        {
          ਬਣਤਰ File_spec ਕੁੰਜੀ;
          ਜੇ (! ਮਾਪਾ_by_name)
            ਜਾਰੀ;

          /* Probe key: basename is the whole string at KEY.NAME.  */
          ਕੁੰਜੀ.ਮਾਪਾ_wd = ev->wd;
          ਕੁੰਜੀ.name = ev->name;
          ਕੁੰਜੀ.basename_ਸ਼ੁਰੂ = 0;
          fspec = hash_lookup (ਮਾਪਾ_by_name, &ਕੁੰਜੀ);

          /* It is not a watched file.  */
          ਜੇ (! fspec)
            ਜਾਰੀ;

          ਪੂਰਨ new_wd = -1;
          bool deleting = !! (ev->ਨਕਾਬ & ਵਿੱਚ_DELETE);

          ਜੇ (! deleting)
            {
              /* Adding the same inode again will look up any existing wd.  */
              new_wd = inotify_ਜੋੜੋ_watch (wd, fspec->name, inotify_wd_ਨਕਾਬ);
            }

          ਜੇ (! deleting && new_wd < 0)
            {
              ਜੇ (errno == ENOSPC || errno == ENOMEM)
                {
                  error (0, 0, _("inotify resources exhausted"));
                  ਵਾਪਸ; /* revert to polling.  */
                }
              ਨਹੀਂਤਾਂ
                {
                  /* Can get ENOENT for a dangling symlink for example.  */
                  error (0, errno, _("cannot watch %s"), quoteaf (fspec->name));
                }
              /* We'll continue below after removing the existing watch.  */
            }

          /* This will be false if only attributes of file change.  */
          bool new_watch;
          new_watch = (! deleting) && (fspec->wd < 0 || new_wd != fspec->wd);

          ਜੇ (new_watch)
            {
              ਜੇ (0 <= fspec->wd)
                {
                  inotify_rm_watch (wd, fspec->wd);
                  hash_remove (wd_ਮੰਜ਼ਿਲ_name, fspec);
                }

              fspec->wd = new_wd;

              ਜੇ (new_wd < 0)
                ਜਾਰੀ;

              /* If the file was moved then inotify will use the source file wd
                for the destination file.  Make sure the key is not present in
                the table.  */
              ਬਣਤਰ File_spec *prev = hash_remove (wd_ਮੰਜ਼ਿਲ_name, fspec);
              ਜੇ (prev && prev != fspec)
                {
                  ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
                    recheck (prev, false);
                  prev->wd = -1;
                  close_fd (prev->fd, prev);
                }

              ਜੇ (hash_ਪਾਓ (wd_ਮੰਜ਼ਿਲ_name, fspec) == ਨਿਰਾ)
                xalloc_die ();
            }

          ਜੇ (follow_ਬਹੁਲਕ == Follow_name)
            recheck (fspec, false);
        }
      ਨਹੀਂਤਾਂ
        {
          ਬਣਤਰ File_spec ਕੁੰਜੀ;
          ਕੁੰਜੀ.wd = ev->wd;
          fspec = hash_lookup (wd_ਮੰਜ਼ਿਲ_name, &ਕੁੰਜੀ);
        }

      ਜੇ (! fspec)
        ਜਾਰੀ;

      ਜੇ (ev->ਨਕਾਬ & (ਵਿੱਚ_ATTRIB | ਵਿੱਚ_DELETE | ਵਿੱਚ_DELETE_SELF | ਵਿੱਚ_MOVE_SELF))
        {
          /* Note for IN_MOVE_SELF (the file we're watching has
             been clobbered via a rename) without --retry we leave the watch
             in place since it may still be part of the set
             of watched names.  */
          ਜੇ (ev->ਨਕਾਬ & ਵਿੱਚ_DELETE_SELF
              || (!reopen_inaccessible_files && (ev->ਨਕਾਬ & ਵਿੱਚ_MOVE_SELF)))
            {
              inotify_rm_watch (wd, fspec->wd);
              hash_remove (wd_ਮੰਜ਼ਿਲ_name, fspec);
            }

          /* Note we get IN_ATTRIB for unlink() as st_nlink decrements.
             The usual path is a close() done in recheck() triggers
             an IN_DELETE_SELF event as the inode is removed.
             However sometimes open() will succeed as even though
             st_nlink is decremented, the dentry (cache) is not updated.
             Thus we depend on the IN_DELETE event on the directory
             to trigger processing for the removed file.  */

          recheck (fspec, false);

          ਜਾਰੀ;
        }
      check_fspec (fspec, &prev_fspec);
    }
}
#ਜੇ_ਅੰਤ

/* Output the last bytes of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_BYTES bytes.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ਸਥਿਰ off_t
ਪੂਛ_ਬਾਈਟਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st,
            ਗਿਣਤੀ_t n_ਬਾਈਟਾਂ)
{
  off_t current_pos
    = (presume_ਨਿਵੇਸ਼_pipe
       ? -1
       : lseek (fd, from_ਸ਼ੁਰੂ ? MIN (n_ਬਾਈਟਾਂ, OFF_T_MAX) : 0, SEEK_CUR));

  ਜੇ (from_ਸ਼ੁਰੂ)
    {
      ਜੇ (current_pos < 0)
        {
          ਪੂਰਨ t = ਸ਼ੁਰੂ_ਬਾਈਟਾਂ (prettyname, fd, n_ਬਾਈਟਾਂ);
          ਜੇ (t < 0)
            ਵਾਪਸ t;
        }
      n_ਬਾਈਟਾਂ = COPY_ਮੰਜ਼ਿਲ_EOF;
    }
  ਨਹੀਂਤਾਂ
    {
      off_t initial_pos = current_pos;
      off_t ਸਿਰਾ_pos = -1;

      ਜੇ (0 <= current_pos)
        {
          ਜੇ (usable_st_ਮਾਪ (st))
            {
              /* Use st_size only if it's so large that this is
                 probably not a /proc or similar file, where st_size
                 is notional.  */
              off_t smallish_ਮਾਪ = STP_BLKSIZE (st);
              ਜੇ (smallish_ਮਾਪ < st->st_ਮਾਪ)
                ਸਿਰਾ_pos = st->st_ਮਾਪ;
            }
          ਨਹੀਂਤਾਂ
            {
              off_t minus_n;
              ਜੇ (ckd_sub (&minus_n, 0, n_ਬਾਈਟਾਂ))
                ਸਿਰਾ_pos = 0;
              ਨਹੀਂਤਾਂ
                {
                  off_t e_n_pos = lseek (fd, minus_n, SEEK_END);
                  ਜੇ (0 <= e_n_pos)
                    {
                      current_pos = e_n_pos;
                      ਸਿਰਾ_pos = e_n_pos + n_ਬਾਈਟਾਂ;
                    }
                  ਨਹੀਂਤਾਂ
                    {
                      /* POSIX says errno is unreliable here.
                         Perhaps the file is smaller than N_BYTES.
                         Try again at file end.  */
                      off_t e_pos = lseek (fd, 0, SEEK_END);
                      ਜੇ (0 <= e_pos)
                        current_pos = ਸਿਰਾ_pos = e_pos;
                    }
               }
            }
        }

      /* If the end is known and more than N_BYTES after the initial
         position, go to N_BYTES before the end; otherwise go back to
         the initial position.  But do not lseek if lseek already failed.  */
      off_t pos = (initial_pos < ਸਿਰਾ_pos && n_ਬਾਈਟਾਂ < ਸਿਰਾ_pos - initial_pos
                   ? ਸਿਰਾ_pos - n_ਬਾਈਟਾਂ
                   : initial_pos);
      ਜੇ (pos != current_pos)
        current_pos = xlseek (fd, pos, SEEK_SET, prettyname);

      ਜੇ (ਸਿਰਾ_pos < 0)
        ਵਾਪਸ pipe_ਬਾਈਟਾਂ (prettyname, fd, n_ਬਾਈਟਾਂ, current_pos);
    }

  ਗਿਣਤੀ_t nr = dump_remainder (false, prettyname, fd, n_ਬਾਈਟਾਂ);
  ਵਾਪਸ current_pos < 0 ? -1 : current_pos + nr;
}

/* Output the last lines of the file PRETTYNAME open for reading
   in FD and with status ST.  Output the last N_LINES lines.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ਸਥਿਰ off_t
ਪੂਛ_ਸਤਰਾਂ (ਅੱਖਰ ਅਚਲ *prettyname, ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st,
            ਗਿਣਤੀ_t n_ਸਤਰਾਂ)
{
  ਜੇ (from_ਸ਼ੁਰੂ)
    {
      /* If skipping all input use lseek if possible, for speed.  */
      ਜੇ (OFF_T_MAX <= n_ਸਤਰਾਂ)
        {
          off_t e = lseek (fd, 0, SEEK_END);
          ਜੇ (0 <= e)
            ਵਾਪਸ e;
        }

      ਪੂਰਨ t = ਸ਼ੁਰੂ_ਸਤਰਾਂ (prettyname, fd, n_ਸਤਰਾਂ);
      ਜੇ (t < 0)
        ਵਾਪਸ t;
      dump_remainder (false, prettyname, fd, COPY_ਮੰਜ਼ਿਲ_EOF);
      ਵਾਪਸ -1;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Use file_lines only if FD is a regular file and SEEK_END
         reports more data.  */
      off_t ਸ਼ੁਰੂ_pos = (!presume_ਨਿਵੇਸ਼_pipe && S_ISREG (st->st_ਬਹੁਲਕ)
                         ? lseek (fd, 0, SEEK_CUR)
                         : -1);
      off_t ਸਿਰਾ_pos = ਸ਼ੁਰੂ_pos < 0 ? -1 : lseek (fd, 0, SEEK_END);
      ਵਾਪਸ (ਸਿਰਾ_pos < 0
              ? pipe_ਸਤਰਾਂ (prettyname, fd, n_ਸਤਰਾਂ)
              : ਸ਼ੁਰੂ_pos < ਸਿਰਾ_pos
              ? file_ਸਤਰਾਂ (prettyname, fd, st, n_ਸਤਰਾਂ, ਸ਼ੁਰੂ_pos, ਸਿਰਾ_pos)
              : ਸ਼ੁਰੂ_pos != ਸਿਰਾ_pos
              ? (/* Do not read from before the start offset,
                    even if the input file shrank.  */
                 xlseek (fd, ਸ਼ੁਰੂ_pos, SEEK_SET, prettyname))
              : ਸ਼ੁਰੂ_pos);
    }
}

/* Display the last part of file FILENAME,
   open for reading via FD and with status *ST.
   Return (-1 - errno) on failure, otherwise the resulting file offset
   if known, otherwise -1.  */

ਸਥਿਰ off_t
ਪੂਛ (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ fd, ਬਣਤਰ stat ਅਚਲ *st, ਗਿਣਤੀ_t n_units)
{
  ਵਾਪਸ (ਗਿਣਤੀ_ਸਤਰਾਂ ? ਪੂਛ_ਸਤਰਾਂ : ਪੂਛ_ਬਾਈਟਾਂ) (filename, fd, st, n_units);
}

/* Display the last N_UNITS units of the file described by F.
   Return true if successful.  */

ਸਥਿਰ bool
ਪੂਛ_file (ਬਣਤਰ File_spec *f, ਗਿਣਤੀ_t n_files, ਗਿਣਤੀ_t n_units)
{
  ਪੂਰਨ fd;
  bool ok;

  /* Avoid blocking if we may need to process asynchronously.  */
  bool nonblocking = forever && (nbpids || n_files > 1);

  bool is_stdin = (streq (f->name, "-"));

  ਜੇ (is_stdin)
    {
      have_read_stdin = true;
      fd = STDIN_FILENO;
      xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDIN_FILENO, O_ਦੁਆਧਾਰੀ);
    }
  ਨਹੀਂਤਾਂ
    fd = open (f->name, O_RDONLY | O_ਦੁਆਧਾਰੀ | (nonblocking ? O_NONBLOCK : 0));

  f->tailable = false;

  ਜੇ (fd < 0)
    {
      ਜੇ (forever)
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
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (print_ਸਿਰਲੇਖਾਂ)
        write_ਸਿਰਲੇਖ (f->prettyname);

      off_t read_pos;
      ਬਣਤਰ stat stats;
      ok = 0 <= fstat (fd, &stats);
      ਜੇ (!ok)
        {
          f->errnum = errno;
          error (0, f->errnum, _("cannot fstat %s"), quoteaf (f->prettyname));
        }
      ਨਹੀਂਤਾਂ
        {
          read_pos = ਪੂਛ (f->prettyname, fd, &stats, n_units);
          ok = -1 <= read_pos;
          f->errnum = ok ? 0 : -1 - read_pos;

          ਜੇ (IS_TAILABLE_FILE_TYPE (stats.st_ਬਹੁਲਕ))
            f->tailable = true;
          ਨਹੀਂਤਾਂ ਜੇ (forever)
            {
              ok = false;
              f->errnum = -1;
              error (0, 0, _("%s: cannot follow end of this type of file%s"),
                     quotef (f->prettyname),
                     (reopen_inaccessible_files ? ""
                      : _("; giving up on this name")));
            }
        }

      ਜੇ (forever)
        {
          ਜੇ (!ok)
            {
              f->ignore = ! reopen_inaccessible_files;
              close_fd (fd, f);
              f->fd = -1;
            }
          ਨਹੀਂਤਾਂ
            {
              ਰਿਕਾਰਡ_open_fd (f, fd, read_pos, &stats, (is_stdin ? -1 : 1));
              f->remote = fremote (fd, f);
            }
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (!is_stdin && close (fd) < 0)
            {
              error (0, errno, _("error reading %s"),
                     quoteaf (f->prettyname));
              ok = false;
            }
        }
    }

  ਵਾਪਸ ok;
}

/* If obsolete usage is allowed, and the command line arguments are of
   the obsolete form and the option string is well-formed, set
   *N_UNITS, the globals COUNT_LINES, FOREVER, and FROM_START, and
   return true.  If the command line arguments are obviously incorrect
   (e.g., because obsolete usage is not allowed and the arguments are
   incorrect for non-obsolete usage), report an error and exit.
   Otherwise, return false and don't modify any parameter or global
   variable.  */

ਸਥਿਰ bool
parse_obsolete_option (ਪੂਰਨ argc, ਅੱਖਰ * ਅਚਲ *argv, ਗਿਣਤੀ_t *n_units)
{
  bool t_from_ਸ਼ੁਰੂ;
  bool t_ਗਿਣਤੀ_ਸਤਰਾਂ = true;
  bool t_forever = false;

  /* With the obsolete form, there is one option string and at most
     one file argument.  Watch out for "-" and "--", though.  */
  ਜੇ (! (argc == 2
         || (argc == 3 && ! (argv[2][0] == '-' && argv[2][1]))
         || (3 <= argc && argc <= 4 && streq (argv[2], "--"))))
    ਵਾਪਸ false;

  ਪੂਰਨ posix_ver = posix2_ਸੰਸਕਰਣ ();
  bool obsolete_usage = posix_ver < 200112;
  bool traditional_usage = obsolete_usage || 200809 <= posix_ver;
  ਅੱਖਰ ਅਚਲ *p = argv[1];

  ਚੋਣ (*p++)
    {
    ਮੂਲ:
      ਵਾਪਸ false;

    ਹਾਲਤ '+':
      /* Leading "+" is a file name in the standard form.  */
      ਜੇ (!traditional_usage)
        ਵਾਪਸ false;

      t_from_ਸ਼ੁਰੂ = true;
      ਰੋਕੋ;

    ਹਾਲਤ '-':
      /* In the non-obsolete form, "-" is standard input and "-c"
         requires an option-argument.  The obsolete multidigit options
         are supported as a GNU extension even when conforming to
         POSIX 1003.1-2001 or later, so don't complain about them.  */
      ਜੇ (!obsolete_usage && !p[p[0] == 'c'])
        ਵਾਪਸ false;

      t_from_ਸ਼ੁਰੂ = false;
      ਰੋਕੋ;
    }

  ਗਿਣਤੀ_t n;
  ਜੇ (!c_isdigit (*p))
    n = DEFAULT_N_ਸਤਰਾਂ;
  ਨਹੀਂਤਾਂ
    ਲਈ (n = 0; c_isdigit (*p); p++)
      n = ckd_mul (&n, n, 10) || ckd_ਜੋੜੋ (&n, n, *p - '0') ? ਗਿਣਤੀ_MAX : n;

  ਚੋਣ (*p)
    {
    ਹਾਲਤ 'b': n = ckd_mul (&n, n, 512) ? ਗਿਣਤੀ_MAX : n; FALLTHROUGH;
    ਹਾਲਤ 'c': t_ਗਿਣਤੀ_ਸਤਰਾਂ = false; FALLTHROUGH;
    ਹਾਲਤ 'l': p++; ਰੋਕੋ;
    }

  ਜੇ (*p == 'f')
    {
      t_forever = true;
      ++p;
    }

  ਜੇ (*p)
    ਵਾਪਸ false;

  *n_units = n;

  /* Set globals.  */
  from_ਸ਼ੁਰੂ = t_from_ਸ਼ੁਰੂ;
  ਗਿਣਤੀ_ਸਤਰਾਂ = t_ਗਿਣਤੀ_ਸਤਰਾਂ;
  forever = t_forever;

  ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
parse_options (ਪੂਰਨ argc, ਅੱਖਰ **argv,
               ਗਿਣਤੀ_t *n_units, ਗਿਣਤੀ ਸਿਰਲੇਖ_ਬਹੁਲਕ *ਸਿਰਲੇਖ_ਬਹੁਲਕ,
               ਦੁੱਗਣਾ *sleep_interval)
{
  ਪੂਰਨ c;

  ਜਦੋਂ ((c = getopt_long (argc, argv, "c:n:fFqs:vz0123456789",
                           long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'F':
          forever = true;
          follow_ਬਹੁਲਕ = Follow_name;
          reopen_inaccessible_files = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
        ਹਾਲਤ 'n':
          ਗਿਣਤੀ_ਸਤਰਾਂ = (c == 'n');
          ਜੇ (*optarg == '+')
            from_ਸ਼ੁਰੂ = true;
          ਨਹੀਂਤਾਂ ਜੇ (*optarg == '-')
            ++optarg;

          *n_units = xnumtoimax (optarg, 10, 0, ਗਿਣਤੀ_MAX, "bkKmMGTPEZYRQ0",
                                 (ਗਿਣਤੀ_ਸਤਰਾਂ
                                  ? _("invalid number of lines")
                                  : _("invalid number of bytes"))
                                 , 0, XTOINT_MAX_QUIET);
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
        ਹਾਲਤ LONG_FOLLOW_OPTION:
          forever = true;
          ਜੇ (optarg == ਨਿਰਾ)
            follow_ਬਹੁਲਕ = DEFAULT_FOLLOW_ਬਹੁਲਕ;
          ਨਹੀਂਤਾਂ
            follow_ਬਹੁਲਕ = XARGMATCH ("--follow", optarg,
                                     follow_ਬਹੁਲਕ_string, follow_ਬਹੁਲਕ_map);
          ਰੋਕੋ;

        ਹਾਲਤ RETRY_OPTION:
          reopen_inaccessible_files = true;
          ਰੋਕੋ;

        ਹਾਲਤ MAX_UNCHANGED_STATS_OPTION:
          /* --max-unchanged-stats=N */
          max_n_unchanged_stats_between_opens =
            xnumtoimax (optarg, 10, 0, ਗਿਣਤੀ_MAX, "",
                        _("invalid maximum number of unchanged stats"
                          " between opens"),
                        0, XTOINT_MAX_QUIET);
          ਰੋਕੋ;

        ਹਾਲਤ DEBUG_ਪ੍ਰੋਗਰਾਮ_OPTION:
          debug = true;
          ਰੋਕੋ;

        ਹਾਲਤ DISABLE_INOTIFY_OPTION:
          disable_inotify = true;
          ਰੋਕੋ;

        ਹਾਲਤ PID_OPTION:
          ਜੇ (nbpids == pids_alloc)
            pids = xpalloc (pids, &pids_alloc, 1,
                            MIN (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, IDX_MAX), ਆਕਾਰ *pids);
          pids[nbpids++] = xdectoumax (optarg, 0, PID_T_MAX, "",
                                       _("invalid PID"), 0);
          ਰੋਕੋ;

        ਹਾਲਤ PRESUME_ਨਿਵੇਸ਼_PIPE_OPTION:
          presume_ਨਿਵੇਸ਼_pipe = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'q':
          *ਸਿਰਲੇਖ_ਬਹੁਲਕ = never;
          ਰੋਕੋ;

        ਹਾਲਤ 's':
          {
            ਅੱਖਰ *ep;
            errno = 0;
            ਦੁੱਗਣਾ s = cl_strtod (optarg, &ep);
            ਜੇ (optarg == ep || *ep || ! (0 <= s))
              error (EXIT_FAILURE, 0,
                     _("invalid number of seconds: %s"), quote (optarg));
            *sleep_interval = dtimespec_bound (s, errno);
          }
          ਰੋਕੋ;

        ਹਾਲਤ 'v':
          *ਸਿਰਲੇਖ_ਬਹੁਲਕ = always;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          ਸਤਰ_ਸਿਰਾ = '\0';
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3': ਹਾਲਤ '4':
        ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7': ਹਾਲਤ '8': ਹਾਲਤ '9':
          error (EXIT_FAILURE, 0, _("option used in invalid context -- %c"), c);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (reopen_inaccessible_files)
    {
      ਜੇ (!forever)
        {
          reopen_inaccessible_files = false;
          error (0, 0, _("warning: --retry ignored; --retry is useful"
                         " only when following"));
        }
      ਨਹੀਂਤਾਂ ਜੇ (follow_ਬਹੁਲਕ == Follow_descriptor)
        error (0, 0, _("warning: --retry only effective for the initial open"));
    }

  ਜੇ (pids && (!forever || (kill (pids[0], 0) < 0 && errno == ENOSYS)))
    {
      error (0, 0,
             _(forever
               ? "warning: --pid=PID is not supported on this system"
               : ("warning: PID ignored;"
                  " --pid=PID is useful only when following")));
      ਮੁਕਤ (pids);
      pids = ਨਿਰਾ;
    }
}

/* Mark as '.ignore'd each member of F that corresponds to a
   pipe or fifo, and return true if any are not marked.  */
ਸਥਿਰ bool
ignore_fifo_and_pipe (ਬਣਤਰ File_spec *f, ਪੂਰਨ n_files)
{
  /* When there is no FILE operand and stdin is a pipe or FIFO
     POSIX requires that tail ignore the -f option.
     Since we allow multiple FILE operands, we extend that to say: with -f,
     ignore any "-" operand that corresponds to a pipe or FIFO.  */
  bool some_viable = false;

  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      bool is_a_fifo_or_pipe =
        (streq (f[i].name, "-")
         && !f[i].ignore
         && 0 <= f[i].fd
         && (S_ISFIFO (f[i].ਬਹੁਲਕ_2)
             || (HAVE_FIFO_PIPES != 1 && isapipe (f[i].fd))));
      ਜੇ (is_a_fifo_or_pipe)
        {
          f[i].fd = -1;
          f[i].errnum = -1;
          f[i].ignore = true;
        }
      ਨਹੀਂਤਾਂ
        some_viable = true;
    }

  ਵਾਪਸ some_viable;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਗਿਣਤੀ ਸਿਰਲੇਖ_ਬਹੁਲਕ ਸਿਰਲੇਖ_ਬਹੁਲਕ = ਗੁਣਜ_files;
  bool ok = true;
  /* If from_start, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  */
  ਗਿਣਤੀ_t n_units = DEFAULT_N_ਸਤਰਾਂ;
  ਪੂਰਨ n_files;
  ਅੱਖਰ **file;
  ਬਣਤਰ File_spec *F;
  bool obsolete_option;

  /* The number of seconds to sleep between iterations.
     During one iteration, every file name or descriptor is checked to
     see if it has changed.  */
  ਦੁੱਗਣਾ sleep_interval = 1.0;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  {
    ਪੂਰਨ p = getpagesize ();
    ਜੇ (IDX_MAX < p)
      xalloc_die ();
    ਪੰਨਾ_ਮਾਪ = p;
  }

  obsolete_option = parse_obsolete_option (argc, argv, &n_units);
  argc -= obsolete_option;
  argv += obsolete_option;
  parse_options (argc, argv, &n_units, &ਸਿਰਲੇਖ_ਬਹੁਲਕ, &sleep_interval);

  /* To start printing with item N_UNITS from the start of the file, skip
     N_UNITS - 1 items.  'tail -n +0' is actually meaningless, but for Unix
     compatibility it's treated the same as 'tail -n +1'.  */
  n_units -= from_ਸ਼ੁਰੂ && 0 < n_units && n_units < ਗਿਣਤੀ_MAX;

  ਜੇ (optind < argc)
    {
      n_files = argc - optind;
      file = argv + optind;
    }
  ਨਹੀਂਤਾਂ
    {
      ਸਥਿਰ ਅੱਖਰ *dummy_stdin = (ਅੱਖਰ *) "-";
      n_files = 1;
      file = &dummy_stdin;
    }

  {
    bool ਮਿਲਿਆ_hyphen = false;

    ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
      ਜੇ (streq (file[i], "-"))
        ਮਿਲਿਆ_hyphen = true;

    /* When following by name, there must be a name.  */
    ਜੇ (ਮਿਲਿਆ_hyphen && follow_ਬਹੁਲਕ == Follow_name)
      error (EXIT_FAILURE, 0, _("cannot follow %s by name"), quoteaf ("-"));

    /* When following forever, and not using simple blocking, warn if
       any file is '-' as the stats() used to check for input are ineffective.
       This is only a warning, since tail's output (before a failing seek,
       and that from any non-stdin files) might still be useful.  */
    ਜੇ (forever && ਮਿਲਿਆ_hyphen)
      {
        ਬਣਤਰ stat ਵਿੱਚ_stat;
        bool blocking_stdin;
        blocking_stdin = (!nbpids && follow_ਬਹੁਲਕ == Follow_descriptor
                          && n_files == 1 && ! fstat (STDIN_FILENO, &ਵਿੱਚ_stat)
                          && ! S_ISREG (ਵਿੱਚ_stat.st_ਬਹੁਲਕ));

        ਜੇ (! blocking_stdin && isatty (STDIN_FILENO))
          error (0, 0, _("warning: following standard input"
                         " indefinitely is ineffective"));
      }
  }

  /* Don't read anything if we'll never output anything.  */
  ਜੇ (! forever && n_units == (from_ਸ਼ੁਰੂ ? ਗਿਣਤੀ_MAX : 0))
    ਵਾਪਸ EXIT_SUCCESS;

  ਜੇ (IDX_MAX < n_files)
    xalloc_die ();
  F = xinmalloc (n_files, ਆਕਾਰ *F);
  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    {
      F[i].name = file[i];
      F[i].prettyname = streq (file[i], "-") ? _("standard input") : file[i];
    }

  ਜੇ (ਸਿਰਲੇਖ_ਬਹੁਲਕ == always
      || (ਸਿਰਲੇਖ_ਬਹੁਲਕ == ਗੁਣਜ_files && n_files > 1))
    print_ਸਿਰਲੇਖਾਂ = true;

  xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);

  ਲਈ (ਪੂਰਨ i = 0; i < n_files; i++)
    ok &= ਪੂਛ_file (&F[i], n_files, n_units);

  ਜੇ (forever && ignore_fifo_and_pipe (F, n_files))
    {
      /* If stdout is a fifo or pipe, then monitor it
         so that we exit if the reader goes away.  */
      ਬਣਤਰ stat ਬਾਹਰਲਾ_stat;
      ਜੇ (fstat (STDOUT_FILENO, &ਬਾਹਰਲਾ_stat) < 0)
        error (EXIT_FAILURE, errno, _("standard output"));
      monitor_output = (S_ISFIFO (ਬਾਹਰਲਾ_stat.st_ਬਹੁਲਕ)
                        || (HAVE_FIFO_PIPES != 1 && isapipe (STDOUT_FILENO)));

#ਜੇ HAVE_INOTIFY
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
      ਜੇ (!disable_inotify && (tailable_stdin (F, n_files)
                               || any_remote_file (F, n_files)
                               || ! any_non_remote_file (F, n_files)
                               || any_symlinks (F, n_files)
                               || any_non_regular_fifo (F, n_files)
                               || (!ok && follow_ਬਹੁਲਕ == Follow_descriptor)))
        disable_inotify = true;

      ਜੇ (!disable_inotify)
        {
          ਪੂਰਨ wd = inotify_init ();
          ਜੇ (0 <= wd)
            {
              /* Flush any output from tail_file, now, since
                 tail_forever_inotify flushes only after writing,
                 not before reading.  */
              ਜੇ (fflush (stdout) < 0)
                write_error ();

              Hash_ਸਾਰਣੀ *ht;
              Hash_ਸਾਰਣੀ *ਮਾਪਾ_ht = ਨਿਰਾ;
              ਪੂਛ_forever_inotify (wd, F, n_files, sleep_interval,
                                    &ht, &ਮਾਪਾ_ht);
              hash_free (ht);
              ਜੇ (ਮਾਪਾ_ht)
                hash_free (ਮਾਪਾ_ht);
              close (wd);
              errno = 0;
            }
          error (0, errno, _("inotify cannot be used, reverting to polling"));
        }
#ਜੇ_ਅੰਤ
      disable_inotify = true;
      ਪੂਛ_forever (F, n_files, sleep_interval);
    }

  ਜੇ (have_read_stdin && close (STDIN_FILENO) < 0)
    error (EXIT_FAILURE, errno, "-");
  main_exit (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}
